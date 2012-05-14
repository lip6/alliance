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
| Tool    :                     Abl                           |
|                                                             |
| File    :                   ablstr.c                        |
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
# include <ctype.h>
# include <string.h>
# include "ablstr.h"
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

  static char *AblScanString  = (char *)0;
  static char  AblStringOper  = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Skip Blank                         |
|                                                             |
\------------------------------------------------------------*/

static void loc_strablskipblank()

{
  while ( ( *AblScanString != '\0'       ) &&
          ( isspace( (int)*AblScanString ) ) )
  {
    AblScanString = AblScanString + 1;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                           End Word                          |
|                                                             |
\------------------------------------------------------------*/

static void loc_strablendword()

{
  int Open;

  Open = 0;

  while ( ( *AblScanString != '\0'         ) &&
          ( ! isspace( (int)*AblScanString ) ) )
  {
    if ( *AblScanString == '(' )
    {
      if ( Open == 0 )
      {
        Open = 1; *AblScanString = ' ';
      }
      else break;
    }
    else
    if ( *AblScanString == ')' )
    {
      if ( Open == 1 )
      {
        Open = 0; *AblScanString = '\0';
      }
      else break;
    }

    AblScanString = AblScanString + 1;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                           Get Word                          |
|                                                             |
\------------------------------------------------------------*/

static char *loc_strablgetword()

{
  char *BeginWord;
  char *EndWord;
  char  SwapChar;

  BeginWord = AblScanString;
  loc_strablendword();
  EndWord   = AblScanString;

  if ( BeginWord == EndWord ) 
  {
    return( (char *)0 );
  }

  SwapChar = *EndWord;
  *EndWord = '\0';
  BeginWord = namealloc( BeginWord );
  *EndWord = SwapChar;

  return( BeginWord );
}

/*------------------------------------------------------------\
|                                                             |
|                        Str  Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Str Abl Infix Expr                      |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *loc_strablinfix()
{
  ablexpr *Expr;
  ablexpr *ExprQueue;
  ablexpr *ExprStr;
  char        ToggleOper;
  char       *Name;
  long        Oper;

  loc_strablskipblank();

  if ( *AblScanString == '(' )
  {
    AblScanString = AblScanString + 1;
    loc_strablskipblank();

    if ( *AblScanString != '\0' )
    {
      ExprStr = loc_strablinfix();

      if ( ExprStr == (ablexpr *)0 )
      {
        return( (ablexpr *)0 );
      }

      Expr = addchain( (ablexpr *)0,
                       (void *)ExprStr );
      ExprQueue  = Expr;

      if ( AblStringOper )
      {
        AblStringOper = 0;

        Oper       = ABL_OPER( Expr );
        ToggleOper = 0;
      }
      else
      {
        Oper       = -1;
        ToggleOper = 1;
      }

      loc_strablskipblank();

      while ( ( *AblScanString != ')'  ) &&
              ( *AblScanString != '\0' ) )
      {
        ExprStr = loc_strablinfix();

        if ( ExprStr == (ablexpr *)0 )
        {
          freeablexpr( Expr );
          return( (ablexpr *)0 );
        }

        if ( AblStringOper )
        {
          AblStringOper = 0;

          if ( ! ToggleOper )
          {
            freeablexpr( ExprStr );
            freeablexpr( Expr );
            return( (ablexpr *)0 );
          }

          if ( Oper == -1 )
          {
            Expr = addchain( Expr, (void *)ExprStr );
            Oper = ABL_OPER( Expr );
          }
          else
          {
            if ( Oper != (long)ABL_CAR( ExprStr ) )
            {
              freeablexpr( ExprStr );
              freeablexpr( Expr );
              return( (ablexpr *)0 );
            }

            freeablexpr( ExprStr );
          }
        }
        else
        {
          if ( ToggleOper )
          {
            freeablexpr( ExprStr );
            freeablexpr( Expr );
            return( (ablexpr *)0 );
          }

          ABL_CDR( ExprQueue ) = addchain( (ablexpr *)0,
                                           (void *)ExprStr );
          ExprQueue = ABL_CDR( ExprQueue );
        }

        loc_strablskipblank();

        ToggleOper = ! ToggleOper;
      }

      if ( Oper == -1 )
      {
        ExprStr = ABL_CAR( Expr );
        freechain( Expr );
        Expr = ExprStr;
      }
      else
      if ( isablunaryoper( Oper ) )
      {
        if ( ABL_CDDR( Expr ) != (ablexpr *)0 )
        {
          freeablexpr( Expr );
          return( (ablexpr *)0 );
        }
      }
      else
      if ( ABL_CDDR( Expr ) == (ablexpr *)0 )
      {
        freeablexpr( Expr );
        return( (ablexpr *)0 );
      }
    }
    else
    {
      return( (ablexpr *)0 );
    }

    if ( *AblScanString == ')' ) 
    {
      AblScanString = AblScanString + 1;
    }
    else
    {
      freeablexpr( Expr );
      return( (ablexpr *)0 );
    }
  }
  else
  {
    Name = loc_strablgetword();

    if ( Name == (char *)0 )
    {
      return( (ablexpr *)0 );
    }

    Oper = getabloperbyname( Name );

    if ( Oper == -1 )
    {
      Expr = addchain( (ablexpr *)0, (void *)Name );
    }
    else
    {
      Expr = addchain( (ablexpr *)0, (void *)Oper );
      AblStringOper = 1;
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                     Str Abl Prefix Expr                     |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *loc_strablprefix()
{
  ablexpr *Expr;
  ablexpr *ExprQueue;
  ablexpr *ExprStr;
  char       *Name;
  long        Oper;

  loc_strablskipblank();

  if ( *AblScanString == '(' )
  {
    AblScanString = AblScanString + 1;
    loc_strablskipblank();

    Name = loc_strablgetword();

    if ( Name == (char *)0 ) 
    {
      return( (ablexpr *)0 );
    }

    Oper = getabloperbyname( Name );

    loc_strablskipblank();

    if ( *AblScanString == ')' )
    {
      if ( Oper == -1 ) 
      {
        Expr = addchain( (ablexpr *)0, Name );
      }
      else
      {
        return( (ablexpr *)0 );
      }
    }
    else
    if ( *AblScanString != '\0' )
    {
      Expr = addchain( (ablexpr *)0,
                       (void *)addchain( (ablexpr *)0,
                                         (void *)Oper ) );

      ExprQueue = Expr;

      do
      {
        ExprStr = loc_strablprefix();

        if ( ExprStr == (ablexpr *)0 )
        {
          freeablexpr( Expr );
          return( (ablexpr *)0 );
        }

        ABL_CDR( ExprQueue ) = addchain( (ablexpr *)0,
                                         (void *)ExprStr );
        ExprQueue = ABL_CDR( ExprQueue );

        loc_strablskipblank();
      }
      while ( ( *AblScanString != ')'  ) &&
              ( *AblScanString != '\0' ) );

      if ( isablunaryoper( Oper ) )
      {
        if ( ABL_CDDR( Expr ) != (ablexpr *)0 )
        {
          freeablexpr( Expr );
          return( (ablexpr *)0 );
        }
      }
      else
      if ( ABL_CDDR( Expr ) == (ablexpr *)0 )
      {
        freeablexpr( Expr );
        return( (ablexpr *)0 );
      }
    }
    else
    {
      return( (ablexpr *)0 );
    }

    if ( *AblScanString == ')' ) 
    {
      AblScanString = AblScanString + 1;
    }
    else
    {
      freeablexpr( Expr );
      return( (ablexpr *)0 );
    }
  }
  else
  {
    Name = loc_strablgetword();

    if ( Name == (char *)0 )
    {
      return( (ablexpr *)0 );
    }

    Oper = getabloperbyname( Name );

    if ( Oper == -1 )
    {
      Expr = addchain( (ablexpr *)0, (void *)Name );
    }
    else
    {
      return( (ablexpr *)0 );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                     Str Abl Postfix Expr                    |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *loc_strablpostfix()
{
  ablexpr *Expr;
  ablexpr *ExprQueue;
  ablexpr *ExprStr;
  char       *Name;
  long        Oper;

  loc_strablskipblank();

  if ( *AblScanString == '(' )
  {
    AblScanString = AblScanString + 1;
    loc_strablskipblank();

    if ( *AblScanString != '\0' )
    {
      ExprStr = loc_strablpostfix();

      if ( ExprStr == (ablexpr *)0 )
      {
        return( (ablexpr *)0 );
      }

      if ( AblStringOper )
      {
        AblStringOper = 0;

        freeablexpr( ExprStr );
        return( (ablexpr *)0 );
      }

      Expr = addchain( (ablexpr *)0,
                       (void *)ExprStr );

      ExprQueue = Expr;
      Oper      = -1;

      loc_strablskipblank();

      while ( ( Oper           == -1   ) &&
              ( *AblScanString != ')'  ) &&
              ( *AblScanString != '\0' ) )
      {
        ExprStr = loc_strablpostfix();

        if ( ExprStr == (ablexpr *)0 )
        {
          freeablexpr( Expr );
          return( (ablexpr *)0 );
        }

        if ( AblStringOper )
        {
          Expr          = addchain( Expr, (void *)ExprStr );
          Oper          = ABL_OPER( Expr );
          AblStringOper = 0;
        }
        else
        {
          ABL_CDR( ExprQueue ) = addchain( (ablexpr *)0,
                                           (void *)ExprStr );
          ExprQueue = ABL_CDR( ExprQueue );
        }

        loc_strablskipblank();
      }

      if ( Oper == -1 )
      {
        if ( ABL_CDR( Expr ) != (ablexpr *)0 )
        {
          freeablexpr( Expr );
          return( (ablexpr *)0 );
        }

        ExprStr = ABL_CAR( Expr );
        freechain( Expr );
        Expr = ExprStr;
      }
      else
      if ( isablunaryoper( Oper ) )
      {
        if ( ABL_CDDR( Expr ) != (ablexpr *)0 )
        {
          freeablexpr( Expr );
          return( (ablexpr *)0 );
        }
      }
      else
      if ( ABL_CDDR( Expr ) == (ablexpr *)0 )
      {
        freeablexpr( Expr );
        return( (ablexpr *)0 );
      }
    }
    else
    {
      return( (ablexpr *)0 );
    }

    if ( *AblScanString == ')' ) 
    {
      AblScanString = AblScanString + 1;
    }
    else
    {
      freeablexpr( Expr );
      return( (ablexpr *)0 );
    }
  }
  else
  {
    Name = loc_strablgetword();

    if ( Name == (char *)0 )
    {
      return( (ablexpr *)0 );
    }

    Oper = getabloperbyname( Name );

    if ( Oper == -1 )
    {
      Expr = addchain( (ablexpr *)0, (void *)Name );
    }
    else
    {
      Expr = addchain( (ablexpr *)0, (void *)Oper );
      AblStringOper = 1;
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                         Str Abl Expr                        |
|                                                             |
\------------------------------------------------------------*/

ablexpr *strablexpr( String, Mode )

  char *String;
  int   Mode;
{
  ablexpr *Expr;

  String = mbkstrdup( String );

  AblScanString  = String;
  AblStringOper  = 0;

  if ( Mode == ABL_VIEW_INFIX )
  {
    Expr = loc_strablinfix();
  }
  else
  if ( Mode == ABL_VIEW_PREFIX )
  {
    Expr = loc_strablprefix();
  }
  else
  {
    Expr = loc_strablpostfix();
  }

  loc_strablskipblank();

  if ( ( AblStringOper          ) ||
       ( *AblScanString != '\0' ) )
  {
    freeablexpr( Expr );
    Expr = (ablexpr *)0;
  }

  mbkfree( String );

  return( Expr );
}
