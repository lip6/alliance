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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/* ###--------------------------------------------------------------### */
/*																		*/
/* file		: fbl_util.c												*/
/* date		: Sep 14 1992												*/
/* version	: v100														*/
/* author	: Pirouz BAZARGAN SABET, M.HANAFI							*/
/* description	: This file contains some utility functions :			*/
/*		  fbl_error  , fsp_y_error, fsp_toolbug, fsp_message,			*/
/*																		*/
/* ###--------------------------------------------------------------### */
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mut.h"
#include "abl.h"
#include "aut.h"
#include "bdd.h"
#include "fvh.h"
#include "fbl_utype.h"
#include "fbl_util.h"
#include "fbl_utdef.h"

char             FBL_ERRFLG  = 0;            /* if = 1 no structure is made  */
int              FBL_LINNUM  = 1;            /* file's line number           */
int              FBL_NUMPTP  = 0;            /* file's line number           */
fbfig_list      *FBL_HEADFIG = (fbfig_list *)0;


/* ###--------------------------------------------------------------### */
/*  function : fbl_toolbug						*/
/* ###--------------------------------------------------------------### */
void fbl_toolbug (code,str1,str2,nbr1)

int   code;
char *str1;
char *str2;
int   nbr1;

  {
  (void) fprintf (stderr,"Fatal error %d executing `%s`: ", code,str1);
  switch (code)
    {
    case 1:
      (void) fprintf (stderr,"unknown operator\n");
      break;
    case 2:
      (void) fprintf (stderr,"cannot create empty atom\n");
      break;
    case 3:
      (void) fprintf (stderr,"cannot build NOT of empty expression\n");
      break;
    case 4:
      (void) fprintf (stderr,"cannot combine empty expressions\n");
      break;
    case 5:
      (void) fprintf (stderr,"cannot find terminal\n");
      break;
    case 6:
      (void) fprintf (stderr,"cannot make BDD of empty expression\n");
      break;
    case 7:
      (void) fprintf (stderr,"unknown type `%c` for IO `%s`\n",nbr1,str2);
      break;
    case 8:
      (void) fprintf (stderr,"illegal signal value : hex `%x`\n",nbr1);
      break;
    case 9:
      (void) fprintf (stderr,"unknown mode `%c` for IO `%s`\n",nbr1,str2);
      break;
    case 10:
      (void) fprintf (stderr,"decompiler called on empty fbfig\n");
      break;
    case 12:
      (void) fprintf (stderr,"illegal user value '%c' for IO\n",nbr1);
      break;
    case 13 :
      (void) fprintf (stderr,"cannot find model of `%s`\n",str2);
      break;
    case 14 :
      (void) fprintf (stderr,"cannot convert empty expression\n");
      break;
    case 15 :
      (void) fprintf (stderr,"illegal bit string value : `%c`\n",nbr1);
      break;
    case 16 :
      (void) fprintf (stderr,"the same expression cannot be used twice\n");
      break;
    case 17 :
      (void) fprintf (stderr,"unknown base specifier : `%c`\n",nbr1);
      break;
    case 18:
      (void) fprintf (stderr,"unknown mode `%c` for port `%s`\n",nbr1,str2);
      break;
    case 19:
      (void) fprintf (stderr,"empty guard expression: `%s`\n",str2);
      break;
    case 20:
      (void) fprintf (stderr,"empty waveform expression: `%s`\n",str2);
      break;
    case 21:
      (void) fprintf (stderr,"cannot find structural level\n");
      break;
    case 22:
      (void) fprintf (stderr,"`%s` : `%c` unknown resolution function\n",str2,nbr1);
      break;
    }
  autexit (1);
  }

/* ###--------------------------------------------------------------### */
/*  function : fbl_message						*/
/* ###--------------------------------------------------------------### */
void fbl_message (code,str1,nmb1)

int   code;
char *str1;
int   nmb1;

  {
  switch (code)
    {
    case 1:
      (void) fprintf (stderr,"Usage : asimut <root> [<pattern> <result_file>] [-option]\n");
      (void) fprintf (stderr,"or      asimut -help\n");
      break;
    case 2:
      (void) fprintf (stdout,"Initializing ...\n");
      break;
    case 3:
      (void) fprintf (stdout,"Compiling `%s` (Behaviour) ...\n",str1);
      break;
    case 4:
      (void) fprintf (stdout,"Compiling `%s` (Structural) ...\n\n",str1);
      break;
    case 5:
      (void) fprintf (stdout,"Searching `%s` ...\n",str1);
      break;
    case 8:
      (void) fprintf (stdout,"Compiling `%s` (Pattern) ...\n\n",str1);
      break;
    case 9:
      (void) fprintf (stdout,"Linking ...\n");
      break;
    case 10:
      (void) fprintf (stdout,"###----- processing pattern %d -----###\n",nmb1);
      break;
    case 11:
/*\
      (void) fprintf (stdout,"Decompiling ...\n");
\*/
      break;
    case 12:
/*\
      (void) fprintf (stdout,"Generating the file '%s' ...\n",str1);
\*/
      break;
    case 13:
/*\
      (void) fprintf (stdout,"Saving '%s' in a vhdl file\n",str1);
\*/
      break;
    case 14:
/*\
      (void) fprintf (stdout,"File '%s' has been generated.\n",str1);
\*/
      break;
    case 15:
      (void) fprintf (stdout,"Making Bdd ...\n\n");
      break;
    case 16:
      (void) fprintf (stdout,"Restoring ...\n\n");
      break;
    default:
      (void) fprintf (stderr,"fbl_message : code %d unknown.\n",code);
    }
  }

/* ###--------------------------------------------------------------### */
/*  function : fbl_error						*/
/* ###--------------------------------------------------------------### */

void loc_fbl_error (code,str1)

int   code;
char *str1;

  {
  FBL_ERRFLG++;
  if (code < 100)
    (void)fprintf (stderr,"Error %d line %d :",code,FBL_LINNUM);
  else
    {
    if (code < 200)
      (void)fprintf (stderr,"Error %d :",code);
    }

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
    case 27:
      (void) fprintf (stderr," '%s' only variable can be assigned with ':=' \n",str1);
      break;
    case 28:
      (void) fprintf (stderr,"duplicate choice in selected signal assignment\n");
      break;
    case 30:
      (void) fprintf (stderr,"illegal use of OTHERS in selected signal assignment\n");
      break;
    case 31:
      (void) fprintf (stderr,"illegal use of OTHERS in case statement \n");
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
    case 80:
      (void) fprintf (stderr,"variable `%s` already declared\n",str1);
      break;
    case 81:
      (void) fprintf (stderr,"internal signal not allowed `%s`\n",str1);
      break;
    case 82:
      (void) fprintf (stderr,"enumerate valu `%s` already declared\n",str1);
      break;
    case 83:
      (void) fprintf (stderr,"'%s' illegal index subtype definition \n",str1);
      break;
    case 84:
      (void) fprintf (stderr,"'%s' illegal type mark definition \n",str1);
      break;
    case 85:
      (void) fprintf (stderr,"when choice is empty in case statement \n");
      break;
    case 86:
      (void) fprintf (stderr,"bad pragma\n");
      break;

    case 100:
      (void) fprintf (stderr,"cannot find `%s`\n",str1);
      break;
    case 107:
      (void) fprintf (stderr,"Cannot open result file\n");
      break;
    case 112:
      (void) fprintf (stderr,"multiple driver on guarded signal `%s`\n",str1);
      break;

    default:
      (void) fprintf (stderr,"syntax error\n");
      break;
    }

  if (FBL_ERRFLG > FBL_MXRDFN)
    {
    (void) fprintf (stderr,"Too many errors. Cannot continue further more\n");
    (void) fprintf (stderr,"\n		Have a nice day...\n");
    autexit (1);
    }

  }

/* ###--------------------------------------------------------------### */
/*  function : fbl_y_error						*/
/* ###--------------------------------------------------------------### */
void fbl_y_error (str)

char *str;
  {
  FBL_ERRFLG++;
  (void)fprintf (stderr,"Error line %d : %s\n",FBL_LINNUM,str);
  }

/* ###--------------------------------------------------------------### */
/*  function : fbl_vectorize						*/
/*  content  : put parenthesis for bused signals			*/
/* ###--------------------------------------------------------------### */

char *fbl_vectorize( Name )

  char *Name;
{
  char *Blank;
  char *VhdlName;

static char Buffer[ 1024 ];

  Blank = strchr( Name, ' ' );

  if ( Blank != (char *)0 )
  {
    strcpy( Buffer, Name );
    Buffer[ Blank - Name ] = '\0';

    VhdlName = vhdlablname( Buffer );

    sprintf( Buffer, "%s(%s)", VhdlName, Blank + 1 );

    Name = namealloc( Buffer );
  }
  else
  {
    if ( strchr( Name, '\'' ) == (char *)0 )
    {
      Name = vhdlablname( Name );
    }
  }

  return( Name );
}
  
/* ###--------------------------------------------------------------### */
/*  function : fbl_vectnam                                              */
/*  analyze a list of signal and return the bounds of the vectorised	*/
/*    signals, if they occure.						*/
/* ###--------------------------------------------------------------### */
void *fbl_vectnam(pt_list,left,right,name,type)
          
void *pt_list;
int *left, *right;  
char **name;        
char type;		/* If type = 0 fbpor_list, type = 1 fbbux_list	*/        
			/* If type = 2 fbreg_list, type = 3 fbaux_list	*/        
                 
  {      
  char *blank_space;
  char *sig_name;
  char name_tmp[200];
  char number[200]; 
  fbpor_list *ptpor;
  fbbux_list *ptbux;
  fbaux_list *ptaux;
  fbreg_list *ptreg;
  char END = 0;     
                    
  /* Case fbpor_list */
  if(type==0)
    {               
    ptpor = (fbpor_list *)pt_list;
      *left = *right = -1;
      sig_name = ptpor->NAME;
      *name = (char*)mbkalloc(strlen(sig_name) + 1);
      strcpy(*name,sig_name);
      blank_space = strchr(*name,' ');
      if (blank_space != 0)
        {           
        strcpy(number,blank_space);
        *left = atoi(number);
        *right = *left;
        *blank_space = '\0';
        }           
                    
      while(!END)   
        {
        if(ptpor->NEXT != 0)
          {
          strcpy(name_tmp,(ptpor->NEXT)->NAME);
          blank_space = strchr(name_tmp,' ');
          if(blank_space!=0)
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
    return((void *)ptpor);
    }
              
  /*case fbbux_list */
  if(type==1)
    {     
    ptbux = (fbbux_list *)pt_list;
    /* Extract the name and number of an element */
    *left = *right = -1;
    sig_name = ptbux->NAME;
    *name = (char *)mbkalloc(strlen(sig_name) + 1);
    strcpy(*name,sig_name);
    blank_space = strchr(*name,' ');
    if (blank_space != 0)
      {        
      strcpy(number,blank_space);
      *right = atoi(number);
      *left = *right;
      *blank_space = '\0';
      }
                    
    while(END != 1) 
      {
      if(ptbux->NEXT != 0)
        {           
        strcpy(name_tmp,ptbux->NEXT->NAME);
        blank_space = strchr(name_tmp,' ');
        if(blank_space!=0)
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
    return((void *)ptbux);  
    }               

  /*case fbreg_list */
  if(type==2)
    {     
    ptreg = (fbreg_list *)pt_list;
    /* Extract the name and number of an element */
    *left = *right = -1;
    sig_name = ptreg->NAME;
    *name = (char *)mbkalloc(strlen(sig_name) + 1);
    strcpy(*name,sig_name);
    blank_space = strchr(*name,' ');
    if (blank_space != 0)
      {        
      strcpy(number,blank_space);
      *right = atoi(number);
      *left = *right;
      *blank_space = '\0';
      }
                    
    while(END != 1) 
      {
      if(ptreg->NEXT != 0)
        {           
        strcpy(name_tmp,ptreg->NEXT->NAME);
        blank_space = strchr(name_tmp,' ');
        if(blank_space!=0)
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
    return((void *)ptreg);  
    }               

  /*case fbaux_list */
  if(type==3)
    {     
    ptaux = (fbaux_list *)pt_list;
    /* Extract the name and number of an element */
    *left = *right = -1;
    sig_name = ptaux->NAME;
    *name = (char *)mbkalloc(strlen(sig_name) + 1);
    strcpy(*name,sig_name);
    blank_space = strchr(*name,' ');
    if (blank_space != 0)
      {        
      strcpy(number,blank_space);
      *right = atoi(number);
      *left = *right;
      *blank_space = '\0';
      }
                    
    while(END != 1) 
      {
      if(ptaux->NEXT != 0)
        {           
        strcpy(name_tmp,ptaux->NEXT->NAME);
        blank_space = strchr(name_tmp,' ');
        if(blank_space!=0)
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
    return((void *)ptaux);  
    }               
    return((void *)0);
  }            

/* ###--------------------------------------------------------------### */
/*  function : fbl_printabl					*/
/* ###--------------------------------------------------------------### */

/*\
void fbl_printabl( Expr )

  chain_list *Expr;
{
  int   Oper;
  char *String;
  int   Length;

  if ( fbl_abllength > 80 )
  {
    fprintf( fbl_ablfile, "\n  " );
    fbl_abllength = 0;
  }

  if ( ABL_ATOM( Expr ) )
  {
    String = fbl_vectorize( ABL_ATOM_VALUE( Expr ) );
    fbl_abllength += strlen( String );
    fprintf( fbl_ablfile, String );
  }
  else
  {
    Oper   = ABL_OPER( Expr );
    String = getablopername( Oper );
    Length = strlen( String );

    if ( Oper == ABL_NOT )
    {
      fbl_abllength += Length + 1;
      fprintf( fbl_ablfile, "%s(", String );

      fbl_printabl( ABL_CADR( Expr ) );

      fprintf( fbl_ablfile, ")" );
      fbl_abllength += 1;
    }
    else
    if ( Oper == ABL_STABLE )
    {
      fbl_printabl( ABL_CADR( Expr ) );
      fbl_abllength += Length + 1;
      fprintf( fbl_ablfile, "'%s", String );
    }
    else
    {
      if ( Oper != ABL_NXOR )
      {
        fprintf( fbl_ablfile, "(" );
        fbl_abllength += 1;
      }
      else
      {
        fprintf( fbl_ablfile, "not(" );
        fbl_abllength += 4;

        String = getablopername( ABL_XOR );
        Length = strlen( String );
      }

      while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
      {
        fbl_printabl( ABL_CAR( Expr ) );

        if ( ABL_CDR( Expr ) )
        {
          fbl_abllength += Length + 2;
          fprintf( fbl_ablfile, " %s ", String );
        }
      }

      fprintf( fbl_ablfile, ")" );
      fbl_abllength += 2;
    }
  }
}
\*/

void fbl_printablfile( AblFile, Expr )

  FILE       *AblFile;
  ablexpr    *Expr;
{
  viewablexprfile( AblFile, Expr, ABL_VIEW_VHDL );
}

/*\

void fbl_printsens( AblFile, Expr )

  FILE *AblFile;
{
  chain_list *Support;
  chain_list *ScanSupport;
  int         First;

  Support = getablexprsupport( Expr, ABL_SUPPORT_CHAIN );
  First   = 1;

  for ( ScanSupport  = Support;
        ScanSupport != (chain_list *)0;
        ScanSupport  = ScanSupport->NEXT )
  {
    if ( First ) fprintf( AblFile, "( %s", fbl_vectorize( ScanSupport->DATA ) );
    else         fprintf( AblFile, ", %s", fbl_vectorize( ScanSupport->DATA ) );

    First = 0;
  }

  freechain( Support );

  if ( ! First ) (void) fprintf( AblFile, " )" );
}

\*/
