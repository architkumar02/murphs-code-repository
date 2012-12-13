%% Checks the implementation of my statistics
% Creating and checking the distribution
b = random('norm',0,1,1E6,1);
hist(b,100);

% Computing the mean and variance itteratively
myMu = 0;
myV = 0;
numEntries = 0;
for i = 1:numel(b)
    myV = myV + b(i).^2;
    myMu = myMu + b(i);
    
    numEntries = numEntries +1;
end
myV = (numEntries/(numEntries-1))*(myV/numEntries - (myMu/numEntries)^2);
myMu = myMu/numEntries;

% Comparing to Matlab
fprintf(1,'\t\tMean\tVar\n');
fprintf(1,'Mine:\t%5.4f\t%5.4f\n',myMu,myV);
fprintf(1,'Matlab:\t%5.4f\t%5.4f\n',mean(b),var(b));
fprintf(1,'Diff:\t%5.3e\t%5.3e\n',(mean(b)-myMu),(var(b)-myV));
