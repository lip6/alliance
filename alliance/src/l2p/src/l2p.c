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
/*    Produit :  (R)ectangle (D)ata (S)tructure (2)to (P)ost(S)cript driver */
/*    Fichier :  rds2ps.c                                                   */
/*                                                                          */
/*    (c) copyright 1991-94 Laboratoire MASI equipe CAO & VLSI              */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Auteur(s) :  Gilles-Eric DESCAMPS                 le : 23/03/1993     */
/*                                                                          */
/*    Modifie par :  Gilles-Eric DESCAMPS               le : 30/01/1994     */
/*    Modifie par :  Etienne LACOUME                    le : 02/03/1995     */
/*                                                                          */
/****************************************************************************/

#ident "$Id: l2p.c,v 1.4 2002/09/30 16:20:42 czo Exp $"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mut.h"
#include "mph.h"
#include "mpu.h"
#include "rds.h"
#include "rwi.h"
#include "rut.h"
#include "rps_inc.h"
#include "rtl.h"
#include "rpr.h"
#include "rfm.h"

extern void rps_print_man ();
extern struct rps_param p;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void print_usage (progname)
char *progname;
{
	printf ("usage: %s [options] cellname\n", progname);
	printf ("               where cellname is the name of the figure ");
	printf ("without extensions,\n");
	printf ("or:    %s -help\n", progname);
	printf ("               in order to get the full list of ");
	printf ("options.\n\n");
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ParseArgs (argc, argv, param)
int     argc;
char    **argv;
struct rps_param	*param;
{
	int	a,stext;
	rps_pge	fakepage;

	strcpy (param->cmdline, "");
	for (a=0 ; a<argc ; a++){
		strcat (param->cmdline, " ");
		strcat (param->cmdline, argv[a]);
	};
	if (argc == 2) {
		if (!strcmp(argv[1], "-givebwdict")) {
			fakepage.nb_bytes_written = 0;
			fakepage.nb_last_written = 0;
			fakepage.f = stdout;
			fakepage.param = param;
			rps_print_dict_bw (&fakepage);
			exit (0);
		};
		if (!strcmp(argv[1], "-givecolordict")) {
			fakepage.nb_bytes_written = 0;
			fakepage.nb_last_written = 0;
			fakepage.f = stdout;
			fakepage.param = param;
			rps_print_dict_color (&fakepage);
			exit (0);
		};
		if (!strcmp(argv[1], "-help")) {
			rps_print_man ();
			exit (0);
		};
	};
	alliancebanner ("L2P",VERSION,
		"(L)ayout to(2) (P)aper: A tool for PostScript plots",
		"1994",ALLIANCE_VERSION);
	if (argc == 1) {
		print_usage (argv[0]);
		exit (1);
	};
	param->figname=namealloc (argv[argc-1]);
	param->draw_w = 740;
	param->draw_h = 1094;
	param->paper_x = 50;
	param->paper_y = 25;
	param->paper_w = 740;
	param->paper_h = 1119;
	param->verbose = RDS_YES;
	param->a3tray = RDS_NO;
	param->couleur = RDS_NO;
	param->norectname = RDS_NO;
	param->noconame = RDS_NO;
	param->norefname = RDS_NO;
	param->noinstname = RDS_NO;
	param->nosegname = RDS_NO;
	param->textsize = 8;
	param->manualfeed = RDS_NO;
	param->flatten = RDS_NO;
	param->header = RDS_YES;
	param->landscape = RDS_NO;
	param->real = RDS_NO;
	param->resol = 100;
	param->nbx_pages = 0;
	param->nby_pages = 0;
	param->tops = 0.0;
	strcpy (param->papername,"a4");
	param->headpages = NULL;
	strcpy (param->dictfilename, "");
	for (a=1 ; a<(argc-1) ; a++) {
		if (argv[a][0] != '-') {
			print_usage (argv[0]);
			exit (1);
		};
		if (!strcmp(argv[a], "-fA3")) {
			param->a3tray = RDS_YES;
			param->draw_w = 1100;
			param->draw_h = 1565;
			param->paper_x = 50;
			param->paper_y = 25;
			param->paper_w = 1100;
			param->paper_h = 1590;
			continue;
		};
                if (!strcmp(argv[a], "-fLETTER")) {
                        param->draw_w = 770;
                        param->draw_h = 1025;
                        param->paper_x = 50;
                        param->paper_y = 25;
                        param->paper_w = 770;
                        param->paper_h = 1050;
                        continue;
                };
                if (!strcmp(argv[a], "-fLEGAL")) {
                        param->draw_w = 770;
                        param->draw_h = 1325;
                        param->paper_x = 50;
                        param->paper_y = 25;
                        param->paper_w = 770;
                        param->paper_h = 1350;
                        continue;
                };
		if (!strcmp(argv[a], "-color")) {
			param->couleur = RDS_YES;
			continue;
		};
		if (!strncmp(argv[a], "-nrname",7)) {
			param->norectname = RDS_YES;
			continue;
		};
		if (!strncmp(argv[a], "-ncname",7)) {
			param->noconame = RDS_YES;
			continue;
		};
		if (!strncmp(argv[a], "-nrfname",8)) {
			param->norefname = RDS_YES;
			continue;
		};
		if (!strncmp(argv[a], "-niname",7)) {
			param->noinstname = RDS_YES;
			continue;
		};
		if (!strncmp(argv[a], "-nsname",7)) {
			param->nosegname = RDS_YES;
			continue;
		};
		if (!strncmp(argv[a],"-tsize=",7)) {
			if (sscanf(&(argv[a] [7]),"%d",&stext) !=1) {
				print_usage(argv[a]);
				exit(1);
			};
			switch(stext) {
				case 6:
					param->textsize=6;
				break;
				case 10:
					param->textsize=10;
				break;
				case 12:
					param->textsize=12;
				break;
				case 14:
					param->textsize=14;
				break;
				default:
					param->textsize=8;
				break;
			};
			continue;
		};
		if (!strncmp(argv[a],"-mfeed",6)) {
			param->manualfeed = RDS_YES;
			continue;
		};
		if (!strncmp(argv[a], "-drawingsize=", 13)) {
			if (sscanf (&(argv[a][13]),"%ldx%ld",
			    &param->draw_w,
			    &param->draw_h) != 2) {
				print_usage (argv[0]);
				exit (1);
			};
			continue;
		};
		if (!strcmp(argv[a], "-landscape")) {
			param->landscape = RDS_YES;
			continue;
		};
		if (!strcmp(argv[a], "-noheader")) {
			param->header = RDS_NO;
			continue;
		};
		if (!strncmp(argv[a], "-papersize=", 11)) {
			if (sscanf (&(argv[a][11]),"%ldx%ldx%ldx%ld",
			    &param->paper_x, &param->paper_y,
			    &param->paper_w, &param->paper_h) != 4) {
				print_usage (argv[0]);
				exit (1);
			};
			continue;
		};
		if (!strncmp(argv[a], "-pages=", 7)) {
			if (sscanf (&(argv[a][7]),"%ldx%ld",
			    &param->nbx_pages,
			    &param->nby_pages) != 2) {
				print_usage (argv[0]);
				exit (1);
			};
			continue;
		};
		if (!strcmp(argv[a], "-real")) {
			param->real = RDS_YES;
			continue;
		};
		if (!strcmp(argv[a], "-rflattentrans")) {
			param->flatten = RDS_YES;
			param->tilcatal = NO;
			continue;
		};
		if (!strcmp(argv[a], "-rflattencatal")) {
			param->flatten = RDS_YES;
			param->tilcatal = YES;
			continue;
		};
		if (!strncmp(argv[a], "-resol=", 7)) {
			if (sscanf (&(argv[a][7]),"%ld",
			    &param->resol) != 1) {
				print_usage (argv[0]);
				exit (1);
			};
			continue;
		};
		if (!(strcmp(argv[a], "-rotate")
		      && strcmp(argv[a], "-landscape"))) {
			param->landscape = RDS_YES;
			continue;
		};
		if (!strncmp(argv[a], "-scale=", 7)) {
			if (sscanf (&(argv[a][7]),"%lf",
			    &param->tops) != 1) {
				print_usage (argv[0]);
				exit (1);
			};
			continue;
		};
		if (!strncmp(argv[a], "-usedict=", 9)) {
			if (sscanf (&(argv[a][9]),"%s",
			    param->dictfilename) != 1) {
				print_usage (argv[0]);
				exit (1);
			};
			continue;
		};
		print_usage (argv[0]);
		exit (1);
	};
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int main (argc, argv)
int  argc;
char **argv;
{
	phfig_list	*mbk_fig;

/* Initialisations */
	mbkenv ();
	rdsenv ();
	ParseArgs (argc, argv, &p);
	printf("\n- Loading technology file : %s\n", RDS_TECHNO_NAME);
	loadrdsparam();

/* Chargement de la figure */
	if (p.real) {
		printf ("- Loading rds figure: %s.%s\n", p.figname, RDS_IN);
		fflush (stdout);
		p.fig = getrdsfig (p.figname, 'A', 0);
		if (p.flatten) {
			printf ("- Flattening rds figure: %s\n", p.figname);
			fflush (stdout);
			rflattenrdsfig (p.fig, YES, p.tilcatal);
		};
	} else {
		printf ("- Loading mbk figure: %s.%s\n", p.figname, IN_PH);
		fflush (stdout);
		mbk_fig = getphfig (p.figname, 'A');
		if (p.flatten) {
			printf ("- Flattening mbk figure: %s\n", p.figname);
			fflush (stdout);
			rflattenphfig (mbk_fig, YES, p.tilcatal);
		};
		figmbkrds (mbk_fig, 0, 0);
		p.fig = HEAD_RDSFIG;
	};
		
/* Sauvegarde en PostScript */
	printf ("- Saving PostScript figure: %s\n", p.figname);
	fflush (stdout);
	saveps (&p);

/* Tout est bien qui finit bien */
	printf ("- All done.\n");
	return (0);
}
/****************************************************************************/
