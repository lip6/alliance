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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Abl                           |
|                                                             |
| File    :                   ablview.c                       |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"

# include <stdio.h>
# include <string.h>
# include "ablview.h"
# include "ablerror.h"

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

  static FILE  *AblViewFile     = (FILE *)0;
  static int    AblViewLength   = 0;
  static void (*AblViewFunction)();

  static char *AblStringBuffer = (char *)0;
  static int   AblStringSize   = 0;
  static int   AblStringLength = 0;

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
|                      View Local Abl File                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewablfile( String, Length )

  char *String;
  int   Length;
{
  AblViewLength += Length;

  if ( AblViewLength >= ABL_VIEW_MAX_LENGTH )
  {
    fprintf( AblViewFile, "\n" );
    AblViewLength = 0;
  }

  fprintf( AblViewFile, "%s", String );
}

/*------------------------------------------------------------\
|                                                             |
|                    View Local Abl String                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewablstr( String, Length )

  char *String;
  int   Length;
{
  char *Buffer;

  if ( ( AblStringLength + Length ) >= AblStringSize )
  {
    AblStringSize = AblStringSize << 1;
    Buffer = (char *)autallocblock( AblStringSize );
    strcpy( Buffer, AblStringBuffer );
    autfreeblock( AblStringBuffer );
    AblStringBuffer = Buffer;
  }

  AblViewLength += Length;

  if ( AblViewLength >= ABL_VIEW_MAX_LENGTH )
  {
    AblViewLength = 0;
    loc_viewablstr( "\n", 1 );
  }

  strcpy( AblStringBuffer + AblStringLength, String );
  AblStringLength += Length;
}

/*------------------------------------------------------------\
|                                                             |
|                      View Local Expr Prefix                 |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewablprefix( Expr )

  ablexpr *Expr;
{
  long  Oper;
  char *OperName;

  if ( ABL_ATOM( Expr ) )
  {
    (*AblViewFunction)( ABL_ATOM_VALUE( Expr ),
                        strlen( ABL_ATOM_VALUE( Expr ) ) );
  }
  else
  {
    Oper     = ABL_OPER( Expr );
    OperName = getabloperuppername( Oper );

    if ( OperName == (char *)0 )
    {
      ablerror( ABL_OPERATOR_ERROR, Oper );
    }
  
    (*AblViewFunction)( "(", 1 );
    (*AblViewFunction)( OperName, strlen( OperName ) );

    Expr = ABL_CDR( Expr );

    while ( Expr != (ablexpr *)0 )
    {
      (*AblViewFunction)( " ", 1 );

      loc_viewablprefix( ABL_CAR( Expr ) );

      Expr = ABL_CDR( Expr );
    }

    (*AblViewFunction)( ")", 1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    View Local Expr Postfix                  |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewablpostfix( Expr )

  ablexpr *Expr;
{
  long  Oper;
  char *OperName;

  if ( ABL_ATOM( Expr ) )
  {
    (*AblViewFunction)( ABL_ATOM_VALUE( Expr ),
                        strlen( ABL_ATOM_VALUE( Expr ) ) );
  }
  else
  {
    Oper     = ABL_OPER( Expr );
    OperName = getabloperuppername( Oper );

    if ( OperName == (char *)0 )
    {
      ablerror( ABL_OPERATOR_ERROR, Oper );
    }

    (*AblViewFunction)( "(", 1 );

    Expr = ABL_CDR( Expr );

    while ( Expr != (ablexpr *)0 )
    {
      loc_viewablpostfix( ABL_CAR( Expr ) );

      Expr = ABL_CDR( Expr );

      (*AblViewFunction)( " ", 1 );
    }

    (*AblViewFunction)( OperName, strlen( OperName ) );
    (*AblViewFunction)( ")", 1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     View Local Expr Infix                   |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewablinfix( Expr )

  ablexpr *Expr;
{
  long  Oper;
  char *OperName;
  int   Length;

  if ( ABL_ATOM( Expr ) )
  {
    (*AblViewFunction)( ABL_ATOM_VALUE( Expr ),
                        strlen( ABL_ATOM_VALUE( Expr ) ) );
  }
  else
  {
    Oper     = ABL_OPER( Expr );
    OperName = getabloperuppername( Oper );
    Length   = strlen( OperName );

    if ( OperName == (char *)0 )
    {
      ablerror( ABL_OPERATOR_ERROR, Oper );
    }

    (*AblViewFunction)( "(", 1 );

    if ( isablunaryoper( Oper ) )
    {
      (*AblViewFunction)( OperName, Length );
      (*AblViewFunction)( " ", 1 );

      loc_viewablinfix( ABL_CADR( Expr ) );
    }
    else
    {
      while ( ( Expr = ABL_CDR( Expr ) ) )
      {
        loc_viewablinfix( ABL_CAR( Expr ) );
      
        if ( ABL_CDR( Expr ) != (ablexpr *)0 )
        {
          (*AblViewFunction)( " ", 1 );
          (*AblViewFunction)( OperName, Length );
          (*AblViewFunction)( " ", 1 );
        }
      }
    }

    (*AblViewFunction)( ")", 1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    View Local Expr Vhdl                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_viewablvhdl( Expr )

  ablexpr *Expr;
{
  long  Oper;
  char *String;
  int   Length;

  if ( ABL_ATOM( Expr ) )
  {
    String = ABL_ATOM_VALUE( Expr );

    if ( ( String != ABL_ATOM_NAME_ZERO     ) &&
         ( String != ABL_ATOM_NAME_ONE      ) &&
         ( String != ABL_ATOM_NAME_DC       ) &&
         ( String != ABL_ATOM_NAME_TRISTATE ) )
    {
      String = vhdlablname( String );
    }

    (*AblViewFunction)( String, strlen( String ) );
  }
  else
  {
    Oper   = ABL_OPER( Expr );
    String = getabloperuppername( Oper );
    Length = strlen( String );

    if ( String == (char *)0 )
    {
      ablerror( ABL_OPERATOR_ERROR, Oper );
    }

    if ( isablunaryoper( Oper ) )
    {
      if ( Oper == ABL_STABLE )
      {
        (*AblViewFunction)( "(", 1 );

        loc_viewablvhdl( ABL_CADR( Expr ) );

        (*AblViewFunction)( "'", 1 );
        (*AblViewFunction)( String, Length );
      }
      else
      {
        (*AblViewFunction)( String, Length );
        (*AblViewFunction)( "(", 1 );

        loc_viewablvhdl( ABL_CADR( Expr ) );
      }
    }
    else
    {
      if ( getabloperpolar( Oper ) == ABL_POLAR_POSITIVE )
      {
        (*AblViewFunction)( "(", 1 );
      }
      else
      {
        (*AblViewFunction)( "NOT(", 4 );
        String = getabloperuppername( getablopernot( Oper ) );
        Length = strlen( String );
      }

      while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)NULL )
      {
        loc_viewablvhdl( ABL_CAR( Expr ) );

        if ( ABL_CDR( Expr ) != (ablexpr *)0 )
        {
          (*AblViewFunction)( " ", 1 );
          (*AblViewFunction)( String, Length );
          (*AblViewFunction)( " ", 1 );
        }
      }
    }

    (*AblViewFunction)( ")", 1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    View Abl Expr File                       |
|                                                             |
\------------------------------------------------------------*/

void viewablexprfile( AblFile, Expr, Mode )

  FILE    *AblFile;
  ablexpr *Expr;
  int      Mode;
{
  AblViewFile     = AblFile;
  AblViewLength   = 0;
  AblViewFunction = loc_viewablfile;

  if ( Mode == ABL_VIEW_INFIX )
  {
    loc_viewablinfix( Expr );
  }
  else
  if ( Mode == ABL_VIEW_PREFIX )
  {
    loc_viewablprefix( Expr );
  }
  else
  if ( Mode == ABL_VIEW_POSTFIX )
  {
    loc_viewablpostfix( Expr );
  }
  else
  if ( Mode == ABL_VIEW_VHDL )
  {
    loc_viewablvhdl( Expr );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    View Abl Expr File NewLine               |
|                                                             |
\------------------------------------------------------------*/

void viewablexprfileln( AblFile, Expr, Mode )

  FILE    *AblFile;
  ablexpr *Expr;
  int      Mode;
{
  viewablexprfile( AblFile, Expr, Mode );
  fprintf( AblViewFile, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Abl Expr                       |
|                                                             |
\------------------------------------------------------------*/

void viewablexpr( Expr, Mode )

  ablexpr *Expr;
  int         Mode;
{
  viewablexprfile( stdout, Expr, Mode );
}

/*------------------------------------------------------------\
|                                                             |
|                         View Abl Expr NewLine               |
|                                                             |
\------------------------------------------------------------*/

void viewablexprln( Expr, Mode )

  ablexpr *Expr;
  int      Mode;
{
  viewablexprfileln( stdout, Expr, Mode );
}

/*------------------------------------------------------------\
|                                                             |
|                    View Abl Expr String                     |
|                                                             |
\------------------------------------------------------------*/

char *viewablexprstr( Expr, Mode )

  ablexpr *Expr;
  int         Mode;
{
  if ( AblStringBuffer == (char *)0 )
  {
    AblStringBuffer = (char *)autallocblock( ABL_STRING_BUFFER_SIZE );
    AblStringSize   = ABL_STRING_BUFFER_SIZE;
  }

  AblStringLength = 0;
  AblStringBuffer[ 0 ] = '\0';

  AblViewLength   = 0;
  AblViewFunction = loc_viewablstr;

  if ( Mode == ABL_VIEW_INFIX )
  {
    loc_viewablinfix( Expr );
  }
  else
  if ( Mode == ABL_VIEW_PREFIX )
  {
    loc_viewablprefix( Expr );
  }
  else
  if ( Mode == ABL_VIEW_POSTFIX )
  {
    loc_viewablpostfix( Expr );
  }
  else
  if ( Mode == ABL_VIEW_VHDL )
  {
    loc_viewablvhdl( Expr );
  }

  return( AblStringBuffer );
}

/*------------------------------------------------------------\
|                                                             |
|                    View Abl Expr String NewLine             |
|                                                             |
\------------------------------------------------------------*/

char *viewablexprstrln( Expr, Mode )

  ablexpr *Expr;
  int      Mode;
{
  viewablexprstr( Expr, Mode );
  strcpy( AblStringBuffer + AblStringLength, "\n" );

  return( AblStringBuffer );
}
