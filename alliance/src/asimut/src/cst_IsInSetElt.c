

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_IsInSetElt.c					*/
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
/* function	: cst_IsInSetElement					*/
/* description	: Is a pointer element in a pointer set.		*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

int cst_IsInSetElement (set,element)

chain_list    *set     ;
void          *element ;

  {
  chain_list  *set_pnt = NULL;

  CST_STATUS = CST_SUCCEED;

  if ( set == NULL )
    {
    cst_error(2,"cst_IsInSetElement",NULL,0);
    return( 0 );
    }

  set_pnt = set;

  while ( set_pnt != NULL )
    {
    if ( CST_ELEMENT_COMPARE(element,set_pnt->DATA) == 0 )
      {
      return( 1 );
      }

    if ( CST_ELEMENT_COMPARE(element,set_pnt->DATA) < 0 )
      {
      return( 0 );
      }

    set_pnt = set_pnt->NEXT;
    }

  return ( 0 );
  }
