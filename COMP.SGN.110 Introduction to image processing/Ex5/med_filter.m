%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function Imed= med_filter(I,s)
Imed= zeros(size(I));

sz= size(I);
sz_x = sz(1);
sz_y = sz(2);

for i_y= 1:sz_y
    for i_x = 1:sz_x
                
        Neighbours = I( max(i_y- floor(s/2), 1) : min(i_y + floor(s/2), sz_y), max(i_x - floor(s/2), 1): min(i_x + floor(s/2), sz_x)) ;
        
        %if ( abs( Imed(i_y, i_x ) - median(Neighbours(:)) ) > alpha )
        %Imed(i_y, i_x ) = I(i_y, i_x) ; 
        %else
             Imed(i_y, i_x) = floor(median(Neighbours(:)));
        %end   
    end
end

Imed =  uint8(Imed );

end
