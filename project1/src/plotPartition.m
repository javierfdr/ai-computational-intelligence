function circlePlot = plotPartition(partition)
    figure();
    
    parts = unique(partition)
    for i=1:length(parts)
        radius = sum(partition==i)
        [x,xp,yp] =circle(0,0,radius);
        plot(x+xp,x+yp,'Color', [rand() rand() rand()]); hold on;
    end

end