%% Data Distributions
% Examaing the class and instance distrubtions
SVMMatlabSetup
ProjectSetup
i = 1;
for data = dataSets
    fprintf(1,'Processing data set: %s\n',cell2mat(dataSets(i)));
    % Reading in the data
    [label,inst]=libsvmread(fullfile(dataPath,dataSets{i}));

    [n,xout] = hist(label,unique(label));
    bar(xout,n/numel(label));
    
    % Save the figure
    title(sprintf('Class Distribution: %s',setNames{i}));
    filename = fullfile(imdir,sprintf('%s_ClassDist',setNames{i}));
    xlabel('Class number');
    ylabel('Frequency');
    print(gcf,'-dpng',filename);
    
    i = i + 1;
end