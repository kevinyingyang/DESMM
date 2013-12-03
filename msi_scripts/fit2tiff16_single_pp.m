function fit2tiff16_single_pp(img_fname,output_fname,WB,debug);

display([output_fname]);

img       = fitsread(img_fname);

if (debug == 1)
    display(['img [min,max] - [',num2str(min(min(img))),',',num2str(max(max(img))),']']);
    display(['WB  [min,max] - [',num2str(min(min(WB))),',',num2str(max(max(WB))),']']);
end;

img       = img ./ WB;

if (debug == 1)
    display(['img [min,max] - [',num2str(min(min(img))),',',num2str(max(max(img))),']']);
end;

img       = img .* 65535;

if (debug == 1)
    display(['img [min,max] - [',num2str(min(min(img))),',',num2str(max(max(img))),']']);
end;

img       = uint16(img);

if (debug == 1)
    display(['img [min,max] - [',num2str(min(min(img))),',',num2str(max(max(img))),']']);
end;

imwrite(img,output_fname,'tif','Compression','none');