%% ADA TEST
% Driver Program for Testing

%% Setting up the enviroment
SVMMatlabSetup


%% User input
options = {'Liver','Glass','Vowel'};
[selection,ok] = listdlg('ListString',options,'SelectionMode','single','PromptString','Choose a trained SVM ensamble');
switch options{selection}
    case 'Liver'
        fprintf(1,'Using the trained Liver ensamble SVM\n');
    case 'Glass'
        fprintf(1,'Using the trained Glass ensamble SVM\n');
    case 'Vowel'
        fprintf(1,'Using the trained Vowel ensamble SVM\n');
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