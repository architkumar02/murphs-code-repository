function [bestcv, bestc, bestg] = gridParam(c,g,label,inst)
% [bestcv, bestc, bestg] = gridParam(c,g,label,inst)
% Preforms a grid search of the parameters c and g for an SVM
% Inputs:
%   c   - [cmin cmax] range of penatly / cost for error
%   g   - [gmin gmax] range of gamma (RBF kernal) value
%   label - label vector of classes
%   inst  - instance matrix of classes
% Outputs:
%   bestcv - the best accuracy found
%   bestc  - the cost value assocatied with the highest accuracy
%   bestg  - the RBF value asscoiated with the highest accuracy
% Ussage:
%   Labels and inst are generated with the libsvmread command:
%   [label,inst]=libsvmread('data\glass_scale_test.txt');
% 
%   The prameter search is preformed with:
%   [bestcv, bestc, bestg] = gridParam([-1:3],[-4:1],label,inst)
i = 0;
results=zeros(numel(c),numel(g));
bestcv = 0;
for log2c = c,
    i = i+1;
    j = 0;
  for log2g = g,
      j = j+1;
    cmd = ['-q -e 2 -v 5 -c ', num2str(2^log2c), ' -g ', num2str(2^log2g)];
    weights = ones(size(label))/numel(label);
    cv = svmtrain(weights,label, inst, cmd);     % Ignore the first argument (no weights)
    results(i,j) = cv;
    if (cv >= bestcv),
      bestcv = cv; bestc = log2c; bestg = log2g;
    end
  end
end
fprintf('best c=%g, g=%g, rate=%g)\n',bestc, bestg, bestcv);
[~,h] = contour(c,g,results');
set(h,'ShowText','on','TextStep',get(h,'LevelStep')*2)
xlabel('log_2 c','fontsize',12);
ylabel('log_2 \sigma','fontsize',12);
title('Accuracy','fontsize',14)
colorbar;