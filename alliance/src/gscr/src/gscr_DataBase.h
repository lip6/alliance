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
 
# define MAX(a,b)       ((a>b) ? a:b)
# define BOOLEAN        int
# define FALSE          ((int ) 0)
# define TRUE           ((int ) 1)
# define NOP		((char )'0')
# define HOR		((char )'H')
# define VER		((char )'V')

typedef struct SCR_StaticPoint
	{
	 struct SCR_StaticPoint		*NextPoint ;
	 struct SCR_StaticPoint		*RightPoint;
	 long				PointName  ;
	 BOOLEAN			Via        ;
	 char				Layer1     ;
	 char				Layer2     ;
	}StaticPoint;	

typedef struct SCR_StaticColumn
	{
	 struct SCR_StaticColumn	*NextCol  ;
	 struct SCR_StaticColumn	*PrevCol  ;
	 long				NorthCon  ;
	 long				SouthCon  ;
	 struct SCR_StaticPoint		*PointList;
	}StaticColumn;


