% Exercise 6, Task 3 Mikhail Silaev

clear all
close all

I = imread("cameraman.tif");
If = fftshift(fft2(I));

IfiltF_lp =ifftshift(BWLPfilter(If, 20, 2).*If);
res_lp = ifft2(IfiltF_lp);

IfiltF_hp = ifftshift(If-BWLPfilter(If, 20, 2).*If);
res_hp =  ifft2(IfiltF_hp);

figure
subplot(2,2,1)
imshow(uint8(real(res_lp)))

subplot(2,2,2)
imshow(1- BWLPfilter(If, 20, 2) )

subplot(2,2,3)
imshow(BWLPfilter(If, 20, 2))

subplot(2,2,4)
imshow(uint8(real(res_hp)))




