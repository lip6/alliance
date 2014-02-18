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
/* file		: bvl_bspec.c						*/
/* date		: Jan 18 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET, VUONG Huu Nghia		*/
/* content	: contains all specific functions used to build		*/
/*		  behaviour data structures :				*/
/*		  bvl_addgen   , bvl_tobin , bvl_cpyabllst,		*/
/*		  bvl_cpyablstr, bvl_crtabl, bvl_select			*/
/*									*/
/* ###--------------------------------------------------------------### */

/* 13 Nov 1992 : VHN  Now when oper = EQ or NE return AND (or AND NOT)
                 of each member of the equality                         */

#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "abe.h"

typedef struct bvl_expr
  {
  char	 	*IDENT; 	/* identifier or constant name		*/
  struct chain	*LIST_ABL;	/* pointer on bvl_abllst list		*/
  short		 WIDTH;         /* width of bit vector			*/
  }
bvl_ablstr;

#include "bvl_bspec.h"
#include "bvl_bedef.h"

/* ###--------------------------------------------------------------### */
/* function	: bvl_addgen						*/
/* description	: create one or more BEGEN structures			*/
/*		  For a scalar a BEGEN is created at the head of	*/
/*		  existing BEGEN list.					*/
/*		  For an array (including arraies of one element) a set	*/
/*		  of BEGENs are created in a sorted list. BEGEN related	*/
/*		  to the index i of the array is named `name(i)`. The	*/
/*		  head of the list represents the right bound of the	*/
/*		  array. This list is then chained to the head of	*/
/*		  existing BEGEN list.					*/
/* called func.	: beh_addbegen, namealloc				*/
/* ###--------------------------------------------------------------### */

struct begen *bvl_addgen (lastgeneric,nat_lst,nam_lst,type,left,right)

struct begen *lastgeneric;	/* pointer on the last begen structure	*/
struct chain *nam_lst;		/* generic's name list			*/
struct chain *nat_lst;		/* generic's value list			*/
char         *type;		/* generic's type			*/
short         left;		/* array's left bound (= -1 if scalar)	*/
short         right;		/* array's right bound (= -1 if scalar)	*/

  {
  char          extname[100];
  char         *name;
  struct begen *ptgen;
  struct chain *ptauxnam;
  struct chain *ptauxnat;
  long           i;
  long           inc = 1;

  ptgen    = lastgeneric;
  ptauxnam = nam_lst;
  ptauxnat = nat_lst;

  if ((left == -1) && (right == -1))
    if ((ptauxnat != NULL) && (ptauxnat->NEXT == NULL))
      while (ptauxnam != NULL)
        {
        name     = namealloc((char *)ptauxnam->DATA);
        ptgen    = beh_addbegen (ptgen,name,type,(void *)ptauxnat->DATA);
        ptauxnam = ptauxnam->NEXT;
        }
    else
      bvl_error(75,NULL);
  else
    {
    if (left >= right)
      inc = -1;
    while (ptauxnam != NULL)
      {
      for (i=left ; i!=(right+inc) ; i+=inc)
        {
        sprintf (extname,"%s %ld",(char *)ptauxnam->DATA,i);
        name = namealloc(extname);
        if (ptauxnat != NULL)
          {
          ptgen    = beh_addbegen (ptgen,name,type,(void *)ptauxnat->DATA);
	  ptauxnat = ptauxnat->NEXT;
          }
        else
          bvl_error(75,NULL);
        }
      if (ptauxnat != NULL)
        bvl_error (75,NULL);
      ptauxnat = nat_lst;
      ptauxnam = ptauxnam->NEXT;
      }
    }
  return (ptgen);
  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_cpyabllst						*/
/* description	: duplicate bvl_abllst structure 			*/
/* called func.	: addchain, reverse, dupablexpr				*/
/* ###--------------------------------------------------------------### */

struct chain *bvl_cpyabllst (abllst)

struct chain *abllst;
  {
  struct chain *pt_abllst = NULL;

  while (abllst != NULL)
    {
    pt_abllst = addchain (pt_abllst,dupablexpr((struct chain *)abllst->DATA));
    abllst    = abllst->NEXT;
    }
  pt_abllst = reverse (pt_abllst);
  return (pt_abllst);
  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_cpyablstr						*/
/* description	: duplicate bvl_ablstr structure 			*/
/* called func.	: mbkalloc <mbk>, bvl_cpyabllst				*/
/* ###--------------------------------------------------------------### */

bvl_ablstr bvl_cpyablstr ( bvl_ablstr ablstr )
  {
  bvl_ablstr pt_ablstr;

  pt_ablstr.IDENT    = NULL;
  pt_ablstr.WIDTH    = ablstr.WIDTH;
  pt_ablstr.LIST_ABL = bvl_cpyabllst (ablstr.LIST_ABL);
  return (pt_ablstr);
  }

/* ###--------------------------------------------------------------### */
/* function     : bvl_crtabl                                            */
/* description  : combine at most two ABLs and build a new one          */
/*    		  The following operations can be performed :		*/
/*		    CONC    perform concatenation			*/
/*		    NOPI    initialize a structure for a signal (scalar	*/
/*		            or array)					*/
/*		    NOPS    initialize a structure for a literal	*/
/*		    NE      create a structure with an ABL representing	*/
/*		            the 'non equality' of two expressions	*/
/*		    EQ      create a structure with an ABL representing	*/
/*		            the 'equality' of two expressions		*/
/*		    NOT     perform logical not of an expression	*/
/*		    AND     perform logical and  between two expressions*/
/*		    OR      perform logical or   between two expressions*/
/*		    NAND    perform logical nand between two expressions*/
/*		    NOR     perform logical nor  between two expressions*/
/*		    XOR     perform logical xor  between two expressions*/
/*		    ANDM    perform logical and  between two expressions*/
/*		            (the second expression is a scalar)		*/
/* called func. : createablatom , createabloper, addablqexpr , beh_toolbug,	*/
/*		  bvl_error , addchain   , freechain			*/
/* ###--------------------------------------------------------------### */

bvl_ablstr bvl_crtabl ( 

short      oper,
bvl_ablstr expr1,
bvl_ablstr expr2,
long        left,
long        right )

{
  char            name[256];
  char           *name2;
  struct chain   *pt_abl1;
  struct chain   *pt_abl2;
  struct chain   *pt_aux1;
  struct chain   *pt_aux2;
  bvl_ablstr      result;
  char            lcl_buffer[256];
  long            inc;
  long            i;
#if 0
  char            true_flag_un = 0;
  char            true_flag_zero = 0;

  struct chain *abl_un    = createablatom("'1'");
  struct chain *abl_zero  = createablatom("'0'");
#endif
  result.IDENT    = NULL;
  result.LIST_ABL = NULL;
  result.WIDTH    = 0;

  switch (oper)
    {
    case CONC :
      if ((expr1.LIST_ABL == NULL) || (expr2.LIST_ABL == NULL))
        beh_toolbug (4,"bvl_crtabl",NULL,0);
      else
        {
        if (expr1.LIST_ABL == expr2.LIST_ABL)
          beh_toolbug (16,"bvl_crtabl",NULL,0);
        else
          {
          pt_aux2 = expr2.LIST_ABL;
          while (pt_aux2->NEXT != NULL)
            pt_aux2 = pt_aux2->NEXT;

          pt_aux2->NEXT = expr1.LIST_ABL;

          result.LIST_ABL = expr2.LIST_ABL;
          result.WIDTH    = expr1.WIDTH + expr2.WIDTH;

          expr1.LIST_ABL  = NULL;
          expr2.LIST_ABL  = NULL;
          }
        }
      break;

      case NOPI :
        if ( expr1.IDENT == NULL )
          beh_toolbug (2,"bvl_crtabl",NULL,0);
        else
          {
          if ((left == -1) && (right == -1))
            {
            result.LIST_ABL = addchain(result.LIST_ABL,createablatom(expr1.IDENT));
            result.WIDTH    = 1;
            }
          else
            {
            if (left <= right)
              {
              inc = 1;
              result.WIDTH = right - left + 1;
              }
            else
              {
              inc = -1;
              result.WIDTH = left - right + 1;
              }

            for (i=left ; i!=(right+inc) ; i+=inc)
              {
              sprintf (name,"%s %li",expr1.IDENT,i);
              name2           = namealloc (name);
              result.LIST_ABL = addchain (result.LIST_ABL,createablatom(name2));
              }
            }
          expr1.IDENT = NULL;
          }
        break;

      case NOPS :
        if ( expr1.IDENT == NULL )
          beh_toolbug (2,"bvl_crtabl",NULL,0);
        else
          {
          bvl_tobin (lcl_buffer,expr1.IDENT,-1,-1);
          if ((left == -1) && (right == -1))
            {
            left = 0;
            right = strlen (lcl_buffer) - 1;
            }

          for (i=left ; i<=right ; i++)
            {
            switch ( lcl_buffer[i] )
              {
              case '0' :
                result.LIST_ABL = addchain (result.LIST_ABL,createablatom("'0'"));
                break;
              case '1' :
                result.LIST_ABL = addchain (result.LIST_ABL,createablatom("'1'"));
                break;
/*-----------        Beware Not VHDL        -------------*/
              case 'd' :
                result.LIST_ABL = addchain (result.LIST_ABL,createablatom("'D'"));
                break;
              default  :
                beh_toolbug (15,"bvl_crtabl",NULL,expr1.IDENT[i]);
              }
            }
          result.WIDTH = right - left + 1;
          }
        break;

      case ABL_STABLE :
        if (expr1.LIST_ABL == NULL)
	  beh_toolbug (3,"bvl_crtabl",NULL,0);
        else
          {
	  pt_aux1  = expr1.LIST_ABL;
          while (pt_aux1 != NULL)
            {
            pt_abl1  = createabloper (ABL_STABLE);
            addablqexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
            pt_aux1->DATA = pt_abl1;
            pt_aux1  = pt_aux1->NEXT;
            }

          result.LIST_ABL = expr1.LIST_ABL;
          result.WIDTH    = expr1.WIDTH;

          expr1.LIST_ABL  = NULL;
          }
        break;

      case ABL_NOT :
        if (expr1.LIST_ABL == NULL)
	  beh_toolbug (3,"bvl_crtabl",NULL,0);
        else
          {
	  pt_aux1  = expr1.LIST_ABL;
          while (pt_aux1 != NULL)
            {
            pt_abl1  = createabloper (ABL_NOT);
            addablqexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
            pt_aux1->DATA = pt_abl1;
            pt_aux1  = pt_aux1->NEXT;
            }

          result.LIST_ABL = expr1.LIST_ABL;
          result.WIDTH    = expr1.WIDTH;

          expr1.LIST_ABL  = NULL;
          }
        break;

      case EQ :
        if ((expr1.LIST_ABL == NULL) || (expr2.LIST_ABL == NULL))
	  beh_toolbug (4,"bvl_crtabl",NULL,0);
        else
          {
          if (expr1.WIDTH != expr2.WIDTH)
            {
            bvl_error (38,NULL);
            pt_abl2 = createablatom ("'1'");

	    pt_aux1 = expr1.LIST_ABL;
            while (pt_aux1 != NULL)
              {
              freeablexpr (pt_aux1->DATA);
              pt_aux1 = pt_aux1->NEXT;
              }
	    pt_aux2 = expr2.LIST_ABL;
            while (pt_aux2 != NULL)
              {
              freeablexpr (pt_aux2->DATA);
              pt_aux2 = pt_aux2->NEXT;
              }
            }
          else
            {
	    pt_aux1 = expr1.LIST_ABL;
	    pt_aux2 = expr2.LIST_ABL;
# if 0
            /* If expr2 = '1' then return expr1 */
            while(pt_aux2 != NULL)
              {
              true_flag_un = 1;
              if(!isablequalexpr(pt_aux2->DATA,abl_un))
                {
                /* One abl not abl_un */
                true_flag_un = 0;
                break;
                }
              pt_aux2 = pt_aux2->NEXT;  
              }
            if(true_flag_un == 1)
              {
              pt_abl1 = (struct chain *)pt_aux1->DATA;
              pt_aux1 = pt_aux1->NEXT;
              if(pt_aux1 != NULL)
                {
                pt_abl2 = createabloper(ABL_AND);
                addablqexpr(pt_abl2,pt_abl1);
                pt_abl1 = pt_abl2;
                }
              while(pt_aux1 != NULL)
                {
                addablqexpr(pt_abl1,(struct chain *)pt_aux1->DATA);
                pt_aux1 = pt_aux1->NEXT;
                }
              result.LIST_ABL = addchain(result.LIST_ABL,pt_abl1);
              result.WIDTH    = 1;
              expr1.LIST_ABL  = NULL;
              expr2.LIST_ABL  = NULL;
              }
            /* If expr2 = '0' then return NOT(expr1) */
	    pt_aux2 = expr2.LIST_ABL;
            true_flag_zero = 0;
            while(pt_aux2 != NULL)
              {
              true_flag_zero = 1;
              if(!isablequalexpr(pt_aux2->DATA,abl_zero))
                {
                /* One abl not abl_zero */
                true_flag_zero = 0;
                break;
                }
              pt_aux2 = pt_aux2->NEXT;  
              }
            if(true_flag_zero == 1)
              {
              while(pt_aux1 != NULL)
                {
                pt_abl1 = createabloper(ABL_NOT);
                addablqexpr(pt_abl1,(struct chain *)pt_aux1->DATA);
                pt_aux1 = pt_aux1->NEXT;
                if(pt_aux1 != NULL)
                  {
                  pt_abl2 = createabloper(ABL_AND);
                  addablqexpr(pt_abl2,pt_abl1);
                  pt_abl1 = pt_abl2;
                  }
                while(pt_aux1 != NULL)
                  {
                  pt_abl2 = createabloper(ABL_NOT);
                  addablqexpr(pt_abl2,(struct chain *)pt_aux1->DATA);
                  addablqexpr(pt_abl1,pt_abl2);
                  pt_aux1 = pt_aux1->NEXT;
                  }
                /* --pt_abl1 = createabloper(ABL_NOT);
                addablqexpr(pt_abl1,(struct chain *)pt_aux1->DATA);
                pt_aux1->DATA = pt_abl1;
                pt_aux1       = pt_aux1->NEXT;
                --- */
                }
              result.LIST_ABL = addchain(result.LIST_ABL,pt_abl1);
              result.WIDTH    = 1;
              expr1.LIST_ABL  = NULL;
              expr2.LIST_ABL  = NULL;
              }
	    pt_aux2 = expr2.LIST_ABL;
            if((true_flag_zero == 0) && (true_flag_un == 0))
# endif
              {
              pt_abl1 = createabloper (ABL_XOR);
              addablqexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
              addablqexpr (pt_abl1, (struct chain *)pt_aux2->DATA);

              pt_aux1 = pt_aux1->NEXT;
              pt_aux2 = pt_aux2->NEXT;

	      while (pt_aux1 != NULL)
	        {
                pt_abl2 = createabloper (ABL_OR);
                addablqexpr (pt_abl2,pt_abl1);

                pt_abl1 = createabloper (ABL_XOR);
                addablqexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
                addablqexpr (pt_abl1, (struct chain *)pt_aux2->DATA);

                addablqexpr (pt_abl2, pt_abl1);
                pt_abl1 = pt_abl2;

	        pt_aux1 = pt_aux1->NEXT;
	        pt_aux2 = pt_aux2->NEXT;
	        }
              pt_abl2 = createabloper (ABL_NOT);
              addablqexpr (pt_abl2, pt_abl1);

              result.LIST_ABL = addchain (result.LIST_ABL,pt_abl2);
              result.WIDTH    = 1;
              freechain (expr1.LIST_ABL);
              freechain (expr2.LIST_ABL);
              expr1.LIST_ABL = NULL;
              expr2.LIST_ABL = NULL;
              }
            }
          }
        break;

      case NE :

        if ((expr1.LIST_ABL == NULL) || (expr2.LIST_ABL == NULL))
	  beh_toolbug (4,"bvl_crtabl",NULL,0);
        else
          {
          if (expr1.WIDTH != expr2.WIDTH)
            {
            bvl_error(38,NULL);
            pt_abl1 = createablatom ("'1'");

	    pt_aux1 = expr1.LIST_ABL;
            while (pt_aux1 != NULL)
              {
              freeablexpr (pt_aux1->DATA);
              pt_aux1 = pt_aux1->NEXT;
              }
	    pt_aux2 = expr2.LIST_ABL;
            while (pt_aux2 != NULL)
              {
              freeablexpr (pt_aux2->DATA);
              pt_aux2 = pt_aux2->NEXT;
              }
            }
          else
            {
	    pt_aux1 = expr1.LIST_ABL;
	    pt_aux2 = expr2.LIST_ABL;

# if 0
            /* If expr2 = '0' then return expr1 */
            while(pt_aux2 != NULL)
              {
              true_flag_zero = 1;
              if(!isablequalexpr(pt_aux2->DATA,abl_zero))
                {
                /* One abl not abl_zero */
                true_flag_zero = 0;
                break;
                }
              pt_aux2 = pt_aux2->NEXT;  
              }
            if(true_flag_zero == 1)
              {
              result.LIST_ABL = expr1.LIST_ABL;
              result.WIDTH    = 1;
              expr1.LIST_ABL  = NULL;
              expr2.LIST_ABL  = NULL;
              }
            /* If expr2 = '1' then return NOT(expr1) */
	    pt_aux2 = expr2.LIST_ABL;
            true_flag_un = 0;
            while(pt_aux2 != NULL)
              {
              true_flag_un = 1;
              if(!isablequalexpr(pt_aux2->DATA,abl_un))
                {
                /* One abl not abl_un */
                true_flag_un = 0;
                break;
                }
              pt_aux2 = pt_aux2->NEXT;  
              }
            if(true_flag_un == 1)
              {
              while(pt_aux1 != NULL)
                {
                pt_abl1 = createabloper(ABL_NOT);
                addablqexpr(pt_abl1,(struct chain *)pt_aux1->DATA);
                pt_aux1->DATA = pt_abl1;
                pt_aux1       = pt_aux1->NEXT;
                }
              result.LIST_ABL = expr1.LIST_ABL;
              result.WIDTH    = 1;
              expr1.LIST_ABL  = NULL;
              expr2.LIST_ABL  = NULL;
              }
               
              
	    pt_aux2 = expr2.LIST_ABL;
            if((true_flag_zero == 0) && (true_flag_un == 0))
# endif
              {
              pt_abl1 = createabloper (ABL_XOR);
              addablqexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
              addablqexpr (pt_abl1, (struct chain *)pt_aux2->DATA);
  
              pt_aux1 = pt_aux1->NEXT;
              pt_aux2 = pt_aux2->NEXT;

  	      for (i=2 ; i<=expr1.WIDTH ; i++)
	        {
                pt_abl2 = createabloper (ABL_OR);
                addablqexpr (pt_abl2, pt_abl1);

                pt_abl1 = createabloper (ABL_XOR);
                addablqexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
                addablqexpr (pt_abl1, (struct chain *)pt_aux2->DATA);

                addablqexpr (pt_abl2, pt_abl1);
                pt_abl1 = pt_abl2;

	        pt_aux1 = pt_aux1->NEXT;
	        pt_aux2 = pt_aux2->NEXT;
	        }
              result.LIST_ABL = addchain(result.LIST_ABL,pt_abl1);
              result.WIDTH    = 1;
              freechain (expr1.LIST_ABL);
              freechain (expr2.LIST_ABL);
              expr1.LIST_ABL = NULL;
              expr2.LIST_ABL = NULL;
              }
            }
          }
        break;

      case ABL_AND  :
      case ABL_NAND :
      case ABL_OR   :
      case ABL_NOR  :
      case ABL_XOR  :

        if (expr1.LIST_ABL == NULL)
          {
          if (expr2.LIST_ABL == NULL)
	    beh_toolbug (4,"bvl_crtabl",NULL,0);
          else
            {
            result.LIST_ABL = expr2.LIST_ABL;
            result.WIDTH    = expr2.WIDTH;
            expr2.LIST_ABL  = NULL;
            }
          }
        else
          {
          if (expr2.LIST_ABL == NULL)
            {
            result.LIST_ABL = expr1.LIST_ABL;
            result.WIDTH    = expr1.WIDTH;
            expr1.LIST_ABL  = NULL;
            }
          else
            {
            if (expr1.LIST_ABL == expr2.LIST_ABL)
              beh_toolbug (16,"bvl_crtabl",NULL,0);
            else
              {
              if (expr1.WIDTH != expr2.WIDTH)
                {
                bvl_error(38,NULL);
                }
              else
                {
	        pt_aux1 = expr1.LIST_ABL;
	        pt_aux2 = expr2.LIST_ABL;

	        for (i=1 ; i<=expr1.WIDTH ; i++)
	          {
                  pt_abl1 = createabloper (oper);
                  addablqexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
                  addablqexpr (pt_abl1, (struct chain *)pt_aux2->DATA);

                  pt_aux1->DATA = (void *)pt_abl1;

	          pt_aux1       = pt_aux1->NEXT;
	          pt_aux2       = pt_aux2->NEXT;
		  }
                }
              result.LIST_ABL = expr1.LIST_ABL;
              result.WIDTH    = expr1.WIDTH;
              freechain (expr2.LIST_ABL);
              expr1.LIST_ABL  = NULL;
              expr2.LIST_ABL  = NULL;
              }
            }
          }
        break;

      case ANDM :
        if ((expr1.LIST_ABL == NULL) || (expr2.LIST_ABL == NULL))
	    beh_toolbug (4,"bvl_crtabl",NULL,0);
        else
          {
          if (expr2.WIDTH != 1)
            {
            bvl_error( 38,NULL);
            }
          else
            {
            pt_aux1 = expr1.LIST_ABL;
            pt_aux2 = expr2.LIST_ABL;
            while (pt_aux1 != NULL)
              {
              pt_abl1 = createabloper (ABL_AND);
              addablqexpr (pt_abl1,          (struct chain *)pt_aux1->DATA);
              addablqexpr (pt_abl1, dupablexpr((struct chain *)pt_aux2->DATA));
              pt_aux1->DATA = (void *)pt_abl1;

              pt_aux1       = pt_aux1->NEXT;
              }
            }
          result.LIST_ABL = expr1.LIST_ABL;
          result.WIDTH    = expr1.WIDTH;
	  pt_aux2 = expr2.LIST_ABL;
          while (pt_aux2 != NULL)
            {
            freeablexpr (pt_aux2->DATA);
            pt_aux2 = pt_aux2->NEXT;
            }
          freechain (expr2.LIST_ABL);
          expr2.LIST_ABL  = NULL;
          expr1.LIST_ABL  = NULL;
          }
        break;

      default :
	beh_toolbug (1,"bvl_crtabl",NULL,0);
      }

    return (result);
}

/* ###--------------------------------------------------------------### */
/* function	: bvl_select						*/
/* description	: create an abl representing the choice in a selected	*/
/*		  signal assignment and perform unicity verification	*/
/*		  using BDDs.						*/
/* called func.	: bvl_tobin   , convertlogbddnode     , addInputCct      ,	*/
/*		  substPhyExpr, initializeCct, getlogbddnodeterm,	*/
/*		  bvl_error   , getlogbddnodenot       , applylogbddnode      ,	*/
/*		  addchain    , namealloc				*/
/* ###--------------------------------------------------------------### */

extern bddcircuit *BvlBddCircuit;

void bvl_select (

bvl_ablstr   *result,
struct chain *pt_str,		/* pointer on a list of choices		*/
bddnode     **pt_bdd,		/* used to check if a choice is legal	*/
bvl_ablstr   pt_ablstr )	/* tested expression			*/

  {
  char             binstr[256];
  long              i;
  struct chain    *pt_auxabl;
  bddnode         *pt_bddres;
  bddnode         *pt_bddnew;
  bddnode         *pt_bddtmp;
  bddnode         *pt_bddaux;
  char             nomvar[10];
  struct chain    *pt_newabl;
  struct chain    *pt_newabl2;
  static long       oth_flg=0;
  static long       last_width=0;

  result->LIST_ABL = NULL;
  result->IDENT    = NULL;

  pt_bddtmp = getlogbddnodezero();

  if (*pt_bdd == NULL)
    {
    if (last_width < pt_ablstr.WIDTH)
      {
      for (; last_width<pt_ablstr.WIDTH ; last_width++)
        {
        sprintf (nomvar,"(%ld)",last_width);
        addlogbddcircuitin(BvlBddCircuit,nomvar);
        }
      }
    *pt_bdd = getlogbddnodezero();
    oth_flg = 0;
    }

  while (pt_str != NULL)
    {
    bvl_tobin (binstr, (char *)pt_str->DATA,-1,-1);
    if (oth_flg != 0)
      {
      bvl_error(30,NULL);
      }

    if (strcmp("others", (char *)pt_str->DATA))
      {
      pt_bddres = getlogbddnodeone();

      if (strlen (binstr) != pt_ablstr.WIDTH)
        {
        bvl_error(38,NULL);
        }
      for (i=0 ; binstr[i]!='\0' ; i++)
        {
        pt_bddaux = getlogbddnodeterm(i+2);
        if (binstr[i] == '0')
          pt_bddaux = getlogbddnodenot (pt_bddaux);
        pt_bddres = applylogbddnode (ABL_AND, pt_bddaux, pt_bddres);
        }
      pt_bddnew = applylogbddnode (ABL_OR,*pt_bdd,pt_bddres);

      if (*pt_bdd == pt_bddnew)
         {
         bvl_error(28,NULL);
         }

      *pt_bdd = pt_bddnew;
      }
    else
      {
      oth_flg   = 1;
      pt_bddres = getlogbddnodenot (*pt_bdd);
      *pt_bdd   = getlogbddnodeone();
      }
    pt_bddtmp = applylogbddnode (ABL_OR,pt_bddtmp,pt_bddres);
    pt_str = pt_str->NEXT;
    }

  pt_newabl = convertlogbddnode (BvlBddCircuit, pt_bddtmp);

  pt_auxabl = pt_ablstr.LIST_ABL;
  i         = pt_ablstr.WIDTH - 1;
  while (pt_auxabl != NULL)
    {
    sprintf (nomvar,"(%li)",i);
/*--------
    substPhyExpr (pt_newabl,namealloc(nomvar),(struct chain *)pt_auxabl->DATA);
---------*/
    pt_newabl2 = substdupablexpr (pt_newabl,namealloc(nomvar),
                            (struct chain *)pt_auxabl->DATA);
    freeablexpr (pt_newabl);
    pt_newabl = pt_newabl2;

    i--;
    pt_auxabl = pt_auxabl->NEXT;
    }
  result->LIST_ABL = addchain (NULL, pt_newabl);
  result->WIDTH    = 1;

  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_tobin						*/
/* description	: transform a StringLit, BitStringLit or CharacterLit	*/
/*		  in a string of '0' and '1's				*/
/* ###--------------------------------------------------------------### */

long bvl_tobin (

char *trg,
char *src,
long   left,
long   right )

{
  char base;
  long  indx;
  long  j = 0;
  long  errflg = 0;
  char lcl_trg[256];

  lcl_trg[0] = '\0';

  if (src == NULL)
    {
    strcpy (trg,"0");
    }
  else
    {
    if (!strcmp (src,"others"))
      {
      strcpy(trg,src);
      }
    else
      {
      if ((src[0] != '\'') && (src[0] != '"') && (src[0] != '%'))
        {
        base = src[0];
        indx = 2;
        }
      else
        {
        base = 'B';
        indx = 1;
        }

      switch (base)
        {
        case 'B' :
        case 'b' :
          while ((lcl_trg[j] = src[indx]) != '\0')
            {
            switch (src[indx])
              {
              case '0':
              case '1':
              case 'd':				/* Beware Not VHDL	*/
                j++; break;

              case '%' :
              case '"' :
              case '\'':
              case '_' :
                break;

	    default :
	      errflg = 1; bvl_error(73,src);
              }
            indx++;
            }
          break;

        case 'O' :
        case 'o' :
          while (src[indx] != '\0')
            {
            j += 3;
            switch (src[indx])
              {
              case '0' :
                strcat (lcl_trg,"000"); break;
              case '1' :
                strcat (lcl_trg,"001"); break;
              case '2' :
                strcat (lcl_trg,"010"); break;
              case '3' :
                strcat (lcl_trg,"011"); break;
              case '4' :
                strcat (lcl_trg,"100"); break;
              case '5' :
                strcat (lcl_trg,"101"); break;
              case '6' :
                strcat (lcl_trg,"110"); break;
              case '7' :
                strcat (lcl_trg,"111"); break;
              case '"' :
              case '%' :
              case '_' :
                j -= 3; break;
	      default :
	        j -= 3; errflg = 1; bvl_error(73,src);
              }
            indx++;
            }
          break;

        case 'X' :
        case 'x' :
          while (src[indx] != '\0')
            {
            j += 4;
            switch (src[indx])
              {
              case '0' :
                strcat (lcl_trg,"0000"); break;
              case '1' :
                strcat (lcl_trg,"0001"); break;
              case '2' :
                strcat (lcl_trg,"0010"); break;
              case '3' :
                strcat (lcl_trg,"0011"); break;
              case '4' :
                strcat (lcl_trg,"0100"); break;
              case '5' :
                strcat (lcl_trg,"0101"); break;
              case '6' :
                strcat (lcl_trg,"0110"); break;
              case '7' :
                strcat (lcl_trg,"0111"); break;
              case '8' :
                strcat (lcl_trg,"1000"); break;
              case '9' :
                strcat (lcl_trg,"1001"); break;
              case 'a' :
              case 'A' :
                strcat (lcl_trg,"1010"); break;
              case 'b' :
              case 'B' :
                strcat (lcl_trg,"1011"); break;
              case 'c' :
              case 'C' :
                strcat (lcl_trg,"1100"); break;
              case 'd' :
              case 'D' :
                strcat (lcl_trg,"1101"); break;
              case 'e' :
              case 'E' :
                strcat (lcl_trg,"1110"); break;
              case 'f' :
              case 'F' :
                strcat (lcl_trg,"1111"); break;
              case '%' :
              case '"' :
              case '_' :
                j -= 4; break;
	      default :
	        j -= 4; errflg = 1; bvl_error(73,src);
              }
            indx++;
            }
          break;

        default :
          beh_toolbug (17,"bvl_tobin",NULL,base);
        }

      if ((j == 0) || (j <= right))
        {
        trg[0] = '0';
        trg[1] = '\0';
        }
      else
        {
        if (left != -1)
          {
          strcpy (trg, &lcl_trg[left]);
          trg[right - left + 1] = '\0';
          }
        else
          strcpy (trg, lcl_trg);
        }
      }
    }

  return (errflg);
  }
