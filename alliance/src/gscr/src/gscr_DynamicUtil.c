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
/*                             STANDARD CELLS ROUTER                          */
/*    FILE : DynamicUtility.c   LAST MODIFICATION : JUL/16/1991               */
/******************************************************************************/

# include <stdio.h>
# include "gscr_DataBase.h"
# include "gscr_greedy.h"

char	car;
char	string[10];
int	i=0;

/******************************************************************************/
/*                        LOAD DYNAMIC DATA BASE                              */
/******************************************************************************/
long 	U_ReadNumber(ptfich)

FILE	*ptfich;

{
 long	Number = 0;
 i = 0;
 car = getc(ptfich); 
 while ((car != ' ') && (car != ',') && (car != '}')) {
  if (car != '\n') string[i++] = car;
   car = getc(ptfich);
  }
  string[i] = '\0';
  Number = atol(string);
 return (Number);
}
/******************************************************************************/

void	U_GetDynamicDataBase(FichName, Density)

char	*FichName;
long	Density;

{
 long	Line   = 0;
 FILE	*ptfich;

 ptfich = fopen(FichName,"r");
  while ((car = getc(ptfich)) != '\n') string[i++] = car;
  string[i] = '\0';
  for (; Line <= Density + 1; Line++)  {
   car = getc(ptfich); 
   PREVHNET(Line) = U_ReadNumber(ptfich);
   HNET(Line)     = U_ReadNumber(ptfich);
   SPLIT(Line)    = U_ReadNumber(ptfich);
   SFRT(Line)     = U_ReadNumber(ptfich);
   VNET(Line)     = U_ReadNumber(ptfich);
   car = getc(ptfich);
  }
 fclose(ptfich);
}

/******************************************************************************/
/*                               PRINT COLUMN                                 */
/******************************************************************************/
void	U_PrintDynamicDataBase(Density)

long	Density;

{
 long	Column = 0;
 long	Line   = 0;
 printf(" P                   \n");
 printf(" R                   \n");
 printf(" E           W       \n");
 printf(" V     S     E     C \n");
 printf(" H  H  P  S  I  V  O \n");
 printf(" N  N  L  F  G  N  M \n");
 printf(" E  E  I  R  H  E  B \n");
 printf(" T  T  T  T  T  T  I \n\n");

 for (Line = 0; Line <= Density + 1; Line++) {
  for (Column = 0; Column <= EndPattern; Column++) 
   printf("%2ld ", TabRout[Column][Line]);
  printf("\n");
 }
}

/******************************************************************************/
