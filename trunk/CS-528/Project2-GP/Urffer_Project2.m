%% Urffer_Project2
% Driver function for Symbolic Regression


%% Terminal and Function Set
% Default Function Set
fSet = {'sin','cos','exp','power','plus','minus','mldivide','times'};
fProbability = ones(numel(fSet),1)/numel(fSet);
fSetMap = containers.Map(fSet,fProbability);

% Default Terminal Set
tSet = {'x','1','pi'};
tProbability = ones(numel(tSet),1)/numel(tSet);
tSetMap = containers.Map(tSet,tProbability);

%% Running Symbolic Regression
matlabpool open
for population = [100:50:500]
    for treeDepth = [4:2:12]
        fprintf(1,'\n ##### Population: %d TreeDepth: %d\n',population,treeDepth);
        input = struct('functionSet',fSetMap,'terminalSet',tSetMap,...
            'population',population,'treeDepth',treeDepth);
        GPOptions = struct('pruneFraction',0.5,'minPruneDepth',3,...
            'crossOverDepth',3);
        SymbolicRegression(input,GPOptions)
    end
end
matlabpool close