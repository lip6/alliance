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
/*    Fichier : distance.c                                                  */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Olivier Van Hautte                  le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

/*--------------------------------------------------------------------------*/
/* RING 19 aout 92                   DISTANCE.C                             */
/*--------------------------------------------------------------------------*/

#include "distance.h"

#include <stdio.h>
#include <string.h>

/*-----------------------------------------------------------------------------------*/
/* Calcul de la distance entre deux connecteurs                                      */
/* Si un des connecteurs n'est pas de largeur standard, on calcul la distance reelle.*/
/* A ce moment on prend en compte l'encombrement d'un via                            */
/* S'ils sont de largeur standard alors la distance est la diff des coordonnees      */
/*-----------------------------------------------------------------------------------*/

long distance_con_con(long x1, long y1, long w1, char l1,
	long x2, long y2, long w2, char l2, int face)
{
	long	dist, xa, ya, wa;
	long	largeurmin, dminmetalmetal;
	char	la;

	switch (face) {
	case NORD:
	case SUD :

		/* ------------------------------------------------- */
		/* on garde dans x1 ,y1 les plus petites coordonnees */
		/* ------------------------------------------------- */

		if (x2 < x1) { 
			xa = x1;
			ya = y1;
			wa = w1;
			la = l1; 
			x1 = x2; 
			y1 = y2; 
			w1 = w2; 
			l1 = l2;
			x2 = xa; 
			y2 = ya; 
			w2 = wa; 
			l2 = la;
		}

		dminmetalmetal = ymetal_dmin;

		if ((l1 != ymetal) || (l2 != ymetal)) {
			if (ymetal_width < xmetal_width) 
				largeurmin = ymetal_width;
			else 
				largeurmin = xmetal_width;
		} else
		 {
			largeurmin = ymetal_width;
		}

		if ((w1 > largeurmin) || (w2 > largeurmin)) {
			dist = x2 - x1 - w2 / 2 - w1 / 2;
			if ((w1 < taille_via) && (w2 < taille_via)) 
				dist -= (taille_via - w1 / 2 - w2 / 2);
			else if ((w1 < taille_via) && !(w2 < taille_via)) 
				dist -= (taille_via / 2 - w1 / 2);
			else if (!(w1 < taille_via) && (w2 < taille_via)) 
				dist -= (taille_via / 2 - w2 / 2);
		} else 
			dist = x2 - x1;

		return(dist);

	case EST  :
	case OUEST:

		/* ------------------------------------------------- */
		/* on garde dans x1 ,y1 les plus petites coordonnees */
		/* ------------------------------------------------- */

		if (y2 < y1) { 
			xa = x1;
			ya = y1;
			wa = w1;
			la = l1; 
			x1 = x2; 
			y1 = y2; 
			w1 = w2; 
			l1 = l2;
			x2 = xa; 
			y2 = ya; 
			w2 = wa; 
			l2 = la;
		}

		dminmetalmetal = xmetal_dmin;

		if ((l1 != xmetal) || (l2 != xmetal)) {
			if (ymetal_width < xmetal_width) 
				largeurmin = ymetal_width;
			else 
				largeurmin = xmetal_width;
		} else
		 {
			largeurmin = xmetal_width;
		}

		if ((w1 > largeurmin) || (w2 > largeurmin)) {
			dist = y2 - y1 - w2 / 2 - w1 / 2;
			if ((w1 < taille_via) && (w2 < taille_via)) 
				dist -= (taille_via - w1 / 2 - w2 / 2);
			else if ((w1 < taille_via) && !(w2 < taille_via)) 
				dist -= (taille_via / 2 - w1 / 2);
			else if (!(w1 < taille_via) && (w2 < taille_via)) 
				dist -= (taille_via / 2 - w2 / 2);
		} else 
			dist = y2 - y1;

		return(dist);
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------*/
/* Cette fonction utilisee pour la fabrication de nouveaux pas de grille                   */
/* parcoure toutes les coordonnees precedentes correspondant a des pseudo-connecteurs      */
/* pour donner la distance minimum qu'il existe entre le connecteur                        */
/* et les connecteurs precedents d'alim. Ceci pour corriger un bug                         */
/* qui vient du fait que des alim peuvent etre en vis a vis et que                         */
/* si on calcule seulement la distance avec le connecteur precedent                        */
/* on ne prend peut etre pas en compte un connecteur d'alim voisin                         */
/* plus gros qui recouvre largement le connecteur precedent.                               */
/* on retourne une coordonnee qui est soit la precedente soit                              */
/* une autre qui rcouvre la coordonne precedente                                           */
/*-----------------------------------------------------------------------------------------*/

PT_COORDONNEES existe_con_precalim(long x2, long y2, long w2, char l2, int face,
	PT_COORDONNEES liste_coor, long distance)
{
	long	mindist = distance;
	long	dist;
	LST_PSEUDO_CON con;
	PT_COORDONNEES coordalim = NULL;

	if (mode_debug) 
		printf("Existeprecalim x %ld y %ld face %d ptcoor %ld\n\n", x2, y2, face, (long)liste_coor);

	switch (face) {
	case NORD:
	case SUD:
		while (NULL != liste_coor) {
			if ((NULL != liste_coor->proprio) && ((eq_vdd == ((LST_PSEUDO_CON)liste_coor->proprio)->nom_con)  ||
			    (eq_vss == ((LST_PSEUDO_CON)liste_coor->proprio)->nom_con)) && (liste_coor->xabs < x2)) {
				con = ((LST_PSEUDO_CON)liste_coor->proprio);
				dist = distance_con_con(liste_coor->xabs, liste_coor->yabs, con->largeur, con->layer, x2,
				     y2, w2, l2, face);

				if (dist < mindist) {
					mindist = dist; 
					coordalim = liste_coor;
				}
			}

			if (liste_coor->xabs >= x2) 
				break;
			else 
				liste_coor = liste_coor->suiv;
		}

		return(coordalim);

		break;

	case EST:
	case OUEST:
		while (NULL != liste_coor) {
			if ((NULL != liste_coor->proprio) && ((eq_vdd == ((LST_PSEUDO_CON)liste_coor->proprio)->nom_con)  ||
			    (eq_vss == ((LST_PSEUDO_CON)liste_coor->proprio)->nom_con)) && (liste_coor->yabs < y2)) {
				con = ((LST_PSEUDO_CON)liste_coor->proprio);
				dist = distance_con_con(liste_coor->xabs, liste_coor->yabs, con->largeur, con->layer, x2,
				     y2, w2, l2, face);

				if (dist < mindist) {
					mindist = dist; 
					coordalim = liste_coor;
				}
			}

			if (liste_coor->yabs >= y2) 
				break;
			else 
				liste_coor = liste_coor->suiv;
		}

		return(coordalim);
	}
	return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/* Cette fonction utilisee pour la fabrication de nouveaux pas de grille                   */
/* parcoure toutes les coordonnees suivantes correspondant a des pseudo-connecteurs        */
/* pour donner la distance minimum qu'il existe entre le connecteur                        */
/* et les connecteurs suivants d'alim. Ceci pour corriger un bug                           */
/* qui vient du fait que des alim peuvent etre en vis a vis et que                         */
/* si on calcule seulement la distance avec le connecteur suivant                          */
/* on ne prend peut etre pas en compte un connecteur d'alim voisin                         */
/* plus gros qui recouvre largement le connecteur precedent.                               */
/* on retourne une coordonnee soit la precedente soit une autre qui                        */
/* recouvre la precedente.                                                                 */
/*-----------------------------------------------------------------------------------------*/

PT_COORDONNEES existe_con_suiv(long x1, long y1, long w1, char l1,
	int face, PT_COORDONNEES liste_coor)
{
	long	mindist = 0;
	int	first = 1;
	long	dist;
	LST_PSEUDO_CON con;
	PT_COORDONNEES coordalim = NULL;

	if (mode_debug) 
		printf("Existesuivalim x %ld y %ld face %d ptcoor %ld\n\n", x1, y1, face, (long)liste_coor);

	switch (face) {
	case NORD:
	case SUD:
		while (NULL != liste_coor) {
			if (mode_debug) 
				printf("ptcoor %;d proprio %;d x %ld\n", (long) liste_coor, (long)liste_coor->proprio, liste_coor->xabs);

			if (NULL != liste_coor->proprio) {
				con = ((LST_PSEUDO_CON)liste_coor->proprio);
				dist = distance_con_con(x1, y1, w1, l1, liste_coor->xabs, liste_coor->yabs, con->largeur,
				     con->layer, face);

				if (first) { 
					mindist = dist; 
					first = 0; 
					coordalim = liste_coor;
				} else if (dist < mindist) {
					mindist = dist; 
					coordalim = liste_coor;
				}
				if (mode_debug) 
					printf("ptcoordalim %ld\n", (long) coordalim);
			}

			liste_coor = liste_coor->suiv;
		}

		return(coordalim);

		break;

	case EST:
	case OUEST:
		while (NULL != liste_coor) {
			if (mode_debug) 
				printf("ptcoor %ld proprio %ld x %ld\n", (long) liste_coor, (long)liste_coor->proprio, liste_coor->yabs);

			if (NULL != liste_coor->proprio) {
				con = ((LST_PSEUDO_CON)liste_coor->proprio);
				dist = distance_con_con(x1, y1, w1, l1, liste_coor->xabs, liste_coor->yabs, con->largeur,
				     con->layer, face);

				if (first) { 
					mindist = dist; 
					first = 0;
					coordalim = liste_coor;
				} else if (dist < mindist) {
					mindist = dist; 
					coordalim = liste_coor;
				}
				if (mode_debug) 
					printf("ptcoordalim %ld\n", (long) coordalim);
			}

			liste_coor = liste_coor->suiv;
		}

		return(coordalim);
	}
	return NULL;
}
