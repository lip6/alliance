/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     SYF                           |
|                                                             |
| File    :                   syf_env.c                       |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                    06.03.95                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H
# include FSM_H
# include FVH_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "syf_env.h"
# include "syf_fsm.h"
# include "syf_error.h"

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

  long   SYF_BDD_MAX_NODE         = 900000;
  long   SYF_BDD_VAR_NODE         =     50;
  long   SYF_BDD_OPER_NODE        =  10000;
  long   SYF_BDD_REORDER_NODE     = 100000;
  long   SYF_BDD_REORDER_RATIO    =     50;
  void (*SYF_BDD_REORDER_FUNC)()  = reorderbddsystemsimple;

  int    SYF_MUSTANG_JEDI_ATOM    =     1;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

void SyfEnv( Trace )

  int Trace;
{
  char *Variable;
  char  ReorderFunc;

  Variable = mbkgetenv( "SYF_BDD_VAR_NODE" );
  if ( Variable != (char *)0 )
  {
    SYF_BDD_VAR_NODE = atoi( Variable );
  }

  Variable = mbkgetenv( "SYF_BDD_MAX_NODE" );
  if ( Variable != (char *)0 )
  {
    SYF_BDD_MAX_NODE = atoi( Variable );
  }

  Variable = mbkgetenv( "SYF_BDD_OPER_NODE" );
  if ( Variable != (char *)0 )
  {
    SYF_BDD_OPER_NODE = atoi( Variable );
  }

  Variable = mbkgetenv( "SYF_BDD_REORDER_NODE" );
  if ( Variable != (char *)0 )
  {
    SYF_BDD_REORDER_NODE = atoi( Variable );
  }

  Variable = mbkgetenv( "SYF_BDD_REORDER_RATIO" );
  if ( Variable != (char *)0 )
  {
    SYF_BDD_REORDER_RATIO = atoi( Variable );
  }

  Variable = mbkgetenv( "SYF_BDD_REORDER_FUNC" );
  if ( Variable != (char *)0 )
  {
    ReorderFunc = Variable[ 0 ];

    switch( ReorderFunc )
    {
      case 's' : SYF_BDD_REORDER_FUNC = reorderbddsystemsimple;
      break;

      case 'w' : SYF_BDD_REORDER_FUNC = reorderbddsystemwindow;
      break;

      case 'g' : SYF_BDD_REORDER_FUNC = garbagebddsystem;
      break;

      default  : SYF_BDD_REORDER_FUNC = (void *)0;
    }
  }
  else
  {
    ReorderFunc = 's';
  }

  Variable = mbkgetenv( "SYF_MUSTANG_JEDI_ATOM" );
  if ( Variable != (char *)0 )
  {
    SYF_MUSTANG_JEDI_ATOM = atoi( Variable );
  }

  if ( Trace )
  {
    fprintf( stdout, "\t\t--> Bdd environement\n\n" );
    fprintf( stdout, "\t\t\tSYF_BDD_VAR_NODE      : %ld\n", SYF_BDD_VAR_NODE      );
    fprintf( stdout, "\t\t\tSYF_BDD_MAX_NODE      : %ld\n", SYF_BDD_MAX_NODE      );
    fprintf( stdout, "\t\t\tSYF_BDD_OPER_NODE     : %ld\n", SYF_BDD_OPER_NODE     );
    fprintf( stdout, "\t\t\tSYF_BDD_REORDER_NODE  : %ld\n", SYF_BDD_REORDER_NODE  );
    fprintf( stdout, "\t\t\tSYF_BDD_REORDER_RATIO : %ld\n", SYF_BDD_REORDER_RATIO );
    fprintf( stdout, "\t\t\tSYF_BDD_REORDER_FUNC  : %c\n", ReorderFunc           );

    fprintf( stdout, "\n\t\t--> Mustang and Jedi environement\n\n" );
    fprintf( stdout, "\t\t\tSYF_MUSTANG_JEDI_ATOM : %d\n", SYF_MUSTANG_JEDI_ATOM );
    fprintf( stdout, "\n" );
  }
}
