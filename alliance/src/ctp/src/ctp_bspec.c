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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "vex.h"
#include "aut.h"
#include "ctl.h"

#include "ctp_bedef.h"
#include "ctp_bspec.h"

static int CtpEvalError = 0;

# ifndef NO_DEBUG
# define my_ctp_toolbug(V1,V2,V3,V4) \
   do { fprintf( stderr, "%s.%d:", autbasename(__FILE__,NULL), __LINE__);\
           ctp_toolbug(V1,V2,V3,V4); } while(0)
# define my_ctp_error(N,V) \
   do { fprintf( stderr, "%s.%d:", autbasename(__FILE__,NULL), __LINE__);\
           ctp_error(N,V); } while(0)
# else
# define my_ctp_toolbug ctp_toolbug
# define my_ctp_error ctp_error
# endif

/* ###--------------------------------------------------------------### */
/* function	: ctp_stostr						*/
/* description	: purge double % (percent) or Quote (") from string	*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

char *ctp_stostr(str)

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
    
  mbkfree( str );
  return( &resstr[0] );
}

/* ###--------------------------------------------------------------### */
/* function     : ctp_crtvex                                            */
/* description  : combine at most two ABLs and build a new one          */
/*    		  The following operations can be performed :		*/
/*		    CONC    perform concatenation			*/
/*		    NOPI    initialize a structure for a signal (scalar	*/
/*		            or array)					*/
/*		    NOPS    initialize a structure for a literal	*/
/*		    EMPTYOP initialize a structure for an empty op	*/
/*		    NE      create a structure with an ABL representing	*/
/*		            the 'non equality' of two expressions	*/
/*		    EQ      create a structure with an ABL representing	*/
/*		            the 'equality' of two expressions		*/
/*		    NOT     perform logical not of an expression	*/
/*		    VEX_AND     perform logical and  between two expressions*/
/*		    VEX_OR      perform logical or   between two expressions*/
/*		    VEX_NAND    perform logical nand between two expressions*/
/*		    NOR     perform logical nor  between two expressions*/
/*		    XOR     perform logical xor  between two expressions*/
/* called func. : createvexatom , createvexoper, addvexqexpr , my_ctp_toolbug,	*/
/*		  ctp_error , addchain   , freechain			*/
/* ###--------------------------------------------------------------### */

ctltype_list *ctp_getbasetype( type )

  int type;
{
  ctltype_list *ScanType;

  for ( ScanType  = CTP_HEADFIG->TYPE;
        ScanType != (ctltype_list *)0;
        ScanType  = ScanType->NEXT )
  {
    if ( ScanType->INDEX == type )
    {
      if ( ScanType->BASE != (ctltype_list *)0 )
      {
        ScanType = ScanType->BASE;
      }

      break;
    }
  }

  return( ScanType );
}

vexexpr *ctp_vexatom( Name, Left, Right )

   char          *Name;
   long           Left;
   long           Right;
{
  vexexpr *Result = (vexexpr *)0;

  if ( ( Left  == -1 ) && 
       ( Right == -1 )  )
  {
    Result = createvexatombit( Name );
  }
  else
  {
    Result = createvexatomvec( Name, Left, Right );
  }

  return( Result );

}

ctp_vexstr ctp_crtvex( oper, expr1, expr2, left, right)

short      oper;
ctp_vexstr expr1;
ctp_vexstr expr2;
long       left;
long       right;
{
  char            name[128];
  ctp_vexstr      result;
  char            lcl_buffer[128];
  int             Length;
  ctltype_list     *BaseType1;
  ctltype_list     *BaseType2;
  ctltype_list     *CtlType;

  result.IDENT = NULL;
  result.VEX   = NULL;
  result.WIDTH = 0;
  result.AGGREG = 0;

  switch (oper)
  {
    case VEX_CONCAT :
      if ((expr1.VEX == NULL) || (expr2.VEX == NULL))
        my_ctp_toolbug (4,"ctp_crtvex","CONC",0);
      else
      {
        if (expr1.VEX == expr2.VEX)
          my_ctp_toolbug (16,"ctp_crtvex",NULL,0);
        else
        {
          result.VEX = createvexbinexpr( VEX_CONCAT, 1, expr1.VEX, expr2.VEX );

          /* TYPE ET LEFT, RIGHT A VERIFIER !!! */

          result.WIDTH  = expr1.WIDTH + expr2.WIDTH;
	  result.TYPE   = VEX_TYPE_BIT_VECTOR;
	  result.SIGNED = expr1.SIGNED;

          result.VEX->WIDTH = result.WIDTH;
          result.VEX->LEFT  = 0;
          result.VEX->RIGHT = result.WIDTH - 1;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          expr1.VEX  = NULL;
          expr2.VEX  = NULL;
        }
      }
      break;

      case SUBVAR :
        if ( expr1.VEX == NULL )
          my_ctp_toolbug (2,"ctp_crtvex","SUBVAR VEX NIL ",0);
        else
        {
          result.VEX   = expr1.VEX;
          result.WIDTH = result.VEX->WIDTH;
	  result.TYPE  = expr1.TYPE;
	  result.SIGNED = expr1.SIGNED;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );
        }
        break;

      case NOPI :
        if ( expr1.IDENT == NULL )
          my_ctp_toolbug (2,"ctp_crtvex","NOPI IDENT = ) ",0);
        else
        {
          result.VEX = ctp_vexatom( expr1.IDENT, left, right ); 

          result.WIDTH = result.VEX->WIDTH;
	  result.TYPE  = expr1.TYPE;
	  result.SIGNED = expr1.SIGNED;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          if ( result.TYPE >= VEX_MAX_TYPE )
          {
            CtlType = ctp_getbasetype( result.TYPE );

            if ( ( CtlType != (ctltype_list *)0 ) &&
                 ( CtlType->CLASS == 'A'      ) )
            {
              if ( CtlType->LEFT > CtlType->RIGHT )
              {
                result.WIDTH = CtlType->LEFT - CtlType->RIGHT + 1;
              }
              else
              {
                result.WIDTH = CtlType->RIGHT - CtlType->LEFT + 1;
              }
            }
          }

          expr1.IDENT  = NULL;
        }
        break;

      case NOPS :

          if ( expr1.IDENT == NULL )
            my_ctp_toolbug (2,"ctp_crtvex","NOPS",0);
          else
          {
            if ( expr1.VEX == (vexexpr *)0 )
            {
              result.TYPE = ctp_tobin(lcl_buffer,expr1.IDENT,left,right);
              Length = strlen( lcl_buffer );

              if ( ( left  == -1 ) &&
                   ( right == -1 ) )
              {
                if ( ( result.TYPE == VEX_TYPE_INTEGER ) ||
                     ( result.TYPE == VEX_TYPE_NATURAL ) )
                {
                  left = Length - 1; right = 0;
                }
                else
                {
                  left = 0; right = Length - 1;
                }
              }

              if ( left == right )
              {
                name[ 0 ] = '\'';
                name[ 1 ] = lcl_buffer[ 0 ];
                name[ 2 ] = '\'';
                name[ 3 ] = '\0';

                result.VEX = createvexatombit( name );
              }
              else
              if ((left+right+1) != Length )
              {
                my_ctp_error(97,NULL);
              }
              else
              {
                sprintf( name, "\"%s\"", lcl_buffer );
                result.VEX = createvexatomvec( name, left, right );
              }

              if ( result.TYPE == VEX_TYPE_INTEGER ) result.SIGNED = 1;
              else                                   result.SIGNED = 0;
            }
            else
            {
              result.VEX    = dupvexexpr( expr1.VEX );
              result.TYPE   = expr1.TYPE;
              result.SIGNED = expr1.SIGNED;
            }
          }
  
          result.WIDTH = result.VEX->WIDTH;
          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

        break;

      case VEX_EVENT :
        if (expr1.VEX == NULL)
	  my_ctp_toolbug (3,"ctp_crtvex","EVENT",0);
        else
        {
          result.VEX = createvexunaryexpr( VEX_EVENT, 1, expr1.VEX );
          result.WIDTH = 1;
          result.TYPE  = VEX_TYPE_BIT;
          result.SIGNED = expr1.SIGNED;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          expr1.VEX = NULL;
        }
        break;

      case VEX_NOT :
        if (expr1.VEX == NULL)
	  my_ctp_toolbug (3,"ctp_crtvex","NOT",0);
        else
        {
          result.VEX = createvexunaryexpr( VEX_NOT, expr1.WIDTH, expr1.VEX );
          result.VEX->WIDTH = expr1.WIDTH;
          result.VEX->LEFT  = expr1.VEX->LEFT;
          result.VEX->RIGHT = expr1.VEX->RIGHT;

          if ( IsVexNodeVarWidth( expr1.VEX ) )
          {
            SetVexNodeVarWidth( result.VEX );
          }

          result.WIDTH = expr1.WIDTH;
          result.SIGNED = expr1.SIGNED;
	  result.TYPE  = expr1.TYPE;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          expr1.VEX = NULL;
        }
        break;
      case VEX_AF  :
      case VEX_AG  :
      case VEX_AX  :
      case VEX_EF  :
      case VEX_EG  :
      case VEX_EX  :
        if (expr1.VEX == NULL)
	  my_ctp_toolbug (3,"ctp_crtvex",getvexopername( oper ),0);
        else
        {
          if ( expr1.WIDTH != 1 )
          {
            my_ctp_error(38,NULL);
          }

          result.VEX    = createvexunaryexpr( oper, 1, expr1.VEX );
          result.WIDTH  = 1;
          result.TYPE   = VEX_TYPE_BOOLEAN;
          result.SIGNED = 0;

          expr1.VEX = NULL;
        }
        break;
      case VEX_AU    :
      case VEX_EU    :
      case VEX_EQUIV :
      case VEX_IMPLY :

        if ( (expr1.VEX == NULL) || 
             (expr2.VEX == NULL) )
	  my_ctp_toolbug (4,"ctp_crtvex","AU/EU/EQUIV or IMPLY",0);
        else
        {
          if ( ( expr1.WIDTH != 1 ) ||
               ( expr2.WIDTH != 1 ) )
          {
            my_ctp_error(38,NULL);
          }

          result.VEX    = createvexbinexpr( oper, 1, expr1.VEX, expr2.VEX );
          result.WIDTH  = 1;
          result.TYPE   = VEX_TYPE_BOOLEAN;
          result.SIGNED = 0;

          expr1.VEX = NULL;
          expr2.VEX = NULL;

        }
        break;
      case VEX_OTHERS :
        if ((expr1.VEX == NULL)||
            (! IsVexNodeAtom(expr1.VEX))||
            (! IsVexAtomLiteral( expr1.VEX )))
	  my_ctp_toolbug (3,"ctp_crtvex","OTHERS",0);
        else
        {
          result.VEX = createvexunaryexpr( VEX_OTHERS, expr1.WIDTH, expr1.VEX );
          result.VEX->WIDTH = expr1.WIDTH;
          result.VEX->LEFT  = expr1.VEX->LEFT;
          result.VEX->RIGHT = expr1.VEX->RIGHT;

          if ( IsVexNodeVarWidth( expr1.VEX ) )
          {
            SetVexNodeVarWidth( result.VEX );
          }

          result.WIDTH = expr1.WIDTH;
          result.SIGNED = expr1.SIGNED;
	  result.TYPE  = expr1.TYPE;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          expr1.VEX = NULL;
        }
        break;

      case VEX_NEG :
      case VEX_ABS :
        if (expr1.VEX == NULL)
	  my_ctp_toolbug (3,"ctp_crtvex","NEG/ABS",0);
        else
        {
          result.VEX = createvexunaryexpr( oper, 0, expr1.VEX );

          result.VEX->WIDTH = expr1.WIDTH;
          result.VEX->LEFT  = expr1.VEX->LEFT;
          result.VEX->RIGHT = expr1.VEX->RIGHT;

          if ( IsVexNodeVarWidth( expr1.VEX ) )
          {
            SetVexNodeVarWidth( result.VEX );
          }

          result.WIDTH = expr1.WIDTH;
	  result.TYPE  = expr1.TYPE;

          if ( oper == VEX_NEG )
          {
            result.SIGNED = 1;
            SetVexNodeSigned( result.VEX );
          }

          expr1.VEX = NULL;
        }
        break;

      case VEX_EQ :
      case VEX_NE :

        if ( (expr1.VEX == NULL) || 
             (expr2.VEX == NULL) )
	  my_ctp_toolbug (4,"ctp_crtvex","EQ or NE",0);
        else
        {
          BaseType1 = ctp_getbasetype( expr1.TYPE );
          BaseType2 = ctp_getbasetype( expr2.TYPE );

          if ( ( BaseType1->INDEX != VEX_TYPE_INTEGER  ) &&
               ( BaseType2->INDEX != VEX_TYPE_INTEGER  ) )
          {
            if ( ( expr1.WIDTH != expr2.WIDTH       ) &&
                 ( ! IsVexNodeVarWidth( expr1.VEX ) ) &&
                 ( ! IsVexNodeVarWidth( expr2.VEX ) ) )
            {
              my_ctp_error(38,NULL);
            }
          }

          result.VEX = createvexbinexpr( oper, 1, expr1.VEX, expr2.VEX );
          result.WIDTH = 1;
          result.TYPE  = VEX_TYPE_BIT;
          result.SIGNED = 0;

          expr1.VEX = NULL;
          expr2.VEX = NULL;
        }
        break;

      case VEX_LT :
      case VEX_LE :
      case VEX_GT :
      case VEX_GE :

        if ((expr1.VEX == NULL) || (expr2.VEX == NULL))
          my_ctp_toolbug (4,"ctp_crtvex","LT, LE, GT or GE",0);
        else
        {
          BaseType1 = ctp_getbasetype( expr1.TYPE );
          BaseType2 = ctp_getbasetype( expr2.TYPE );

          if ( ( BaseType1->INDEX != VEX_TYPE_INTEGER  ) &&
               ( BaseType2->INDEX != VEX_TYPE_INTEGER  ) )
          {
            if ( ( expr1.WIDTH != expr2.WIDTH ) &&
                 ( ! IsVexNodeVarWidth( expr1.VEX ) ) &&
                 ( ! IsVexNodeVarWidth( expr2.VEX ) ) )
            {
              my_ctp_error (38,NULL);
            }
          }

          result.VEX = createvexbinexpr( oper, 1, expr1.VEX, expr2.VEX );
          result.WIDTH = 1;
          result.TYPE  = VEX_TYPE_BIT;
          result.SIGNED = 0;

          expr1.VEX = NULL;
          expr2.VEX = NULL;
        }

      break;

      case VEX_AND  :
      case VEX_NAND :
      case VEX_OR   :
      case VEX_NOR  :
      case VEX_XOR  :
      case VEX_NXOR :

        if ((expr1.VEX == NULL) || (expr2.VEX == NULL))
	  my_ctp_toolbug (4,"ctp_crtvex","logical operators",0);
        else
        {
          if ( expr1.WIDTH != expr2.WIDTH )
          {
            my_ctp_error (38,NULL);
          }

          result.VEX = createvexbinexpr( oper, 1, expr1.VEX, expr2.VEX );
          result.VEX->WIDTH = expr1.WIDTH;
          result.VEX->LEFT  = expr1.VEX->LEFT;
          result.VEX->RIGHT = expr1.VEX->RIGHT;
          result.TYPE       = expr1.TYPE;
          result.WIDTH      = expr1.WIDTH;
          result.SIGNED     = 0;

          if ( IsVexNodeVarWidth( expr1.VEX ) )
          {
            SetVexNodeVarWidth( result.VEX );
          }

          expr1.VEX = NULL;
          expr2.VEX = NULL;
        }
        break;

      case VEX_IFT  :

        if ((expr1.VEX == NULL) || (expr2.VEX == NULL))
	  my_ctp_toolbug (4,"ctp_crtvex","logical operators",0);
        else
        {
          result.VEX = createvexbinexpr( oper, 1, expr1.VEX, expr2.VEX );
          result.VEX->WIDTH = expr2.WIDTH;
          result.VEX->LEFT  = expr2.VEX->LEFT;
          result.VEX->RIGHT = expr2.VEX->RIGHT;
          result.TYPE       = expr2.TYPE;
          result.WIDTH      = expr2.WIDTH;
          result.SIGNED     = 0;

          if ( IsVexNodeVarWidth( expr2.VEX ) )
          {
            SetVexNodeVarWidth( result.VEX );
          }


          expr1.VEX = NULL;
          expr2.VEX = NULL;
        }
        break;

      case VEX_ADD  :
      case VEX_SUB  :
      case VEX_MUL  :
      case VEX_DIV  :
      case VEX_MOD  :
      case VEX_REM  :
      case VEX_EXP  :

        if ((expr1.VEX == NULL) || (expr2.VEX == NULL))
	  my_ctp_toolbug (4,"ctp_crtvex","arithmetic operators",0);
        else
        {
          int Width;

          result.VEX = createvexbinexpr( oper, 1, expr1.VEX, expr2.VEX );

          if ((oper == VEX_ADD) ||
              (oper == VEX_SUB))
          {
            if ( expr2.WIDTH > expr1.WIDTH ) Width = expr2.WIDTH;
            else                             Width = expr1.WIDTH;
          }
          else
          if (oper == VEX_MUL)
          {
            if ( ( ( expr1.TYPE == VEX_TYPE_INTEGER ) ||
                   ( expr1.TYPE == VEX_TYPE_NATURAL ) ) &&
                 ( ( expr2.TYPE == VEX_TYPE_INTEGER ) ||
                   ( expr2.TYPE == VEX_TYPE_NATURAL ) ) )
            {
              if ( expr1.WIDTH > expr2.WIDTH ) Width = expr1.WIDTH;
              else                             Width = expr2.WIDTH;
            }
            else
            if ( ( expr1.TYPE == VEX_TYPE_INTEGER ) ||
                 ( expr1.TYPE == VEX_TYPE_NATURAL ) )
            {
              Width = expr2.WIDTH * 2;
            }
            else
            if ( ( expr2.TYPE == VEX_TYPE_INTEGER ) ||
                 ( expr2.TYPE == VEX_TYPE_NATURAL ) )
            {
              Width = expr1.WIDTH * 2;
            }
            else
            {
              Width = expr1.WIDTH + expr2.WIDTH;
            }
          }
          else
          if (oper == VEX_DIV)
          {
            Width = expr1.WIDTH;
          }
          else
          if (oper == VEX_EXP)
          {
            Width = expr1.WIDTH * expr2.WIDTH;
          }
          else /* VEX_MOD VEX_REM */
          {
            Width = expr2.WIDTH;
          }

          result.VEX->WIDTH = Width;
          result.VEX->LEFT  = Width - 1;
          result.VEX->RIGHT = 0;
          result.TYPE       = expr1.TYPE;
          result.WIDTH      = expr1.WIDTH;
          result.SIGNED     = expr1.SIGNED || expr2.SIGNED;

          if ( ( IsVexNodeVarWidth( expr1.VEX ) ) ||
               ( IsVexNodeVarWidth( expr2.VEX ) ) )
          {
            SetVexNodeVarWidth( result.VEX );
          }

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          expr1.VEX = NULL;
          expr2.VEX = NULL;
        }
        break;

      case EMPTYOP :
          result.VEX = NULL;
      break;

      case VEX_TO :
      case VEX_DOWNTO :
        if (expr1.VEX == NULL)
	  my_ctp_toolbug (3,"ctp_crtvex","VEX_SLICE",0);
        else
        {
          if ( right == 0 )
          {
            result.VEX = createvexbinexpr( VEX_INDEX, 1, expr1.VEX, 
                      dupvexexpr((vexexpr *)left) );
          }
          else
          {
            result.VEX = createvexternaryexpr( oper, 1, expr1.VEX, 
                      dupvexexpr((vexexpr *)left), dupvexexpr((vexexpr *)right) );

            SetVexNodeVarWidth( result.VEX );
          }

          /*
          result.VEX->WIDTH = expr1.WIDTH;
          result.VEX->LEFT  = expr1.VEX->LEFT;
          result.VEX->RIGHT = expr1.VEX->RIGHT;
          */

          result.WIDTH  = result.VEX->WIDTH;
	  result.TYPE   = expr1.TYPE;
          result.SIGNED = expr1.SIGNED;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          expr1.VEX = NULL;
        }
      break;
      default : my_ctp_toolbug (1,"ctp_crtvex","Default",0);
    }

    return (result);
}

/* ###--------------------------------------------------------------### */
/* function	: ctp_numtobin						*/
/* description	: transform a ENUMERATE 		*/
/*		  in a string of '0' and '1's				*/
/* ###--------------------------------------------------------------### */
long ctp_bintonum(str)
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
/* function	: ctp_numtobin						*/
/* description	: transform a ENUMERATE 		*/
/*		  in a string of '0' and '1's				*/
/* ###--------------------------------------------------------------### */
char *ctp_numtobin(num)
long num;

{
   char val[256]; 
   char *str;
	long res,i=1;

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

/*------------------------------------------------------------*/
/* function : ctp_intSize, compute the number of bits to      */
/*            store an integer value, given the left and right */
/*------------------------------------------------------------*/

int ctp_intSize( Left, Right )

  long Left;
  long Right;
{
  return( getvexintervalnumbit( Left, Right ) );
}


/* ###--------------------------------------------------------------### */
/* function	: ctp_tobin						*/
/* description	: transform a StringLit, BitStringLit or CharacterLit	*/
/*		  AbstractLit in a string of '0' and '1's		*/
/* ###--------------------------------------------------------------### */

static char *long_bin(s, i, n)
char *s;
long i, n;
{
long j, k;

	*(s + n) = '\0'; /* end of string */
	for (j = 1, k = 0; k < n; k++, j <<= 1)
		*(s + n - k - 1) = (i & j) ? '1' : '0';
	return s;
} 

int ctp_tobin (trg,src,left,right)

char *trg;
char *src;
int   left;
int   right;

{
  char base;
  int  indx;
  int  j = 0;
  int  type = -1;
  char lcl_trg[256];
  unsigned long  Value;
  unsigned long  Mask;
  char *endp;

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
      if ( (src[0] != '\'') && 
           (src[0] != '"' ) && 
           (src[0] != '_' ) && 
           (src[0] != '%' ) )
      {
        base = src[0];
        indx = 2;

        if ( ( ( base >= '0' ) &&
               ( base <= '9' ) ) ||
             (   base == '-'   ) )
        {
          indx = 1;
          base = 'D';
        }
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

            type = VEX_TYPE_BIT_VECTOR;

          while ((lcl_trg[j] = src[indx]) != '\0')
            {
            switch (src[indx])
              {
              case 'd':	lcl_trg[j]='-';

              case 'u':
              case 'x':
              case '0':
              case '1':
              case 'z':
              case 'w':
              case 'l':
              case 'h':
              case '-':	/* Beware Not VHDL	*/
                j++; break;

              case '%' :
              case '"' :
              case '\'':
              case '_' :
                break;

	    default : for ( indx = 1; src[ indx ] != '\0'; indx++ )
                      {
                        trg[ indx - 1] = src[ indx ];
                      }
                      trg[ indx - 2 ] = '\0';

                      return( VEX_TYPE_STRING );
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
	        j -= 3; type = -1; my_ctp_error(73,src);
              }
            indx++;
            }

            type = VEX_TYPE_BIT_VECTOR;

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
	        j -= 4; type = -1; my_ctp_error(73,src);
              }
            indx++;
          }
          type = VEX_TYPE_BIT_VECTOR;
          break;

        case 'D' : 

           Value = strtol( src, &endp, 10);
           if (*endp == '#') /* This is base specifier in the n#num# form */
           {
             (void)long_bin(lcl_trg, strtol(endp + 1, NULL, Value), 32);
             type = VEX_TYPE_INTEGER;
           }
           else
           {
  /*\
             if ( src[ 0 ] == '-' ) lcl_trg[ 0 ] = '1';
             else                   lcl_trg[ 0 ] = '0';

             j = 1;
  \*/

             type = VEX_TYPE_NATURAL;

             if ( Value != 0 )
             {
               Mask = 1L << 31;

               if ( src[ 0 ] == '-' )
               {
                 while ( ( Value & Mask ) != 0 )
                 {
                   Mask = Mask >> 1;
                 }

                 if ( Mask == 0 ) Mask = 0x1;
                 else             Mask = Mask << 1;

                 type = VEX_TYPE_INTEGER;
               }
               else
               {
                 while ( ( Value & Mask ) == 0 )
                 {
                   Mask = Mask >> 1;
                 }
               }
             }
             else
             {
               Mask = 0x1;
             }

             do
             {
               if ( Value & Mask ) lcl_trg[ j ] = '1';
               else                lcl_trg[ j ] = '0';

               j++;

               Mask = Mask >> 1;
             }
             while ( Mask != 0 );

             lcl_trg[ j ] = '\0';
           }

          break;

        default :

          type = -1;
          my_ctp_toolbug (17,"ctp_tobin",NULL,base);
      }

      strcpy (trg, lcl_trg);
    }
  }

  return (type);
}

static long ctp_vexliteval( Vex )

  vexexpr *Vex;
{
  long  Value;

  if ( evalvexatomlong( Vex, &Value ) )
  {
    CtpEvalError = 1;
  }

  return( Value );
}

static long ctp_vexeval( Vex )

  vexexpr *Vex;
{
  chain_list *ScanOper;
  long        Value;
  long        Operand;
  long        Oper;

  Value = 1;

  if ( IsVexNodeOper( Vex ) )
  {
    Oper     = GetVexOperValue( Vex );
    ScanOper = Vex->OPERAND;
    Vex      = GetVexOperand( ScanOper );
    ScanOper = ScanOper->NEXT;
    Value    = ctp_vexeval( Vex );

    if ( Oper == VEX_NEG )
    {
      Value = - Value;
    }
    else
    if ( Oper == VEX_ABS )
    {
      if ( Value < 0 ) Value = - Value;
    }
    else
    if ( ScanOper == (chain_list *)0 )
    {
      CtpEvalError = 1;
    }

    while ( ScanOper != (chain_list *)0 )
    {
      Vex     = GetVexOperand( ScanOper );
      Operand = ctp_vexeval( Vex );

      if ( CtpEvalError ) break;

      switch ( Oper )
      {
        case VEX_ADD : Value += Operand;
        break;

        case VEX_SUB : Value -= Operand;
        break;

        case VEX_MUL : Value *= Operand;
        break;

        case VEX_DIV : if ( Operand == 0 ) CtpEvalError = 1;
                       else                Value /= Operand;
        break;

        default      : CtpEvalError = 1; 
        break;
      }

      ScanOper = ScanOper->NEXT;
    }
  }
  else
  if ( ( IsVexNodeAtom( Vex )    ) &&
       ( IsVexAtomLiteral( Vex ) ) )
  {
    Value = ctp_vexliteval( Vex );
  }
  else
  {
    CtpEvalError = 1;
  }

  return( Value );
}

int ctp_vextonum( Vex, PValue )

  vexexpr *Vex;
  long    *PValue;
{
  CtpEvalError = 0;
  *PValue = ctp_vexeval( Vex );

# ifdef DEBUG
fprintf( stdout, "ctp_vextonum " );
viewvexexprbound( Vex );
fprintf( stdout, "\n%ld %d\n", *PValue, CtpEvalError );
# endif

  return( CtpEvalError );
}
