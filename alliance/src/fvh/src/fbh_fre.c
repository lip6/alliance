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
/* file		: fbh_frefbaux.c					*/
/* date		: Aug 24 1992						*/
/* version	: v102							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: contains a specific function used to delete a list	*/
/*		  of fbhaviour data structures				*/
/*									*/
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "abl.h" 
#include "bdd.h"
#include "fvh.h"

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbaux						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbaux (listfbaux)

struct fbaux *listfbaux;	/* list of fbaux to be deleted		*/

  {
  struct fbaux *ptfbaux;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listfbaux != 0)
    {
    freeablexpr (listfbaux->ABL);

    ptfbaux   = listfbaux;
    listfbaux = listfbaux->NEXT;
    mbkfree (ptfbaux);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbbus						*/
/* description	: delete a list of BEBUS structures and all objects	*/
/*		  pointed by any os BEBUSs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbbus (listfbbus)

struct fbbus *listfbbus;	/* list of fbbus to be deleted		*/

  {
  struct fbbus *ptfbbus;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listfbbus != 0)
    {
    fbh_frebiabl  (listfbbus->BIABL);
    fbh_frebinode (listfbbus->BINODE);

    ptfbbus   = listfbbus;
    listfbbus = listfbbus->NEXT;
    mbkfree (ptfbbus);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbbux						*/
/* description	: delete a list of BEBUX structures and all objects	*/
/*		  pointed by any os BEBUXs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbbux (listfbbux)

struct fbbux *listfbbux;	/* list of fbbux to be deleted		*/

  {
  struct fbbux *ptfbbux;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listfbbux != 0)
    {
    fbh_frebiabl  (listfbbux->BIABL);
    fbh_frebinode (listfbbux->BINODE);

    ptfbbux   = listfbbux;
    listfbbux = listfbbux->NEXT;
    mbkfree (ptfbbux);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbfig						*/
/* description	: delete a list of BEFIG structures and all objects	*/
/*		  pointed by any os BEFIGs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbfig (listfbfig)

struct fbfig *listfbfig;	/* list of fbfig to be deleted		*/

  {
  struct fbfig *ptfbfig;
  struct ptype *ptptype;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */



  while (listfbfig != 0)
    {
    ptfbfig   = listfbfig;
    fbh_frefbreg (ptfbfig->BEREG);
    fbh_frefbmsg (ptfbfig->BEMSG);
    fbh_frefbrin (ptfbfig->BERIN);
    fbh_frefbout (ptfbfig->BEOUT);
    fbh_frefbbus (ptfbfig->BEBUS);
    fbh_frefbaux (ptfbfig->BEAUX);
    fbh_frefbaux (ptfbfig->BEDLY);
    fbh_frefbbux (ptfbfig->BEBUX);
    fbh_frefbpor (ptfbfig->BEPOR);
    fbh_frefbpgm (ptfbfig->BEPGM);
    if ((ptptype = getptype (ptfbfig->USER,FBH_GENERIC)) != 0)
      fbh_frefbgen (ptptype->DATA);

    listfbfig = listfbfig->NEXT;
    mbkfree (ptfbfig);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbgen						*/
/* description	: delete a list of BEGEN structures and all objects	*/
/*		  pointed by any os BEGENs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbgen (listfbgen)

struct fbgen *listfbgen;	/* list of fbgen to be deleted		*/

  {
  struct fbgen *ptfbgen;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listfbgen != 0)
    {
    mbkfree (listfbgen->VALUE);

    ptfbgen   = listfbgen;
    listfbgen = listfbgen->NEXT;
    mbkfree (ptfbgen);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbmsg						*/
/* description	: delete a list of BEMSG structures and all objects	*/
/*		  pointed by any os BEMSGs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbmsg (listfbmsg)

struct fbmsg *listfbmsg;	/* list of fbmsg to be deleted		*/

  {
  struct fbmsg *ptfbmsg;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listfbmsg != 0)
    {
    freeablexpr (listfbmsg->ABL);

    ptfbmsg   = listfbmsg;
    listfbmsg = listfbmsg->NEXT;
    mbkfree (ptfbmsg);
    }

  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_frefbmsg                                          */
/* description  : delete a list of BEMSG structures and all objects     */
/*                pointed by any os BEMSGs in the list                  */
/* called func. : mbkfree                                               */
/* ###--------------------------------------------------------------### */

void fbh_frefbpgm (listfbpgm)

struct fbpgm *listfbpgm;        /* list of fbpgm to be deleted          */

  {
  struct fbpgm *ptfbpgm;

        /* ###------------------------------------------------------### */
        /*    for each object of the list, first delete pointed objects */
        /* then, delete the object itself                               */
        /* ###------------------------------------------------------### */

  while (listfbpgm != 0)
    {
    ptfbpgm   = listfbpgm;
    listfbpgm = listfbpgm->NEXT;
    mbkfree (ptfbpgm);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbout						*/
/* description	: delete a list of BEOUT structures and all objects	*/
/*		  pointed by any os BEOUTs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbout (listfbout)

struct fbout *listfbout;	/* list of fbout to be deleted		*/

  {
  struct fbout *ptfbout;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listfbout != 0)
    {
    freeablexpr (listfbout->ABL);

    ptfbout   = listfbout;
    listfbout = listfbout->NEXT;
    mbkfree (ptfbout);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbpor						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbpor (listfbpor)

struct fbpor *listfbpor;	/* list of fbpor to be deleted		*/

  {
  struct fbpor *ptfbpor;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, delete the object itself	*/
	/* ###------------------------------------------------------### */

  while (listfbpor != 0)
    {
    ptfbpor   = listfbpor;
    listfbpor = listfbpor->NEXT;
    mbkfree (ptfbpor);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbreg						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbreg (listfbreg)

struct fbreg *listfbreg;	/* list of fbreg to be deleted		*/

  {
  struct fbreg *ptfbreg;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listfbreg != 0)
    {
    fbh_frebiabl  (listfbreg->BIABL);
    fbh_frebinode (listfbreg->BINODE);

    ptfbreg   = listfbreg;
    listfbreg = listfbreg->NEXT;
    mbkfree (ptfbreg);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbrin						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbrin (listfbrin)

struct fbrin *listfbrin;	/* list of fbrin to be deleted		*/

  {
  struct fbrin *ptfbrin;

	/* ###------------------------------------------------------### */
	/*    for each object of the list delete the object itself	*/
	/* ###------------------------------------------------------### */

  while (listfbrin != 0)
    {
    ptfbrin   = listfbrin;
    listfbrin = listfbrin->NEXT;
    mbkfree (ptfbrin);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frefbtyp						*/
/* description	: delete a list of BETYP structures and all objects	*/
/*		  pointed by any os BETYPs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frefbtyp (listfbtyp)

struct fbtyp *listfbtyp;	/* list of fbtyp to be deleted		*/

  {
  struct fbtyp *ptfbtyp;

	/* ###------------------------------------------------------### */
	/*    for each object of the list delete the object itself	*/
	/* ###------------------------------------------------------### */

  while (listfbtyp != 0)
    {
    ptfbtyp   = listfbtyp;
    listfbtyp = listfbtyp->NEXT;
    mbkfree (ptfbtyp);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frebiabl						*/
/* description	: delete a list of BIABL structures and all objects	*/
/*		  pointed by any os BIABLs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frebiabl (listbiabl)

struct fbbiabl *listbiabl;	/* list of fbbiabl to be deleted		*/

  {
  struct fbbiabl *ptbiabl;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbiabl != 0)
    {
    freeablexpr (listbiabl->VALABL);
    freeablexpr (listbiabl->CNDABL);

    ptbiabl   = listbiabl;
    listbiabl = listbiabl->NEXT;
    mbkfree (ptbiabl);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_frebinode						*/
/* description	: delete a list of BINODE structures and all objects	*/
/*		  pointed by any os BINODEs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void fbh_frebinode (listbinode)

struct fbbinode *listbinode;	/* list of fbbinode to be deleted		*/

  {
  struct fbbinode *ptbinode;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbinode != 0)
    {
    ptbinode   = listbinode;
    listbinode = listbinode->NEXT;
    mbkfree (ptbinode);
    }

  }
