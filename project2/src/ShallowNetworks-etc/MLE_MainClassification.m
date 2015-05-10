
clear all;

%%% Elapsed time
tic;

%----------------------------------
%%% Defaulta parameters

MatlabMLExperimentsHome = ...
 '/home/eromero/PPAL/Docencia/Master-ATCI/Software/Sources/ShallowNetworks-etc';
addpath(genpath(MatlabMLExperimentsHome));  %%% recursive addpath

[ParamGeneral ParamData ...
 ParamPreprocess ParamSamplingDevelopment ...
 ParamConstructModel ParamResults] = MLE_DefaultParameters;

%----------------------------------
%%% Classification Problem

ParamGeneral.ClassificationProblem = 1;

%----------------------------------
%%% Original Data

ParamData.DataDirectory = 'Data';
ParamData.DevelopmentInputsFile = 'vehicle.data';
ParamData.DevelopmentLabelsFile = 'vehicle.labels';
ParamData.ValidInputsFile = '';
ParamData.ValidLabelsFile = '';
ParamData.TestInputsFile = '';
ParamData.TestLabelsFile = '';

%%%ParamData.Features = [3:5,10];

%----------------------------------
%%% Preprocessing Data

%ParamPreprocess.TypeTransform = 'NoTransform';
ParamPreprocess.TypeTransform = 'Mean0DevStd1';
%ParamPreprocess.TypeTransform = 'Hypercube01';
%ParamPreprocess.TypeTransform = 'Scaling';
%ParamPreprocess.ParamTransform = 1/255;

%----------------------------------
%%% Sampling

ParamSamplingDevelopment.NTimes = 10;
ParamSamplingDevelopment.Randomize = 1;
ParamSamplingDevelopment.RandomSeed = 0;  %%% values <=0: clock
ParamSamplingDevelopment.Stratified = 1;
ParamSamplingDevelopment.Type = 'HoldOutTVT';
ParamSamplingDevelopment.HoldOutTVT.PctValid = 0.20;  %%% 0 <= Pct < 1
ParamSamplingDevelopment.HoldOutTVT.PctTest = 0.20;   %%% 0 <= Pct < 1
%ParamSamplingDevelopment.Type = 'CrossValidation';
%ParamSamplingDevelopment.CrossValidation.NFolds = 4;
%ParamSamplingDevelopment.CrossValidation.NFoldsDouble = 5;
%ParamSamplingDevelopment.Type = 'Bootstrapping';
%ParamSamplingDevelopment.Bootstrapping.NBSamplings = 10;

%----------------------------------
%%% Classification models and parameters

%---
ParamConstructModel.ModelName = 'ELM_NNClassification';
ParamConstructModel.ParamModel.NUnits = 70;
ParamConstructModel.ParamModel.ActFunH = 'gau';
ParamConstructModel.ParamModel.Temperature = 0.001;
%ParamConstructModel.ParamModel.ActFunH = 'sig';
%ParamConstructModel.ParamModel.Temperature = 0.01;
%ParamConstructModel.ParamModel.ActFunH = 'sin';
%ParamConstructModel.ParamModel.ActFunH = 'hardlim';
ParamConstructModel.ParamModel.WithBias = 1;  %%% only if sig/sin/hardlim
%ParamConstructModel.ParamModel.WithBias = 0;  %%% only if sig/sin/hardlim
ParamConstructModel.ParamModel.IniWeights = 'Random';
%ParamConstructModel.ParamModel.IniWeights = 'InputRandom';
ParamConstructModel.ParamModel.MaxIterInput = 50;
%---
%ParamConstructModel.ModelName = 'NNToolbox_BackPropagationNNClassification';
%ParamConstructModel.ParamModel.NUnits = 10;    %%% For several layers: [20 10]
%ParamConstructModel.ParamModel.NIters = 100;
%%% Toolbox default transfer functions:
%%%   compet / hardlim / hardlims / logsig / netinv / poslin / purelin
%%%   radbas / satlin / satlins / softmax / tansig / tribas / elliotsig
%ParamConstructModel.ParamModel.ActFunH = 'tansig';
%ParamConstructModel.ParamModel.LearningRate = 0.01;
%ParamConstructModel.ParamModel.Momentum = 0.9;
%%ParamConstructModel.ParamModel.Goal = 0.0001;
%%ParamConstructModel.ParamModel.Show = 50;
%---
%ParamConstructModel.ModelName = 'NNToolbox_LevenbergMarquardtNNClassification';
%ParamConstructModel.ParamModel.NUnits = 10;    %%% For several layers: [20 10]
%ParamConstructModel.ParamModel.NIters = 100;
%%% Toolbox default transfer functions:
%%%   compet / hardlim / hardlims / logsig / netinv / poslin / purelin
%%%   radbas / satlin / satlins / softmax / tansig / tribas / elliotsig
%ParamConstructModel.ParamModel.ActFunH = 'tansig';
%ParamConstructModel.ParamModel.Mu = 1.0000e-03;
%ParamConstructModel.ParamModel.MuDec = 0.1;
%ParamConstructModel.ParamModel.MuInc = 10;
%%ParamConstructModel.ParamModel.Goal = 0.0001;
%%ParamConstructModel.ParamModel.Show = 50;
%%ParamConstructModel.ParamModel.MemReduc = 1;
%---
%ParamConstructModel.ModelName = 'LIBSVM_SupportVectorCostClassification';
%ParamConstructModel.ParamModel.ParamKernel = '-t 2 -g 0.07 -r 0'; %'
%ParamConstructModel.ParamModel.ParamCSVC = '-c 1';
%---
%ParamConstructModel.ModelName = 'LIBSVM_SupportVectorNuClassification';
%ParamConstructModel.ParamModel.ParamKernel = '-t 2 -g 0.07 -r 0'; %'
%ParamConstructModel.ParamModel.ParamNuSVC = '-n 0.3';
%---
%ParamConstructModel.ModelName = 'PRTools_kNNClassification';
%ParamConstructModel.ParamModel.kNN = 3;
%---
%ParamConstructModel.ModelName = 'PRTools_LogisticClassification';
%---
%ParamConstructModel.ModelName = 'PRTools_LeastSquaresClassification';
%---
%ParamConstructModel.ModelName = 'PRTools_ParzenClassification';
%---
%ParamConstructModel.ModelName = 'PRTools_NaiveBayesClassification';
%ParamConstructModel.ParamModel.NBins = 10;
%---
%ParamConstructModel.ModelName = 'PRTools_SupportVectorClassification';
%ParamConstructModel.ParamModel.TKernel = 'radial_basis'; %%% 'gaussian'
%%ParamConstructModel.ParamModel.TKernel = 'polynomial';
%%ParamConstructModel.ParamModel.TKernel = 'cosine';
%ParamConstructModel.ParamModel.KParam = 0.1;
%ParamConstructModel.ParamModel.CParam = 10;
%---

%----------------------------------

ParamResults.ParamSaveModels.SaveModels = 0;
ParamResults.ParamSaveModels.DirSavedModels = 'SavedModels';
%ParamResults.ParamSaveModels.PrefixSavedModels = 'model';
ParamResults.ParamSaveModels.PrefixSavedModels = ['model-' ParamConstructModel.ModelName];

ParamResults.ParamSaveFinalResults.SaveFinalResults = 0;
ParamResults.ParamSaveFinalResults.DirSavedFinalResults = 'SavedFinalResults';
%ParamResults.ParamSaveFinalResults.PrefixSavedFinalResults = 'results';

%----------------------------------

Data = MLE_LoadData (ParamGeneral,ParamData);

Data = MLE_PreprocessData (ParamGeneral,ParamPreprocess,Data);

SamplingDevelopment = ...
  MLE_SamplingDevelopmentData (ParamGeneral,ParamSamplingDevelopment,Data);

Results = MLE_ConstructModelObtainResults...
            (ParamGeneral,ParamConstructModel,ParamResults,...
             Data,SamplingDevelopment);

%----------------------------------

Results

%----------------------------------

%%% Elapsed time
toc;
