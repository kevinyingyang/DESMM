function process_all;

%per-pixel
spectralon_pp_dir        = '/home/ruggero/Documents/yale/dataset/multispectral/Oxford/02_tiff16/spectralon/per-pixel/standard';
spectralon_pp_fname_base = 'spectralon-pp';
spectralon_dir           = '/home/ruggero/Documents/yale/dataset/multispectral/Oxford/02_tiff16/spectralon/single';
spectralon_fname         = 'spectralon-003';
data_dir = '/home/ruggero/Documents/yale/dataset/multispectral/Oxford/02_tiff16/data/bodley-861';
light_angle = 45;
debug = 0;%[0,1] - 0 means no debug - 1 means debug
fit2tiff16_pp(spectralon_pp_dir,spectralon_pp_fname_base,spectralon_dir,spectralon_fname,data_dir,light_angle,debug);

%single
%spectralon_fname_base ='/home/ruggero/Documents/yale/dataset/multispectral/Ellesmere/per-pixel_calibration/spectralon/spectralon-001F';%not WB
%basedir = '/home/ruggero/Documents/yale/dataset/multispectral/Ellesmere/per-pixel_calibration/data';
%fit2tiff16(basedir,spectralon_fname_base);

return;