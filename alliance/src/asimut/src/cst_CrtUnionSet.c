

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_CrtUnionSet.c					*/
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
/* function	: cst_CreateUnionSet					*/
/* description	: Create The Union between two pointers sets.		*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

chain_list  *cst_CreateUnionSet (set1, set2)

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
        set_pnt = addchain (set_pnt, set1->DATA);
        set1 = set1->NEXT;
        }
      else
        {
        set_pnt = addchain (set_pnt, set2->DATA);
        set2 = set2->NEXT;
        }
      }
    }

  while (set1 != NULL)
    {
    set_pnt = addchain (set_pnt, set1->DATA);
    set1 = set1->NEXT;
    }

  while (set2 != NULL)
    {
    set_pnt = addchain (set_pnt, set2->DATA);
    set2 = set2->NEXT;
    }

  set_pnt = reverse(set_pnt);

  return (set_pnt);
  }
