/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: beh_makgex.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Xavier Picat, Pirouz BAZARGAN SABET			*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

#define BEH_GEXBLK	256

static struct begex *BEH_GEXHED = NULL;
static ht           *HashTable  = NULL;

/* ###--------------------------------------------------------------### */
/* function	: beh_addgexex						*/
/* description	: create a GEX expression				*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

static struct begex *beh_addgexex (oper, type)

unsigned int   oper;
unsigned short type;

  {
  struct begex *ptgex;
  int           i;

  if (BEH_GEXHED == NULL)
    {
	/* ###------------------------------------------------------### */
	/*    if there is no more structure allocate a new block	*/
	/* (use OPERAND field to chain structures in a list)		*/
	/* ###------------------------------------------------------### */

    BEH_GEXHED = mbkalloc (sizeof (struct begex) * BEH_GEXBLK);

    ptgex = BEH_GEXHED;
    for (i=1 ; i<BEH_GEXBLK ; i++)
      {
      ptgex->OPERAND = (struct chain *) (ptgex + 1);
      ptgex++;
      }
    ptgex->OPERAND = NULL;
    }

  ptgex      = BEH_GEXHED;
  BEH_GEXHED = (struct begex *) BEH_GEXHED->OPERAND;

  ptgex->TERM    = oper;
  ptgex->TYPE    = type;
  ptgex->OPERAND = NULL;

  return (ptgex);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_addterm						*/
/* description	: create a GEX for a terminal. For each terminal there	*/
/*		  is a unique GEX					*/
/* called func.	: beh_addgexex, mbkalloc, addchain			*/
/* ###--------------------------------------------------------------### */

static struct begex *beh_addterm (oper)

unsigned int oper;

  {
  static struct chain  *term_tab = NULL;
  static unsigned int   bank_nbr = 0;
  struct chain         *tmp_chn  = NULL;
  struct begex         *locl_tab;
  unsigned int          bank;
  struct begex         *resgex;
  int                   i;
  int                   j;

  bank = oper / 64;
  if (bank >= bank_nbr)
    {
    for (i=bank_nbr ; i<=bank ; i++)
      {
      locl_tab = (struct begex *) mbkalloc (64 * sizeof (struct begex));

      for (j=0 ; j<64 ; j++)
        {
        (locl_tab [j]).TERM    = j + (i * 64);
        (locl_tab [j]).TYPE    = 0;
        (locl_tab [j]).OPERAND = NULL;
        }

      if (term_tab == NULL)
        term_tab = addchain (NULL, locl_tab);
      else
        {
        tmp_chn = term_tab;
        while (tmp_chn->NEXT != NULL)
          tmp_chn = tmp_chn->NEXT;
        tmp_chn->NEXT = addchain (NULL, locl_tab);
        }
      }

    bank_nbr = bank + 1;
    }

  tmp_chn = term_tab;
  for (i=0 ; i<bank ; i++)
    tmp_chn = tmp_chn->NEXT;

  locl_tab = (struct begex *) tmp_chn->DATA;

  resgex = & (locl_tab [(unsigned int) (oper % 64)]);

  return (resgex);
  }

/* ###--------------------------------------------------------------### */
/* function	: mad_fregex						*/
/* description	: release a GEX expression (excluding terminals)	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void *mad_fregex (ptgex, mode)

struct begex *ptgex;
char          mode ;

  {
  struct begex *tmpgex;
  struct chain *tmpchn;

  if (ptgex != NULL)
    {
    if ((tmpchn = ptgex->OPERAND) != NULL)
      {
      if (mode == 'a')
        {
        while (tmpchn != NULL)
          {
          mad_fregex ((struct begex *) tmpchn->DATA, 'a');
          tmpchn = tmpchn->NEXT;
          }
        freechain (ptgex->OPERAND);
        }

      ptgex->OPERAND = (struct chain *) BEH_GEXHED;
      BEH_GEXHED     = ptgex;
      }
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: flatgex						*/
/* description	: flatten the top level of a complex expression		*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

static struct begex *flatgex (gexpnt)

struct begex *gexpnt;

  {
  int           top_operator;
  int           operator;
  struct chain *tmp_chn;
  struct chain *cur_oper;
  struct chain *oper_list = NULL;
  struct begex *operand;
  struct begex *resgex   = gexpnt;
  char          flatflag = 0;

  if ((gexpnt != NULL) && (gexpnt->OPERAND != NULL))
    {
    cur_oper = gexpnt->OPERAND;

    while (cur_oper != NULL)
      {
      flatflag     = 0;
      top_operator = gexpnt->TERM;
      operand      = (struct begex *) cur_oper->DATA;

      if (operand->OPERAND != NULL)
        {
        operator = operand->TERM;
        switch (operator)
          {
          case OR :
            if (top_operator == NOT)
              {
              gexpnt->TERM = NOR;
              flatflag = 1;
              }
            else
              {
              if ((top_operator == OR) || (top_operator == NOR))
                flatflag = 1;
              }
            break;

          case AND :
            if (top_operator == NOT)
              {
              gexpnt->TERM = NAND;
              flatflag = 1;
              }
            else
              {
              if ((top_operator == AND) || (top_operator == NAND))
                flatflag = 1;
              }
            break;

          case XOR :
            if (top_operator == NOT)
              {
              gexpnt->TERM = NXOR;
              flatflag = 1;
              }
            else
              {
              if ((top_operator == XOR) || (top_operator == NXOR))
                flatflag = 1;
              }
            break;

          case NXOR :
            if (top_operator == XOR)
              {
              gexpnt->TERM = NXOR;
              flatflag     = 1;
              }
            else
              {
              if ((top_operator == NXOR) || (top_operator == NOT))
                {
                gexpnt->TERM = XOR;
                flatflag     = 1;
                }
              }
            break;
          }
        }

      if (flatflag == 1)
        {
        tmp_chn = operand->OPERAND;
        while (tmp_chn->NEXT != NULL)
          tmp_chn = tmp_chn->NEXT;

        tmp_chn->NEXT = oper_list;
        oper_list     = operand->OPERAND;
        mad_fregex (operand, 'p');
        }
      else
        oper_list = addchain (oper_list, operand);

      cur_oper = cur_oper->NEXT;
      }

    freechain (gexpnt->OPERAND);

    gexpnt->OPERAND = oper_list;
    resgex          = gexpnt;
    }

  return (resgex);
  }

/* ###--------------------------------------------------------------### */
/* function	: rmvconst						*/
/* description	: remove constantes that appear at the top level of a	*/
/*		  espression						*/
/* called func.	: addchain, freechain, mad_fregex			*/
/* ###--------------------------------------------------------------### */

static struct begex *rmvconst (gexpnt)

struct begex *gexpnt;

  {
  struct begex *resgex;
  struct begex *operand;
  struct begex *gex_zero;
  struct begex *gex_one ;
  struct chain *new_oper = NULL;
  struct chain *cur_oper;
  struct chain *tmp_chn = NULL;
  int           operator;
  char          freflag = 0;

  resgex   = gexpnt;
  gex_zero = beh_addterm (0);
  gex_one  = beh_addterm (1);

  if (gexpnt != NULL)
    {
    if (gexpnt->OPERAND != NULL)
      {
      cur_oper  = gexpnt->OPERAND;

      while (cur_oper != NULL)
        {
        operator = gexpnt->TERM;
        operand  = (struct begex *) cur_oper->DATA;
        freflag  = 0;

        if (operand == gex_zero)
          {
          switch (operator)
            {
            case AND :
              freflag = 2; resgex = gex_zero; break;
            case NAND :
            case NOT  :
              freflag = 2; resgex = gex_one ; break;
            case OR   :
            case XOR  :
            case NOR  :
            case NXOR :
              freflag = 1; break;
              break;
            }
          }

        if (operand == gex_one)
          {
          switch (operator)
            {
            case OR :
              freflag = 2; resgex = gex_one ; break;
            case NOR :
            case NOT :
              freflag = 2; resgex = gex_zero; break;
            case NAND :
            case AND  :
              freflag = 1; break;
              break;
            case XOR  :
              gexpnt->TERM = NXOR; freflag = 1; break;
              break;
            case NXOR :
              gexpnt->TERM = XOR ; freflag = 1; break;
              break;
            }
          }

        if (freflag == 2)
          {
          mad_fregex (gexpnt);
          freechain (new_oper);
          gexpnt = NULL;
          new_oper = NULL;
          break;
          }
        else
          {
          if (freflag == 0)
            new_oper = addchain (new_oper, cur_oper->DATA);
          cur_oper = cur_oper->NEXT;
          }
        }
      if (gexpnt != NULL)
        {
        operator = gexpnt->TERM;
        if (new_oper == NULL)
          {
          switch (operator)
            {
            case AND  :
            case NOR  :
            case NXOR :
              resgex = gex_one ;
              break;
            case OR   :
            case XOR  :
            case NAND :
              resgex = gex_zero;
              break;
            }
          mad_fregex (gexpnt);
          gexpnt = NULL;
          }
        else
          {
          if (new_oper->NEXT == NULL)
            {
            switch (operator)
              {
              case AND  :
              case OR   :
              case XOR  :
                resgex = (struct begex *) new_oper->DATA;
                freechain (new_oper);
                gexpnt = NULL;
                new_oper = NULL;
                break;
              case NOR  :
              case NXOR :
              case NAND :
              case NOT  :
                tmp_chn = gexpnt->OPERAND;
                gexpnt->OPERAND = new_oper;
                gexpnt->TERM    = NOT;
                freechain (tmp_chn);
                tmp_chn = NULL;
                resgex  = gexpnt;
                break;
              }
            }
          }
        }
      }
    }

  return (resgex);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_abl2gex						*/
/* description	: transform recursively abl in gex 			*/
/* called func.	: beh_toolbug, beh_abl2gex, gethtitem beh_addgexex,	*/
/*		  namealloc  , addchain					*/
/* ###--------------------------------------------------------------### */

static struct begex *beh_abl2gex (ptr_befig, expr)

struct befig *ptr_befig;
struct chain *expr;

  {
struct chain *my_operand;

  char          delayed [128];
  static char  *str_zero   = NULL;
  static char  *str_one    = NULL;
  struct begex *resgex       ;
  struct begex *gex_elt      ;
  struct chain *oper_list    ;
  struct chain *operand      ;
  char         *term         ;
  long          term_idx     ;
  long          operator     ;

  if (expr == NULL)
    beh_toolbug (103, "beh_abl2gex", NULL, 0);

  if (str_zero == NULL)
    {
    str_zero = namealloc ("'0'");
    str_one  = namealloc ("'1'");
    }

  if (expr->NEXT != NULL)
    {
	/* ###------------------------------------------------------### */
	/*    if the expression is not a terminal ...			*/
	/* ###------------------------------------------------------### */

    operator = (long) ((struct chain *)expr->DATA)->DATA;
    switch (operator)
      {
      case STABLE:

	/* ###------------------------------------------------------### */
	/*    translate signal'stable into a logical expression.	*/
	/* notice :							*/
	/*    signal'stable = not (signal xor signal'delayed)		*/
	/* ###------------------------------------------------------### */

        resgex = beh_addgexex (NXOR, 0);

        operand = (struct chain *) expr->NEXT->DATA;

        if (operand->NEXT != NULL)
          beh_toolbug (100, "beh_abl2gex", NULL, 0);
        else
          {
          term     = (char *) operand->DATA;
          term_idx = gethtitem (HashTable, term);

          if (term_idx == EMPTYHT)
            beh_toolbug (101, "beh_abl2gex", term, 0);
          else
            {
            gex_elt = beh_addterm (term_idx);

            resgex->OPERAND = addchain (NULL, gex_elt);

            sprintf (delayed, "%s'delayed", term);
            term     = namealloc (delayed);
            term_idx = gethtitem (HashTable, term);

            if (term_idx == EMPTYHT)
              beh_toolbug (101, "beh_abl2gex", term, 0);
            else
              {
              gex_elt = beh_addterm (term_idx);
              resgex->OPERAND->NEXT = addchain (NULL, gex_elt);
              }
            }
          }
        break;

      case OR:
      case AND:
      case XOR:
      case NOR:
      case NAND:
      case NXOR:
      case NOT:

	/* ###------------------------------------------------------### */
	/*    for other operator make the top level, then, translate	*/
	/* each operand							*/
	/* ###------------------------------------------------------### */

        resgex    = beh_addgexex (operator, 0);
        oper_list = expr->NEXT;
        while (oper_list != NULL)
          {
          gex_elt         = beh_abl2gex (ptr_befig, oper_list->DATA);
          resgex->OPERAND = addchain (resgex->OPERAND, gex_elt);
          oper_list    = oper_list->NEXT;
          }
        break;

      default:
        beh_toolbug (102, "beh_abl2gex", NULL, operator);
      }

    resgex = rmvconst (resgex);
    resgex = flatgex  (resgex);
    }

  else
    {
	/* ###------------------------------------------------------### */
	/*    if the expression is a terminal create a terminal gex	*/
	/* with the index correspnding to the terminal			*/
	/* ###------------------------------------------------------### */

    term     = (char *) expr->DATA;
    term_idx = gethtitem (HashTable, term);
    if (term_idx == EMPTYHT)
      {
      if (expr->DATA == str_one)
        resgex = beh_addterm (1);
      else
        {
        if (expr->DATA == str_zero)
          resgex = beh_addterm (0);
        else
          beh_toolbug (101, "beh_abl2gex", term, 0);
        }
      }
    else
      resgex = beh_addterm (term_idx);

    }

  return (resgex);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_makgex						*/
/* description	: make gex for each signal of a befig structure 	*/
/* called func.	: beh_abl2gex, addht, addhtitem, delht			*/
/* ###--------------------------------------------------------------### */

void beh_makgex (pt_befig)

struct befig *pt_befig;			/* befig containing expressions	*/

  {
  struct beaux  *pt_beaux ;
  struct bemsg  *pt_bemsg ;
  struct beout  *pt_beout ;
  struct bebus  *pt_bebus ;
  struct bebux  *pt_bebux ;
  struct bereg  *pt_bereg ;
  struct biabl  *pt_biabl ;
  struct binode *pt_binode;
  struct berin  *pt_rin   ;
  unsigned int   count    ;

  if (!(pt_befig->TYPE & BEH_NODE_GEX));
    {
	/* ###------------------------------------------------------### */
	/*    initialize hash table					*/
	/* ###------------------------------------------------------### */

    count  = 1;
    pt_rin = pt_befig->BERIN;

    while (pt_rin != NULL)
      {
      count ++;
      pt_rin = pt_rin->NEXT;
      }

    HashTable = addht (count);

    count  = 2;
    pt_rin = pt_befig->BERIN;

    while (pt_rin != NULL)
      {
      addhtitem (HashTable, pt_rin->NAME, count);
      count ++;
      pt_rin = pt_rin->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a gex for each simple internal signal		*/
	/* ###------------------------------------------------------### */

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      if (pt_befig->TYPE & BEH_NODE_QUAD)
        ((bequad_list *)(pt_beaux->NODE))->GEX =
		      beh_abl2gex (pt_befig, pt_beaux->ABL);
      else
        pt_beaux->NODE= (pNode)beh_abl2gex (pt_befig, pt_beaux->ABL);

      pt_beaux = pt_beaux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a gex for each delayed internal signal		*/
	/* ###------------------------------------------------------### */

    pt_beaux = pt_befig->BEDLY;
    while (pt_beaux != NULL)
      {
      if (pt_befig->TYPE & BEH_NODE_QUAD)
        ((bequad_list *)(pt_beaux->NODE))->GEX =
		      beh_abl2gex (pt_befig, pt_beaux->ABL);
      else 
        pt_beaux->NODE= (pNode)beh_abl2gex(pt_befig,pt_beaux->ABL);

      pt_beaux = pt_beaux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a gex for each assertion				*/
	/* ###------------------------------------------------------### */

    pt_bemsg = pt_befig->BEMSG;
    while (pt_bemsg != NULL)
      {
      if (pt_befig->TYPE & BEH_NODE_QUAD)
        ((bequad_list *)(pt_bemsg->NODE))->GEX =
		      beh_abl2gex (pt_befig, pt_bemsg->ABL);
      else
        pt_bemsg->NODE=(pNode)beh_abl2gex(pt_befig,pt_bemsg->ABL);
      pt_bemsg       = pt_bemsg->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a gex for each simple output				*/
	/* ###------------------------------------------------------### */

    pt_beout = pt_befig->BEOUT;
    while (pt_beout != NULL)
      {
      if (pt_befig->TYPE & BEH_NODE_QUAD)
         ((bequad_list *)(pt_beout->NODE))->GEX =
		      beh_abl2gex (pt_befig, pt_beout->ABL);
      else 
        pt_beout->NODE=(pNode) beh_abl2gex (pt_befig,pt_beout->ABL);

      pt_beout = pt_beout->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a gex for each bussed output				*/
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
          ((bequad_list *)(pt_binode->CNDNODE))->GEX =
		        beh_abl2gex (pt_befig, pt_biabl->CNDABL);
          ((bequad_list *)(pt_binode->VALNODE))->GEX =
		        beh_abl2gex (pt_befig, pt_biabl->VALABL);
          }
        else 
	  {
	  pt_binode->CNDNODE=(pNode)beh_abl2gex(pt_befig,pt_biabl->CNDABL);
	  pt_binode->VALNODE=(pNode)beh_abl2gex(pt_befig,pt_biabl->VALABL);
	  }

        pt_biabl  = pt_biabl->NEXT;
        pt_binode = pt_binode->NEXT;
        }
      pt_bebus = pt_bebus->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a gex for each bussed internal signal		*/
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
          ((bequad_list *)(pt_binode->CNDNODE))->GEX  =
		        beh_abl2gex (pt_befig, pt_biabl->CNDABL);
          ((bequad_list *)(pt_binode->VALNODE))->GEX   =
		        beh_abl2gex (pt_befig, pt_biabl->VALABL);
          }
        else 
	  {
	  pt_binode->CNDNODE=(pNode)beh_abl2gex(pt_befig,pt_biabl->CNDABL);
	  pt_binode->VALNODE=(pNode)beh_abl2gex(pt_befig,pt_biabl->VALABL);
	  }

        pt_biabl  = pt_biabl->NEXT;
        pt_binode = pt_binode->NEXT;
        }
      pt_bebux = pt_bebux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a gex for each internal register			*/
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
          ((bequad_list *)(pt_binode->CNDNODE))->GEX  =
		        beh_abl2gex (pt_befig, pt_biabl->CNDABL);
          ((bequad_list *)(pt_binode->VALNODE))->GEX   =
		        beh_abl2gex (pt_befig, pt_biabl->VALABL);
          }
        else 
	  {
	  pt_binode->CNDNODE=(pNode)beh_abl2gex(pt_befig,pt_biabl->CNDABL);
	  pt_binode->VALNODE=(pNode)beh_abl2gex(pt_befig,pt_biabl->VALABL);
	  }


        pt_biabl  = pt_biabl->NEXT;
        pt_binode = pt_binode->NEXT;
        }
      pt_bereg = pt_bereg->NEXT;
      }

    delht (HashTable);
    pt_befig->TYPE |= BEH_NODE_GEX;
    }
  }
