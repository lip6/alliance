/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  DEFTOOLS.H                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   26/07/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       DefTools  Fonctions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrucChaineCompose                   |
| Accroche le rectangle source au rectangle dest par le champ |
| NEXT_DEFINE pour connaite la liste des rectangles ayant     |
| contribue a la creation du rectangle dest                   |
\------------------------------------------------------------*/
extern void DrucChaineCompose ();
/*------------------------------------------------------------\
|                                                             |
|                         DrucAddRecFigure                    |
| insert le rectangle (initialise) dans la liste des          |
| rectangles du layer donne                                   |
\------------------------------------------------------------*/
extern void DrucAddRecFigure ();
/*------------------------------------------------------------\
|                                                             |
|                         DrucInclusionRectangle              |
| Cree     la copie du rectangle 2 s'il est inclus entierement|
| dans le rectangle 1 puis chaine les 2 rectangles source     |
| par le pointeur USER au rectangle cree                      |
| ou retoune NULL                                             |
\------------------------------------------------------------*/
extern void DrucInclusionRectangle ();
/*------------------------------------------------------------\
|                                                             |
|                         DrucIntersectRectangle              |
| Cree un rectangle de dimensions et coordonnees              |
| correspondant a l'intersection des 2 rectangles passes en   |
| parametre                                                   |
| ou retoune NULL si les 2 rectangles ne sont pas en          |
| intersection                                                |
\------------------------------------------------------------*/
extern void DrucIntersectRectangle ();
/*------------------------------------------------------------\
|                                                             |
|                         DrucExclusionRectangle              |
| Retourne la copie du rectangle 2 si il n'est pas en         |
| intersection ou en contact avec le rectangle 1              |
| ou retoune NULL                                             |
\------------------------------------------------------------*/
extern void DrucExclusionRectangle ();
/*------------------------------------------------------------\
|                                                             |
|                         DrucMarqueInclusion                 |
| Marque le rectangle 2 si il est inclus entierement          |
| dans le rectangle 1 puis chaine les 2 rectangles source     |
| par le pointeur USER au rectangle cree                      |
| ou retoune NULL                                             |
\------------------------------------------------------------*/
extern void DrucMarqueInclusion ();
