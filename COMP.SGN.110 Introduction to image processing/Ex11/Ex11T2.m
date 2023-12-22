rgbImage = imread('lena.tiff');

ycbcrImage = rgb2ycbcr(rgbImage);

subplot(1, 3, 1);
imshow(ycbcrImage(:,:,1)); 
title('Y Component');

subplot(1, 3, 2);
imshow(ycbcrImage(:,:,2)); 
title('Cb Component');

subplot(1, 3, 3);
imshow(ycbcrImage(:,:,3)); 
title('Cr Component');

% 4:2:2 subsampling
cb_422 = ycbcrImage(:, 1:2:end, 2); 
cr_422 = ycbcrImage(:, 1:2:end, 3);

% 4:1:1 subsampling
cb_411 = ycbcrImage(1:4:end, 1:4:end, 2); 
cr_411 = ycbcrImage(1:4:end, 1:4:end, 3);

% 4:2:0 subsampling
cb_420 = ycbcrImage(1:2:end, 1:2:end, 2); % Cb component 4:2:0
cr_420 = ycbcrImage(1:2:end, 1:2:end, 3); % Cr component 4:2:0

% Perform subsampling on luminance component (4:1:1)
y_411 = ycbcrImage(1:4:end, 1:4:end, 1); % Y component 4:1:1

% Upsample the components back to original resolution using imresize
cb_422_up = imresize(cb_422, size(ycbcrImage(:,:,1))); % Upsample Cb 4:2:2
cr_422_up = imresize(cr_422, size(ycbcrImage(:,:,1))); % Upsample Cr 4:2:2

cb_411_up = imresize(cb_411, size(ycbcrImage(:,:,1))); % Upsample Cb 4:1:1
cr_411_up = imresize(cr_411, size(ycbcrImage(:,:,1))); % Upsample Cr 4:1:1

cb_420_up = imresize(cb_420, size(ycbcrImage(:,:,1))); % Upsample Cb 4:2:0
cr_420_up = imresize(cr_420, size(ycbcrImage(:,:,1))); % Upsample Cr 4:2:0

y_411_up = imresize(y_411, size(ycbcrImage(:,:,1))); % Upsample Y 4:1:1

% Recombine the subsampled components
ycbcrImage_422 = cat(3, y_411_up, cb_422_up, cr_422_up); % Combine subsampled components for 4:2:2
ycbcrImage_411 = cat(3, y_411_up, cb_411_up, cr_411_up); % Combine subsampled components for 4:1:1
ycbcrImage_420 = cat(3, y_411_up, cb_420_up, cr_420_up); % Combine subsampled components for 4:2:0

% Convert the subsampled images back to RGB
rgbImage_422 = ycbcr2rgb(ycbcrImage_422); % Convert to RGB for 4:2:2
rgbImage_411 = ycbcr2rgb(ycbcrImage_411); % Convert to RGB for 4:1:1
rgbImage_420 = ycbcr2rgb(ycbcrImage_420); % Convert to RGB for 4:2:0

% Display the original and subsampled images together on a subplot
figure;
subplot(2, 3, 1);
imshow(rgbImage);
title('Original RGB Image');

subplot(2, 3, 2);
imshow(rgbImage_422);
title('4:2:2 Subsampled RGB Image');

subplot(2, 3, 3);
imshow(rgbImage_411);
title('4:1:1 Subsampled RGB Image');

subplot(2, 3, 4);
imshow(rgbImage_420);
title('4:2:0 Subsampled RGB Image');

% Calculate mean squared error values (MSE) of subsampled images with respect to the original
mse_422 = immse(rgbImage, rgbImage_422); % MSE for 4:2:2
mse_411 = immse(rgbImage, rgbImage_411); % MSE for 4:1:1
mse_420 = immse(rgbImage, rgbImage_420); % MSE for 4:2:0

% Display MSE values
disp(['MSE for 4:2:2 subsampling: ', num2str(mse_422)]);
disp(['MSE for 4:1:1 subsampling: ', num2str(mse_411)]);
disp(['MSE for 4:2:0 subsampling: ', num2str(mse_420)]);