
/***********************************************************************
****       Funciones s-barra *******************************************
***********************************************************************/
// OJO: ja no coincideixen els numeros amb la tesi.

typedef Real (*SbarraF)  (Real);

// 1-d
inline Real s0_0 (Real d)
{
  return 1.0-d;
}

// (1-d)^2
inline Real s0_1 (Real d)
{
  Real tmp = 1.0-d;

  return tmp*tmp;
}

// (1-d)^4
inline Real s0_2 (Real d)
{
  Real tmp = 1.0-d;

  tmp *= tmp;
  return tmp*tmp;
}

// 1-sqrt(d)
inline Real s0_3 (Real d)
{
  return 1.0-sqrt(d);
}

// (1-sqrt(d))^2
inline Real s0_4 (Real d)
{
  Real tmp = 1.0-sqrt(d);

  return tmp*tmp;
}

// (1-sqrt(d))^4
inline Real s0_5 (Real d)
{
  Real tmp = 1.0-sqrt(d);

  tmp *= tmp;
  return tmp*tmp;
}

// 1-sqrt4(d)
inline Real s0_6 (Real d)
{
  return 1.0-sqrt(sqrt(d));
}

// (1-sqrt4(d))^2
inline Real s0_7 (Real d)
{
  Real tmp = 1.0-sqrt(sqrt(d));

  return tmp*tmp;
}

// (1-sqrt4(d))^4
inline Real s0_8 (Real d)
{
  Real tmp = 1.0-sqrt(sqrt(d));

  tmp *= tmp;
  return tmp*tmp;
}

// TODA esta familia s1* NO requiere d en [0, 1] (i.e. NONE o STANDARD)
// 1/(1+sqrt4(d))
inline Real s1_0 (Real d)
{
  return 1.0/(1.0+sqrt(sqrt(d)));
}

// 1/(1+sqrt(d))
inline Real s1_1 (Real d)
{
  return 1.0/(1.0+sqrt(d));
}

// 1/(1+d)
inline Real s1_2 (Real d)
{
  return 1.0/(1.0+d);
}

// 1/(1+d^2)
inline Real s1_3 (Real d)
{
  return 1.0/(1.0+d*d);
}

// 1/(1+sqrt(d))^2
inline Real s1_4 (Real d)
{
  Real tmp = 1.0+sqrt(d);
  return 1.0/(tmp*tmp);
}

// TODA esta familia s1* requiere d en [0, 1] (i.e. PROBEN)
// 2/(1+sqrt4(d))-1
inline Real s1_estrella_0 (Real d)
{
  return 2*s1_0(d)-1;
}

// 2/(1+sqrt(d))-1
inline Real s1_estrella_1 (Real d)
{
  return 2*s1_1(d)-1;
}

// 2/(1+d)-1
inline Real s1_estrella_2 (Real d)
{
  return 2*s1_2(d)-1;
}

// 2/(1+d^2)-1
inline Real s1_estrella_3 (Real d)
{
  return 2*s1_3(d)-1;
}

// exp(-6d)
inline Real s2_0 (Real d)
{
  return exp(-6*d);
}

// exp(-6d^2)
inline Real s2_1 (Real d)
{
  return exp(-6*d*d);
}

// exp(-6sqrt(d))
inline Real s2_2 (Real d)
{
  return exp(-6*sqrt(d));
}

// 1.0-sa(d)
inline Real s3_0 (Real d)
{
  return 1.0-LogisticH(1.0*d);
}

// 1.0-sa(d)
inline Real s3_1 (Real d)
{
  return 1.0-LogisticH(1.5*d);
}

// 1.0-sa(d)
inline Real s3_2 (Real d)
{
  return 1.0-LogisticH(2.0*d);
}

// 1.0-sa(d)
inline Real s3_3 (Real d)
{
  return 1.0-LogisticH(2.5*d);
}


// Funciones s_barra para REAL

enum { S0_0, S0_1, S0_2, S0_3, S0_4, S0_5, S0_6, S0_7, S0_8, 
       S1_0, S1_1, S1_2, S1_3, S1_4, 
       S1_ESTR_0, S1_ESTR_1, S1_ESTR_2, S1_ESTR_3, 
       S2_0, S2_1, S2_2, S3_0, S3_1, S3_2, S3_3 };

SbarraF TablaSbarraF[] = 
       { s0_0, s0_1, s0_2, s0_3, s0_4, s0_5, s0_6, s0_7, s0_8,
         s1_0, s1_1, s1_2, s1_3, s1_4,
         s1_estrella_0, s1_estrella_1, s1_estrella_2, s1_estrella_3, 
         s2_0, s2_1, s2_2, s3_0, s3_1, s3_2, s3_3 };

// las elegidas
unsigned SiR = S0_1;   // Alt: S0_4,S1_2,S1_4, for (Si = S0_0; Si<=S0_6; Si++)
unsigned SiI = S1_2;

//****     fin  Funciones s-barra ************************************

// REAL

// A esta le iría bien el preproceso PROBEN
inline Real Euclidean (ParamH& in, ParamH& w)
{
  //  printf("REAL = %g", TablaSbarraF[SiR](fabs(in.real-w.real)));
  return TablaSbarraF[SiR](fabs(in.real-w.real));
}

inline Real Clarke (ParamH& in, ParamH& w)
{
  Real d = fabs(in.real-w.real);
  return (in.real+w.real!=0) ? TablaSbarraF[SiR](d/(in.real+w.real)) : 1.0;
}

// A esta le iría bien el preproceso STANDARD
inline Real Normalized_Euclidean (ParamH& in, ParamH& w)
{
  Real d = fabs(in.real-w.real);
  return TablaSbarraF[SiR](d/(d+1));
}


char* SimilFunREAL_N[]= { "EUCLIDEAN", "CLARKE", "NORMALIZED-EUCLIDEAN", "" };


SimilFun SimilFunREAL[] = 
  {
    Euclidean, Clarke, Normalized_Euclidean 
  };

