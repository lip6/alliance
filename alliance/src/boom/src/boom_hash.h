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
| Tool    :                     BOOM                           |
|                                                             |
| File    :                   boom_hash.h                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef BOOM_HASH_H
# define BOOM_HASH_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Boom Hash Abl                       |
|                                                             |
\------------------------------------------------------------*/

  typedef struct boomhabl
  {
    bddnode    *NODE;
    chain_list *ABL;

  } boomhabl;

/*-----------------------------------------------------------\
|                                                            |
|                          Boom Hash Abl Table                |
|                                                            |
\-----------------------------------------------------------*/

  typedef struct boomhabltable
  {
    boomhabl *TABLE;
    long     TABLE_SIZE;
    long     NUMBER_ABL;
    long     NUMBER_ADD;
    long     NUMBER_HIT;
    long     NUMBER_MISS;
    long     NUMBER_RESET;

  } boomhabltable;

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

  extern boomhabltable *BoomCreateHashAblTable();
  extern void          BoomDestroyHashAblTable();
  extern void          BoomResetHashAblTable();

  extern boomhabl      *BoomAddHashAbl();
  extern boomhabl      *BoomSearchHashAbl();

# endif
