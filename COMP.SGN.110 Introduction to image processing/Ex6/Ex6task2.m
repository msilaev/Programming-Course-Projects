% Exercise 6, Task 2 Mikhail Silaev
close all
clear all

%a
I = ones(128,128)/2;
If = fftshift(fft2(I));
figure
z =  abs(If);
imagesc( (log(z+0.0001)))
title("a")

%b
sz_x=128;
sz_y=128;
I = zeros(128,128);
I(floor(sz_y/2)-10: floor(sz_y/2)+10, floor(sz_x/2)-10: floor(sz_x/2)+10) =256;
If = fftshift(fft2(I));
figure
z =  (abs(If));
imagesc( (log(z+0.0001)))


title("b")

%c
I=repmat((0:127)/127, 128,1);
If = fftshift(fft2(I));
figure
z =  (abs(If));
imagesc( (log(z+0.0001)))

title("c")

%d
I = zeros(128,128);
I(floor(sz_y/2): floor(sz_y/2)+1, floor(sz_x/2): floor(sz_x/2)+1) =1/4;
If = fftshift(fft2(I));
figure
z =  (abs(If));
imagesc( (log(z+0.0001)))


title("d")

%e
[x,y] = meshgrid(1:128,1:128);
I = cos(8*pi*x/128)+cos(8*pi*y/128);
If = fftshift(fft2(I));
figure
z =  (abs(If));
imagesc( (log(z+0.0001)))

title("f")


% From the FT we get the information about frequency (or more correctly momentum)
% components of the image.

% DFT of image 1 is concentrated at zero frequency because the image is
% constant







