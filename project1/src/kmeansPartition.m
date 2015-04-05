function partition = kmeansPartition(numGroups, data)
    niter = 100;
    partition = kmeans(data,numGroups,'MaxIter',niter,'Replicates',5,'EmptyAction','singleton');    
end