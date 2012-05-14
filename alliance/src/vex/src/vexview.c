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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Vex                           |
|                                                             |
| File    :                   vexview.c                       |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Modified:               Frédéric Pétrot                     |
|           Add support for Verilog expressions               |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "vex.h"

# include <stdio.h>
# include <string.h>
# include "vexview.h"
# include "vexerror.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static FILE  *VexViewFile     = (FILE *)0;
  static int    VexViewLength   = 0;
  static void (*VexViewFunction)();

  static char *VexStringBuffer = (char *)0;
  static int   VexStringSize   = 0;
  static int   VexStringLength = 0;
  static int   VexViewBound    = 0;
  static int   VexViewLanguage = VEX_VIEW_VHDL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        View  Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      View Local Vex File                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvexfile( String, Length )

  char *String;
  int   Length;
{
  VexViewLength += Length;

  if ( VexViewLength >= VEX_VIEW_MAX_LENGTH )
  {
    fprintf( VexViewFile, "\n" );
    VexViewLength = 0;
  }

  if ( *String )
  {
     fwrite( String, sizeof( char ), Length, VexViewFile );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    View Local Vex String                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvexstr( String, Length )

  char *String;
  int   Length;
{
  char *Buffer;

  if ( ( VexStringLength + Length ) >= VexStringSize )
  {
    VexStringSize = VexStringSize << 1;
    Buffer = (char *)autallocblock( VexStringSize );
    strcpy( Buffer, VexStringBuffer );
    autfreeblock( VexStringBuffer );
    VexStringBuffer = Buffer;
  }

  VexViewLength += Length;

  if ( VexViewLength >= VEX_VIEW_MAX_LENGTH )
  {
    VexViewLength = 0;
    loc_viewvexstr( "\n", 1 );
  }

  strcpy( VexStringBuffer + VexStringLength, String );
  VexStringLength += Length;
}

/*------------------------------------------------------------\
|                                                             |
|                    View Local Expr Vhdl All                 |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvexexprbound( Expr )

  vexexpr *Expr;
{
  char  Buffer[ 32 ];
  char  Signed;

  if ( IsVexNodeSigned( Expr ) ) Signed = 's';
  else                           Signed = 'u';

  if ( IsVexNodeVarWidth( Expr ) )
  {
    sprintf( Buffer, "{?:?:?:%c}", Signed );
  }
  else
  {
    sprintf( Buffer, "{%d:%d:%d:%c}", Expr->LEFT, Expr->RIGHT, Expr->WIDTH, Signed  );
  }

  (*VexViewFunction)( Buffer, strlen( Buffer ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    View Local Expr Vhdl Atom                |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvexatom( Expr )

  vexexpr *Expr;
{
  char *String;
  char  Buffer[ 32 ];
  int   Index;

  switch (VexViewLanguage)
  {
    case VEX_VIEW_VHDL:
      String = GetVexAtomValue( Expr );
      (*VexViewFunction)( String, strlen( String ) );
      break;

    case VEX_VIEW_VERILOG:
      String = GetVexAtomValue( Expr );
      if ( IsVexAtomLiteral ( Expr ) )
      {
         sprintf( Buffer, "%d'b", Expr->WIDTH );
         (*VexViewFunction)( Buffer, strlen( Buffer ) );

         for ( Index = 1;
               Index < strlen ( String ) - 1;
               Index++ )
         {
            if ( String[ Index ] == '-' )
            {
               (*VexViewFunction)( "?", 1 );
            }
            else
            {
               (*VexViewFunction)( String + Index, 1 );
            }
         }
      }
      else
      {
         (*VexViewFunction)( String, strlen( String ) );
      }
      break;
  }

  if ( VexViewBound )
  {
    loc_viewvexexprbound( Expr );
  }

  if ( (   IsVexAtomVector( Expr )  ) &&
       ( ! IsVexAtomLiteral( Expr ) ) )
  {
    if ( IsVexNodeVarWidth( Expr ) )
    {
      switch (VexViewLanguage)
      {
        case VEX_VIEW_VHDL:
          strcpy( Buffer, "(??)" );
          break;

        case VEX_VIEW_VERILOG:
          strcpy( Buffer, "[??]" );
          break;
      }
    }
    else
    if ( Expr->LEFT == Expr->RIGHT )
    {
      switch (VexViewLanguage)
      {
        case VEX_VIEW_VHDL:
          sprintf( Buffer, "(%d)", Expr->LEFT );
          break;

        case VEX_VIEW_VERILOG:
          sprintf( Buffer, "[%d]", Expr->LEFT );
          break;
      }
    }
    else
    {
      switch (VexViewLanguage)
      {
        case VEX_VIEW_VHDL:
          if ( Expr->LEFT <  Expr->RIGHT )
          {
            sprintf( Buffer, "(%d to %d)", Expr->LEFT, Expr->RIGHT );
          }
          else
          {
            sprintf( Buffer, "(%d downto %d)", Expr->LEFT, Expr->RIGHT );
          }
          break;

        case VEX_VIEW_VERILOG:
          sprintf( Buffer, "[%d:%d]", Expr->LEFT, Expr->RIGHT );
          break;
      }
    }

    (*VexViewFunction)( Buffer, strlen( Buffer ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    View Local Expr Vhdl                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewvex( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  long        Oper;
  char       *String;
  int         Length;

  if ( IsVexNodeAtom( Expr ) )
  {
    loc_viewvexatom( Expr );
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper   = GetVexOperValue( Expr );
    String = (char *)0;

    switch (VexViewLanguage)
    {
      case VEX_VIEW_VHDL:
        String = getvexoperuppername( Oper );
        break;

      case VEX_VIEW_VERILOG:
        String = getvexoperverilogname( Oper );
        break;
    }

    if ( String == (char *)0 )
    {
      vexerror( VEX_OPERATOR_ERROR, Oper );
    }

    Length = strlen( String );

    if ( VexViewBound )
    {
      loc_viewvexexprbound( Expr );
    }

    if ( isvexunaryoper( Oper ) )
    {
      if ( ( Oper == VEX_EVENT  ) ||
           ( Oper == VEX_DRIVER ) )
      {
        (*VexViewFunction)( "(", 1 );

        loc_viewvex( GetVexOperand( Expr->OPERAND ) );

        (*VexViewFunction)( "'", 1 );
        (*VexViewFunction)( String, Length );
      }
      else
      {
        (*VexViewFunction)( String, Length );
        (*VexViewFunction)( "(", 1 );

        loc_viewvex( GetVexOperand( Expr->OPERAND ) );
      }
    }
    else
    {
      if ( ( VexViewBound              ) || 
           ( isvexpositiveoper( Oper ) ) )
      {
        if ( Oper == VEX_CONCAT && VexViewLanguage == VEX_VIEW_VERILOG )
        {
          (*VexViewFunction)( "{", 1 );
        }
        else
        {
          (*VexViewFunction)( "(", 1 );
        }
      }
      else
      {
        switch (VexViewLanguage)
        {
          case VEX_VIEW_VHDL:
            (*VexViewFunction)( "NOT(", 4 );
            String = getvexoperuppername( getvexnotoper( Oper ) );
            break;

          case VEX_VIEW_VERILOG:
            (*VexViewFunction)( "~(", 2 );
            String = getvexoperverilogname( getvexnotoper( Oper ) );
            break;
        }
        Length = strlen( String );
      }

      for ( ScanOper  = Expr->OPERAND;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        loc_viewvex( GetVexOperand( ScanOper ) );
        
        if ( ScanOper->NEXT != (chain_list *)0 )
        {
          (*VexViewFunction)( " ", 1 );
          (*VexViewFunction)( String, Length );
          (*VexViewFunction)( " ", 1 );
        }
      }
    }

    if ( Oper == VEX_CONCAT && VexViewLanguage == VEX_VIEW_VERILOG )
    {
      (*VexViewFunction)( "}", 1 );
    }
    else
    {
      (*VexViewFunction)( ")", 1 );
    }
  }
  else
  {
    String = GetVexFuncValue( Expr );
    Length = strlen( String );

    if ( VexViewBound )
    {
      if ( getvexstdfuncid( String ) != -1 )
      {
        (*VexViewFunction)( "(std)", 5 );
      }

      loc_viewvexexprbound( Expr );
    }

    (*VexViewFunction)( String, Length );
    (*VexViewFunction)( "(", 1 );

    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      loc_viewvex( GetVexOperand( ScanOper ) );
       
      if ( ScanOper->NEXT != (chain_list *)0 )
      {
        (*VexViewFunction)( ",", 1 );
      }
    }

    (*VexViewFunction)( ")", 1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    View Vex Expr File                       |
|                                                             |
\------------------------------------------------------------*/

void viewvexexprfile( VexFile, Expr )

  FILE    *VexFile;
  vexexpr *Expr;
{
  VexViewFile     = VexFile;
  VexViewLength   = 0;
  VexViewFunction = loc_viewvexfile;

  if ( Expr != (vexexpr *)0 ) loc_viewvex( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vex Expr                       |
|                                                             |
\------------------------------------------------------------*/

void viewvexexpr( Expr )

  vexexpr *Expr;
{
  viewvexexprfile( stdout, Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vex Expr NewLine               |
|                                                             |
\------------------------------------------------------------*/

void viewvexexprln( Expr )

  vexexpr *Expr;
{
  viewvexexprfile( stdout, Expr );
  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Vex Expr Bound                 |
|                                                             |
\------------------------------------------------------------*/

void viewvexexprbound( Expr )

  vexexpr *Expr;
{
  VexViewBound = 1;
  viewvexexprfile( stdout, Expr );
  VexViewBound = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                      View Vex Expr Bound NewLine            |
|                                                             |
\------------------------------------------------------------*/

void viewvexexprboundln( Expr )

  vexexpr *Expr;
{
  VexViewBound = 1;
  viewvexexprfile( stdout, Expr );
  fprintf( stdout, "\n" );
  VexViewBound = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                    View Vex Expr String                     |
|                                                             |
\------------------------------------------------------------*/

char *viewvexexprstr( Expr )

  vexexpr *Expr;
{
  if ( VexStringBuffer == (char *)0 )
  {
    VexStringBuffer = (char *)autallocblock( VEX_STRING_BUFFER_SIZE );
    VexStringSize   = VEX_STRING_BUFFER_SIZE;
  }

  VexStringLength = 0;
  VexStringBuffer[ 0 ] = '\0';

  VexViewLength   = 0;
  VexViewFunction = loc_viewvexstr;

  if ( Expr != (vexexpr *)0 ) loc_viewvex( Expr );

  return( VexStringBuffer );
}

/*------------------------------------------------------------\
|                                                             |
|                 View Vex Expr String Bound                  |
|                                                             |
\------------------------------------------------------------*/

char *viewvexexprstrbound( Expr )

  vexexpr *Expr;
{
  VexViewBound = 1;
  viewvexexprstr( Expr );
  VexViewBound = 0;

  return( VexStringBuffer );
}

/*------------------------------------------------------------\
|                                                             |
|                       Set View Mode                         |
|                                                             |
\------------------------------------------------------------*/

void setvexviewmode( Mode )

  int Mode;
{
  VexViewLanguage = Mode;
}

/*------------------------------------------------------------\
|                                                             |
|                       Get View Mode                         |
|                                                             |
\------------------------------------------------------------*/

int getvexviewmode()
{
  return( VexViewLanguage );
}
