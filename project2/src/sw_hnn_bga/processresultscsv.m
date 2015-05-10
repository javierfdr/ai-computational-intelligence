paramsets = {'Exploit','Explore'}
paramsetsn = [ 60, 10, 60000, 10; 600, 10, 20000, 10]
datasets = {'Audiology','CreditCard','HorseColic','Servo'}
datatypes = {'class','class','class','reg'}
neurons = [10,12,14,16,18,20]
prefixes = {'hnn','mlp','rbf'}

cd('Results')
for par = 1:length(paramsets)
    params = paramsets{par}
    for d = 1:length(datasets)
        dataset = datasets{d}
        datatype = datatypes{d};
        cd([params, '/', dataset])
        for pr = 1:length(prefixes)
            prefix = prefixes{pr}
            for n = neurons
                res = csvread([prefix,'-',num2str(n),'.csv']);
                if strcmp(datatype,'class')
                    Sample_Accuracy.MeanAccuracy_DevelopTrain = res(1);
                    Sample_Accuracy.MeanAccuracy_DevelopValid = res(2);
                    Sample_Accuracy.MeanAccuracy_DevelopTest = res(3);
                    Sample_Accuracy.units = n;
                    Sample_Accuracy.BGA_mu = paramsetsn(par,1);
                    Sample_Accuracy.BGA_tau = paramsetsn(par,2);
                    Sample_Accuracy.BGA_FFEvals = paramsetsn(par,3);
                    Sample_Accuracy.BGA_NRuns = paramsetsn(par,4);
                    Sample_Accuracy;
                    save('-mat7-binary', [prefix,'-',num2str(n),'.mat'],'Sample_Accuracy');
                elseif strcmp(datatype,'reg')
                    Sample_MSError.MeanMSError_DevelopTrain = res(1);
                    Sample_MSError.MeanMSError_DevelopValid = res(2);
                    Sample_MSError.MeanMSError_DevelopTest = res(3);
                    Sample_MSError.units = n;
                    Sample_MSError.BGA_mu = paramsetsn(par,1);
                    Sample_MSError.BGA_tau = paramsetsn(par,2);
                    Sample_MSError.BGA_FFEvals = paramsetsn(par,3);
                    Sample_MSError.BGA_NRuns = paramsetsn(par,4);
                    Sample_MSError;
                    save('-mat7-binary', [prefix,'-',num2str(n),'.mat'],'Sample_MSError');
                end
            end       
        end
        cd('../../');        
    end
end
cd('..')

