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
/*     File     : scp_main.c                                                  */
/*     Contents : main of placer                                              */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   : Pierre Fedrichkine                 Date : 02/07/1993     */
/*     Modified by : Xavier Picat                       Date : 08/07/1993     */
/*                                                                            */
/******************************************************************************/

/*---------------------------------------------------------\
					Les includes
\---------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>
#include <mut.h>
#include <mlo.h>
#include <mph.h>
#include "scp_types.h"
#include "scp_modif.h"
#include "scp_main.h"
#include "scp_inits.h"
#include "scp_mbk2scp.h"
#include "scp_scp2mbk.h"
#include "scp_time.h"

/*---------------------------------------------------------\
					Les prototypes
\---------------------------------------------------------*/
extern float CalculCoutTotal   ();
extern long  Accepte           ();

/*---------------------------------------------------------\
					Les variables globales
\---------------------------------------------------------*/
long           PoidsX, PoidsY;
cell_list     *Cells;
net_list      *Nets;
row_elt       *Rows;
placement_fig  Placement;

/*---------------------------------------------------------\
					Les variables locales
\---------------------------------------------------------*/
static long CoutMoyen = 141900;

/*---------------------------------------------------------\
					Place 
\---------------------------------------------------------*/

int SXMODE=0;
int SCR_RATIO=100; /* carre */

placement_fig *Place (LogicalFigur,NombreModifications,NbRow,WeightX,WeightY,WeightRows,WeightCon,Connectors,WHratio)
lofig_list    *LogicalFigur;
unsigned long  NombreModifications;
unsigned long  NbRow;
unsigned long  WeightX;
unsigned long  WeightY;
unsigned long  WeightRows;
long           WeightCon;
int            WHratio;
PlaceConList  *Connectors;
{
	float CoutInitial; /* il faut le laisser en flottant car le cout initial est >> 2 000 000 000 */
	int   Iteration;
	int   Acceptees, Totales;
	long  DeltaCout;
	nets_of_cell *pt_cn1; 
	nets_of_cell *pt_cn2; 
	cells_of_net *pt_cl1; 
	cells_of_net *pt_cl2; 
	int   iNet, iCell;

	set_time ();
	srand(getpid());
	PoidsX = WeightX;
	PoidsY = WeightY;
	printf("Loading SCP data base ...\n");
	chargement_figure(LogicalFigur,Connectors); 
	initialisation(WeightRows,NbRow,WHratio); 
	printf("Generating initial placement ... \n");
	generation_configuration_initiale(LogicalFigur, Connectors, WeightCon, WeightY);
	CoutInitial = CalculCoutTotal();
	printf ("%d cells %d nets in %d rows\n", NombreCellules, NombreSignaux, NombreLignes);
	Placement.Rows       = Rows;
	Placement.NbRows     = NombreLignes;
	Placement.Cells      = Cells;
	Placement.NbCells    = NombreCellules;
	if (NombreCellules > NombreModifications)
		NombreModifications = 10*NombreCellules;

	printf("Placement in process of treatment : ");
	Iteration = 85;
	while (Iteration)
	{
		printf ("%3d%%\b\b\b\b",100-20*Iteration/17); 
		fflush (stdout);
		Acceptees = 0;
		Totales = 0;
		while (Acceptees < NombreModifications && Totales < NombreModifications*3)
	  	{
			DeltaCout = modification_configuration ();
			if (DeltaCout <= 0 || Accepte (DeltaCout))
			{
		 		memorisation_nouvelle_configuration();
				Acceptees++;
			}
			Totales++;
	 	}
		if (CoutMoyen < 10240)
		{
			CoutMoyen *= 9;
			CoutMoyen /= 10;
		}
		else if (CoutMoyen < 51200)
		{
			CoutMoyen <<= 2;
			CoutMoyen /= 5;
		}
		else
		{
	   		CoutMoyen *= 49;
	   		CoutMoyen /= 50;
		}
		Iteration--;
	}
	printf ("100%%\n%d%% saved in %4.1f s\n", (int)(100*(CoutInitial-CalculCoutTotal())/CoutInitial), get_time ());
	fflush (stdout);

	/* liberation des structures associees au signaux */
	for (iCell = 0;iCell < NombreCellules;iCell++)
		if (pt_cn1=Cells[iCell].first)
		{
			for(pt_cn2=pt_cn1->next;pt_cn2;pt_cn1=pt_cn2,pt_cn2=pt_cn2->next)
				mbkfree(pt_cn1);
			Cells[iCell].first = NULL;
		}
	for (iNet = 0;iNet < NombreSignaux;iNet++)
		if (pt_cl1=Nets[iNet].first)
			for(pt_cl2=pt_cl1->next;pt_cl2;pt_cl1=pt_cl2,pt_cl2=pt_cl2->next)
				mbkfree(pt_cl1);
	mbkfree (Nets);
	return (&Placement);
}

/*---------------------------------------------------------\
					Accepte
\---------------------------------------------------------*/
long Accepte (DeltaCout)
long DeltaCout;
{
	long Proba;

	Proba = CoutMoyen-(DeltaCout<<10);
	if (Proba <0)
		return (0);
	else
		return (CoutMoyen*(rand() % 100) < 100*Proba);
}

/*---------------------------------------------------------\
					CalculCoutTotal
\---------------------------------------------------------*/
float CalculCoutTotal ()
{
	cells_of_net *cell1, *cell2;
	long          CoutNoeud;
	float         CoutTotal;
	int           Signal;

	CoutTotal = 0;
   	/*** parcours et calcul du cout total du a la longueur des nets *******/
	for (Signal = 0; Signal < NombreSignaux; Signal++)
	{
		CoutNoeud = 0;
		for (cell1 = Nets[Signal].first; cell1; cell1 = cell1->next)
			for (cell2 = cell1->next; cell2; cell2 = cell2->next)
		   		CoutNoeud += (abs(cell1->cell->x - cell2->cell->x)<<PoidsX)
	  			           +  abs(cell1->cell->y - cell2->cell->y); 
		CoutTotal += CoutNoeud * Nets[Signal].coeff;
	}
	return (CoutTotal);
}

