//DATASET.H

#ifndef _DATA_
#define _DATA_

#include <stdio.h>

#include "config.h"
#include "defs.h"

struct DataStats
{
  Real* M;
  Real* m;
  Real* mean;
  Real* stddev;
  Real* divi;
  Real* rest;
  int* tmp;
  //  Real** probs;
  Real* pmiss;
  Real overall_var;
};

class Data
{
 private:
 // Info del .cfg
  int nIns, nOuts;         // no. d_entrades/sortides
  TiposH* tipus;           // vector de tipus de les entrades/sortides
  L_MODE lmode;            // Learning mode
  N_MODE nmode;            // Normalization mode
  ErrorFun errorF;         // ErrorFun to be used
  char cfgfile[MAXLINE];   // nom del fitxer de .cfg

 // Info del .in
  int nDades;              // no. de patrons
  ParamH** ins;            // vector dels vectors entrada
  ParamH** outs;           // vector dels vectors sortida
  ParamH* real_outs;       // vector de les sortides realimentades

  int TR_left, TR_right;     // limits dels TR, VA, TE 
  int VA_left, VA_right;
  int TE_left, TE_right;

  int quants_missing;
  Rang* new_rang;          // para dejar los rangos de las entradas, 
                           // tanto si se normaliza como si no
  int per_class[MAXCLASS]; // balance en classes (si CLASSIF)
  char infile[MAXLINE];    // nom del fitxer de .in

  DataStats datastats;     // Estadistiques del .in
 
  Bool error;              // diu si la darrera operacio (Read, Save) ana be
  ulong line;              // per lectura interna
  FILE* f;
  char token[MAXLINE];    

  // Funcions auxiliars ocultes
  void IniData (void);
  void FiData  (void);
  void ReadInputPattern (int, int);
  void ReadOutputPattern (int, int);
  void ReadDadesF (void);
  void Preprocess(void);
  void Missinger (int);       // pone missing a voluntad
  void Ini_DataStats (int);
  void Calc_Stats (void);
  void Sweep_Stats (void);

  public:

  // Constructora por defecto
  Data ();

  // Lee de un fichero de config 'path'
  void ReadConfigF (char path[]);

  // Lee de un fichero de datos 'path'
  void ReadData (char path[]);  

  // Dice si hay datos
  Bool EmptyData () { return nDades<=0; }; 

  // Dice si hay error
  Bool ErrorinData () { return error; }; 

  // Mostra per pantalla
  void ShowData () const;

  // Destructora por defecto
  ~Data ();

  // Devuelve el vector de entrada i 
  ParamH* In (int i);

  // Devuelve el vector de salida i 
  ParamH* Out (int i);

  // Chapucilla (estan los tipos de e/s consecutivos)
  TiposH* TiposIn (void) { return tipus; }
  TiposH* TiposOut (void) { return tipus+nIns; }

  int NDades (void)  const { return nDades; }  
  int NIns (void)    const { return nIns;   } 
  int NOuts (void)   const { return nOuts;  } 

  int TRleft (void)  const { return TR_left;  } 
  int TRright (void) const { return TR_right; } 
  int VAleft (void)  const { return VA_left;  } 
  int VAright (void) const { return VA_right; } 
  int TEleft (void)  const { return TE_left;  } 
  int TEright (void) const { return TE_right; } 

  void SetTRleft  (int t) { TR_left=t;  } 
  void SetTRright (int t) { TR_right=t; } 
  void SetVAleft  (int t) { VA_left=t;  } 
  void SetVAright (int t) { VA_right=t; } 
  void SetTEleft  (int t) { TE_left=t;  } 
  void SetTEright (int t) { TE_right=t; } 

  int HowManyTR (void) const { return TR_right-TR_left+1; } 
  int HowManyVA (void) const { return VA_right-VA_left+1; } 
  int HowManyTE (void) const { return TE_right-TE_left+1; } 

  Rang* GetRangs (void) { return new_rang; }  
  Real GetVar (void)    { return datastats.overall_var; }
  char* Cfgfile (void)  { return cfgfile; }  
  char* Infile (void)   { return infile; }  
  L_MODE LMode (void)   { return lmode; }  
  ErrorFun GetErrorFun (void) { return errorF; }
  void DataSwap (int, int, int);
  void SetMissingROut (int);
  void AbocarRealim (int);
  void SetRout (int, Real);
};

#endif 
