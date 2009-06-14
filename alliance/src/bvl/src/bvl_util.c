
/* ###--------------------------------------------------------------### */
/* file		: bvl_util.c						*/
/* date		: Mar  8 2000						*/
/* version	: v114							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: This file contains some utility functions :		*/
/*		  bvl_getvers, bvl_error, bvl_vpor,			*/
/*		  bvl_vaux   , bvl_vbux , bvl_vreg,			*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bvl.h"

/* ###--------------------------------------------------------------### */
/* function	: bvl_getvers						*/
/* description	: return the revision number				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char *bvl_getvers ()

  {
  return ("v114");
  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_error						*/
/* description	: print an error message				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void bvl_error (code, str1)

long   code;
char *str1;

  {
  BVL_ERRFLG++;
  fprintf (stderr, "`%s` Error %d line %d :", BVL_CURFIL, code, BEH_LINNUM);

  switch (code)
    {
    case 1:
      fprintf (stderr, "`%s` is incompatible with the entity name\n", str1);
      break;
    case 2:
      fprintf (stderr, "bad entity declaration\n");
      break;
    case 3:
      fprintf (stderr, "bad port clause declaration\n");
      break;
    case 4:
      fprintf (stderr, "port `%s` already declared\n", str1);
      break;
    case 5:
      fprintf (stderr, "illegal port declaration (mode, type, kind)\n");
      break;
    case 6:
      fprintf (stderr, "bad port declaration\n");
      break;
    case 7:
      fprintf (stderr, "`%s` is incompatible with the architecture name\n", str1);
      break;
    case 8:
      fprintf (stderr, "bad architecture declaration\n");
      break;
    case 9:
      fprintf (stderr, "illegal declaration\n");
      break;
    case 10:
      fprintf (stderr, "signal `%s` already declared\n", str1);
      break;
    case 11:
      fprintf (stderr, "illegal signal declaration (type, kind)\n");
      break;
    case 17:
      fprintf (stderr, "`%s` unknown port or signal\n", str1);
      break;
    case 18:
      fprintf (stderr, "illegal concurrent statement\n");
      break;
    case 19:
      fprintf (stderr, "label `%s` already declared\n", str1);
      break;
    case 20:
      fprintf (stderr, "`%s` is incompatible with the block's label\n", str1);
      break;
    case 21:
      fprintf (stderr, "input port `%s` cannot be assigned\n", str1);
      break;
    case 22:
      fprintf (stderr, "illegal unguarded signal assignment for `%s`\n", str1);
      break;
    case 23:
      fprintf (stderr, "ilegal guarded signal assignment `%s`\n", str1);
      break;
    case 25:
      fprintf (stderr, "some choices missing in the selected signal assignment\n");
      break;
    case 26:
      fprintf (stderr, "output port `%s` cannot be read\n", str1);
      break;
    case 28:
      fprintf (stderr, "duplicate choice in selected signal assignment\n");
      break;
    case 30:
      fprintf (stderr, "illegal use of OTHERS in selected signal assignment\n");
      break;
    case 32:
      fprintf (stderr, "null array not supported\n");
      break;
    case 33:
      fprintf (stderr, "incompatible constraint and type\n");
      break;
    case 35:
      fprintf (stderr, "illegal assignment of `%s` (widths mismatch)\n", str1);
      break;
    case 36:
      fprintf (stderr, "signal `%s` used out of declared range\n", str1);
      break;
    case 38:
      fprintf (stderr, "width or/and type mismatch\n");
      break;
    case 39:
      fprintf (stderr, "signal `%s` assigned more than once\n", str1);
      break;
    case 40:
      fprintf (stderr, "signal `%s` never assigned\n", str1);
      break;

    case 68:
      fprintf (stderr, "BEPOR type is unknown\n");
      break;
    case 73:
      fprintf (stderr, "`%s` is not a bit string litteral\n", str1);
      break;
    case 74:
      fprintf (stderr, "bad generic declaration\n");
      break;
    case 75:
      fprintf (stderr, "bad generic element\n");
      break;
    case 76:
      fprintf (stderr, "`%s`: when expression must be a constant\n", str1);
      break;
    case 77:
      fprintf (stderr, "illegal generic declaration (type, kind)\n");
      break;
    case 78:
      fprintf (stderr, "illegal constant declaration (type, kind)\n");
      break;
    case 79:
      fprintf (stderr, "illegal use of attribute STABLE on `%s`\n", str1);
      break;
    case 80:
      fprintf (stderr, "different delays not supported on waveforms\n");
      break;
    case 81:
      fprintf (stderr, "unknown time unit\n");
      break;

    default:
      fprintf (stderr, "syntax error\n");
      break;
    }

  if (BVL_ERRFLG > 30)
    {
    fprintf (stderr, "Too many errors. Cannot continue further more\n");
    fprintf (stderr, "\n		Have a nice day ...\n");
    exit (1);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_vpor						*/
/* description	: check a list of ports to identify a vector		*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

struct bepor *bvl_vpor (pt_por, pt_lft, pt_rit, name)

struct bepor  *pt_por ;
long           *pt_lft ;
long           *pt_rit ;
char          *name   ;

  {
  struct bepor *prv_por       = NULL;
  long           left          = -1  ;
  long           right         = -1  ;
  long           index         = -1  ;
  char          tmp_nam [256] ;

  prv_por = pt_por;
  if (pt_por != NULL)
    {
    if (sscanf (pt_por->NAME, "%s %u", name, &left) == 1)
      left = -1;
    else
      {
      right   = left        ;
      pt_por  = pt_por->NEXT;
    
      while (pt_por != NULL)
        {
        if (sscanf (pt_por->NAME, "%s %u", tmp_nam, &index) == 1)
          break;
        else
          {
          if (strcmp (name, tmp_nam))
            break;
          }
        right   = index       ;
        prv_por = pt_por      ;
        pt_por  = pt_por->NEXT;
        }
      }
    }

  *pt_lft = left ;
  *pt_rit = right;

  return (prv_por);
  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_vaux						*/
/* description	: check a list of simple internal signals to identify a	*/
/*		  vector						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

struct beaux *bvl_vaux (pt_aux, pt_lft, pt_rit, name)

struct beaux  *pt_aux ;
long           *pt_lft ;
long           *pt_rit ;
char          *name   ;

  {
  struct beaux *prv_aux       = NULL;
  long           left          = -1  ;
  long           right         = -1  ;
  long           index         = -1  ;
  char          tmp_nam [256] ;

  prv_aux = pt_aux;
  if (pt_aux != NULL)
    {
    if (sscanf (pt_aux->NAME, "%s %u", name, &left) == 1)
      left = -1;
    else
      {
      right   = left        ;
      pt_aux  = pt_aux->NEXT;
    
      while (pt_aux != NULL)
        {
        if (sscanf (pt_aux->NAME, "%s %u", tmp_nam, &index) == 1)
          break;
        else
          {
          if (strcmp (name, tmp_nam))
            break;
          }
        right   = index       ;
        prv_aux = pt_aux      ;
        pt_aux  = pt_aux->NEXT;
        }
      }
    }

  *pt_lft = left ;
  *pt_rit = right;

  return (prv_aux);
  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_vbux						*/
/* description	: check a list of bussed internal signals to identify a	*/
/*		  vector						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

struct bebux *bvl_vbux (pt_bux, pt_lft, pt_rit, name)

struct bebux  *pt_bux ;
long           *pt_lft ;
long           *pt_rit ;
char          *name   ;

  {
  struct bebux *prv_bux       = NULL;
  long           left          = -1  ;
  long           right         = -1  ;
  long           index         = -1  ;
  char          tmp_nam [256] ;

  prv_bux = pt_bux;
  if (pt_bux != NULL)
    {
    if (sscanf (pt_bux->NAME, "%s %u", name, &left) == 1)
      left = -1;
    else
      {
      right   = left        ;
      pt_bux  = pt_bux->NEXT;
    
      while (pt_bux != NULL)
        {
        if (sscanf (pt_bux->NAME, "%s %u", tmp_nam, &index) == 1)
          break;
        else
          {
          if (strcmp (name, tmp_nam))
            break;
          }
        right   = index       ;
        prv_bux = pt_bux      ;
        pt_bux  = pt_bux->NEXT;
        }
      }
    }

  *pt_lft = left ;
  *pt_rit = right;

  return (prv_bux);
  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_vreg						*/
/* description	: check a list of internal registers to identify a	*/
/*		  vector						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

struct bereg *bvl_vreg (pt_reg, pt_lft, pt_rit, name)

struct bereg  *pt_reg ;
long           *pt_lft ;
long           *pt_rit ;
char          *name   ;

  {
  struct bereg *prv_reg       = NULL;
  long           left          = -1  ;
  long           right         = -1  ;
  long           index         = -1  ;
  char          tmp_nam [256] ;

  prv_reg = pt_reg;
  if (pt_reg != NULL)
    {
    if (sscanf (pt_reg->NAME, "%s %u", name, &left) == 1)
      left = -1;
    else
      {
      right   = left        ;
      pt_reg  = pt_reg->NEXT;
    
      while (pt_reg != NULL)
        {
        if (sscanf (pt_reg->NAME, "%s %u", tmp_nam, &index) == 1)
          break;
        else
          {
          if (strcmp (name, tmp_nam))
            break;
          }
        right   = index       ;
        prv_reg = pt_reg      ;
        pt_reg  = pt_reg->NEXT;
        }
      }
    }

  *pt_lft = left ;
  *pt_rit = right;

  return (prv_reg);
  }

/* ###--------------------------------------------------------------### */
/*  function : bvl_printabl						*/
/*  content  : put a \n for a better presentation of an abl		*/
/* ###--------------------------------------------------------------### */

char *bvl_printabl (chaine)

char *chaine;

  {
  char *chaine_tmp = NULL;
  char *blanc      = NULL;

  chaine_tmp = chaine;
  while (strlen(chaine_tmp) > 60)
    {
    chaine_tmp = &chaine_tmp[60];
    blanc      = strchr (chaine_tmp, ' ');
    if(blanc != NULL)
      {
      *blanc     = '\n';
      chaine_tmp = blanc;
      }
    }
  return (chaine);
  }

/* ###--------------------------------------------------------------### */
/* function	: bvl_abl2str						*/
/* description	: return a string corresponding to an expression	*/
/* called func.	:							*/
/* ###--------------------------------------------------------------### */

char *bvl_abl2str (expr, chaine, size_pnt)

struct chain *expr    ;			/* expression			*/
char         *chaine  ;			/* target string		*/
long          *size_pnt;			/* size of available space	*/

  {
  long           operator;
  char         *oper    ;
  struct chain *operand ;
  static char  *str_z   = NULL;
  static char  *str_o   = NULL;
  static char  *str_d   = NULL;

  if (str_z == NULL)
    {
    str_z = namealloc ("'0'");
    str_o = namealloc ("'1'");
    str_d = namealloc ("'d'");
    }

	/* ###------------------------------------------------------### */
	/*    if there is not enough space left allocate a bigger block	*/
	/* ###------------------------------------------------------### */

  if (*size_pnt < (strlen (chaine) + 50))
    {
    *size_pnt = *size_pnt + 256;
    chaine  = (char *) realloc (chaine, *size_pnt);
    }

	/* ###------------------------------------------------------### */
	/*    if the expression is a terminal (atom) add its name to	*/
	/* the previous string						*/
	/* ###------------------------------------------------------### */

  if (expr->NEXT == NULL)
    {
    if ((expr->DATA == (void *) str_z) ||
        (expr->DATA == (void *) str_o) ||
        (expr->DATA == (void *) str_d)   )
      strcat (chaine, expr->DATA);
    else
      strcat (chaine, beh_vhvector (expr->DATA));
    }

  else
    {
	/* ###------------------------------------------------------### */
	/*    if the expression is not a terminal :			*/
	/*      - for unary operators (not, stable) add			*/
	/*        "operator ( operand )" to the previous string		*/
	/*								*/
	/*      - for binary operators (and, or, nor, xor, ...) add	*/
	/*        "(operand operator operand operator operand ...)"	*/
	/* ###------------------------------------------------------### */

    operator = (long) ((struct chain *) expr->DATA)->DATA;
    operand  = (struct chain *) expr->NEXT->DATA;

    if (operator == STABLE)
      {
      strcat (chaine, beh_vhvector (operand->DATA));
      strcat (chaine, "'STABLE");
      }
    else
      {
      if (operator == NOT)
        {
        strcat (chaine, "not (");
        chaine = bvl_abl2str (operand, chaine, size_pnt);
        }
      else
        {
        oper = operToChar (operator);
        strcat (chaine,"(");
        while ((expr = expr->NEXT) != NULL)
          {
          chaine = bvl_abl2str (expr->DATA, chaine, size_pnt);
          if ((expr->NEXT) != NULL)
            {
            strcat (chaine, " ");
            strcat (chaine, oper);
            strcat (chaine, " ");
            }
          }
        }
      strcat (chaine, ")");
      }
    }

  return (chaine);
  }
