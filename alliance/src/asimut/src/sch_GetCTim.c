
/* ###--------------------------------------------------------------### */
/* file		: sch_GetCTim.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_GetCurrentTime					*/
/* description	: returns the scheduler's current time			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

unsigned int sch_GetCurrentTime (pt_shdul)

struct shdul *pt_shdul;

  {
  return (pt_shdul->CURTIME);
  }
