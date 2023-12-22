%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function Imed= med_weighted_filter(I,s)
Imed= zeros(size(I));

sz= size(I);
sz_x = sz(1);
sz_y = sz(2);

for i_y= 1:sz_y

    for i_x = 1:sz_x
                
        Neighbours = I( max(i_y- s, 1) : min(i_y + s, sz_y), max(i_x - s, 1): min(i_x + s, sz_x)) ;       
        N1D = Neighbours(:);
        N1D = [N1D; Neighbours(3,3); Neighbours(3,3); Neighbours(3,3); Neighbours(3,3)];

        Imed(i_y, i_x) = floor(median(N1D));
        
    end
end

Imed =  uint8(Imed );

end
