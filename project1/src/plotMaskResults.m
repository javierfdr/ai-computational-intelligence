% plotting mask results for best discretization and solution

legend = {'All-Kmeans-8'};

predList = [];
realList = [];
qualityList = [];
hrList =[];

load('best-mask2.mat');
predList=[predList;prediccio];
realList=[realList;real];
qualityList = [qualityList;0.411];
hrList = [hrList;0.411];

load('best-mask3.mat');
predList=[predList;prediccio];
realList=[realList;real];
qualityList = [qualityList;0.452];
hrList = [hrList;0.459];

load('best-mask4.mat');
predList=[predList;prediccio];
realList=[realList;real];
qualityList = [qualityList;0.460];
hrList = [hrList;0.5541];

load('best-mask5.mat');
predList=[predList;prediccio];
realList=[realList;real];
qualityList = [qualityList;0.381];
hrList = [hrList;0.726];

load('best-mask-6.mat');
predList=[predList;prediccio];
realList=[realList;real];
qualityList = [qualityList;0.310];
hrList = [hrList;0.798];

load('best-mask-7.mat');
predList=[predList;prediccio];
realList=[realList;real];
qualityList = [qualityList;0.265];
hrList = [hrList;0.9255];


plotMultiRegressionMADResults(predList,realList,[2;3;4;5;6;7],qualityList, hrList,'best-solution-masks');