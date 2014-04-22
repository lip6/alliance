

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_GetPrevElt.c					*/
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
/* function	: cst_GetPreviousElement				*/
/* description	: Get the previous pointer element in a pointer set.	*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

void  *cst_GetPreviousElement (set, current)

chain_list    *set     ;
void          *current ;

  {
  void        *element = NULL;
  chain_list  *sav_pnt = NULL;

  CST_STATUS = CST_SUCCEED;

  if ( set == NULL )
    {
    cst_error(2,"cst_GetPreviousElement",NULL,0);
    }

  while (set != NULL)
    {
    if ( CST_ELEMENT_COMPARE(current,set->DATA) <= 0 )
      {
      break;
      }
    sav_pnt = set;
    set = set->NEXT;
    }

  if (sav_pnt == NULL)
    {
    CST_STATUS = CST_FAILED;
    }
  else
    {
    element = sav_pnt->DATA;
    }

  return ( element );
  }
