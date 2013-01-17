function BE = SemiEmpericalMass(A,Z)
if (mod(A,2)==1 && mod(Z,2)==1)
    delta = -11.18;
elseif (mod(A,2)==0 && mod(Z,2) ==0);
    delta = 11.18;
else
    delta = 0;
end
t1 = 15.56*A;
t2 = -17.23*A.^(2/3);
t3 = -0.7*Z^2.*A^(-1/3);
t4 = -23.285*(A-2*Z)^2 / A;
t5 = delta*A^(-1/2);
BE = t1 + t2 + t3 + t4 + t5;