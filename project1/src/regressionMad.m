function madValue = regressionMad(predicted, real)
    %accuracy = 1-median(abs(predicted-real));
    madValue = mean(abs(predicted-real));
end