

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_DisjunctSet.c					*/
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
/* function	: cst_DisjunctiveSet					*/
/* description	: check The Disjunction between two pointers sets.	*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

int cst_DisjunctiveSet (set1, set2)

chain_list    *set1     ;
chain_list    *set2     ;

  {
  CST_STATUS = CST_SUCCEED;

  while ((set1 != NULL) && ( set2 != NULL))
    {
    if ( CST_ELEMENT_COMPARE(set1->DATA, set2->DATA) == 0 )
      {
      return (0);
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

  return (1);
  }
