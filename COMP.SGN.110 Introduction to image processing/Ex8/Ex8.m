% Exercise 8, Task 1 Mikhail Silaev
close all
clear all

I = imread("DIP.jpg");
sz = size(I);
row = sz(1);
col = sz(2);

b=0.1;
a = b;
T=1;
[u, v] = meshgrid(-row/2 +0.01 :row/2-1 +0.01, -col/2:col/2-1);
ab = (a*u + b*v)*pi;
H = T*sin(ab).*exp(-j*ab)./ab;

%%%%%%%%%%%%%%%%%%

I_motion =  ifft2( ifftshift ( H.*fftshift( fft2(I) ) ));
I_restored = (ifft2( ifftshift ( fftshift( fft2( real(I_motion )) )./H ) ));


%%%%%%%%%%%%%%%%%%%%%

figure

subplot(1,3,1)
imshow(I)
title( "Original" )

subplot(1,3,2)
imshow(uint8(real(I_motion)))
title( "Motion blurred" )

subplot(1,3,3)
imshow(uint8(real(I_restored)))
title( "Restored" )

fprintf('MSE motion blurred: %f\n', immse(I, uint8(real(I_motion)) ));
fprintf('MSE restored: %f\n', immse(I, uint8(real(I_restored)) ))




% Exercise 8, Task 2 Mikhail Silaev

% a-d
sz = size(I_motion);
sz_x = sz(1);
sz_y = sz(2);

I_noise =50*(randn(sz_x));

I_motion_noise = real( ifft2( ifftshift ( H.*fftshift( fft2(I) ) )) )+ I_noise;

I_restored_simle =ifft2( ifftshift ( fftshift( fft2( I_motion_noise ) )./H ) );

S_f = abs(( fftshift(  fft2(I) ) ).^2);
S_n = abs(( fftshift( fft2(I_noise) ) ).^2);

H_w = (abs(H.^2)./( abs(H.^2) + S_n./S_f) )./H; 

I_restored_W =ifft2( ifftshift ( fftshift( fft2( I_motion_noise ) ).*H_w ) );

figure

subplot(1,3,1)
imshow(uint8((I_motion_noise)))
title( "Motion blurred + noise" )

subplot(1,3,2)
imshow(uint8(real(I_restored_simle)))
title( "Simple invertion" )

subplot(1,3,3)
%imshow(uint8(real(I_restored_W)))
imshow(uint8(real(I_restored_W)))

title( "Wiener" )


% f
k=0.01;
H_w = (abs(H.^2)./( abs(H.^2) + k) )./H; 

I_restored_k_001 =ifft2( ifftshift ( fftshift( fft2( I_motion_noise ) ).*H_w ) );

k=0.1;
H_w = (abs(H.^2)./( abs(H.^2) + k) )./H; 

I_restored_k_01 =ifft2( ifftshift ( fftshift( fft2( I_motion_noise ) ).*H_w ) );

k=0.5;
H_w = (abs(H.^2)./( abs(H.^2) + k) )./H; 

I_restored_k_1 =ifft2( ifftshift ( fftshift( fft2( I_motion_noise ) ).*H_w ) );


figure

subplot(1,3,1)
imshow(uint8(real(I_restored_k_001)))
title( "restored, k=0.01" )

subplot(1,3,2)
imshow(uint8(real(I_restored_k_01)))
title( "restored, k=0.1" )

subplot(1,3,3)
%imshow(uint8(real(I_restored_W)))
imshow(uint8(real(I_restored_k_1)))

title( "restored, k=0.5" )
