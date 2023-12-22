function ContrastStretch(im, lo, hi)
im = double(im);
immax = max(max(im));
immin = min(min(im));
im1 = (im - immin)*(hi-lo)/(immax-immin)+lo;

figure;
subplot(221), imshow(uint8(im)), title('Original Image');
subplot(222), imhist(uint8(im), gray(256)), title('Histogram');
subplot(223), imshow(uint8(im1)), title('Cont. Stretched Image');
subplot(224), imhist(uint8(im1), gray(256)), title('Histogram');

end