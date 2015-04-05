function plotPartition(partition,figureName)
    figure('name',figureName);
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
    
    for i=1:length(values)
        radius = (values(i)/2);
        %pos = [0 0 radius radius];
        %rectangle('Position',pos,'Curvature',[1 1],'FaceColor',[rand() rand() rand()]);
        %recPlotList = [recPlotList,p];
        coverage = (values(i)/totalValues)*100;
        [x,xp,yp] =circle(0,0,radius);
        dn = strcat('Part: ',num2str(index(i)),' Size: ',num2str(values(i)),' Cov: ',num2str(round(coverage)),'%');
        p = plot(x+xp,x+yp,'Color', cc(i,:),'DisplayName',dn,'LineWidth',5); hold on;
        recPlotList = [recPlotList,p];
    end  
    l = legend(gca,'show');
    set(l,'FontSize',14);
end