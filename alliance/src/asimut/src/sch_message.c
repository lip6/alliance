
/* ###--------------------------------------------------------------### */
/* file		: sch_message.c						*/
/* date		: Aug 10 1997						*/
/* version	: v101							*/
/* authors	: Nizar ABDALLAH, Pirouz BAZARGAN SABET			*/
/* content	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>

/* ###--------------------------------------------------------------### */
/* function	: sch_message						*/
/* description	: print a message corresponding to the argument code	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void sch_message (code, str1, nmb1)

int   code;
char *str1;
int   nmb1;

  {
  fprintf (stdout, "`%s` :", str1);

  switch (code)
    {
    case 19:
      fprintf (stdout, "stop mark on current structure\n");
      break;
    case 20:
      fprintf (stdout, "--- STACK OVERFLOW !! ---\n");
      break;
    default:
      fprintf (stderr,"unknown code %d\n", code);
    }
  }
