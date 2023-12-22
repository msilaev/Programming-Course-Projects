clear all

% Load the YUV data from the 'yuvdata.mat' file
load('yuvdata.mat'); % Assuming your YUV data is stored in a variable named 'yuvData'

% Extract Y, U, V channels from the loaded data
% Y = yuvData(:,:,1); % Y channel
% U = yuvData(:,:,2); % U channel
% V = yuvData(:,:,3); % V channel

% Convert YUV to RGB

% Reshape YUV data to create 2D matrices
Y_matrix = reshape(yy, rows, cols);



% Display the RGB image
imshow( uint8 (Y_matrix)); % Display the RGB image in a figure window
title('YUV to RGB Image'); % Set the title of the figure