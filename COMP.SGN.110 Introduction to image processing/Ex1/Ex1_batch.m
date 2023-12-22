function Ex1_batch
for n=1:6
    fname = strcat("c_", num2str(n), ".jpg");  
    I =imread(fname);
    I1=Process(I);
    fname = strcat("c_", num2str(n), ".bmp");  
    imwrite(I1,fname, "bmp");
end
end

function I1=Process(I) 
imresize(I,0.75);
s=size(I);
s_x = s(2);
I1= I (:, s_x/2:s_x,:);
I1=fliplr(I1);
I1=imrotate(I1,90);
end