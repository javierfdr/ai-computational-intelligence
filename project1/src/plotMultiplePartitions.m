function plotMultiplePartitions(partitionList,figureName)

    numSubPlot = length(partitionList(1,:)); % must be dividible by 2
    plotCount = 1;
    n = round(numSubPlot/2);
    f = figure('name',figureName,'Visible','off');
    
    for pp=1:length(partitionList(1,:))
        partition = partitionList(:,pp);
        parts = unique(partition);

        sizeArray = [];
        for p=1:length(parts)
            sizeArray=[sum(partition==p),p;sizeArray];
        end

        [values,index] = sort(sizeArray(:,1),'descend');
        recPlotList = [];

        cc=hsv(12);
        ix = randperm(12); % random shuffle
        cc = cc(ix,:);

        totalValues = length(partition);

        subplot(2,n,plotCount);
        for i=1:length(values)
            radius = (values(i)/2);
            quality = getPartitionQuality(partition);
            %pos = [0 0 radius radius];
            %rectangle('Position',pos,'Curvature',[1 1],'FaceColor',[rand() rand() rand()]);
            %recPlotList = [recPlotList,p];
            coverage = (values(i)/totalValues)*100;
            [x,xp,yp] =circle(0,0,radius);
            dn = strcat('Size: ',num2str(values(i)),' Cov: ',num2str(round(coverage)),'%');
            p = plot(x+xp,x+yp,'Color', cc(i,:),'DisplayName',dn,'LineWidth',5); hold on;
            title(strcat(num2str(length(values)),' Partitions ',' DistError: ',num2str(round(quality))));
            recPlotList = [recPlotList,p];
        end  
        l = legend(gca,'show');
        set(l,'FontSize',8);
        
        plotCount = plotCount+1;
    end 
    
    saveas(f, strcat('../generated_images/',figureName),'png');
    
end
