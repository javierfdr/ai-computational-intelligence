// neurona.cc

#include <stdio.h>
#include "neurona.h"
#include "entsal.h"
#include "tabla.h"
#include "misc.h"
#include "funneu.h"

// TOT esta adaptat a qualsevol numero de capes fins a MAXCAPA, menys EVALCAPA

extern char*  TablaNetI_N[];
extern InputFun TablaNetI_C[];
extern char*  TablaActiv_N[];
extern ActivFun TablaActiv_C[];
char*  TablaNeus_N[]= { "NORMAL", "HETEROGENEA", "FULLFUZZY", "" };


Capa::Capa (void)
{
  ini= fi= 0;
}

void Capa::Set (TipoNeurona t, int i, int f, InputFun in, ActivFun act)
{
  if (f>=i && i>=1)
  {
    tipo_neu= t; ini=i; fi= f; inputF= in; activF= act; 
  }
  else { printf("Limits de capa invalids: %d, %d\n", i, f); exit(0); }
}

void Capa::EvalCapa (ParamH* neus[], ParamH in[], ParamH out[])
{
  int i, j;

  for (i=ini, j=1; i<=fi; )
    {
      //      printParamH (in, nins, tipo_neu, tipos_in);
      out[j++].real= activF(inputF(in, neus[i++], nins));
    }
}


//*************************************************************
// RED.CC
//
// TODAS las tablas empiezan desde 1 (les 'capes' i 'neurones')
//*************************************************************

void Red::Init (void) 
{ 
  neurones= NULL;
  tmp= resultat= NULL;
  nneurones= ncapes= npesos= 0;
  error_IO= FALS;
  weights_loaded= FALS;
  softmax=FALS;
}

Red::Red () 
{ 
  Init();
}

Red::~Red ()
{
  if (neurones)
  {
    //DiscardWeights();
    delete [] neurones;
    delete [] resultat;
    delete [] tmp;
    Init();
  }
}

void Red::ReInit (void) 
{
  if (neurones)
  {
    DiscardWeights();
    delete [] neurones;
    delete [] resultat;
    delete [] tmp;
    Init();
  }
}

// Creo que ya no es util, aparte de al cerrar
void Red::DiscardWeights (void)
{
  for (int i=1; i<=nneurones; i++)
    delete [] neurones[i];
  weights_loaded = FALS;
}

void Red::SetWeight (int i, ParamH* w, int n)
// Pre: la memoria para la neurona 'i' ya fue pedida
{
  for (int j=0; j<=n; j++)  // el bias esta en [0]
    neurones[i][j] = w[j];
}

ParamH* Red::GetWeight (int i)
// No lo copia: solo devuelve el *
{
  return neurones[i];
}

/* A partir de un "cromosoma" w, donde los pesos de las n neuronas
   estan consecutivos, pone en la red esos pesos, copiando solo los n
   punteros */
void Red::SetWeightDyn (ParamH* w)
{
  int i,j,aux;

  for (i=1; i<=ncapes; i++)
  {
    aux = capes[i].Nins()+1;     // +1 pel bias
    for (j= capes[i].Ini(); j<=capes[i].Fi(); j++)
    {
      neurones[j]= w;
      w += aux; 
    } 
  }
}

/* A partir de los nuevos rangos producto de la normalizacion
[new_rang_min, new_rang_max][j], se los pone a la red del BGA para las
variables de la 1ª capa.  Para el resto de capas se deja lo que habia
segun el default de ErrorRed en FFs.cc (e.g. [-10, 10]). */

// OJO: esto asume que hay una (la 1ª) o ninguna capa HETEROGÉNEA
// En el futuro poner #define MAXWEIGHT 10.0 (uso tb en ElsProblemes)

void Red::Omple_Rangs_Tipus (Rang destR[], Rang sourceR[], 
                             TiposH destT[], TiposH sourceT[])
{
  int i,j,k,h,aux;

  j=0;

  // Capa 1
  
  if (capes[1].TipoNeu()==HETEROGENEA) 
    for (k=1; k<=capes[1].Nneus(); k++)
      {
	// el bias 
	destR[j].min = 0.0;
	destR[j].max = 1.0;
	destT[j] = REAL;
        j++;

	// el resto
	for (i=1; i<=capes[1].Nins(); i++, j++)
	  {
	    destR[j] = sourceR[i];
	    destT[j] = sourceT[i];
	  }
      }

  // ********revisarlo************
  if (capes[1].GetNetI()==SimilProdEscalar) 
    for (j=0; j<= (capes[1].Nins()+1)*capes[1].Nneus(); j++)
    {
      destR[j].min = 0.0;
      destR[j].max = 1.0;
    }
  

  // Aqui se podría aprovechar para poner [0:10] en vez de [-10:10]
  // en caso de RbfEstandar (el sigma)

  // Capas > 1: se asume son ya todas NORMALES 
//   for (h=2; h<=ncapes; h++)
//   {
//     aux = capes[h].Nins();
//     for (k=1; k<=capes[h].Nneus(); k++)
//       for (i=1; i<=aux; i++, j++)
// 	{
//           destR[j].min = 0.0;
//           destR[j].max = 1.0;
// 	}
//  }
}

void Red::PrintRed (void)
{
  int i,j;

  for (i=1; i<=ncapes; i++)
    for (j= capes[i].Ini(); j<=capes[i].Fi(); j++)
      printParamH (GetWeight(j), capes[i].Nins(), capes[i].TipoNeu(), capes[i].TiposIn());
}

// Se asume que hay dos capas, para los EXPERIMENTOS
// El resultat queda a 'resultat'
void Red::EvalRed (ParamH in[])
{
  /*
  if (ncapes==2)
  {
    capes[1].EvalCapa (neurones, in, tmp);
    capes[2].EvalCapa (neurones, tmp, resultat);
  }
  else capes[1].EvalCapa (neurones, in, resultat); */

  capes[1].EvalCapa (neurones, in, tmp);
  capes[2].EvalCapa (neurones, tmp, resultat);

  /*
  for (int i=2; i<=ncapes; i++)
  {
    capes[i].EvalCapa (neurones, resultat, tmp);
    CopyParamH (resultat, tmp, capes[i].Nneus()); // OJO con esta funcion (ver entsal.cc (last)
  } */
}

void process_softmax (ParamH res[], int n)
{
  int i=n;
  Real r=0.0;

  for (; n ; n--) r += res[n].real;
  for (; i ; i--) res[i].real /= r;
}

// El no. de salidas de la red es el no. de neus de la ultima capa
Real Red::error_patroF (ParamH out[])
{
  int outs = capes[ncapes].Nneus();
  if (softmax) process_softmax (resultat, outs);

  return errorF (out, resultat, capes[ncapes].Nneus());
}

// Al fitxer es: MSE NMSE [%]
void Red::TestRedData (Data* data, int left, int right, Bool Mplot, FILE* fM)
{
  int quants = (right-left+1);

  hits=0; error=0.0;

 // mecanismo de delays realimentados
#if (REALIM_DELAY!=0)
    // poner a X las salidas anteriores a REALIM_DELAY (w.r.t. left)
    data->SetMissingROut (left);
#endif

  for (int i=left; i<=right; i++)
  {
#if (REALIM_DELAY!=0)
    // poner a su valor las salidas reales anteriores que se realimentan
    data->AbocarRealim (i);
#endif

    EvalRed (data->In(i)); // el resultat queda intern a 'resultat'

#if (REALIM_DELAY!=0)  // guardar el resultat
      data->SetRout(i, resultat[1].real);
#endif

    error += error_patroF (data->Out(i));
    if (data->LMode()==CLASSIF && IsAHit(resultat, data->Out(i), data->NOuts())) 
       hits++;
  }

  mse = error/(quants*data->NOuts());
  // GetVar = Varianca de les sortides. 
  // OJO: esto debiera hacerse para cada salida por separado
  nmse = mse/data->GetVar();  
  if (Mplot) fprintf (fM, "%3.4f  %3.4f", mse, nmse);
  if (data->LMode()==CLASSIF)
  {
    hitsP = 100.0*Real(hits)/Real(quants);
    if (Mplot) fprintf (fM, "  %3.2f%%\n", hitsP);
      // fprintf (fM, "Accuracy = %d de %d (%3.2f%%)\n", hits, quants, hitsP);
  }
  else if (Mplot) fprintf (fM, "\n");
}

void Red::PlotOutputs (Data* data)
{
  f= fopen ("outputs.gpt", "a");
  for (int i=1; i<=data->NDades(); i++)
  {
    EvalRed (data->In(i)); // el resultat queda intern a 'resultat'
    fprintf (f,"%d ", i);
    for (int j=1; j<=data->NOuts(); j++)
      fprintf (f,"%g ", resultat[j].real);
    fprintf (f,"\n");
  }
  fclose (f);
}

void Red::PlotHidden (Data* data)
{
  #define MENOR 0.2
  #define MAJOR 0.8

  int i,j, h=capes[1].Nneus();
  Real* mitjes = new Real[h+1];
  int* menors = new int[h+1];
  int* majors = new int[h+1];

  if (ncapes==1) PlotOutputs (data);
  else 
  {
    f= fopen ("hidden.gpt", "w");
    for (j=1; j<=h; j++) 
    {
      mitjes[j] = 0.0;
      menors[j] = 0;
      majors[j] = 0;
    }
      
    for (i=1; i<=data->NDades(); i++)
    {
      capes[1].EvalCapa (neurones, data->In(i), tmp); 
      //      fprintf (f,"%d ", i);
      for (j=1; j<=h; j++)
      {
	//        fprintf (f,"%g ", tmp[j].real);
        mitjes[j] += tmp[j].real;
        if (tmp[j].real <= MENOR) menors[j]++;
        if (tmp[j].real >= MAJOR) majors[j]++;
      }
      fprintf (f,"\n");
    }
    for (j=1; j<=h; j++) mitjes[j] /= data->NDades();
    fprintf (f,"Mitjes");
    for (j=1; j<=h; j++) 
      fprintf (f,"%g ", mitjes[j]);
    fprintf (f,"\n");
    fprintf (f,"Majors");
    for (j=1; j<=h; j++) 
      fprintf (f,"%d ", majors[j]);
    fprintf (f,"\n");
    fprintf (f,"Menors");
    for (j=1; j<=h; j++) 
      fprintf (f,"%d ", menors[j]);
    fprintf (f,"\n");

    fclose (f);
  }
}

void Red::ReadWeightsF (void)
{
  char token[MAXLINE]; 
  int i,j,k, ninsy;
  TiposH tipy;

  for (i=1; i<=ncapes; i++)
  {
   ninsy= capes[i].Nins();  // no. d'entrades a la capa
   for (j= capes[i].Ini(); j<=capes[i].Fi() && !error_IO; j++)
   {
    for (k= 0; k<=ninsy && !error_IO; k++)
    {
     error_IO= ReadTokenF (f, token, line); 
     tipy= capes[1].TiposIn()[k];
     if (!error_IO)
       switch (tipy)
       {
         case REAL:
	   if (IsReal(token) || IsMissing(token)) 
	     {
	      if (!IsMissing(token)) tmp[k].real = atof(token);
	      SetMissing (tmp[k], IsMissing(token));
	    }
	   else error_and_exit ("REAL value expected at line/column", line, k);
           break;
         case FUZZY:
	  if (IsReal(token) || IsMissing(token)) 
	    {
	      if (!IsMissing(token)) 
		{
		  tmp[k].fn.mode = atof(token);
                  error_IO= ReadTokenF (f, token, line); 
                  if (!error_IO) tmp[k].fn.spread= atof(token);
		}
	      SetMissing (tmp[k], IsMissing(token));
	    }
	  else error_and_exit ("FUZZY value expected at line/column", line, k);
	  break;
      case INTEGER:
      case NOMINAL:
      case BINARY:
	  if ((IsInteger(token) && atoi(token)>=1) || IsMissing(token)) 
	    {
	      if (!IsMissing(token)) tmp[k].disc= Disc(atoi(token));
	      SetMissing (tmp[k], IsMissing(token));
	    }
	  else error_and_exit("DISCRETE value expected at line/column", line, k);
	  break;

       default: error_and_exit("Error interno: Red::ReadWeightsF\n");
       }
     else error_and_exit("Fin prematuro al leer, en la linea", line);
    }
    //    printf ("Poniendo la neurona %d a:\n", j);
    //    printParamH (tmp, ninsy, capes[i].TipoNeu()==NORMAL);
    SetWeight (j, tmp, ninsy);
   }
  }
}

// Assumeix una o cap capa hidden en els printf inicials
void Red::SaveRed (char path[], Data* d)
{
  int i,j;

  f= fopen (path, "w");
  if (f==NULL) error_and_exit ("Error abriendo el fichero: %s\n", path); 

  fprintf (f,"; Fitxer: %s\n", path);
  fprintf (f,"; Esta es una red: (%d) %dx%d\n", 
       capes[1].Nins(), ncapes==1?0:capes[1].Nneus(), capes[ncapes].Nneus());
  fprintf (f,"; Neuronas totales: %d\n", nneurones);
  fprintf (f,"; Fichero de configuracion: %s\n", d->Cfgfile());
  fprintf (f,"; Fichero de         datos: %s\n", d->Infile());
  fprintf (f,"; MSE = %3.3f  (NMSE = %3.3f)", mse, nmse);
  if (d->LMode()==CLASSIF) fprintf(f,"  (Hits: %3.3f%%)\n", hitsP);
  else fprintf(f,"\n");
  fprintf (f,"; BSSM: %3.6f\n", BSSM(d->NIns(), mse, npesos)); 

  fprintf (f, "%d\n", nneurones-capes[ncapes].Nneus());
  fprintf (f, "%d\n\n", ncapes-1);

  // Salvar las capas: tipo neurona, InputFun, ActivFun, No. neuronas/capa

  for (i=1; i<=ncapes; i++)
  {
    fprintf (f, "%s\n", Enum2Nom(TablaNeus_N, int(capes[i].TipoNeu())));
    fprintf (f, "%s\n", 
         NetIFun2Nom (TablaNetI_N, TablaNetI_C, capes[i].GetNetI()));
    fprintf (f, "%s\n", 
         ActivFun2Nom (TablaActiv_N, TablaActiv_C, capes[i].GetActiv()));
    if (i<ncapes) fprintf (f, "%d\n\n", capes[i].Nneus());
  }

  // Salvar los pesos
  fputs("\n\n",f);
  for (i=1; i<=ncapes; i++, fputs("\n",f))
    for (j= capes[i].Ini(); j<=capes[i].Fi(); j++)
      fprintParamH (f, GetWeight(j), capes[i].Nins(), 
         capes[i].TipoNeu(), capes[i].TiposIn());

  fclose (f);
}

void Red::SaveOut (char path[], Data* data, int left, int right)
{
  int i,j;

  f= fopen (path, "w");
  if (f==NULL) error_and_exit ("Error abriendo el fichero: %s\n", path); 

  fprintf (f,"#Esperado    Real\n");

  for (i=left; i<=right; i++)
  {
    EvalRed (data->In(i)); // el resultat queda intern a 'resultat'
    fprintf (f,"%d ", i);
    for (j=1; j<=data->NOuts(); j++)
      fprintf (f,"%g %g   ", data->Out(i)[j].real, resultat[j].real);
    fprintf (f,"\n");
  }
  fclose (f);
}


// S_assumeix que s'ha llegit el .cfg (s'hauria de comprovar)
// Si hi havia una xarxa carregada, es perdra, a menys que no E el fitxer
void Red::ReadRed (char path[], Bool weights, Data& d)
{
  char token[MAXLINE];
  int i, j, c, nh, nch, nnc, index, maxtmp, ninsy;
  TipoNeurona tn = NORMAL;
  InputFun If = NULL;
  ActivFun Af = NULL;

  f= fopen (path, "r");
  if (f==NULL) error_and_exit ("Error abriendo el fichero: %s\n", path); 

  ReInit();

// Leer posibles comentarios: un comentario es toda linea empezada
// por COMMENT_SYMBOL y acabada en '\n'. 
 
  line= 1;
  c= SkipCommentsF(f, line);
  ungetc(c, f);
  
// Leer cabecera de datos: recordar que || es de cortocircuito
  if (!ReadIntF (f, &nh, line, 0) || !ReadIntF (f, &nch, line, 0))
  {
    fclose(f);  
    return;
  }

// Crear estructura
  nneurones= nh+d.NOuts();           // total = hidden+outs
  neurones= new ParamH*[nneurones+1];  
  CheckMem (neurones);
  ncapes= nch+1;
  resultat= new ParamH[nneurones+1]; // son upper bounds, pero bueno
  CheckMem (resultat);
  tmp= new ParamH[nneurones+1];    
  CheckMem (tmp);
  neurones[0]=NULL;                  // no se va a utilizar

// Poner tipos totales (info ya conocida del cfg)
// Los tipos de entrada a la red los tiene la 1a capa
// Los de salida los tiene la red
  capes[1].SetTiposIn (d.TiposIn());
  tipos_out=d.TiposOut();

// Leer las capas: tipo neurona, InputFun, ActivFun, No. neuronas/capa

  for (i=index=1, maxtmp=nneurones; i<=ncapes && !error_IO; i++)
  {
  // Leer tipo neurona
    error_IO= ReadTokenF (f, token, line);
    if (error_IO) 
      error_and_exit ("Fin prematuro al leer un tipo de neurona, en la línea", line);
    else if (HiEs(TablaNeus_N, token))
              tn= TipoNeurona(Nom2Index(TablaNeus_N, token));
         else error_and_exit ("Tipo de neurona desconocido", token);
       
  // Leer tipo NetInput
    error_IO= ReadTokenF (f, token, line);
    if (error_IO) 
       error_and_exit ("Fin prematuro al leer un tipo de neurona, en la línea", line);
    else if (HiEs(TablaNetI_N, token))
              If= TablaNetI_C[Nom2Index(TablaNetI_N, token)];
         else error_and_exit ("Tipo de NetInput desconocido", token);

  // Leer tipo Activ
    error_IO= ReadTokenF (f, token, line);
    if (error_IO) 
       error_and_exit ("Fin prematuro al leer un tipo de neurona, en la línea", line);
    else if (HiEs(TablaActiv_N, token))
              Af= TablaActiv_C[Nom2Index(TablaActiv_N, token)];
         else error_and_exit ("Tipo de Activ desconocido", token);

  // Leer el no. de neuronas/capa 

   if (i<ncapes) error_IO = !ReadIntF (f, &nnc, line, 1, maxtmp) || error_IO;
   else nnc = d.NOuts();

   if (!error_IO) 
    {
      capes[i].Set (tn, index, index+nnc-1, If, Af);
      ninsy= i>1 ? capes[i-1].Nneus() : d.NIns();
      capes[i].SetNins (ninsy);

     // Activar las neuronas de la capa
      if (weights) 
        for(j=index; j<=index+nnc-1; j++)
        {
          // el no. de entradas (ninsy) depende de la capa
          neurones[j] = new ParamH[ninsy+1]; // el +1 es por el bias
          CheckMem (neurones[j]);
        }
     // Actualizar vars. temporales del bucle
      index+=nnc; maxtmp -= nnc;

     // Actualizar nº de pesos totales
      ninsy++;   // el +1 es por el bias ?
      npesos+= ninsy*nnc;
    }
  }

  if (maxtmp!=0) error_and_exit ("No coincide la suma total de neuronas");
  if (weights) ReadWeightsF ();

  // Determinacio automatica de la funcio de cross-entropy
  // Aquesta 'CrossEntropy' era una 'dummy'
  if (d.GetErrorFun()==CrossEntropy)
    if (d.LMode()==CLASSIF) 
      if (d.NOuts()==1) 
      {
        softmax = FALS;
        errorF = CrossEntropy_1;
        switch (capes[ncapes].TipoNeu())
        {
          case HETEROGENEA: capes[ncapes].SetActivF (LogisticJulio);
                            break;
	  case NORMAL:      capes[ncapes].SetActivF (Logistic);
                            break;
  	  case FULLFUZZY:   break;
	}
      }
      else 
      {
        softmax = CERT;
        errorF = CrossEntropy_C;
	capes[ncapes].SetActivF (Softmax);
      }
    else error_and_exit ("CrossEntropy declared when not in CLASSIF mode");
  else SetErrorFun(d.GetErrorFun());
  weights_loaded= weights;  

  fclose (f);
}


