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
| File    :                  G_global.h                       |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                             Constants                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef G_GLOBAL_H
# define G_GLOBAL_H

/* Limits of scale factor for Xdrawing */

# define MAX_ZOOM      80
# define MIN_ZOOM      0.01

# define LONG_MAX      1000000

# define STEP   0       /* Step by step running for connectors in fill_ins */
# define RUN    1       /* single pass running for connectors in fill_ins  */

# define GENVIEW_SIZE  ( sizeof( genviewrec ) )
# define GENVIEW_TAG   (long)(0x80000000)


/*------------------------------------------------------------\
|                                                             |
|                      Connector Orient                       |
|                                                             |
\------------------------------------------------------------*/

# define GENVIEW_NORTH        0
# define GENVIEW_SOUTH        1
# define GENVIEW_EAST         2
# define GENVIEW_WEST         3

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define GENVIEW_MBK( R )                                   \
                                                            \
    (((genviewrec *)((char *)(R)+sizeof(rdsrec_list)))->MBK )

# define SetTagGenview( Rectangle )                         \
                                                            \
   ( (Rectangle)->FLAGS |= GENVIEW_TAG                      )

# define ClearTagGenview( Rectangle )                       \
                                                            \
   ( (Rectangle)->FLAGS &= ~GENVIEW_TAG                     )

# define IsTagGenview( Rectangle )                          \
                                                            \
   ( ( (Rectangle)->FLAGS & GENVIEW_TAG ) == GENVIEW_TAG    )


/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct genviewrec
  {
    void *MBK;

  } genviewrec;

  typedef struct zoomBack
  {
     long  x,y,dx,dy;
     float scale;

  } zoomBack;

/*------------------------------------------------------------\
|                                                             |
|                             Variables                       |
|                                                             |
\------------------------------------------------------------*/

  extern phfig_list *WORK_PHFIG;
  extern phins_list *WORK_PHINS;

# ifdef MLO

  extern lofig_list *WORK_LOFIG;

# endif


  extern rdsfig_list *Visu_db;
  extern rdsfig_list *Peek_db;
  extern rdsins_list *Ptinsref;


  extern phfig_list *xvisufig;

  extern long        xabfig;
  extern long        yabfig;
  extern long        xab2fig;
  extern long        yab2fig;

  extern long        xvisu;
  extern long        yvisu;
  extern long        dxvisu;
  extern long        dyvisu;

  extern long        xscreen;
  extern long        yscreen;
  extern long        dxscreen;
  extern long        dyscreen;

  extern int         dxWin;
  extern int         dyWin;

  extern float       scale;

  extern FILE       *xinspect;
  extern zoomBack    zoomTrack;

  extern char        run_mode;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

# endif
