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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/*******************************************************************************
*                                                                              *
*  Tool        : Spice parser / driver v 7.00                                  *
*  Author(s)   : Gregoire AVOT                                                 *
*  Updates     : March, 18th 1998                                              *
*                                                                              *
*******************************************************************************/

#ifndef SPI_HASH
#define SPI_HASH

typedef struct shashelem
{
  struct shashelem	*suivant;
  char			*mot;
  void			*ptr;
} hashelem;

typedef struct sthash
{
  int		  entree;       /* Nombre d'entrées dans la table de hash 2^n */
  int		  nbelem;	/* Nombre d'element dans les liste            */
  hashelem	**table;	/* informations dans la table                 */
  hashelem	 *tete;		/* Blocs libre pour allocation par tat        */
  chain_list	 *libere;	/* Pointeurs pour les libérations             */
} thash;

thash* creatthash __P(( ));
/* 
  Crée une nouvelle table de hash. Le parametre d'entrée est le nombre
d'entrees de la nouvelle table
*/

void   freethash __P(( thash *pt ));
/*
  Libère une table de hash précédement allouée
*/

void   addthashelem __P(( char *nouveau, void *ptr, thash *table ));
/*
  Ajoute un élément dans la table de hash. Le premier parametre est
la chaine identifiant l'élément, le second est l'élément que l'on place dans
la table et finalement le troisième est l'élément vers la table de hash
*/

void*  getthashelem __P(( char *elem, thash *table, int *status ));
/*
  Récupère un élément dans la table de hash à partir de son identificateur
passé en premier paramètre. Le second argument est la table de hash sur laquelle
la recherche est effectuée. Si le troisième élément est différent de NULL,
la valeur 1 est plcée à cette adresse si l'élément est trouvé, 0 sinon. La
valeur renvoyée est celle de l'élément recherché.
*/

int    thashsignature __P(( char *c, int l ));
/*
  Utilisé en interne
*/

void	resizetable    __P(( thash*, int ));
hashelem* nouvhashelem  __P(( thash* ));
void	liberehashelem __P(( thash*, hashelem* ));
#endif
