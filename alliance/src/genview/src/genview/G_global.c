/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                  G_global.c                       |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  rdsfig_list *Visu_db  = (rdsfig_list *)NULL;
  rdsfig_list *Peek_db  = (rdsfig_list *)NULL;
  rdsins_list *Ptinsref = (rdsins_list *)NULL;

  phfig_list *xvisufig = (phfig_list *)NULL;

  long        xabfig   = 0;
  long        yabfig   = 0;
  long        xab2fig  = 0;
  long        yab2fig  = 0;

  long        xvisu  = 0;
  long        yvisu  = 0;
  long        dxvisu = 0;
  long        dyvisu = 0;

  long        xscreen  = 0;
  long        yscreen  = 0;
  long        dxscreen = 0;
  long        dyscreen = 0;

  int         dxWin = 0;
  int         dyWin = 0;

  float       scale = 1.0;

  FILE       *xinspect;
  zoomBack    zoomTrack;

  char        run_mode = STEP;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
