// miscel.lania

#include "misc.h"
#include <stdio.h>
#include <stdlib.h>

#include "ctype.h" // isprint
#include <string.h> // srtcat

Bool IsMissing (char token[])
{
  return ( !strcmp("X", token) || 
           !strcmp("*", token) || 
           !strcmp("?", token) );
}

Bool IsSeparator (int c)
{
  return (c==' ' || c=='\n' || c==',' || c=='\t' || !isprint(c) || c=='\v');
}

Bool IsCommentMark (int c)
{
  return (c==COMMENT_SYMBOL);
}

/* Para estas dos funciones, el token acaba en ENDTOKEN y se supone que ya no hay separadores delante */

Bool IsInteger (char token[])
{
  unsigned i= 0;

  if (token[1]==ENDTOKEN) return isdigit(token[0]);
  if (token[0]=='-' || token[0]=='+') i=1;
  while(isdigit(token[i])) i++;
  return (i>1 && token[i]==ENDTOKEN);
}


// Permite dejar en blanco la parte real o la fraccionaria
Bool IsReal (char token[])
{
  unsigned i= 0;

  if (token[1]==ENDTOKEN) return isdigit(token[0]);
  if (token[0]=='-' || token[0]=='+') i++;
  while(isdigit(token[i])) i++;

  if (token[i]=='.')
  {
    do i++; while(isdigit(token[i]));
    if (token[i]=='E' || token[i]=='e' )
    {
      i++;
      if (token[i]=='-' || token[i]=='+') i++;
      while(isdigit(token[i])) i++;
    }
  }
  return token[i]==ENDTOKEN;
}

ulong dos_elevat_a (ulong n)
{
  const uint i = 2;

  switch (n)
  {
    case 0:  return 1;
    case 1:  return i;
    default: return i << (n-1);
  }
}

// S'assumeix que 'i' cap a 'a'
void itoa (int i, char a[])
{
  int j=0;

  do
  {
    a[j] = i/10+48;
    i /= 10;
  } while (i);
  a[j] = ENDTOKEN;
}

// Pre: n=1 (Per classificacio amb UNA sortida)
Bool IsAHit_1 (ParamH res, ParamH des)
{
  const Real tolerance = 0.4;

  return ( (des.real==1.0 && res.real>=1.0-tolerance) ||
           (des.real==0.0 && res.real<=tolerance) );
}

// Pre: n>=2 (Per classificacio 1-out-of-n)
Bool IsAHit_N (ParamH res[], ParamH des[], int n)
{
  int i, pos=1;
  Real max = res[1].real;

  for (i=2; i<=n; i++)
    if (res[i].real > max) 
    {
      max = res[i].real;
      pos = i;
    }
  return (des[pos].real==1.0 ? CERT: FALS);
}

Bool IsAHit (ParamH res[], ParamH des[], int n)
{
  return (n>1 ? IsAHit_N (res, des, n) : IsAHit_1 (res[1], des[1]));
}

void CheckMem (void* p)
{
  if (!p) 
  {
    printf ("Me falta MEMORIA. Cerrando ...\n");
    exit(0);
  }
}

// Pre: 0<= run < 100
void run_to_name (int run, char* path, char* base, char* extension, char* name)
{
      char caux[4];

      caux[0] = '-'; 
      caux[1]= run/10+48;
      caux[2]= run-10*(run/10)+48;
      caux[3]= name[0]= ENDTOKEN;
      strcat (name, path);
      strcat (name, base);
      strcat (name, caux);
      strcat (name, extension);
}


void warning (char* e, char* m)
{
  printf ("WARNING: %s", e);
  if (m) printf (" (%s)\n", m); else puts("");
  exit(0);
}

void error_and_exit (char* e, char* m)
{
  printf ("ERROR: %s", e);
  if (m) printf (" (%s)\n", m); else puts("");
  exit(0);
}

void error_and_exit (char* e, ulong l1)
{
  printf ("ERROR: %s (%lu)\n", e, l1);
  exit(0);
}

void error_and_exit (char* e, Real r)
{
  printf ("ERROR: %s (%g)\n", e, r);
  exit(0);
}

void error_and_exit (char* e, ulong l1, ulong l2)
{
  printf ("ERROR: %s (%lu,%lu)\n", e, l1, l2);
  exit(0);
}
