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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_comp.h                     |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   05.13.02                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef MOCHA_CTL_H
# define MOCHA_CTL_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define MOCHA_CTL_HASH_SUPPORT_MASK   0x1

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define MOCHA_CTL_BIABLA( O ) ( (biablarray *)((O)->USER) )

# define IsMochaCtlHashSupport( O )    ((O)->FLAGS &   MOCHA_CTL_HASH_SUPPORT_MASK)
# define SetMochaCtlHashSupport( O )   ((O)->FLAGS |=  MOCHA_CTL_HASH_SUPPORT_MASK)
# define ClearMochaCtlHashSupport( O ) ((O)->FLAGS &= ~MOCHA_CTL_HASH_SUPPORT_MASK)


/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct ablarray
  {
    ablexpr **ARRAY;
    long      WIDTH;

  } ablarray;

  typedef struct biablarray
  {
    ablarray *TARGET;
    ablarray *EXPR;

  } biablarray;

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

  extern void MochaCompileCtl();

# endif
