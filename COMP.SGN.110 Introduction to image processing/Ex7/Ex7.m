% Exercise 7, Task 1 Mikhail Silaev
close all
clear all

I = imread("lena.jpg");

Ign = imnoise(I, "gaussian", 60/255, 400/(255)^2);

Isp = imnoise(I,'salt & pepper',0.1) ;

sz = size(I);
height = sz(1);
width = sz(2);

a= 0;
b = 1;
noise = a + sqrt(-b*log((1-rand(width, height))));
Irl = imadd((I), uint8(50*noise));

figure

subplot(2,2,1)
imshow(I)
title( "Original" )

subplot(2,2,2)
imshow(Ign)
title( "Gaussian" )

subplot(2,2,3)
imshow(Isp)
title( "Salt pepper" )

subplot(2,2,4)
imshow(Irl)
title( "Rayleight" )

% Exercise 7, Task 2 Mikhail Silaev

m=5;
n=5;
% a
AFgn = imfilter(Ign,fspecial('average',[m n])); 
AFsp = imfilter(Isp,fspecial('average',[m n]));
AFrl = imfilter(Irl,fspecial('average',[m n]));

% b
GFgn = exp(imfilter(log(im2double(Ign)),ones(m,n),'replicate')).^(1/(m*n)) ;
GFsp = exp(imfilter(log( im2double(Isp) ),ones(m,n),'replicate')).^(1/(m*n)) ;
GFrl = exp(imfilter(  log(im2double(Irl)),ones(m,n),'replicate')).^(1/(m*n)) ;

% c
HMFgn = (m*n) ./ imfilter(1 ./ (im2double(Ign) + eps), ones(m, n), 'replicate'); 
HMFsp = (m*n) ./ imfilter(1 ./ (im2double(Isp) + eps), ones(m, n), 'replicate'); 
HMFrl = (m*n) ./ imfilter(1 ./ (im2double(Irl) + eps), ones(m, n), 'replicate'); 

%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure

subplot(2,2,1)
imshow(Ign)
title( "Gaussian unfiltered" )

subplot(2,2,2)
imshow(AFgn)
title( "Arithmetic" )

subplot(2,2,3)
imshow(GFgn)
title( "Geometric" )

subplot(2,2,4)
imshow(uint8(255*HMFgn))
title( "Harmonic" )

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure

subplot(2,2,1)
imshow(Isp)
title( "Pepper sault unfiltered" )

subplot(2,2,2)
imshow(AFsp)
title( "Arithmetic" )

subplot(2,2,3)
imshow(GFsp)
title( "Geometric" )

subplot(2,2,4)
imshow(uint8(255*HMFsp))
title( "Harmonic" )

%%%%%%%%%%%%%%%%%%%%%%%%%
figure

subplot(2,2,1)
imshow(Irl)
title( "Rayleight unfiltered" )

subplot(2,2,2)
imshow(AFrl)
title( "Arithmetic" )

subplot(2,2,3)
imshow(GFsp)
title( "Geometric" )

subplot(2,2,4)
imshow(uint8(255*HMFsp))
title( "Harmonic" )


% Exercise 7, Task 3 Mikhail Silaev

Isp = imnoise(I,'salt & pepper',0.02) ;

%%%%%%%%%%%%%%%%%%%%%%%%%
figure

subplot(2,2,1)
imshow(I)
title( "Original" )

subplot(2,2,2)
imshow(Isp)
title( "Paper salt unfiltered" )

subplot(2,2,3)
imshow(med_filter(Isp,2))
title( "Median" )

subplot(2,2,4)
imshow(med_weighted_filter(Isp,2))
title( "Weighted median" )




