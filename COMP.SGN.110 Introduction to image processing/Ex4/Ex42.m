% Exercise 4, Task 2 Mikhail Silaev

clear all
close all
%(b)
%(moon.png, house.png, spine.jpg, church.png

Imoon = imread("moon.png");
Ihouse = imread("house.png");
Ispine = imread("spine.jpg");
Ichirch = imread("church.png");

Imoon_eq = histequal (Imoon) ;
Ihouse_eq = histequal (Ihouse) ;
Ispine_eq = histequal (Ispine) ;
Ichirch_eq = histequal (Ichirch) ;



figure
subplot(2,4,1)
imshow(Imoon)
title("Original")
subplot(2,4,2)
imshow(Ihouse)
subplot(2,4,3)
imshow(Ispine)
subplot(2,4,4)
imshow(Ichirch)

subplot(2,4,5)
imshow(Imoon_eq)
title("Using histequal")
subplot(2,4,6)
imshow(Ihouse_eq)
subplot(2,4,7)
imshow(Ispine_eq)
subplot(2,4,8)
imshow(Ichirch_eq)



figure
subplot(2,4,1)
hist_moon_eq = imhist(Imoon);
bar(hist_moon_eq);
title("Original")


subplot(2,4,2)
hist_moon_eq = imhist(Ihouse);
bar(hist_moon_eq);

subplot(2,4,3)
hist_moon_eq = imhist(Ispine);
bar(hist_moon_eq);

subplot(2,4,4)
hist_moon_eq = imhist(Ichirch);
bar(hist_moon_eq);


subplot(2,4,5)
hist_moon_eq = imhist(Imoon_eq);
bar(hist_moon_eq);
title("Using histequal")


subplot(2,4,6)
hist_moon_eq = imhist(Ihouse_eq);
bar(hist_moon_eq);

subplot(2,4,7)
hist_moon_eq = imhist(Ispine_eq);
bar(hist_moon_eq);

subplot(2,4,8)
hist_moon_eq = imhist(Ichirch_eq);
bar(hist_moon_eq);


 %ContrastStretch (Imoon, 0, 255) ;
 % Image obtained by equalization is much better than that obtained by
 % contrast stretch.
 
 %ContrastStretch (Ihouse, 0, 255) ;
%ContrastStretch (Ispine, 0, 255) ;
%ContrastStretch (Ichirch, 0, 255) ;











