
/* ###--------------------------------------------------------------### */
/* file		: sch_Free.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_FreeTranscations					*/
/* description	: release a list of transactions extracted from the	*/
/*		  scheduler						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void sch_FreeTransactions (pt_shtra)

struct shtra   *pt_shtra;

  {
  struct shtra *pt_last;

  pt_last = pt_shtra;

  while (pt_last->NEXT != NULL)
    pt_last = pt_last->NEXT;

  pt_last->NEXT  = SCH_FREE_SHTRA;
  SCH_FREE_SHTRA = pt_shtra      ;
  }
