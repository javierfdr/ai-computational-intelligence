function partition = equalWidthPartition(numGroups, data)
    width = (max(data)-min(data))/numGroups;
   
    m = min(data);
    interval = [m];
    for i=1:numGroups-1
        interval = [interval,m+(i*width)];
    end
    interval = [interval,max(data)]
   
    partition = [];
    for i=1:length(data)
        bin = 1;
        for in=1:length(interval)
            if (in+1>length(interval))
                break;
            end

            if (data(i)>= interval(in) && data(i)<=interval(in+1))
                partition = [partition,bin];
                break;
            end
            bin = bin+1;
        end
    end
end