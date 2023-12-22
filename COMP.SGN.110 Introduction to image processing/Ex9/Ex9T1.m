% Exercise 9, Task 1 Mikhail Silaev
close all
clear all

%a
I = imread("fruits.jpg");
Ieq = I;

Ieq(:,:,1) = histeq(I(:,:,1));
Ieq(:,:,2) = histeq(I(:,:,2));
Ieq(:,:,3) = histeq(I(:,:,3));

figure
subplot(1,2,1)
imshow(I);
title("Original")

subplot(1,2,2)
imshow(Ieq);
title("RGB channels eq")

%c

Ifr = imread("fruits.jpg");
Ifest = imread("festia.jpg")

IfrHISTeq = Ifr;
IfestHISTeq = Ifest;

IfrHISTeq(:,:,1) = histeq(Ifr(:,:,1));
IfrHISTeq(:,:,2) = histeq(Ifr(:,:,2));
IfrHISTeq(:,:,3) = histeq(Ifr(:,:,3));

IfestHISTeq(:,:,1) = histeq(Ifest(:,:,1));
IfestHISTeq(:,:,2) = histeq(Ifest(:,:,2));
IfestHISTeq(:,:,3) = histeq(Ifest(:,:,3));

IfrINTeq = intensityeq(Ifr);
IfestINTeq = intensityeq(Ifest);

figure

subplot(3,2,1)
imshow(Ifr);
title("Original")

subplot(3,2,2)
imshow(Ifest);
title("Original")

subplot(3,2,3)
imshow(IfrHISTeq);
title("RGB channels eq")

subplot(3,2,4)
imshow(IfestHISTeq);
title("RGB channels eq")


subplot(3,2,5)
imshow(IfrINTeq);
title("HSV eq")


subplot(3,2,6)
imshow(IfestINTeq);
title("HSV eq")


