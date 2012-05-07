%% Evaluate a 3D Integral using Monte Carlo
% Exercise 5-5
%%
% 
% <latex>
% I = \int_{0}^{2}\int_{0}^{2}\int_{0}^{2}(x+y+z )^2dxdydz
% </latex>
% 
% The averaging method is employed
% Steps are as follows:
%%
%
% # Choose an xhat (xi) uniformly between the limits
% # Score Ii = f(xi)*diff(limits)
function Ex5_5

exampleIntegration();
    iAnyl = 80;
    haltonBase = [2,3,5];
    Samples = pow2(1:19);
    errorHalton = zeros(numel(Samples),3);
    errorAverage = zeros(numel(Samples),1);
    
    for N=1:numel(Samples)
        for base=1:numel(haltonBase)
        [Iavg,Ihalton] = computeMC(Samples(N),haltonBase(base));
        errorHalton(N,base) = Ihalton(1)-iAnyl;
        end
        errorAverage(N) = Iavg(1)-iAnyl;
    end
    figure;
    loglog(Samples,abs(errorAverage),Samples,abs(errorHalton(:,1)),Samples,abs(errorHalton(:,2)),Samples,abs(errorHalton(:,3)));
    title('Error for Monte Carlo Integration of I = \int_{0}^{2}\int_{0}^{2}\int_{0}^{2}(x+y+z )^2dxdydz');
    legend('Average','Halton Base 2','Halton Base 3','Halton Base 5');
    xlabel('Number MC Samples');
    ylabel('|Error|');
end
function [Iavg Ihalton] = computeMC(N,haltonBase)
f = @(x) (x(1)+x(2)+x(3)).^2;
xLimits = [0 2; 0 2; 0 2];
Volume = prod(diff(xLimits'));
n = N;
numDim = 3;

%% Random Deviates
% Computing random deviates
xi = diag(xLimits(:,1))*ones(numDim,n) + diag(diff(xLimits'))*rand(numDim,n);
% Computing Scores
avgIRand = zeros(1,n);
for i=1:n
    avgIRand(i) =f(xi(:,i));
end
avgIRand = avgIRand*Volume;
avgI_mu = sum(avgIRand)/n;
avgI_std = std(avgIRand)/sqrt(n);
Iavg = [avgI_mu, avgI_std];


%% Halton Sequence
% Generating the Halton Sequence of Base 3, skipping the first 1,000, and
% grapping every 100th tiplet after that

% Calculating the size of array needed
numElements = numDim*n;
p = haltonset(haltonBase,'Skip',1e3,'Leap',1e2);
deviates = p(1:ceil(numElements/haltonBase),:);
deviates = reshape(deviates,numel(deviates),1);
deviates(floor(numel(deviates)/numElements)*numElements+1:end) = [];  % Chooping off end
deviates = reshape(deviates,n,numDim);
xi =  diag(xLimits(:,1))*ones(numDim,n)+diag(diff(xLimits'))*deviates';

% Computing Scores
avgIHalt = zeros(1,n);
for i=1:n
    avgIHalt(i) =f(xi(:,i));
end
avgIHalt = avgIHalt*Volume;
avgI_mu = sum(avgIHalt)/n;
avgI_std = std(avgIHalt)*numDim/sqrt(n);
Ihalton = [avgI_mu, avgI_std];

end

function exampleIntegration()
% Problem Setup
% We let f(x') now be a 'vector' function, where x' = (x,y,z)
f = @(x) (x(1)+x(2)+x(3)).^2;
% Integration limits.  First column is lower bound, second is upper bound
xLimits = [0 2; 0 2; 0 2];
Volume = prod(diff(xLimits'));
n = 1000;
numDim = 3;

%% Random Deviates
% Computing random deviates
xi = diag(xLimits(:,1))*ones(numDim,n) + diag(diff(xLimits'))*rand(numDim,n);
figure(1); hist(xi',25);
legend('x','y','z'); title('Random Deviates');

% Computing Scores
avgIRand = zeros(1,n);
for i=1:n
    avgIRand(i) =f(xi(:,i));
end
avgIRand = avgIRand*Volume;
avgI_mu = sum(avgIRand)/n;
avgI_std = std(avgIRand)*numDim/sqrt(n);
fprintf(1,'Average integaration (with random sampling) yeilds %f +/- %f with n=%d samples\n',avgI_mu,avgI_std,n);

%% Halton Sequence
% Generating the Halton Sequence of Base 3, skipping the first 1,000, and
% grapping every 100th tiplet after that
p = haltonset(3,'Skip',1e3,'Leap',1e2);
xi =  diag(xLimits(:,1))*ones(numDim,n)+diag(diff(xLimits'))*p(1:n,:)';
figure(2); hist(xi',25);
legend('x','y','z'); title('Halton Sequence');

% Computing Scores
avgIHalt = zeros(1,n);
for i=1:n
    avgIHalt(i) =f(xi(:,i));
end
avgIHalt = avgIHalt*Volume;
avgI_mu = sum(avgIHalt)/n;
avgI_std = std(avgIHalt)*numDim/sqrt(n);
fprintf(1,'Average integaration (Halton Sequence) yeilds %f +/- %f with n=%d samples\n',avgI_mu,avgI_std,n);

%% Comparison between the two
figure(3);
hist([avgIRand;avgIHalt]',25);
 xlim([0 250]);
legend('Random Deviates','3D Halton');
title('Monte Carlo Approximation to \int_{0}^{2}\int_{0}^{2}\int_{0}^{2}(x+y+z )^2dxdydz');
ylabel(sprintf('Frequency (n=%d samples',n));
xlabel('Integral Value.  Analytical Value is 80.0');

end
