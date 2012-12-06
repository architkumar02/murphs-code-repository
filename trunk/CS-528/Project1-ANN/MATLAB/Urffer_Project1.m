%% Urffer_Project1.m
% Artifical Neural Network implementation of character recongization.
%
% Author: Matthew J. Urffer (matthew.urffer@gmail.com)
%
% Collobarated with Matthew Lish
%

%% Setting Data Paths

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
charKey = {'A','C','D','E','F','G','H','L','P','R'};
%% Training a nework
train = input('Train Network [Y/N]','s');
if ~isempty(train) && train == 'Y'
    if ~exist('P','var')
        fprintf(1,'Reading in learning data set\n');
        [P,T] = importDataSet(path,learnPath);
    end
    if ~exist('valP','var')
        fprintf(1,'Reading in validation data set\n');
        [valP,valT] = importDataSet(path,validatePath);
    end
    
    
    %% Setting up a Network
    net = ANN(P,T,20);
    net
    %% Setting and training a Network
    classNames = {'A','C','D','E','F','G','H','L','P','R'};
    mseGoal = 1E-3;
    [net,perf] = net.trainANN(P,T,{valP,valT},mseGoal);
    net.plotPerformance(perf,classNames);
end
% Call some training function (back prop)
% Maybe k-fold cross validation

%% Validate the network
validate = input('Validate the network? [Y/N]','s');
if ~isempty(validate) && validate == 'Y'
    load('BestNet.mat');
    
    if ~exist('testP','var')
        fprintf(1,'Reading in testing data set\n');
        [testP,testT] = importDataSet(path,testPath);
    end
    
    [m,order,accuracy] = bestNet.ConfusionMatrix(testP,testT,classNames)
    
end
