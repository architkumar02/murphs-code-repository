
%% Terminal and Function Set
% Default Function Set
fSet = {'sin','cos','exp','power','plus','minus','mldivide','times'};
fProbability = ones(numel(fSet),1)/numel(fSet);
fSetMap = containers.Map(fSet,fProbability);

% Default Terminal Set
tSet = {'x','1','pi'};
tProbability = ones(numel(tSet),1)/numel(tSet);
tSetMap = containers.Map(tSet,tProbability);

%% Building the Initial Population
%matlabpool local
populationSize = 100;
treeDepth = 5;
forest = cell(populationSize,1);
tic;

for tree=1:populationSize
    forest{tree} = ExpTree(treeDepth,fSetMap,tSetMap);
end
fprintf(1,'%d Full Tree created in %f s\n',populationSize,toc);

% Prune the first half of the trees
minDepth = 3;
tic;
for tree=1:floor(populationSize/2)
    depth = randi([minDepth treeDepth],1);
    forest{tree} = forest{tree}.prune(depth);
end
fprintf(1,'Pruned %d trees in %f s\n',floor(populationSize/2),toc);

% Using the Standard Devaition to get an idea of the diversity.
values = zeros(populationSize,1);
tic;
for tree=1:populationSize
    values(tree) = forest{tree}.eval(1);
end
fprintf(1,'%d Tree evaluations in %f s\n',populationSize,toc);
hist(values,25);
% Can test for tree equality by just evaluating.  This isn't a strict
% equality of tree equality
%matlabpool close

%% Test Swap
t1 = ExpTree(); 
t1.WriteExprTree('test.dot'); 
t2 = ExpTree();
t2.WriteExprTree('test2.dot');
[t1,t2] = t1.swap(t2,2);
t1.WriteExprTree('Swap1.dot');
t2.WriteExprTree('Swap2.dot');