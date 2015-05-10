#ifndef _DEFS_ 
#define _DEFS_

#include <math.h> 
#include <values.h> // MAXFLOAT 

#define FALS 0
#define CERT 1      // a la antigua usanza
typedef int Bool;

#define MIN(a,b) ( (a) < (b) ? (a) : (b) )
#define MAX(a,b) ( (a) < (b) ? (b) : (a) )

// Tipos de las entradas de una neurona 
enum TiposH { REAL, FUZZY, INTEGER, VARLING, NOMINAL, BINARY, ORDINAL, SET, BITSTRING, PROBABILITY, INTERVAL};

// Tipos de neuronas
enum TipoNeurona { NORMAL, HETEROGENEA, FULLFUZZY };

// Tipos de Learning Modes
enum L_MODE { CLASSIF, REGRESS, PREDICT };

// Tipos de Normalization Modes
enum N_MODE { NONE, MINUSMEAN, PROBEN, STANDARD, MIDRANG };
 
// Abreviaturas
typedef unsigned int  uint;
typedef unsigned long ulong;

// Representacion usada para cada tipo de 'TiposH'
typedef double Real;        // Per REAL, PROBABILITY
typedef unsigned int Disc;  // Per NOMINAL, INTEGER, ORDINAL

struct TrianFN
   {
     Real spread;
     Real mode;
   };                       // Per FUZZY, INTERVAL

struct TrapezFQ
   {
     Real spread1, spread2;
     Real mode1, mode2;     // Per VARLING
   };
 
//Tipo General de entrada/salida
struct ParamH
{ 
  Bool missing;
  unsigned char joker;    // este es un campo multiusos
  union 
  { 
    Real real; 
    Disc disc;
    TrianFN fn;
    TrapezFQ fq;
  }; 
};   

//Faltan SET y BITSTRING (es lo steso). Una idea es tomar el set.cc del SFSA
// y el campo será un pointer to set

// Tractament dels MISSING

inline Bool IsMissing (ParamH& x) { return x.missing; }
inline void SetMissing (ParamH& x, Bool w) { x.missing = w; }


struct Rang
{
  Real min, max;  // se aprovecha el Real tb para el Disc
};
 

// Fuzziness dels FUZZY
#define PERC_FUZZY 0.05 // 0.005  

// Pel VARLING (crossing at 0.5)
#define DX         0.25        

#define MAXCLASS   10 // nº máximo de clases en CLASSIF

enum { TR_ONLY, TR_VA_TE, TR_K_FOLD };

// En cas de seleccionar TR_K_FOLD, a mes de posar una K, posar el PERCENT_TR
// corresponent. Per exemple, K=5 --> PERCENT_TR = 60 (3/K * 100)
// El PERCENT_VA no es fa servir en aquest cas.

#define TR_MODE    TR_VA_TE //TR_ONLY
#define PERCENT_TR 50        // % de TR en cas TR_MODE=TR_ONLY (la resta es TE)
#define PERCENT_VA 25  
#define K_FOLD      5          // K>=3

/////////// Declaracio de funcions internes de la neurona 

typedef Real (*InputFun) (ParamH [], ParamH [], int);
typedef Real (*ActivFun) (Real);
typedef Real (*ErrorFun) (ParamH [], ParamH [], int);
typedef Real (*SimilFun) (ParamH&, ParamH&);

// Interficie de comunicacio entre el BGA i l_exterior
// Els valors son valors per defecte

struct BGASetup
{
  static const Real rang_ratio = 0.5;
  static const int k_precision = 8;

  static const int each = 1;          // Cada quant es vol info. en pantalla/plot

  int trunc;
  int q;
  Real delta;
  static const Real change_amount = 0.0; // % de sortida real en realim

  int FFevals;
  int NRuns;         // No. de runs amb diferents poblacions

  static const Bool plot     = FALS;  // Si es vol els (.gpt) dels best (BGA)
  static const Bool Mplot    = FALS;  // Si es vol els MSE, NMSE, % en .TR, .TE
  static const Bool tracaTE  = FALS;  // Si es vol la traca cada each del MSE en TE (TR_ONLY)
  static const Bool save     = FALS;  // Si es vol salvar les xarxes (.re)
  static const Bool save_BGA = FALS;  // Si es vol salvar l_estat del BGA
  static const Bool save_out = FALS;  // Si es vol salvar els outs en regressio

  static const Bool verbose      = FALS;
  static const Bool verbose_full = FALS; 

  char path_out[255];                  // Path a on s-escriu
};

#endif 
