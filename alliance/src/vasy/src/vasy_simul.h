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
| Tool    :                     VASY                           |
|                                                             |
| File    :                 vasy_simul.h                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   28.08.97                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef VASY_SIMUL_H
# define VASY_SIMUL_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             Loop                            |
|                                                             |
\------------------------------------------------------------*/

# define VASY_MAX_SIMULATE_LOOP       1000

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define SetVasySymbolInOut( S, IO ) ((S)->FLAGS = IO )
# define GetVasySymbolInOut( S )     ((S)->FLAGS      )

# define SetVasySymbolEvent( S, E ) ((S)->USER = (void *)E )
# define GetVasySymbolEvent( S )    ((paevt_list *)(S)->USER )

# define IncVasySimulateLoop( O )   ((O)->FLAGS++  )
# define ClearVasySimulateLoop( O ) ((O)->FLAGS = 0)
# define IsVasySimulateLoop( O )    ((O)->FLAGS ==  VASY_MAX_SIMULATE_LOOP)

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct vasysimul
  {
    char  *VALUE;
    short  WIDTH;
    short  SIGNED;

  } vasysimul;

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

  extern void      VasySimulateDelSimul();
  extern vasysimul *VasySimulateVexExpr();
  extern void      VasySimulateVpnProc();

# endif
