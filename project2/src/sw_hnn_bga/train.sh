#!/bin/csh
# Trainer for the HNN-1
#
# Lluís Belanche
# belanche@lsi.upc.edu
# March 2013

# $1 = problems to test (a list, must match the directory names where the problems are)
# $2 = ID (for regression) or SIGM (for classification)
# $3 = directory (within the directory where the problem is) to write the results files
# $4 to $7 = mu tau FFEvals NRuns (BGA parameters)
# $8 = neuron types (h for the HNN, n for the MLP, and r for the RBF networks)
# $9 = neurons to test (a list)

# Must set: 
#
# p_data = path where the data is read from (.in, .cfg files)
# p_nets = path where the networks are read from (.net files)

set p_data = $HOME/WORK-UPC/Software/HNN-1-BGA/NUEVOARRANQUE/Datasets
set p_nets = $HOME/WORK-UPC/Software/HNN-1-BGA/NUEVOARRANQUE/newnets


# if the neuron type is 'n' or 'r', we read the '_n.in' and '_n.cfg' files

set typein = n
if ($8 == "h") then
  set typein = h
endif

foreach data ($1)

  foreach h ($9)

   set prefix  = ""
   if ($h < 10) then
      set prefix  = "0"
   endif

   set xarxa    = $prefix$h$8"_"$2
   set p_out    = $p_data"/"$data"/"$3"/"$xarxa
   set file_net = $p_nets"/"$xarxa".net"
   set file_cfg = $p_data"/"$data"/"$data"_"$typein".cfg"
   set file_in  = $p_data"/"$data"/"$data"_"$typein".in"
   set file_out = $p_out"/resul-"$xarxa".info"
   mkdir -p $p_out

   ./HNN-1bis-linux.exe $file_cfg $file_in $file_net $p_out $4 $5 $6 $7 > $file_out
end 
