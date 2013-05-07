function A = acf(image,flag);

S = size(image);
height   = S(1);
width    = S(2);

SS = size(S);
if (SS(2) > 2) 
    display "ERROR - image must be 1-channel"
end

image = double(image);

image_fft = fft2(image);

image_fft = image_fft .* conj(image_fft);

A = ifftshift(ifft2(image_fft));

if true(flag)
    A = (A - min(min(A))) / (max(max(A))-min(min(A)));
end

return