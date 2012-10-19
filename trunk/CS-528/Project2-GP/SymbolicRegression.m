function [avgFittness,generation] = SymbolicRegression(GPInit,GPOptions,data)

% Intilizing the population
fprintf(1,'Intitializing the population\n');
forest = initPopulation(GPInit);

maxItter = 100;
maxFitness = 0;
maxFit = zeros(maxItter,1);
minFit = zeros(maxItter,1);
avgFit = zeros(maxItter,1);
itter = 1;
bestFitness = Inf;
fprintf(1,'\n\t\t\t Starting Evoluation\n\n');
fprintf(1,'Itteration\tMean Fitness\n');
while itter < maxItter && bestFitness > maxFitness
    % Evaluate the current population
    [fit,maxFit(itter),minFit(itter),avgFit(itter)] = populationFitness(forest,data);
    bestFitness = minFit(itter);
    
    % Select best individuals
    forest = fitnessSelection(forest,fit,GPOptions);
    
    % Perform genetic operators
    forest = crossOver(forest,GPOptions);     
%     forest = mutate(forest,GPOptions);
    
    if numel(forest) < GPOptions.minPopSize
        forest = rabits(forest,GPOptions);
    end
    %if mod(itter,1) == 1
        fprintf(1,'\t%d\t\t%5.3e\n',itter,avgFit(itter));
    %end
end

% Plot Fitness trend
itter = 1:maxItter;
hold all;
plot(itter,maxFit,'-g',itter,avgFit,'-k',itter,minFit,'-r');
legend('Lowest','Average', 'Highest');
xlabel('Generation');
ylabel('Fitness (SSE)');

end

function forest = rabits(forest,GPOptions)
% forest = rabits(forest,GPOptions)
%   Adds trees by crossover to replensish the population
newPopSize = GPOptions.minPopSize*2;
newForest = cell(newPopSize,1);

for tree = 1:numel(forest)
    newForest{tree} = forest{tree};
end
for tree = numel(forest):newPopSize
    parents = randperm(numel(forest),2);
    newForest{tree} = forest{parents(1)}.swap(forest{parents(2)},2);
    newForest{tree} = newForest{tree}.mutate(1);
end
forest = newForest;
end

function forest = fitnessSelection(forest,fit,GPOptions)
% forest = fitnessSelection(forest,fit,GPOptions)

numSpartans = floor(numel(forest)*GPOptions.reproduceFraction);
[v,I] = sort(fit);
I = I(1:numSpartans);
forest = forest(I);
end

function forest = initPopulation(GPInit)
% forest = initPopulation(GPInit)
%   inputs:
%       GPInit - input stucture of tree building arguments
%           functionSet -> Collection Map between function and probability
%           terminalSet -> Collection Map between terminal and probability
%           population -> Size of the intial population
%           treeDepth -> Maximum depth of the tree
%           initMethod -> options of the tree intialization
%               name -> 'full','rampedHalfHalf','grow'
%               popFraction -> fraction of population to be applied to
%               pruneDepth -> min depth at which the tree can be pruned
%               numPrunes -> number of pruning operations per tree
%   Outputs:
%       A cell array of trees (forest)
%
% All of the trees are intially built as full trees, and then pruned to
% simulate the grow method. The pruning is completed by chosing a random
% depth and node, and then eliminating all of the sub children of that
% node.

fSetMap = GPInit.functionSet;
tSetMap = GPInit.terminalSet;
populationSize = GPInit.population;
treeDepth = GPInit.treeDepth;

% Building all of the full trees
forest = cell(populationSize,1);

parfor tree=1:populationSize
    
    forest{tree} = ExpTree(treeDepth,fSetMap,tSetMap);
end

% Applying other variations to the methods
initMethod = GPInit.initPopMethod;
if ~strcmp(initMethod.name,'full')
    pruneItter = 1;
    numPruned = floor(initMethod.popFraction*GPInit.population);
    parfor tree=1:numPruned
        depth = randi([initMethod.pruneDepth,GPInit.treeDepth-1]);
        forest{tree} = forest{tree}.prune(depth);
    end
    
end
end

function forest = crossOver(forest,GPOptions)
% forest = crossOver(forest,GPOptions)
%   Performs cross over by swapping branches of trees.
%   Inputs (GPOptions struct):
%       crossOverFraction - fraction of population on which to preform
%           crossover
%       crossOverDepth - Depth of tree in order to start randomly choosing
%           a branch
popSize = floor(numel(forest)*GPOptions.crossOverFraction);
order = randperm(numel(forest),popSize);
swapDepth = GPOptions.crossOverDepth;
for i=1:(popSize/2)
    t1 = forest{order(i)};
    t2 = forest{order(popSize-i)};
    [t1,t2] = t1.swap(t2,swapDepth);
    forest{order(i)} = t1;
    forest{order(popSize-i)} = t2;
end
end

function forest = mutate(forest,GPOptions)
% forest = muate(forest,GPOptions)
%   Preforms muation by swapping leaves of a tree
%   Inputs (GPOptions struct):
%       mutationFraction - fraction of population on which to preform
%           mutations
%       mutationRate - propability that a leaf will be mutated
popSize = floor(numel(forest)*GPOptions.mutationFraction);
order = randperm(numel(forest),popSize);
mutRate = GPOptions.mutationRate;
for tree = order
    forest{tree} = forest{tree}.mutate(mutRate);
end
end

function [values,numEqual] = computeDiversity(forest,testValue)
% [values,numEqual] = computeDiversity(forest,testValue)
%
popSize = numel(forest);
values = zeros(popSize,1);

parfor tree=1:popSize
    values(tree) = forest{tree}.eval(testValue);
end

numEqual = [];

for t1=1:popSize
    for t2=(t1+1):popSize
        if forest{t2}.equals(forest{t1})
            numEqual(end+1) = t1;
        end
    end
end

end

function [fit,maxFit,minFit,avgFit] = populationFitness(forest,dataSet)
% [fit,maxFit,minFit,avgFit] = fitness(forest,dataSet)
%   Calculates the fitness of a data set. Fitness is computed as the sum
%   squared error for all of the X values in the data set.
%   Inputs:
%       forest - population of expression tress on which to compute the
%           fitness
%       dataSet - [X;Y] data set for training

X = dataSet(:,1);
Y = dataSet(:,2);
fit = zeros(numel(forest),1);
p = zeros(size(X));

for tree = 1:numel(forest)
    % Evaluating
    for i = 1:numel(X)
        p(i) = forest{tree}.eval(X(i));
    end
    % Calculating the SSE
    fit(tree) = sse(p-Y);
end

maxFit = max(fit);
minFit = min(fit);
avgFit = nanmean(fit);

end

