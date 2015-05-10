//CONFIG.H

#ifndef _CONFIGURATION_
#define _CONFIGURATION_

// Para el mecanismo de delays: la salida real de la red es la que se realimenta
// OJO: las columnas de datos han de ser las que deberian en caso de no mecanismo
// e.g.: si se quiere realimentar con *3* delays, las columnas de -3, -2 y -1 han de existir en el .in (o sea, se han de declarar esas *3* entradas mas). HA DE COINCIDIR!!!
// OJO: se asume *una* salida REAL

/* >0 (mecanismo con delay) 
   =0 (no mecanismo) 
   <0 (primer change_amount% no mecanismo y el resto mecanismo con |delay|) */

#define REALIM_DELAY 0

// extensiones de los ficheros usados

// Para la definicion de la red 
#define EXT_NET ".net"
// Para la red generada completa
#define EXT_RED ".re"
// Para los datos de entrada
#define EXT_DATA ".in"
// Para los gnuplot del BGA
#define EXT_GNU ".gpt"
// Para los plots en regresion
#define EXT_OUT ".gpt"


// lectura/escritura de fichero

#define FORMAT_PANTALLA "%5.3f "
#define FORMAT_FITXER "%5.6f "

#define MAXLINE 255
#define ENDTOKEN '\0'  // Esto no se debe cambiar, pues es la convencion C/C++ para fin de cadenas.

#define MISSING_SYMBOL "X"
#define COMMENT_SYMBOL ';'

enum {SILENT, FAIR, VERBOSE};
#define IO_MODE FAIR
enum {SAFE, NOSAFE};
#define EXEC_MODE 0

#endif

