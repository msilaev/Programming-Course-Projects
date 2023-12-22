%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function Imed= med_filter_threshold(I,O,alpha)
Imed= O;

sz= size(I);
sz_x = sz(1);
sz_y = sz(2);

for i_y= 1:sz_y
    for i_x = 1:sz_x         
        
        %Neighbours = I( max(i_y-s, 1) : min(i_y+s, sz_y), max(i_x-s, 1): min(i_x+s, sz_x)) ;    
        %abs( double(I(i_y, i_x )) - double(O(i_y, i_x)))
        
        
        %pause
        if (abs( double(I(i_y, i_x )) - double(O(i_y, i_x))) > alpha)
            Imed(i_y, i_x) = I(i_y, i_x) ;         
        end   
    end
end

Imed =  uint8(Imed );

end
