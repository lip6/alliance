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
/*    Fichier : bigvia.c                                                    */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Fred                                le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

#include "bigvia.h"

/*-----------------------------------------------------------------------------*/
/* #define BV_VIA_VIA 4  must be even, whatever!                               */
/* #define BV_VIASIZE 3 design rule for equipotential vias                     */
/*                                                                             */
/* this creates a mega-via, the best way we can do it, I hope                  */
/* the envelop of the bigvia is calculated, and then its center is placed      */
/* as close as possible of the x, y coordinates given as arguments.            */
/*-----------------------------------------------------------------------------*/
void
bigvia_ring(char l1, char l2, phfig_list* f, long x, long y, long dx, long dy)
{
	int	i, j;
	long	stepx, stepy, xv, yv, dxv, dyv;

        if (((l1==ALU1)&&(l2==ALU2))
	||  ((l2==ALU1)&&(l1==ALU2))) {
		addphvia(f, CONT_VIA, x, y, dx, dy, (char *)NULL);
	} else
	if (((l1==ALU2)&&(l2==ALU3))
	||  ((l2==ALU2)&&(l1==ALU3))) {
		addphvia(f, CONT_VIA2, x, y, dx, dy, (char *)NULL);
        } else {
		fflush(stdout);
		fprintf(stderr, "*** mbk error ***\n bigvia impossible :");
		EXIT(1);
	}
return;

	if (dx < 0 || dy < 0) {
		fflush(stdout);
		fprintf(stderr, "*** mbk error ***\n bigvia impossible :");
		fprintf(stderr, " negative values dx = %ld, dy = %ld\n", dx, dy);
		fprintf(stderr, "in figure '%s' at (%ld, %ld)\n", f->NAME, x, y);
		EXIT(1);
	}

	/* -------------------------------------------------------------------- */
	/* euclidian division :                                                 */
	/* dividing and then multipliing doesen't give the initial value.       */
	/* calculate the center of the bunch of vias and adjust it to the given */
	/* x, y coordinates.                                                    */
	/* -------------------------------------------------------------------- */

	stepx = (dx - BV_VIASIZE * SCALE_X) / (BV_VIA_VIA * SCALE_X);
	stepy = (dy - BV_VIASIZE * SCALE_X) / (BV_VIA_VIA * SCALE_X);
	dxv = stepx * BV_VIA_VIA * SCALE_X;
	dyv = stepy * BV_VIA_VIA * SCALE_X;
	xv = x - dxv / 2;
	yv = y - dyv / 2;

	/* ------------- */
	/* draw the vias */
	/* ------------- */

	for (i = 0; i <= stepx; i++)
		for (j = 0; j <= stepy; j++) {
			bigvias++;

			if (mode_debug) {

				/* ------------------------------------------------------------ */
				/* new rules for disk space : just a ring of via for big pates. */
				/* ------------------------------------------------------------ */

				if (i == 0 || i == stepx || j == 0 || j == stepy) {
				        if (((l1==ALU1)&&(l2==ALU2))
					||  ((l2==ALU1)&&(l1==ALU2))) {
				  	  addphvia(f, CONT_VIA, 
							xv + i * BV_VIA_VIA * SCALE_X, yv + j * BV_VIA_VIA * SCALE_X,0,0, NULL);
					} else
				        if (((l1==ALU2)&&(l2==ALU3))
					||  ((l2==ALU2)&&(l1==ALU3))) {
				  	  addphvia(f, CONT_VIA2, 
							xv + i * BV_VIA_VIA * SCALE_X, yv + j * BV_VIA_VIA * SCALE_X,0,0, NULL);
					}
					ringvias++;
				}
			} /* fin du if mode_debug */ else {   /* pave complet de vias ! */
				if (i == 0 || i == stepx || j == 0 || j == stepy) 
					ringvias++;

				if (((l1==ALU1)&&(l2==ALU2))
				||  ((l2==ALU1)&&(l1==ALU2))) {
				  addphvia(f, CONT_VIA,
						xv + i * BV_VIA_VIA * SCALE_X, yv + j * BV_VIA_VIA * SCALE_X,0,0, NULL);
				} else
				if (((l1==ALU2)&&(l2==ALU3))
				||  ((l2==ALU2)&&(l1==ALU3))) {
				  addphvia(f, CONT_VIA2,
						xv + i * BV_VIA_VIA * SCALE_X, yv + j * BV_VIA_VIA * SCALE_X,0,0, NULL);
				}    
			}
		}

	/* ----------------------------------------------------------------------------------------- */
	/* put segments around the vias : this warranties that no notches may appear on the borders. */
	/* ----------------------------------------------------------------------------------------- */

	if (stepx)
		addphseg(f, l1, BV_VIASIZE * SCALE_X, xv, yv, xv + stepx * BV_VIA_VIA * SCALE_X, yv, NULL);
	if (stepx || stepy)
		addphseg(f, l1, BV_VIASIZE * SCALE_X, xv + stepx * BV_VIA_VIA * SCALE_X, yv, xv + stepx * BV_VIA_VIA *
		    SCALE_X, yv + stepy * BV_VIA_VIA * SCALE_X, NULL);
	if (stepx || stepy)
		addphseg(f, l1, BV_VIASIZE * SCALE_X, xv + stepx * BV_VIA_VIA * SCALE_X, yv + stepy * BV_VIA_VIA * SCALE_X,
		     xv, yv + stepy * BV_VIA_VIA * SCALE_X, NULL);
	if (stepy)
		addphseg(f, l1, BV_VIASIZE * SCALE_X, xv, yv, xv, yv + stepy * BV_VIA_VIA * SCALE_X, NULL);
	if (stepx)
		addphseg(f, l2, BV_VIASIZE * SCALE_X, xv, yv, xv + stepx * BV_VIA_VIA * SCALE_X, yv, NULL);
	if (stepx || stepy)
		addphseg(f, l2, BV_VIASIZE * SCALE_X, xv + stepx * BV_VIA_VIA * SCALE_X, yv, xv + stepx * BV_VIA_VIA *
		    SCALE_X, yv + stepy * BV_VIA_VIA * SCALE_X, NULL);
	if (stepx || stepy)
		addphseg(f, l2, BV_VIASIZE * SCALE_X, xv + stepx * BV_VIA_VIA * SCALE_X, yv + stepy * BV_VIA_VIA * SCALE_X,
		     					xv, yv + stepy * BV_VIA_VIA * SCALE_X, NULL);
	if (stepy)
		addphseg(f, l2, BV_VIASIZE * SCALE_X, xv, yv, xv, yv + stepy * BV_VIA_VIA * SCALE_X, NULL);

	/* ----------------------------------------------------------------------------------------------------------------------------- */
	/* fill the leftover space : we use computed via coordinates, and substract one in order not to take care of even or odd values. */
	/* ----------------------------------------------------------------------------------------------------------------------------- */

	if (dxv && dyv)
		addphseg(f, l2, dyv - 1 * SCALE_X, xv, yv + dyv / 2, xv + dxv, yv + dyv / 2, NULL);
	if (dxv && dyv)
		addphseg(f, l1, dyv - 1 * SCALE_X, xv, yv + dyv / 2, xv + dxv, yv + dyv / 2, NULL);
}
