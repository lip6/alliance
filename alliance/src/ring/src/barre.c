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
/*    Fichier : barre.c                                                     */
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
/* RING    26 mai 92                                                                 */
/* Fichier BARRE.C                                                                   */
/* Fichier utilise pour construire les barres de plots avec genlib                   */
/* Interdiction d'utiliser les super structures de mbk ou autres car genlib          */
/* generateur de macro. Type de donnees autorises long et char* exclusivement        */
/*-----------------------------------------------------------------------------------*/

#include <stdio.h>
#include <genlib.h>

#include "barre.h"

extern char	mode_debug;

/*-----------------------------------------------------------------------------------*/
/* cette fonction ajoute un transforme un nom de signal vectorise mbk                */
/* avec des espaces en nom de signal GENLIB avec des crochets [ ]                    */
/*-----------------------------------------------------------------------------------*/

static char	*busname(name)
char	*name;
{
static char	buffer[255];
char	*s, *t;
char	one = 1;

	if (!name)
		return NULL;
	s = name;
	t = buffer;
	while (*s) {
		if (*s == ' ')
		{
			if (one) {
				*t++ = '[';
				s++;
				one = 0;
			} else {
				*t++ = ']';
				*t++ = '[';
				s++;
			}
		}
		if (*s == '_' && !one) { /* was SEPAR and not / */
			*t++ = ']';
			one = 1;
		}
		*t++ = *s++;
	}
	if (!one)
		*t++ = ']';
	*t = '\0';
	return buffer;
}

/*-----------------------------------------------------------------------------------*/

void definir_fig_ph(char *nomfig)
{
	if (mode_debug) 
		printf("defphfig %s\n", nomfig);
	GENLIB_DEF_PHFIG(nomfig);

}

/*-----------------------------------------------------------------------------------*/
/* Attention utiliser definir_reference_inst() avant d'utiliser cette fonction       */
/*-----------------------------------------------------------------------------------*/

void abouteright_plot_fig(char *nomfig, char *nominst)
{

	if (mode_debug) 
		printf("abouteright_plot_fig %s inst %s\n", nomfig, nominst);

	GENLIB_PLACE_RIGHT(nomfig, nominst, NOSYM);

}

/*-----------------------------------------------------------------------------------*/
/* Attention utiliser definir_reference_inst() avant d'utiliser cette fonction       */
/*-----------------------------------------------------------------------------------*/

void abouteleft_plot_fig(char *nomfig, char *nominst)
{

	if (mode_debug) 
		printf("abouteleft_plot_fig %s inst %s\n", nomfig, nominst);

	GENLIB_PLACE_LEFT(nomfig, nominst, NOSYM);

}

/*-----------------------------------------------------------------------------------*/

void definir_reference_inst(char *name)
{

	if (mode_debug) 
		printf("definir reference instance %s\n", name);

	GENLIB_DEF_PHINS(name);
}

/*-----------------------------------------------------------------------------------*/

void definir_ab_fig()
{
	if (mode_debug) 
		printf("def ab 0000\n");

	GENLIB_DEF_AB(0L, 0L, 0L, 0L);
}

void definir_ab_fig2()
{
	if (mode_debug) 
		printf("def ab2\n");

	GENLIB_DEF_AB(-100L, -100L, 100L, 100L);
}

/*-----------------------------------------------------------------------------------*/

void sauver_fig_ph()
{
	if (mode_debug) 
		printf("sauve phfig\n");

	GENLIB_SAVE_PHFIG();
}

/*-----------------------------------------------------------------------------------*/

void remonter_consud_instph(char *nominst)
{
	if (mode_debug) 
		printf("Remonte con sud de l'inst %s\n", nominst);

	GENLIB_COPY_UP_ALL_CON(SOUTH, nominst, YES);
}

/*-----------------------------------------------------------------------------------*/

void remonter_connord_instph(char *nominst)
{
	if (mode_debug) 
		printf("Remonte con nord de l'inst %s\n", nominst);

	GENLIB_COPY_UP_ALL_CON(NORTH, nominst, YES);
}

/*-----------------------------------------------------------------------------------*/

void placer_instph(char *nomfig, char *nominst, char symetrie,
	long x, long y)
{

	if (mode_debug) 
		printf("placer instance nomfig %s inst %s symetrie %c  x %ld  y %ld\n", nomfig, nominst, symetrie, x, y);

	GENLIB_PLACE(nomfig, nominst, symetrie, x, y);
}

/*-----------------------------------------------------------------------------------*/

void placer_segph(char layer, long width, char *nameseg,
	long x1, long y1, long x2, long y2)
{

	if (mode_debug)
	{
	    if (nameseg)
		printf("placer segment layer %d width %ld name %s x1 %ld y1 %ld x2 %ld y2 %ld\n", (int)layer, width, nameseg,
			x1, y1, x2, y2);
	    else
		printf("placer segment layer %d width %ld x1 %ld y1 %ld x2 %ld y2 %ld\n", (int)layer, width,
			x1, y1, x2, y2);
	}

	GENLIB_PHSEG(layer, width, busname(nameseg), x1, y1, x2, y2);
}

/*----------------------------------------------------------------------------------*/
void miseaplat_instph(char *insname, char cheminom)
{

	if (mode_debug) 
		printf("mise a plat de l'instance %s et cheminon %d\n", insname, (int)cheminom);

	GENLIB_FLATTEN_PHFIG(insname, cheminom);
}

/*----------------------------------------------------------------------------------*/

void remonter_uncon_instph(long index, char *locname, char *insname, char *newname)
{

	if (mode_debug) 
		printf("remontee du con %s index %ld de l'instance %s et newname %s\n", locname, index, insname, newname);

	locname = namealloc( busname(locname));
	newname = namealloc( busname(newname));
	GENLIB_COPY_UP_CON(index, locname, insname, newname);
}

/*----------------------------------------------------------------------------*/

void poser_wire1(char layer, long width, char* ins1, char *con1,
	long index1, char* ins2, char* con2,long index2)
{

	if (mode_debug) 
		printf("wire1 layer %d width %ld ins1 %s con1 %s index1 %ld ins2 %s con2 %s index2 %ld\n", (int) layer, width,
		     ins1, con1, index1, ins2, con2, index2);

	GENLIB_WIRE1(layer, width, ins1, con1, index1, ins2, con2, index2);
}

/*--------------------------------------------------------------------------------------*/

void poser_wire2(char layer, long width, char* ins1, char* con1,
	long index1, char* ins2, char* con2, long index2, long x, long y)
{

	if (mode_debug) 
		printf("wire2 layer %d width %ld ins1 %s con1 %s index1 %ld ins2 %s con2 %s index2 %ld x %ld y %ld\n", (int) layer,
		     width, ins1, con1, index1, ins2, con2, index2, x, y);

	GENLIB_WIRE2(layer, width, ins1, con1, index1, ins2, con2, index2, x, y);
}
