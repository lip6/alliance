

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_CmpSet.c					*/
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
/* function	: cst_SetCompare					*/
/* description	: compare two sets.					*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

int  cst_SetCompare (set1, set2)

chain_list    *set1     ;
chain_list    *set2     ;

  {
  CST_STATUS = CST_SUCCEED;

  while ((set1 != NULL) && ( set2 != NULL))
    {
    if ( CST_ELEMENT_COMPARE(set1->DATA, set2->DATA) == 0 )
      {
      set1 = set1->NEXT;
      set2 = set2->NEXT;
      }
    else
      return(1);
    }

  if ((set1 != NULL) || (set2 != NULL))
      return(1);

  return (0);
  }
