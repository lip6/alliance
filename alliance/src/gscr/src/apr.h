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
/*     Product  : Automatic Router and Placer library                         */
/*     File     : @(#)                                                        */
/*     Contents :                                                             */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   : El housseine REJOUAN               Date : 06/10/1993     */
/*     Modified by :                                    Date : ../../....     */
/*     Modified by :                                    Date : ../../....     */
/*     Modified by :                                    Date : ../../....     */
/*                                                                            */
/******************************************************************************/

/*############################################################################*/
/*                Symbolic Channel Router                                     */
/*############################################################################*/
# define BOOLEAN	int 
# define NONET		((long ) 0)
# define PITCH_X	((long )(/*6*/5 * SCALE_X))
# define PITCH_Y	((long )(5 * SCALE_X))
# define NORTHOFFSET	((long )((L3MODE?5:4) * SCALE_X))
# define SOUTHOFFSET	((long )((L3MODE?0:4) * SCALE_X))
# define WESTOFFSET	(PITCH_X /* / 2 */)
# define EASTOFFSET	(0 /* PITCH_X / 2 */)
# define LAYER1WIDTH	((long) 2 * SCALE_X)
# define LAYER2WIDTH	((long) 2 * SCALE_X)

/******************************************************************************/
typedef struct SCR_ConnectorList
	{
	 struct SCR_ConnectorList	*NextCon;
	 long				ConName ;/* index signal */
	 long				Mark    ;/* number of the column */
	}ConnectorList;

typedef struct SCR_SegmentList
	{
	 struct SCR_SegmentList		*NextSeg;
	 char				*SegName;
	 long				X1Seg   ;
	 long				Y1Seg   ;
	 long				X2Seg   ;
	 long				Y2Seg   ;
	}SegmentList;

typedef struct SCR_ViasList
	{
	 struct SCR_ViasList		*NextVia;
	 long				XVia    ;
	 long				YVia    ;
	} ViasList;

/******************************************************************************/
extern  BOOLEAN         SymbolicChannelRouter();

/*############################################################################*/
/*                Symbolic Placer                                             */
/*############################################################################*/

typedef struct CaracCon
        {
         struct CaracCon	*NEXT;
         char           	*NAME; /* nom du connecteur a place */
         long           	USER;  /* ordre du connecteur a place sur une  */
                                       /* face donnee (concerne pour scr)      */
                                       /* numero du slice dans lequel le       */
                                       /* connecteur doit etre place (concerne */
                                       /* pour dpr) */
        } CaracConList;

typedef struct PlaceCon
        {
         CaracConList	*NORTH_CON;
         CaracConList	*SOUTH_CON;
         CaracConList	*WEST_CON;
         CaracConList	*EAST_CON;
         long           VER_FEED; /* pour scr, ce champs designe le nombre de transparences verticales */
         long           HOR_FEED; /* pour scr, ce champs designe le nombre de transparences horizontales */
	} PlaceConList;

/*---------------------------------------------------------\
					includes
\---------------------------------------------------------*/
#include <mut.h> 
#include <mlo.h> 
#include <mph.h> 

/*---------------------------------------------------------\
					typedefs
\---------------------------------------------------------*/
typedef struct cell_struct
{
	struct cell        *cell;
	struct cell_struct *next;
} cells_of_net;
typedef struct
{
	long          coeff;
	cells_of_net *first;
} net_list;

typedef struct net_struct
{
	net_list           *net;
	struct net_struct  *next;
} nets_of_cell;
typedef struct cell
{
	struct cell  *next;
	struct cell  *prev;
	int           width;
	int           x;
	int           y;
	int           row;
	loins_list   *ins;
	void         *first;
} cell_list;

typedef struct
{
	cell_list *head;
	cell_list *tail;
	long       length;
} row_elt;

typedef struct
{
	row_elt      *Rows;
	long          NbRows;
	cell_list    *Cells;
	long          NbCells;
} placement_fig;

/*---------------------------------------------------------\
					Les prototypes
\---------------------------------------------------------*/
extern void sx2sc ();
extern placement_fig *Place();
extern phfig_list *OpenVerticalChannel();

/*############################################################################*/
