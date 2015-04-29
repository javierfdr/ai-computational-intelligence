// tabla

#include "tabla.h"
#include <string.h> // srtcat

// Pre: HiEs (names,n)
int Nom2Index (char* names[], char* n)
{
  int i=0;

  while (strcmp(n, names[i])) i++;
  return i;
}

Bool HiEs (char* names[], char* n)
{
  int i;

  for (i=0; *names[i]; i++)
    if (!strcmp(n, names[i]))
      return CERT;
  return FALS;
}

// Pre: HiEs (names,n)
char* Enum2Nom (char* names[], int n)
{
  return names[n];
}

// Pre: HiEs (Tabla_C, f)
char* NetIFun2Nom (char* names[], InputFun Tabla_C[], InputFun f)
{
  int i=0;

  while (f!=Tabla_C[i]) i++;
  return Enum2Nom (names, i);
}

// Pre: HiEs (Tabla_C, f)
char* ActivFun2Nom (char* names[], ActivFun Tabla_C[], ActivFun f)
{
  int i=0;

  while (f!=Tabla_C[i]) i++;
  return Enum2Nom (names, i);
}
