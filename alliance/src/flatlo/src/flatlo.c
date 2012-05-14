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

#ident "$Id: flatlo.c,v 1.4 2012/05/14 14:20:10 alliance Exp $"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mut.h"
#include "mlo.h"


usage(progname)
char *progname;
{
	fprintf(stderr, "usage : %s logical_figure instance output_name\n",
				progname);
	fputs("        flatten the instance in the logical_figure\n", stderr);
	fprintf(stderr, "usage : %s -r logical_figure output_name\n", progname);
	fputs("        flatten logical_figure to the catalog\n", stderr);
	fprintf(stderr, "usage : %s -t logical_figure output_name\n", progname);
	fputs("        flatten logical_figure to the transistor\n", stderr);
	EXIT(1);
}

main(argc, argv)
int argc;
char **argv;
{
lofig_list *p;


	mbkenv();
	alliancebanner ("flatLO", "1.4", "FLATen LOgical figure", "1990", ALLIANCE_VERSION);

	if (argc != 4)
		usage(argv[0]);

	if (!strcmp(argv[1], "-r"))
		rflattenlofig(p = getlofig(argv[2],'A'), YES, YES);
	else if (!strcmp(argv[1], "-t"))
		rflattenlofig(p = getlofig(argv[2],'A'), YES, NO);
	else
		flattenlofig(p = getlofig(argv[1], 'A'), argv[2], YES);

        p->NAME=argv[3];
	savelofig(p);
	exit(0);
}
