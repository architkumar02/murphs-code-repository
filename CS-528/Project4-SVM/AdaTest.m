%% ADA TEST
% Driver Program for Testing

%% Setting up the enviroment
SVMMatlabSetup

%% User input
% Gets the user input and loads the data files
options = {'Liver','Glass','Vowel'};
[selection,ok] = listdlg('ListString',options,'SelectionMode','single','PromptString','Choose a trained SVM ensamble');
switch options{selection}
    case 'Liver'
        fprintf(1,'Using the trained Liver ensamble SVM\n');
        load('LiverEnsambleSVM.mat');
    case 'Glass'
        fprintf(1,'Using the trained Glass ensamble SVM\n');
        load('GlassEnsambleSVM.mat');
    case 'Vowel'
        fprintf(1,'Using the trained Vowel ensamble SVM\n');
        load('VowelEnsambleSVM.mat');
    otherwise
        error('ADATest: %s %s\n','unreconized option: ',options{selection});
end
     
[filename,pathname] = uigetfile('*.*','Select the testing set','data');
if isequal(filename,0)
    error('ADATest: %s\n','User did not select a test set');
else
    file = fullfile(pathname,filename);
    fprintf(1,'Training on %s\n',file);
    [label,inst]=libsvmread(file);
end

%% Running the data throught the classifer
[predict_label,accuracy] = SVMAdaPredict(label,inst, ensambleSVM,alpha);
fprintf(1,'The accuracy of the ensamble is %5.2f \n',accuracy);
% Calculating the confusion matrix
[C,order] = confusionmat(label,predict_label);
fprintf(1,'The confusion matrix is below;\n');
C
fprintf(1,'The order of the classes is:\n');
order'