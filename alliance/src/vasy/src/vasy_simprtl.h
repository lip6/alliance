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
| Tool    :                     VASY                          |
|                                                             |
| File    :                 vasy_simprtl.h                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef VASY_SIMP_RTL_H
# define VASY_SIMP_RTL_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Declar Mask                      |
|                                                             |
\------------------------------------------------------------*/

# define VASY_DECLAR_ASG_MASK           0x01
# define VASY_DECLAR_READ_MASK          0x02
# define VASY_DECLAR_USED_MASK          0x03
# define VASY_DECLAR_DEFINE_MASK        0x04

/*------------------------------------------------------------\
|                                                             |
|                          Macros Declar                      |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyRtlDeclarAsg( D )   ((D)->FLAGS |=  VASY_DECLAR_ASG_MASK)
# define ClearVasyRtlDeclarAsg( D ) ((D)->FLAGS &= ~VASY_DECLAR_ASG_MASK)
# define IsVasyRtlDeclarAsg( D )    ((D)->FLAGS & VASY_DECLAR_ASG_MASK)

# define SetVasyRtlDeclarRead( D )   ((D)->FLAGS |=  VASY_DECLAR_READ_MASK)
# define ClearVasyRtlDeclarRead( D ) ((D)->FLAGS &= ~VASY_DECLAR_READ_MASK)
# define IsVasyRtlDeclarRead( D )    ((D)->FLAGS & VASY_DECLAR_READ_MASK)

# define IsVasyRtlDeclarUsed( D )    ((D)->FLAGS & VASY_DECLAR_USED_MASK)
# define ClearVasyRtlDeclarUsed( D ) ((D)->FLAGS &= ~VASY_DECLAR_USED_MASK)

# define SetVasyRtlDeclarDefine( D )   ((D)->FLAGS |=  VASY_DECLAR_DEFINE_MASK)
# define ClearVasyRtlDeclarDefine( D ) ((D)->FLAGS &= ~VASY_DECLAR_DEFINE_MASK)
# define IsVasyRtlDeclarDefine( D )    ((D)->FLAGS & VASY_DECLAR_DEFINE_MASK)

/*------------------------------------------------------------\
|                                                             |
|                          Assign Mask                        |
|                                                             |
\------------------------------------------------------------*/

# define VASY_ASSIGN_SUBST_MASK         0x01
# define VASY_ASSIGN_SUBST_DONE_MASK    0x02

# define VASY_ASSIGN_NUMBER_READ_MASK   0xF0
# define VASY_ASSIGN_NUMBER_READ_SHIFT  4

/*------------------------------------------------------------\
|                                                             |
|                          Macros Assign                      |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyRtlAssignSubst( D )   ((D)->FLAGS |=  VASY_ASSIGN_SUBST_MASK)
# define ClearVasyRtlAssignSubst( D ) ((D)->FLAGS &= ~VASY_ASSIGN_SUBST_MASK)
# define IsVasyRtlAssignSubst( D )    ((D)->FLAGS &   VASY_ASSIGN_SUBST_MASK)

# define GetVasyRtlAssignNumberRead( D ) \
  (((D)->FLAGS & VASY_ASSIGN_NUMBER_READ_MASK) >> VASY_ASSIGN_NUMBER_READ_SHIFT)

# define SetVasyRtlAssignNumberRead( D, V ) \
  ((D)->FLAGS = ((D)->FLAGS & ~VASY_ASSIGN_NUMBER_READ_MASK) | \
    (((V) << VASY_ASSIGN_NUMBER_READ_SHIFT) & VASY_ASSIGN_NUMBER_READ_MASK))

# define IncVasyRtlAssignNumberRead( D ) \
   (SetVasyRtlAssignNumberRead( D, GetVasyRtlAssignNumberRead( D ) + 1 ))

# define SetVasyRtlAssignSubstDone( D )   ((D)->FLAGS |=  VASY_ASSIGN_SUBST_DONE_MASK)
# define ClearVasyRtlAssignSubstDone( D ) ((D)->FLAGS &= ~VASY_ASSIGN_SUBST_DONE_MASK)
# define IsVasyRtlAssignSubstDone( D )    ((D)->FLAGS &   VASY_ASSIGN_SUBST_DONE_MASK)

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

  extern void     VasySimplifyRtlFig();

# endif
