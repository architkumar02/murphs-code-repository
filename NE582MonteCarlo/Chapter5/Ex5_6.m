%% Exercise 5-6
% Write a Monte Carlo Ccode to sample a differential equation and boundary
% condition


%% Problem Setup
histories = 1000000;
df = @(x) 2./x;
xi = zeros(histories,1);
ui = zeros(histories,1);

% Choosing xi and ui
for i=1:histories
   xi(i) = random('uniform',2,4);           % uniform from 2 to 4
   ui(i) = random('uniform',2,xi(i));       % uniform from 2 to xi
end
figure(1);
subplot(2,1,1);
hist(xi);
title('xi samples');
subplot(2,1,2);
hist(ui);
title('ui samples');

%% Scoring
IIntegral = 2*df(ui).*(xi-2) + 2*df(2);
IValue = 2*df(xi) + df(2);

%% Output results
% Computing 
Imu = sum(IIntegral)/histories;
Istd = 2*std(IIntegral)/sqrt(histories);
fprintf(1,'Integration from 2 to 4 yeilds    %f +/- %f with n=%d samples\n',Imu,Istd,histories);
figure(2);hist(IIntegral);title('\int_{2}^{4} 2/x dx');

Imu = sum(IValue)/histories;
Istd = std(IValue)/sqrt(histories);
fprintf(1,'Integration from 2 to 4 yeilds    %f +/- %f with n=%d samples\n',Imu,Istd,histories);
figure(3);hist(IValue);title('f(4)');