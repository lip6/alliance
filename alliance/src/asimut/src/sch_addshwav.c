
/* ###--------------------------------------------------------------### */
/* file		: sch_addshwav.c					*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_addshwav						*/
/* description	: create an identifier entry point to the scheduler	*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct shwav *sch_addshwav (old_wav, ident)

struct shwav *old_wav;
void         *ident  ;

  {
  struct shwav *pt_shwav;
  unsigned int  i       ;

  if (SCH_FREE_SHWAV == NULL)
    {
    SCH_FREE_SHWAV = (struct shwav *)
                     mbkalloc (sizeof (struct shwav) * SCH_DFN_WAVBLOCK);

    pt_shwav = SCH_FREE_SHWAV;
    for (i=0 ; i<SCH_DFN_WAVBLOCK ; i++)
      {
      pt_shwav->NEXT = pt_shwav + 1;
      pt_shwav ++;
      }
    (pt_shwav - 1)->NEXT = NULL;
    }

  pt_shwav       = SCH_FREE_SHWAV;
  SCH_FREE_SHWAV = pt_shwav->NEXT;

  pt_shwav->IDENT = ident  ;
  pt_shwav->NEXT  = old_wav;
  pt_shwav->TRANS = NULL   ;

  return (pt_shwav);
  }
