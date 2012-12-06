%%  NETWORK SETUPS
% Driver for measuring the performance of various ANN for the character
% recongination project


charKey = {'A','C','D','E','F','G','H','L','P','R'};

if exist('DataFiles.mat','file')
    load('DataFiles.mat');
else
    path = pwd;
    % path = '~parker/courses/cs425-528/Project1/';
    learnPath = 'learn-grid';
    testPath = 'test-grid';
    validatePath = 'validate-grid';
    if ~exist('P','var')
        fprintf(1,'Reading in learning data set\n');
        [P,T] = importDataSet(path,learnPath);
    end
    if ~exist('valP','var')
        fprintf(1,'Reading in validation data set\n');
        [valP,valT] = importDataSet(path,validatePath);
    end
    if ~exist('testP','var')
        fprintf(1,'Reading in testing data set\n');
        [testP,testT] = importDataSet(path,testPath);
    end
end

%% Let's parallelize these test
matlab pool open local

%% NUMBER OF HIDDEN NODES
evalHiddenNodes = input('Evaluate the network performace as function of hidden nodes [Y/N]? ','s');
if ~isempty(evalHiddenNodes) && strcmpi(evalHiddenNodes,'y')
    
    hiddenNodes = 2:2:40;
    perf = cell(numel(hiddenNodes),1);
    confMatrix = cell(numel(hiddenNodes),1);
    accuracy = cell(numel(hiddenNodes),1);
    trainTime = cell(numel(hiddenNodes),1);
    mseGoal = 1E-3;
    parfor i=1:numel(hiddenNodes)
        tic;
        net = ANN(P,T,hiddenNodes(i));
        [net,perf{i}] = net.trainANN(P,T,{valP,valT},mseGoal);
        trainTime{i} = toc;
        [confMatrix{i},order,accuracy{i}] = net.ConfusionMatrix(testP,testT,charKey);
    end
    
    save('HiddenNodeResults.mat','hiddenNodes','perf',...
        'confMatrix','accuracy','trainTime','-mat');
end

%% INCRREASE MAX ITTERATIONS!

%% HIDDEN NODE TYPE
% Options are tansig, logsig,purelin
evalHiddenNodeType = input('Evaluate the network performance with different node types [Y/N]? ','s');
if ~isempty(evalHiddenNodeType) && strcmpi(evalHiddenNodeType,'y')
    
    hiddenNodes = 20;
    hiddenNodeFunctions = {'tansig','purelin','logsig'};
    perf = cell(numel(hiddenNodeFunctions),1);
    confMatrix = cell(numel(hiddenNodeFunctions),1);
    accuracy = cell(numel(hiddenNodeFunctions),1);
    trainTime = cell(numel(hiddenNodeFunctions),1);
    mseGoal = 8E-3;
    parfor i=1:numel(hiddenNodeFunctions)
        nodeName = hiddenNodeFunctions{i};
        
        net = ANN(P,T,hiddenNodes);
        net.performance = false;
        net.hiddenFcn = hiddenNodeFunctions{i};
        
        tic;
        [net,perf{i}] = net.trainANN(P,T,{valP,valT},mseGoal);
        trainTime{i} = toc;
        [confMatrix{i},order,accuracy{i}] = net.ConfusionMatrix(testP,testT,charKey);
    end
    
    save('HiddenNodeFunctionsResults.mat','hiddenNodes','perf',...
        'confMatrix','accuracy','trainTime','-mat');
end

%% LEARNING RATE
evalLearningRate = input('Evaluate the network performance with differnet inital learning rates [Y/N]? ',s');
if ~isempty(evalLearningRate) && strcmpi(evalLearningRate,'y')
    
    hiddenNames = 20;
    learningRates = linspace(0.001,1,100);
    perf = cell(numel(learningRates),1);
    confMatrix = cell(numel(learningRates),1);
    accuracy = cell(numel(learningRates),1);
    trainTime = cell(numel(learningRates),1);
    mseGoal = 8E-3;
    parfor i=1:numel(learningRates)
        tic;
        net = ANN(P,T,hiddenNames);
        net.learnRate = learningRates(i);
        [net,perf{i}] = net.trainANN(P,T,{valP,valT},mseGoal);
        trainTime{i} = toc;
        [confMatrix{i},order,accuracy{i}] = net.ConfusionMatrix(testP,testT,charKey);
    end
    
    save('LearningRateResults.mat','hiddenNodes','perf',...
        'confMatrix','accuracy','trainTime','-mat');
end

%% INPUT NODE WORK


%% Close parallel session
matlabpool close