
/* ###--------------------------------------------------------------### */
/* file		: sch_GetCTra.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_GetCurrentTransactions				*/
/* description	: return all transactions at current date and remove	*/
/*		  them from the scheduler				*/
/* called func.	: sch_bug						*/
/* ###--------------------------------------------------------------### */

struct shtra *sch_GetCurrentTransactions (pt_shdul)

struct shdul *pt_shdul;

  {
  struct shtra  *pt_shtra    = NULL;
  struct shwav  *pt_shwav    = NULL;
  struct shent  *pt_shent    = NULL;
  unsigned int   time_window ;
  unsigned int   wind_offset ;
  unsigned int   wind_number ;
  void          *ident       ;
  unsigned int   index       ;

  time_window = (pt_shdul->CURTIME >>  3) & 0x000000ff;
  wind_number = (pt_shdul->CURTIME >> 11) & 0x001fffff;
  wind_offset = (pt_shdul->CURTIME      ) & 0x00000007;

  pt_shent = pt_shdul->TTABLE [time_window];


  if ((pt_shent == NULL) || (pt_shent->DATE != wind_number))
    {
    sch_bug (1, "sch_GetCurrentTranscations", NULL);
    }
  else
    {
    pt_shtra = pt_shent->TRANS [wind_offset];
    if (pt_shtra == NULL)
      {
  printf ("sch_bug amar 2 : %x\n", pt_shtra);
      sch_bug (1, "sch_GetCurrentTranscations", NULL);
      }
    else
      {
      while (pt_shtra != NULL)
        {
        ident    = pt_shtra->IDENT;
        index    = sch_hash (ident);
        pt_shwav = pt_shdul->ITABLE [index];

        while (pt_shwav->IDENT != ident)
          pt_shwav = pt_shwav->NEXT;

        if ((pt_shwav == NULL) || (pt_shwav->TRANS != pt_shtra))
{
printf ("wav : %x, tra : %x\n", pt_shwav->TRANS, pt_shtra);
sch_debug (pt_shtra, "shtra");
sch_debug (pt_shwav, "shwav");
          sch_bug (2, "sch_GetCurrentTranscations", NULL);
}
        else
          pt_shwav->TRANS = pt_shtra->WAVE;

        pt_shtra = pt_shtra->NEXT;
        }

      pt_shtra                      = pt_shent->TRANS [wind_offset];
      pt_shent->TRANS [wind_offset] = NULL;
      }
    }

  return (pt_shtra);
  }
