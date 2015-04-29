// tabla

#ifndef _TABLA_
#define _TABLA_

#include "defs.h"
#include "config.h"

int Nom2Index (char* names[], char* n);
Bool HiEs (char* names[], char* n);
char* Enum2Nom (char* names[], int n);
char* NetIFun2Nom (char* names[], InputFun Tabla_C[], InputFun f);
char* ActivFun2Nom (char* names[], ActivFun Tabla_C[], ActivFun f);

#endif
