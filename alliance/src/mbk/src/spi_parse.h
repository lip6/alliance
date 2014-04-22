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

#ifndef SPI_PARSEH
#define SPI_PARSEH

#define SPIEQUI 1664

#define TAILLENOM 4096

typedef struct s_circuit
{
  struct s_noeud	*NOEUD;
  struct s_trans	*TRANS;
  struct s_resi		*RESI;
  struct s_capa		*CAPA;
  struct s_inst		*INST;
  struct s_valim        *VALIM;
  char			*NOM;
  chain_list		*CINTERF;        /* DATA fiels : s_noeud */
  chain_list		*FREE;
  tableint		*INT_NOEUD;
  thash                 *NOM_NOEUD;
  thash			*HASHGNAME;
  thash			*HASHCAPA;
  thash			*HASHRESI;
  thash			*HASHVALIM;
  thash			*HASHINST;
  thash			*HASHTRAN;
  long                   TAILLE;
  char                  *ALLOUENOM;
  int                    RESTENOM;
} circuit;

typedef struct s_noeud
{
  struct s_noeud	*SUIV;
  char			*NOM;
  int			 SIGNAL;
  int			 RCN;
  int			 SPICE;
} noeud;

typedef struct s_trans
{
  struct s_trans	*SUIV;
  struct s_noeud	*DRAIN;
  struct s_noeud	*SOURCE;
  struct s_noeud	*GRILLE;
  struct s_noeud	*SUBST;
  char			 TYPE;
  char			*NOM;
  float			 L;
  float			 W;
  float			 AS;
  float			 AD;
  float			 PS;
  float			 PD;
  float			 X;
  float			 Y;
} trans;

typedef struct s_inst
{
  struct s_inst		*SUIV;
  char			*NOM;
  char			*MODELE;
  chain_list		*IINTERF;
} inst;

typedef struct s_resi
{
  struct s_resi		*SUIV;
  float			 RESI;
  struct s_noeud	*N1;
  struct s_noeud	*N2;
  char			*NOM;
  float			 CAPA;
} resi;

typedef struct s_capa
{
  struct s_capa		*SUIV;
  float			 CAPA;
  struct s_noeud	*N1;
  struct s_noeud	*N2;
  char			*NOM;
} capa;

typedef struct s_valim
{
  struct s_valim        *SUIV;
  float                  TENSION;
  struct s_noeud        *N1;
  struct s_noeud        *N2;
  char                  *NOM;
} valim;

typedef struct s_interf
{
   struct s_interf      *SUIV;
   char                 *NOM;
   chain_list           *GINTERF;        /* DATA field : char* nom des locons */
} ginterf;

#define LONG_LIGNE 16384

typedef struct s_spifile
{
  FILE          *df;
  chain_list    *decomp1;
  chain_list    *decomp2;
  char           file_line[LONG_LIGNE];
  char           file_line1[LONG_LIGNE];
  char           file_line2[LONG_LIGNE];
  int		 msl_line;
  char           *filename;
} spifile ;

/* Fonctions d'allocation mémoire pour un circuit */

void*		spiciralloue  __P(( circuit*, int ));
void		liberecircuit __P(( circuit* ));

/* Fonctions pour parser le fichier au format Spice */

circuit*	lirecircuit     __P(( spifile*, ginterf** ));
chain_list*	lireligne       __P(( spifile* ));
chain_list*	decompligne     __P(( spifile* ));
noeud*		ajoutenoeud     __P(( circuit*, char*, spifile* ));
void		nomenoeud       __P(( circuit*, noeud*, char*, spifile* ));
char*		spicenamealloc  __P(( circuit*, char* ));
void		affvuespice     __P(( circuit* ));
float		spicefloat      __P(( char*, int* ));
chain_list*     recupereinterf  __P(( char* ));
void            loconinterf     __P(( lofig_list*, chain_list* ));
ginterf*        traiteinclude   __P(( ginterf*, char* ));
char*		retireextention __P(( char *nom ));
lofig_list*	recuperemodele  __P(( lofig_list*, char*, chain_list* ));
ginterf*	constinterf     __P(( circuit*, ginterf* ));
ginterf*	spiceloading    __P(( lofig_list*, char*, char, ginterf* ));
spifile*        spifileopen     __P(( char* ));
void            spierror        __P(( int, char*, int ));

/* Fonctions pour convertir la vue spice en vue Alliance */

void		constequi      __P(( circuit*, ginterf* ));
void		constlofig     __P(( circuit*, lofig_list*, ginterf*, char));
void		triecapa       __P(( circuit* ));
long          	float2long     __P(( float ));
ptype_list*     constphinterf  __P(( ptype_list*, ginterf* ));
char*           spi_devect     __P(( char* ));

int             nodenameisequi __P(( char*, char* ));
void            stopchainsepar __P(( char* ));
void            spi_dump_equi  __P(( char*, ptype_list* ));
void            taillevuespice __P(( circuit* ));
#endif
