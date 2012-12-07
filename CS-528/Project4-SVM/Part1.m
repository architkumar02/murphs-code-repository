%% PART ONE
% Matthew J. Urffer (matthew.urffer@gmail.com)
% CS-528 Project4-SVM
% Using LibSVM to find ideal C and gamma values
SVMMatlabSetup
ProjectSetup
f = fopen('GridSearchOutput.txt','w');

%% Corse Grid Search
n  = 10;
fprintf(f,'Coarse Grid Search with %d values\n',n^2);
fprintf(f,'Coarse Grid Search\n');
fprintf(f,'\t\t\tDataSet\t\t\t\tminC\tmaxC\tminG\tmaxGAccuracy\tg\t\tc\n');
c_coarse = zeros(size(dataSets));
g_coarse = zeros(size(dataSets));
i = 1;
for data = dataSets
    fprintf(1,'Processing data set: %s\n',cell2mat(dataSets(i)));
    % Reading in the data
    [label,inst]=libsvmread(fullfile(dataPath,dataSets{i}));
    
    % Preforming a coarse mesh (same for all)
    c = linspace(-5,5,n);
    g = linspace(-5,5,n);
    [cv,c_coarse(i),g_coarse(i)] = gridParam(c,g,label,inst);
  
    
    % Writing the values
    fprintf(f,'%s\t%5.3e\t%5.3e\t%5.3e\t%5.3e\t%5.3e\t%5.3e\t%5.3e\n',...
        setNames{i},min(c),max(c),min(g),max(g),cv,c_coarse(i),g_coarse(i));
    
    % Save the figure
    title(sprintf('Coarse Parameter Search: %s',setNames{i}));
    filename = fullfile(imdir,sprintf('%s_coarseSearch',setNames{i}));
    print(gcf,'-dpng',filename);
    
    i = i + 1;
end

%% Fine Grid Search
c_fine = zeros(size(dataSets));
g_fine = zeros(size(dataSets));
i = 1;
n  = 10;
fprintf(f,'\nFine Grid Search with %d values\n',n^2);
fprintf(f,'\t\t\tDataSet\t\t\t\tminC\tmaxC\tminG\tmaxGAccuracy\tg\t\tc\n');
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
    fprintf(f,'%s\t%5.3e\t%5.3e\t%5.3e\t%5.3e\t%5.3e\t%5.3e\t%5.3e\n',...
        setNames{i},min(c),max(c),min(g),max(g),cv,c_fine(i),g_fine(i));
    
    % Save the figure
    title(sprintf('Fine Parameter Search: %s',setNames{i}));
    filename = fullfile(imdir,sprintf('%s_fineSearch',setNames{i}));
    print(gcf,'-dpng',filename);
    i = i + 1;
end

%% Probably need to do validation
fclose(f);