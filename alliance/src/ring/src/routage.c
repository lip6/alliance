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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ring router                                                 */
/*    Fichier : routage.c                                                   */
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
/* RING 22 juillet 92                   ROUTAGE.C                                    */
/* routage des equipotentielles sauf alim (deja fait)                                */
/*-----------------------------------------------------------------------------------*/

#include "routage.h"
#include "placement.h"
#include "sesame.h"

/*-----------------------------------------------------------------------------------*/
/* Remplit les ptype listes de projetes des equipotentielles, par calcul de leur     */
/* abscisse curviligne, puis reordonnancement des coordonnees pour garder le bipoint */
/* le plus grand                                                                     */
/*-----------------------------------------------------------------------------------*/

void remplit_eq_curv(LST_EQUIPO lst_equipo, COEUR lecoeur)
{
	LST_EQUIPO equipo_vdd, equipo_vss;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

	while (NULL != lst_equipo) {
		if ((lst_equipo != equipo_vdd) && (lst_equipo != equipo_vss)) {
			calcul_eq_proj(lst_equipo, lecoeur);
			ordonne_eq_proj(lst_equipo, lecoeur);
		}

		lst_equipo = lst_equipo->suiv;
	}

}

/*-----------------------------------------------------------------------------------*/
/* Remplit la ptype liste de coordonnees avec valeur de l'abscisse curviligne, en    */
/* triant ds l'ordre croissant.                                                      */
/*-----------------------------------------------------------------------------------*/

void calcul_eq_proj(LST_EQUIPO equipo, COEUR lecoeur)
{
	LST_PSEUDO_CON con;
	chain_list     * lst_con;
	PT_COORDONNEES coord;
	long	valcurv;

	lst_con = equipo->lst_con;
	(equipo->cour).lst_projetes = NULL;

	while (lst_con) {
		con = (LST_PSEUDO_CON) lst_con->DATA;
		if (NULL != con->deport) 
			coord = con->deport;
		else 
			coord = con->coord;

		valcurv = calcul_curv(con, lecoeur);
		insere_valcurv(valcurv, coord, equipo);

		lst_con = lst_con->NEXT;
	}
}

/*-----------------------------------------------------------------------------------*/
/* Calcul de l'abscisse curviligne en fonctions des coordonnees et de la face        */
/*-----------------------------------------------------------------------------------*/

long calcul_curv(LST_PSEUDO_CON con, COEUR lecoeur)
{
	long	valcurv = 0;
	PT_COORDONNEES coord;

	if (NULL != con->deport) 
		coord = con->deport;
	else 
		coord = con->coord;

	switch (con->face) {
	case OUEST :
		valcurv = coord->yabs - lecoeur.coord.yabs;
		break;

	case NORD  :
		valcurv = lecoeur.height + coord->xabs - lecoeur.coord.xabs;
		break;

	case EST   :
		valcurv = lecoeur.height + lecoeur.width + lecoeur.height - (coord->yabs - lecoeur.coord.yabs);
		break;

	case SUD   :
		valcurv = lecoeur.height + lecoeur.width + lecoeur.height + lecoeur.width - (coord->xabs - lecoeur.coord.xabs);
		break;
	}

	if (mode_debug) 
		printf("Valcurv %ld con %s face %d \n", valcurv, con->nom_con, con->face);

	return(valcurv);
}

/*-------------------------------------------------------------------------------------*/
/* cette procedure insere les coord et sa valeur curviligne dans la ptype list en      */
/* triant par ordre croissant                                                          */
/*-------------------------------------------------------------------------------------*/

void insere_valcurv(long valcurv, PT_COORDONNEES coord, LST_EQUIPO equipo)
{

	ptype_list * lst_proj, *prec_proj;

	prec_proj = lst_proj = (equipo->cour).lst_projetes;

	if (NULL == lst_proj)
		(equipo->cour).lst_projetes = addptype(lst_proj, valcurv, (void * ) coord);

	else	 {
		while (NULL != lst_proj) {
			if (valcurv <= lst_proj->TYPE) {
				if (lst_proj == (equipo->cour).lst_projetes) /* tete de liste */
					(equipo->cour).lst_projetes = addptype(lst_proj, valcurv, (void * ) coord);
				else
				 {
					lst_proj = addptype(lst_proj, valcurv, (void * ) coord);
					prec_proj->NEXT = lst_proj;
				}
				break;
			}

			prec_proj = lst_proj;
			lst_proj  = lst_proj->NEXT;
		}

		if (NULL == lst_proj) {
			lst_proj = addptype(lst_proj, valcurv, (void * ) coord);
			prec_proj->NEXT = lst_proj;
		}
	} /* fin du else */

}

/*------------------------------------------------------------------------------------*/
/* Cette procedure ordonne les coordonnees des projetes en prenant pour depart et fin */
/* le bipoint le plus grand , ceci en utilisant les distances curvilignes calculees   */
/* precedemment.                                                                      */
/*------------------------------------------------------------------------------------*/

void ordonne_eq_proj(LST_EQUIPO equipo, COEUR lecoeur)
{

	ptype_list * lst_proj = NULL, *prec_proj = NULL, *prem = NULL, *deux = NULL, *precprem = NULL;
	long	delta = 0, deltamax = 0;

	prec_proj = lst_proj = (equipo->cour).lst_projetes;

	if (mode_debug) 
		printf("Equipo no %ld\n", equipo->index);

	while ((NULL != lst_proj) && (NULL != lst_proj->NEXT)) {
		delta = (lst_proj->NEXT)->TYPE - lst_proj->TYPE;
		if (mode_debug) 
			printf("Delta %5ld\n", delta);
		if (delta > deltamax) { 
			prem = lst_proj; 
			precprem = prec_proj; 
			deux = lst_proj->NEXT; 
			deltamax = delta;
		}

		prec_proj = lst_proj;
		lst_proj = lst_proj->NEXT;
	}

	/* -------------------------------------------------- */
	/* dist curviligne entre le 1er et le dernier projete */
	/* -------------------------------------------------- */

	if (prec_proj == (equipo->cour).lst_projetes) /* tete liste */
		prec_proj = ((equipo->cour).lst_projetes)->NEXT;

	/* ---------------------------------------- */
	/* prec_proj pointe sur la derniere cellule */
	/* ---------------------------------------- */

	delta = 2 * lecoeur.height + 2 * lecoeur.width + ((equipo->cour).lst_projetes)->TYPE - prec_proj->TYPE;
	;
	if (mode_debug) 
		printf("Delta entre 1er et der  %5ld deltamax %5ld\n", delta, deltamax);

	if (delta < deltamax)  /* sinon deja ordonne on bouge pas (>=) */ {

		/* ----------------------------------------------------------------------------- */
		/* on remplit la valeur perimetre de la couronne , servira pour trier les equipo */
		/* ----------------------------------------------------------------------------- */

		(equipo->cour).perimetre = lecoeur.width * 2 + lecoeur.height * 2 - deltamax;

		if (mode_debug) 
			printf("Deltamax trouve %ld\n", deltamax);
		lst_proj = deux;
		while (NULL != deux->NEXT) 
			deux = deux->NEXT; /* on va en fin de liste */

		/* ----------------------------------------------- */
		/* et on chaine avec le debut de la liste suivante */
		/* ----------------------------------------------- */

		deux->NEXT = (equipo->cour).lst_projetes;

		prem->NEXT = NULL; /* fin de la liste */

		(equipo->cour).lst_projetes = lst_proj; /* tete de liste */

	}  else	 {

		/* ----------------------------------------------------------------------------- */
		/* on remplit la valeur perimetre de la couronne , servira pour trier les equipo */
		/* ----------------------------------------------------------------------------- */

		(equipo->cour).perimetre = lecoeur.width * 2 + lecoeur.height * 2 - delta;
	}

}

/*-------------------------------------------------------------------------------------*/
/* Cette procedure route toutes les equipotentielles (sauf alim) en 2 phases:          */
/*	- routage de la couronne associee                                              */
/*	- routage des colonnes a partir des connecteurs de l'equipotentielle           */
/*-------------------------------------------------------------------------------------*/

void route_equipo(long piste_infalim[NB_FACES], LST_EQUIPO lst_equipo,
	COEUR lecoeur, LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES])
{

	long	piste_cour[NB_FACES];

	LST_EQUIPO equipo_vdd, equipo_vss;

	recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

	while (NULL != lst_equipo) {
		if ((lst_equipo != equipo_vdd) && (lst_equipo != equipo_vss)) {
			if (mode_debug) 
				printf("Routage des equipotentielles (sauf alim) %ld \n", lst_equipo->index);
			construit_couronne(piste_cour, piste_infalim, lst_equipo, lecoeur, segx_occ, segy_occ);
			poseseg_colonnes(piste_cour, lst_equipo, segx_occ, segy_occ);
		}

		lst_equipo = lst_equipo->suiv;
	}

}

/*------------------------------------------------------------------------------------*/
/* Cette procedure contruit la couronne associee a l'equipotentielle a partir de la   */
/* liste des projetes construite.                                                     */
/*------------------------------------------------------------------------------------*/

void construit_couronne(long piste_cour[NB_FACES], long	piste_infalim[NB_FACES],
	LST_EQUIPO  equipo, COEUR lecoeur, LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES])
{

	PT_COORDONNEES coin = NULL, nouveaucoin = NULL, debut_coor;
	LST_PSEUDO_CON prec_con = NULL, con = NULL;
	LST_SEGMENT    * segpiste = NULL;
	long	pistealim = 0, largmetalpiste = 0, pistedebut = 0;
	ptype_list     * lst_proj, *prec_proj;
	int	change_face = 0, firstface = 1, segpiste_enplus = 0;
	char	metalpiste = 0;

	lst_proj = (equipo->cour).lst_projetes;

	while (NULL != lst_proj) {
		if (!firstface)

			if (segpiste_enplus) {
				debut_coor = nouveaucoin; 
				segpiste_enplus = 0;
				switch (prec_con->face) /* mise a jour de la fin du segment supplementaire */ {
				case SUD: 
					nouveaucoin->xabs = lecoeur.coord.xabs - piste_cour[OUEST] * pitch;
					break;
				case NORD: 
					nouveaucoin->xabs = lecoeur.coord.xabs + lecoeur.width + piste_cour[EST] * pitch;
					break;
				case EST: 
					nouveaucoin->yabs = lecoeur.coord.yabs - piste_cour[SUD] * pitch;
					break;
				case OUEST: 
					nouveaucoin->yabs = lecoeur.coord.xabs + lecoeur.height + piste_cour[NORD] * pitch;
					break;
				}
				if (mode_debug) 
					printf("construit cour face %ld  debutcoorx %ld nouveaucoin x %ld debpt %ld coinpt%ld\n",
					     (long)prec_con->face, debut_coor->xabs, nouveaucoin->xabs, (long)debut_coor, (long)nouveaucoin);
			} /* fin du if */

			else		 {
				debut_coor = coin;

				switch (prec_con->face) /* mise a jour des coord du coin pour la face suivante */ {
				case SUD:
					coin->yabs = lecoeur.coord.yabs - pistedebut * pitch;
					break;
				case NORD:
					coin->yabs = lecoeur.height + lecoeur.coord.yabs + pistedebut * pitch;
					break;
				case EST:
					coin->xabs = lecoeur.width + lecoeur.coord.xabs + pistedebut * pitch;
					break;
				case OUEST:
					coin->xabs = lecoeur.coord.xabs - pistedebut * pitch;
					break;
				}
			}  /* fin du else */

		else
			debut_coor = (PT_COORDONNEES)lst_proj->DATA; /* c'est la 1ere fois */

		prec_proj = lst_proj ;
		prec_con = (LST_PSEUDO_CON)((PT_COORDONNEES)lst_proj->DATA)->proprio;
		lst_proj = lst_proj->NEXT;
		change_face = 0;

		if (mode_debug) 
			printf("prec-con-pt %ld\n", (long)prec_con);

		while ((NULL != lst_proj) && (!change_face)) {
			con = (LST_PSEUDO_CON)((PT_COORDONNEES)lst_proj->DATA)->proprio;
			if (mode_debug) 
				printf("con-pt %ld\n", (long)con);
			change_face = prec_con->face != con->face;
			if (!change_face) { 
				prec_con = con; 
				prec_proj = lst_proj; 
				lst_proj = lst_proj->NEXT;
			}
		}

		switch (prec_con->face) {
		case SUD :
		case NORD:
			segpiste = segx_occ;
			metalpiste = xmetal;
			largmetalpiste = xmetal_width;
			break;
		case EST  :
		case OUEST:
			segpiste = segy_occ;
			metalpiste = ymetal;
			largmetalpiste = ymetal_width;
			break;
		}

		if (change_face) {

			alloue_coord(0L, 0, &coin);
			switch (prec_con->face) { 

				/* ---------------------------------------------------------------------- */
				/* Au plus  un seul segment piste est rajoute                             */
				/* les valeurs 10 ajoutees ou enlevees sont totalement arbitraires, mais  */
				/* sont destinees pour les connecteurs placees en face des coins du coeur */
				/* sinon mauvais tri des coordonnes ds alloue_etchainesegment             */ 
				/* cas tres rare mais POSSIBLE !                                          */
				/* ---------------------------------------------------------------------- */

			case SUD :
				coin->xabs = lecoeur.coord.xabs - 10;
				if (con->face != OUEST) { 
					segpiste_enplus = 1;
					ajout_segmentpiste(coin, &nouveaucoin, OUEST, piste_cour, piste_infalim, equipo,
					    lecoeur, segx_occ, segy_occ); 
				}
				break;
			case NORD:
				coin->xabs = lecoeur.width + lecoeur.coord.xabs + 10;
				if (con->face != EST) { 
					segpiste_enplus = 1;
					ajout_segmentpiste(coin, &nouveaucoin, EST, piste_cour, piste_infalim, equipo, lecoeur,
					     segx_occ, segy_occ); 
				}
				break;
			case EST  :
				coin->yabs = lecoeur.coord.yabs - 10;
				if (con->face != SUD) { 
					segpiste_enplus = 1;
					ajout_segmentpiste(coin, &nouveaucoin, SUD, piste_cour, piste_infalim, equipo, lecoeur,
					     segx_occ, segy_occ); 
				}
				break;
			case OUEST:
				coin->yabs = lecoeur.height + lecoeur.coord.yabs + 10;
				if (con->face != NORD) { 
					segpiste_enplus = 1;
					ajout_segmentpiste(coin, &nouveaucoin, NORD, piste_cour, piste_infalim, equipo, lecoeur,
					     segx_occ, segy_occ); 
				}
				break;
			}
		} /* fin du if */  else 
			coin = (PT_COORDONNEES)prec_proj->DATA;

		pistedebut = 1; 
		pistealim = piste_infalim[prec_con->face];

		while ((pistedebut != pistealim) && (!segment_libre(debut_coor, coin, pistedebut, pistedebut, prec_con->face,
		     SEG_PISTE, segpiste, metalpiste)))
			pistedebut++;

		if (pistedebut == pistealim) 
			ringerreur(ERR_NOPISTE, &(prec_con->face), NULL);
		piste_cour[prec_con->face] = pistedebut;

		alloue_etchaine_segment(debut_coor, coin, pistedebut, pistedebut, prec_con->face, SEG_PISTE, segpiste, largmetalpiste,
		     metalpiste, VRAI_SEG, AVEC_VIA, equipo);

		if (segpiste_enplus) {
			switch (prec_con->face) /* mise a jour du debut segment supplementaire */ {
			case SUD: 
				coin->yabs = lecoeur.coord.yabs - pistedebut * pitch;
				coin->xabs = lecoeur.coord.xabs - piste_cour[OUEST] * pitch;
				break;
			case NORD: 
				coin->yabs = lecoeur.height + lecoeur.coord.yabs + pistedebut * pitch;
				coin->xabs = lecoeur.coord.xabs + lecoeur.width + piste_cour[EST] * pitch;
				break;
			case EST: 
				coin->xabs = lecoeur.width + lecoeur.coord.xabs + pistedebut * pitch;
				coin->yabs = lecoeur.coord.yabs - piste_cour[SUD] * pitch;
				break;
			case OUEST: 
				coin->xabs = lecoeur.coord.xabs - pistedebut * pitch;
				coin->yabs = lecoeur.coord.yabs + lecoeur.height + piste_cour[NORD] * pitch;
				break;
			}
		} /* fin du if */

		if (firstface) 
			firstface = 0 ;
		else /* mise a jour des coordonnees du precedent coin de la face precedente */		 {
			switch (prec_con->face) {
			case SUD: 
				debut_coor->yabs = lecoeur.coord.yabs - pistedebut * pitch;
				break;
			case NORD: 
				debut_coor->yabs = lecoeur.height + lecoeur.coord.yabs + pistedebut * pitch;
				break;
			case EST: 
				debut_coor->xabs = lecoeur.width + lecoeur.coord.xabs + pistedebut * pitch;
				break;
			case OUEST: 
				debut_coor->xabs = lecoeur.coord.xabs - pistedebut * pitch;
				break;
			}
		} /* fin du else */

		if (mode_debug) 
			printf("construit cour face %ld debutcoorx %ld coin x %ld debpt %ld coinpt%ld\n", (long)prec_con->face,
			     debut_coor->xabs, coin->xabs, (long)debut_coor, (long)coin);

	} /* fin du 1er while */

}

/*----------------------------------------------------------------------------------*/
/* Cette procedure tire un segment de type seg_piste pour completer une couronne.   */
/* Rq: on ajoutera au maximum un segment de ce type par couronne, sinon ca veut dire*/
/* qu'il existe un plus court chemin dans l'autre sens.                             */
/*----------------------------------------------------------------------------------*/

void ajout_segmentpiste(PT_COORDONNEES coin, PT_COORDONNEES *nouveaucoin,
	int face, long	piste_cour[NB_FACES], long piste_infalim[NB_FACES],
	LST_EQUIPO  equipo, COEUR lecoeur, LST_SEGMENT segx_occ[NB_FACES],
	LST_SEGMENT segy_occ[NB_FACES])
{
	LST_SEGMENT    * segpiste = NULL;
	long	pistealim = 0, largmetalpiste = 0, pistedebut = 0;
	char	metalpiste = 0;
	alloue_coord(0L, 0, nouveaucoin);

	switch (face) {

		/* ---------------------------------------------------------------------- */
		/* les valeurs 10 ajoutees ou enlevees sont totalement arbitraires, mais  */
		/* sont destinees pour les connecteurs placees en face des coins du coeur */
		/* sinon mauvais tri des coordonnes ds alloue_etchainesegment             */
		/* cas tres rare mais POSSIBLE !                                          */
		/* ---------------------------------------------------------------------- */

	case SUD :
		coin->xabs = lecoeur.coord.xabs + lecoeur.width + 10;
		(*nouveaucoin)->xabs = lecoeur.coord.xabs - 10;
		segpiste = segx_occ;
		metalpiste = xmetal;
		largmetalpiste = xmetal_width;
		break;
	case NORD:
		coin->xabs = lecoeur.coord.xabs - 10;
		(*nouveaucoin)->xabs = lecoeur.width + lecoeur.coord.xabs + 10;
		segpiste = segx_occ;
		metalpiste = xmetal;
		largmetalpiste = xmetal_width;
		break;
	case EST  :
		coin->yabs = lecoeur.coord.yabs + lecoeur.height + 10;
		(*nouveaucoin)->yabs = lecoeur.coord.yabs - 10;
		segpiste = segy_occ;
		metalpiste = ymetal;
		largmetalpiste = ymetal_width;
		break;
	case OUEST:
		coin->yabs = lecoeur.coord.yabs - 10;
		(*nouveaucoin)->yabs = lecoeur.height + lecoeur.coord.yabs + 10;
		segpiste = segy_occ;
		metalpiste = ymetal;
		largmetalpiste = ymetal_width;
		break;
	}

	pistedebut = 1; 
	pistealim = piste_infalim[face];

	while ((pistedebut != pistealim) && (!segment_libre(coin, (*nouveaucoin), pistedebut, pistedebut, face, SEG_PISTE,
	     segpiste, metalpiste)))
		pistedebut++;

	if (pistedebut == pistealim) 
		ringerreur(ERR_NOPISTE, &face, NULL);
	piste_cour[face] = pistedebut;

	alloue_etchaine_segment(coin, (*nouveaucoin), pistedebut, pistedebut, face, SEG_PISTE, segpiste, largmetalpiste,
	    metalpiste, VRAI_SEG, AVEC_VIA, equipo);

}

/*--------------------------------------------------------------------------------------*/
/* Une fois la couronne partielle de l'equipotentielle posee, on tire les colonnes      */
/* issues des connecteurs, no de piste sont dans piste_cour                             */
/* Modif Verif de la largeur metal colonne pour eviter bug avec pad12 et pad15          */
/*--------------------------------------------------------------------------------------*/

void poseseg_colonnes(long piste_cour[NB_FACES], LST_EQUIPO equipo,
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES])
{

	PT_COORDONNEES debut_coor;
	LST_PSEUDO_CON con;
	LST_SEGMENT    * segcol = NULL;
	long	largeurmin = 0, largeurvia = 0, largeurseg = 0;
	char	metalcol = 0;
	ptype_list     * lst_proj;

	lst_proj = (equipo->cour).lst_projetes;

	while (NULL != lst_proj) {
		con = (LST_PSEUDO_CON) ((PT_COORDONNEES)lst_proj->DATA)->proprio;
		if (NULL == con) 
			ringerreur(ERR_INTERNE_EQ, NULL, NULL);
		debut_coor = (PT_COORDONNEES) lst_proj->DATA;

		switch (con->face) {
		case SUD :
		case NORD:
			segcol = segy_occ;
			metalcol = ymetal;
			largeurmin = ymetal_width;
			largeurvia = ymetal_wvia;
			break;
		case EST  :
		case OUEST:
			segcol = segx_occ;
			metalcol = xmetal;
			largeurvia = xmetal_wvia;
			largeurmin = xmetal_width;
			break;
		}

		if (segment_libre(debut_coor, debut_coor, debut_coor->piste, piste_cour[con->face], con->face, SEG_COL, segcol,
		     metalcol)) {
			if (con->largeur > largeurvia) 
				largeurseg = largeurvia;
			else if (con->largeur < largeurmin) 
				largeurseg = largeurmin;
			else 
				largeurseg = con->largeur;

			alloue_etchaine_segment(debut_coor, debut_coor, debut_coor->piste, piste_cour[con->face], con->face,
			     SEG_COL, segcol, largeurseg, metalcol, VRAI_SEG, AVEC_VIA, equipo);
		}  else 
			ringerreur(ERR_NOCOL, (void * )con, NULL);

		lst_proj = lst_proj->NEXT;
	}

}

/*----------------------------------------------------------------------------------*/
/* Cette procedure ordonne les equipotentielles selon les criteres:                 */
/*      -- les alim en tete (pour acces plus rapide                                 */
/*      -- les coeur-coeur par ordre curviligne decroissant                         */
/*      -- les coeur-plot  par ordre curviligne decroissant                         */
/*      -- les plot-plot   par ordre curviligne decroissant                         */
/*      -- les autres equipotentielles restantes                                    */
/*----------------------------------------------------------------------------------*/

void ordonne_equipotentielles(LST_EQUIPO *lst_equipo)
{
	LST_EQUIPO eq, lst_eq = *lst_equipo;
	chain_list * lst_alim = NULL, *lst_coeurcoeur = NULL, *lst_coeurplot = NULL;
	chain_list * lst_plotplot = NULL, *lst_reste = NULL, *lst, *prec_lst;
	long	perimetre;

	/* eclatement en 4 liste triees */

	while (NULL != lst_eq) {

		switch (lst_eq->type) {
		case ALIM: 
			lst_alim = addchain(lst_alim, (void * )lst_eq);
			break;
		case COEUR_COEUR:
			perimetre = (lst_eq->cour).perimetre;
			prec_lst = lst = lst_coeurcoeur;

			if (NULL == lst) 
				lst_coeurcoeur = addchain(lst_coeurcoeur, (void * )lst_eq);
			else
			 {
				while (NULL != lst) {
					if (perimetre > (((LST_EQUIPO)lst->DATA)->cour).perimetre) {
						if (lst == lst_coeurcoeur) /* tete de liste */
							lst_coeurcoeur = addchain(lst_coeurcoeur, (void * )lst_eq);
						else
						 {
							lst = addchain(lst, (void * ) lst_eq);
							prec_lst->NEXT = lst;
						}
						break;
					}
					prec_lst = lst;
					lst = lst->NEXT;
				}
				if (NULL == lst) {
					lst = addchain(lst, (void * ) lst_eq);
					prec_lst->NEXT = lst;
				}
			} /* fin du else */
			break;

		case COEUR_PLOT:
			perimetre = (lst_eq->cour).perimetre;
			prec_lst = lst = lst_coeurplot;

			if (NULL == lst) 
				lst_coeurplot = addchain(lst_coeurplot, (void * )lst_eq);
			else
			 {
				while (NULL != lst) {
					if (perimetre > (((LST_EQUIPO)lst->DATA)->cour).perimetre) {
						if (lst == lst_coeurplot) /* tete de liste */
							lst_coeurplot = addchain(lst_coeurplot, (void * )lst_eq);
						else
						 {
							lst = addchain(lst, (void * ) lst_eq);
							prec_lst->NEXT = lst;
						}
						break;
					}
					prec_lst = lst;
					lst = lst->NEXT;
				}
				if (NULL == lst) {
					lst = addchain(lst, (void * ) lst_eq);
					prec_lst->NEXT = lst;
				}
			} /* fin du else */
			break;

		case PLOT_PLOT:
			perimetre = (lst_eq->cour).perimetre;
			prec_lst = lst = lst_plotplot;

			if (NULL == lst) 
				lst_plotplot = addchain(lst_plotplot, (void * )lst_eq);
			else
			 {
				while (NULL != lst) {
					if (perimetre > (((LST_EQUIPO)lst->DATA)->cour).perimetre) {
						if (lst == lst_plotplot) /* tete de liste */
							lst_plotplot = addchain(lst_plotplot, (void * )lst_eq);
						else
						 {
							lst = addchain(lst, (void * ) lst_eq);
							prec_lst->NEXT = lst;
						}
						break;
					}
					prec_lst = lst;
					lst = lst->NEXT;
				}
				if (NULL == lst) {
					lst = addchain(lst, (void * ) lst_eq);
					prec_lst->NEXT = lst;
				}
			} /* fin du else */
			break;

		default: 
			lst_reste = addchain(lst_reste, (void * )lst_reste);
			break;
		}

		lst_eq = lst_eq->suiv;
	}

	/* --------------------------------------------------- */
	/* fusion des listes triees selon les criteres choisis */
	/* --------------------------------------------------- */

	if ((NULL == lst_alim) || (NULL == lst_alim->NEXT)) 
		ringerreur(ERR_INTERNE_EQ, NULL, NULL);

	eq = lst_eq = (LST_EQUIPO) lst_alim->DATA;

	lst = lst_alim->NEXT;
	eq->suiv = (LST_EQUIPO) lst->DATA;
	eq = eq->suiv;

	lst = lst_coeurcoeur;

	while (NULL != lst) {
		eq->suiv = (LST_EQUIPO) lst->DATA;
		eq = eq->suiv;
		lst = lst->NEXT;
	}

	lst = lst_coeurplot;

	while (NULL != lst) {
		eq->suiv = (LST_EQUIPO) lst->DATA;
		eq = eq->suiv;
		lst = lst->NEXT;
	}

	lst = lst_plotplot;

	while (NULL != lst) {
		eq->suiv = (LST_EQUIPO) lst->DATA;
		eq = eq->suiv;
		lst = lst->NEXT;
	}

	lst = lst_reste;

	while (NULL != lst) {
		eq->suiv = (LST_EQUIPO) lst->DATA;
		eq = eq->suiv;
		lst = lst->NEXT;
	}

	eq->suiv = NULL;

	(*lst_equipo) = lst_eq;

}
