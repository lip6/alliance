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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  pattern.c                        |
|                                                             |
| Authors :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.07.95                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef LYNX_PATTERN_H
# define LYNX_PATTERN_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   Max Width and Length in lambda            |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_MAX_WIDTH   10
# define LYNX_MAX_LENGTH  10

/*------------------------------------------------------------\
|                                                             |
|                             Macros                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Lynx Pattern List                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct lynxpattern
  {
    rdsrec_list  *REAL[ LYNX_MAX_ORIENT ];
    rdsrec_list  *SYMB[ LYNX_MAX_ORIENT ];
    long          WIDTH;

  } lynxpattern;
  
/*------------------------------------------------------------\
|                                                             |
|                      Lynx Transistor List                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct lynxtrans
  {
    struct lynxtrans   *NEXT;
    struct lynxpattern  PATTERN[ LYNX_MAX_WIDTH ];
    char                MBK_LAYER;
    char                GATE_LAYER;
    char                DIFF_LAYER;
    char                ACTIV_LAYER;
    char                ORIENT;
    long                LENGTH;

  } lynxtrans_list;

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

  extern long LynxReconizeTransistor();

# endif
