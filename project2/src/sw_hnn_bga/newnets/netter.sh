#!/bin/csh
# Lluís Belanche
#
# Crea les .net (n,h,r) necessàries per la HNN-1bis, de 'ini' a 'fi' neurones,
# tant per ID com per SIGM
#
# crida: netter.sh ini fi

# Tb la puc parametritzar per canviar la InputFun (SIMIL-ARIT-MEAN,...)

# $1 = ini
# $2 = fi

@ run = $1
while ( $run <= $2 )

  foreach tipus (n h r)
    foreach out (ID SIGM)
      set net = $run$tipus"_"$out".net"
      echo "; Esta es una red de una capa hidden" "("$run")" >> $net
      echo "" >> $net
      echo $run "; no. neuronas hidden totales" >> $net
      echo "1 ; no. capas hidden" >> $net
      echo "" >> $net

      if ($tipus == "h") then
        echo "HETEROGENEA"  >> $net
        echo "SIMIL-ARIT-MEAN" >> $net
        echo "LOGISTIC-JULIO" >> $net
      endif

      if ($tipus == "n") then
        echo "NORMAL"  >> $net
        echo "PROD-ESCALAR" >> $net
        echo "LOGISTIC" >> $net
      endif

      if ($tipus == "r") then
        echo "NORMAL"  >> $net
        echo "RBF-ESTANDAR" >> $net
        echo "GAUSSIANA" >> $net
      endif

      echo $run >> $net
      echo "" >> $net

      echo "NORMAL"  >> $net
      echo "PROD-ESCALAR" >> $net

      if ($out == "SIGM") then
        echo "LOGISTIC" >> $net
      endif

      if ($out == "ID") then
        echo "IDENTITY" >> $net
      endif
    end
  end
  
  @ run++
end
