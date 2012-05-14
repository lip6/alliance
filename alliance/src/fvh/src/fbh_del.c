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
/*                                     */
/* file        : fbh_delfbaux.c                    */
/* date        : Aug 24 1992                        */
/* version    : v102                            */
/* authors    : Pirouz BAZARGAN SABET                    */
/* content    : contains a specific function used to delete a        */
/*          fbhaviour data structure                */
/*                                    */
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "abl.h" 
#include "bdd.h"
#include "fvh.h"

/* ###--------------------------------------------------------------### */
/* function    : fbh_delfbaux                        */
/* description    : delete a BEAUX structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbaux *fbh_delfbaux (listfbaux, ptfbaux, mode)

struct fbaux *listfbaux;    /* list of fbaux containing the object    */
struct fbaux *ptfbaux;        /* pointer of the BEAUX to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbaux  headaux;
struct fbaux *ptlastaux;

if ((listfbaux != 0) && (ptfbaux != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headaux.NEXT = listfbaux;
  ptlastaux    = &headaux;
  while ((ptlastaux != 0) && (ptlastaux->NEXT != ptfbaux))
    ptlastaux = ptlastaux->NEXT;

  if (ptlastaux != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if (ptfbaux->ABL != 0)
        (void) fprintf (stderr,"FBH_warning : fbaux `%s` not empty\n",
                        ptfbaux->NAME);
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
/* function    : fbh_delfbbus                        */
/* description    : delete a BEBUS structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbbus *fbh_delfbbus (listfbbus, ptfbbus, mode)

struct fbbus *listfbbus;    /* list of fbbus containing the object    */
struct fbbus *ptfbbus;        /* pointer of the BEBUS to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbbus  headbus;
struct fbbus *ptlastbus;

if ((listfbbus != 0) && (ptfbbus != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headbus.NEXT = listfbbus;
  ptlastbus    = &headbus;
  while ((ptlastbus != 0) && (ptlastbus->NEXT != ptfbbus))
    ptlastbus = ptlastbus->NEXT;

  if (ptlastbus != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if ((ptfbbus->BIABL != 0) || (ptfbbus->BINODE != 0))
        (void) fprintf (stderr,"FBH_warning : fbbus `%s` not empty\n",
                        ptfbbus->NAME);
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
/* function    : fbh_delfbbux                        */
/* description    : delete a BEBUX structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbbux *fbh_delfbbux (listfbbux, ptfbbux, mode)

struct fbbux *listfbbux;    /* list of fbbux containing the object    */
struct fbbux *ptfbbux;        /* pointer of the BEBUX to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbbux  headbux;
struct fbbux *ptlastbux;

if ((listfbbux != 0) && (ptfbbux != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headbux.NEXT = listfbbux;
  ptlastbux    = &headbux;
  while ((ptlastbux != 0) && (ptlastbux->NEXT != ptfbbux))
    ptlastbux = ptlastbux->NEXT;

  if (ptlastbux != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if ((ptfbbux->BIABL != 0) || (ptfbbux->BINODE != 0))
        (void) fprintf (stderr,"FBH_warning : fbbux `%s` not empty\n",
                        ptfbbux->NAME);
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
/* function    : fbh_delfbfig                        */
/* description    : delete a BEFIG structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N.    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbfig *fbh_delfbfig (listfbfig, ptfbfig, mode)

struct fbfig *listfbfig;    /* list of fbfig containing the object    */
struct fbfig *ptfbfig;        /* pointer of the BEFIG to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbfig  headfig;
struct fbfig *ptlastfig;
struct ptype *ptptype;

if ((listfbfig != 0) && (ptfbfig != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headfig.NEXT = listfbfig;
  ptlastfig    = &headfig;
  while ((ptlastfig != 0) && (ptlastfig->NEXT != ptfbfig))
    ptlastfig = ptlastfig->NEXT;

  if (ptlastfig != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if ((ptfbfig->BEREG != 0) || (ptfbfig->BEMSG != 0) ||
          (ptfbfig->BERIN != 0) || (ptfbfig->BEOUT != 0) ||
          (ptfbfig->BEBUS != 0) || (ptfbfig->BEAUX != 0) ||
          (ptfbfig->BEDLY != 0) || (ptfbfig->BEBUX != 0) || 
      (ptfbfig->BEPOR != 0) || (ptfbfig->USER  != 0))

        (void) fprintf (stderr,"FBH_warning : fbfig `%s` not empty\n",
                        ptfbfig->NAME);
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
/* function    : fbh_delfbgen                        */
/* description    : delete a BEGEN structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbgen *fbh_delfbgen (listfbgen, ptfbgen, mode)

struct fbgen *listfbgen;    /* list of fbgen containing the object    */
struct fbgen *ptfbgen;        /* pointer of the BEGEN to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbgen  headgen;
struct fbgen *ptlastgen;

if ((listfbgen != 0) && (ptfbgen != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headgen.NEXT = listfbgen;
  ptlastgen    = &headgen;
  while ((ptlastgen != 0) && (ptlastgen->NEXT != ptfbgen))
    ptlastgen = ptlastgen->NEXT;

  if (ptlastgen != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if (ptfbgen->VALUE != 0)
        (void) fprintf (stderr,"FBH_warning : fbgen `%s` not empty\n",
                        ptfbgen->NAME);
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
/* function    : fbh_delfbmsg                        */
/* description    : delete a BEMSG structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbmsg *fbh_delfbmsg (listfbmsg, ptfbmsg, mode)

struct fbmsg *listfbmsg;    /* list of fbmsg containing the object    */
struct fbmsg *ptfbmsg;        /* pointer of the BEMSG to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbmsg  headmsg;
struct fbmsg *ptlastmsg;

if ((listfbmsg != 0) && (ptfbmsg != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headmsg.NEXT = listfbmsg;
  ptlastmsg    = &headmsg;
  while ((ptlastmsg != 0) && (ptlastmsg->NEXT != ptfbmsg))
    ptlastmsg = ptlastmsg->NEXT;

  if (ptlastmsg != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if (ptfbmsg->ABL != 0)
        (void) fprintf (stderr,"FBH_warning : fbmsg `%s` not empty\n",
                        ptfbmsg->LABEL);
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
/* function    : fbh_delfbout                        */
/* description    : delete a BEOUT structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbout *fbh_delfbout (listfbout, ptfbout, mode)

struct fbout *listfbout;    /* list of fbout containing the object    */
struct fbout *ptfbout;        /* pointer of the BEOUT to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbout  headout;
struct fbout *ptlastout;

if ((listfbout != 0) && (ptfbout != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headout.NEXT = listfbout;
  ptlastout    = &headout;
  while ((ptlastout != 0) && (ptlastout->NEXT != ptfbout))
    ptlastout = ptlastout->NEXT;

  if (ptlastout != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if (ptfbout->ABL != 0)
        (void) fprintf (stderr,"FBH_warning : fbout `%s` not empty\n",
                        ptfbout->NAME);
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
/* function    : fbh_delfbpor                        */
/* description    : delete a BEPOR structure and return the pointer of    */
/*          the next object.                    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbpor *fbh_delfbpor (listfbpor, ptfbpor)

struct fbpor *listfbpor;    /* list of fbpor containing the object    */
struct fbpor *ptfbpor;        /* pointer of the BEPOR to be deleted    */

{
struct fbpor  headpor;
struct fbpor *ptlastpor;

if ((listfbpor != 0) && (ptfbpor != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headpor.NEXT = listfbpor;
  ptlastpor    = &headpor;
  while ((ptlastpor != 0) && (ptlastpor->NEXT != ptfbpor))
    ptlastpor = ptlastpor->NEXT;

  if (ptlastpor != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /* ###------------------------------------------------------### */

    ptlastpor->NEXT = ptfbpor->NEXT;
    mbkfree (ptfbpor);
    }

  listfbpor = headpor.NEXT;
  }

return(listfbpor);
}

/* ###--------------------------------------------------------------### */
/* function    : fbh_delfbreg                        */
/* description    : delete a BEREG structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbreg *fbh_delfbreg (listfbreg, ptfbreg, mode)

struct fbreg *listfbreg;    /* list of fbreg containing the object    */
struct fbreg *ptfbreg;        /* pointer of the BEREG to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbreg  headreg;
struct fbreg *ptlastreg;

if ((listfbreg != 0) && (ptfbreg != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headreg.NEXT = listfbreg;
  ptlastreg    = &headreg;
  while ((ptlastreg != 0) && (ptlastreg->NEXT != ptfbreg))
    ptlastreg = ptlastreg->NEXT;

  if (ptlastreg != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if ((ptfbreg->BIABL != 0) || (ptfbreg->BINODE != 0))
        (void) fprintf (stderr,"FBH_warning : fbreg `%s` not empty\n",
                        ptfbreg->NAME);
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
/* function    : fbh_delfbrin                        */
/* description    : delete a BERIN structure and return the pointer of    */
/*          the next object.                    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbrin *fbh_delfbrin (listfbrin, ptfbrin)

struct fbrin *listfbrin;    /* list of fbrin containing the object    */
struct fbrin *ptfbrin;        /* pointer of the BERIN to be deleted    */

{
struct fbrin  headrin;
struct fbrin *ptlastrin;

if ((listfbrin != 0) && (ptfbrin != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headrin.NEXT = listfbrin;
  ptlastrin    = &headrin;
  while ((ptlastrin != 0) && (ptlastrin->NEXT != ptfbrin))
    ptlastrin = ptlastrin->NEXT;

  if (ptlastrin != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /* ###------------------------------------------------------### */

    ptlastrin->NEXT = ptfbrin->NEXT;
    mbkfree (ptfbrin);
    }

  listfbrin = headrin.NEXT;
  }

return(listfbrin);
}

/* ###--------------------------------------------------------------### */
/* function    : fbh_delbiabl                        */
/* description    : delete a BIABL structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbbiabl *fbh_delbiabl (listbiabl, ptbiabl, mode)

struct fbbiabl *listbiabl;    /* list of fbbiabl containing the object    */
struct fbbiabl *ptbiabl;        /* pointer of the BIABL to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbbiabl  headabl;
struct fbbiabl *ptlastabl;

if ((listbiabl != 0) && (ptbiabl != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headabl.NEXT = listbiabl;
  ptlastabl    = &headabl;
  while ((ptlastabl != 0) && (ptlastabl->NEXT != ptbiabl))
    ptlastabl = ptlastabl->NEXT;

  if (ptlastabl != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if ((ptbiabl->VALABL != 0) || (ptbiabl->CNDABL != 0))
        (void) fprintf (stderr,"FBH_warning : fbbiabl not empty\n");
      }
    else
      {
      delablexpr (ptbiabl->VALABL);
      delablexpr (ptbiabl->CNDABL);
      }

    ptlastabl->NEXT = ptbiabl->NEXT;
    mbkfree (ptbiabl);
    }

  listbiabl = headabl.NEXT;
  }

return(listbiabl);
}

/* ###--------------------------------------------------------------### */
/* function    : fbh_delbinode                        */
/* description    : delete a BINODE structure and return the pointer of    */
/*          the next object. A warning is printed out if the    */
/*          object to be deleted is not empty when the mode is N    */
/* called func.    : mbkfree                        */
/* ###--------------------------------------------------------------### */

struct fbbinode *fbh_delbinode (listbinode, ptbinode, mode)

struct fbbinode *listbinode;    /* list of fbbinode containing the object    */
struct fbbinode *ptbinode;    /* pointer of the BINODE to be deleted    */
char          mode;        /* recursive delete or not (Y or N)    */

{
struct fbbinode  headnode;
struct fbbinode *ptlastnode;

if ((listbinode != 0) && (ptbinode != 0))
  {

  /* ###------------------------------------------------------### */
  /*    Search the object to be deleted                */
  /* ###------------------------------------------------------### */

  headnode.NEXT = listbinode;
  ptlastnode    = &headnode;
  while ((ptlastnode != 0) && (ptlastnode->NEXT != ptbinode))
    ptlastnode = ptlastnode->NEXT;

  if (ptlastnode != 0)
    {

  /* ###------------------------------------------------------### */
  /*    If the object doesn't exist return the list without    */
  /* modification.                        */
  /*    If the object has been found check the mode and, if asked    */
  /* delete pointed objects recursively.                */
  /* ###------------------------------------------------------### */

    if (mode == 'N')
      {
      if ((ptbinode->VALNODE != 0) || (ptbinode->CNDNODE != 0))
        (void) fprintf (stderr,"FBH_warning : fbbinode not empty\n");
      }

    ptlastnode->NEXT = ptbinode->NEXT;
    mbkfree (ptbinode);
    }

  listbinode = headnode.NEXT;
  }

return(listbinode);
}
