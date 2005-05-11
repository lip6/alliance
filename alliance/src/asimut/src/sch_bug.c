
/* ###--------------------------------------------------------------### */
/* file		: sch_bug.c						*/
/* date		: Aug 10 1997						*/
/* version	: v101							*/
/* authors	: Nizar ABDALLAH, Pirouz BAZARGAN SABET			*/
/* content	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <stdlib.h>

/* ###--------------------------------------------------------------### */
/* function	: sch_bug						*/
/* description	: print a bug message depending on the argument code	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void sch_bug (code, str1, str2)

int   code;
char *str1;
char *str2;

  {
  fprintf (stderr,"bug %d in `%s`:", code, str1);

  switch (code)
    {
    case 1:
      fprintf (stderr, "unconsistency: no transaction at current time\n");
      break;
    case 2:
      fprintf (stderr, "unconsistency: not the first transaction\n");
      break;
    default:
      fprintf (stderr,"unknown bug\n");
      break;
    }

  exit (1);
  }
