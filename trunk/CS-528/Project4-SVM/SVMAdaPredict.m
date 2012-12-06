function [predict_label] =SVMAdaPredict(inst, ensambleSVM,alpha)
% [predict_label, accuracy, dec_values] = SVMWeightPredictict(weight, label,inst, model)
% Computes the perfromance of an esamble method of SVM's
% INPUTS:
%   inst   - the data point (vector)
%   ensabmleSVM - the cell array of SVM models (see AdaBoostM1)
%   alpha       - the weight of each member of the ensamble 
% OUTPUTS:
%   predict_label - the predicted class label

%% Basic input checking
if (numel(ensambleSVM) ~= numel(alpha))
   error('SVMAdaPredict: %s','Number of ensamble classifiers must match alpha\n'); 
end

%% Setting up return variables

%% Looping through all of the classifiers
% Since we do not know the class, we must provide an emptpy array
fakeLabels = zero(size(inst));
for i=1:numuel(ensambleSVM)
    
    % Classificaiton
    [predict,~,dec_value] = svmpredict(fakeLabels,inst,ensambleSVM{i});
    
    % Weighting scheme
    
end
