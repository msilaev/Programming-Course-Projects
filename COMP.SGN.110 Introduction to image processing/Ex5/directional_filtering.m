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
dir_kernel (sz_fil+1 -i, i) = 1/sz_fil ;
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