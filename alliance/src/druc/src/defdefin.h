/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  DEFDEFIN.H                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   26/07/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       DefDefin  Fonctions                   |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         DrucDefine                          |
| Definit un layer temporaire caracterise par les parametres  |
| contenus dans la StructInstruction pointee par son numero   |
| puis chaine le layer en equipotentielles                    |
\------------------------------------------------------------*/
extern void DrucDefine ();

/*------------------------------------------------------------\
|                                                             |
|                         DrucUndefine                        |
| Detruit le layer temporaire caracterise par les parametres  |
| contenus dans la StructInstruction pointee par son numero   |
| on parcourt tous les rectangles du layer defini dans la     |
|   fenetre et on les remet dans le tas gere par rdswin       |
\------------------------------------------------------------*/
extern void DrucUndefine ();
