
/* ###--------------------------------------------------------------### */
/* file		: sch_rmventry.c					*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_rmventry						*/
/* description	: remove a transaction from the time entry of the	*/
/*		  scheduler						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

struct shtra *sch_rmventry (pt_shtra)

struct shtra *pt_shtra;

  {
  struct shent *pt_shent   ;
  struct shtra *pt_nxttra  ;
  unsigned int  wind_offset;

  pt_nxttra = pt_shtra->WAVE;

  if ((pt_shtra->FLAGS & SCH_TRA_ENTFIRST) != 0)
    {
    pt_shent = (struct shent *) pt_shtra->PREV;

    wind_offset = pt_shtra->DATE & 0x00000007;
    pt_shent->TRANS [wind_offset] = pt_shtra->NEXT;

    if (pt_shtra->NEXT != NULL)
      {
      pt_shtra->NEXT->FLAGS |= SCH_TRA_ENTFIRST         ;
      pt_shtra->NEXT->PREV   = (struct shtra *) pt_shent;
      }
    }
  else
    {
    pt_shtra->PREV->NEXT = pt_shtra->NEXT;

    if (pt_shtra->NEXT != NULL)
      pt_shtra->NEXT->PREV = pt_shtra->PREV;
    }

  pt_shtra->NEXT = SCH_FREE_SHTRA;
  SCH_FREE_SHTRA = pt_shtra      ;

  return (pt_nxttra);
  }
