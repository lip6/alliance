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
| File    :                  ctlview.c                        |
|                                                             |
| Date    :                   28.08.96                        |
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
# include "ctlview.h"
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
|                       Ctl View Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Ctl View Line                        |
|                                                             |
\------------------------------------------------------------*/

void viewctlline( Line )

  ctlline_list *Line;
{
  char *File;

  fprintf( stdout, "\n--> Line" );
  fprintf( stdout, "\n\t\tLINE  : " );

  File = (char *)0;

  while ( Line != (ctlline_list *)0 )
  {
    if ( Line->NAME != File ) 
    {
      File = Line->NAME;
      fprintf( stdout, "  %s [%ld]", Line->NAME, Line->LINE );
    }
    else
    {
      fprintf( stdout, "  [%ld]", Line->LINE );
    }

    Line = Line->NEXT;
  }

  fprintf( stdout, "\n<-- Line" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl View Symbol                      |
|                                                             |
\------------------------------------------------------------*/

void viewctlsym( Symbol )

  ctlsym *Symbol;
{
  fprintf( stdout, "\n--> Symbol" );
  fprintf( stdout, "\n\t\tNAME  : %s", Symbol->NAME  );

  fprintf( stdout, "\n\t\tDECL  : " );
  viewvexexprbound( Symbol->DECL->VEX_ATOM );

  fprintf( stdout, "\n\t\tINDEX : %d", (int)Symbol->INDEX );
  fprintf( stdout, "\n\t\tFLAGS : %lx", Symbol->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Symbol->USER );
  fprintf( stdout, "\n<-- Symbol" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl View Declaration                 |
|                                                             |
\------------------------------------------------------------*/

void viewctldecl( Decl )

  ctldecl_list *Decl;
{
  ctlsym *DeclSym;
  int     Scan;
  int     Width;
  
  fprintf( stdout, "\n--> Declaration" );

  viewctlline( Decl->LINE );

  if ( *Decl->PREV != Decl )
  {
    fprintf( stdout, "\n\t\tPREV     : ERROR" );
  }

  fprintf( stdout, "\n\t\tVEX_ATOM : " );
  viewvexexprbound( Decl->VEX_ATOM );

  fprintf( stdout, "\n\t\tVEX_INIT : " );
  viewvexexprbound( Decl->VEX_INIT );

  fprintf( stdout, "\n\t\tDECL_SYM :\n" );

  DeclSym = Decl->DECL_SYM;
  Width   = Decl->VEX_ATOM->WIDTH;

  for ( Scan = 0; Scan < Width; Scan++ )
  {
    viewctlsym( &DeclSym[ Scan ] );
  }

  if ( Decl->DECL_TYPE >= CTL_MAX_DECLAR_TYPE )
  {
    fprintf( stdout, "\n\t\tDECL_TYPE : %d", Decl->DECL_TYPE  );
  }
  else
  {
    fprintf( stdout, "\n\t\tDECL_TYPE : %s", CTL_DECLAR_TYPE[ Decl->DECL_TYPE ] );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Decl->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Decl->USER );
  fprintf( stdout, "\n<-- Declaration" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Ctl View Type                           |
|                                                             |
\------------------------------------------------------------*/

void viewctltype( Type )

  ctltype_list *Type;
{
  long Index;

  fprintf( stdout, "\n--> Type" );

  viewctlline( Type->LINE );

  fprintf( stdout, "\n\t\tNAME     : %s",  Type->NAME  );
  fprintf( stdout, "\n\t\tINDEX    : %ld", Type->INDEX );
  fprintf( stdout, "\n\t\tLEFT     : %ld", Type->LEFT  );
  fprintf( stdout, "\n\t\tRIGHT    : %ld", Type->RIGHT );
  fprintf( stdout, "\n\t\tCLASS    : %c" , Type->CLASS );
  fprintf( stdout, "\n\t\tSIZE     : %ld", Type->SIZE  );

  if ( Type->BASE != (ctltype_list *)0 )
  {
    fprintf( stdout, "\n\t\tBASE     : %s", Type->BASE->NAME  );
  }

  fprintf( stdout, "\n\t\tVALUE    : " );

  for ( Index = 0; Index < Type->SIZE; Index++ )
  {
    fprintf( stdout, "%s ", Type->VALUE[ Index ] );
  }

  fprintf( stdout, "\n\t\tFLAGS    : %lx", Type->FLAGS );
  fprintf( stdout, "\n\t\tUSER     : %lx", (long)Type->USER );

  fprintf( stdout, "\n<-- Type" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Ctl View Form                           |
|                                                             |
\------------------------------------------------------------*/

void viewctlform( Form )

  ctlform_list *Form;
{
  fprintf( stdout, "\n--> Form" );

  viewctlline( Form->LINE );

  if ( *Form->PREV != Form )
  {
    fprintf( stdout, "\n\t\tPREV     : ERROR" );
  }

  fprintf( stdout, "\n\t\tNAME     : %s", Form->NAME );

  fprintf( stdout, "\n\t\tVEX_EXPR : \n" );

  viewvexexprbound( Form->VEX_EXPR );

  fprintf( stdout, "\n\t\tFLAGS    : %lx", Form->FLAGS );
  fprintf( stdout, "\n\t\tUSER     : %lx", (long)Form->USER );
  fprintf( stdout, "\n<-- Form" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl View Figure                      |
|                                                             |
\------------------------------------------------------------*/

void viewctlfig( Figure )

  ctlfig_list *Figure;
{
  ctltype_list  *Type;
  ctlform_list  *Form;
  ctldecl_list  *Decl;
  int            Index;

  fprintf( stdout, "\n--> Figure" );
  fprintf( stdout, "\n\tNAME            : %s", Figure->NAME  );

  fprintf( stdout, "\n\tTYPE : " );

  for ( Type  = Figure->TYPE;
        Type != (ctltype_list *)0;
        Type  = Type->NEXT )
  {
    viewctltype( Type );
  }

  for ( Index = 0; Index < CTL_MAX_DECLAR_TYPE; Index++ )
  {
    fprintf( stdout, "\n\tDECLARATION %s : ", CTL_DECLAR_TYPE[ Index ]  );

    for ( Decl  = Figure->DECLAR[ Index ];
          Decl != (ctldecl_list *)0;
          Decl  = Decl->NEXT )
    {
      viewctldecl( Decl );
    }
  }

  fprintf( stdout, "\n\tFORM : " );

  for ( Form  = Figure->FORM;
        Form != (ctlform_list *)0;
        Form  = Form->NEXT )
  {
    viewctlform( Form );
  }

  fprintf( stdout, "\n\tFLAGS : %lx", Figure->FLAGS );
  fprintf( stdout, "\n\tUSER  : %lx", (long)Figure->USER );
  fprintf( stdout, "\n<-- Figure\n" );
}
