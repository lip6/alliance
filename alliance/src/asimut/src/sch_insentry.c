
/* ###--------------------------------------------------------------### */
/* file		: sch_insentry.c					*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_insentry						*/
/* description	: insert a new transaction into the scheduler (time	*/
/*		  access)						*/
/* called func.	: sch_addshent						*/
/* ###--------------------------------------------------------------### */

void sch_insentry (pt_shdul, pt_shtra)

struct shdul *pt_shdul;
struct shtra *pt_shtra;

  {
  struct shent *pt_shent   ;
  struct shent *pt_prvent  ;
  unsigned int  time_window;
  unsigned int  wind_number;
  unsigned int  wind_offset;

  time_window = (pt_shtra->DATE >>  3) & 0x000000ff;
  wind_number = (pt_shtra->DATE >> 11) & 0x001fffff;
  wind_offset = (pt_shtra->DATE      ) & 0x00000007;

  pt_shent = pt_shdul->TTABLE [time_window];

  if ((pt_shent == NULL) || (pt_shent->DATE > wind_number))
    {
    pt_shdul->TTABLE [time_window] = sch_addshent (pt_shent, wind_number);
    pt_shent                       = pt_shdul->TTABLE [time_window];
    }
  else
    {
    pt_prvent = pt_shent;

    while ((pt_shent != NULL) && (pt_shent->DATE < wind_number))
      {
      pt_prvent = pt_shent      ;
      pt_shent  = pt_shent->NEXT;
      }

    if ((pt_shent == NULL) || (pt_shent->DATE > wind_number))
      {
      pt_shent        = sch_addshent (pt_shent, wind_number);
      pt_prvent->NEXT = pt_shent;
      }
    }
  
  pt_shtra->FLAGS |= SCH_TRA_ENTFIRST;
  pt_shtra->PREV   = (struct shtra *) pt_shent;

  pt_shtra->NEXT   = pt_shent->TRANS [wind_offset] ;

  if (pt_shtra->NEXT != NULL)
    {
    pt_shtra->NEXT->FLAGS ^= SCH_TRA_ENTFIRST ;
    pt_shtra->NEXT->PREV   = (void *) pt_shtra;
    }

  pt_shent->TRANS [wind_offset] = pt_shtra;
  }
