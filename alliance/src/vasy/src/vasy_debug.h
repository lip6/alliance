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
| Tool    :                     VASY                          |
|                                                             |
| File    :                 vasy_debug.h                      |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef VASY_DEBUG_H
# define VASY_DEBUG_H

# include <time.h>

/*------------------------------------------------------------\
|                                                             |
|                         Debug Mode                          |
|                                                             |
\------------------------------------------------------------*/

# define VASY_DEBUG_LEVEL0     0x01
# define VASY_DEBUG_LEVEL1     0x03
# define VASY_DEBUG_LEVEL2     0x07

/*------------------------------------------------------------\
|                                                             |
|                         Debug Option                        |
|                                                             |
\------------------------------------------------------------*/

# define VASY_DEBUG_NO_RED_INST  0x01
# define VASY_DEBUG_STATISTICS   0x02
# define VASY_DEBUG_DRIVE_STDOUT 0x04

/*------------------------------------------------------------\
|                                                             |
|                           Chrono                            |
|                                                             |
\------------------------------------------------------------*/

# define VASY_DEBUG_MAX_CHRONO   10

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Save Macro                         |
|                                                             |
\------------------------------------------------------------*/

# define VasyDebugSaveVpnFig( F ) \
                                 \
  (VasyDebugLocSaveVpnFig( (F), basename(__FILE__), __LINE__ ))

/*------------------------------------------------------------\
|                                                             |
|                         Debug Macro                         |
|                                                             |
\------------------------------------------------------------*/

# define IsVasyDebugLevel0()  (VasyDebugSearchItem( basename(__FILE__), VASY_DEBUG_LEVEL0))
# define IsVasyDebugLevel1()  (VasyDebugSearchItem( basename(__FILE__), VASY_DEBUG_LEVEL1))
# define IsVasyDebugLevel2()  (VasyDebugSearchItem( basename(__FILE__), VASY_DEBUG_LEVEL2))

/*------------------------------------------------------------\
|                                                             |
|                         Print Macro                         |
|                                                             |
\------------------------------------------------------------*/

# define VasyPrintf   VasyDebugPrint( basename(__FILE__), __LINE__ ); fprintf 

/*------------------------------------------------------------\
|                                                             |
|                       Debug Option Macro                    |
|                                                             |
\------------------------------------------------------------*/

# define IsVasyDebugNoRedInst()    (VasyDebugOption & VASY_DEBUG_NO_RED_INST)
# define SetVasyDebugNoRedInst()   (VasyDebugOption |= VASY_DEBUG_NO_RED_INST)
# define IsVasyDebugStatistics()   (VasyDebugOption & VASY_DEBUG_STATISTICS)
# define SetVasyDebugStatistics()  (VasyDebugOption |= VASY_DEBUG_STATISTICS)
# define IsVasyDebugDriveStdout()  (VasyDebugOption & VASY_DEBUG_DRIVE_STDOUT)
# define SetVasyDebugDriveStdout() (VasyDebugOption |= VASY_DEBUG_DRIVE_STDOUT)

/*------------------------------------------------------------\
|                                                             |
|                          Time Macro                         |
|                                                             |
\------------------------------------------------------------*/

# define VasyDebugStartChrono(N) (void)time(&VasyDebugChrono[(N)])
# define VasyDebugReadChrono(N)  (time((void *)0) - VasyDebugChrono[(N)])

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

  extern int    VasyDebugFlag;
  extern int    VasyDebugOption;
  extern time_t VasyDebugChrono[ VASY_DEBUG_MAX_CHRONO ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern int  VasyDebugSearchItem();
  extern int  VasyDebugAddItem();

  extern int  VasyDebugSetOption();

  extern void VasyDebugLocSaveVpnFig();
  extern void VasyDebugPrint();

# endif
