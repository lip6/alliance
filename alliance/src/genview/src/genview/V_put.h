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
| File    :                   V_put.h                         |
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

# ifndef V_PUT_H  
# define V_PUT_H  

# define GENVIEW_DRAWN (long)0x00020000

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define IsGenviewDrawn( Rectangle )        \
                                            \
   ( ( Rectangle )->FLAGS & GENVIEW_DRAWN   )

# define SetGenviewDrawn( Rectangle )       \
                                            \
   ( ( Rectangle )->FLAGS |= GENVIEW_DRAWN  )

# define ClearGenviewDrawn( Rectangle )     \
                                            \
   ( ( Rectangle )->FLAGS &= ~GENVIEW_DRAWN )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          String View                        |
|                                                             |
\------------------------------------------------------------*/

  typedef struct strview

  {
    struct strview  *NEXT;
    rdsrec_list     *RECTANGLE;

  } strview ;

/*------------------------------------------------------------\
|                                                             |
|                             Variables                       |
|                                                             |
\------------------------------------------------------------*/

  extern int  GenviewInterrupt;
  extern int  GenviewCheckInt;

/*------------------------------------------------------------\
|                                                             |
|                             Functions                       |
|                                                             |
\------------------------------------------------------------*/

  extern void V_DrawBox();
  extern void Put_it();
  extern void Put_it_pattern();
  extern void Put_it_wire();
  extern void Put_it_fill();

  extern int  GenviewCheckInterrupt();
  extern void GenviewInterruptDisplay();
  extern void GenviewFlushEventDisplay();

# endif
