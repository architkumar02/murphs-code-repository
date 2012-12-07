[heart_scale_label, heart_scale_inst] = libsvmread('libsvm-3.14/heart_scale');
heart_scale_weight = load('libsvm-3.14/heart_scale.wgt');
model = svmtrain(heart_scale_weight, heart_scale_label, heart_scale_inst, '-c 1 -q');

% Testing
[predict_label, accuracy, dec_values] = svmpredict(heart_scale_label, heart_scale_inst, model); % test the training data

% To test without knowing the labels:
% The accuracy no longer makes sense
b = zeros(size(heart_scale_label));
[predict_label,~,dec_values] = svmpredict(b, heart_scale_inst, model); % test the training data
