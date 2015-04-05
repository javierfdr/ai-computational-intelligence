% calculates partition quality as the average of the difference
% of the sum difference of coverage percentage between each sample
function quality = getPartitionQuality(partition)
    numPartitions = length(unique(partition));
    totalValues = length(partition);
    
    sizeArray = [];
    for p=1:numPartitions
        sizeArray=[sum(partition==p);sizeArray];
    end
    
    quality = 0;
    for p=1:numPartitions-1
        for p2=p+1:numPartitions
            cov1 = (sizeArray(p)/totalValues)*100;
            cov2 = (sizeArray(p2)/totalValues)*100;
            quality = quality+(abs(cov1-cov2));
        end
    end
end