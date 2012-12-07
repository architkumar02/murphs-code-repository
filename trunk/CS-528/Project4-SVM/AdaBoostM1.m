function [ensambleSVM,alpha] = AdaBoostM1(label,inst,c,sigmaInit,sigmaMin,sigmaStep,nEnsamble)
% [ensambleSVM] = AdaBoostM1(label,inst,sigmaInit,sigmaMin,sigmaStep)
% Implementation of AdaBoostM1 accoring to:
% X. Li et al.
% Engineering Applications of Artificial Intelligence 21 (2008) 785–795
% INPUTS:
%   label - labels of the class (see libsvmread)
%   inst  - instances of the class (see libsvmread)
%   c     - the regularization parmater (see gridParam)
%   simgaInit - intial size of the RBF kernel (gamma, see gridParam)
%   sigmaMin  - minimum size of the RBF kernal (gamma)
%   sigmaStep - step size of the RBF kernal
%   nEsamble  - the number of weak classifiers to learn
% OUTPUTS:
%   ensambleSVM - the ensamble of SVMs
%   alpha       - the weights of the SVM classifers

% Setting up the ensamble
ensambleSVM = cell(nEnsamble,1);

% AdaBoost Variables
alpha = ones(nEnsamble,1)/nEnsamble;
weights = ones(size(label));   % Initialize weights to 1/N

for t=1:nEnsamble
    sigma = sigmaInit;
    sigmaPrev = sigma;
    weightsPrev = weights;
    while(sigma > sigmaMin)
        
        % Training the classifier and calculate the error
        cmd = ['-c ', num2str(2^c), ' -g ', num2str(2^sigma)];
        model = svmtrain(weights,label,inst,cmd);
        [predict_label, accuracy, dec_values] = svmpredict(label,inst, model);
        
        error = 1- accuracy(1)/100;
        
        % We want a weak learner (just above 50%)
        if error > 0.5
            sigmaPrev = sigma;
            weightsPrev = weights;
            sigma = sigma-2^(sigma/4);
        else
            cmd = ['-c ', num2str(2^c), ' -g ', num2str(2^sigmaPrev)];
            model = svmtrain(weightsPrev,label,inst,cmd);
            [predict_label, accuracy, dec_values] = svmpredict(label,inst, model);
            error = 1- accuracy(1)/100;
            % No longer weak
            break;
        end
    end % End of svm training
    
    % Setting the weight of the component classifer
    alpha(t) = 0.5*log((1-error)/error);
    ensambleSVM{t} = model;
    
    % Update the weights of the training data
    weights = weights.*exp(-alpha(t)*error);
    weights = weights/sum(weights);
end

end % End of AdaBoostM1