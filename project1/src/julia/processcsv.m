tstfiles = ls('*.tst.csv')
tstresults = []
for f in tstfiles
    tstresults = [tstresults ; csvread(f)]
end

real = tstresults[:,1]
prediccio = tstresults[:,2]

save('resultstst.mat','prediccio','real')

clear

trafiles = ls('*.tra.csv')

traresults = []
for f in trafiles
    traresults = [traresults ; csvread(f)]
end

real = traresults[:,1]
prediccio = traresults[:,2]

save('resultstra.mat','prediccio','real')
clear
