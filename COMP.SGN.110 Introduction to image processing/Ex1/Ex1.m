% (a)
I = imread("peppers.png");
imshow(I)
I1 = rgb2gray(I);


% (b)
Ir= I;
Ir(:,:,2) =0;
Ir(:,:,3) =0;
figure
imshow(Ir)

%(c)
I2= I;
I2(:,:,2) = I2(:,:,2) +50;

figure
imshow(I2)

%(d)
R= I(:,:,1);
G= I(:,:,2);
B= I(:,:,3);
I3 = cat(3,R,G,B);
figure
imshow(I3)

figure
subplot(2,2,1)
imshow(I)

subplot(2,2,2)
imshow(I1)

subplot(2,2,3)
imshow(I2)

subplot(2,2,4)
imshow(I3)






