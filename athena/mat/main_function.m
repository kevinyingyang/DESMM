function main_function()


%% Settings
biMethod    = 'matlab';
weightHist  = 0;
numOfBins   = 360;  % used for building the histogram of angles
imageCut    = 0;
imageCircle = 0;    % indicate if we only consider pixels within circle

if exist('BMPFiles','dir') == 0
    mkdir('BMPFiles');
end
delete([pwd '/BMPFiles/*.bmp']);
if exist('CircleFiles', 'dir') == 0
    mkdir('CircleFiles');
end
if exist('PGMFiles','dir') == 0
    mkdir('PGMFiles');
end
delete([pwd '/PGMFiles/*.pgm']);
if exist('EPSFiles','dir') == 0
    mkdir('EPSFiles');
end
delete([pwd '/EPSFiles/*.eps']);
if exist('TXTFiles','dir') == 0
    mkdir('TXTFiles');
end
if exist('PNGFiles','dir') == 0
    mkdir('PNGFiles');
end
delete([pwd '/PNGFiles/*.png']);
if exist('HISTFiles','dir') == 0
    mkdir('HISTFiles');
end
delete([pwd '/HISTFiles/*.png']);
if exist('AngleResults','dir') == 0
    mkdir('AngleResults');
end
if exist('RotatedImageResults','dir') == 0
    mkdir('RotatedImageResults');
end
delete([pwd '/RotatedImageResults/*.png']);

% save the angle to the file
fid = fopen(['AngleResults\ComputedAngles' '_' biMethod '.txt'], 'w');

strPattern = '*.jpg';
[imNum, imageNames] = GetFileNumName([pwd '\RotatedImagesDataset\'], strPattern);

for i = 1 : 1 : imNum
    
    imageNameTmp = imageNames{i};
%     imageNameTmp = '00deb1ef-33fd-4ae8-b30a-415137ca6d9e_rotated.jpg';
    imageName = [pwd '\RotatedImagesDataset\' imageNameTmp];
    fprintf('Testing %d/%d-th image %s...\n', i, imNum, imageNameTmp);
    
    % Angle with respect to the y-axis (negative: clock-wise and positive: clock-wise)
    mainAngle = ComputeTextDirection(imageName, biMethod, weightHist, numOfBins, imageCut, imageCircle);
    fprintf(fid, 'Rotation angle of %s: %f\n', imageNameTmp, mainAngle);
    
    rotatedImName = [pwd '\RotatedImageResults\' strrep(imageNameTmp, '.jpg', '_rotated_back.png')];
    
    imageData = imread(imageName);
    imageData = imrotate(imageData, mainAngle);
    imwrite(imageData, rotatedImName);
    
end

fclose(fid);


% To compare the computed angles with the groudtruth
fidA = fopen('AngleResults\RotatedAngles_Groundtruth.txt', 'r');
fidB = fopen(['AngleResults\ComputedAngles' '_' biMethod '.txt'], 'r');
fidC = fopen(['AngleResults\AngleDiffVec' '_' biMethod '.txt'], 'w');

imageNum = 0;
angleDiffVec = zeros(1, imNum);
while (~feof(fidA)) && (~feof(fidB))
    
    lineA = fgets(fidA);
    lineB = fgets(fidB);
    
    [imageNameA, angleA] = GetImageFileNameAngle(lineA);
    [imageNameB, angleB] = GetImageFileNameAngle(lineB);
    
    imageNameB = strrep(imageNameB, '_rotated', '');
    
    while ~strcmp(imageNameB, imageNameA)
        lineA = fgets(fidA);
        [imageNameA, angleA] = GetImageFileNameAngle(lineA);
    end
    
    % Compute angle difference: different ways to define pos and neg angles, so use + here, not -
    imageNum = imageNum + 1;
    angleDiffVec(imageNum) = abs(angleA + angleB);
    fprintf(fidC, 'Angle difference of %s: %f\n', imageNameB, angleDiffVec(imageNum));
    
end

fclose(fidA);
fclose(fidB);
fclose(fidC);

angleDiffVec = angleDiffVec(1 : imageNum);
save(['AngleResults\AngleDiffNumVec' '_' biMethod '.txt'], 'angleDiffVec', '-ASCII');

set(gcf,'Visible','on');


function [imageName, angle] = GetImageFileNameAngle(str)

% str follows the format: Rotation angle of 00024bd2-5e6a-4f3d-8aed-80f89cdc67b5.jpg: 56.650264

index = strfind(str, ':');
imageName = str(18:index-1);

index = strfind(str, ':');
angle = str2num(str(index+2:end));




function [imageName, angle] = GetAngleDifference(str)

% str follows the format: Rotation angle of 00024bd2-5e6a-4f3d-8aed-80f89cdc67b5.jpg: 56.650264

index = strfind(str, ':');
imageName = str(18:index-1);

index = strfind(str, ':');
angle = str2num(str(index+2:end));


