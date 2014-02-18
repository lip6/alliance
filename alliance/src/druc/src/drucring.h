/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  DRUCRING.H                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   03/07/95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  DrucRing    Types                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  DrucRing    Defines                        |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_BLOC_PERE                      (rdsins_list *)NULL
#define DRUC_DEMI_CONNECTEUR                                  4
#define DRUC_COURONNE_EXTEND                             "_RNG"

extern void DrucDelLayer             ();
extern void DrucFlattenInstanceRings ();
extern void DrucCreateCouron         ();
