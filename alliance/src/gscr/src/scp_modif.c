/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
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
 
/******************************************************************************/
/*                                                                            */
/*                    CAO & VLSI's cad tools chain Alliance                   */
/*                                                                            */
/*     Product  : Standard Cell Placer                                        */
/*     File     : scp_modif.c                                                 */
/*     Contents : fonctions de modification de la configuration               */
/*                et du calcul du cout associe                                */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   : Pierre Fedrichkine                 Date : 02/07/1993     */
/*     Modified by : Xavier Picat                       Date : 09/07/1993     */
/*                                                                            */
/******************************************************************************/
/*---------------------------------------------------------\
					Les includes 
\---------------------------------------------------------*/
#include <mut.h>
#include <mlo.h> 
#include <mph.h>
#include "scp_types.h"
#include "scp_modif.h"
#include "scp_inits.h"
#include "scp_main.h"
#include "scp_mbk2scp.h"

/*---------------------------------------------------------\
					Les variables locales
\---------------------------------------------------------*/
static cell_list *Cell1, *Cell2; /* Cellules selectionnees pour la modification */
static long       DeltaWidth;    /* Difference de largeur entre Cell2 et Cell1  */

/*---------------------------------------------------------\
			memorisation_nouvelle_configuration
\---------------------------------------------------------*/
void memorisation_nouvelle_configuration ()
{
	cell_list *Cell;
	int        Pos;

    /****** mise a jour des dernieres cellules ********/
	if (Cell1->next == NULL)
		Rows[Cell1->row].tail = Cell2;
	if (Cell2->next == NULL)
		Rows[Cell2->row].tail = Cell1;

    /****** mise a jour des premieres cellules *******/
	if (Cell1->prev == NULL)
		Rows[Cell1->row].head = Cell2;
	if (Cell2->prev == NULL)
		Rows[Cell2->row].head = Cell1;

	/********* mise a jour des longueurs des rangeesRows[].length ********/
	Rows[Cell1->row].length += DeltaWidth; 
	Rows[Cell2->row].length -= DeltaWidth; 

	/*************** mise a jour des positions de cellules ****************/
	/* ne marche pas si les 2 cellules sont sur la meme rangee */
	/* si 2 suit 1, 2 sera decale de deltawidth .. */
	for(Cell = Cell1->next; Cell; Cell = Cell->next)
		Cell->x += DeltaWidth;
 
	for(Cell = Cell2->next; Cell; Cell = Cell->next)
		Cell->x -= DeltaWidth; 

	Pos      = Cell2->x - (DeltaWidth >> 0 /* 99/9/21 1 */);
	Cell2->x = Cell1->x + (DeltaWidth >> 0 /* 99/9/21 1 */);
	Cell1->x = Pos;

	if (Cell1->next == Cell2)
	{
		if (Cell1->prev)
			Cell1->prev->next = Cell2;
		if (Cell2->next)
			Cell2->next->prev = Cell1;
		Cell1->next = Cell2->next;
		Cell2->next = Cell1;
		Cell2->prev = Cell1->prev;
		Cell1->prev = Cell2;
	}
	else
	{
		int        Row;
		int        Y;
		cell_list *Cell;

		if (Cell1->next)
			Cell1->next->prev = Cell2;
		if (Cell1->prev)
			Cell1->prev->next = Cell2;
		if (Cell2->next)
			Cell2->next->prev = Cell1;
		if (Cell2->prev)
			Cell2->prev->next = Cell1;

	 	Row        = Cell1->row;
	 	Cell1->row = Cell2->row;
	 	Cell2->row = Row;

	 	Y        = Cell1->y;
	 	Cell1->y = Cell2->y;
	 	Cell2->y = Y;

	 	Cell        = Cell1->next;
	 	Cell1->next = Cell2->next;
	 	Cell2->next = Cell;

	 	Cell        = Cell1->prev;
	 	Cell1->prev = Cell2->prev;
	 	Cell2->prev = Cell;
	}
} /* fin de memorisation_nouvelle_configuration */

/*---------------------------------------------------------\
					modification_configuration

Effectue une modification elementaire de la configuration
et renvoie une approximation du cout qu'elle occasionnerait
\---------------------------------------------------------*/
long modification_configuration ()
{
	long          DeltaCoutSignal, DeltaCout, DeltaCoutBandes;
	long          NewX, NewY;
	long          OldX, OldY;
	long          XCellule, YCellule;
	nets_of_cell *signal;
	cells_of_net *cell;
	cell_list    *Cellule;

				/* Recherche de deux cellules a echanger */
	Cell1 = &Cells[rand() % NombreCellules];
	/* sur les tests effectues, on obtient JAMAIS (sur des millions d'appels)
	 * deux fois la meme cellule, il est donc inutile de passer par un entier
	 * qui contiendrait le numero de la cellule, plutot que de calculer directement
	 * l'adresse de la cellule
	 */
	do
	{
		Cell2 = &Cells[rand() % NombreCellules];
	} while (Cell2 == Cell1) ;
	if (Cell2->next == Cell1)
	{
		Cellule = Cell1;
		Cell1 = Cell2;
		Cell2 = Cellule;
	}
	DeltaWidth = Cell2->width - Cell1->width; 

			/* calcul du cout occasionne par la premiere cellule */
	DeltaCout = 0;
	NewX = Cell2->x + ((Cell1->width - Cell2->width) >> 0 /* 99/9/21 1 */);
	NewY = Cell2->y; /* bande de finale de la cellule */
	OldX = Cell1->x; /* position initiale cellule */
	OldY = Cell1->y; /* bande initiale de la cellule */
	for (signal=Cell1->first; signal; signal=signal->next)
	{
		DeltaCoutSignal = 0;
		for(cell=signal->net->first; cell; cell=cell->next)
	 	{
	  		Cellule  = cell->cell;
			if ((Cellule != Cell1) && (Cellule != Cell2)) 
			{
	  			XCellule = Cellule->x;
	  			YCellule = Cellule->y;
				if (Cellule->width)
					DeltaCoutSignal += ((abs(NewX - XCellule) - abs(OldX - XCellule)) << PoidsX)
				                     + abs(NewY - YCellule) - abs(OldY - YCellule);
				else if (XCellule)
					/* c'est un connecteur EST/OUEST */
					DeltaCoutSignal += ((abs(NewX - XCellule) - abs(OldX - XCellule)) << PoidsX)
				                     + ((abs(NewY - YCellule) - abs(OldY - YCellule)) << 1);
				else
					/* c'est un connecteur NORD/SUD */
					DeltaCoutSignal += ((abs(NewY - YCellule) - abs(OldY - YCellule)) << 1);
			}
	 	} /* fin du parcours des cellules signalees a ce signal */
		DeltaCout += DeltaCoutSignal * signal->net->coeff; 
	} /* fin du parcours des signaux attaches a cette cellule */
	DeltaCoutBandes = abs(Rows[Cell1->row].length + DeltaWidth - LargeurIdeale)
	                - abs(Rows[Cell1->row].length - LargeurIdeale);

			/* calcul du cout occasionne par la premiere cellule */
	NewX = Cell1->x + ((Cell2->width - Cell1->width) >> 0 /* 99/9/21 1 */);
	NewY = Cell1->y; /* bande de finale de la cellule */
	OldX = Cell2->x; /* position initiale cellule */
	OldY = Cell2->y; /* bande initiale de la cellule */
	for (signal=Cell2->first; signal; signal=signal->next)
	{
		DeltaCoutSignal = 0;
		for(cell=signal->net->first; cell; cell=cell->next)
	 	{
	  		Cellule  = cell->cell;
			if ((Cellule != Cell1) && (Cellule != Cell2)) 
			{
	  			XCellule = Cellule->x;
	  			YCellule = Cellule->y;
				if (Cellule->width)
					DeltaCoutSignal += ((abs(NewX - XCellule) - abs(OldX - XCellule)) << PoidsX)
				                     + abs(NewY - YCellule) - abs(OldY - YCellule);
				else if (XCellule)
					/* c'est un connecteur EST/OUEST */
					DeltaCoutSignal += ((abs(NewX - XCellule) - abs(OldX - XCellule)) << PoidsX)
				                     + ((abs(NewY - YCellule) - abs(OldY - YCellule)) << 1);
				else
					/* c'est un connecteur NORD/SUD */
					DeltaCoutSignal += ((abs(NewY - YCellule) - abs(OldY - YCellule)) << 1);
			}
	 	} /* fin du parcours des cellules signalees a ce signal */
		DeltaCout += DeltaCoutSignal * signal->net->coeff; 
	} /* fin du parcours des signaux attaches a cette cellule */
	DeltaCoutBandes += 2* (abs(Rows[Cell2->row].length - DeltaWidth - LargeurIdeale)
	                      - abs(Rows[Cell2->row].length - LargeurIdeale));

	return (DeltaCout/Norme + DeltaCoutBandes);
} /* fin de calcul_difference_cout */
