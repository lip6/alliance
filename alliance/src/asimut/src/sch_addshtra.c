
/* ###--------------------------------------------------------------### */
/* file		: sch_addshtra.c					*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_addshtra						*/
/* description	: create a transaction and initilize its field		*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct shtra *sch_addshtra (ident, scdid, date, cond, valu, uflag, flags)

void           *ident;
void           *scdid;
unsigned int    date ;
unsigned char   cond ;
unsigned char   valu ;
unsigned int    uflag;
unsigned short  flags;

  {
  struct shtra *pt_shtra;
  unsigned int  i       ;

  if (SCH_FREE_SHTRA == NULL)
    {
    SCH_FREE_SHTRA = (struct shtra *)
                     mbkalloc (sizeof (struct shtra) * SCH_DFN_TRABLOCK);

    pt_shtra = SCH_FREE_SHTRA;
    for (i=0 ; i<SCH_DFN_TRABLOCK ; i++)
      {
      pt_shtra->NEXT = pt_shtra + 1;
      pt_shtra ++;
      }
    (pt_shtra - 1)->NEXT = NULL;
    }

  pt_shtra        = SCH_FREE_SHTRA;
  SCH_FREE_SHTRA  = pt_shtra->NEXT;

  pt_shtra->DATE  = date ;
  pt_shtra->IDENT = ident;
  pt_shtra->SCDID = scdid;
  pt_shtra->UFLAG = uflag;
  pt_shtra->COND  = cond ;
  pt_shtra->VALU  = valu ;
  pt_shtra->FLAGS = flags;

  return (pt_shtra);
  }
