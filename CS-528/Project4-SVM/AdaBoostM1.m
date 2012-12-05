function [bestSVM] = AdaBoostM1()
% [bestSVM] = AdaBoostM1
% Implementation of AdaBoostM1 accoring to:
% X. Li et al. 
% Engineering Applications of Artificial Intelligence 21 (2008) 785–795



for t=1:T
[classifier,error,diff_vec]=train_classifier_on_weighted_data(X,y,w);
if (error<=0 || error>=.5) return;
alpha=log((1-error)/error);
ensemble{t,1}=alpha; ensemble{t,2}=classifier;
w=normalize(w.*exp(alpha*(diff_vec*2-1)));
end