% Ex 3 part 4
% (a) If c>0, there will be peak at 255 with the value 
% \sum_x,y (g(x,y) + c  >= 255). Counts corresponding to g(x,y) < 255 -c will be shifted by c.   
%If c<0 there will be peak at 0 with the value \sum_x,y (g(x,y) + c <= 0). 
% Counts corresponding to g(x,y) > -c will be shifted by c.  

% (b) If c>1, there will be peak at 255 with the value 
% \sum_x,y (g(x,y) * c  >= 255). Values of beans  corresponding to counts with < 255/c will be multiplied by c. 
% In other words, the rest of histogram will be stretched. 
% If 0<c<1 the histogram will just shrink.

% (c) Rotation does not change histogram.