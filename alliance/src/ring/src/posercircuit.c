/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ring router                                                 */
/*    Fichier : posercircuit.c                                              */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Olivier Van Hautte                  le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

/*-----------------------------------------------------------------------------------*/
/* RING  3 juillet 92                   POSERCIRCUIT.C                               */
/* poser des blocs coeur et barre de plots + segments pour fabriquer fig MBK         */
/*-----------------------------------------------------------------------------------*/

#include "posercircuit.h"
#include "bigvia.h"
#include "lireplace.h"
#include "compress.h"
#include "barre.h"
#include "stat.h"
#include "sesame.h"

extern long	bigvias, ringvias;

/*-----------------------------------------------------------------------------------*/
/* placement des  blocs barre de plots + coeur , avec les rotations necessaires.     */
/* pour face: nord -> sans rotation                                                  */
/*            sud  -> SYM_Y                                                          */
/*            est  -> SY_RP                                                          */
/*           ouest -> ROT_P                                                          */
/* warning: les coord pour genlib doivent etre / par SCALE_X                         */
/*-----------------------------------------------------------------------------------*/

void poser_blocs(phfig_list **circuit_ph, char *nomcircuit,
	BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur)
{
	long	val;
	int	face;

	definir_fig_ph(nomcircuit);

	/* ----------------------------- */
	/* placement des barres de plots */
	/* ----------------------------- */

	for (face = 0; face < NB_FACES; face++) {
		if (mode_debug) 
			printf("face %d largeur %ld hauteur %ld x %ld y %ld piste %ld width coeur %ld height coeur %ld\n",
			     	        face, tab_plots[face].width, tab_plots[face].height, tab_plots[face].coord.xabs, tab_plots[face].coord.yabs,
			     tab_plots[face].coord.piste, lecoeur.width, lecoeur.height);

		if (0 != tab_plots[face].width) {
			switch (face) {
			case NORD:
				val = (tab_plots[face].coord.piste * pitch + lecoeur.coord.yabs + lecoeur.height) / SCALE_X;
				placer_instph(tab_plots[face].nomfig, tab_plots[face].nominst, NOSYM, (tab_plots[face].coord.xabs
				    / SCALE_X), val);
				break;

			case SUD:
				val = (tab_plots[face].coord.piste * -pitch - tab_plots[face].height + lecoeur.coord.yabs) /
				    SCALE_X;
				placer_instph(tab_plots[face].nomfig, tab_plots[face].nominst, SYM_Y, (tab_plots[face].coord.xabs
				    / SCALE_X), val);
				break;

			case EST:
				val = (tab_plots[face].coord.piste * pitch + lecoeur.coord.xabs + lecoeur.width) / SCALE_X;
				placer_instph(tab_plots[face].nomfig, tab_plots[face].nominst, SY_RP, val, (tab_plots[face].coord.yabs
				    / SCALE_X));
				break;

			case OUEST:
				val = (tab_plots[face].coord.piste * -pitch - tab_plots[face].height + lecoeur.coord.xabs) /
				    SCALE_X;
				placer_instph(tab_plots[face].nomfig, tab_plots[face].nominst, ROT_P, val, (tab_plots[face].coord.yabs
				    / SCALE_X));
				break;
			}
		}
	}

	/* ---------------------------------- */
	/* placement du coeur (sans rotation) */
	/* ---------------------------------- */

	placer_instph((lecoeur.coeur_ph)->NAME, (lecoeur.coeur_lo)->INSNAME, NOSYM, (lecoeur.coord.xabs / SCALE_X),  (lecoeur.coord.yabs / SCALE_X));

	/* ---------------------------- */
	/* Initialisation de circuit_ph */
	/* ---------------------------- */

	(*circuit_ph) = getphfig(nomcircuit, 'A');

	/* sauver_fig_ph();  */
}

/*----------------------------------------------------------------------------------*/
/* Cette procedure pour chaque equipotentielle lance la pose des segments mbk       */
/* et des vias associes.                                                            */
/*----------------------------------------------------------------------------------*/

void poser_segments(LST_EQUIPO lst_equipo, COEUR lecoeur,
	lofig_list *circuit_lo, phfig_list *circuit_ph)
{
	long	ringviasanc = 0, bigviasanc = 0;

	while (NULL != lst_equipo) {
		if (mode_debug) 
			printf("Pose des segments MBK des equipotentielles %ld \n", lst_equipo->index);
		poser_segments_eq(lst_equipo, lecoeur, circuit_lo);

		ringviasanc = ringvias; 
		bigviasanc = bigvias;
		poser_vias_eq(lst_equipo, lecoeur, circuit_ph);
		if (mode_stat)
			maj_stat_vias(lst_equipo, ringviasanc, bigviasanc, ringvias, bigvias);
		lst_equipo = lst_equipo->suiv;
	}

	if (mode_debug) 
		printf("***** VIAS TOTAL %ld VIAS COURONNE %ld ****\n\n", bigvias, ringvias);

	/* sauver_fig_ph();  */
}

/*---------------------------------------------------------------------------------*/
/* pose de tous les vrais segments de l'equipo courante                            */
/*---------------------------------------------------------------------------------*/

void poser_segments_eq(LST_EQUIPO equipo, COEUR lecoeur, lofig_list *circuit_lo)
{
	long	x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	LST_SEGMENT precseg;
	chain_list * lst_seg = equipo->lst_seg;
	char	*nameseg;

	nameseg = getsigname(getlosig(circuit_lo, equipo->index));

	/* --------------------------------- */
	/* Parcours des segments de l`equipo */
	/* --------------------------------- */

	while (NULL != lst_seg) {
		/* ----------------------------------------------------------- */
		/* calcul des coordonnees du seg sachant que (x1,y1) < (x2,y2) */
		/* ----------------------------------------------------------- */

		precseg = (LST_SEGMENT) lst_seg->DATA;

		/*if (mode_debug) printf("pointeur ptprecseg %d face %d\n",(int) precseg,(int)precseg->face); */

		if (VRAI_SEG == precseg->vraifaux) {
			switch (precseg->face) {
			case NORD:
				x1 = precseg->c1->xabs / SCALE_X; 
				x2 = precseg->c2->xabs / SCALE_X;
				y1 = (precseg->piste1 * pitch + lecoeur.coord.yabs + lecoeur.height) / SCALE_X;
				y2 = (precseg->piste2 * pitch + lecoeur.coord.yabs + lecoeur.height) / SCALE_X;
				break;

			case SUD:
				x1 = precseg->c1->xabs / SCALE_X; 
				x2 = precseg->c2->xabs / SCALE_X;
				y1 = (precseg->piste1 * -pitch + lecoeur.coord.yabs ) / SCALE_X;
				y2 = (precseg->piste2 * -pitch + lecoeur.coord.yabs ) / SCALE_X;
				break;

			case EST:
				y1 = precseg->c1->yabs / SCALE_X; 
				y2 = precseg->c2->yabs / SCALE_X;
				x1 = (precseg->piste1 * pitch + lecoeur.coord.xabs + lecoeur.width) / SCALE_X;
				x2 = (precseg->piste2 * pitch + lecoeur.coord.xabs + lecoeur.width) / SCALE_X;
				break;

			case OUEST:
				y1 = precseg->c1->yabs / SCALE_X; 
				y2 = precseg->c2->yabs / SCALE_X;
				x1 = (precseg->piste1 * -pitch + lecoeur.coord.xabs ) / SCALE_X;
				x2 = (precseg->piste2 * -pitch + lecoeur.coord.xabs ) / SCALE_X;
				break;
			}

			placer_segph(precseg->layer, (precseg->largeur / SCALE_X), nameseg, x1, y1, x2, y2);

			/* ----------------------------------------- */
			/* mode stat on remplit la stucture adequate */
			/* ----------------------------------------- */

			if (mode_stat) 
				remplit_stat(precseg, equipo);

		} /* fin du if vrai seg */

		lst_seg = lst_seg->NEXT;
	}

}

/*----------------------------------------------------------------------------------------------------*/
/* Cette procedure pose les vias pour les segments d'une equipo                                       */
/* Elle recherche les intersections de segments de layer different pour poser le vias de 2 manieres:  */
/* -- recherche par direction perpendiculaire au seg courant dans la meme face                        */
/* -- recherche de coordonnees identiques (coins de couronnes qui n'ont pas de proprietaire)          */
/* Modif: si on traite les eq alim, on parcours les segments avec l'attribut sans_via                 */
/*        car si le con coeur a un deport layer, il ne faut pas oublier de poser un via.              */
/* (ce trt special vient du fait que l'attribut via ne determine pas lequel des 2 points              */
/* du segment subit cet attribut)                                                                     */
/*----------------------------------------------------------------------------------------------------*/

void poser_vias_eq(LST_EQUIPO equipo, COEUR lecoeur, phfig_list *circuit_ph)
{
	long	x1 = 0, y1 = 0, x2 = 0, y2 = 0, minvia = 0;
	long	x1seg2 = 0, y1seg2 = 0, x2seg2 = 0, y2seg2 = 0;
	LST_SEGMENT seg, seg2;
	chain_list * liste, *lst_seg = equipo->lst_seg;
	int	viapose;
	LST_PSEUDO_CON con = (LST_PSEUDO_CON) (equipo->lst_con)->DATA;

	if ((con->nom_con == eq_vdd) || (con->nom_con == eq_vss))

		/* --------------------------------------------------------------------------------------- */
		/* parcours des segments sans via pour traiter les cas de changement de layer des concoeur */
		/* --------------------------------------------------------------------------------------- */

		poser_vias_visavisalim(equipo, lecoeur, circuit_ph);

	while (NULL != lst_seg) {
		seg = (LST_SEGMENT) lst_seg->DATA;

		/* -------------------------- */
		/* parcours des faux segments */
		/* -------------------------- */

		while ((NULL != lst_seg->NEXT) && (FAUX_SEG == seg->vraifaux)) {
			lst_seg = lst_seg->NEXT;
			seg = (LST_SEGMENT) lst_seg->DATA;
		}

		if (mode_debug) 
			printf("seg pt %d layer %d ptc1 %d ptc2 %d piste1 %ld piste2 %ld\n", (int) seg, (int)seg->layer,
			    (int)seg->c1, (int)seg->c2, seg->piste1, seg->piste2);

		if (FAUX_SEG == seg->vraifaux) 
			break;

		/* ----------------------------------------------------------- */
		/* calcul des coordonnees du seg sachant que (x1,y1) < (x2,y2) */
		/* ----------------------------------------------------------- */

		switch (seg->face) {
		case NORD:
			x1 = seg->c1->xabs; 
			x2 = seg->c2->xabs;
			y1 = (seg->piste1 * pitch + lecoeur.coord.yabs + lecoeur.height) ;
			y2 = (seg->piste2 * pitch + lecoeur.coord.yabs + lecoeur.height);
			break;

		case SUD:
			x1 = seg->c1->xabs; 
			x2 = seg->c2->xabs;
			y1 = (seg->piste1 * -pitch + lecoeur.coord.yabs );
			y2 = (seg->piste2 * -pitch + lecoeur.coord.yabs ) ;
			break;

		case EST:
			y1 = seg->c1->yabs; 
			y2 = seg->c2->yabs;
			x1 = (seg->piste1 * pitch + lecoeur.coord.xabs + lecoeur.width);
			x2 = (seg->piste2 * pitch + lecoeur.coord.xabs + lecoeur.width);
			break;

		case OUEST:
			y1 = seg->c1->yabs; 
			y2 = seg->c2->yabs;
			x1 = (seg->piste1 * -pitch + lecoeur.coord.xabs );
			x2 = (seg->piste2 * -pitch + lecoeur.coord.xabs );
			break;
		}

		if (mode_debug) 
			printf("seg pt %d layer %d x1 %ld y1 %ld x2 %ld y2 %ld\n", (int) seg, (int)seg->layer, x1, y1, x2,
			     y2);
		liste = lst_seg;

		/* ----------------------------------------------------------- */
		/* parcours des autres segments par rapport au segment courant */
		/* ----------------------------------------------------------- */

		while ((NULL != liste) && (AVEC_VIA == seg->via)) {
			seg2 =  (LST_SEGMENT) liste->DATA;
			viapose = 0;

			if ((VRAI_SEG == seg2->vraifaux) && (AVEC_VIA == seg2->via)) {

				/* --------------------------------------------------------------------------- */
				/* calcul des coordonnees du seg sachant que (x1seg2,y1seg2) < (x2seg2,y2seg2) */
				/* --------------------------------------------------------------------------- */

				switch (seg2->face) {
				case NORD:
					x1seg2 = seg2->c1->xabs; 
					x2seg2 = seg2->c2->xabs;
					y1seg2 = (seg2->piste1 * pitch + lecoeur.coord.yabs + lecoeur.height);
					y2seg2 = (seg2->piste2 * pitch + lecoeur.coord.yabs + lecoeur.height);
					break;

				case SUD:
					x1seg2 = seg2->c1->xabs; 
					x2seg2 = seg2->c2->xabs;
					y1seg2 = (seg2->piste1 * -pitch + lecoeur.coord.yabs );
					y2seg2 = (seg2->piste2 * -pitch + lecoeur.coord.yabs );
					break;

				case EST:
					y1seg2 = seg2->c1->yabs; 
					y2seg2 = seg2->c2->yabs;
					x1seg2 = (seg2->piste1 * pitch + lecoeur.coord.xabs + lecoeur.width);
					x2seg2 = (seg2->piste2 * pitch + lecoeur.coord.xabs + lecoeur.width);
					break;

				case OUEST:
					y1seg2 = seg2->c1->yabs; 
					y2seg2 = seg2->c2->yabs;
					x1seg2 = (seg2->piste1 * -pitch + lecoeur.coord.xabs );
					x2seg2 = (seg2->piste2 * -pitch + lecoeur.coord.xabs );
					break;
				}

				if (mode_debug) 
					printf("*seg2 pt %d layer %d x1 %ld y1 %ld x2 %ld y2 %ld\n", (int) seg2, (int)seg2->layer,
					     x1seg2, y1seg2, x2seg2, y2seg2);

				/* ------------------------------*/
				/* si les 2 seg sont de layer <> */
				/* ------------------------------*/

				if ((seg2 != seg) && (seg2->layer != seg->layer)) {

					/* ---------------------------------------------------------------------------------- */
					/* Si le segment seg a une extremite qui est un coin de couronne (proprio == NULL)    */
					/* on regarde si on trouve pas son segment voisin seg2, et alors catapultage d`un via */
					/* Ancienne version  au niveau genlib x* y* a diviser par SCALE_X                     */
   					/* pose d'un via unique, pour debogage rapide                                         */
					/* ---------------------------------------------------------------------------------- */

					/* On regarde si seg2 touche seg */
					if ((x1 == x2) && ((x1seg2 == x1) || (x2seg2 == x1))) {
						if ((y1seg2 >= y1) && (y1seg2 <= y2)) {
							viapose = 1;
							bigvia_ring(seg->layer,seg2->layer,circuit_ph, x1, y1seg2, seg->largeur, seg2->largeur);
						} else if ((y2seg2 >= y1) && (y2seg2 <= y2)) {
							viapose = 1;
							bigvia_ring(seg->layer,seg2->layer,circuit_ph, x1, y2seg2, seg->largeur, seg2->largeur);
						}
					} else if ((y1 == y2) && ((y1seg2 == y1) || (y2seg2 == y1))) {
						if ((x1seg2 >= x1) && (x1seg2 <= x2)) {
							viapose = 1;
							bigvia_ring(seg->layer,seg2->layer,circuit_ph, x1seg2, y1, seg2->largeur, seg->largeur);
						} else if ((x2seg2 >= x1) && (x2seg2 <= x2)) {
							viapose = 1;
							bigvia_ring(seg->layer,seg2->layer,circuit_ph, x2seg2, y1, seg2->largeur, seg->largeur);
						}
					}
	
					if (!viapose) {
						if ((x1seg2 == x2seg2) && ((x1seg2 == x1) || (x1seg2 == x2))) {
							if ((y1 >= y1seg2) && (y1 <= y2seg2)) {
								viapose = 1;
								bigvia_ring(seg->layer,seg2->layer,circuit_ph, x1seg2, y1, seg2->largeur, seg->largeur);
							} else if ((y2 >= y1seg2) && (y2 <= y2seg2)) {
								viapose = 1;
								bigvia_ring(seg->layer,seg2->layer,circuit_ph, x1seg2, y2, seg2->largeur, seg->largeur);
							}
						} else if ((y1seg2 == y2seg2) && ((y1seg2 == y1) || (y1seg2 == y2))) {
							if ((x1 >= x1seg2) && (x1 <= x2seg2)) {
								viapose = 1;
								bigvia_ring(seg->layer,seg2->layer,circuit_ph, x1, y1seg2, seg->largeur, seg2->largeur);
							} else if ((x2 >= x1seg2) && (x2 <= x2seg2)) {
								viapose = 1;
								bigvia_ring(seg->layer,seg2->layer,circuit_ph, x2, y1seg2, seg->largeur, seg2->largeur);
							}
						}
					} /* fin du if !viapose */
	
					if (!viapose) {
						if (seg->largeur < seg2->largeur) 
							minvia = seg->largeur;
						else 
							minvia = seg2->largeur;
	
						if ((x1 == x1seg2) && (y1 == y1seg2)) {
							bigvia_ring(seg->layer,seg2->layer,circuit_ph, x1, y1, minvia, minvia);
						}
	
						if ((x1 == x2seg2) && (y1 == y2seg2)) {
							bigvia_ring(seg->layer,seg2->layer,circuit_ph, x1, y1, minvia, minvia);
						}
	
						if ((x2 == x1seg2) && (y2 == y1seg2)) {
							bigvia_ring(seg->layer,seg2->layer,circuit_ph, x2, y2, minvia, minvia);
						}
	
						if ((x2 == x2seg2) && (y2 == y2seg2)) {
							bigvia_ring(seg->layer,seg2->layer,circuit_ph, x2, y2, minvia, minvia);
						}
	
					}
	
				}

			} /* fin du if vrai seg */

			liste = liste->NEXT;
		}

		/* MARQUAGE DU SEGMENT TRAITE */
		seg->via = SANS_VIA;

		lst_seg = lst_seg->NEXT;
	} /* fin du while */
}

/*---------------------------------------------------------------------------------*/

void remonte_con_circuit(chain_list *nom_plot[NB_FACES],
	chain_list *liste_plotsph, BARRE_PLOTS tab_plots[NB_FACES],
	COEUR lecoeur, lofig_list *circuit_lo, phfig_list *circuit_ph,
	char	*nom_circuit_ph, chain_list  *lst_conestouest)
{

	int	face;

	/* ---------------------------------------------------------------------------- */
	/* rq: obligation de definir l'abutmentbox avant la mise a plat, car bug genlib */
	/* pour definir l'ab apres la mise a plat des barres de plots                   */
	/* definition de l'abument box du circuit                                       */
	/* ---------------------------------------------------------------------------- */

	definir_ab_fig();

	/* ------------------------------- */
	/* mise a plat des barres de plots */
	/* ------------------------------- */

	for (face = 0; face < NB_FACES; face++)
		if (0 != tab_plots[face].width) 
			miseaplat_instph(tab_plots[face].nominst, NO);

	tirer_coins(circuit_ph, tab_plots, lst_conestouest);

	/* -------------------------------------------------- */
	/* Si on veut mettre des wire a la place des plots... */
	/* miseaplat_plotsvide(nom_circuit_ph);               */
	/* -------------------------------------------------- */


	/* -------------------------------- */
	/* construction des equipo externes */
	/* -------------------------------- */

	traite_equipo_ext(nom_plot, liste_plotsph, tab_plots, lecoeur, circuit_lo, circuit_ph);

	sauver_fig_ph();
}

/*------------------------------------------------------------------------------------*/
/* cette procedure fabrique les eq externe con circuit avec con plots,                */
/* mais verifie ces eq de telle sorte les connecteurs de plots de ces eq              */
/* appartiennent aux faces nord des modeles physique du plot, sinon ringerreur dans   */
/* le fichier C compile par genlib                                                    */
/*------------------------------------------------------------------------------------*/

void traite_equipo_ext(chain_list *nom_plot[NB_FACES], chain_list *liste_plotsph,
	BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur,
	lofig_list *circuit_lo, phfig_list *circuit_ph)
{

	locon_list * con_circuit = circuit_lo->LOCON ;
	locon_list * con_plot;
	losig_list * sig_circuit;
	loins_list * plot;
	int	trouve, face;
	chain_list * liste_plots, *lsterr = NULL;

	if (mode_debug) 
		printf("Traitement des equipo externes\n");

	/* --------------------------- */
	/* boucle des locon du circuit */
	/* --------------------------- */

	while (con_circuit != NULL) {
		sig_circuit = con_circuit->SIG;  /* un signal unique par connecteur */
		if (mode_debug) 
			printf("pointeur Sigcircuit %d\n", (int) sig_circuit);
		if (mode_debug) 
			printf("*******NOM concircuit %s signal %ld*********\n", con_circuit->NAME, sig_circuit->INDEX);

		trouve = 0;

		/* ---------------------------- */
		/* Parcours des instances plots */
		/* ---------------------------- */

		for (face = 0; (face < NB_FACES); face++) {/*  for (face=0;((face< NB_FACES) && (!trouve));face++)  */

			liste_plots = nom_plot[face];

			while (liste_plots != NULL) {

				plot = (loins_list * ) liste_plots->DATA;

				/* -------------------------------------------------------------------------- */
				/* S'il s'agit d'un plot on regarde les signaux des connecteurs du plot pour  */
				/* trouver meme signal que con_circuit                                        */
				/* -------------------------------------------------------------------------- */

				if (mode_debug) 
					printf("PLOT  %s de la face %d\n", plot->INSNAME, face);

				if ((con_plot = existe_sig_plot(sig_circuit, plot, liste_plotsph)) != NULL) {
					if (existe_leconnecteur_faceplot(plot->FIGNAME, con_plot->NAME, NORD, liste_plotsph)) {
						trouve = 1;
						remonte_con(con_circuit, con_plot, liste_plotsph, circuit_ph);

						/* ----------------------------------------------------------------------------------------------- */
						/* Pour les vdd* et vss* possibilite de plusieurs connecteurs physiques de meme nom sur le circuit */
						/* donc pas de break;                                                                              */
						/* ----------------------------------------------------------------------------------------------- */
					} else if (existe_leconnecteur_faceplot(plot->FIGNAME, con_plot->NAME, SUD, liste_plotsph)){
						lsterr = addchain(lsterr, (void * )con_plot->NAME);
						lsterr = addchain(lsterr, (void * )plot->INSNAME);
						lsterr = addchain(lsterr, (void * )con_circuit->NAME);
						trouve = 1;
						break;
					}
				}

				liste_plots = liste_plots->NEXT;
			} /* fin parcours instances plots d'une face */

		} /* fin  de parcours de toutes les faces */

		if (!trouve) 
			ringerreur(ERR_CONEXT_ENLAIR, con_circuit->NAME, NULL);

		con_circuit = con_circuit->NEXT;

	} /* fin de parcours des con du circuit  */

	if (NULL != lsterr) 
		ringerreur(ERR_EQEXTERNE, (void * )lsterr, NULL);

}

/*-----------------------------------------------------------------------------------*/

void remonte_con(locon_list *con_circuit, locon_list *con_plot,
	chain_list *liste_plotsph, phfig_list *circuit_ph)
{
	phcon_list * conph;

	conph = cherche_conph_circuit(con_plot, liste_plotsph, circuit_ph);

	if (NULL != conph)
		remonter_uncon_instph(conph->INDEX, conph->NAME, ((loins_list * )con_plot->ROOT)->INSNAME, con_circuit->NAME);

}

/*-----------------------------------------------------------------------------*/
/* Cette procedure termine la couronne de plots en tirant les fils necessaires */
/* pour fabriquer les coins de fils. utilisation de lst_conestouest.           */
/* on tire en coin que si 2 barres de plots sont mitoyennes                    */
/*-----------------------------------------------------------------------------*/

void tirer_coins(phfig_list  *circuit_ph, BARRE_PLOTS tab_plots[NB_FACES],
	chain_list  *lst_conestouest)
{
	phfig_list * barre[NB_FACES], *fig, *fig2;
	phins_list * firstinst, *lastinst, *firstinst2, *lastinst2;
	phcon_list * con, *con1, *con2;
	chain_list * lst;
	int	face;
	long	xcon1, xcon2, ycon1, ycon2;

	for (face = 0; face < NB_FACES; face++)
		if (0 != tab_plots[face].width)
			barre[face] = getphfig(tab_plots[face].nomfig, 'A');

	if (0 != tab_plots[SUD].width) {
		if (0 != tab_plots[OUEST].width) {

			/* ------------------------------------------------------------- */
			/* recuperation des 1eres et der instances de plots des 2 barres */
			/* ------------------------------------------------------------- */

			if (mode_debug) 
				printf("coin sud-ouest\n");
			firstlastinst_barre(barre[SUD], &firstinst, &lastinst);
			firstlastinst_barre(barre[OUEST], &firstinst2, &lastinst2);

			fig = getphfig(firstinst->FIGNAME, 'A');
			fig2 = getphfig(firstinst2->FIGNAME, 'A');

			firstinst = getphins(circuit_ph, firstinst->INSNAME);
			firstinst2 = getphins(circuit_ph, firstinst2->INSNAME);
			lst = lst_conestouest;

			/* ------------------------------------------------------------ */
			/* Calcul des coordonnees des coins et pose des fils avec wire2 */
			/* ------------------------------------------------------------ */

			while (NULL != lst) {
				con = (phcon_list * ) lst->DATA;
				con1 = getphcon(fig, con->NAME, con->INDEX);
				con2 = getphcon(fig2, con->NAME, con->INDEX);

				xyflat(&xcon1, &ycon1, con1->XCON, con1->YCON, firstinst->XINS, firstinst->YINS, fig->XAB1,
				     			fig->YAB1, fig->XAB2, fig->YAB2, firstinst->TRANSF);

				xyflat(&xcon2, &ycon2, con2->XCON, con2->YCON, firstinst2->XINS, firstinst2->YINS, fig2->XAB1,
				     			fig2->YAB1, fig2->XAB2, fig2->YAB2, firstinst2->TRANSF);

				poser_wire2(con->LAYER, con->WIDTH / SCALE_X, firstinst->INSNAME, con->NAME, con->INDEX, firstinst2->INSNAME,
				     con->NAME, con->INDEX, xcon2 / SCALE_X, ycon1 / SCALE_X);

				/* ---------------------------------------- */
				/* coin alu2 ajoute wx = wy obligatoirement */
				/* ---------------------------------------- */

				ajout_coin(xcon2 / SCALE_X, ycon1 / SCALE_X, con->WIDTH / SCALE_X, con->WIDTH / SCALE_X, con->LAYER);

				lst = lst->NEXT->NEXT;
			}

		}

		if (0 != tab_plots[EST].width) {

			/* ------------------------------------------------------------- */
			/* recuperation des 1eres et der instances de plots des 2 barres */
			/* ------------------------------------------------------------- */

			if (mode_debug) 
				printf("coin sud-est\n");

			firstlastinst_barre(barre[SUD], &firstinst, &lastinst);
			firstlastinst_barre(barre[EST], &firstinst2, &lastinst2);

			fig = getphfig(lastinst->FIGNAME, 'A');
			fig2 = getphfig(firstinst2->FIGNAME, 'A');

			lastinst = getphins(circuit_ph, lastinst->INSNAME);
			firstinst2 = getphins(circuit_ph, firstinst2->INSNAME);
			lst = lst_conestouest;

			/* ------------------------------------------------------------- */
			/* Calcul des coordonnees des coins et pose des fils avec wire2  */
			/* ------------------------------------------------------------- */

			while (NULL != lst) {
				con1 = (phcon_list * ) lst->NEXT->DATA;
				con2 = (phcon_list * ) lst->DATA;
				con1 = getphcon(fig, con1->NAME, con1->INDEX);
				con2 = getphcon(fig2, con2->NAME, con2->INDEX);

				xyflat(&xcon1, &ycon1, con1->XCON, con1->YCON, lastinst->XINS, lastinst->YINS, fig->XAB1,
				     			fig->YAB1, fig->XAB2, fig->YAB2, lastinst->TRANSF);

				xyflat(&xcon2, &ycon2, con2->XCON, con2->YCON, firstinst2->XINS, firstinst2->YINS, fig2->XAB1,
				     			fig2->YAB1, fig2->XAB2, fig2->YAB2, firstinst2->TRANSF);

				poser_wire2(con1->LAYER, con1->WIDTH / SCALE_X, lastinst->INSNAME, con1->NAME, con1->INDEX, 
				     firstinst2->INSNAME, con2->NAME, con2->INDEX, xcon2 / SCALE_X, ycon1 / SCALE_X);

				/* ---------------------------------------- */
				/* coin alu2 ajoute wx = wy obligatoirement */
				/* ---------------------------------------- */

				ajout_coin(xcon2 / SCALE_X, ycon1 / SCALE_X, con1->WIDTH / SCALE_X, con1->WIDTH / SCALE_X, con1->LAYER);

				lst = lst->NEXT->NEXT;
			}
		}
	}

	if (0 != tab_plots[NORD].width) {
		if (0 != tab_plots[OUEST].width) {

			/* ------------------------------------------------------------- */
			/* recuperation des 1eres et der instances de plots des 2 barres */
			/* ------------------------------------------------------------- */

			if (mode_debug) 
				printf("coin nord-ouest\n");

			firstlastinst_barre(barre[NORD], &firstinst, &lastinst);
			firstlastinst_barre(barre[OUEST], &firstinst2, &lastinst2);

			fig = getphfig(firstinst->FIGNAME, 'A');
			fig2 = getphfig(lastinst2->FIGNAME, 'A');

			firstinst = getphins(circuit_ph, firstinst->INSNAME);
			lastinst2 = getphins(circuit_ph, lastinst2->INSNAME);
			lst = lst_conestouest;

			/* ------------------------------------------------------------- */
			/* Calcul des coordonnees des coins et pose des fils avec wire2  */
			/* ------------------------------------------------------------- */

			while (NULL != lst) {
				con1 = (phcon_list * ) lst->DATA;
				con2 = (phcon_list * ) lst->NEXT->DATA;
				con1 = getphcon(fig, con1->NAME, con1->INDEX);
				con2 = getphcon(fig2, con2->NAME, con2->INDEX);

				xyflat(&xcon1, &ycon1, con1->XCON, con1->YCON, firstinst->XINS, firstinst->YINS, fig->XAB1,
				     			fig->YAB1, fig->XAB2, fig->YAB2, firstinst->TRANSF);

				xyflat(&xcon2, &ycon2, con2->XCON, con2->YCON, lastinst2->XINS, lastinst2->YINS, fig2->XAB1,
				     			fig2->YAB1, fig2->XAB2, fig2->YAB2, lastinst2->TRANSF);

				poser_wire2(con1->LAYER, con1->WIDTH / SCALE_X, firstinst->INSNAME, con1->NAME, con1->INDEX, lastinst2->INSNAME,
				     con2->NAME, con2->INDEX, xcon2 / SCALE_X, ycon1 / SCALE_X);

				/* ---------------------------------------- */
				/* coin alu2 ajoute wx = wy obligatoirement */
				/* ---------------------------------------- */

				ajout_coin(xcon2 / SCALE_X, ycon1 / SCALE_X, con1->WIDTH / SCALE_X, con1->WIDTH / SCALE_X, con1->LAYER);

				lst = lst->NEXT->NEXT;
			}
		}

		if (0 != tab_plots[EST].width) {

			/* ------------------------------------------------------------- */
			/* recuperation des 1eres et der instances de plots des 2 barres */
			/* ------------------------------------------------------------- */

			if (mode_debug) 
				printf("coin nord-est\n");

			firstlastinst_barre(barre[NORD], &firstinst, &lastinst);
			firstlastinst_barre(barre[EST], &firstinst2, &lastinst2);

			fig = getphfig(lastinst->FIGNAME, 'A');
			fig2 = getphfig(lastinst2->FIGNAME, 'A');

			lastinst = getphins(circuit_ph, lastinst->INSNAME);
			lastinst2 = getphins(circuit_ph, lastinst2->INSNAME);
			lst = lst_conestouest;

			/* ------------------------------------------------------------- */
			/* Calcul des coordonnees des coins et pose des fils avec wire2  */
			/* ------------------------------------------------------------- */

			while (NULL != lst) {
				con = (phcon_list * ) lst->NEXT->DATA;
				con1 = getphcon(fig, con->NAME, con->INDEX);
				con2 = getphcon(fig2, con->NAME, con->INDEX);

				xyflat(&xcon1, &ycon1, con1->XCON, con1->YCON, lastinst->XINS, lastinst->YINS, fig->XAB1,
				     			fig->YAB1, fig->XAB2, fig->YAB2, lastinst->TRANSF);

				xyflat(&xcon2, &ycon2, con2->XCON, con2->YCON, lastinst2->XINS, lastinst2->YINS, fig2->XAB1,
				     			fig2->YAB1, fig2->XAB2, fig2->YAB2, lastinst2->TRANSF);

				poser_wire2(con->LAYER, con->WIDTH / SCALE_X, lastinst->INSNAME, con->NAME, con->INDEX, lastinst2->INSNAME,
				     con->NAME, con->INDEX, xcon2 / SCALE_X, ycon1 / SCALE_X);

				/* ---------------------------------------- */
				/* coin alu2 ajoute wx = wy obligatoirement */
				/* ---------------------------------------- */

				ajout_coin(xcon2 / SCALE_X, ycon1 / SCALE_X, con->WIDTH / SCALE_X, con->WIDTH / SCALE_X, con->LAYER);

				lst = lst->NEXT->NEXT;
			}
		}
	}

}

/*-------------------------------------------------------------------------------*/
/* Ajout de coin en alu2 car la fonction wire2 ne le fait pas                    */
/*-------------------------------------------------------------------------------*/

void ajout_coin(long x, long y, long wx, long wy, char layer)
{
	long	x1, x2, y1, y2, w;

	if ((wx > 2 * EXTENSION_ALU2) && (wy > 2 * EXTENSION_ALU2)) {
		if (0 == (wx % 2)) { /* fil vertical */
			y1 = y - wx / 2 + EXTENSION_ALU2;
			y2 = y + wx / 2 - EXTENSION_ALU2;
			w  = wy;
			x1 = x2 = x;
		}  else {
			x1 = x - wy / 2 + EXTENSION_ALU2;
			x2 = x + wy / 2 - EXTENSION_ALU2;
			w  = wx;
			y1 = y2 = y;
		}
		placer_segph(layer, w, genNAME("coin_%ld_%ld_%ld_%ld",x1, y1, x2, y2), x1, y1, x2, y2);
	}
}

/*-----------------------------------------------------------------------*/
/* cette procedure parcours les segments avec l'attribut SANS_VIA et     */
/* regarde s'il ne manque pas de via vers le con du coeur, cas ou le     */
/* con coeur a un deport layer                                           */
/*-----------------------------------------------------------------------*/

void poser_vias_visavisalim(LST_EQUIPO equipo, COEUR lecoeur,
	phfig_list *circuit_ph)
{
	long	x1 = 0, y1 = 0, x2 = 0, y2 = 0, minvia = 0;
	long	x1seg2 = 0, y1seg2 = 0, x2seg2 = 0, y2seg2 = 0;
	LST_SEGMENT seg, seg2;
	chain_list * liste, *lst_seg = equipo->lst_seg;

	if (mode_debug) 
		printf("poser_vias_viavisalim index %ld\n", equipo->index);

	while (NULL != lst_seg) {
		seg = (LST_SEGMENT) lst_seg->DATA;

		/* -------------------------- */
		/* parcours des faux segments */
		/* -------------------------- */

		while ((NULL != lst_seg->NEXT) && (FAUX_SEG == seg->vraifaux)) {
			lst_seg = lst_seg->NEXT;
			seg = (LST_SEGMENT) lst_seg->DATA;
		}

		if (mode_debug) 
			printf("seg pt %d layer %d ptc1 %d ptc2 %d piste1 %ld piste2 %ld\n", (int) seg, (int)seg->layer,
			    (int)seg->c1, (int)seg->c2, seg->piste1, seg->piste2);

		if (FAUX_SEG == seg->vraifaux) 
			break;

		/* ----------------------------------------------------------- */
		/* calcul des coordonnees du seg sachant que (x1,y1) < (x2,y2) */
		/* ON GARDE DS X1,Y1 LA COORDONNEE LA + PROCHE DU COEUR        */
		/* ----------------------------------------------------------- */

		switch (seg->face) {
		case NORD:
			x1 = seg->c1->xabs; 
			x2 = seg->c2->xabs;
			y1 = (seg->piste1 * pitch + lecoeur.coord.yabs + lecoeur.height) ;
			y2 = (seg->piste2 * pitch + lecoeur.coord.yabs + lecoeur.height);
			if (y2 < y1) { 
				y1 = y2; 
				x1 = x2;
			}
			break;

		case SUD:
			x1 = seg->c1->xabs; 
			x2 = seg->c2->xabs;
			y1 = (seg->piste1 * -pitch + lecoeur.coord.yabs );
			y2 = (seg->piste2 * -pitch + lecoeur.coord.yabs ) ;
			if (y2 > y1) { 
				y1 = y2; 
				x1 = x2;
			}
			break;

		case EST:
			y1 = seg->c1->yabs; 
			y2 = seg->c2->yabs;
			x1 = (seg->piste1 * pitch + lecoeur.coord.xabs + lecoeur.width);
			x2 = (seg->piste2 * pitch + lecoeur.coord.xabs + lecoeur.width);
			if (x2 < x1) { 
				y1 = y2; 
				x1 = x2;
			}
			break;

		case OUEST:
			y1 = seg->c1->yabs; 
			y2 = seg->c2->yabs;
			x1 = (seg->piste1 * -pitch + lecoeur.coord.xabs );
			x2 = (seg->piste2 * -pitch + lecoeur.coord.xabs );
			if (x2 > x1) { 
				y1 = y2; 
				x1 = x2;
			}
			break;
		}

		if (mode_debug) 
			printf("seg pt %d layer %d x1 %ld y1 %ld x2 %ld y2 %ld\n", (int) seg, (int)seg->layer, x1, y1, x2,
			     y2);
		liste = lst_seg;

		/* ----------------------------------------------------------- */
		/* parcours des autres segments par rapport au segment courant */
		/* ----------------------------------------------------------- */

		while ((NULL != liste) && (SANS_VIA == seg->via)) {
			seg2 =  (LST_SEGMENT) liste->DATA;

			if ((VRAI_SEG == seg2->vraifaux) && (AVEC_VIA == seg2->via)) {

				/* --------------------------------------------------------------------------- */
				/* calcul des coordonnees du seg sachant que (x1seg2,y1seg2) < (x2seg2,y2seg2) */
				/* --------------------------------------------------------------------------- */

				switch (seg2->face) {
				case NORD:
					x1seg2 = seg2->c1->xabs; 
					x2seg2 = seg2->c2->xabs;
					y1seg2 = (seg2->piste1 * pitch + lecoeur.coord.yabs + lecoeur.height);
					y2seg2 = (seg2->piste2 * pitch + lecoeur.coord.yabs + lecoeur.height);
					break;

				case SUD:
					x1seg2 = seg2->c1->xabs; 
					x2seg2 = seg2->c2->xabs;
					y1seg2 = (seg2->piste1 * -pitch + lecoeur.coord.yabs );
					y2seg2 = (seg2->piste2 * -pitch + lecoeur.coord.yabs );
					break;

				case EST:
					y1seg2 = seg2->c1->yabs; 
					y2seg2 = seg2->c2->yabs;
					x1seg2 = (seg2->piste1 * pitch + lecoeur.coord.xabs + lecoeur.width);
					x2seg2 = (seg2->piste2 * pitch + lecoeur.coord.xabs + lecoeur.width);
					break;

				case OUEST:
					y1seg2 = seg2->c1->yabs; 
					y2seg2 = seg2->c2->yabs;
					x1seg2 = (seg2->piste1 * -pitch + lecoeur.coord.xabs );
					x2seg2 = (seg2->piste2 * -pitch + lecoeur.coord.xabs );
					break;
				}

				if (mode_debug) 
					printf("*seg2 pt %d layer %d x1 %ld y1 %ld x2 %ld y2 %ld\n", (int) seg2, (int)seg2->layer,
					     x1seg2, y1seg2, x2seg2, y2seg2);

				/* ----------------------------- */
				/* si les 2 seg sont de layer <> */
				/* ----------------------------- */

				if ((seg2 != seg) && (seg2->layer != seg->layer)) {

					/* ---------------------------------------------------------------------------------- */
					/* Si le segment seg a une extremite qui est un coin de couronne (proprio == NULL)    */
					/* on regarde si on trouve pas son segment voisin seg2, et alors catapultage d`un via */
					/* ---------------------------------------------------------------------------------- */

					if (seg->largeur < seg2->largeur) 
						minvia = seg->largeur;
					else 
						minvia = seg2->largeur;
	
					if ((x1 == x1seg2) && (y1 == y1seg2)) {
						bigvia_ring(seg->layer, seg2->layer, circuit_ph, x1, y1, minvia, minvia);
					}
	
					if ((x1 == x2seg2) && (y1 == y2seg2)) {
						bigvia_ring(seg->layer, seg2->layer, circuit_ph, x1, y1, minvia, minvia);
					}
				} /* fin du if seg2 */
	
			} /* fin du if vrai seg */
	
			liste = liste->NEXT;
		}
	
		lst_seg = lst_seg->NEXT;
	} /* fin du while */

}
