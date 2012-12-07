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

