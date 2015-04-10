function plotRegressionMADResults(prediction, real)

    tolerance = [0.25,0.5,1,1.5];
    madArray = [];
    for i=1:length(tolerance)
        mad = madCalculation(prediction, real);
        madArray = [madArray,mad];
    end
    
    f = figure('name','REC Curve');
    area(tolerance,madArray,'FaceColor','g'); hold on;
    plot(tolerance,madArray,'ro','MarkerSize',10, 'LineWidth',2); hold on;
   
    labels = cellstr( num2str(madArray) );  %' # labels correspond to their order
    %text(tolerance, madAccuracyArray, labels, 'VerticalAlignment','bottom', ...
    %                         'HorizontalAlignment','right')
    for i=1:length(madArray)
        text(tolerance(i),madArray(i),num2str(madArray(i)),'VerticalAlignment','bottom', ...
                             'HorizontalAlignment','left'); hold on;
    end
    set(findall(f,'type','text'),'FontSize',15,'fontWeight','bold');
    title('REC Curve');
    xlabel('Tolerance value');
    ylabel('Accuracy');

end