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
/*     File     : scp_mbk2scp.c                                               */
/*     Contents : function wich loads the netlist in the internal data 	      */
/* 		          structure through MBK data base 			                  */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   : Pierre Fedrichkine                 Date : 02/07/1993     */
/*     Modified by : Xavier Picat                       Date : 09/08/1993     */
/*                                                                            */
/******************************************************************************/
#include <memory.h>
#include <mut.h>
#include <mlo.h> 
#include <mph.h>
#include "scp_types.h"
#include "scp_inits.h"
#include "scp_main.h"
#include "scp_mbk2scp.h"

/*---------------------------------------------------------\
					Prototypes
\---------------------------------------------------------*/
static void add_net_cell ();
static void add_cell_net ();
static long man2dp       ();
static long get_index    ();

/*---------------------------------------------------------\
					Les variables globales
\---------------------------------------------------------*/
long NombreCellules;
long NombreSignaux;
long LargeurTotale;
long Hauteur;

/*---------------------------------------------------------\
					chargement_figure
\---------------------------------------------------------*/
void chargement_figure(ptlofig,Connectors)
lofig_list   *ptlofig;
PlaceConList *Connectors;
{
	phfig_list   *ptphfig;
	locon_list   *ptlocon;
	loins_list   *ptloins;
	losig_list   *ptlosig;
	int           NetMax,iNet;
	int           NombreConnecteurs;
	int          *inter_net;
	CaracConList *Con;
	net_list     *Net;
	cell_list    *Cell;

	/**************** comptage du nombre de cellules *************************/
	NombreCellules = 0;
	for (ptloins=ptlofig->LOINS; ptloins; ptloins=ptloins->NEXT) 
		NombreCellules++;
	NombreConnecteurs = 0;
	if (Connectors)
	{
		for (Con = Connectors->NORTH_CON; Con; Con = Con->NEXT)
			NombreConnecteurs++;
		for (Con = Connectors->SOUTH_CON; Con; Con = Con->NEXT)
			NombreConnecteurs++;
		for (Con = Connectors->WEST_CON;  Con; Con = Con->NEXT)
			NombreConnecteurs++;
		for (Con = Connectors->EAST_CON;  Con; Con = Con->NEXT)
			NombreConnecteurs++;
	}
	Cells = (cell_list *) mbkalloc (sizeof (cell_list) * (NombreCellules + NombreConnecteurs));
	memset (Cells, 0, sizeof (cell_list) * (NombreCellules + NombreConnecteurs));

	/**************** comptage du nombre de signaux *************************/
	NombreSignaux = 0;
	NetMax = 0;
	for (ptlosig=ptlofig->LOSIG; ptlosig; ptlosig=ptlosig->NEXT)
	{
		NombreSignaux++;
		if (ptlosig->INDEX>NetMax)
			NetMax = ptlosig->INDEX;
	}
	NetMax++; /* les signaux partent de 0 */
	Nets = (net_list *) mbkalloc (sizeof (net_list) * NombreSignaux);
	memset (Nets, 0, sizeof (net_list) * NombreSignaux);
	inter_net = (int *) mbkalloc (sizeof (int) * NetMax);
	iNet = 0;
	for (ptlosig=ptlofig->LOSIG; ptlosig; ptlosig=ptlosig->NEXT)
	{
		inter_net[ptlosig->INDEX] = iNet;
		iNet++;
	}

	/******************** lecture des instances (cellules) ********************/
	LargeurTotale = 0;
	Cell          = &Cells[0];
	for (ptloins=ptlofig->LOINS; ptloins; ptloins=ptloins->NEXT) 
	{
		Cell->ins      = ptloins;
		ptphfig        = getphfig(ptloins->FIGNAME,'A');
        sx2sc (ptphfig);
		Cell->width    = (ptphfig->XAB2 - ptphfig->XAB1) / SCALE_X;
		LargeurTotale += Cell->width;
		for(ptlocon=ptloins->LOCON; ptlocon; ptlocon=ptlocon->NEXT)  
		{		 
			char *NomConnecteur;

			NomConnecteur = namealloc(ptlocon->NAME);
			if (!(isvdd(NomConnecteur) || isvss(NomConnecteur))) 
			{
				Net = &Nets[inter_net[ptlocon->SIG->INDEX]];
				add_net_cell(Cell,Net);
				add_cell_net(Net,Cell);
		 	}
		}
		Cell++;
	}
	if (Connectors)
	{
		int           i;

		for (i = 0; i < 4; i++)
			for (Con = ((CaracConList **)Connectors)[i]; Con; Con = Con->NEXT)
			{
				Net = &Nets[inter_net[get_index(ptlofig->LOSIG, namealloc(Con->NAME))]];
				add_net_cell(Cell,Net);
				add_cell_net(Net,Cell);
				Cell++;
			}
	}
    for (iNet=0; iNet<NombreSignaux; iNet++)
        Nets[iNet].coeff = man2dp(Nets[iNet].coeff);
    Hauteur = (ptphfig->YAB2 - ptphfig->YAB1) / SCALE_X;

	/******************** lecture des connecteurs *********************/
	mbkfree (inter_net);
} /* fin de chargement_figure */

/*---------------------------------------------------------\
					get_index
\---------------------------------------------------------*/
static long get_index (ptlosig, ConName)
losig_list *ptlosig;
char       *ConName;
{
	chain_list *NameList;

	while (ptlosig)
	{
		NameList = ptlosig->NAMECHAIN;
		while (NameList)
		{
			if (ConName == NameList->DATA)
				return (ptlosig->INDEX);
			NameList = NameList->NEXT;
		}
		ptlosig = ptlosig->NEXT;
	}
	fprintf (stderr, "\nThere is no signal named %s\n", ConName);
	exit (1);
} /* fin de get_index */

/*---------------------------------------------------------\
					add_net_cell
\---------------------------------------------------------*/
static void add_net_cell(Cell,Net)
cell_list *Cell;
net_list  *Net;
{
	nets_of_cell *NetOfCell;

	NetOfCell       = (nets_of_cell *) mbkalloc (sizeof (nets_of_cell));
	NetOfCell->net  = Net;
	NetOfCell->next = Cell->first; 
	Cell->first     = NetOfCell; 
} /* fin de add_net_cell */

/*---------------------------------------------------------\
					add_cell_net
\---------------------------------------------------------*/
static void add_cell_net(Net,Cell)
net_list  *Net;
cell_list *Cell;
{
	cells_of_net *CellOfNet;

	CellOfNet = (cells_of_net *) mbkalloc (sizeof (cells_of_net));
	CellOfNet->cell = Cell;
	CellOfNet->next = Net->first;
	Net->coeff++;
	Net->first      = CellOfNet;
} /* fin de add_cell_net */

/*---------------------------------------------------------\
					man2dp
\---------------------------------------------------------*/
static long man2dp(card)
int card;
{
	switch(card)
	{
		case 0 :
		case 1 :
			/* fprintf (stderr, "Warning : a signal isn't connected to any cell\n"); */
			return(0);
			break;

		case 2 : return(FLOAT_TO_LONG * 1);
			break;

		case 3 : return(FLOAT_TO_LONG * 0.5);
			break;

		case 4 : return(FLOAT_TO_LONG * 0.333);
			break;

		case 5 : return(FLOAT_TO_LONG * 0.222);
			break;

		case 6 : return(FLOAT_TO_LONG * 0.15);
			break;

		case 7 : return(FLOAT_TO_LONG * 0.11);
			break;

		case 8 : return(FLOAT_TO_LONG * 0.087);
			break;

		case 9 : return(FLOAT_TO_LONG * 0.0689);
			break;

		case 10 : return(FLOAT_TO_LONG * 0.0563);
			break;

		default : return(FLOAT_TO_LONG * 6.0 / (card*(card+1.0)));
	}
} /* fin de man2dp */
