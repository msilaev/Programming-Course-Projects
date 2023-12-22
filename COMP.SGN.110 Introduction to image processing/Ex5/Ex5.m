% Exercise 5, Task 2 Mikhail Silaev

clear all
close all

I = imread("cameraman.tif");

% a
n_level =20;
sz = size(I);
sz_x = sz(2);
sz_y = sz(1);

noise = zeros(sz);
noise100 = randi([1, n_level], 100, 100);
noise (floor(sz_y/2) -50 : floor(sz_y/2) +49, floor(sz_x/2) -50 : floor(sz_x/2) +49 ) = noise100;
I_noise = imadd(I, uint8(noise));

figure

subplot(1,2,1)
imshow(I)
title( "Original" )

subplot(1,2,2)
imshow(I_noise)
title( "Noise level: "+ num2str(n_level) )

% b
filter_size = 3;
filtred_images = directional_filtering(I_noise,filter_size);

figure
subplot(2,2,1)
imshow(filtred_images{1})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(0) );

subplot(2,2,2)
imshow(filtred_images{2})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(45) );

subplot(2,2,3)
imshow(filtred_images{3})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(90) );

subplot(2,2,4)
imshow(filtred_images{4})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(135) );

%%%%%%%%%%%%%%
filter_size = 5;
filtred_images = directional_filtering(I_noise,filter_size);

figure
subplot(2,2,1)
imshow(filtred_images{1})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(0) );

subplot(2,2,2)
imshow(filtred_images{2})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(45) );

subplot(2,2,3)
imshow(filtred_images{3})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(90) );

subplot(2,2,4)
imshow(filtred_images{4})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(135) );

%%%%%%%%%%%%%%%%%%%%%55
filter_size = 7;
filtred_images = directional_filtering(I_noise,filter_size);

figure
subplot(2,2,1)
imshow(filtred_images{1})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(0) );

subplot(2,2,2)
imshow(filtred_images{2})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(45) );

subplot(2,2,3)
imshow(filtred_images{3})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(90) );

subplot(2,2,4)
imshow(filtred_images{4})
title( "Dirictional filter, size = " + num2str(filter_size) + ", dir = " + num2str(135) );

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function filtred_images = directional_filtering(I,s)

filtred_images = cell(1, 4);

dir_kernel =  zeros(s);
sz_fil = size(dir_kernel);
sz_fil = sz_fil(1);

for i=1:sz_fil
dir_kernel ( ceil(sz_fil/2), i) = 1/sz_fil ;
end
filtred_images{1} =  imfilter(I, dir_kernel);

dir_kernel =  zeros(s);
for i=1:sz_fil
dir_kernel (sz_fil+1 -1, i) = 1/sz_fil ;
end
filtred_images{2} =  uint8( imfilter(I, dir_kernel) );


dir_kernel =  zeros(s);
for i=1:sz_fil
    dir_kernel (  i , ceil(sz_fil/2)) = 1/sz_fil;
end
filtred_images{3} =  imfilter(I, dir_kernel);

dir_kernel =  zeros(s);
for i=1:sz_fil
    dir_kernel (  i , i) = 1/sz_fil;
end
filtred_images{4} =  imfilter(I, dir_kernel);

end


