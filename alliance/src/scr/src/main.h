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

/******************************************************************************/
/*                                                                            */
/*                    CAO & VLSI's cad tools chain Alliance                   */
/*                                                                            */
/*     Product  : main.h                                                      */
/*     File     : @(#)                                                        */
/*     Contents :                                                             */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   : El housseine REJOUAN               Date : 22/08/1992     */
/*     Modified by :                                    Date : ../../....     */
/*     Modified by :                                    Date : ../../....     */
/*     Modified by :                                    Date : ../../....     */
/*                                                                            */
/******************************************************************************/
typedef struct Option
	{
	 char		*ChannelName;
	 char		*InputFileName;
	 char		*OutputFileName;
	 BOOLEAN	Placer;
	 BOOLEAN	Router;
	 int		SupplyRecall;
	 int		Row;
	 int		Iteration;
	}OptionList;

typedef struct XSupplyRecall
	{
	 struct XSupplyRecall	*NEXT;
	 long			X;
	}XSupplyRecallList;
/******************************************************************************/
/*  Variables Globales                                                        */
/******************************************************************************/
char	*NameVdd;
char	*NameVss;
