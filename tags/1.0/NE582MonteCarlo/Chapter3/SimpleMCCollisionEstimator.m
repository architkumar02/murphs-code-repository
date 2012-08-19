function [xhat,binNames,absorb,nsour]= SimpleMCCollisionEstimator()
%% Simple MC With a Collision Esimator
% Matthew J. Urffer (matthew.urffer@gmail.com)


%% Material Properties
ng = 2;                                 % Number of groups
totxs = [0.1, 0.2];                     % Total cross section in group i
scat = [0.05, 0.04; 0.0, 0.1];          % Scatter from group i to j
sour = [4, 6];                          % Source in group i

% Displaying Parameters
fprintf(1,'Total number of E groups %d\n',ng);
fprintf(1,'Total Cross Sections\n');
disp(totxs);
fprintf(1,'Inter Group Scattering\n');
disp(scat);
fprintf(1,'Source Cross Sections\n');
disp(sour);
%% Geometry Properies
width = 50;
sWidth = 5;
V = width; 

%% Computing Derived Quanties
% Total Scattering Cross Section for each group
totscat = zeros(ng,1);
for i=1:ng
    for j=1:ng
        totscat(i) = totscat(i) + scat(i,j);
    end
end
fprintf(1,'Total Scattering Matrix\n');
disp(totscat);

% Convert Source to CDF
sour = cumsum(sour);
sour = sour/sour(end);
fprintf(1,'Source CDF\n');
disp(sour);

% Converting Scattering Cross Sections to CDF
for i = 1:ng
    for j=2:ng
        scat(i,j) = scat(i,j)+scat(i,j-1);
    end
    for j=1:ng
        scat(i,j) = scat(i,j)/scat(i,end);
    end
end
fprintf(1,'Total Scattering CDF\n');
disp(scat);

%% Setting up Binning Strucutres
nbins = 6;
bin = zeros(nbins,1);
binNames = ...
   {'Particles Exiting Left (group 1) ','Particles Exiting Left (group 2) ',...
    'Particles Exiting Right (group 1)','Particles Exiting Right (group 2)',...
    'Flux (group 1)                   ','Flux (group 2)                   '};
bin2 = zeros(nbins,1);
absorb = zeros(ng,1);
nsour = zeros(ng,1);
pos = 0;

%% Running Simulation
nhist = 100000;

for ih=1:nhist
    tbin = zeros(nbins,1);
    
    %% Find the Original Position, energy and direction
    x = sWidth*rand(1);      % Position
    mu = 2*rand(1)-1;        % Direction cosine
    w = 1;                   % Particle Weight
    if ( rand(1) < sour(1))
        ig = 1;
    else
        ig = 2;
    end
    nsour(ig) = nsour(ig) + 1;
    %% Simulate the Particle bouncing around
    % This is done by using an infinate loop that is broken once the
    % particle leaves the problem geometry or is absorbed
    while (true)
        %% Mean Free Paths to Next Collision and Distance
        mfp = -log(1-rand(1));
        dd = mfp/totxs(ig);
        x = x+dd*mu;
        pos(end+1) = x;
        
        %% What Could have Happened?        
        if (x <0)               % Particle has exited the left boundary
            % Contribute to leakage
            tbin(ig) = tbin(ig)+1;
            break;
        elseif (x > width)      % Particle has exited the right boundary
            % Contibute to leakage
            tbin(ig+ng) = tbin(ig+ng)+1;
            break;
        else                    % Must have been a collision
            % Contribute to flux bins 

            tbin(ig+4)= tbin(ig+4)+ w/totxs(ig);
            if (rand(1) > totscat(ig)/totxs(ig) )
                % Absorbition Collions
                absorb(ig) = absorb(ig) +1;
                break;
            else
                % Pick new direction and energy group
                mu = 2*rand(1)-1;
                if ( rand(1) < scat(ig,1))
                    ig = 1;
                else
                    ig = 2;
                end
                % Code continues on to MFP calcuation (top of while)
            end
            
        end
    end
    bin = bin + tbin;
    bin2 = bin2 + tbin.^2;
end
% Dividing the Flux Bins by the volume
bin(4:end) = bin(4:end)/V;
bin2(4:end) = bin(4:end)/V;
%% Output results
xhat = bin/nhist;
x2hat = bin2/nhist;
sampleVar = (nhist/(nhist-1))*abs(x2hat - xhat.^2);
meanVar = sampleVar/nhist;
meanSTD = sqrt(meanVar);
precent = meanSTD./xhat;
fprintf(1,'Simulating %d histories\n',nhist);
for i=1:nbins
    fprintf(1,'For bin %d %s \t%2.4e\t +/- %2.2e (%2.4f%%)\n',i,binNames{i},xhat(i),meanSTD(i),precent(i));
end

% Looking at histograms of direction and position

pos(1) = [];
pos(pos <0 | pos > width) = [];
figure(1);  hist(pos);  title(sprintf('Position (n=%d)',nhist)); 
ylabel('Frequency'); xlabel('cm');
end