#ifndef __PLACEMENT_H
#define __PLACEMENT_H

#include "struct.h"

extern void remplir_tabplots(BARRE_PLOTS tab_plots[NB_FACES], LST_EQUIPO lst_equipo);
extern void remplir_tabcoeur(LST_PSEUDO_CON tabcoeur[NB_FACES], COEUR lecoeur, LST_EQUIPO lst_equipo);
extern void affic_tabcoeur(LST_PSEUDO_CON tabcoeur[NB_FACES]);
extern void affic_tabplots(BARRE_PLOTS tabplot[NB_FACES]);
extern void largeur_ab_plots(BARRE_PLOTS tab_plots[NB_FACES],
	chain_list *nomplot[NB_FACES], chain_list *liste_plotsph);
extern void fabrique_barre_plots(BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur,
	chain_list *nomplot[NB_FACES], chain_list  *liste_plotsph,
	chain_list  *lst_conestouest);
extern void fabrique_grille_primaire(LST_PSEUDO_CON tab_coeur[NB_FACES],
	GRILLE tab_grilles[NB_FACES]);
extern void place_et_cout_barreplot(LST_PSEUDO_CON tab_coeur[NB_FACES],
	BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur,
	LST_EQUIPO     lst_equipo);
extern void place_vertical_barreplot(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES], COEUR lecoeur,
	LST_EQUIPO lst_equipo);
extern int calcul_nbdeport(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES], COEUR lecoeur,
	LST_EQUIPO lst_equipo, long barrecourant, int face);
extern int moins_dun_pitch_confacecoeur(long position, LST_PSEUDO_CON plotcon,
	LST_PSEUDO_CON liste_coeur, LST_EQUIPO lst_equipo,
	int face);
extern int calcul_nbdeport_cyclesface(LST_EQUIPO lst_equipo);
extern int calcul_nbdeport_equipo_alim(LST_EQUIPO lst_equipo, long barrecourant, int face);
extern int croisement_con(chain_list *ptcour, chain_list *ptalim, 
	long barrecourant, int	face);

extern void recherche_equipo_alim(LST_EQUIPO *equipo_vdd,
	LST_EQUIPO *equipo_vss,
	LST_EQUIPO  lst_equipo);

extern int conplotalim_dans_coeur(LST_EQUIPO lst_equipo, COEUR coeur,
	long barrecourant, int face);

extern void maj_coordplots(BARRE_PLOTS tab_plots[NB_FACES], LST_EQUIPO lst_equipo);

extern void fabrique_grille_finale(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES], COEUR lecoeur,
	LST_EQUIPO lst_equipo, GRILLE tab_grilles[NB_FACES]);

extern void ajout_pas_grille(GRILLE tab_grilles[NB_FACES], COEUR lecoeur, int face);

extern void relier_plots_wire1(phins_list *lastinst, phins_list *lastinst2, chain_list *lst);

#endif /* __PLACEMENT_H */
