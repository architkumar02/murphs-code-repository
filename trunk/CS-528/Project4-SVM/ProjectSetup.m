%% Adding the data files
dataPath='data';
dataSets={'glass_scale_train.txt','liver-disorders_scale_train.txt','vowel_scale_train.txt'};
testSets={'glass_scale_test.txt','liver-disorders_scale_test.txt','vowel_scale_test.txt'};
setNames={'Glass','Liver','Vowel'};
fprintf('The data sets are located in %s\n',dataPath);
fprintf('The sets are named as:\n');
for i=1:numel(dataSets)
    fprintf('%s -> %s\n',setNames{i},dataSets{i});
end
fprintf('\n');

%% Image setup
imdir = fullfile('writeup','images');
imtype = '-dpng';
fprintf('Saving images to %s as %s\n\n',imdir,imtype);