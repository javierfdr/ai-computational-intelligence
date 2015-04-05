function plotRegressionResults(prediction, real)

    tolerance = [0.25,0.5,1,1.5];
    madAccuracyArray = [];
    for i=1:length(tolerance)
        accuracy = madAccuracy(prediction, real, tolerance(i));
        madAccuracyArray = [madAccuracyArray,accuracy];
    end
    
    f = figure('name','REC Curve');
    area(tolerance,madAccuracyArray,'FaceColor','g'); hold on;
    plot(tolerance,madAccuracyArray,'ro','MarkerSize',10, 'LineWidth',2); hold on;
   
    labels = cellstr( num2str(madAccuracyArray) );  %' # labels correspond to their order
    %text(tolerance, madAccuracyArray, labels, 'VerticalAlignment','bottom', ...
    %                         'HorizontalAlignment','right')
    for i=1:length(madAccuracyArray)
        text(tolerance(i),madAccuracyArray(i),num2str(madAccuracyArray(i)),'VerticalAlignment','bottom', ...
                             'HorizontalAlignment','left'); hold on;
    end
    set(findall(f,'type','text'),'FontSize',15,'fontWeight','bold');
    title('REC Curve');
    xlabel('Tolerance value');
    ylabel('Accuracy');

end