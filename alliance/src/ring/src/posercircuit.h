#ifndef __POSERCIRCUIT_H
#define __POSERCIRCUIT_H

#include "struct.h"

extern void poser_blocs(phfig_list **circuit_ph, char *nomcircuit,
	BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur);
extern void poser_segments(LST_EQUIPO lst_equipo, COEUR lecoeur,
	lofig_list *circuit_lo, phfig_list *circuit_ph);
extern void poser_segments_eq(LST_EQUIPO equipo, COEUR lecoeur,
	lofig_list *circuit_lo);
extern void poser_vias_eq(LST_EQUIPO equipo, COEUR lecoeur, phfig_list *circuit_ph);

extern void remonte_con_circuit(chain_list *nom_plot[NB_FACES],
	chain_list *liste_plotsph, BARRE_PLOTS tab_plots[NB_FACES],
	COEUR lecoeur, lofig_list *circuit_lo, phfig_list *circuit_ph,
	char	*nom_circuit_ph, chain_list  *lst_conestouest);

extern void traite_equipo_ext(chain_list *nom_plot[NB_FACES],
	chain_list *liste_plotsph,
	BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur,
	lofig_list *circuit_lo, phfig_list *circuit_ph);
	
extern void remonte_con(locon_list *con_circuit, locon_list *con_plot,
	chain_list *liste_plotsph, phfig_list *circuit_ph);

extern void tirer_coins(phfig_list  *circuit_ph, BARRE_PLOTS tab_plots[NB_FACES],
	chain_list  *lst_conestouest);

extern void ajout_coin(long x, long y, long wx, long wy, char layer);
extern void poser_vias_visavisalim(LST_EQUIPO equipo, COEUR lecoeur,
	phfig_list *circuit_ph);

#endif /* __POSERCIRCUIT_H */
