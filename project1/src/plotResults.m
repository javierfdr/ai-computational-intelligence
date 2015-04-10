% plotting all set compared and random uniform

legend = {'All-Frequency-4','All-Frequency-8','All-KMeans-4','All-Kmeans-8','Frequency-High8-Low4','Frequency-High4-Low8','KMeans-High8-Low4','KMeans-High4-Low8','Kmeans-High-Frequency-Low','Kmeans-Low-Frequency-High','All-Frequency-3','All-KMeans-8-Quality-3'};

predList = [];
realList = [];
load('res-4-frequency-comp5.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-8-frecuency-comp4');
predList=[predList;prediccio];
realList=[realList;real];

load('res-4-kmeans-comp5.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('result-8-kmeans-comp4.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-frequency-8h4low-comp4.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-frequency-4h8low-comp5.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-kmeans-8h4low-comp4.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-kmeans-4h8low-comp4.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-kmeans-high-freq-low-comp5.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-kmeans-low-freq-high-comp4.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-equalfrequency-3.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-8-kmeans-3quality.mat');
predList=[predList;prediccio];
realList=[realList;real];

plotMultiRegressionResults(predList,realList,legend,'all-discretization');
%plotMultiRegressionMADResults(predList,realList,legend,'all-discretization-MAD');

%Plotting best 4 discretizations for T1.0

legend = {'All-Kmeans-8','Frecuency-High8-Low4','Frecuency-High4-Low8','Kmeans-High-Frecuency-Low','All-KMeans-8-Quality-3'};

predList = [];
realList = [];

load('result-8-kmeans-comp4.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-frequency-8h4low-comp4.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-frequency-4h8low-comp5.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-kmeans-high-freq-low-comp5.mat');
predList=[predList;prediccio];
realList=[realList;real];

load('res-8-kmeans-3quality.mat');
predList=[predList;prediccio];
realList=[realList;real];

plotMultiRegressionResults(predList,realList,legend,'best-five-discretizations');
%plotMultiRegressionMADResults(predList,realList,legend,'best-five-discretizationsMAD');

