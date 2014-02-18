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
/*    Fichier : deport.c                                                    */
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
/* RING 17 juillet 92                   DEPORT.C                                     */
/* traitement des deports (y compris alim)                                           */
/* et pose des segments pour les deports (hors alim)                                 */
/*-----------------------------------------------------------------------------------*/

#include "deport.h"
#include "placement.h"
#include "sesame.h"

/*-----------------------------------------------------------------------------------*/
/* Attribution des couronnes d'alimentation de maniere a avoir le moins de fils a    */
/* tirer pour la couronne inferieure . Exception: si alim ds mauvais layer, on       */
/* essayre de s'arranger pour mettre ce type d'alim en couronne superieure (on tirera*/
/* directement le fil dans le mauvais layer                                          */
/*-----------------------------------------------------------------------------------*/

void attribuer_couronne_alim(LST_EQUIPO lst_equipo, char **coursup, char **courinf)
{

	LST_EQUIPO equipo_vdd, equipo_vss;
	LST_PSEUDO_CON con;
	ptype_list  * ptvdd = NULL , *ptvss = NULL,  *ptcour;
	int	hyp1, hyp2, vddequi = 0, vddtotal = 0, vssequi = 0, vsstotal = 0;
	char	*alim;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

	ptvdd = equipo_vdd->lst_visavis;
	ptvss = equipo_vss->lst_visavis;

	/* -------------------------- */
	/* trt des visavis equipo vdd */
	/* -------------------------- */

	ptcour = ptvdd;
	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;

		if (mode_debug) 
			printf("visavis  vdd %s\n", con->nom_con);

		if (con->nom_con == eq_vdd) { 
			vddequi++; 
			vddtotal++; 
		} else if (con->nom_con == eq_vss) 
			vddtotal++;

		ptcour = ptcour->NEXT;
	}

	/* -------------------------- */
	/* trt des visavis equipo vss */
	/* -------------------------- */

	ptcour = ptvss;
	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;

		if (mode_debug) 
			printf("visavis  vss %s\n", con->nom_con);

		if (con->nom_con == eq_vss) { 
			vssequi++; 
			vsstotal++; 
		} else if (con->nom_con == eq_vdd) 
			vsstotal++;

		ptcour = ptcour->NEXT;
	}

	hyp1 = vsstotal + vddequi;
	hyp2 = vddtotal + vssequi;

	if ((alim = existe_unique_deport_alim(equipo_vdd, equipo_vss)) == NULL)
		if (hyp1 < hyp2) {
			*courinf = eq_vss; 
			*coursup = eq_vdd;
		}
		else {
			*courinf = eq_vdd; 
			*coursup = eq_vss;
		}
	else
	 {
		if (mode_debug) 
			printf("Unique deport trouve %s\n", alim);
		if (alim == eq_vss) {
			*courinf = eq_vdd; 
			*coursup = eq_vss;
		} else {
			*courinf = eq_vss; 
			*coursup = eq_vdd;
		}
	}

	if (mode_debug) 
		printf("COURSUP %s courinf %s vddtotal %d vsstotal %d vddequi %d vssequi %d\n", *coursup, *courinf, vddtotal,
		     vsstotal, vddequi, vssequi);
}

/*------------------------------------------------------------------------------*/
/* Retourne le nom de l'equipo  alim  ssi unique deport pour les alim           */
/*------------------------------------------------------------------------------*/

char* existe_unique_deport_alim(LST_EQUIPO equipo_vdd, LST_EQUIPO equipo_vss)
{
	LST_PSEUDO_CON con;
	chain_list     * ptvdd = NULL , *ptvss = NULL,  *ptcour;

	int	trouvevdd = 0, trouvevss = 0;

	ptvdd = equipo_vdd->lst_con;
	ptvss = equipo_vss->lst_con;

	ptcour = ptvdd;

	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;
		if ((con->coeur_plot == PLOT_CON) && (con->deport != NULL)) 
			trouvevdd = 1;
		ptcour = ptcour->NEXT;
	}

	ptcour = ptvss;

	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;
		if ((con->coeur_plot == PLOT_CON) && (con->deport != NULL)) 
			trouvevss = 1;
		ptcour = ptcour->NEXT;
	}

	if ((trouvevdd && trouvevss) == 0)
		if (trouvevdd) 
			return(eq_vdd);
		else if (trouvevss) 
			return(eq_vss);
		else 
			return(NULL);

	else 
		return(NULL);

}

/*-----------------------------------------------------------------------------------------*/
/* trt des deports d'alim: pour le coeur changement layer ds ts les cas, prend une         */
/* piste, ou plus si largeur du connecteur non standart.                                   */
/*                         pour les plots, si coursup = alim mauvais layer, on ne deporte  */
/*                         pas, sinon on deporte quand meme.                               */
/*                                                                                         */
/* trt des vis a vis d'alim: si elles se croisent et non equi, on deporte alim coeur       */
/*                          (cherche_deport_alim)                                          */
/*                          on occupe les colonnes avoisinantes pour la taille du fil      */
/*                          si elles se croisent et equi, et croisement < largfil /2       */
/*                          on deporte aussi.                                              */
/*SI DEPORT PLOT ALIM (MAUVAIS LAYER) => ALLOCATION EXCEPTIONNELLE DE FAUX SEGMENTS        */
/*-----------------------------------------------------------------------------------------*/

void deport_alim(LST_EQUIPO lst_equipo, BARRE_PLOTS tab_plots[NB_FACES],
	char *coursup, char *courinf, LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES])
{
	PT_COORDONNEES lst_coor, ptcoor;
	LST_EQUIPO equipo_vdd, equipo_vss;
	LST_PSEUDO_CON con;
	chain_list  * ptvdd = NULL , *ptvss = NULL,  *ptcour;
	ptype_list  * ptvddvav = NULL , *ptvssvav = NULL,  *ptcourvav;
	long	largeurmin = 0;
	int	i, nbcolatrouver, face;
	long	largmetalpiste, largmetalcol, piste;
	char	niveaupiste;
	LST_SEGMENT    * segpiste;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

	ptvdd = equipo_vdd->lst_con;
	ptvss = equipo_vss->lst_con;

	ptvddvav = equipo_vdd->lst_visavis;
	ptvssvav = equipo_vss->lst_visavis;

	for (face = 0; face < NB_FACES; face++) {
		segx_occ[face] = NULL; /* INITIALISATION DES SEGMENTS */
		segy_occ[face] = NULL;
	}

	/* ---------------------------- */
	/* traitement des deports layer */
	/* ---------------------------- */

	ptcour = ptvdd;

	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;
		if ((con->coeur_plot == PLOT_CON) && (con->deport != NULL)) {
			if (con->nom_con != coursup) 

				/* -------------------------------------- */
				/* foutu, on est oblige de deporter arghh */
				/* -------------------------------------- */

				switch (con->face) {
				case NORD:
				case SUD :
					niveaupiste = xmetal;
                  //segcol = segy_occ;
					segpiste = segx_occ;
					largmetalpiste = xmetal_width;
					largmetalcol   = ymetal_width;

					if (mode_debug) 
						printf("deport alimplot layer %s\n", con->nom_con);
					(con->deport)->xabs = (con->coord)->xabs;
					(con->deport)->piste = (tab_plots[con->face].coord).piste - (con->largeur + pitch)
					    / pitch;

					/* ------------------------------------------------- */
					/* allocation de faux segments si deport alim plot ! */
					/* ------------------------------------------------- */

					if (con->largeur > largmetalcol) {
						for (piste = tab_plots[con->face].coord.piste - 1; piste >= (con->deport->piste  -
						    (con->largeur / 2 + pitch) / pitch); piste--)
							alloue_etchaine_segment(con->coord, con->coord, piste, piste, con->face,
							     SEG_PISTE, segpiste, largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA,
							     equipo_vdd);
					}

					break;

				case EST  :
				case OUEST:
					niveaupiste = ymetal;
                  //segcol = segx_occ;
					segpiste = segy_occ;
					largmetalpiste = ymetal_width;
					largmetalcol   = xmetal_width;

					if (mode_debug) 
						printf("deport alimplot layer %s\n", con->nom_con);
					(con->deport)->yabs = (con->coord)->yabs;
					(con->deport)->piste = (tab_plots[con->face].coord).piste - (con->largeur + pitch)
					    / pitch;

					/* ------------------------------------------------- */
					/* allocation de faux segments si deport alim plot ! */
					/* ------------------------------------------------- */

					if (con->largeur > largmetalcol) {
						for (piste = tab_plots[con->face].coord.piste - 1; piste >= (con->deport->piste  -
						    (con->largeur / 2 + pitch) / pitch); piste--)
							alloue_etchaine_segment(con->coord, con->coord, piste, piste, con->face,
							     SEG_PISTE, segpiste, largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA,
							     equipo_vdd);
					}

					break;
				}
		} /* fin du if */ else if (con->deport != NULL) /* deport coeur */
			switch (con->face) {
			case NORD:
			case SUD :
				largeurmin = ymetal_width;
				if (mode_debug) 
					printf("deport coeuralim  layer %s\n", con->nom_con);
				(con->deport)->xabs = (con->coord)->xabs;
				if (con->largeur > largeurmin) 
					(con->deport)->piste = (con->largeur + pitch) / pitch;
				else 
					(con->deport)->piste = 1; /* premiere piste pour deport layer */

				break;

			case EST  :
			case OUEST:
				largeurmin = xmetal_width;
				if (mode_debug) 
					printf("deport coeuralim layer %s\n", con->nom_con);
				(con->deport)->yabs = (con->coord)->yabs;
				if (con->largeur > largeurmin) 
					(con->deport)->piste = (con->largeur + pitch) / pitch;
				else 
					(con->deport)->piste = 1; /* premiere piste pour deport layer */

				break;
			}

		ptcour = ptcour->NEXT;
	}

	ptcour = ptvss;

	while (ptcour != NULL) {
		con = (LST_PSEUDO_CON) ptcour->DATA;
		if ((con->coeur_plot == PLOT_CON) && (con->deport != NULL)) {
			if (con->nom_con != coursup) 

				/* -------------------------------------- */
				/* foutu, on est oblige de deporter arghh */
				/* -------------------------------------- */

				switch (con->face) {
				case NORD:
				case SUD :
					niveaupiste = xmetal;
                  //segcol = segy_occ;
					segpiste = segx_occ;
					largmetalpiste = xmetal_width;
					largmetalcol   = ymetal_width;

					if (mode_debug) 
						printf("deport alimplot layer  %s\n", con->nom_con);
					(con->deport)->xabs = (con->coord)->xabs;
					(con->deport)->piste = (tab_plots[con->face].coord).piste - (con->largeur + pitch) / pitch;

					/* ------------------------------------------------- */
					/* allocation de faux segments si deport alim plot ! */
					/* ------------------------------------------------- */

					if (con->largeur > largmetalcol) {
						for (piste = tab_plots[con->face].coord.piste - 1; piste >= (con->deport->piste  -
						    (con->largeur / 2 + pitch) / pitch); piste--)
							alloue_etchaine_segment(con->coord, con->coord, piste, piste, con->face,
							     SEG_PISTE, segpiste, largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA,
							     equipo_vss);
					}

					break;

				case EST  :
				case OUEST:
					niveaupiste = ymetal;
                  //segcol = segx_occ;
					segpiste = segy_occ;
					largmetalpiste = ymetal_width;
					largmetalcol   = xmetal_width;

					if (mode_debug) 
						printf("deport alimplot layer %s\n", con->nom_con);
					(con->deport)->yabs = (con->coord)->yabs;
					(con->deport)->piste = (tab_plots[con->face].coord).piste - (con->largeur + pitch)
					    / pitch;

					/* ------------------------------------------------- */
					/* allocation de faux segments si deport alim plot ! */
					/* ------------------------------------------------- */

					if (con->largeur > largmetalcol) {
						for (piste = tab_plots[con->face].coord.piste - 1; piste >= (con->deport->piste  -
						    (con->largeur / 2 + pitch) / pitch); piste--)
							alloue_etchaine_segment(con->coord, con->coord, piste, piste, con->face,
							     SEG_PISTE, segpiste, largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA,
							     equipo_vss);
					}

					break;
				}
		} /* fin du if */ else if (con->deport != NULL) /* deport coeur */
			switch (con->face) {
			case NORD:
			case SUD :
				if (mode_debug) 
					printf("deport coeuralim layer %s\n", con->nom_con);
				(con->deport)->xabs = (con->coord)->xabs;
				if (con->largeur > largeurmin) 
					(con->deport)->piste = (con->largeur + pitch) / pitch;
				else 
					(con->deport)->piste = 1; /* premiere piste pour deport layer */

				break;

			case EST  :
			case OUEST:
				if (mode_debug) 
					printf("deport coeuralim layer %s\n", con->nom_con);
				(con->deport)->yabs = (con->coord)->yabs;
				if (con->largeur > largeurmin) 
					(con->deport)->piste = (con->largeur + pitch) / pitch;
				else 
					(con->deport)->piste = 1; /* premiere piste pour deport layer */

				break;
			}

		ptcour = ptcour->NEXT;
	}

	/*-----------------------------------------------------------------------------------*/
	/*    traitement des vis a  vis coeur alim, si alim <> et se croisent on deporte     */
	/*    si alim = et croisement < largfil /2                                           */
	/*-----------------------------------------------------------------------------------*/

	if (mode_debug) 
		printf("traitement des vis a vis alim\n");

	ptcourvav = ptvddvav;

	while (ptcourvav != NULL) {
		con = (LST_PSEUDO_CON) ptcourvav->DATA;
		if ((con->coeur_plot == COEUR_CON) && (con->nom_con == eq_vss) && (courinf == eq_vdd)) {

			/* -------------------------------------- */
			/* foutu, on est oblige de deporter arghh */
			/* -------------------------------------- */

			if (mode_debug) 
				printf("\n**** alim visavis coeur vss %s\t", con->con_lo->NAME);
			ptcoor = cherche_deport_grille(con, ptvdd);

			/* ------------------------------------------------------- */
			/* Occupation des colonnes necessaires au deport de l'alim */
			/* ------------------------------------------------------- */

			nbcolatrouver = (con->largeur + pitch) / (2 * pitch);
			lst_coor = ptcoor->suiv;
			for (i = 0; i < nbcolatrouver; i++)
				if ((NULL != lst_coor) && (NULL == lst_coor->proprio)) {
					if (mode_debug) 
						printf("colonne occupee par deport x %ld y %ld\n", lst_coor->xabs, lst_coor->yabs);
					lst_coor->proprio = (void * )con;
					lst_coor = lst_coor->suiv; 
				}

			lst_coor = ptcoor->prec;
			for (i = 0; i < nbcolatrouver; i++)
				if ((NULL != lst_coor) && (NULL == lst_coor->proprio)) {
					if (mode_debug) 
						printf("colonne occupee par deport x %ld y %ld\n", lst_coor->xabs, lst_coor->yabs);
					lst_coor->proprio = (void * )con;
					lst_coor = lst_coor->prec;
				}

			con->deport = ptcoor;
			ptcoor->proprio = (void * ) con;
			if (mode_debug) 
				printf("deport alim coeur trouve %ld %ld\n", ptcoor->xabs, ptcoor->yabs);

		}

		if ((con->coeur_plot == COEUR_CON) && (con->nom_con == eq_vdd) && (!croisementok_alimequi(con, ptvdd, SANS_TESTLAYER))) {

			/* -------------------------------------- */
			/* foutu, on est oblige de deporter arghh */
			/* -------------------------------------- */

			if (mode_debug) 
				printf("\n**** alim equi visavis coeur vdd %s\t", con->con_lo->NAME);
			ptcoor = cherche_deport_grille(con, ptvdd);

			/* ------------------------------------------------------- */
			/* Occupation des colonnes necessaires au deport de l'alim */
			/* ------------------------------------------------------- */

			nbcolatrouver = (con->largeur + pitch) / (2 * pitch);
			lst_coor = ptcoor->suiv;
			for (i = 0; i < nbcolatrouver; i++)
				if ((NULL != lst_coor) && (NULL == lst_coor->proprio)) {
					if (mode_debug) 
						printf("colonne occupee par deport x %ld y %ld\n", lst_coor->xabs, lst_coor->yabs);
					lst_coor->proprio = (void * )con;
					lst_coor = lst_coor->suiv; 
				}

			lst_coor = ptcoor->prec;
			for (i = 0; i < nbcolatrouver; i++)
				if ((NULL != lst_coor) && (NULL == lst_coor->proprio)) {
					if (mode_debug) 
						printf("colonne occupee par deport x %ld y %ld\n", lst_coor->xabs, lst_coor->yabs);
					lst_coor->proprio = (void * )con;
					lst_coor = lst_coor->prec;
				}

			con->deport = ptcoor;
			ptcoor->proprio = (void * ) con;
			if (mode_debug) 
				printf("deport alim coeur trouve %ld %ld\n", ptcoor->xabs, ptcoor->yabs);

		}

		ptcourvav = ptcourvav->NEXT;
	}

	ptcourvav = ptvssvav;

	while (ptcourvav != NULL) {
		con = (LST_PSEUDO_CON) ptcourvav->DATA;
		if ((con->coeur_plot == COEUR_CON) && (con->nom_con == eq_vdd) && (courinf == eq_vss)) {

			/* -------------------------------------- */
			/* foutu, on est oblige de deporter arghh */
			/* -------------------------------------- */

			if (mode_debug) 
				printf("\n**** alim visavis coeur vdd %s\t", con->con_lo->NAME);
			ptcoor = cherche_deport_grille(con, ptvss);

			/* ------------------------------------------------------- */
			/* Occupation des colonnes necessaires au deport de l'alim */
			/* ------------------------------------------------------- */

			nbcolatrouver = (con->largeur + pitch) / (2 * pitch);
			lst_coor = ptcoor->suiv;
			for (i = 0; i < nbcolatrouver; i++)
				if ((NULL != lst_coor) && (NULL == lst_coor->proprio)) {
					if (mode_debug) 
						printf("colonne occupee par deport x %ld y %ld\n", lst_coor->xabs, lst_coor->yabs);
					lst_coor->proprio = (void * )con;
					lst_coor = lst_coor->suiv; 
				}

			lst_coor = ptcoor->prec;
			for (i = 0; i < nbcolatrouver; i++)
				if ((NULL != lst_coor) && (NULL == lst_coor->proprio)) {
					if (mode_debug) 
						printf("colonne occupee par deport x %ld y %ld\n", lst_coor->xabs, lst_coor->yabs);
					lst_coor->proprio = (void * )con;
					lst_coor = lst_coor->prec;
				}

			con->deport = ptcoor;
			ptcoor->proprio = (void * ) con;
			if (mode_debug) 
				printf("deport alim coeur trouve %ld %ld\n", ptcoor->xabs, ptcoor->yabs);

		}

		if ((con->coeur_plot == COEUR_CON) && (con->nom_con == eq_vss) && (!croisementok_alimequi(con, ptvss, SANS_TESTLAYER))) {

			/* -------------------------------------- */
			/* foutu, on est oblige de deporter arghh */
			/* -------------------------------------- */

			if (mode_debug) 
				printf("\n**** alim equi visavis coeur vss %s\t", con->con_lo->NAME);
			ptcoor = cherche_deport_grille(con, ptvss);

			/* ------------------------------------------------------- */
			/* Occupation des colonnes necessaires au deport de l'alim */
			/* ------------------------------------------------------- */

			nbcolatrouver = (con->largeur + pitch) / (2 * pitch);
			lst_coor = ptcoor->suiv;
			for (i = 0; i < nbcolatrouver; i++)
				if ((NULL != lst_coor) && (NULL == lst_coor->proprio)) {
					if (mode_debug) 
						printf("colonne occupee par deport x %ld y %ld\n", lst_coor->xabs, lst_coor->yabs);
					lst_coor->proprio = (void * )con;
					lst_coor = lst_coor->suiv; 
				}

			lst_coor = ptcoor->prec;
			for (i = 0; i < nbcolatrouver; i++)
				if ((NULL != lst_coor) && (NULL == lst_coor->proprio)) {
					if (mode_debug) 
						printf("colonne occupee par deport x %ld y %ld\n", lst_coor->xabs, lst_coor->yabs);
					lst_coor->proprio = (void * )con;
					lst_coor = lst_coor->prec;
				}

			con->deport = ptcoor;
			ptcoor->proprio = (void * ) con;
			if (mode_debug) 
				printf("deport alim coeur trouve %ld %ld\n", ptcoor->xabs, ptcoor->yabs);

		}

		ptcourvav = ptcourvav->NEXT;
	}

}

/*------------------------------------------------------------------------------------*/
/*  Traitement de tous les deports coeur et plot sauf alim (deja fait)                */
/*  on commence par traiter les deports qui sont en face du coeur,                    */
/*  puis les deports gauches                                                          */
/*  puis les deports droits                                                           */
/*------------------------------------------------------------------------------------*/

void deport_connecteurs(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES],
	COEUR lecoeur, GRILLE tab_grilles[NB_FACES])
{

	int	face, trouve;
	long	/*pistelayercentre, pistelayergauche, pistelayerdroite,*/ piste, pistederniere;
	long	largeurmin;

	LST_PSEUDO_CON liste_coeur, liste_plots, ptcentre, ptdernier;
	PT_COORDONNEES ptcoor, ptqueue;

	for (face = 0; face < NB_FACES; face++) {

		if (mode_debug) 
			printf("FACE %d\n", face);

		/* --------------------------------------------------- */
		/* Tous les connecteurs coeur/plot trt layer sauf alim */
		/* Quand  on est dans la croix du coeur trt specialu   */
		/* --------------------------------------------------- */

		liste_plots = tab_plots[face].lst_con;
		liste_coeur = tab_coeur[face];
      //pistelayergauche = 0;
      //pistelayerdroite = 0;
      //pistelayercentre = 0;
		ptcentre = NULL;
		ptdernier = NULL;
		pistederniere = (tab_plots[face].coord).piste;

		switch (face) {
		case NORD:
		case SUD :

			/* ------------------------------------------------------------------------------------------------------------------- */
			/* ptcentre debut conplots dans la face du coeur, ptdernier dernier conplot normalement en dehors de la croix du coeur */
			/* ------------------------------------------------------------------------------------------------------------------- */

			largeurmin = ymetal_width;

			while (liste_plots != NULL) {
				if ((liste_plots->coord)->xabs >= (lecoeur.coord).xabs) {
					if ((liste_plots->prec == NULL) ||  (((liste_plots->prec)->coord)->xabs  < (lecoeur.coord).xabs))
						ptcentre = liste_plots;
				}
				if (liste_plots->suiv ==  NULL) 
					ptdernier = liste_plots;

				liste_plots = liste_plots->suiv;
			}

			/*------------------- TRT DES DEPORTS DES CONPLOTS DU CENTRE ---------------------- */

			while ((ptcentre != NULL) && ((ptcentre->coord)->xabs <= ((lecoeur.coord).xabs + lecoeur.width))) {
				if ((ptcentre->nom_con != eq_vdd) && (ptcentre->nom_con != eq_vss))

					if (ptcentre->deport != NULL) {
						ptcentre->situe_deport = DEPORTC;

						if (mode_debug) 
							printf("deport centre con %s %s piste%ld\n", ptcentre->nom_con, 
								((loins_list *)(ptcentre->con_lo)->ROOT)->INSNAME, (ptcentre->deport)->piste);
                      //pistelayercentre = 1;
						if ((ptcentre->deport)->piste != 1) /* pas uniquement layer */ {
							
							/*----------------------------------------------------------------------------*/
							/* Le deport n'est pas chaine dans la grille , donc on appelle la procedure   */
							/* qui modifie ses pointeurs suiv et prec                                     */
							/*----------------------------------------------------------------------------*/

							chaine_deportcon_grille(ptcentre->coord, tab_grilles[face].lst_pas,
							     face);

							/* ----------------------------------------- */
							/* on marque qu'il existe des deports layers */
							/* ----------------------------------------- */

							//if (ptcentre->layer != ymetal) 
							//	pistelayercentre = 1;

							if (((ptcentre->coord)->prec != NULL) && (((ptcentre->coord)->prec)->proprio ==
							    NULL)) /*gauche */ {
								ptcentre->deport = (ptcentre->coord)->prec;
								(ptcentre->deport)->proprio = (void * )ptcentre;
								(ptcentre->deport)->piste   = pistederniere - 1; /* 1ere piste */
							} else if (((ptcentre->coord)->suiv != NULL) && (((ptcentre->coord)->suiv)->proprio ==
							    NULL)) /* droite */ {
								ptcentre->deport = (ptcentre->coord)->suiv;
								(ptcentre->deport)->proprio = (void * ) ptcentre;
								(ptcentre->deport)->piste   = pistederniere - 1; /* 1ere piste */
							} else
								ringerreur(ERR_ECHECDEPORT, (void * )ptcentre, NULL);
						} else /* trt layer unique */								 {
							(ptcentre->deport)->xabs = (ptcentre->coord)->xabs;
							(ptcentre->deport)->piste   = pistederniere - 1; /* 1ere piste */
						}

					} /* fin trt deport */
				ptcentre = ptcentre->suiv;

			} /* fin trt centre */

			/*-------------- TRT DES DEPORTS GAUCHE EN DEHORS DE FACE COEUR --------------------- */

			liste_plots = tab_plots[face].lst_con;
			piste = pistederniere - 2; /* on laisse une piste pour les deports */

			while ((liste_plots != NULL) && ((liste_plots->coord)->xabs < (lecoeur.coord).xabs)) {
            //if (liste_plots->deport->piste == 1) 
			//		pistelayergauche = 1;

				/* -------------------- */
				/* ds ts les cas deport */
				/* -------------------- */

				liste_plots->situe_deport = DEPORTG;

				if (mode_debug) 
					printf("deport gauche con %s %s\n", liste_plots->nom_con, ((loins_list * )(liste_plots->con_lo)->ROOT)->INSNAME);
				trouve = 0;

				ptcoor = tab_grilles[face].lst_pas;
				while (ptcoor != NULL) {
					trouve = (ptcoor->proprio == NULL);
					if (trouve) 
						break;
					ptcoor = ptcoor->suiv;
				}

				if (trouve) {
					ptcoor->piste  = piste;
					ptcoor->proprio = (void * ) liste_plots;
					piste--;
					liste_plots->deport = ptcoor;
				} else
					ringerreur(ERR_ECHECDEPORT, liste_plots, NULL);

				liste_plots = liste_plots->suiv;
			}

			/*------------- TRT DES DEPORTS DROIT EN DEHORS FACE COEUR --------------------------*/

			ptqueue = tab_grilles[face].lst_pas;
			while ((ptqueue != NULL) && (ptqueue->suiv != NULL))
				ptqueue = ptqueue->suiv;

			piste = pistederniere - 2; /* on laisse une piste pour les deports */

			while ((ptdernier != NULL) && ((ptdernier->coord)->xabs > ((lecoeur.coord).xabs + lecoeur.width))) {
            //if ((ptdernier->deport)->piste == 1) 
				//	pistelayerdroite = 1;

				/* -------------------- */
				/* ds ts les cas deport */
				/* -------------------- */

				ptdernier->situe_deport = DEPORTD;

				if (mode_debug) 
					printf("deport droite con %s %s\n", ptdernier->nom_con, ((loins_list * )(ptdernier->con_lo)->ROOT)->INSNAME);

				trouve = 0;

				ptcoor = ptqueue;
				while (ptcoor != NULL) {
					if (mode_debug) 
						printf("coord x%ld y%ld \n", ptcoor->xabs, ptcoor->yabs);

					trouve = (ptcoor->proprio == NULL);
					if (trouve) 
						break;
					ptcoor = ptcoor->prec;
				}

				if (trouve) {
					ptcoor->piste  = piste;
					ptcoor->proprio = (void * ) ptdernier;
					piste--;
					ptdernier->deport = ptcoor;
				} else
					ringerreur(ERR_ECHECDEPORT, ptdernier, NULL);

				ptdernier = ptdernier->prec;
			}

			/*---------------------- TRT DES CONCOEUR, DEPORT LAYER UNIQUEMENT --------------------*/

			while (liste_coeur != NULL) {
				if ((liste_coeur->deport != NULL) && (liste_coeur->nom_con != eq_vdd) && (liste_coeur->nom_con !=
				    eq_vss)) /* changement layer */ {
					liste_coeur->situe_deport = DEPORTC;
					liste_coeur->deport->xabs = (liste_coeur->coord)->xabs;
					if (liste_coeur->largeur > largeurmin) 
						(liste_coeur->deport)->piste = (liste_coeur->largeur + pitch) / pitch;
					else 
						(liste_coeur->deport)->piste = 1; /* premiere piste pour deport layer */
					if (mode_debug)
						printf("Deport layer concoeur %s piste %ld largeur %ld\n", liste_coeur->nom_con,
						     liste_coeur->deport->piste, liste_coeur->largeur);
				}
				liste_coeur = liste_coeur->suiv;
			}

			break;

		case EST  :
		case OUEST:

			/* ------------------------------------------------------------------------------------------------------------------- */
			/* ptcentre debut conplots dans la face du coeur, ptdernier dernier conplot normalement en dehors de la croix du coeur */
			/* ------------------------------------------------------------------------------------------------------------------- */

			largeurmin = xmetal_width;

			while (liste_plots != NULL) {
				if ((liste_plots->coord)->yabs >= (lecoeur.coord).yabs) {
					if ((liste_plots->prec == NULL) ||  (((liste_plots->prec)->coord)->yabs  < (lecoeur.coord).yabs))
						ptcentre = liste_plots;
				}
				if (liste_plots->suiv ==  NULL) 
					ptdernier = liste_plots;

				liste_plots = liste_plots->suiv;
			}

			/*------------------ TRT DES DEPORTS DES CONPLOTS DU CENTRE --------------------------*/

			while ((ptcentre != NULL) && ((ptcentre->coord)->yabs <= ((lecoeur.coord).yabs + lecoeur.height))) {
				if ((ptcentre->nom_con != eq_vdd) && (ptcentre->nom_con != eq_vss))

					if (ptcentre->deport != NULL) {
						ptcentre->situe_deport = DEPORTC;
                      //pistelayercentre = 1;
						if ((ptcentre->deport)->piste != 1) /* pas uniquement layer */ {
							if (mode_debug) 
								printf("deport centre con %s %s piste%ld\n", ptcentre->nom_con,
								     ((loins_list * )(ptcentre->con_lo)->ROOT)->INSNAME, (ptcentre->deport)->piste);

							/* ------------------------------------------------------------------------ */
							/* Le deport n'est pas chaine dans la grille , donc on appelle la procedure */
							/* qui modifie ses pointeurs suiv et prec                                   */
							/* ------------------------------------------------------------------------ */

							chaine_deportcon_grille(ptcentre->coord, tab_grilles[face].lst_pas,
							     face);

							/* ----------------------------------------- */
							/* on marque qu'il existe des deports layers */
							/* ----------------------------------------- */

                          //if (ptcentre->layer != xmetal) 
                          //	pistelayercentre = 1;

							if (((ptcentre->coord)->prec != NULL) && (((ptcentre->coord)->prec)->proprio ==
							    NULL)) /*gauche */ {
								ptcentre->deport = (ptcentre->coord)->prec;
								(ptcentre->deport)->proprio = (void * )ptcentre;
								(ptcentre->deport)->piste   = pistederniere - 1; /* 1ere piste */
							} else if (((ptcentre->coord)->suiv != NULL) && (((ptcentre->coord)->suiv)->proprio ==
							    NULL)) /* droite */ {
								ptcentre->deport = (ptcentre->coord)->suiv;
								(ptcentre->deport)->proprio = (void * ) ptcentre;
								(ptcentre->deport)->piste   = pistederniere - 1; /* 1ere piste */
							} else
								ringerreur(ERR_ECHECDEPORT, ptcentre, NULL);
						} else /* trt layer unique */								 {
							(ptcentre->deport)->yabs = (ptcentre->coord)->yabs;
							(ptcentre->deport)->piste   = pistederniere - 1; /* 1ere piste */
						}

					} /* fin trt deport */
				ptcentre = ptcentre->suiv;

			} /* fin trt centre */

			/*--------------- TRT DES DEPORTS GAUCHE EN DEHORS DE FACE COEUR -------------------- */

			liste_plots = tab_plots[face].lst_con;
			piste = pistederniere - 2; /* on laisse une piste pour les deports */

			while ((liste_plots != NULL) && ((liste_plots->coord)->yabs < (lecoeur.coord).yabs)) {
            //if (liste_plots->deport->piste == 1) 
				//	pistelayergauche = 1;
				/* ds ts les cas deport */

				liste_plots->situe_deport = DEPORTG;

				if (mode_debug) 
					printf("deport gauche con %s %s\n", liste_plots->nom_con, ((loins_list * )(liste_plots->con_lo)->ROOT)->INSNAME);

				trouve = 0;

				ptcoor = tab_grilles[face].lst_pas;
				while (ptcoor != NULL) {
					trouve = (ptcoor->proprio == NULL);
					if (trouve) 
						break;
					ptcoor = ptcoor->suiv;
				}

				if (trouve) {
					ptcoor->piste  = piste;
					ptcoor->proprio = (void * ) liste_plots;
					piste--;
					liste_plots->deport = ptcoor;
				} else
					ringerreur(ERR_ECHECDEPORT, liste_plots, NULL);
				liste_plots = liste_plots->suiv;
			}

			/*------------------ TRT DES DEPORTS DROIT EN DEHORS FACE COEUR----------------------- */

			ptqueue = tab_grilles[face].lst_pas;
			while ((ptqueue != NULL) && (ptqueue->suiv != NULL))
				ptqueue = ptqueue->suiv;

			piste = pistederniere - 2; /* on laisse une piste pour les deports */

			while ((ptdernier != NULL) && ((ptdernier->coord)->yabs > ((lecoeur.coord).yabs + lecoeur.height))) {
            //if (ptdernier->deport->piste == 1) 
			//		pistelayerdroite = 1;

				/* -------------------- */
				/* ds ts les cas deport */
				/* -------------------- */

				ptdernier->situe_deport = DEPORTD;

				if (mode_debug) 
					printf("deport droite con %s %s\n", ptdernier->nom_con, ((loins_list * )(ptdernier->con_lo)->ROOT)->INSNAME);

				trouve = 0;

				ptcoor = ptqueue;
				while (ptcoor != NULL) {
					trouve = (ptcoor->proprio == NULL);
					if (trouve) 
						break;
					ptcoor = ptcoor->prec;
				}

				if (trouve) {
					ptcoor->piste  = piste;
					ptcoor->proprio = (void * ) ptdernier;
					piste--;
					ptdernier->deport = ptcoor;
				} else
					ringerreur(ERR_ECHECDEPORT, ptdernier, NULL);

				ptdernier = ptdernier->prec;
			}

			/*----------------------- TRT DES CONCOEUR, DEPORT LAYER UNIQUEMENT ----------------- */

			while (liste_coeur != NULL) {
				if ((liste_coeur->deport != NULL) && (liste_coeur->nom_con != eq_vdd) && (liste_coeur->nom_con !=
				    eq_vss)) /* changement layer */ {
					liste_coeur->situe_deport = DEPORTC;
					liste_coeur->deport->yabs = (liste_coeur->coord)->yabs;
					if (liste_coeur->largeur > largeurmin) 
						(liste_coeur->deport)->piste = (liste_coeur->largeur + pitch) / pitch;
					else 
						(liste_coeur->deport)->piste = 1; /* premiere piste pour deport layer */
					if (mode_debug)
						printf("Deport layer concoeur %s piste %ld largeur %ld\n", liste_coeur->nom_con,
						     liste_coeur->deport->piste, liste_coeur->largeur);
				}
				liste_coeur = liste_coeur->suiv;
			}

			break;
		} /* fin du switch , ouf */

	} /* fin du for */
}

/*-------------------------------------------------------------------------------------------------------*/
/* Allocation des segments pour les deports. Pour les deports en face du coeur on reste                  */
/* dans le meme  metal                                                                                   */
/* bug corrige dans le cas de deport en face du coeur: existe_sig_equipo ajoute                          */
/* 14 sept 92: nouveau bug corrige: pour les deports en dehors du coeur                                  */
/* les segments pistes ne sont plus tires de la largeur du connecteur,(erreur avec nouv pad12 et pad 15) */
/* mais de la largeur du via si plus grand que le minimum.                                               */
/*-------------------------------------------------------------------------------------------------------*/

void pose_segdeport(BARRE_PLOTS tab_plots[NB_FACES],
	LST_PSEUDO_CON tab_coeur[NB_FACES], COEUR lecoeur, LST_EQUIPO lst_equipo,
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES])
{
	LST_EQUIPO     eq;
	LST_PSEUDO_CON lp, lc;
	PT_COORDONNEES c1, c2, deb, fin;
	long	p1 = 0, p2 = 0, largmetalpiste = 0, largmetalcol = 0, largcolvia = 0, largpistevia = 0, piste = 0, largeurseg = 0;
	int	face;
	char	niveaucol = 0, niveaupiste = 0;
	LST_SEGMENT    * segcol = NULL;
	LST_SEGMENT    * segpiste = NULL;

	alloue_coord(0L, 0, &deb); /* coordonnees pour reserver la piste de deport du coeur */
	alloue_coord(0L, 0, &fin);
	deb->xabs = lecoeur.coord.xabs - 10; 
	fin->xabs = lecoeur.width + lecoeur.coord.xabs + 10;
	deb->yabs = lecoeur.coord.yabs - 10; 
	fin->yabs = lecoeur.height + lecoeur.coord.yabs + 10;
	;

	for (face = 0; face < NB_FACES; face++) {
		lp = tab_plots[face].lst_con;
		lc = tab_coeur[face];

#ifdef COMMENT
		/* ----------------------------------------------------------------- */
		/* init mises dans deport alim au lieu d'ici (pour deport plot alim) */
		/* ----------------------------------------------------------------- */

		segx_occ[face] = NULL; /* INITIALISATION DES SEGMENTS */
		segy_occ[face] = NULL;
#endif

		switch (face) {
		case NORD:
		case SUD :
			niveaucol = ymetal;
			largcolvia = ymetal_wvia;
			largmetalcol = ymetal_width;
			niveaupiste = xmetal;
			largmetalpiste = xmetal_width;
			largpistevia = xmetal_wvia;
			segcol = segy_occ;
			segpiste = segx_occ;
			break;
		case EST  :
		case OUEST:
			niveaucol = xmetal;
			largcolvia = xmetal_wvia;
			largmetalcol = xmetal_width;
			niveaupiste = ymetal;
			largmetalpiste = ymetal_width;
			largpistevia = ymetal_wvia;
			segcol = segx_occ;
			segpiste = segy_occ;
			break;
		}

		while (NULL != lp) {
			if ((NULL != lp->deport) && (lp->nom_con != eq_vdd) && (lp->nom_con != eq_vss)) {

				/* ------------- */
				/* trt du deport */
				/* ------------- */

				if ((DEPORTG == lp->situe_deport) || (DEPORTD == lp->situe_deport)) { /* DEPORT GAUCHE OU DROIT */

					eq = existe_sig_equipo(lst_equipo, ((lp->con_lo)->SIG)->INDEX);
					if (NULL == eq) 
						ringerreur(ERR_INTERNE_EQ, NULL, NULL);

					if (lp->layer != niveaucol) { /* deport layer d'abord */
						c1 = c2 = lp->coord; /* segment col */
						p2 = (tab_plots[face].coord).piste;
						p1 = p2 - 1; /* 1 piste pour dep layer */

						if (segment_libre(c1, c2, p1, p2, face, SEG_COL, segcol, lp->layer))
							alloue_etchaine_segment(c1, c2, p1, p2, face, SEG_COL, segcol, lp->largeur,
							     lp->layer, VRAI_SEG, AVEC_VIA, eq);
						else 
							ringerreur(ERR_SEGDEPORT, (void * )lp, NULL);

						/* ---------------------------- */
						/* ensuite seg col ds bon layer */
						/* ---------------------------- */

						p2 = p1;
						p1 = (lp->deport)->piste;

						if (lp->largeur > largcolvia) 
							largeurseg = largcolvia;
						else if (lp->largeur < largmetalcol) 
							largeurseg = largmetalcol;
						else 
							largeurseg = lp->largeur;

						if (segment_libre(c1, c2, p1, p2, face, SEG_COL, segcol, niveaucol))
							alloue_etchaine_segment(c1, c2, p1, p2, face, SEG_COL, segcol, largeurseg,
							     niveaucol, VRAI_SEG, AVEC_VIA, eq);
						else 
							ringerreur(ERR_SEGDEPORT, (void * )lp, NULL);
					} else
					 { /* seg col tire */
						c1 = c2 = lp->coord; /* segment col */
						p2 = (tab_plots[face].coord).piste;
						p1 = (lp->deport)->piste;

						if (segment_libre(c1, c2, p1, p2, face, SEG_COL, segcol, lp->layer))
							alloue_etchaine_segment(c1, c2, p1, p2, face, SEG_COL, segcol, lp->largeur,
							     lp->layer, VRAI_SEG, AVEC_VIA, eq);
						else 
							ringerreur(ERR_SEGDEPORT, (void * )lp, NULL);
					}

					/* -------------- */
					/* seg piste tire */
					/* -------------- */

					c1 = lp->coord;
					c2 = lp->deport;

					p1 = p2 = (lp->deport)->piste;

					if (lp->largeur > largpistevia) 
						largeurseg = largpistevia;
					else if (lp->largeur < largmetalpiste) 
						largeurseg = largmetalpiste;
					else 
						largeurseg = lp->largeur;

					if (segment_libre(c1, c2, p1, p2, face, SEG_PISTE, segpiste, niveaupiste))
						alloue_etchaine_segment(c1, c2, p1, p2, face, SEG_PISTE, segpiste, largeurseg,
						     niveaupiste, VRAI_SEG, AVEC_VIA, eq);
					else 
						ringerreur(ERR_SEGDEPORT, (void * )lp, NULL);
				}  else /* DEPORT DANS EN FACE DU COEUR */ {

					eq = existe_sig_equipo(lst_equipo, ((lp->con_lo)->SIG)->INDEX);
					if (NULL == eq) 
						ringerreur(ERR_INTERNE_EQ, NULL, NULL);

					/* ------------ */
					/* seg col tire */
					/* ------------ */

					c1 = c2 = lp->coord; /* segment col */
					p2 = (tab_plots[face].coord).piste;
					p1 = (lp->deport)->piste;

					if (segment_libre(c1, c2, p1, p2, face, SEG_COL, segcol, lp->layer))
						alloue_etchaine_segment(c1, c2, p1, p2, face, SEG_COL, segcol, lp->largeur,
						     lp->layer, VRAI_SEG, AVEC_VIA, eq);
					else 
						ringerreur(ERR_SEGDEPORT, (void * )lp, NULL);

					/* -------------- */
					/* seg piste tire */
					/* -------------- */

					c1 = lp->coord;
					c2 = lp->deport;

					p1 = p2 = (lp->deport)->piste;

					/* ------------------------------- */
					/* LES 2 SEGMENTS DS LE MEME METAL */
					/* ------------------------------- */

					if (segment_libre(c1, c2, p1, p2, face, SEG_PISTE, segpiste, lp->layer))
						alloue_etchaine_segment(c1, c2, p1, p2, face, SEG_PISTE, segpiste, lp->largeur,
						     lp->layer, VRAI_SEG, AVEC_VIA, eq);
					else 
						ringerreur(ERR_SEGDEPORT, (void * )lp, NULL);
				}
			} /* fin du if  deport */

			lp = lp->suiv;
		} /* fin du while */

		while (NULL != lc) {
			if ((NULL != lc->deport) && (lc->nom_con != eq_vdd) && (lc->nom_con != eq_vss)) { /* deport layer uniquement */
				eq = existe_sig_equipo(lst_equipo, ((lc->con_lo)->SIG)->INDEX);
				if (NULL == eq) 
					ringerreur(ERR_INTERNE_EQ, NULL, NULL);

				c1 = c2 = lc->coord; /* segment col */
				p2 = lc->deport->piste; /* piste pour deport coeur */
				p1 = 0;

				/* ------------------------------------------------------------------ */
				/* Allocation d'un faux segment au cas ou il existe des deports coeur */
				/* pour empecher de router dans la piste reservee au deport           */
				/* ------------------------------------------------------------------ */

				if (segment_libre(deb, fin, 1L, 1L, face, SEG_PISTE, segpiste, niveaupiste))
					alloue_etchaine_segment(deb, fin, 1L, 1L, face, SEG_PISTE, segpiste, lc->largeur,
					     niveaupiste, FAUX_SEG, AVEC_VIA, eq);

				/* ---------------------------------------------------- */
				/* Allocations de faux segments si largeur non standard */
				/* ---------------------------------------------------- */

				if (p2 > 1) {
					for (piste = 2; piste <= (p2 + (lc->largeur / 2 + pitch) / pitch); piste++)
						alloue_etchaine_segment(c1, c2, piste, piste, face, SEG_PISTE, segpiste,
						    largmetalpiste, niveaupiste, FAUX_SEG, AVEC_VIA, eq);
				}

				if (segment_libre(c1, c2, p1, p2, face, SEG_COL, segcol, lc->layer))
					alloue_etchaine_segment(c1, c2, p1, p2, face, SEG_COL, segcol, lc->largeur, lc->layer,
					     VRAI_SEG, AVEC_VIA, eq);
				else 
					ringerreur(ERR_SEGDEPORT, (void * )lc, NULL);
			}
			lc = lc->suiv;
		}
	} /* fin du  for */
}
