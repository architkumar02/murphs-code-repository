%% Exercise 5-9
% Monte Carlo Drunken Sailor
clear all;
close all;

%% Setting up the grid
gridSize = 5;               % 5x5 grid
gridResolution = 0.25;       % Smallest Division
x = 0:gridResolution:gridSize;
grid = zeros(numel(x));
grid(:,1) = 1;              % Left Boundary
grid(end,:) = 4;              % Top Boundary
grid(:,end) = 3;            % Right Boundary
grid(1,:) = 2;            % Bottom Boundary
[C,h] = contourf(x,x,grid,3);
colormap jet; colorbar;
title('Initial Grid');

%% Monte Carlo Scoring
% Scoring is accomplished by the following steps:
% # Begin a history
% # Let the particle wander from point to point on the gird
% # Score the particle by the boundary value once it reaches the boundary


nHist = 10000;
[n m] = size(grid);
bin = zeros(n,m);
bin2 = zeros(n,m);

for i=2:n-1
    for j=2:m-1
        
        for itter=1:nHist
            xPos = i;
            yPos = j;
            while (true)
                
                % Checking that we are still in range
                if (xPos == 1 || xPos==n || yPos ==1 || yPos==m)
                    % Score, and then break
                    
                    score = grid(xPos,yPos);
%                     fprintf(' Particle at (%d %d), score: %d\n',xPos,yPos,score);
                    bin(i,j) = bin(i,j) + score;
                    bin2(i,j) = bin2(i,j) + score.^2;
                    break;
                end
                
                % Choosing a left - right direction
                dx = +1;
                if (rand < 0.5)
                    dx = -1;
                end
                
                % Choosing an up -down direction
                dy = +1;
                if (rand < 0.5)
                    dy = -1;
                end
                
                % Moving the Particle
                xPos = xPos + dx;
                yPos = yPos + dy;
            end
        end    % End of History looping
    end
end

%% Output
xhat = bin/nHist;
x2hat = bin2/nHist;
sampleVar = (nHist/(nHist-1))*abs(x2hat - xhat.^2);
meanSTD = sqrt(sampleVar/nHist);

figure(2);
contourf(x,x,xhat);
colormap jet; colorbar;
title('Diffusion Estimates');

figure(3);
contourf(x,x,meanSTD);
colormap jet; colorbar;
title('Standard Devation on Diffusion Estimates');
yIndex = find(x==3.5);
xIndex = find(x==1.5);
fprintf(1,'Function at (3.5, 1.5) is %f +/- %f with %d samples\n',xhat(xIndex,yIndex),meanSTD(7,3),nHist);
