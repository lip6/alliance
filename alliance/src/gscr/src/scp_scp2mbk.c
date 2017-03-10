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
/*     File     : scp_scp2mbk.c                                               */
/*     Contents : function sauvegarde_figure wich save the final placement    */
/*                through MBK data base                                       */
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
#include "apr.h"

/*---------------------------------------------------------\
				Placer2Scr

          ecriture du placement dans mbk physique
\---------------------------------------------------------*/
phfig_list	*Placer2Scr(Nom,FigurePlacee)
char		*Nom;
placement_fig	*FigurePlacee;

{
	phfig_list   *ptphfig;
	int           iCell, iRow;
	int           Width;
	int           Height; 
	row_elt      *Row;
	cell_list    *Cell;
	long          step, percent, i;
	long          NombreLignes, NombreCellules, Hauteur;

	printf ("Saving placement ");
	/******************  recuperation de la hauteur d'UNE cellule ********************/
	Row  = FigurePlacee->Rows;
	ptphfig = getphfig (Row[0].head->ins->FIGNAME,'P');
    sx2sc(ptphfig);
	Hauteur = (ptphfig->YAB2 - ptphfig->YAB1) / SCALE_X;

	/******************  chargement du pointeur figure ********************/
	ptphfig = addphfig(Nom);
	NombreCellules = FigurePlacee->NbCells;
	NombreLignes   = FigurePlacee->NbRows;

	/**************** calcul de la taille de la figure ********************/
	Width = 0;
	for (iRow=0;iRow<NombreLignes;iRow++)		/* bande la plus large */
		if (Row[iRow].length>Width) 
			Width=Row[iRow].length;	
	Height = NombreLignes*Hauteur;
	ptphfig->XAB1 = 0;
	ptphfig->YAB1 = 0; 
	ptphfig->XAB2 = Width*SCALE_X;
	ptphfig->YAB2 = Height*SCALE_X; 

	step = NombreCellules/100;
	percent = 0;
	i = step - 1;
	/********************* placement des cellules *************************/
	Cell = FigurePlacee->Cells;
	for (iCell=0;iCell < NombreCellules;iCell++)
	{
		i++;
		if (i == step)
		{
			i = 0;
			printf ("%3d%%\b\b\b\b", percent);
			fflush (stdout);
			percent++;
		}
		addphins (ptphfig,
		          Cell->ins->FIGNAME,
		          Cell->ins->INSNAME,
		          NOSYM,
		          (Cell->x-(Cell->width>>0 /* 99/9/21 1 */))*SCALE_X,
		          Cell->row*Hauteur*SCALE_X
		         );
		Cell++;
	}
	printf ("100%\n");
	mbkfree (FigurePlacee->Rows);
	mbkfree (FigurePlacee->Cells);
	return(ptphfig);
} /* fin de Placer2Scr */
