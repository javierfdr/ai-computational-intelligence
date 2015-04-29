// ************************************************
//             Funcions de Fitness
// ************************************************

// Es de min (of course)
Real BGA::ErrorRed (Gene* cromo)
{
  Real error = 0.0;

  //  for (int i=0; i<n; i++)
  //    printf ("[%d] %g  \n", i, cromo[i].real);

 // mecanismo de delays realimentados
#if (REALIM_DELAY!=0)

#if (REALIM_DELAY<0)
    if (change())
#endif

    // poner a X las salidas anteriores a REALIM_DELAY (w.r.t. left)
    data->SetMissingROut (data->TRleft());
#endif

  red->SetWeightDyn (cromo);

  for (int i=data->TRleft(); i<=data->TRright(); i++)
  {
    
#if (REALIM_DELAY!=0)

#if (REALIM_DELAY<0)
    if (change())
#endif

    // poner a su valor las salidas reales anteriores que se realimentan
    data->AbocarRealim (i);
#endif

    red->EvalRed (data->In(i)); // el resultat queda intern a 'resultat'

#if (REALIM_DELAY!=0)  // guardar el resultat

#if (REALIM_DELAY<0)
    if (change())
#endif

      data->SetRout(i, red->Resultat()[1].real);
#endif

    error += red->error_patroF (data->Out(i));
  }
  return error;
}

// Es de min (of course)
// Es el % de misclassified
// he fet int --> real al tipus del resultat
Real BGA::ErrorRedP (Gene* cromo)
{
  int hits=0;

  red->SetWeightDyn (cromo);

  for (int i=data->TRleft(); i<=data->TRright(); i++)
  {
    red->EvalRed (data->In(i));
    if (IsAHit(red->Resultat(), data->Out(i), data->NOuts())) hits++;
  }
  return data->HowManyTR()-hits;
}

Real BGA::F1 (Gene* cromo)
{
  Real sum= 0.0;
  Real x;
  
  for (int i=0; i<n; i++)
  {
    x= cromo[i].real;
    sum+= x*x;
  }
  return sum;
}

// La n=2 pero es posa directament
Real BGA::F2 (Gene* cromo)
{
  Real x= cromo[0].real*cromo[0].real-cromo[1].real;
  Real y= 1-cromo[1].real;

  return 100*x*x+y*y;
}

Real BGA::F3 (Gene* cromo)
{
  Real sum= 0.0;
  
  for (int i=0; i<n; i++)
    sum+= int(cromo[i].real);

  return sum;
}

Real BGA::F3bis (Gene* cromo)
{
  Real sum= 1.0;
  
  for (int i=0; i<n; i++)
    sum*= int(cromo[i].real);

  return sum;
}

Real BGA::F4 (Gene* cromo)
{
  Real sum= 0.0;
  Real x;  

  for (int i=1; i<=n; i++)
  {
    x= cromo[i-1].real*cromo[i-1].real;
    sum+= i*x*x + Normal (0,1);
  }
  return sum;
}

// Para F5
// INLINES ???

int a (int j) { return 16*(j%5-2); }
int b (int j) { return 16*(int(j/5)-2); }
Real p6 (Real x) { return x*x*x*x*x*x; }

// La n=2 pero es posa directament
Real BGA::F5 (Gene* cromo)
{
  const Real eps= 0.002;
  const int j=25;
  Real sum= 0.0;
  Real x1 = cromo[0].real;
  Real x2 = cromo[1].real;
  
  for (int i=1; i<=j; i++)
    sum+= i + (x1-a(i)) + (x2-b(i));

  return 1.0/(eps+sum);
}

Real BGA::F6 (Gene* cromo)
{
  const Real A= 10.0;
  Real sum= 0.0;
  Real x;
  
  for (int i=0; i<n; i++)
  {
    x= cromo[i].real;
    sum+= x*x - A*cos(2.0*M_PI*x);
  }
  return n*A+sum;
}

Real BGA::F7 (Gene* cromo)
{
  const Real V= 418.9828873*n;
  Real sum= 0.0;
  Real x;
  
  for (int i=0; i<n; i++)
  {
    x= cromo[i].real;
    sum+= x*sin(sqrt(fabs(x)));
  }
  return V-sum;
}

Real BGA::F8 (Gene* cromo)
{
  Real sum=0.0, prod= 1.0;
  const Real V= 400.0*n;
  Real x;
  
  for (int i=0; i<n; i++)
  {
    x= cromo[i].real;
    sum+= (x*x)/V;
    prod*= cos(x/sqrt(i+1));
  }
  return sum-prod+1.0;
}

Real BGA::F9 (Gene* cromo)
{
  Real e1=0.0, e2 = 0.0;
  const Real v= 20.0;
  Real x;
  
  for (int i=0; i<n; i++)
  {
    x= cromo[i].real;
    e1+= x*x;
    e2+= cos(2.0*M_PI*x);
  }
  return (-v*exp(-0.2*sqrt(e1/n)) - exp(e2/n) + v + M_E);
}

// La n=2 pero es posa directament
Real BGA::F10 (Gene* cromo)
{
  Real x= cromo[0].real*cromo[0].real + cromo[1].real*cromo[1].real;
  Real y= sin(sqrt(x));
  Real z= 1.0 + 0.001*x;

  return (0.5 + (y*y-0.5)/(z*z));
}

// La n=2 pero es posa directament
Real BGA::F11 (Gene* cromo)
{
  Real x= cromo[0].real*cromo[0].real + cromo[1].real*cromo[1].real;
  Real y= sqrt(sqrt(x));
  Real z= sin(50.0*pow(x, 0.1));

  return y*(z*z+1.0);
}

// La n=2 pero es posa directament
Real BGA::F12 (Gene* cromo)
{
  Real d= cromo[0].real*cromo[0].real + cromo[1].real*cromo[1].real
          + 0.5*cromo[0].real*cromo[1].real;
  Real e= exp(-d/8.0);
  Real c= cos(4.0*sqrt(d));

  return 1.0-e*c;
}

// ************************************************
//             Funcions Composades
// ************************************************

Real F2 (Real x, Real y)
{
  Real x2= x*x-y;
  Real y2= 1-x;

  return 100*x2*x2+y2*y2;
}

Real F8 (Real x)
{
  const Real V= 4000.0;
  
  return (x*x)/V-cos(x)+1.0;
}

Real BGA::F8F2 (Gene* cromo)
{  
  Real sum= 0.0;

  for (int i=0; i<n-1; i++)
  {
    sum+= ::F8(::F2(cromo[i].real, cromo[i+1].real));
  }
  return sum + ::F8(::F2(cromo[n-1].real, cromo[0].real));
}

/* Funcio de Fletcher-Powell */

#define N 30
typedef Real Realmatrix[N][N];
typedef Real Realvector[N];
static Realmatrix A, B;
static Realvector alpha, sin_alpha, cos_alpha, a_i;

void BGA::Ini_FP (void)
{  
  int i, j; 

  puts ("Initializing Fletcher-Powell function ...");
  for (i=0; i<n; i++)
  {
    for (j=0; j<n; j++)
    {
      A[i][j] = random_minMax (-100.0, 100.0);
      if (setup->save_BGA) printf ("%g ", A[i][j]);  
      B[i][j] = random_minMax (-100.0, 100.0);
      if (setup->save_BGA) printf ("%g ", B[i][j]);  
    }
    alpha[i] =  random_minMax (-M_PI, M_PI);  // (this is the solution)
    if (setup->save_BGA) printf ("%g \n", alpha[i]);  
    sin_alpha[i] = sin(alpha[i]);
    cos_alpha[i] = cos(alpha[i]);
  }
  for (i=0; i<n; i++)
    for (j=0, a_i[i]=0.0; j<n; j++)
      a_i[i] += A[i][j]*sin_alpha[j] + B[i][j]*cos_alpha[j]; 
}

Real BGA::FP (Gene* cromo)
{  
  Real summy, sum= 0.0;
  Real b_i;
  int i, j;  

  for (i=0; i<n; i++)
  {
    for (j=0, b_i=0.0; j<n; j++)  
      b_i += A[i][j]*sin(cromo[j].real) + B[i][j]*cos(cromo[j].real); 
    summy = a_i[i] - b_i;   
    sum += summy*summy;
  }
  return sum;
}

// ************************************************
//             Estructura d'un problema
// ************************************************
#define LONGNAME  50
#define SHORTNAME 10

struct Problema
{
  char nom[SHORTNAME];
  char comment[LONGNAME];
  FitnessF fitness;
  Rang rang;
  TiposH tipo;
  Bool es_min;
  Real best;
  int n;
};

Problema ElsProblemes[] =
{
  { "ErrorRed", "Error cuadrático",    
        &BGA::ErrorRed,  { -10.0, 10.0},  REAL, CERT,  0.0,  0 },
  { "F1", "De Jong-1",    
        &BGA::F1,    { -5.12, 5.12 },   REAL, CERT,   0.0,  3 },
  { "F2", "De Jong-2",    
        &BGA::F2,    { -2.048, 2.048 }, REAL, CERT,   0.0,  2 },
  { "F3", "De Jong-3",    
        &BGA::F3,    { -5.12, 5.12 },   REAL, CERT, -25.0,  5 },
  { "F4", "De Jong-4",    
        &BGA::F4,    { -1.28, 1.28 },   REAL, CERT,  -2.5, 30 },
  { "F5", "De Jong-5",    
        &BGA::F5,    { -65536, 65536 }, REAL, CERT, 1.002, 25 },
  { "F6", "Rastrigin",    
        &BGA::F6,    { -5.12, 5.12 },   REAL, CERT,   0.0, 20 },
  { "F7", "Schwefel",    
        &BGA::F7,    { -512, 512 },     REAL, CERT,   0.0, 10 },
  { "F8", "Griewangk",    
        &BGA::F8,    { -512, 512 },     REAL, CERT,   0.0, 10 },
  { "F9", "Ackley",    
        &BGA::F9,    { -30, 30 },       REAL, CERT,   0.0, 30 },
  { "F10", "Schaffer-1",    
        &BGA::F10,   { -100, 100 },     REAL, CERT,   0.0,  2 },
  { "F11", "Schaffer-2",    
        &BGA::F11,   { -100, 100 },     REAL, CERT,   0.0,  2 },
  { "F12", "Julio",    
        &BGA::F12,   { -5.0, 5.0 },     REAL, CERT,   0.0,  2 },
  { "F8F2", "Whitley",    
        &BGA::F8F2,  { -2.048, 2.047},  REAL, CERT,   0.0, 10 },
  { "F8F2", "Whitley",    
        &BGA::F8F2,  { -2.048, 2.047},  REAL, CERT,   0.0, 20 },
  { "F8F2", "Whitley",    
        &BGA::F8F2,  { -2.048, 2.047},  REAL, CERT,   0.0, 50 },
  { "F8F2", "Whitley",    
        &BGA::F8F2,  { -2.048, 2.047},  REAL, CERT,   0.0, 100 },
  { "F8F2", "Whitley",    
        &BGA::F8F2,  { -2.048, 2.047},  REAL, CERT,   0.0, 200 },
  { "F3bis", "De Jong-Belanche", 
        &BGA::F3bis, { -5.12, 5.12 },   REAL, CERT,  3125,  5 },
  { "FP", "Fletcher-Powell",  
        &BGA::FP,    { -M_PI, M_PI },   REAL, CERT,   0.0,  N },
  { "ErrorRedP", "Numero de mal clasificados",    
        &BGA::ErrorRedP,  { -10.0, 10.0},  REAL, CERT,  0.0,  0 }
};

