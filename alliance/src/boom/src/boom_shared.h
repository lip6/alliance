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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                  vasy_shared.h                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef BOOM_SHARED_H
# define BOOM_SHARED_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Aux Flags                         |
|                                                             |
\------------------------------------------------------------*/

# define BOOM_AUX_KEEP_MASK       0x01
# define BOOM_AUX_DONT_TOUCH_MASK 0x02
# define BOOM_AUX_TAG_MASK        0x04 

/*------------------------------------------------------------\
|                                                             |
|                           Rin Flags                         |
|                                                             |
\------------------------------------------------------------*/

# define BOOM_RIN_ORDER_MASK      0x01

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Aux Flags                         |
|                                                             |
\------------------------------------------------------------*/

# define IsBoomBehAuxTag( A )     ((A)->FLAGS &   BOOM_AUX_TAG_MASK)
# define SetBoomBehAuxTag( A )    ((A)->FLAGS |=  BOOM_AUX_TAG_MASK)
# define ClearBoomBehAuxTag( A )  ((A)->FLAGS &= ~BOOM_AUX_TAG_MASK)

# define IsBoomBehAuxKeep( A )     ((A)->FLAGS &   BOOM_AUX_KEEP_MASK)
# define SetBoomBehAuxKeep( A )    ((A)->FLAGS |=  BOOM_AUX_KEEP_MASK)
# define ClearBoomBehAuxKeep( A )  ((A)->FLAGS &= ~BOOM_AUX_KEEP_MASK)

# define IsBoomBehAuxDontTouch( A )     ((A)->FLAGS &   BOOM_AUX_DONT_TOUCH_MASK)
# define SetBoomBehAuxDontTouch( A )    ((A)->FLAGS |=  BOOM_AUX_DONT_TOUCH_MASK)
# define ClearBoomBehAuxDontTouch( A )  ((A)->FLAGS &= ~BOOM_AUX_DONT_TOUCH_MASK)

/*------------------------------------------------------------\
|                                                             |
|                           Rin Flags                         |
|                                                             |
\------------------------------------------------------------*/

# define IsBoomBehRinOrder( R )     ((R)->FLAGS &   BOOM_RIN_ORDER_MASK)
# define SetBoomBehRinOrder( R )    ((R)->FLAGS |=  BOOM_RIN_ORDER_MASK)
# define ClearBoomBehRinOrder( R )  ((R)->FLAGS &= ~BOOM_RIN_ORDER_MASK)

/*------------------------------------------------------------\
|                                                             |
|                          Optimization Algorithm             |
|                                                             |
\------------------------------------------------------------*/

# define BOOM_OPTIM_ALGORITHM_TOP        0
# define BOOM_OPTIM_ALGORITHM_WINDOW     1
# define BOOM_OPTIM_ALGORITHM_SIMPLE     2
# define BOOM_OPTIM_ALGORITHM_ONE        3
# define BOOM_OPTIM_ALGORITHM_RANDOM     4
# define BOOM_OPTIM_ALGORITHM_PROCRAST   5
# define BOOM_OPTIM_ALGORITHM_BURGUN     6
# define BOOM_OPTIM_ALGORITHM_NOTHING    7
# define BOOM_OPTIM_ALGORITHM_GRADIENT   8
# define BOOM_OPTIM_ALGORITHM_SIMUL      9
# define BOOM_OPTIM_ALGORITHM_ANNEAL    10

# define BOOM_MAX_OPTIM_ALGORITHM       11

/*------------------------------------------------------------\
|                                                             |
|                         Beh Fig Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetBoomBehFigInfo( F, I )  ((F)->USER = (void *)(I))
# define GetBoomBehFigInfo( F    )  ((boombefiginfo *)(F)->USER)

/*------------------------------------------------------------\
|                                                             |
|                         Beh Aux Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetBoomBehAuxInfo( A, I )  ((A)->USER = (void *)(I))
# define GetBoomBehAuxInfo( A    )  ((boombeauxinfo *)(A)->USER)

/*------------------------------------------------------------\
|                                                             |
|                         Beh Out Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetBoomBehOutInfo( A, I )  ((A)->USER = (void *)(I))
# define GetBoomBehOutInfo( A    )  ((boombeoutinfo *)(A)->USER)

/*------------------------------------------------------------\
|                                                             |
|                         Beh Reg Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetBoomBehRegInfo( A, I )  ((A)->USER = (void *)(I))
# define GetBoomBehRegInfo( A    )  ((boombereginfo *)(A)->USER)

/*------------------------------------------------------------\
|                                                             |
|                         Beh Bus Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetBoomBehBusInfo( A, I )  ((A)->USER = (void *)(I))
# define GetBoomBehBusInfo( A    )  ((boombebusinfo *)(A)->USER)

/*------------------------------------------------------------\
|                                                             |
|                         Beh Bux Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetBoomBehBuxInfo( A, I )  ((A)->USER = (void *)(I))
# define GetBoomBehBuxInfo( A    )  ((boombebuxinfo *)(A)->USER)

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct boombeauxinfo 
  {
    long  DELAY_COST;
    long  DEPTH_COST;
    long  SURFACE_COST;
    long  LITERAL_COST;

  } boombeauxinfo;

  typedef struct boombeoutinfo 
  {
    long  DELAY_COST;
    long  DEPTH_COST;
    long  SURFACE_COST;
    long  LITERAL_COST;

  } boombeoutinfo;

  typedef struct boombereginfo 
  {
    long  DELAY_COST;
    long  DEPTH_COST;
    long  SURFACE_COST;
    long  LITERAL_COST;

  } boombereginfo;

  typedef struct boombebusinfo 
  {
    long  DELAY_COST;
    long  DEPTH_COST;
    long  SURFACE_COST;
    long  LITERAL_COST;

  } boombebusinfo;

  typedef struct boombebuxinfo 
  {
    long  DELAY_COST;
    long  DEPTH_COST;
    long  SURFACE_COST;
    long  LITERAL_COST;

  } boombebuxinfo;

  typedef struct boombefiginfo
  {
    chain_list   *KEEP_LIST;
    chain_list   *DONT_TOUCH_LIST;
    chain_list   *BDD_ORDER_LIST;
    long          SURFACE_COST;
    long          DEPTH_COST;
    long          LITERAL_COST;
    long          GLOBAL_COST;
    long          INITIAL_SURFACE_COST;
    long          INITIAL_LITERAL_COST;
    long          INITIAL_DEPTH_COST;
    unsigned char OPTIM_ALGORITHM;
    unsigned char OPTIM_LEVEL;
    unsigned char OPTIM_DELAY_PERCENT;
    unsigned char KEEP_AUX;
    unsigned char INIT_BDD_ORDER;
    unsigned char VERBOSE;
    unsigned char TRACE;
    unsigned char USE_LITERAL;
    unsigned char DONT_CARE;
    bddnode      *BDD_DONT_CARE;
    int           ITERATION;
    int           AMPLITUDE;

  } boombefiginfo;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern int   BoomFlagOption;
  extern int   BoomFlagDebug;
  extern char *BoomOptimAlgorithmName[ BOOM_MAX_OPTIM_ALGORITHM ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
 
  extern boombeauxinfo *BoomAddBehAuxInfo();
  extern void           BoomDelBehAuxInfo();
  extern void           BoomBehFreeBehAux();

  extern boombebuxinfo *BoomAddBehBuxInfo();
  extern void           BoomDelBehBuxInfo();
  extern void           BoomBehFreeBehBux();

  extern boombebusinfo *BoomAddBehBusInfo();
  extern void           BoomDelBehBusInfo();
  extern void           BoomBehFreeBehBus();

  extern boombeoutinfo *BoomAddBehOutInfo();
  extern void           BoomDelBehOutInfo();
  extern void           BoomBehFreeBehOut();

  extern boombereginfo *BoomAddBehRegInfo();
  extern void           BoomDelBehRegInfo();
  extern void           BoomBehFreeBehReg();

  extern boombefiginfo *BoomAddBehFigInfo();
  extern void           BoomDelBehFigInfo();
  extern void           BoomBehFreeBehFig();

  extern long           BoomComputeLog2();

# endif
