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
/*									*/
/* file		: bvl_util.c						*/
/* date		: Jan 18 1993						*/
/* version	: v106							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: This file contains some utility functions :		*/
/*		  bvl_error  , bvl_y_error				*/
/*									*/
/* ###--------------------------------------------------------------### */
	
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "abe.h"
#include "bvl_utype.h"
#include "bvl_util.h"
#include "bvl_utdef.h"

/* ###--------------------------------------------------------------### */
/*  function : bvl_error						*/
/* ###--------------------------------------------------------------### */

void bvl_error (code, str1)

int   code;
char *str1;

  {
  BVL_ERRFLG++;
  (void)fprintf (stderr,"`%s` Error %d line %d :",BVL_CURFIL,code,BVL_LINNUM);

  switch (code)
    {
    case 1:
      (void) fprintf (stderr,"`%s` is incompatible with the entity name\n",str1);
      break;
    case 2:
      (void) fprintf (stderr,"bad entity declaration\n");
      break;
    case 3:
      (void) fprintf (stderr,"bad port clause declaration\n");
      break;
    case 4:
      (void) fprintf (stderr,"port `%s` already declared\n",str1);
      break;
    case 5:
      (void) fprintf (stderr,"illegal port declaration (mode, type, kind)\n");
      break;
    case 6:
      (void) fprintf (stderr,"bad port declaration\n");
      break;
    case 7:
      (void) fprintf (stderr,"`%s` is incompatible with the architecture name\n",str1);
      break;
    case 8:
      (void) fprintf (stderr,"bad architecture declaration\n");
      break;
    case 9:
      (void) fprintf (stderr,"illegal declaration\n");
      break;
    case 10:
      (void) fprintf (stderr,"signal `%s` already declared\n",str1);
      break;
    case 11:
      (void) fprintf (stderr,"illegal signal declaration (type, kind)\n");
      break;
    case 17:
      (void) fprintf (stderr,"`%s` unknown port or signal\n",str1);
      break;
    case 18:
      (void) fprintf (stderr,"illegal concurrent statement\n");
      break;
    case 19:
      (void) fprintf (stderr,"label `%s` already used\n",str1);
      break;
    case 20:
      (void) fprintf (stderr,"`%s` is incompatible with the block's label\n",str1);
      break;
    case 21:
      (void) fprintf (stderr,"input port `%s` cannot be assigned\n",str1);
      break;
    case 22:
      (void) fprintf (stderr,"unguarded signal `%s` assigned by guarded assignment\n",str1);
      break;
    case 23:
      (void) fprintf (stderr,"guarded signal `%s` assigned by unguarded assignment\n",str1);
      break;
    case 25:
      (void) fprintf (stderr,"some choices missing in the selected signal assignment\n");
      break;
    case 26:
      (void) fprintf (stderr,"output port `%s` cannot be read\n",str1);
      break;
    case 28:
      (void) fprintf (stderr,"duplicate choice in selected signal assignment\n");
      break;
    case 30:
      (void) fprintf (stderr,"illegal use of OTHERS in selected signal assignment\n");
      break;
    case 32:
      (void) fprintf (stderr,"null array not supported\n");
      break;
    case 33:
      (void) fprintf (stderr,"incompatible constraint and type\n");
      break;
    case 35:
      (void) fprintf (stderr,"illegal assignment of `%s` (widths mismatch)\n",str1);
      break;
    case 36:
      (void) fprintf (stderr,"signal `%s` used out of declared range\n",str1);
      break;
    case 38:
      (void) fprintf (stderr,"width or/and type mismatch\n");
      break;
    case 39:
      (void) fprintf (stderr,"signal `%s` assigned more than once\n",str1);
      break;
    case 40:
      (void) fprintf (stderr,"signal `%s` never assigned\n",str1);
      break;

    case 68:
      (void) fprintf (stderr,"BEPOR type is unknown\n");
      break;
    case 73:
      (void) fprintf (stderr,"`%s` is not a bit string litteral\n",str1);
      break;
    case 74:
      (void) fprintf (stderr,"bad generic declaration\n");
      break;
    case 75:
      (void) fprintf (stderr,"bad generic element\n");
      break;
    case 76:
      (void) fprintf (stderr,"`%s`: when expression must be a constant\n",str1);
      break;
    case 77:
      (void) fprintf (stderr,"illegal generic declaration (type, kind)\n");
      break;
    case 78:
      (void) fprintf (stderr,"illegal constant declaration (type, kind)\n");
      break;
    case 79:
      (void) fprintf (stderr,"illegal use of attribute STABLE on `%s`\n",str1);
      break;

    default:
      (void) fprintf (stderr,"syntax error\n");
      break;
    }

  if (BVL_ERRFLG > BVL_MXRDFN)
    {
    (void) fprintf (stderr,"Too many errors. Cannot continue further more\n");
    (void) fprintf (stderr,"\n		Have a nice day...\n");
    autexit (1);
    }

  }

/* ###--------------------------------------------------------------### */
/*  function : bvl_y_error						*/
/* ###--------------------------------------------------------------### */
void bvl_y_error (str)

char *str;
  {
  BVL_ERRFLG++;
  (void)fprintf (stderr,"`%s` Error line %d : %s\n",BVL_CURFIL,BVL_LINNUM,str);
  }

/* ###--------------------------------------------------------------### */
/*  function : vhdlablname						*/
/* ###--------------------------------------------------------------### */

/*\
char *vhdlablname (name)

char *name;

  {
  char                 *new_name;
  char                 *prv_name;
  char                 *tmp_name;
  char                  buffer[200];
  int                   i,j,flag,number;
  static struct beden **namtab=NULL;
  static char          *keywrd [] = {
                                     "abs"         , "access"       ,
                                     "after"       , "alias"        ,
                                     "all"         , "and"          ,
                                     "architecture", "array"        ,
                                     "assert"      , "attribute"    ,
                                     "begin"       , "bit"          ,
                                     "bit_vector"  , "block"        ,
                                     "body"        , "buffer"       ,
                                     "bus"         , "case"         ,
                                     "component"   , "configuration",
                                     "constant"    , "disconnect"   ,
                                     "downto"      , "else"         ,
                                     "elsif"       , "end"          ,
                                     "entity"      , "error"        ,
                                     "exit"        , "file"         ,
                                     "for"         , "function"     ,
                                     "generate"    , "generic"      ,
                                     "guarded"     , "if"           ,
                                     "in"          , "inout"        ,
                                     "is"          , "label"        ,
                                     "library"     , "linkage"      ,
                                     "loop"        , "map"          ,
                                     "mod"         , "mux_bit"      ,
                                     "mux_vector"  , "nand"         ,
                                     "natural"     , "new"          ,
                                     "next"        , "nor"          ,
                                     "not"         , "null"         ,
                                     "of"          , "on"           ,
                                     "open"        , "or"           ,
                                     "others"      , "out"          ,
                                     "package"     , "port"         ,
                                     "procedure"   , "process"      ,
                                     "range"       , "record"       ,
                                     "reg_bit"     , "reg_vector"   ,
                                     "register"    , "rem"          ,
                                     "report"      , "return"       ,
                                     "select"      , "severity"     ,
                                     "signal"      , "stable"       ,
                                     "subtype"     , "then"         ,
                                     "to"          , "transport"    ,
                                     "type"        , "units"        ,
                                     "until"       , "use"          ,
                                     "variable"    , "wait"         ,
                                     "warning"     , "when"         ,
                                     "while"       , "with"         ,
                                     "wor_bit"     , "wor_vector"   ,
                                     "xor"
                                     };

  if (namtab == NULL)
    {
    namtab = beh_initab ();
    for (i=0 ; i<93 ; i++)
      beh_addtab (namtab, namealloc (keywrd [i]), NULL, BVL_NEWDFN , 1);
    }

  tmp_name = namealloc (name);
  new_name = (char *) beh_chktab (namtab,tmp_name,NULL,BVL_PNTDFN);

  if (beh_chktab (namtab,tmp_name,NULL,BVL_NAMDFN) == 0)
    {
    i = 0;
    j = 0;
    number = 0;
    flag = 1;
    while (tmp_name[i] != '\0')
      {
      buffer[j] = tmp_name[i];
      if ( ((tmp_name[i] >= 'a') && (tmp_name[i] <= 'z')) ||
           ((tmp_name[i] >= 'A') && (tmp_name[i] <= 'Z')) ||
           ((tmp_name[i] >= '0') && (tmp_name[i] <= '9') && (i != 0)))
        flag = 0;
      else
        {
        if (flag == 1)
          buffer[j++] = 'v';
        buffer[j] = '_';
        flag = 1;
        }
      i++;
      j++;
      }
    if (buffer[j-1] == '_')
      buffer[j++] = '0';
    buffer[j] = '\0';
    new_name = namealloc (buffer);

    prv_name = new_name;
    while (beh_chktab (namtab,new_name,NULL,BVL_NEWDFN) != 0)
      {
      new_name = prv_name;
      sprintf (buffer,"%s_%d",new_name,number++);
      prv_name = new_name;
      new_name = namealloc (buffer);
      }
    beh_addtab (namtab,new_name,NULL,BVL_NEWDFN,1);
    beh_addtab (namtab,tmp_name,NULL,BVL_PNTDFN,(long)new_name);
    beh_addtab (namtab,tmp_name,NULL,BVL_NAMDFN,1);
    }

  return (new_name);
  }
\*/


/* ###--------------------------------------------------------------### */
/* function	: bvl_vectorize						*/
/* description	: put parenthesis on element of bussed signals		*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

char *bvl_vectorize(name)

char *name;
  {
  char *new_name  ;
  char  tmp1 [256];
  int   i          = 0;
  static struct beden **tab = NULL;

  if (tab == NULL)
    tab = beh_initab ();

  if ((new_name = (char *) beh_chktab (tab, name, NULL, BVL_PNTDFN)) == NULL)
    {
    strcpy (tmp1, name);

    while ((name [i] != '\0') && (name [i] != ' ' ) && (name [i] != '\''))
      i ++;

    tmp1 [i] = '\0';
    new_name = vhdlablname (tmp1);

    if (name [i] != '\0')
      {
      if (name [i] == ' ')
        sprintf (tmp1, "%s (%s)", new_name, &name[i+1]); 
      else
        sprintf (tmp1, "%s'%s"  , new_name, &name[i+1]); 
      new_name = namealloc (tmp1);
      }
    beh_addtab (tab, name, NULL, BVL_PNTDFN, new_name);
    }

  return (new_name);
  }

/* ###--------------------------------------------------------------### */
/*  function : bvl_vectnam                                              */
/*  analyze a list of signal and return the bounds of the vectorised	*/
/*    signals, if they occure.						*/
/* ###--------------------------------------------------------------### */

void *bvl_vectnam(pt_list,left,right,name,type)

void *pt_list;
int *left, *right;
char **name;
char type;		/* If type = 0 bepor_list, type = 1 bebux_list	*/
			/* If type = 2 bereg_list, type = 3 beaux_list	*/

  {
  char *blank_space;
  char *sig_name;
  char name_tmp[200];
  char number[200];
  bepor_list *ptpor;
  bebux_list *ptbux;
  beaux_list *ptaux;
  bereg_list *ptreg;
  char END = 0;

  /* Case bepor_list */
  if(type==0)
    {
    ptpor = (bepor_list *)pt_list;
      *left = *right = -1;
      sig_name = ptpor->NAME;
      *name = (char*)mbkalloc(strlen(sig_name) + 1);
      strcpy(*name,sig_name);
      blank_space = strchr(*name,' ');
      if (blank_space != NULL)
        {
        strcpy(number,blank_space);
        *left = atoi(number);
        *right = *left;
        *blank_space = '\0';
        }

      while(!END)
        {
        if(ptpor->NEXT != NULL)
          {
          strcpy(name_tmp,(ptpor->NEXT)->NAME);
          blank_space = strchr(name_tmp,' ');
          if(blank_space!=NULL)
            {
            strcpy(number,blank_space);
            *blank_space = '\0';
            if(!strcmp(*name,name_tmp))
              {
              *right = atoi(number);
              ptpor = ptpor->NEXT;
              }
            else
              END = 1;
            }
          else
            END = 1;
          }
        else
          END = 1;
      }
    return(ptpor);
    }

  /*case bebux_list */
  if(type==1)
    {
    ptbux = (bebux_list *)pt_list;
    /* Extract the name and number of an element */
    *left = *right = -1;
    sig_name = ptbux->NAME;
    *name = (char *)mbkalloc(strlen(sig_name) + 1);
    strcpy(*name,sig_name);
    blank_space = strchr(*name,' ');
    if (blank_space != NULL)
      {
      strcpy(number,blank_space);
      *right = atoi(number);
      *left = *right;
      *blank_space = '\0';
      }

    while(END != 1)
      {
      if(ptbux->NEXT != NULL)
        {
        strcpy(name_tmp,ptbux->NEXT->NAME);
        blank_space = strchr(name_tmp,' ');
        if(blank_space!=NULL)
          {
          strcpy(number,blank_space);
          *blank_space = '\0';
          if(!strcmp(*name,name_tmp))
            {
            *right = atoi(number);
            ptbux = ptbux->NEXT;
            }
          else
            END = 1;
          }
        else
          END = 1;
        }
      else
        END = 1;
      }
    return(ptbux);
    }

  /*case bereg_list */
  if(type==2)
    {
    ptreg = (bereg_list *)pt_list;
    /* Extract the name and number of an element */
    *left = *right = -1;
    sig_name = ptreg->NAME;
    *name = (char *)mbkalloc(strlen(sig_name) + 1);
    strcpy(*name,sig_name);
    blank_space = strchr(*name,' ');
    if (blank_space != NULL)
      {
      strcpy(number,blank_space);
      *right = atoi(number);
      *left = *right;
      *blank_space = '\0';
      }

    while(END != 1)
      {
      if(ptreg->NEXT != NULL)
        {
        strcpy(name_tmp,ptreg->NEXT->NAME);
        blank_space = strchr(name_tmp,' ');
        if(blank_space!=NULL)
          {
          strcpy(number,blank_space);
          *blank_space = '\0';
          if(!strcmp(*name,name_tmp))
            {
            *right = atoi(number);
            ptreg = ptreg->NEXT;
            }
          else
            END = 1;
          }
        else
          END = 1;
        }
      else
        END = 1;
      }
    return(ptreg);
    }

  /*case beaux_list */
  if(type==3)
    {
    ptaux = (beaux_list *)pt_list;
    /* Extract the name and number of an element */
    *left = *right = -1;
    sig_name = ptaux->NAME;
    *name = (char *)mbkalloc(strlen(sig_name) + 1);
    strcpy(*name,sig_name);
    blank_space = strchr(*name,' ');
    if (blank_space != NULL)
      {
      strcpy(number,blank_space);
      *right = atoi(number);
      *left = *right;
      *blank_space = '\0';
      }

    while(END != 1)
      {
      if(ptaux->NEXT != NULL)
        {
        strcpy(name_tmp,ptaux->NEXT->NAME);
        blank_space = strchr(name_tmp,' ');
        if(blank_space!=NULL)
          {
          strcpy(number,blank_space);
          *blank_space = '\0';
          if(!strcmp(*name,name_tmp))
            {
            *right = atoi(number);
            ptaux = ptaux->NEXT;
            }
          else
            END = 1;
          }
        else
          END = 1;
        }
      else
        END = 1;
      }
    return(ptaux);
    }
    return( (void *)0 );
  }

/* ###--------------------------------------------------------------### */
/*  function : bvl_printabl						*/
/*  content  : put a \n for a better presentation of an abl		*/
/* ###--------------------------------------------------------------### */
/*\
char *bvl_printabl(chaine)
char *chaine;
  {
  char *chaine_tmp = NULL;
  char *blanc = NULL;

  chaine_tmp = chaine;
  while(strlen(chaine_tmp)>60)
    {
    chaine_tmp = &chaine_tmp[60];
    blanc = strchr(chaine_tmp,' ');
    if(blanc != NULL)
      {
      *blanc = '\n';
      chaine_tmp = blanc;
      }
    }
  return(chaine);
  }
\*/

void bvl_printablfile( AblFile, Expr )

  FILE       *AblFile;
  chain_list *Expr;
{
  viewablexprfile( AblFile, Expr, ABL_VIEW_VHDL );
}

/* ###--------------------------------------------------------------### */
/* function	: bvl_abl2str						*/
/* description	: return a string corresponding to an expression	*/
/* called func.	:							*/
/* ###--------------------------------------------------------------### */

char *bvl_abl2str (expr, chaine, size_pnt)

struct chain *expr;			/* expression			*/
char         *chaine;			/* target string		*/
int          *size_pnt;			/* size of available space	*/

  {
  int           operator;
  char         *oper    ;
  struct chain *operand ;
  static char  *str_z = NULL;
  static char  *str_o = NULL;
  static char  *str_d = NULL;

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
      strcat (chaine, bvl_vectorize (expr->DATA));
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

    operator = (int) ((struct chain *) expr->DATA)->DATA;
    operand  = (struct chain *)expr->NEXT->DATA;

    if (operator == ABL_STABLE)
      {
      strcat (chaine, bvl_vectorize (operand->DATA));
      strcat (chaine, "'STABLE");
      }
    else
      {
      if (operator == ABL_NOT)
        {
        strcat (chaine, "not (");
        chaine = bvl_abl2str (operand, chaine, size_pnt);
        }
      else
        {
        oper = getablopername (operator);
        strcat (chaine,"(");
        while ( (expr = expr->NEXT) != NULL )
          {
          chaine = bvl_abl2str (expr->DATA, chaine, size_pnt);
          if (expr->NEXT)
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
