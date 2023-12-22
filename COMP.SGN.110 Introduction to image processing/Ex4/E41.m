% Exercise 4, Task 1 Mikhail Silaev

clear all
close all
%(a)
I = imread("university.png");
% these parameters (e.g. large c1 and c2) allow to stretch low-intensity domain to the whole range
% which make small variations of intensity more visible. The concave
% functions like log and power law with gamma<1 are also good for this purpose.  
c1=5*255/log(2); c2=3*255; gamma =0.8;

I_log = ContrastLog (I,c1);
I_pow = ContrastPow (I,c2,gamma);

hist_orig = imhist(I);
hist_log = imhist(I_log);
hist_pow = imhist(I_pow);



figure
subplot(2,3,1)
imshow(I)
title("Original im")

subplot(2,3,2)
imshow(I_log)
title("log compr im")


subplot(2,3,3)
imshow(I_pow)
title("power-law compr im")



subplot(2,3,4)
bar(hist_orig);

subplot(2,3,5)
bar(hist_log);

subplot(2,3,6)
bar(hist_pow);



function Icontrast = ContrastLog(I,c)

Icontrast = zeros(size(I));
sz = size(I);
for rows= 1:sz(1)
    for cols = 1:sz(2)
        r = double( I(rows, cols))/255;
        Icontrast(rows, cols) = int32(c*log(1 + r )); 
    end    
end 
Icontrast = uint8(Icontrast);
end



function Icontrast = ContrastPow(I,c,gamma)

Icontrast = zeros(size(I));
sz = size(I);
for rows= 1:sz(1)
    for cols = 1:sz(2)
        r = double( I(rows, cols))/255;
        Icontrast(rows, cols) = int32(c*r^gamma); 
    end    
end 
Icontrast = uint8(Icontrast);
end


function counts_self  = hist_self(I)
counts_self = [];
for ind= 0:255    
    counts_self = [counts_self sum(sum(I == ind))];        
end
end 
