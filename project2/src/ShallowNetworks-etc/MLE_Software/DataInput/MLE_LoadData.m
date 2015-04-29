
function Data = MLE_LoadData (ParamGeneral,Param);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Output:
%
%  Data.NExamplesDevelopment --- integer
%  Data.InputsDevelopment    --- matrix
%  Data.LabelsDevelopment    --- vector
%  Data.NExamplesValid       --- integer (optionally)
%  Data.InputsValid          --- matrix  (optionally)
%  Data.LabelsValid          --- vector  (optionally)
%  Data.NExamplesTest        --- integer (optionally)
%  Data.InputsTest           --- matrix  (optionally)
%  Data.LabelsTest           --- vector  (optionally)
%  Data.OriginalSetOfLabels  --- vector  (only if classification)
%  Data.InternalSetOfLabels  --- vector  (only if classification)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fprintf('Loading data');

if ParamGeneral.ClassificationProblem
  SetOfLabels = [];
end;

%%%=======================================================================

%%%------------------------------------
%%% Validation of mandatory parameters
%%%------------------------------------

MLE_ValidateMandatoryParameters (ParamGeneral,Param);

%%%-----------------------------
%%% Default optional parameters
%%%-----------------------------

if ~isfield(Param,'DataDirectory')
  Param.DataDirectory = '.';
end;

%%%=======================================================================

%%%------------------
%%% Development Data
%%%------------------

fprintf(' (%s/%s)...',Param.DataDirectory,Param.DevelopmentInputsFile);

filename = sprintf('%s/%s',Param.DataDirectory,Param.DevelopmentInputsFile);
Data.InputsDevelopment = load(filename);
if isfield(Param,'DevelopmentLabelsFile') && ~isempty(Param.DevelopmentLabelsFile)
  filename = sprintf('%s/%s',Param.DataDirectory,Param.DevelopmentLabelsFile);
  LabelsDevelopment = load(filename);
  Data.LabelsDevelopment = LabelsDevelopment;
  Data.LabelsDevelopmentExist = 1;
  if ParamGeneral.ClassificationProblem
    SetOfLabels = [SetOfLabels; LabelsDevelopment];
  end;
else
  Data.LabelsDevelopment = [];
end;
Data.NExamplesDevelopment = size(Data.InputsDevelopment,1);
NVariablesOrig = size(Data.InputsDevelopment,2);

%%%----------------------------
%%% Validation Data (optional)
%%%----------------------------

if isfield(Param,'ValidInputsFile') && ~strcmp(Param.ValidInputsFile,'')
  fprintf(' (%s/%s)...',Param.DataDirectory,Param.ValidInputsFile);
  filename = sprintf('%s/%s',Param.DataDirectory,Param.ValidInputsFile);
  Data.InputsValid = load(filename);
  if isfield(Param,'ValidLabelsFile') && ~isempty(Param.ValidLabelsFile)
    filename = sprintf('%s/%s',Param.DataDirectory,Param.ValidLabelsFile);
    LabelsValid = load(filename);
    Data.LabelsValid = LabelsValid;
    if ParamGeneral.ClassificationProblem
      SetOfLabels = [SetOfLabels; LabelsValid];
    end;
  else
    Data.LabelsValid = [];
  end;
  Data.NExamplesValid = size(Data.InputsValid,1);
  if NVariablesOrig ~= size(Data.InputsValid,2)
    error('MLE_LoadData: NVariablesOrig ~= size(Data.InputsValid,2)');
  end;
else
  Data.LabelsValid = [];
end;

%%%----------------------
%%% Test Data (optional)
%%%----------------------

if isfield(Param,'TestInputsFile') && ~strcmp(Param.TestInputsFile,'')
  fprintf(' (%s/%s)...',Param.DataDirectory,Param.TestInputsFile);
  filename = sprintf('%s/%s',Param.DataDirectory,Param.TestInputsFile);
  Data.InputsTest = load(filename);
  if isfield(Param,'TestLabelsFile') && ~isempty(Param.TestLabelsFile)
    filename = sprintf('%s/%s',Param.DataDirectory,Param.TestLabelsFile);
    LabelsTest = load(filename);
    Data.LabelsTest = LabelsTest;
    if ParamGeneral.ClassificationProblem
      SetOfLabels = [SetOfLabels; LabelsTest];
    end;
  else
    Data.LabelsTest = [];
  end;
  Data.NExamplesTest = size(Data.InputsTest,1);
  if NVariablesOrig ~= size(Data.InputsTest,2)
    error('MLE_LoadData: NVariablesOrig ~= size(Data.InputsTest,2)');
  end;
else
  Data.LabelsTest = [];
end;

%%%----------------------------
%%% Features Subset (optional)
%%%----------------------------

if isfield(Param,'Features') && ~isempty(Param.Features)
  Features = sort(Param.Features);
  Data.InputsDevelopment = Data.InputsDevelopment(:,Features);
  NVariablesOrig = size(Data.InputsDevelopment,2);
  if isfield(Data,'NExamplesValid')
    Data.InputsValid = Data.InputsValid(:,Features);
  end;
  if isfield(Data,'NExamplesTest')
    Data.InputsTest = Data.InputsTest(:,Features);
  end;
end;

%%%--------------------------
%%% New labels are 1,2,3,...
%%%--------------------------

if ParamGeneral.ClassificationProblem
  SetOfLabels = sort(unique(SetOfLabels));
  for i=1:size(SetOfLabels,1)
    if isfield(Data,'LabelsDevelopmentExist')
      Data.LabelsDevelopment(find(LabelsDevelopment==SetOfLabels(i))) = i;
    end;
    if ~isempty(Data.LabelsValid)
      Data.LabelsValid(find(LabelsValid==SetOfLabels(i))) = i;
    end;
    if ~isempty(Data.LabelsTest)
      Data.LabelsTest(find(LabelsTest==SetOfLabels(i))) = i;
    end;
  end;
  Data.OriginalSetOfLabels = sort(unique(SetOfLabels));
  Data.InternalSetOfLabels = [1:size(SetOfLabels,1)]'; %'
end;

%%%------

fprintf(' done\n');

fprintf('  Number of variables: %d\n',NVariablesOrig);

return;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Auxiliary Functions
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function MLE_ValidateMandatoryParameters (ParamGeneral,Param);

if ~isfield(Param,'DevelopmentInputsFile')
  error('MLE_LoadData: No development data file');
end;

if ParamGeneral.ClassificationProblem || ParamGeneral.RegressionProblem
  if ~isfield(Param,'DevelopmentLabelsFile')
    error('MLE_LoadData: No development labels file');
  end;
end;

return;

%%%-------------------------------------------------
