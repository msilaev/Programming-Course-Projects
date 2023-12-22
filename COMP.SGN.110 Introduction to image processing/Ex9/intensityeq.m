%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function Ieq_rgb= intensityeq(I)

Ihsv = rgb2hsv(I);

Ieq = Ihsv;

Ieq(:,:,1) = histeq(Ihsv(:,:,1));
Ieq(:,:,2) = histeq(Ihsv(:,:,2));
Ieq(:,:,3) = histeq(Ihsv(:,:,3));

Ieq_rgb = hsv2rgb(Ieq);
