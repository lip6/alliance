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
/*    Fichier : ring2.c                                                     */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Olivier Van Hautte                  le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le : 13/11/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

#ident "@(#)ring 2.8 9/11/94 ALLIANCE 3.0 pad ring router"
/*----------------------------------------------------------------------------------- */
/* RING    27 avril 92                      RING2.C                                   */
/* Programme principal: declarations des variables (globales et locales)              */
/*----------------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "param.h"
#include "sesame.h"
#include "lirevues.h"
#include "lireplace.h"
#include "placement.h"
#include "deport.h"
#include "routalim.h"
#include "routage.h"
#include "compress.h"
#include "posercircuit.h"
#include "stat.h"

/*----------------------------------------------------------------------------------- */
/* Variables globales */
char	mode_debug;                      /* si debug alors mode_debug=1, 0 sinon */
char	mode_stat;                       /* si stat alors mode_stat=1, 0 sinon */
char	*nomfic_stat;                    /* nom du fichier stat */
char	xmetal;                          /* metal horizontal, faces est et Ouest */
char	ymetal;                          /* metal vertical, faces Nord et Sud */
long	xmetal_width, ymetal_width;      /* largeur des metaux x, y */
long	xmetal_dmin, ymetal_dmin;        /* dist minimum entre 2 meme metaux */
long	xmetal_wvia, ymetal_wvia;        /* taille des contacts pour chaque metal */
long	pitch;                           /* pitch a calculer */
long	lambda;                          /* lambda (=SCALE_X) */
long	taille_via;                      /* taille du via a calculer */
long	vdd_width;                       /* largeur couronnes d'alim */
long	vss_width;
char  *eq_vdd;
char  *eq_vss;


char	*nom_fic_param;                  /* nom du fichier parametre */

chain_list *nom_plot[NB_FACES];          /* liste pour l'analyseur des noms de plots */
                                         /* contiendra aussi les instances logiques de plots */
chain_list *liste_width;                 /* liste pour l'analyseur des noms
                                         et largeurs des connecteurs speciaux */

long	bigvias = 0, ringvias = 0;       /* nombres de vias poses en total par bigvia 
                                         ou en couronne (mode debug). car pb versatil
                                         si bcp de vias (fichier .cif enorme) */

/*----------------------------------------------------------------------------------- */

int
main (argc, argv)
int	argc;
char	**argv;
{

	/* declarations des variables locales */

	BARRE_PLOTS    tab_plots[NB_FACES];      /* nbfaces barres de plots */
	LST_PSEUDO_CON tab_coeur[NB_FACES];      /* nbfaces listes de connecteurs du coeur */

	chain_list     * liste_plotsph;          /* liste de pointeurs sur les modeles physiques
	                                            des plots */
	chain_list     * lst_conestouest;        /* liste des con estouest alim */

	int            nb_inst_plots;            /* nombres d'instances de plots rencontrees */

	GRILLE         tab_grilles[NB_FACES];    /* une grille symbolique par face */

	LST_EQUIPO     eq_interne;               /* liste des equipo. internes (coeur/plots) */

	LST_SEGMENT    segx_occ[NB_FACES];       /* liste des segments occupes */
	LST_SEGMENT    segy_occ[NB_FACES];       /* liste des segments occupes */

	long	       piste_supalim[NB_FACES];  /* No des pistes alim couronnes inf et sup */

	long	       piste_infalim[NB_FACES];
	long	       tabpiste[NB_FACES];       /* No des 1eres pistes libres en partant 
                                                    de la barre de plots, sert pour les couronnes d'alim */

	char	       * nom_circuit_lo, *nom_circuit_ph; /* nom du circuit logique et resultat (physique) */

	COEUR          lecoeur;                  /* structure contenant les informations du coeur */

	lofig_list     * circuit_lo;             /* pointeur sur la figure logique du coeur */
	phfig_list     * circuit_ph;             /* pointeur sur la figure physique du coeur */

	char	       * coursup, *courinf;      /* nom des couronnes d'alim */
	long	       firstlibre[NB_FACES], nblibres[NB_FACES]; /* tab des 1eres pistes libres et nbre de piste */

	/* ---------------------------------------------------------------------------------- */
	/* PHASE 1                                                                            */
	/* lecture et verification du nombre de parametres                                    */
	/* ---------------------------------------------------------------------------------- */

	lecture_param(argc, argv, &nom_circuit_lo, &nom_circuit_ph, &nom_fic_param);
	banner();

	/* ---------------------------------------------------------------------------------- */
	/* PHASE 2                                                                            */
	/* lecture vues structurelles circuit et coeur                                        */
	/* ---------------------------------------------------------------------------------- */

	fprintf(stderr, "\to reading netlists, layout views of core and pads.\n");
	lecture_vues(nom_circuit_lo, &lecoeur, &circuit_lo , &liste_plotsph, &nb_inst_plots, &lst_conestouest);

	/* ---------------------------------------------------------------------------------- */
	/* PHASE 3                                                                            */
	/* ---------------------------------------------------------------------------------- */

	fprintf(stderr, "\to reading file of parameters, including the placements of pads.\n");
	lecture_fic(nom_fic_param, circuit_lo, lecoeur, nb_inst_plots);
	if (mode_debug) 
		affic_plotwidth();

	fprintf(stderr, "\to making equipotential list.\n");
	fabrique_equipo(nom_plot, lecoeur, &eq_interne, liste_plotsph);
	if (mode_debug) 
		affic_equipo(eq_interne);

	/* ---------------------------------------------------------------------------------- */
	/* verification de la liste des equipotentielles internes                             */
	/* ---------------------------------------------------------------------------------- */
	verif_eq_interne(eq_interne, liste_plotsph);

	/* ---------------------------------------------------------------------------------- */
	/* PHASE 4                                                                            */
	/* ---------------------------------------------------------------------------------- */

	fprintf(stderr, "\to making the first placement of pads.\n");
	fabrique_barre_plots(tab_plots, lecoeur, nom_plot, liste_plotsph, lst_conestouest);

	remplir_tabcoeur(tab_coeur, lecoeur, eq_interne);

	if (mode_debug) {
		printf("afficher tab_coeur\n"); 
		affic_tabcoeur(tab_coeur);
	}

	fprintf(stderr, "\to filling data internal structures.\n");
	remplir_tabplots(tab_plots, eq_interne);
	if (mode_debug) 
		affic_tabplots(tab_plots);

	if (mode_debug) 
		printf("*** (larg plot alim) vdd_width: %ld vss_width: %ld\n", vdd_width, vss_width);

	fprintf(stderr, "\to reading the connectors positions of the core.\n");
	fabrique_grille_primaire(tab_coeur, tab_grilles);
	if (mode_debug) 
		affic_grille(tab_grilles);

	fprintf(stderr, "\to computing the best placement of the pads.\n");
	place_et_cout_barreplot(tab_coeur, tab_plots, lecoeur, eq_interne);
	maj_coordplots(tab_plots, eq_interne);

	fprintf(stderr, "\to reading the connectors positions of the pads.\n");
	fabrique_grille_finale(tab_plots, tab_coeur, lecoeur, eq_interne, tab_grilles);
	if (mode_debug) 
		affic_grille(tab_grilles);
	if (mode_debug) 
		affic_tabplots(tab_plots);

	if (mode_debug) 
		affic_grille(tab_grilles);

	/* ---------------------------------------------------------------------------------- */
	/* PHASE 5                                                                            */
	/* ---------------------------------------------------------------------------------- */

	fprintf(stderr, "\to routing deportation of connectors.\n");
	attribuer_couronne_alim(eq_interne, &coursup, &courinf);

	/* ---------------------------------------------------------------------------------- */
	/* initialisations de segx-occ[] et segy_occ[]                                        */
	/* on alloue exceptionnellement des faux segments pour deport plot alim !             */
	/* ---------------------------------------------------------------------------------- */

	deport_alim(eq_interne, tab_plots, coursup, courinf, segx_occ, segy_occ);

	deport_connecteurs(tab_plots, tab_coeur, lecoeur, tab_grilles);
	if (mode_debug) 
		affic_equipo(eq_interne);

	pose_segdeport(tab_plots, tab_coeur, lecoeur, eq_interne, segx_occ, segy_occ);

	fprintf(stderr, "\to routing supply tracks.\n");

	/* ---------------------------------------------------------------------------------- */
	/* tabpiste va contenir les no des 1eres pistes libres en partant des plots           */
	/* qui vont servir pour poser les couronnes d'alim                                    */
	/* ---------------------------------------------------------------------------------- */

	pose_couralim(coursup, piste_supalim, piste_infalim, eq_interne, lecoeur, tab_plots, segx_occ, segy_occ, tabpiste);

	/* ---------------------------------------------------------------------------------- */
	/* On peut se servir de tabpiste pour poser des faux segments, au cas ou on           */
	/* tire une alim ds le mauvais layer, ceci pour eviter des croisements                */
	/* (courts-circuits) avec les deports eventuels de plots                              */
	/* ---------------------------------------------------------------------------------- */

	tire_etdeport_alim(coursup, piste_supalim, piste_infalim, eq_interne, tab_plots, segx_occ, segy_occ, tabpiste);

	if (mode_debug) 
		printf("*** vdd_width: %ld vss_width: %ld\n", vdd_width, vss_width);
	if (mode_debug) 
		affic_equipo(eq_interne);

	/* ---------------------------------------------------------------------------------- */
	/* PHASE 6                                                                            */
	/* ---------------------------------------------------------------------------------- */

	fprintf(stderr, "\to routing equipotentials.\n");
	remplit_eq_curv(eq_interne, lecoeur);
	ordonne_equipotentielles(&eq_interne);

	route_equipo(piste_infalim, eq_interne, lecoeur, segx_occ, segy_occ);

	if (mode_debug) 
		affic_equipo(eq_interne);

	if (mode_debug) {
		printf("------------------------------------------------------------------\n\n\n");
		printf("SEGMENT YMETAL\n");
		affic_listeseg(segy_occ);
		printf("SEGMENT XMETAL\n");
		affic_listeseg(segx_occ);
	}

	/* ---------------------------------------------------------------------------------- */
	/* PHASE 7                                                                            */
	/* ---------------------------------------------------------------------------------- */

	fprintf(stderr, "\to compressing channels.\n");
	calcul_nbpiste_libre(tab_plots, lecoeur, segx_occ, segy_occ, nblibres, firstlibre);
	compression_canaux(tab_plots, lecoeur, segx_occ, segy_occ, nblibres, firstlibre);

	/* ---------------------------------------------------------------------------------- */
	/* PHASE 8                                                                            */
	/* ---------------------------------------------------------------------------------- */

	fprintf(stderr, "\to saving in MBK data structure.\n");
	poser_blocs(&circuit_ph, nom_circuit_ph, tab_plots, lecoeur);
	poser_segments(eq_interne, lecoeur, circuit_lo, circuit_ph);

	remonte_con_circuit(nom_plot, liste_plotsph, tab_plots, lecoeur, circuit_lo, circuit_ph, nom_circuit_ph, lst_conestouest);

	if (mode_stat) 
		finir_stat(eq_interne);

	fprintf(stderr, "lucky, no error.\n");
	exit(0); /* fin correcte du routeur */

}

