#ifndef __ROUTAGE_H
#define __ROUTAGE_H

#include "struct.h"
extern void remplit_eq_curv(LST_EQUIPO lst_equipo, COEUR lecoeur);
extern void insere_valcurv(long valcurv, PT_COORDONNEES coord, LST_EQUIPO equipo);
extern void ordonne_eq_proj(LST_EQUIPO equipo, COEUR lecoeur);
extern void route_equipo(long piste_infalim[NB_FACES], LST_EQUIPO lst_equipo,
	COEUR lecoeur, LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES]);
extern void construit_couronne(long piste_cour[NB_FACES], long	piste_infalim[NB_FACES],
	LST_EQUIPO  equipo, COEUR lecoeur, LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES]);

extern void ajout_segmentpiste(PT_COORDONNEES coin, PT_COORDONNEES *nouveaucoin,
	int face, long	piste_cour[NB_FACES], long piste_infalim[NB_FACES],
	LST_EQUIPO  equipo, COEUR lecoeur, LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES]);

extern void poseseg_colonnes(long piste_cour[NB_FACES], LST_EQUIPO equipo,
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES]);

extern void ordonne_equipotentielles(LST_EQUIPO *lst_equipo);

void calcul_eq_proj(LST_EQUIPO equipo, COEUR lecoeur);
long calcul_curv(LST_PSEUDO_CON con, COEUR lecoeur);

#endif /* __ROUTAGE_H */
