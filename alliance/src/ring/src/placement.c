/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ring router                                                 */
/*    Fichier : placement.c                                                 */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Olivier Van Hautte                  le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

/*--------------------------------------------------------------------------------------*/
/* RING 15 juillet 92            PLACEMENT.C                                            */
/* PHASE 4                                                                              */
/*                                                                                      */
/* Les tableaux tab_plots tab_coeur et la liste des connecteurs speciaux sont remplies. */
/*--------------------------------------------------------------------------------------*/

#include "placement.h"
#include "compress.h"
#include "distance.h"
#include "sesame.h"
#include "barre.h"

/*-----------------------------------------------------------------------------------*/
/* Cette procedure initialise les structures de donneees necessaires au barres       */
/* de plots . Elle prend les connecteurs de plots sur l'aboutement de la figure      */
/* physique de la barre construite precedemment. Si un connecteur n'existe pas       */
/* dans le niveau demande, on cree un deport, sera traite ulterieurement.            */
/*-----------------------------------------------------------------------------------*/

void remplir_tabplots(BARRE_PLOTS tab_plots[NB_FACES], LST_EQUIPO lst_equipo)
{
	int	face, sansdeport = 0, avecdeport = 1;
	phfig_list * ptfig;
	phcon_list * liste_con;
	char	niveau = 0;
	long	x, y, xarr, yarr;
	LST_PSEUDO_CON con;

	for (face = 0; face < NB_FACES; face++) {
		switch (face) {
		case NORD:
		case SUD : 
			niveau = ymetal;
			break;
		case EST :
		case OUEST: 
			niveau = xmetal;
			break;
		}

		if (tab_plots[face].width != 0) {
			ptfig = getphfig(tab_plots[face].nomfig, 'A');
			liste_con = ptfig->PHCON;

			while (liste_con != NULL) {
	
				/* --------------------------------------------------------------- */
				/* 1ere passe, on ne prend que les connecteurs du layer de la face */
				/* --------------------------------------------------------------- */

				if (liste_con->LAYER == niveau) {

#ifdef COMMENT
					/* --------------------------------- */
					/* ces 3 lignes uniquement pour test */
					/* --------------------------------- */

					con = existe_conplot_lstequipo(liste_con->NAME, lst_equipo);
					if ((con != NULL) && (con->nom_con == eq_vdd)) {
#endif
						/* ------------------------- */
						/* Coordonnees du connecteur */
						/* ------------------------- */

						x = liste_con->XCON;
						y = liste_con->YCON;

						/* ------------------------------------------------------------------------ */
						/* Mise a plat des coordonnees par rapport a l'origine de la barre de plots */
						/* ------------------------------------------------------------------------ */

						xyflat(&xarr, &yarr, x, y, 0L, 0L, ptfig->XAB1, ptfig->YAB1, ptfig->XAB2,
						                        ptfig->YAB2, NOSYM);

						/* ------------------------------------------------------------------------ */
						/* Ajout du pseudo connecteur dans la liste associee a la face du plot      */
						/* ------------------------------------------------------------------------ */

						ajout_pseudo_con_plot(xarr, yarr, liste_con->WIDTH, liste_con->LAYER, liste_con->NAME,
						     face, sansdeport, tab_plots, lst_equipo);

#ifdef COMMENT
					}
#endif
				}

				liste_con = liste_con->NEXT;
			} /* fin du while parcours des conecteurs physiques */

			/*-------------------------------------------------------------------*/
			/* 2eme passe : on traite les connecteurs qui n'ont pas le bon layer */
			/*-------------------------------------------------------------------*/

			liste_con = ptfig->PHCON;

			while (liste_con != NULL) {

				if (liste_con->LAYER != niveau) {

					/* ------------------------- */
					/* Coordonnees du connecteur */
					/* ------------------------- */

					con = existe_conplot_lstequipo(liste_con->NAME, lst_equipo);

					if ((con != NULL) && (con->largeur == 0)) /* non traite */ {
						/* deport a effectuer */

						if (mode_debug) 
							printf("Deport mauvais layer %d connecteur %s\n", (int) niveau, liste_con->NAME);
						alloue_coord(0L, 0, &(con->deport));

						/* con->deport = (PT_COORDONNEES) mbkalloc ((unsigned int) sizeof(COORDONNEES)); */

						(con->deport)->piste = 1; /* C'est un mauvais layer */

						con->deport->proprio = (void * ) con;

						x = liste_con->XCON;
						y = liste_con->YCON;

						/* ------------------------------------------------------------------------ */
						/* Mise a plat des coordonnees par rapport a l'origine de la barre de plots */
						/* ------------------------------------------------------------------------ */

						xyflat(&xarr, &yarr, x, y, 0L, 0L, ptfig->XAB1, ptfig->YAB1, ptfig->XAB2,
						                        ptfig->YAB2, NOSYM);

						/* ------------------------------------------------------------------------ */
						/* Ajout du pseudo connecteur dans la liste associee a la face du plot      */
						/* ------------------------------------------------------------------------ */

						ajout_pseudo_con_plot(xarr, yarr, liste_con->WIDTH, liste_con->LAYER, liste_con->NAME,
						     face, avecdeport, tab_plots, lst_equipo);
					} /* fin du 2eme if */
				} /* fin du 1er if */

				liste_con = liste_con->NEXT;
			} /* fin du while parcours des conecteurs physiques mauvais layer */

		} /* fin du if */ else 
			tab_plots[face].lst_con = NULL;

	} /* fin du for */

}

/*-----------------------------------------------------------------------------------*/
/* Cette procedure remplir le tableau de pseudo connecteurs du coeur, en les rangeant*/
/* par face et par abscisse, x ou y selon la face horizontale ou verticale           */
/*-----------------------------------------------------------------------------------*/

void remplir_tabcoeur(LST_PSEUDO_CON tabcoeur[NB_FACES], COEUR lecoeur, LST_EQUIPO lst_equipo)
{

	long	x, y, xarr, yarr;
	phcon_list * liste_con = (lecoeur.coeur_ph)->PHCON;
	phfig_list * coeurph = lecoeur.coeur_ph;
	int	face;

	/* ----------------------------------- */
	/* Initialisation du tableau de listes */
	/* ----------------------------------- */

	for (face = 0; face < NB_FACES; face++)
		tabcoeur[face] = NULL;

	if (mode_debug) 
		printf("REMplir tabcoeur\n");

	if (liste_con == NULL)
		ringerreur(ERR_CONCOEUREMPTY, &lecoeur, NULL);

	while (liste_con != NULL) {

		/* ------------------------- */
		/* Coordonnees du connecteur */
		/* ------------------------- */

		x = liste_con->XCON;
		y = liste_con->YCON;

		/* ------------------------------------------------------------ */	
		/* Mise a plat des coordonnees par rapport a l'origine du coeur */
		/* ------------------------------------------------------------ */

		xyflat(&xarr, &yarr, x, y, lecoeur.coord.xabs, lecoeur.coord.yabs, coeurph->XAB1, coeurph->YAB1, coeurph->XAB2,
		     				 coeurph->YAB2, lecoeur.rotation);

		/* ---------------------------------------------------------------------- */
		/* Reorientation eventuelle du connecteur si le coeur a subi une rotation */
		/* ---------------------------------------------------------------------- */

		face = reorientation_con(liste_con->ORIENT, NOSYM);

		/* ---------------------------------------------------------------------- */
		/* Ajout du pseudo connecteur dans la liste associee a la face du coeur   */
		/* ---------------------------------------------------------------------- */

		ajout_pseudo_con_coeur(xarr, yarr, liste_con, face, tabcoeur, lst_equipo);
		liste_con = liste_con->NEXT;
	}

}

/*-----------------------------------------------------------------------------------*/

void affic_tabcoeur(LST_PSEUDO_CON tabcoeur[NB_FACES])
{

	int	i;

	for (i = 0; i < NB_FACES; i++) {
		printf("Affic lst face coeur no %d\n", i);
		affic_lstcon(tabcoeur[i]);
	}

}

/*-----------------------------------------------------------------------------------*/

void affic_tabplots(BARRE_PLOTS tabplot[NB_FACES])
{

	int	i;

	for (i = 0; i < NB_FACES; i++) {
		printf("Nomfig %s largeur %ld\n", tabplot[i].nomfig, tabplot[i].width);
		printf("Affic lst face barre de plots no %d\n", i);
		affic_lstcon(tabplot[i].lst_con);
	}

}

/*-----------------------------------------------------------------------------------*/
/* Calcul largeur des barres de plots et faces coeurs                                */
/*-----------------------------------------------------------------------------------*/

void largeur_ab_plots(BARRE_PLOTS tab_plots[NB_FACES],
	chain_list *nomplot[NB_FACES], chain_list *liste_plotsph)
{
	int	face, first;
	phfig_list * ptfig;
	long	largeur;
	chain_list * liste;

	/* --------------------------------------------------------- */
	/* Initialisations et calcul largeur ab d'une barre de plots */
	/* --------------------------------------------------------- */

	tab_plots[NORD].nomfig   = namealloc(FBARRE_N);
	tab_plots[NORD].nominst  = namealloc(IBARRE_N);
	tab_plots[SUD].nomfig    = namealloc(FBARRE_S);
	tab_plots[SUD].nominst   = namealloc(IBARRE_S);
	tab_plots[EST].nomfig    = namealloc(FBARRE_E);
	tab_plots[EST].nominst   = namealloc(IBARRE_E);
	tab_plots[OUEST].nomfig  = namealloc(FBARRE_O);
	tab_plots[OUEST].nominst = namealloc(IBARRE_O);

	for (face = 0; face < NB_FACES; face++) {
		first = 1;
		tab_plots[face].lst_con = NULL;
		tab_plots[face].nb_deport = 0;

		liste = nomplot[face];

		largeur = 0;

		while (liste != NULL) {
			if (mode_debug) 
				printf("figure %s\n", ((loins_list * )liste->DATA)->FIGNAME);

			if ((ptfig = appartient_listeplotsph(((loins_list * )liste->DATA)->FIGNAME, liste_plotsph)) != NULL) {
				if (first) { 
					first = 0; 
					tab_plots[face].height = ptfig->YAB2 - ptfig->YAB1;
				}

				largeur += (ptfig->XAB2 - ptfig->XAB1);
				if (mode_debug) 
					printf("largeur AB %ld plot %s \n", (ptfig->XAB2 - ptfig->XAB1), ((loins_list * )liste->DATA)->FIGNAME);
			}

			liste = liste->NEXT;
		}

		tab_plots[face].width = largeur;
	}

}

/*-----------------------------------------------------------------------------------*/

void fabrique_barre_plots(BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur,
	chain_list *nomplot[NB_FACES], chain_list  *liste_plotsph,
	chain_list  *lst_conestouest)
{
	int	face, first;
	long	width_coeur, height_coeur, largeur = 0;
	chain_list * liste;
	phfig_list  *barre, *lastfig;
	long	diff, each, touslesnb, nbplotsface, x, y;
	long	nbplotsvidetot, cptplot, cptplotvide;
	double	nbeachvide;
	phins_list * firstinst, *lastinst, *lastinst2;

	/* ------------------------------------ */
	/* Il faudra traiter cas rotation coeur */
	/* ------------------------------------ */

	width_coeur  = lecoeur.width;
	height_coeur = lecoeur.height;

	largeur_ab_plots(tab_plots, nomplot, liste_plotsph);

	/* ---------------------------------------- */
	/* Construction des quatres barres de plots */
	/* ---------------------------------------- */

	for (face = 0; face < NB_FACES; face++) {
		if (tab_plots[face].width != 0) {

			/* ---------------------- */
			/* Barre de plots definie */ 
			/* ---------------------- */

			switch (face) {
			case EST:
			case OUEST: 
				largeur = height_coeur;
				break;
			case NORD:
			case SUD:   
				largeur = width_coeur;
				break;
			}

			if (tab_plots[face].width >= largeur) {
				if (mode_debug) 
					printf("face %d plots abutes simplement\n", face);

				liste = nomplot[face]; 
				first = 1;

				definir_fig_ph(tab_plots[face].nomfig);

				while (liste != NULL) {
					if (first) {
						first = 0;
						placer_instph(((loins_list * )liste->DATA)->FIGNAME, ((loins_list * )liste->DATA)->INSNAME,
						     NOSYM, 0L, 0L);
					} else
						abouteright_plot_fig(((loins_list * )liste->DATA)->FIGNAME, ((loins_list
						    *)liste->DATA)->INSNAME);
					liste = liste->NEXT;
				}

				definir_ab_fig();
			}  else {

				/* -------------------------- */
				/* barre plot < face du coeur */
				/* -------------------------- */

				if (mode_debug) 
					printf("Largeur barre < largeur coeur \n");

				nbplotsface = compte_nbplotsface(nomplot[face]);
				diff = largeur - tab_plots[face].width;
				nbplotsvidetot = diff / lambda;

				if (nbplotsvidetot < 1) {
					touslesnb = 1; 
					each = 0;
				} else {

					/* -------------------------- */
					/* pas de plots vide a ajoute */ 
					/* -------------------------- */

					if (1 != nbplotsface) 
						nbeachvide = nbplotsvidetot / (float) (nbplotsface - 1);
					else 
						nbeachvide = nbplotsvidetot / (float) nbplotsface;
					if (nbeachvide >= 1) {
						touslesnb = 1; 
						each = (long) nbeachvide;
					} else {
						touslesnb = (long) ((1 / nbeachvide) + 1); 
						each = 1;
					}
				}

				if (mode_debug) 
					printf("On ajoute %ld lambda de fils tous les %ld plots\n", each, touslesnb);

				liste = nomplot[face];
				cptplot = cptplotvide = 0; 
				first = 1;

				definir_fig_ph(tab_plots[face].nomfig);
				barre = getphfig(tab_plots[face].nomfig, 'A');

				while (liste != NULL) {
					if ((NULL != liste) && (0 != cptplot) && (0 == cptplot % touslesnb)) {
						firstlastinst_barre(barre, &firstinst, &lastinst);

						lastfig = getphfig(lastinst->FIGNAME, 'A');

						x = lastinst->XINS + lastfig->XAB2 - lastfig->XAB1 ; 
						y =  lastinst->YINS;
						cptplot++; 
						cptplotvide++;

						placer_instph(((loins_list * )liste->DATA)->FIGNAME, ((loins_list * )liste->DATA)->INSNAME
						    , NOSYM, (x + each * lambda) / SCALE_X , y / SCALE_X);

						firstlastinst_barre(barre, &firstinst, &lastinst2);

						relier_plots_wire1(lastinst, lastinst2, lst_conestouest);

						liste = liste->NEXT;
					} else
					 {
						if (first) {
							first = 0;
							placer_instph(((loins_list * )liste->DATA)->FIGNAME, ((loins_list
							    *)liste->DATA)->INSNAME, NOSYM, 0L, 0L);
						} else
							abouteright_plot_fig(((loins_list * )liste->DATA)->FIGNAME, ((loins_list
							    *)liste->DATA)->INSNAME);
						cptplot++;
						liste = liste->NEXT;
					}
				}

				definir_ab_fig();

				/* ---------------------------------------------- */
				/* mise a jour de la largeur de la barre de plots */
				/* ---------------------------------------------- */

				tab_plots[face].width += cptplotvide * each * lambda;

			}

		} /* fin du if largeur <> 0 */  else if /* largeur =0 else du 1er if */
		(mode_debug) 
			printf("barre face %d vide\n", face);

	}  /* fin du for */

	/* -------------------------------------------------------------- */
	/* ON reprend chaque figure de plot et on remonte les connecteurs */
	/* -------------------------------------------------------------- */

	for (face = 0; face < NB_FACES; face++) {
		if (tab_plots[face].width != 0) {
			liste = nomplot[face];

			definir_fig_ph(tab_plots[face].nomfig);

			while (liste != NULL) {
				if (existe_connecteur_faceplot(((loins_list * )liste->DATA)->FIGNAME, SUD, liste_plotsph))
					remonter_consud_instph(((loins_list * )liste->DATA)->INSNAME);
				liste = liste->NEXT;
			}

			if (mode_debug)	 
				sauver_fig_ph();
		}
	}

	/* for (face=0; face < NB_FACES; face++) {
		if (tab_plots[face].largeur != 0) {
			ptfig = getphfig(tab_plots[face].nomfig,'A');
			viewphfig(ptfig);
		}
	}
        */

}

/*----------------------------------------------------------------------------*/
/* Fabrication de la grille primaire a partir des connecteurs du coeur.       */
/* Verification de la distance des conecteurs entre eux leur largeur et leur  */
/* niveau                                                                     */
/*----------------------------------------------------------------------------*/

void fabrique_grille_primaire(LST_PSEUDO_CON tab_coeur[NB_FACES],
	GRILLE tab_grilles[NB_FACES])
{
	LST_PSEUDO_CON liste;
	int	face;
	char	niveau = 0;
	long	largeurmin = 0, largeurmax;

	if (WVIA_ALU1 > WVIA_ALU2) 
		largeurmax = WVIA_ALU1 * lambda;
	else 
		largeurmax = WVIA_ALU2 * lambda;

	if (mode_debug) 
		printf("Fabrique grille primaire \n");
	if (mode_debug) 
		printf("alu1 %d alu2 %d\n", ALU1, ALU2);

	for (face = 0; face < NB_FACES; face++) {
		liste = tab_coeur[face];

		if (mode_debug) 
			printf("Face %d\n", face);

		/* --------------------------- */
		/* Initialisation de la grille */
		/* --------------------------- */

		tab_grilles[face].lst_pas     = NULL;
		tab_grilles[face].lst_deportg = NULL;
		tab_grilles[face].lst_deportd = NULL;

		switch (face) {
		case NORD:
		case SUD :
			niveau = ymetal;
			largeurmin = ymetal_width;
			tab_grilles[face].piste.largeur_pas = pitch;
			break;

		case EST:
		case OUEST:
			niveau = xmetal;
			largeurmin = xmetal_width;
			tab_grilles[face].piste.largeur_pas = pitch;
			break;
		}

		while (liste != NULL) {
			if (mode_debug) 
				printf("connecteur %s\n", liste->nom_con);
			if (mode_debug) 
				printf("width connecteur %ld largeurmin %ld niveau requis %d\n", liste->largeur, largeurmin,
				     (int) niveau);

			if(!isvdd(liste->nom_con) && !isvss(liste->nom_con) && (liste->largeur > largeurmax))
				ringerreur(ERR_CONLARGEUR, liste, NULL);

			if (liste->layer != niveau) /* deport a effectuer */ {
				if (mode_debug) 
					printf("Deport mauvais layer %d connecteur %s\n", (int) liste->layer, liste->nom_con);
				alloue_coord(0L, 0, &(liste->deport));

				(liste->deport)->piste = 1; /* mauvais layer */
				liste->deport->proprio = (void * ) liste;

				switch (liste->layer) {
				case ALU1: 
					if (liste->largeur < WMIN_ALU1) /* ringerreur largeur connecteur */
						ringerreur(ERR_CONLARGEUR, liste, NULL);
					break;
				case ALU2: 
					if (liste->largeur < WMIN_ALU2) /* ringerreur largeur connecteur */
						ringerreur(ERR_CONLARGEUR, liste, NULL);

					break;
				}

			}   else if (liste->largeur < largeurmin) /* ringerreur largeur connecteur */
				ringerreur(ERR_CONLARGEUR, liste, NULL);

			if (moins_dun_pitch_preccon(liste))
				ringerreur(ERR_CONDISTANCE, liste, NULL);

			ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_pas), face);

			liste = liste->suiv;
		}  /* fin du while */

	} /* fin du for */

}

/*-----------------------------------------------------------------------------*/
/* Cette procedure place chaque barre de plots par rapport a la face du coeur. */
/* Elle essaye plusieurs positions possibles pour chaque barre calule son cout */
/* en nombre de deports et garde la meilleure et la plus centree               */
/*-----------------------------------------------------------------------------*/

void place_et_cout_barreplot(LST_PSEUDO_CON tab_coeur[NB_FACES],
	BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur,
	LST_EQUIPO     lst_equipo)
{

	int	face, gauche, droite, haut, bas, cout, first = 0;
	long	diff, barrexmilieu, barreymilieu, barrexcourant, barreycourant;

	if (mode_debug) 
		printf("PLACE barre plots\n");

	for (face = 0; face < NB_FACES; face++) {
		if (mode_debug) 
			printf("\n******************** FACE %d *************************\n", face);

		if (tab_plots[face].width != 0) {
			switch (face) {
			case NORD:
			case SUD :

				diff = tab_plots[face].width - lecoeur.width;

				if (mode_debug) 
					printf("diff barre - face coeur %ld\n", diff);

              //if (diff >= 0)
					barrexmilieu = tab_plots[face].coord.xabs =  ((lecoeur.coord.xabs - diff / 2) / SCALE_X) *
					    SCALE_X;
						
					/* --------------------------------------- */
					/* Largeur barre de plots >= largeur coeur */
					/* --------------------------------------- */

                  //else
                  //barrexmilieu = tab_plots[face].coord.xabs =  ((lecoeur.coord.xabs - diff / 2) / SCALE_X) *
                  //    SCALE_X;

				/*---------------------------------------------------------------- */
				/* Largeur barre de plots < largeur coeur                          */
				/* La barre de plots est centree sur le coeur                      */
				/* Nb iterations a effectuer en deplacant la barre vers la gauche  */
				/* puis vers la droite, on garde la position la plus centree avec  */
				/* le moins de deports                                             */
				/*---------------------------------------------------------------- */

				if (mode_debug) 
					printf("face %d debutbarremilieu %ld\n", face, barrexmilieu);
				first = 1;

				for (gauche = 0; gauche <= (NB_ITERATIONS / 2); gauche++) {
					barrexcourant = barrexmilieu - (gauche * lambda);
					if (mode_debug) 
						printf("\tbarrexcourant %ld", barrexcourant);
					if (conplotalim_dans_coeur(lst_equipo, lecoeur, barrexcourant, face))
					{

						if (first) {
							if (mode_debug) 
								printf("first place barre alim sont ok\n");
							first = 0;
							tab_plots[face].nb_deport = calcul_nbdeport(tab_plots, tab_coeur,
							     lecoeur, lst_equipo, barrexcourant, face);
							tab_plots[face].coord.xabs = barrexcourant;
						}
						else
						{
							if (mode_debug) 
								printf("place barre alim sont ok\n");
							if ((cout = calcul_nbdeport(tab_plots, tab_coeur, lecoeur, lst_equipo,
							     barrexcourant, face)) < tab_plots[face].nb_deport) {
								if (mode_debug) 
									printf("Nouveau cout bestg %d barrexcourant %ld\n", cout, barrexcourant);
								tab_plots[face].nb_deport = cout;
								tab_plots[face].coord.xabs = barrexcourant;
							}
						}
					}
				}

				/* ----------------------------------- */
				/* parcours droit, on repart du milieu */
				/* ----------------------------------- */

				for (droite = 1; droite <= (NB_ITERATIONS / 2); droite++) {
					barrexcourant = barrexmilieu + (droite * lambda);
					if (mode_debug) 
						printf("\tbarrexcourant %ld", barrexcourant);
					if (conplotalim_dans_coeur(lst_equipo, lecoeur, barrexcourant, face))
					{

						if (first) {
							if (mode_debug) 
								printf("first place barre alim sont ok\n");
							first = 0;
							tab_plots[face].nb_deport = calcul_nbdeport(tab_plots, tab_coeur,
							     lecoeur, lst_equipo, barrexcourant, face);
							tab_plots[face].coord.xabs = barrexcourant;
						}
						else
						{
							if (mode_debug) 
								printf("place barre alim sont ok\n");
							if ((cout = calcul_nbdeport(tab_plots, tab_coeur, lecoeur, lst_equipo,
							     barrexcourant, face)) < tab_plots[face].nb_deport) {
								if (mode_debug) 
									printf("Nouveau cout bestd %d barrexcourant %ld\n", cout, barrexcourant);
								tab_plots[face].nb_deport = cout;
								tab_plots[face].coord.xabs = barrexcourant;
							} else 
								if ((cout == tab_plots[face].nb_deport) 
								&& (tab_plots[face].coord.xabs <= barrexmilieu) 
								&& ((barrexcourant - barrexmilieu) < (barrexmilieu - tab_plots[face].coord.xabs))) {

								/* ------------------------------------------------------------------ */
								/* test egalite et proximite du milieu                                */
								/* vrai si egalite et position retenue precedemment etait a gauche    */
								/* du milieu et si position courante droite est plus proche du milieu */
								/* que la gauche, on garde la plus centree, uggh !                    */
								/* ------------------------------------------------------------------ */

								if (mode_debug) 
									printf("Nouveau cout bestd centree %d barrexcourant %ld\n", cout, barrexcourant);
								tab_plots[face].coord.xabs = barrexcourant;
							}
						} /* fin du else */
					}
				} /* fin du for */

				break; /* Fin Nord Sud */

			case EST  :
			case OUEST:

				diff = tab_plots[face].width - lecoeur.height;
				if (mode_debug) 
					printf("diff barre - face coeur %ld\n", diff);

              //if (diff >= 0)
					barreymilieu = tab_plots[face].coord.yabs =  ((lecoeur.coord.yabs - diff / 2) / SCALE_X) *
					    SCALE_X;

					/*---------------------------------------- */
					/* Hauteur barre de plots >= Hauteur coeur */
					/*---------------------------------------- */

                  //else
                  //barreymilieu = tab_plots[face].coord.yabs =  ((lecoeur.coord.yabs - diff / 2) / SCALE_X) *
                  //    SCALE_X;

				/* ------------------------------------------------------------ */
				/* Hauteur barre de plots < Hauteur coeur                       */
				/* La barre de plots est centree sur le coeur                   */
				/* Nb iterations a effectuer en deplacant la barre vers le bas  */
				/* puis vers le haut, on garde la position la plus centree avec */
				/* le moins de deports                                          */
				/* ------------------------------------------------------------ */

				if (mode_debug) 
					printf("face %d debutbarremilieu %ld\n", face, barreymilieu);

				first = 1;

				for (bas = 0; bas <= (NB_ITERATIONS / 2); bas++) {
					barreycourant = barreymilieu - (bas * lambda);
					if (mode_debug) 
						printf("\tbarreycourant %ld", barreycourant);

					if (conplotalim_dans_coeur(lst_equipo, lecoeur, barreycourant, face))
					{

						if (first) {
							if (mode_debug) 
								printf("first place barre alim sont ok\n");
							first = 0;
							tab_plots[face].nb_deport = calcul_nbdeport(tab_plots, tab_coeur,
							     lecoeur, lst_equipo, barreycourant, face);
							tab_plots[face].coord.yabs = barreycourant;
						}
						else
						{
							if (mode_debug) 
								printf("place barre alim sont ok\n");
							if ((cout = calcul_nbdeport(tab_plots, tab_coeur, lecoeur, lst_equipo,
							     barreycourant, face)) < tab_plots[face].nb_deport) {
								if (mode_debug) 
									printf("Nouveau cout bestb %d barreycourant %ld\n", cout, barreycourant);
								tab_plots[face].nb_deport = cout;
								tab_plots[face].coord.yabs = barreycourant;
							}
						}
					}
				}

				/* ---------------------------------- */
				/* parcours haut, on repart du milieu */
				/* ---------------------------------- */

				for (haut = 1; haut <= (NB_ITERATIONS / 2); haut++) {
					barreycourant = barreymilieu + (haut * lambda);
					if (mode_debug) 
						printf("\tbarreycourant %ld", barreycourant);

					if (conplotalim_dans_coeur(lst_equipo, lecoeur, barreycourant, face))
					{

						if (first) {
							if (mode_debug) 
								printf("first place barre alim sont ok\n");
							first = 0;
							tab_plots[face].nb_deport = calcul_nbdeport(tab_plots, tab_coeur,
							     lecoeur, lst_equipo, barreycourant, face);
							tab_plots[face].coord.yabs = barreycourant;
						}
						else
						{
							if (mode_debug) 
								printf("place barre alim sont ok\n");

							if ((cout = calcul_nbdeport(tab_plots, tab_coeur, lecoeur, lst_equipo,
							     barreycourant, face)) < tab_plots[face].nb_deport) {
								if (mode_debug) 
									printf("Nouveau cout besth %d barreycourant %ld\n", cout, barreycourant);
								tab_plots[face].nb_deport = cout;
								tab_plots[face].coord.yabs = barreycourant;
							} else 
								if ((cout == tab_plots[face].nb_deport) 
								&& (tab_plots[face].coord.yabs <= barreymilieu) 
								&& ((barreycourant - barreymilieu) < (barreymilieu - tab_plots[face].coord.yabs))) {

									/* ----------------------------------------------------------------- */
									/* test egalite et proximite du milieu                               */
									/* vrai si egalite et position retenue precedemment etait en bas     */
									/* du milieu et si position courante haute est plus proche du milieu */
									/* que celle du bas, on garde la plus centree, uggh !                */
									/* ----------------------------------------------------------------- */

								if (mode_debug) 
									printf("Nouveau cout besth centre %d barreycourant %ld\n", cout, barreycourant);
								tab_plots[face].coord.yabs = barreycourant;
							}
						}
					}
				}

				break; /* Fin Est Ouest */
			} /* Fin du switch */

			if (first) 
				/* ------------------------------------------------------------------------ */
				/* Ce qui veut dire qu'on a pas pu placer la barre de plot a cause des alim */
				/* ------------------------------------------------------------------------ */
				ringerreur(ERR_BARREALIM, (void * ) & face, NULL);

			if (mode_debug) 
				printf("Placement de la barre %d xabs %ld yabs  %ld\n", face, tab_plots[face].coord.xabs, tab_plots[face].coord.yabs);
		} /* Fin du if barre existante */

	} /* Fin du for */

	place_vertical_barreplot(tab_plots, tab_coeur, lecoeur, lst_equipo);
}

/*--------------------------------------------------------------------------------*/
/* Placement vertical des barres de plots = maximum des 4 faces. Pour chaque face */
/* nb connect coeur + nb connect plot + 2 fois largalim                           */
/*--------------------------------------------------------------------------------*/

void place_vertical_barreplot(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES], COEUR lecoeur,
	LST_EQUIPO lst_equipo)
{
	LST_EQUIPO equipo_vdd, equipo_vss;
	LST_PSEUDO_CON liste;
	int	face;
	long	largvdd, largvss, diff1, diff2, diff;

	long	cptcon = 0, maxcoeur = 0, maxplot = 0, largalim, dist;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

	largeur_vddvss(equipo_vdd, equipo_vss, &largvdd, &largvss);

	if (largvdd < largvss) 
		largalim = largvss; 
	else 
		largalim = largvdd;

	for (face = 0; face < NB_FACES; face++) {

		if (tab_plots[face].width != 0) {
			liste = tab_plots[face].lst_con;
			cptcon = 0;

			while (liste != NULL) { 
				cptcon++; 
				liste = liste->suiv; 
			}
			if (cptcon > maxplot) 
				maxplot = cptcon;

			liste = tab_coeur[face];
			cptcon = 0;

			while (liste != NULL) { 
				cptcon++; 
				liste = liste->suiv; 
			}

			if (cptcon > maxcoeur) 
				maxcoeur = cptcon;
		}
	}

	dist = (maxcoeur * pitch + maxplot * pitch + 2 * largalim) * 2;

	for (face = 0; face < NB_FACES; face++)
		(tab_plots[face].coord).piste = dist / pitch;

	if (tab_plots[NORD].width != 0)
		(tab_plots[NORD].coord).yabs = (lecoeur.coord).yabs + lecoeur.height + dist;
	if (tab_plots[SUD].width != 0)
		(tab_plots[SUD].coord).yabs = (lecoeur.coord).yabs - dist;
	if (tab_plots[OUEST].width != 0)
		(tab_plots[OUEST].coord).xabs = (lecoeur.coord).xabs - dist;
	if (tab_plots[EST].width != 0)
		(tab_plots[EST].coord).xabs = (lecoeur.coord).xabs + lecoeur.width + dist;

	if (mode_debug) 
		printf("**** nord x%ld y%ld sud x%ld y%ld ouest x%ld y%ld est x%ld y%ld\n", (tab_plots[NORD].coord).xabs,
		     (tab_plots[NORD].coord).yabs, (tab_plots[SUD].coord).xabs, (tab_plots[SUD].coord).yabs, (tab_plots[OUEST].coord).xabs,
		     (tab_plots[OUEST].coord).yabs, (tab_plots[EST].coord).xabs, (tab_plots[EST].coord).yabs);

	/*-------------  VERIFICATION DU NON CROISEMEMT DES BARRES MITOYENNES ----------*/

	for (face = 0; face < NB_FACES; face++) {
		diff1 = diff2 = 0;

		if (0 != tab_plots[face].width)
			switch (face) {
			case NORD:
				if (mode_debug) 
					printf("nord\t");
				if (0 != tab_plots[EST].width)
					diff1 = ((lecoeur.coord.yabs + lecoeur.height + tab_plots[NORD].coord.piste * pitch)
					    -(tab_plots[EST].coord.yabs + tab_plots[EST].width));
				if (0 != tab_plots[OUEST].width)
					diff2 = ((lecoeur.coord.yabs + lecoeur.height + tab_plots[NORD].coord.piste * pitch)
					    -(tab_plots[OUEST].coord.yabs + tab_plots[OUEST].width));

				if (diff1 < diff2) 
					diff = diff1; 
				else 
					diff = diff2;
				if (diff < 0) 
					tab_plots[NORD].coord.piste += ((-diff + pitch) / pitch);

				break;

			case SUD:
				if (mode_debug) 
					printf("sud\t");
				if (0 != tab_plots[EST].width)
					diff1 = ( -(lecoeur.coord.yabs - tab_plots[SUD].coord.piste * pitch) + (tab_plots[EST].coord.yabs));
				if (0 != tab_plots[OUEST].width)
					diff2 = ( -(lecoeur.coord.yabs - tab_plots[SUD].coord.piste * pitch) + (tab_plots[OUEST].coord.yabs));

				if (diff1 < diff2) 
					diff = diff1; 
				else 
					diff = diff2;
				if (diff < 0) 
					tab_plots[SUD].coord.piste += ((-diff + pitch) / pitch);

				break;

			case EST:
				if (mode_debug) 
					printf("est\t");
				if (0 != tab_plots[NORD].width)
					diff1 = ((lecoeur.coord.xabs + lecoeur.width + tab_plots[EST].coord.piste * pitch) -
					    (tab_plots[NORD].coord.xabs + tab_plots[NORD].width));
				if (0 != tab_plots[SUD].width)
					diff2 = ((lecoeur.coord.xabs + lecoeur.width + tab_plots[EST].coord.piste * pitch) -
					    (tab_plots[SUD].coord.xabs + tab_plots[SUD].width));

				if (diff1 < diff2) 
					diff = diff1; 
				else 
					diff = diff2;
				if (diff < 0) 
					tab_plots[EST].coord.piste += ((-diff + pitch) / pitch);
				break;

			case OUEST:
				if (mode_debug) 
					printf("ouest\t");
				if (0 != tab_plots[NORD].width)
					diff1 = (-(lecoeur.coord.xabs - tab_plots[OUEST].coord.piste * pitch ) + (tab_plots[NORD].coord.xabs));

				if (0 != tab_plots[SUD].width)
					diff2 = (-(lecoeur.coord.xabs - tab_plots[OUEST].coord.piste * pitch ) + (tab_plots[SUD].coord.xabs));

				if (diff1 < diff2) 
					diff = diff1; 
				else 
					diff = diff2;
				if (diff < 0) 
					tab_plots[OUEST].coord.piste += ((-diff + pitch) / pitch);
				break;
			}
	} /* fin du for */

	if (tab_plots[NORD].width != 0)
		(tab_plots[NORD].coord).yabs = tab_plots[NORD].coord.piste * pitch + lecoeur.height;
	if (tab_plots[SUD].width != 0)
		(tab_plots[SUD].coord).yabs = (lecoeur.coord).yabs - tab_plots[SUD].coord.piste * pitch;
	if (tab_plots[OUEST].width != 0)
		(tab_plots[OUEST].coord).xabs = (lecoeur.coord).xabs - tab_plots[OUEST].coord.piste * pitch;
	if (tab_plots[EST].width != 0)
		(tab_plots[EST].coord).xabs = (lecoeur.coord).xabs + lecoeur.width + tab_plots[EST].coord.piste * pitch;

	if (mode_debug) 
		printf("**** nord x%ld y%ld sud x%ld y%ld ouest x%ld y%ld est x%ld y%ld\n", (tab_plots[NORD].coord).xabs,
		     (tab_plots[NORD].coord).yabs, (tab_plots[SUD].coord).xabs, (tab_plots[SUD].coord).yabs, (tab_plots[OUEST].coord).xabs,
		     (tab_plots[OUEST].coord).yabs, (tab_plots[EST].coord).xabs, (tab_plots[EST].coord).yabs);

}

/*-----------------------------------------------------------------------------------*/
/* Cette procedure calcule le nombre total de deport de la barre de plot placee      */
/* a barrecourant. Cout des alim, connecteurs de plots a deporter, calcul du nombres */
/* de cycles.                                                                        */
/* nb: la fonction moins_dun_pitch_confacecoeur met a jour la liste des vis a vis    */
/*-----------------------------------------------------------------------------------*/

int calcul_nbdeport(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES], COEUR lecoeur,
	LST_EQUIPO lst_equipo, long barrecourant, int face)
{
	int	nbdepalim = 0, nbdepplots = 0, nbdepcycles = 0, nbdeptotal;
	long	position;
	LST_PSEUDO_CON liste_plots, liste_coeur;
	LST_EQUIPO     liste_eq;

	/* -------------------------------------------------------------------- */
	/* Calcul du nombre de deport provoques par l'alim par rapport au pitch */
	/* -------------------------------------------------------------------- */

	if (mode_debug) 
		printf("\tCalcul_nbdeport\n");

	nbdepalim = calcul_nbdeport_equipo_alim(lst_equipo, barrecourant, face);

	liste_eq    = lst_equipo;
	liste_plots = tab_plots[face].lst_con;
	liste_coeur = tab_coeur[face];

	/* ----------------------------------------------- */
	/* Mise a nulle des listes de vis a vis des equipo */
	/* ----------------------------------------------- */

	while (liste_eq) {
		liste_eq->lst_visavis = NULL; /* prevoir desallocation */
		liste_eq = liste_eq->suiv;
	}

	switch (face) {
	case NORD:
	case SUD :

		while (liste_plots != NULL) {
			if (mode_debug) 
				printf("\t%s\n", liste_plots->nom_con);

			if(!isvdd(liste_plots->nom_con) && !isvss(liste_plots->nom_con)) {
				position = (liste_plots->coord)->xabs + barrecourant;

				if (mode_debug) 
					printf("position %ld du connecteur %s face %d\n", position, liste_plots->nom_con,
					     face);

				/* --------------------------- */
				/* en dehors du coeur = deport */
				/* --------------------------- */

				if ((position < lecoeur.coord.xabs) ||  (position > (lecoeur.coord.xabs + lecoeur.width))) { 
					if (mode_debug) 
						printf("deport en dehors coeur\t");
					nbdepplots++;
				}  else if (moins_dun_pitch_confacecoeur(position, liste_plots , liste_coeur, lst_equipo,
				     face)) { 
					if (mode_debug) 
						printf("deport - 1 pitch\t");
					nbdepplots++;
				}

			} /* fin du if */

			liste_plots = liste_plots->suiv;
		}  /* fin du while */

		break;

	case EST  :
	case OUEST:

		while (liste_plots != NULL) {
			if (mode_debug) 
				printf("\t%s\n", liste_plots->nom_con);

			if(!isvdd(liste_plots->nom_con) && !isvss(liste_plots->nom_con)) {
				position = (liste_plots->coord)->yabs + barrecourant;

				if (mode_debug) 
					printf("position %ld du connecteur %s face %d\n", position, liste_plots->nom_con,
					     face);

				/* --------------------------- */
				/* en dehors du coeur = deport */
				/* --------------------------- */

				if ((position < lecoeur.coord.yabs) ||  (position > (lecoeur.coord.yabs + lecoeur.height))) { 
					if (mode_debug) 
						printf("deport en dehors coeur\t");
					nbdepplots++;
				}  else if (moins_dun_pitch_confacecoeur(position, liste_plots , liste_coeur, lst_equipo,
				     face)) { 
					if (mode_debug) 
						printf("deport - 1 pitch\n");
					nbdepplots++;
				}

			}

			liste_plots = liste_plots->suiv;
		}

		break;
	}  /* Fin du switch() */

	if (mode_debug) 
		printf("\tNb depplots %d", nbdepplots);

	/* --------------------------------------------------------------------------------------------------- */
	/* Par la methode deroutage , il n'y aura pas de cycles, donc inutile de calculer les cycles eventuels */
	/* nbdepcycles = calcul_nbdeport_cyclesface(lst_equipo);                                               */
	/* --------------------------------------------------------------------------------------------------- */

	nbdeptotal = nbdepalim + nbdepplots + nbdepcycles;

	if (nbdeptotal < 0) 
		nbdeptotal = 0;

	if (mode_debug) 
		printf("Nombre total de deport %d pour face %d et barrecourant %ld\n", nbdeptotal, face, barrecourant);

	return(nbdeptotal);

}

/*------------------------------------------------------------------------------------*/
/* Cette fonction retourne vrai 1 si la position du connecteur plot est a moins d'un  */
/* pitch d'un connecteur quelconque du coeur, sinon 0; et mets a jour la liste des    */
/* vis a vis des equipo pour calcul ulterieur du nombre de cycle pour la face         */
/* Utilisation de la fonction calcul distance entre 2 connecteurs                     */
/*------------------------------------------------------------------------------------*/

int moins_dun_pitch_confacecoeur(long position, LST_PSEUDO_CON plotcon,
	LST_PSEUDO_CON liste_coeur, LST_EQUIPO lst_equipo,
	int face)
{
	long	dist, largcon = 0, largconprec = 0, largeurmin = 0, dminmetalmetal = 0;
	long	coordcon = 0, coordconprec = 0;

	if (liste_coeur == NULL) 
		return(0); /* pas de connecteurs coeurs donc grille
	                                    formee par les con de plots */

	switch (face) {
	case NORD:
	case SUD :
		if ((liste_coeur->layer != ymetal) || (plotcon->layer != ymetal)) {
			if (ymetal_width < xmetal_width) 
				largeurmin = ymetal_width;
			else 
				largeurmin = xmetal_width;
			/*	dminmetalmetal = xmetal_dmin; */
			dminmetalmetal = ymetal_dmin;
		} else
		 {
			largeurmin = ymetal_width;
			dminmetalmetal = ymetal_dmin;
		}

		while (liste_coeur != NULL) {
			dist = distance_con_con(liste_coeur->coord->xabs, 0L, liste_coeur->largeur, liste_coeur->layer, position,
			     0L, plotcon->largeur, plotcon->layer, face);

			if ((liste_coeur->largeur > largeurmin) || (plotcon->largeur > largeurmin)) {

			if (mode_debug) 
				printf("confacecoeur coordcon %ld coordconprec %ld largcon %ld largconprec %ld dist %ld\n",
					coordcon,coordconprec,largcon,largconprec,dist);  

				if (dist < dminmetalmetal) {
					ajout_visavis(lst_equipo, liste_coeur, ((plotcon->con_lo)->SIG)->INDEX);
					if(!isvdd(plotcon->nom_con) && !isvss(plotcon->nom_con))
						return(1);
	
					/* ------------------------------------------------------------- */
					/* pas de deport s'il s'agit d'une alim, on continue a parcourir */
					/* ------------------------------------------------------------- */
				}
			}  else	{
				if (dist  < pitch) {
					if (mode_debug) 
						printf("visavis posconcoeur %ld position %ld\n", (liste_coeur->coord)->xabs,
						     position);

					ajout_visavis(lst_equipo, liste_coeur, ((plotcon->con_lo)->SIG)->INDEX);
					return(1);  /* deport a faire */
				}
			} /* fin du else */

			liste_coeur = liste_coeur->suiv;
		} /* fin du while */

		return(0); /* Pas d'ringerreur c'est lesieur */

		break;

	case EST  :
	case OUEST:
		if ((liste_coeur->layer != xmetal) || (plotcon->layer != xmetal)) {
			if (ymetal_width < xmetal_width) 
				largeurmin = ymetal_width;
			else 
				largeurmin = xmetal_width;
			dminmetalmetal = xmetal_dmin;
		} else
		 {
			largeurmin = xmetal_width;
			dminmetalmetal = xmetal_dmin;
		}

		while (liste_coeur != NULL) {
			dist = distance_con_con(0L, liste_coeur->coord->yabs, liste_coeur->largeur, liste_coeur->layer, 0L,
			     position, plotcon->largeur, plotcon->layer, face);

			if ((liste_coeur->largeur > largeurmin) || (plotcon->largeur > largeurmin)) {

			if (mode_debug) 
				printf("confacecoeur coordcon %ld coordconprec %ld largcon %ld largconprec %ld dist %ld\n",
					coordcon,coordconprec,largcon,largconprec,dist); 

				if (dist < dminmetalmetal) {
					ajout_visavis(lst_equipo, liste_coeur, ((plotcon->con_lo)->SIG)->INDEX);
					if(!isvdd(plotcon->nom_con) && !isvss(plotcon->nom_con))
						return(1);

					/* ------------------------------------------------------------- */
					/* pas de deport s'il s'agit d'une alim, on continue a parcourir */
					/* ------------------------------------------------------------- */
				}
			}  else	 {
				if (dist < pitch) {
					ajout_visavis(lst_equipo, liste_coeur, ((plotcon->con_lo)->SIG)->INDEX);
					return(1);  /* deport a faire */
				}
			} /*  fin  */

			liste_coeur = liste_coeur->suiv;
		}

		return(0); /* Pas d'ringerreur c'est lesieur */
	}
	return(0);

}

/*--------------------------------------------------------------------------------*/
/* Calcul du nombre de cycles avec la liste des visavis des equipo                */
/* Prevoir liberation des ptype liste allouees.                                   */
/*--------------------------------------------------------------------------------*/

int calcul_nbdeport_cyclesface(LST_EQUIPO lst_equipo)
{
	int	nbdeport = 0;
	ptype_list * liste;

	while (lst_equipo != NULL) {
		liste = lst_equipo->lst_visavis;

		while (liste != NULL) {
			if (existe_visavis(lst_equipo->index, liste->TYPE, lst_equipo)) {
				nbdeport++;
			}

			liste = liste->NEXT;
		}
		lst_equipo = lst_equipo->suiv;
	}

	if (mode_debug) 
		printf("Nb deports cycles trouves %d\n", nbdeport);

	return(nbdeport);
}

/*---------------------------------------------------------------------------------*/
/* Calcul du nombre de deports possibles engendres par le placement de la barre    */
/*---------------------------------------------------------------------------------*/

int calcul_nbdeport_equipo_alim(LST_EQUIPO lst_equipo, long barrecourant, int face)
{
	LST_EQUIPO equipo_vdd, equipo_vss;
	LST_PSEUDO_CON con;
	chain_list     * ptvdd = NULL , *ptvss = NULL,  *ptcour;
	int	nbdeport = 0, bonus , malus;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

	ptvdd = equipo_vdd->lst_con;
	ptvss = equipo_vss->lst_con;

	/* ------------------ */
	/* trt des equipo vdd */
	/* ------------------ */

	if (mode_debug) 
		printf("trt eq VDD\n");
	ptcour = ptvdd;

	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;

		if ((bonus = croisement_con(ptcour, ptvdd, barrecourant, face)) != 0) {
			if (mode_debug) 
				printf("bonus con %s de %d\n", con->nom_con, bonus);
			nbdeport -= (bonus / pitch) ;
		}

		if ((malus = croisement_con(ptcour, ptvss, barrecourant, face)) != 0) {
			if (mode_debug) 
				printf("malus con %s de %ld\n", con->nom_con, ((vdd_width / pitch) + (vdd_width % pitch)));
			nbdeport += ((vdd_width / pitch) + (vdd_width % pitch)) ;
		}

		ptcour = ptcour->NEXT;
	}

	/* ------------------ */
	/* trt des equipo vss */
	/* ------------------ */

	ptcour = ptvss;

	if (mode_debug) 
		printf("trt eq VSS\n");

	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;

		bonus = croisement_con(ptcour, ptvss, barrecourant, face);

		if (bonus != 0) {
			if (mode_debug) 
				printf("bonus con %s de %d\n", con->nom_con, bonus);
			nbdeport -= (bonus / pitch) ;
		}

		ptcour = ptcour->NEXT;
	}

	if (mode_debug) 
		printf("Nb deports total alim %d \n", nbdeport);

	return(nbdeport);
}

/*--------------------------------------------------------------------------------*/
/* Parcours de la liste des connecteurs d'alim pour voir s'il existe des vis      */
/* a vis. Si aucun vis a vis retour 0, sinon retour du croisement des 2           */
/* connecteurs                                                                    */
/*--------------------------------------------------------------------------------*/

int croisement_con(chain_list *ptcour, chain_list *ptalim, 
	long barrecourant, int	face)
{

	LST_PSEUDO_CON con, concour;
	long	position = 0, positioncour = 0, dminmetalmetal = 0, diff = 0;

	concour = (LST_PSEUDO_CON) ptcour->DATA;

	/*
	if (mode_debug) 
		printf("\tCroisement con  %s larg %ld face %d x %ld y %ld\n",concour->nom_con,
			concour->largeur,face,(concour->coord)->xabs,(concour->coord)->yabs);
	*/

	switch (face) {
	case NORD:
	case SUD :
		positioncour = (concour->coord)->xabs + barrecourant;
		dminmetalmetal = ymetal_dmin;
		break;
	case EST  :
	case OUEST:
		positioncour = (concour->coord)->yabs + barrecourant;
		dminmetalmetal = xmetal_dmin;
		break;
	}

	while (ptalim != NULL) {
		con = (LST_PSEUDO_CON) ptalim->DATA;

		if ((ptalim != ptcour)  && (con->face == face) && (con->coeur_plot != concour->coeur_plot)) {
			switch (face) {
			case NORD:
			case SUD :
				position = (con->coord)->xabs + barrecourant;
				break;
			case EST  :
			case OUEST:
				position = (con->coord)->yabs + barrecourant;
				break;
			}

			/* --------------------------------------- */
			/* meme layer car on ne s'en preoccupe pas */
			/* --------------------------------------- */

			diff = distance_con_con(position, position, con->largeur, con->layer, positioncour, positioncour,
			     concour->largeur, con->layer, face);

			/* diff = position - (con->width / 2) - (con->width % 2)
				- (positioncour + (concour->width / 2) + (concour->width % 2));
          		Avant simplification */

			if ((diff < 0) || (diff < dminmetalmetal))
				if (mode_debug) 
					printf("\tposition cour %ld nom %s position %ld ***diffcrois %ld larg %ld\n", positioncour,
					     con->nom_con, position, diff, con->largeur);

			if (diff < 0)  
				return((int) -diff);
			if (diff < dminmetalmetal) 
				return ((int) pitch);
		} /* fin du if */

		ptalim = ptalim->NEXT;
	}

	return((int) 0);  /* Aucun croisement trouve */
}

/*-----------------------------------------------------------------------------------*/
/*  procedure qui retourne les equipotentielles d'alim                               */
/*-----------------------------------------------------------------------------------*/

void recherche_equipo_alim(LST_EQUIPO *equipo_vdd,
	LST_EQUIPO *equipo_vss,
	LST_EQUIPO  lst_equipo)
{
	LST_PSEUDO_CON con;

	(*equipo_vdd) = NULL;
	(*equipo_vss) = NULL;

	while (lst_equipo != NULL) {
	    con = (LST_PSEUDO_CON) ((lst_equipo->lst_con)->DATA);
		if (lst_equipo->type == ALIM) {
			con = (LST_PSEUDO_CON) ((lst_equipo->lst_con)->DATA);

			if (isvdd(con->nom_con)) 
				(*equipo_vdd) = lst_equipo;
			else 
				(*equipo_vss) = lst_equipo;

			if (((*equipo_vss) != NULL) && ((*equipo_vdd) != NULL))
				break;
		}
		lst_equipo = lst_equipo->suiv;
	}

	if (((*equipo_vss) == NULL) || ((*equipo_vdd) == NULL))
		ringerreur(ERR_MANQUEALIM, NULL, NULL);
}

/*-----------------------------------------------------------------------------------*/
/* Verification si les connecteurs d'alim sont dans la face du coeur                 */
/*-----------------------------------------------------------------------------------*/

int conplotalim_dans_coeur(LST_EQUIPO lst_equipo, COEUR coeur,
	long barrecourant, int face)
{
	LST_EQUIPO equipo_vdd, equipo_vss;
	LST_PSEUDO_CON con;
	chain_list * liste_vdd;
	chain_list * liste_vss;
	long	alasoupe;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

	liste_vdd = equipo_vdd->lst_con;
	liste_vss = equipo_vss->lst_con;

	if (mode_debug) 
		printf("barrecourant %ld coeurheight %ld coeurwidth %ld\n", barrecourant, coeur.height, coeur.width);

	while (liste_vdd != NULL) {
		con = (LST_PSEUDO_CON) liste_vdd->DATA;
		if ((con->coeur_plot == PLOT_CON) && (con->face == face))

			switch (con->face) {
			case NORD:
			case SUD :
				if (((barrecourant + con->coord->xabs - con->largeur / 2) < (coeur.coord).xabs) ||  ((barrecourant +
				    con->coord->xabs + con->largeur / 2) > ((coeur.coord).xabs + coeur.width)))
					return(0); /* le connecteur n'est pas en face du coeur */

				break;

			case EST  :
			case OUEST:
				alasoupe =  ((barrecourant + con->coord->yabs - con->largeur / 2));
				if (mode_debug) 
					printf("binf %ld\t", alasoupe);
				alasoupe =  ((barrecourant + con->coord->yabs + con->largeur / 2));
				if (mode_debug) 
					printf("bsup %ld\t", alasoupe);

				if (mode_debug) 
					printf("plot %s coord x %ld y %ld larg %ld\n", con->nom_con, con->coord->xabs, con->coord->yabs,
					     con->largeur);
				if (((barrecourant + con->coord->yabs - con->largeur / 2) < (coeur.coord).yabs) ||  ((barrecourant +
				    con->coord->yabs + con->largeur / 2) > ((coeur.coord).yabs + coeur.height)))
					return(0); /* le connecteur n'est pas en face du coeur */

				break;
			}
		liste_vdd = liste_vdd->NEXT;
	}

	while (liste_vss != NULL) {
		con = (LST_PSEUDO_CON) liste_vss->DATA;
		if ((con->coeur_plot == PLOT_CON) && (con->face == face))

			switch (con->face) {
			case NORD:
			case SUD :
				if (((barrecourant + con->coord->xabs - con->largeur / 2) < (coeur.coord).xabs) ||  ((barrecourant +
				    con->coord->xabs + con->largeur / 2) > ((coeur.coord).xabs + coeur.width)))
					return(0); /* le connecteur n'est pas en face du coeur */

				break;

			case EST  :
			case OUEST:
				alasoupe =  ((barrecourant + con->coord->yabs - con->largeur / 2));
				if (mode_debug) 
					printf("binf %ld\t", alasoupe);
				alasoupe =  ((barrecourant + con->coord->yabs + con->largeur / 2));
				if (mode_debug) 
					printf("bsup %ld\t", alasoupe);

				if (mode_debug) 
					printf("plot %s coord x %ld y %ld larg %ld\n", con->nom_con, con->coord->xabs, con->coord->yabs,
					     con->largeur);
				if (((barrecourant + con->coord->yabs - con->largeur / 2) < (coeur.coord).yabs) ||  ((barrecourant +
				    con->coord->yabs + con->largeur / 2) > ((coeur.coord).yabs + coeur.height)))
					return(0); /* le connecteur n'est pas en face du coeur */

				break;
			}
		liste_vss = liste_vss->NEXT;
	}

	/* ningun problema */
	return(1);

}

/*--------------------------------------------------------------------------------*/
/* Apres le placement de la barre de plots, on mets a jour les coordonnes des     */
/* connecteurs                                                                    */
/*--------------------------------------------------------------------------------*/

void maj_coordplots(BARRE_PLOTS tab_plots[NB_FACES], LST_EQUIPO lst_equipo)
{
	LST_EQUIPO     liste_eq = lst_equipo;
	LST_PSEUDO_CON liste;
	int	face;
  //char	niveau;
	long	position, /*largeurmin,*/ largeurmax;

	if (WVIA_ALU1 > WVIA_ALU2) 
		largeurmax = WVIA_ALU1 * lambda;
	else 
		largeurmax = WVIA_ALU2 * lambda;

	if (mode_debug) 
		printf("Largeurmax = %ld\n", largeurmax);

	/* ----------------------------------------------- */
	/* Mise a nulle des listes de vis a vis des equipo */
	/* ----------------------------------------------- */

	while (liste_eq) {
		liste_eq->lst_visavis = NULL; /* prevoir desallocation */
		liste_eq = liste_eq->suiv;
	}

	for (face = 0; face < NB_FACES; face++) {
		liste = tab_plots[face].lst_con;

		if (mode_debug) 
			printf("Face %d\n", face);

		switch (face) {

		case NORD:
		case SUD :
        //niveau = ymetal;
        //largeurmin = ymetal_width;

			while (liste != NULL) {
				if (mode_debug) 
					printf("\t%s width = %ld\n", liste->nom_con, liste->largeur);

				if(!isvdd(liste->nom_con) && !isvss(liste->nom_con) && (liste->largeur > largeurmax)) 
					ringerreur(ERR_CONLARGEUR, liste, NULL);

				switch (liste->layer) {
				case ALU1: 
					if (liste->largeur < WMIN_ALU1) /* ringerreur largeur connecteur */
						ringerreur(ERR_CONLARGEUR, liste, NULL);
					break;
				case ALU2: 
					if (liste->largeur < WMIN_ALU2) /* ringerreur largeur connecteur */
						ringerreur(ERR_CONLARGEUR, liste, NULL);
					break;
				default :  
					ringerreur(ERR_CONLARGEUR, liste, NULL);
				}

				position = (liste->coord)->xabs + (tab_plots[face].coord).xabs;
				(liste->coord)->xabs = position;
				(liste->coord)->yabs = tab_plots[face].coord.yabs;
				(liste->coord)->piste = (tab_plots[face].coord).piste;

				if (mode_debug) 
					printf("position %ld du connecteur %s face %d\n", position, liste->nom_con, face);

				liste = liste->suiv;
			}  /* fin du while */
			break;

		case EST  :
		case OUEST:
        //niveau = xmetal;
        //largeurmin = xmetal_width;

			while (liste != NULL) {
				if (mode_debug) 
					printf("\t%s width=%ld y %ld ybarre %ld \n", liste->nom_con, liste->largeur, liste->coord->yabs,
					     tab_plots[face].coord.yabs);
				if(!isvdd(liste->nom_con) && !isvss(liste->nom_con) && (liste->largeur > largeurmax)) 
					ringerreur(ERR_CONLARGEUR, liste, NULL);

				switch (liste->layer) {
				case ALU1: 
					if (liste->largeur < WMIN_ALU1) /* ringerreur largeur connecteur */
						ringerreur(ERR_CONLARGEUR, liste, NULL);
					break;
				case ALU2: 
					if (liste->largeur < WMIN_ALU2) /* ringerreur largeur connecteur */

						ringerreur(ERR_CONLARGEUR, liste, NULL);
					break;
				default :  
					ringerreur(ERR_CONLARGEUR, liste, NULL);
				}

				position = (liste->coord)->yabs + (tab_plots[face].coord).yabs;
				(liste->coord)->yabs = position;
				(liste->coord)->xabs = (tab_plots[face].coord).xabs;
				(liste->coord)->piste = (tab_plots[face].coord).piste;

				if (mode_debug) 
					printf("position %ld du connecteur %s face %d\n", position, liste->nom_con, face);

				liste = liste->suiv;
			}  /* fin du while */
			break;
		} /* fin du switch */

	} /* fin du for */
}

/*------------------------------------------------------------------------------------*/
/* Fabrication de la grille finale a partir des connecteurs de plots.                 */
/* Verification de la distance des connecteurs entre eux leur largeur                 */
/* Mise a jour des futurs deports.                                                    */
/*                                                                                    */
/* Rq: la dmin a1 a1 est de 3 cad 30 avec le SCALE_X actuel, pour faire les nouveaux  */
/* pas de grille, on se sert de dmin/2, mais les pas doivent etre multiples de        */
/* scale_x, donc pour les calculs des nouveaux pas on prend                           */
/* ((pos + scale_x/2) /scale_x) *scale_x                                              */
/*------------------------------------------------------------------------------------*/

void fabrique_grille_finale(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES], COEUR lecoeur,
	LST_EQUIPO lst_equipo, GRILLE tab_grilles[NB_FACES])
{
	LST_PSEUDO_CON liste, liste_coeur;
	int	face;
	long	position;

	if (mode_debug) 
		printf("Fabrique grille finale\n");
	if (mode_debug) 
		printf("Coord coeur x%ld y%ld w%ld h%ld\n", lecoeur.coord.xabs, lecoeur.coord.yabs, lecoeur.width, lecoeur.height);

	for (face = 0; face < NB_FACES; face++) {
		liste = tab_plots[face].lst_con;
		liste_coeur = tab_coeur[face];

		if (mode_debug) 
			printf("Face %d\n", face);

		switch (face) {
		case NORD:
		case SUD :
        //niveau = ymetal;
        //largeurmin = ymetal_width;

			while (liste != NULL) {
				if (mode_debug) 
					printf("\t%s\n", liste->nom_con);

				if (moins_dun_pitch_preccon(liste))
					ringerreur(ERR_CONDISTANCE, liste, NULL);

				position = (liste->coord)->xabs;

				if (mode_debug) 
					printf("position %ld du connecteur %s face %d\n", position, liste->nom_con, face);

				if(!isvdd(liste->nom_con) && !isvss(liste->nom_con)) {

					/* --------------------------- */
					/* en dehors du coeur = deport */
					/* --------------------------- */

					if (position < (lecoeur.coord).xabs) {
						if (mode_debug) 
							printf("Deportg connecteur %s %ld\n", liste->nom_con, position);
						alloue_coord(0L, 0, &(liste->deport));
						/* liste->deport = (PT_COORDONNEES) mbkalloc ((unsigned int) sizeof(COORDONNEES)); */

						(liste->deport)->piste = 0; /* deport uniquement pour differencier du layer */

						ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_deportg),
						    face);
					} else if (position > ((lecoeur.coord).xabs + lecoeur.width)) {
						if (mode_debug) 
							printf("Deportd connecteur %s %ld\n", liste->nom_con, position);
						alloue_coord(0L, 0, &(liste->deport));
						/* liste->deport = (PT_COORDONNEES) mbkalloc ((unsigned int) sizeof(COORDONNEES)); */

						(liste->deport)->piste = 0; /* deport uniquement pour differencier du layer */

						ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_deportd),
						    face);
					}  else if (moins_dun_pitch_confacecoeur(position, liste, liste_coeur, lst_equipo,
					     face)) { /* Ne forme pas un nouveau pas de grille */
						if (mode_debug) 
							printf("Deport connecteur %s\n", liste->nom_con);
						alloue_coord(0L, 0, &(liste->deport));
						/* liste->deport = (PT_COORDONNEES) mbkalloc ((unsigned int) sizeof(COORDONNEES)); */

						(liste->deport)->piste = 0; /* deport uniquement pour differencier du layer */
					} else
					 {
						if (mode_debug) 
							printf("Nouveau pas connecteur %s %ld\n", liste->nom_con, position);

						ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_pas), face);
					}

				} /* fin du if  */  else { 

					/* ------------------------------------------------------------------------- */
					/* Il s'agit d'un plot d'alimentation, c'est un pas obligatoire de la grille */
					/* si ringerreur ici => ringerreur dans le placement de la barre de plot!    */
					/* ------------------------------------------------------------------------- */

					if (position < (lecoeur.coord).xabs) 
						ringerreur(ERR_BARREALIM, (void * ) & face, NULL);
					if (position > ((lecoeur.coord).xabs + lecoeur.width)) 
						ringerreur(ERR_BARREALIM, (void * ) & face, NULL);

					moins_dun_pitch_confacecoeur(position, liste, liste_coeur, lst_equipo, face);
					if (mode_debug) 
						printf("Nouveau pas ALIM connecteur %s %ld\n", liste->nom_con, position);

					ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_pas), face);
				}

				liste = liste->suiv;
			}  /* fin du while */
			break;

		case EST  :
		case OUEST:
        //niveau = xmetal;
		//largeurmin = xmetal_width;

			while (liste != NULL) {
				if (mode_debug) 
					printf("\t%s y %ld ybarre %ld \n", liste->nom_con, liste->coord->yabs, tab_plots[face].coord.yabs);

				if (moins_dun_pitch_preccon(liste))
					ringerreur(ERR_CONDISTANCE, liste, NULL);

				position = (liste->coord)->yabs;
				if (mode_debug) 
					printf("position %ld du connecteur %s face %d\n", position, liste->nom_con, face);

				if(!isvdd(liste->nom_con) && !isvss(liste->nom_con)) {

					/* --------------------------- */
					/* en dehors du coeur = deport */
					/* --------------------------- */

					if (position < (lecoeur.coord).yabs) {
						if (mode_debug) 
							printf("Deport connecteur %s %ld\n", liste->nom_con, position);
						alloue_coord(0L, 0, &(liste->deport));
						/* liste->deport = (PT_COORDONNEES) mbkalloc ((unsigned int) sizeof(COORDONNEES)); */
						(liste->deport)->piste = 0; /* deport uniquement pour differencier du layer */

						ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_deportg),
						    face);
					} else if (position > ((lecoeur.coord).yabs + lecoeur.height)) {
						if (mode_debug) 
							printf("Deport connecteur %s %ld\n", liste->nom_con, position);
						alloue_coord(0L, 0, &(liste->deport));
						/* liste->deport = (PT_COORDONNEES) mbkalloc ((unsigned int) sizeof(COORDONNEES)); */
						(liste->deport)->piste = 0; /* deport uniquement pour differencier du layer */

						ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_deportd),
						    face);
					}  else if (moins_dun_pitch_confacecoeur(position, liste, liste_coeur, lst_equipo,
					     face)) {
						if (mode_debug) 
							printf("Deport connecteur %s %ld\n", liste->nom_con, position);
						alloue_coord(0L, 0, &(liste->deport));
						/* liste->deport = (PT_COORDONNEES) mbkalloc ((unsigned int) sizeof(COORDONNEES)); */
						(liste->deport)->piste = 0; /* deport uniquement pour differencier du layer */
					} else
					 {
						if (mode_debug) 
							printf("Nouveau pas connecteur %s %ld\n", liste->nom_con, position);

						ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_pas), face);
					}

				} /* fin du if */ else { 

					/* ------------------------------------------------------------------------- */
					/* Il s'agit d'un plot d'alimentation, c'est un pas obligatoire de la grille */
					/* si ringerreur ici => ringerreur dans le placement de la barre de plot! */
					/* ------------------------------------------------------------------------- */

					if (position < (lecoeur.coord).yabs) 
						ringerreur(ERR_BARREALIM, (void * ) & face, NULL);
					if (position > ((lecoeur.coord).yabs + lecoeur.height)) 
						ringerreur(ERR_BARREALIM, (void * ) & face, NULL);

					moins_dun_pitch_confacecoeur(position, liste, liste_coeur, lst_equipo, face);
					if (mode_debug) 
						printf("Nouveau pas ALIM connecteur %s %ld\n", liste->nom_con, position);

					ajout_coordonnees_grille(liste->coord, &(tab_grilles[face].lst_pas), face);
				}

				liste = liste->suiv;
			} /* fin du while */

			break;
		}  /* Fin du switch() */

		/*------------- AJOUT DE PAS SUPPLEMENTAIRES SI DIST ENTRE PAS >= 2 PITCH --------*/

		ajout_pas_grille(tab_grilles, lecoeur, face);

	} /* fin du for */
}

/*------------------------------------------------------------------------------------*/
/* Ajout de nouveaux pas de grille si distance entre connecteur est suffisante        */
/* Rq: la dmin a1 a1 est de 3 cad 30 avec le SCALE_X actuel, pour faire les nouveaux  */
/* pas de grille, on se sert de dmin/2, mais les pas doivent etre multiples de        */
/* scale_x, donc pour les calculs des nouveaux pas on prend pour a1 ou a2             */
/* ((pos + scale_x) /scale_x) *scale_x                                                */
/* ceci est utilise quand connecteurs de largeur non standard sinon pas de pb         */
/*                                                                                    */
/* bug corrige au niveau des calculs de pas (+lamda / lambda * lambda)                */
/* 13 sept 92: autre bug corrige.                                                     */
/* Les connecteurs d'alim peuvent etre en vis a vis et se recouvrir, ce qui           */
/* faisait creer des nouveaux pas dans les alim !                                     */
/* Les fonctions existe_con_prec 	et existe_con_suiv trouve le                  */
/* connecteur le plus proche de celui qu'on cherche, donc une alim                    */
/* recouvrant si elle existe, et repousse les nouveaux pas dans une                   */
/* zone de routage vraiment libre.                                                    */
/*                                                                                    */
/* Attention: Le reglage pour les pas de grille est tres precis!                      */ 
/* si modif, verifier a la main qu'on respecte les distances ds ts les cas !          */
/*------------------------------------------------------------------------------------*/

void ajout_pas_grille(GRILLE tab_grilles[NB_FACES], COEUR lecoeur, int face)
{
	PT_COORDONNEES coordalim, nouveau, c1, c2, liste_coor;
	long	np = 0, enleve = 0, coordcon = 0, coordconprec = 0, largcon = 0, largconprec = 0;
	long	debut1 = 0, fin1 = 0, dist = 0, largeurmin = 0, dminmetalmetal = 0;
	char	layercon, layerconprec;

	if (mode_debug) 
		printf("FACE a completer %d\n", face);

	liste_coor = tab_grilles[face].lst_pas;

	/* --------------*/
	/* pour les vias */
	/* --------------*/

	if (ymetal_dmin > xmetal_dmin) 
		dminmetalmetal = ymetal_dmin;
	else 
		dminmetalmetal = xmetal_dmin;

	/*------------------ DEBUT DE GRILLE ----------------------------------------*/
	if (liste_coor != NULL)
		switch (face) {
		case NORD :
		case SUD  : 
			if (mode_debug) 
				printf("1er coordonnee %ld\n", liste_coor->xabs);
			largeurmin = ymetal_wvia;

			/* ----------------------------------------------------------- */
			/* on recherche si alim suivante recouvrant connecteur  existe */
			/* ----------------------------------------------------------- */

			liste_coor = existe_con_suiv(0L, 0L, ymetal_width, ymetal, face, tab_grilles[face].lst_pas);
			if (NULL == liste_coor) 
				ringerreur(ERR_GRILLEINTERNE, NULL, NULL);

			if (mode_debug) 
				printf("1ER CONNECTEUR x %ld y %ld face %d\n\n", liste_coor->xabs, liste_coor->yabs, face);

			if (((LST_PSEUDO_CON)liste_coor->proprio)->largeur > largeurmin) {
				enleve = ((LST_PSEUDO_CON)liste_coor->proprio)->largeur / 2 + dminmetalmetal + taille_via /
				    2;
				if (((LST_PSEUDO_CON)liste_coor->proprio)->largeur < taille_via)
					enleve += ( taille_via / 2 - ((LST_PSEUDO_CON)liste_coor->proprio)->largeur / 2);

				enleve = ((enleve + lambda) / lambda) * lambda;
			} else 
				enleve = pitch;

			for (np = (lecoeur.coord).xabs + pitch; np <= (liste_coor->xabs - enleve); np = np + pitch) {

				/* ----------------------------------------- */
				/* ce for calcule les nouveaux pas de grille */
				/* ----------------------------------------- */

				alloue_coord(np, face, &nouveau);
				ajout_coordonnees_grille(nouveau, &(tab_grilles[face].lst_pas), face);
			}
			break;
		case EST  :
		case OUEST:
			if (mode_debug) 
				printf("1er coordonnee %ld\n", liste_coor->yabs);
			largeurmin = xmetal_wvia;

			/* ----------------------------------------------------------- */
			/* on recherche si alim suivante recouvrant connecteur  existe */
			/* ----------------------------------------------------------- */

			liste_coor = existe_con_suiv(0L, 0L, xmetal_width, xmetal, face, tab_grilles[face].lst_pas);
			if (NULL == liste_coor) 
				ringerreur(ERR_GRILLEINTERNE, NULL, NULL);

			if (mode_debug) 
				printf("1ER CONNECTEUR x %ld y %ld face %d\n\n", liste_coor->xabs, liste_coor->yabs, face);

			if (((LST_PSEUDO_CON)liste_coor->proprio)->largeur > largeurmin) {
				enleve = ((LST_PSEUDO_CON)liste_coor->proprio)->largeur / 2 + dminmetalmetal + taille_via /
				    2;
				if (((LST_PSEUDO_CON)liste_coor->proprio)->largeur < taille_via)
					enleve += ( taille_via / 2 - ((LST_PSEUDO_CON)liste_coor->proprio)->largeur / 2);

				enleve = ((enleve + lambda) / lambda) * lambda;
			} else 
				enleve = pitch;

			for (np = (lecoeur.coord).yabs + pitch; np <= (liste_coor->yabs - enleve); np = np + pitch) {

				/* ----------------------------------------- */
				/* ce for calcule les nouveaux pas de grille */
				/* ----------------------------------------- */

				alloue_coord(np, face, &nouveau);
				ajout_coordonnees_grille(nouveau, &(tab_grilles[face].lst_pas), face);
			}
			break;
		}

	/*------------------------- GRILLE COURANTE --------------------------------*/

	if (mode_debug) 
		printf("Grille courante\n\n");

	while ((liste_coor != NULL) && (liste_coor->suiv != NULL)) {
		c1 = liste_coor; 
		c2 = liste_coor->suiv;

		switch (face) {
		case NORD:
		case SUD :
			if (mode_debug) 
				printf("grille courante 1er %ld\n", liste_coor->xabs);
			coordcon = c2->xabs;
			coordconprec = c1->xabs;
			break;
		case EST  :
		case OUEST:
			coordcon = c2->yabs;
			coordconprec = c1->yabs;
			break;
		}

		largcon      = ((LST_PSEUDO_CON)c2->proprio)->largeur;
		layercon      = ((LST_PSEUDO_CON)c2->proprio)->layer;
		largconprec  = ((LST_PSEUDO_CON)c1->proprio)->largeur;
		layerconprec  = ((LST_PSEUDO_CON)c1->proprio)->layer;

		dist = distance_con_con(coordcon, coordcon, largcon, layercon, coordconprec, coordconprec, largconprec, layerconprec,
		     face);

		/* ---------------------------------------------------------------------- */
		/* on recherche si alim precedente recouvrant connecteur precedent existe */
		/* ---------------------------------------------------------------------- */

		coordalim = existe_con_precalim(coordcon, coordcon, largcon, layercon, face, tab_grilles[face].lst_pas, dist);

		if (NULL != coordalim) {
			if (mode_debug) 
				printf("RECOUVRE TROUVE x %ld y %ld coordcon x %ld y %ld face %d DIST= %ld\n\n", coordalim->xabs,
				     coordalim->yabs, coordcon, coordcon, face, dist);
			if ((NORD == face) || (SUD == face)) 
				coordconprec = coordalim->xabs;
			else 
				coordconprec = coordalim->yabs;
			largconprec  = ((LST_PSEUDO_CON)coordalim->proprio)->largeur;
			layerconprec  = ((LST_PSEUDO_CON)coordalim->proprio)->layer;
		}

		dist = distance_con_con(coordcon, coordcon, largcon, layercon, coordconprec, coordconprec, largconprec, layerconprec,
		     face);

		if ((largcon > largeurmin) || (largconprec > largeurmin)) {
			if (mode_debug) 
				printf("largeur non standard\n %ld %ld", largconprec, largcon);

			if (largconprec <= largeurmin) 
				debut1 = pitch;
			else
			 { 
				debut1 = largconprec / 2 + dminmetalmetal + taille_via / 2;
				if (largconprec < taille_via) 
					debut1 += (taille_via / 2 - largconprec / 2);
				debut1 = ((debut1 + lambda) / lambda) * lambda;
			}

			if (largcon <= largeurmin) 
				fin1 = pitch;
			else
			 {
				fin1 = largcon / 2 + dminmetalmetal + taille_via / 2;
				if (largcon < taille_via) 
					fin1 += (taille_via / 2 - largcon / 2);

				fin1 = ((fin1 + lambda) / lambda) * lambda;

			}

			if (dist >= (2 * dminmetalmetal + largeurmin))
				for (np = coordconprec + debut1; np <=  (coordcon - fin1); np = np + pitch) {

					/* ------------------------------------------------- */
					/* ce for costaud calcule les nouveaux pas de grille */
					/* ------------------------------------------------- */

					alloue_coord(np, face, &nouveau);
					ajout_coordonnees_grille(nouveau, &(tab_grilles[face].lst_pas), face);
				}
		} else
		 {
			if (dist >= (2 * pitch))
				for (np = coordconprec + pitch; np <= (coordcon - pitch); np = np + pitch) {
					alloue_coord(np, face, &nouveau);
					ajout_coordonnees_grille(nouveau, &(tab_grilles[face].lst_pas), face);
				}
		}

		/* ----------------------------------------------------------------------------------------------- */
		/* Si on a inserer de nouveaux elements il faut pas les traiter donc on prend c2 comme pas suivant */
		/* ----------------------------------------------------------------------------------------------- */

		liste_coor = c2;
	} /*fin du while */

	if (liste_coor != NULL) {

		/*------------------------------- FIN DE GRILLE ----------------------------*/

		switch (face) {
		case NORD :
		case SUD  :
			/* ----------------------------------------------------------- */
			/* on recherche si alim suivante recouvrant connecteur  existe */
			/* ----------------------------------------------------------- */

			liste_coor = existe_con_suiv(lecoeur.width + lecoeur.coord.xabs, lecoeur.height + lecoeur.coord.yabs,
			     			 ymetal_width, ymetal, face, tab_grilles[face].lst_pas);
			if (NULL == liste_coor) 
				ringerreur(ERR_GRILLEINTERNE, NULL, NULL);

			if (mode_debug) 
				printf("Fin de grille dernier %ld\n", liste_coor->xabs);

			if (((LST_PSEUDO_CON)liste_coor->proprio)->largeur > largeurmin) {
				enleve = ((LST_PSEUDO_CON)liste_coor->proprio)->largeur / 2 + dminmetalmetal + taille_via /
				    2;
				if (((LST_PSEUDO_CON)liste_coor->proprio)->largeur < taille_via)
					enleve += ( taille_via / 2 - ((LST_PSEUDO_CON)liste_coor->proprio)->largeur / 2);

				enleve = ((enleve + lambda) / lambda) * lambda;
			} else 
				enleve = pitch;

			for (np = liste_coor->xabs + enleve; np <= ((lecoeur.coord).xabs + lecoeur.width - pitch); np = np + pitch) {
				/* ------------------------------------------------ */
				/* ce for calcule les nouveaux pas en fin de grille */
				/* ------------------------------------------------ */

				alloue_coord(np, face, &nouveau);
				ajout_coordonnees_grille(nouveau, &(tab_grilles[face].lst_pas), face);
			}
			break;
		case EST  :
		case OUEST:

			/* ----------------------------------------------------------- */
			/* on recherche si alim suivante recouvrant connecteur  existe */
			/* ----------------------------------------------------------- */

			liste_coor = existe_con_suiv(lecoeur.width + lecoeur.coord.xabs, lecoeur.height + lecoeur.coord.yabs,
			     			 xmetal_width, xmetal, face, tab_grilles[face].lst_pas);
			if (NULL == liste_coor) 
				ringerreur(ERR_GRILLEINTERNE, NULL, NULL);

			if (mode_debug) 
				printf("Fin de grille dernier %ld\n", liste_coor->yabs);

			if (((LST_PSEUDO_CON)liste_coor->proprio)->largeur > largeurmin) {
				enleve = ((LST_PSEUDO_CON)liste_coor->proprio)->largeur / 2 + dminmetalmetal + taille_via /
				    2;
				if (((LST_PSEUDO_CON)liste_coor->proprio)->largeur < taille_via)
					enleve += ( taille_via / 2 - ((LST_PSEUDO_CON)liste_coor->proprio)->largeur / 2);

				enleve = ((enleve + lambda) / lambda) * lambda;
			} else 
				enleve = pitch;

			for (np = liste_coor->yabs + enleve; np <= ((lecoeur.coord).yabs + lecoeur.height - pitch); np = np + pitch) {
				/* ------------------------------------------------ */
				/* ce for calcule les nouveaux pas en fin de grille */
				/* ------------------------------------------------ */

				alloue_coord(np, face, &nouveau);
				ajout_coordonnees_grille(nouveau, &(tab_grilles[face].lst_pas), face);
			}
			break;
		}

	} /* fin du if != NULL */

}

/*----------------------------------------------------------------------------*/
/* Cette procedure relie les plots eloignes par fabrique_barre avec la liste  */
/* lst_conestouest, cad relie les alim internes faces ouest et est            */
/*----------------------------------------------------------------------------*/

void relier_plots_wire1(phins_list *lastinst, phins_list *lastinst2, chain_list *lst)
{
	phcon_list * con1, *con2;

	while (NULL != lst) {
		con2 = (phcon_list * ) lst->DATA;
		con1 = (phcon_list * ) lst->NEXT->DATA;

		poser_wire1(con1->LAYER, con1->WIDTH / SCALE_X, lastinst->INSNAME, con1->NAME, con1->INDEX, lastinst2->INSNAME,
		    con2->NAME, con2->INDEX);

		lst = lst->NEXT->NEXT;
	}

}
