/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| Tool    :                     VASY                          |
|                                                             |
| File    :                 vasy_drvvex.c                     |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "rtn.h"

# include "vasy_debug.h"
# include "vasy_error.h"
# include "vasy_simprtl.h"
# include "vasy_shared.h"
# include "vasy_drvvex.h"

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
/*------------------------------------------------------------\
|                                                             |
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/

  static FILE        *VasyFile         = (FILE        *)0;
  static rtlfig_list *VasyFigure       = (rtlfig_list *)0; 
  static long         VasyDriveLength  = 0;
  static int          VasyVerilog      = 0;

  static int          VasyDriveVexAtomLock = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     VasyDriveVexFile                        |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVexFile( String, Length )

  char *String;
  int   Length;
{
  VasyDriveLength += Length;

  if ( ( ! VasyDriveVexAtomLock                   ) &&
       ( VasyDriveLength >= VASY_DRIVE_VEX_LENGTH ) )
  {
    fprintf( VasyFile, "\n" );
    VasyDriveLength = 0;
  }

  if ( *String )
  {
    fwrite( String, sizeof(char), Length, VasyFile );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyDriveVexAtom                        |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVexAtom( Expr )

  vexexpr *Expr;
{
  rtldecl_list *RtlDeclar;
  vexexpr      *RtlAtom;
  char         *String;
  char          Buffer[32];
  int           Index;
  int           Length;

  VasyDriveVexAtomLock = 1;

  String = GetVexAtomValue( Expr );
  Length = strlen( String );

  if ( ( VasyVerilog               ) &&
       ( IsVexAtomLiteral ( Expr ) ) )
  {
    sprintf( Buffer, "%d'b", Expr->WIDTH );
    VasyDriveVexFile( Buffer, strlen( Buffer ) );

    for ( Index = 1; Index < Length - 1; Index++ )
    {
      if ( ( String[ Index ] == '-' ) ||
           ( String[ Index ] == 'd' ) ) VasyDriveVexFile( "?", 1 );
      else                              VasyDriveVexFile( String + Index, 1 );
    }
  }
  else
  {
    VasyDriveVexFile( String, Length );
  }

  if ( (   IsVexAtomVector( Expr )  ) &&
       ( ! IsVexAtomLiteral( Expr ) ) )
  {
    if ( Expr->LEFT == Expr->RIGHT )
    {
      if ( VasyVerilog ) sprintf( Buffer, "[%d]", Expr->LEFT );
      else               sprintf( Buffer, "(%d)", Expr->LEFT );
    }
    else
    {
      RtlDeclar = searchrtldecl( VasyFigure, String );

      if ( RtlDeclar != (rtldecl_list *)0 )
      {
        RtlAtom = RtlDeclar->VEX_ATOM;

        if ( ( Expr->LEFT  == RtlAtom->LEFT   ) &&
             ( Expr->RIGHT == RtlAtom->RIGHT  ) ) return;
      }

      if ( VasyVerilog )
      {
        sprintf( Buffer, "[%d:%d]", Expr->LEFT, Expr->RIGHT );
      }
      else
      if ( Expr->LEFT <  Expr->RIGHT )
      {
        sprintf( Buffer, "(%d to %d)", Expr->LEFT, Expr->RIGHT );
      }
      else
      {
        sprintf( Buffer, "(%d downto %d)", Expr->LEFT, Expr->RIGHT );
      }
    }

    VasyDriveVexFile( Buffer, strlen( Buffer ) );
  }

  VasyDriveVexAtomLock = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyDriveVexExpr                         |
|                                                             |
\------------------------------------------------------------*/

static void loc_VasyDriveVexExpr( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  long        Oper;
  char       *String;
  int         Length;

  if ( IsVexNodeAtom( Expr ) )
  {
    VasyDriveVexAtom( Expr );
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( VasyVerilog ) String = getvexoperverilogname( Oper );
    else               String = getvexoperuppername( Oper );

    if ( String == (char *)0 )
    {
      VasyError( VASY_ERROR_IN_DRVVEX, Oper );
    }

    Length = strlen( String );

    if ( isvexunaryoper( Oper ) )
    {
      if ( ( Oper == VEX_EVENT  ) ||
           ( Oper == VEX_DRIVER ) )
      {
        VasyDriveVexFile( "(", 1 );

        loc_VasyDriveVexExpr( GetVexOperand( Expr->OPERAND ) );

        VasyDriveVexFile( "'", 1 );
        VasyDriveVexFile( String, Length );
      }
      else
      if ( Oper == VEX_NEG )
      {
        VasyDriveVexFile( "(-", 2 );
        loc_VasyDriveVexExpr( GetVexOperand( Expr->OPERAND ) );
      }
      else
      {
        VasyDriveVexFile( String, Length );
        VasyDriveVexFile( "(", 1 );
        loc_VasyDriveVexExpr( GetVexOperand( Expr->OPERAND ) );
      }
    }
    else
    {
      if ( isvexpositiveoper( Oper ) )
      {
        if ( ( VasyVerilog        ) &&
             ( Oper == VEX_CONCAT ) ) VasyDriveVexFile( "{", 1 );
        else                          VasyDriveVexFile( "(", 1 );
      }
      else
      {
        if ( VasyVerilog )
        {
          VasyDriveVexFile( "~(", 2 );
          String = getvexoperverilogname( getvexnotoper( Oper ) );
        }
        else
        {
          VasyDriveVexFile( "NOT(", 4 );
          String = getvexoperuppername( getvexnotoper( Oper ) );
        }

        Length = strlen( String );
      }

      for ( ScanOper  = Expr->OPERAND;
            ScanOper != (chain_list *)0;
            ScanOper  = ScanOper->NEXT )
      {
        loc_VasyDriveVexExpr( GetVexOperand( ScanOper ) );
        
        if ( ScanOper->NEXT != (chain_list *)0 )
        {
          VasyDriveVexFile( " ", 1 );
          VasyDriveVexFile( String, Length );
          VasyDriveVexFile( " ", 1 );
        }
      }
    }

    if ( ( VasyVerilog        ) &&
         ( Oper == VEX_CONCAT ) ) VasyDriveVexFile( "}", 1 );
    else                          VasyDriveVexFile( ")", 1 );
  }
  else
  {
    String = GetVexFuncValue( Expr );
    Length = strlen( String );

    VasyDriveVexFile( String, Length );
    VasyDriveVexFile( "(", 1 );

    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      loc_VasyDriveVexExpr( GetVexOperand( ScanOper ) );
       
      if ( ScanOper->NEXT != (chain_list *)0 )
      {
        VasyDriveVexFile( ",", 1 );
      }
    }

    VasyDriveVexFile( ")", 1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyDriveVexExpr                         |
|                                                             |
\------------------------------------------------------------*/

void VasyDriveVexExpr( File, RtlFigure, Expr, Verilog )

  FILE        *File;
  rtlfig_list *RtlFigure;
  vexexpr     *Expr;
  int          Verilog;
{
  if ( Expr != (vexexpr *)0 )
  {
    VasyFigure      = RtlFigure;
    VasyFile        = File;
    VasyDriveLength = 0;
    VasyVerilog     = Verilog;

    loc_VasyDriveVexExpr( Expr );
  }
}
