// breeder.cc

/************************************************************
   Los cromo empiezan en [0]
************************************************************/

#include "breeder.h"
#include "misc.h"
#include "entsal.h"
#include <memory.h>
#include <math.h>
#include <float.h>  // DBL_EPSILON
#include <stdlib.h>
#include <string.h> // srtcat

// Per posar-les a un fitxer separat
#include "FFs.cc"

// ************************************************
//             Funcions Petitoncies
// ************************************************


// NO copia els fitness (nomes els genes)
inline void BGA::CopyCromo (Gene* destin, Gene* source)
{
  memcpy (destin, source, sizeof(Gene)*n);
}  

// Implementa el q-elitisme
// Despres del SortPop(), el millor ha quedat en pop[1], etc.
// Y ahora los ponemos (elite) a partir del final
void BGA::AddBest (void)
{
  for (int i=0; i<q; i++)
    CopyCromo (popaux[popsize-i]->cromo, pop[i+1]->cromo);
}  

inline Real BGA::GetBest (void)
{
  return pop[1]->fitness;
}

inline Gene* BGA::GetBestCromo (void)
{
  return pop[1]->cromo;
}

inline Gene* BGA::GetBestAux (void)
{
  return auxx;
}

// ************************************************
//             Funcions de Terminacio
// ************************************************

inline Bool BGA::MaxGens (void)
{
  return (gens>=generations);
}

inline Bool BGA::WithinBest (Real x)
{
  return ( best==0.0 ? 
   	     fabs(x) <= epsilon :
             fabs(x-best) <= epsilon*fabs(best) );
}

// Igual que l'anterior pero assumeix que best=0
inline Bool BGA::WithinZero (Real x)
{
  return ( fabs(x) <= epsilon );
}

inline Bool BGA::Composite (void)
{
  return MaxGens() || WithinBest(GetBest());
}

// ************************************************
//             Funcions de Replacement
// ************************************************


// ************************************************
//             Funcions de Seleccio
// ************************************************

// Retorna 2 naturals aleatoris entre [1, t]
inline void BGA::RandomSelect (int& pare, int& mare, int t)
{
  pare = randint (1, t);
  mare = randint (1, t);
}

// *************************************************
//             Funcions de Recombinacio
//
// Recombinen (pare, mare) de pop --> fill de popaux
// *************************************************


//             Seleccio d'alfa (% de cada progenitor)
//                  (TOTES Candidata a INLINE)

inline Real BGA::FixedAlfaSelection (void)
{
  return fixedalfa;
}

// Aquesta es l'original per els BGA
inline Real BGA::UniformAlfa_BGADelta_Selection (void)
{
  return random_minMax(-delta, 1.0+delta);
}

inline Real BGA::GaussianAlfa_BGADelta_Selection (void)
{
  return Normal (0.5, (0.5+delta)/2.5);
}

Real BGA::UniformAlfa_RangeDelta_Selection (int i, Real p, Real m)
{
  Real denom = rangs[i].max - rangs[i].min;
  Real delta_min = (m-rangs[i].min)/denom;
  Real delta_max = (rangs[i].max-p)/denom;

  return random_minMax(-delta_min, 1.0+delta_max);
}

Real BGA::GaussianAlfa_RangeDelta_Selection (int i, Real p, Real m)
{
  Real delta2 = MIN(m-rangs[i].min, rangs[i].max-p);

  delta2 /= rangs[i].max-rangs[i].min;
  return Normal (0.5, (0.5+delta2)/2.5);
}

// *************************************************
//             Funcions de Recombinacio
// *************************************************
// ES PODRIA FER UN INVENT SIMILAR A LES S_i: UNA TAULA JA AMB LES QUE TOCA I TREURE EL switch

void BGA::Recombination (int pare, int mare, int fill)
{
  for (int i=0; i<n; i++)
    switch (tipus[i])
    {
      case REAL:    (this->*recombin_Real) (pop[pare]->cromo[i], pop[mare]->cromo[i],
                                            popaux[fill]->cromo[i], i); 
                    break;

      case FUZZY:   (this->*recombin_Fuzzy) (pop[pare]->cromo[i].fn, pop[mare]->cromo[i].fn, 
					     popaux[fill]->cromo[i].fn, i); 
                    break;

      case VARLING: (this->*recombin_Varling) (pop[pare]->cromo[i].fq, pop[mare]->cromo[i].fq, 
                                      popaux[fill]->cromo[i].fq, i); 
                    break;
      case INTEGER: (this->*recombin_Integer) (pop[pare]->cromo[i], pop[mare]->cromo[i], 
                                      popaux[fill]->cromo[i], i); 
                    break;
      case NOMINAL: (this->*recombin_Nominal) (pop[pare]->cromo[i], pop[mare]->cromo[i], 
                                      popaux[fill]->cromo[i], i); 
                    break;
      case BINARY: (this->*recombin_Binary) (pop[pare]->cromo[i], pop[mare]->cromo[i], 
                                      popaux[fill]->cromo[i], i); 
                    break;
      case ORDINAL: (this->*recombin_Ordinal) (pop[pare]->cromo[i], pop[mare]->cromo[i], 
                                      popaux[fill]->cromo[i], i); 
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


void BGA::DiscreteRecombin_UNIVERSAL (Gene& p, Gene& m, Gene& fill, int i)
{
  fill = flip(0.5) ? p : m;
}

void BGA::ExtendedInterRecombin_REAL (Gene& p, Gene& m, Gene& fill, int i)
{
  Real alfa;

  // alfa = FixedAlfaSelection ();

  alfa = UniformAlfa_BGADelta_Selection ();
  fill.real = p.real + alfa*(m.real-p.real);

  if (fill.real > rangs[i].max) { fill.real = rangs[i].max; }
  if (fill.real < rangs[i].min) { fill.real = rangs[i].min; }
}

void BGA::ExtendedInterRecombin_Range_REAL (Gene& p, Gene& m, Gene& fill, int i)
{
  Real alfa, tmp, pp= p.real, mm= m.real;

  // Range_delta
  if (pp<mm) { tmp=pp; pp=mm; mm=tmp; }
  alfa = UniformAlfa_RangeDelta_Selection (i, pp, mm);
  fill.real = mm + alfa*(pp-mm);  
}

void BGA::ExtendedInterRecombin_FUZZY (TrianFN& p, TrianFN& m, TrianFN& fill, int i)
{
  Real alfa; 

  alfa = UniformAlfa_BGADelta_Selection ();
  fill.mode = p.mode + alfa*(m.mode-p.mode);
  fill.spread = p.spread + alfa*(m.spread-p.spread);

  // Los rangos del spread no se tratan del todo bien (podemos salirnos: mode+spread)
  if (fill.mode > rangs[i].max) { fill.mode = rangs[i].max; }
  if (fill.mode < rangs[i].min) { fill.mode = rangs[i].min; } 
  if (fill.spread > rangs[i].max) { fill.spread = rangs[i].max; }
  if (fill.spread < rangs[i].min) { fill.spread = rangs[i].min; } 
}

// Muy pura pero no chuta
void BGA::ExtendedInterRecombin_Range_FUZZY (TrianFN& p, TrianFN& m, TrianFN& fill, int i)
{
  Real alfa;
  TrianFN tmp, pp= p, mm= m;

  // Range_delta
  if (p.mode<m.mode) { tmp=pp; pp=mm; mm=tmp; }
  alfa = UniformAlfa_RangeDelta_Selection (i, pp.mode, mm.mode);
  fill.mode = mm.mode + alfa*(pp.mode-mm.mode);  

  pp= p; mm= m;
  if (p.mode+p.spread<m.mode+m.spread) { tmp=pp; pp=mm; mm=tmp; }
  alfa = UniformAlfa_RangeDelta_Selection (i, pp.mode+pp.spread, mm.mode+mm.spread);
  fill.spread = mm.mode+mm.spread + alfa*(pp.mode+pp.spread-mm.mode-mm.spread) - fill.mode;  
}

void BGA::ExtendedInterRecombin_VARLING (TrapezFQ& p, TrapezFQ& m, TrapezFQ& fill, int i)
{
  Real alfa, mode1, mode2;

  alfa = UniformAlfa_BGADelta_Selection ();
  mode1 = p.mode1 + alfa*(m.mode1-p.mode1);
  mode2 = p.mode2 + alfa*(m.mode2-p.mode2);

  if (mode1 > rangs[i].max) { mode1 = rangs[i].max; }
  if (mode2 > rangs[i].max) { mode2 = rangs[i].max; }
  if (mode1 < rangs[i].min) { mode1 = rangs[i].min; } 
  if (mode2 < rangs[i].min) { mode2 = rangs[i].min; } 
  
  if (mode1 > mode2) { fill.mode1 = mode2; fill.mode2 = mode1; }
  else  { fill.mode1 = mode1; fill.mode2 = mode2; }

  fill.spread1 = p.spread1 + alfa*(m.spread1-p.spread1);
  fill.spread2 = p.spread2 + alfa*(m.spread2-p.spread2);

  if (fill.spread1 > rangs[i].max) { fill.spread1 = rangs[i].max; }
  if (fill.spread2 > rangs[i].max) { fill.spread2 = rangs[i].max; }
  if (fill.spread1 < rangs[i].min) { fill.spread1 = rangs[i].min; } 
  if (fill.spread2 < rangs[i].min) { fill.spread2 = rangs[i].min; } 
}


// S'ha d'adaptar abans de provar-la amb Range_delta. O provar-la nomes amb BGA

// void BGA::ExtendedLineRecombin_REAL (Gene& p, Gene& m, Gene& fill, int i)
// {
//   Real alfa = UniformAlfa_BGADelta_Selection ();
//   Real p,m,aux;

//   for (int i=0; i<n; i++)
//   {
//     p = pop[pare]->cromo[i].real;
//     m = pop[mare]->cromo[i].real;
//     aux = p + alfa*(m-p);
//     if (aux > rangs[i].max) aux = rangs[i].max;
//     if (aux < rangs[i].min) aux = rangs[i].min;
//     popaux[fill]->cromo[i].real = aux;
//   }
// }


// En principi no s'hauria de sortir de rang: comprovar-ho i treure el codi
// No adaptada a FUZZY
void BGA::FuzzyRecombin_REAL (Gene& p, Gene& m, Gene& fill, int i)
{
  Real pp= p.real, mm= m.real, aux;

  aux = pp>=mm ? TrianBimodal (mm, pp) : TrianBimodal (pp, mm);

  if (aux > rangs[i].max) { aux = rangs[i].max; } //puts("Me paso!"); }
  if (aux < rangs[i].min) { aux = rangs[i].min; } //puts("No llego!"); }
  fill.real = aux;
}

// S'assumeix alfa=0.5 (es calcula la mediana)
void BGA::LineRecombin_INTEGER (Gene& p, Gene& m, Gene& fill, int i)
{
  Disc j = p.disc+m.disc;

  if (j&1) fill.disc = flip(0.5) ? j>>1 : (j>>1)+1; // senar(j)
  else fill.disc = j>>1;                            // parell(j)
}

/*
void BGA::ExtendedInterRecombin_INTEGER (Gene& p, Gene& m, Gene& fill, int i)
{
  Disc tmp, left, right;

  if (p.disc<m.disc) { tmp=p.disc; p.disc=m.disc; m.disc=tmp; }

  left  = m.disc-deltaD < Disc(rangs[i].min) ? Disc(rangs[i].min) : m.disc-deltaD;
  right = p.disc+deltaD > Disc(rangs[i].max) ? Disc(rangs[i].max) : p.disc+deltaD;

  fill.disc = randint (left, right);
} */

// Versió mes ràpida de l'anterior, que assumeix que deltaD=0
void BGA::ExtendedInterRecombin_INTEGER (Gene& p, Gene& m, Gene& fill, int i)
{
  if (p.disc<m.disc) { Disc tmp=p.disc; p.disc=m.disc; m.disc=tmp; }
  fill.disc = randint (m.disc, p.disc);
}


// Aquesta es l'equivalent del GA i dona 2 fills
//        --> n'agafem el millor dels dos
// Pre: n>=2
// Tal com esta muntat l'esquema, no va: aquesta treballa sobre tot el cromosoma

// void BGA::OnePointRecombin_UNIVERSAL (Gene& p, Gene& m, Gene& fill, int i)
// {
//   int i, cut= randint (1, n-1);

//   for (i=0; i<cut; i++)
//   {
//     popaux[fill]->cromo[i] = pop[pare]->cromo[i];
//     auxx[i] = pop[mare]->cromo[i];
//   }
//   for (; i<n; i++)
//   {
//     popaux[fill]->cromo[i] = pop[mare]->cromo[i];
//     auxx[i] = pop[pare]->cromo[i];
//   }
//   if (fitnessF(popaux[fill]->cromo) < fitnessF (auxx))
//     if (!minim) CopyCromo (popaux[fill]->cromo, auxx);
//     else;
//   else if (minim) CopyCromo (popaux[fill]->cromo, auxx);
// }


// Aquesta seria l'equivalent del GA i dona 2 fills 
//       --> n'agafem el millor dels dos 
// Pre: n>=3

/* void BGA::TwoPointRecombin_UNIVERSAL (Gene& p, Gene& m, Gene& fill, int i)
{
}
*/

// ************************************************
//             Funcions de Mutacio
// ************************************************


void BGA::Mutation (int fill)
{
  mutation_flag = CERT;
  for (int i=0; i<n; i++)
    if (flip(mut_rate))
      switch (tipus[i])
      {
        case REAL:    (this->*mutation_Real) (popaux[fill]->cromo[i].real, i); 
                      break;
        case FUZZY:   (this->*mutation_Fuzzy) (popaux[fill]->cromo[i].fn, i); 
                      break;
        case VARLING: (this->*mutation_Varling) (popaux[fill]->cromo[i].fq, i); 
                      break;
        case INTEGER: (this->*mutation_Integer) (popaux[fill]->cromo[i].disc, i); 
                      break;
        case NOMINAL: (this->*mutation_Nominal) (popaux[fill]->cromo[i].disc, i); 
                      break;
        case BINARY:  (this->*mutation_Binary) (popaux[fill]->cromo[i].disc, i); 
	              break;
        case ORDINAL: (this->*mutation_Ordinal) (popaux[fill]->cromo[i].disc, i); 
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

void BGA::DiscreteMutation_REAL (Real& x, int i)
{
  if (mutation_flag)
  {
    _delta = 0.0; Real pot = 1.0;
  
    for (int j=0; j<setup->k_precision; j++, pot *= 0.5)
      if (flip(k_precision_invers)) _delta += pot;
     mutation_flag = FALS;
  }

  x += random_sign()*rangs_aux[i]*_delta;
  if (x > rangs[i].max) x = rangs[i].max;
  if (x < rangs[i].min) x = rangs[i].min;
}

void BGA::ContinuousMutation_REAL (Real& x, int i)
{
  if (mutation_flag)
  {
     _delta = Real(pow(2.0, -setup->k_precision*random_01()));
     mutation_flag = FALS;
  }
  x += random_sign()*rangs_aux[i]*_delta;
  if (x > rangs[i].max) x = rangs[i].max;
  if (x < rangs[i].min) x = rangs[i].min;
}

void BGA::DiscreteMutation_FUZZY (TrianFN& x, int i)
{
  if (mutation_flag)
  {
    _delta = 0.0; Real pot = 1.0;
  
    for (int j=0; j<setup->k_precision; j++, pot *= 0.5)
      if (flip(k_precision_invers)) _delta += pot;
     mutation_flag = FALS;
  }

  int sign = random_sign(); 
  x.mode += sign*rangs_aux[i]*_delta;
  if (x.mode > rangs[i].max) x.mode = rangs[i].max;
  if (x.mode < rangs[i].min) x.mode = rangs[i].min;

  x.spread += sign*rangs_aux[i]*_delta; //*PERC_FUZZY;
  if (x.spread > rangs[i].max) x.spread = rangs[i].max;
  if (x.spread < rangs[i].min) x.spread = rangs[i].min;
}

void BGA::ContinuousMutation_FUZZY (TrianFN& x, int i)
{
  if (mutation_flag)
  {
     _delta = Real(pow(2.0, -setup->k_precision*random_01()));
     mutation_flag = FALS;
  }

  int sign = random_sign(); 
  x.mode += sign*rangs_aux[i]*_delta;
  if (x.mode > rangs[i].max) x.mode = rangs[i].max;
  if (x.mode < rangs[i].min) x.mode = rangs[i].min;

  x.spread += sign*rangs_aux[i]*_delta; //*PERC_FUZZY;
  if (x.spread > rangs[i].max) x.spread = rangs[i].max;
  if (x.spread < rangs[i].min) x.spread = rangs[i].min;
}

void BGA::ContinuousMutation_VARLING (TrapezFQ& x, int i)
{
  if (mutation_flag)
  {
     _delta = Real(pow(2.0, -setup->k_precision*random_01()));
     mutation_flag = FALS;
  }

  Real aux = random_sign()*rangs_aux[i]*_delta;

  x.mode1 += aux;
  if (x.mode1 > rangs[i].max) x.mode1 = rangs[i].max;
  if (x.mode1 < rangs[i].min) x.mode1 = rangs[i].min;
  x.mode2 += aux;
  if (x.mode2 > rangs[i].max) x.mode2 = rangs[i].max;
  if (x.mode2 < rangs[i].min) x.mode2 = rangs[i].min;

  x.spread1 += aux;
  if (x.spread1 > rangs[i].max) x.spread1 = rangs[i].max;
  if (x.spread1 < rangs[i].min) x.spread1 = rangs[i].min;
  x.spread2 += aux;
  if (x.spread2 > rangs[i].max) x.spread2 = rangs[i].max;
  if (x.spread2 < rangs[i].min) x.spread2 = rangs[i].min;
}

void BGA::RandomMutation_NOMINAL (Disc& x, int i)
{
  x = randint (Disc(rangs[i].min), Disc(rangs[i].max));
}

// Ja realitza el bounding = wrapping per defecte (els dos else's)
void BGA:: SwitchMutation_INTEGER (Disc& x, int i)
{
  if (flip(0.5))
    if (x<Disc(rangs[i].max)) x++;
    else x = Disc(rangs[i].min);
  else
    if (x>Disc(rangs[i].min)) x--;
    else x = Disc(rangs[i].max);
}


// Le he quitado el log()
// void BGA::MyMutation (int fill)
// {
//   Real _delta = 0.35;
//   Real aux;

//   for (int i=0; i<n; i++)
//     if (flip(mut_rate))
//     {
//       aux = popaux[fill]->cromo[i].real;
//       aux *= random_minMax(1.0-_delta, 1.0+_delta);
//       if (aux > rangs[i].max) aux = rangs[i].max;
//       if (aux < rangs[i].min) aux = rangs[i].min;
//       popaux[fill]->cromo[i].real = aux;
//     }
// }


// Es podrien posar politiques de: alfa, delta, rangbounds

// **************************************************************
// **************************************************************


void BGA::CreatePop (Cromo**& p)
{
  p = new Cromo*[popsize+1];
  CheckMem (p);
  p[0]=NULL;  // no se usa
  for (int i=1; i<=popsize; i++)
  {
    p[i] = new Cromo; 
    CheckMem (p[i]);
    p[i]->cromo = new Gene[n];
    CheckMem (p[i]->cromo);
  }
}

// Una xarxa te el codi n=0 o el n=20 (si es vol el nº de hits en CLASSIF)
Bool BGA::IsANN (void) 
{ 
  return (quin==0 || quin==20) ? CERT : FALS;
}

// Aqui es posa la informacio FIXE (popsize, problema i mida) pels NRuns
// S'assumeix que el problema ha estat definit 
BGA::BGA (int quants, int problema, int mida) 
{
  popsize= quants; 
  quin= problema; 

  // Agafar la mida per defecte o la que ens donen, si es una ANN
  if (IsANN()) n = mida; 
  else n = ElsProblemes[quin].n;

  printf ("Creando BGA de %d individuos\nProblema: %s\nNo. variables: %d\n",
          quants, ElsProblemes[problema].nom, n);
  CreatePop (pop);
  CreatePop (popaux);
 
  rangs = NULL; 
  tipus = NULL;
  auxx = NULL;
  stats.bestes = NULL;
  if (IsANN()) stats.TR_Bestes = stats.VA_Bestes = stats.TE_Bestes = NULL;
}

//***********************
// Funcions de Interficie
//***********************

void BGA::DataExternalSetup (Red* r, Data* d)
{
  red = r;
  data = d;
}

void BGA::Setup (BGASetup* s)
{
  setup = s;
}

// Calcula 'abs', 'mean', 'gmean', 'var' i 'gvar' de n Reals a 'r[]' i ho deixa a 'stats'
void Calc_Stats (Real r[], int n, Stats* s)
{
  int i;
  Real amean, gmean, gvar, avar, abs_best;

  amean= gmean= abs_best= r[0];
  for (i=1; i<n; i++)
  {
    amean += r[i];
    gmean = pow (gmean, Real(i)/Real(i+1)) * pow (r[i], 1.0/Real(i+1));
    if (r[i] < abs_best) abs_best = r[i]; // Asumimos que es_min=CERT
  }

  amean /= Real(n); 
  for (avar=gvar=0.0, i=0; i<n; i++)
  {
    gvar += (r[i] - gmean) * (r[i] - gmean);
    avar += (r[i] - amean) * (r[i] - amean);
  }
  if (n>1) { gvar /= Real(n-1); avar /= Real(n-1); }

  s->gmean = gmean;
  s->amean = amean;
  s->gvar = gvar;
  s->avar = avar;
  s->abs = abs_best;
}

void BGA::Finalize (void)
{
  Calc_Stats (stats.bestes, setup->NRuns, &stats);

  if (setup->Mplot) 
  { 
    fclose (fplotTR); 
    if (TR_MODE != TR_ONLY || PERCENT_TR < 100) fclose (fplotTE); 
  }
  if (setup->tracaTE) fclose (ftracaTE); 
}

// Retorna la mitja de NMSE si REGRESS i la mitja de % si CLASSIF
Real BGA::ShowANNResults (char s[], Real r[], int hits, int howmany)
{
  Real retornar;

  puts("\n***********"); puts (s); puts("***********\n"); 
  Calc_Stats (r, setup->NRuns, &stats);    
  printf ("MSE (Best) %g (Mean) %g (Variance) %g\n", 
          stats.abs, stats.amean, stats.avar);  

  Real tmp4 = 1.0/data->GetVar();
  printf ("NMSE (Best) %g (Mean) %g (Variance) %g\n", stats.abs/data->GetVar(), 
	  retornar = stats.amean/data->GetVar(), stats.avar*tmp4*tmp4);

  if (data->LMode()==CLASSIF)
    printf ("---> Mean Accuracy %3.3f%% <---\n", 
	    retornar = 100.0*Real(hits)/Real(howmany*setup->NRuns)); 
  return retornar;
}

// Retorna la mitja dels fitness en general, 
// si REGRESS,
//   la mitja dels NMSE en TR si TR_ONLY i PERCENT_TR = 100
//   la mitja dels NMSE en TE si TR_ONLY i PERCENT_TR < 100
//   la mitja dels NMSE en TE en altre cas
// si CLASSIF,
//   la mitja dels % en TR si TR_ONLY i PERCENT_TR = 100
//   la mitja dels % en TE si TR_ONLY i PERCENT_TR < 100
//   la mitja dels % en TE en altre cas.

Real BGA::ShowResults (void)
{
  Real retornar = stats.amean;
 
  if (IsANN())
  {
    retornar = ShowANNResults ("TRAINING", stats.TR_Bestes, stats.TR_Hits, data->HowManyTR());

    if (TR_MODE==TR_VA_TE || TR_MODE==TR_K_FOLD)
    {
      ShowANNResults ("VALIDATION", stats.VA_Bestes, stats.VA_Hits, data->HowManyVA());
      retornar = ShowANNResults ("TEST", stats.TE_Bestes, stats.TE_Hits, data->HowManyTE());
      printf ("NMSE at min VA %g", stats.TEAbsError/data->GetVar());
      if (data->LMode()==CLASSIF) printf (" (%3.3f%%)", stats.TEAbsAcc);
      printf (" (run %d)\n\n", stats.run_of_min_VA_error);
    }
    else if (PERCENT_TR < 100) 
      retornar = ShowANNResults ("TEST", stats.TE_Bestes, stats.TE_Hits, data->HowManyTE());
  }
  else printf ("\n\nFITNESS (Best) %g (Mean) %g (Variance) %g\n", 
	       stats.abs, stats.amean, stats.avar);  

  return retornar;
}
     

void BGA::ReFinalize (int run)
{
  if (setup->plot) { fprintf (fplot, "%g\n", GetBest()); fclose (fplot); }
  if (setup->tracaTE) fclose (ftracaTE);
  if (setup->verbose_full) Print (gens);
  if (setup->verbose) printf ("Abs Best %g\n", GetBest());
  stats.bestes[run-1] = GetBest();

  if (IsANN()) 
  { 
    // Calcular errores en TRAINING
    red->SetWeightDyn (GetBestCromo());
    red->TestRedData (data, data->TRleft(), data->TRright(), setup->Mplot, fplotTR);
    stats.TR_Bestes[run-1] = red->Mse();
    if (data->LMode()==CLASSIF) stats.TR_Hits += red->Hits();

    if (TR_MODE==TR_VA_TE || TR_MODE==TR_K_FOLD) 
    {
      // Calcular errores en VALIDATION
      red->SetWeightDyn (GetBestAux());
      red->TestRedData (data, data->VAleft(), data->VAright(), FALS, NULL);
      stats.VA_Bestes[run-1] = red->Mse();
      if (data->LMode()==CLASSIF) stats.VA_Hits += red->Hits();
      // Calcular errores en TEST
      red->TestRedData (data, data->TEleft(), data->TEright(), setup->Mplot, fplotTE);
      stats.TE_Bestes[run-1] = red->Mse();
      // Esto es para el Max TE (min VA)
      if (stats.VA_Bestes[run-1] < stats.VAAbsError)
      { 
        stats.VAAbsError = stats.VA_Bestes[run-1];
        stats.TEAbsError = stats.TE_Bestes[run-1];
        if (data->LMode()==CLASSIF) stats.TEAbsAcc = red->HitsP();
        stats.run_of_min_VA_error = run;
      }
      if (data->LMode()==CLASSIF) stats.TE_Hits += red->Hits();
    }
    else if (PERCENT_TR < 100)
         {
           // Calcular errores en TEST
           red->TestRedData (data, data->TEleft(), data->TEright(), setup->Mplot, fplotTE);
           stats.TE_Bestes[run-1] = red->Mse();
	   if (data->LMode()==CLASSIF) stats.TE_Hits += red->Hits();
	 }

    // OJO: al salvar la red se salvan los errores en TE, pues fueron los
    // últimos evaluados. Quizá separarlos
    if (setup->save) 
    {
      run_to_name (run, setup->path_out, "/red", EXT_RED, plotfile);
      if ((fplot=fopen(plotfile,"w"))==NULL)
        printf("Warning: Can't open output file: %s.\n", plotfile);
      else switch (TR_MODE)
	   {
  	     case TR_VA_TE: red->SetWeightDyn (GetBestAux());
	     case TR_ONLY:  red->SaveRed (plotfile, data);
	   }
    }

    if (setup->save_out) 
    {
      run_to_name (run, setup->path_out, "/out", EXT_OUT, plotfile);
      if ((fplot=fopen(plotfile,"w"))==NULL)
        printf("Warning: Can't open output file: %s.\n", plotfile);
      else red->SaveOut (plotfile, data, 1, data->NDades());
    }

  }
}

void BGA::ReInitialize (int run)
{
 int i;

 if (setup->verbose_full) 
  printf ("\nSolving problem ... %s (%s): Run %d\n", 
          ElsProblemes[quin].nom, ElsProblemes[quin].comment, run); 
 gens= 0;

 if (IsANN())
 {
   stats.VABestError = MAXFLOAT;
   stats.VAGens = 0;
 }

 if (setup->plot) 
 {
   run_to_name (run, setup->path_out, "/plots", EXT_GNU, plotfile);
   if ((fplot=fopen(plotfile,"w"))==NULL)
     printf("Warning: Can't open output file: %s.\n", plotfile);
 }
  
 if (setup->tracaTE)
 {
   run_to_name (run, setup->path_out, "/traca", ".TE", plotfile);
   if ((ftracaTE=fopen(plotfile,"w"))==NULL)
     printf("Warning: Can't open output file: %s.\n", plotfile);
 }
}

void BGA::Initialize (void)
{
  int i;

  gens= 0;
  mut_rate= 1.0/n;
  recomb_rate= 1.0;   // no se utiliza
  k_precision_invers = 1.0/Real(setup->k_precision);
  trunc = setup->trunc;
  delta= 0.45;
  deltaD = 0;   
  q = 1;      
  fixedalfa= 0.5;
  generations= int(Real(setup->FFevals)/Real(popsize));

  auxx = new Gene[n];      CheckMem (auxx);
  rangs = new Rang[n];     CheckMem (rangs);
  tipus = new TiposH[n];   CheckMem (tipus);
  rangs_aux = new Real[n]; CheckMem (rangs_aux);

  terminF= &BGA::MaxGens;
  selectF= &BGA::RandomSelect;
  //recombin_Real    = ExtendedInterRecombin_Range_REAL;
  //recombin_Real    = FuzzyRecombin_REAL;
  //recombin_Real    = DiscreteRecombin_UNIVERSAL;
  //mutation_Real    = DiscreteMutation_REAL;
  //recombin_Fuzzy   = ExtendedInterRecombin_Range_FUZZY;
  //recombin_Integer = ExtendedInterRecombin_INTEGER;
  recombin_Real    = &BGA::ExtendedInterRecombin_REAL;
  recombin_Fuzzy   = &BGA::ExtendedInterRecombin_FUZZY;
  recombin_Varling = &BGA::ExtendedInterRecombin_VARLING;
  recombin_Nominal = &BGA::DiscreteRecombin_UNIVERSAL;
  recombin_Integer = &BGA::LineRecombin_INTEGER;
  recombin_Ordinal = &BGA::LineRecombin_INTEGER;
  recombin_Binary  = &BGA::DiscreteRecombin_UNIVERSAL;

  mutation_Real    = &BGA::ContinuousMutation_REAL;
  mutation_Fuzzy   = &BGA::ContinuousMutation_FUZZY;
  mutation_Varling = &BGA::ContinuousMutation_VARLING;
  mutation_Nominal = &BGA::RandomMutation_NOMINAL;
  mutation_Integer = &BGA::SwitchMutation_INTEGER;
  mutation_Ordinal = &BGA::SwitchMutation_INTEGER;
  mutation_Binary  = &BGA::RandomMutation_NOMINAL;

  printf ("\nSolving problem ... '%s' (%s)\n", 
          ElsProblemes[quin].nom, ElsProblemes[quin].comment); 
  fitnessF= ElsProblemes[quin].fitness;
  minim= ElsProblemes[quin].es_min;
  best= ElsProblemes[quin].best;

  // En general, posar els rangs i tipus declarats
  for (i=0; i<n; i++) 
  {
    rangs[i] = ElsProblemes[quin].rang;
    tipus[i] = ElsProblemes[quin].tipo;
  }

  // Si es una XN, posar els rangs i els tipus de les dades
  if (IsANN()) 
    red->Omple_Rangs_Tipus (rangs, data->GetRangs(), tipus, data->TiposIn());
  else // Fer accions init. addicionals si toca (nomes per FP)
    if (quin==19) Ini_FP ();

  for (i=0; i<n; i++) 
    rangs_aux[i] = (rangs[i].max-rangs[i].min)*setup->rang_ratio;

  /*  puts("Rangos y Tipos puestos:");
  for (i=0; i<n; i++) 
  printf ("rangs[%d] = [%g, %g] (%d)\n", i, rangs[i].min, rangs[i].max, tipus[i]); */

  // Plots de TR i TE

  if (setup->Mplot)
  {
     plotfile[0]= ENDTOKEN;
     strcat (plotfile, setup->path_out);
     strcat (plotfile, "/resul.TR");
     if ((fplotTR=fopen(plotfile,"w"))==NULL)
        printf("Warning: Can't open output file: %s.\n", "resul.TR");

     plotfile[0]= ENDTOKEN;
     strcat (plotfile, setup->path_out);
     strcat (plotfile, "/resul.TE");
     if (TR_MODE != TR_ONLY || PERCENT_TR < 100)
       if ((fplotTE=fopen(plotfile,"w"))==NULL)
          printf("Warning: Can't open output file: %s.\n", "resul.TE");
  }
 
 
  // Inicialitzar les estadístiques

  CheckMem (stats.bestes = new Real[setup->NRuns]);
  if (IsANN()) 
  {  
    stats.TR_Hits = stats.VA_Hits = stats.TE_Hits = 0;
    stats.VAAbsError = stats.TEAbsError = stats.TEAbsAcc = MAXFLOAT;

    CheckMem (stats.TR_Bestes = new Real[setup->NRuns]);
    CheckMem (stats.VA_Bestes = new Real[setup->NRuns]);
    CheckMem (stats.TE_Bestes = new Real[setup->NRuns]);
    CheckMem (stats.meansimils_i = new Real[data->NIns()+1]);

    for (i=1; i<=data->NIns(); i++) 
      stats.meansimils_i[i] = 0.0;
    stats.meansimil = 0.0;
    stats.finesimil = 0.0;
  }
  incy = 1;
  do incy *= 3; while (++incy<=popsize);  // pel ShellSort
}

void BGA::IniPopRandom (void)
{
  int i,j;

  for (i=1; i<=popsize; i++)
  {
    for (j=0; j<n; j++)
      switch (tipus[j])
      {
        case REAL:    pop[i]->cromo[j].real = 
                        random_minMax (rangs[j].min, rangs[j].max);
                      break;
        case FUZZY:   pop[i]->cromo[j].fn.mode = 
                        random_minMax (rangs[j].min, rangs[j].max);
                      pop[i]->cromo[j].fn.spread = 
                        random_minMax (rangs[j].min, rangs[j].max)*PERC_FUZZY;
                      break;
        case VARLING: if (flip(0.5))
	              {
                        pop[i]->cromo[j].fq.mode1 =
                           random_minMax (rangs[j].min, rangs[j].max);
                        pop[i]->cromo[j].fq.mode1 = 
                           random_minMax (pop[i]->cromo[j].fq.mode1, rangs[j].max); 
		      }
                      else
	              {
                        pop[i]->cromo[j].fq.mode2 =
                           random_minMax (rangs[j].min, rangs[j].max);
                        pop[i]->cromo[j].fq.mode1 = 
                           random_minMax (rangs[j].min, pop[i]->cromo[j].fq.mode2);
		      }
                      pop[i]->cromo[j].fq.spread1 = random_01 (); 
                      pop[i]->cromo[j].fq.spread2 = random_01 (); 
                      break;
        case INTEGER: 
        case ORDINAL: 
        case NOMINAL: 
        case BINARY:  pop[i]->cromo[j].disc = 
                        randint (Disc(rangs[j].min), Disc(rangs[j].max));
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
    pop[i]->fitness = 0.0;
  }
}

void BGA::Print (int gen)
{
  int i,j;

  printf ("*** Generation: %d ***\n", gen); 

  for (i=1; i<=popsize; i++)
  {
    printf ("Indi %d:", i);
    for (j=0; j<n; j++)
      switch (tipus[j])
      {
        case REAL:    printf (" %g ", pop[i]->cromo[j].real);                   
                      break;
        case FUZZY:   printf (" %g (%g)", 
                        pop[i]->cromo[j].fn.mode, pop[i]->cromo[j].fn.spread);
                      break;
        case VARLING: printf (" (%g %g %g %g)", 
                        pop[i]->cromo[j].fq.spread1, pop[i]->cromo[j].fq.mode1,
                        pop[i]->cromo[j].fq.spread2, pop[i]->cromo[j].fq.mode2);
                      break;
        case INTEGER:
        case NOMINAL:
        case ORDINAL: 
        case BINARY:  printf (" %d ", pop[i]->cromo[j].disc);
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
    printf ("(%g)\n", pop[i]->fitness);
  }
}

// Aqui es posen coses relatives al problema concret,
// que s'hagin de fer al final de cada generacio
// ******** OJO: si se cambia la FITNESSF, esto no lo refleja *********
void BGA::AdditionalWork (void)
{
 if (IsANN())
  if (TR_MODE==TR_VA_TE || TR_MODE==TR_K_FOLD) 
  {
    red->SetWeightDyn (GetBestCromo());
    red->TestRedData (data, data->VAleft(), data->VAright(), FALS, NULL);
    if (red->Error() < stats.VABestError)
    { 
      CopyCromo (auxx, GetBestCromo());
      stats.VABestError = red->Error();
      stats.VAGens = gens;
    } 
  }
  else // TR_MODE==TR_ONLY
    if (setup->tracaTE && (gens%setup->each)==0)
    {
      red->SetWeightDyn (GetBestCromo());
      red->TestRedData (data, data->TEleft(), data->TEright(), CERT, ftracaTE);
    }  
}

void BGA::Exec (void)
{
  int i, pare, mare, run, TrunCPopsize;

  Initialize();
  TrunCPopsize = int(Real(trunc*popsize)/100.0);

  for (run=1; run<= setup->NRuns; run++)
  {
    IniPopRandom ();
    ReInitialize (run);
    UpdateFitness ();
    SortPop ();

    do          
    {
      if (setup->plot && gens%setup->each==0) 
        fprintf (fplot, "%g\n", GetBest());
      AddBest ();                  
                                   
      for (i=1; i<=popsize-q; i++)
      {      
        (this->*selectF) (pare, mare, TrunCPopsize);
        Recombination (pare, mare, i);
        Mutation (i);
      }

      Swap_pops ();
      UpdateFitness ();
      SortPop ();
      UpdateGenCounter ();
      AdditionalWork ();
  
    } while (!(this->*terminF) ());

    ReFinalize (run);
  }
  Finalize();
}

void BGA::Swap_pops (void)
{
  Cromo** tmp = popaux;
  popaux = pop;
  pop = tmp;
}


void BGA::UpdateFitness (void)
{
  for (int i=popsize; i; i--)
    pop[i]->fitness = (this->*fitnessF) (pop[i]->cromo);
}


BGA::~BGA ()
{
  for (int i=1; i<=popsize; i++)
  {
    delete [] (pop[i]->cromo);
    delete pop[i];
    delete [] (popaux[i]->cromo);
    delete popaux[i];
  }
  delete [] pop;
  delete [] popaux;
  delete [] auxx;
  delete [] rangs;
  delete [] rangs_aux;
  delete [] stats.bestes;
}

/*
// Dona CERT si c1<c2 i es_min = CERT (es un prob. de min)
Bool BGA::compare_cromos (Cromo* c1, Cromo* c2)
{
  return (minim ? c1->fitness > c2->fitness : c1->fitness < c2->fitness);
}
*/

// Dona CERT si c1>c2 (suposa que minim=CERT)
inline Bool BGA::compare_cromos (Cromo* c1, Cromo* c2)
{
  return c1->fitness > c2->fitness;
}

void BGA::ShellSort (Cromo* p[], int nn)
{
  int i, inc=incy, bb;
  Cromo* c;

  do
  {
    inc /= 3;
    for (i= inc+1; i<=nn; i++)
    { 
      c= p[i];
      bb= i-inc;
      while ( compare_cromos(p[bb], c) ) 
      {
        p[bb+inc]= p[bb];
        bb -= inc;
        if (bb<=0) break;
      }
      p[bb+inc]= c;
    }
  } while (inc>1);
}

// Torna la pos del minim de p entre i, j  (suposa que minim=CERT)
int BGA::cerca_elim (Cromo* p[], int i, int j)
{
  while (i!=j)
   if (p[i]->fitness>p[j]->fitness) i++; else j--;
  return i;
}


// Nomes ordena una quarta part (i.e., va be per tau<=25)
void BGA::MySort (Cromo* p[], int nn)
{
  int i, pos;
  Cromo* c;

  for (i= 1; i<=(nn >> 2); i++)
  {
    pos = cerca_elim (p, i, nn);
    c=p[i]; p[i]=p[pos]; p[pos]=c;
  }
}

// Ordena en ordre ascendent (el millor queda en pop[1])
void BGA::SortPop (void)
{
  ShellSort (pop, popsize);
}

