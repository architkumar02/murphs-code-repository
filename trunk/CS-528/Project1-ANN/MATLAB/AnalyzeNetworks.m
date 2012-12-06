%% ANALYZE NETWORKS
% Read in  and analyze the structures created in NetworkSetups.m

%% Looking at the number of hidden nodes
load('HiddenNodeResults.mat');
leg = cell(numel(hiddenNodes)-1,1);
figure(1);
hold all;
for i=1:(numel(hiddenNodes)-1)
    
    val = cell2mat(perf{i}.sseValSetPerEpoch);
    val = val(:,end);
    if sum(val)
        leg{i} = sprintf('%d',hiddenNodes(i));
        x = 1:numel(val);
        plot(x,val);
    end
end
hold off;
xlabel('Epoch');
ylabel('Sum Squared Error');
legend(leg);
set(gca,'yscale','log');
ylim([100,2500]);
print(gcf,'HiddenNodes_SSE','-depsc');
%% Looking at the type of hidden node
load('HiddenNodeFunctionsResults.mat');
hiddenNodeFunctions = {'tansig','purelin','logsig'};
figure(2);
hold all;
for i=1:numel(perf)
    
    val = cell2mat(perf{i}.sseValSetPerEpoch);
    val = val(:,end);
    if sum(val)
        x = 1:numel(val);
        plot(x,val);
    end
end
hold off;
xlabel('Epoch');
ylabel('Sum Squared Error');
legend(hiddenNodeFunctions);
set(gca,'yscale','log');
print(gcf,'NodeType_SSE','-depsc');
%% Looking at the learning rate
clear leg
leg = {};
load('LearningRateResults.mat');
figure(3);
epochs = [50,100,150,200,250];
learningRates = learningRates(learningRates<1);
data = zeros(numel(learningRates),numel(epochs));
for i=1:numel(learningRates)
    % Getting the final sse for selected training epochs
    val = perf{i}.sseValSetPerEpoch(epochs);
    val = cell2mat(val);
    val = val(:,end);
    for j=1:numel(val)
        data(i,j) = val(j);
    end
end
leg = cell(numel(epochs),1);
fprintf(1,'Staring to plot\n');
hold all;
for i=1:numel(epochs)
    plot(learningRates,data(:,i));
    leg{i}  = int2str(epochs(i));
end
hold off;
xlabel('Learning Rate');
ylabel('Sum Squared Error');
legend(leg);
print(gcf,'LearningRate_SSE','-depsc');

%% Lookign at Intial Weight Results
figure(4);
clear leg
load('InitialWeightResults.mat');
leg = cell(numel(initialWeights),1);
hold all;
for i=1:numel(initialWeights)
    
    val = cell2mat(perf{i}.sseValSetPerEpoch);
    val = val(:,end);
    if sum(val)
        leg{i} = sprintf('[-%3.2f , %3.2f]',initialWeights(i),initialWeights(i));
        x = 1:numel(val);
        plot(x,val);
    end
end
hold off;
xlabel('Epoch');
ylabel('Sum Squared Error');
legend(leg);
set(gca,'yscale','log');
ylim([100,2500]);
print(gcf,'IntialWeights_SSE','-depsc');


%% Lookign at repeated networks
figure(5);
clear leg
load('RepeatNetworkResults.mat');
leg = cell(numel(networks),1);
hold all;
for i=1:numel(networks)
    
    val = cell2mat(perf{i}.sseValSetPerEpoch);
    val = val(:,end);
    if sum(val)
        leg{i} = sprintf('Trial %d ',networks(i));
        x = 1:numel(val);
        plot(x,val);
    end
end
hold off;
xlabel('Epoch');
ylabel('Sum Squared Error');
legend(leg);
set(gca,'yscale','log');
ylim([100,2500]);
print(gcf,'RepeatNetwors_SSE','-depsc');
%% Looking at MATLAB's and my BestNet
load('DataFiles.mat');
load('BestNet.mat');
load('MatlabNet.mat');
charKey = {'A','C','D','E','F','G','H','L','P','R'};

% Confusion Matrix of Matlab's
[~,numSamples] = size(testT);

mpClass = zeros(numSamples,1);
actualClass = zeros(numSamples,1);

for i=1:numSamples
    [~,I] = max(network1(testP(:,i)));
    mpClass(i) = classNames{I};
    [~,I] = max(testT(:,i));
    actualClass(i) = classNames{I};
end
[mConf,order] = confusionmat(actualClass,mpClass);
mAccuracy = trace(mConf)/sum(sum(mConf));

fprintf(1,'Matlabs Accuracy is %f with confusion matrix below\n',mAccuracy);
mConf

% Getting my performance
fprintf(1,'My Network Performance\n');
[m,order,accuracy] = bestNet.ConfusionMatrix(testP,testT,charKey)


