function result = div(a,b)
if (b < 1E-6)
    result = 0;
else
    result = a./b;
end