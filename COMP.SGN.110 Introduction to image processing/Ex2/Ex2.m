% part 1
clear all
close all

% 1.c
A = 0:255;
step = 256/4;
QA1 = quant(A, step);

partition = step:step:256-step;
codebook = step/2: step: 256-step/2;
[indx, QA2] = quantiz(A, partition, codebook);

% 1.d
unique(QA1)
unique(QA2)

% part 2
% 2.a
I = imread("lena_face.png");

%2.b
sz = size(I);
quantization_levels = [128, 64, 32, 16, 8, 4];

quantized_images = cell(1, length(quantization_levels));

for i= 1: length(quantization_levels)
    step = 256/quantization_levels(i);

    partition = step:step:256-step;
    codebook = step/2: step: 256-step/2;

    [indx, quantized_images{i}] = quantiz(I(:), partition, codebook);
    quantized_images{i} = reshape(quantized_images{i} , sz);

end

figure;
for i = 1:length(quantization_levels)
    subplot(2, 3, i);
    imshow(uint8(quantized_images{i}));
    title(['Q Im (', num2str(quantization_levels(i)), '-level)']);
end

% 2.c
X = rand(sz);
k=4;
I_noise = double(I) + 20*X;
step = 256/quantization_levels(k);

partition = step:step:256-step;
codebook = step/2: step: 256-step/2;

quantized_images = cell(1, 2);
[indx, quantized_images{1}] = quantiz(I(:), partition, codebook);
[indx, quantized_images{2}] = quantiz(I_noise(:), partition, codebook);

quantized_images{1} = reshape(quantized_images{1} , sz);
quantized_images{2} = reshape(quantized_images{2} , sz);

tit =[strcat("Q Im ", num2str(quantization_levels(k))), "- level", strcat("noisy Q Im", num2str(quantization_levels(k)), "- level") ];
figure;
for i = 1:2
    subplot(1, 2, i);
    imshow(uint8(quantized_images{i}));
    title(tit(i));
end


pause

% part 3
close all
clear all

S_outer = ones(400,400);
S_outer (200-80: 200+80, 200-80: 200+80) =0;
S_inner = zeros(400,400);
S_inner (200-80: 200+80, 200-80: 200+80) =1;

S_left = 63*S_outer + 127*S_inner; 
S_right = 223*S_outer + 127*S_inner; 

S_tot = [uint8(S_left), uint8(S_right)];
imshow(S_tot);

pause 
close all
% try: reduce intencity
figure
level_r = 127;
for level_l = 127:  floor((63-127)/10): 63
S_left = 63*S_outer + level_l*S_inner; 
S_right = 223*S_outer + level_r*S_inner; 

S_tot = [uint8(S_left), uint8(S_right)];
imshow(S_tot);
title(strcat("Levels: ", num2str(level_l), ", ", num2str(level_r)));

pause
end

% try: increase intencity 
close all
level_l=127;
figure
for level_r = 127: floor((-127)/10): 0
S_left = 63*S_outer + level_l*S_inner; 
S_right = 223*S_outer + level_r*S_inner; 

S_tot = [uint8(S_left), uint8(S_right)];
imshow(S_tot);
title( strcat ("Levels: ", num2str(level_l), ", " , num2str(level_r)));
pause
end