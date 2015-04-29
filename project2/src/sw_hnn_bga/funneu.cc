// funneu.cc

/* Funcions de: - net input (normals i heterogènies: similituds)
                - activació
		- agregació
                - error
		- network complexity
*/

#include <math.h>
#include <stdlib.h> // abs()
#include "funneu.h"
#include "entsal.h"
#include "misc.h"

extern  Real** probs; // això s'haurà de posar a Stats.

// ****************************************************
//        Funciones de NET INPUT para neurona NORMAL
// ****************************************************

// ***** basadas en producto escalar *****
Real ProdEscalar (ParamH in[], ParamH w[], int n)
{
  Real p=0.0;

  for (; n ; n--)
    p += in[n].real*w[n].real;

  return p+w[0].real;  // sumar el bias  
}

Real SimilProdEscalar (ParamH in[], ParamH w[], int n)
{
  Real p=0.0; 

  for (int i=1; i<=n ; i++)
    p += in[i].real*w[i].real;

  return (p+w[0].real)/(n+1.0);
}

Real SimilAngular (ParamH in[], ParamH w[], int n)
{
  Real p=w[0].real;
  Real m_in=1.0, m_w=w[0].real*w[0].real;

  for (int i=1; i<=n ; i++)
  {
    m_in += in[i].real*in[i].real;
    m_w += w[i].real*w[i].real;
    p += in[i].real*w[i].real;
  }

  return (p/sqrt(m_in*m_w) + 1.0)*0.5;
}

Real SimilMinmax (ParamH in[], ParamH w[], int n)
{
  Real p=w[0].real, tmpM, tmpm;
  Real m_in=1.0, m_w=w[0].real*w[0].real;

  for (int i=1; i<=n ; i++)
  {
    m_in += in[i].real*in[i].real;
    m_w += w[i].real*w[i].real;
    p += in[i].real*w[i].real;
  }

  if (m_in<m_w) { tmpM = m_w; tmpm = m_in;} 
  else { tmpm = m_w; tmpM = m_in;} 
  return ((p/sqrt(m_in*m_w) + 1.0)*0.5*tmpm/tmpM);
}

Real RbfEstandar (ParamH in[], ParamH w[], int n)
{
  Real pp, p=0.0; 

  for (; n ; n--)
  {
    pp = in[n].real-w[n].real; 
    p += pp*pp;
  }

  return p/((w[0].real)*(w[0].real)); // w[0] hace de stddev
}

// Función de NET INPUT para neurona HETEROGÉNEA es la similitud

//Luego ya lo desglobalizaremos

SimilFun* VectorSimilFun; // tabla de f. de similitud
Real* s;                  // tabla de similitudes parciales

// Llena la tabla de similitudes parciales [1..q] y devuelve q
int Similarity (ParamH in[], ParamH w[], int n)
{
  int i,q=0;

  for (i=1; i<=n; i++)
    if (!IsMissing(in[i]) && !IsMissing(w[i]))
      s[++q] = VectorSimilFun[i](in[i], w[i]);

  return q;
}

// ************************************************************
// Funciones de AGREGACIÓN para neurona HETEROGÉNEA
// ************************************************************

/* Todas tienen el tratamiento de Missing (que parece de Dixon), que
   consiste en dividir por el nº de similitudes parciales no missing.

 En todas ellas w[0].real hace de parámetro libre (sigma) en la ActivF
 a la que se pasa este resultado, o sea la logística en [0,1].

 En SimilInvqMean y SimilQuadMean no estoy seguro si debo dividir por q */

Real SimilInvqMean (ParamH in[], ParamH w[], int n)
{
  Real s_ = 0.0;
  int q = Similarity (in, w, n);
  for (int i=1; i<=q ; i++)
    s_ += sqrt(s[i]);

  Real aux = s_/Real(q);
  return aux*aux*w[0].real;
}

Real SimilHarmMean (ParamH in[], ParamH w[], int n)
{
  Real s_ = 0.0;
  int q = Similarity (in, w, n);
  for (int i=1; i<=q ; i++)
    s_ += 1.0/s[i];
  return w[0].real*Real(q)/s_;
}

// Ojo a la precisión: una alternativa es hacer exp de la media aritmética de logs
// OJO especial: si alguna s[i] es 0, todo lo será. De momento pongo un GEOM_EPSILON
#define GEOM_EPSILON 10e-5
Real SimilGeomMean (ParamH in[], ParamH w[], int n)
{
  Real s_ = 1.0;
  int q = Similarity (in, w, n);
  for (int i=1; i<=q ; i++)
    s_ *= (s[i]==0 ? GEOM_EPSILON : s[i]);
  return w[0].real*pow(s_,1.0/Real(q));
}

Real SimilAritMean (ParamH in[], ParamH w[], int n)
{
  Real s_ = 0.0;
  int q = Similarity (in, w, n);
  for (int i=1; i<=q ; i++)
    s_ += s[i];
  return w[0].real*s_/Real(q);
}

Real SimilQuadMean (ParamH in[], ParamH w[], int n)
{
  Real s_ = 0.0;
  int q = Similarity (in, w, n);
  for (int i=1; i<=q ; i++)
    s_ += s[i]*s[i];
  return w[0].real*sqrt(s_/Real(q));
}


/* Taula de tipus de funcions de NetInput */

char*  TablaNetI_N[]= 
  { 
    "PROD-ESCALAR", "SIMIL-PROD-ESCALAR", "SIMIL-ANGULAR",
    "SIMIL-INVQ-MEAN", "SIMIL-HARM-MEAN", "SIMIL-GEOM-MEAN", 
    "SIMIL-ARIT-MEAN", "SIMIL-QUAD-MEAN", 
    "SIMIL-MINMAX", "RBF-ESTANDAR", "" 
  };

InputFun TablaNetI_C[]= 
  {  
    ProdEscalar, SimilProdEscalar, SimilAngular,
    SimilInvqMean, SimilHarmMean, SimilGeomMean, 
    SimilAritMean, SimilQuadMean, 
    SimilMinmax, RbfEstandar
  };


// ************************************************************
// Funciones de SIMILITUD para neurona HETEROGÉNEA
// ************************************************************


// FUZZY y VARLING
#include "fuzzy.cc" // puesto aparte por ser largo

// CONTINUOUS
#include "real.cc" // puesto aparte por ser largo

// ORDINAL

// Notar el uso del campo x.joker=y.joker: el índice de variables
Real compara_ORDINAL (ParamH& x, ParamH& y)
{
  Disc minord = MIN(x.disc, y.disc);
  Disc maxord = MAX(x.disc, y.disc);
  Real num  = 0.0, den = 0.0;

  if (minord==maxord) return 1.0;

  for (Disc i = minord; i <= maxord; i++)
    {
      num += probs[x.joker][i];
      den += log(probs[x.joker][i]);
    }

  //  printf("ORDINAL = %20.19f para %d,%d(%d)\n", 2.0*log(num)/den, minord, maxord, x.joker);
  return 2.0*log(num)/den;
}

/* PROB: OJO esto está preparado para comparar valores via sus
   probabilidades; en realidad en PROB se pasan ya las probabilidades
   como x,y (habrá que cambiarlo) */

// The Harmonic mean
inline Real HARM (Real x, Real y)
{
  return 2.0/(1.0/x+1.0/y);
}

Real compara_PROB (ParamH& x, ParamH& y)
{
  if (x.disc==y.disc) return 1.0-probs[x.joker][x.disc]/2.0;
  return HARM (1.0-probs[x.joker][x.disc], 1.0-probs[x.joker][y.disc]);
}

// NOMINAL

Real compara_NOMINAL_overlap (ParamH& x, ParamH& y)
{
  return x.disc==y.disc ? 1.0 : 0.0;
}

// Chapussa per NDADES (ja ho arreglaré)
extern int NDADES;

Real compara_NOMINAL_Karina (ParamH& x, ParamH& y)
{
  return x.disc==y.disc ? 1.0 : 
    1.0-(1.0/probs[x.joker][x.disc]+1.0/probs[x.joker][y.disc])/Real(NDADES); 
}

Real compara_NOMINAL_probs (ParamH& x, ParamH& y)
{
  return compara_PROB (x,y);
}

Real compara_NOMINAL (ParamH& x, ParamH& y)
{
  //  printf("NOMINAL = %g", compara_NOMINAL_overlap (x,y));
  return compara_NOMINAL_overlap (x,y);
}

// BINARY

Real compara_BINARY (ParamH& x, ParamH& y)
{
  //  printf("BINARY = %g", compara_PROB (x,y));
  return compara_PROB (x,y);
}

// INTEGER

// Li cal una Sbarra que admeti argument [0, inf)
Real compara_INTEGER (ParamH& x, ParamH& y)
{
  //  printf("INTEGER = %g", TablaSbarraF[SiI](Real(abs(x.disc-y.disc))));
  return TablaSbarraF[SiI](Real(abs(x.disc-y.disc)));
}



// ****************************************************
//                    Funcions  d'ACTIVACIÓ
// ****************************************************

Real Identity (Real r)
{
  return r;
}

Real SemiLinear (Real r)
{
  if (r>1.0) return 1.0;
  if (r<0.0) return 0.0;
  return r;
}

Real Gaussiana (Real r)
{
  return exp(-0.5*r);
}

#define threshold 10.0
Real Logistic (Real r)
{
  if (fabs(r)<threshold) return 1.0/(1.0+exp(-r));
  if (r>threshold) return 1.0; else return 0.0;
}

// [-10:10] ---> [0:1]
Real LogisticAbs (Real r)
{
  return 0.55*r/(fabs(r)+1)+0.5;
}

// [0,1] ---> (0,1)
Real LogisticH (Real r)
{
  return Logistic (threshold*(r+r-1)); //=(2.0*threshold*(r-0.5))
}

// Aqui comença la del Julio

inline Real a (Real k)
{
  return ( -0.25 + sqrt(0.0625+k) );  // [-0.5+sqrt(0.5^2+4k)]/2
}  

#define K   0.1
#define AK  0.1531128874
#define AK1 1.1531128874  // AK+1

//#define K   0.05
//#define AK  0.0854102
//#define AK1 1.0854102

//#define K   0.01
//#define AK  0.0192582
//#define AK1 1.0192582

Real LogisticJulio (Real r)
{
  //#if (EXEC_MODE==0)
  if (r<0.0 || r>1.0) 
    error_and_exit ("Sigmoidal_JulioHETEROGENEA: argument fora de rang!", r);
  //#endif
  return (r<=0.5 ? -K/(r-0.5-AK)-AK : -K/(r-0.5+AK)+AK1);  
}

// Aquesta ve d'adaptar LogisticAbs a [0:1] ---> [0:1]
Real LogisticMia (Real r)
{
  Real rr = 21*r-10;
  return (r>=0.5 ? rr/(rr-r+1) : r/(-20*r+11));
}

Real Softmax (Real r)
{
  return exp(r);
}


// Taula de tipus de funcions d'Activació

char*  TablaActiv_N[]= 
  { 
    "IDENTITY", "SEMILINEAR", "GAUSSIANA", 
    "LOGISTIC", "LOGISTIC-ABS", "LOGISTIC-H", 
    "LOGISTIC-JULIO", "LOGISTIC-MIA", "SOFTMAX", "" 
  };

ActivFun TablaActiv_C[]= 
  {  
    Identity, SemiLinear, Gaussiana, 
    Logistic, LogisticAbs, LogisticH, 
    LogisticJulio, LogisticMia, Softmax
  };                 

// *********************
// Funcions d'ERROR
// *********************

Real ErrorQuadratic (ParamH in[], ParamH w[], int n)
{
  Real p=0.0, pp;

  for (; n ; n--)
  {
    pp = in[n].real-w[n].real;
    p += pp*pp;
  }
  return p;
}

// Dummy function: despres sera una de les dues seguents
Real CrossEntropy (ParamH t[], ParamH y[], int n)
{
  return 1.0;
}

//////////////////////////////////////////
// Assumeixen 1 neurona de sortida i CLASSIF
//////////////////////////////////////////

// Expressio general
Real CrossEntropy_1_ (ParamH t[], ParamH y[], int n)
{
  return -t[1].real*log(y[1].real) - (1.0-t[1].real)*log(1.0-y[1].real);
}

// Expressio simplificada quan targets = {0, 1}
Real CrossEntropy_1 (ParamH t[], ParamH y[], int n)
{
  return (t[1].real ? -log(y[1].real) : -log(1.0-y[1].real));
}

///////////////////////////////////////////
// Assumeixen >1 neurona de sortida i CLASSIF
///////////////////////////////////////////

// Expressio general
Real CrossEntropy_C_ (ParamH t[], ParamH y[], int n)
{
  Real p=0.0;

  for (; n ; n--)
    p -= t[n].real*log(y[n].real);

  return p; 
}

// Expressio simplificada quan targets = {0, 1}
Real CrossEntropy_C (ParamH t[], ParamH y[], int n)
{
  for (; n ; n--)
    if (t[n].real) return -log(y[n].real);
}

///////// Taula de tipus de funcions d_error

char*  TablaError_N[]= { "MSE", "CROSS-ENTROPY", "" };

ErrorFun TablaError_C[]= {  
                           ErrorQuadratic,
                           CrossEntropy
                        };

// ****************************************************
// Funcions de NETWORK COMPLEXITY
// ****************************************************

// Cal afegir-ne més

// BSSM = Bayesian Statistical Significance Measure
// N = input dim, nn = no. of free params.
Real BSSM (int N, Real MSE, int nn)
{
  return N*log(MSE)+nn*log(N);
}





// ************************************************************
// Funciones de SIMILITUD estáticas (todo esto a eliminar)
// ************************************************************



SimilFun SimilFunDefecto[] = 
  {
    Euclidean, compara_FUZZY, compara_INTEGER, compara_VARLING, compara_NOMINAL, 
    compara_BINARY, compara_ORDINAL,NULL,NULL,compara_PROB,NULL
  };


// faltarían SET, BITSTRING, INTERVAL (ara són NULL)
