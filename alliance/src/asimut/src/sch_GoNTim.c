
/* ###--------------------------------------------------------------### */
/* file		: sch_Goto.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_GotoNextTime					*/
/* description	: set the current absolute time to the date of the	*/
/*		  first transaction and return it as an argument. If	*/
/*		  the scheduler is empty the function returns the value	*/
/*		  0. The functions also	releases empty time-entries in	*/
/*		  the scheduler						*/
/* called func.	: sch_GetNextTime					*/
/* ###--------------------------------------------------------------### */

int sch_GotoNextTime (pt_shdul)

struct shdul *pt_shdul;

  {
  int          found  = 0;
  unsigned int n_date ;

  found = sch_GetNextTime (pt_shdul, &n_date);
  if (found  != 0)
    pt_shdul->CURTIME = n_date;

  return (found);
  }
