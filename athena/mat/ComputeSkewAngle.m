function [maxRose, maxRoseIndex, roseArray] = ComputeSkewAngle(acf)

[height, width] = size(acf);
if width < height
    D0 = width / 2 - 1;
else
    D0 = height / 2 - 1;
end

d0 = D0 / 10;

roseArray = zeros(1, 180);
for theta = 0 : 179
    for r = d0 : D0
        rowIndex = floor(height / 2 - r * cos(pi * theta / 180));
        colIndex = floor(width / 2  + r * sin(pi * theta / 180));
        roseArray(theta + 1) = roseArray(theta + 1) + acf(rowIndex, colIndex);
    end
end

[maxRose, maxRoseIndex] = max(roseArray);





