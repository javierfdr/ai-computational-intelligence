/* Basic Random number generation and manipulation */

#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdlib.h>  // drand48 et al.
#include <time.h>    // clock_t, clock()
#include <stdio.h>   // printf
#include "defs.h"
#include <limits.h>  // ULONG_MAX

void ini_random (void);
void ini_random_c (void);
Real random_01 (void);
Real random_minMax (Real, Real);
long random_long (void);
ulong random_ulong (void);
int flip (Real prob);
Real random_1_flip (void);
Real random_1 (void);
uint randint (uint low, uint high);
int randbyte (void);
int random_sign (void);
Real Normal (Real, Real);
Real TrianUnimodal (Real s, Real m);
Real TrianBimodal (Real m1, Real m2);

#endif
