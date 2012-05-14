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
| Tool    :                     Ctl                           |
|                                                             |
| File    :                  ctlfree.c                        |
|                                                             |
| Date    :                   04.18.02                        |
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
# include <string.h>
# include "ctlfree.h"
# include "ctlalloc.h"
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
|                     Ctl Free Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Ctl Free Figure                        |
|                                                             |
\------------------------------------------------------------*/
 
void freectlfig( Figure )

  ctlfig_list *Figure;
{
  autfreeblock( (char *)Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Free Form                        |
|                                                             |
\------------------------------------------------------------*/

void freectlform( Form )

  ctlform_list *Form;
{
  autfreeheap( (char *)Form, sizeof( ctlform_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Free Type                        |
|                                                             |
\------------------------------------------------------------*/

void freectltype( Type )

  ctltype_list *Type;
{
  autfreeheap( (char *)Type, sizeof( ctltype_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Free Symbol                      |
|                                                             |
\------------------------------------------------------------*/

void freectlsym( Sym )

  ctlsym *Sym;
{
  autfreeblock( (char *)Sym );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Free Declaration                 |
|                                                             |
\------------------------------------------------------------*/

void freectldecl( Decl )

  ctldecl_list *Decl;
{
  autfreeheap( (char *)Decl, sizeof( ctldecl_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Free Line                        |
|                                                             |
\------------------------------------------------------------*/

void freectlline( Line )

  ctlline_list *Line;
{
  autfreeheap( (char *)Line, sizeof( ctlline_list ));
}

