function [days,calls,secs,texts] = importSparseData(filename)
% [days,calls,secs,texts] = importSparseData(filename)

fid = fopen(filename,'r');
C = textscan(fid,'%f %f %f %f %f %f %f %f');
fclose(fid);

% Filling the Matricies
days = sparse(C{1},C{3},C{5});
calls= sparse(C{1},C{3},C{6});
secs= sparse(C{1},C{3},C{7});
texts= sparse(C{1},C{3},C{8});
end