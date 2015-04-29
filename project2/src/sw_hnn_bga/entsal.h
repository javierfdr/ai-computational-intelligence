// Entrada/Salida

#ifndef _ENTSAL_
#define _ENTSAL_

#include <stdio.h>
#include <limits.h> //INT_MAX

#include "config.h"
#include "defs.h"

Bool ReadIntF (FILE *f, int* i, ulong& line, int min, int max=INT_MAX);

//Longitud maxima del nombre del fichero dado
#define NAME_F 80

typedef char PathStrF[NAME_F];

//Lee un nombre de fichero, mostrando el mensaje 'mens' para la entrada
//int   ReadNameF(char *mens, PathStrF name);

//Asigna como entrada standard el fichero
FILE* ReroutF (PathStrF name);

//Dice si un fichero existe
Bool ExistF (PathStrF name);

/*
  Lee como maximo MAXLINE_C caracteres por token (contando el '\0' que aniade)
  Pasa todas las letras a mayusculas
  Un token empieza cuando tenemos el primer caracter que no sea un separador 
  (pasamos toda la basura que haya antes) y acaba cuando llega un separador.
  Retorna el numero de lineas que ha pasado hasta leer el token
     -1: si no lo ha podido leer nada
   !!debe haber menos de MAX_INT lineas
*/
Bool ReadTokenF (FILE *f, char token[], ulong &line);

//Lee un cardinal pasando separadores iniciales
//Pre: El cardinal a leer debe ser menor que 9999
//errorcode ReadCardF (FILE *f, uint& Card, ulong& line);

/*
//Lee una cadena de maxLon>0 caracteres
char ReadStrBinF(FILE *f, char s[], unsigned maxLon);
*/

int SkipCommentsF (FILE *f, ulong& line);
int SkipSeparatorsF (FILE *f, ulong& line);

void printParamH (ParamH MiOut[], uint n, TipoNeurona tn, TiposH* t);
void fprintParamH (FILE* f, ParamH MiOut[], uint n, TipoNeurona tn, TiposH* t);
void CopyParamH (ParamH* destin, ParamH* source, uint n);

#endif

