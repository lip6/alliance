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
| Tool    :                     Ctl                           |
|                                                             |
| File    :                   ctldel.c                        |
|                                                             |
| Date    :                   04.07.96                        |
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
# include "vex.h"
# include "ctl.h"

# include <stdio.h>
# include "ctldel.h"
# include "ctlerror.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Ctl Del Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Ctl Del All Line                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallctlline( ScanLine )

  ctlline_list *ScanLine;
{
  ctlline_list  *Delete;

  while ( ScanLine != (ctlline_list *)0 )
  {
    Delete   = ScanLine;
    ScanLine = ScanLine->NEXT;

    freectlline( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Del Declar                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_delctldecl( Figure, Declar )

  ctlfig_list  *Figure;
  ctldecl_list *Declar;
{
  char *Name;

  *Declar->PREV = Declar->NEXT;

  if ( Declar->NEXT != (ctldecl_list *)0 ) 
  { 
    Declar->NEXT->PREV = Declar->PREV;
  }

  Name = Declar->DECL_SYM->NAME;
  delauthelem( Figure->HASH_DECLAR[ Declar->TYPE   ], Name ); 
  delauthelem( Figure->HASH_DECLAR[ CTL_DECLAR_ALL ], Name );

  freectlsym( Declar->DECL_SYM );
  freevexexpr( Declar->VEX_ATOM );
  freevexexpr( Declar->VEX_INIT );
  freectldecl( Declar );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Del Figure                       |
|                                                             |
\------------------------------------------------------------*/

int delctlfig( Name )

   char *Name;
{
  ctlfig_list   *Figure;
  ctlfig_list  **Previous;
  ctlform_list  *ScanForm;
  ctldecl_list  *ScanDecl;
  ctltype_list  *ScanType;
  void          *Delete;
  int            Type;

  Name     = namealloc( Name );
  Previous = &HEAD_CTLFIG;

  for ( Figure  = HEAD_CTLFIG;
        Figure != (ctlfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;

    Previous = &Figure->NEXT;
  }

  if ( Figure == (ctlfig_list *)0 ) return( 0 );

  *Previous = Figure->NEXT;

  for ( Type = 0; Type < CTL_MAX_DECLAR_TYPE; Type++ )
  {
    destroyauthtable( Figure->HASH_DECLAR[ Type ] );
  }

  ScanForm = Figure->FORM;

  while ( ScanForm != (ctlform_list *)0 )
  {
    freevexexpr( ScanForm->VEX_EXPR );

    Delete   = (void *)( ScanForm );
    ScanForm = ScanForm->NEXT;
  
    freectlform( Delete );
  }

  ScanType = Figure->TYPE;

  while ( ScanType != (ctltype_list *)0 )
  {
    autfreeblock( ScanType->VALUE );

    Delete   = (void *)( ScanType );
    ScanType = ScanType->NEXT;
  
    freectltype( Delete );
  }

  for ( Type = 0; Type < CTL_MAX_DECLAR_TYPE; Type++ )
  {
    ScanDecl = Figure->DECLAR[ Type ];

    while ( ScanDecl != (ctldecl_list *)0 )
    {
      loc_delallctlline( ScanDecl->LINE );

      freevexexpr( ScanDecl->VEX_ATOM );
      freevexexpr( ScanDecl->VEX_INIT );
      freectlsym( ScanDecl->DECL_SYM );

      Delete    = (void *)( ScanDecl );
      ScanDecl = ScanDecl->NEXT;

      freectldecl( Delete );
    }
  }

  freectlfig( Figure );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Del Line                         |
|                                                             |
\------------------------------------------------------------*/

int delctlline( Figure, PrevLine, Line )

  ctlfig_list   *Figure;
  ctlline_list **PrevLine;
  ctlline_list  *Line;
{
  ctlline_list *ScanLine;

  for ( ScanLine  = *PrevLine;
        ScanLine != (ctlline_list *)0;
        ScanLine  = Line->NEXT )
  {
    if ( ScanLine == Line )
    {
      *PrevLine = Line->NEXT;
      freectlline( Line );

      return( 1 );
    }

    PrevLine = &ScanLine->NEXT;
  }

  return( 0 );
}
