
/* ###--------------------------------------------------------------### */
/* file		: sch_hash.c						*/
/* date		: Aug 10 1997						*/
/* version	: v101							*/
/* authors	: Nizar ABDALLAH, Pirouz BAZARGAN SABET			*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>

/* ###--------------------------------------------------------------### */
/* function	: sch_hash						*/
/* description	: return a hash code					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

unsigned int sch_hash (ident)

void *ident;

  {
  unsigned int index;

  index  = (unsigned int) ident;
  index  = (((index >> 2) ^ (index >> 10)) + (index >> 12) + (index >> 20));
  index &= index & 0x000000ff;

  return (index);
  }
