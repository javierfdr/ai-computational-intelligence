// Entrada_Salida

#include <stdio.h>
#include <ctype.h>  //isprint, toupper, ...
#include <string.h> //strlen
#include <stdlib.h> //atoi
#include <limits.h> //INT_MAX

#include "entsal.h"
#include "misc.h"

/*
int ReadName (char *mens, PathStrF name)
{
  printf("\n%s file: ", mens);
  name[0]= ENDTOKEN;
  return ReadString (name, NAME_F);
}

FILE *ReroutF (PathStrF name)
{
  FILE *inf;
    
  inf= freopen(name, "r", stdin);
  return inf;
}
*/

Bool ExistF (PathStrF name)
{
  FILE *f;
  Bool hay=FALS;

  f= fopen(name, "r");
  if (f)
  {
    fclose(f);
    hay= CERT;
  }
  return hay;
}

//Pasa caracteres sin interes y devuelve el motivo de parada
int SkipSeparatorsF (FILE *f, ulong& line)
{
  int c;

    do 
    {
       c= fgetc(f); 
       if (c==ENDTOKEN) line++;
    }
    while (c!=EOF && IsSeparator(c));
    return c;
}

//Pasa comentarios y devuelve el motivo de parada
int SkipCommentsF (FILE *f, ulong& line)
{
  int c;

  do
  {
    c= fgetc(f);
    // Pasamos comentarios hasta el final de linea
    if (IsCommentMark(c))
      do c= fgetc(f);  while (c!=EOF && c!='\n');
    if (c=='\n') line++;
  }
  while (c!=EOF && (!isprint(c) || IsSeparator(c)));

  return c;
}

// Hay que reformarla un poquito solo*********
// Devuelve FALS si todo ha ido bien (no hay error)
// Devuelve CERT si no se ha podido leer el token (no habia mas)
Bool ReadTokenF (FILE *f, char token[], ulong& line)
{ 
  uint i= 0;
  int c;

  c= SkipCommentsF (f, line);
  //Leemos el token que nos interesa
  while (c!=EOF && isprint(c) && !IsSeparator(c) && i<MAXLINE-1)
  {
    token[i]= toupper(c);  // no la toca si no es una letra
    c= fgetc(f);
    i++;
  }
    //el caracter que ha hecho de separador lo dejamos(e.g. si fuese ';' puede
    //significar que lo de detras es un comentario)
  if (c!=EOF) ungetc(c, f);
  token[i]= ENDTOKEN;
  return (i>0 ? FALS : CERT);
}


Bool ReadIntF (FILE *f, int* i, ulong& line, int min, int max)
{
  char token[MAXLINE];
  Bool error;

  error= ReadTokenF(f, token, line);
  if (!error)
    if (IsInteger(token))
    {    
      *i= atoi(token);
      if (*i<min || *i>max) 
      {
        error=CERT; 
        printf ("Entero (%d) fuera de rango en linea: %lu\n", *i, line);
      }
    }
    else
    {
      error=CERT; 
      printf("Esperando un entero en la linea: %lu\n", line);
    } 
  return !error;
}

// Estas no se si debieran estar aqui
// Y ademas no hay bias para los vectores In/Out
// Y están duplicadas: quizás usar la ReroutF

void printMissing (void)
{
  printf (" %s ", MISSING_SYMBOL);
}

void printParamH (ParamH MiOut[], uint n, TipoNeurona tn, TiposH* t)
{
  uint i;

  switch (tn)
  { 
   case NORMAL: 
          printf(FORMAT_PANTALLA, MiOut[0].real); // el bias
          for (i=1; i<=n; i++)
            printf(FORMAT_PANTALLA, MiOut[i].real);
          break;
   case HETEROGENEA: 
          printf(FORMAT_PANTALLA, MiOut[0].real); // el bias
          for (i=1; i<=n; i++)
	    if (IsMissing(MiOut[i])) printMissing ();
	    else switch (t[i])
	      {
	      case REAL:  
		printf(FORMAT_PANTALLA, MiOut[i].real);
		break;
	      case FUZZY:   
		printf(FORMAT_PANTALLA, MiOut[i].fn.mode);
		printf(FORMAT_PANTALLA, MiOut[i].fn.spread);
		break;
	      case VARLING: 
		printf(FORMAT_PANTALLA, MiOut[i].fq.mode1);
		printf(FORMAT_PANTALLA, MiOut[i].fq.spread1);
		printf(FORMAT_PANTALLA, MiOut[i].fq.mode2);
		printf(FORMAT_PANTALLA, MiOut[i].fq.spread2);
		break;
	      case INTEGER:
	      case NOMINAL:
	      case BINARY:  
	      case ORDINAL: 
		printf("%d ", MiOut[i].disc);
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
          break;
   case FULLFUZZY: break;
  }
  printf("\n");
}

void fprintMissing (FILE* f)
{
  fprintf (f, "  %s", MISSING_SYMBOL);
}

void fprintParamH (FILE* f, ParamH MiOut[], uint n, TipoNeurona tn, TiposH* t)
{
  uint i;

  switch (tn)
  { 
   case NORMAL: 
          fprintf(f, FORMAT_FITXER, MiOut[0].real); // el bias
          for (i=1; i<=n; i++)
            fprintf(f, FORMAT_FITXER, MiOut[i].real);
          break;
   case HETEROGENEA: 
          fprintf(f, FORMAT_FITXER, MiOut[0].real); // el bias
          for (i=1; i<=n; i++)
	    if (IsMissing(MiOut[i])) fprintMissing (f);
	    else switch (t[i])
	      {
	      case REAL:  
		fprintf(f, FORMAT_FITXER, MiOut[i].real);
		break;
	      case FUZZY:   
		fprintf(f, FORMAT_FITXER, MiOut[i].fn.mode);
		fprintf(f, FORMAT_FITXER, MiOut[i].fn.spread);
		break;
	      case VARLING: 
		fprintf(f, FORMAT_FITXER, MiOut[i].fq.mode1);
		fprintf(f, FORMAT_FITXER, MiOut[i].fq.spread1);
		fprintf(f, FORMAT_FITXER, MiOut[i].fq.mode2);
		fprintf(f, FORMAT_FITXER, MiOut[i].fq.spread2);
		break;
	      case INTEGER:
	      case NOMINAL:
	      case BINARY:  
	      case ORDINAL: 
		fprintf(f, "%d ", MiOut[i].disc);
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
          break;
   case FULLFUZZY: break;
  }
  fprintf(f, "\n");
}

// OJO con esta function (el (n+1)) no lo veo claro
void CopyParamH (ParamH* destin, ParamH* source, uint n)
{
  memcpy (destin, source, sizeof(ParamH)*(n+1));  // el +1 es por el bias[0]
}

