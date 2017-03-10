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
/*     File     : scp_inits.c                                                 */
/*     Contents : fonctions d'initialisation                                  */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   : Pierre Fedrichkine                 Date : 02/07/1993     */
/*     Modified by : Xavier Picat	                Date : 08/07/1993     */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <memory.h>
#include <mut.h>
#include <mlo.h>
#include <mph.h>
#include "scp_types.h"
#include "scp_main.h"
#include "scp_mbk2scp.h"
#include "scp_inits.h"

/*---------------------------------------------------------\
					Les prototypes
\---------------------------------------------------------*/
static void InsereConnecteur ();

/*---------------------------------------------------------\
					Les variables globales
\---------------------------------------------------------*/
long NombreLignes;
long LargeurIdeale;
long Norme;

/*---------------------------------------------------------\
					initialisation
\---------------------------------------------------------*/
void initialisation(WeightRow,nbrow,WHratio)
int	WeightRow;
int	nbrow; 
int WHratio;
{
	long NombreLignesIdeal, NormeLignesIdeal;
	long NormeBandes, NormeCellules;

	/* on considere que pour une bande de hauteur n, la hauteur du canal
	 * de routage est egale a 2.5*n d'ou le /3.5 -> 99/23/9 WHratio
	 */
    if (WHratio==0) WHratio=100;
	NombreLignesIdeal = (int)(sqrt((float)LargeurTotale/(3 * (float)Hauteur * WHratio/100)));
	NormeLignesIdeal = (int)(sqrt((float)NombreCellules)/2);
	if (NombreLignesIdeal == 0)
		NombreLignesIdeal++;
	if (NormeLignesIdeal == 0)
		NormeLignesIdeal++;
	NombreLignes = (nbrow == 0) ? NombreLignesIdeal : nbrow;

	LargeurIdeale = LargeurTotale / NombreLignes;
    fprintf(stderr,"Original width : %ld\n", LargeurIdeale);
	NormeBandes = 1;
	if (NombreLignes < (NormeLignesIdeal >> 1))
		NormeBandes = -((NombreLignes << 3) / NormeLignesIdeal) + 5;
	else if (NombreLignes > (NormeLignesIdeal << 1))
		NormeBandes = ((NombreLignes << 1) / NormeLignesIdeal) - 3;

	NormeCellules = FLOAT_TO_LONG;
	if (NombreCellules > 100)
	{
		NormeCellules *= NombreCellules;
		NormeCellules /= 350;
		NormeCellules += 0.71 * FLOAT_TO_LONG;
	}
	Norme = NormeCellules * NormeBandes * WeightRow;

	Rows = (row_elt *) mbkalloc (sizeof (row_elt) * NombreLignes);
	memset (Rows, 0, sizeof (row_elt) * NombreLignes);
} /* fin de initialisation */

/*---------------------------------------------------------\
				generation_configuration_initiale

       fonction realisant le placement initial aleatoire
\---------------------------------------------------------*/
void generation_configuration_initiale(ptlofig, Connectors, WeightCon, WeightY)
lofig_list   *ptlofig;
PlaceConList *Connectors;
long          WeightCon;
long          WeightY;
{
	CaracConList *Con;
	int    nLigne;
	int    iCell;  /* indice de cellule  */
	int    iRow;   /* indice de collonne */
	int    iCol;   /* indice de colonne  */
	long   iPos;   /* indice de position */
	int    up = 1;
	int    NombreCellulesParRangee;
	int   *tab_row;       /* table d'occupation des rangees (nb cellules) */
	int  **tab_row_col;   /* position des cellules */

	/*********************** initialisations ******************************/
	tab_row_col = (int **) mbkalloc (sizeof (int *) * NombreLignes);
	for (iRow =0; iRow < NombreLignes; iRow++)
	{
		tab_row_col[iRow] = mbkalloc (sizeof (int) * NombreCellules);
		memset (tab_row_col[iRow], NONE, sizeof (int) * NombreCellules);
	}
	tab_row = (int *) mbkalloc (sizeof (int) * NombreLignes);
	memset (tab_row, 0, sizeof (int) * NombreLignes);

	/*********************** initialisations ******************************/
	NombreCellulesParRangee = NombreCellules / NombreLignes;  /* nb de cells par rangee (div entiere) */

	/************ repartition des cellules dans les rangees ***************/
	for (iCell = 0;iCell < NombreCellules;iCell++)
	{
		do
		{
			nLigne = rand()%NombreLignes;
		} while(tab_row[nLigne] > NombreCellulesParRangee);
		tab_row[nLigne]++;
		Cells[iCell].row = nLigne;
		Cells[iCell].y   = nLigne << WeightY;
	}

	/************* repartition des cellules  par colonnes *****************/
	for (iCell = 0; iCell < NombreCellules; iCell++)
	{
		nLigne = Cells[iCell].row;
		iCol = rand()%tab_row[nLigne];
		while(tab_row_col[nLigne][iCol] != NONE)
			if (up)
				if(iCol == tab_row[nLigne]-1)
					iCol =0;
				else
					iCol++;
			else
				if(iCol == 0)
					iCol = tab_row[nLigne]-1;
				else
					iCol--;
		tab_row_col[nLigne][iCol] = iCell;
		up = 1 - up;
	}

	/******** affectation du placement dans la structure Cells  ************/
	for (iRow=0; iRow < NombreLignes; iRow++)
	{
		cell_list *Cell, *PrevCell;

		iCell = tab_row_col[iRow][0];
		Cell  = &Cells[iCell];
		iPos  = Cell->width;

		Rows[iRow].head = Cell;	
		Cell->prev      = NULL;
		Cell->x         = Cell->width >> 0 /* 99/9/21 1 */;

		for (iCol=1; iCol < tab_row[iRow]; iCol++)
		{
			PrevCell = Cell;

			iCell   = tab_row_col[iRow][iCol];
			Cell    = &Cells[iCell];
			Cell->x = iPos + (Cell->width >> 0 /* 99/9/21 1 */);
			iPos   += Cell->width;

			Cell->prev     = PrevCell;
			PrevCell->next = Cell;
		}
		Rows[iRow].tail   = Cell;	
		Cell->next        = NULL;
		Rows[iRow].length = iPos;
	}

	/* insertion des connecteurs */
	if (Connectors)
	{
		cell_list *Cell;

		iCell = NombreCellules;
		for (Con = Connectors->NORTH_CON; Con; Con = Con->NEXT)
		{
			Cell = &Cells[iCell];
			InsereConnecteur (Cell, NombreLignes - 1);
			/* Cell->x = 0; */
			Cell->y = (NombreLignes << WeightY) + WeightCon;
			iCell++;
		}
		for (Con = Connectors->SOUTH_CON; Con; Con = Con->NEXT)
		{
			Cell = &Cells[iCell];
			InsereConnecteur (Cell, 0);
			/* Cell->x = 0; */
			Cell->y = - WeightCon;
			iCell++;
		}
		for (Con = Connectors->WEST_CON; Con; Con = Con->NEXT)
		{
			Cell = &Cells[iCell];
			InsereConnecteur (Cell, Con->USER);
			Cell->x = -WeightCon;
			Cell->y = Con->USER << WeightY;
			iCell++;
		}
		for (Con = Connectors->EAST_CON; Con; Con = Con->NEXT)
		{
			Cell = &Cells[iCell];
			InsereConnecteur (Cell, Con->USER);
			Cell->x = LargeurIdeale+WeightCon;
			Cell->y = Con->USER << WeightY;
			iCell++;
		}
	}

	for (iRow =0; iRow < NombreLignes; iRow++)
		mbkfree (tab_row_col[iRow]);
	mbkfree (tab_row_col);
	mbkfree (tab_row);
} /* fin de generation_configuration_initiale */

/*---------------------------------------------------------\
					InsereConnecteur
\---------------------------------------------------------*/
static void InsereConnecteur (Cell,nLigne)
cell_list *Cell;
int nLigne;
{
	if (nLigne >= NombreLignes)
	{
		fprintf (stderr, "There are only %d rows and a connector is placed in row %d\n", NombreLignes, nLigne);
		exit (1);
	}
	Cell->row = nLigne;
	Rows[nLigne].head->prev = Cell;
	Cell->next = Rows[nLigne].head;
	Cell->prev = NULL;
	Rows[nLigne].head = Cell;
} /* fin de InsereConnecteur */




/* Cette version genere un placement environ deux fois meilleur que le placement
 * aleatoire genere ci-dessus. Il pourrait etre interessant de l'utiliser pour
 * un nombre d'iterations a 0, ou en diminuant le taux d'acceptation initial, car
 * le resultat obtenu est identique.
 * ATTENTION, elle n'utilise pas le meme format de donnee que celui implemente
void generation_configuration_initiale(ptlofig, Connectors, ConWeight)
lofig_list   *ptlofig;
PlaceConList *Connectors;
long          ConWeight;
{
	int        iCell,j;
	int        NonPlacee;
	cell_list *Cell;

	for (iCell=0; iCell<NombreCellules; iCell++)
	{
		Cell = &Cells[iCell];
		NonPlacee = TRUE;
		j = 0;
		do
		{
			if ( Rows[j].head->next == NULL ||
			    ((Rows[j].length+Cell->width < LargeurIdeale) && SignalCommun (Cell,Rows[j].tail)))
			{
				InsereCellule (Cell,j);
				NonPlacee = FALSE;
			}
			j++;
		} while (j<NombreLignes && NonPlacee);
		if (NonPlacee)
		{
			int Largeur, DepassementMin, Ligne;

			Largeur = Cell->width;
			DepassementMin = Rows[0].length+Largeur-LargeurIdeale;
			Ligne = 0;
			for (j=1; j<NombreLignes; j++)
				if (Rows[j].length+Largeur-LargeurIdeale < DepassementMin)
				{
					Ligne = j;
					DepassementMin = Rows[j].length+Largeur-LargeurIdeale;
				}
			InsereCellule (Cell, Ligne);
		}
	}
}

static int SignalCommun (Cell1,Cell2)
cell_list *Cell1;
cell_list *Cell2;
{
	nets_of_cell *sig1, *sig2;

	for (sig1 = Cell1->first; sig1; sig1 = sig1->next)
		for (sig2 = Cell2->first; sig2; sig2 = sig2->next)
			if (sig1->net == sig2->net)
				return (1);
	return (0);
}

static void InsereCellule (Cell,Ligne)
cell_list *Cell; 
int Ligne;
{
	Cell->pos = Cell->width>>1;
	if (Rows[Ligne].head->next)
		Cell->pos += Rows[Ligne].tail->pos + (Rows[Ligne].tail->width>>1);
	Rows[Ligne].length += Cell->width;
	Cell->row = Ligne;

	Cell->next = (cell_list *)&Rows[Ligne].null;
	Cell->prev = Rows[Ligne].tail;
	Rows[Ligne].tail->next = Cell;
	Rows[Ligne].tail = Cell;
}
 */
