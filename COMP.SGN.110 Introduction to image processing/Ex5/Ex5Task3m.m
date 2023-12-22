% Exercise 5, Task 3 Mikhail Silaev

close all
clear all

% Part a
I = imread("miranda1.tif") ;
n_level =20;
sz = size(I);
sz_x = sz(2);
sz_y = sz(1);

noise = zeros(sz);
noise100 = randi([1, n_level], 100, 100);
noise (floor(sz_y/2) -50 : floor(sz_y/2) +49, floor(sz_x/2) -50 : floor(sz_x/2) +49 ) = noise100;
I_noise = imadd(I, uint8(noise));


%Part b
figure
subplot(2,3,1)
imshow(I_noise)
title( "Noisy image" );

subplot(2,3,2)
filter_size = 3;
imshow(med_filter(I_noise,filter_size))
title( "Median filter, size = " + num2str(filter_size) );

subplot(2,3,3)
filter_size = 5;
imshow(med_filter(I_noise,filter_size))
title( "Median filter, size = " + num2str(filter_size) );

subplot(2,3,4)
filter_size = 7;
imshow(med_filter(I_noise,filter_size))
title( "Median filter, size = " + num2str(filter_size) );

subplot(2,3,5)
filter_size = 9;
imshow(med_filter(I_noise,filter_size))
title( "Median filter, size = " + num2str(filter_size) );

subplot(2,3,6)
filter_size = 11;
imshow(med_filter(I_noise,filter_size))
title( "Median filter, size = " + num2str(filter_size) );

% Part c
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%I_noise = imread("miranda1.tif") ;
figure
subplot(1,3,1)
imshow(I_noise)
title( "Noisy image" );

subplot(1,3,2)
filter_size = 5;
alpha = n_level;
O = med_filter(I_noise,filter_size);
imshow(med_filter_threshold(I_noise,O,alpha));
title( "Median filter, size = " + num2str(filter_size) + ", threshold = " + num2str(alpha));

subplot(1,3,3)
filter_size = 5;
alpha = 1000;
O = med_filter(I_noise,filter_size);
imshow(med_filter_threshold(I_noise,O,alpha));
%title( "Median filter, size = " + num2str(filter_size) + ", threshold = " + num2str(alpha));
title( "Median filter, size = " + num2str(filter_size) + ", no threshold ");


% Conlcusion: threshold helps to reduce the noise while keeping the details
% in the rest of the image. The natural choise of the thresold is the noise
% amplitude. If noise amplitude is smaller than the ordinary variation of intensity
% within the image this approach seems preferable.






