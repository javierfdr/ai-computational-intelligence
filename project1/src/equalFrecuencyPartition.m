function partition = equalFrecuencyPartition(numGroups, data)
    partition = ceil(numGroups * tiedrank(data) / length(data));
end