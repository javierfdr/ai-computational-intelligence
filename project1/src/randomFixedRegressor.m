function prediction = randomFixedRegressor(real)
    b=7;
    a=5;
    prediction = (b-a).*rand(1,length(real)) + a;
end