

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_CrtCompSet.c					*/
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
/* function	: cst_CreateComplementSet				*/
/* description	: Create The Complement between two pointers sets.	*/
/*                This mean set1 - set2.				*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

chain_list  *cst_CreateComplementSet (set1, set2)

chain_list    *set1     ;
chain_list    *set2     ;

  {
  chain_list  *set_pnt  = NULL;

  CST_STATUS = CST_SUCCEED;

  while ((set1 != NULL) && (set2 != NULL))
    {
    if ( CST_ELEMENT_COMPARE(set1->DATA, set2->DATA) < 0 )
      {
      set_pnt = addchain (set_pnt, set1->DATA);
      set1 = set1->NEXT;
      }
    else
      {
      if ( CST_ELEMENT_COMPARE(set1->DATA, set2->DATA) == 0 )
        {
        set1 = set1->NEXT;
        set2 = set2->NEXT;
        }
      else
        {
        cst_error(5, "cst_CreateComplementSet", NULL, 0);
        set2 = set2->NEXT;
        }
      }
    }

  while (set1 != NULL)
    {
    set_pnt = addchain (set_pnt, set1->DATA);
    set1 = set1->NEXT;
    }

  if (set2 != NULL)
    {
    cst_error(5, "cst_CreateComplementSet", NULL, 0);
    }

  set_pnt = reverse (set_pnt);
  return (set_pnt);
  }
