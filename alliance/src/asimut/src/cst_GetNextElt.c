

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_GetNextElt.c					*/
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
/* function	: cst_GetNextElement					*/
/* description	: Get the next pointer element in a pointer set.	*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

void  *cst_GetNextElement (set, current)

chain_list    *set     ;
void          *current ;

  {
  void  *element = NULL;

  CST_STATUS = CST_SUCCEED;

  if ( set == NULL )
    {
    cst_error(2,"cst_GetNextElement",NULL,0);
    return ( element );
    }

  while (set != NULL)
    {
    if ( CST_ELEMENT_COMPARE(current,set->DATA) < 0 )
      {
      element = set->DATA;
      break;
      }
    set = set->NEXT;
    }

  if (element == NULL)
    {
    CST_STATUS = CST_FAILED;
    }

  return ( element );
  }
