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
/*    Fichier : param.c                                                     */
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
/* RING 28 avril 92                  PARAM.C                                */
/* PHASE 1                                                                  */
/* Lecture des parametres et de l'environnement mbk                         */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>

#include "param.h"
#include "sesame.h"


/*-----------------------------------------------------------------------------------*/
/* lecture des arguments (ligne de commande), lecture envt mbk et                    */
/* initialisations des variables globales.                                           */
/*-----------------------------------------------------------------------------------*/

void lecture_param(int nbarg, char** tab, char** nom_circuit_lo,
	char** nom_circuit_ph, char** nom_fic_param)
{
	long	max1, max2, pitchalu1, pitchalu2;
	FILE* f_catal;

	/* ---------------------- */
	/* arguments du programme */
	/* ---------------------- */

	if ((nbarg < 3) || (nbarg > 5))
		ringerreur(ERR_ARGUMENT, NULL, NULL);

	*nom_circuit_lo = tab[1];
	*nom_circuit_ph = tab[2];

	*nom_fic_param = (char * ) mbkalloc ((unsigned int) strlen(*nom_circuit_lo) + 5);
	strcpy(*nom_fic_param, *nom_circuit_lo);
	strcat(*nom_fic_param, ".rin");

	mode_debug = 0;
	mode_stat = 0;

	if (nbarg == 4) {
		if (!strcmp(tab[3], "debug")) {
			mode_debug = 1;
			if (mode_debug) 
				fprintf(stderr, "\n\t*** Debug mode ***\n\n");
		}
		if (!strcmp(tab[3], "stat")) {
			mode_stat = 1;
			nomfic_stat = (char * ) mbkalloc ((unsigned int) strlen(*nom_circuit_ph) + 6);
			strcpy(nomfic_stat, *nom_circuit_ph);
			strcat(nomfic_stat, ".stat");
			if (mode_stat) 
				fprintf(stderr, "\n\t*** Statistic mode -> see the file < %s > ***\n\n", nomfic_stat);
		}
		if ((!mode_debug) && (!mode_stat)) 
			ringerreur(ERR_ARGUMENT, NULL, NULL);
	}

	if (nbarg == 5) {
		if ((!strcmp(tab[3], "debug")) || (!strcmp(tab[4], "debug"))) {
			mode_debug = 1;
			if (mode_debug) 
				fprintf(stderr, "\n\t*** Debug mode ***\n\n");
		}
		if ((!strcmp(tab[3], "stat")) || (!strcmp(tab[4], "stat"))) {
			mode_stat = 1;
			nomfic_stat = (char * ) mbkalloc ((unsigned int) strlen(*nom_circuit_ph) + 6);
			strcpy(nomfic_stat, *nom_circuit_ph);
			strcat(nomfic_stat, ".stat");
			if (mode_stat) 
				fprintf(stderr, "\n\t*** Statistic mode -> see the file < %s > ***\n\n", nomfic_stat);
		}
		if (!(mode_debug && mode_stat)) 
			ringerreur(ERR_ARGUMENT, NULL, NULL);
	}

	if (mode_debug) 
		printf("nbarg %d\n", nbarg);
	if (mode_debug) 
		printf("circlo-%s-circph-%s-\n", *nom_circuit_lo, *nom_circuit_ph);
	if (mode_debug) 
		printf("fic-paramr-%s-\n", *nom_fic_param);

	/*---------------------------- INITIALISATIONS ----------------------------------------*/

	/* ---------------- */
	/* arguments de mbk */
	/* ---------------- */

	mbkenv();  /* choix de l'utilisateur mis dans des variables UNIX */
	/* on recupere (long) SCALE_X                         */

	if (mode_debug) 
		printf("CATALNAME  is %s\n", CATAL);

	if (NULL == (f_catal = mbkfopen(CATAL, NULL, READ_TEXT))) 
		ringerreur(ERR_CATAL, CATAL, NULL);

	fclose(f_catal);

	/* ----------------------------------------------------------------- */
	/* Affectation des variables globales representnat les equipo d'alim */
	/* ----------------------------------------------------------------- */

	eq_vdd = namealloc(VDD);
	eq_vss = namealloc(VSS);

#ifdef COMMENT
	/* ------------------------------------------- */
	/* Directions privilegiees de metaux inversees */
	/* ------------------------------------------- */

	ymetal =       ALU1; /* Horizontal faces Est et Ouest */
	ymetal_width = WMIN_ALU1 * SCALE_X;
	ymetal_wvia =  WVIA_ALU1 * SCALE_X;
	ymetal_dmin =  DMIN_ALU1_ALU1 * SCALE_X;

	xmetal =       ALU2; /* Vertical faces Nord et Sud */
	xmetal_width = WMIN_ALU2 * SCALE_X;
	xmetal_wvia =  WVIA_ALU2 * SCALE_X;
	xmetal_dmin =  DMIN_ALU2_ALU2 * SCALE_X;
#endif

	xmetal =       ALU1; /* Horizontal faces Est et Ouest */
	xmetal_width = WMIN_ALU1 * SCALE_X;
	xmetal_wvia =  WVIA_ALU1 * SCALE_X;
	xmetal_dmin =  DMIN_ALU1_ALU1 * SCALE_X;

	ymetal =       ALU2; /* Vertical faces Nord et Sud */
	ymetal_width = WMIN_ALU2 * SCALE_X;
	ymetal_wvia =  WVIA_ALU2 * SCALE_X;
	ymetal_dmin =  DMIN_ALU2_ALU2 * SCALE_X;

	lambda      = 1 * SCALE_X;

	if (mode_debug) 
		printf("xmetal_width %ld ymetal_width %ld scale_x %ld\n", xmetal_width, ymetal_width, SCALE_X);
	if (mode_debug) 
		printf("xmetal_DMIN %ld ymetal_DMIN %ld\n", xmetal_dmin, ymetal_dmin);

	vdd_width = 0;
	vss_width = 0;

	/* pitcha1= dmina1a1 + max (wminalu1, wviaa1) */
	/* pitcha2= dmina2a2 + max (wminalu2, wviaa2) */

	if (WMIN_ALU1 > WVIA_ALU1) 
		max1 = WMIN_ALU1;
	else 
		max1 = WVIA_ALU1;

	pitchalu1 = DMIN_ALU1_ALU1 + max1;

	if (WMIN_ALU2 > WVIA_ALU2) 
		max2 = WMIN_ALU2;
	else 
		max2 = WVIA_ALU2;

	pitchalu2 = DMIN_ALU2_ALU2 + max2;

	if (mode_debug) 
		printf("Pitch alu1 %ld Pitch alu2 %ld\n", pitchalu1, pitchalu2);

	if (pitchalu1 > pitchalu2) 
		pitch = pitchalu1 * SCALE_X;
	else 
		pitch = pitchalu2 * SCALE_X;

	if (WVIA_ALU1 > WVIA_ALU2)
		taille_via = WVIA_ALU1 * SCALE_X;
	else 
		taille_via = WVIA_ALU2 * SCALE_X;

	if (mode_debug) 
		printf("pitch %ld taille_via %ld\n", pitch, taille_via);

}
