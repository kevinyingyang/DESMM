function BinarizeImage(imGrayMatrix, bmpImageName, pgmImageName, biMethod)

% Convert a grayscale image to binary image

if strcmp(biMethod, 'ours') == 1
    %% Some settings
    BLACKPIXEL = 0;
    WHITEPIXEL = 1;

    % Compute image histogram and find local minimas
    [counts, ~] = imhist(imGrayMatrix);
    if isempty(strfind(bmpImageName, 'newtest'))
        [~, locs] = findpeaks(-counts, 'minpeakdistance', 80);
    else
        [~, locs] = findpeaks(-counts, 'minpeakdistance', 30);
    end

    % Find the classification threshold
    pixelThresholdA = locs(1);
    pixelThresholdB = locs(2);
    indexMatrixA = imGrayMatrix >= pixelThresholdA;
    indexMatrixB = imGrayMatrix <= pixelThresholdB;
    indexMatrix  = indexMatrixA & indexMatrixB;

    imGrayMatrix(indexMatrix) = BLACKPIXEL;
    imGrayMatrix(logical(1-indexMatrix)) = WHITEPIXEL;
    
elseif strcmp(biMethod, 'sauvola')
    imGrayMatrix = sauvola(imGrayMatrix);
elseif strcmp(biMethod, 'matlab')
    level = graythresh(imGrayMatrix);
    imGrayMatrix = im2bw(imGrayMatrix, level);
else
    eid = 'BinarizeImage:invalidInputs';    
    error(eid, '%s', 'Only two binarization methods implemented (ours or sauvola).');
end

% Write to files
if max(max(double(imGrayMatrix))) == 1 && min(min(double(imGrayMatrix))) == 0
    imwrite(logical(imGrayMatrix), bmpImageName);
    imwrite(logical(imGrayMatrix), pgmImageName);
else
    imwrite(uint8(imGrayMatrix), bmpImageName);
    imwrite(uint8(imGrayMatrix), pgmImageName);
end


