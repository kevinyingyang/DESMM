function RotateImageRandomly()

if exist('RotatedImagesDataset','dir') == 0
    mkdir('RotatedImagesDataset');
end
if exist('AngleResults','dir') == 0
    mkdir('AngleResults');
end

strPattern = '*.jpg';
[imNum, imNames] = GetFileNumName([pwd '\100_image_dataset\'], strPattern);

angleMin = -90;
angleMax = 90;

rorateAngleVec = angleMin + (angleMax - angleMin) .* rand(1, imNum);

fid = fopen('AngleResults\RotatedAngles_Groundtruth.txt', 'w');

for i = 1 : 1 : imNum
    
    % Read an image and convert it into a grayscale image
    imNameTmp = imNames{i};
    imName = ['100_image_dataset\' imNames{i}];
    imGrayMatrix = imread(imName);
    imRotatedMatrix = imrotate(imGrayMatrix, rorateAngleVec(i));
    
    rotateImName = ['RotatedImagesDataset\' strrep(imNameTmp, '.jpg', '_rotated.jpg')];
    imwrite(imRotatedMatrix, rotateImName);
    
    fprintf(fid, 'Rotation angle of %s: %f\n', imNameTmp, rorateAngleVec(i));
    
end

fclose(fid);





