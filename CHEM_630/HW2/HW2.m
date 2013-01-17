%% Binding Energy Per Nucleon
Z = 1:172;
A = 1:300;

BE = zeros(numel(Z),numel(A));
for i =1:numel(Z)
    for j=1:numel(A)
        BE(i,j) = SemiEmpericalMass(A(j),Z(i))/A(j);
        if BE(i,j) < 0
            BE(i,j) = 0;
        end
    end
end
figure(1);
[C,h] = contour(A,Z,BE);
clabel(C,h);
xlabel('A');
ylabel('Z');
title({'Binding Energy per nucleon','Semi-emperical Mass Model'});

%% Check published values of B.E. per Nucleon
M = csvread('BindingEnergiesNNDC.csv',2,0);
Z = 1:max(M(:,1));
A = 1:max(M(:,2));
BEPub = zeros(numel(Z),numel(A));
BEError = zeros(size(BEPub));
[s,~] = size(M);
for i=1:s
    BEPub(M(i,1),M(i,2)) = M(i,3);
    BEError(M(i,1),M(i,2)) = M(i,3)- (SemiEmpericalMass(M(i,2),M(i,1))/M(i,2))*1000;
end
figure(2);
[C,h] = contour(A,Z,BEPub);
xlabel('A');
ylabel('Z');
title({'Binding Energy per nucleon (keV)','NNDC Data'});

figure(3);
[C,h] = contour(A,Z,log(BEError));
xlabel('A');
ylabel('Z');
title({'Comparison between Binding Energies','log(BEPub-BESemi)'});

%% Agreement

[s,~] = size(M);
for i=1:s
    BEPub(M(i,1),M(i,2)) = M(i,3);
end

%% What happens / is there a limit?
% Z = 1:1250;
% A = 1:2500;
% BE = zeros(numel(Z),numel(A));
% for i =1:numel(Z)
%     for j=1:numel(A)
%         BE(i,j) = SemiEmpericalMass(A(j),Z(i))/A(j);
%         if BE(i,j) < 0
%             BE(i,j) = 0;
%         end
%     end
% end
% figure(3);
% [C,h] = contour(A,Z,BE);
% clabel(C,h);
% xlabel('A');
% ylabel('Z');
% title({'Binding Energy per nucleon','Semi-emperical Mass Model'});