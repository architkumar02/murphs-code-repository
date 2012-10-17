
function [avgFittness,generation] = SymbolicRegression(GPInit,GPOptions,dataSet)

forest = initPopulation(GPInit,GPOptions);
testValues = [-pi,0,pi];
for v = testValues
    [values,sameTrees] = computeDiversity(forest,v);
    fprintf(1,'\tValue: %f mean: %f std: %f numSameTrees %f\n',...
        v,nanmean(values),nanstd(values),numel(sameTrees));
end
forest = crossOver(forest,GPOptions);
end

function forest = initPopulation(GPInit,GPOptions)
% forest = initPopulation(GPInit,GPOptions)
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
initMethod = GPOptions.initPopMethod;
if initMethod.name ~= full
    pruneItter = 1;
    numPruned = floor(initMethod.popFraction*GPInit.population);
    while pruneItter < intitMethod.numPrunes
        
        parfor tree=1:numPruned
            depth = randi([initMethod.pruneDepth,GPInit.treeDepth]);
            forest{tree} = forest{tree}.prune(depth);
        end
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
parfor tree = order
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

function [fit,max,min,avg] = fitness(forest,dataSet)
% [fit,max,min,avg] = fitness(forest,dataSet)
%   Calculates the fitness of a data set. Fitness is computed as the sum
%   squared error for all of the X values in the data set.
%   Inputs:
%       forest - population of expression tress on which to compute the
%           fitness
%       dataSet - [X;Y] data set for training

X = dataSet(:,1);
Y = dataSet(:,2);
values = zeros(numel(forest),1);
for tree = 1:numel(forest)
    
end


end

