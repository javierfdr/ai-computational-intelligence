// breederGA

#ifndef _BREEDER_
#define _BREEDER_

/************************************************************
   SE ASUME DE MOMENTO TODO REALes
   Los cromo empiezan en [0]
************************************************************/

#include "config.h"
#include "defs.h"
#include "random.h"
#include "neurona.h"
#include "data.h"

// De que son los cromosomas (i.e. que es un Gene)
typedef ParamH Gene;

// Defs. varias

struct Cromo
{
  Gene* cromo;
  Real fitness;
};

struct Stats
{
// Generals
  Real* bestes;
  Real amean, gmean, abs, avar, gvar;

// Si ANN  
  int TR_Hits, VA_Hits, TE_Hits;
  Real* TR_Bestes;
  Real* VA_Bestes;
  Real* TE_Bestes;
  Real VABestError;   // across 1 run
  int VAGens;
  Real VAAbsError;   // across NRuns 
  Real TEAbsError;   // across NRuns (corresp. to VAAbsError)
  Real TEAbsAcc;     // idem pel % (si CLASSIF)
  int run_of_min_VA_error;
  Real* meansimils_i;
  Real  meansimil;
  Real  finesimil;
};

class BGA;
typedef Bool (BGA::*TerminF)   (void);
typedef void (BGA::*SelectF)   (int&, int&, int);
typedef Real (BGA::*FitnessF)  (Gene*);

typedef void (BGA::*Recombination_REAL)    (Gene&, Gene&, Gene&, int);
typedef void (BGA::*Recombination_FUZZY)   (TrianFN&, TrianFN&, TrianFN&, int);
typedef void (BGA::*Recombination_VARLING) (TrapezFQ&, TrapezFQ&, TrapezFQ&, int);
typedef void (BGA::*Recombination_NOMINAL) (Gene&, Gene&, Gene&, int);
typedef void (BGA::*Recombination_INTEGER) (Gene&, Gene&, Gene&, int);
typedef void (BGA::*Recombination_ORDINAL) (Gene&, Gene&, Gene&, int);
typedef void (BGA::*Recombination_BINARY)  (Gene&, Gene&, Gene&, int);

typedef void (BGA::*Mutation_REAL)    (Real&, int);
typedef void (BGA::*Mutation_FUZZY)   (TrianFN&, int);
typedef void (BGA::*Mutation_VARLING) (TrapezFQ&, int);
typedef void (BGA::*Mutation_NOMINAL) (Disc&, int);
typedef void (BGA::*Mutation_INTEGER) (Disc&, int);
typedef void (BGA::*Mutation_ORDINAL) (Disc&, int);
typedef void (BGA::*Mutation_BINARY)  (Disc&, int);

class BGA
{
  Cromo** pop;                     // Population
  Cromo** popaux;                  // Population (auxiliar)  

// Info. externa (setup)
  int popsize;                     // Population size
  Real epsilon;                    // precissio d_apropament al best
  int generations;                 // max. no. de generations permeses 
  int trunc;                       // Truncation threshold
  int q;                           // Pel q-elitisme

  Recombination_REAL    recombin_Real;
  Recombination_FUZZY   recombin_Fuzzy;
  Recombination_VARLING recombin_Varling;
  Recombination_NOMINAL recombin_Nominal;
  Recombination_INTEGER recombin_Integer;
  Recombination_ORDINAL recombin_Ordinal;
  Recombination_BINARY  recombin_Binary;

  Mutation_REAL         mutation_Real;
  Mutation_FUZZY        mutation_Fuzzy;
  Mutation_VARLING      mutation_Varling;
  Mutation_NOMINAL      mutation_Nominal;
  Mutation_INTEGER      mutation_Integer;
  Mutation_ORDINAL      mutation_Ordinal;
  Mutation_BINARY       mutation_Binary;

  TerminF   terminF;
  SelectF   selectF;
  FitnessF  fitnessF;

// Info. del problema
  int quin;                        // No. del problema que estem ressolent
  int n;                           // no. de genes/individu (s_agafa del quin)
  Real best;                       // Absolute best (ideal)

  Red* red;                        // Info. externa si IsANN()
  Data* data;

  Rang* rangs;                     // Rangs i tipus de les vars.
  TiposH* tipus;

  Stats stats;                     // estadistiques dels runs 
  BGASetup* setup;                 // External setup

  int gens;                        // generation counter
  Real mut_rate;                   // mutation rate
  Real recomb_rate;                // recombination rate
  Bool minim;                      // CERT <=> es un problema de minimització

  Real fixedalfa, delta;           // params. de la Recombination
  Disc deltaD;

  FILE* fplot;                     // fitxer on es fara el plot
  FILE* fplotTR;
  FILE* fplotTE;                   // fitxers on es faran el Mplots
  FILE* ftracaTE;                  // fitxers on es faran les traces de TE

  char plotfile[MAXLINE];          // Path temporal dels fitxers de plot

  Gene* auxx;                      // info. auxiliar per calculs varis
  Real _delta;                     // per la mutacio
  Bool mutation_flag;              //     idem
  Real k_precision_invers;         //     idem
  int incy;                        // pel ShellSort
  Real* rangs_aux;                 // conte (max-min)*rang_ratio

// Funcions Privades

  void IniPopRandom (void); 
  void CreatePop (Cromo**&);
  void Initialize (void);          // initializes everything
  void ReInitialize (int);         // re-initializes what varied
  void Finalize (void);            // closes everything
  void ReFinalize (int);           //       
  void UpdateFitness (void);
  void AdditionalWork (void);
  void SortPop (void);
  void AddBest (void);
  Real GetBest (void);
  Gene* GetBestCromo (void);
  Gene* GetBestAux (void);

  Bool compare_cromos (Cromo*, Cromo*);
  void ShellSort (Cromo* p[], int n);
  int cerca_elim (Cromo* p[], int i, int j);
  void MySort (Cromo* p[], int nn);

  void Swap_pops (void);
  void UpdateGenCounter (void) { gens++; }
  void CopyCromo (Gene* destin, Gene* source);
  Bool IsANN (void);
  Real ShowANNResults (char [], Real [], int, int);

  Bool MaxGens (void);
  Bool WithinBest (Real);
  Bool WithinZero (Real);
  Bool Composite (void);

  void RandomSelect (int&, int&, int t);

  // UNIVERSAL vol dir que serveixen per tots els tipus de dades

  void Recombination (int, int, int);
  void DiscreteRecombin_UNIVERSAL (Gene&, Gene&, Gene&, int);
  void OnePointRecombin_UNIVERSAL (Gene&, Gene&, Gene&, int);
  void TwoPointRecombin_UNIVERSAL (Gene&, Gene&, Gene&, int);

  void ExtendedInterRecombin_REAL       (Gene&, Gene&, Gene&, int);
  void ExtendedInterRecombin_Range_REAL (Gene&, Gene&, Gene&, int);
  void FuzzyRecombin_REAL               (Gene&, Gene&, Gene&, int);

  void ExtendedInterRecombin_FUZZY       (TrianFN&, TrianFN&, TrianFN&, int);
  void ExtendedInterRecombin_Range_FUZZY (TrianFN&, TrianFN&, TrianFN&, int);

  void ExtendedInterRecombin_VARLING (TrapezFQ&, TrapezFQ&, TrapezFQ&, int);

  void LineRecombin_INTEGER          (Gene&, Gene&, Gene&, int);
  void ExtendedInterRecombin_INTEGER (Gene&, Gene&, Gene&, int);

  void Mutation (int);
  void DiscreteMutation_REAL (Real&, int);
  void ContinuousMutation_REAL (Real&, int);

  void DiscreteMutation_FUZZY (TrianFN&, int);
  void ContinuousMutation_FUZZY (TrianFN&, int);

  void ContinuousMutation_VARLING (TrapezFQ&, int);

  void RandomMutation_NOMINAL (Disc&, int);
  void SwitchMutation_INTEGER (Disc&, int);

  Real FixedAlfaSelection (void);
  Real UniformAlfa_BGADelta_Selection (void);
  Real GaussianAlfa_BGADelta_Selection (void);
  Real UniformAlfa_RangeDelta_Selection (int, Real, Real);
  Real GaussianAlfa_RangeDelta_Selection (int, Real, Real);

  inline Bool change (void) { return gens>(generations*setup->change_amount); }

public:

  BGA (int, int, int);
  void DataExternalSetup (Red*, Data*);
  void Setup (BGASetup*);
  void Exec (void);
  Real ShowResults (void);
  void Print (int);
  ~BGA ();

// Funcions de fitness a disposicio

  Real ErrorRed (Gene*);
  Real F1    (Gene*);
  Real F2    (Gene*);
  Real F3    (Gene*);
  Real F3bis (Gene*);
  Real F4    (Gene*);
  Real F5    (Gene*);
  Real F6    (Gene*);
  Real F7    (Gene*);
  Real F8    (Gene*);
  Real F9    (Gene*);
  Real F10   (Gene*);
  Real F11   (Gene*);
  Real F12   (Gene*);
  Real F8F2  (Gene*);

  void Ini_FP (void);
  Real FP    (Gene*);
  Real ErrorRedP (Gene*);
};

#endif
