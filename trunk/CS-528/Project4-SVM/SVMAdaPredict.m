function [predict_label,accuracy] =SVMAdaPredict(label, inst, ensambleSVM,alpha)
% [predict_label, accuracy, dec_values] = SVMWeightPredictict(weight, label,inst, model)
% Computes the perfromance of an esamble method of SVM's
% INPUTS:
%   label  - the label of the class (or zeros)
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
predict_label = zeros(size(label),numel(ensambleSVM));

%% Looping through all of the classifiers
for i=1:numel(ensambleSVM)
    
    % Classification
    [predict,~,dec_value] = svmpredict(label,inst,ensambleSVM{i});
    
    % Weighting scheme
    predict_label(:,i) = predict;  
end
predict_label = mode(predict_label');
classes = ensambleSVM{1}.nr_class;
[CM,order] = confusionmat(label,predict_label);
accuracy = trace(CM)/sum(sum(CM));
