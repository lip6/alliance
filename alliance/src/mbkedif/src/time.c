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
 
/*
 * gives time the format edif wants
 * $Log: time.c,v $
 * Revision 1.2  2002/03/14 12:36:30  fred
 * Makes the correct substitutions in lex and yacc generated files.
 * Fixing includes in dot c files
 *
 */

#include <stdio.h>
#include <time.h>

char *EdifTime()
{
int EdifMonth();
char *date;
long tim;
char day[6];
int	year;
int	nday;
int	hour;
int	minute;
int	second;
char month[6];
int	nmonth;

	time(&tim);
	date = (char *)malloc(30);
	strcpy(date, ctime(&tim));
	sscanf(date, "%s %s %d %d:%d:%d %d",
		day,month,&nday,&hour,&minute,&second,&year);
	sprintf(date, "%04d %02d %02d %02d %02d %02d",
	      	year,EdifMonth(month),nday,hour,minute,second); 
	return date;
}

int EdifMonth(month)
char *month;
{
	if (!strcmp(month, "Jan"))
		return 1;
	else if (!strcmp(month, "Feb"))
		return 2;
	else if (!strcmp(month, "Mar")) 
		return 3;
	else if (!strcmp(month, "Apr")) 
		return 4;
	else if (!strcmp(month, "May")) 
		return 5;
	else if (!strcmp(month, "Jun")) 
		return 6;
	else if (!strcmp(month, "Jul")) 
		return 7;
	else if (!strcmp(month, "Aug")) 
		return 8;
	else if (!strcmp(month, "Sep")) 
		return 9;
	else if (!strcmp(month, "Oct")) 
		return 10;
	else if (!strcmp(month, "Nov")) 
		return 11;
	else if (!strcmp(month, "Dec")) 
		return 12;
	else return 0;
}
