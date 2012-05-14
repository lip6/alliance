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

#ifndef SPI_DRIVER
#define SPI_DRIVER

#define SPI_DRIVER_PTYPE 6809

/* Capacite minimum acceptable par Spice en pf */
#define CAPAMINI 0.000001
/* Resistance minimum acceptable par Spice en ohm */
#define RESIMINI 0.001

typedef struct sconvindex
{
  losig_list    *sig;           /* Pointeur sur le signal */
  int            premier;       /* Index du premier noeud sur le signal */
} convindex;

/* NOTE : 
  Le champs premier contient le numerot du premier noeud dans le fichier
  Spice. Dans les vues RCN, le premier numérot de noeud est le 1. Le numérot
  de noeud Spice est donc donné par la relation : 
      noeud_spice = noeud_rcn + premier - 1
*/

char*  spinamednode         __P(( losig_list *losig, long node ));
void   spi_vect             __P(( char* ));
void   cherche_alim         __P(( lofig_list *ptfig, char **vdd, char **vss ));
void   sortrcn              __P(( lofig_list *ptfig, FILE *df, char *vss ));
void   signalnoeud          __P(( lofig_list *ptfig ));
int    sortconnecteur       __P(( FILE *df, locon_list *c, int ));
void   sortnet              __P(( lofig_list *ptfig, FILE *df ));
void   sortinstance         __P(( lofig_list *ptfig, FILE *df ));
void   sorttransistormos    __P(( lofig_list *ptfig,
                                  FILE *df,
                                  char *vss,
                                  char *vdd
                               ));
void   sortcircuit          __P(( lofig_list *ptfig, FILE *df ));
void   tooutput             __P(( FILE *, ...));
int    sortconnecteur_ordre __P(( FILE *df, chain_list*, locon_list*, int ));

#endif
