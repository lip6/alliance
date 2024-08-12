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
/*     File     : scp_channel.c                                               */
/*     Contents : main of placer                                              */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   : Xavier Picat                       Date : 21/07/1993     */
/*                                                                            */
/******************************************************************************/

/*---------------------------------------------------------\
					Les includes
\---------------------------------------------------------*/
#include "scp_channel.h"
extern int SXMODE;
extern long MBK_X_GRID;
# define PITCH_X (MBK_X_GRID * SCALE_X)
/*---------------------------------------------------------\
					Les definitions de type
\---------------------------------------------------------*/
typedef struct cells
{
	struct cells *NEXT;
	phins_list   *INS;
} cells_list;
#define min(x,y) ((x<y)?(x):(y))

/*---------------------------------------------------------\
					OpenVerticalChannel 

L'abscisse, Epsilon et Largeur sont un multiples de SCALE_X
\---------------------------------------------------------*/
phfig_list *OpenVerticalChannel (Figure, Abscisse, Epsilon, Largeur)
phfig_list    *Figure;
unsigned long  Abscisse;
unsigned long  Epsilon;
unsigned long  Largeur;
{
	cells_list **Rows, **List;
	phins_list *Ins;
	phfig_list *Fig;
	cells_list *Cell, *Current, *Previous;
	long        Height;
	long        NbRow;
	long        Row;
	long        X, i;
	long        XOpen, MinCost;
	long        Ox, Oy;
	long        NbCells;
	long        iCell;
	cells_list *CellTab;

	if (Figure == NULL || Figure->PHINS == NULL)
		return (NULL);
	if (Abscisse % PITCH_X ||
	    Epsilon  % PITCH_X ||
	    Largeur  % PITCH_X)
	{
		fprintf (stderr, "X(%d), Width(%d), Epsilon(%d) MUST be PITCHED\n", Abscisse, Largeur, Epsilon);
		exit (1);
	}
	Abscisse += Figure->XAB1;
	Oy  = Figure->YAB1;
	Fig = getphfig(Figure->PHINS->FIGNAME,'A');
    sx2sc (Fig);
	Height = Fig->YAB2 - Fig->YAB1;
	if (Epsilon + Abscisse > Figure->XAB2 ||
		Epsilon - Abscisse <= Figure->XAB1)
		Epsilon = 0;

	/* on commence par calculer le nombre de lignes qu'occupe la figure */
	NbCells = 0;
	NbRow   = 0;
	for (Ins = Figure->PHINS; Ins; Ins=Ins->NEXT)
	{
		NbCells++;
		Row = (Ins->YINS - Oy) / Height;
		if (Row > NbRow)
			NbRow = Row;
	}
	NbRow++; /* la ligne 0 existe : 0 a 3 donne 4 lignes */

	/* on remplit la structure rows */
	Rows = (cells_list **) mbkalloc (sizeof (cells_list *) * NbRow);
	memset (Rows, 0, sizeof (cells_list *) * NbRow);
	List = (cells_list **) mbkalloc (sizeof (cells_list *) * NbRow);
	CellTab = (cells_list *) mbkalloc (sizeof (cells_list) * NbCells);
	memset (CellTab, 0, sizeof (cells_list) * NbCells);
	iCell = 0;
	for (Ins = Figure->PHINS; Ins; Ins=Ins->NEXT, iCell++)
	{
		Row  = (Ins->YINS - Oy) / Height;
		X    = Ins->XINS;
		Cell = &CellTab[iCell];
		Cell->INS = Ins;
		Fig  = getphfig (Ins->FIGNAME,'A');
        sx2sc (Fig);
		Previous = (cells_list *)&Rows[Row];
		for (Current = Rows[Row]; Current && X > Current->INS->XINS; Previous = Current, Current = Current->NEXT);
		Cell->NEXT     = Current;
		Previous->NEXT = Cell;
	}

	/* on recherche l'abscisse optimale */
	XOpen   = Abscisse - Epsilon;
	MinCost = 1<<30;
	for (X = XOpen; X <= Abscisse + Epsilon; X+=PITCH_X)
	{
		long Left, Right, Cost;

		Left  = 1<<30;
		Right = 0;
		Cost  = 0;
		for (i=0; i<NbRow; i++)
		{
			Previous  = NULL;
			for (Cell = Rows[i]; Cell && Cell->INS->XINS < X; Previous = Cell, Cell = Cell->NEXT);
			if (Previous)
			{
				if (Previous->INS->XINS < Left)
					Left = Previous->INS->XINS;
				if (Previous->INS->XINS > Right)
					Right = Previous->INS->XINS;
			}
			if (Right - Left > Cost)
				Cost = Right - Left;
		}
		if (MinCost > Cost &&
		    abs(XOpen-Abscisse) >= abs(X-Abscisse))
		{
			MinCost = Cost;
			XOpen = X;
		}
	}

	X = MinCost + Largeur;
	if (X%PITCH_X)
		X += PITCH_X - X % PITCH_X;
	for (i=0; i<NbRow; i++)
	{
		Previous  = NULL;
		for (Cell = Rows[i]; Cell && Cell->INS->XINS < XOpen; Previous = Cell, Cell = Cell->NEXT);
		for (Cell = Previous; Cell; Cell = Cell->NEXT)
			Cell->INS->XINS += X;
	}
	Figure->XAB2 += X;
	mbkfree (CellTab);
	mbkfree (Rows);
	mbkfree (List);
} /* fin de OpenVerticalChannel */
