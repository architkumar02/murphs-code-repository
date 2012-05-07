function  Ex5_7()
%% Exercise 5-7
% Evaluate the following integral:
%%
% $\int_{1}^{2} y(x)dx$
%%
% Given the following:
%%
% $y''(x) = x^2;~~y'(0)=0; ~~ y(0) = 3$

% Defining Paramters and Functions
close all;
n = 100000;
f = @(x) x.^2;

% Solving Integrals
IZeroToTwo = integration(f,[0 2],n);
IZeroToOne = integration(f,[0,1],n);
IOneToTwo = IZeroToTwo - IZeroToOne;

PrintOutput(IOneToTwo,[1 2],n);

end
function [I] = integration(f,xLimits,n)

nBins = 25;

% Choosing Deviates
xi = zeros(n,1);        % total integral
xv = zeros(n,1);        % first derivative
xu = zeros(n,1);        % second derivative
for i=1:n
    xi(i) = random('uniform',xLimits(1),xLimits(2));
    xv(i) = random('uniform',0,xi(i));
    xu(i) = random('uniform',0,xv(i));
end
% figure;
% subplot(3,1,1); hist(xi,nBins); title('xi deviates');
% subplot(3,1,2); hist(xv,nBins); title('xv deviates');
% subplot(3,1,3); hist(xu,nBins); title('xu deviates');

% Evaluating the integral
wu = f(xu).*(xv-0);
wv = (3+wu).*(xi-0);
wf = wv.*(2-0);
I = wf;

PrintOutput(I,xLimits,n);

end

function PrintOutput(I,xLimits,n)
% Computing Output
nBins = 25;

Imu = sum(I)/n;
Istd = std(I)/sqrt(n)*3;
fprintf(1,'Integration from %3.2f to %3.2f is %f +/- %f with n=%d samples\n',xLimits(1),xLimits(2),Imu,Istd,n);
figure;hist(I,nBins);title(strcat('\int_',sprintf('{%3.2f}^{%3.2f}y(x)dx',xLimits(1),xLimits(2))));
end
