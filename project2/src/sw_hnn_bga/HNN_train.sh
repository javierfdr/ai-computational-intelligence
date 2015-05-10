#!/bin/csh
# Trainer for the HNN-1
#
# Lluís Belanche
# belanche@lsi.upc.edu
# March 2013

# $1 = problem name (must match the directory name where the problem is)
# $2 = initial number of neurons
# $3 = final number of neurons 
# $4 = ID (for regression) or SIGM (for classification)
# $5 = directory (within the directory where the problem is) to write the results files

@ hiddens = $2
while ( $hiddens <= $3 )

  foreach neu (n h r)

    ./train.sh $1 $4 $5 600 10 20000 10 $neu `expr $hiddens \* 2`
  end

  @ hiddens ++

end


