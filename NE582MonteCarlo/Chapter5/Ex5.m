%% Monte Carlo Integration
% Computing the intgral of exp(x), from zero to 3
anylI = exp(3) - 1;     % Analytical result
f = @(x) exp(x);
xlims = [0 3];
n = 10000;
fprintf(1,'Analytical Integration yields %f\n',anylI);

%% Exercise 5-1 - Rejection Integration
% The integration methods approximates the integral by the entire area if
% it is a hit, or zero if it is a miss.  the maxmium value of the inegral
% needs to be found; this is exp(3).
% Steps are as follows:
%%
%
% # Choose an xhat (xi) unifomrly between the limits
% # Choose a value of fi unifomly between 0 and fmax
% # Score Ii = fmax*diff(limits) if fi < f(xi)
fmax = f(xlims(end));               % Max Function Value
xi = xlims(end)*rand(n,1)-xlims(1); % Choosing xi between xlims
fi = fmax*rand(n,1);                % Choosing fi uniformly between 0 and fmax
rejI = fi < f(xi);
rejI = rejI*fmax*diff(xlims);
rejI_mu = sum(rejI)/n;
rejI_std = std(rejI)/sqrt(n);
fprintf(1,'5-1 Rejection integaration yeilds %f +/- %f with n=%d samples\n',rejI_mu,rejI_std,n);

%% Exercise 5-2 - Averaging Integration
% Steps are as follows:
%%
%
% # Choose an xhat (xi) unifomrly between the limits
% # Score Ii = f(xi)*diff(limits)
xi = xlims(end)*rand(n,1)-xlims(1); % Choosing xi between xlims
avgI = f(xi)*diff(xlims);
avgI_mu = sum(avgI)/n;
avgI_std = std(avgI)/sqrt(n);
fprintf(1,'5-2 Average integaration yeilds   %f +/- %f with n=%d samples\n',avgI_mu,avgI_std,n);

%% Exercise 5-3 - Control Variates
% The Control Varaiates Method Esentially Integrates the differnece between
% a user inputed, well behaved function and the integral in question
% Steps are as follows:
%%
%
% # Choose an xhat (xi) uniformly between the limits
% # Score Ii = (f(xi)-h(xi))*diff(limits)+Ih
h = @(x) 1  + sqrt(1/2)*x.^3;
Ih = 81/8*sqrt(2)+3;
xi = xlims(end)*rand(n,1)-xlims(1); % Choosing xi between xlims
cvI = (f(xi)-h(xi))*diff(xlims)+Ih;
cvI_mu = sum(cvI)/n;
cvI_std = std(cvI)/sqrt(n);
fprintf(1,'5-3 Control Variates int. yeilds  %f +/- %f with n=%d samples\n',cvI_mu,cvI_std,n);

%% Exercise 5-4 - Importance Sampling
% Steps are as follows:
%%
%
% # Pick random deviate xi from h(x)
% # score result according to Ii = f(xi)/h(xi)*Ih
% Picking From h(x) = 1+sqrt(1/2)*x^3 by rejection
piMax = h(xlims(2));
xi = zeros(n,1);
i = 1;
while (i<=n) 
  x = xlims(1)+diff(xlims)*rand(1);
  y = piMax*rand(1);
  if (y < h(x))
      xi(i) = x;
      i = i+1;
  end
end
% figure(1);hist(xi);title('Rejection Picking From h(x)');
% Computing scores
isI = f(xi)./h(xi)*Ih;
isI_mu = sum(isI)/n;
isI_std = std(isI)/sqrt(n);
fprintf(1,'5-4 Importance Sampling yeilds    %f +/- %f with n=%d samples\n',isI_mu,isI_std,n);

%% Looking at the various distibutions performance
figure;
hist([rejI,avgI,cvI,isI],50);
xlim([0 60]);
legend('Rejection','Average','Control Variates','Importance Sampling');
title('Monte Carlo Approximation to \int_{0}^{3} e^x');
