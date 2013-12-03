function fit2tiff16_pp(spectralon_pp_dir,spectralon_pp_fname_base,spectralon_dir,spectralon_fname,data_dir,light_angle,debug);

row = 3326;
col = 2504;

listing_spec = dir([spectralon_pp_dir,'/*F1.fit']);
S_spec = size(listing_spec);
if (debug == 1)
    display(['Number of spectralon images - ',int2str(S_spec(1))]);
end;
dim = S_spec(1);

WB = zeros(row,col,8);

for K = 1:8;
    display(['Computing white board for band ',int2str(K)]);
    WB(:,:,K) = white_board(row,col,dim,K,spectralon_pp_dir,spectralon_pp_fname_base);
    if (debug == 1)
        output_fname = [spectralon_pp_dir,'/',spectralon_pp_fname_base,'_white_board_',int2str(K),'.tif'];
        imwrite(WB(:,:,K)./max(max(WB(:,:,K))),output_fname,'tif','Compression','none');
    end;
end;

med_vec = zeros(1,8);
spec_mask = double(imread([spectralon_dir,'/',spectralon_fname,'_mask.png']))./255;
vect = zeros(1,sum(sum(spec_mask)));
for K = 1:8;
    spec = fitsread([spectralon_dir,'/',spectralon_fname,'F',int2str(K),'.fit']);
    w = spec./WB(:,:,K);
    vect = vect .* 0;
    count = 1;
    for I = 1:row;
        for J = 1:col;
            if (spec_mask(I,J) > 0.5)
                vect(count) = w(I,J);
                count = count + 1;
            end;
        end;
    end;
    med_vec(K) = median(vect);
    if (debug == 1)
        display(['Median value ',int2str(K),' - ',num2str(med_vec(K))]);
    end;
end;

wb_factor = min(med_vec)*cos(light_angle/360*2*pi);
if (debug == 1)
    display(['WB Factor - ',num2str(wb_factor)]);
end;

for I = 1:8;
    WB(:,:,I) = WB(:,:,I)./wb_factor;
end;

for I = 1:8;
    listing = dir([data_dir,'/*F',int2str(I),'.fit']);
    S = size(listing);
    display(['Number of images - ',int2str(S(1))]);
    for J = 1:S(1);
        image_fname = [data_dir,'/',listing(J).name];
        %display(['Image - ',image_fname]);
        [pathstr, name, ext] = fileparts(image_fname);
        output_fname = [pathstr,'/',name,'.tif'];
        fit2tiff16_single_pp(image_fname,output_fname,WB(:,:,I),debug);
    end;
end;

return;