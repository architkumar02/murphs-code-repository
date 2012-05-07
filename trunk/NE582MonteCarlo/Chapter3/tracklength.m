%NE 582
%slab

clear
clc

%constants

totxs(1)=0.1;
totxs(2)=0.2;
scat(1,1)=.05;
scat(1,2)=0.04;
scat(2,1)=0;
scat(2,2)=0.1;
sour(1)=4;
sour(2)=6;


ng=2;
for ib=1:6
    bin(ib)=0;
    bin2(ib)=0;
end

%total scattering cross section
for ig=1:ng
    totscat(ig)=0;
    for jg=1:ng
        totscat(ig)=totscat(ig)+scat(ig,jg);
    end
end

%Convert source to CDF
for ig=2:ng
    sour(ig)=sour(ig-1)+sour(ig);
end
for ig=1:ng
    sour(ig)=sour(ig)/sour(ng);
end

%Convert scattering cross sections to CDF
for ig=1:ng
    for jg=2:ng
        scat(ig,jg)=scat(ig,jg-1)+scat(ig,jg);
    end
    for jg=1:ng
        scat(ig,jg)=scat(ig,jg)/scat(ig,ng);
    end
end

nhist=input('Number? ');

blah=0;
for ih=1:nhist
    count=1;
    distance=0;
    
    
    for ib=1:6
        tbin(ib)=0;
    end
    
    %position
    x=5*rand;
    
    %direction
    mu=2*rand-1;
    
    %energy group
    if rand<0.4
        ig=1;
    else
        ig=2;
    end
    
    while count==1
    %mean free paths
    test=1-rand;
    if test<=0
        test=1-rand;
    end
    mfp=-log(test);
    
    %mfp to distance in cm
    dd=mfp/totxs(ig);
    
    %new position
    x=x+dd*mu;
    
    %if exit left boundary
    loopexit=0;
    if x<0
        distance=abs(dd*mu)+x;
        tbin(ig+4)=tbin(ig+4)+(abs(distance/(50*mu)));
       
        tbin(ig)=tbin(ig)+1;
        loopexit=1;
        count=0;
        for ib=1:6
            bin(ib)=bin(ib)+tbin(ib);
            bin2(ib)=bin2(ib)+tbin(ib)*tbin(ib);
        end
        break
    end
    
    if loopexit==1;
        continue
    end
    
    %if exit right boundary
    loopexit=0;
    if x>50
        distance=dd*mu+50-x;
        tbin(ig+4)=tbin(ig+4)+(distance/(50*mu));
        
        tbin(ig+2)=tbin(ig+2)+1;
        loopexit=1;
        count=0;
        for ib=1:6
            bin(ib)=bin(ib)+tbin(ib);
            bin2(ib)=bin2(ib)+tbin(ib)*tbin(ib);
        end
        break
    end
    
    if loopexit==1;
        continue
    end
    
    %if absorption collision
    
    loopexit=0;
    if rand > totscat(ig)/totxs(ig)
        distance=dd*mu;
        tbin(ig+4)=tbin(ig+4)+(distance/(50*mu));
        loopexit=1;
        count=0;
        for ib=1:6
            bin(ib)=bin(ib)+tbin(ib);
            bin2(ib)=bin2(ib)+tbin(ib)*tbin(ib);
        end
        break
    end
    if loopexit==1;
        continue
    end
    
    %if scattering collision
    blah=blah+1;
    distance=dd*mu;
    tbin(ig+4)=tbin(ig+4)+abs((distance/(50*mu)));
    mu=2*rand-1;
     if rand <scat(ig,1)
         ig=1;
     else
         ig=2;
     end
    end
     
end


    
    
for ib=1:6
    xhat=bin(ib)/nhist;
    x2hat=bin2(ib)/nhist;
    sample_variance=(nhist/(nhist-1))*(x2hat-(xhat*xhat));
    sample_sd=sqrt(sample_variance);
    mean_variance=sample_variance/(nhist-1);
    mean_sd=sqrt(mean_variance);
    
   disp(['For bin ', num2str(ib),' ', '==>', ' ',num2str(xhat),' ','+/-',' ', num2str(mean_sd)]);
    
    
    
end

group1abs=bin(5)/nhist;
group2abs=bin(6)/nhist;
leakage=sum(bin(1:4))/nhist;
disp(['Source =',' ',num2str(1)])
abs1=(group1abs*50*(totxs(1)-totscat(1)));
abs2=(group2abs*50*(totxs(2)-totscat(2)));
disp(['Absorption+Leakage =',' ',num2str(abs1+abs2+leakage)])
        
        
    
    
        
        
    
    
    
    
    








