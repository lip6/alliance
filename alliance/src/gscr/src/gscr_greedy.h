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
 
# define IMPAIRE(a)	(((a) & 1) ? TRUE : FALSE)
# define JOG_LENGTH(a)  ((long ) (IMPAIRE(a) ? ((a+1) >> 2) : ((a) >> 2)))
# define MINJOG_LENGTH ((long ) 1)
# define LEADPATTERN   6     
extern long EndPattern;
extern long TabRout[MAXCOL][MAXLINE]; 

#define PREVHNET(I)   TabRout[0][I]
#define HNET(I)       TabRout[1][I]
#define SPLIT(I)      TabRout[2][I]
#define SFRT(I)       TabRout[3][I] 
#define WEIGHT(I)     TabRout[4][I] 
#define VNET(I)       TabRout[5][I] 
#define COMBI(I,J)    TabRout[I][J]
#define SCR_TOP       TabRout[1][0] 
#define SCR_BOTTOM(I) TabRout[1][I]  

#define SNC           ((long ) 1000)
#define TERMINAL      ((long ) 0)    
#define NOTTERMINAL   ((long ) 1)
#define STEADY        ((long ) 2)
#define FALLING       ((long ) 3)
#define RISING        ((long ) 4)
