
/* ###--------------------------------------------------------------### */
/* file		: sch_error.c						*/
/* date		: Aug 10 1997						*/
/* version	: v101							*/
/* authors	: Nizar ABDALLAH, Pirouz BAZARGAN SABET			*/
/* content	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>

/* ###--------------------------------------------------------------### */
/* function	: sch_error						*/
/* description	: print an error message depending on the argument code	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

int sch_error (code, str1, str2)

int   code;
char *str1;
char *str2;

  {
  fprintf (stderr,"Error %d in `%s`:", code, str1);

  switch (code)
    {
    case 118:
      fprintf (stderr,"command not understood\n");
      break;
    case 119:
      fprintf (stderr,"cannot display VOID pointer\n");
      break;
    case 120:
      fprintf (stderr,"cannot push NULL pointer\n");
      break;
    default:
      fprintf (stderr,"unknown error code\n");
      break;
    }

  return (1);
  }
