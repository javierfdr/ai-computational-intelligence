// funneu.h

#ifndef _FUNNEU_
#define _FUNNEU_

#include "defs.h"

// NET INPUT
Real ProdEscalar (ParamH [], ParamH [], int);
Real SimilProdEscalar (ParamH [], ParamH [], int);
Real SimilAngular (ParamH [], ParamH [], int);
Real SimilMinmax (ParamH [], ParamH [], int);
Real RbfEstandar (ParamH [], ParamH [], int);
Real SimilInvqMean (ParamH [], ParamH [], int);
Real SimilHarmMean (ParamH [], ParamH [], int);
Real SimilGeomMean (ParamH [], ParamH [], int);
Real SimilAritMean (ParamH [], ParamH [], int);
Real SimilQuadMean (ParamH [], ParamH [], int);

// ACTIVATIONS
Real Identity (Real);
Real SemiLinear (Real);
Real Gaussiana (Real);
Real Logistic (Real);
Real LogisticAbs (Real);
Real LogisticH (Real);
Real LogisticJulio (Real);
Real LogisticMia (Real);
Real Softmax (Real);

// ERRORS
Real ErrorQuadratic (ParamH [], ParamH [], int);
Real CrossEntropy   (ParamH [], ParamH [], int);
Real CrossEntropy_1 (ParamH [], ParamH [], int);
Real CrossEntropy_C (ParamH [], ParamH [], int);

// NETWORK COMPLEXITY
Real BSSM (int, Real, int);

#endif
