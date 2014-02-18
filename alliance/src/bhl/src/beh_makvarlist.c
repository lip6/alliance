/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: beh_makvarlist.c					*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: DUNOYER Julien.					*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_abl2varlist					*/
/* description	: for an expression create the list of terminals. Each	*/
/*		  terminal may be present only once in the list.	*/
/*		  Constante values are not present in the list. Each	*/
/*		  terminal is represented by its BDD index		*/
/*									*/
/* called func.	: searchInputCct   , beh_abl2varlist   , beh_error,	*/
/*		  cst_AddSetElement, cst_CreateUnionSet			*/
/* ###--------------------------------------------------------------### */

static struct chain *beh_abl2varlist (pt_befig, expr)

struct befig *pt_befig;		/* pointer on  behavioural figure	*/
struct chain *expr    ;		/* pointer abl expression		*/

  {
  struct chain   *set_pnt              = NULL;
  struct circuit *pt_circuit           = pt_befig->CIRCUI;
  int             err_flg              = 0;
  int             index                ;
  char           *atom                 ;
  static char     buffer [128]         ;
  struct chain   *cst_AddSetElement  ();
  struct chain   *cst_CreateUnionSet ();

	/* ###------------------------------------------------------### */
	/*    check that the expression exists				*/
	/* ###------------------------------------------------------### */

  if (expr == NULL)
    err_flg = beh_error (2, NULL);
  else
    {
    if (expr->NEXT == NULL)
      {
	/* ###------------------------------------------------------### */
	/*    if the expression is a terminal (atom)			*/
	/* ###------------------------------------------------------### */

      atom = (char *) expr->DATA;

	/* ###------------------------------------------------------### */
	/*    if the terminal is a primary signal create an element	*/
	/* with its index and return					*/
	/* ###------------------------------------------------------### */

      if ((index = searchInputCct (pt_circuit, atom)) != VIDE)
        set_pnt = cst_AddSetElement (set_pnt, index);
      }
    else
      {
	/* ###------------------------------------------------------### */
	/*    if the expression is not a terminal (is a complex expr.)	*/
	/* ###------------------------------------------------------### */

      if (OPER(expr) != STABLE)
       {
       while ((expr = CDR(expr)) != NULL)
         {
         set_pnt = cst_CreateUnionSet
                      (set_pnt, beh_abl2varlist (pt_befig, CAR(expr)));
         }
       }
      else
       {
	expr = CDR(expr);
	if (!ATOM(expr))
	  err_flg = beh_error (4, NULL);
        else
          {
	  atom = (char *)VALUE_ATOM (CAR(expr));

          if ((index = searchInputCct (pt_circuit, atom)) != VIDE)
            {
            set_pnt = cst_AddSetElement (set_pnt, index);

	    sprintf (buffer, "%s'delayed", atom);
	    atom = namealloc (buffer);

            if ((index = searchInputCct (pt_circuit, atom)) != VIDE)
              {
              set_pnt = cst_AddSetElement (set_pnt, index);
              }
            else
	      err_flg = beh_error (3, atom);
            }
          else
	    err_flg = beh_error (3, atom);
          }
        }
      }
    }

  if (err_flg != 0)
    pt_befig->ERRFLG = 1;

  return (set_pnt);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_mvlsttot						*/
/* description	: make list of variables for all primary signals	*/
/*		  simple and bussed outputs,				*/
/*		  simple and bussed internal signals, registers.	*/
/* called func.	: beh_abl2varlist					*/
/* ###--------------------------------------------------------------### */

static void beh_mvlsttot (pt_befig)

struct befig *pt_befig;		/* pointer on current BEFIG	*/

  {
  struct beaux  *pt_beaux;
  struct beaux  *pt_bedly;
  struct bemsg  *pt_bemsg;
  struct beout  *pt_beout;
  struct bebus  *pt_bebus;
  struct bebux  *pt_bebux;
  struct bereg  *pt_bereg;
  struct biabl  *pt_biabl;
  struct binode *pt_binode;

	/* ###------------------------------------------------------### */
	/*    make a var list for each simple internal signal		*/
	/* ###------------------------------------------------------### */

  pt_beaux = pt_befig->BEAUX;

  while (pt_beaux != NULL)
    {
    if (pt_befig->TYPE & BEH_NODE_QUAD)
      {
      ((bequad_list *)(pt_beaux->NODE))->VARLIST = 
                        beh_abl2varlist (pt_befig, pt_beaux->ABL);
      }
    else
      {
      pt_beaux->NODE = (struct node *)beh_abl2varlist (pt_befig, pt_beaux->ABL);
      }
    pt_beaux = pt_beaux->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a var list for each delayed internal signal		*/
	/* ###------------------------------------------------------### */

  pt_bedly = pt_befig->BEDLY;
  while (pt_bedly != NULL)
    {
    if (pt_befig->TYPE & BEH_NODE_QUAD)
      {
      ((bequad_list *)(pt_bedly->NODE))->VARLIST = 
                        beh_abl2varlist (pt_befig, pt_bedly->ABL);
      }
    else
      {
      pt_bedly->NODE = (struct node *)beh_abl2varlist (pt_befig, pt_bedly->ABL);
      }
    pt_bedly = pt_bedly->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a var list for each assertion			*/
	/* ###------------------------------------------------------### */

  pt_bemsg = pt_befig->BEMSG;
  while (pt_bemsg != NULL)
    {
    if (pt_befig->TYPE & BEH_NODE_QUAD)
      {
      ((bequad_list *)(pt_bemsg->NODE))->VARLIST = 
                        beh_abl2varlist (pt_befig, pt_bemsg->ABL);
      }
    else
      {
      pt_bemsg->NODE = (struct node *)beh_abl2varlist (pt_befig, pt_bemsg->ABL);
      }

    pt_bemsg = pt_bemsg->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a var list for each simple output port		*/
	/* ###------------------------------------------------------### */

  pt_beout = pt_befig->BEOUT;
  while (pt_beout != NULL)
    {
    if (pt_befig->TYPE & BEH_NODE_QUAD)
      {
      ((bequad_list *)(pt_beout->NODE))->VARLIST = 
                        beh_abl2varlist (pt_befig, pt_beout->ABL);
      }
    else
      {
      pt_beout->NODE = (struct node *)beh_abl2varlist (pt_befig, pt_beout->ABL);
      }

    pt_beout = pt_beout->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a var list for each bussed output port		*/
	/* ###------------------------------------------------------### */

  pt_bebus = pt_befig->BEBUS;
  while (pt_bebus != NULL)
    {
    pt_biabl  = pt_bebus->BIABL;
    pt_binode = pt_bebus->BINODE;

    while (pt_biabl != NULL)
      {
      if (pt_befig->TYPE & BEH_NODE_QUAD)
        {
        ((bequad_list *)(pt_binode->CNDNODE))->VARLIST = 
                              beh_abl2varlist (pt_befig, pt_biabl->CNDABL);
        ((bequad_list *)(pt_binode->VALNODE))->VARLIST = 
                              beh_abl2varlist (pt_befig, pt_biabl->VALABL);
        }
      else
        {
        pt_binode->CNDNODE = (struct node *)beh_abl2varlist (pt_befig, pt_biabl->CNDABL);
        pt_binode->VALNODE = (struct node *)beh_abl2varlist (pt_befig, pt_biabl->VALABL);
        }

      pt_biabl = pt_biabl->NEXT;
      pt_binode = pt_binode->NEXT;
      }

    pt_bebus = pt_bebus->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a var list for each bussed internal signal		*/
	/* ###------------------------------------------------------### */

  pt_bebux = pt_befig->BEBUX;
  while (pt_bebux != NULL)
    {
    pt_biabl  = pt_bebux->BIABL;
    pt_binode = pt_bebux->BINODE;

    while (pt_biabl != NULL)
      {
      if (pt_befig->TYPE & BEH_NODE_QUAD)
        {
        ((bequad_list *)(pt_binode->CNDNODE))->VARLIST = 
                              beh_abl2varlist (pt_befig, pt_biabl->CNDABL);
        ((bequad_list *)(pt_binode->VALNODE))->VARLIST = 
                              beh_abl2varlist (pt_befig, pt_biabl->VALABL);
        }
      else
        {
        pt_binode->CNDNODE = (struct node *)beh_abl2varlist (pt_befig, pt_biabl->CNDABL);
        pt_binode->VALNODE = (struct node *)beh_abl2varlist (pt_befig, pt_biabl->VALABL);
        }

      pt_biabl  = pt_biabl->NEXT;
      pt_binode = pt_binode->NEXT;
      }
    pt_bebux = pt_bebux->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a var list for each internal register		*/
	/* ###------------------------------------------------------### */

  pt_bereg = pt_befig->BEREG;
  while (pt_bereg != NULL)
    {
    pt_biabl  = pt_bereg->BIABL;
    pt_binode = pt_bereg->BINODE;

    while (pt_biabl != NULL)
      {
      if (pt_befig->TYPE & BEH_NODE_QUAD)
        {
        ((bequad_list *)(pt_binode->CNDNODE))->VARLIST = 
                              beh_abl2varlist (pt_befig, pt_biabl->CNDABL);
        ((bequad_list *)(pt_binode->VALNODE))->VARLIST = 
                              beh_abl2varlist (pt_befig, pt_biabl->VALABL);
        }
      else
        {
        pt_binode->CNDNODE = (struct node *)beh_abl2varlist (pt_befig, pt_biabl->CNDABL);
        pt_binode->VALNODE = (struct node *)beh_abl2varlist (pt_befig, pt_biabl->VALABL);
        }

      pt_biabl  = pt_biabl->NEXT;
      pt_binode = pt_binode->NEXT;
      }
    pt_bereg = pt_bereg->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_makvarlist					*/
/* description	: make var list for each signal of a befig structure 	*/
/* called func.	: initializeCct, addInputCct, beh_mvlsttot		*/
/* ###--------------------------------------------------------------### */

void beh_makvarlist (pt_befig)

struct befig *pt_befig;

  {
  int         err_flg   = 0;

  if (!(pt_befig->TYPE & BEH_NODE_VARLIST));
    {
    beh_indexbdd (pt_befig);

        /* ###------------------------------------------------------### */
        /*    get the list of variables for all expressions		*/
        /* ###------------------------------------------------------### */

    beh_mvlsttot (pt_befig);

    pt_befig->TYPE |= BEH_NODE_VARLIST;

    if (err_flg != 0)
      pt_befig->ERRFLG = 1;
    }

  }
