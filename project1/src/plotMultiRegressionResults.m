function plotMultiRegressionResults(predictionList, realList, legendNames,filename)

    tolerance = [0.25,0.5,1,1.5];
    f = figure('name','REC Curve');
    cmap = hsv(13);
    
    count = 1;
    for p=1:length(predictionList(:,1))
        prediction = predictionList(p,:);
        real = realList(p,:);
        
        madAccuracyArray = [];
        for i=1:length(tolerance)
            accuracy = madAccuracy(prediction, real, tolerance(i));
            madAccuracyArray = [madAccuracyArray,accuracy];
        end

        %plot(tolerance,madAccuracyArray,'ro','MarkerSize',10, 'LineWidth',2); hold on;
        plot(tolerance,madAccuracyArray,'-s','MarkerSize',10, 'LineWidth',2,'Color',cmap(count,:)); hold on;

        count=count+1;

        labels = cellstr( num2str(madAccuracyArray) );  %' # labels correspond to their order
        %text(tolerance, madAccuracyArray, labels, 'VerticalAlignment','bottom', ...
        %                         'HorizontalAlignment','right')
        %for i=1:length(madAccuracyArray)
        %    text(tolerance(i),madAccuracyArray(i),num2str(madAccuracyArray(i)),'VerticalAlignment','top', ...
        %                         'HorizontalAlignment','left'); hold on;
        %end    
    end

    

    set(findall(f,'type','text'),'FontSize',15,'fontWeight','bold');
    title('REC Curve');
    xlabel('Tolerance value');
    ylabel('Accuracy');
    legend(legendNames,'Location','northwest');
    saveas(f, strcat('../generated_images/',filename),'png');

end