function [predict_label, accuracy, dec_values,C,order] =  ...
    SVMWeightPredict(weight, label,inst, model)
% [predict_label, accuracy, dec_values,C,Order] = SVMWeightPredictict(weight, label,inst, model)
% Computes the predicted class and accuracy of an SVM model. The accuracy
% is weighted by the weight of that point (Table 2, X. Li et al). LIBSVM is
% used to calculate the predicted label, accuracy, and probability /
% decision value for each data point.  The weights are then applied.
% INPUTS:
%   weight - the weight of the data point (vector)
%   label  - the actual lable of the data point (vector)
%   inst   - the data point (vector)
%   model  - the SVM model (see svmtrain)
% OUTPUTS:
%   predict_label - the predicted class label
%   accuracy      - the weighted accuracy of the classifer
%   dec_values    - decision values (actual values returned from SVM)
%   C             - Confusion Matrix (see confusionmat)
%   order         - Order  (see confusionmat)

% Input checking
if (numel(weight) ~= numel(label) ~= numel(inst))
    error('SVMWeightPredict: %s','number of input elements must match');
end

% Allocating return values
predict_label = zeros(size(weight));
accuracy = zeros(size(weight));
dec_values = zeros(size(weight));

% Looping through the data set
for i = 1:numel(weight)
    % Calling LIBSVM svmpredict
    [p,a,d] = svmpredict(label(i),inst(i),model);
    
    % Assigning return values
    predict_label(i) = p;
    accuracy(i) = a*weight(i);
    dec_values(i) = d;
end
% Calculating the confusion matrix
[C,order] = confusionmat(label,predict_label);
end