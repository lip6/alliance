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

#ident "$Author: fred $ $Date: 2002/03/13 17:58:33 $ $Revision: 1.1 $"

/*******************************************************************************
*                                                                              *
*  Tool        : Spice parser / driver v 7.00                                  *
*  Author(s)   : Gregoire AVOT                                                 *
*  Updates     : March, 18th 1998                                              *
*                                                                              *
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include <mut.h>
#include <mlo.h>
#include <rcn.h>
#include "msl.h"
#include "spi_int.h"
#include "spi_hash.h"
#include "spi_parse.h"
#include "spi_msg.h"
#include "spi_global.h"

/* Fonctions dynamiques */
static int	 (*ext_handler)() = NULL;
static int	 (*cmp_handler)() = NULL;

/* Variables globales MSL */
lofig_list	*msl_ptfig;
int		 msl_figloaded;
char*		 msl_figname;
char*		 msl_subckt;
int              msl_line;

char              SPI_SEPAR           ;
char             *SPI_NETNAME         ;
char              SPI_ONE_NODE_NORC   ;
char              SPI_MERGE           ;
char              SPI_VERBOSE         ;
char              SPI_FILE            ;
char              SPI_CHECK_NAME      ;
char              SPI_AUTO_LOAD       ;
char              SPI_NODE_EQUI_DRIVE ;
char              SPI_AFF_UNK         ;

/******************************************************************************/

void		spiceloadlofig(ptfig, name, mode)
lofig_list      *ptfig;
char            *name;
char             mode;
{
  char		*env;
  ginterf	*teteinterf;
  ginterf	*effinterf;
  ginterf	*scaninterf;
  chain_list	*scanchain;
  char          *spicename;

  /* Comportement par defaut */
 
  SPI_SEPAR            = '.'   ;
  SPI_NETNAME          = "NET" ;
  SPI_MERGE            = 1     ;
  SPI_VERBOSE          = 0     ;
  SPI_FILE             = 0     ;
  SPI_ONE_NODE_NORC    = 0     ;
  SPI_CHECK_NAME       = 0     ;
  SPI_AUTO_LOAD        = 0     ;
  SPI_NODE_EQUI_DRIVE  = 0     ;
  SPI_AFF_UNK          = 1     ;
  
  spi_init_lang();
 
  env = mbkgetenv( "MBK_SPI_NETNAME" );

  if( env )
    SPI_NETNAME = env;
 
  env = mbkgetenv( "MBK_SPI_SEPAR" );
  
  if( env )
  {
    switch( strlen( env ) )
    {
      case 0:
        SPI_SEPAR = 0;
        SPI_MERGE = 0;
        break;
      case 1:
        SPI_SEPAR = *env;
        break;
      default :
        fflush( stdout );
        fprintf( stderr, "%s.\n", SPIMSG(7) );
        EXIT(1);
        break;
    }
  }

  env = mbkgetenv( "MBK_SPI_CHECK_NAME" );

  if( env )
    SPI_CHECK_NAME = 1;

  env = mbkgetenv( "MBK_SPI_AUTO_LOAD" );

  if( env )
    SPI_AUTO_LOAD = 1;

  /* Variables d'environnement cachees */
  
  /* MBK_SPI_MERGE. Si positionnee, on n'effectue pas
   * de regroupement de locon sur les noms toto, toto.1, toto.2, toto.3...
   */
  
  env = mbkgetenv( "MBK_SPI_MERGE" );

  if( env )
    SPI_MERGE = 0;
  
  env = mbkgetenv( "MBK_SPI_FILE" );

  if( env )
    SPI_FILE = 1;

  env = mbkgetenv( "MBK_SPI_VERBOSE" );

  if( env )
    SPI_VERBOSE = 1;

  env = mbkgetenv( "MBK_SPI_ONE_NODE_NORC" );

  if( env )
    SPI_ONE_NODE_NORC = 1;

  env = mbkgetenv( "MBK_SPI_NODE_EQUI_DRIVE" );

  if( env )
    SPI_NODE_EQUI_DRIVE = 1;

  env = mbkgetenv( "MBK_SPI_NO_AFF_UNK" );

  if( env )
    SPI_AFF_UNK = 0;
  
  if( SPI_VERBOSE )
  {
    printf( "Parser Spice compile le %s a %s\n", __DATE__, __TIME__ );
    printf( "Revision     : %s\n", "$Revision: 1.1 $" );
    printf( "Date         : %s\n", "$Date: 2002/03/13 17:58:33 $"     );
    
    printf( "Separateur   : '%c'\n", SPI_SEPAR  );
    printf( "Nom de noeud : %s\n", SPI_NETNAME  );
    printf( "Regroupement : %s\n", SPI_MERGE ? "Oui" : "Non" );
    printf( "Noms         : %s\n", SPI_CHECK_NAME ? "Oui" : "Non" );
    printf( 
 "Ne cree pas les vues RC sur les reseaux ne contenant qu'un seul noeud : %s\n",
            SPI_ONE_NODE_NORC ? "Oui" : "Non"
          );
  }
  
  if( mode != 'A'  )
  {
    fflush( stdout );
    fprintf( stderr, "%s\n", SPIMSG(0) );
    fprintf( stderr, "%s : '%c'\n", SPIMSG(8), mode );
    EXIT(1);
  }

  spicename = (char*)mbkalloc( sizeof(char)*( strlen(name)+5 ));
  sprintf( spicename, "%s.%s", name, IN_LO );

  msl_figname = name;
  msl_ptfig   = NULL;

  teteinterf  = NULL;
  
  teteinterf = spiceloading( ptfig, spicename, mode, teteinterf );

  if( teteinterf )
  {
    effinterf = NULL;
    for( scaninterf = teteinterf; scaninterf ; scaninterf = scaninterf->SUIV )
    {
      if( effinterf )
      {
	free( effinterf->NOM );
        for( scanchain = effinterf->GINTERF ;
  	     scanchain ;
  	     scanchain = scanchain->NEXT )
          mbkfree( scanchain->DATA );
	freechain( effinterf->GINTERF );
        mbkfree( effinterf );
      }
      effinterf = scaninterf;
    }
    if( effinterf )
    {
      free( effinterf->NOM );
      for( scanchain = effinterf->GINTERF ;
        scanchain ;
  	scanchain = scanchain->NEXT )
      mbkfree( scanchain->DATA );
      freechain( effinterf->GINTERF );
      mbkfree( effinterf );
    }
  }

  mbkfree( spicename );
}

spifile*         spifileopen( name )
char            *name;
{
  spifile       *pt;

  if( SPI_FILE )
  {
    fflush( stdout );
    fprintf( stderr, "%s : %s\n", SPIMSG(53), name );
  }
  
  pt          = mbkalloc( sizeof( *pt ) );
  pt->df      = mbkfopen( name, NULL, READ_TEXT );
  pt->decomp1 = NULL;
  pt->decomp2 = NULL;
  pt->filename = name;
  pt->msl_line = -1;
  *(pt->file_line) = '\0';
  

  if( ! pt->df )
  {
    fflush( stdout );
    fprintf( stderr, "%s\n", SPIMSG(0) );
    fprintf( stderr, "%s %s.\n", SPIMSG(9), name );
    EXIT(1);
  }

  return( pt );
}

void             spifileclose( pt )
spifile         *pt;
{
  fclose( pt->df );
  
  if( pt->decomp1 || pt->decomp2 )
  {
    fflush( stdout );
    fprintf( stderr, "%s\n", SPIMSG(0) );
    fprintf( stderr, "%s.\n", SPIMSG(10) );
    EXIT(1);
  }

  mbkfree( pt );
}

ginterf*	spiceloading(ptfig, name, mode, teteinterf )
lofig_list	*ptfig;
char		*name;
char             mode;
ginterf         *teteinterf;
{
  circuit	        *ptcir;
  spifile               *df;
  lofig_list	        *newfig;

  df = spifileopen( name );


  while( ( ptcir = lirecircuit( df, &teteinterf ) ) )
  {
    constequi( ptcir, teteinterf );
    teteinterf = constinterf( ptcir, teteinterf );

    if( SPI_VERBOSE )
      taillevuespice( ptcir );

    if( ptfig && strcasecmp( ptcir->NOM, ptfig->NAME ) == 0 )
    {
      if( SPI_VERBOSE )
        printf( "Construction figure pere.\n" );

      constlofig( ptcir, ptfig, teteinterf, mode );
      msl_ptfig = ptfig;
      ptfig->USER = constphinterf( ptfig->USER, teteinterf );
    }
    else
    {
      /* Si la figure est vide, on ne construit pas la lofig correspondante */
      if( ptcir->RESI  ||
          ptcir->VALIM ||
          ptcir->TRANS ||
          ptcir->CAPA  ||
          ptcir->INST
        )
      {
        if( SPI_VERBOSE )
          printf( "Construction figure fille %s.\n", ptcir->NOM );

        /* figure fille ou n'ayant rien a voir avec ptfig */
        newfig = addlofig( ptcir->NOM );

        constlofig( ptcir, newfig, teteinterf, mode );
        msl_ptfig = newfig;
        newfig->USER = constphinterf( newfig->USER, teteinterf );
      }
      else
        if( SPI_VERBOSE )
          printf( "Ne construit pas la figure %s.\n", ptcir->NOM );
    }

    if( SPI_VERBOSE )
      printf( "Taille memoire occupee par le subckt %s : %ld\n",
              ptcir->NOM,
              ptcir->TAILLE
            );

    liberecircuit( ptcir );
  }

  spifileclose( df );

  if( cmp_handler )
  {
    msl_figloaded = FALSE;
    msl_subckt    = NULL;
    msl_line      = df->msl_line;
    cmp_handler();
  }

  return( teteinterf );
}

/******************************************************************************/

ptype_list*      constphinterf( pt, interface )
ptype_list      *pt;
ginterf         *interface;
{
  chain_list    *tete_ph_interf;
  chain_list    *scan;
  char          *nomdevec;
 
  tete_ph_interf=NULL;

  for( scan = interface->GINTERF ; scan ; scan = scan->NEXT )
  {
    nomdevec = spi_devect( (char*)(scan->DATA) );
    tete_ph_interf = addchain( tete_ph_interf, namealloc( nomdevec ) );
    mbkfree( nomdevec );
  }
   
  return( addptype( pt, PH_INTERF, reverse( tete_ph_interf )) );
}

/******************************************************************************/

ginterf*	constinterf( ptcir, teteinterf )
circuit		*ptcir;
ginterf		*teteinterf;
{
  ginterf	*ninterf;
  chain_list	*scaninterf;
  char		**tabnom;
  int		 i;
  int		 nb;
  noeud		*ptnoeud;
  noeud		*scannoeud;
  char		nom[80];

  ninterf = (ginterf*) mbkalloc( sizeof( ginterf ) );
  ninterf->NOM = mbkstrdup( ptcir->NOM );
  
  ninterf->SUIV = teteinterf;

  /* On cree pour chaque signal de l'interface une chain_list contenant tous
   * les noms possibles pour un signal. Pour chaque signal, le nom retenu en
   * priorite est le premier nommé sur l'interface */

  /* Calcul du nombre d'elements */
  nb = 0;
  for( scaninterf = ptcir->CINTERF ;
       scaninterf ;
       scaninterf = scaninterf->NEXT
     )
  {
    ptnoeud = (noeud*) scaninterf->DATA;

    if( ptnoeud->SIGNAL > nb )
      nb = ptnoeud->SIGNAL;
  }

  /* Creation du tableau */
  tabnom = (char**) mbkalloc( sizeof(char*) * nb );
  for( i = 0 ; i < nb ; i++ )
    tabnom[ i ] = NULL;
  
  /* Remplissage du tableau */
  /* On regarde d'abord l'interface */
  for( scaninterf = ptcir->CINTERF ;
       scaninterf ;
       scaninterf = scaninterf->NEXT
     )
  {
    ptnoeud = (noeud*) scaninterf->DATA;
    if( ptnoeud->NOM && ! tabnom[ ptnoeud->SIGNAL - 1 ] )
    {
      tabnom[ ptnoeud->SIGNAL - 1 ] = ptnoeud->NOM;
    }
  }
  /* Puis les noeuds constituant les signaux */

  for( scaninterf = ptcir->CINTERF ;
       scaninterf ;
       scaninterf = scaninterf->NEXT
     )
  {
    ptnoeud = (noeud*) scaninterf->DATA;
    if( ! tabnom[ ptnoeud->SIGNAL - 1 ] )
    {
      for( scannoeud = ptcir->NOEUD ; scannoeud ; scannoeud = scannoeud->SUIV )
      {
        if( scannoeud->SIGNAL == ptnoeud->SIGNAL && scannoeud->NOM )
        {
          /* Il est possible que le nom de signal soit deja utilise : 
	   * on le verifie*/
          for( i = 0 ; i < nb ; i++ )
            if( tabnom[ i ] == scannoeud->NOM )
 	      break;

          if( i == nb )
	  {
            tabnom[ ptnoeud->SIGNAL - 1 ] = scannoeud->NOM;
	    break;
	  }
	}
      }
    }
  }
  
  /* finallement on cree un nom de connecteurs */
  i=0;
  for( scaninterf = ptcir->CINTERF ;
       scaninterf ;
       scaninterf = scaninterf->NEXT
     )
  {
    ptnoeud = (noeud*) scaninterf->DATA;
    if( tabnom[ ptnoeud->SIGNAL - 1 ] == NULL )
    {
      sprintf( nom, "SPICE_C_%d",i++);
      tabnom[ ptnoeud->SIGNAL - 1 ] = spicenamealloc( ptcir, nom );
    }
  }

  /* On cree la chainlist de interf */
  
  ninterf->GINTERF = NULL;
  for( scaninterf = ptcir->CINTERF ;
       scaninterf ;
       scaninterf = scaninterf->NEXT
     )
  {
    ptnoeud = (noeud*) scaninterf->DATA;
    strcpy( nom, tabnom[ ptnoeud->SIGNAL -1 ] );
    
    if( SPI_MERGE )
      stopchainsepar( nom );

    ninterf->GINTERF = addchain( ninterf->GINTERF, 
                                 mbkstrdup( nom )
                               );
  }
  
  ninterf->GINTERF = reverse( ninterf->GINTERF );

  mbkfree( tabnom );

  return( ninterf );
}

/******************************************************************************/

circuit*	lirecircuit( df, teteinterf )
spifile		*df;
ginterf         **teteinterf;
{
  char		complet;
  char		debut;
  chain_list	*ligne;
  chain_list	*elem, *elem1, *elem2;
  float		valeur;
  circuit	*ptcir;
  noeud		*ptnoeud;
  chain_list	*sc1, *sc2, *prev;
  resi		*ptresi, *scanresi;
  capa		*ptcapa, *scancapa;
  trans		*pttrans, *scantrans;
  inst		*ptinst, *scaninst;
  valim         *ptvalim, *scanvalim;
  int		nbelem;
  int		status;
  int		i;
  int		traitecomment;
  float         com_x,com_y;
  ginterf      *scaninterf;
  char          buf[1024];
  char          tt;

  debut   = 0;
  complet = 0;
  ptcir   = NULL;

  while( !complet )
  {
    ligne = lireligne( df );
    traitecomment = TRUE;

    if( !ligne ) /* fin de fichier */
      break;

    com_x = 0.0;
    com_y = 0.0;

    /* On récupère les paramètres non standards : $X, $Y */
    for( sc1 = ligne; sc1; sc1 = sc1->NEXT )
    {
      if( strcasecmp( (char*)(sc1->DATA), "$X" ) == 0 &&
          sc1->NEXT &&
          sc1->NEXT->NEXT
        )
      {
        if( strcmp( (char*)(sc1->NEXT->DATA), "=" ) )
          continue;
        valeur = spicefloat( (char*)(sc1->NEXT->NEXT->DATA) ,&status );
        if( status == -1 )
          continue;
        com_x = valeur;
      }
      else
      if( strcasecmp( (char*)(sc1->DATA), "$Y" ) == 0 &&
          sc1->NEXT &&
          sc1->NEXT->NEXT
        )
      {
        if( strcmp( (char*)(sc1->NEXT->DATA), "=" ) )
          continue;
        valeur = spicefloat( (char*)(sc1->NEXT->NEXT->DATA) ,&status );
        if( status == -1 )
          continue;
        com_y = valeur;
      }
    }

    prev = NULL;
   
    for( sc1 = ligne; sc1; sc1 = sc1->NEXT )
    {
      if( *((char*)(sc1->DATA)) == '$' )
        break;
      prev = sc1;
    }
    
    if( sc1 )
    {
      for( sc2 = sc1 ; sc2 ; sc2 = sc2->NEXT )
        mbkfree( sc2->DATA );
        
      if( prev )
        prev->NEXT = NULL;
        
      freechain( sc1 );
    }

    if( ! prev )
      ligne = NULL;
      
    if( !ligne )
    {
    }
    else
    if( strcasecmp( (char*)ligne->DATA, ".INCLUDE" ) == 0 )
    {
      if( ptcir )
        spierror( 2, df->filename, df->msl_line );
      
      elem = ligne->NEXT;

      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      if( elem->NEXT )
        spierror( 14, df->filename, df->msl_line );

      *teteinterf = traiteinclude( *teteinterf, (char*)elem->DATA );
    }
    else
    if( strcasecmp( (char*)ligne->DATA, ".SUBCKT" ) == 0 )
    {
      if( ptcir )
        spierror( 15, df->filename, df->msl_line );

      ptcir            = (circuit*)mbkalloc( sizeof( circuit ) );
      ptcir->NOEUD     = NULL;
      ptcir->TRANS     = NULL;
      ptcir->RESI      = NULL;
      ptcir->CAPA      = NULL;
      ptcir->INST      = NULL;
      ptcir->NOM       = NULL;
      ptcir->CINTERF   = NULL;
      ptcir->FREE      = NULL;
      ptcir->VALIM     = NULL;
      ptcir->INT_NOEUD = creattableint();
      ptcir->NOM_NOEUD = creatthash( );
      ptcir->HASHGNAME = creatthash( );
      ptcir->HASHCAPA  = creatthash( );
      ptcir->HASHRESI  = creatthash( );
      ptcir->HASHVALIM = creatthash( );
      ptcir->HASHINST  = creatthash( );
      ptcir->HASHTRAN  = creatthash( );
      ptcir->TAILLE    = sizeof( circuit );
      ptcir->RESTENOM  = 0;

      elem = ligne->NEXT;

      if( !elem )
        spierror( 16, df->filename, df->msl_line );

      ptcir->NOM = (char*)spiciralloue( ptcir, 
                           sizeof(char)*( strlen( (char*)(elem->DATA) ) + 1 )
                                       );
      strcpy( ptcir->NOM, (char*)( elem->DATA ) );

      /* Pour verifier qu'on a pas plusieurs fois le meme noeud, on
       * met un 1 dans le champs RCN */
      for( elem = elem->NEXT; elem; elem = elem->NEXT )
      {
        ptnoeud       = ajoutenoeud( ptcir, (char*)(elem->DATA), df );

        if( ptnoeud->RCN )
        {
          fflush( stdout );
          fprintf( stderr, "%s.\n", SPIMSG(0) );
          fprintf( stderr, "%s : %s - %d\n", SPIMSG(6),
                                             ptnoeud->NOM,
                                             ptnoeud->SPICE
                 );
          fprintf( stderr, "%s : %s, %s : %d\n", SPIMSG(11), df->filename,
                                                 SPIMSG(12), df->msl_line
                 );
          exit( 1 );
        }
        ptnoeud->RCN = 1;
          
        ptcir->CINTERF = addchain( ptcir->CINTERF, ptnoeud );
      }

      for( sc1 = ptcir->CINTERF ; sc1 ; sc1 = sc1->NEXT )
        ((noeud*)(sc1->DATA))->SPICE = -1;

      ptcir->CINTERF = reverse( ptcir->CINTERF );

    }
    else
    if( *( (char*)(ligne->DATA) ) == '*' )
    {
      traitecomment = FALSE;
      
      elem = ligne->NEXT;

      if( ptcir && elem )
      {

        if( strcasecmp( (char*)(elem->DATA), SPI_NETNAME ) == 0 )
        {
	  traitecomment = TRUE;
          elem = elem->NEXT;
          sc1  = elem;
          sc2  = NULL;

          for( ; elem && strcasecmp( (char*)(elem->DATA), "=" ) != 0;
               elem = elem->NEXT )
          {
            for( i=0; ((char*)(elem->DATA))[i] >= '0' && 
                      ((char*)(elem->DATA))[i] <= '9'     ; i++ ) ;

            if( ((char*)(elem->DATA))[i] != 0 )
              spierror( 17, df->filename, df->msl_line );

            ptnoeud = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
            sc2     = addchain( sc2, ptnoeud );
          }

          if( !elem )
            spierror( 18, df->filename, df->msl_line );

          if( elem == sc1 )
            spierror( 19, df->filename, df->msl_line );

          elem = elem->NEXT;	/* on passe le = */

          if( !elem )
            spierror( 20, df->filename, df->msl_line );

          for( sc1 = sc2; sc1; sc1 = sc1->NEXT)
            nomenoeud( ptcir, (noeud*)(sc1->DATA), (char*)(elem->DATA), df );
          freechain(sc2);

          elem = elem->NEXT;
          if( elem )
            spierror( 21, df->filename, df->msl_line );
        }
      }

      if( ext_handler && ! traitecomment )
      {
        msl_figloaded = ( ptcir ? TRUE : FALSE );
	msl_subckt    = ( ptcir ? ptcir->NOM : NULL );
        msl_line      = df->msl_line;
        ext_handler( df->file_line1 );
      }
    }
    else
    if( strcasecmp( (char*)(ligne->DATA), ".ENDS" ) == 0 )
    {
      if( !ptcir )
        spierror( 22, df->filename, df->msl_line );

      if( !ligne->NEXT )
      {
        /*spierror( 23, df->filename, df->msl_line );*/
      }
      else
      {
        if( strcasecmp( (char*)(ligne->NEXT->DATA), ptcir->NOM ) != 0 )
          spierror( 24, df->filename, df->msl_line );

        if( ligne->NEXT->NEXT )
          spierror( 14, df->filename, df->msl_line );
      }
      complet = 1;

      /* Vérification de l'existance des modèles pour les instances */
      if( SPI_AUTO_LOAD )
      {
        for( scaninst = ptcir->INST ; scaninst ; scaninst = scaninst->SUIV )
        {
          for( scaninterf = *teteinterf ;
               scaninterf ;
               scaninterf = scaninterf->SUIV
             )
          {
            if( strcasecmp( scaninterf->NOM, scaninst->MODELE )==0 )
              break;
          }

          if( !scaninterf )
          {
            sprintf( buf,"%s.%s", scaninst->MODELE, IN_LO );
            *teteinterf = traiteinclude( *teteinterf, buf );
          }
        }
      }
    }
    else
    if( ( *((char*)ligne->DATA)=='M' || *((char*)ligne->DATA)=='m' ) && ptcir )
    {
      pttrans        = (trans*) spiciralloue( ptcir, sizeof( trans ) );
      pttrans->L     = 0.0;
      pttrans->W     = 0.0;
      pttrans->AS    = 0.0;
      pttrans->AD    = 0.0;
      pttrans->PS    = 0.0;
      pttrans->PD    = 0.0;

      if( *( (char*)( ligne->DATA ) + 1 ) == 0 )
        spierror( 25, df->filename, df->msl_line );

      pttrans->NOM = spicenamealloc( ptcir, (char*)(ligne->DATA) + 1  );

      scantrans = getthashelem( pttrans->NOM, ptcir->HASHTRAN, NULL );
    
      if( scantrans )
      {  
        if( SPI_CHECK_NAME )
          spierror( 26, df->filename, df->msl_line );
      }
      else
        addthashelem( pttrans->NOM, pttrans, ptcir->HASHTRAN );

      pttrans->SUIV  = ptcir->TRANS;
      ptcir->TRANS   = pttrans;

      elem = ligne->NEXT;	/* drain */

      if( !elem )
        spierror( 27, df->filename, df->msl_line );

      ptnoeud        = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      pttrans->DRAIN = ptnoeud;

      elem = elem->NEXT;	/* grille */

      if( !elem )
        spierror( 28, df->filename, df->msl_line );

      ptnoeud         = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      pttrans->GRILLE = ptnoeud;

      elem = elem->NEXT;	/* source */

      if( !elem )
        spierror( 29, df->filename, df->msl_line );

      ptnoeud         = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      pttrans->SOURCE = ptnoeud;

      elem = elem->NEXT;	/* bulk ou type du transistor */
       
      if( !elem )
        spierror( 30, df->filename, df->msl_line );

      tt = spitranstype( (char*)(elem->DATA) );
      if( tt != SPI_UNK_TRANS_TYPE )
      {
        pttrans->TYPE = tt;
        pttrans->SUBST  = NULL;
      }
      else
      {
        ptnoeud         = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
        pttrans->SUBST  = ptnoeud;

        elem = elem->NEXT;	/* type du transistor */

        if( !elem )
          spierror( 31, df->filename, df->msl_line );

        tt = spitranstype( (char*)(elem->DATA) );
        if( tt != SPI_UNK_TRANS_TYPE )
          pttrans->TYPE = tt;
        else
          spierror( 32, df->filename, df->msl_line );
      }

      /* Le reste des parametres est de la forme xxx = yyy */

      while( (elem = elem->NEXT ) )
      {
        elem1 = elem->NEXT;
        elem2 = ( elem1 ? elem1->NEXT : NULL );

        if( !elem2 )
          spierror( 33, df->filename, df->msl_line );

        if( strcmp( (char*)(elem1->DATA), "=" ) )
          spierror( 33, df->filename, df->msl_line );

        valeur = spicefloat( (char*)(elem2->DATA) ,&status );
        if( status == -1 )
          spierror( 34, df->filename, df->msl_line );

        if( strcasecmp( (char*)(elem->DATA), "L" ) == 0 )
          pttrans->L  = valeur;
        else
        if( strcasecmp( (char*)(elem->DATA), "W" ) == 0 )
          pttrans->W  = valeur;
        else
        if( strcasecmp( (char*)(elem->DATA), "AS" ) == 0 )
          pttrans->AS = valeur;
        else
        if( strcasecmp( (char*)(elem->DATA), "AD" ) == 0 )
          pttrans->AD = valeur;
        else
        if( strcasecmp( (char*)(elem->DATA), "PS" ) == 0 )
          pttrans->PS = valeur;
        else
        if( strcasecmp( (char*)(elem->DATA), "PD" ) == 0 )
          pttrans->PD = valeur;
        else
        {
          fflush( stdout );
          fprintf( stderr, "%s.\n", SPIMSG(0) );
          fprintf( stderr, "%s. %s %s. %s %d.\n",
                   SPIMSG(1),
                   SPIMSG(11),
                   df->filename,
                   SPIMSG(12),
                   df->msl_line
                 );
          fprintf( stderr, "%s : %s.\n", SPIMSG(35), (char*)(elem->DATA) );
          EXIT(1);
        }
        elem = elem->NEXT->NEXT;
      }
      pttrans->X = com_x;
      pttrans->Y = com_y;
    }
    else
    if( ( *((char*)ligne->DATA)=='R' || *((char*)ligne->DATA)=='r' ) && ptcir )
    {
      ptresi       = (resi*) spiciralloue( ptcir, sizeof(resi) );
      
      if( *( (char*)( ligne->DATA ) +1 ) == 0 )
        spierror( 36, df->filename, df->msl_line );

      ptresi->NOM = spicenamealloc( ptcir, (char*)(ligne->DATA) + 1  );

      scanresi = getthashelem( ptresi->NOM, ptcir->HASHRESI, NULL );
      
      if( scanresi )
      {
        if( SPI_CHECK_NAME )
          spierror( 37, df->filename, df->msl_line );
      }
      else
        addthashelem( ptresi->NOM, ptresi, ptcir->HASHRESI );

      ptresi->SUIV = ptcir->RESI;
      ptcir->RESI  = ptresi;

      elem = ligne->NEXT;	/* first node */

      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      ptnoeud        = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      ptresi->N1     = ptnoeud;
 
      elem = elem->NEXT;	/* second node */

      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      ptnoeud        = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      ptresi->N2     = ptnoeud;

      elem = elem->NEXT;	/* R value */
      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      valeur = spicefloat( (char*)(elem->DATA) ,&status );
      if( status == -1 )
        spierror( 34, df->filename, df->msl_line );

      ptresi->RESI = valeur;

      elem = elem->NEXT;	/* no more parameters */
      if( elem )
        spierror( 14, df->filename, df->msl_line );
    }
    else
    if( ( *((char*)ligne->DATA)=='C' || *((char*)ligne->DATA)=='c' ) && ptcir )
    {
      ptcapa     = (capa*) spiciralloue( ptcir, sizeof(capa) );
      
      if( *( (char*)( ligne->DATA ) +1 ) == 0 )
        spierror( 38, df->filename, df->msl_line );

      ptcapa->NOM = spicenamealloc( ptcir, (char*)(ligne->DATA) + 1  );

      scancapa = getthashelem( ptcapa->NOM, ptcir->HASHCAPA, NULL );
      if( scancapa )
      {
        if( SPI_CHECK_NAME )
          spierror( 39, df->filename, df->msl_line );
      }
      else
        addthashelem( ptcapa->NOM, ptcapa, ptcir->HASHCAPA);


      ptcapa->SUIV = ptcir->CAPA;
      ptcir->CAPA  = ptcapa;

      elem = ligne->NEXT;	/* first node */

      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      ptnoeud        = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      ptcapa->N1     = ptnoeud;
 
      elem = elem->NEXT;	/* second node */

      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      ptnoeud        = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      ptcapa->N2     = ptnoeud;

      elem = elem->NEXT;	/* C value */
      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      valeur = spicefloat( (char*)(elem->DATA) ,&status );

      if( status == -1 )
        spierror( 34, df->filename, df->msl_line );

      /* Les valeurs dans Alliance sont en pif */
      ptcapa->CAPA = valeur * 1e12 ;

      elem = elem->NEXT;	/* no more parameters */

      if( elem )
        spierror( 14, df->filename, df->msl_line );
    }
    else
    if( ( *((char*)ligne->DATA)=='V' || *((char*)ligne->DATA)=='v' ) && ptcir )
    {
      ptvalim       = (valim*) spiciralloue( ptcir, sizeof(valim) );
      
      if( *( (char*)( ligne->DATA ) +1 ) == 0 )
        spierror( 36, df->filename, df->msl_line );

      ptvalim->NOM = spicenamealloc( ptcir, (char*)(ligne->DATA) + 1  );

      scanvalim = getthashelem( ptvalim->NOM, ptcir->HASHVALIM, NULL );
      
      if( scanvalim )
      {
        if( SPI_CHECK_NAME )
          spierror( 37, df->filename, df->msl_line );
      }
      else
        addthashelem( ptvalim->NOM, ptvalim, ptcir->HASHVALIM );

      ptvalim->SUIV = ptcir->VALIM;
      ptcir->VALIM  = ptvalim;

      elem = ligne->NEXT;	/* first node */

      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      ptnoeud        = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      ptvalim->N1     = ptnoeud;
 
      elem = elem->NEXT;	/* second node */

      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      ptnoeud        = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
      ptvalim->N2     = ptnoeud;

      elem = elem->NEXT;	/* R value */
      if( !elem )
        spierror( 13, df->filename, df->msl_line );

      valeur = spicefloat( (char*)(elem->DATA) ,&status );
      if( status == -1 )
        spierror( 34, df->filename, df->msl_line );

      ptvalim->TENSION = valeur;

      elem = elem->NEXT;	/* no more parameters */
      if( elem )
        spierror( 14, df->filename, df->msl_line );
    }
    else
    if( ( *(char*)(ligne->DATA) == 'X' || *(char*)(ligne->DATA) == 'x' ) &&
        ptcir                                                               )
    {
      ptinst          = (inst*)spiciralloue( ptcir, sizeof(inst) );
      ptinst->IINTERF = NULL;

      if( *( ((char*)(ligne->DATA)) + 1) == 0 )
        spierror( 40, df->filename, df->msl_line );

      ptinst->NOM = spicenamealloc( ptcir, ((char*)(ligne->DATA)) + 1 );
      

      scaninst = getthashelem( ptinst->NOM, ptcir->HASHINST, NULL ); 
      if( scaninst )
        spierror( 41, df->filename, df->msl_line );

      ptinst->SUIV   = ptcir->INST;
      ptcir->INST    = ptinst;
      addthashelem( ptinst->NOM, ptinst, ptcir->HASHINST );

      for( nbelem=0, elem = ligne->NEXT ; elem ; elem = elem->NEXT, nbelem++ );

      if( nbelem <= 1 )
        spierror( 13, df->filename, df->msl_line );

      for( elem = ligne->NEXT; elem->NEXT; elem = elem->NEXT )
      {
        ptnoeud         = ajoutenoeud( ptcir, (char*)(elem->DATA), df );
        ptinst->IINTERF = addchain( ptinst->IINTERF, ptnoeud );
      }
      ptinst->IINTERF = reverse( ptinst->IINTERF );

      ptinst->MODELE = spicenamealloc( ptcir, (char*)elem->DATA );
    }
    else
    {
      if( SPI_AFF_UNK )
      {
        fflush( stdout );
        fprintf( stderr, "%s\n", SPIMSG(0) );
        fprintf( stderr, "%s. %s %s. %s %d :\n",
                 SPIMSG(42),
                 SPIMSG(11),
                 df->filename,
                 SPIMSG(12),
                 df->msl_line
               );
        fprintf( stderr, "%s\n", (char*)ligne->DATA );
      }
    }

    if( ligne )
    {
      for( sc1 = ligne; sc1; sc1 = sc1->NEXT )
        mbkfree( sc1->DATA );

      freechain( ligne );
    }
  }


  return( ptcir );
}

/******************************************************************************/

ginterf*	traiteinclude( instance, nom )
ginterf		*instance;
char            *nom;
{
  ginterf	*sci;
  ginterf	*nouvinterf;
  
  for( sci = instance ; sci ; sci = sci->SUIV )
  {
    if( strcasecmp( sci->NOM, nom ) == 0 )
    {
      fflush( stdout );
      fprintf( stderr, "%s\n", SPIMSG(0) );
      fprintf( stderr, "%s : %s.\n", SPIMSG(41), nom );
      EXIT( 1 );
    }
  }

  nouvinterf = spiceloading( NULL, nom, 'A', instance );

  return( nouvinterf );
}

/******************************************************************************/

void            loconinterf( ptfig, tete )
lofig_list      *ptfig;
chain_list      *tete;
{
  chain_list    *sc1;
  chain_list    *sc2;
  char          *nomdevec;
  /*int           sigidx;
  losig_list    *ptsig;

  sigidx = 0;*/
  
  for( sc1 = tete ; sc1 ; sc1 = sc1->NEXT )
  {
    for( sc2 = tete; sc2 != sc1 ; sc2 = sc2->NEXT )
      if( strcasecmp( sc1->DATA, sc2->DATA ) == 0 )
        break;
    if( sc1 == sc2 )
    {
      /*
      sigidx++;
      ptsig = addlosig( ptfig, sigidx, NULL, 'E' );
      */
      nomdevec = spi_devect( (char*)(sc1->DATA) );
      addlocon( ptfig, namealloc(nomdevec), /*ptsig*/ NULL, 'X' );
      mbkfree( nomdevec );
    }
  }

  ptfig->LOCON = (locon_list*)reverse( (chain_list*)ptfig->LOCON );
}

/******************************************************************************/

void		constequi( ptcir, instances )
circuit		*ptcir;
ginterf		*instances;
{
  inst		*scaninst;
  chain_list	*sc1, *sc2, *sc3, *sc4, *sc5 ;
  ptype_list	*pt, *pt2;
  int		idsig = 0 ; /* Warning a la compil */
  int		signal;
  ptype_list	*headsig;
  int		nb;
  int		t;
  ptype_list	*spt;
  noeud		*scannoeud;
  resi		*scanresi;
  valim		*scanvalim;
  ginterf	*sci;
  char		*vdd;
  char		*vss;
  int		vuevss;
  int		vuevdd;
  int		cirvss;
  int		cirvdd;
  chain_list    *scanchain;
  noeud         *node1;
  noeud         *node2;

  signal  = 1;
  headsig = NULL;

  /*	traite les instances						*/

  for( scaninst = ptcir->INST; scaninst; scaninst = scaninst->SUIV )
  {
    for( sci = instances; sci; sci = sci->SUIV )
    {
      if( strcasecmp( sci->NOM, scaninst->MODELE ) == 0 )
        break;
    }

    if( !sci )
    {
      fflush( stdout );
      fprintf( stderr, "%s\n", SPIMSG(0) );
      fprintf( stderr, "%s %s.\n", SPIMSG( 43 ), scaninst->MODELE );
      EXIT(1);
    }

    for( sc1 = sci->GINTERF, sc2 = scaninst->IINTERF ;
         sc1 && sc2 ;
         sc1 = sc1->NEXT, sc2 = sc2->NEXT )
    {
      /* On verifie qu'on a une nouvelle equipotentielle */

      for( sc3 = sci->GINTERF ; sc3 != sc1 ; sc3 = sc3->NEXT )
        if( strcasecmp( sc3->DATA, sc1->DATA ) == 0 )
          break;
      if( sc3 != sc1 )
        continue;

      nb = 0;	/* nombre d'equipotentielle deja existant sur l'interface */

      for( sc3 = sc1, sc4 = sc2 ; sc3 && sc4 ; sc3 = sc3->NEXT, sc4 = sc4->NEXT)
        if( strcasecmp( sc3->DATA, sc1->DATA ) == 0 &&
            ( (noeud*)( sc4->DATA ) )->SIGNAL != 0        )
        {
          nb ++;
          idsig = ( (noeud*)( sc4->DATA ) )->SIGNAL;
        }

      switch( nb )
      {
        case 0 :	/* creation d'une nouvelle equi */

        headsig = addptype( headsig, signal, NULL );

	for( sc3 = sc1, sc4 = sc2 ;
	     sc3 && sc4 ;
	     sc3 = sc3->NEXT, sc4 = sc4->NEXT)
	  if( strcasecmp( sc3->DATA, sc1->DATA ) == 0 )
	  {
            ((noeud*)(sc4->DATA))->SIGNAL = signal;
            headsig->DATA = addchain( headsig->DATA, sc4->DATA );
          }

          signal++;

          break;

	case 1:         /* On reutilise l'equi existante */

        pt = getptype( headsig, idsig );

	for( sc3 = sc1, sc4 = sc2 ;
	     sc3 && sc4 ;
	     sc3 = sc3->NEXT, sc4 = sc4->NEXT)
	  if( strcasecmp( sc3->DATA, sc1->DATA ) == 0 )
	  {
            if( ! ((noeud*)(sc4->DATA))->SIGNAL )
	    {
	      ((noeud*)( sc4->DATA ))->SIGNAL = idsig;
	      pt->DATA = addchain( pt->DATA, sc4->DATA ); 
	    }
          }

	break;

	default :       /* Il y a plus de deux equi existante : il faut
			   les regrouper en une seule */
        pt = getptype( headsig, idsig );

	for( sc3 = sc1, sc4 = sc2 ;
	     sc3 && sc4 ;
	     sc3 = sc3->NEXT, sc4 = sc4->NEXT)
	  if( strcasecmp( sc3->DATA, sc1->DATA ) == 0 )
	  {
	    /* Pas de signal sur le noeud sc4 */
            if( ! ( (noeud*)( sc4->DATA ) )->SIGNAL )
	    {
	      ((noeud*)( sc4->DATA ))->SIGNAL = idsig;
	      pt->DATA = addchain( pt->DATA, sc4->DATA ); 
	    }
	    else
	    /* Si on se trouve sur un autre signal */
            if( ( (noeud*)( sc4->DATA ) )->SIGNAL != idsig )
	    {
	      /* t : index signal source */
	      t   = ( (noeud*)( sc4->DATA ) )->SIGNAL;
	      pt2 = getptype( headsig, t );

	      /* on change ne numero de signal de tous les noeuds du signal 
	       * source */
	      for( sc5 = pt2->DATA ; sc5 ; sc5 = sc5->NEXT )
	        ( (noeud*)( sc5->DATA ) )->SIGNAL = idsig;

	      /* on ajoute la chain_list source a la fin de la chain_list
	       * destination */
	      for( sc5 = (chain_list*)( pt->DATA ) ;
		   sc5->NEXT ;
		   sc5 = sc5->NEXT );
	      sc5->NEXT = (chain_list*)pt2->DATA;

	      /* On libere le signal source */
              headsig = delptype( headsig, t );
	        
	    }
	    /* Cas où on est sur le meme signal : on ne fait rien */
	  }

	break;
      }
    }

    if( sc1 || sc2 )
    {
      fflush( stdout );
      fprintf( stderr, "%s\n", SPIMSG(0) );
      fprintf( stderr, "%s :\n%s %s. %s %s. %s %s.\n", SPIMSG( 44 ),
                                                       SPIMSG( 54 ),
                                                       ptcir->NOM,
                                                       SPIMSG( 55 ),
                                                       scaninst->NOM,
                                                       SPIMSG( 56 ),
                                                       scaninst->MODELE
             );
      EXIT(1);
    }
  }

  /*	traite les resistances : Ca peut s'ameliorer pour aller + vite	*/

  for( scanresi = ptcir->RESI ; scanresi ; scanresi = scanresi->SUIV )
  {
    if( scanresi->N1->SIGNAL == 0 && scanresi->N2->SIGNAL == 0 )
    {
      headsig = addptype( headsig, signal, NULL );
      scanresi->N1->SIGNAL = signal;
      scanresi->N2->SIGNAL = signal;
      headsig->DATA = addchain( headsig->DATA, scanresi->N1 );
      headsig->DATA = addchain( headsig->DATA, scanresi->N2 );
      signal++;
    }
    else
    if( scanresi->N1->SIGNAL != 0 && scanresi->N2->SIGNAL == 0 )
    {
      pt = getptype( headsig, scanresi->N1->SIGNAL );
      pt->DATA = addchain( pt->DATA, scanresi->N2 );
      scanresi->N2->SIGNAL = pt->TYPE;
    }
    else
    if( scanresi->N1->SIGNAL == 0 && scanresi->N2->SIGNAL != 0 )
    {
      pt = getptype( headsig, scanresi->N2->SIGNAL );
      pt->DATA = addchain( pt->DATA, scanresi->N1 );
      scanresi->N1->SIGNAL = pt->TYPE;
    }
    else
    if( scanresi->N1->SIGNAL != scanresi->N2->SIGNAL )
    {
      pt  = getptype( headsig, scanresi->N1->SIGNAL );
      pt2 = getptype( headsig, scanresi->N2->SIGNAL );

      for( sc1 = pt2->DATA ; sc1 ; sc1 = sc1->NEXT )
        ((noeud*)(sc1->DATA))->SIGNAL = pt->TYPE;
      for( sc1 = pt->DATA ; sc1->NEXT ; sc1 = sc1->NEXT ) ;
      sc1->NEXT = pt2->DATA;
      headsig = delptype( headsig, pt2->TYPE );
    }
  }

  for( scanvalim = ptcir->VALIM ; scanvalim ; scanvalim = scanvalim->SUIV )
  {
    if( scanvalim->N1->SIGNAL == 0 && scanvalim->N2->SIGNAL == 0 )
    {
      headsig = addptype( headsig, signal, NULL );
      scanvalim->N1->SIGNAL = signal;
      scanvalim->N2->SIGNAL = signal;
      headsig->DATA = addchain( headsig->DATA, scanvalim->N1 );
      headsig->DATA = addchain( headsig->DATA, scanvalim->N2 );
      signal++;
    }
    else
    if( scanvalim->N1->SIGNAL != 0 && scanvalim->N2->SIGNAL == 0 )
    {
      pt = getptype( headsig, scanvalim->N1->SIGNAL );
      pt->DATA = addchain( pt->DATA, scanvalim->N2 );
      scanvalim->N2->SIGNAL = pt->TYPE;
    }
    else
    if( scanvalim->N1->SIGNAL == 0 && scanvalim->N2->SIGNAL != 0 )
    {
      pt = getptype( headsig, scanvalim->N2->SIGNAL );
      pt->DATA = addchain( pt->DATA, scanvalim->N1 );
      scanvalim->N1->SIGNAL = pt->TYPE;
    }
    else
    if( scanvalim->N1->SIGNAL != scanvalim->N2->SIGNAL )
    {
      pt  = getptype( headsig, scanvalim->N1->SIGNAL );
      pt2 = getptype( headsig, scanvalim->N2->SIGNAL );

      for( sc1 = pt2->DATA ; sc1 ; sc1 = sc1->NEXT )
        ((noeud*)(sc1->DATA))->SIGNAL = pt->TYPE;
      for( sc1 = pt->DATA ; sc1->NEXT ; sc1 = sc1->NEXT ) ;
      sc1->NEXT = pt2->DATA;
      headsig = delptype( headsig, pt2->TYPE );
    }
  }

  if( SPI_MERGE == 1 )
  {
    for( sc1 = ptcir->CINTERF ; sc1 ; sc1 = sc1->NEXT )
    {
      node1 = ( noeud* )( sc1->DATA );
      
      for( sc2 = ptcir->CINTERF ; sc2 != sc1 ; sc2 = sc2->NEXT )
      {
        node2 = ( noeud* )( sc2->DATA );
        if( nodenameisequi( node1->NOM, node2->NOM ) )
          break;
      }

      if( sc1 != sc2 )
        continue;

      for( sc2 = sc1->NEXT ; sc2 ; sc2 = sc2->NEXT )
      {
        node2 = ( noeud* )( sc2->DATA );

        if( nodenameisequi( node1->NOM, node2->NOM ) )
        {
          if( node1->SIGNAL == 0 && node2->SIGNAL == 0 )
          {
            headsig = addptype( headsig, signal, NULL );
            node1->SIGNAL = signal;
            node2->SIGNAL = signal;
            headsig->DATA = addchain( headsig->DATA, node1 );
            headsig->DATA = addchain( headsig->DATA, node2 );
            signal++;
          }
          else
          if( node1->SIGNAL != 0 && node2->SIGNAL == 0 )
          {
            pt = getptype( headsig, node1->SIGNAL );
            pt->DATA = addchain( pt->DATA, node2 );
            node2->SIGNAL = node1->SIGNAL;
          }
          else
          if( node1->SIGNAL == 0 && node2->SIGNAL != 0 )
          {
            pt = getptype( headsig, node2->SIGNAL );
            pt->DATA = addchain( pt->DATA, node1 );
            node1->SIGNAL = node2->SIGNAL;
          }
          else
          if( node1->SIGNAL != node2->SIGNAL )
          {
            pt  = getptype( headsig, node1->SIGNAL );
            pt2 = getptype( headsig, node2->SIGNAL );

            for( sc3 = pt2->DATA ; sc3 ; sc3 = sc3->NEXT )
              ((noeud*)(sc3->DATA))->SIGNAL = node1->SIGNAL;
            for( sc3 = pt->DATA ; sc3->NEXT ; sc3 = sc3->NEXT ) ;
            sc3->NEXT = pt2->DATA;
            headsig = delptype( headsig, pt2->TYPE );
          }
          else
          {
            /* rien a faire : node1->SIGNAL == node2->SIGNAL */
          }
        }
      }
    }
  }
  
  /*	On s'occupe des noeuds libres					*/

  for( scannoeud = ptcir->NOEUD; scannoeud; scannoeud = scannoeud->SUIV )
  {
    if( scannoeud->SIGNAL == 0 )
    {
      headsig = addptype( headsig, signal, addchain( NULL, scannoeud ) );
      scannoeud->SIGNAL = signal++;
      
    }
  }

  /*	Remplie le champ RCN de chaque noeud				*/

  for( spt = headsig ; spt ; spt = spt->NEXT )
  {
    sc1 = (chain_list*)(spt->DATA);
    
    if( sc1->NEXT || SPI_ONE_NODE_NORC == 0 )
    {
      nb = 1;
      for( ; sc1 ; sc1 = sc1->NEXT )
        ((noeud*)(sc1->DATA))->RCN = nb++ ;
    }
    else
    {
      ((noeud*)(sc1->DATA))->RCN = 0;
    }
  }

  /*	Tous les noeuds d'un signal VSS ou VDD ont le nom VSS ou VDD	*/

  vss    = spicenamealloc( ptcir, VSS );
  vdd    = spicenamealloc( ptcir, VDD );
  cirvss = 0;
  cirvdd = 0;
  
  for( spt = headsig ; spt ; spt = spt->NEXT )
  {
    vuevss = vuevdd = 0;
    
    for( sc1 = spt->DATA ; sc1 ; sc1 = sc1->NEXT )
    {
      if( ( ( noeud* )( sc1->DATA ) )->NOM == vss )
        vuevss = 1;
      
      if( ( ( noeud* )( sc1->DATA ) )->NOM == vdd )
        vuevdd = 1;
    }

    if( vuevss && vuevdd )
    {
      fflush( stdout );
      fprintf( stderr, "%s\n", SPIMSG(0) );
      fprintf( stderr, "%s : %s\n", SPIMSG(45), ptcir->NOM );
      EXIT( 1 );
    }

    if( vuevss && cirvss )
    {
      fflush( stdout );
      fprintf( stderr, "%s\n", SPIMSG(0) );
      fprintf( stderr, "%s : %s\n", SPIMSG(46), ptcir->NOM );
      EXIT( 1 );
    }
    
    if( vuevss )
    {
      for( sc1 = spt->DATA ; sc1 ; sc1 = sc1->NEXT )
         ( ( noeud* )( sc1->DATA ) )->NOM = vss;
      cirvss = 1;
    }

    if( vuevdd && cirvdd )
    {
      fflush( stdout );
      fprintf( stderr, "%s\n", SPIMSG(0) );
      fprintf( stderr, "%s : %s\n", SPIMSG(47), ptcir->NOM );
      EXIT( 1 );
    }
    
    if( vuevdd )
    {
      for( sc1 = spt->DATA ; sc1 ; sc1 = sc1->NEXT )
         ( ( noeud* )( sc1->DATA ) )->NOM = vdd;
      cirvdd = 1;
    }
  }

  /*
  if( !cirvss || !cirvdd )
  {
    fflush( stdout );
    fprintf( stderr,
             "*** mbkspi error *** : No vdd or vss nodes in %s\n",
             ptcir->NOM
	   );
    EXIT( 1 );
  }
  */

  /* On renumerote les signaux en partant de 1, c'est plus propre.      */
  idsig = 1;
  for(spt = headsig ; spt ; spt = spt->NEXT )
  {
    if( spt->DATA )
    {
      for( scanchain = (chain_list*)(spt->DATA) ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {
        scannoeud = (noeud*)(scanchain->DATA);
        scannoeud->SIGNAL = idsig;
      }
      idsig++;
    }
  }

  if( SPI_NODE_EQUI_DRIVE )
    spi_dump_equi( ptcir->NOM, headsig );

  /*	libere la liste des noeuds par signal				*/

  for(spt = headsig; spt; spt = spt->NEXT )
    if( spt->DATA )
      freechain( (chain_list*)(spt->DATA) );
  freeptype( headsig );

}

/******************************************************************************/
void spi_dump_equi( char *figname, ptype_list *headsig )
{
  ptype_list *spt;
  chain_list *scan;
  FILE       *ptf;
  noeud      *node;
  
  ptf = mbkfopen( figname, "equi", WRITE_TEXT );
  if( !ptf )
    return;
  
  for( spt = headsig ; spt ; spt = spt->NEXT )
  {
    if( spt->DATA )
    {
      scan = (chain_list*)spt->DATA ;
      fprintf( ptf, "Signal %d.\n", ((noeud*)(scan->DATA))->SIGNAL );
      for( ; scan ; scan = scan->NEXT )
      {
        node = ((noeud*)scan->DATA );
        fprintf( ptf,
                 "  %5d %5d %s\n",
                 node->SPICE,
                 node->RCN,
                 node->NOM ? node->NOM : "-"
               );
      }
    }
  }
  fclose( ptf );
}

/******************************************************************************/

int             nodenameisequi( name1, name2 )
/* Renvoie 1 si les deux noms sont sur la meme equipotentielle */
char            *name1;
char            *name2;
{
  int           end1;
  int           end2;
  int           end1x;
  int           end2x;

  if( name1 == NULL || name2 == NULL )
    return(0);

  /*modif karim pour passer les fichier ck1 ck1.1 ck1.2*/
  /*end1 = strlen( name1 ) - 1 ;*/
  end1x = end1 = strlen( name1 ) - 1 ;
  
  /* on passe les chiffres */
  
  while( end1 > 0 && isdigit( (int)name1[ end1 ] ) )
    end1--;
  
  /* on passe le '.' */
  
  /*modif karim pour passer les fichier ck1 ck1.1 ck1.2*/
  if( name1[ end1 ] == SPI_SEPAR )
    end1--;
  else 
    end1 = end1x ;

  /*modif karim pour passer les fichier ck1 ck1.1 ck1.2*/
  /*end2 = strlen( name2 ) - 1 ;*/
  end2x = end2 = strlen( name2 ) - 1 ;
  
  /* on passe les chiffres */
  
  while( end2 > 0 && isdigit( (int)name2[ end2 ] ) )
    end2--;
  
  /* on passe le '.' */
  
  /*modif karim pour passer les fichier ck1 ck1.1 ck1.2*/
  if( name2[ end2 ] == SPI_SEPAR )
    end2--;
  else 
    end2 = end2x ;

  end1++;
  end2++;
  
  if( end1 == end2 )
    if( strncmp( name1, name2, end1 ) == 0 )
      return( 1 );

  return( 0 );
}

/******************************************************************************/

void		constlofig( ptcir, ptfig, instances, mode )
circuit		*ptcir;
lofig_list	*ptfig;
ginterf		*instances;
char		mode;
{
  noeud		*scannoeud;
  noeud		*ptnoeud;
  chain_list	*sc1, *sc2, *sc3, *sc4, *sc5;
  locon_list	*scanlocon;
  losig_list	*ptsig;
  int		nbsig;
  int		i;
  losig_list	**tabsig;
  num_list	*interf;
  chain_list	*connec_sig;
  chain_list	*connec_noeud;
  chain_list    *connec_nom;
  chain_list    *connec_con;
  trans		*scantrans;
  inst		*scaninst;
  char		*nom;
  int		idxcon;
  lotrs_list	*pttrs;
  locon_list	*scancon;
  capa		*scancapa;
  loins_list	*ptinst;
  resi		*scanresi;
  valim		*scanvalim;
  ginterf	*sci;
  lofig_list	*modele;
  lofig_list	*tetemodele;
  loctc_list    *ptctc;
  char          *devect;
  ptype_list    *ptnodename;


  tetemodele = NULL;
  
  /* Création du tableau des signaux */

  nbsig = 0;
  for( scannoeud = ptcir->NOEUD; scannoeud ; scannoeud = scannoeud->SUIV )
    if( scannoeud->SIGNAL > nbsig )
      nbsig = scannoeud->SIGNAL;

  tabsig = (losig_list**)mbkalloc( sizeof(losig_list*)*nbsig );
  for( i = 0 ; i < nbsig ; i++ )
    tabsig[i] = NULL;

  /* Création des signaux */

  setsigsize( ptfig, nbsig > 1024 ? 1024 : nbsig );

  for( scannoeud = ptcir->NOEUD; scannoeud ; scannoeud = scannoeud->SUIV )
  {
    if( !(ptsig = tabsig[ scannoeud->SIGNAL - 1 ]) )
    {
      ptsig = addlosig( ptfig, scannoeud->SIGNAL, NULL, INTERNAL );
      tabsig[ scannoeud->SIGNAL - 1 ] = ptsig;
    }

    if( scannoeud->NOM )
    {
      devect = spi_devect( scannoeud->NOM );

      nom = namealloc( devect );
      mbkfree( devect );
      
      for( sc1 = ptsig->NAMECHAIN; sc1 ; sc1 = sc1->NEXT )
        if( sc1->DATA == nom )
          break;

      if( ! sc1 )
        ptsig->NAMECHAIN = addchain( ptsig->NAMECHAIN, nom );
    }
  }

  /* On construit les vues RCN et/ou CTC */
  /* 
  for( ptsig = ptfig->LOSIG ; ptsig ; ptsig = ptsig->NEXT )
    if( ! ptsig->PRCN )
      addlorcnet( ptsig );
  */

  triecapa( ptcir );

  for( scancapa = ptcir->CAPA ; scancapa ; scancapa = scancapa->SUIV )
  {
    if( !tabsig[ scancapa->N1->SIGNAL -1 ]->PRCN )
      addlorcnet( tabsig[ scancapa->N1->SIGNAL -1 ] );

    if( !tabsig[ scancapa->N2->SIGNAL -1 ]->PRCN )
      addlorcnet( tabsig[ scancapa->N2->SIGNAL -1 ] );

    tabsig[ scancapa->N1->SIGNAL -1 ]->PRCN->CAPA += scancapa->CAPA;
    tabsig[ scancapa->N2->SIGNAL -1 ]->PRCN->CAPA += scancapa->CAPA;
  }

  for( scanresi = ptcir->RESI ; scanresi ; scanresi = scanresi->SUIV )
  {
    if( !tabsig[ scanresi->N1->SIGNAL - 1 ]->PRCN )
      addlorcnet( tabsig[ scanresi->N1->SIGNAL - 1 ] );

    addlowire( tabsig[ scanresi->N1->SIGNAL - 1 ],
               RCN_WIRE_UNKNOW,
               0,
               scanresi->RESI,
               scanresi->CAPA,
               0l,
               0l,
               0l,
               0l,
               scanresi->N1->RCN,
               scanresi->N2->RCN
             );
  }

  for( scanvalim = ptcir->VALIM ; scanvalim ; scanvalim = scanvalim->SUIV )
  {
    if( !tabsig[ scanvalim->N1->SIGNAL - 1 ]->PRCN )
      addlorcnet( tabsig[ scanvalim->N1->SIGNAL - 1 ] );

    addlowire( tabsig[ scanvalim->N1->SIGNAL - 1 ],
               RCN_WIRE_UNKNOW,
               0,
               0.0,
               0.0,
               0l,
               0l,
               0l,
               0l,
               scanvalim->N1->RCN,
               scanvalim->N2->RCN
             );
  }

  for( scancapa = ptcir->CAPA ; scancapa ; scancapa = scancapa->SUIV )
  {
    if( !tabsig[ scancapa->N1->SIGNAL - 1 ]->PRCN )
      addlorcnet( tabsig[ scancapa->N1->SIGNAL - 1 ] );

    if( !tabsig[ scancapa->N2->SIGNAL - 1 ]->PRCN )
      addlorcnet( tabsig[ scancapa->N2->SIGNAL - 1 ] );

    if( scancapa->NOM != NULL && scancapa->N1 != scancapa->N2 )
    {
      ptctc = getloctc( tabsig[ scancapa->N1->SIGNAL - 1 ],
                        scancapa->N1->RCN ? scancapa->N1->RCN : 1,
                        tabsig[ scancapa->N2->SIGNAL - 1 ],
                        scancapa->N2->RCN ? scancapa->N2->RCN : 1
                      );

      if( ptctc )
        ptctc->CAPA = ptctc->CAPA + scancapa->CAPA;
      else
        addloctc( tabsig[ scancapa->N1->SIGNAL - 1 ],
                  scancapa->N1->RCN ? scancapa->N1->RCN : 1,
                  tabsig[ scancapa->N2->SIGNAL - 1 ],
                  scancapa->N2->RCN ? scancapa->N2->RCN : 1,
                  scancapa->CAPA
                );
    }
  }

  /* Connecteurs de l'interface */

  idxcon = 1;
  for( sc1 = ptcir->CINTERF, sc2 = instances->GINTERF ;
       sc1 && sc2 ;
       sc1 = sc1->NEXT, sc2 = sc2->NEXT
     )
  {
    ptnoeud = (noeud*)(sc1->DATA);

    for( scanlocon = ptfig->LOCON ; scanlocon ; scanlocon = scanlocon->NEXT )
      if( scanlocon->SIG->INDEX == ptnoeud->SIGNAL )
        break;

    if( !scanlocon )
    {
      ptsig       = tabsig[ ptnoeud->SIGNAL - 1 ];
      ptsig->TYPE = 'E';

      devect = spi_devect( (char*)(sc2->DATA) );

      if( SPI_MERGE == 1 )
        stopchainsepar( devect );

      nom = namealloc( devect );
      mbkfree( devect );

      scanlocon = addlocon( ptfig, nom, ptsig, UNKNOWN );
      scanlocon->USER = addptype( scanlocon->USER, PNODENAME, NULL );
    }

    if( ptnoeud->RCN )
    {
      if( !scanlocon->SIG->PRCN )
        addlorcnet( scanlocon->SIG );
      
      setloconnode( scanlocon, ptnoeud->RCN );
      ptnodename = getptype( scanlocon->USER, PNODENAME );
     
      if( ptnoeud->NOM )
        ((chain_list*)(ptnodename->DATA)) =
       addchain( ((chain_list*)(ptnodename->DATA)), namealloc( ptnoeud->NOM ) );
      else
        ((chain_list*)(ptnodename->DATA)) =
                            addchain( ((chain_list*)(ptnodename->DATA)), NULL );
    }
  }

  if( sc1 || sc2 )
  {
    fflush( stdout );
    fprintf( stderr,"sc1 ou sc2.\n" );
    EXIT(-1);
  }

  for( scanlocon = ptfig->LOCON ; scanlocon ; scanlocon = scanlocon->NEXT )
  {
    (chain_list*)scanlocon->PNODE = reverse( (chain_list*)scanlocon->PNODE );
  
    ptnodename = getptype( scanlocon->USER, PNODENAME );
    for( sc1 = (chain_list*)( ptnodename->DATA ) ; sc1 ; sc1 = sc1->NEXT )
      if( sc1->DATA )
        break;
    if( sc1 )
      ((chain_list*)(ptnodename->DATA)) =
                                   reverse( ((chain_list*)(ptnodename->DATA)) );
    else
    {
      freechain( (chain_list*)ptnodename->DATA );
      scanlocon->USER = delptype( scanlocon->USER, PNODENAME );
    }
  }

  (chain_list*)ptfig->LOCON = reverse( (chain_list*)ptfig->LOCON );

  /* On cree les transistors */

  for( scantrans = ptcir->TRANS; scantrans; scantrans = scantrans->SUIV )
  {
    pttrs = addlotrs( 
                 ptfig,
                 scantrans->TYPE,
                 float2long( scantrans->X ),
                 float2long( scantrans->Y ),
                 float2long( scantrans->W  * SCALE_X * 1e6 ),
                 float2long( scantrans->L  * SCALE_X * 1e6 ),
                 float2long( scantrans->PS * SCALE_X * 1e6 ),
                 float2long( scantrans->PD * SCALE_X * 1e6 ),
                 float2long( scantrans->AS * SCALE_X * 1e6 / scantrans->W ),
                 float2long( scantrans->AD * SCALE_X * 1e6 / scantrans->W ),
                 tabsig[ scantrans->GRILLE->SIGNAL - 1 ],
                 tabsig[ scantrans->SOURCE->SIGNAL - 1 ],
                 tabsig[ scantrans->DRAIN->SIGNAL  - 1 ],
                 scantrans->SUBST ?
                   tabsig[ scantrans->SUBST->SIGNAL  - 1 ] :
                   NULL,
                 scantrans->NOM
                    );

    if( scantrans->DRAIN->RCN )
    {
      if( !pttrs->DRAIN->SIG->PRCN )
        addlorcnet( pttrs->DRAIN->SIG );
      setloconnode(pttrs->DRAIN,  scantrans->DRAIN->RCN  );
      if( scantrans->DRAIN->NOM )
        pttrs->DRAIN->USER = addptype( pttrs->DRAIN->USER,
                                       PNODENAME,
                                       addchain( NULL, 
                                               namealloc( scantrans->DRAIN->NOM)
                                               )
                                     );
    }

    if( scantrans->GRILLE->RCN )
    {
      if( !pttrs->GRID->SIG->PRCN )
        addlorcnet( pttrs->GRID->SIG );
      setloconnode(pttrs->GRID,   scantrans->GRILLE->RCN );
      if( scantrans->GRILLE->NOM )
        pttrs->GRID->USER = addptype( pttrs->GRID->USER,
                                      PNODENAME,
                                      addchain( NULL, 
                                              namealloc( scantrans->GRILLE->NOM)
                                              )
                                    );
    }

    if( scantrans->SOURCE->RCN )
    {
      if( !pttrs->SOURCE->SIG->PRCN )
        addlorcnet( pttrs->SOURCE->SIG );
      setloconnode(pttrs->SOURCE, scantrans->SOURCE->RCN );
      if( scantrans->SOURCE->NOM )
        pttrs->SOURCE->USER = addptype( pttrs->SOURCE->USER,
                                        PNODENAME,
                                        addchain( NULL, 
                                              namealloc( scantrans->SOURCE->NOM)
                                                )
                                      );
    }

    if( scantrans->SUBST )
    {
      if( scantrans->SUBST->RCN )
      {
        if( !pttrs->BULK->SIG->PRCN )
          addlorcnet( pttrs->BULK->SIG );
        setloconnode(pttrs->BULK,   scantrans->SUBST->RCN   );
        if( scantrans->SUBST->NOM )
          pttrs->BULK->USER = addptype( pttrs->BULK->USER,
                                        PNODENAME,
                                        addchain( NULL, 
                                               namealloc( scantrans->SUBST->NOM)
                                                )
                                      );
      }
    }
  } 

  /* On cree les instances */


  for( scaninst = ptcir->INST ; scaninst ; scaninst = scaninst->SUIV )
  {
    connec_sig   = NULL;
    connec_noeud = NULL;
    connec_nom   = NULL;
    connec_con   = NULL;

    for( sci = instances ; sci ; sci = sci->SUIV )
    {
      if( strcasecmp( sci->NOM, scaninst->MODELE ) == 0 )
        break;
    }

    if( !sci )
    {
      fflush( stdout );
      fprintf( stderr,
               "Internal error : can't find model %s.\n",
               scaninst->MODELE
             );
      EXIT( 1 );
    }

    /* Constitue la liste des signaux */
    for( sc1 = sci->GINTERF, sc2 = scaninst->IINTERF ;
         sc1 && sc2 ;
         sc1 = sc1->NEXT, sc2 = sc2->NEXT
       )
    {
      for( sc3 = sci->GINTERF ; sc3 != sc1 ; sc3 = sc3->NEXT )
        if( strcasecmp( sc3->DATA, sc1->DATA ) == 0 )
          break;

      if( sc3 == sc1 ) /* Creation d'un nouveau locon */
      {
        connec_sig = addchain( connec_sig,
                               tabsig[ ((noeud*)(sc2->DATA))->SIGNAL - 1 ]
                             );
        connec_noeud = addchain( connec_noeud,
                                 addnum( NULL, ((noeud*)(sc2->DATA))->RCN )
                               );
        connec_nom = addchain( connec_nom,
                               addchain( NULL, 
                                         namealloc( ((noeud*)(sc2->DATA))->NOM )
                                       )
                             );
        connec_con = addchain( connec_con, sc1->DATA );
      }
      else /* On complete des noeuds d'un locon */
      {
        for ( sc3 = connec_con, sc4 = connec_noeud, sc5 = connec_nom ;
              strcasecmp( (char*)sc3->DATA, (char*)sc1->DATA ) != 0 ;
              sc3 = sc3->NEXT, sc4 = sc4->NEXT, sc5 = sc5->NEXT 
            ) ;

        sc4->DATA = addnum( sc4->DATA, ((noeud*)(sc2->DATA))->RCN );
        sc5->DATA = addchain( sc5->DATA, namealloc (
                                                    ((noeud*)(sc2->DATA))->NOM
                                                   )
                            );
      }
    }

    freechain( connec_con );
    connec_sig   = reverse( connec_sig   );
    connec_noeud = reverse( connec_noeud );
    connec_nom   = reverse( connec_nom   );

    nom = namealloc( sci->NOM );
     
    modele = getlomodel( tetemodele, nom );
    if( !modele )
      modele = tetemodele = recuperemodele( tetemodele, nom, sci->GINTERF );

    ptinst = addloins( ptfig, "*", modele, connec_sig );
    ptinst->INSNAME = namealloc( scaninst->NOM ) ;

    ptinst->USER = constphinterf( ptinst->USER, sci );

    for( scancon = ptinst->LOCON, sc1 = connec_noeud, sc2 = connec_nom ;
         scancon ;
         scancon = scancon->NEXT, sc1 = sc1->NEXT, sc2 = sc2->NEXT
       )
    {
      interf = (num_list*)sc1->DATA; 
      scancon->USER = addptype( scancon->USER, PNODENAME, NULL );
      ptnodename = scancon->USER;

      if( interf->DATA )
      {
        if( !scancon->SIG->PRCN )
          addlorcnet( scancon->SIG );
        
        for( ; interf ; interf = interf->NEXT )
          setloconnode( scancon, interf->DATA );

        sc3 = (chain_list*)sc2->DATA; 
        for( ; sc3 ; sc3 = sc3->NEXT )
          ((chain_list*)(ptnodename->DATA)) = addchain(
                                              ((chain_list*)(ptnodename->DATA)),
                                                        sc3->DATA
                                                      );

      }
    }

    freechain( connec_sig   );
    for( sc1 = connec_noeud ; sc1 ; sc1 = sc1->NEXT )
      freenum( sc1->DATA );
    freechain( connec_noeud );
    for( sc1 = connec_nom ; sc1 ; sc1 = sc1->NEXT )
      freechain( sc1->DATA );
    freechain( connec_nom );

    for( scancon = ptinst->LOCON ; scancon ; scancon = scancon->NEXT )
    {
      ptnodename = getptype( scancon->USER, PNODENAME );
      for( sc1 = (chain_list*)( ptnodename->DATA ) ; sc1 ; sc1 = sc1->NEXT )
        if( sc1->DATA )
          break;
      if( !sc1 )
      {
        freechain( (chain_list*)ptnodename->DATA );
        scancon->USER = delptype( scancon->USER, PNODENAME );
      }
    }
  }

  ptfig->MODE = mode;
  mbkfree( tabsig );
  freelomodel( tetemodele );

  ptfig->LOTRS = ( lotrs_list* ) reverse( (chain_list*) ptfig->LOTRS );

  lofigchain( ptfig );
}

/******************************************************************************/

chain_list*	lireligne( df )
spifile		*df;
{
  chain_list		*sc;
  chain_list		*eff;

  if( df->decomp2 == NULL )
  {
    df->decomp1 = decompligne(df);
    strcpy( df->file_line1, df->file_line );
  }
  else
  {
    df->decomp1 = df->decomp2;
    strcpy( df->file_line1, df->file_line2 );
  }

  if( df->decomp1 == NULL ) /* cas d'un fichier vide */
    return(NULL);

  while( TRUE )
  {
    df->decomp2 = decompligne(df);
    strcpy( df->file_line2, df->file_line );

    if( df->decomp2 == NULL )
      break;

    /*
    if( strcmp( (char*)df->decomp2->DATA, "+" ) != 0 )
    */
    if( ((char*)(df->decomp2->DATA))[0] != '+' ||
        ((char*)(df->decomp2->DATA))[1] != 0      )
      break;

    /* On reforme les lignes séparées par des + */

    eff = df->decomp2;
    df->decomp2 = df->decomp2->NEXT;
    eff->NEXT = NULL;
    mbkfree( eff->DATA );
    freechain(eff);

    for( sc = df->decomp1; sc->NEXT; sc = sc->NEXT );
    sc->NEXT = df->decomp2;
  }
  return( df->decomp1 );
}

/******************************************************************************/

chain_list*	decompligne( df )
spifile		*df;
{
  char		mot[LONG_LIGNE];
  int		lg;
  chain_list	*decomp;
  char		motencours;
  char		*element;
  int		i, j=0 ; /* Warning a la compilation */

  do
  {
    df->msl_line++;
    decomp = NULL;

    fgets( df->file_line, LONG_LIGNE, df->df );

    if( feof(df->df) )
      return( NULL );

    lg = strlen(df->file_line);

    if( lg == LONG_LIGNE-1 )
      spierror( 48, df->filename, df->msl_line );

    motencours = 0;
    
    /* Un cas particulier : si le premier caractere de la ligne est
     * un '+'. Sur la suite de la ligne, le '+' sera considere comme un
     * caractere standard. */

    i=0;
    while( df->file_line[i] == ' ' )
      i++;
    
    if( df->file_line[i] == '+' )
    {
      element  = (char*)mbkalloc( 2 * sizeof(char) );
      element[0] = df->file_line[i];
      element[1] = 0;
      decomp = addchain( decomp, element );
      i++;
    }
   
    /* On traite le reste de la ligne */
  
    for( ; i<lg; i++ )
    {
      if( ( df->file_line[i] >= 'A' && df->file_line[i] <= 'Z' ) ||
          ( df->file_line[i] >= 'a' && df->file_line[i] <= 'z' ) ||
          ( df->file_line[i] >= '0' && df->file_line[i] <= '9' ) ||
          strchr( ":/$<>[]._-|+@", df->file_line[i] )               )
      {
        if( !motencours )
        {
          motencours = 1;
          j = 0;
        }
        mot[j++] = df->file_line[i];
      }
      else
      {
        if( motencours )
        {
          mot[j++] = 0;

          element  = (char*)mbkalloc( j * sizeof(char) );
          memcpy( element, mot, j );
          decomp = addchain( decomp, element );

          motencours = 0;
        }

        if( strchr( "=*$", df->file_line[i] ) )
        {
          element  = (char*)mbkalloc( 2 * sizeof(char) );
          element[0] = df->file_line[i];
          element[1] = 0;
          decomp = addchain( decomp, element );
        }
      }
    }
  }
  while(!decomp);

  return(reverse(decomp));
}

/******************************************************************************/

void		liberecircuit( ptcir )
circuit		*ptcir;
{
  chain_list	*scanchain;
  inst          *scaninst;

  for( scaninst = ptcir->INST ; scaninst ; scaninst = scaninst->SUIV )
    freechain( scaninst->IINTERF );

  for( scanchain = ptcir->FREE ; scanchain ; scanchain = scanchain->NEXT )
    mbkfree( scanchain->DATA );

  freechain( ptcir->FREE   );
  freechain( ptcir->CINTERF );
  freetableint( ptcir->INT_NOEUD );
  freethash( ptcir->HASHGNAME );
  freethash( ptcir->HASHCAPA );
  freethash( ptcir->HASHRESI );
  freethash( ptcir->HASHVALIM );
  freethash( ptcir->HASHINST );
  freethash( ptcir->HASHTRAN );
  freethash( ptcir->NOM_NOEUD );
  mbkfree( ptcir );
}

/******************************************************************************/

noeud*		ajoutenoeud( ptcir, index, df )
circuit		*ptcir;
char		*index;
spifile         *df;
{
  int		val;
  char		*fin;
  noeud		*sn;
  char		*nom;

  val = -1;
  nom = NULL;

  val = strtol( index, &fin, 10 );

  if( fin != index ) /* Ca commence par un nombre */
  {
    if( *fin!=0 )
    {
      fflush( stdout );
      fprintf( stderr, "%s\n", SPIMSG(0) );
      fprintf( stderr, "%s : %s\n", SPIMSG(49), index );
      fprintf( stderr, "%s %s. %s %d :\n",
               SPIMSG(11),
               df->filename,
               SPIMSG(12),
               df->msl_line
             );
      EXIT( 1 );
    }

    sn = (noeud*) tsttableint( ptcir->INT_NOEUD, val );
    if( sn )
      return( sn );
  }
  else
  {
    if(!(( index[0] >= 'A' && index[0] <= 'Z' ) ||
         ( index[0] >= 'a' && index[0] <= 'z' ) ||
         ( index[0] == '_' || index[0] == '@' )    ) )
    {
      fflush( stdout );
      fprintf( stderr, "%s\n", SPIMSG(0) );
      fprintf( stderr, "%s : %s\n", SPIMSG(50), index );
      fprintf( stderr, "%s %s. %s %d :\n",
               SPIMSG(11),
               df->filename,
               SPIMSG(12),
               df->msl_line
             );
      EXIT( 1 );
    }

    nom = spicenamealloc( ptcir, index );

    sn = getthashelem( nom, ptcir->NOM_NOEUD, NULL );
    if( sn )
      return( sn );
  }

  sn           = (noeud*)spiciralloue( ptcir, sizeof(noeud) );
  sn->SUIV     = ptcir->NOEUD;
  ptcir->NOEUD = sn;

  sn->NOM      = nom;
  sn->SIGNAL   = 0;
  sn->RCN      = 0;
  sn->SPICE    = val;

  if( val != -1 )
    settableint( ptcir->INT_NOEUD, val, sn );
  if( nom )
    addthashelem( nom, sn, ptcir->NOM_NOEUD ); 

  return(sn);
}

/******************************************************************************/

void		nomenoeud( ptcir, ptnoeud, ptnom, df )
circuit		*ptcir;
noeud		*ptnoeud;
char		*ptnom;
spifile         *df;
{
  if( ptnoeud->NOM )
  {
    fflush( stdout );
    fprintf( stderr, "%s\n", SPIMSG(0) );
    fprintf( stderr, "%s : %s\n", SPIMSG(51), ptnoeud->NOM );
    fprintf( stderr, "%s %s. %s %d :\n",
             SPIMSG(11),
             df->filename,
             SPIMSG(12),
             df->msl_line
           );
    EXIT( 1 );
  }
      
  ptnoeud->NOM = spicenamealloc( ptcir, ptnom );
  addthashelem( ptnoeud->NOM, ptnoeud, ptcir->NOM_NOEUD );
}

/******************************************************************************/

char*		spicenamealloc( ptcir, ptnom )
circuit		*ptcir;
char		*ptnom;
{
  char		*elem;
  char		tmp[LONG_LIGNE];
  int           l;
  
  downstr( ptnom, tmp );
  /* Bug dans la fonction downstr : il ne recopie pas le 0 de fin de chaine */
  tmp[ strlen(ptnom) ] = 0;
  
  elem = getthashelem( tmp, ptcir->HASHGNAME, NULL );
  if( elem )
    return( elem );

  l = sizeof(char) * (strlen( tmp ) + 1 );
  if( ptcir->RESTENOM < l )
  {
    ptcir->ALLOUENOM = (char*)spiciralloue( ptcir, TAILLENOM );
    ptcir->RESTENOM = TAILLENOM;
  }

  elem = strcpy( ptcir->ALLOUENOM + ( TAILLENOM - ptcir->RESTENOM ), tmp );
  ptcir->RESTENOM = ptcir->RESTENOM - l;
  addthashelem( elem, elem, ptcir->HASHGNAME );
  return( elem );
}

/******************************************************************************/

void*		spiciralloue( cir, taille )
circuit		*cir;
int		 taille;
{
  void		*pt;

  if(!cir)
  {
    fflush( stdout );
    fprintf( stderr, "*** mbkspi internal error *** : spiciralloue()\n" );
    EXIT(1);
  }

  pt = mbkalloc( taille );
  cir->FREE = addchain( cir->FREE, pt );
  cir->TAILLE = cir->TAILLE + taille ;
  return( pt );
}

/******************************************************************************/

float		spicefloat( nombre, status )
char		*nombre;
int		*status;
{
  float		valeur;
  char		*fin;

  /* Les caractères suivant une unité sont ignorés */
  
  *status = -1 ;

  valeur = (float)strtod( nombre, &fin );

  if( *fin == 0 )
    *status = 1;
  else
  if( strncasecmp( fin, "F", 1 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e-15 ;
  }
  else
  if( strncasecmp( fin, "P", 1 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e-12 ;
  }
  else
  if( strncasecmp( fin, "N", 1 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e-9 ;
  }
  else
  if( strncasecmp( fin, "U", 1 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e-6 ;
  }
  else
  if( strncasecmp( fin, "M", 1 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e-3 ;
  }
  else
  if( strncasecmp( fin, "K", 1 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e+3 ;
  }
  else
  if( strncasecmp( fin, "MEG", 3 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e+6 ;
  }
  else
  if( strncasecmp( fin, "MI", 2 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 25.4e+6 ;
  }
  else
  if( strncasecmp( fin, "G", 1 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e+9 ;
  }
  else /*unit values*/
  if( strncasecmp( fin, "V", 1 ) == 0 )
  {
    *status = 1;
    valeur = valeur * 1e+9 ;
  }

  return( valeur);
}

/******************************************************************************/

void		triecapa( ptcir )
circuit		*ptcir;
{
  resi		*scanresi;
  char		*n1, *n2;
  capa		*c1, *c2;
  noeud		*tmp;
  char		*vss;
  char		*vdd;

  vdd = spicenamealloc( ptcir, VDD );
  vss = spicenamealloc( ptcir, VSS);

  for( scanresi = ptcir->RESI ; scanresi ; scanresi = scanresi->SUIV )
  {
    n1 = mbkalloc( sizeof( char ) * ( strlen(scanresi->NOM) + 2 ) );
    n2 = mbkalloc( sizeof( char ) * ( strlen(scanresi->NOM) + 2 ) );

    sprintf( n1 , "%s1", scanresi->NOM );
    sprintf( n2 , "%s2", scanresi->NOM );

    c1 = NULL;
    c2 = NULL;

    c1 = getthashelem( n1, ptcir->HASHCAPA, NULL );
    c2 = getthashelem( n2, ptcir->HASHCAPA, NULL );

    if( !c1 || !c2 )
    {
      scanresi->CAPA = 0.0;
      mbkfree( n1 );
      mbkfree( n2 );
      continue;
    }

    if( c1->N2->NOM == vss )
    {
      tmp = c1->N2;
      c1->N2 = c1->N1;
      c1->N1 = tmp;
    }
    else
    if( c1->N1->NOM != vss )
    {
      scanresi->CAPA = 0.0;
      mbkfree( n1 );
      mbkfree( n2 );
      continue;
    }

    if( c2->N2->NOM == vss )
    {
      tmp = c2->N2;
      c2->N2 = c2->N1;
      c2->N1 = tmp;
    }
    else
    if( c2->N1->NOM != vss )
    {
      scanresi->CAPA = 0.0;
      mbkfree( n1 );
      mbkfree( n2 );
      continue;
    }

    if( c1->CAPA != c2->CAPA )
    {
      scanresi->CAPA = 0.0;
      mbkfree( n1 );
      mbkfree( n2 );
      continue;
    }

    if( ( c1->N2 == scanresi->N1 && c2->N2 == scanresi->N2 )  ||
        ( c2->N2 == scanresi->N1 && c1->N2 == scanresi->N1 )     )
    {
      /* On ne prendra plus ces capacites en compte */
      
      c1->NOM = NULL; 
      c2->NOM = NULL;

      scanresi->CAPA = c1->CAPA+c1->CAPA;
    }
    else
    {
      scanresi->CAPA = 0.0;
      mbkfree( n1 );
      mbkfree( n2 );
      continue;
    }

    mbkfree( n1 );
    mbkfree( n2 );

  }
}

/******************************************************************************/

long            float2long( v )
float		v;
{
  long  i;
  float	r;

  i = v;
  r = v-i;

  if( r >= 0.5 )
    return ( i+1 );
  return(i);
}

/******************************************************************************/

void            taillevuespice( ptcir )
circuit         *ptcir;
{
  int nb_noeud   = 0;
  int nb_resi    = 0;
  int nb_capa    = 0;
  int nb_inst    = 0;
  int nb_alim    = 0;
  int nb_trans   = 0;
  int total;
  noeud         *scannoeud;
  resi          *scanresi;
  capa          *scancapa;
  trans         *scantrans;
  inst          *scaninst;
  valim         *scanalim;
  
  for( scannoeud = ptcir->NOEUD ; scannoeud ; scannoeud = scannoeud->SUIV )
    nb_noeud++;

  for( scantrans = ptcir->TRANS ; scantrans ; scantrans = scantrans->SUIV )
    nb_trans++;

  for( scanresi = ptcir->RESI ; scanresi ; scanresi = scanresi->SUIV )
    nb_resi++;

  for( scancapa = ptcir->CAPA ; scancapa ; scancapa = scancapa->SUIV )
    nb_capa++;

  for( scaninst = ptcir->INST ; scaninst ; scaninst = scaninst->SUIV )
    nb_inst++;

  for( scanalim = ptcir->VALIM ; scanalim ; scanalim = scanalim->SUIV )
    nb_alim++;

  printf( "Data size structure :\n" );
  printf( "  noeud : %7dx%3d = %9d\n",
          nb_noeud,
          sizeof( noeud ),
          nb_noeud * sizeof(noeud)
        );
  printf( "  trans : %7dx%3d = %9d\n",
          nb_trans,
          sizeof(trans),
          nb_trans * sizeof(trans)
        );
  printf( "  resi  : %7dx%3d = %9d\n",
          nb_resi,
          sizeof(resi),
          nb_resi * sizeof(resi)
        );
  printf( "  capa  : %7dx%3d = %9d\n",
          nb_capa,
          sizeof(capa),
          nb_capa * sizeof(capa)
        );
  printf( "  inst  : %7dx%3d = %9d\n",
          nb_inst,
          sizeof(inst),
          nb_inst * sizeof(inst)
        );
  printf( "  alim  : %7dx%3d = %9d\n",
          nb_alim,
          sizeof(valim),
          nb_alim * sizeof(valim)
        );

  total = nb_noeud * sizeof(noeud) +
          nb_trans * sizeof(trans) +
          nb_resi * sizeof(resi)   +
          nb_capa * sizeof(capa)   +
          nb_inst * sizeof(inst)   +
          nb_alim * sizeof(valim);

  printf( "Total : %d.\n", total );
  printf( "Real size : %ld\n", ptcir->TAILLE );
          
}

/******************************************************************************/

void		affvuespice( ptcir )
circuit		*ptcir;
{
  chain_list	*sc;
  inst		*scaninst;
  trans		*scantrans;
  resi		*scanresi;
  capa		*scancapa;

  printf("Circuit : %s\n", ptcir->NOM );

  printf("Interface :\n");
  for( sc = ptcir->CINTERF; sc; sc = sc->NEXT )
    printf("  %8X %4d %4d %4d %s\n",
           ((unsigned int)(sc->DATA)),
           ((noeud*)(sc->DATA))->SPICE,
           ((noeud*)(sc->DATA))->SIGNAL,
           ((noeud*)(sc->DATA))->RCN,
           ((noeud*)(sc->DATA))->NOM ? ((noeud*)(sc->DATA))->NOM : "" );

  for(scantrans = ptcir->TRANS; scantrans; scantrans = scantrans->SUIV)
  {
    printf("Transitor : %s\n",scantrans->NOM);
    printf("  DRAIN   %8X %4d %4d %4d %s\n",
            (unsigned int)scantrans->DRAIN,
            scantrans->DRAIN->SPICE,
            scantrans->DRAIN->SIGNAL,
            scantrans->DRAIN->RCN,
            scantrans->DRAIN->NOM ? scantrans->DRAIN->NOM : "" );
    printf("  GRILLE  %8X %4d %4d %4d %s\n",
            (unsigned int)scantrans->GRILLE,
            scantrans->GRILLE->SPICE,
            scantrans->GRILLE->SIGNAL,
            scantrans->GRILLE->RCN,
            scantrans->GRILLE->NOM ? scantrans->GRILLE->NOM : ""    );
    printf("  SOURCE  %8X %4d %4d %4d %s\n",
            (unsigned int)scantrans->SOURCE,
            scantrans->SOURCE->SPICE,
            scantrans->SOURCE->SIGNAL,
            scantrans->SOURCE->RCN,
            scantrans->SOURCE->NOM ? scantrans->SOURCE->NOM : ""    );
    if( scantrans->SUBST )
    {
      printf("  SUBST   %8X %4d %4d %4d %s\n",
              (unsigned int)scantrans->SUBST,
              scantrans->SUBST->SPICE,
              scantrans->SUBST->SIGNAL,
              scantrans->SUBST->RCN,
              scantrans->SUBST->NOM ? scantrans->SUBST->NOM : ""     );
      printf("  TYPE    %c\n",scantrans->TYPE );
      printf("  L=%g W=%g AS=%g AD=%g PS=%g PD=%g\n",scantrans->L,
                                                     scantrans->W,
                                                     scantrans->AS,
                                                     scantrans->AD,
                                                     scantrans->PS,
                                                     scantrans->PD      );
    }
  }

  for(scanresi = ptcir->RESI ; scanresi; scanresi = scanresi->SUIV )
  {
    printf("Resistance : %s\n",scanresi->NOM);
    printf("  N1   %8X %4d %4d %4d %s\n",
            (unsigned int)scanresi->N1,
            scanresi->N1->SPICE,
            scanresi->N1->SIGNAL,
            scanresi->N1->RCN,
            scanresi->N1->NOM ? scanresi->N1->NOM : ""    );
    printf("  N2   %8X %4d %4d %4d %s\n",
            (unsigned int)scanresi->N2,
            scanresi->N2->SPICE,
            scanresi->N2->SIGNAL,
            scanresi->N2->RCN,
            scanresi->N2->NOM ? scanresi->N2->NOM : ""    );
    printf("  R=%g\n",scanresi->RESI);
  }

  for(scancapa = ptcir->CAPA ; scancapa; scancapa = scancapa->SUIV )
  {
    printf("Capacite : %s\n",scancapa->NOM);
    printf("  N1   %8X %4d %4d %4d %s\n",
            (unsigned int)scancapa->N1,
            scancapa->N1->SPICE,
            scancapa->N1->SIGNAL,
            scancapa->N1->RCN,
            scancapa->N1->NOM ? scancapa->N1->NOM : ""    );
    printf("  N2   %8X %4d %4d %4d %s\n",
            (unsigned int)scancapa->N2,
            scancapa->N2->SPICE,
            scancapa->N2->SIGNAL,
            scancapa->N2->RCN,
            scancapa->N2->NOM ? scancapa->N2->NOM : ""    );
    printf("  C=%g\n",scancapa->CAPA);
  }

  for( scaninst = ptcir->INST; scaninst; scaninst = scaninst->SUIV)
  {
    printf("Instance %s basee sur %s\n",scaninst->NOM, scaninst->MODELE);
    for( sc = scaninst->IINTERF; sc; sc = sc->NEXT )
      printf("  %8X %4d %4d %4d %s\n",
             ((unsigned int)(sc->DATA)),
             ((noeud*)(sc->DATA))->SPICE,
             ((noeud*)(sc->DATA))->SIGNAL,
             ((noeud*)(sc->DATA))->RCN,
             ((noeud*)(sc->DATA))->NOM ? ((noeud*)(sc->DATA))->NOM : ""    );
  }
}

/******************************************************************************/

char*		retireextention( nom )
char		*nom;
{
  char		*pt;
  int		i;

  pt = mbkstrdup( nom );
 
  /* On retire l'extention du nom, c'est a dire qu'on arrete la chaine au
   * dernier point rencontré */
  /* >0 : On laisse quand meme 1 caractere si le nom commence par un point */
  for( i = strlen( pt ) - 1 ; i > 0 ; i-- )
    if( pt[i] == '.' )
    {
      pt[i] = 0;
      break;
    }

  return( pt );
}

/******************************************************************************/

lofig_list*	recuperemodele( tetemodel, nom, interf )
lofig_list	*tetemodel;
char		*nom;
chain_list	*interf;
{
  lofig_list	*ptmod;
  chain_list	*sc1, *sc2;
  char          *nomdevec;
  
  ptmod = addlomodel( tetemodel, nom );

  for( sc1 = interf ; sc1 ; sc1 = sc1->NEXT )
  {
    for( sc2 = interf; sc1 != sc2 ; sc2 = sc2->NEXT )
      if( strcasecmp( sc1->DATA, sc2->DATA ) == 0 ) 
        break;
    
    if( sc1 != sc2 )
      continue;
    
    nomdevec = spi_devect( (char*)(sc1->DATA) );
    ptmod->LOCON = addlocon( ptmod, namealloc( nomdevec ), NULL, 'X' );
    mbkfree( nomdevec );
  }

  ptmod->LOCON = (locon_list*)reverse( (chain_list*) ptmod->LOCON );
  return( ptmod );
}

/******************************************************************************/

void		mslAddExtension(mslExtHandler)
int		(*mslExtHandler)();
{
  ext_handler = mslExtHandler;
}

/******************************************************************************/

void		mslRmvExtension()
{
  ext_handler = NULL;
}

/******************************************************************************/

void		mslAddCompletion(mslCmpHandler)
int             (*mslCmpHandler)();
{
  cmp_handler = mslCmpHandler;
}

/******************************************************************************/

void		mslRmvCompletion()
{
  cmp_handler = NULL;
}

/******************************************************************************/

char*           spi_devect( nom )
char            *nom;
{
  int           taille;
  int           i;
  int           j;
  char         *nouv;
  int           fin;
  int           modif;
  
  taille = strlen( nom );
  if( taille == 0 )
  {
    /* message */
    EXIT(1);
  }

  nouv  = (char*)mbkalloc( sizeof(char) * (taille+1) );
  fin   = taille - 1 ;
  modif = 0;

  if( nom[fin] == ']' )
  { 
    /* Retrouve le crochet ouvrant */
    for( i = fin-1 ; i >= 0 && isdigit( (int)nom[i] ) ; i-- );
    
    if( nom[i] == '[' )
    {
      if( i == 0 || i == fin - 1 )
      {
        /* nom de variable bizare, de la forme [43] ou toto[] */
        fflush( stdout );
        fprintf( stderr, "%s : \"%s\"\n", SPIMSG(52), nom );
        EXIT(1);
      }
      else
      {
        /* bon vecteur : toto[32] */
        for( j = 0 ; j < fin ; j++ )
          if( i != j )
            nouv[j] = nom[j];
          else
            nouv[i] = ' ';
        nouv[j] = 0;

        modif = 1;
      }
    }
    else
    {
      /* Nom de variable bizarre, de la forme toto32], toto] ou 32] */
      fflush( stdout );
      fprintf( stderr, "%s : \"%s\"\n", SPIMSG(52), nom );
      EXIT(1);
    }
  }
 
  if( ! modif )
    strcpy( nouv, nom );

  return(nouv);
}

/******************************************************************************/

void spierror( mesg, file, line )
int      mesg;
char    *file;
int      line;
{
  fflush( stdout );
  fprintf( stderr, "%s.\n", SPIMSG(0) );
  fprintf( stderr, "%s. %s %s. %s %d.\n",
           SPIMSG(1),
           SPIMSG(11),
           file,
           SPIMSG(12),
           line
         );
  fprintf( stderr, "%s.\n",SPIMSG(mesg));
  EXIT(1);
}

/******************************************************************************/

void    stopchainsepar( chaine )
char    *chaine;
{
  int i;

  /* On retire le nombre en fin de nom et le SPI_SEPAR */
  for( i = strlen( chaine ) - 1 ; i >= 0 ; i-- )
    if( ! isdigit( (int)chaine[i] ) )
      break;

  if( i > 0 )
    if( chaine[i] == SPI_SEPAR )
      chaine[i] = '\0' ;
}
