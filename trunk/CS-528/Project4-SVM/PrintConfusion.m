function PrintConfusion(f,label,predict_label)

[C,order] = confusionmat(label,predict_label);
[x,y] = size(C);

s = ' c | ';
for i=1:numel(order)
    s = strcat(s, ' c ');
end
% Table Header
fprintf(f,'\\begin{tabular} { %s }\n',s);
fprintf(f,'\\hline \n');

s = ' ';
for i=1:x
    s = strcat(s, sprintf(' & %d ',i));
end
fprintf(f,'%s \\\\ \n',s);
fprintf(f,'\\hline \n');

% Printing out the values
for i=1:x
    s = sprintf('%d',i);
    for j=1:y
        s = strcat(s,sprintf(' & %d ',C(i,j)));
    end
    fprintf(f,'%s \\\\ \n',s);
end

% Table end
fprintf(f,'\\hline \n');
fprintf(f,'\\end{tabular}\n');
end