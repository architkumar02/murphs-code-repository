function [P,T] = importDataSet(path,dataSetDir)
% [P,T] = importDataSet(path,dataSetDir)
% Imports the data contained in dataSetDir found on path to a cell array in
% dataSet
% Inputs:
%   path - path to the dir containing the folder of data to be read
%   dataSetDir - directory (folder) that contains the data to be read
% Returns:
%   P - array of parterns(utility for using builtin neural nets)
%   T - array of targets (utility for using builtin neural nets)
%       The native neutral net tools in Matlab call train(net,P,T) where P
%       is the network input patterns (Ni x TS array, ni = # inputs)
%       and T is the network targets (NO x TS array, NO = # outputs)

fileList = dir(dataSetDir);         % List of files in that
fileList(1:2) = [];                 % Skipping the first two files (the . and ..)
% Creating P and T arrays
P = cell(1,numel(fileList));
T = cell(1,numel(fileList));

% Looping through all of the files
for i=1:numel(fileList)
    file = fullfile(path,dataSetDir,fileList(i).name);
    [data,class] = importChar(file);
    P{i} = data;        % Pattern
    T{i} = class;       % Target
end

P = cell2mat(P);
T = cell2mat(T);
end

function [data,class] = importChar(filename)
% [data,class] = importChar(filename)
%   filename - path of data file
%   Returns a boolean vector of data indicating the presence
%       of a line on a grid, as well a vector giving the class
% IT IS ASSUMED THAT THE ARRAY HAS 96 ELEMENTS IN THE PATTERN, AND 10 IN
% THE CLASS.

temp = importdata(filename);        % Imports data. Empty spaces are NaN's
[n,p] = size(temp);
temp = reshape(temp',n*p,1);        % Flattens the Array
temp(isnan(temp)) = [];             % Removes the NaN's
data = (temp(1:12*p));
[n,m] = size(data);
%data = data +ones(n,m);
class = temp(12*p+1:end);      
end