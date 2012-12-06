%% PART ONE
% Matthew J. Urffer (matthew.urffer@gmail.com)
% CS-528 Project4-SVM
% Using LibSVM to find ideal C and gamma values
SVMMatlabSetup

%% Corse Grid Search
c_coarse = zeros(size(dataSets));
g_coarse = zeros(size(dataSets));
i = 1;
for data = dataSets
    fprintf(1,'Processing data set: %s\n',cell2mat(dataSets(1)));
    % Reading in the data
    [label,inst]=libsvmread(cell2mat(fullfile(dataPath,dataSets(1))));
    
    % Preforming a coarse mesh (same for all)
    c = -4:0.1:4;
    g = -4:0.1:4;
    [~,c_coarse(i),g_coarse(i)] = gridParam(c,g,label,inst);
    i = i + 1;
    
    % Want to grab the lowest, and further refine
    c = 0.5*c:0.01:1.5*c;
    g = 0.5*g:0.01:1.5*g;
    [cv,c,g] = gridParam(c,g,label,inst);
    
    % Save the figure
    title(sprintf('Coarse Parameter Search: %s',setNames{i}));
    filename = fullfile(imdir,sprintf('%s_coarseSearch',setNames{i}));
    print(gca,'-dpng',filename);
end

%% Fine Grid Search
c_fine = zeros(size(dataSets));
g_fine = zeros(size(dataSets));
i = 1;
for data = dataSets
    fprintf(1,'Processing data set: %s\n',cell2mat(dataSets(1)));
    % Reading in the data
    [label,inst]=libsvmread(cell2mat(fullfile(dataPath,dataSets(1))));
    
    % Want to grab the lowest, and further refine. This is based on the
    % lowest c, g value from the coarse search
    c = 0.5*c_coarse(i):0.01:1.5*c_coarse(i);
    g = 0.5*g_coarse(i):0.01:1.5*g_coarse(i);
    [~,c_fine(i),g_fine(i)] = gridParam(c,g,label,inst);
    i = i + 1;
    
    % Save the figure
    title(sprintf('P Parameter Search: %s',setNames{i}));
    filename = fullfile(imdir,sprintf('%s_fineSearch',setNames{i}));
    print(gca,'-dpng',filename);
end