

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: cst_AddSetElt.c					*/
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
/* function	: cst_AddSetElement					*/
/* description	: Add a pointer element in a pointer set.		*/
/* called func.	: 							*/
/* ###--------------------------------------------------------------### */

chain_list  *cst_AddSetElement (set,element)

chain_list    *set     ;
void          *element ;

  {
  chain_list  *set_pnt = NULL;
  chain_list  *sav_pnt = NULL;

  CST_STATUS = CST_SUCCEED;

  if ( set == NULL )
    {
    set = addchain ( set, element );
    return( set );
    }

  if ( CST_ELEMENT_COMPARE(element,set->DATA) == 0 )
    {
    return( set );
    }

  if ( CST_ELEMENT_COMPARE(element,set->DATA) < 0 )
    {
    set = addchain ( set, element );
    return( set );
    }

  sav_pnt = set;
  set_pnt = set->NEXT;

  while ( set_pnt != NULL )
    {
    if ( CST_ELEMENT_COMPARE(element,set_pnt->DATA) == 0 )
      {
      return( set );
      }

    if ( CST_ELEMENT_COMPARE(element,set_pnt->DATA) < 0 )
      {
      set_pnt = addchain ( set_pnt, element );
      sav_pnt->NEXT = set_pnt;
      return( set );
      }

    sav_pnt = set_pnt;
    set_pnt = set_pnt->NEXT;
    }

  set_pnt = addchain ( set_pnt, element );
  sav_pnt->NEXT = set_pnt;
  return (set);
  }
