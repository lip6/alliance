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
/* file		: beh_delbeaux.c					*/
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
/* function	: beh_delbeaux						*/
/* description	: delete a BEAUX structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, freeablexpr					*/
/* ###--------------------------------------------------------------### */

struct beaux *beh_delbeaux (listbeaux, ptbeaux, mode)

struct beaux *listbeaux;	/* list of beaux containing the object	*/
struct beaux *ptbeaux;		/* pointer of the BEAUX to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct beaux  headaux;
  struct beaux *ptlastaux;

  if ((listbeaux != NULL) && (ptbeaux != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headaux.NEXT = listbeaux;
    ptlastaux    = &headaux;
    while ((ptlastaux != NULL) && (ptlastaux->NEXT != ptbeaux))
      ptlastaux = ptlastaux->NEXT;

    if (ptlastaux != NULL)
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
                          ptbeaux->NAME);
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
/* file		: beh_delbebus.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbebus						*/
/* description	: delete a BEBUS structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, beh_frebiabl, beh_frebinode			*/
/* ###--------------------------------------------------------------### */

struct bebus *beh_delbebus (listbebus, ptbebus, mode)

struct bebus *listbebus;	/* list of bebus containing the object	*/
struct bebus *ptbebus;		/* pointer of the BEBUS to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bebus  headbus;
  struct bebus *ptlastbus;

  if ((listbebus != NULL) && (ptbebus != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headbus.NEXT = listbebus;
    ptlastbus    = &headbus;
    while ((ptlastbus != NULL) && (ptlastbus->NEXT != ptbebus))
      ptlastbus = ptlastbus->NEXT;

    if (ptlastbus != NULL)
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
                          ptbebus->NAME);
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
/* file		: beh_delbebux.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbebux						*/
/* description	: delete a BEBUX structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, beh_frebiabl, beh_frebinode			*/
/* ###--------------------------------------------------------------### */

struct bebux *beh_delbebux (listbebux, ptbebux, mode)

struct bebux *listbebux;	/* list of bebux containing the object	*/
struct bebux *ptbebux;		/* pointer of the BEBUX to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bebux  headbux;
  struct bebux *ptlastbux;

  if ((listbebux != NULL) && (ptbebux != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headbux.NEXT = listbebux;
    ptlastbux    = &headbux;
    while ((ptlastbux != NULL) && (ptlastbux->NEXT != ptbebux))
      ptlastbux = ptlastbux->NEXT;

    if (ptlastbux != NULL)
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
                          ptbebux->NAME);
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
/* file		: beh_delbefig.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbefig						*/
/* description	: delete a BEFIG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N.	*/
/* called func.	: autfreeblock     , beh_frebereg, beh_frebemsg,		*/
/*		  beh_freberin, beh_frebeout, beh_frebebus,		*/
/*		  beh_frebeaux, beh_frebebux, beh_frebepor,		*/
/*		  beh_frebegen, getptype				*/
/* ###--------------------------------------------------------------### */

struct befig *beh_delbefig (listbefig, ptbefig, mode)

struct befig *listbefig;	/* list of befig containing the object	*/
struct befig *ptbefig;		/* pointer of the BEFIG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct befig  headfig;
  struct befig *ptlastfig;
  struct ptype *ptptype;

  if ((listbefig != NULL) && (ptbefig != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headfig.NEXT = listbefig;
    ptlastfig    = &headfig;
    while ((ptlastfig != NULL) && (ptlastfig->NEXT != ptbefig))
      ptlastfig = ptlastfig->NEXT;

    if (ptlastfig != NULL)
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
                          ptbefig->NAME);
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
/* file		: beh_delbegen.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbegen						*/
/* description	: delete a BEGEN structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

struct begen *beh_delbegen (listbegen, ptbegen, mode)

struct begen *listbegen;	/* list of begen containing the object	*/
struct begen *ptbegen;		/* pointer of the BEGEN to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct begen  headgen;
  struct begen *ptlastgen;

  if ((listbegen != NULL) && (ptbegen != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headgen.NEXT = listbegen;
    ptlastgen    = &headgen;
    while ((ptlastgen != NULL) && (ptlastgen->NEXT != ptbegen))
      ptlastgen = ptlastgen->NEXT;

    if (ptlastgen != NULL)
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
                          ptbegen->NAME);
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
/* file		: beh_delbemsg.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbemsg						*/
/* description	: delete a BEMSG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, freeablexpr					*/
/* ###--------------------------------------------------------------### */

struct bemsg *beh_delbemsg (listbemsg, ptbemsg, mode)

struct bemsg *listbemsg;	/* list of bemsg containing the object	*/
struct bemsg *ptbemsg;		/* pointer of the BEMSG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bemsg  headmsg;
  struct bemsg *ptlastmsg;

  if ((listbemsg != NULL) && (ptbemsg != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headmsg.NEXT = listbemsg;
    ptlastmsg    = &headmsg;
    while ((ptlastmsg != NULL) && (ptlastmsg->NEXT != ptbemsg))
      ptlastmsg = ptlastmsg->NEXT;

    if (ptlastmsg != NULL)
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
                          ptbemsg->LABEL);
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
/* file		: beh_delbeout.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbeout						*/
/* description	: delete a BEOUT structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, freeablexpr					*/
/* ###--------------------------------------------------------------### */

struct beout *beh_delbeout (listbeout, ptbeout, mode)

struct beout *listbeout;	/* list of beout containing the object	*/
struct beout *ptbeout;		/* pointer of the BEOUT to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct beout  headout;
  struct beout *ptlastout;

  if ((listbeout != NULL) && (ptbeout != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headout.NEXT = listbeout;
    ptlastout    = &headout;
    while ((ptlastout != NULL) && (ptlastout->NEXT != ptbeout))
      ptlastout = ptlastout->NEXT;

    if (ptlastout != NULL)
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
                          ptbeout->NAME);
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
/* file		: beh_delbepor.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbepor						*/
/* description	: delete a BEPOR structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

struct bepor *beh_delbepor (listbepor, ptbepor)

struct bepor *listbepor;	/* list of bepor containing the object	*/
struct bepor *ptbepor;		/* pointer of the BEPOR to be deleted	*/

  {
  struct bepor  headpor;
  struct bepor *ptlastpor;

  if ((listbepor != NULL) && (ptbepor != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headpor.NEXT = listbepor;
    ptlastpor    = &headpor;
    while ((ptlastpor != NULL) && (ptlastpor->NEXT != ptbepor))
      ptlastpor = ptlastpor->NEXT;

    if (ptlastpor != NULL)
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
/* file		: beh_delbereg.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbereg						*/
/* description	: delete a BEREG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, beh_frebiabl, beh_frebinode			*/
/* ###--------------------------------------------------------------### */

struct bereg *beh_delbereg (listbereg, ptbereg, mode)

struct bereg *listbereg;	/* list of bereg containing the object	*/
struct bereg *ptbereg;		/* pointer of the BEREG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bereg  headreg;
  struct bereg *ptlastreg;

  if ((listbereg != NULL) && (ptbereg != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headreg.NEXT = listbereg;
    ptlastreg    = &headreg;
    while ((ptlastreg != NULL) && (ptlastreg->NEXT != ptbereg))
      ptlastreg = ptlastreg->NEXT;

    if (ptlastreg != NULL)
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
                          ptbereg->NAME);
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
/* file		: beh_delberin.c					*/
/* date		: Sep 20 1994						*/
/* version	: v107							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delberin						*/
/* description	: delete a BERIN structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

struct berin *beh_delberin (listberin, ptberin)

struct berin *listberin;	/* list of berin containing the object	*/
struct berin *ptberin;		/* pointer of the BERIN to be deleted	*/

  {
  struct berin  headrin;
  struct berin *ptlastrin;

  if ((listberin != NULL) && (ptberin != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headrin.NEXT = listberin;
    ptlastrin    = &headrin;
    while ((ptlastrin != NULL) && (ptlastrin->NEXT != ptberin))
      ptlastrin = ptlastrin->NEXT;

    if (ptlastrin != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */

      ptlastrin->NEXT = ptberin->NEXT;
      autfreeblock (ptberin);
      }

    listberin = headrin.NEXT;
    }

  return(listberin);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_delbiabl.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbiabl						*/
/* description	: delete a BIABL structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock, freeablexpr					*/
/* ###--------------------------------------------------------------### */

struct biabl *beh_delbiabl (listbiabl, ptbiabl, mode)

struct biabl *listbiabl;	/* list of biabl containing the object	*/
struct biabl *ptbiabl;		/* pointer of the BIABL to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct biabl  headabl;
  struct biabl *ptlastabl;

  if ((listbiabl != NULL) && (ptbiabl != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headabl.NEXT = listbiabl;
    ptlastabl    = &headabl;
    while ((ptlastabl != NULL) && (ptlastabl->NEXT != ptbiabl))
      ptlastabl = ptlastabl->NEXT;

    if (ptlastabl != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptbiabl->VALABL != NULL) || (ptbiabl->CNDABL != NULL))
          (void) fprintf (stderr,"BEH_warning : biabl not empty\n");
        }
      else
        {
        freeablexpr (ptbiabl->VALABL);
        freeablexpr (ptbiabl->CNDABL);
        freeptype(ptbiabl->USER);
        }

      ptlastabl->NEXT = ptbiabl->NEXT;
      autfreeblock (ptbiabl);
      }

    listbiabl = headabl.NEXT;
    }

  return(listbiabl);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_delbinod.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_delbinode						*/
/* description	: delete a BINODE structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

struct binode *beh_delbinode (listbinode, ptbinode, mode)

struct binode *listbinode;	/* list of binode containing the object	*/
struct binode *ptbinode;	/* pointer of the BINODE to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct binode  headnode;
  struct binode *ptlastnode;

  if ((listbinode != NULL) && (ptbinode != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headnode.NEXT = listbinode;
    ptlastnode    = &headnode;
    while ((ptlastnode != NULL) && (ptlastnode->NEXT != ptbinode))
      ptlastnode = ptlastnode->NEXT;

    if (ptlastnode != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptbinode->VALNODE != NULL) || (ptbinode->CNDNODE != NULL))
          (void) fprintf (stderr,"BEH_warning : binode not empty\n");
        }

      ptlastnode->NEXT = ptbinode->NEXT;
      autfreeblock (ptbinode);
      }

    listbinode = headnode.NEXT;
    }

  return(listbinode);
  }
