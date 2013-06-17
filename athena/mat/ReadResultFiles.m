function lineCoordinateMatrix = ReadResultFiles(resultEPSFileName, resultTxtFileName)

% Get the start and ending points of each line segment

%% Read EPS file first
fid = fopen(resultEPSFileName);
tline = fgets(fid);
pattern = 'BoundingBox';
while isempty(strfind(tline, pattern))
    tline = fgets(fid);
end
fclose(fid);

% Get the heigh of image
digitPosLogicVec = isstrprop(tline, 'digit');
digitPosVec = find(digitPosLogicVec == 1);
numStrVec = tline(digitPosVec(1):end);        % Numbers in string
numVec = sscanf(numStrVec,'%f');              % Convert into real numbers
imWidth  = numVec(end-1);
imHeight = numVec(end);


%% Read text file
lineResultMatrix = load(resultTxtFileName);

% % For plot function
% lineResultMatrix(:,2) = imHeight - lineResultMatrix(:,2);
% lineResultMatrix(:,4) = imHeight - lineResultMatrix(:,4);

lineCoordinateMatrix = lineResultMatrix(:,(1:4));

