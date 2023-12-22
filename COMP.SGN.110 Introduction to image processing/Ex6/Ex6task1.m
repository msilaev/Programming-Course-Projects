% Exercise 6, Task 1 Mikhail Silaev
close all
clear all

%a
S_a = ones(128,128)/2;

%b
sz_x=128;
sz_y=128;
S_b = zeros(128,128);
S_b(floor(sz_y/2)-10: floor(sz_y/2)+10, floor(sz_x/2)-10: floor(sz_x/2)+10) =256;
%imshow(S_b)

%c
S_b=repmat((0:127)/127, 128,1);
%imshow(S_b)

%d
S_b = zeros(128,128);
S_b(floor(sz_y/2): floor(sz_y/2)+1, floor(sz_x/2): floor(sz_x/2)+1) =1/4;

%e
[x,y] = meshgrid(1:128,1:128);
S_b = cos(8*pi*x/128) + cos( 8*pi*y/128);
%contourf(x,y,S_b)






