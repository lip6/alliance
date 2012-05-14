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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbeaux.c					*/
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
/* function	: beh_rmvbeaux						*/
/* description	: delete a BEAUX structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, freeablexpr					*/
/* ###--------------------------------------------------------------### */

struct beaux *beh_rmvbeaux (listbeaux, beauxname, mode)

struct beaux *listbeaux;	/* list of beaux containing the object	*/
char         *beauxname;	/* name of the BEAUX to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct beaux  headaux;
  struct beaux *ptlastaux;
  struct beaux *ptbeaux;

  if (listbeaux != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headaux.NEXT = listbeaux;
    headaux.NAME = NULL     ;
    headaux.ABL  = NULL     ;
    ptbeaux      = &headaux;
    while ((ptbeaux != NULL) && (ptbeaux->NAME != beauxname))
      {
      ptlastaux = ptbeaux;
      ptbeaux   = ptbeaux->NEXT;
      }

    if (ptbeaux != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptbeaux->ABL != NULL)
          (void) fprintf (stderr,"BEH_warning : beaux `%s` not empty\n",
                          beauxname);
        }
      else
        freeablexpr (ptbeaux->ABL);

      ptlastaux->NEXT = ptbeaux->NEXT;
      autfreeblock (ptbeaux);
      }

    listbeaux = headaux.NEXT;
    }

  return(listbeaux);
  }
/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbebus.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbebus						*/
/* description	: delete a BEBUS structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, beh_frebiabl, beh_frebinode			*/
/* ###--------------------------------------------------------------### */

struct bebus *beh_rmvbebus (listbebus, bebusname, mode)

struct bebus *listbebus;	/* list of bebus containing the object	*/
char         *bebusname;	/* name of the BEBUS to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bebus  headbus;
  struct bebus *ptlastbus;
  struct bebus *ptbebus;

  if (listbebus != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headbus.NEXT   = listbebus;
    headbus.NAME   = NULL     ;
    headbus.BIABL  = NULL     ;
    headbus.BINODE = NULL     ;
    ptbebus        = &headbus;
    while ((ptbebus != NULL) && (ptbebus->NAME != bebusname))
      {
      ptlastbus = ptbebus;
      ptbebus   = ptbebus->NEXT;
      }

    if (ptbebus != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptbebus->BIABL != NULL) || (ptbebus->BINODE != NULL))
          (void) fprintf (stderr,"BEH_warning : bebus `%s` not empty\n",
                          bebusname);
        }
      else
        {
        beh_frebiabl  (ptbebus->BIABL);
        beh_frebinode (ptbebus->BINODE);
        }

      ptlastbus->NEXT = ptbebus->NEXT;
      autfreeblock (ptbebus);
      }

    listbebus = headbus.NEXT;
    }

  return(listbebus);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbebux.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbebux						*/
/* description	: delete a BEBUX structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, beh_frebiabl, beh_frebinode			*/
/* ###--------------------------------------------------------------### */

struct bebux *beh_rmvbebux (listbebux, bebuxname, mode)

struct bebux *listbebux;	/* list of bebux containing the object	*/
char         *bebuxname;	/* name of the BEBUX to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bebux  headbux;
  struct bebux *ptlastbux;
  struct bebux *ptbebux;

  if (listbebux != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headbux.NEXT   = listbebux;
    headbux.NAME   = NULL     ;
    headbux.BIABL  = NULL     ;
    headbux.BINODE = NULL     ;
    ptbebux        = &headbux;
    while ((ptbebux != NULL) && (ptbebux->NAME != bebuxname))
      {
      ptlastbux = ptbebux;
      ptbebux   = ptbebux->NEXT;
      }

    if (ptbebux != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptbebux->BIABL != NULL) || (ptbebux->BINODE != NULL))
          (void) fprintf (stderr,"BEH_warning : bebux `%s` not empty\n",
                          bebuxname);
        }
      else
        {
        beh_frebiabl  (ptbebux->BIABL);
        beh_frebinode (ptbebux->BINODE);
        }

      ptlastbux->NEXT = ptbebux->NEXT;
      autfreeblock (ptbebux);
      }

    listbebux = headbux.NEXT;
    }

  return(listbebux);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbefig.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbefig						*/
/* description	: delete a BEFIG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N.	*/
/* called func.	: autfreeblock     , beh_frebereg, beh_frebemsg,		*/
/*		  beh_freberin, beh_frebeout, beh_frebebus,		*/
/*		  beh_frebeaux, beh_frebebux, beh_frebepor,		*/
/*		  beh_frebegen, getptype				*/
/* ###--------------------------------------------------------------### */

struct befig *beh_rmvbefig (listbefig, befigname, mode)

struct befig *listbefig;	/* list of befig containing the object	*/
char         *befigname;	/* name of the BEFIG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct befig  headfig;
  struct befig *ptlastfig;
  struct befig *ptbefig;
  struct ptype *ptptype;

  if (listbefig != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headfig.NEXT  = listbefig;
    headfig.NAME  = NULL     ;
    headfig.BEREG = NULL     ;
    headfig.BEMSG = NULL     ;
    headfig.BERIN = NULL     ;
    headfig.BEAUX = NULL     ;
    headfig.BEBUX = NULL     ;
    headfig.BEDLY = NULL     ;
    headfig.BEPOR = NULL     ;
    headfig.BEPGM = NULL     ;
    headfig.USER  = NULL     ;
    ptbefig       = &headfig;
    while ((ptbefig != NULL) && (ptbefig->NAME != befigname))
      {
      ptlastfig = ptbefig;
      ptbefig   = ptbefig->NEXT;
      }

    if (ptbefig != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptbefig->BEREG != NULL) || (ptbefig->BEMSG != NULL) ||
            (ptbefig->BERIN != NULL) || (ptbefig->BEOUT != NULL) ||
            (ptbefig->BEBUS != NULL) || (ptbefig->BEAUX != NULL) ||
            (ptbefig->BEBUX != NULL) || (ptbefig->BEDLY != NULL) ||
            (ptbefig->BEPOR != NULL) || (ptbefig->USER  != NULL) ||
            (ptbefig->BEPGM != NULL))

          (void) fprintf (stderr,"BEH_warning : befig `%s` not empty\n",
                          befigname);
        }
      else
        {
        beh_frebereg (ptbefig->BEREG);
        beh_frebemsg (ptbefig->BEMSG);
        beh_freberin (ptbefig->BERIN);
        beh_frebeout (ptbefig->BEOUT);
        beh_frebebus (ptbefig->BEBUS);
        beh_frebeaux (ptbefig->BEAUX);
        beh_frebeaux (ptbefig->BEDLY);
        beh_frebebux (ptbefig->BEBUX);
        beh_frebepor (ptbefig->BEPOR);
        beh_frebepgm (ptbefig->BEPGM);
        if ((ptptype = getptype (ptbefig->USER,BEH_GENERIC)) != NULL)
          beh_frebegen (ptptype->DATA);
        }

      ptlastfig->NEXT = ptbefig->NEXT;
      autfreeblock (ptbefig);
      }

    listbefig = headfig.NEXT;
    }

  return(listbefig);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbegen.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbegen						*/
/* description	: delete a BEGEN structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

struct begen *beh_rmvbegen (listbegen, begenname, mode)

struct begen *listbegen;	/* list of begen containing the object	*/
char         *begenname;	/* name of the BEGEN to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct begen  headgen;
  struct begen *ptlastgen;
  struct begen *ptbegen;

  if (listbegen != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headgen.NEXT  = listbegen;
    headgen.NAME  = NULL     ;
    headgen.VALUE = NULL     ;
    ptbegen       = &headgen;
    while ((ptbegen != NULL) && (ptbegen->NAME != begenname))
      {
      ptlastgen = ptbegen;
      ptbegen   = ptbegen->NEXT;
      }

    if (ptbegen != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptbegen->VALUE != NULL)
          (void) fprintf (stderr,"BEH_warning : begen `%s` not empty\n",
                          begenname);
        }
      else
        autfreeblock (ptbegen->VALUE);

      ptlastgen->NEXT = ptbegen->NEXT;
      autfreeblock (ptbegen);
      }

    listbegen = headgen.NEXT;
    }

  return(listbegen);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbemsg.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbemsg						*/
/* description	: delete a BEMSG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, freeablexpr					*/
/* ###--------------------------------------------------------------### */

struct bemsg *beh_rmvbemsg (listbemsg, bemsglabl, mode)

struct bemsg *listbemsg;	/* list of bemsg containing the object	*/
char         *bemsglabl;	/* label of the BEMSG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bemsg  headmsg;
  struct bemsg *ptlastmsg;
  struct bemsg *ptbemsg;

  if (listbemsg != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headmsg.NEXT  = listbemsg;
    headmsg.LABEL = NULL     ;
    headmsg.ABL   = NULL     ;
    ptbemsg       = &headmsg;
    while ((ptbemsg != NULL) && (ptbemsg->LABEL != bemsglabl))
      {
      ptlastmsg = ptbemsg;
      ptbemsg   = ptbemsg->NEXT;
      }

    if (ptbemsg != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptbemsg->ABL != NULL)
          (void) fprintf (stderr,"BEH_warning : bemsg `%s` not empty\n",
                          bemsglabl);
        }
      else
        freeablexpr (ptbemsg->ABL);

      ptlastmsg->NEXT = ptbemsg->NEXT;
      autfreeblock (ptbemsg);
      }

    listbemsg = headmsg.NEXT;
    }

  return(listbemsg);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbeout.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbeout						*/
/* description	: delete a BEOUT structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, freeablexpr					*/
/* ###--------------------------------------------------------------### */

struct beout *beh_rmvbeout (listbeout, beoutname, mode)

struct beout *listbeout;	/* list of beout containing the object	*/
char         *beoutname;	/* name of the BEOUT to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct beout  headout;
  struct beout *ptlastout;
  struct beout *ptbeout;

  if (listbeout != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headout.NEXT = listbeout;
    headout.NAME = NULL     ;
    headout.ABL  = NULL     ;
    ptbeout      = &headout;
    while ((ptbeout != NULL) && (ptbeout->NAME != beoutname))
      {
      ptlastout = ptbeout;
      ptbeout   = ptbeout->NEXT;
      }

    if (ptbeout != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptbeout->ABL != NULL)
          (void) fprintf (stderr,"BEH_warning : beout `%s` not empty\n",
                          beoutname);
        }
      else
        freeablexpr (ptbeout->ABL);

      ptlastout->NEXT = ptbeout->NEXT;
      autfreeblock (ptbeout);
      }

    listbeout = headout.NEXT;
    }

  return(listbeout);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbepor.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbepor						*/
/* description	: delete a BEPOR structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

struct bepor *beh_rmvbepor (listbepor, beporname)

struct bepor *listbepor;	/* list of bepor containing the object	*/
char         *beporname;	/* name of the BEPOR to be deleted	*/

  {
  struct bepor  headpor;
  struct bepor *ptlastpor;
  struct bepor *ptbepor;

  if (listbepor != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headpor.NEXT = listbepor;
    headpor.NAME = NULL     ;
    ptbepor      = &headpor;
    while ((ptbepor != NULL) && (ptbepor->NAME != beporname))
      {
      ptlastpor = ptbepor;
      ptbepor   = ptbepor->NEXT;
      }

    if (ptbepor != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */

      ptlastpor->NEXT = ptbepor->NEXT;
      autfreeblock (ptbepor);
      }

    listbepor = headpor.NEXT;
    }

  return(listbepor);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvbereg.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbereg						*/
/* description	: delete a BEREG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, beh_frebiabl, beh_frebinode			*/
/* ###--------------------------------------------------------------### */

struct bereg *beh_rmvbereg (listbereg, beregname, mode)

struct bereg *listbereg;	/* list of bereg containing the object	*/
char         *beregname;	/* name of the BEREG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bereg  headreg;
  struct bereg *ptlastreg;
  struct bereg *ptbereg;

  if (listbereg != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headreg.NEXT   = listbereg;
    headreg.NAME   = NULL     ;
    headreg.BIABL  = NULL     ;
    headreg.BINODE = NULL     ;
    ptbereg        = &headreg;
    while ((ptbereg != NULL) && (ptbereg->NAME != beregname))
      {
      ptlastreg = ptbereg;
      ptbereg   = ptbereg->NEXT;
      }

    if (ptbereg != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptbereg->BIABL != NULL) || (ptbereg->BINODE != NULL))
          (void) fprintf (stderr,"BEH_warning : bereg `%s` not empty\n",
                          beregname);
        }
      else
        {
        beh_frebiabl  (ptbereg->BIABL);
        beh_frebinode (ptbereg->BINODE);
        }

      ptlastreg->NEXT = ptbereg->NEXT;
      autfreeblock (ptbereg);
      }

    listbereg = headreg.NEXT;
    }

  return(listbereg);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_rmvberin.c					*/
/* date		: Sep 20 1994						*/
/* version	: v107							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvberin						*/
/* description	: delete a BERIN structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

struct berin *beh_rmvberin (listberin, berinname)

struct berin *listberin;	/* list of berin containing the object	*/
char         *berinname;	/* name of the BERIN to be deleted	*/

  {
  struct berin  headrin;
  struct berin *ptlastrin;
  struct berin *ptberin;

  if (listberin != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headrin.NEXT    = listberin;
    headrin.NAME    = NULL;
    headrin.OUT_REF = NULL;
    headrin.MSG_REF = NULL;
    headrin.AUX_REF = NULL;
    headrin.BUX_REF = NULL;
    headrin.BUS_REF = NULL;
    headrin.REG_REF = NULL;
    headrin.DLY_REF = NULL;
    ptberin      = &headrin;
    while ((ptberin != NULL) && (ptberin->NAME != berinname))
      {
      ptlastrin = ptberin;
      ptberin   = ptberin->NEXT;
      }

    if (ptberin != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */

      freechain (ptberin->OUT_REF);
      freechain (ptberin->MSG_REF);
      freechain (ptberin->AUX_REF);
      freechain (ptberin->BUX_REF);
      freechain (ptberin->BUS_REF);
      freechain (ptberin->REG_REF);
      freechain (ptberin->DLY_REF);

      ptlastrin->NEXT = ptberin->NEXT;
      autfreeblock (ptberin);
      }

    listberin = headrin.NEXT;
    }

  return(listberin);
  }
