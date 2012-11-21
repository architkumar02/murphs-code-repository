load 'Data.mat'

figure(1);

f = @(x) 1.285*cos(1.102*x)+0.2662*cos(0.724*x)+0.3337*cos(3*x)+0.485*cos(1.19*x)+...
    0.2*cos(5*x)+1.05*cos(0.02*x)+0.143*cos(7*x)+0.11*cos(9*x);

plot(x,y,'-g',x,f(x),'-k');
xlabel('t (s)');
ylabel('f(t)');
title('Orginal Signal');

% Computing Fourier Transform
% T = x(2)-x(1);      % Sample Time
% Fs = 1/T;           % Sample Frequency
% 
% L = numel(x);
% NFFT = 2^nextpow2(L);
% Y = fft(y,NFFT)/L;
% f = Fs.*2*pi/2*linspace(0,1,NFFT/2+1);

% figure(2);
% plot(f,2*abs(Y(1:NFFT/2+1)));
% title('Single-Sided Amplide Spectrum');
% xlabel('Frequency (\omega)');
% ylabel('|y(t)|');
% numPeaks = 5;
% [v,I] = findpeaks(2*abs(Y(1:NFFT/2+1)),numPeaks);
% fprintf(1,'%d largest frequencies\n',numPeaks);
% x(I)


% Look at Matlab's best fit
% cftool 'BestFit.sfit'
