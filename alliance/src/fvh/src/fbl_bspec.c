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
/*                                     */
/* file    : fbl_bspec.c                        */
/* date    : Nouv 05 1992                        */
/* version  : v100                            */
/* authors  : M.HANAFI,Pirouz BAZARGAN SABET              */
/* content  : contains all specific functions used to build        */
/*      fbhaviour data structures :                  */
/*      fbl_addgen   , fbl_tobin , fbl_stostr, fbl_cpyabllst,      */
/*      fbl_cpyablstr, fbl_crtabl, fbl_select              */
/*                                    */
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "fvh.h"

#include "fbl_bspec.h"
#include "fbl_bedef.h"

/* ###--------------------------------------------------------------### */
/* function  : fbl_addgen            */
/* description  : create one or more BEGEN structures      */
/*      For a scalar a BEGEN is created at the head of  */
/*      existing BEGEN list.          */
/*      For an array (including arraies of one element) a set  */
/*      of BEGENs are created in a sorted list. BEGEN related  */
/*      to the index i of the array is named `name(i)`. The  */
/*      head of the list represents the right bound of the  */
/*      array. This list is then chained to the head of  */
/*      existing BEGEN list.          */
/* called func.  : fbh_addfbgen, namealloc        */
/* ###--------------------------------------------------------------### */

struct fbgen *fbl_addgen (lastgeneric,nat_lst,nam_lst,type,left,right)

struct fbgen *lastgeneric;  /* pointer on the last fbgen structure  */
struct chain *nam_lst;    /* generic's name list      */
struct chain *nat_lst;    /* generic's value list      */
char         *type;    /* generic's type      */
short         left;    /* array's left bound (= -1 if scalar)  */
short         right;    /* array's right bound (= -1 if scalar)  */

  {
  char          extname[100];
  char         *name;
  struct fbgen *ptgen;
  struct chain *ptauxnam;
  struct chain *ptauxnat;
  int           i;
  int           inc = 1;

  ptgen    = lastgeneric;
  ptauxnam = nam_lst;
  ptauxnat = nat_lst;

  if ((left == -1) && (right == -1))
    if ((ptauxnat != 0) && (ptauxnat->NEXT == 0))
      while (ptauxnam != 0)
        {
        name     = namealloc((char *)ptauxnam->DATA);
        ptgen    = fbh_addfbgen (ptgen,name,type,(void *)ptauxnat->DATA);
        ptauxnam = ptauxnam->NEXT;
        }
    else
    {
      fbl_error(75,0);
    }
  else
    {
    if (left >= right)
      inc = -1;
    while (ptauxnam != 0)
      {
      for (i=left ; i!=(right+inc) ; i+=inc)
        {
        sprintf (extname,"%s %d",(char *)ptauxnam->DATA,i);
        name = namealloc(extname);
        if (ptauxnat != 0)
          {
          ptgen    = fbh_addfbgen (ptgen,name,type,(void *)ptauxnat->DATA);
    ptauxnat = ptauxnat->NEXT;
          }
        else
        {
          fbl_error(75,0);
        }
        }
      if (ptauxnat != 0)
      {
        fbl_error (75,0);
      }
      ptauxnat = nat_lst;
      ptauxnam = ptauxnam->NEXT;
      }
    }
  return (ptgen);
  }

/* ###--------------------------------------------------------------### */
/* function  : fbl_stostr            */
/* description  : purge double % (percent) or Quote (") from string  */
/* called func.  : mbkalloc            */
/* ###--------------------------------------------------------------### */

char *fbl_stostr(str)

char *str;

  {
  char *resstr;
  int   i;
  int   j=0;

  resstr = (char *)mbkalloc(100);
  resstr[0] = '\0';

  for ( i=1;str[i+1]!='\0';i++)
    {
    resstr[j++]=str[i];
    if ((str[i]==str[0])&&(str[i+1]==str[0]))
      i++;
    }
    
  return( &resstr[0] );
  }

/* ###--------------------------------------------------------------### */
/* function  : fbl_cpyabllst            */
/* description  : duplicate fbl_abllst structure       */
/* called func.  : addchain, reverse, dupablexpr        */
/* ###--------------------------------------------------------------### */

struct chain *fbl_cpyabllst (abllst)

struct chain *abllst;
  {
  struct chain *pt_abllst = 0;

  while (abllst != 0)
    {
    pt_abllst = addchain (pt_abllst,dupablexpr((struct chain *)abllst->DATA));
    abllst    = abllst->NEXT;
    }
  pt_abllst = reverse (pt_abllst);
  return (pt_abllst);
  }

/* ###--------------------------------------------------------------### */
/* function  : fbl_cpyablstr            */
/* description  : duplicate fbl_ablstr structure       */
/* called func.  : mbkalloc <mbk>, fbl_cpyabllst        */
/* ###--------------------------------------------------------------### */

fbl_ablstr fbl_cpyablstr (ablstr)

fbl_ablstr ablstr;
  {
  fbl_ablstr pt_ablstr;

  pt_ablstr.IDENT    = 0;
  pt_ablstr.WIDTH    = ablstr.WIDTH;
  pt_ablstr.TYPE    = ablstr.TYPE;
  pt_ablstr.LIST_ABL = fbl_cpyabllst (ablstr.LIST_ABL);
  return (pt_ablstr);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbl_crtabl                                            */
/* description  : combine at most two ABLs and build a new one          */
/*          The following operations can be performed :    */
/*        CONC    perform concatenation      */
/*        NOPI    initialize a structure for a signal (scalar  */
/*                or array)          */
/*        NOPS    initialize a structure for a literal  */
/*        NE      create a structure with an ABL representing  */
/*                the 'non equality' of two expressions  */
/*        EQ      create a structure with an ABL representing  */
/*                the 'equality' of two expressions    */
/*        NOT     perform logical not of an expression  */
/*        AND     perform logical and  between two expressions*/
/*        OR      perform logical or   between two expressions*/
/*        NAND    perform logical nand between two expressions*/
/*        NOR     perform logical nor  between two expressions*/
/*        XOR     perform logical xor  between two expressions*/
/*        ANDM    perform logical and  between two expressions*/
/*                (the second expression is a scalar)    */
/* called func. : createablatom , createabloper, addablhexpr , fbl_toolbug,  */
/*      fbl_error , addchain   , freechain      */
/* ###--------------------------------------------------------------### */

fbl_ablstr fbl_crtabl ( oper, expr1, expr2, left, right )

short      oper;
fbl_ablstr expr1;
fbl_ablstr expr2;
int        left;
int        right;

{
  char            name[256];
  char           *name2;
  struct chain   *pt_abl1;
  struct chain   *pt_abl2;
  struct chain   *pt_aux1;
  struct chain   *pt_aux2;
  fbl_ablstr      result;
  char            lcl_buffer[256];
  short           inc;
  short           i;

  result.IDENT    = 0;
  result.LIST_ABL = 0;
  result.WIDTH    = 0;
  result.TYPE    = 0;

  switch (oper)
    {
    case CONC :
      if ((expr1.LIST_ABL == 0) || (expr2.LIST_ABL == 0))
        fbl_toolbug (4,"fbl_crtabl","CONC",0);
      else
        {
        if (expr1.LIST_ABL == expr2.LIST_ABL)
          fbl_toolbug (16,"fbl_crtabl",0,0);
        else
          {
          pt_aux2 = expr2.LIST_ABL;
          while (pt_aux2->NEXT != 0)
            pt_aux2 = pt_aux2->NEXT;

          pt_aux2->NEXT = expr1.LIST_ABL;

          result.LIST_ABL = expr2.LIST_ABL;
          result.WIDTH    = expr1.WIDTH + expr2.WIDTH;
    result.TYPE    = expr1.TYPE;
          expr1.LIST_ABL  = 0;
          expr2.LIST_ABL  = 0;
          }
        }
      break;

      case SUBVAR :
        if ( expr1.LIST_ABL == 0 )
          fbl_toolbug (2,"fbl_crtabl",0,0);
        else
          {
          if ((left == -1) && (right == -1))
            {
      result.IDENT    = 0;
            result.LIST_ABL = addchain (result.LIST_ABL,dupablexpr((struct chain*)(expr1.LIST_ABL->DATA)));
            result.WIDTH    = 1;
      result.TYPE     = expr1.TYPE;
            }
          else
            {
    struct chain *  abl = expr1.LIST_ABL ;
    
            for (i=0 ; i< left ; i++)
              {
    if( abl )
         abl = abl->NEXT ;  
    else 
          fbl_toolbug (2,"fbl_crtabl",0,0);
              }
            for (i=left ; i<= right ; i++)
              {
    if( abl )
    {
              result.LIST_ABL = addchain (result.LIST_ABL,dupablexpr((struct chain*)(abl->DATA)));
         abl = abl->NEXT ;  
    }
    else 
          fbl_toolbug (2,"fbl_crtabl",0,0);
              }
            }
    result.IDENT    = 0;
    result.TYPE    = expr1.TYPE;
          result.WIDTH = right - left + 1;
          }
        break;

      case NOPI :
        if ( expr1.IDENT == 0 )
          fbl_toolbug (2,"fbl_crtabl",0,0);
        else
          {
          if ((left == -1) && (right == -1))
            {
            result.LIST_ABL = addchain(result.LIST_ABL,createablatom(expr1.IDENT));
            result.WIDTH    = 1;
    result.TYPE    = expr1.TYPE;
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
              sprintf (name,"%s %i",expr1.IDENT,i);
              name2           = namealloc (name);
              result.LIST_ABL = addchain (result.LIST_ABL,createablatom(name2));
              }
            }
    result.TYPE    = expr1.TYPE;
          expr1.IDENT = 0;
          }
        break;

      case NOPS :
        if ( expr1.IDENT == 0 )
          fbl_toolbug (2,"fbl_crtabl",0,0);
        else
          {
          fbl_tobin (lcl_buffer,expr1.IDENT,-1,-1);
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
                result.LIST_ABL = addchain (result.LIST_ABL,createablatomzero());
                break;
              case '1' :
                result.LIST_ABL = addchain (result.LIST_ABL,createablatomone());
                break;
/*-----------        Beware Not VHDL        -------------*/
              case 'd' :
              case '-' :
                result.LIST_ABL = addchain (result.LIST_ABL,createablatomdc());
                break;
              default  :
                fbl_toolbug (15,"fbl_crtabl",0,expr1.IDENT[i]);
              }
            }
          result.WIDTH = right - left + 1;
          }
    result.TYPE    = expr1.TYPE;
        break;

      case ABL_STABLE :
        if (expr1.LIST_ABL == 0)
    fbl_toolbug (3,"fbl_crtabl",0,0);
        else
          {
    pt_aux1  = expr1.LIST_ABL;
          while (pt_aux1 != 0)
            {
            pt_abl1  = createabloper (ABL_STABLE);
            addablhexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
            pt_aux1->DATA = (void *)pt_abl1;
            pt_aux1  = pt_aux1->NEXT;
            }

          result.LIST_ABL = expr1.LIST_ABL;
          result.WIDTH    = expr1.WIDTH;
          result.TYPE    = expr1.TYPE;

          expr1.LIST_ABL  = 0;
          }
        break;

      case ABL_NOT :
        if (expr1.LIST_ABL == 0)
    fbl_toolbug (3,"fbl_crtabl",0,0);
        else
          {
    pt_aux1  = expr1.LIST_ABL;
          while (pt_aux1 != 0)
            {
            pt_abl1  = createabloper (ABL_NOT);
            addablhexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
            pt_aux1->DATA = (void *)pt_abl1;
            pt_aux1  = pt_aux1->NEXT;
            }

          result.LIST_ABL = expr1.LIST_ABL;
          result.WIDTH    = expr1.WIDTH;
    result.TYPE    = expr1.TYPE;

          expr1.LIST_ABL  = 0;
          }
        break;

      case EQ :
        if ((expr1.LIST_ABL == 0) || (expr2.LIST_ABL == 0))
    fbl_toolbug (4,"fbl_crtabl","EQ",0);
        else
          {
    result.TYPE    = expr1.TYPE;
          if (expr1.WIDTH != expr2.WIDTH  || (expr1.TYPE != expr2.TYPE && expr2.TYPE !=-1))
            {
            fbl_error (38,0);
            pt_abl2 = createablatomone();

      pt_aux1 = expr1.LIST_ABL;
            while (pt_aux1 != 0)
              {
              delablexpr (pt_aux1->DATA);
              pt_aux1 = pt_aux1->NEXT;
              }
      pt_aux2 = expr2.LIST_ABL;
            while (pt_aux2 != 0)
              {
              delablexpr (pt_aux2->DATA);
              pt_aux2 = pt_aux2->NEXT;
              }
            }
          else
            {
      pt_aux1 = expr1.LIST_ABL;
      pt_aux2 = expr2.LIST_ABL;

            pt_abl1 = createabloper (ABL_XOR);
            addablhexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
            addablhexpr (pt_abl1, (struct chain *)pt_aux2->DATA);

            pt_aux1 = pt_aux1->NEXT;
            pt_aux2 = pt_aux2->NEXT;

      while (pt_aux1 != 0)
        {
              pt_abl2 = createabloper (ABL_OR);
              addablhexpr (pt_abl2,pt_abl1);

              pt_abl1 = createabloper (ABL_XOR);
              addablhexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
              addablhexpr (pt_abl1, (struct chain *)pt_aux2->DATA);

              addablhexpr (pt_abl2, pt_abl1);
              pt_abl1 = pt_abl2;

        pt_aux1 = pt_aux1->NEXT;
        pt_aux2 = pt_aux2->NEXT;
        }
            pt_abl2 = createabloper (ABL_NOT);
            addablhexpr (pt_abl2, pt_abl1);

            }
          result.LIST_ABL = addchain (result.LIST_ABL,pt_abl2);
          result.WIDTH    = 1;
    result.TYPE    = expr1.TYPE;
          freechain (expr1.LIST_ABL);
          freechain (expr2.LIST_ABL);
          expr1.LIST_ABL = 0;
          expr2.LIST_ABL = 0;
          }
        break;

      case NE :

        if ((expr1.LIST_ABL == 0) || (expr2.LIST_ABL == 0))
    fbl_toolbug (4,"fbl_crtabl",0,0);
        else
          {
    result.TYPE    = expr1.TYPE;
          if (expr1.WIDTH != expr2.WIDTH  || (expr1.TYPE != expr2.TYPE && expr2.TYPE !=-1))
            {
            fbl_error(38,0);
            pt_abl1 = createablatomone();

      pt_aux1 = expr1.LIST_ABL;
            while (pt_aux1 != 0)
              {
              delablexpr (pt_aux1->DATA);
              pt_aux1 = pt_aux1->NEXT;
              }
      pt_aux2 = expr2.LIST_ABL;
            while (pt_aux2 != 0)
              {
              delablexpr (pt_aux2->DATA);
              pt_aux2 = pt_aux2->NEXT;
              }
            }
          else
            {
      pt_aux1 = expr1.LIST_ABL;
      pt_aux2 = expr2.LIST_ABL;

            pt_abl1 = createabloper (ABL_XOR);
            addablhexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
            addablhexpr (pt_abl1, (struct chain *)pt_aux2->DATA);

            pt_aux1 = pt_aux1->NEXT;
            pt_aux2 = pt_aux2->NEXT;

      for (i=2 ; i<=expr1.WIDTH ; i++)
        {
              pt_abl2 = createabloper (ABL_OR);
              addablhexpr (pt_abl2, pt_abl1);

              pt_abl1 = createabloper (ABL_XOR);
              addablhexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
              addablhexpr (pt_abl1, (struct chain *)pt_aux2->DATA);
              addablhexpr (pt_abl2, pt_abl1);
              pt_abl1 = pt_abl2;

        pt_aux1 = pt_aux1->NEXT;
        pt_aux2 = pt_aux2->NEXT;
        }

            }
          result.LIST_ABL = addchain(result.LIST_ABL,pt_abl1);
    result.TYPE    = expr1.TYPE;
          result.WIDTH    = 1;
          freechain (expr1.LIST_ABL);
          freechain (expr2.LIST_ABL);
          expr1.LIST_ABL = 0;
          expr2.LIST_ABL = 0;
          }
        break;

      case ABL_AND  :
      case ABL_NAND :
      case ABL_OR   :
      case ABL_NOR  :
      case ABL_XOR  :

        if (expr1.LIST_ABL == 0)
          {
          if (expr2.LIST_ABL == 0)
      fbl_toolbug (4,"fbl_crtabl",0,0);
          else
            {
            result.LIST_ABL = expr2.LIST_ABL;
            result.WIDTH    = expr2.WIDTH;
    result.TYPE    = expr2.TYPE;
            expr2.LIST_ABL  = 0;
            }
          }
        else
          {
          if (expr2.LIST_ABL == 0)
            {
            result.LIST_ABL = expr1.LIST_ABL;
            result.WIDTH    = expr1.WIDTH;
    result.TYPE    = expr1.TYPE;
            expr1.LIST_ABL  = 0;
            }
          else
            {
            if (expr1.LIST_ABL == expr2.LIST_ABL)
              fbl_toolbug (16,"fbl_crtabl",0,0);
            else
              {
                /* LUDO
              if ((expr1.WIDTH != expr2.WIDTH)
     || (expr1.TYPE != expr2.TYPE && expr2.TYPE !=-1)) */
              if (expr1.WIDTH != expr2.WIDTH)
              {
                fbl_error(38,0);
              }
              else
                {
          pt_aux1 = expr1.LIST_ABL;
          pt_aux2 = expr2.LIST_ABL;

          for (i=1 ; i<=expr1.WIDTH ; i++)
            {
                  pt_abl1 = createabloper (oper);
                  addablhexpr (pt_abl1, (struct chain *)pt_aux1->DATA);
                  addablhexpr (pt_abl1, (struct chain *)pt_aux2->DATA);
                  pt_aux1->DATA = (void *)pt_abl1;

            pt_aux1       = pt_aux1->NEXT;
            pt_aux2       = pt_aux2->NEXT;
      }
                }
              result.LIST_ABL = expr1.LIST_ABL;
              result.WIDTH    = expr1.WIDTH;
              result.TYPE    = expr1.TYPE;
              freechain (expr2.LIST_ABL);
              expr1.LIST_ABL  = 0;
              expr2.LIST_ABL  = 0;
              }
            }
          }
        break;

      case ANDM :
        if ((expr1.LIST_ABL == 0) || (expr2.LIST_ABL == 0))
        {
          fbl_toolbug (4,"fbl_crtabl",0,0);
        }
        else
          {
          if (expr2.WIDTH != 1)
          {
            fbl_error(38,0);
          }
          else
            {
            pt_aux1 = expr1.LIST_ABL;
            pt_aux2 = expr2.LIST_ABL;
            while (pt_aux1 != 0)
              {
              pt_abl1 = createabloper (ABL_AND);
              addablhexpr (pt_abl1,          (struct chain *)pt_aux1->DATA);
              addablhexpr (pt_abl1, dupablexpr((struct chain *)pt_aux2->DATA));
              pt_aux1->DATA = (void *)pt_abl1;

              pt_aux1       = pt_aux1->NEXT;
              }
            }
          result.LIST_ABL = expr1.LIST_ABL;
          result.WIDTH    = expr1.WIDTH;
          result.TYPE    = expr1.TYPE;
    pt_aux2 = expr2.LIST_ABL;
          while (pt_aux2 != 0)
            {
            delablexpr (pt_aux2->DATA);
            pt_aux2 = pt_aux2->NEXT;
            }
          freechain (expr2.LIST_ABL);
          expr2.LIST_ABL  = 0;
          expr1.LIST_ABL  = 0;
          }
        break;

      default :

  fbl_toolbug (1,"fbl_crtabl",0,0);
      }

    return (result);
}


/* ###--------------------------------------------------------------### */
/* function  : fbl_numtobin            */
/* description  : transform a ENUMERATE     */
/*      in a string of '0' and '1's        */
/* ###--------------------------------------------------------------### */
int fbl_bintonum(str)
char* str;

  {
  int res,i=0;

   res = 0; 
  while(str[i])
  {
  if(str[i] <'0' || str[i] > '1')
    return(-1);
         res = res*2 + str[i] -'0';
    i++;
  }
  return(res);
  }
/* ###--------------------------------------------------------------### */
/* function  : fbl_numtobin            */
/* description  : transform a ENUMERATE     */
/*      in a string of '0' and '1's        */
/* ###--------------------------------------------------------------### */
char *fbl_numtobin(num)
int num;

  {
   char val[256]; 
   char *str;
  int res,i=1;

   val[255] = '\0';
   res = num; 
  while(res)
  {
          val[255 - i] = res%2 + '0';
    res = (res / 2);
    i++;
  }
   val[255 - i] = '0';
    str = namealloc((char *)(val + 255-i));
  return(str);
  }
/* ###--------------------------------------------------------------### */
/* function  : fbl_tobin            */
/* description  : transform a StringLit, BitStringLit or CharacterLit  */
/*      in a string of '0' and '1's        */
/* ###--------------------------------------------------------------### */

int fbl_tobin (trg,src,left,right)

char *trg;
char *src;
int   left;
int   right;

  {
  char base;
  int  indx;
  int  j = 0;
  int  errflg = 0;
  char lcl_trg[256];

  lcl_trg[0] = '\0';

  if (src == 0)
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
              case 'd':        /* Beware Not VHDL  */
              case '-':
                j++; break;

              case '%' :
              case '"' :
              case '\'':
              case '_' :
                break;

      default :
        errflg = 1;
        fbl_error(73,src);
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
          j -= 3; errflg = 1; fbl_error(73,src);
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
          j -= 4; errflg = 1; fbl_error(73,src);
              }
            indx++;
            }
          break;

        default :
          fbl_toolbug (17,"fbl_tobin",0,base);
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


/* ###--------------------------------------------------------------### */
/* function	: fbl_select						*/
/* description	: create an abl representing the choice in a selected	*/
/*		  signal assignment and perform unicity verification	*/
/*		  using BDDs.						*/
/* called func.	: fbl_tobin   , convertlogbddnode     , addInputCct      ,	*/
/*		  substPhyExpr, initializeCct, getlogbddnodeterm,	*/
/*		  fbl_error   , getlogbddnodenot       , applylogbddnode      ,	*/
/*		  addchain    , namealloc				*/
/* ###--------------------------------------------------------------### */

extern bddcircuit *FblBddCircuit;

void fbl_select ( result,pt_str, pt_bdd, pt_ablstr)
fbl_ablstr   *result;
struct chain *pt_str;		/* pointer on a list of choices		*/
bddnode     **pt_bdd;		/* used to check if a choice is legal	*/
fbl_ablstr  *pt_ablstr;		/* tested expression			*/
{
  char             binstr[512];
  int              i;
  struct chain    *pt_auxabl;
  bddnode         *pt_bddres;
  bddnode         *pt_bddnew;
  bddnode         *pt_bddtmp;
  bddnode         *pt_bddaux;
  char             nomvar[10];
  struct chain    *pt_newabl;
  struct chain    *pt_newabl2;
  static int       oth_flg=0;
  static int       last_width=0;

  result->LIST_ABL = NULL;
  result->IDENT    = NULL;

  pt_bddtmp = getlogbddnodezero();

  if (*pt_bdd == NULL)
    {
    if (last_width < pt_ablstr->WIDTH)
      {
      for (; last_width<pt_ablstr->WIDTH ; last_width++)
        {
        sprintf (nomvar,"(%d)",last_width);
        addlogbddcircuitin(FblBddCircuit,nomvar);
        }
      }
    *pt_bdd = getlogbddnodezero();
    oth_flg = 0;
    }

  while (pt_str != NULL)
    {
    fbl_tobin (binstr, (char *)pt_str->DATA,-1,-1);
    if (oth_flg != 0)
      {
      fbl_error(30,NULL);
      }

    if (strcmp("others", (char *)pt_str->DATA))
      {
      pt_bddres = getlogbddnodeone();

      if (strlen (binstr) != pt_ablstr->WIDTH)
        {
        fbl_error(38,NULL);
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
         fbl_error(28,NULL);
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

  pt_newabl = convertlogbddnode (FblBddCircuit, pt_bddtmp);

  pt_auxabl = pt_ablstr->LIST_ABL;
  i         = pt_ablstr->WIDTH - 1;
  while (pt_auxabl != NULL)
    {
    sprintf (nomvar,"(%i)",i);
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
