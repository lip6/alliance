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
/*    Fichier : status.c                                                     */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Olivier Van Hautte                  le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

/*--------------------------------------------------------------------------------*/
/* RING 28 aout 92                  STAT.C                                        */
/* Emission de statistiques sur les equipotentielles, un fichier de stat est cree */
/*--------------------------------------------------------------------------------*/

#include "stat.h"
#include "sesame.h"

extern long	bigvias, ringvias;

/*-----------------------------------------------------------------------------------*/
void remplit_stat(LST_SEGMENT seg, LST_EQUIPO eq)
{

	switch (seg->layer) {
	case ALU1:
		if (seg->c1 == seg->c2) {
			eq->status.lg_a1 += ((seg->piste2 - seg->piste1) * pitch / lambda);
			eq->status.su_a1 += (((seg->piste2 - seg->piste1) * pitch / lambda) * (seg->largeur / lambda));
		} else if ((seg->face == NORD) || (seg->face == SUD)) {
			eq->status.lg_a1 += ((seg->c2->xabs - seg->c1->xabs) / lambda);
			eq->status.su_a1 += (((seg->c2->xabs - seg->c1->xabs) / lambda) * (seg->largeur / lambda));
		}  else
		 {
			eq->status.lg_a1 += ((seg->c2->yabs - seg->c1->yabs) / lambda);
			eq->status.su_a1 += (((seg->c2->yabs - seg->c1->yabs) / lambda) * (seg->largeur / lambda));
		}
		break;

	case ALU2:
		if (seg->c1 == seg->c2) {
			eq->status.lg_a2 += ((seg->piste2 - seg->piste1) * pitch / lambda);
			eq->status.su_a2 += (((seg->piste2 - seg->piste1) * pitch / lambda) * (seg->largeur / lambda));
		} else if ((seg->face == NORD) || (seg->face == SUD)) {
			eq->status.lg_a2 += ((seg->c2->xabs - seg->c1->xabs) / lambda);
			eq->status.su_a2 += (((seg->c2->xabs - seg->c1->xabs) / lambda) * (seg->largeur / lambda));
		}  else
		 {
			eq->status.lg_a2 += ((seg->c2->yabs - seg->c1->yabs) / lambda);
			eq->status.su_a2 += (((seg->c2->yabs - seg->c1->yabs) / lambda) * (seg->largeur / lambda));
		}
		break;
	} /* fin switch */

}

void finir_stat(LST_EQUIPO eq)
{
	FILE   * ficstat;
	static long	totlga1, totlga2, totsua1, totsua2, totnbvias;

	if ((ficstat = mbkfopen(nomfic_stat, NULL, WRITE_TEXT)) == NULL) 
		ringerreur(ERR_FICSTAT, nomfic_stat, NULL);

	fprintf(ficstat, "#\t\t\t                            *** STATISTIC FILE < %s > ***\n\n\n", nomfic_stat);
	fprintf(ficstat, "\t\t\t                                  Equipotential list : \n\n");

	fprintf(ficstat, "       index      |          name           | length alu1 | length alu2 |   area    alu1 | area    alu2  | nb vias\n\n");

	fprintf(ficstat, "__________________________________________________________________________________________________________________\n");
	while (NULL != eq) {
		fprintf(ficstat, "        %3ld       |  %20s   |", eq->index, ((LST_PSEUDO_CON)eq->lst_con->DATA)->nom_con);

		fprintf(ficstat, "   %7ld   |   %7ld   |   %10ld   |   %10ld  | %7ld\n\n", eq->status.lg_a1, eq->status.lg_a2,
		     eq->status.su_a1, eq->status.su_a2, eq->status.nb_vias);

		if (((LST_PSEUDO_CON)eq->lst_con->DATA)->coeur_plot == PLOT_CON)
			fprintf(ficstat, "                   (%20s)\n", ((loins_list * )((LST_PSEUDO_CON)eq->lst_con->DATA)->con_lo->ROOT)->INSNAME);
		fprintf(ficstat, "__________________________________________________________________________________________________________________\n");
		totlga1 += eq->status.lg_a1;
		totlga2 += eq->status.lg_a2;
		totsua1 += eq->status.su_a1;
		totsua2 += eq->status.su_a2;
		totnbvias += eq->status.nb_vias;
		eq = eq->suiv;
	}

	fprintf(ficstat, "\n\nTotal length alu1  :   %10ld (lambdas)\n", totlga1);
	fprintf(ficstat, "Total length alu2  :   %10ld (lambdas)\n", totlga2);
	fprintf(ficstat, "Total area alu1    :   %10ld (lambdas * lambdas)\n", totsua1);
	fprintf(ficstat, "Total area alu2    :   %10ld (lambdas * lambdas)\n", totsua2);
	fprintf(ficstat, "Total of vias      :   %10ld\n", totnbvias);

	fclose(ficstat);

}

void maj_stat_vias(LST_EQUIPO eq, long ancr, long ancp, long r, long p)
{

	if (mode_debug)
		eq->status.nb_vias += (r - ancr); /* maj couronne nb vias */
	else
		eq->status.nb_vias += (p - ancp); /* maj pave nb vias */
}
