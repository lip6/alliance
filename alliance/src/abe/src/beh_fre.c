/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebeaux.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "abe.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_frebeaux						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebeaux (listbeaux)

struct beaux *listbeaux;	/* list of beaux to be deleted		*/

  {
  struct beaux *ptbeaux;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbeaux != NULL)
    {
    freeablexpr (listbeaux->ABL);

    ptbeaux   = listbeaux;
    listbeaux = listbeaux->NEXT;
    autfreeblock (ptbeaux);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebebus.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebebus						*/
/* description	: delete a list of BEBUS structures and all objects	*/
/*		  pointed by any os BEBUSs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebebus (listbebus)

struct bebus *listbebus;	/* list of bebus to be deleted		*/

  {
  struct bebus *ptbebus;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbebus != NULL)
    {
    beh_frebiabl  (listbebus->BIABL);
    beh_frebinode (listbebus->BINODE);

    ptbebus   = listbebus;
    listbebus = listbebus->NEXT;
    autfreeblock (ptbebus);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebebux.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebebux						*/
/* description	: delete a list of BEBUX structures and all objects	*/
/*		  pointed by any os BEBUXs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebebux (listbebux)

struct bebux *listbebux;	/* list of bebux to be deleted		*/

  {
  struct bebux *ptbebux;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbebux != NULL)
    {
    beh_frebiabl  (listbebux->BIABL);
    beh_frebinode (listbebux->BINODE);

    ptbebux   = listbebux;
    listbebux = listbebux->NEXT;
    autfreeblock (ptbebux);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebefig.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebefig						*/
/* description	: delete a list of BEFIG structures and all objects	*/
/*		  pointed by any os BEFIGs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebefig (listbefig)

struct befig *listbefig;	/* list of befig to be deleted		*/

  {
  struct befig *ptbefig;
  struct ptype *ptptype;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbefig != NULL)
    {
    beh_frebereg (listbefig->BEREG);
    beh_frebemsg (listbefig->BEMSG);
    beh_freberin (listbefig->BERIN);
    beh_frebeout (listbefig->BEOUT);
    beh_frebebus (listbefig->BEBUS);
    beh_frebeaux (listbefig->BEAUX);
    beh_frebeaux (listbefig->BEDLY);
    beh_frebebux (listbefig->BEBUX);
    beh_frebepor (listbefig->BEPOR);
    beh_frebepgm (listbefig->BEPGM);
    if ((ptptype = getptype (listbefig->USER,BEH_GENERIC)) != NULL)
      beh_frebegen (ptptype->DATA);

    ptbefig   = listbefig;
    listbefig = listbefig->NEXT;
    autfreeblock (ptbefig);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebegen.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebegen						*/
/* description	: delete a list of BEGEN structures and all objects	*/
/*		  pointed by any os BEGENs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebegen (listbegen)

struct begen *listbegen;	/* list of begen to be deleted		*/

  {
  struct begen *ptbegen;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbegen != NULL)
    {
    autfreeblock (listbegen->VALUE);

    ptbegen   = listbegen;
    listbegen = listbegen->NEXT;
    autfreeblock (ptbegen);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebemsg.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebemsg						*/
/* description	: delete a list of BEMSG structures and all objects	*/
/*		  pointed by any os BEMSGs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebemsg (listbemsg)

struct bemsg *listbemsg;	/* list of bemsg to be deleted		*/

  {
  struct bemsg *ptbemsg;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbemsg != NULL)
    {
    freeablexpr (listbemsg->ABL);

    ptbemsg   = listbemsg;
    listbemsg = listbemsg->NEXT;
    autfreeblock (ptbemsg);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebeout.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebeout						*/
/* description	: delete a list of BEOUT structures and all objects	*/
/*		  pointed by any os BEOUTs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebeout (listbeout)

struct beout *listbeout;	/* list of beout to be deleted		*/

  {
  struct beout *ptbeout;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbeout != NULL)
    {
    freeablexpr (listbeout->ABL);

    ptbeout   = listbeout;
    listbeout = listbeout->NEXT;
    autfreeblock (ptbeout);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebepgm.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebepgm						*/
/* description	: delete a list of BEMSG structures and all objects	*/
/*		  pointed by any os BEMSGs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebepgm (listbepgm)

struct bepgm *listbepgm;	/* list of bepgm to be deleted		*/

  {
  struct bepgm *ptbepgm;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbepgm != NULL)
    {
    ptbepgm   = listbepgm;
    listbepgm = listbepgm->NEXT;
    autfreeblock (ptbepgm);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebepor.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebepor						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebepor (listbepor)

struct bepor *listbepor;	/* list of bepor to be deleted		*/

  {
  struct bepor *ptbepor;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, delete the object itself	*/
	/* ###------------------------------------------------------### */

  while (listbepor != NULL)
    {
    ptbepor   = listbepor;
    listbepor = listbepor->NEXT;
    autfreeblock (ptbepor);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebereg.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebereg						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebereg (listbereg)

struct bereg *listbereg;	/* list of bereg to be deleted		*/

  {
  struct bereg *ptbereg;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbereg != NULL)
    {
    beh_frebiabl  (listbereg->BIABL);
    beh_frebinode (listbereg->BINODE);

    ptbereg   = listbereg;
    listbereg = listbereg->NEXT;
    autfreeblock (ptbereg);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_freberin.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_freberin						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_freberin (listberin)

struct berin *listberin;	/* list of berin to be deleted		*/

  {
  struct berin *ptberin;

	/* ###------------------------------------------------------### */
	/*    for each object of the list delete the object itself	*/
	/* ###------------------------------------------------------### */

  while (listberin != NULL)
    {
    ptberin   = listberin;
    listberin = listberin->NEXT;
    autfreeblock (ptberin);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebiabl.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebiabl						*/
/* description	: delete a list of BIABL structures and all objects	*/
/*		  pointed by any os BIABLs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebiabl (listbiabl)

struct biabl *listbiabl;	/* list of biabl to be deleted		*/

  {
  struct biabl *ptbiabl;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbiabl != NULL)
    {
    freeablexpr (listbiabl->VALABL);
    freeablexpr (listbiabl->CNDABL);
    freeptype(listbiabl->USER);

    ptbiabl   = listbiabl;
    listbiabl = listbiabl->NEXT;
    autfreeblock (ptbiabl);
    }

  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_frebinod.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_frebinode						*/
/* description	: delete a list of BINODE structures and all objects	*/
/*		  pointed by any os BINODEs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_frebinode (listbinode)

struct binode *listbinode;	/* list of binode to be deleted		*/

  {
  struct binode *ptbinode;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbinode != NULL)
    {
    ptbinode   = listbinode;
    listbinode = listbinode->NEXT;
    autfreeblock (ptbinode);
    }

  }
