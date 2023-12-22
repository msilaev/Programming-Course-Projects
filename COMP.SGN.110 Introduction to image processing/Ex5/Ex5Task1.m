% Exercise 5, Task 1 Mikhail Silaev

clear all
close all

I = imread("cameraman.tif");

% Define the Laplacian kernel
figure

subplot(2,2,1)
imshow(I)
title( "Original" )


subplot(2,2,2)
a = 8;
laplacian_kernel = [0, -1, 0; -1, a, -1; 0, -1, 0];
result = imfilter(I, laplacian_kernel);
imshow(result)
title( "Laplacian, a= "+ num2str(a) )

subplot(2,2,3)
a = 9;
laplacian_kernel = [0, -1, 0; -1, a, -1; 0, -1, 0];
result = imfilter(I, laplacian_kernel);
imshow(result)
title( "a = "+ num2str(a) )

subplot(2,2,4)
a = 9.7;
laplacian_kernel = [0, -1, 0; -1, a, -1; 0, -1, 0];
result = imfilter(I, laplacian_kernel);
imshow(result)
title( "a = "+ num2str(a) )




