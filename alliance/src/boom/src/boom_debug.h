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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                 boom_debug.h                      |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   06.06.00                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef BOOM_DEBUG_H
# define BOOM_DEBUG_H

# include <time.h>

/*------------------------------------------------------------\
|                                                             |
|                         Debug Mode                          |
|                                                             |
\------------------------------------------------------------*/

# define BOOM_DEBUG_LEVEL0     0x01
# define BOOM_DEBUG_LEVEL1     0x03
# define BOOM_DEBUG_LEVEL2     0x07

/*------------------------------------------------------------\
|                                                             |
|                         Debug Option                        |
|                                                             |
\------------------------------------------------------------*/

/*
# define BOOM_DEBUG_NO_RED_INST  0x01
# define BOOM_DEBUG_STATISTICS   0x02
# define BOOM_DEBUG_DRIVE_STDOUT 0x04
*/

/*------------------------------------------------------------\
|                                                             |
|                           Chrono                            |
|                                                             |
\------------------------------------------------------------*/

# define BOOM_DEBUG_MAX_CHRONO   10

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

# define BoomDebugSaveBehFigure( F, T ) \
                                 \
  (BoomDebugLocSaveBehFigure( (F), (T), basename(__FILE__), __LINE__ ))

/*------------------------------------------------------------\
|                                                             |
|                         Debug Macro                         |
|                                                             |
\------------------------------------------------------------*/

# define IsBoomDebugLevel0()  (BoomDebugSearchItem( basename(__FILE__), BOOM_DEBUG_LEVEL0))
# define IsBoomDebugLevel1()  (BoomDebugSearchItem( basename(__FILE__), BOOM_DEBUG_LEVEL1))
# define IsBoomDebugLevel2()  (BoomDebugSearchItem( basename(__FILE__), BOOM_DEBUG_LEVEL2))

/*------------------------------------------------------------\
|                                                             |
|                         Print Macro                         |
|                                                             |
\------------------------------------------------------------*/

# define BoomPrintf   BoomDebugPrint( basename(__FILE__), __LINE__ ); fprintf 

/*------------------------------------------------------------\
|                                                             |
|                       Debug Option Macro                    |
|                                                             |
\------------------------------------------------------------*/

/*
# define IsBoomDebugNoRedInst()    (BoomDebugOption & BOOM_DEBUG_NO_RED_INST)
# define SetBoomDebugNoRedInst()   (BoomDebugOption |= BOOM_DEBUG_NO_RED_INST)
# define IsBoomDebugStatistics()   (BoomDebugOption & BOOM_DEBUG_STATISTICS)
# define SetBoomDebugStatistics()  (BoomDebugOption |= BOOM_DEBUG_STATISTICS)
# define IsBoomDebugDriveStdout()  (BoomDebugOption & BOOM_DEBUG_DRIVE_STDOUT)
# define SetBoomDebugDriveStdout() (BoomDebugOption |= BOOM_DEBUG_DRIVE_STDOUT)
*/

/*------------------------------------------------------------\
|                                                             |
|                          Time Macro                         |
|                                                             |
\------------------------------------------------------------*/

# define BoomDebugStartChrono(N) (void)time(&BoomDebugChrono[(N)])
# define BoomDebugReadChrono(N)  (time((void *)0) - BoomDebugChrono[(N)])

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

  extern int    BoomDebugFlag;
  extern int    BoomDebugOption;
  extern time_t BoomDebugChrono[ BOOM_DEBUG_MAX_CHRONO ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern int  BoomDebugSearchItem();
  extern int  BoomDebugAddItem();

  extern int  BoomDebugSetOption();

  extern void BoomDebugLocSaveBehFigure();
  extern void BoomDebugPrint();

# endif
