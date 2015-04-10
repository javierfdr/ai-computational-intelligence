tstfiles = dir('*.tst.csv')
tstresults = [];
for f = tstfiles
    filename = f.name;
    tstresults = [tstresults ; csvread(filename)];
end

real = tstresults(:,1)';
prediccio = tstresults(:,2)';

save('resultstst.mat','prediccio','real')

clear
