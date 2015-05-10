// miscel.lania

#ifndef _MISC_
#define _MISC_

#include "config.h"
#include "defs.h"

Bool  IsMissing (char token[MAXLINE]);
Bool  IsSeparator (int c);
Bool  IsCommentMark (int c);
Bool  IsReal (char token[MAXLINE]);
Bool  IsInteger (char token[MAXLINE]);
ulong dos_elevat_a (ulong);
Bool IsAHit (ParamH res[], ParamH des[], int n);
void CheckMem (void*);
void run_to_name (int run, char* path, char* base, char* extension, char* name);
void warning (char* e, char* m=ENDTOKEN);
void error_and_exit (char* e, char* m=ENDTOKEN);
void error_and_exit (char* e, ulong l1);
void error_and_exit (char* e, Real r);
void error_and_exit (char* e, ulong l1, ulong l2);


#endif


