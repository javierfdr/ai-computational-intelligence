// Funcions de similaritat entre valors fuzzy (FUZZY, VARLING)


// FUZZY

#define EPSILON 10e-8
Real compara_FUZZY (ParamH& l, ParamH& r)
{
 int talls;
 Real e1,m1,e2,m2,res,aux;
 Real x[4],y[4]; /* com a molt hi han dos talls */

 talls=0;

 e1= l.fn.spread;
 m1= l.fn.mode;

 e2= r.fn.spread;
 m2= r.fn.mode;

 if (e1<0 || e2<0) return 0.0;  // printf("Mal formats\n");
 if (m2+e2<m1-e1)  return 0.0;  // printf("No intersec left.\n"); 
 if (m2-e2>m1+e1)  return 0.0;  // printf("No intersec right.\n"); 
 
 if (m1-m2<EPSILON) return 1.0; // printf("Son iguals\n");
         
 // Si arribem aqui, es que hi ha interseccio

             aux = (m2-m1)/(e1-e2);
             if (!( (m1-e1<m2-e2 && m1<m2) || (m1-e1>m2-e2 && m1>m2) ))
             {
               x[talls]= m1+aux*e1;
               y[talls]= aux+1.0;
               talls++;
             }

             if (!( (m1+e1<m2+e2 && m1<m2) || (m1+e1>m2+e2 && m1>m2) ))
             {
               x[talls]= m1+aux*e1;
               y[talls]= 1.0-aux;
               talls++;
             }

             aux = (m2-m1)/(e1+e2);
             if (!( (m1-e1<m2+e2 && m1<m2) || (m1-e1>m2+e2 && m1>m2) ))
             {
               if (talls>=2) printf("Comparar fuzzy: Something is wrong!!!!\n");
               x[talls]= m1+aux*e1;
               y[talls]= aux+1.0;
               talls++;
             }

             if (!( (m1+e1<m2-e2 && m1<m2) || (m1+e1>m2-e2 && m1>m2) ))
             {
               if (talls>=2) printf("Comparar fuzzy: Something is wrong!!!!\n");
               x[talls]= m1+aux*e1;
               y[talls]= 1.0-aux;
               talls++;
             }

 if (talls>2) 
 {
   printf("Comparar fuzzy: pues Something is wrong!!!!\n");
   printf("Talls: %d, e1=%f,m1=%f,e2=%f,m2=%f\n",talls,e1,m1,e2,m2);
   return 0.0;
 }

 //      printf("x[%d]= %f, y[%d]= %f\n",i,x[i],i,y[i]);
 // Si hem arribat aqui es que 1<=talls<=2 (es a dir, tot OK)
  
 res= y[0];                          // 1er tall, i si existeix un 
 if (talls>1 && y[1]>res) res= y[1]; // 2on tall, em quedo amb el mes gran
 return res;
}


// VARLING

// TRAPEZI: e1, e2 son offsets
Real mu (Real e1, Real m1, Real m2, Real e2, Real x)
{
  if ( m1 <= x && x <= m2 ) return 1.0;
  if ( x <= m1-e1 || x >= m2+e2 ) return 0.0;
  if ( m1-e1 <= x && x <= m1 ) return 1.0+(x-m1)/e1;
  return 1.0+(m2-x)/e2;
}


#define SAMPLE_POINTS 50

// Se asume que no hay numeros negativos
// El min es siempre 1.0 y el max es new_rang[j].max (hay que pasárselo)
Real compara_VARLING (ParamH& l, ParamH& r)
{
  Real le1, le2, lm1, lm2, re1, re2, rm1, rm2;
  int max = 10; // hauria de ser el ordinal original més gran

  le1= l.fq.spread1;
  le2= l.fq.spread2;
  lm1= l.fq.mode1;
  lm2= l.fq.mode2;

  re1= r.fq.spread1;
  re2= r.fq.spread2;
  rm1= r.fq.mode1;
  rm2= r.fq.mode2;

  if (lm1>lm2 || rm1>rm2) return 0.0; //printf("Mal formats\n");
  if (lm1-le1<1.0-DX || lm2+le2>max+DX) return 0.0; //printf("Mal formats\n");
  if (rm1-re1<1.0-DX || rm2+re2>max+DX) return 0.0; //printf("Mal formats\n");
 
  // Si arribem aqui, es que estan ben formats

  Real pleft =  MIN (lm1-le1, rm1-re1);
  Real pright = MAX (lm2+le2, rm2+re2);

  Real muL, muR, su, sa, step, sample;

  su = sa = 0.0; 
  step = (pright-pleft)/(SAMPLE_POINTS+1); // Asi coge SAMPLE_POINTS de dentro

  for (sample=pleft+step; sample<pright; sample+=step)
  {
    muL = mu (le1, lm1, lm2, le2, sample);
    muR = mu (re1, rm1, rm2, re2, sample);    

    //    printf ("(%g) muL = %g; muR = %g\n", sample, muL, muR);

    if (muL<=muR)
    {
      sa += muL;
      su += muR;
    }
    else
    {
      sa += muR;
      su += muL;
    }
  }
  // printf ("sa = %g; su = %g (%g)\n", sa, su, sa/su);
  return sa/su;
}
