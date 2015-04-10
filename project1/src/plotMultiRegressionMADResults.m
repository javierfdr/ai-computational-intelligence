function plotMultiRegressionMADResults(predictionList, realList, maskSizeList, qaList, hrList,filename)

    tolerance = [0.25,0.5,1,1.5];
    f = figure();
    cmap = hsv(11);
    
    count = 1;
    madArray = [];
    madAccArray = [];
    madAccArray2 = [];
    for p=1:length(predictionList(:,1))
        prediction = predictionList(p,:);
        real = realList(p,:);

        mad = madCalculation(prediction, real);
        madArray = [madArray;mad];
        
        accuracy = madAccuracy(prediction,real,0.5);
        madAccArray = [accuracy;madAccArray];
        
        accuracy2 = madAccuracy(prediction,real,1);
        madAccArray2 = [accuracy2;madAccArray2];
    end
    
    bar(maskSizeList,[madArray,madAccArray,madAccArray2],'grouped'); hold on;% groups by row
    %bar(maskSizeList,madAccArray,'grouped'); hold on;% groups by row
    plot(maskSizeList, qaList, 'r-s'); hold on;
    plot(maskSizeList, hrList, 'g-s'); hold on;
    
    set(findall(f,'type','text'),'FontSize',15,'fontWeight','bold');
    title('MAD/QA/HR Values Comparison');
    xlabel('Mask Size');
    legend({'MAD','Accuracy T0.5','Accuracy T1.0','Mask Quality','HR'},'Location','northwest');
    saveas(f, strcat('../generated_images/',filename),'png');

end