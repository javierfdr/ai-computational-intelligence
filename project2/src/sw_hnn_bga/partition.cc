// Partition Magic

#include "partition.h"

enum STATE_MODE { VOIDS, T1, T2, T3, V, E };    // TR, VA, TE

// OJO: debiera ser 1..K_FOLD; ahora K_FOLD<=5
STATE_MODE state[] = {VOIDS, T1, T2, T3, V, E}; // particio inicial

// Partition limits
struct
{
  int left, right;
} part_limits[11]; // OJO: debiera ser 1..K_FOLD; ahora K_FOLD<=10

void ShowPartition (void)
{
  int k;

  printf("********************************");
  for (k=1; k<=K_FOLD; k++)
    switch(state[k])
    {
      case T1: printf ("T1 "); break;
      case T2: printf ("T2 "); break;
      case T3: printf ("T3 "); break;
      case V: printf ("V "); break;
      case E: printf ("E "); break;
      default: break;
    }
  printf("********************************");
}

/* K*(K-1) = no. de permutacions (as Ripley)
   Fet per K_FOLD=5 *ONLY* 

     T1 T2 T3  V  E
     T1 T2 V   E  T3
     T1 T3 V   T2 E
     V  E  T1  T2 T3
     V  E  T2  T3 T1
     T2 T3 V   E  T1
     T2 T3 E   T1 V
     T1 T3 E   V  T2
     T1 T2 E   T3 V
     T3 V  E   T1 T2
   -------------------
     6  6  6   2  2   veces cada uno OK

   PARTIMOS de T1 T2 T3  V  E; de dos en dos swaps se va obteniendo la lista
   precedente */

int SwapList[][2] = { {3,4}, {4,5}, {2,5}, {4,5}, {2,5}, {1,3}, {3,5}, {3,4}, {2,4}, {1,3}, {3,4}, {4,5}, {1,5}, {1,4}, {2,5}, {4,5}, {1,4}, {2,5}, {0,0}, {0,0} };

void SwapPartition (Data* d, int i, int j)
{
  if (i==0 || j==0) return;

  // Swap state
  STATE_MODE s = state[i];
  state[i] = state[j];
  state[j] = s;

  // Swap Data
    
  d->DataSwap (part_limits[i].left, part_limits[j].left, d->NDades()/K_FOLD);
}

void IniPartition (Data* d)
{
  int i, j, q, r;  
  
  q = d->NDades()/K_FOLD;
  r = d->NDades()%K_FOLD;

  // Assignacio base
  for (i=1; i<=K_FOLD; i++)
  {
    part_limits[i].left  = q*(i-1)+1;
    part_limits[i].right = q*i;
  }

  // Assignacio de les restes modul K_FOLD al TR (pel principi)
  // Aixo fa que VA i TE siguin sempre de la mateixa mida
  // de fet, tots els folds ho son ara (els K_FOLD-2 de TR, i els de VA,TE)
  for (j=1; j<=K_FOLD; j++)
  {
    part_limits[j].left  += r;
    part_limits[j].right += r;
  }

  // Actualitzar limits TR/VA/TE (ja es queden fixes)

  d->SetTRleft  (1); d->SetTRright (part_limits[K_FOLD-2].right); 
  d->SetVAleft  (part_limits[K_FOLD-1].left);
  d->SetVAright (part_limits[K_FOLD-1].right);
  d->SetTEleft  (part_limits[K_FOLD].left);
  d->SetTEright (part_limits[K_FOLD].right);

  printf("TRleft = %d, TRright = %d (size= %d)\n", d->TRleft(), d->TRright(), d->HowManyTR());
  printf("VAleft = %d, VAright = %d (size= %d)\n", d->VAleft(), d->VAright(), d->HowManyVA());
  printf("TEleft = %d, TEright = %d (size= %d)\n", d->TEleft(), d->TEright(), d->HowManyTE());
}

// void dec_modulus (int& l)
// {
//   if (l==1) l=K_FOLD; else l--;
// }
