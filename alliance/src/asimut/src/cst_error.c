
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: cst_error.c						*/
/* date		: Dec 24 1994						*/
/* version	: v000							*/
/* authors	: DUNOYER Julien & ABDALLAH Nizar			*/
/* description	: system level functions				*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "cst.h"

/* ###--------------------------------------------------------------### */
/* function	: cst_error						*/
/* description	: print an error message on standard error output	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void cst_error (code, str1, str2, dat1)

int   code;
char *str1;
char *str2;
int   dat1;

  {
  CST_ERRCNT++;
  CST_ERRCOD = code;
  CST_STATUS = CST_ERROR;

  if (((CST_MSGLVL & CST_MSG__ERR) != 0) && ((CST_ERRCNT < CST_MAXERR)))
    {
    fprintf (stderr, "Cst Error %d in func. `%s`: ", code, str1);

    switch (code)
      {

      default:
        fprintf (stderr,"unknown error: %d\n", code);
        break;
      }
    }
  if (CST_ERRCNT == CST_MAXERR)
    fprintf (stderr ,"Too many errors : cant explain them further more"); 
  }
