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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "vex.h"
#include "aut.h"
#include "vbh.h"

#include "vbl_bedef.h"
#include "vbl_bspec.h"
#include "vbl_util.h"

static long VblEvalError = 0;

/* ###--------------------------------------------------------------### */
/* function	: vbl_stostr						*/
/* description	: purge double % (percent) or Quote (") from string	*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

char *vbl_stostr(str)

char *str;

{
  char *resstr;
  long   i;
  long   j=0;

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
/* function     : vbl_crtvex                                            */
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
/* called func. : createvexatom , createvexoper, addvexqexpr , vbl_toolbug,	*/
/*		  vbl_error , addchain   , freechain			*/
/* ###--------------------------------------------------------------### */

vbtyp_list *vbl_getbasetype( long type )
{
  vbtyp_list *ScanType;

  for ( ScanType  = VBL_HEADFIG->BETYP;
        ScanType != (vbtyp_list *)0;
        ScanType  = ScanType->NEXT )
  {
    if ( ScanType->INDEX == type )
    {
      if ( ScanType->BASE != (vbtyp_list *)0 )
      {
        ScanType = ScanType->BASE;
      }

      break;
    }
  }

  return( ScanType );
}

vexexpr *vbl_dynamicvexatom( 

   char          *Name,
   long           Left,
   long           Right,
   unsigned char  Dynamic )
{
  vexexpr *Result;

  if ( Dynamic )
  {
    if ( Dynamic == VBL_UPTDFN ) Result = createvexoper( VEX_TO, 0 );
    else                         Result = createvexoper( VEX_DOWNTO, 0 );

    addvexqexpr( Result, createvexatomvec( Name, -1, -1 ) );
    addvexqexpr( Result, dupvexexpr( (vexexpr *)Left ) );
    addvexqexpr( Result, dupvexexpr( (vexexpr *)Right ) );
  }
  else
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

vbl_vexstr vbl_crtvex( 

short      oper,
vbl_vexstr expr1,
vbl_vexstr expr2,
long       left,
long       right,
unsigned char dynamic )
{
  char            name[128];
  vbl_vexstr      result;
  char            lcl_buffer[128];
  long             Length;
  vbtyp_list     *BaseType1;
  vbtyp_list     *BaseType2;
  vbtyp_list     *VbhType;

  result.IDENT  = NULL;
  result.VEX    = NULL;
  result.SIGNED = 0;
  result.TYPE   = 0;
  result.WIDTH  = 0;
  result.AGGREG = 0;

  switch (oper)
  {
    case VEX_CONCAT :
      if ((expr1.VEX == NULL) || (expr2.VEX == NULL))
        vbl_toolbug (4,"vbl_crtvex","CONC",0);
      else
      {
        if (expr1.VEX == expr2.VEX)
          vbl_toolbug (16,"vbl_crtvex",NULL,0);
        else
        {
          result.VEX = createvexbinexpr( VEX_CONCAT, 1, expr1.VEX, expr2.VEX );

          /* TYPE ET LEFT, RIGHT A VERIFIER !!! */

          result.WIDTH  = expr1.WIDTH + expr2.WIDTH;
	  result.TYPE   = VBH_TYPE_BIT_VECTOR;
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
          vbl_toolbug (2,"vbl_crtvex","SUBVAR VEX NIL ",0);
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
          vbl_toolbug (2,"vbl_crtvex","NOPI IDENT = ) ",0);
        else
        {
          result.VEX = vbl_dynamicvexatom( expr1.IDENT, left, right, dynamic ); 

          result.WIDTH = result.VEX->WIDTH;
	  result.TYPE  = expr1.TYPE;
	  result.SIGNED = expr1.SIGNED;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          if ( result.TYPE >= VBH_MAX_TYPE )
          {
            VbhType = vbl_getbasetype( result.TYPE );

            if ( ( VbhType != (vbtyp_list *)0 ) &&
                 ( VbhType->CLASS == 'A'      ) )
            {
              if ( VbhType->LEFT > VbhType->RIGHT )
              {
                result.WIDTH = VbhType->LEFT - VbhType->RIGHT + 1;
              }
              else
              {
                result.WIDTH = VbhType->RIGHT - VbhType->LEFT + 1;
              }
            }
          }

          expr1.IDENT  = NULL;
        }
        break;

      case NOPS :

          if ( expr1.IDENT == NULL )
            vbl_toolbug (2,"vbl_crtvex","NOPS",0);
          else
          {
            if ( expr1.VEX == (vexexpr *)0 )
            {
              result.TYPE = vbl_tobin(lcl_buffer,expr1.IDENT,left,right);
              Length = strlen( lcl_buffer );

              if ( ( left  == -1 ) &&
                   ( right == -1 ) )
              {
                if ( ( result.TYPE == VBH_TYPE_INTEGER ) ||
                     ( result.TYPE == VBH_TYPE_NATURAL ) )
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
                vbl_error(97,NULL);
              }
              else
              {
                sprintf( name, "\"%s\"", lcl_buffer );
                result.VEX = createvexatomvec( name, left, right );
              }

              if ( result.TYPE == VBH_TYPE_INTEGER ) result.SIGNED = 1;
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
	  vbl_toolbug (3,"vbl_crtvex","EVENT",0);
        else
        {
          result.VEX = createvexunaryexpr( VEX_EVENT, 1, expr1.VEX );
          result.WIDTH = 1;
          result.TYPE  = VBH_TYPE_BIT;
          result.SIGNED = expr1.SIGNED;

          if ( result.SIGNED ) SetVexNodeSigned( result.VEX );

          expr1.VEX = NULL;
        }
        break;

      case VEX_NOT :
        if (expr1.VEX == NULL)
	  vbl_toolbug (3,"vbl_crtvex","NOT",0);
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
      case VEX_OTHERS :
        if ((expr1.VEX == NULL)||
            (! IsVexNodeAtom(expr1.VEX))||
            (! IsVexAtomLiteral( expr1.VEX )))
	  vbl_toolbug (3,"vbl_crtvex","OTHERS",0);
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
	  vbl_toolbug (3,"vbl_crtvex","NEG/ABS",0);
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
	  vbl_toolbug (4,"vbl_crtvex","EQ or NE",0);
        else
        {
          BaseType1 = vbl_getbasetype( expr1.TYPE );
          BaseType2 = vbl_getbasetype( expr2.TYPE );

          if ( ( BaseType1->INDEX != VBH_TYPE_INTEGER  ) &&
               ( BaseType2->INDEX != VBH_TYPE_INTEGER  ) )
          {
            if ( ( expr1.WIDTH != expr2.WIDTH       ) &&
                 ( ! IsVexNodeVarWidth( expr1.VEX ) ) &&
                 ( ! IsVexNodeVarWidth( expr2.VEX ) ) )
            {
              vbl_error(38,NULL);
            }
          }

          result.VEX = createvexbinexpr( oper, 1, expr1.VEX, expr2.VEX );
          result.WIDTH = 1;
          result.TYPE  = VBH_TYPE_BIT;
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
          vbl_toolbug (4,"vbl_crtvex","LT, LE, GT or GE",0);
        else
        {
          BaseType1 = vbl_getbasetype( expr1.TYPE );
          BaseType2 = vbl_getbasetype( expr2.TYPE );

          if ( ( BaseType1->INDEX != VBH_TYPE_INTEGER  ) &&
               ( BaseType2->INDEX != VBH_TYPE_INTEGER  ) )
          {
            if ( ( expr1.WIDTH != expr2.WIDTH ) &&
                 ( ! IsVexNodeVarWidth( expr1.VEX ) ) &&
                 ( ! IsVexNodeVarWidth( expr2.VEX ) ) )
            {
              vbl_error (38,NULL);
            }
          }

          result.VEX = createvexbinexpr( oper, 1, expr1.VEX, expr2.VEX );
          result.WIDTH = 1;
          result.TYPE  = VBH_TYPE_BIT;
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
	  vbl_toolbug (4,"vbl_crtvex","logical operators",0);
        else
        {
          if ( expr1.WIDTH != expr2.WIDTH )
          {
            vbl_error (38,NULL);
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
	  vbl_toolbug (4,"vbl_crtvex","logical operators",0);
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
	  vbl_toolbug (4,"vbl_crtvex","arithmetic operators",0);
        else
        {
          long Width;

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
            if ( ( ( expr1.TYPE == VBH_TYPE_INTEGER ) ||
                   ( expr1.TYPE == VBH_TYPE_NATURAL ) ) &&
                 ( ( expr2.TYPE == VBH_TYPE_INTEGER ) ||
                   ( expr2.TYPE == VBH_TYPE_NATURAL ) ) )
            {
              if ( expr1.WIDTH > expr2.WIDTH ) Width = expr1.WIDTH;
              else                             Width = expr2.WIDTH;
            }
            else
            if ( ( expr1.TYPE == VBH_TYPE_INTEGER ) ||
                 ( expr1.TYPE == VBH_TYPE_NATURAL ) )
            {
              Width = expr2.WIDTH * 2;
            }
            else
            if ( ( expr2.TYPE == VBH_TYPE_INTEGER ) ||
                 ( expr2.TYPE == VBH_TYPE_NATURAL ) )
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
	  vbl_toolbug (3,"vbl_crtvex","VEX_SLICE",0);
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
      default : vbl_toolbug (1,"vbl_crtvex","Default",0);
    }

    return (result);
}

/* ###--------------------------------------------------------------### */
/* function	: vbl_numtobin						*/
/* description	: transform a ENUMERATE 		*/
/*		  in a string of '0' and '1's				*/
/* ###--------------------------------------------------------------### */
long vbl_bintonum(str)
char* str;

{
	long res,i=0;

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
/* function	: vbl_numtobin						*/
/* description	: transform a ENUMERATE 		*/
/*		  in a string of '0' and '1's				*/
/* ###--------------------------------------------------------------### */
char *vbl_numtobin( long num )

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
/* function : vbl_codeSize, compute the number of bits to     */
/*            store a user-type value, given the scope width  */
/*------------------------------------------------------------*/

long vbl_codeSize( long Size )
{
  return( getvexintnumbit( Size ) );
}

/*------------------------------------------------------------*/
/* function : vbl_intSize, compute the number of bits to      */
/*            store an integer value, given the left and right */
/*------------------------------------------------------------*/

long vbl_intSize( long Left, long Right )
{
  return( getvexintervalnumbit( Left, Right ) );
}


/* ###--------------------------------------------------------------### */
/* function     : vbl_enumbitstring                                          */
/* description  : transform a ENUMERATE                 */
/*                in a string of '0' and '1' and '-'                    */
/* ###--------------------------------------------------------------### */
char *vbl_enumbitstring(
long num,
long size,
long numbit )

{
  char  val[40];
  long   i;
  long  shiftnum;
  long  sup_bound; /*         (1 << (numbit-1))  */
  long  inf_bound; /* size & ~(1 << (numbit-1))  */

  if ( numbit == 1 ) 
  {
    if ( num == 1 ) return ( "'1'" ); 
    else            return ( "'0'" );
  }

  sup_bound = 1 << (numbit - 1);
  inf_bound = size & ~sup_bound;

  val[0] = '"';
  val[numbit+1] = '"';
  val[numbit+2] = '\0';

  shiftnum = num;

  for (i=1; i<=numbit; i++)
  {
    val[i] = (shiftnum & 1) + '0';
    shiftnum = shiftnum>>1 ;
  }

  if ( ( num >= inf_bound ) &&
       ( num <  sup_bound ) )
  {
  /* ICI LUDO !!
    val[numbit] = '-';
   */
    val[numbit] = '0';
  }

  return( namealloc( val ) );
}

/* ###--------------------------------------------------------------### */
/* function	: vbl_tobin						*/
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

long vbl_tobin (

char *trg,
char *src,
long   left,
long   right )

{
  char base;
  long  indx;
  long  j = 0;
  long  type = -1;
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

            type = VBH_TYPE_BIT_VECTOR;

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

                      return( VBH_TYPE_STRING );
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
	        j -= 3; type = -1; vbl_error(73,src);
              }
            indx++;
            }

            type = VBH_TYPE_BIT_VECTOR;

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
	        j -= 4; type = -1; vbl_error(73,src);
              }
            indx++;
          }
          type = VBH_TYPE_BIT_VECTOR;
          break;

        case 'D' : 

           Value = strtol( src, &endp, 10);
           if (*endp == '#') /* This is base specifier in the n#num# form */
           {
             (void)long_bin(lcl_trg, strtol(endp + 1, NULL, Value), 32);
             type = VBH_TYPE_INTEGER;
           }
           else
           {
  /*\
             if ( src[ 0 ] == '-' ) lcl_trg[ 0 ] = '1';
             else                   lcl_trg[ 0 ] = '0';

             j = 1;
  \*/

             type = VBH_TYPE_NATURAL;

             if ( Value != 0 )
             {
               Mask = 1U << 31;

               if ( src[ 0 ] == '-' )
               {
                 while ( ( Value & Mask ) != 0 )
                 {
                   Mask = Mask >> 1;
                 }

                 if ( Mask == 0 ) Mask = 0x1;
                 else             Mask = Mask << 1;

                 type = VBH_TYPE_INTEGER;
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
          vbl_toolbug (17,"vbl_tobin",NULL,base);
      }

      strcpy (trg, lcl_trg);
    }
  }

  return (type);
}

static long vbl_vexliteval( Vex )

  vexexpr *Vex;
{
  long  Value;

  if ( evalvexatomlong( Vex, &Value ) )
  {
    VblEvalError = 1;
  }

  return( Value );
}

static long vbl_vexeval( Vex )

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
    Value    = vbl_vexeval( Vex );

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
      VblEvalError = 1;
    }

    while ( ScanOper != (chain_list *)0 )
    {
      Vex     = GetVexOperand( ScanOper );
      Operand = vbl_vexeval( Vex );

      if ( VblEvalError ) break;

      switch ( Oper )
      {
        case VEX_ADD : Value += Operand;
        break;

        case VEX_SUB : Value -= Operand;
        break;

        case VEX_MUL : Value *= Operand;
        break;

        case VEX_DIV : if ( Operand == 0 ) VblEvalError = 1;
                       else                Value /= Operand;
        break;

        default      : VblEvalError = 1; 
        break;
      }

      ScanOper = ScanOper->NEXT;
    }
  }
  else
  if ( ( IsVexNodeAtom( Vex )    ) &&
       ( IsVexAtomLiteral( Vex ) ) )
  {
    Value = vbl_vexliteval( Vex );
  }
  else
  {
    VblEvalError = 1;
  }

  return( Value );
}

long vbl_vextonum( vexexpr *Vex, long    *PValue )
{
  VblEvalError = 0;
  *PValue = vbl_vexeval( Vex );

# ifdef DEBUG
fprintf( stdout, "vbl_vextonum " );
viewvexexprbound( Vex );
fprintf( stdout, "\n%ld %d\n", *PValue, VblEvalError );
# endif

  return( VblEvalError );
}
