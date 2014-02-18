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

#ident "$Id: flatph.c,v 1.3 2012/05/14 14:20:10 alliance Exp $"
#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "mph.h"
#include "mpu.h"

void usage(progname)
char *progname;
{
	fprintf(stderr, "usage : %s physical_figure instance output_name\n", progname);
	fputs("        flatten the instance in the physical_figure\n", stderr);
	fprintf(stderr, "usage : %s -r physical_figure output_name\n", progname);
	fputs("        flatten physical_figure to the catalog\n", stderr);
	fprintf(stderr, "usage : %s -t physical_figure output_name\n", progname);
	fputs("        flatten physical_figure to the transistor\n", stderr);
	EXIT(1);
}

int main(argc, argv)
int argc;
char **argv;
{
phfig_list *p;

	mbkenv();
	alliancebanner ("flatph", "1.4", "FLATen PHysical figure", "1990", ALLIANCE_VERSION);

	if (argc != 4)
		usage(argv[0]);

	if (!strcmp(argv[1], "-r"))
		rflattenphfig(p = getphfig(argv[2],'A'), YES, YES);
	else if (!strcmp(argv[1], "-t"))
		rflattenphfig(p = getphfig(argv[2],'A'), YES, NO);
	else
		flattenphfig(p = getphfig(argv[1], 'A'), argv[2], YES);

        p->NAME=argv[3]; 
	savephfig(p);
	exit(0);
    return 0;
}
