
/* ###--------------------------------------------------------------### */
/* file		: sch_addshent.c					*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_addshent						*/
/* description	: create a time entry point to the scheduler		*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct shent *sch_addshent (old_ent, date)

struct shent   *old_ent;
unsigned int    date   ;

  {
  struct shent *pt_shent;
  unsigned int  i       ;

  if (SCH_FREE_SHENT == NULL)
    {
    SCH_FREE_SHENT = (struct shent *)
                     mbkalloc (sizeof (struct shent) * SCH_DFN_ENTBLOCK);

    pt_shent = SCH_FREE_SHENT;
    for (i=0 ; i<SCH_DFN_ENTBLOCK ; i++)
      {
      pt_shent->NEXT = pt_shent + 1;
      pt_shent ++;
      }
    (pt_shent - 1)->NEXT = NULL;
    }

  pt_shent       = SCH_FREE_SHENT;
  SCH_FREE_SHENT = pt_shent->NEXT;

  for (i=0 ; i<8 ; i++)
    pt_shent->TRANS [i] = NULL;

  pt_shent->DATE  = date   ;
  pt_shent->NEXT  = old_ent;

  return (pt_shent);
  }
