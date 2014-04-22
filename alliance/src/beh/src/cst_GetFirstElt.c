

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_GetFirstElt.c					*/
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
/* function	: cst_GetFirstElement					*/
/* description	: Get the first pointer element in a pointer set.	*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

void  *cst_GetFirstElement (set)

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
    element = set->DATA;
    }

  return ( element );
  }
