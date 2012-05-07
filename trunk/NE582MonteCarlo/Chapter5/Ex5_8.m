%% Exercise 5-8
% Monte Carlo Integration of Nested Radioactive Decay

% Setting up enviroment
clear all;
close all;

%% Problem Constants
A = 1;          % Index of Isotope A
B = 2;          % Index of Isotope B
C = 3;          % Index of Isotope C
numIsotopes = 3;
halfLives = [1 0.5];                    % Half lives of the isotopes (s)
lambda = log(2)./halfLives;             % Decay Constant (1/s)
tMax = 3;                               % Max Time

%% Nested Integration Implementation
nHist = 100000;
tA = zeros(nHist,1);
tB = zeros(nHist,1);
tC = zeros(nHist,1);

% Calculating Deviates
for i=1:nHist
   tA(i) = random('uniform',0,tMax);
   tB(i) = random('uniform',0,tA(i));
   tC(i) = random('uniform',0,tB(i));
end

% Calcualting Weights
wa = exp(-lambda(A).*tA)*tMax;
wb = lambda(A).*wa.*exp(-lambda(B).*(tA-tB)).*tA./sqrt(1/2);
wc = lambda(B).*wb;

% Results
fprintf('Integration Results with %d histories\n',nHist);
fprintf('\t Amount of C: %3.5f +/- %3.5f\n',sum(wc)/nHist,std(wc)/sqrt(nHist));

nBins = 25;
figure(1); hist(wc,nBins); title('Amount of C');

%% Analog Implementation
dt = 0.01;                              % Time Setup
t = 0:dt:tMax;
N = zeros(numIsotopes,numel(t));        % Inital Conditions
N0 = 10000;
N(:,1) = N0*[1 0 0]';

% Looping through time
for i=2:numel(t)
    % Default is nothing happening
    N(:,i) = N(:,i-1);
    
    % Simulating Decay for Isotope A.   Need to loop over all of the
    % nuclei present, giving each a chance to decay.
    for j=1:N(A,i)
        if ( rand() < lambda(A)*dt)
            N(A,i) = N(A,i)-1;              % Decays from A to B
            N(B,i) = N(B,i)+1;
        end
    end
    
    % Simulating Decay for Isotope B.  Need to loop over all of the nuclei
    % present giving each a chance to decay.
    for j=1:N(B,i)
        if ( rand() < lambda(B)*dt)
            N(B,i) = N(B,i)-1;              % Decays from B to C
            N(C,i) = N(C,i)+1;
        end
    end
end

% Output
N = N./N0;                              % Normalizing to inital amount
fprintf('Analog Results after %3.2f s\n',t(end));
fprintf('\t Amount of A: %3.2e\n',N(A,end));
fprintf('\t Amount of B: %3.2e\n',N(B,end));
fprintf('\t Amount of C: %3.2e\n',N(C,end));
figure(2);
semilogy(t,N(A,:),t,N(B,:),t,N(C,:));
grid on;
legend('A','B','C');
title('Analog Decay');
xlabel('Time (s)');
ylabel('Fraction of Initial Amount');