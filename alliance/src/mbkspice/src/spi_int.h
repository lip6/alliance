/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


/*******************************************************************************
*                                                                              *
*  Tool        : Spice parser / driver v 7.00                                  *
*  Author(s)   : Gregoire AVOT                                                 *
*  Updates     : March, 18th 1998                                              *
*                                                                              *
*******************************************************************************/

#ifndef spi_int
#define spi_int

typedef struct st_tableint
{
  int                     index;
  void                   *data;
  struct st_tableint     *down;  
} tableint;

/* Lors qu'on ajoute quelque chose dans la liste, le pointeur DATA ne doit pas
   etre NULL, cette valeur etant utilisée en interne sur ce champs */

#define SPI_TABLEINTMAX  256
#define SPI_TABLEINTMASK 0x000000FF

tableint*	creattableint __P(());
/* Crée une nouvelle table						*/

int		settableint __P(( tableint *table, int value, void *data ));
/* Ajoute un élément dans la table. Renvoie 1 si l'élément est nouveau,
   0 sinon (l'élément précédent est alors remplacé)			*/

void*		tsttableint __P(( tableint *table, int value ));
/* Renvoie l'élément recherché par la valeur				*/

void		freetableint __P(( tableint *table ));
/* Libère la table							*/

int             scanint __P(( tableint *table, int n ));
/* Parcour des éléments mémorisés dans la table. On commence par mettre
  l'entier d'entrée à 0, puis pour les elements suivant on met la valeur
  du noeud retourné précédement. La fin est indiqué par un retour 0	*/


#endif
