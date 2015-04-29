// Principal
#include <stdio.h>
#include <string.h> // srtcat

#include "config.h"
#include "defs.h"
#include "random.h"
#include "data.h"
#include "funneu.h"
#include "neurona.h"
#include "entsal.h"
#include "breeder.h"
#include "partition.h"
#include "misc.h"

extern int count;
extern int count_recombin;

/*******************************************************

 * Primero se ha de leer el .cfg
   Luego, en no importa que orden, el .re (la red) y el .in (los datos)
   Estos se pueden ir cambiando sin ningun problema mientras no cambie el .cfg
   (es decir, mientras no cambien los tipos, no. entradas y/o salidas) 

 * Llamada: HNN-1bis.exe <.cfg> <.in> <.net> mu tau FFEvals NRuns

********************************************************/ 


int main (int argc, char *argv[])
{
  Data d;
  Red r;
  BGASetup setup;

  if (argc!=9) error_and_exit ("Número de paràmetres incorrecte.");

  d.ReadConfigF (argv[1]);
  puts ("Configuración leída OK");
  
  d.ReadData (argv[2]);
  puts ("Datos leídos OK"); 
  d.ShowData ();


  //  puts("OK"); exit(0);

  // Le hacen falta los datos (d) por los tipos, no. entradas y salidas
  r.ReadRed (argv[3], FALS, d);
  puts ("Red leída OK");

  // Agafar els parametres (path, mu, tau, FFEvals, NRuns)
  strcpy (setup.path_out, argv[4]);
  int mu = atoi(argv[5]);
  setup.trunc = atoi(argv[6]);
  setup.FFevals = atoi(argv[7]);
  setup.NRuns = atoi(argv[8]);
  
  ini_random ();  

  BGA algo (mu, 0, r.Npesos()); 
  algo.DataExternalSetup (&r, &d);

  if (TR_MODE==TR_K_FOLD)
  {
    // Guardarem els resultats totals (mean, best-fold)

    Real mean = 0.0, tmp, best = MAXFLOAT;
    int best_fold;

    // Executar el Partition Magic

    int k;
    IniPartition (&d);

    for (k=0; k < 2*K_FOLD; k++)    
    {       
      ShowPartition (); 
      //      d.ShowData ();
      
      algo.Setup (&setup);
      algo.Exec  ();
      if ( (tmp = algo.ShowResults ()) < best) { best = tmp; best_fold = k+1; }
      mean += tmp;

      SwapPartition (&d, SwapList[2*k][0], SwapList[2*k][1]);
      SwapPartition (&d, SwapList[2*k+1][0], SwapList[2*k+1][1]);
    }

    // Mostrar els resultats totals
    printf ("Mean NMSE (TOTAL FOLDS) %g; best %g (fold %d)\n", 
	    mean/10.0, best, best_fold); // esto hay que mirarlo
  }
  else // Una execució normal (un sol TR/TE ó TR/VA/TE)
  { 
    algo.Setup (&setup);
    algo.Exec  ();
    printf ("Mean %s in TE is %g\n", 
	    d.LMode()==CLASSIF ? "Accuracy" : "NMSE", algo.ShowResults ()); 
  }
}

