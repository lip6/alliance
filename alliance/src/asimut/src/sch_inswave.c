
/* ###--------------------------------------------------------------### */
/* file		: sch_inswave.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_inswave						*/
/* description	: insert a new transaction into the scheduler (main	*/
/*		  identifier access)					*/
/* called func.	: sch_rmventry						*/
/* ###--------------------------------------------------------------### */

void sch_inswave (pt_shdul, pt_shtra, flags)

struct shdul   *pt_shdul;
struct shtra   *pt_shtra;
unsigned short  flags   ;

  {
  struct shtra *pt_afttra;
  struct shtra *pt_deltra;
  struct shtra *pt_lsttra;
  struct shtra *pt_tmptra;
  struct shwav *pt_shwav ;
  unsigned int  index    ;

  index    = sch_hash (pt_shtra->IDENT);
  pt_shwav = pt_shdul->ITABLE [index];
  
  while ((pt_shwav != NULL) && (pt_shwav->IDENT != pt_shtra->IDENT))
    pt_shwav = pt_shwav->NEXT;

  if (pt_shwav == NULL)
    {
    pt_shwav                 = sch_addshwav (pt_shdul->ITABLE [index],
                                             pt_shtra->IDENT          );
    pt_shdul->ITABLE [index] = pt_shwav;
    }

  pt_lsttra = NULL;
  pt_deltra = NULL;
  pt_afttra = pt_shwav->TRANS;
  while ((pt_afttra != NULL) && (pt_afttra->DATE < pt_shtra->DATE))
    {
    pt_lsttra = pt_afttra;
    if ((pt_afttra->COND != pt_shtra->COND) ||
        (pt_afttra->VALU != pt_shtra->VALU)   )
      pt_deltra = pt_afttra;

    pt_afttra = pt_afttra->WAVE;
    }

  if ((flags & SCH_TRA_DELAFTER) != 0)
    {
    while (pt_afttra != NULL)
      pt_afttra = sch_rmventry (pt_afttra);
    }
    
  if (pt_lsttra != NULL)
    pt_lsttra->WAVE = pt_shtra;
  else
    pt_shwav->TRANS = pt_shtra;

  pt_shtra->WAVE = pt_afttra;

  if ((pt_deltra != NULL) && ((flags & SCH_TRA_DELBEFORE) != 0))
    {
    pt_tmptra = pt_shwav->TRANS;
    while (pt_tmptra != pt_deltra)
      pt_tmptra = sch_rmventry (pt_tmptra);

    pt_shwav->TRANS = sch_rmventry (pt_tmptra);
    }

  }
