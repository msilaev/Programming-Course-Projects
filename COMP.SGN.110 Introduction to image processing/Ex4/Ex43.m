% Exercise 4, Task 3 Mikhail Silaev
% Histogram matching

clear all
close all

Icorel = imread("corel.png");
Ichurch = imread("church.png");


Ichurch_corr = hismatch ( Icorel, Ichurch) ;

figure
subplot(1,2,1)
imshow(Ichurch)
title("Original im")

subplot(1,2,2)
imshow(Ichurch_corr)
title("Histogram matched im")

figure
subplot(1,2,1)

 hist_moon_eq = cumsum(imhist(Ichurch_corr));
 bar(hist_moon_eq);
 title("Matched CDF of church im")

% 
 subplot(1,2,2)
 hist_moon_eq = cumsum(imhist(Icorel));

 bar(hist_moon_eq);
 title("CDF of corel im")

% 



  













