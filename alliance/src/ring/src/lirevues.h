#ifndef __LIREVUES_H
#define __LIREVUES_H

#include "struct.h"

extern void lecture_vues(char *nom_circuit_lo, COEUR *lecoeur, lofig_list **circuit_lo,
	chain_list **liste_plotsph, int	*nbplots, chain_list **lst_conestouest);

extern void verif_con_estouest(chain_list *liste_plotsph, chain_list **lst_conestouest);

#endif /* __LIREVUES_H */
