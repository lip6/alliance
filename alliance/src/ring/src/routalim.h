#ifndef __ROUTALIM_H
#define __ROUTALIM_H

#include "struct.h"

extern void pose_couralim(char *coursup, long piste_sup[NB_FACES],
	long piste_inf[NB_FACES],
	LST_EQUIPO lst_equipo, COEUR lecoeur, BARRE_PLOTS tab_plots[NB_FACES],
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES],
	long	tabpiste[NB_FACES]);

extern void tire_etdeport_alim(char *coursup, long piste_sup[NB_FACES],
	long piste_inf[NB_FACES], LST_EQUIPO lst_equipo,
	BARRE_PLOTS tab_plots[NB_FACES], LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES], long tabpiste[NB_FACES]);

void tire_etdeport_unealim(char *coursup, char *eq_alim, long piste_sup[NB_FACES],
	long piste_inf[NB_FACES], LST_EQUIPO equipo, BARRE_PLOTS tab_plots[NB_FACES],
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES],
	long	tabpiste[NB_FACES]);

#endif /* __ROUTALIM_H */
