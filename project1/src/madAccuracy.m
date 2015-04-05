function accuracy = madAccuracy(predicted, real,tolerance)
    %accuracy = 1-median(abs(predicted-real));
    accuracy = mean(abs(predicted-real)<=tolerance);
end