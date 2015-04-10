function mad = madCalculation(predicted, real)
    %accuracy = 1-median(abs(predicted-real));
    mad = mean(abs(predicted-real));
end