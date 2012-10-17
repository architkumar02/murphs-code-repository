function [avgFittness,generation] = SymbolicRegression(input,GPOptions)
    forest = createForest(input);
    forest = initPopulation(forest,input,GPOptions);
    testValues = [-pi,0,pi];
    for v = testValues
        [values,sameTrees] = computeDiversity(forest,v);
        fprintf(1,'\tValue: %f mean: %f std: %f numSameTrees %f\n',...
            v,nanmean(values),nanstd(values),numel(sameTrees));
    end
    forest = crossOver(forest,GPOptions);
end

function forest = createForest(input)
% Creates a forest of trees according to input arguments
   fSetMap = input.functionSet;
   tSetMap = input.terminalSet;
   populationSize = input.population;
   treeDepth = input.treeDepth;
   
   
   forest = cell(populationSize,1);
 tic;
parfor tree=1:populationSize
    forest{tree} = ExpTree(treeDepth,fSetMap,tSetMap);
end
fprintf(1,'%d Full Trees of %d depth created in %f s\n',populationSize,treeDepth,toc);

end

function forest = initPopulation(forest,input,GPOptions)
% Prune the first half of the trees
minDepth = GPOptions.minPruneDepth;
numPruned = floor(GPOptions.pruneFraction*numel(forest));
tic;
parfor tree=1:numPruned
    depth = randi([minDepth input.treeDepth],1);
    forest{tree} = forest{tree}.prune(depth);
end
fprintf(1,'Pruned %d trees in %f s\n',numPruned,toc);
end

function forest = crossOver(forest,GPOptions)
popSize = numel(forest);
order = randperm(popSize);
swapDepth = GPOptions.crossOverDepth;
tic
for i=1:(popSize/2)
   t1 = forest{order(i)};
   t2 = forest{order(popSize-i)};
   [t1,t2] = t1.swap(t2,swapDepth);
   forest{order(i)} = t1;
   forest{order(popSize-i)} = t2;
end

fprintf(1,'Preformed %d crossover in %f s\n',popSize,toc);
end

function [values,numEqual] = computeDiversity(forest,testValue)
% [values,numEqual] = computeDiversity(forest,testValue)
% 
popSize = numel(forest);
values = zeros(popSize,1);
tic;
parfor tree=1:popSize
    values(tree) = forest{tree}.eval(testValue);
end
fprintf(1,'%d Tree evaluations in %f s\n',popSize,toc);
numEqual = [];
tic;
for t1=1:popSize
    for t2=(t1+1):popSize
        if forest{t2}.equals(forest{t1})
           numEqual(end) = t1; 
        end
    end
end
fprintf(1,'%d test for equality in %f s\n',popSize,toc);
end

