function I_eq = histequal(I)
I_eq = I;
    counts_self = [];
    for ind= 0:255    
        counts_self = [counts_self sum(sum(I == ind))];        
    end

    
    sz = size(I);
        s = double(cumsum(counts_self)/(sz(1)*sz(2)));

for rows= 1:sz(1)
    for cols = 1:sz(2)

 r =  uint8(I(rows, cols)) +1;
 %uint8( floor ( double(255.0)*s(r) +0.5)  )
 %s(r)
 %pause
         I_eq(rows, cols) = uint8( floor ( double(255.0)*s(r) +0.5)  );


    end
end

    I_eq = uint8(I_eq);

end