
/* ###--------------------------------------------------------------### */
/* file		: sch_AddTra.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_AddTranscation					*/
/* description	: insert a new transaction into the scheduler		*/
/* called func.	: sch_addshent, sch_addshtra				*/
/* ###--------------------------------------------------------------### */

void sch_AddTransaction (pt_shdul, ident, scdid, date, cond, valu, uflag, flags)

struct shdul   *pt_shdul;
void           *ident   ;
void           *scdid   ;
unsigned int    date    ;
unsigned char   cond    ;
unsigned char   valu    ;
unsigned int    uflag   ;
unsigned short  flags   ;

  {
  struct shtra  *pt_shtra;

  date     += pt_shdul->CURTIME;
  pt_shtra  = sch_addshtra (ident, scdid, date, cond, valu, uflag, flags);

  sch_insentry (pt_shdul, pt_shtra       );
  sch_inswave  (pt_shdul, pt_shtra, flags);
  }
