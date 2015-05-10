// El objetivo es que no tengan que hacerse tres .in distintos (n, h, f)
// Para eso hace falta el .cfg

#include <string.h>   //strlen
#include <ctype.h>
#include <stdlib.h>   //atof

#include "misc.h"
#include "tabla.h"
#include "data.h"
#include "entsal.h"
#include "misc.h"
#include "funneu.h"
#include "random.h"

// Chapussa per NDADES (ja ho arreglaré)
int NDADES;

///////// Taula de tipus de dades processats per les neurones
char*  TiposData_N[]= { "REAL", "FUZZY", "INTEGER", "VARLING", "NOMINAL", "BINARY", "ORDINAL", "SET", "BITSTRING", "PROBABILITY", "INTERVAL", "" };


///////// Taula de tipus de Learning Modes
char*  TablaLModes_N[]= { "CLASSIF", "REGRESS", "PREDICT", "" };

///////// Taula de tipus de Normalization Modes
char*  TablaNModes_N[]= { "NONE", "MINUSMEAN", "PROBEN", "STANDARD", "MIDRANG", "" }; 
 
extern char*  TablaError_N[];
extern ErrorFun TablaError_C[];

Real** probs; // això s'haurà de posar a Stats.

void Data::IniData (void)
{
  nDades= nIns= nOuts= 0;
  quants_missing=0;
  tipus= NULL;
  ins= outs= NULL;
  real_outs= NULL;
  error= FALS;
  datastats.M = datastats.m = NULL;
  datastats.mean = datastats.stddev = NULL;
  datastats.divi = datastats.rest = NULL;
  datastats.tmp = NULL;
  datastats.pmiss = NULL;
  probs = NULL;
  new_rang = NULL;
  for (int i=1; i<MAXCLASS; per_class[i++]=0);
}

// OJO: no se puede hacer inline
Data::Data ()
{
  IniData();
}

Data::~Data ()
{
  if (!EmptyData())
  {
    for (int i=1; i<=nDades; i++)
    {
      delete [] ins[i];
      delete [] outs[i];
    }

    delete [] ins;
    delete [] outs;
    delete [] real_outs;
    delete [] tipus;
  }
}

void Data::FiData (void)
{
  this->~Data ();
}

ParamH* Data::In (int i)
{
  return ins[i];
}

ParamH* Data::Out (int i)
{
  return outs[i];
}

// Lee los tipos del fichero de tipos. Si no existe no pasara
// nada. Sin embargo, si hay algun error en su contenido, se perderan
// los tipos que habia.

void Data::ReadConfigF (char nom[])
{ 
  int i, nIns_tmp, nOuts_tmp;

  f= fopen (nom, "r");
  if (f==NULL) error_and_exit ("abriendo el fichero", nom); 

// Leer numero de entradas y numero de salidas

  line= 1;
  if (!ReadIntF (f, &nIns_tmp, line, 1) || !ReadIntF (f, &nOuts_tmp, line, 1))
  {
    fclose(f);  
    error_and_exit ("Error leyendo la cabecera del fichero", nom);
  }

  nIns= nIns_tmp; nOuts= nOuts_tmp;
  
  //Poner los <tipos> de las entradas/salidas
  CheckMem (tipus = new TiposH[nIns+nOuts+1]);

  tipus[0]= REAL; // El bias es sempre REAL
  for (i=1; i<=nIns && !error; i++)
  {
    error= ReadTokenF (f, token, line);
    if (error) 
      error_and_exit ("Fin prematuro al leer los tipos, en la linea", line);
    else if (HiEs(TiposData_N,token))
              tipus[i] = TiposH(Nom2Index(TiposData_N,token));
    else error_and_exit ("Tipo desconocido: %s\n", token);
  }

  //Poner los <tipos> de las salidas
  for (; i<=nOuts; i++) tipus[i] = REAL;

  // Leer el tipo de preproceso
  
  error= ReadTokenF (f, token, line);
  if (error) 
     error_and_exit ("Fin prematuro al leer el nmode, en la linea", line);
  else if (HiEs(TablaNModes_N,token))
          nmode = N_MODE(Nom2Index(TablaNModes_N,token));
         else error_and_exit ("Tipo de preproceso desconocido", token);
	 
  // Leer el L_MODE (learning mode)

  error= ReadTokenF (f, token, line);
  if (error) 
    error_and_exit ("Fin prematuro al leer el lmode, en la linea", line);
  else if (HiEs(TablaLModes_N,token))
          lmode = L_MODE(Nom2Index(TablaLModes_N,token));
         else error_and_exit ("Tipo de LMODE desconocido", token);

  // Leer el Errorfun 

  error= ReadTokenF (f, token, line);
  if (error) 
     error_and_exit ("Fin prematuro al leer la ErrorFun, en la linea", line);
  else if (HiEs(TablaError_N,token))
          errorF = TablaError_C[Nom2Index(TablaError_N,token)];
         else error_and_exit ("Tipo de ErrorFun desconocido", token);

  // Llenar la tabla VectorSimilFun y de paso declarar la tabla donde quedarán las s_i (debería hacerse sólo si la neurona es HETEROGÉNEA)

  extern SimilFun* VectorSimilFun; // tabla de f. de similitud actuales, por input
  extern SimilFun SimilFunDefecto[]; // idem por defecto, por tipo
  extern Real* s;                  // tabla de similitudes parciales
  VectorSimilFun = new SimilFun[nIns+1];
  VectorSimilFun[0] = NULL;
  s = new Real[nIns+1];

  for (i=0; i<=nIns; i++)
    VectorSimilFun[i] = SimilFunDefecto[tipus[i]];

  strcpy (cfgfile, nom);
  fclose(f);  
}


void Data::ReadInputPattern (int i, int quants)
{
  for (int j=1; j<=quants && !error; j++)
  {
    error= ReadTokenF (f, token, line);
    if (!error)
      switch (tipus[j])
      {
        case REAL:
	  if (IsReal(token) || IsMissing(token)) 
	    {
	      if (IsMissing(token)) quants_missing++;
	      else ins[i][j].real = atof(token);
	      SetMissing (ins[i][j], IsMissing(token));
	    }
	  else error_and_exit ("REAL value expected at line/column", line, j);
          break;

        case FUZZY:
	  if (IsReal(token) || IsMissing(token)) 
	    {
	      if (IsMissing(token)) quants_missing++;
	      else 
		{
		  Real aux = atof(token);
		  ins[i][j].fn.mode= aux;
		  ins[i][j].fn.spread= PERC_FUZZY*aux;
		}
	      SetMissing (ins[i][j], IsMissing(token));
	    }
	  else error_and_exit ("FUZZY value expected at line/column", line, j);
          break;

      case VARLING:
      case INTEGER:
      case NOMINAL:
      case ORDINAL: 
      case BINARY:
	  if ((IsInteger(token) && atoi(token)>=1) || IsMissing(token)) 
	    {
	      if (IsMissing(token)) quants_missing++;
	      else ins[i][j].disc= Disc(atoi(token));
	      SetMissing (ins[i][j], IsMissing(token));
	      ins[i][j].joker = j;
	    }
	  else error_and_exit("DISCRETE value expected at line/column", line, j);
          break;

      case SET: 
                    break;
      case BITSTRING: 
                    break;
      case PROBABILITY: 
                    break;
      case INTERVAL: 
                    break;
      }
  }
}

// Los outputs se dejan en el campo .real siempre
void Data::ReadOutputPattern (int i, int quants)
{
  int check=0; // dice si hay *un solo 1* en los outputs
  Real aux=0;

  for (int j=1; j<=quants && !error; j++)
  {
    error= ReadTokenF (f, token, line);
    if (!error)
      {
	aux = atof(token);
	if (IsReal(token) || IsMissing(token)) 
	  {
	    if (IsMissing(token)) 
	      error_and_exit ("Missing value in output at line", line);
	    else outs[i][j].real = aux;
	    SetMissing (outs[i][j], false);
	  }
	else error_and_exit ("REAL value expected at line/column", line, j);
	if (lmode==CLASSIF)
	  if (aux==1) { per_class[j]++; check++; }
	  else if (aux!=0)
	    error_and_exit ("Not in 1-out-of-c CLASSIF format at line", line);
      }
    else error_and_exit("Fin prematuro al leer, en la linea", line);
  }
  if (lmode==CLASSIF && check!=1 && nOuts>1)
    error_and_exit ("Not in 1-out-of-c CLASSIF format at line", line);
}

void Data::ReadDadesF (void)
{
  for (int i=1; i<=nDades && !error;  i++) 
  {
    ReadInputPattern (i, nIns);
    ReadOutputPattern (i, nOuts);
  }

  switch (TR_MODE)
  { 
    case TR_VA_TE:
    case TR_K_FOLD:
             TR_left = 1; TR_right = nDades*PERCENT_TR/100;
             VA_left = TR_right+1; VA_right = TR_right+nDades*PERCENT_VA/100;
             TE_left = VA_right+1; TE_right = nDades;
             break;
    case TR_ONLY: 
             TR_left = 1; TR_right = nDades*PERCENT_TR/100;
             VA_left = VA_right = 0; 
             if (PERCENT_TR < 100) TE_left = TR_right+1; TE_right = nDades;
             break;
  }		       
				 
  if (!ReadTokenF (f, token, line))
    puts("WARNING: there are more examples than declared!\n");

  // Chapussa per NDADES (ja ho arreglaré)
  NDADES = nDades;
}

// Intercanvia 'quants' dades a partir de les posicions 'one' i 'two'
// Pre: 1<= one+quants <= nDades i 1<= two+quants <= nDades
void Data::DataSwap (int one, int two, int quants)
{
  int k; ParamH* tmp;

  if (one+quants-1 > nDades || two+quants-1 > nDades) puts("SHIT1 **********");
  if (one+quants-1 < 1 || two+quants-1 < 1) puts("SHIT2 **********");

  for (k=0; k<quants; k++)
  {
    // Swap ins
    tmp = ins[one+k];
    ins[one+k] = ins[two+k];
    ins[two+k] = tmp;

    // Swap outs
    tmp = outs[one+k];
    outs[one+k] = outs[two+k];
    outs[two+k] = tmp;
  }
}

/*
void Data::Clean (void)
{
  int i,j;

  for (i=1; i<=nDades; i++) 
    for (j=1; j<=abs(REALIM_DELAY); j++)
      ins[i][nIns-j+1].real = 0.0;
}*/

void Data::SetMissingROut (int i)
{
  for (int j=1; j<=abs(REALIM_DELAY); j++) 
    SetMissing(real_outs[i+abs(REALIM_DELAY)-1-j], true);      
}

void Data::AbocarRealim (int i)
{
  for (int j=1; j<=abs(REALIM_DELAY); j++)
    ins[i][nIns-j+1].real = real_outs[i+abs(REALIM_DELAY)-1-j].real;
}

void Data::SetRout(int i, Real res)
{
  real_outs[i+abs(REALIM_DELAY)-1].real = res;
}

// Solo toca los REAL, FUZZY 
void Data::Sweep_Stats (void)
{
  int i, j;

  for (i=1; i<=nDades; i++) 
    for (j=1; j<=nIns; j++)
      if (datastats.divi[j]!=0)
	switch (tipus[j])
	  {
	  case REAL:  if (!IsMissing(ins[i][j])) 
	    ins[i][j].real = (ins[i][j].real - datastats.rest[j])/datastats.divi[j];
	    break;
	  case FUZZY: if (!IsMissing(ins[i][j])) 
	    {
	      ins[i][j].fn.mode = (ins[i][j].fn.mode - datastats.rest[j])/datastats.divi[j];
	      ins[i][j].fn.spread = PERC_FUZZY*ins[i][j].fn.mode;
	    }
	    break;
	  default: break;
	  }

  for (j=1; j<=nIns; j++) 
    if (tipus[j]==REAL || tipus[j]==FUZZY)
      if (datastats.divi[j]!=0)
	{
	  datastats.m[j] = (datastats.m[j] - datastats.rest[j])/datastats.divi[j];
	  datastats.M[j] = (datastats.M[j] - datastats.rest[j])/datastats.divi[j];
	  datastats.mean[j] = (datastats.mean[j] - datastats.rest[j])/datastats.divi[j];
	  datastats.stddev[j] /= datastats.divi[j];
	}

}

void Data::Calc_Stats (void)
{
  int i, j, rang_; Real sum; ParamH aux;

  // Initializations

  datastats.overall_var = 0.0;

  for (j=1; j<=nIns+nOuts; j++)
  {
    datastats.tmp[j] = 0;
    datastats.M[j] = -MAXFLOAT;  // Problemes si Real=float? (per no existir 
    datastats.m[j] = MAXFLOAT;   // aquest numero)
    datastats.stddev[j] = 0.0;
    datastats.mean[j] = 0.0;
  }


  // First pass: M, m, mean (s'assumeix que les sortides no son HETEROG)

  for (j=1; j<=nIns+nOuts; j++)
    switch (tipus[j])
      {
        case REAL:   
	  for (i=1; i<=nDades;  i++) 
	    {
	      if (j<=nIns) aux = ins[i][j]; else aux = outs[i][j-nIns];
	      if (!IsMissing(aux))
	      {             
		datastats.tmp[j]++; datastats.mean[j] += aux.real;
		if (aux.real > datastats.M[j]) datastats.M[j] = aux.real;
		if (aux.real < datastats.m[j]) datastats.m[j] = aux.real;
	      }
	    }
	  break;
        case FUZZY:  
	  for (i=1; i<=nDades;  i++) 
	    if (!IsMissing(ins[i][j])) 
	      {             
		Real mode_ = ins[i][j].fn.mode;
		datastats.tmp[j]++; datastats.mean[j] += mode_;
		if (mode_ > datastats.M[j]) datastats.M[j] = mode_;
		if (mode_ < datastats.m[j]) datastats.m[j] = mode_;
	      }
	  break;
        case INTEGER: 
        case NOMINAL:
        case ORDINAL:
        case BINARY: 
	  for (i=1; i<=nDades;  i++) 
	    {
	      if (j<=nIns) aux = ins[i][j]; else aux = outs[i][j-nIns];
	      if (!IsMissing(aux))
		{
		  datastats.tmp[j]++; //datastats.mean[j] += aux.disc;
		  if (aux.disc > datastats.M[j]) datastats.M[j] = aux.disc;
		  if (aux.disc < datastats.m[j]) datastats.m[j] = aux.disc;
		}
	    }
	  break;
      case SET: 
                    break;
      case BITSTRING: 
                    break;
      case PROBABILITY: 
                    break;
      case INTERVAL: 
                    break;
      case VARLING: break;
      }

  // Comprovació de columnes constants

  for (j=1; j<=nIns+nOuts; j++)
    if (datastats.M[j]==datastats.m[j])
      printf ("La columna %d és CONSTANT!\n", j);

  // probs serveix per calcular les probabilitats de cada valor DISCRET (només pels Inputs)
  probs[0] = NULL;
  for (j=1; j<=nIns+nOuts; j++)
    switch (tipus[j])
	{
	case FUZZY:
        case REAL:  datastats.mean[j] /= datastats.tmp[j];
           	    probs[j] = NULL;
                    break;
        case INTEGER: 
        case NOMINAL:
        case ORDINAL:
        case BINARY: rang_ = Disc(datastats.M[j]);
                     CheckMem (probs[j] = new Real[rang_+1]);
                     for (i=1; i<=rang_; i++) 
		       probs[j][i] = 0.0;
		     //		     datastats.mean[j] /= datastats.tmp[j];
                     break;
        default: break;
      }

  // Second pass: stddev, probs

  for (i=1; i<=nDades;  i++) 
    for (j=1; j<=nIns; j++)
      switch (tipus[j])
	{
        case REAL:  if (!IsMissing(ins[i][j]))
                    {
                      Real aux_ = ins[i][j].real-datastats.mean[j];
                      datastats.stddev[j] += aux_*aux_;
                    }
                    break;
        case FUZZY: if (!IsMissing(ins[i][j]))
                    {
                      Real aux_ = ins[i][j].fn.mode-datastats.mean[j];
                      datastats.stddev[j] += aux_*aux_;
                    }
                    break;
        case INTEGER: 
        case NOMINAL:
        case ORDINAL:
        case BINARY: 
	  if (!IsMissing(ins[i][j])) 
	    probs[j][ins[i][j].disc] += 1.0; 
	  break;       
        default: break;
	}
  

  // Finalitzacions
  for (j=1; j<=nIns; j++)
    {
      datastats.pmiss[j] = 1.0-Real(datastats.tmp[j])/Real(nDades);
      switch (tipus[j])
	{
        case REAL:  
        case FUZZY:     
	  datastats.stddev[j] = sqrt(datastats.stddev[j]/(datastats.tmp[j]-1));
	  break;
        case INTEGER: 
        case NOMINAL:
        case ORDINAL:
        case BINARY:
	  // Comprovació rutinària de consistència
	  sum = 0.0; rang_ = Disc(datastats.M[j]);

	  for (i=1; i<= rang_; i++) 
	    sum += probs[j][i];

	  if (sum!=datastats.tmp[j]) 
	    error_and_exit ("Error intern al càlcul del vector 'probs' per la variable", (long unsigned int)j);

	  for (i=1; i<= rang_; i++) 
	    probs[j][i] /= datastats.tmp[j];
	  break;       
        default: break;
	}
    }


  for (i=1; i<=nDades;  i++) 
    for (j=1; j<=nOuts; j++)
      datastats.stddev[j+nIns] += 
	(outs[i][j].real-datastats.mean[j])*(outs[i][j].real-datastats.mean[j]);
	
  for (j=nIns+1; j<=nOuts; j++)
    datastats.stddev[j] = sqrt(datastats.stddev[j]/(datastats.tmp[j]-1));

  // OJO: esto está puesto sólo para la 1ª salida!!!!!
  datastats.overall_var = datastats.stddev[nIns+1]*datastats.stddev[nIns+1];
}


// 1. Passar les dades d'entrada per un preprocés de normalització
// 2. Estandaritzar les dades de sortida si estem en regressió
// 3. Recalcular estadístiques i posar els (nous) rangs pel BGA
void Data::Preprocess (void)
{
  int i,j;

  Calc_Stats ();

  switch (nmode)
  {
    case MINUSMEAN: 
      for (j=1; j<=nIns; j++) 
	{
	  datastats.divi[j] = 1.0;
	  datastats.rest[j] = datastats.m[j];
	}
      Sweep_Stats ();
      break;

    case PROBEN:  // passa a [0,1] dividint per la maxima desviació
      for (j=1; j<=nIns; j++) 
	{
	  datastats.divi[j] = datastats.M[j]-datastats.m[j]; 
	  datastats.rest[j] = datastats.m[j];
	}
      Sweep_Stats ();
      break;

    case STANDARD: // la z-standardització 
      for (j=1; j<=nIns; j++) 
	{
	  datastats.divi[j] = datastats.stddev[j];
	  datastats.rest[j] = datastats.mean[j];
	}
      Sweep_Stats ();
      break;

    case MIDRANG: 
      for (j=1; j<=nIns; j++) 
	{
	  datastats.divi[j] = (datastats.M[j]-datastats.m[j])*0.5; 
	  datastats.rest[j] = (datastats.M[j]+datastats.m[j])*0.5; 
	}
      Sweep_Stats ();
      break;

    case NONE:    break;
  }

  // 2.

  if (lmode==REGRESS)
  for (i=1; i<=nDades; i++) 
    for (j=1; j<=nOuts; j++)
      outs[i][j].real = (outs[i][j].real - datastats.mean[j+nIns])/datastats.stddev[j+nIns];

  for (j=1; j<=nOuts; j++)
    {
      datastats.mean[j+nIns] = 0.0;
      datastats.stddev[j+nIns] = 1.0;
    }

  // 3.

  for (j=1; j<=nIns; j++) 
  {
    new_rang[j].min = datastats.m[j];
    new_rang[j].max = datastats.M[j];
  }

}


void Data::Ini_DataStats (int n)
{
  CheckMem (datastats.M = new Real[n]);
  CheckMem (datastats.m = new Real[n]);
  CheckMem (datastats.mean = new Real[n]);
  CheckMem (datastats.stddev = new Real[n]);
  CheckMem (datastats.divi = new Real[n]);
  CheckMem (datastats.rest = new Real[n]);
  CheckMem (datastats.tmp = new int[n]);
  CheckMem (datastats.pmiss = new Real[n]);
  CheckMem (probs = new Real*[n]);
}


/***********************************************************************
Si algun fichero no existe o hay algun error en el fichero de
configuracion (de cualquier indole), no se pierde nada. Sin embargo,
si hay un error en el *contenido* del fichero de datos, los datos
antiguos se perderan (se devuelven datos vacios).

Nota: si se corresponden los nIns y nOuts bien, y se le pide que lea
MENOS patrones de los que realmente hay, lo hara bien.
*************************************************************************/

// Falta controlar los EOFs intermedios y limpiar un poco, p.e. lo
// del 'too long' y los errores
// Falta la check (..., SLIGHT) que fa el return ... per la mem. demanada
// OJO: Asume que los tipos ya estan leidos (nIns, nOuts y TiposH)

void Data::ReadData (char nom[])
{
  int i, c, nDades_tmp;

  f= fopen (nom, "r");
  if (f==NULL) error_and_exit ("Error abriendo el fichero", nom); 

// Ignorar posibles comentarios: un comentario es toda linea empezada
// por COMMENT_SYMBOL y acabada en '\n'. 

  line= 1;
  c= SkipCommentsF(f, line);
  ungetc(c, f);

// Leer numero de datos

  if (!ReadIntF (f, &nDades_tmp, line, 1))
  {
    fclose(f);  
    error_and_exit ("Error en la cabecera del fichero de datos", nom);
  }

// Llegados aqui, los datos antiguos (si los habia) se perderan
// Reinicializar si hace falta
  if (!EmptyData()) 
  { 
      FiData();           
      IniData(); 
  }

// reservamos espacio para los <vectores> de entrada/salida

  nDades= nDades_tmp; 
  ins = new ParamH*[nDades+1];
  CheckMem (ins);
  outs = new ParamH*[nDades+1];
  CheckMem (outs);
  // mecanismo de delays realimentados
  #if (REALIM_DELAY!=0)
  {
    real_outs = new ParamH[nDades+abs(REALIM_DELAY)];
    CheckMem (real_outs);
    // poner a X las salidas anteriores a REALIM_DELAY
    // OJO: se asume que la salida es REAL
    for (i=0; i<abs(REALIM_DELAY); i++)
      SetMissing(real_outs[i].real);
  }
  #endif
  ins[0]= outs[0]= NULL;
  
  for (i=1; i<=nDades; i++)
  {
    ins[i]  = new ParamH[nIns+1];      // +1 pel bias
    CheckMem (ins[i]);
    outs[i] = new ParamH[nOuts+1];     // +1 pel bias
    CheckMem (outs[i]);
  }
  Ini_DataStats (nIns+nOuts+1);
  CheckMem (new_rang = new Rang[nIns+1]);

  printf ("Loading %s ... ",  nom);
  ReadDadesF ();
  puts ("OK");
  strcpy (infile, nom);
  Preprocess ();
  fclose(f);
}

void Data::ShowData () const
{
  int i,j;

  if (IO_MODE>=FAIR)
  {
    printf("Ndades: %d\nNInputs: %d\nNOutputs: %d\n", nDades, nIns, nOuts);
  
    printf("\nLMode: %s", TablaLModes_N[lmode]);

    if (lmode==CLASSIF)
      for(i= 1; i<=nOuts; i++)
        printf("  %d (%3.3f%%)", per_class[i], 
                100*Real(per_class[i])/Real(nDades));
    printf("\nMissing: %d de %d (%3.3f%%)\n",
         quants_missing, nIns*nDades, 
         100*Real(quants_missing)/Real(nIns*nDades));

    puts("\nVariable     Type    #Miss  Pmiss     Max      Min    Mean    Stddev");
    for (j=1; j<=nIns; j++)
      {
	printf ("  %2d. %12s  %4d  %7.3f  %7.3f  %7.3f", j, TiposData_N[tipus[j]],
	  nDades-datastats.tmp[j], datastats.pmiss[j], datastats.M[j], datastats.m[j]);
	if (tipus[j]==REAL || tipus[j]==FUZZY)
	  printf (" %7.3f  %7.3f\n", datastats.mean[j], datastats.stddev[j]);
	else puts("    -      -");
      }
  }

// Es podria fer amb printParamH pero hi ha problemes de format (el"| ")
// Ja ho arreglaré 
  if (IO_MODE>=VERBOSE)
    for(i= 1; i<=nDades; i++) 
    {
      for (j=1; j<=nIns; j++)
        switch (tipus[j])
        {
          case REAL:   if (IsMissing(ins[i][j])) 
                         printf ("  %s   ", MISSING_SYMBOL);
                       else printf (FORMAT_PANTALLA, ins[i][j].real);
                       break;
          case FUZZY:  if (IsMissing(ins[i][j])) 
                         printf ("  %s   ", MISSING_SYMBOL);
                       else { 
                              printf (FORMAT_PANTALLA, ins[i][j].fn.mode);
                              printf (FORMAT_PANTALLA, ins[i][j].fn.spread);
		            }
                       break;
          case VARLING:if (IsMissing(ins[i][j])) 
                         printf ("  %s   ", MISSING_SYMBOL);
                       else { 
                              printf (FORMAT_PANTALLA, ins[i][j].fq.mode1);
                              printf (FORMAT_PANTALLA, ins[i][j].fq.spread1);
                              printf (FORMAT_PANTALLA, ins[i][j].fq.mode2);
                              printf (FORMAT_PANTALLA, ins[i][j].fq.spread2);
		            }
                       break;
          case INTEGER: 
          case NOMINAL:
          case BINARY: if (IsMissing(ins[i][j]))
                         printf ("  %s   ", MISSING_SYMBOL);
                       else printf ("%d ", ins[i][j].disc);
                       break;
	case ORDINAL: 
                    break;
	case SET: 
                    break;
	case BITSTRING: 
	  break;
      case PROBABILITY: 
                    break;
      case INTERVAL: 
                    break;
	}
      printf ("| ");
      for (j=1; j<=nOuts; j++)
      if (IsMissing(outs[i][j]))
            printf ("  %s   ", MISSING_SYMBOL);
      else  printf (FORMAT_PANTALLA, outs[i][j].real);
      printf ("\n");
    }
}

// Llena de missing los datos (el perc por ciento). 
// Toma en cuenta los que ya pudiera haber.
void Data::Missinger (int perc)
{
  uint fila, columna;
  int qmporto;

  // Antes de empezar hay ya 'quants_missing'
  // Calcular cuántos se necesitan entonces
  qmporto = int(perc*NDades()*NIns()/100) - quants_missing;
  if (qmporto<0) 
    error_and_exit ("Ya hay más missing de los que se quieren poner."); 
  if (qmporto==0) return;

  do
    {
      fila = randint (1, NDades());
      columna = randint (1, NIns());

      if (!IsMissing (ins[fila][columna]))
	{
	  SetMissing (ins[fila][columna], true);
	  qmporto--;
	}
    }
  while (qmporto>0);
  Calc_Stats ();
}
