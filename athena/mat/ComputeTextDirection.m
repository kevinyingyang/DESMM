function mainAngle = ComputeTextDirection(varargin)

[imageName, biMethod, weightHist, numOfBins, imageCut, imageCircle] = parse_inputs(varargin{:});

% Generate some temporary files
indexPos = find(imageName == '\', 1, 'last');
imageNameTmp = imageName(indexPos+1 : end);
bmpImageName = [pwd '\BMPFiles\' strrep(imageNameTmp, '.jpg', '_binary.bmp')];
pgmImageName = [pwd '\PGMFiles\' strrep(imageNameTmp, '.jpg', '_binary.pgm')];
epsResultName  = [pwd '\EPSFiles\' strrep(imageNameTmp, '.jpg', '_binary_result.eps')];
pngResultName  = [pwd '\PNGFiles\' strrep(imageNameTmp, '.jpg', '_binary_result.png')];
txtResultName  = [pwd '\TXTFiles\' strrep(imageNameTmp, '.jpg', '_binary_result.txt')];
histResultName = [pwd '\HISTFiles\' strrep(imageNameTmp, '.jpg', '_binary_result.png')];

% Read an image file and binarize it
imGrayMatrix = imread(imageName);
imGrayMatrix = rgb2gray(imGrayMatrix);
if imageCut == 1 % Take the central part of the image to avoid margins
    [imRowNum, imColNum] = size(imGrayMatrix);
    imGrayMatrix = imGrayMatrix( ceil(imRowNum/4) : floor(3*imRowNum/4), ceil(imColNum/4) : floor(3*imColNum/4) );
end
BinarizeImage(imGrayMatrix, bmpImageName, pgmImageName, biMethod);

% Call .exe file to detect lines
system(['LineDetector.exe -P ' epsResultName ' ' pgmImageName ' ' txtResultName]);

% Cut the image until it's small enough for processing
while exist(epsResultName, 'file') == 0
    
%     [imRowNum, imColNum] = size(imGrayMatrix);
%     imGrayMatrix = imGrayMatrix( ceil(imRowNum/4) : floor(3*imRowNum/4), ceil(imColNum/4) : floor(3*imColNum/4) );
    
    imGrayMatrix = imresize(imGrayMatrix, 0.90);
    
    BinarizeImage(imGrayMatrix, bmpImageName, pgmImageName, biMethod);
    % Call .exe file to detect lines
    system(['LineDetector.exe -P ' epsResultName ' ' pgmImageName ' ' txtResultName]);
    
end

% Convert eps file to .png
system(['"C:\Program Files (x86)\ImageMagick-6.8.5-Q16\convert.exe" ' epsResultName ' ' pngResultName]);

% Read the results of straight lines
lineCoordinateMatrix = ReadResultFiles(epsResultName, txtResultName);



[centerRowFull, centerColFull] = size(imGrayMatrix);
% Compute the radius of a circle
if centerColFull < centerRowFull
    radius = centerColFull / 2 - 1;
else
    radius = centerRowFull / 2 - 1;
end
% radius = radius / 2;
centerX = floor(centerColFull/2);
centerY = floor(centerRowFull/2);
% nseg = 720;
% imshow(imGrayMatrix);
% DrawCircle(centerX, centerY, radius, nseg, [1 0 0]);
% saveas(gcf, circleImageName); hold off

% Compute the angles of line segments and x-axis
[angleVec, lengthVec] = ComputeLineAngleLength(lineCoordinateMatrix, imageCircle, radius, centerX, centerY);


% Weighted histogram
if weightHist == 1
    weightVec = lengthVec ./ sum(lengthVec);
    numOfBins = 360;
    stepSize  = 180 / numOfBins;
    edgeVec   = -90 : stepSize : 90;
    freqCountsVec = weightedhistc(angleVec, weightVec, edgeVec);
    [~, maxCountIndex] = max(freqCountsVec);
    mainAngle = edgeVec(maxCountIndex);
    h = figure;
    set(gcf,'Visible','on');
    bar(edgeVec, freqCountsVec);
else
    stepSize = 180 / numOfBins;
    edgeVec   = -90 : stepSize : 90;
    weightVec = ones(1, length(angleVec));
    freqCountsVec = weightedhistc(angleVec, weightVec, edgeVec);
    [~, maxCountIndex] = max(freqCountsVec);
    mainAngle = edgeVec(maxCountIndex) + stepSize / 2;
    if mainAngle > 90
        mainAngle = mainAngle - stepSize / 2;
    end
    h = figure;
    set(gcf,'Visible','on');
    bar(edgeVec, freqCountsVec);
    
%     % normal histogram without weight
%     [freqCountsVec, binLocationVec] = hist(angleVec, numOfBins);
%     [~, maxCountIndex] = max(freqCountsVec);
%     mainAngle = binLocationVec(maxCountIndex);
%     h = figure;
%     set(gcf,'Visible','off');
%     hist(angleVec, numOfBins);
end

print(h, '-dpng', histResultName);
close(h);

% set(gcf,'Visible','on');
% % Show the lines
% lineNum = size(lineResultMatrix, 1);
% figure; hold on
% for i = 1 : 1 : lineNum
%     line([lineCoordinateMatrix(i, 1) lineCoordinateMatrix(i, 3)], [lineCoordinateMatrix(i, 2) lineCoordinateMatrix(i, 4)], 'LineWidth',1,'Color',[1 0 0]);
% end
% 
% xlim([0 imWidth]); ylim([0 imHeight]);

function [imageName, biMethod, weightHist, numOfBins, imageCut, imageCircle] = parse_inputs(varargin)
% Outputs:  imageName:   filename of the image
%           biMethod:    method method to do the binarization
%           weight:      weighted histogram
%           numOfBins:   number of bins
%           imageCut:    consider cutting the image before processing

biMethod = 'ours';
weightHist = 0;
numOfBins = 90;
imageCut = 0;
imageCircle = 0;

switch nargin
    case 1             % ComputeTextDirection(imageName)
        imageName = varargin{1};
    case 2             % ComputeTextDirection(imageName, biMethod)
        imageName = varargin{1};
        biMethod  = varargin{2};
    case 3             % ComputeTextDirection(imageName, biMethod, weight)
        imageName = varargin{1};
        biMethod  = varargin{2};
        weightHist    = varargin{3};
    case 4             % ComputeTextDirection(imageName, biMethod, weight, numOfBins)
        imageName = varargin{1};
        biMethod  = varargin{2};
        weightHist    = varargin{3};
        numOfBins = varargin{4};
    case 5             % ComputeTextDirection(imageName, biMethod, weight, numOfBins, imageCut)
        imageName = varargin{1};
        biMethod  = varargin{2};
        weightHist    = varargin{3};
        numOfBins = varargin{4};
        imageCut  = varargin{5};
    case 6             % ComputeTextDirection(imageName, biMethod, weight, numOfBins, imageCut, imageCircle)
        imageName = varargin{1};
        biMethod  = varargin{2};
        weightHist    = varargin{3};
        numOfBins = varargin{4};
        imageCut  = varargin{5};
        imageCircle = varargin{6};
    otherwise,
        eid = 'ComputeTextDirection:invalidInputs';    
        error(eid, '%s', 'Invalid input arguments.');
end
