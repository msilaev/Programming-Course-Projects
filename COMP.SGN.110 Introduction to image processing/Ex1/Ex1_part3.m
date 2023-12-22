I = [0, 0, 0, 0, 0, 0, 0, 1, 1, 0; 
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1 ; 
    1, 0, 0, 1, 0, 1, 1, 0, 0, 0;
    0, 0, 1, 1, 1, 0, 0, 0, 0, 0;
    0, 0, 1, 1, 1, 0, 0, 1, 1, 1];

figure
imshow(I)

sz = size(I);
sz_y = sz(1);
sz_x = sz(2);

S1= I(1:sz_y-1, 2: floor(sz_x/2) );
S2= I(1:sz_y-1,  floor(sz_x/2)+1 : sz_x-1 );


fid = fopen('res.txt','w');
fprintf(fid, '%d %d', num(S1), num(S2));
fclose(fid);
 
load S.mat
num(S)

function num = num(S)

num=sum(sum(S));

end 





