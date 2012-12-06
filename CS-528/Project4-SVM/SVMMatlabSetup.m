% Setups up the enviroment for LIBSVM with matlab

%% Adding the LIBSVM to the path
libsvmpath = 'libsvm-3.14';
addpath(genpath(libsvmpath));
fprintf(1,'Added %sto the path\n',libsvmpath);

% Making sure libsvm is compiled
if (exist('svmtrain','file')==3 && ...
        exist('libsvmread','file') &&  ...
        exist('svmpredict','file') == 3)
    fprintf(1,'libsvm is compiled\n');
else
    fprintf(1,'libsvm is not compiled, attempting to compile\n');
    if ~isunix()
        fprintf('Attempting to setup VCVARS.\n');
        tmp = pwd;
        VCVARS = 'c:\Program Files\Microsoft Visual Studio 10.0\VC\bin';
        cd(VCVARS);
        system('vcvars32.bat');
        cd(tmp)
    end
    run(fullfile(libsvmpath,'matlab','make.m'));
end
fprintf('Libsvm should be setup on the system\n\n');

%% Adding the data files
dataPath='data';
dataSets={'glass_scale_train.txt','liver-disorders_scale_train.txt','vowel_scale_train.txt'};
setNames={'Glass','Liver Disorders','Vowel'};
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