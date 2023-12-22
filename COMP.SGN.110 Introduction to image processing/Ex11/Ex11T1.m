
close all
clear all
load('yuvdata.mat');


% rows × cols suggest that the yy component contains luminance information for every pixel in the image (360 × 640 = 230400 pixels). 
%Meanwhile, the uu and vv components, representing the chrominance, usually have a lower resolution 
% and contain color information subsampled from the luminance component.

% Reshape yy to image size
yy_reshaped = reshape(yy, cols, rows)';

% Upsample uu and vv to match image size using imresize
uu_upsampled = imresize(reshape(uu,  [cols/2, rows/2])', [rows, cols] );
vv_upsampled = imresize(reshape(vv,  [ cols/2, rows/2])', [rows, cols] );

% Display the components in a 1x3 subplot
figure
subplot(1, 3, 1);
imshow(yy_reshaped, []);
title('Y Component');

subplot(1, 3, 2);
imshow(uu_upsampled, []);
title('U Component');

subplot(1, 3, 3);
imshow(vv_upsampled, []);
title('V Component');


figure
YuvToRgb = [ 1, 0, 1.402 ; 1, -0.34413, -0.71414; 1, 1.1772, 0  ];

% Center the U and V components around zero by subtracting 127
u_centered = uu_upsampled -127;
v_centered = vv_upsampled -127;

% Flatten and concatenate the components for conversion to RGB
YUV = cat(2, yy_reshaped(:), u_centered(:), u_centered(:));

% Perform the transformation from YUV to RGB with the matrix given above (YuvToRgb)
RGB = YuvToRgb * YUV';

% Reshape each component back to the image size
R = reshape(RGB(1,:), rows, cols);
G = reshape(RGB(2,:), rows, cols);
B = reshape(RGB(3,:), rows, cols);

% Combine R, G, B components to form RGB image
RGB_image = uint8(cat(3, R, G, B));

% Show the RGB image
imshow(  RGB_image);