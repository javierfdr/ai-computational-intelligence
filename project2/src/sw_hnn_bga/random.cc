/* Basic Random number generation and manipulation */

#include <math.h>
#include "random.h"

void ini_random (void)
{
  unsigned short seedlong[3];
  int i;
  
  for (i=0; i<=2; i++)
    seedlong[i] = (unsigned short)10001*(i+1);

  seed48 (seedlong);
}

void ini_random_c (void)
{
  union
  {
    struct                              // 32 + 16 = 48 bits
    {
      clock_t time;                    
      unsigned short dummy;   
    } t;                                 
    unsigned short seedlong[3];        // 16 * 3 = 48 bits
  };

  t.time = clock();
  printf ("Clock = %ld\n", t.time);
  t.dummy = 11111;                   // Un no. com altre qualsevol
  
  seed48 (seedlong);
}

Real random_01 (void)
// Torna un no. aleatori en [0,1)
{
  return (Real(drand48()));
}

Real random_minMax (Real min, Real Max)
// Torna un no. aleatori en [min, Max)
{
  return (random_01()*(Max-min)+min);
}

long random_long (void)
// Torna un no. aleatori en [-2**31, 2**31)
{
  return (mrand48());
}

ulong random_ulong (void)
// Torna un no. aleatori en (0, 2**31)
{
  return (lrand48());
}

int flip (Real prob)
// Torna CERT amb probabilitat = prob
{
  return ( prob==1.0f ? 1 : prob>=random_01() ); 
}

Real random_1_flip (void)
// Torna un no. aleatori en [-1,1)
{
  Real f = Real(drand48());
  return (flip(0.5) ? f : -f);
}

Real random_1 (void)
// Torna un no. aleatori en [-1,1)
{
  return (Real(random_long())/Real(LONG_MAX));
}

uint randint (uint low, uint high)
// Torna un unsigned int aleatori entre low i high (inclosos)
// Sembla que (!!!) low>=0
{
  uint i;

  if (low>=high) i = low;
  else 
  {
    i = (uint)(random_01()*Real(high-low+1)+Real(low));
    if (i>high) i = high;
  }
  return i;
}

int randbyte (void)
{
  return randint(0, 255U);
}

int random_sign (void)
{
  return (flip(0.5) ? +1 : -1);
}

/* -- Normal --
  Retorna un valor de una distribucion NORMAL de media 'mu' y varianza 'sigmaC'
  Metodo descrito en Numerical Recipes in C
  Como se generan dos numeros a la vez se guarda uno para la proxima llamada a
  la funcion.
*/

Real Normal (Real mu, Real sigmaC)
{
  static Bool cached= FALS;
  static Real cachedValue;
  Real v1, v2, w, y;

  if (cached)
  {
    cached= FALS;
    return mu+sigmaC*cachedValue;
  }
  do
  {
    v1= 2.0*random_01()-1.0;
    v2= 2.0*random_01()-1.0;
    w = v1*v1+v2*v2;
  }
  while (w>=1 || w==0);    //segun J.L. Ferrer era solo (w>1)
  y= sqrt(-2.0*log(w)/w);
  cachedValue= v1 * y;
  cached= CERT;
  return mu+sigmaC*v2*y;
}

/***********************************************************************
  TRIANGULAR pdf: PROBABILIDAD que X = x
************************************************************************/


Real f1 (Real x, Real s, Real m)
{
  return (x + s - m)/(s*s);
}

Real f2 (Real x, Real s, Real m)
{
  return (-x + s + m)/(s*s);
}

Real Triangular_Unimodal (Real x, Real s, Real m)
{
  if (x<m-s) return 0;
  if (x>m+s) return 0;
  if (m-s<=x && x<=m) return f1(x, s, m);
  return f2(x, s, m);
}

Real Triangular_Bimodal (Real x, Real s1, Real m1, Real s2, Real m2)
{
  return 0.5*(Triangular_Unimodal(x, s1, m1) + Triangular_Unimodal(x, s2, m2));
}

//***********************************************************************
//  TRIANGULAR pdf ACUMULADA PROBABILIDAD que X <= x0
//                  (es la integral de la anterior)
//***********************************************************************
/*
Real F1 (Real x, Real s, Real m)
{
  return x*(0.5*x + s - m)/(s*s);
}

Real F2 (Real x, Real s, Real m)
{
  return x*(-0.5*x + s + m)/(s*s);
}

Real Triangular_Unimodal_Acumulada (Real x0, Real s, Real m) 
{
  if (x0<m-s) return 0;
  if (x0>m+s) return 1;
  if (m-s<=x0 && x0<=m) return F1(x0, s, m) - F1(m-s, s, m);
  return F2(x0, s, m) - F2(m, s, m) + 0.5;
}

Real Triangular_Bimodal_Acumulada (Real x0, Real s1, Real m1, Real s2, Real m2)
{
  return 0.5*( Triangular_Unimodal_Acumulada(x0, s1, m1) 
                 + Triangular_Unimodal_Acumulada(x0, s2, m2));
}
*/

// Estas son las que se han de usar (lo anterior en realidad no sirve)
Real TrianUnimodal_Aux (Real y, Real s, Real m)
{
  if (y<=0.5) return m-s+s*sqrt(2.0*y);
  return m+s-s*sqrt(2.0*(1.0-y));
}

Real TrianUnimodal (Real s, Real m)
{
  return TrianUnimodal_Aux (random_01 (), s, m);
}

Real TrianBimodal_e (Real s1, Real m1, Real s2, Real m2)
{
  Real y = random_01 ();

  if (y<=0.5) return TrianUnimodal_Aux (2.0*y, s1, m1);
  return TrianUnimodal_Aux (2.0*y-1.0, s2, m2);
}

// Pre: m1<m2
Real TrianBimodal (Real m1, Real m2)
{
  const Real e = 0.5;

  return TrianBimodal_e (e*(m2-m1), m1, e*(m2-m1), m2);
}

