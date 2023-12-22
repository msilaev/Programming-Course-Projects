%% Exercise 3, Task 2 Mikhail Silaev

clear all
close all
%2(a)
I = imread("mbaboon.bmp");

fun = @(blockStruct)blockStruct.data(2,2);

B22 = blockproc(I,[4,4],fun) ;
figure
imshow(B22)


%2(b)
fun = @(blockStruct)blockStruct.data(1,1);
B11 = blockproc(I,[4,4],fun) ;
figure
imshow(B11)

%2(c)

fun = @(blockStruct)mean(blockStruct.data(:));

Bmean =  uint8(blockproc(I,[4,4],fun) );
figure
imshow(Bmean,'Colormap',gray(256))

function size = fun1 (blockStruct) 
size = size(blockStruct.data)
pause
end 

% It is interestin that the eye does not average over this very small 4x4
% pixel square