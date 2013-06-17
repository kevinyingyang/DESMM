function [angleVec, lengthVec] = ComputeLineAngleLength(lineCoordinateMatrix, imageCircle, radius, centerX, centerY)

% Each row of the input contains the coordinates of two points. 
% The coordinate of the first point, followed by the coordinate of the
% second point.

lineNum = size(lineCoordinateMatrix, 1);

angleVec  = zeros(1, lineNum);
lengthVec = zeros(1, lineNum);
lineCount = 0;

for i = 1 : 1 : lineNum
    
    lineVec = [lineCoordinateMatrix(i, 1) - lineCoordinateMatrix(i, 3)... 
               lineCoordinateMatrix(i, 2) - lineCoordinateMatrix(i, 4)];
    
    if lineVec(2) < 0
        lineVec = -lineVec; % Make y >= 0
    end
    
    if imageCircle == 1 % remove angles who lines are out of the circle
        
        distanceA = sqrt( (lineCoordinateMatrix(i, 1)-centerX)^2 + (lineCoordinateMatrix(i, 2)-centerY)^2 );
        distanceB = sqrt( (lineCoordinateMatrix(i, 3)-centerX)^2 + (lineCoordinateMatrix(i, 4)-centerY)^2 );
        
%         if i <= 10
%             line([lineCoordinateMatrix(i, 1) lineCoordinateMatrix(i, 3)],...
%                  [lineCoordinateMatrix(i, 2) lineCoordinateMatrix(i, 4)]);
%             (lineCoordinateMatrix(i, 2)-lineCoordinateMatrix(i, 4))/(lineCoordinateMatrix(i, 1)-lineCoordinateMatrix(i, 3))
%         end
        
        if (distanceA <= radius) && (distanceB <= radius)
            
            lineCount = lineCount + 1;
            if lineVec(1) > 0
                angleVec(lineCount) = ComputeTwoVecAngle( abs(lineVec), [0 1] ); 
                angleVec(lineCount) = -angleVec(lineCount);  % clockwise
            elseif lineVec(1) < 0
                angleVec(lineCount) = ComputeTwoVecAngle( abs(lineVec), [0 1] );  % counter-clockwise
            else
                angleVec(lineCount) = 0;
            end
            
            % Compute line length
            lengthVec(lineCount) = sqrt( dot(lineVec, lineVec) );
            
        end
        
    else
        
        lineCount = lineCount + 1;
        if lineVec(1) > 0
            angleVec(lineCount) = ComputeTwoVecAngle( abs(lineVec), [0 1] ); 
            angleVec(lineCount) = -angleVec(lineCount);  % clockwise
        elseif lineVec(1) < 0
            angleVec(lineCount) = ComputeTwoVecAngle( abs(lineVec), [0 1] );  % counter-clockwise
        else
            angleVec(lineCount) = 0;
        end
        
        % Compute line length
        lengthVec(lineCount) = sqrt( dot(lineVec, lineVec) );
        
    end
    
end

angleVec  = angleVec(1 : lineCount);
lengthVec = lengthVec(1 : lineCount);



