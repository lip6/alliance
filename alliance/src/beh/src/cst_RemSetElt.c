

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_RemSetElt.c					*/
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
/* function	: cst_RemSetElement					*/
/* description	: Remove a pointer element in a pointer set.		*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

chain_list  *cst_RemSetElement (set,element)

chain_list    *set     ;
void          *element ;

  {
  chain_list  *set_pnt = NULL;

  CST_STATUS = CST_SUCCEED;

  if ( set == NULL )
    {
    cst_error(2,"cst_RemSetElement",NULL,0);
    return( set );
    }

  set_pnt = set;

  while ( set_pnt != NULL )
    {
    if ( CST_ELEMENT_COMPARE(element,set_pnt->DATA) == 0 )
      {
      set = delchain (set, set_pnt);
      return( set );
      }

    if ( CST_ELEMENT_COMPARE(element,set_pnt->DATA) < 0 )
      {
      cst_error(4,"cst_RemSetElement",NULL,0);
      return( set );
      }

    set_pnt = set_pnt->NEXT;
    }

  cst_error(4,"cst_RemSetElement",NULL,0);
  return (set);
  }
