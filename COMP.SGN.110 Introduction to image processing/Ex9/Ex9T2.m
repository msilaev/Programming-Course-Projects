% Exercise 9, Task 2 Mikhail Silaev
close all
clear all

I = imread("lake.jpg");

Ihsv = rgb2hsv (I);

% filter by saturation because zntral lake appears to have highest contrast
[labelMatrix, numLabels] = bwlabel(im2bw(Ihsv(:,:,2)));
Ibw = bwlabel( im2bw(Ihsv(:,:,2)) );
Imask = zeros(size(Ibw)); 

for i = 1:size(Imask(:))
if (Ibw(i)<550 && Ibw(i)>500) 
    Imask (i) = 1;
end 
end 

Imask3D = cat(3, Imask, Imask, Imask);
Ifilt = rgb2hsv( uint8(Imask3D).*I);


% we need one more filtering because some tiny yellow parts appear to have
% same gray level as blue lake
Ibw =   bwlabel( im2bw(Ifilt(:,:,1)) );
Ifilt = uint8(cat(3, Ibw, Ibw, Ibw)).*I;

% plot the final result
figure
subplot(1,2,1)
imshow(I)

subplot(1,2,2)
imshow(Ifilt)