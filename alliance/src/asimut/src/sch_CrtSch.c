
/* ###--------------------------------------------------------------### */
/* file		: sch_CrtSch.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_CreatScheduler					*/
/* description	: create a new scheduler and initialize its time table 	*/
/*		  and identifier table					*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct shdul *sch_CreateScheduler (old_shdul, label)

struct shdul *old_shdul;
void         *label    ;

  {
  unsigned int   i       ;
  struct shdul  *pt_shdul;
  struct shent **pt_time ;
  struct shwav **pt_ident;

  
  pt_shdul = (struct shdul  *) mbkalloc (sizeof (struct shdul  )      );
  pt_time  = (struct shent **) mbkalloc (sizeof (struct shent *) * 256);
  pt_ident = (struct shwav **) mbkalloc (sizeof (struct shwav *) * 256);

  for (i=0 ; i<256 ; i++)
    pt_time [i]  = NULL;

  for (i=0 ; i<256 ; i++)
    pt_ident [i] = NULL;

  pt_shdul->LABEL   = label    ;
  pt_shdul->TTABLE  = pt_time  ;
  pt_shdul->ITABLE  = pt_ident ;
  pt_shdul->CURTIME = 0        ;
  pt_shdul->NEXT    = old_shdul;

  return (pt_shdul);
  }
