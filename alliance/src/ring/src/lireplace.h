#ifndef __LIREPLACE_H
#define __LIREPLACE_H

#include "struct.h"

extern void lecture_fic(char *nomfic, lofig_list *circuit_lo,
	COEUR lecoeur, int nb_inst_plots);
extern void fabrique_equipo(chain_list *nomplot[NB_FACES], COEUR lecoeur, 
	LST_EQUIPO *lst_equipo, chain_list *liste_plotsph);

extern locon_list *existe_sig_plot(losig_list *sig_coeur, loins_list *plot,
	chain_list *liste_plotsph);

#endif /* __LIREPLACE_H */
