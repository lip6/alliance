#ifndef __SESAME_H
#define __SESAME_H

#include "struct.h"
extern void ringerreur(int code, void *pt_liste, void *pt_liste2);
extern void usage ();
extern void banner();
extern void verif_eq_interne(LST_EQUIPO lst_equipo, chain_list *liste_plotsph);
extern int existe_leconnecteur_faceplot(char* figure, char* conname, int face,
	chain_list *liste_plotsph);
extern void testcon_modelfig(lofig_list* ptfiglo);

#endif /* __SESAME_H */
