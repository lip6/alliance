
/* ###--------------------------------------------------------------### */
/* file		: bvl_bcomp.yac						*/
/* date		: Mar  8 2000						*/
/* version	: v114							*/
/* author	: Pirouz BAZARGAN SABET, L.A. TABUSSE, VUONG H.N.	*/
/* content	: yacc rules for behavioural VHDL			*/
/* ###--------------------------------------------------------------### */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bvl.h"
#include "bvl_byacc.h"
#include "bvl_bedef.h"
#include "bvl_bcomp_y.h"

/* ###--------------------------------------------------------------### */
/* function	: bvl_y_error						*/
/* description	: print an error message				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

extern int  bvl_y_lex   (void);
static void bvl_y_error (str)

char *str;
  {
  BVL_ERRFLG++;
  fprintf (stderr, "`%s` Error line %d : %s\n", BVL_CURFIL, BEH_LINNUM, str);
  }

/* ###--------------------------------------------------------------### */
/* function	: tobin							*/
/* description	: translate a StringLit, BitStringLit or CharacterLit	*/
/*		  in a string of '0' and '1's				*/
/* ###--------------------------------------------------------------### */

static long tobin (

char *trg  ,
char *src  ,
long   left ,
long   right )

{
  char base         ;
  long  indx         ;
  long  j            = 0;
  long  errflg       = 0;
  char lcl_trg [256];

  lcl_trg [0] = '\0';

  if (src == NULL)
    {
    strcpy (trg,"0");
    }
  else
    {
    if (!strcmp (src,"others"))
      {
      strcpy (trg, src);
      }
    else
      {
      if ((src [0] != '\'') && (src [0] != '"') && (src [0] != '%'))
        {
        base = src [0];
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
	      errflg = 1; bvl_error (73,src);
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
	        j -= 3; errflg = 1; bvl_error (73, src);
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
          beh_toolbug (17, "tobin", NULL, base);
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
/* function	: chkdcl						*/
/* ###--------------------------------------------------------------### */

static long chkdcl (object, mode, type, flag, kind, constraint, conf)

char object    ;
long  mode      ;
long  type      ;
char flag      ;
long  kind      ;
char constraint;
long  *conf     ;

  {
  long errflg = 0;
  long lclcnf = 0;

  if (flag != constraint)
    {
    errflg = 1;
    bvl_error (33, NULL);
    }
  else
    {
    switch (object)
      {
      case 'P':
	/* ###------------------------------------------------------### */
	/*    If object is a port :					*/
	/*       - if type is bit, no guard indication can be used	*/
	/*       - if type is wor_bit or mux_bit, bus kind must be used	*/
	/*       - other types are illegal				*/
	/* ###------------------------------------------------------### */

        switch (type)
          {
          case BIT:

            lclcnf += BVL_BITDFN + BVL_NORDFN;
            switch (mode)
              {
              case _IN :
                lclcnf += BVL_ICNDFN; break;
              case _OUT:
                lclcnf += BVL_OCNDFN; break;
              case _INOUT :
                lclcnf += BVL_BCNDFN; break;
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != 0)
              errflg = 1;
            break;

          case MUX_BIT:

            lclcnf += BVL_MUXDFN + BVL_BUSDFN;
            switch (mode)
              {
              case _OUT :
                lclcnf += BVL_OCNDFN; break;
              case _INOUT :
                lclcnf += BVL_BCNDFN; break;
              case _IN :
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != BUS)
              errflg = 1;
            break;

          case WOR_BIT:

            lclcnf += BVL_WORDFN + BVL_BUSDFN;
            switch (mode)
              {
              case _OUT :
                lclcnf += BVL_OCNDFN; break;
              case _INOUT :
                lclcnf += BVL_BCNDFN; break;
              case _IN :
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != BUS)
              errflg = 1;
            break;

          case REG_BIT:
          case NATURAL:

            errflg = 1;
            break;
          }
        if (errflg == 1)
          bvl_error (5, NULL);
        break;

      case 'G':
	/* ###------------------------------------------------------### */
	/*    If object is a generic :					*/
	/*       - only natural type is allowed				*/
	/* ###------------------------------------------------------### */

        if ((type != NATURAL) || (mode != 0) || (kind != 0))
          {
          errflg = 1;
          bvl_error (77, NULL);
          }
        break;

      case 'S':
	/* ###------------------------------------------------------### */
	/*    If object is a signal :					*/
	/*       - no mode can be specified				*/
	/*       - if type is bit no guard indication can be used	*/
	/*       - if type is wor_bit or mux_bit, bus kind must be used	*/
	/*       - if type is reg_bit, register kind must be used	*/
	/*       - other types are illegal				*/
	/* ###------------------------------------------------------### */

        switch (type)
          {
          case BIT:
            lclcnf += BVL_BITDFN + BVL_NORDFN;
            if ((mode != 0) || (kind != 0))
              errflg = 1;
            break;
          case MUX_BIT:
            lclcnf += BVL_MUXDFN + BVL_BUSDFN;
            if ((mode != 0) || (kind != BUS))
              errflg = 1;
            break;
          case WOR_BIT:
            lclcnf += BVL_WORDFN + BVL_BUSDFN;
            if ((mode != 0) || (kind != BUS))
              errflg = 1;
            break;
          case REG_BIT:
            lclcnf += BVL_RBIDFN + BVL_REGDFN;
            if ((mode != 0) || (kind != REGISTER))
              errflg = 1;
            break;
          case NATURAL:
            errflg = 1; break;
          }

        if (mode != 0)
          errflg = 1;

        if (errflg == 1)
          bvl_error (11, NULL);
        break;

      case 'C':
	/* ###------------------------------------------------------### */
	/*    If object is a constant :					*/
	/*       - only bit type without any other indication is legal	*/
	/* ###------------------------------------------------------### */

        lclcnf += BVL_CSTDFN;
        if ((type != BIT) || (mode != 0) || (kind != 0))
          {
          errflg = 1;
          bvl_error (78, NULL);
          }
        break;
      }
    }

  *conf = lclcnf ;
  return (errflg);
  }

/* ###--------------------------------------------------------------### */
/* function	: addstr						*/
/* ###--------------------------------------------------------------### */

static void *addstr (ptfig, object, mode, type, flag, name, left, right)

struct befig *ptfig ;
char          object;
long           mode  ;
long           type  ;
char          flag  ;
char         *name  ;
short         left  ;
short         right ;

  {
  void  *pnt          = NULL;
  char   porflg       = 0;
  char   rinflg       = 0;
  char   outflg       = 0;
  char   busflg       = 0;
  char   auxflg       = 0;
  char   buxflg       = 0;
  char   regflg       = 0;
  char   lclmod       = 'I';
  char   lcltyp       = 'B';
  char   extname [100];
  short  i            ;
  short  inc          = 1;

  switch (object)
    {
    case 'P':

	/* ###------------------------------------------------------### */
	/*   if object is a port ...					*/
	/* ###------------------------------------------------------### */

      porflg = 1;
      switch (mode)
        {
        case _IN:
          lclmod = 'I'; lcltyp = 'B'; rinflg = 1; break;
        case _OUT:
          switch (type)
            {
            case BIT:
              lclmod = 'O'; lcltyp = 'B'; outflg = 1; break;
            case MUX_BIT:
              lclmod = 'Z'; lcltyp = 'M'; busflg = 1; break;
            case WOR_BIT:
              lclmod = 'Z'; lcltyp = 'W'; busflg = 1; break;
            }
          break;
        case _INOUT:
          rinflg = 1;
          switch (type)
            {
            case BIT:
              lclmod = 'B'; lcltyp = 'B'; outflg = 1; break;
            case MUX_BIT:
              lclmod = 'T'; lcltyp = 'M'; busflg = 1; break;
            case WOR_BIT:
              lclmod = 'T'; lcltyp = 'W'; busflg = 1; break;
            }
          break;
        }
      break;

    case 'S':

	/* ###------------------------------------------------------### */
	/*   if object is a signal ...					*/
	/* ###------------------------------------------------------### */

      switch (type)
        {
        case BIT:
          lcltyp = 'B'; rinflg = BVL_AUXMOD; auxflg = 1; break;
        case REG_BIT:
          rinflg = 1; regflg = 1; break;
        case MUX_BIT:
          lcltyp = 'M'; rinflg = 1; buxflg = 1; break;
        case WOR_BIT:
          lcltyp = 'W'; rinflg = 1; buxflg = 1; break;
        }
      break;
    }

  if (flag == 'S')
    {
	/* ###------------------------------------------------------### */
	/*   if object is a scalar ...					*/
	/* ###------------------------------------------------------### */

    if (porflg == 1)
      ptfig->BEPOR = beh_addbepor (ptfig->BEPOR, name, lclmod, lcltyp);
    if (rinflg == 1)
      ptfig->BERIN = beh_addberin (ptfig->BERIN, name);
    if (outflg == 1)
      ptfig->BEOUT = beh_addbeout (ptfig->BEOUT, name, NULL, NULL);
    if (busflg == 1)
      ptfig->BEBUS = beh_addbebus (ptfig->BEBUS, name, NULL, NULL, lcltyp);
    if (auxflg == 1)
      ptfig->BEAUX = beh_addbeaux (ptfig->BEAUX, name, NULL, NULL);
    if (buxflg == 1)
      ptfig->BEBUX = beh_addbebux (ptfig->BEBUX, name, NULL, NULL, lcltyp);
    if (regflg == 1)
      ptfig->BEREG = beh_addbereg (ptfig->BEREG, name, NULL, NULL);

    }
  else
    {
	/* ###------------------------------------------------------### */
	/*   if object is an array ...					*/
	/* ###------------------------------------------------------### */

    if (left >= right)
      inc = -1;

    for (i=left ; i!=(right+inc) ; i+=inc)
      {
      sprintf (extname, "%s %d", name, i);

      if (porflg == 1)
        ptfig->BEPOR = beh_addbepor (ptfig->BEPOR, extname, lclmod, lcltyp);
      if (rinflg == 1)
        ptfig->BERIN = beh_addberin (ptfig->BERIN, extname);
      if (outflg == 1)
        ptfig->BEOUT = beh_addbeout (ptfig->BEOUT, extname, NULL, NULL);
      if (busflg == 1)
        ptfig->BEBUS = beh_addbebus (ptfig->BEBUS, extname, NULL, NULL, lcltyp);
      if (auxflg == 1)
        ptfig->BEAUX = beh_addbeaux (ptfig->BEAUX, extname, NULL, NULL);
      if (buxflg == 1)
        ptfig->BEBUX = beh_addbebux (ptfig->BEBUX, extname, NULL, NULL, lcltyp);
      if (regflg == 1)
        ptfig->BEREG = beh_addbereg (ptfig->BEREG, extname, NULL, NULL);
      }
    }

  if (outflg == 1)
    pnt = (void *) ptfig->BEOUT;
  if (busflg == 1)
    pnt = (void *) ptfig->BEBUS;
  if (auxflg == 1)
    pnt = (void *) ptfig->BEAUX;
  if (buxflg == 1)
    pnt = (void *) ptfig->BEBUX;
  if (regflg == 1)
    pnt = (void *) ptfig->BEREG;

  return (pnt);
  }

/* ###--------------------------------------------------------------### */
/* function	: addgen						*/
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

static struct begen *addgen (last_gen, nat_lst, nam_lst, type, left, right)

struct begen *last_gen; 	/* pointer on the last begen structure	*/
struct chain *nam_lst ;		/* generic's name list			*/
struct chain *nat_lst ;		/* generic's value list			*/
char         *type    ;		/* generic's type			*/
short         left    ;		/* array's left bound (= -1 if scalar)	*/
short         right   ;		/* array's right bound (= -1 if scalar)	*/

  {
  char          extname [128];
  char         *name         ;
  struct begen *ptgen        ;
  struct chain *ptauxnam     ;
  struct chain *ptauxnat     ;
  long           i            ;
  long           inc          = 1;

  ptgen    = last_gen;
  ptauxnam = nam_lst ;
  ptauxnat = nat_lst ;

  if ((left == -1) && (right == -1))
    {
    if ((ptauxnat != NULL) && (ptauxnat->NEXT == NULL))
      {
      while (ptauxnam != NULL)
        {
        name     = namealloc    ((char *) ptauxnam->DATA);
        ptgen    = beh_addbegen (ptgen, name, type, (void *) ptauxnat->DATA);
        ptauxnam = ptauxnam->NEXT;
        }
      }
    else
      bvl_error (75, NULL);
    }
  else
    {
    if (left >= right)
      inc = -1;

    while (ptauxnam != NULL)
      {
      for (i=left ; i!=(right+inc) ; i+=inc)
        {
        sprintf (extname, "%s %ld", (char *)ptauxnam->DATA, i);
        name = namealloc (extname);

        if (ptauxnat != NULL)
          {
          ptgen    = beh_addbegen (ptgen, name, type, (void *)ptauxnat->DATA);
	  ptauxnat = ptauxnat->NEXT;
          }
        else
          bvl_error (75, NULL);
        }

      if (ptauxnat != NULL)
        bvl_error (75, NULL);

      ptauxnat = nat_lst;
      ptauxnam = ptauxnam->NEXT;
      }
    }
  return (ptgen);
  }

/* ###--------------------------------------------------------------### */
/* function	: cpyabllst						*/
/* description	: duplicate bvl_abllst structure 			*/
/* called func.	: addchain, reverse, copyExpr				*/
/* ###--------------------------------------------------------------### */

static struct chain *cpyabllst (abllst)

struct chain *abllst;
  {
  struct chain *pt_abllst = NULL;

  while (abllst != NULL)
    {
    pt_abllst = addchain (pt_abllst, copyExpr ((struct chain *)abllst->DATA));
    abllst    = abllst->NEXT;
    }

  pt_abllst = reverse (pt_abllst);
  return (pt_abllst);
  }

/* ###--------------------------------------------------------------### */
/* function	: cpyablstr						*/
/* description	: duplicate bvl_ablstr structure 			*/
/* called func.	: mbkalloc, cpyabllst					*/
/* ###--------------------------------------------------------------### */

static bvl_ablstr cpyablstr (ablstr)

bvl_ablstr ablstr;
  {
  bvl_ablstr pt_ablstr;

  pt_ablstr.IDENT    = NULL;
  pt_ablstr.WIDTH    = ablstr.WIDTH;
  pt_ablstr.LIST_ABL = cpyabllst (ablstr.LIST_ABL);

  return (pt_ablstr);
  }

/* ###--------------------------------------------------------------### */
/* function     : crtabl						*/
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
/* called func. : createAtom , createExpr, addQExpr , beh_toolbug,	*/
/*		  bvl_error , addchain   , freechain			*/
/* ###--------------------------------------------------------------### */

static bvl_ablstr crtabl (

short      oper ,
bvl_ablstr expr1,
bvl_ablstr expr2,
long        left ,
long        right )

  {
  char            name [256]      ;
  char           *name2           ;
  struct chain   *pt_abl1         = NULL;
  struct chain   *pt_abl2         = NULL;
  struct chain   *pt_aux1         = NULL;
  struct chain   *pt_aux2         = NULL;
  bvl_ablstr      result          ;
  char            lcl_buffer [256];
  short           inc             ;
  short           i               ;
  char            true_flag_un    = 0;
  char            true_flag_zero  = 0;
  struct chain   *abl_un          = createAtom ("'1'");
  struct chain   *abl_zero        = createAtom ("'0'");

  result.IDENT    = NULL;
  result.TIME     = 0   ;
  result.LIST_ABL = NULL;
  result.WIDTH    = 0   ;

  switch (oper)
    {
    case CONC :
      if ((expr1.LIST_ABL == NULL) || (expr2.LIST_ABL == NULL))
        beh_toolbug (4, "crtabl", NULL, 0);
      else
        {
        if (expr1.LIST_ABL == expr2.LIST_ABL)
          beh_toolbug (16, "crtabl", NULL, 0);
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
        if (expr1.IDENT == NULL)
          beh_toolbug (2, "crtabl", NULL, 0);
        else
          {
          if ((left == -1) && (right == -1))
            {
            result.LIST_ABL = addchain(result.LIST_ABL,createAtom(expr1.IDENT));
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
              sprintf (name,"%s %i",expr1.IDENT,i);
              name2           = namealloc (name);
              result.LIST_ABL = addchain (result.LIST_ABL,createAtom(name2));
              }
            }
          expr1.IDENT = NULL;
          }
        break;

      case NOPS :
        if (expr1.IDENT == NULL)
          beh_toolbug (2, "crtabl", NULL, 0);
        else
          {
          tobin (lcl_buffer, expr1.IDENT, -1, -1);
          if ((left == -1) && (right == -1))
            {
            left  = 0;
            right = strlen (lcl_buffer) - 1;
            }

          for (i=left ; i<=right ; i++)
            {
            switch ( lcl_buffer[i] )
              {
              case '0' :
                result.LIST_ABL = addchain (result.LIST_ABL,createAtom("'0'"));
                break;
              case '1' :
                result.LIST_ABL = addchain (result.LIST_ABL,createAtom("'1'"));
                break;
/*-----------        Beware Not VHDL        -------------*/
              case 'd' :
                result.LIST_ABL = addchain (result.LIST_ABL,createAtom("'D'"));
                break;
              default  :
                beh_toolbug (15, "crtabl", NULL, expr1.IDENT[i]);
              }
            }
          result.WIDTH = right - left + 1;
          }
        break;

      case STABLE :
        if (expr1.LIST_ABL == NULL)
	  beh_toolbug (3, "crtabl", NULL, 0);
        else
          {
	  pt_aux1 = expr1.LIST_ABL;
          while (pt_aux1 != NULL)
            {
            pt_abl1  = createExpr (STABLE);
            addQExpr (pt_abl1, (struct chain *)pt_aux1->DATA);
            pt_aux1->DATA = pt_abl1;
            pt_aux1  = pt_aux1->NEXT;
            }

          result.LIST_ABL = expr1.LIST_ABL;
          result.WIDTH    = expr1.WIDTH;

          expr1.LIST_ABL  = NULL;
          }
        break;

      case NOT :
        if (expr1.LIST_ABL == NULL)
	  beh_toolbug (3, "crtabl", NULL, 0);
        else
          {
	  pt_aux1  = expr1.LIST_ABL;
          while (pt_aux1 != NULL)
            {
            pt_abl1  = createExpr (NOT);
            addQExpr (pt_abl1, (struct chain *)pt_aux1->DATA);
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
	  beh_toolbug (4, "crtabl", NULL, 0);
        else
          {
          if (expr1.WIDTH != expr2.WIDTH)
            {
            bvl_error (38,NULL);
            pt_abl2 = createAtom ("'1'");

	    pt_aux1 = expr1.LIST_ABL;
            while (pt_aux1 != NULL)
              {
              freeExpr (pt_aux1->DATA);
              pt_aux1 = pt_aux1->NEXT;
              }
	    pt_aux2 = expr2.LIST_ABL;
            while (pt_aux2 != NULL)
              {
              freeExpr (pt_aux2->DATA);
              pt_aux2 = pt_aux2->NEXT;
              }
            }
          else
            {
	    pt_aux1 = expr1.LIST_ABL;
	    pt_aux2 = expr2.LIST_ABL;
            /* If expr2 = '1' then return expr1 */
            while(pt_aux2 != NULL)
              {
              true_flag_un = 1;
              if(!equalExpr(pt_aux2->DATA,abl_un))
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
                pt_abl2 = createExpr(AND);
                addQExpr(pt_abl2,pt_abl1);
                pt_abl1 = pt_abl2;
                }
              while(pt_aux1 != NULL)
                {
                addQExpr(pt_abl1,(struct chain *)pt_aux1->DATA);
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
              if(!equalExpr(pt_aux2->DATA,abl_zero))
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
                pt_abl1 = createExpr(NOT);
                addQExpr(pt_abl1,(struct chain *)pt_aux1->DATA);
                pt_aux1 = pt_aux1->NEXT;
                if(pt_aux1 != NULL)
                  {
                  pt_abl2 = createExpr(AND);
                  addQExpr(pt_abl2,pt_abl1);
                  pt_abl1 = pt_abl2;
                  }
                while(pt_aux1 != NULL)
                  {
                  pt_abl2 = createExpr(NOT);
                  addQExpr(pt_abl2,(struct chain *)pt_aux1->DATA);
                  addQExpr(pt_abl1,pt_abl2);
                  pt_aux1 = pt_aux1->NEXT;
                  }
                /* --pt_abl1 = createExpr(NOT);
                addQExpr(pt_abl1,(struct chain *)pt_aux1->DATA);
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
              {
              pt_abl1 = createExpr (XOR);
              addQExpr (pt_abl1, (struct chain *)pt_aux1->DATA);
              addQExpr (pt_abl1, (struct chain *)pt_aux2->DATA);

              pt_aux1 = pt_aux1->NEXT;
              pt_aux2 = pt_aux2->NEXT;

	      while (pt_aux1 != NULL)
	        {
                pt_abl2 = createExpr (OR);
                addQExpr (pt_abl2,pt_abl1);

                pt_abl1 = createExpr (XOR);
                addQExpr (pt_abl1, (struct chain *)pt_aux1->DATA);
                addQExpr (pt_abl1, (struct chain *)pt_aux2->DATA);

                addQExpr (pt_abl2, pt_abl1);
                pt_abl1 = pt_abl2;

	        pt_aux1 = pt_aux1->NEXT;
	        pt_aux2 = pt_aux2->NEXT;
	        }
              pt_abl2 = createExpr (NOT);
              addQExpr (pt_abl2, pt_abl1);

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
	  beh_toolbug (4, "crtabl", NULL, 0);
        else
          {
          if (expr1.WIDTH != expr2.WIDTH)
            {
            bvl_error(38,NULL);
            pt_abl1 = createAtom ("'1'");

	    pt_aux1 = expr1.LIST_ABL;
            while (pt_aux1 != NULL)
              {
              freeExpr (pt_aux1->DATA);
              pt_aux1 = pt_aux1->NEXT;
              }
	    pt_aux2 = expr2.LIST_ABL;
            while (pt_aux2 != NULL)
              {
              freeExpr (pt_aux2->DATA);
              pt_aux2 = pt_aux2->NEXT;
              }
            }
          else
            {
	    pt_aux1 = expr1.LIST_ABL;
	    pt_aux2 = expr2.LIST_ABL;

            /* If expr2 = '0' then return expr1 */
            while(pt_aux2 != NULL)
              {
              true_flag_zero = 1;
              if(!equalExpr(pt_aux2->DATA,abl_zero))
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
              if(!equalExpr(pt_aux2->DATA,abl_un))
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
                pt_abl1 = createExpr(NOT);
                addQExpr(pt_abl1,(struct chain *)pt_aux1->DATA);
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
              {
              pt_abl1 = createExpr (XOR);
              addQExpr (pt_abl1, (struct chain *)pt_aux1->DATA);
              addQExpr (pt_abl1, (struct chain *)pt_aux2->DATA);
  
              pt_aux1 = pt_aux1->NEXT;
              pt_aux2 = pt_aux2->NEXT;

  	      for (i=2 ; i<=expr1.WIDTH ; i++)
	        {
                pt_abl2 = createExpr (OR);
                addQExpr (pt_abl2, pt_abl1);

                pt_abl1 = createExpr (XOR);
                addQExpr (pt_abl1, (struct chain *)pt_aux1->DATA);
                addQExpr (pt_abl1, (struct chain *)pt_aux2->DATA);

                addQExpr (pt_abl2, pt_abl1);
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

      case AND  :
      case NAND :
      case OR   :
      case NOR  :
      case XOR  :

        if (expr1.LIST_ABL == NULL)
          {
          if (expr2.LIST_ABL == NULL)
	    beh_toolbug (4, "crtabl", NULL, 0);
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
              beh_toolbug (16, "crtabl", NULL, 0);
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
                  pt_abl1 = createExpr (oper);
                  addQExpr (pt_abl1, (struct chain *)pt_aux1->DATA);
                  addQExpr (pt_abl1, (struct chain *)pt_aux2->DATA);

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
	    beh_toolbug (4, "crtabl", NULL, 0);
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
              pt_abl1 = createExpr (AND);
              addQExpr (pt_abl1,          (struct chain *)pt_aux1->DATA);
              addQExpr (pt_abl1, copyExpr((struct chain *)pt_aux2->DATA));
              pt_aux1->DATA = (void *)pt_abl1;

              pt_aux1       = pt_aux1->NEXT;
              }
            }
          result.LIST_ABL = expr1.LIST_ABL;
          result.WIDTH    = expr1.WIDTH;
	  pt_aux2 = expr2.LIST_ABL;
          while (pt_aux2 != NULL)
            {
            freeExpr (pt_aux2->DATA);
            pt_aux2 = pt_aux2->NEXT;
            }
          freechain (expr2.LIST_ABL);
          expr2.LIST_ABL  = NULL;
          expr1.LIST_ABL  = NULL;
          }
        break;

      default :
	beh_toolbug (1, "crtabl", NULL, 0);
      }

    return (result);
    }

/* ###--------------------------------------------------------------### */
/* function	: bvl_select						*/
/* description	: create an abl representing the choice in a selected	*/
/*		  signal assignment and perform unicity verification	*/
/*		  using BDDs.						*/
/* called func.	: tobin       , bddToAbl     , addInputCct      ,	*/
/*		  substPhyExpr, initializeCct, createNodeTermBdd,	*/
/*		  bvl_error   , notBdd       , applyBinBdd      ,	*/
/*		  addchain    , namealloc				*/
/* ###--------------------------------------------------------------### */

static void bvl_select (result, pt_str, pt_bdd, pt_ablstr)

bvl_ablstr   *result;
struct chain *pt_str;		/* pointer on a list of choices		*/
pNode        *pt_bdd;		/* used to check if a choice is legal	*/
bvl_ablstr    pt_ablstr;	/* tested expression			*/

  {
  char             binstr[256];
  long              i;
  struct chain    *pt_auxabl;
  pNode            pt_bddres;
  pNode            pt_bddnew;
  pNode            pt_bddtmp;
  pNode            pt_bddaux;
  char             nomvar[10];
  struct chain    *pt_newabl;
  struct chain    *pt_newabl2;
  static long       oth_flg=0;
  static long       last_width=0;
  static pCircuit  pC = NULL;

  result->LIST_ABL = NULL;
  result->IDENT    = NULL;

  pt_bddtmp = zero;

  if (pC == NULL)
    pC = initializeCct ("-select-",200,0);

  if (*pt_bdd == NULL)
    {
    if (last_width < pt_ablstr.WIDTH)
      {
      for (; last_width<pt_ablstr.WIDTH ; last_width++)
        {
        sprintf (nomvar,"(%ld)",last_width);
        createNodeTermBdd (addInputCct(pC,nomvar));
        }
      }
    *pt_bdd = zero;
    oth_flg = 0;
    }

  while (pt_str != NULL)
    {
    tobin (binstr, (char *)pt_str->DATA, -1, -1);
    if (oth_flg != 0)
      {
      bvl_error(30,NULL);
      }

    if (strcmp("others", (char *)pt_str->DATA))
      {
      pt_bddres = one;

      if (strlen (binstr) != pt_ablstr.WIDTH)
        {
        bvl_error(38,NULL);
        }
      for (i=0 ; binstr[i]!='\0' ; i++)
        {
        pt_bddaux = createNodeTermBdd (i+2);
        if (binstr[i] == '0')
          pt_bddaux = notBdd (pt_bddaux);
        pt_bddres = applyBinBdd (AND, pt_bddaux, pt_bddres);
        }
      pt_bddnew = applyBinBdd (OR,*pt_bdd,pt_bddres);

      if (*pt_bdd == pt_bddnew)
         {
         bvl_error(28,NULL);
         }

      *pt_bdd = pt_bddnew;
      }
    else
      {
      oth_flg   = 1;
      pt_bddres = notBdd (*pt_bdd);
      *pt_bdd   = one;
      }
    pt_bddtmp = applyBinBdd (OR,pt_bddtmp,pt_bddres);
    pt_str = pt_str->NEXT;
    }

  pt_newabl = bddToAbl (pt_bddtmp, pC->pNameI);

  pt_auxabl = pt_ablstr.LIST_ABL;
  i         = pt_ablstr.WIDTH - 1;
  while (pt_auxabl != NULL)
    {
    sprintf (nomvar,"(%li)",i);
/*--------
    substPhyExpr (pt_newabl,namealloc(nomvar),(struct chain *)pt_auxabl->DATA);
---------*/
    pt_newabl2 = substExpr (pt_newabl,namealloc(nomvar),
                            (struct chain *)pt_auxabl->DATA);
    freeExpr (pt_newabl);
    pt_newabl = pt_newabl2;

    i--;
    pt_auxabl = pt_auxabl->NEXT;
    }
  result->LIST_ABL = addchain (NULL, pt_newabl);
  result->WIDTH    = 1;

  }

%}

%union  {
	long		 valu;
        float            flov;
	char		*text;
        bvl_ablstr       list;
	bvl_name	 name;
        struct g_type    dble;
	};

%token         tok_AND
%token         _BEGIN
%token         _END
%token         _EQSym
%token         _EXIT
%token         _FILE
%token         _GESym
%token         _GTSym
%token         _IN
%token         _INOUT
%token         _LABEL
%token         _LESym
%token         _LINKAGE
%token         _LTSym
%token         _NAND
%token         _NESym
%token         _NEXT
%token         _NOR
%token         _NOT
%token         tok_NULL
%token         _OR
%token         _OUT
%token         _XOR
%token         ABS
%token         ACCESS
%token         AFTER
%token         ALIAS
%token         ALL
%token         ARCHITECTURE
%token         ARRAY
%token         ASSERT
%token         ATTRIBUTE
%token  <text> AbstractLit
%token         Ampersand
%token         Apostrophe
%token         Arrow
%token         BIT
%token         BIT_VECTOR
%token         BLOCK
%token         BODY
%token         BUFFER
%token         BUS
%token         Bar
%token         BasedInt
%token         BasedReal
%token  <text> BitStringLit
%token         Box
%token         CASE
%token         COMPONENT
%token         CONFIGURATION
%token         CONSTANT
%token  <text> CharacterLit
%token         Colon
%token         Comma
%token         DISCONNECT
%token         DOWNTO
%token         DecimalInt
%token         DecimalReal
%token         Dot
%token         DoubleStar
%token         ELSE
%token         ELSIF
%token         ENTITY
%token         ERROR
%token         FOR
%token         FS
%token         FUNCTION
%token         GENERATE
%token         GENERIC
%token         GUARDED
%token         IF
%token         IS
%token  <text> Identifier
%token         LIBRARY
%token         LOOP
%token         LeftParen
%token         MAP
%token         MOD
%token         MS
%token         MUX_BIT
%token         MUX_VECTOR
%token         Minus
%token         NATURAL
%token         NATURAL_VECTOR
%token         NEW
%token         NS
%token         OF
%token         ON
%token         OPEN
%token         OTHERS
%token         _PACKAGE
%token         PORT
%token         PROCEDURE
%token         PROCESS
%token         PS
%token         Plus
%token         RANGE
%token         RECORD
%token         REG_BIT
%token         REG_VECTOR
%token         REGISTER
%token         REM
%token         REPORT
%token         RETURN
%token         RightParen
%token         SELECT
%token         SEVERITY
%token         SIGNAL
%token         _STABLE
%token         SUBTYPE
%token         Semicolon
%token         Slash
%token         Star
%token  <text> StringLit
%token         THEN
%token         TO
%token         TRANSPORT
%token         _TYPE
%token         UNITS
%token         UNTIL
%token         US
%token         USE
%token         VARIABLE
%token         VarAsgn
%token         WAIT
%token         WARNING
%token         WHEN
%token         WHILE
%token         WITH
%token         WOR_BIT
%token         WOR_VECTOR

%left         tok_AND _OR _NAND _NOR _XOR
%left         _EQSym _NESym
%left         _NOT

%type <text>    choice
%type <text>   .simple_name.
%type <text>    simple_name
%type <text>    a_label
%type <text>   .label.
%type <text>    label
%type <valu>    severity__message
%type <text>    report__message
%type <valu>   .SEVERITY__expression.
%type <text>   .REPORT__expression.
%type <text>   .guard_expression.
%type <text>    guard_expression

%type <list> ...waveform__WHEN__choices..
%type <list>    waveform__WHEN__choices
%type <list>    waveform_element
%type <list>    waveform
%type <list>    expression
%type <valu>   .GUARDED.
%type <valu>   .TRANSPORT.

%type <list>    relation..AND__relation..
%type <list>    relation..OR__relation..
%type <list>    relation.NAND_NOR__relation.
%type <list>    relation..XOR__relation..
%type <list>    relation
%type <list>    simple_expression
%type <list>   .sign.term..add_op__term..
%type <list>    term
%type <list>    factor
%type <list>    primary
%type <valu>    relational_operator
%type <text>    literal
%type <list>    aggregate

%type <valu>   .signal_kind.
%type <valu>   .mode.
%type <dble>    type_mark
%type <name>   .constraint.
%type <name>    constraint
%type <name>    range
%type <valu>    direction
%type <valu>    abstractlit
%type <name>    name
%type <name>    slice_name
%type <name>    indexed_name
%type <name>    target
%type <name>    attribute_name
%type <valu>    generic_element_association
%type <text>    constant_VarAsgn__expression
%type <flov>    time_unit
%type <valu>    delay_expression
%type <valu>   .AFTER__delay_expression.

%start design_file

%%

design_file
	: /*empty*/
		{
		/* ###----------------------------------------------### */
		/*    Initializations					*/
		/*      - initialize the time unit conversion variables	*/
		/*      - erroneous  description presumed !		*/
		/*      - zero delay description presumed !		*/
		/* ###----------------------------------------------### */

		BVL_NM1LST = NULL;
		BVL_GRDLST = NULL;
		BVL_CNDLST = NULL;
		BVL_VALLST = NULL;
		BVL_ERRFLG = 0;
		switch (BEH_TIMEUNIT)
		  {
		  case BEH_TU__FS :
		    BVL_CNVFS = 1      ;
		    BVL_CNVPS = 1.0E+3 ;
		    BVL_CNVNS = 1.0E+6 ;
		    BVL_CNVUS = 1.0E+9 ;
		    BVL_CNVMS = 1.0E+12;
		    break;

		  case BEH_TU__PS :
		    BVL_CNVFS = 1.0E-3 ;
		    BVL_CNVPS = 1      ;
		    BVL_CNVNS = 1.0E+3 ;
		    BVL_CNVUS = 1.0E+6 ;
		    BVL_CNVMS = 1.0E+9 ;
		    break;

		  case BEH_TU__NS :
		    BVL_CNVFS = 1.0E-6 ;
		    BVL_CNVPS = 1.0E-3 ;
		    BVL_CNVNS = 1      ;
		    BVL_CNVUS = 1.0E+3 ;
		    BVL_CNVMS = 1.0E+6 ;
		    break;

		  case BEH_TU__US :
		    BVL_CNVFS = 1.0E-9 ;
		    BVL_CNVPS = 1.0E-6 ;
		    BVL_CNVNS = 1.0E-3 ;
		    BVL_CNVUS = 1      ;
		    BVL_CNVMS = 1.0E+3 ;
		    break;

		  case BEH_TU__MS :
		    BVL_CNVFS = 1.0E-12;
		    BVL_CNVPS = 1.0E-9 ;
		    BVL_CNVNS = 1.0E-6 ;
		    BVL_CNVUS = 1.0E-3 ;
		    BVL_CNVMS = 1      ;
		    break;

		  default :
		    BVL_CNVFS = 1      ;
		    BVL_CNVPS = 1      ;
		    BVL_CNVNS = 1      ;
		    BVL_CNVUS = 1      ;
		    BVL_CNVMS = 1      ;
		    bvl_error (83, NULL);
		  }

		if (BVL_ERRFLG == 0)
		  {
		  BVL_CNVFS = BVL_CNVFS / BEH_TIMESTEP;
		  BVL_CNVPS = BVL_CNVPS / BEH_TIMESTEP;
		  BVL_CNVNS = BVL_CNVNS / BEH_TIMESTEP;
		  BVL_CNVUS = BVL_CNVUS / BEH_TIMESTEP;
		  BVL_CNVMS = BVL_CNVMS / BEH_TIMESTEP;
		  }

		dic                 = beh_initab ();
		BVL_BEFPNT          = beh_addbefig (BVL_HEDFIG, NULL);
		BVL_BEFPNT->FLAG   |= BEH_FIG_ZERODELAY;
		BVL_BEFPNT->ERRFLG  = 1;
		BVL_HEDFIG          = BVL_BEFPNT;
		}
	  entity_declaration
	  architecture_body
		{
		if (BVL_ERRFLG == 0)
		  BVL_BEFPNT->ERRFLG = 0;

		beh_fretab (dic);
		}
	;

entity_declaration
	: ENTITY
	  simple_name
	  IS
		{
		BVL_BEFPNT->NAME = $2;
		BVL_MODNAM       = $2;
		}
	  .generic_clause.
	  .port_clause.
	  END_ERR
	  .simple_name.
	  Semicolon_ERR
		{
		if (($8 != NULL) && ($8 != $2))
		  bvl_error (1, $8);
		}
	| ENTITY
	  error
		{
		bvl_error (2, NULL);
		}
	;

.generic_clause.
	: /*empty*/
	| generic_clause
		{
		BVL_BEFPNT->BEGEN = BVL_GENPNT;
		BVL_GENPNT        = NULL      ;
		}
	;

generic_clause
	: GENERIC
	  LeftParen
	  formal_generic_list
	  RightParen_ERR
	  Semicolon_ERR
	| GENERIC
	  error
	  Semicolon_ERR
		{ bvl_error (74, NULL); }
	;

formal_generic_list
	: formal_generic_element
	  ...formal_generic_element..
	;

...formal_generic_element..
	: /*empty*/
	| ...formal_generic_element..
	  Semicolon_ERR
	  formal_generic_element
	;

formal_generic_element
	: CONSTANT
	  identifier_list
	  Colon
	  type_mark
	  .constraint.
	  generic_VarAsgn__expression
		{
		char *type;

		if ($5.FLAG == $4.FLAG)
		  {
		  BVL_NM1LST = reverse   (BVL_NM1LST);
		  type       = namealloc ("natural");
		  BVL_GENPNT = addgen (BVL_GENPNT, BVL_INTLST, BVL_NM1LST,
		                       type, $5.LEFT, $5.RIGHT);
		  }
		else
		  bvl_error (33, NULL);
		freechain (BVL_NM1LST);
		freechain (BVL_INTLST);
		BVL_NM1LST = NULL;
		BVL_INTLST = NULL;
		}
	| error
		{
		/* ###----------------------------------------------### */
		/*   The following 3 lines reject tokens until the	*/
		/* sync. token 'Semicolon' is found			*/
		/* ###----------------------------------------------### */

		do
		  yychar = yylex ();
		while ((yychar != Semicolon) && (yychar != 0));
		yyerrok;
		bvl_error (75, NULL);
		}
	;

generic_VarAsgn__expression
	: VarAsgn
	  generic_expression
	;

generic_expression
	: abstractlit
		{
		long *ptlong;

		ptlong = (long *) mbkalloc (sizeof(long));
		*ptlong = $1;
		BVL_INTLST = addchain (BVL_INTLST, (void *)ptlong);
		}
	| generic_aggregate
	;

generic_aggregate
	: LeftParen
	  generic_element_association
		{
		long *ptlong;

		ptlong = (long *) mbkalloc (sizeof(long));
		*ptlong = $2;
		BVL_INTLST = addchain (BVL_INTLST, (void *)ptlong);
		}
	  ...generic_element_association..
	  RightParen_ERR
	;

...generic_element_association..
	: /* empty */
	| ...generic_element_association..
	  Comma
	  generic_element_association
		{
		long *ptlong;

		ptlong = (long *) mbkalloc (sizeof(long));
		*ptlong = $3;
		BVL_INTLST = addchain (BVL_INTLST, (void *)ptlong);
		}
	;

generic_element_association
	: abstractlit
		{$$ = $1;}
	;

.constraint.
	: /*empty*/
		{
		$$.FLAG  = 'S';
		$$.LEFT  = -1;
		$$.RIGHT = -1;
		}
	| constraint
		{ $$ = $1; }
	;

constraint
	: LeftParen
	  range
	  RightParen_ERR
		{ $$ = $2; }
	;

range
	: abstractlit
	  direction
	  abstractlit
		{
		$$.FLAG  = 'A';
		$$.LEFT  = $1;
		$$.RIGHT = $3;

		if ((($1 > $3) && ($2 == BVL_UPTDFN)) ||
		    (($1 < $3) && ($2 == BVL_DWTDFN)) ||
		    (($1 < 0 ) || ($2 < 0 )))
		  {
		  bvl_error (32, NULL);
		  }
		}
	;

direction
	: TO
		{ $$ = BVL_UPTDFN; }
	| DOWNTO
		{ $$ = BVL_DWTDFN; }
	;

.port_clause.
	: /*empty*/
	| port_clause
	;

port_clause
	: PORT
	  LeftParen
	  formal_port_list
	  RightParen_ERR
	  Semicolon_ERR
	| PORT
	  error
	  Semicolon_ERR
		{ bvl_error (3, NULL); }
	;

formal_port_list
	: formal_port_element
	  ...formal_port_element..
	;

...formal_port_element..
	: /*empty*/
	| ...formal_port_element..
	  Semicolon_ERR
	  formal_port_element
	;

formal_port_element
	: .SIGNAL.
	  identifier_list
	  Colon
	  .mode.
	  type_mark
	  .constraint.
	  .signal_kind.
		{
		char *signame;
		long   sigconf;
		void *pnt;

		/* ###----------------------------------------------### */
		/*    First, check the validity of the declaration.	*/
		/* Then, for each port, create the apropriate set of	*/
		/* structures (berin, bepor, beout, bebus)		*/
		/* ###----------------------------------------------### */

		chkdcl ('P', $4, $5.VALU, $5.FLAG, $7, $6.FLAG, &sigconf);

		BVL_NM1LST = reverse (BVL_NM1LST);
		while (BVL_NM1LST != NULL)
		  {
		  signame = (char *)BVL_NM1LST->DATA;
		  if (beh_chktab (dic, signame, BVL_MODNAM, BVL_SIGDFN) != 0)
		    bvl_error (4, signame);

		  pnt = addstr (BVL_BEFPNT, 'P', $4, $5.VALU, $5.FLAG, signame,
		                $6.LEFT, $6.RIGHT);

		  beh_addtab (dic, signame, BVL_MODNAM, BVL_SIGDFN, sigconf);
		  beh_addtab (dic, signame, BVL_MODNAM, BVL_WMNDFN, $6.LEFT);
		  beh_addtab (dic, signame, BVL_MODNAM, BVL_WMXDFN, $6.RIGHT);
		  beh_addtab (dic, signame, BVL_MODNAM, BVL_PNTDFN, (long)pnt);

		  BVL_NM1LST = delchain (BVL_NM1LST, BVL_NM1LST);
		  }
		}
	| error
		{
		/* ###----------------------------------------------### */
		/*   The following 3 lines reject tokens until the	*/
		/* sync. token 'Semicolon' is found			*/
		/* ###----------------------------------------------### */

		do
		  yychar = yylex ();
		while ((yychar != Semicolon) && (yychar != 0));
		yyerrok;

		bvl_error (6, NULL);
		}
	;

.SIGNAL.
	: /*empty*/
	| SIGNAL
	;

.mode.
	: /*empty*/
		{ $$ = _IN; }
	| _IN
		{ $$ = _IN; }
	| _OUT
		{ $$ = _OUT; }
	| _INOUT
		{ $$ = _INOUT; }
	;

architecture_body
	: ARCHITECTURE
	  Identifier
	  OF
	  simple_name
	  IS
		{
		if ($4 != BVL_MODNAM)
		  bvl_error (1, $4);
		}
	  architecture_declarative_part
	  _BEGIN
	  architecture_statement_part
	  END_ERR
	  .simple_name.
	  Semicolon_ERR
		{
		if (($11 != NULL) && ($11 != $2))
		  bvl_error (7, $11);
		}
	| ARCHITECTURE
	  error
		{ bvl_error (8, NULL); }
	;

architecture_declarative_part
	: ..block_declarative_item..
	;

..block_declarative_item..
	: /*empty*/
	| ..block_declarative_item..
	  block_declarative_item
	;

block_declarative_item
	: signal_declaration
	| constant_declaration
	| error
	  Semicolon_ERR
		{ bvl_error (9, NULL); }
	;

constant_declaration
	: CONSTANT
	  Identifier
	  Colon
	  type_mark
	  .constraint.
	  constant_VarAsgn__expression
	  Semicolon_ERR
		{
		long sigconf;
		
		if (chkdcl ('C', 0, $4.VALU, $4.FLAG, 0, $5.FLAG, &sigconf)==0)
		  {
		  beh_addtab (dic, $2, BVL_MODNAM, BVL_WMNDFN, $5.LEFT);
		  beh_addtab (dic, $2, BVL_MODNAM, BVL_WMXDFN, $5.RIGHT);
		  beh_addtab (dic, $2, BVL_MODNAM, BVL_SIGDFN, sigconf);
		  beh_addtab (dic, $2, NULL, BVL_PNTDFN, $6);
		  }
		}
	;

constant_VarAsgn__expression
	: VarAsgn
	  literal
		{ $$ = $2; }
	;

signal_declaration
	: SIGNAL
	  identifier_list
	  Colon
	  type_mark
	  .constraint.
	  .signal_kind.
	  Semicolon_ERR
		{
		char *signame;
		long   sigconf;
		void *pnt;
		long   errflg;

		errflg = chkdcl ('S',0,$4.VALU,$4.FLAG,$6,$5.FLAG,&sigconf);

		/* ###----------------------------------------------### */
		/*    First, check the validity of the declaration.	*/
		/* Then, for each signal, create the apropriate set of	*/
		/* structures (berin, bereg, beaux, bebux)		*/
		/* ###----------------------------------------------### */

		BVL_NM1LST = reverse (BVL_NM1LST);
		while (BVL_NM1LST != NULL)
		  {
		  signame = (char *)BVL_NM1LST->DATA;
		  if (beh_chktab (dic, signame, BVL_MODNAM, BVL_SIGDFN) != 0)
		    bvl_error (10, signame);

		  pnt = addstr (BVL_BEFPNT, 'S', 0, $4.VALU, $4.FLAG, signame,
		                $5.LEFT, $5.RIGHT);

		  beh_addtab (dic, signame, BVL_MODNAM, BVL_SIGDFN, sigconf);
		  beh_addtab (dic, signame, BVL_MODNAM, BVL_WMNDFN, $5.LEFT);
		  beh_addtab (dic, signame, BVL_MODNAM, BVL_WMXDFN, $5.RIGHT);
		  beh_addtab (dic, signame, BVL_MODNAM, BVL_PNTDFN, (long)pnt);

		  BVL_NM1LST = delchain (BVL_NM1LST, BVL_NM1LST);
		  }
		}
	;

.signal_kind.
	: /*empty*/
		{ $$ = 0; }
	| REGISTER
		{ $$ = REGISTER; }
	| BUS
		{ $$ = BUS; }
	;

architecture_statement_part
	: ..concurrent_statement..
	;

..concurrent_statement..
	: /*empty*/
	| ..concurrent_statement..
	  concurrent_statement
	;

concurrent_statement
	: block_statement
	| concurrent_assertion_statement
	| concurrent_signal_assignment_statement
	| error
	  Semicolon_ERR
		{ bvl_error (18, NULL); }
	;

block_statement
	: a_label
	  BLOCK
	  .guard_expression.
	  _BEGIN
		{
		if (beh_chktab (dic, $1, BVL_MODNAM, BVL_LBLDFN) != 0)
		  bvl_error (19, $1);

		beh_addtab (dic, $1, BVL_MODNAM, BVL_LBLDFN, 1);
		if ($3 != NULL)
		  BVL_GRDLST = addchain (BVL_GRDLST, (char *)$3);
		}
	  set_of_statements
	  END_ERR
	  BLOCK
	  .label.
	  Semicolon_ERR
		{
		if ($3 != NULL)
		  BVL_GRDLST = delchain (BVL_GRDLST, BVL_GRDLST);
		if (($9 != NULL) && ($9 != $1))
		  bvl_error (20, $9);
		}
	;

set_of_statements
	: ..concurrent_statement..
	;

concurrent_assertion_statement
	: a_label
	  unlabeled_concurrent_assertion_statement
		{
		if (beh_chktab (dic, $1, BVL_MODNAM, BVL_LBLDFN) != 0)
		  bvl_error (19, $1);

		beh_addtab (dic, $1, BVL_MODNAM, BVL_LBLDFN, 1);
		if (BVL_ERRFLG == 0)
		  BVL_BEFPNT->BEMSG->LABEL = $1;
		}
	| unlabeled_concurrent_assertion_statement
	;

concurrent_signal_assignment_statement
	: a_label
	  unlabeled_conditional_signal_assignment
		{
		if (beh_chktab (dic, $1, BVL_MODNAM, BVL_LBLDFN) != 0)
		  bvl_error (19, $1);
		beh_addtab (dic, $1, BVL_MODNAM, BVL_LBLDFN, 1);
		}
	| unlabeled_conditional_signal_assignment
	| a_label
	  unlabeled_selected_signal_assignment
		{
		if (beh_chktab (dic, $1, BVL_MODNAM, BVL_LBLDFN) != 0)
		  bvl_error (19, $1);
		beh_addtab (dic, $1, BVL_MODNAM, BVL_LBLDFN, 1);
		}
	| unlabeled_selected_signal_assignment
	;

unlabeled_concurrent_assertion_statement
	: assertion_statement
	;

assertion_statement
	: ASSERT
	  expression
	  .REPORT__expression.
	  .SEVERITY__expression.
	  Semicolon_ERR
		{
		if (BVL_ERRFLG == 0)
		  {
		  BVL_BEFPNT->BEMSG = beh_addbemsg(BVL_BEFPNT->BEMSG, NULL, $4,
		                                   $3, $2.LIST_ABL->DATA, NULL);
		  }
		}
	;

unlabeled_conditional_signal_assignment
	: target
	  _LESym
	  .GUARDED.
	  .TRANSPORT.
	  ..waveform__WHEN__condition__ELSE..
	  waveform
	  Semicolon_ERR
		{
		long             i        ;
		struct beout   *beout_pnt;
		struct bebus   *bebus_pnt;
		struct bereg   *bereg_pnt;
		struct beaux   *beaux_pnt;
		struct bebux   *bebux_pnt;
		struct chain   *abl_pnt  ;
		unsigned long   delay    ;
		struct bvl_expr expr0    ;
		struct bvl_expr expr1    ;
		struct bvl_expr expr2    ;
		struct bvl_expr expr3    ;
		struct bvl_expr expr4    ;
		struct bvl_expr expr5    ;
		struct bvl_expr expr6    ;
		long             rev_flg  = 0;
		long             left_bnd ;
		long             right_bnd;
		long             left     ;
		long             right    ;
		long             in_bound ;
		long             out_bound;
		long             sig_width;
		long             sig_conf ;

		delay = $6.TIME;
		expr4 = $6     ;

		while (BVL_CNDLST != NULL)
		  {
		  expr5 = *((bvl_ablstr *) BVL_CNDLST->DATA);
		  expr6 = *((bvl_ablstr *) BVL_VALLST->DATA);

		  expr0 = cpyablstr (expr5);
		  expr1 = crtabl (NOT , expr5, BVL_EMPSTR, -1, -1);
		  expr5 = expr0;
		  expr2 = crtabl (ANDM, expr4, expr1     , -1, -1);
		  expr3 = crtabl (ANDM, expr6, expr5     , -1, -1);
		  expr4 = crtabl (OR  , expr2, expr3     , -1, -1);

		  BVL_CNDLST = delchain (BVL_CNDLST, BVL_CNDLST);
		  BVL_VALLST = delchain (BVL_VALLST, BVL_VALLST);
		  }

		left_bnd  = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_WMNDFN);
		right_bnd = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_WMXDFN);
		sig_conf  = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_SIGDFN);

		left  = $1.LEFT ;
		right = $1.RIGHT;

		if (left_bnd <= right_bnd)
		  {
		  sig_width = right_bnd - left_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 0;
		    in_bound  = right_bnd - right;
		    out_bound = right_bnd - left ;
		    if ((left < left_bnd) || (right > right_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  else
		    {
		    rev_flg   = 1;
		    in_bound  = right_bnd - left ;
		    out_bound = right_bnd - right;
		    if ((left > right_bnd) || (right < left_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  }
		else
		  {
		  sig_width = left_bnd - right_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 1;
		    in_bound  = left  - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  else
		    {
		    rev_flg   = 0;
		    in_bound  = right - right_bnd;
		    out_bound = left  - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  }

		if ((out_bound - in_bound + 1) != expr4.WIDTH)
		  bvl_error (35, $1.NAME);

		if (rev_flg == 1)
		  expr4.LIST_ABL = reverse (expr4.LIST_ABL);

		abl_pnt = expr4.LIST_ABL;

		switch (sig_conf)
		  {
		  case (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN) :
		    bvl_error (21, $1.NAME);
		    break;

		  case (BVL_OCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_BITDFN + BVL_NORDFN) :
		    if ($3 == BVL_GRDDFN)
		      bvl_error (22, $1.NAME);
		    beout_pnt = (struct beout *)
		                 beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (beout_pnt->ABL != NULL)
		            bvl_error (39, beout_pnt->NAME);
		          else
		            {
		            beout_pnt->ABL  = (struct chain *) abl_pnt->DATA;
		            beout_pnt->TIME = delay;
		            beout_pnt->FLAG = $4   ;
		            }
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        beout_pnt = beout_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_OCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_OCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		    if (($3 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23, $1.NAME);
		    bebus_pnt = (struct bebus *)
		                 beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bebus_pnt->BINODE = beh_addbinode (bebus_pnt->BINODE,
		                                             NULL, NULL);
		          bebus_pnt->BINODE->TIME = delay;
		          bebus_pnt->BINODE->FLAG = $4   ;

		          bebus_pnt->BIABL  = beh_addbiabl  (bebus_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             copyExpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          bebus_pnt->BIABL->TIME = delay;
		          bebus_pnt->BIABL->FLAG = $4   ;
		          abl_pnt                = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bebus_pnt = bebus_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_BITDFN + BVL_NORDFN):
		    if ($3 == BVL_GRDDFN)
		      bvl_error (22, $1.NAME);
		    beaux_pnt = (struct beaux *)
		                beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (beaux_pnt->ABL != NULL)
		            bvl_error (39, beaux_pnt->NAME);
		          else
		            {
		            beaux_pnt->ABL  = (struct chain *) abl_pnt->DATA;
		            beaux_pnt->TIME = delay;
		            beaux_pnt->FLAG = $4   ;
		            }
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        beaux_pnt = beaux_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_RBIDFN + BVL_REGDFN):
		    if (($3 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23, $1.NAME);
		    bereg_pnt = (struct bereg *)
		                beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bereg_pnt->BINODE = beh_addbinode (bereg_pnt->BINODE,
		                                             NULL, NULL);
		          bereg_pnt->BINODE->TIME = delay;
		          bereg_pnt->BINODE->FLAG = $4   ;

		          bereg_pnt->BIABL  = beh_addbiabl  (bereg_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             copyExpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          bereg_pnt->BIABL->TIME = delay;
		          bereg_pnt->BIABL->FLAG = $4   ;
		          abl_pnt                = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bereg_pnt = bereg_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_WORDFN + BVL_BUSDFN) :
		    if (($3 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23, $1.NAME);
		    bebux_pnt = (struct bebux *)
		                beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bebux_pnt->BINODE = beh_addbinode (bebux_pnt->BINODE,
		                                             NULL, NULL);
		          bebux_pnt->BINODE->TIME = delay;
		          bebux_pnt->BINODE->FLAG = $4   ;

		          bebux_pnt->BIABL  = beh_addbiabl  (bebux_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             copyExpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          bebux_pnt->BIABL->TIME = delay;
		          bebux_pnt->BIABL->FLAG = $4   ;
		          abl_pnt                = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bebux_pnt = bebux_pnt->NEXT;
		        }
		      }
		    break;

		  default :
		    bvl_error (17, $1.NAME);
		    break;
		  }
		}
	;

..waveform__WHEN__condition__ELSE..
	: /*empty*/
	| ..waveform__WHEN__condition__ELSE..
	  waveform
	  WHEN
	  expression
	  ELSE
		{
		struct bvl_expr *expr_pnt;

		expr_pnt           = (bvl_ablstr *)mbkalloc(sizeof(bvl_ablstr));
		expr_pnt->WIDTH    = $4.WIDTH;
		expr_pnt->LIST_ABL = $4.LIST_ABL;
		BVL_CNDLST         = addchain (BVL_CNDLST, (char *) expr_pnt);

		expr_pnt           = (bvl_ablstr *)mbkalloc(sizeof(bvl_ablstr));
		expr_pnt->WIDTH    = $2.WIDTH;
		expr_pnt->LIST_ABL = $2.LIST_ABL;
		BVL_VALLST         = addchain (BVL_VALLST, (char *) expr_pnt);
		}
	;

unlabeled_selected_signal_assignment
	: WITH
	  expression
		{
		BVL_SLCEXP = $2;
		BVL_BDDPNT = NULL;
		}
	  SELECT
	  target
	  _LESym
	  .GUARDED.
	  .TRANSPORT.
	  waveform__WHEN__choices
	  ...waveform__WHEN__choices..
	  Semicolon_ERR
		{
		long             i;
		struct beout   *beout_pnt;
		struct bebus   *bebus_pnt;
		struct bereg   *bereg_pnt;
		struct beaux   *beaux_pnt;
		struct bebux   *bebux_pnt;
		struct chain   *abl_pnt  ;
		unsigned long   delay    ;
		struct bvl_expr expr1    ;
		long             rev_flg  = 0;
		long             left_bnd;
		long             right_bnd;
		long             left;
		long             right;
		long             in_bound;
		long             out_bound;
		long             sig_width;
		long             sig_conf;

		/* ###----------------------------------------------### */
		/*    $10 est une structure, pas un pointeur...		*/
		/*    et c'est bien la le probleme............		*/
		/*    Pour BVL_EMPSTR le champs LIST_ABL doit etre NULL	*/
		/* ###----------------------------------------------### */

                if (($10.LIST_ABL != NULL) && ($9.TIME != $10.TIME))
                  {
		  bvl_error (80, NULL);
                  }
		expr1      = crtabl (OR , $9 , $10, -1, -1);
		expr1.TIME = $9.TIME;

		if (BVL_BDDPNT != one)
		  bvl_error (25, NULL);

		left_bnd  = beh_chktab (dic, $5.NAME, BVL_MODNAM, BVL_WMNDFN);
		right_bnd = beh_chktab (dic, $5.NAME, BVL_MODNAM, BVL_WMXDFN);
		sig_conf  = beh_chktab (dic, $5.NAME, BVL_MODNAM, BVL_SIGDFN);

		left  = $5.LEFT ;
		right = $5.RIGHT;

		if (left_bnd <= right_bnd)
		  {
		  sig_width = right_bnd - left_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 0;
		    in_bound  = right_bnd - right;
		    out_bound = right_bnd - left ;
		    if ((left < left_bnd) || (right > right_bnd))
		      bvl_error (36, $5.NAME);
		    }
		  else
		    {
		    rev_flg   = 1;
		    in_bound  = right_bnd - left ;
		    out_bound = right_bnd - right;
		    if ((left > right_bnd) || (right < left_bnd))
		      bvl_error (36, $5.NAME);
		    }
		  }
		else
		  {
		  sig_width = left_bnd - right_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 1;
		    in_bound  = left  - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
		      bvl_error (36, $5.NAME);
		    }
		  else
		    {
		    rev_flg   = 0;
		    in_bound  = right - right_bnd;
		    out_bound = left  - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
		      bvl_error (36, $5.NAME);
		    }
		  }

		if ((out_bound - in_bound + 1) != expr1.WIDTH)
		  bvl_error (35, $5.NAME);

		if (rev_flg == 1)
		  expr1.LIST_ABL = reverse (expr1.LIST_ABL);

		abl_pnt = expr1.LIST_ABL;
		delay   = expr1.TIME    ;

		switch (sig_conf)
		  {
		  case (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN) :
		    bvl_error (21, $5.NAME);
		    break;

		  case (BVL_OCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_BITDFN + BVL_NORDFN) :
		    if ($7 == BVL_GRDDFN)
		      bvl_error (22, $5.NAME);
		    beout_pnt = (struct beout *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (beout_pnt->ABL != NULL)
		            bvl_error (39, beout_pnt->NAME);
		          else
		            {
		            beout_pnt->ABL  = (struct chain *)abl_pnt->DATA;
		            beout_pnt->TIME = delay;
		            }
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        beout_pnt = beout_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_OCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_OCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		    if (($7 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23, $5.NAME);
		    bebus_pnt = (struct bebus *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bebus_pnt->BINODE = beh_addbinode (bebus_pnt->BINODE,
		                                             NULL, NULL);
		          bebus_pnt->BINODE->TIME = delay;
		          bebus_pnt->BINODE->FLAG = $8   ;

		          bebus_pnt->BIABL  = beh_addbiabl  (bebus_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             copyExpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          bebus_pnt->BIABL->TIME = delay;
		          bebus_pnt->BIABL->FLAG = $8   ;
		          abl_pnt                = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bebus_pnt = bebus_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_BITDFN + BVL_NORDFN):
		    if ($7 == BVL_GRDDFN)
		      bvl_error (22, $5.NAME);
		    beaux_pnt = (struct beaux *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (beaux_pnt->ABL != NULL)
		            bvl_error (39, beaux_pnt->NAME);
		          else
		           {
		            beaux_pnt->ABL  = (struct chain *) abl_pnt->DATA;
		            beaux_pnt->TIME = delay;
		            beaux_pnt->FLAG = $8   ;
		            }
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        beaux_pnt = beaux_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_RBIDFN + BVL_REGDFN):
		    if (($7 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23, $5.NAME);
		    bereg_pnt = (struct bereg *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bereg_pnt->BINODE = beh_addbinode (bereg_pnt->BINODE,
		                                             NULL, NULL);
		          bereg_pnt->BINODE->TIME = delay;
		          bereg_pnt->BINODE->FLAG = $8   ;

		          bereg_pnt->BIABL  = beh_addbiabl  (bereg_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             copyExpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          bereg_pnt->BIABL->TIME = delay;
		          bereg_pnt->BIABL->FLAG = $8   ;
		          abl_pnt                = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bereg_pnt = bereg_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_WORDFN + BVL_BUSDFN) :
		    if (($7 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23, $5.NAME);
		    bebux_pnt = (struct bebux *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bebux_pnt->BINODE = beh_addbinode (bebux_pnt->BINODE,
		                                             NULL, NULL);
		          bebux_pnt->BINODE->TIME = delay;
		          bebux_pnt->BINODE->FLAG = $8   ;

		          bebux_pnt->BIABL  = beh_addbiabl  (bebux_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             copyExpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          bebux_pnt->BIABL->TIME = delay;
		          bebux_pnt->BIABL->FLAG = $8   ;
		          abl_pnt                = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bebux_pnt = bebux_pnt->NEXT;
		        }
		      }
		    break;

		  default :
		    bvl_error (17, $5.NAME);
		    break;
		  }
		}
	;

...waveform__WHEN__choices..
	: /*empty*/
		{ $$ = BVL_EMPSTR; }
	| ...waveform__WHEN__choices..
	  Comma
	  waveform__WHEN__choices
		{
		/* ###----------------------------------------------### */
		/*    $9 est une structure, pas un pointeur...		*/
		/*    et c'est bien la le probleme............		*/
		/*    Pour BVL_EMPSTR le champs LIST_ABL doit etre NULL	*/
		/* ###----------------------------------------------### */

		if (($1.LIST_ABL != NULL) && ($1.TIME != $3.TIME))
                  {
		  bvl_error (80, NULL);
                  }

		$$      = crtabl (OR, $1, $3, -1, -1);
		$$.TIME = $3.TIME;
		}
	;

waveform__WHEN__choices
	: waveform
	  WHEN
	  choices
		{
		struct bvl_expr expr1;

		bvl_select (&expr1, BVL_NM1LST, &BVL_BDDPNT, BVL_SLCEXP);
		freechain  (BVL_NM1LST);
		BVL_NM1LST = NULL;

		$$      = crtabl (ANDM, $1, expr1, -1, -1);
		$$.TIME = $1.TIME;
		}
	;

waveform
	: waveform_element
		{ $$ = $1; }
	;

waveform_element
	: expression
	  .AFTER__delay_expression.
		{
		$$      = $1;
		$$.TIME = $2;
		}
	;

.AFTER__delay_expression.
	: /*empty*/
		{ $$ = 0; }
	| AFTER
	  delay_expression
		{ $$ = $2; }
	;

delay_expression
	: abstractlit
	  time_unit
		{
		unsigned long dly;

		/* ###----------------------------------------------### */
		/*    if the delay is not null unset the 'zero delay'	*/
		/* flag of the description				*/
		/* ###----------------------------------------------### */

		dly = $1 * $2;
		if (dly != 0)
		  BVL_BEFPNT->FLAG &= ~BEH_FIG_ZERODELAY;

		$$ = dly;
		}
	;

time_unit
	: FS
		{ $$ = BVL_CNVFS ; }
	| PS
		{ $$ = BVL_CNVPS ; }
	| NS
		{ $$ = BVL_CNVNS ; }
	| US
		{ $$ = BVL_CNVUS ; }
	| MS
		{ $$ = BVL_CNVMS ; }
	;

choices
	: choice
		{ BVL_NM1LST = addchain (NULL, $1); }
	  ..Bar__choice..
		{ yyerrok; }
	;

..Bar__choice..
	: /*empty*/
	| ..Bar__choice..
	  Bar
	  choice
		{
        if (!strncmp($3,"others",7))
		  bvl_error (30, NULL);
		BVL_NM1LST = addchain (BVL_NM1LST, $3);
		}
	;

choice
	: literal
		{ $$ = $1; }
	| OTHERS
		{ $$ = "others"; }
	| name
		{
		char           *val;
		char            val2[256];
		long             left;
		long             right;
		long             in_bound;
		long             out_bound;
		long             left_bnd;
		long             right_bnd;
		long             sig_conf;

		strcpy (val2, "B\"");
		sig_conf = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_SIGDFN);
		val      = (char *) beh_chktab (dic, $1.NAME, NULL, BVL_PNTDFN);
		if (sig_conf == 0)
		  bvl_error (17, $1.NAME);
		else
		  {
		  if (sig_conf != BVL_CSTDFN)
		    bvl_error (76, $1.NAME);
		  }

		left_bnd  = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_WMNDFN);
		right_bnd = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_WMXDFN);

		left  = $1.LEFT;
		right = $1.RIGHT;

		if (left_bnd <= right_bnd)
		  {
		  if (left <= right)
		    {
		    in_bound = left - left_bnd;
		    out_bound = right - left_bnd;
		    if ((left < left_bnd) || (right > right_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  else
		    {
		    in_bound = left - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left > right_bnd) || (right < left_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  }
		else
		  {
		  if (left <= right)
		    {
		    in_bound = right - left_bnd;
		    out_bound = left - left_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  else
		    {
		    in_bound = right - right_bnd;
		    out_bound = left - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  }

		tobin (&val2[2], val, in_bound, out_bound);
		strcat (val2, "\"");
		$$ = namealloc (val2);
		}
	;

.REPORT__expression.
	: /*empty*/
		{ $$ = NULL; }
	| REPORT
	  report__message
		{ $$ = $2; }
	;

.SEVERITY__expression.
	: /*empty*/
		{ $$ = 'E'; }
	| SEVERITY
	  severity__message
		{ $$ = $2; }
	;

report__message
	: StringLit
		{ $$ = $1; }
	;

severity__message
	: ERROR
		{ $$ = 'E'; }
	| WARNING
		{ $$ = 'W'; }
	;

expression
	: relation..AND__relation..
		{ $$ = $1; }
	| relation..OR__relation..
		{ $$ = $1; }
	| relation.NAND_NOR__relation.
		{ $$ = $1; }
	| relation..XOR__relation..
		{ $$ = $1; }
	;

relation..AND__relation..
	: relation
	  tok_AND
	  relation
		{ $$ = crtabl (AND , $1 , $3 , -1, -1); }
	| relation..AND__relation..
	  tok_AND
	  relation
		{ $$ = crtabl (AND , $1 , $3 , -1, -1); }
	;

relation..OR__relation..
	: relation
	  _OR
	  relation
		{ $$ = crtabl (OR , $1 , $3 , -1, -1); }
	| relation..OR__relation..
	  _OR
	  relation
		{ $$ = crtabl (OR , $1 , $3 , -1, -1); }
	;

relation.NAND_NOR__relation.
	: relation
		{ $$ = $1; }
	| relation
	  _NAND
	  relation
		{ $$ = crtabl (NAND , $1 , $3 , -1, -1); }
	| relation
	  _NOR
	  relation
		{ $$ = crtabl (NOR , $1 , $3 , -1, -1); }
	;


relation..XOR__relation..
	: relation
	  _XOR
	  relation
		{ $$ = crtabl (XOR , $1 , $3 , -1, -1); }
	| relation..XOR__relation..
	  _XOR
	  relation
		{ $$ = crtabl (XOR , $1 , $3 , -1, -1); }
	;

relation
	: simple_expression
		{ $$ = $1; }
	| simple_expression
	  relational_operator
	  simple_expression
		{ $$ = crtabl ($2 , $1 , $3 , -1, -1); }
	;

simple_expression
	: .sign.term..add_op__term..
		{ $$ = $1; }
	;

.sign.term..add_op__term..
	: term
		{ $$ = $1; }
	| .sign.term..add_op__term..
	  Ampersand
	  term
		{ $$ = crtabl (CONC, $1, $3, -1, -1); }

	;

term
	: factor
		{ $$ = $1; }
	;

factor
	: primary
		{ $$ = $1; }
	| _NOT
	  primary
		{ $$ = crtabl (NOT, $2, BVL_EMPSTR, -1, -1); }
	;

primary
	: literal
		{
		struct bvl_expr expr1;
		expr1.IDENT = $1;
		$$ = crtabl (NOPS, expr1, BVL_EMPSTR, -1, -1);
		}
	| aggregate
		{ $$ = $1; }
	| name
		{
		struct bvl_expr expr1;
		long             left;
		long             right;
		long             left_bnd;
		long             right_bnd;
		long             in_bound;
		long             out_bound;
		long             sig_conf;

		sig_conf = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_SIGDFN);
		switch (sig_conf)
		  {
		  case (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		  case (BVL_BITDFN + BVL_NORDFN):
		  case (BVL_MUXDFN + BVL_BUSDFN):
		  case (BVL_WORDFN + BVL_BUSDFN):
		  case (BVL_RBIDFN + BVL_REGDFN):
		  case (BVL_CSTDFN):
		    break;
		  case (BVL_OCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_OCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_OCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		    bvl_error (26, $1.NAME);
		    break;
		  default :
		    bvl_error (17, $1.NAME);
		    break;
		  }

		left_bnd  = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_WMNDFN);
		right_bnd = beh_chktab (dic, $1.NAME, BVL_MODNAM, BVL_WMXDFN);

		left  = $1.LEFT;
		right = $1.RIGHT;

		if (left_bnd <= right_bnd)
		  {
		  if (left <= right)
		    {
		    in_bound  = left  - left_bnd;
		    out_bound = right - left_bnd;
		    if ((left < left_bnd) || (right > right_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  else
		    {
		    in_bound  = right - left_bnd;
		    out_bound = left  - left_bnd;
		    if ((left > right_bnd) || (right < left_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  }
		else
		  {
		  if (left <= right)
		    {
		    in_bound  = left  - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  else
		    {
		    in_bound  = right - right_bnd;
		    out_bound = left  - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
		      bvl_error (36, $1.NAME);
		    }
		  }

		if (sig_conf != BVL_CSTDFN)
		  {
		  expr1.IDENT = $1.NAME;
		  if($1.FLAG == 'X')
		    {
		    expr1 = crtabl (NOPI  , expr1, BVL_EMPSTR, left, right);
		    $$    = crtabl (STABLE, expr1, BVL_EMPSTR, left, right);
		    }
		  else 
		    $$    = crtabl (NOPI  , expr1, BVL_EMPSTR, left, right);
		  }
		else
		  {
		  expr1.IDENT = (char *)beh_chktab(dic,$1.NAME,NULL,BVL_PNTDFN);
		  $$ = crtabl (NOPS,expr1,BVL_EMPSTR,in_bound,out_bound);
		  }
		}
	;

relational_operator
	: _EQSym
		{ $$ = EQ; }
	| _NESym
		{ $$ = NE; }
	;

literal
	: CharacterLit
		{ $$ = $1; }
	| StringLit
		{ $$ = $1; }
	| BitStringLit
		{ $$ = $1; }
	;

aggregate
	: LeftParen
	  expression
	  RightParen
		{ $$ = $2; }
	;

name
	: simple_name
		{
		$$.NAME  = $1;
		$$.LEFT  = beh_chktab (dic, $1, BVL_MODNAM, BVL_WMNDFN);
		$$.RIGHT = beh_chktab (dic, $1, BVL_MODNAM, BVL_WMXDFN);
		}
	| indexed_name
		{ $$ = $1; }
	| slice_name
		{ $$ = $1; }
	| attribute_name
		{ $$ = $1; }
	;

indexed_name
	: simple_name
	  LeftParen
	  abstractlit
	  RightParen_ERR
		{
		$$.NAME  = $1;
		$$.LEFT  = $3;
		$$.RIGHT = $3;
		}
	;

slice_name
	: simple_name
	  LeftParen
	  abstractlit
	  direction
	  abstractlit
	  RightParen_ERR
		{
		if ((($5 > $3) && ($4 != BVL_UPTDFN)) ||
		    (($5 < $3) && ($4 != BVL_DWTDFN)))
		  bvl_error (32, $1);

		$$.NAME  = $1;
		$$.LEFT  = $3;
		$$.RIGHT = $5;
		}
	;

attribute_name
	: simple_name
	  Apostrophe
	  attribute_designator
		{
		char             extname [100];
		char            *lclname      ;
		long              sig_conf     ;
		struct bvl_expr  expr1        ;
		struct bvl_expr  expr2        ;
		struct chain    *ptabl        ;

		sig_conf = beh_chktab (dic, $1, BVL_MODNAM, BVL_SIGDFN);
		switch (sig_conf)
		  {
		  case (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		  case (BVL_BITDFN + BVL_NORDFN):
		  case (BVL_MUXDFN + BVL_BUSDFN):
		  case (BVL_WORDFN + BVL_BUSDFN):
		  case (BVL_RBIDFN + BVL_REGDFN):
		    break;
		  default :
		    bvl_error (79, $1);
		  }

		if (beh_chktab (dic, $1, BVL_MODNAM, BVL_WMNDFN) != -1)
		  bvl_error (79, $1);

		sprintf (extname, "%s'delayed", $1);
		lclname = namealloc (extname);

		if (BVL_ERRFLG == 0)
		  {
		  if (beh_chktab (dic, $1, BVL_MODNAM, BVL_STBDFN) == 0)
		    {
		    expr1.IDENT = $1;
		    expr1.WIDTH  = 1;
		    expr2 = crtabl (NOPI, expr1, BVL_EMPSTR, -1, -1);
		    ptabl = expr2.LIST_ABL->DATA;

		    if (BVL_AUXMOD == 1)
		      BVL_BEFPNT->BERIN=beh_addberin(BVL_BEFPNT->BERIN,lclname);

		    BVL_BEFPNT->BEDLY = beh_addbeaux(BVL_BEFPNT->BEDLY,lclname,
		                                     ptabl, NULL);
		    beh_addtab (dic, $1     , BVL_MODNAM, BVL_STBDFN, 1);
		    beh_addtab (dic, lclname, BVL_MODNAM, BVL_WMNDFN, -1);
		    beh_addtab (dic, lclname, BVL_MODNAM, BVL_WMXDFN, -1);
		    beh_addtab (dic, lclname, BVL_MODNAM, BVL_SIGDFN,
		                (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN));
		    }
		  }
		BVL_BEFPNT->TYPE |= BEH_STABLE;
		$$.NAME  = $1 ;
		$$.LEFT  = -1 ;
		$$.RIGHT = -1 ;
		$$.FLAG  = 'X';

		}
	;

attribute_designator
	: _STABLE
	;

type_mark
	: BIT
		{ $$.VALU = BIT; $$.FLAG = 'S'; }
	| WOR_BIT
		{ $$.VALU = WOR_BIT; $$.FLAG = 'S'; }
	| MUX_BIT
		{ $$.VALU = MUX_BIT; $$.FLAG = 'S'; }
	| BIT_VECTOR
		{ $$.VALU = BIT; $$.FLAG = 'A'; }
	| WOR_VECTOR
		{ $$.VALU = WOR_BIT; $$.FLAG = 'A'; }
	| MUX_VECTOR
		{ $$.VALU = MUX_BIT; $$.FLAG = 'A'; }
	| REG_BIT
		{ $$.VALU = REG_BIT; $$.FLAG = 'S'; }
	| REG_VECTOR
		{ $$.VALU = REG_BIT; $$.FLAG = 'A'; }
	| NATURAL
		{ $$.VALU = NATURAL; $$.FLAG = 'S'; }
	| NATURAL_VECTOR
		{ $$.VALU = NATURAL; $$.FLAG = 'A'; }
	;

identifier_list
	: Identifier
		{ BVL_NM1LST = addchain (BVL_NM1LST, $1); }
	  ...identifier..
	;

...identifier..
	: /*empty*/
	| ...identifier..
	  Comma
	  Identifier
		{ BVL_NM1LST = addchain (BVL_NM1LST, $3); }
	;

.label.
	: /*empty*/
		{ $$ = NULL; }
	| label
		{ $$ = $1; }
	;

.guard_expression.
	: /*empty*/
		{ $$ = NULL; }
	| guard_expression
		{ $$ = $1; }
	;

guard_expression
	: LeftParen
	  expression
	  RightParen_ERR
		{ $$ = (char *)$2.LIST_ABL->DATA; }
	;

.TRANSPORT.
	: /*empty*/
		{ $$ = BEH_ASG_INERTIAL ; }
	| TRANSPORT
		{ $$ = BEH_ASG_TRANSPORT; }
	;

.GUARDED.
	: /*empty*/
		{ $$ = BVL_UNGDFN ; }
	| GUARDED
		{ $$ = BVL_GRDDFN; }
	;

.simple_name.
	: /*empty*/
		{ $$ = NULL; }
	| simple_name
		{ $$ = $1; }
	;

simple_name
	: Identifier
		{ $$ = $1; }
	;

target
	: name
		{ $$ = $1; }
	;

a_label
	: label
	  Colon
		{
		BVL_LBLNAM = $1;
		$$ = $1;
		}
	;

label
	: Identifier
		{ $$ = $1; }
	;

abstractlit
	: AbstractLit
		{ $$ = atoi ($1); }
	;

RightParen_ERR
	: RightParen
		{ yyerrok; }
	;

Semicolon_ERR
	: Semicolon
		{ yyerrok; }
	;

END_ERR
	: _END
		{ yyerrok; }
	;
%%
