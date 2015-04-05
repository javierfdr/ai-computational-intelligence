function analyzeDiscretization(data,dataVariableName,range, firstBatch)

    prefix = '2-';

    if firstBatch
        prefix = '1-';
    end

    %analyzing equal frequency partition
    partitions = [];
    for i=1:length(range)
        partition = equalFrecuencyPartition(range(i), data);
        partitions = [partitions,partition];
    end    
    plotMultiplePartitions(partitions,strcat(prefix,'EqualFrecuencyPartition on ',dataVariableName));

    %analyzing equal width partition
    partitions = [];
    for i=1:length(range)
        partition = equalWidthPartition(range(i), data);
        partitions = [partitions,partition];
    end    
    plotMultiplePartitions(partitions,strcat(prefix,'EqualWidthPartition on ',dataVariableName));
    
    %analyzing kmeans partitioning
    partitions = [];
    for i=1:length(range)
        partition = kmeansPartition(range(i), data);
        partitions = [partitions,partition];
    end    
    plotMultiplePartitions(partitions,strcat(prefix,'KMeans on ',dataVariableName));
    
    
end