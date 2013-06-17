function DrawLines(lineResultMatrix)

% lineResultMatrix = load(resultFileName);
lineNum = size(lineResultMatrix, 1);

figure; hold on
for i = 1 : 1 : lineNum
    line([lineResultMatrix(i, 1) lineResultMatrix(i, 3)], [lineResultMatrix(i, 2) lineResultMatrix(i, 4)], 'LineWidth',1,'Color',[1 0 0]);
end