clear all
close all
I = imread("lena.tiff");

Ihsv = rgb2hsi(I);

figure
subplot(3,2,1)
imshow(I(:,:,1))
title("red")

subplot(3,2,3)
imshow(I(:,:,2))
title("green")

subplot(3,2,5)
imshow(I(:,:,3))
title("blue")
%%%%%%%%%%%%%%%

subplot(3,2,2)
imshow(Ihsv(:,:,1))
title("hue")

subplot(3,2,4)
imshow(Ihsv(:,:,2))
title("saturation")

subplot(3,2,6)
imshow(Ihsv(:,:,3))
title("intensity")

noisyGreenCh = imnoise(I(:,:,2), 'gaussian', 0, 0.1); % Adjust noise parameters as needed
noisyGreenIm = cat(3, I(:,:,1), noisyGreenCh, I(:,:,3));
noisyGreenImHSV =rgb2hsi(noisyGreenIm);

figure
subplot(2,2,1)
imshow(noisyGreenIm)
%imshow(I)
title("noisy green channel")

subplot(2,2,2)
imshow(noisyGreenImHSV(:,:,1))
title("hue")

subplot(2,2,3)
imshow(noisyGreenImHSV(:,:,2))
title("saturation")

subplot(2,2,4)
imshow(noisyGreenImHSV(:,:,3))
title("intensity")

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
noisyGreenCh = imnoise(I(:,:,2), 'gaussian', 0, 0.1); % Adjust noise parameters as needed
noisyRedCh = imnoise(I(:,:,1), 'gaussian', 0, 0.1); % Adjust noise parameters as needed
noisyBlueCh = imnoise(I(:,:,3), 'gaussian', 0, 0.1); % Adjust noise parameters as needed

noisyRGBIm = cat(3, noisyRedCh, noisyGreenCh, noisyBlueCh);
noisyHSV =rgb2hsi(noisyRGBIm);

figure
subplot(2,2,1)
imshow(noisyRGBIm)
title("noisy all channels")
%imshow(I)

subplot(2,2,2)
imshow(noisyHSV(:,:,1))
title("noisy H channel")

subplot(2,2,3)
imshow(noisyHSV(:,:,2))
title("noisy S channel")

subplot(2,2,4)
imshow(noisyHSV(:,:,3))
title("noisy V channel")

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
meanFilter = fspecial('average', 5);
filtNoisyGreenCh = imfilter(noisyGreenCh, meanFilter, 'replicate'); % 'replicate' pads the image borders
filtNoisyRedCh = imfilter(noisyRedCh, meanFilter, 'replicate'); % 'replicate' pads the image borders
filtNoisyBlueCh = imfilter(noisyBlueCh, meanFilter, 'replicate'); % 'replicate' pads the image borders

figure
subplot(2,2,1)
imshow( cat(3, noisyRedCh, filtNoisyGreenCh,  noisyBlueCh) )
title("filt red")
subplot(2,2,2)
imshow( cat(3, filtNoisyRedCh, noisyGreenCh,  noisyBlueCh) )
title("filt green")

subplot(2,2,3)
imshow( cat(3, noisyRedCh, noisyGreenCh,  filtNoisyBlueCh) )
title("filt blue")

subplot(2,2,4)
imshow( cat(3, filtNoisyRedCh, filtNoisyGreenCh,  filtNoisyBlueCh) )
title("filt all")

%%%%%%%%%%%%%%%%%%%%

filtNoisyHCh = imfilter(noisyHSV(:,:,1), meanFilter, 'replicate'); % 'replicate' pads the image borders
filtNoisySCh = imfilter(noisyHSV(:,:,2), meanFilter, 'replicate'); % 'replicate' pads the image borders
filtNoisyVCh = imfilter(noisyHSV(:,:,3), meanFilter, 'replicate'); % 'replicate' pads the image borders

figure
subplot(2,2,1)
imshow( hsi2rgb( cat(3, filtNoisyHCh, noisyHSV(:,:,2), noisyHSV(:,:,3)) ))
title("filt hue")

subplot(2,2,2)
imshow( hsi2rgb( cat(3, noisyHSV(:,:,1), filtNoisySCh,  noisyHSV(:,:,3))) )
title("flit saturation")

subplot(2,2,3)
imshow( hsi2rgb(cat(3, noisyHSV(:,:,1), noisyHSV(:,:,3),  filtNoisyVCh)) )
title("flit intensity")

subplot(2,2,4)
imshow(hsi2rgb( cat(3, filtNoisyHCh, filtNoisySCh,  filtNoisyVCh) ))
title("flit all")
