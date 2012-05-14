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

#include <stdio.h>
#include <mut.h>
#include "spi_hash.h"

/* 2^n elements dans la table de hash */
#define HASH_MINI	4
#define HASH_MAXI	15

/* 2^n Seuil d'augmentation */
#define HASH_DELTA	1

thash*          creatthash( )
{
  thash		*new;
  int		 i;
  int		 n;

  new           = ( thash* )mbkalloc( sizeof( thash ) );
  new->entree	= HASH_MINI ;
  n             = 1 << new->entree;
  new->table	= ( hashelem** )mbkalloc( sizeof( hashelem* ) * n );
  new->nbelem   = 0;
  new->tete     = NULL;
  new->libere   = NULL;

  for( i = 0 ; i < n ; i++ )
    new->table[i]  = NULL;

  return( new );
}

void            freethash( pt )
thash           *pt;
{
  chain_list	*scanchain;

  mbkfree( pt->table );
  /*
  if( pt->tete )
    mbkfree( pt->tete );
  */
  
  for( scanchain = pt->libere ; scanchain ; scanchain = scanchain->NEXT )
    mbkfree( scanchain->DATA );
  freechain( pt->libere );

  mbkfree( pt );
}

hashelem*	nouvhashelem( table )
thash		*table;
{
  int		i;
  hashelem	*elem;

  if( ! table->tete )
  {
    table->tete   = ( hashelem* ) mbkalloc ( sizeof( hashelem ) * 64 );
    table->libere = addchain( table->libere, table->tete );
    elem          = table->tete;

    for( i = 1 ; i < 64 ; i++ )
    {
      elem->suivant = elem + 1;
      elem++;
    }
    elem->suivant = NULL;
  }

  elem              = table->tete;
  table->tete       = table->tete->suivant;

  return( elem );
}

void		liberehashelem( table, elem )
thash		*table;
hashelem	*elem;
{
  elem->suivant = table->tete;
  table->tete   = elem;
}

void            addthashelem( nouveau, ptr, table )
char            *nouveau;
void            *ptr;
thash           *table;
{
  int           s;
  hashelem      *elm;


  if( table->nbelem == 1 << ( table->entree + HASH_DELTA ) &&
      table->entree < HASH_MAXI                               )
    resizetable( table, table->entree + 1 );
    
  s   = thashsignature( nouveau, table->entree );
  elm = nouvhashelem( table );

  elm->suivant      = table->table[ s ];
  table->table[ s ] = elm;

  elm->mot          = nouveau;
  elm->ptr          = ptr;

  table->nbelem++;
}

void*           getthashelem( elem, table, status )
char            *elem;
thash           *table;
int             *status;
{
  int		 s;
  hashelem	*scan;

  s = thashsignature( elem, table->entree );

  for( scan = table->table[ s ] ; scan ; scan = scan->suivant )
  {
    if( scan->mot == elem || strcmp( scan->mot, elem ) == 0 )
    {
      if( status != NULL )
        *status = 1;
      return( scan->ptr );
    }
  }

  if( status != NULL )
    *status = 0;
  
  return( NULL );
}

int             thashsignature( c, l )
char            *c;
int             l;
{
  int bit;
  int b;
  int dec;

  dec = 0;
  bit = 0;

  while( *c )
  {
    for( bit = 0; bit < 8 ; bit ++ )
    {
      b = ( *c >> bit ) & 0x1;

      /* Polynome générateur : X^15 + X + 1 */
      dec = ( ( dec & 0x3FFE ) << 1 )                               |
            ( ( ( dec & 0x4000 ) >> 13 ) ^ ( ( dec & 0x1 ) << 1 ) ) |
	    ( ( ( dec & 0x4000 ) >> 14 ) ^ b ) ;
    }
    c++;
  }

  return( dec % ( 1 << l ) );
}

void resizetable( table, elem )
thash		*table;
int		elem;
{
  hashelem	**new;
  hashelem	 *nouv;
  hashelem	 *scan;
  hashelem	 *suiv;
  int		  n;
  int		  i;
  int		  s;

  n = 1 << elem;
  
  new = ( hashelem** )mbkalloc( sizeof( hashelem* ) * n );
  for( i = 0 ; i < n ; i++ )
    new[ i ] = NULL;
  
  n = 1 << table->entree;

  for( i = 0 ; i < n ; i++ )
  {
    for( scan = table->table[i] ; scan ; scan = scan->suivant )
    {
      nouv = nouvhashelem( table );
      s = thashsignature( scan->mot, elem );
       
      nouv->suivant = new[ s ];
      new[s]        = nouv;
      nouv->mot     = scan->mot;
      nouv->ptr     = scan->ptr;
    }
    
    for( scan = table->table[i] ; scan ; scan = suiv )
    {
      suiv = scan->suivant;
      liberehashelem( table, scan );
    }
  }

  mbkfree( table->table );
  table->table  = new;
  table->entree = elem;
}
