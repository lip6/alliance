/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
/* file		: fbh_rmvfbaux.c					*/
/* date		: Aug 24 1992						*/
/* version	: v102							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: contains a specific function used to delete a		*/
/*		  fbhaviour data structure				*/
/*									*/
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "abl.h" 
#include "bdd.h"
#include "fvh.h"

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbaux						*/
/* description	: delete a BEAUX structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbaux *fbh_rmvfbaux (listfbaux, fbauxname, mode)

struct fbaux *listfbaux;	/* list of fbaux containing the object	*/
char         *fbauxname;	/* name of the BEAUX to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct fbaux  headaux;
  struct fbaux *ptlastaux;
  struct fbaux *ptfbaux;

  if (listfbaux != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headaux.NEXT = listfbaux;
    ptfbaux      = &headaux;
    while ((ptfbaux != 0) && (ptfbaux->NAME != fbauxname))
      {
      ptlastaux = ptfbaux;
      ptfbaux   = ptfbaux->NEXT;
      }

    if (ptfbaux != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptfbaux->ABL != 0)
          (void) fprintf (stderr,"FBH_warning : fbaux `%s` not empty\n",
                          fbauxname);
        }
      else
        delablexpr (ptfbaux->ABL);

      ptlastaux->NEXT = ptfbaux->NEXT;
      mbkfree (ptfbaux);
      }

    listfbaux = headaux.NEXT;
    }

  return(listfbaux);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbbus						*/
/* description	: delete a BEBUS structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbbus *fbh_rmvfbbus (listfbbus, fbbusname, mode)

struct fbbus *listfbbus;	/* list of fbbus containing the object	*/
char         *fbbusname;	/* name of the BEBUS to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct fbbus  headbus;
  struct fbbus *ptlastbus;
  struct fbbus *ptfbbus;

  if (listfbbus != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headbus.NEXT = listfbbus;
    ptfbbus      = &headbus;
    while ((ptfbbus != 0) && (ptfbbus->NAME != fbbusname))
      {
      ptlastbus = ptfbbus;
      ptfbbus   = ptfbbus->NEXT;
      }

    if (ptfbbus != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptfbbus->BIABL != 0) || (ptfbbus->BINODE != 0))
          (void) fprintf (stderr,"FBH_warning : fbbus `%s` not empty\n",
                          fbbusname);
        }
      else
        {
        fbh_frebiabl  (ptfbbus->BIABL);
        fbh_frebinode (ptfbbus->BINODE);
        }

      ptlastbus->NEXT = ptfbbus->NEXT;
      mbkfree (ptfbbus);
      }

    listfbbus = headbus.NEXT;
    }

  return(listfbbus);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbbux						*/
/* description	: delete a BEBUX structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbbux *fbh_rmvfbbux (listfbbux, fbbuxname, mode)

struct fbbux *listfbbux;	/* list of fbbux containing the object	*/
char         *fbbuxname;	/* name of the BEBUX to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct fbbux  headbux;
  struct fbbux *ptlastbux;
  struct fbbux *ptfbbux;

  if (listfbbux != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headbux.NEXT = listfbbux;
    ptfbbux      = &headbux;
    while ((ptfbbux != 0) && (ptfbbux->NAME != fbbuxname))
      {
      ptlastbux = ptfbbux;
      ptfbbux   = ptfbbux->NEXT;
      }

    if (ptfbbux != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptfbbux->BIABL != 0) || (ptfbbux->BINODE != 0))
          (void) fprintf (stderr,"FBH_warning : fbbux `%s` not empty\n",
                          fbbuxname);
        }
      else
        {
        fbh_frebiabl  (ptfbbux->BIABL);
        fbh_frebinode (ptfbbux->BINODE);
        }

      ptlastbux->NEXT = ptfbbux->NEXT;
      mbkfree (ptfbbux);
      }

    listfbbux = headbux.NEXT;
    }

  return(listfbbux);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbfig						*/
/* description	: delete a BEFIG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N.	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbfig *fbh_rmvfbfig (listfbfig, fbfigname, mode)

struct fbfig *listfbfig;	/* list of fbfig containing the object	*/
char         *fbfigname;	/* name of the BEFIG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct fbfig  headfig;
  struct fbfig *ptlastfig;
  struct fbfig *ptfbfig;
  struct ptype *ptptype;

  if (listfbfig != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headfig.NEXT = listfbfig;
    ptfbfig      = &headfig;
    while ((ptfbfig != 0) && (ptfbfig->NAME != fbfigname))
      {
      ptlastfig = ptfbfig;
      ptfbfig   = ptfbfig->NEXT;
      }

    if (ptfbfig != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptfbfig->BEREG != 0) || (ptfbfig->BEMSG != 0) ||
            (ptfbfig->BERIN != 0) || (ptfbfig->BEOUT != 0) ||
            (ptfbfig->BEBUS != 0) || (ptfbfig->BEAUX != 0) ||
            (ptfbfig->BEDLY !=0)  || (ptfbfig->BEBUX != 0) || 
	    (ptfbfig->BEPOR != 0) || (ptfbfig->USER  != 0))

          (void) fprintf (stderr,"FBH_warning : fbfig `%s` not empty\n",
                          fbfigname);
        }
      else
        {
        fbh_frefbreg (ptfbfig->BEREG);
        fbh_frefbmsg (ptfbfig->BEMSG);
        fbh_frefbrin (ptfbfig->BERIN);
        fbh_frefbout (ptfbfig->BEOUT);
        fbh_frefbbus (ptfbfig->BEBUS);
        fbh_frefbaux (ptfbfig->BEAUX);
        fbh_frefbaux (ptfbfig->BEDLY);
        fbh_frefbbux (ptfbfig->BEBUX);
        fbh_frefbpor (ptfbfig->BEPOR);
        if ((ptptype = getptype (ptfbfig->USER,FBH_GENERIC)) != 0)
          fbh_frefbgen (ptptype->DATA);
        }

      ptlastfig->NEXT = ptfbfig->NEXT;
      mbkfree (ptfbfig);
      }

    listfbfig = headfig.NEXT;
    }

  return(listfbfig);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbgen						*/
/* description	: delete a BEGEN structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbgen *fbh_rmvfbgen (listfbgen, fbgenname, mode)

struct fbgen *listfbgen;	/* list of fbgen containing the object	*/
char         *fbgenname;	/* name of the BEGEN to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct fbgen  headgen;
  struct fbgen *ptlastgen;
  struct fbgen *ptfbgen;

  if (listfbgen != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headgen.NEXT = listfbgen;
    ptfbgen      = &headgen;
    while ((ptfbgen != 0) && (ptfbgen->NAME != fbgenname))
      {
      ptlastgen = ptfbgen;
      ptfbgen   = ptfbgen->NEXT;
      }

    if (ptfbgen != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptfbgen->VALUE != 0)
          (void) fprintf (stderr,"FBH_warning : fbgen `%s` not empty\n",
                          fbgenname);
        }
      else
        mbkfree (ptfbgen->VALUE);

      ptlastgen->NEXT = ptfbgen->NEXT;
      mbkfree (ptfbgen);
      }

    listfbgen = headgen.NEXT;
    }

  return(listfbgen);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbmsg						*/
/* description	: delete a BEMSG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbmsg *fbh_rmvfbmsg (listfbmsg, fbmsglabl, mode)

struct fbmsg *listfbmsg;	/* list of fbmsg containing the object	*/
char         *fbmsglabl;	/* label of the BEMSG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct fbmsg  headmsg;
  struct fbmsg *ptlastmsg;
  struct fbmsg *ptfbmsg;

  if (listfbmsg != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headmsg.NEXT = listfbmsg;
    ptfbmsg      = &headmsg;
    while ((ptfbmsg != 0) && (ptfbmsg->LABEL != fbmsglabl))
      {
      ptlastmsg = ptfbmsg;
      ptfbmsg   = ptfbmsg->NEXT;
      }

    if (ptfbmsg != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptfbmsg->ABL != 0)
          (void) fprintf (stderr,"FBH_warning : fbmsg `%s` not empty\n",
                          fbmsglabl);
        }
      else
        delablexpr (ptfbmsg->ABL);

      ptlastmsg->NEXT = ptfbmsg->NEXT;
      mbkfree (ptfbmsg);
      }

    listfbmsg = headmsg.NEXT;
    }

  return(listfbmsg);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbout						*/
/* description	: delete a BEOUT structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbout *fbh_rmvfbout (listfbout, fboutname, mode)

struct fbout *listfbout;	/* list of fbout containing the object	*/
char         *fboutname;	/* name of the BEOUT to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct fbout  headout;
  struct fbout *ptlastout;
  struct fbout *ptfbout;

  if (listfbout != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headout.NEXT = listfbout;
    ptfbout      = &headout;
    while ((ptfbout != 0) && (ptfbout->NAME != fboutname))
      {
      ptlastout = ptfbout;
      ptfbout   = ptfbout->NEXT;
      }

    if (ptfbout != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptfbout->ABL != 0)
          (void) fprintf (stderr,"FBH_warning : fbout `%s` not empty\n",
                          fboutname);
        }
      else
        delablexpr (ptfbout->ABL);

      ptlastout->NEXT = ptfbout->NEXT;
      mbkfree (ptfbout);
      }

    listfbout = headout.NEXT;
    }

  return(listfbout);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbpor						*/
/* description	: delete a BEPOR structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbpor *fbh_rmvfbpor (listfbpor, fbporname)

struct fbpor *listfbpor;	/* list of fbpor containing the object	*/
char         *fbporname;	/* name of the BEPOR to be deleted	*/

  {
  struct fbpor  headpor;
  struct fbpor *ptlastpor;
  struct fbpor *ptfbpor;

  if (listfbpor != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headpor.NEXT = listfbpor;
    ptfbpor      = &headpor;
    while ((ptfbpor != 0) && (ptfbpor->NAME != fbporname))
      {
      ptlastpor = ptfbpor;
      ptfbpor   = ptfbpor->NEXT;
      }

    if (ptfbpor != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */

      ptlastpor->NEXT = ptfbpor->NEXT;
      mbkfree (ptfbpor);
      }

    listfbpor = headpor.NEXT;
    }

  return(listfbpor);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbreg						*/
/* description	: delete a BEREG structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbreg *fbh_rmvfbreg (listfbreg, fbregname, mode)

struct fbreg *listfbreg;	/* list of fbreg containing the object	*/
char         *fbregname;	/* name of the BEREG to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct fbreg  headreg;
  struct fbreg *ptlastreg;
  struct fbreg *ptfbreg;

  if (listfbreg != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headreg.NEXT = listfbreg;
    ptfbreg      = &headreg;
    while ((ptfbreg != 0) && (ptfbreg->NAME != fbregname))
      {
      ptlastreg = ptfbreg;
      ptfbreg   = ptfbreg->NEXT;
      }

    if (ptfbreg != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptfbreg->BIABL != 0) || (ptfbreg->BINODE != 0))
          (void) fprintf (stderr,"FBH_warning : fbreg `%s` not empty\n",
                          fbregname);
        }
      else
        {
        fbh_frebiabl  (ptfbreg->BIABL);
        fbh_frebinode (ptfbreg->BINODE);
        }

      ptlastreg->NEXT = ptfbreg->NEXT;
      mbkfree (ptfbreg);
      }

    listfbreg = headreg.NEXT;
    }

  return(listfbreg);
  }

/* ###--------------------------------------------------------------### */
/* function	: fbh_rmvfbrin						*/
/* description	: delete a BERIN structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct fbrin *fbh_rmvfbrin (listfbrin, fbrinname)

struct fbrin *listfbrin;	/* list of fbrin containing the object	*/
char         *fbrinname;	/* name of the BERIN to be deleted	*/

  {
  struct fbrin  headrin;
  struct fbrin *ptlastrin;
  struct fbrin *ptfbrin;

  if (listfbrin != 0)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headrin.NEXT = listfbrin;
    ptfbrin      = &headrin;
    while ((ptfbrin != 0) && (ptfbrin->NAME != fbrinname))
      {
      ptlastrin = ptfbrin;
      ptfbrin   = ptfbrin->NEXT;
      }

    if (ptfbrin != 0)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */

      ptlastrin->NEXT = ptfbrin->NEXT;
      mbkfree (ptfbrin);
      }

    listfbrin = headrin.NEXT;
    }

  return(listfbrin);
  }
