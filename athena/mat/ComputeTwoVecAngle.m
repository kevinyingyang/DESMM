function thetaInDegrees = ComputeTwoVecAngle(vec1, vec2)

cosTheta = dot(vec1, vec2)/(norm(vec1) * norm(vec2));
thetaInDegrees = acos(cosTheta) * 180 / pi;
