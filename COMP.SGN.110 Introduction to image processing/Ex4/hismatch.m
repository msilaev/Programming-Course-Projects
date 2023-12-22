function I_eq = hismatch(Isourse,Itarget)
I_eq = Itarget;

    counts_sourse = [];
    for ind= 0:255    
        counts_sourse = [counts_sourse sum(sum(Isourse == ind))];        
    end

    counts_target = [];
    for ind= 0:255    
        counts_target = [counts_target sum(sum(Itarget == ind))];        
    end
   
    sz = size(Isourse);        
    s = double(cumsum(counts_sourse)/(sz(1)*sz(2)));

    tz = size(Itarget);        
    t = double(cumsum(counts_target)/(tz(1)*tz(2)));

for rows= 1:tz(1)
    for cols = 1:tz(2)

 r =  uint8(Itarget(rows, cols)) +1;

 % this is inverse operation
 [val, ind] = min( (s -t(r)).^2 ); 
 I_eq(rows, cols) = uint8(ind);

    end
end

    I_eq = uint8(I_eq);

end