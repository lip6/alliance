
/* ###--------------------------------------------------------------### */
/* file		: sch_GetNTim.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

/* ###--------------------------------------------------------------### */
/* function	: sch_GetNextTime					*/
/* description	: return the date of the first transaction as an	*/
/*		  argument. If the scheduler is empty the function	*/
/*		  returns the value 0. The functions also releases	*/
/*/		  empty time-entries in the scheduler			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

int sch_GetNextTime (pt_shdul, pt_date)

struct shdul   *pt_shdul;
unsigned int   *pt_date ;

  {
  struct shent  *pt_shent    ;
  struct shent  *pt_nxtent   ;
  unsigned int   min_window  = 0;
  unsigned int   min_offset  = 0;
  unsigned int   min_number  = 0;
  unsigned int   time_window ;
  unsigned int   wind_number ;
  unsigned int   i           ;
  unsigned int   j           ;
  unsigned int   index       ;
  int            glob_found  = 0;

  time_window = (pt_shdul->CURTIME >>  3) & 0x000000ff;
  wind_number = (pt_shdul->CURTIME >> 11) & 0x001fffff;

  for (i=0 ; i<256 ; i++)
    {
    index    = (i + time_window) % 256;
    pt_shent = pt_shdul->TTABLE [index];
    while (pt_shent != NULL)
      {
      for (j=0 ; j<8 ; j++)
        {
        if (pt_shent->TRANS [j] != NULL)
          {
          if ( (glob_found     == 0         ) ||
               (pt_shent->DATE <  min_number) ||
              ((pt_shent->DATE == min_number) && (index < min_window)))
            {
            min_window = index         ;
            min_offset = j             ;
            min_number = pt_shent->DATE;
            glob_found = 1             ;
            }
          break;
          }
        }
     if (j == 8)
       {
       pt_nxtent                = pt_shent->NEXT;
       pt_shdul->TTABLE [index] = pt_nxtent     ;
       pt_shent->NEXT           = SCH_FREE_SHENT;
       SCH_FREE_SHENT           = pt_shent      ; 
       pt_shent                 = pt_nxtent     ;
       }
     else
       break;
      }

    if ((glob_found == 1) && (min_number == wind_number))
      break;
    }

  if (glob_found == 1)
    (*pt_date) = (min_number << 11) + (min_window << 3) + min_offset;

  return (glob_found);
  }
