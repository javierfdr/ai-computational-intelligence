function fullDiscretizationAnalysis()
    load('../data/DataFIRKEEL-Ref/FIRdata/P1-Train.mat');
    
    firstGroups = [3,4,5,6];
    secondGroups = [7,8,9,10];
    
    analyzeDiscretization(Alcohol,'Alcohol',firstGroups, true);
    analyzeDiscretization(Alcohol,'Alcohol',secondGroups, false);
 
    analyzeDiscretization(Chlo,'Chlo',firstGroups, true);
    analyzeDiscretization(Chlo,'Chlo',secondGroups, false);
 
    analyzeDiscretization(CitAcid,'CitAcid',firstGroups, true);
    analyzeDiscretization(CitAcid,'CitAcid',secondGroups, false);
 
    analyzeDiscretization(Density,'Density',firstGroups, true);
    analyzeDiscretization(Density,'Density',secondGroups, false);
 
    analyzeDiscretization(FixAcid,'FixAcid',firstGroups, true);
    analyzeDiscretization(FixAcid,'FixAcid',secondGroups, false);
 
    analyzeDiscretization(FreeSulfDio,'FreeSulfDio',firstGroups, true);
    analyzeDiscretization(FreeSulfDio,'FreeSulfDio',secondGroups, false);
 
    analyzeDiscretization(Quality,'Quality',firstGroups, true);
    analyzeDiscretization(Quality,'Quality',secondGroups, false);
 
    analyzeDiscretization(ResSug,'ResSug',firstGroups, true);
    analyzeDiscretization(ResSug,'ResSug',secondGroups, false);
 
    analyzeDiscretization(Sulph,'Sulph',firstGroups, true);
    analyzeDiscretization(Sulph,'Sulph',secondGroups, false);
    
    analyzeDiscretization(TotalSulfDio,'TotalSulfDio',firstGroups, true);
    analyzeDiscretization(TotalSulfDio,'TotalSulfDio',secondGroups, false);
    
    analyzeDiscretization(VolAcid,'VolAcid',firstGroups, true);
    analyzeDiscretization(VolAcid,'VolAcid',secondGroups, false);
 
    analyzeDiscretization(pH,'pH',firstGroups, true);
    analyzeDiscretization(pH,'pH',secondGroups, false);
    

end