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
/* file		: beh_makgex.c						*/
/* date		: Mar 20 1994						*/
/* version	: v108							*/
/* authors	: Xavier Picat						*/
/* description	: high level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "abe.h"

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

  resgex = & (locl_tab [oper % 64]);

  return (resgex);
  }

/* ###--------------------------------------------------------------### */
/* function	: mad_fregex						*/
/* description	: release a GEX expression (excluding terminals)	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void mad_fregex (ptgex, mode)

struct begex *ptgex;
char          mode ;

  {
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
          case ABL_OR :
            if (top_operator == ABL_NOT)
              {
              gexpnt->TERM = ABL_NOR;
              flatflag = 1;
              }
            else
              {
              if ((top_operator == ABL_OR) || (top_operator == ABL_NOR))
                flatflag = 1;
              }
            break;

          case ABL_AND :
            if (top_operator == ABL_NOT)
              {
              gexpnt->TERM = ABL_NAND;
              flatflag = 1;
              }
            else
              {
              if ((top_operator == ABL_AND) || (top_operator == ABL_NAND))
                flatflag = 1;
              }
            break;

          case ABL_XOR :
            if (top_operator == ABL_NOT)
              {
              gexpnt->TERM = ABL_NXOR;
              flatflag = 1;
              }
            else
              {
              if ((top_operator == ABL_XOR) || (top_operator == ABL_NXOR))
                flatflag = 1;
              }
            break;

          case ABL_NXOR :
            if (top_operator == ABL_XOR)
              {
              gexpnt->TERM = ABL_NXOR;
              flatflag     = 1;
              }
            else
              {
              if ((top_operator == ABL_NXOR) || (top_operator == ABL_NOT))
                {
                gexpnt->TERM = ABL_XOR;
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
            case ABL_AND :
              freflag = 2; resgex = gex_zero; break;
            case ABL_NAND :
            case ABL_NOT  :
              freflag = 2; resgex = gex_one ; break;
            case ABL_OR   :
            case ABL_XOR  :
            case ABL_NOR  :
            case ABL_NXOR :
              freflag = 1; break;
              break;
            }
          }

        if (operand == gex_one)
          {
          switch (operator)
            {
            case ABL_OR :
              freflag = 2; resgex = gex_one ; break;
            case ABL_NOR :
            case ABL_NOT :
              freflag = 2; resgex = gex_zero; break;
            case ABL_NAND :
            case ABL_AND  :
              freflag = 1; break;
              break;
            case ABL_XOR  :
              gexpnt->TERM = ABL_NXOR; freflag = 1; break;
              break;
            case ABL_NXOR :
              gexpnt->TERM = ABL_XOR ; freflag = 1; break;
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
            case ABL_AND  :
            case ABL_NOR  :
            case ABL_NXOR :
              resgex = gex_one ;
              break;
            case ABL_OR   :
            case ABL_XOR  :
            case ABL_NAND :
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
              case ABL_AND  :
              case ABL_OR   :
              case ABL_XOR  :
                resgex = (struct begex *) new_oper->DATA;
                freechain (new_oper);
                gexpnt = NULL;
                new_oper = NULL;
                break;
              case ABL_NOR  :
              case ABL_NXOR :
              case ABL_NAND :
              case ABL_NOT  :
                tmp_chn = gexpnt->OPERAND;
                gexpnt->OPERAND = new_oper;
                gexpnt->TERM    = ABL_NOT;
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
  char          delayed [128];
  static char  *str_zero   = NULL;
  static char  *str_dc     = NULL;
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
    str_dc   = namealloc ("'d'");
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
      case ABL_STABLE:

	/* ###------------------------------------------------------### */
	/*    translate signal'stable into a logical expression.	*/
	/* notice :							*/
	/*    signal'stable = not (signal xor signal'delayed)		*/
	/* ###------------------------------------------------------### */

        resgex = beh_addgexex (ABL_NXOR, 0);

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

      case ABL_OR:
      case ABL_AND:
      case ABL_XOR:
      case ABL_NOR:
      case ABL_NAND:
      case ABL_NXOR:
      case ABL_NOT:

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
        if ((expr->DATA == str_zero) || (expr->DATA == str_dc))
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

void beh_makgex (ptr_befig, aux_flg, trace_flg)

struct befig *ptr_befig;		/* befig containing expressions	*/
char          aux_flg  ;		/* ignored			*/
char          trace_flg;		/* ignored			*/

  {
  struct beaux  *ptr_beaux;
  struct beaux  *ptr_bedly;
  struct bemsg  *ptr_bemsg;
  struct beout  *ptr_beout;
  struct bebus  *ptr_bebus;
  struct bebux  *ptr_bebux;
  struct bereg  *ptr_bereg;
  struct biabl  *ptr_biabl;
  struct binode *ptr_binode;
  struct berin  *ptr_rin;
  int            i;

	/* ###------------------------------------------------------### */
	/*    initialize hash table					*/
	/* ###------------------------------------------------------### */

  i       = 1;
  ptr_rin = ptr_befig->BERIN;

  while (ptr_rin != NULL)
    {
    i++;
    ptr_rin = ptr_rin->NEXT;
    }

  HashTable = addht (i);

  i       = 2;
  ptr_rin = ptr_befig->BERIN;

  while (ptr_rin != NULL)
    {
    addhtitem (HashTable, ptr_rin->NAME, i);
    i++;
    ptr_rin = ptr_rin->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a gex for each simple internal signal		*/
	/* ###------------------------------------------------------### */

  ptr_beaux = ptr_befig->BEAUX;
  while (ptr_beaux != NULL)
    {
    ptr_beaux->NODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_beaux->ABL);
    ptr_beaux       = ptr_beaux->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a gex for each delayed internal signal		*/
	/* ###------------------------------------------------------### */

  ptr_bedly = ptr_befig->BEDLY;
  while (ptr_bedly != NULL)
    {
    ptr_bedly->NODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_bedly->ABL);
    ptr_bedly       = ptr_bedly->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a gex for each assertion				*/
	/* ###------------------------------------------------------### */

  ptr_bemsg = ptr_befig->BEMSG;
  while (ptr_bemsg != NULL)
    {
    ptr_bemsg->NODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_bemsg->ABL);
    ptr_bemsg       = ptr_bemsg->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a gex for each simple output				*/
	/* ###------------------------------------------------------### */

  ptr_beout = ptr_befig->BEOUT;
  while (ptr_beout != NULL)
    {
    ptr_beout->NODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_beout->ABL);
    ptr_beout = ptr_beout->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a gex for each bussed output				*/
	/* ###------------------------------------------------------### */

  ptr_bebus = ptr_befig->BEBUS;
  while (ptr_bebus != NULL)
    {
    ptr_biabl  = ptr_bebus->BIABL;
    ptr_binode = ptr_bebus->BINODE;

    while (ptr_biabl != NULL)
      {
      ptr_binode->CNDNODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_biabl->CNDABL);
      ptr_binode->VALNODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_biabl->VALABL);

      ptr_biabl  = ptr_biabl->NEXT;
      ptr_binode = ptr_binode->NEXT;
      }
    ptr_bebus = ptr_bebus->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a gex for each bussed internal signal		*/
	/* ###------------------------------------------------------### */

  ptr_bebux = ptr_befig->BEBUX;
  while (ptr_bebux != NULL)
    {
    ptr_biabl  = ptr_bebux->BIABL;
    ptr_binode = ptr_bebux->BINODE;

    while (ptr_biabl != NULL)
      {
      ptr_binode->CNDNODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_biabl->CNDABL);
      ptr_binode->VALNODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_biabl->VALABL);

      ptr_biabl  = ptr_biabl->NEXT;
      ptr_binode = ptr_binode->NEXT;
      }
    ptr_bebux = ptr_bebux->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a gex for each internal register			*/
	/* ###------------------------------------------------------### */

  ptr_bereg = ptr_befig->BEREG;
  while (ptr_bereg != NULL)
    {
    ptr_biabl  = ptr_bereg->BIABL;
    ptr_binode = ptr_bereg->BINODE;

    while (ptr_biabl != NULL)
      {
      ptr_binode->CNDNODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_biabl->CNDABL);
      ptr_binode->VALNODE = (bddnode *) beh_abl2gex (ptr_befig, ptr_biabl->VALABL);

      ptr_biabl  = ptr_biabl->NEXT;
      ptr_binode = ptr_binode->NEXT;
      }
    ptr_bereg = ptr_bereg->NEXT;
    }

  delht (HashTable);
  }
