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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                     VASY                           |
|                                                             |
| File    :                  vasy_redwait.h                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef VASY_REDWAIT_H
# define VASY_REDWAIT_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define VASY_REDWAIT_DEL_MASK      (long)0x01
# define VASY_REDWAIT_TAG_MASK      (long)0x02

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Delete                           |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyRedWaitDelete( O )   ((O)->FLAGS |=  VASY_REDWAIT_DEL_MASK)
# define ClearVasyRedWaitDelete( O ) ((O)->FLAGS &= ~VASY_REDWAIT_DEL_MASK)
# define IsVasyRedWaitDelete( O )    ((O)->FLAGS &   VASY_REDWAIT_DEL_MASK)

/*------------------------------------------------------------\
|                                                             |
|                              Tag                            |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyRedWaitTag( O )   ((O)->FLAGS |=  VASY_REDWAIT_TAG_MASK)
# define ClearVasyRedWaitTag( O ) ((O)->FLAGS &= ~VASY_REDWAIT_TAG_MASK)
# define IsVasyRedWaitTag( O )    ((O)->FLAGS &   VASY_REDWAIT_TAG_MASK)

/*------------------------------------------------------------\
|                                                             |
|                            Place                            |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyRedWaitPlace( T, P ) ((T)->USER = (void *)P )
# define GetVasyRedWaitPlace( T )    ((vpnplace_list *)(T)->USER)
# define ClearVasyRedWaitPlace( T )  ((T)->USER = (void *)0 )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct vasystack_list
  {
    struct vasystack_list *NEXT;
    struct vasystack_list *PREV;
    vpntrans_list        *TRANS;

  } vasystack_list;

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

  extern int VasyRedWaitVpnProc();

# endif
