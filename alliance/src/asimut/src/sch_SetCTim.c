
/* ###--------------------------------------------------------------### */
/* file		: sch_SetCTim.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_SetCurrentTime					*/
/* description	: Set the scheduler's current time.			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void sch_SetCurrentTime (pt_shdul, date)

struct shdul *pt_shdul;
unsigned int  date    ;

  {
  pt_shdul->CURTIME = date;
  }
