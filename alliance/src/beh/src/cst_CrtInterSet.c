

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_CrtInterSet.c					*/
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
/* function	: cst_CreateIntersectionSet				*/
/* description	: Create The Intersection between two pointers sets.	*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

chain_list  *cst_CreateIntersectionSet (set1, set2)

chain_list    *set1     ;
chain_list    *set2     ;

  {
  chain_list  *set_pnt  = NULL;

  CST_STATUS = CST_SUCCEED;

  while ((set1 != NULL) && ( set2 != NULL))
    {
    if ( CST_ELEMENT_COMPARE(set1->DATA, set2->DATA) == 0 )
      {
      set_pnt = addchain (set_pnt, set1->DATA);
      set1 = set1->NEXT;
      set2 = set2->NEXT;
      }
    else
      {
      if ( CST_ELEMENT_COMPARE(set1->DATA, set2->DATA) < 0 )
        {
        set1 = set1->NEXT;
        }
      else
        {
        set2 = set2->NEXT;
        }
      }
    }

  set_pnt = reverse( set_pnt);

  return (set_pnt);
  }
