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
/*    Fichier : routalim.c                                                  */
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
/* RING 21 juillet 92                 ROUTALIM.C                                     */
/* routage des alim : couronne + colonnes + deports eventuels                        */
/*-----------------------------------------------------------------------------------*/

#include "routalim.h"
#include "placement.h"
#include "sesame.h"

/*-------------------------------------------------------------------------------------*/
/* Cette joyeuse procedure pose les couronnes d'alimentation superieures et inferieures*/
/* et retourne les numeros des pistes d'alim dans 2 tableaux coursup[] et courinf[]    */
/*-------------------------------------------------------------------------------------*/

void pose_couralim(char *coursup, long piste_sup[NB_FACES], long piste_inf[NB_FACES],
	LST_EQUIPO lst_equipo, COEUR lecoeur, BARRE_PLOTS tab_plots[NB_FACES],
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES],
	long	tabpiste[NB_FACES])
{
	long	wtheovdd, wtheovss, wreelvdd_ymetal, wreelvdd_xmetal, wreelvss_xmetal;
	long	wreelvss_ymetal, maxlarg_y, maxlarg_x;
	long	nbpistemoinsvdd_ymetal, nbpistemoinsvdd_xmetal;
	long	nbpistemoinsvss_ymetal, nbpistemoinsvss_xmetal;
	long	nbpistemoinsalim_sup = 0, wreelalim_sup = 0, nbpistemoinsalim_inf  = 0, wreelalim_inf = 0, pistealim = 0;
	long	largmetalpiste = 0, pistefin = 0, pistefinpardefaut = 0;

	/* ------------------------------------------------ */
	/* stocker la derniere piste libre pour chaque face */
	/* ------------------------------------------------ */

	int	face;
	char	metalpiste = 0;

	LST_SEGMENT * segpiste = NULL;
	LST_EQUIPO equipo_vdd = NULL, equipo_vss = NULL, eqalim_sup = NULL, eqalim_inf = NULL;
	PT_COORDONNEES c1alim_sup = NULL, c2alim_sup = NULL, c1alim_inf = NULL, c2alim_inf = NULL;
	PT_COORDONNEES c1horalim_sup, c2horalim_sup, c1horalim_inf, c2horalim_inf;
	PT_COORDONNEES c1veralim_sup, c2veralim_sup, c1veralim_inf, c2veralim_inf;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

	largeur_vddvss(equipo_vdd, equipo_vss, &wtheovdd, &wtheovss);

	if (ymetal_width > ymetal_wvia) 
		maxlarg_y = ymetal_width;
	else 
		maxlarg_y = ymetal_wvia;

	if (xmetal_width > xmetal_wvia) 
		maxlarg_x = xmetal_width;
	else 
		maxlarg_x = xmetal_wvia;


	/* ---------------------------------------------------------- */
	/* ON se debrouille pour tomber sur un nombre de pistes paire */
	/* ---------------------------------------------------------- */

	wreelvdd_ymetal = ((wtheovdd + pitch) / (2 * pitch )) * (2 * pitch) + maxlarg_y;
	wreelvdd_xmetal = ((wtheovdd + pitch) / (2 * pitch )) * (2 * pitch) + maxlarg_x;

	wreelvss_ymetal = ((wtheovss + pitch) / (2 * pitch )) * (2 * pitch) + maxlarg_y;
	wreelvss_xmetal = ((wtheovss + pitch) / (2 * pitch )) * (2 * pitch) + maxlarg_x;

	if (mode_debug) 
		printf("VDD largeury %5ld largeurx %5ld\n", wreelvdd_ymetal, wreelvdd_xmetal);
	if (mode_debug) 
		printf("VSS largeury %5ld largeurx %5ld\n", wreelvss_ymetal, wreelvss_xmetal);

	if (mode_debug) 
		printf("VDDpitch largeury %5ld largeurx %5ld\n", wreelvdd_ymetal / pitch, wreelvdd_xmetal / pitch);
	if (mode_debug) 
		printf("VSSpitch largeury %5ld largeurx %5ld\n", wreelvss_ymetal / pitch, wreelvss_xmetal / pitch);

	if (mode_debug) 
		printf("COEUR width %ld height %ld\n", lecoeur.width, lecoeur.height);

	nbpistemoinsvdd_ymetal = ((wtheovdd + pitch) / (2 * pitch ));
	nbpistemoinsvdd_xmetal = ((wtheovdd + pitch) / (2 * pitch ));
	nbpistemoinsvss_ymetal = ((wtheovdd + pitch) / (2 * pitch ));
	nbpistemoinsvss_xmetal = ((wtheovdd + pitch) / (2 * pitch ));

	if (mode_debug) 
		printf("VDDretirepiste largeury %5ld largeurx %5ld\n", nbpistemoinsvdd_ymetal, nbpistemoinsvdd_xmetal);
	if (mode_debug) 
		printf("VSSretirepiste largeury %5ld largeurx %5ld\n", nbpistemoinsvss_ymetal, nbpistemoinsvss_xmetal);

	/*-------------------------- POSE DES COURONNES D'ALIM -------------------------------*/

	/* ---------------------------------------------------------- */
	/* recherche de la derniere piste libre vers la barre de plot */
	/* ---------------------------------------------------------- */

	alloue_coord(0L, 0, &c1horalim_sup); 
	alloue_coord(0L, 0, &c2horalim_sup);
	alloue_coord(0L, 0, &c1veralim_sup); 
	alloue_coord(0L, 0, &c2veralim_sup);
	alloue_coord(0L, 0, &c1horalim_inf); 
	alloue_coord(0L, 0, &c2horalim_inf);
	alloue_coord(0L, 0, &c1veralim_inf); 
	alloue_coord(0L, 0, &c2veralim_inf);

	/*-----------------------------------------------------------------------------------*/     
	/*    REPRESENTATION DES 2 COURONNES D ALIM SUPERIEURE ET INFERIEURE                 */     
	/*                                                                                   */
 	/*c1horalim_sup                    alim_sup                  c2horalim_sup           */
 	/*c2veralim_sup +-------------------------------------------+c2veralim_sup           */
        /*              |                                           |                        */
        /*              |                  alim_inf                 |                        */
        /*              |c1horalim_inf +--------------+c2horalim_inf|                        */
        /*              |c2veralim_inf |              |c2horalim_inf|                        */
        /*              |              |              |             |                        */
        /*              |              |              |             |                        */
        /*              |              |              |             |                        */
        /*              |              |              |             |                        */
        /*              |              |              |             |                        */
        /*              |c1horalim_inf |              |c2horalim_inf|                        */
        /*              |c1veralim_sup +--------------+c1horalim_inf|                        */
 	/*c1horalim_sup |                                           |c2horalim_sup           */
 	/*c1veralim_sup +-------------------------------------------+c1veralim_sup           */
	/*-----------------------------------------------------------------------------------*/ 

	for (face = 0 ; face < NB_FACES; face++) {
		if (0 != tab_plots[face].width) {
			pistefin = tab_plots[face].coord.piste - 2; /* On part de apres le deport */

			switch (face) {
			case NORD:
			case SUD :
				segpiste = segx_occ;
				c1horalim_sup->xabs = lecoeur.coord.xabs;
				c2horalim_sup->xabs = lecoeur.coord.xabs + lecoeur.width;
				metalpiste = xmetal;
				largmetalpiste = xmetal_width;
				break;

			case EST  :
			case OUEST:
				segpiste = segy_occ;
				c1horalim_sup->yabs = lecoeur.coord.yabs;
				c2horalim_sup->yabs = lecoeur.coord.yabs + lecoeur.height;
				metalpiste = ymetal;
				largmetalpiste = ymetal_width;
				break;
			}

			while ((0 != pistefin) && (!segment_libre(c1horalim_sup, c2horalim_sup, pistefin, pistefin, face,
			     SEG_PISTE, segpiste, metalpiste)))
				pistefin--;

			if (0 == pistefin) 
				ringerreur(ERR_NOPISTE, &face, NULL);
			tabpiste[face] = pistefinpardefaut = pistefin;
		}  else 
			tabpiste[face] = 0; /* pas de barre de plots, on ne sait pas */

	}

	for (face = 0; face < NB_FACES; face++) /* maintenant on sait, valeur par defaut  */
		if (0 == tabpiste[face]) 
			tabpiste[face] = pistefinpardefaut;

	/*----------------------- PLACEMENT DES COURONNES -------------------------------------*/

	for (face = 0 ; face < NB_FACES; face++) {
		switch (face) {
		case NORD:
		case SUD :
			segpiste = segx_occ;
			metalpiste = xmetal;
			largmetalpiste = xmetal_width;
			if (coursup == eq_vdd) {
				nbpistemoinsalim_sup = nbpistemoinsvdd_xmetal;
				wreelalim_sup = wreelvdd_xmetal;
				eqalim_sup    = equipo_vdd;
				nbpistemoinsalim_inf = nbpistemoinsvss_xmetal;
				wreelalim_inf = wreelvss_xmetal;
				eqalim_inf    = equipo_vss;
			} else
			 {
				nbpistemoinsalim_sup = nbpistemoinsvss_xmetal;
				wreelalim_sup = wreelvss_xmetal;
				eqalim_sup    = equipo_vss;
				nbpistemoinsalim_inf = nbpistemoinsvdd_xmetal;
				wreelalim_inf = wreelvdd_xmetal;
				eqalim_inf    = equipo_vdd;
			}
			c1veralim_sup->yabs = lecoeur.coord.yabs - ((tabpiste[SUD] - nbpistemoinsalim_sup) * pitch);  
			c1veralim_sup->xabs = 0;
			c2veralim_sup->yabs = lecoeur.coord.yabs + lecoeur.height + ((tabpiste[NORD] - nbpistemoinsalim_sup) *
			    pitch); 
			c2veralim_sup->xabs = 0;

			c1veralim_inf->yabs = c1veralim_sup->yabs + ((nbpistemoinsalim_sup + nbpistemoinsalim_inf + 2) *
			    pitch); 
			c1veralim_inf->xabs = 0;
			c2veralim_inf->yabs = c2veralim_sup->yabs - ((nbpistemoinsalim_sup + nbpistemoinsalim_inf + 2) *
			    pitch); 
			c2veralim_inf->xabs = 0;

			c1alim_sup = c1horalim_sup;
			c2alim_sup = c2horalim_sup;
			c1alim_inf = c1horalim_inf;
			c2alim_inf = c2horalim_inf;
			break;

		case EST  :
		case OUEST:
			segpiste = segy_occ;
			metalpiste = ymetal;
			largmetalpiste = ymetal_width;
			if (coursup == eq_vdd) {
				nbpistemoinsalim_sup = nbpistemoinsvdd_ymetal;
				wreelalim_sup = wreelvdd_ymetal;
				eqalim_sup    = equipo_vdd;
				nbpistemoinsalim_inf = nbpistemoinsvss_ymetal;
				wreelalim_inf = wreelvss_ymetal;
				eqalim_inf    = equipo_vss;
			} else
			 {
				nbpistemoinsalim_sup = nbpistemoinsvss_ymetal;
				wreelalim_sup = wreelvss_ymetal;
				eqalim_sup    = equipo_vss;
				nbpistemoinsalim_inf = nbpistemoinsvdd_ymetal;
				wreelalim_inf = wreelvdd_ymetal;
				eqalim_inf    = equipo_vdd;
			}
			c1horalim_sup->xabs = lecoeur.coord.xabs - ((tabpiste[OUEST] - nbpistemoinsalim_sup) * pitch); 
			c1horalim_sup->yabs = 0 ;
			c2horalim_sup->xabs = lecoeur.coord.xabs + lecoeur.width + ((tabpiste[EST] - nbpistemoinsalim_sup) *
			    pitch); 
			c2horalim_sup->yabs = 0;
			c1horalim_inf->xabs = c1horalim_sup->xabs + ((nbpistemoinsalim_sup + nbpistemoinsalim_inf + 2) *
			    pitch); 
			c1horalim_inf->yabs = 0;
			c2horalim_inf->xabs = c2horalim_sup->xabs - ((nbpistemoinsalim_sup + nbpistemoinsalim_inf + 2) *
			    pitch); 
			c2horalim_inf->yabs = 0;

			c1alim_sup = c1veralim_sup;
			c2alim_sup = c2veralim_sup;
			c1alim_inf = c1veralim_inf;
			c2alim_inf = c2veralim_inf;

			break;
		} /* fin du switch */

		pistefin  = tabpiste[face]; /* derniere piste libre */
		piste_sup[face] = pistealim = pistefin - nbpistemoinsalim_sup ;

		/*----------------------------- ALIM COURONNE SUPERIEURE ------------------------------*/

		/* ------------ */
		/* faux segment */
		/* ------------ */

		for (; pistefin > pistealim; pistefin--)
			alloue_etchaine_segment(c1alim_sup, c2alim_sup, pistefin, pistefin, face, SEG_PISTE, segpiste, largmetalpiste,
			     metalpiste, FAUX_SEG, AVEC_VIA, eqalim_sup);

		/* ------------ */
		/* vrai segment */
		/* ------------ */

		alloue_etchaine_segment(c1alim_sup, c2alim_sup, pistealim, pistealim, face, SEG_PISTE, segpiste, wreelalim_sup,
		     metalpiste, VRAI_SEG, AVEC_VIA, eqalim_sup);

		/* ------------ */
		/* faux segment */
		/* ------------ */

		for (pistefin = pistealim - 1; pistefin >= pistealim - nbpistemoinsalim_sup; pistefin--)
			alloue_etchaine_segment(c1alim_sup, c2alim_sup, pistefin, pistefin, face, SEG_PISTE, segpiste, largmetalpiste,
			     metalpiste, FAUX_SEG, AVEC_VIA, eqalim_sup);

		/*----------------------------- ALIM COURONNE INFERIEURE ------------------------------*/

		pistefin --;
		piste_inf[face] = 	pistealim = pistefin - nbpistemoinsalim_inf;

		/* ------------ */
		/* faux segment */
		/* ------------ */

		for (; pistefin > pistealim; pistefin--)
			alloue_etchaine_segment(c1alim_inf, c2alim_inf, pistefin, pistefin, face, SEG_PISTE, segpiste, largmetalpiste,
			     metalpiste, FAUX_SEG, AVEC_VIA, eqalim_inf);

		/* ------------ */
		/* vrai segment */
		/* ------------ */

		alloue_etchaine_segment(c1alim_inf, c2alim_inf, pistealim, pistealim, face, SEG_PISTE, segpiste, wreelalim_inf,
		     metalpiste, VRAI_SEG, AVEC_VIA, eqalim_inf);

		/* ------------ */
		/* faux segment */
		/* ------------ */

		for (pistefin = pistealim - 1; pistefin > pistealim - nbpistemoinsalim_inf - 1; pistefin--)
			alloue_etchaine_segment(c1alim_inf, c2alim_inf, pistefin, pistefin, face, SEG_PISTE, segpiste, largmetalpiste,
			     metalpiste, FAUX_SEG, AVEC_VIA, eqalim_inf);
	} /* fin du for */

}

/*-------------------------------------------------------------------------------------*/

void tire_etdeport_alim(char *coursup, long piste_sup[NB_FACES],
	long piste_inf[NB_FACES], LST_EQUIPO lst_equipo,
	BARRE_PLOTS tab_plots[NB_FACES], LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES], long tabpiste[NB_FACES])
{
	LST_EQUIPO equipo_vdd, equipo_vss;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);
	tire_etdeport_unealim(coursup, eq_vdd, piste_sup, piste_inf, equipo_vdd, tab_plots, segx_occ, segy_occ, tabpiste);
	tire_etdeport_unealim(coursup, eq_vss, piste_sup, piste_inf, equipo_vss, tab_plots, segx_occ, segy_occ, tabpiste);

}

/*----------------------------------------------------------------------------------------*/
/* cette procedure tire les fils d'une alim sur la couronne precedemment posees et traite */
/* les deports eventuels                                                                  */
/* On peut se servir de tabpiste pour poser des faux segments, au cas ou on               */
/* tire une alim ds le mauvais layer, ceci pour eviter des croisements                    */
/* (courts-circuits) avec les deports eventuels de plots                                  */
/*----------------------------------------------------------------------------------------*/

void tire_etdeport_unealim(char *coursup, char *eq_alim, long piste_sup[NB_FACES],
	long piste_inf[NB_FACES], LST_EQUIPO equipo, BARRE_PLOTS tab_plots[NB_FACES],
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES],
	long	tabpiste[NB_FACES])
{
	chain_list     * ptcour;
	LST_PSEUDO_CON con;
	PT_COORDONNEES c1, c2;
	long	p1 = 0, p2 = 0, *tabpistealim = NULL, pistealim = 0, largmetalpiste = 0, nbpistemoinsfil = 0, pistefin = 0;
	long	largmetalcol = 0, piste = 0, largeur = 0;
	char	niveaucol = 0, niveaupiste = 0;
	LST_SEGMENT    * segcol = NULL;
	LST_SEGMENT    * segpiste = NULL;
	int	visavis;

	/* ---------------------- */
	/* traitement de une alim */
	/* ---------------------- */

	if (mode_debug) 
		printf("Traitement segment alim %s\n", eq_alim);

	ptcour = equipo->lst_con;

	if (coursup == eq_alim) 
		tabpistealim = piste_sup;
	else 
		tabpistealim = piste_inf;

	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;
		switch (con->face) {
		case NORD:
		case SUD :
			niveaucol = ymetal;
			niveaupiste = xmetal;
			segcol = segy_occ;
			segpiste = segx_occ;
			largmetalpiste = xmetal_width;
			largmetalcol   = ymetal_width;
			break;

		case EST  :
		case OUEST:
			niveaucol = xmetal;
			niveaupiste = ymetal;
			segcol = segx_occ;
			segpiste = segy_occ;
			largmetalpiste = ymetal_width;
			largmetalcol   = xmetal_width;
			break;
		} /* FIN DU SWITCH */

		if ((con->coeur_plot == PLOT_CON) && (con->deport != NULL)) {
			if (con->nom_con != coursup) /* foutu, on est oblige de deporter arghh */ {

				/* ----------------------- */
				/* deport layer uniquement */
				/* ----------------------- */

				if (mode_debug) 
					printf("POSEDEPORTALIM %s\n", con->nom_con);

				c1 = c2 = con->coord; /* segment col */
				p1 = con->deport->piste; /* piste calculee precedemment */
				p2 = tab_plots[con->face].coord.piste;

				/* ------------------------------------------------------------- */
				/* ici ce n'est pas la peine de poser des faux segments, car ils */
      				/* ont deja ete poses dans deport_alim()                         */
				/* ------------------------------------------------------------- */

				if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->layer))
					alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->largeur,
					     con->layer, VRAI_SEG, AVEC_VIA, equipo) ;
				else 
					ringerreur(ERR_SEGDEPORT, (void * )con, NULL);

				/* -------------------------------- */
				/* reste du fil jusqu'a la couronne */
				/* -------------------------------- */

				p2 = p1;
				p1 = tabpistealim[con->face];

				if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, niveaucol))
					alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->largeur,
					     niveaucol, VRAI_SEG, AVEC_VIA, equipo);
				else 
					ringerreur(ERR_SEGDEPORT, (void * )con, NULL);
			} /* fin trt deport  layer  plot */  else {

				/* ----------------------------------------- */
				/* on tire le fil d'alim ds le mauvais layer */
				/* ----------------------------------------- */

				c1 = c2 = con->coord;
				p1 = tabpistealim[con->face];
				p2 = tab_plots[con->face].coord.piste;

				/* -------------------------------- */
				/* reste du fil jusqu'a la couronne */
				/* -------------------------------- */

				/* -------------------------------------------------- */
				/* ALLOCATION DE FAUX SEGMENTS CAR MAUVAIS LAYER TIRE */
				/* -------------------------------------------------- */

				for (piste = tab_plots[con->face].coord.piste; piste > (tabpiste[con->face] + 1); piste--)
					if (segment_libre(c1, c2, piste, piste, con->face, SEG_PISTE, segpiste, niveaupiste))
						alloue_etchaine_segment(c1, c2, piste, piste, con->face, SEG_PISTE, segpiste,
						     largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA, equipo);
					else 
						ringerreur(ERR_TIREALIM, con, NULL);

				if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->layer))
					alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->largeur,
					     con->layer, VRAI_SEG, AVEC_VIA, equipo);
				else 
					ringerreur(ERR_SEGDEPORT, (void * )con, NULL);
			} /* fin du else */
		} /* fin trt deport plot */ else {
			if ((COEUR_CON == con->coeur_plot) && (con->deport != NULL)) /* deport coeur */ {
				if (mode_debug) 
					printf("deport coeur %s xdep %ld ydep %ld piste %ld face %d width %ld\n", con->nom_con,
					     con->deport->xabs, con->deport->yabs, con->deport->piste, (int)con->face, con->largeur);
				if (con->layer != niveaucol)  /* deport layer */ {
					if (mode_debug) 
						printf("SEGPOSEDEPORTALIM %s\n", con->nom_con);

					c1 = c2 = con->coord; /* segment col */
					p1 = 0; 
					p2 = con->deport->piste;

					if ((con->face == NORD) || (con->face == SUD)) {
						if ((con->deport)->xabs == (con->coord)->xabs)
							p2 = con->deport->piste; /* piste calculee precedemment */
						else 
							p2 = (con->largeur + pitch) / pitch;
					} else
					 {
						if ((con->deport)->yabs == (con->coord)->yabs)
							p2 = con->deport->piste; /* piste calculee precedemment */
						else 
							p2 = (con->largeur + pitch) / pitch;
					}

					if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->layer))
						alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->largeur,
						     con->layer, VRAI_SEG, AVEC_VIA, equipo);
					else 
						ringerreur(ERR_SEGDEPORT, (void * )con, NULL);

					if (mode_debug) 
						printf("seg colonne mauvais layer tire \n");

					/* --------------------------- */
					/* Allocation de faux segments */
					/* --------------------------- */

					if (con->largeur > largmetalcol) {
						for (piste = 1; piste <= (p2 + (con->largeur / 2 + pitch) / pitch); piste++)
							alloue_etchaine_segment(c1, c2, piste, piste, con->face, SEG_PISTE,
							     segpiste, largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA, equipo);
					}

				} /* fin trt changement layer */  else { 
					p1 = 0; 
					p2 = 0; 
					c1 = c2 = con->coord;	
				}

				/* ------------------------------------------- */
				/* pose du reste de fil sur la couronne d'alim */
				/* ------------------------------------------- */

				if (con->largeur < largmetalcol) 
					largeur =  largmetalcol;
				else 
					largeur = con->largeur;

				if ((con->face == NORD) || (con->face == SUD)) {
					if ((con->deport)->xabs == (con->coord)->xabs) /* fil direct sur couronne */ {
						p1 = p2;
						p2 = tabpistealim[con->face];
						visavis = 0;
					} else 
						visavis = 1; /* vis  a vis d'alim */

				} else				 {
					if ((con->deport)->yabs == (con->coord)->yabs) /* fil direct sur couronne */ {
						p1 = p2;
						p2 = tabpistealim[con->face];
						visavis = 0;
					}  else 
						visavis = 1; /* vis  a vis d'alim */

				}

				if (!visavis)  /* pas d'autre deport */ {
					if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, niveaucol)) {
						if (!croisementok_alimequi(con, equipo->lst_con, AVEC_TESTLAYER))
							alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol,
							     largeur, niveaucol, VRAI_SEG, AVEC_VIA, equipo);
						else
							alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol,
							     largeur, niveaucol, VRAI_SEG, SANS_VIA, equipo);
						if (mode_debug) 
							printf("seg colonne layer tire sans vis a vis \n");
					}  else 
						ringerreur(ERR_SEGDEPORT, (void * )con, NULL);
				}  else /* Pas de chance , il faut traiter vis a vis d'alim */				 {
					if (mode_debug) 
						printf("vis a vis d'alim a traiter\n");
					pistefin = piste_inf[con->face];
					c1 = con->coord; 
					c2 = con->deport;

					while ((0 != pistefin) && (!segment_libre(c1, c2, pistefin, pistefin, con->face,
					    SEG_PISTE, segpiste, niveaupiste)))
						pistefin--;

					if (0 == pistefin) 
						ringerreur(ERR_NOPISTE, &(con->face), NULL);

					nbpistemoinsfil = ((con->largeur + pitch) / (2 * pitch ));
					pistealim = pistefin - nbpistemoinsfil;

					c2 = con->coord; 
					p1 = p2; 
					p2 = pistealim; /* colonne tiree */
					if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, niveaucol))
						alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol, largeur,
						     niveaucol, VRAI_SEG, AVEC_VIA, equipo);
					else 
						ringerreur(ERR_SEGDEPORT, (void * )con, NULL);

					c2 = con->deport;

					/* ------------ */
					/* deport piste */
					/* faux segment */
					/* ------------ */

					for (; pistefin > pistealim; pistefin--)
						alloue_etchaine_segment(c1, c2, pistefin, pistefin, con->face, SEG_PISTE,
						     segpiste, largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA, equipo);

					/* ------------ */
					/* vrai segment */
					/* ------------ */

					alloue_etchaine_segment(c1, c2, pistealim, pistealim, con->face, SEG_PISTE, segpiste,
					     largeur, niveaupiste, VRAI_SEG, AVEC_VIA, equipo);

					/* ------------ */
					/* faux segment */
					/* ------------ */

					for (pistefin = pistealim - 1; pistefin >= (pistealim - nbpistemoinsfil); pistefin--)
						alloue_etchaine_segment(c1, c2, pistefin, pistefin, con->face, SEG_PISTE,
						     segpiste, largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA, equipo);

					/* ----------------- */
					/* dernier col tiree */
					/* ----------------- */

					c1 = c2 = con->deport; 
					p1 = pistealim; 
					p2 = tabpistealim[con->face];
					if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, niveaucol))
						alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol, largeur,
						     niveaucol, VRAI_SEG, AVEC_VIA, equipo);
					else 
						ringerreur(ERR_SEGDEPORT, (void * )con, NULL);
				} /* fin trt vis a vis */
			} /* fin trt du fil deporte du coeur */   else {

				/* ----------------------------------- */
				/* trt du fil d'alim sans aucun deport */
				/* reste du fil jusqu'a la couronne    */
				/* ----------------------------------- */

				c1 = c2 = con->coord;

				if (con->coeur_plot == PLOT_CON) {
					p2 = tab_plots[con->face].coord.piste; 
					p1 = tabpistealim[con->face];
				}  else {
					p1 = 0; 
					p2 = tabpistealim[con->face];
				}

				if (COEUR_CON == con->coeur_plot) {
					if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->layer)) {
						if (!croisementok_alimequi(con, equipo->lst_con, AVEC_TESTLAYER))
							alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol,
							     con->largeur, con->layer, VRAI_SEG, AVEC_VIA, equipo);
						else
							alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol,
							     con->largeur, con->layer, VRAI_SEG, SANS_VIA, equipo);
					}  else 
						ringerreur(ERR_SEGDEPORT, (void * )con, NULL);
				} else
				 {
					if (segment_libre(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->layer))
						alloue_etchaine_segment(c1, c2, p1, p2, con->face, SEG_COL, segcol, con->largeur,
						     con->layer, VRAI_SEG, AVEC_VIA, equipo);

					else 
						ringerreur(ERR_SEGDEPORT, (void * )con, NULL);
				}

			} /* fin trt plot ou coeur alim sans deport */
		} /* fin du if coeur */

		ptcour = ptcour->NEXT;
	} /* fin du while */

}
