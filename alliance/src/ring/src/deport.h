#ifndef __DEPORT_H
#define __DEPORT_H

#include "struct.h"

extern void attribuer_couronne_alim(LST_EQUIPO lst_equipo, char **coursup, char **courinf);
extern char* existe_unique_deport_alim(LST_EQUIPO equipo_vdd, LST_EQUIPO equipo_vss);

extern void deport_alim(LST_EQUIPO lst_equipo, BARRE_PLOTS tab_plots[NB_FACES],
	char *coursup, char *courinf, LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES]);

void deport_connecteurs(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES],
	COEUR lecoeur, GRILLE tab_grilles[NB_FACES]);

void pose_segdeport(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES], COEUR lecoeur, LST_EQUIPO lst_equipo,
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES]);

#endif /* __DEPORT_H */
