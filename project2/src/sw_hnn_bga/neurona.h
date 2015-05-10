// neurona.h

#ifndef _NEURONA_
#define _NEURONA_

#include <stdlib.h> // NULL
#include "defs.h"
#include "config.h"
#include "data.h"

// Esto es fijo, pero no creo que necesite más
#define MAXCAPES 3

class Capa
{
  TipoNeurona tipo_neu; // tipo de las neuronas
  int ini, fi;          // primera-ultima de la capa en la tabla de neuronas
  int nins;             // nº de entradas
  TiposH* tipos_in;     // tipos entradas  
  InputFun inputF;      // funcion de net input       
  ActivFun activF;      // funcion de activacion

 public:

  Capa ();
  void SetNins (int n) { nins=n; }
  void SetActivF (ActivFun a) { activF=a; }
  void Set (TipoNeurona, int, int, InputFun, ActivFun);
  void EvalCapa (ParamH* neus[], ParamH in[], ParamH out[]);
 
  int Ini (void) { return ini; }
  int Fi (void) { return fi; }
  int Nneus (void) { return fi-ini+1; }
  int Nins (void) { return nins; }
  TiposH* TiposIn     (void) { return tipos_in; }
  void SetTiposIn     (TiposH* t) { tipos_in=t; }
  TipoNeurona TipoNeu (void) { return tipo_neu; }
  InputFun GetNetI    (void) { return inputF; }
  ActivFun GetActiv   (void) { return activF; }
};


class Red
{
 private:

  int nneurones;          // nº de neuronas totales
  ParamH** neurones;      // tabla de neuronas (de sus pesos)
  int ncapes;             // nº de capas 
  Capa capes[MAXCAPES+1]; // y las capas
  ParamH* tmp;            // para guardar cosas intermedias
  ParamH* resultat;       // para guardar el resultado de un Eval
  TiposH* tipos_out;      // los tipos de las salidas
  int npesos;             // nº total de pesos
  ErrorFun errorF;        // f. de error que se aplica    
  Real error, mse, nmse;  // resultados del error actuales
  int  hits;              // nº of hits (si CLASSIF)
  Real hitsP;             //           su %

  Bool softmax;  
  Bool error_IO;          // diu si la darrera operacio (Read, Save) anà be
  Bool weights_loaded; // diu si hi ha pesos carregats
  ulong line;          // per lectura/escriptura interna (de fitxer)
  FILE* f;             // idem

  void Init (void);
  void ReInit (void);
  void ReadWeightsF (void);

 public:

  Red ();
  Bool EmptyRed (void) { return nneurones<=0; }

  // f. de error *externa* a utilizar
  Real error_patroF (ParamH out[]);

  // Dice si hay error en E/S, memoria pedida, ...
  Bool ErrorinRed (void) { return error_IO; } 
  int Npesos (void) { return npesos; }
  int Ncapes (void) { return ncapes; }
  Real Error (void) { return error; } 
  Real Mse (void)   { return mse; } 
  int Hits (void)   { return hits; } 
  Real HitsP (void)   { return hitsP; } 
  ParamH* Resultat (void) { return resultat; }
  Bool WeightsLoaded (void) { return weights_loaded; }

  void EvalRed (ParamH []);
  void ReadRed (char [], Bool, Data&);
  void SaveRed (char [], Data*);
  void SaveOut (char [], Data*, int, int);
  void TestRedData (Data*, int, int, Bool, FILE*);
  void PlotHidden (Data*);
  void PlotOutputs (Data*);

  void SetWeight (int, ParamH*, int); 
  ParamH* GetWeight (int);
  void SetWeightDyn (ParamH*);
  void SetErrorFun (ErrorFun eF) { errorF = eF; }
  void PrintRed (void);
  void DiscardWeights(void);
  void Omple_Rangs_Tipus (Rang [], Rang [], TiposH [], TiposH []);
  ~Red ();

}; 

 
#endif
