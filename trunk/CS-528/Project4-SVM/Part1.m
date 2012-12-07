%% PART ONE
% Matthew J. Urffer (matthew.urffer@gmail.com)
% CS-528 Project4-SVM
% Using LibSVM to find ideal C and gamma values
SVMMatlabSetup
ProjectSetup
f = fopen('CoarseGridSearchOutput.dat','w');

%% Corse Grid Search
n  = 20;
fprintf(f,'Data Set & $C_{min}$ & $C_{max}$ & $\\sigma_{min}$ & $\\sigma_{max}$ & $C$ & $\\sigma$ & $\\epsilon$ \\\\ \n');
fprintf(f,'\\hline\n');
c_coarse = zeros(size(dataSets));
g_coarse = zeros(size(dataSets));
i = 1;
for data = dataSets
    fprintf(1,'Processing data set: %s\n',cell2mat(dataSets(i)));
    % Reading in the data
    [label,inst]=libsvmread(fullfile(dataPath,dataSets{i}));
    
    % Preforming a coarse mesh (same for all)
    c = linspace(1,10,n);
    g = linspace(-7,7,n);
    [cv,c_coarse(i),g_coarse(i)] = gridParam(c,g,label,inst);
  
    
    % Writing the values
    fprintf(f,'%s & %3.2f & %3.2f & %3.2f & %3.2f & %3.2f & %3.2f & %3.2f \\\\ \n',...
        setNames{i},min(c),max(c),min(g),max(g),c_coarse(i),g_coarse(i),cv);
    
    % Save the figure
    title(sprintf('Coarse Parameter Search: %s',setNames{i}));
    filename = fullfile(imdir,sprintf('%s_coarseSearch',setNames{i}));
    print(gcf,'-dpng',filename);
    
    i = i + 1;
end
fclose(f);

%% Fine Grid Search
f = fopen('FineGridSearchOutput.dat','w');
c_fine = zeros(size(dataSets));
g_fine = zeros(size(dataSets));
i = 1;
n  = 20;
fprintf(f,'Data Set & $C_{min}$ & $C_{max}$ & $\\sigma_{min}$ & $\\sigma_{max}$ & $C$ & $\\sigma$ & $\\epsilon$ \\\\ \n');
fprintf(f,'\\hline\n');
for data = dataSets
    fprintf(1,'Processing data set: %s\n',cell2mat(dataSets(i)));
    % Reading in the data
    [label,inst]=libsvmread(fullfile(dataPath,dataSets{i}));
    
    % Want to grab the lowest, and further refine. This is based on the
    % lowest c, g value from the coarse search
    c = linspace(0.5*c_coarse(i),1.5*c_coarse(i),n);
    g = linspace(0.5*g_coarse(i),1.5*g_coarse(i),n);
    [cv,c_fine(i),g_fine(i)] = gridParam(c,g,label,inst);
    
    % Writing the values

    fprintf(f,'%s & %3.2f & %3.2f & %3.2f & %3.2f & %3.2f & %3.2f & %3.2f \\\\ \n',...
        setNames{i},min(c),max(c),min(g),max(g),c_fine(i),g_fine(i),cv);
    
    % Save the figure
    title(sprintf('Fine Parameter Search: %s',setNames{i}));
    filename = fullfile(imdir,sprintf('%s_fineSearch',setNames{i}));
    print(gcf,'-dpng',filename);
    i = i + 1;
end
fclose(f);
%% Probably need to do validation
