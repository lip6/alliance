

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_GetLastElt.c					*/
/* date		: Dec 20 1994						*/
/* version	: v100							*/
/* authors	: DUNOYER Julien & ABDALLAH Nizar			*/
/* description	: high level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "cst.h"

/* ###--------------------------------------------------------------### */
/* function	: cst_GetLastElement					*/
/* description	: Get the last pointer element in a pointer set.	*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

void  *cst_GetLastElement (set)

chain_list    *set     ;

  {
  void  *element = NULL;

  CST_STATUS = CST_SUCCEED;

  if ( set == NULL )
    {
    CST_STATUS = CST_FAILED;
    }
  else
    {
    while (set->NEXT != NULL)
      {
      set = set->NEXT;
      }
    element = set->DATA;
    }

  return ( element );
  }
