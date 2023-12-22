% Exercise 3, Task 1 Mikhail Silaev

clear all
close all
%(a)
I = imread("wom1.PNG");
cq= I*10;
counts_self = [];
for ind= 0:255
    counts_self = [counts_self sum(sum(I == ind))];        
end

%(b)
I_stretched = ContrastStretch (I);

% (c)
wom = imread("wom1.PNG");
man = imread("man8.PNG");

wom_str = ContrastStretch(wom);
man_str = ContrastStretch(man);

hist_wom = hist_self(wom);
hist_wom_str = hist_self(wom_str);

hist_cq = hist_self(cq);


hist_man = hist_self(man);
hist_man_str =  hist_self(man_str);
bin_edges = 0:255;

% Create a bar chart using the bar function

 


figure
subplot(2,4,1)
imshow(man)

subplot(2,4,2)
imshow(wom)

subplot(2,4,3)
%plot(hist_man)
bar(bin_edges, hist_man);


subplot(2,4,4)
%plot(hist_wom)
bar(bin_edges, hist_wom);

subplot(2,4,5)
imshow(man_str)

subplot(2,4,6)
imshow(wom_str)

subplot(2,4,7)
%plot(hist_man_str)
bar(bin_edges, hist_man_str);


subplot(2,4,8)
%plot(hist_wom_str)
bar(bin_edges,hist_wom_str);


function Icontrast = ContrastStretch(I)

val = unique(I);
min_val =double( min(val));
max_val= double(max(val));
Icontrast = zeros(size(I));
sz = size(I);
for rows= 1:sz(1)
    for cols = 1:sz(2)
Icontrast(rows, cols) = ceil( 255* double( I(rows, cols) -min_val )/( max_val - min_val ) )  ;
%Icontrast(rows, cols)

%x=(255* ( I(rows, cols) -min_val )/( max_val - min_val ) )

    end    
end 
Icontrast = uint8(Icontrast);
end


function counts_self  = hist_self(I)
counts_self = [];
for ind= 0:255    
    counts_self = [counts_self sum(sum(I == ind))];        
end

end 


% Compute and display the histogram with 256 bins
% num_bins = 256;
% [counts, bins] = imhist(I, num_bins);
% imhist(I, num_bins);
% 
%  figure 
% plot(counts- hist')
