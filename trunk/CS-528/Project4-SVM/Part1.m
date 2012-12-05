%% PART ONE
% Matthew J. Urffer (matthew.urffer@gmail.com)
% CS-528 Project4-SVM
% Using LibSVM to find ideal C and gamma values

% Adding Datasets and LibSVM to path
addpath(genpath('libsvm-3.14'));
dataPath='data';
dataSets={'glass_scale_train.txt','liver-disorders_scale_train.txt','vowel_scale_train.txt'};
setNames={'Glass','Liver Disorders','Vowel'};

for data = dataSets
    fprintf(1,'Processing data set: %s\n',cell2mat(dataSets(1)));
    % Reading in the data
    [label,inst]=libsvmread(cell2mat(fullfile(dataPath,dataSets(1))));
    
    % Preforming a coarse mesh (same for all)
    c = -4:0.1:4;
    g = -4:0.1:4;
    [~,c,g] = gridParam(c,g,label,inst);
    
    % Want to grab the lowest, and further refine
    c = 0.5*c:0.01:1.5*c;
    g = 0.5*g:0.01:1.5*g;
    [cv,c,g] = gridParam(c,g,label,inst);
end