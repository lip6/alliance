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

#ident "$Id: rcn_lo.c,v 1.5 2009/06/14 13:51:52 ludo Exp $"

/*******************************************************************************
* rcn    : acces functions to logical rc network structures                    *
*                                                                              *
* version : 1.02                                                               *
* date   : 07/06/96                                                            *
*******************************************************************************/

/*******************************************************************************
* Grégoire AVOT                                                                *
* Laboratoire LIP6 - Département ASIM                                          *
* Universite Paris 6 - Pierre et Marie Curie                                   *
* 4, Place Jussieu                                                             *
* Paris Cedex 05.                                                              *
*******************************************************************************/
#include <stdlib.h>

#include "mut.h"
#include "mlo.h"
#include "rcn.h"
#include "rcn_lo.h"

lorcnet_list	*HEAD_RCNET_FREE	= NULL;
unsigned int	 NBRCNET=0;
lowire_list	*HEAD_WIRE_FREE		= NULL;
unsigned int	 NBWIRE=0;
loctc_list	*HEAD_CTC_FREE		= NULL;
unsigned int	 NBCTC=0;

/*******************************************************************************
* function addlorcnet()                                                        *
*******************************************************************************/
lorcnet_list *addlorcnet(ptsig)
losig_list *ptsig;
{

  if( ptsig->PRCN )
  {
    (void)fflush(stdout);
    (void)fprintf(stderr, "*** rcn error *** : addlorcnet() impossible\n");
    (void)fprintf(stderr, "rc network already exists on signal %ld\n",
                  ptsig->INDEX);
    EXIT(-1);
  }

  ptsig->PRCN = heaprcnet();
  ptsig->PRCN->PWIRE = NULL;
  ptsig->PRCN->PCTC  = NULL;
  
  return( ptsig->PRCN );
}

/*******************************************************************************
* function givelorcnet()                                                       *
*******************************************************************************/
lorcnet_list *givelorcnet(ptsig)
losig_list *ptsig;
{
  if( ! ptsig->PRCN )
    ptsig->PRCN = heaprcnet();
  return( ptsig->PRCN );
}

/*******************************************************************************
* function getcapa()                                                           *
*******************************************************************************/
float getcapa(ptsig)
losig_list *ptsig;
{
  if( !ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : getcapa() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  return( ptsig->PRCN->CAPA );
}

/*******************************************************************************
* function setcapa()                                                           *
*******************************************************************************/
float setcapa(ptsig,value)
losig_list *ptsig;
float       value;
{

  if( !ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : setcapa() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  ptsig->PRCN->CAPA = value ;
  return( ptsig->PRCN->CAPA );
}

/*******************************************************************************
* function addcapa()                                                           *
*******************************************************************************/
float addcapa(ptsig,value)
losig_list *ptsig;
float       value;
{
  if( !ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : addcapa() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  ptsig->PRCN->CAPA = ptsig->PRCN->CAPA + value ;
  return( ptsig->PRCN->CAPA );
}

/*******************************************************************************
* function addlowire()                                                         *
*******************************************************************************/
lowire_list *addlowire(ptsig, layer, flag, resi, capa, x, y, dx, dy, n1, n2)
losig_list *ptsig;
unsigned char layer;
unsigned char flag;
float resi;
float capa;
long x;
long y;
long dx;
long dy;
long n1;
long n2;
{
  lowire_list	*ptwire;
  lonode_list	*ptnode1;
  lonode_list	*ptnode2;

  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : addlowire() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  ptwire             = heaplowire();
  ptwire->FLAG       = flag;
  ptwire->LAYER      = layer;
  ptwire->RESI       = resi;
  ptwire->CAPA       = capa;
  ptwire->X          = x;
  ptwire->Y          = y;
  ptwire->DX         = dx;
  ptwire->DY         = dy;
  ptwire->NODE1      = n1;
  ptwire->NODE2      = n2;
  ptwire->USER       = NULL;
  ptwire->NEXT       = ptsig->PRCN->PWIRE; 
                                         
  ptsig->PRCN->PWIRE = ptwire;  

  if( gettabnode( ptsig->PRCN ) )
  {
    ptnode1 = givelonode(ptsig, n1);
    ptnode2 = givelonode(ptsig, n2);
    ptnode1->WIRELIST = addchain(ptnode1->WIRELIST,ptwire);
    ptnode2->WIRELIST = addchain(ptnode2->WIRELIST,ptwire);

    switch( RCN_GETFLAG(ptnode1->FLAG,RCN_FLAG_ONE|RCN_FLAG_CROSS|RCN_FLAG_TWO))
    {
      case RCN_FLAG_ONE:
        RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_ONE);
        RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_TWO);
        break;
      case RCN_FLAG_TWO:
        RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_TWO);
        RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_CROSS);
      case RCN_FLAG_CROSS:
        break;
      default:
        RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_ONE);
    }

    switch( RCN_GETFLAG(ptnode2->FLAG,RCN_FLAG_ONE|RCN_FLAG_CROSS|RCN_FLAG_TWO))
    {
      case RCN_FLAG_ONE:
        RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_ONE);
        RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_TWO);
        break;
      case RCN_FLAG_TWO:
        RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_TWO);
        RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_CROSS);
        break;
      case RCN_FLAG_CROSS:
        break;
      default:
        RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_ONE);
    }
  }

  if( n1 >= ptsig->PRCN->NBNODE ) 
    ptsig->PRCN->NBNODE = n1+1;
  if( n2 >= ptsig->PRCN->NBNODE ) 
    ptsig->PRCN->NBNODE = n2+1;

  if (TRACE_MODE == 'Y')
    (void)fprintf(stdout, "--- rcn --- addlowire : between node %ld and node %ld on signal %ld\n",
	         ptwire->NODE1, ptwire->NODE2, ptsig->INDEX);

  return ptwire;
}

/*******************************************************************************
* function addloctc()                                                          *
*******************************************************************************/

loctc_list* addloctc( ptsig1, node1, ptsig2, node2, value )
losig_list	*ptsig1;
long int	 node1;
losig_list	*ptsig2;
long int	 node2;
float		 value;
{
  loctc_list	*newctc;
  lonode_list	*ptnode1;
  lonode_list	*ptnode2;

  if( !ptsig1->PRCN )
  {
    fflush ( stdout );
    fprintf( stderr, "*** mbk error *** : addloctc() impossible.\n" );
    fprintf( stderr, "No rc data on signal %ld.\n", ptsig1->INDEX );
    EXIT(1);
  }

  if( !ptsig2->PRCN )
  {
    fflush ( stdout );
    fprintf( stderr, "*** mbk error *** : addloctc() impossible.\n" );
    fprintf( stderr, "No rc data on signal %ld.\n", ptsig2->INDEX );
    EXIT(1);
  }

  newctc             = heaploctc();

  newctc->CAPA       = value;
  newctc->SIG1       = ptsig1;
  newctc->NODE1      = node1;
  newctc->SIG2       = ptsig2;
  newctc->NODE2      = node2;

  /* Pas de verification de l'existence d'une capa */
  
  ptsig1->PRCN->PCTC = addchain( ptsig1->PRCN->PCTC, newctc );

  if( ptsig1 != ptsig2 )
    ptsig2->PRCN->PCTC = addchain( ptsig2->PRCN->PCTC, newctc );


  if( gettabnode(ptsig1->PRCN) && node1 )
  {
    ptnode1          = givelonode( ptsig1, node1 );
    ptnode1->CTCLIST = addchain( ptnode1->CTCLIST, newctc );
  }

  if( ptsig1->PRCN->NBNODE <= node1 )
    ptsig1->PRCN->NBNODE = node1+1;

  if( gettabnode(ptsig2->PRCN) && node2 )
  {
    ptnode2          = givelonode( ptsig2, node2 );
    ptnode2->CTCLIST = addchain( ptnode2->CTCLIST, newctc );
  }

  if( ptsig2->PRCN->NBNODE <= node2 )
    ptsig2->PRCN->NBNODE = node2+1;

  return( newctc );
}

/*******************************************************************************
* function delloctc()                                                          *
*******************************************************************************/
int delloctc( ptsig1, node1, ptsig2, node2 )
losig_list	*ptsig1;
long int	 node1;
losig_list	*ptsig2;
long int	 node2;
{
  chain_list	*ptloctc1;
  chain_list	*ptloctc2=NULL;
  loctc_list	*ptloctc;
  chain_list	*prev1;
  chain_list	*prev2=NULL;

  chain_list	*scanchain;
  lonode_list	*ptnode;
  

  if( !ptsig1->PRCN )
  {
    fflush ( stdout );
    fprintf( stderr, "*** mbk error *** : delloctc() impossible.\n" );
    fprintf( stderr, "No rc data on signal %ld.\n", ptsig1->INDEX );
    EXIT(1);
  }

  if( !ptsig2->PRCN )
  {
    fflush ( stdout );
    fprintf( stderr, "*** mbk error *** : delloctc() impossible.\n" );
    fprintf( stderr, "No rc data on signal %ld.\n", ptsig2->INDEX );
    EXIT(1);
  }

  /* Cherche le loctc */

  ptloctc = NULL;

  prev1 = NULL;

  for( ptloctc1 =  ptsig1->PRCN->PCTC ;
       ptloctc1 != NULL ; 
       ptloctc1 =  ptloctc1->NEXT )
  {
    ptloctc = (loctc_list*) ptloctc1->DATA ;
   
    if( ptloctc->SIG1     == ptsig1 &&
        ptloctc->SIG2     == ptsig2    ) 
    {
      if( ( ( node1 && ptloctc->NODE1 == node1 ) || !node1 ) &&
	  ( ( node2 && ptloctc->NODE2 == node2 ) || !node2 )    )
        break;
    }

    if( ptloctc->SIG1     == ptsig2 &&
        ptloctc->SIG2     == ptsig1    ) 
    {
      if( ( ( node1 && ptloctc->NODE2 == node1 ) || !node1 ) &&
	  ( ( node2 && ptloctc->NODE1 == node2 ) || !node2 )    )
        break;
    }

    prev1 = ptloctc1;
  }

  if( !ptloctc1 )
    return( 0 );

  if( ptsig1 != ptsig2 )
  {
    prev2 = NULL;
    
    for( ptloctc2 =  ptsig2->PRCN->PCTC ;
         ptloctc2 != NULL ; 
         ptloctc2 =  ptloctc2->NEXT )
    {
      ptloctc = (loctc_list*) ptloctc2->DATA ;
     
      if( ptloctc->SIG1     == ptsig1 &&
          ptloctc->SIG2     == ptsig2    ) 
      {
        if( ( ( node1 && ptloctc->NODE1 == node1 ) || !node1 ) &&
  	  (   ( node2 && ptloctc->NODE2 == node2 ) || !node2 )    )
          break;
      }
  
      if( ptloctc->SIG1     == ptsig2 &&
          ptloctc->SIG2     == ptsig1    ) 
      {
        if( ( ( node1 && ptloctc->NODE2 == node1 ) || !node1 ) &&
  	  (   ( node2 && ptloctc->NODE1 == node2 ) || !node2 )    )
          break;
      }
  
      prev2 = ptloctc2;
    }
  
    if( !ptloctc2)
    {
      fflush( stdout );
      fprintf( stderr, "Erreur interne dans delloctc. Atchoum!, j'ai froid, je meurs...\n" );
      EXIT(1);
    }
  }
  /* Cleanning element in lorcnet */

  if( prev1 == NULL )
    ptsig1->PRCN->PCTC = ptloctc1->NEXT;
  else
    prev1->NEXT = ptloctc1->NEXT;
  ptloctc1->NEXT = NULL;
  freechain( ptloctc1 );
 
  if( ptsig1 != ptsig2 )
  {
    if( prev2 == NULL )
      ptsig2->PRCN->PCTC = ptloctc2->NEXT;
    else
      prev2->NEXT = ptloctc2->NEXT;
    ptloctc2->NEXT = NULL;
    freechain( ptloctc2 );
  }

  /* cleanning element in table */

  if( gettabnode( ptloctc->SIG1->PRCN ) && ptloctc->NODE1 )
  {
    ptnode = getlonode( ptloctc->SIG1, ptloctc->NODE1 );
    prev1 = NULL;

    for( scanchain = ptnode->CTCLIST ; scanchain ; scanchain = scanchain->NEXT )
    {
      if( scanchain->DATA == ptloctc )
        break;
      prev1 = scanchain;
    }

    if( prev1 )
      prev1->NEXT = scanchain->NEXT;
    else
      ptnode->CTCLIST = scanchain->NEXT;

    scanchain->NEXT = NULL;
    freechain( scanchain );
  }

  if( gettabnode( ptloctc->SIG2->PRCN ) && ptsig1 != ptsig2 && ptloctc->NODE2 )
  {
    ptnode = getlonode( ptloctc->SIG2, ptloctc->NODE2 );
    prev2 = NULL;

    for( scanchain = ptnode->CTCLIST ; scanchain ; scanchain = scanchain->NEXT )
    {
      if( scanchain->DATA == ptloctc )
        break;
      prev2 = scanchain;
    }

    if( prev2 )
      prev2->NEXT = scanchain->NEXT;
    else
      ptnode->CTCLIST = scanchain->NEXT;

    scanchain->NEXT = NULL;
    freechain( scanchain );
  }

  /* Really clean the loctc */

  freeloctc( ptloctc );
  
  return(1);
}

/*******************************************************************************
* function getloctc()                                                          *
*******************************************************************************/
loctc_list*	getloctc( ptsig1, node1, ptsig2, node2 )
losig_list	*ptsig1;
long int	 node1;
losig_list	*ptsig2;
long int	 node2;
{
  chain_list	*ptloctc1;
  loctc_list	*ptloctc;
  lonode_list   *ptnode;

  if( ! ptsig1->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : getloctc() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig1->INDEX);
    EXIT(1);
  }

  if( ! ptsig2->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : getloctc() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig2->INDEX);
    EXIT(1);
  }

  if( gettabnode(ptsig1->PRCN) )
  {
    ptnode = getlonode( ptsig1, node1 );
    for( ptloctc1 =  ptnode->CTCLIST ;
         ptloctc1 != NULL ; 
         ptloctc1 =  ptloctc1->NEXT )
    {
      ptloctc = (loctc_list*) ptloctc1->DATA ;
     
      if( ptloctc->SIG1     == ptsig1 &&
          ptloctc->SIG2     == ptsig2    ) 
      {
        if( ( ( node1 && ptloctc->NODE1 == node1 ) || !node1 ) &&
  	  ( ( node2 && ptloctc->NODE2 == node2 ) || !node2 )    )
          return( ptloctc );
      }
  
      if( ptloctc->SIG1     == ptsig2 &&
          ptloctc->SIG2     == ptsig1    ) 
      {
        if( ( ( node1 && ptloctc->NODE2 == node1 ) || !node1 ) &&
  	  ( ( node2 && ptloctc->NODE1 == node2 ) || !node2 )    )
          return( ptloctc );
      }
    }
    return(NULL);
  }
  
  for( ptloctc1 =  ptsig1->PRCN->PCTC ;
       ptloctc1 != NULL ; 
       ptloctc1 =  ptloctc1->NEXT )
  {
    ptloctc = (loctc_list*) ptloctc1->DATA ;
   
    if( ptloctc->SIG1     == ptsig1 &&
        ptloctc->SIG2     == ptsig2    ) 
    {
      if( ( ( node1 && ptloctc->NODE1 == node1 ) || !node1 ) &&
	  ( ( node2 && ptloctc->NODE2 == node2 ) || !node2 )    )
        return( ptloctc );
    }

    if( ptloctc->SIG1     == ptsig2 &&
        ptloctc->SIG2     == ptsig1    ) 
    {
      if( ( ( node1 && ptloctc->NODE2 == node1 ) || !node1 ) &&
	  ( ( node2 && ptloctc->NODE1 == node2 ) || !node2 )    )
        return( ptloctc );
    }
  }

  return(NULL);
}

/*******************************************************************************
* function freectclist()                                                       *
*******************************************************************************/
void freectclist( ptfig, headsig )
lofig_list      *ptfig;
chain_list      *headsig;
{
  chain_list    *allsig;
  losig_list    *scansig;

  if( headsig == NULL )
  {
    allsig = NULL;
    for( scansig = ptfig->LOSIG ; scansig ; scansig = scansig->NEXT )
      if( scansig->PRCN )
        allsig = addchain( allsig, scansig );
  }
  else
    allsig = headsig;

  if( allsig )
    freectcsig( allsig );

  if( allsig && !headsig )
    freechain( allsig );
}

/*******************************************************************************
* function freectcsig()                                                        *
*******************************************************************************/
void freectcsig(  headsig )
chain_list      *headsig;
{
  chain_list   *boulot;
  loctc_list   *ptctc;
  chain_list   *efface;
  chain_list   *sc1;
  chain_list   *sc2;
  chain_list   *sc3;
  losig_list   *ptsig;
  chain_list   *prev;
  lonode_list  *ptnode;

  efface = NULL;
  boulot = NULL;

  for( sc1 = headsig ; sc1 ; sc1 = sc1->NEXT )
  {
    ptsig = (losig_list*)(sc1->DATA);
    if( ptsig->PRCN )
    {
      for( sc2 = ptsig->PRCN->PCTC ; sc2 ; sc2 = sc2->NEXT)
      {
        ptctc = (loctc_list*)( sc2->DATA );
        if( ptctc->CAPA >=0.0 )
        {
          ptctc->CAPA = -1.0;
          
          if( !getptype( ptctc->SIG1->USER, RCN_REPORTED ) )
          {
            ptctc->SIG1->USER = addptype( ptctc->SIG1->USER,
                                          RCN_REPORTED,
                                          NULL
                                        );
            boulot = addchain( boulot, ptctc->SIG1 );
          }
          
          if( !getptype( ptctc->SIG2->USER, RCN_REPORTED ) )
          {
            ptctc->SIG2->USER = addptype( ptctc->SIG2->USER,
                                          RCN_REPORTED,
                                          NULL
                                        );
            boulot = addchain( boulot, ptctc->SIG2 );
          }
          
          efface = addchain( efface, ptctc );
        }
      }
    }
  }

  for( sc3 = boulot ; sc3 ; sc3 = sc3->NEXT )
  {
    ptsig = (losig_list*)(sc3->DATA);
    ptsig->USER = delptype( ptsig->USER, RCN_REPORTED );
    
    prev = NULL;
    sc1  = ptsig->PRCN->PCTC ;
      
    while( sc1 )
    {
      ptctc = (loctc_list*) sc1->DATA ;
      sc2 = sc1->NEXT;
      
      if( ptctc->CAPA == -1.0 )  
      {
        if( sc1 == ptsig->PRCN->PCTC )
          ptsig->PRCN->PCTC = sc2;
        else
          prev->NEXT = sc2;

        sc1->NEXT = NULL;
        freechain( sc1 );
      }
      else
        prev = sc1;
      sc1 = sc2;
    }
  }

  for( sc1 = efface ; sc1 ; sc1 = sc1->NEXT )
  {
    ptctc = (loctc_list*)(sc1->DATA);

    if( gettabnode( ptctc->SIG1->PRCN ) && ptctc->NODE1 )
    {
      ptnode = getlonode( ptctc->SIG1, ptctc->NODE1 );
      prev = NULL;

      for( sc2 = ptnode->CTCLIST ; sc2 ; sc2 = sc2->NEXT )
      {
        if( sc2->DATA == ptctc )
          break;
        prev = sc2;
      }

      if( prev )
        prev->NEXT = sc2->NEXT;
      else
        ptnode->CTCLIST = sc2->NEXT;

      sc2->NEXT = NULL;
      freechain( sc2 );
    }

    if( gettabnode( ptctc->SIG2->PRCN ) && ptctc->NODE2 )
    {
      ptnode = getlonode( ptctc->SIG2, ptctc->NODE2 );
      prev = NULL;

      for( sc2 = ptnode->CTCLIST ; sc2 ; sc2 = sc2->NEXT )
      {
        if( sc2->DATA == ptctc )
          break;
        prev = sc2;
      }

      if( prev )
        prev->NEXT = sc2->NEXT;
      else
        ptnode->CTCLIST = sc2->NEXT;

      sc2->NEXT = NULL;
      freechain( sc2 );
    }

    freeloctc( ptctc );
  }
  
  freechain( efface );
  freechain( boulot );
}

/*******************************************************************************
* function addlonode()                                                         *
*******************************************************************************/
long addlonode(ptsig, ptlocon)
losig_list *ptsig;
locon_list *ptlocon;
{
  int   dispo;
  
  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : addlonode() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  dispo = ptsig->PRCN->NBNODE;
  ptsig->PRCN->NBNODE++ ;

  if(ptlocon != NULL)
    setloconnode(ptlocon, dispo );

  if (TRACE_MODE == 'Y')
  (void)fprintf(stdout, "--- rcn --- addlonode	  : on signal %ld\n",
                ptsig->INDEX);
                
  return( dispo );
}

/*******************************************************************************
* function freelorcnet()                                                       *
*******************************************************************************/
int freelorcnet(signal)
losig_list	*signal;
{
  lowire_list		*scanwire;
  lowire_list		*prevwire;
  ptype_list		*pt;
  chain_list		*scancon;
  locon_list		*ptcon;
  chain_list            *delsig;
  

  if( ! signal->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : freelorcnet() impossible.\n");
    fprintf(stderr,"no rc view on signal %ld.\n",signal->INDEX);
    EXIT(1);
  }

  pt = getptype( signal->USER, LOFIGCHAIN );
  if( !pt )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : freelorcnet() impossible.\n");
    fprintf(stderr,"Call the function lofigchain() before delloconnode.\n");
    EXIT(1);
  }

  /* Libère la table des noeuds */
  if( gettabnode(signal->PRCN) )
    freetable(signal);

  /* Efface les PNODEs des locons sur le signal */
  for( scancon = (chain_list*)(pt->DATA) ; scancon ; scancon = scancon->NEXT )
  {
    ptcon = (locon_list*)(scancon->DATA);

    if( ptcon->PNODE )
    {
      freenum(ptcon->PNODE);
      ptcon->PNODE = NULL;
    }
  }

  /* efface les wire */
  
  if( signal->PRCN->PWIRE )
  {
    prevwire = NULL;
  
    for( scanwire = signal->PRCN->PWIRE ;
         scanwire != NULL ;
         scanwire = scanwire->NEXT)
    {
      if(prevwire)
        freelowire(prevwire);
      prevwire=scanwire;
    }

    if(prevwire)
      freelowire(prevwire);
  }

  /* efface les ctc */

  delsig = addchain( NULL, signal );
  freectcsig( delsig );
  freechain( delsig );
  
  /* Finallement, on efface le reseau lui meme */
  freercnet(signal->PRCN);
  signal->PRCN = NULL;

  return(1);
}

/*******************************************************************************
* function dellowire()                                                         *
*******************************************************************************/
int dellowire(ptsig, node1, node2)
losig_list *ptsig;
long node1;
long node2;
{
lowire_list	*ptwire;
lowire_list	*ptsav=NULL;
lonode_list	*ptnode1;
lonode_list	*ptnode2;
chain_list	*ptchain;

  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : dellowire() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  for (ptwire = ptsig->PRCN->PWIRE; ptwire != NULL ; ptwire = ptwire->NEXT)
  {
    if(   (ptwire->NODE1 == node1 && ptwire->NODE2 == node2)
       || (ptwire->NODE1 == node2 && ptwire->NODE2 == node1) )
      break;
    ptsav = (void *)ptwire;
  }

  if (ptwire == NULL)   
    return 0;
  else 
  {
    if (ptwire == ptsig->PRCN->PWIRE) /* premier de la liste */ 
      ptsig->PRCN->PWIRE = ptwire->NEXT;
    else
      ptsav->NEXT        = ptwire->NEXT;
  }

  freelowire(ptwire);

  if( gettabnode(ptsig->PRCN) != NULL)
  {
    ptnode1 = getlonode(ptsig, node1);
    ptnode2 = getlonode(ptsig, node2);

    /* optimiser */
    for(ptchain = ptnode1->WIRELIST;ptchain != NULL;ptchain = ptchain->NEXT)
      if(ptchain->DATA == (void *)ptwire) break;
    ptnode1->WIRELIST = delchain(ptnode1->WIRELIST,ptchain);

    for(ptchain = ptnode2->WIRELIST;ptchain != NULL;ptchain = ptchain->NEXT)
      if(ptchain->DATA == (void *)ptwire) break;
    ptnode2->WIRELIST = delchain(ptnode2->WIRELIST,ptchain);

    switch(RCN_GETFLAG(ptnode1->FLAG,RCN_FLAG_CROSS|RCN_FLAG_ONE|RCN_FLAG_TWO))
    {
      case RCN_FLAG_CROSS:
        if(ptnode1->WIRELIST->NEXT->NEXT==NULL)
        {
          RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_CROSS);
          RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_TWO);
        }
        break;
      case RCN_FLAG_TWO:
        RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_TWO);
        RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_ONE);
        break;
      case RCN_FLAG_ONE:
        RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_ONE);
        break;
      default:
        fprintf(stderr,"internal error : dellowire()\n");
        exit(-1);
    }

    switch(RCN_GETFLAG(ptnode2->FLAG,RCN_FLAG_CROSS|RCN_FLAG_ONE|RCN_FLAG_TWO))
    {
      case RCN_FLAG_CROSS:
        if(ptnode2->WIRELIST->NEXT->NEXT==NULL)
        {
          RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_CROSS);
          RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_TWO);
        }
        break;
      case RCN_FLAG_TWO:
        RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_TWO);
        RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_ONE);
        break;
      case RCN_FLAG_ONE:
        RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_ONE);
        break;
      default:
        fprintf(stderr,"internal error : dellowire()\n");
        exit(-1);
    }
  }
  
  if (TRACE_MODE == 'Y')
    (void)fprintf(stdout, "--- rcn --- dellowire  : between node %ld and node %ld on signal %ld\n",
                           node1, node2, ptsig->INDEX);

  return 1;
}

/*******************************************************************************
* function mergedellowire()                                                    *
*******************************************************************************/
void mergedellowire(ptsig)
losig_list *ptsig;
{
  lowire_list *ptwire;
  lowire_list *ptprev;
  lowire_list *suiv;

  if( !ptsig->PRCN )
  {
    fflush( stdout );
    fprintf( stderr, "*** RCN error in mergedellowire() : no RCN.\n" );
    EXIT(1); 
  }

  ptprev = NULL;
  for( ptwire = ptsig->PRCN->PWIRE ; ptwire ; ptwire = suiv )
  {
    suiv = ptwire->NEXT;
    if( RCN_GETFLAG( ptwire->FLAG, RCN_FLAG_DEL ) )
    {
      if( ptprev == NULL )
        ptsig->PRCN->PWIRE = ptwire->NEXT;
      else
        ptprev->NEXT = ptwire->NEXT;

      freelowire( ptwire );
    }
    else
      ptprev = ptwire;
  }
}

/*******************************************************************************
* function setdellowire()                                                      *
*******************************************************************************/
void setdellowire(ptsig, ptwire)
losig_list  *ptsig;
lowire_list *ptwire;
{
lonode_list	*ptnode1;
lonode_list	*ptnode2;
chain_list	*ptchain;

  RCN_SETFLAG( ptwire->FLAG, RCN_FLAG_DEL );

  if( gettabnode(ptsig->PRCN) != NULL)
  {
    ptnode1 = getlonode(ptsig, ptwire->NODE1);
    ptnode2 = getlonode(ptsig, ptwire->NODE2);

    /* optimiser */
    for(ptchain = ptnode1->WIRELIST;ptchain != NULL;ptchain = ptchain->NEXT)
      if(ptchain->DATA == (void *)ptwire) break;
    ptnode1->WIRELIST = delchain(ptnode1->WIRELIST,ptchain);

    for(ptchain = ptnode2->WIRELIST;ptchain != NULL;ptchain = ptchain->NEXT)
      if(ptchain->DATA == (void *)ptwire) break;
    ptnode2->WIRELIST = delchain(ptnode2->WIRELIST,ptchain);

    switch(RCN_GETFLAG(ptnode1->FLAG,RCN_FLAG_CROSS|RCN_FLAG_ONE|RCN_FLAG_TWO))
    {
      case RCN_FLAG_CROSS:
        if(ptnode1->WIRELIST->NEXT->NEXT==NULL)
        {
          RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_CROSS);
          RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_TWO);
        }
        break;
      case RCN_FLAG_TWO:
        RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_TWO);
        RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_ONE);
        break;
      case RCN_FLAG_ONE:
        RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_ONE);
        break;
      default:
        fprintf(stderr,"internal error : dellowire()\n");
        exit(-1);
    }

    switch(RCN_GETFLAG(ptnode2->FLAG,RCN_FLAG_CROSS|RCN_FLAG_ONE|RCN_FLAG_TWO))
    {
      case RCN_FLAG_CROSS:
        if(ptnode2->WIRELIST->NEXT->NEXT==NULL)
        {
          RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_CROSS);
          RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_TWO);
        }
        break;
      case RCN_FLAG_TWO:
        RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_TWO);
        RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_ONE);
        break;
      case RCN_FLAG_ONE:
        RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_ONE);
        break;
      default:
        fprintf(stderr,"internal error : dellowire()\n");
        exit(-1);
    }
  }
}

/*******************************************************************************
* function getlowire()                                                         *
*******************************************************************************/
lowire_list *getlowire(ptsig, node1, node2)
losig_list  *ptsig;
long  node1;
long  node2;
{
  lowire_list  *ptwire;
  lonode_list  *ptnode;
  chain_list   *ptchain;

  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : getlowire() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  if( ! gettabnode(ptsig->PRCN) )
  {
    for ( ptwire = ptsig->PRCN->PWIRE ; ptwire != NULL ; ptwire = ptwire->NEXT )
      if ( (   (ptwire->NODE1 == node1 && ptwire->NODE2 == node2)
             ||(ptwire->NODE1 == node2 && ptwire->NODE2 == node1) ) &&
           ! RCN_GETFLAG( ptwire->FLAG, RCN_FLAG_DEL )                 )
        return ptwire;
    return(NULL);
  }

  ptnode = givelonode(ptsig,node1);
  for(ptchain=ptnode->WIRELIST;ptchain!=NULL;ptchain=ptchain->NEXT)
  {
    ptwire = (lowire_list*)ptchain->DATA;
    if( ( ptwire->NODE1 == node2 || ptwire->NODE2 == node2  ) &&
        ! RCN_GETFLAG( ptwire->FLAG, RCN_FLAG_DEL )
      )
      return ptwire;
  }
  return(NULL);
}

/*******************************************************************************
* function getlonode()                                                         *
*******************************************************************************/
lonode_list *getlonode(ptsig, index)
losig_list *ptsig;
long  index;
{
  long taille;
  ptype_list *head;
  chain_list *scan;
  int max;
  int pos;

  head   = getptype( ptsig->PRCN->USER, RCN_LONODE );
  if( head == NULL)
  {
    fflush( stdout );
    fprintf( stderr, "*** rcn error *** getlonode()\n" );
    fprintf( stderr, "No table exist on losig %ld.\n", ptsig->INDEX );
    EXIT(1);
  }

  taille = (long)(getptype( ptsig->PRCN->USER, RCN_SIZETAB )->DATA); 
  scan   = head->DATA;

  pos=0;
  max = index / taille;
  for( pos = 0 ; pos <= max ; pos++ )
  {
    if( !scan )
    {
      fflush( stdout );
      fprintf( stderr, "*** rcn error *** getlonode()\n" );
      fprintf( stderr,
               "Non existant node on exist on losig %ld.\n",
               ptsig->INDEX
             );
      EXIT(1);
    }
    if(pos != max)
     scan = scan->NEXT ;
  } 
  
  pos = index - max * taille ;
  
  return( &(((lonode_list*)(scan->DATA))[pos]) );
}
 
/*******************************************************************************
* function givelonode()                                                        *
*******************************************************************************/
lonode_list *givelonode(ptsig, index)
losig_list  *ptsig;
long  index;
{
  long taille;
  ptype_list *head;
  chain_list *scan;
  chain_list *prev;
  int max;
  int pos;
  int i;
  lonode_list *ptnode;

  if( !ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** rcn error *** : getlonode impossible\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  if( index >= ptsig->PRCN->NBNODE )
    ptsig->PRCN->NBNODE = index+1;

  head   = getptype( ptsig->PRCN->USER, RCN_LONODE );
  if( head == NULL)
  {
    fflush( stdout );
    fprintf( stderr, "*** rcn error *** givelonode()\n" );
    fprintf( stderr, "No table exist on losig %ld.\n", ptsig->INDEX );
    EXIT(1);
  }

  taille = (long)(getptype( ptsig->PRCN->USER, RCN_SIZETAB )->DATA); 
  scan   = head->DATA;
  prev   = head->DATA;

  pos=0;
  max = index / taille;
  for( pos = 0 ; pos <= max ; pos++ )
  {
    if( !scan )
    {
      if( prev )
      {
        prev->NEXT = addchain( NULL, 
                               mbkalloc( sizeof( lonode_list ) * taille )
                             );
        scan = prev->NEXT;
      }
      else
      {
        head->DATA = addchain( NULL,
                               mbkalloc( sizeof( lonode_list ) * taille )
                             );
        scan = head->DATA;
      }

      for( i = 0 ; i < taille ; i++ )
      {
        ptnode = &(((lonode_list*)(scan->DATA))[i]);
        ptnode->INDEX = i + pos * taille ;
        ptnode->WIRELIST = NULL;
        ptnode->CTCLIST  = NULL;
        ptnode->FLAG     = 0;
        ptnode->USER     = NULL;
      }
        
    }
    prev = scan;
    if(pos != max)
     scan = scan->NEXT ;
  } 
  
  pos = index - max * taille ;
  
  return( &(((lonode_list*)(scan->DATA))[pos]) );
}
 
/*******************************************************************************
* function getloconnode()                                                      *
*******************************************************************************/

chain_list* getloconnode( node )
lonode_list *node;
{
  ptype_list		*ptptype;

  ptptype = getptype( node->USER, RCN_LOCON );
  if( !ptptype )
    return( NULL );

  return( (chain_list*)ptptype->DATA );
}
 
/*******************************************************************************
* function setloconnode()                                                      *
*******************************************************************************/

void setloconnode(ptlocon, node)
locon_list *ptlocon;
long node;
{ 
  losig_list		*ptsig;
  ptype_list		*ptptype;
  chain_list		*chain;
  lonode_list		*ptnode;

  ptsig = ptlocon->SIG;

  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : setloconnode() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  if( ptsig->PRCN->NBNODE <= node )
    ptsig->PRCN->NBNODE = node+1;

  if( gettabnode( ptsig->PRCN ) != NULL)
  {
    ptnode = givelonode( ptsig, node );

    if(!RCN_GETFLAG(ptnode->FLAG,RCN_FLAG_LOCON))
    {
      RCN_SETFLAG(ptnode->FLAG,RCN_FLAG_LOCON);
      ptnode->USER = addptype(ptnode->USER,RCN_LOCON,addchain(NULL,ptlocon));
    }
    else
    {
      ptptype = getptype(ptnode->USER,RCN_LOCON);
      for(chain = (chain_list *)ptptype->DATA; chain; chain = chain->NEXT)
        if(ptlocon == (locon_list *)chain->DATA)
          break;

      if(chain == NULL) 
        ptptype->DATA = addchain(ptptype->DATA,ptlocon);
    }
  }

  ptlocon->PNODE = addnum( ptlocon->PNODE, node );

}

/*******************************************************************************
* function delloconnode()                                                      *
*******************************************************************************/

void delloconnode(ptcon, index )
locon_list	*ptcon;
long		 index;
{
  losig_list	*ptsig;
  num_list	*scannum;
  num_list	*prev;
  ptype_list    *ptptype;
  chain_list    *scanchain;
  chain_list    *prevchain;
  lonode_list   *ptnode;

  ptsig = ptcon->SIG;

  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : delloconnode() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  scannum = ptcon->PNODE ;
  prev = NULL;

  while(1)
  {
    for(; scannum ; scannum = scannum->NEXT )
    {
      if( scannum->DATA == index )
        break;
      prev = scannum;
    }

   if(scannum)
   {
     if( prev )
     {
        prev->NEXT = scannum->NEXT;
        scannum->NEXT = NULL;
        freenum( scannum );
        scannum = prev->NEXT ;
     }
     else
     {
       ptcon->PNODE = scannum->NEXT;
       scannum->NEXT = NULL;
       freenum( scannum );
       scannum =  ptcon->PNODE ;
     }
   }
   else
     break ;
  }

  ptnode = NULL;

  if( gettabnode( ptsig->PRCN ) )
  {
    ptnode = getlonode( ptsig, index );
    ptptype = getptype( ptnode->USER, RCN_LOCON );

    prevchain = NULL;

    for( scanchain = (chain_list*)ptptype->DATA ; 
         scanchain ;
         scanchain = scanchain->NEXT
       )
    {
      if( scanchain->DATA == ptcon )
        break;
      prevchain = scanchain;
    }

    if( prevchain )
      prevchain->NEXT = scanchain->NEXT;
    else
      ptptype->DATA = scanchain->NEXT;
      
    scanchain->NEXT = NULL;
    freechain( scanchain );
    
    if( ptptype->DATA == NULL )
    {
      ptnode->USER = delptype( ptnode->USER, RCN_LOCON );
      RCN_CLEARFLAG( ptnode->FLAG, RCN_FLAG_LOCON );
    }
  }
}

/*******************************************************************************
* function addcapawire()                                                       *
*******************************************************************************/

void addcapawire(ptwire, capa)
lowire_list *ptwire;
float capa;
{
	ptwire->CAPA += capa;
}

/*******************************************************************************
* function setcapawire()                                                       *
*******************************************************************************/

void setcapawire(ptwire, capa)
lowire_list *ptwire;
float capa;
{
	ptwire->CAPA = capa;
}

/*******************************************************************************
* function addresiwire()                                                       *
*******************************************************************************/
void addresiwire(ptwire, resi)
lowire_list *ptwire;
float resi;
{
	ptwire->RESI += resi;
}

/*******************************************************************************
* function setresiwire()                                                       *
*******************************************************************************/
void setresiwire(ptwire, resi)
lowire_list *ptwire;
float resi;
{
	ptwire->RESI = resi;
}

/*******************************************************************************
* function addpararesiwire()                                                   *
*******************************************************************************/
void addpararesiwire(ptwire, resi)
lowire_list *ptwire;
float resi;
{
        if(ptwire->RESI==0.0 || resi==0.0)
	  ptwire->RESI = 0.0;
        else
          ptwire->RESI = 1 / ( 1/ptwire->RESI + 1/resi );
}

/*******************************************************************************
* lowire_list* heaplowire(void)						       *
*******************************************************************************/


lowire_list* heaplowire()
{
  lowire_list *ptwire;
  int i;

  if(HEAD_WIRE_FREE==NULL)
  {
    HEAD_WIRE_FREE=(lowire_list*)mbkalloc(RCNBUFSIZE*sizeof(lowire_list));
    ptwire=HEAD_WIRE_FREE;
    for(i=1;i<RCNBUFSIZE;i++)
    {
      ptwire->NEXT=ptwire+1;
      ptwire++;
    }
    ptwire->NEXT=NULL;
  }
  ptwire=HEAD_WIRE_FREE;
  HEAD_WIRE_FREE=HEAD_WIRE_FREE->NEXT;
  
  /* Clean the new wire */
  ptwire->NEXT		= NULL;
  ptwire->NODE1		= 0L;
  ptwire->NODE2		= 0L;
  ptwire->LAYER		= RCN_WIRE_UNKNOW;
  ptwire->FLAG 		= 0;
  ptwire->RESI		= 0.0;
  ptwire->CAPA		= 0.0;
  ptwire->X		= 0l;
  ptwire->Y		= 0l;
  ptwire->DX		= 0l;
  ptwire->DY		= 0l;
  ptwire->USER		= NULL;

  NBWIRE++;
  return(ptwire);
}

/*******************************************************************************
* loctc_list* heaploctc(void)   					       *
*******************************************************************************/

loctc_list* heaploctc()
{
  loctc_list *ptctc;
  int i;

  if(HEAD_CTC_FREE==NULL)
  {

    /* Le chainage des CTC est realise par le champs SIG1 */
   
    HEAD_CTC_FREE=(loctc_list*)mbkalloc(RCNBUFSIZE*sizeof(loctc_list));
    ptctc=HEAD_CTC_FREE;
    for(i=1;i<RCNBUFSIZE;i++)
    {
      ptctc->SIG1 = (struct losig *)ptctc+1;
      ptctc->NODE1 = -1000;
      ptctc++;
    }
    ptctc->SIG1 = NULL;
    ptctc->NODE1 = -1000;
  }

  ptctc=HEAD_CTC_FREE;
  HEAD_CTC_FREE=(loctc_list*)(HEAD_CTC_FREE->SIG1);

  /* Clean the new ctc */
  if( ptctc->NODE1 != -1000 )
  {
    printf( "***** Utilisation dans heaploctc d'une CTC deja prise.\n" );
    exit(-1);
  }
  
  ptctc->SIG1	 = NULL;
  ptctc->NODE1	 = 0;
  ptctc->SIG2	 = NULL;
  ptctc->NODE2	 = 0;
  ptctc->CAPA    = 0.0;
  ptctc->USER    = NULL;

  NBCTC++;
  return(ptctc);
}
/*******************************************************************************
* lorcnet_list* heaprcnet(void)						       *
*******************************************************************************/

lorcnet_list* heaprcnet()
{
  lorcnet_list *ptrcnet;
  int i;

  if(HEAD_RCNET_FREE==NULL)
  {
    HEAD_RCNET_FREE=(lorcnet_list*)mbkalloc(RCNBUFSIZE*sizeof(lorcnet_list));
    ptrcnet=HEAD_RCNET_FREE;

    for(i=1;i<RCNBUFSIZE;i++)
    {
      *((lorcnet_list**)ptrcnet) = ptrcnet + 1;
      ptrcnet++;
    }

    *((lorcnet_list**)ptrcnet)=NULL;
  }

  ptrcnet=HEAD_RCNET_FREE;
  HEAD_RCNET_FREE = *((lorcnet_list**)HEAD_RCNET_FREE);


  /* Clean the new rcnet figure */

  ptrcnet->CAPA		= 0.0;
  ptrcnet->PWIRE	= NULL;
  ptrcnet->NBNODE	= 1l;
  ptrcnet->PCTC		= NULL;
  ptrcnet->USER		= NULL;

  NBRCNET++;
  return(ptrcnet);
}

/*******************************************************************************
* void freercnet(lorcnet_list*)						       *
*******************************************************************************/

void freercnet(pt)
lorcnet_list	*pt;
{
  *((lorcnet_list**)pt) = HEAD_RCNET_FREE;
  HEAD_RCNET_FREE=pt;

  NBRCNET--;
}

/*******************************************************************************
* void freelowire(lowire_list*)						       *
*******************************************************************************/

void freelowire(pt)
lowire_list	*pt;
{
  if( pt->USER )
  {
     fflush( stdout );
     fprintf( stderr, "*** Warnig : freelowire. USER field not empty.\n" );
  }
  
  pt->NEXT=HEAD_WIRE_FREE;
  HEAD_WIRE_FREE=pt;

  NBWIRE--;
}

/*******************************************************************************
* void freeloctc(loctc_list*)						       *
*******************************************************************************/

void freeloctc(pt)
loctc_list	*pt;
{
  if( pt->USER )
  {
     fflush( stdout );
     fprintf( stderr, "*** Warnig : freeloctc. USER field not empty.\n" );
  }

  pt->SIG1      = (losig_list*)HEAD_CTC_FREE;
  if( pt->NODE1 == -1000 )
  {
    printf( "***** Liberation d'une CTC deja liberee.\n" );
    exit(0);
  }
  pt->NODE1     = -1000;
  HEAD_CTC_FREE = pt;
  NBCTC--;
}

/*******************************************************************************
* void freetable(rcnet_list*)						       *
*******************************************************************************/

void freetable(ptsig)
losig_list	*ptsig;
{
  chain_list	*head;
  chain_list	*scan;
  long  	 taille;
  ptype_list	*ptl;
  int            i;
  lonode_list   *ptnode;

  if( !ptsig->PRCN )
  {
    (void)fflush(stdout);
    (void)fprintf(stderr, "*** rcn error *** : freetable() impossible\n");
    (void)fprintf(stderr, "no rc on signal %ld\n",ptsig->INDEX);
    exit(-1);
  }

  ptl = getptype( ptsig->PRCN->USER, RCN_LONODE );
  head = (chain_list*)(ptl->DATA);
  if( head == NULL)
  {
    fflush( stdout );
    fprintf(stderr,"\n*** rcn error ***\n");
    fprintf(stderr,"You can use the function freetable only when de node\n");
    fprintf(stderr,"table exist.\n");
    exit(-1);
  }

  taille = (long)(getptype( ptsig->PRCN->USER, RCN_SIZETAB )->DATA );
  for( scan = head; scan ; scan = scan->NEXT )
  {
    for( i=0 ; i<taille ; i++ )
    {
      ptnode = &(((lonode_list*)(scan->DATA))[i]);
      if( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON ) )
      {
        ptl = getptype( ptnode->USER, RCN_LOCON );
        if( ptl )
          freechain( ptl->DATA );
        ptnode->USER = delptype( ptnode->USER, RCN_LOCON );
        freechain( ptnode->WIRELIST );
        freechain( ptnode->CTCLIST  );
        if( ptnode->USER )
        {
          fflush( stdout );
          fprintf( stderr, "*** rcn warning in freetable : non empty USER.\n");
        }
      }
    }

    mbkfree( scan->DATA );
  }

  freechain(head);
  ptsig->PRCN->USER = delptype( ptsig->PRCN->USER, RCN_LONODE  );
  ptsig->PRCN->USER = delptype( ptsig->PRCN->USER, RCN_SIZETAB );
}

/*******************************************************************************
* void buildtable(ptsig)						       *
*******************************************************************************/

void buildtable(ptsig)
losig_list	*ptsig;
{
  lowire_list		*ptwire;
  lonode_list		*ptnode1;
  lonode_list		*ptnode2;
  chain_list		*scancon;
  locon_list		*ptcon;
  num_list		*scannum;
  lonode_list		*ptnode;
  ptype_list		*ptlocon;
  ptype_list		*pt;
  chain_list		*scanctc;
  loctc_list		*ptloctc;

  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : buildtable() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  pt = getptype( ptsig->USER, LOFIGCHAIN );
  if( !pt )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : buildtable() impossible.\n");
    fprintf(stderr,"Call the function lofigchain() before buildtable().\n");
    EXIT(1);
  }

  if( gettabnode( ptsig->PRCN ) )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : buildtable() impossible.\n");
    fprintf(stderr,"The node table already exist.\n");
    EXIT(1);
  }

  ptsig->PRCN->USER = addptype( ptsig->PRCN->USER,
                                RCN_SIZETAB,
                                (void*)(ptsig->PRCN->NBNODE + 1)
                              );
  ptsig->PRCN->USER = addptype( ptsig->PRCN->USER,
                                RCN_LONODE,
                                NULL
                              );
  /* Cree les tables million pour tous les noeuds jusqu'a nbnode */
  givelonode(ptsig,ptsig->PRCN->NBNODE);

  if( ptsig->PRCN->PWIRE )
  {
    for( ptwire = ptsig->PRCN->PWIRE ; ptwire != NULL ; ptwire = ptwire->NEXT )
    {
      if( RCN_GETFLAG( ptwire->FLAG, RCN_FLAG_DEL ) )
        continue;
      ptnode1           = givelonode(ptsig,ptwire->NODE1);
      ptnode2           = givelonode(ptsig,ptwire->NODE2);
      ptnode1->WIRELIST = addchain(ptnode1->WIRELIST,ptwire);
      ptnode2->WIRELIST = addchain(ptnode2->WIRELIST,ptwire);
  
      switch( 
       RCN_GETFLAG( ptnode1->FLAG, RCN_FLAG_ONE | RCN_FLAG_CROSS | RCN_FLAG_TWO)
	    )
      {
        case RCN_FLAG_ONE:
          RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_ONE);
          RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_TWO);
          break;
        case RCN_FLAG_TWO:
          RCN_CLEARFLAG(ptnode1->FLAG,RCN_FLAG_TWO);
          RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_CROSS);
          break;
        case RCN_FLAG_CROSS:
          break;
        default:
          RCN_SETFLAG(ptnode1->FLAG,RCN_FLAG_ONE);
      }
  
      switch(
       RCN_GETFLAG( ptnode2->FLAG, RCN_FLAG_ONE | RCN_FLAG_CROSS | RCN_FLAG_TWO)
	    )
      {
        case RCN_FLAG_ONE:
          RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_ONE);
          RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_TWO);
          break;
        case RCN_FLAG_TWO:
          RCN_CLEARFLAG(ptnode2->FLAG,RCN_FLAG_TWO);
          RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_CROSS);
          break;
        case RCN_FLAG_CROSS:
          break;
        default:
          RCN_SETFLAG(ptnode2->FLAG,RCN_FLAG_ONE);
      }
    }  
  }

  for( scanctc = ptsig->PRCN->PCTC ; scanctc ; scanctc = scanctc->NEXT )
  {
    ptloctc = (loctc_list*)( scanctc->DATA );
    if( ptloctc->SIG1 == ptsig && ptloctc->NODE1 )
    {
      ptnode1 = givelonode( ptsig, ptloctc->NODE1 );
      ptnode1->CTCLIST = addchain( ptnode1->CTCLIST, ptloctc );
    }
    if( ptloctc->SIG2 == ptsig && ptloctc->NODE2 )
    {
      ptnode2 = givelonode( ptsig, ptloctc->NODE2 );
      ptnode2->CTCLIST = addchain( ptnode2->CTCLIST, ptloctc );
    }
  }

  for( scancon = (chain_list*)(pt->DATA) ; scancon ; scancon = scancon->NEXT )
  {
    ptcon = (locon_list*)(scancon->DATA);

    for( scannum = ptcon->PNODE ; scannum ; scannum = scannum->NEXT )
    {
      ptnode = givelonode( ptsig, scannum->DATA );

      if( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON ) )
      {
        ptlocon = getptype(ptnode->USER, RCN_LOCON );
        ptlocon->DATA=addchain((chain_list*)(ptlocon->DATA),ptcon);
      }
      else
      {
        RCN_SETFLAG( ptnode->FLAG, RCN_FLAG_LOCON );
        ptnode->USER = addptype( ptnode->USER, RCN_LOCON, addchain(NULL,ptcon));
      }
    }
  }
}

/*******************************************************************************
*                                                                              *
*******************************************************************************/

chain_list*	gettabnode( prcnet )
lorcnet_list		*prcnet;
{
  ptype_list		*pt;

  pt = getptype( prcnet->USER, RCN_LONODE );
  if( pt )
    return( (chain_list*)pt->DATA );
  return( NULL );
}

/*******************************************************************************
*                                                                              *
*******************************************************************************/

void delrcnlocon( ptloc )
 locon_list	*ptloc;
{
  freenum( ptloc->PNODE );
}

/*******************************************************************************
*                                                                              *
*******************************************************************************/

int chkloopmain( ptsig, flag, org, from )
losig_list	*ptsig;
unsigned char	 flag;
long		 org;
lowire_list	*from;
{
  lonode_list	*ptnode;
  long		 current_node;
  int		 r;
  chain_list	*scanchain;
  lowire_list	*current_wire;

  ptnode = getlonode(ptsig,org);
  if( RCN_GETFLAG(ptnode->FLAG, flag) )
    return(2);

  RCN_SETFLAG( ptnode->FLAG, flag );

  /*
  Récursif sur les branches multiples
  Itératif entre 2 wires
  */

  for( scanchain = ptnode->WIRELIST ; scanchain ; scanchain = scanchain->NEXT )
  {
    current_wire = (lowire_list*)(scanchain->DATA);
    if( current_wire != from )
    {
      current_node = (current_wire->NODE1==org ? current_wire->NODE2 :
                                                 current_wire->NODE1 );
      ptnode = getlonode( ptsig, current_node );

      while( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_TWO ) )
      {
        if( RCN_GETFLAG(ptnode->FLAG,flag) )
          return(2);

        RCN_SETFLAG(ptnode->FLAG,flag);

        current_wire=(ptnode->WIRELIST->DATA==current_wire ?
                      ptnode->WIRELIST->NEXT->DATA :
                      ptnode->WIRELIST->DATA                  );

        current_node=(current_wire->NODE1==current_node ? current_wire->NODE2 :
                                                 current_wire->NODE1 );

        ptnode=getlonode(ptsig,current_node);
      }

      if( RCN_GETFLAG(ptnode->FLAG,RCN_FLAG_ONE))
      {
        if(RCN_GETFLAG(ptnode->FLAG,flag))
          return(2);
        RCN_SETFLAG(ptnode->FLAG,flag);
      }

      if( RCN_GETFLAG(ptnode->FLAG,RCN_FLAG_CROSS))
      {
        r = chkloopmain(ptsig,flag,current_node,current_wire);
        if(r)
          return(r);
      }
    }
  }
  return(0);
}

int chkloop( ptsig )
losig_list	*ptsig;
{
  int		 r=0;
  long		 i;
  lonode_list	*ptnode;

  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : chkloop() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  if( ! gettabnode( ptsig->PRCN ) )
    return(1);

  clearallwireflag(ptsig->PRCN,RCN_FLAG_PASS);

  for( i=1; i<ptsig->PRCN->NBNODE; i++ )
  {
    ptnode = givelonode( ptsig, i );
    if( !RCN_GETFLAG(ptnode->FLAG,RCN_FLAG_PASS)  )
    {
      r = chkloopmain( ptsig, RCN_FLAG_PASS, i, NULL );

      if(r)
         break;
    }
  }

  return(r);
}

/*******************************************************************************
* void clearallwireflag( ptrcnet,flag )                                        *
*******************************************************************************/

void clearallwireflag(ptrcnet,flag)
lorcnet_list    *ptrcnet;
unsigned char    flag;
{
  long                   i;
  ptype_list		*head;
  long  		 taille;
  chain_list		*scan;

  head = getptype( ptrcnet->USER, RCN_LONODE );
  if( ! head )
    return;

  taille = (long)(getptype( ptrcnet->USER, RCN_SIZETAB )->DATA);
  for( scan = (chain_list*)(head->DATA) ; scan ; scan = scan->NEXT )
  {
    for( i=0 ; i<taille ; i++ )
    {
      RCN_CLEARFLAG( ((lonode_list*)(scan->DATA))[i].FLAG,flag);
    }
  }

}

int	rclevel( ptsig )
losig_list	*ptsig;
{
  if( ! ptsig->PRCN )
    return( MBK_RC_A );
  if( ptsig->PRCN->PWIRE != NULL && ptsig->PRCN->PCTC != NULL )
    return( MBK_RC_E );
  if( ptsig->PRCN->PWIRE == NULL && ptsig->PRCN->PCTC == NULL )
    return( MBK_RC_B );
  if( ptsig->PRCN->PCTC == NULL )
    return( MBK_RC_C );
  if( ptsig->PRCN->PWIRE == NULL )
    return( MBK_RC_D );

  /* never reached */
  return(0);
}

chain_list      *getallctc( ptfig )
lofig_list      *ptfig;
{
  chain_list    *headctc;
  losig_list    *scansig;
  chain_list    *scanctc;
  loctc_list    *ptctc;

  headctc = NULL;
  
  for( scansig = ptfig->LOSIG ; scansig ; scansig = scansig->NEXT )
  {
    if( scansig->PRCN )
    {
      for( scanctc = scansig->PRCN->PCTC ; scanctc ; scanctc = scanctc->NEXT )
      {
        ptctc = (loctc_list*)(scanctc->DATA);
        
        if( ! getptype( ptctc->USER, RCN_REPORTED ) )
        {
          ptctc->USER = addptype( ptctc->USER, RCN_REPORTED, NULL );
          headctc = addchain( headctc, ptctc );
        }
      }
    }  
  }

  for( scanctc = headctc ; scanctc ; scanctc = scanctc->NEXT )
  {
    ptctc = (loctc_list*)(scanctc->DATA);

    ptctc->USER = delptype( ptctc->USER, RCN_REPORTED );
  }

  return( headctc );
}

void statrcn(void)
{
  printf( "NBRCNET = %u\n", NBRCNET );
  printf( "NBWIRE  = %u\n", NBWIRE  );
  printf( "NBCTC   = %u\n", NBCTC   );
}

void chkrcn( ptsig )
losig_list	*ptsig;
{
  int            trouve;
  lonode_list   *ptnode;
  chain_list    *scanchain;
  chain_list    *verifchain;
  lowire_list   *scanwire;
  int            ctcok;
  int            wireok;
  int            conok;
  int            i;
  int            nbwire;
  ptype_list    *ptptype;
  locon_list    *ptcon;
  num_list      *scannum;
  chain_list    *scanchcon;
  loctc_list    *ptctc;
  loctc_list    *ptctc2;
  losig_list    *sig2;
   

  
  if( !ptsig->PRCN )
    return;

  printf( "Verification coherence vue RC pour signal %ld [%s].\n",
          ptsig->INDEX,
          gettabnode( ptsig->PRCN ) ? "TABLE" : "SANS TABLE"
        );
  
  ctcok  = 1;
  wireok = 1;
  conok  = 1;

  for( scanchain = ptsig->PRCN->PCTC ; scanchain ; scanchain = scanchain->NEXT )
  {
    if( !scanchain->DATA )
    {
      ctcok = 0;
      printf( "   [ 6] CTC nulle sur le signal %ld.\n", ptsig->INDEX );
    }
    ptctc = (loctc_list*)(scanchain->DATA);

    if( ptctc->SIG1 != ptsig && ptctc->SIG2 != ptsig )
    {
      ctcok = 0;
      printf( "   [ 1] CTC pas sur le signal entre %ld.%ld et %ld.%ld.\n",
              ptctc->SIG1->INDEX,
              ptctc->NODE1,
              ptctc->SIG2->INDEX,
              ptctc->NODE2
            );
    }
 
  }
  
  if( gettabnode( ptsig->PRCN ) )
  {
    /* Verification des CTC */

    for( scanchain = ptsig->PRCN->PCTC ;
         scanchain ;
         scanchain = scanchain->NEXT
       )
    {
      ptctc = (loctc_list*)(scanchain->DATA);

      sig2 = NULL;

      if( ptctc->SIG1 != ptsig )
        sig2 = ptctc->SIG1;
      if( ptctc->SIG2 != ptsig )
        sig2 = ptctc->SIG2;

      if( ptctc->SIG1 != ptsig && ptctc->SIG2 != ptsig )
      {
      }
      else
      {
        if( sig2 )
        {
          trouve = 0;
           
          for( verifchain = sig2->PRCN->PCTC ;
               verifchain ;
               verifchain = verifchain->NEXT
             )
          {
            if( verifchain->DATA == ptctc )
            {
              if( trouve )
              {
                ctcok = 0;
                printf( "   [ 2] CTC %ld.%ld et %ld.%ld definie plusieurs fois sur le signal %ld.\n",
                         ptctc->SIG1->INDEX,
                         ptctc->NODE1,
                         ptctc->SIG2->INDEX,
                         ptctc->NODE2,
                         sig2->INDEX
                      );
              }
              trouve = 1;
            }
          }

          if( !trouve )
          {
            ctcok = 0;
            printf( "   [ 3] CTC %ld.%ld et %ld.%ld non definie sur le signal %ld.\n",
                     ptctc->SIG1->INDEX,
                     ptctc->NODE1,
                     ptctc->SIG2->INDEX,
                     ptctc->NODE2,
                     sig2->INDEX
                  );
          }
        }

        if( ptsig == ptctc->SIG1 && ptctc->NODE1 )
        {
          ptnode = getlonode( ptsig, ptctc->NODE1 );
          
          trouve = 0;
          for( verifchain = ptnode->CTCLIST ;
               verifchain ;
               verifchain = verifchain->NEXT
             )
          {
            if( verifchain->DATA == ptctc )
            {
              if( trouve )
              {
                ctcok = 0;
                printf( "   [ 4] CTC %ld.%ld et %ld.%ld definie plusieurs fois sur le noeud %ld.\n",
                         ptctc->SIG1->INDEX,
                         ptctc->NODE1,
                         ptctc->SIG2->INDEX,
                         ptctc->NODE2,
                         ptctc->NODE1
                  );
              }
              trouve = 1;
            }            
          }
          if( !trouve )
          {
            ctcok = 0;
            printf( "   [ 5] CTC %ld.%ld et %ld.%ld non definie sur le noeud %ld.\n",
                     ptctc->SIG1->INDEX,
                     ptctc->NODE1,
                     ptctc->SIG2->INDEX,
                     ptctc->NODE2,
                     ptctc->NODE1
                  );
          }
        }

        if( ptsig == ptctc->SIG2 && ptctc->NODE2 )
        {
          ptnode = getlonode( ptsig, ptctc->NODE2 );
          
          trouve = 0;
          for( verifchain = ptnode->CTCLIST ;
               verifchain ;
               verifchain = verifchain->NEXT
             )
          {
            if( verifchain->DATA == ptctc )
            {
              if( trouve )
              {
                ctcok = 0;
                printf( "   [ 6] CTC %ld.%ld et %ld.%ld definie plusieurs fois sur le noeud %ld.\n",
                         ptctc->SIG1->INDEX,
                         ptctc->NODE1,
                         ptctc->SIG2->INDEX,
                         ptctc->NODE2,
                         ptctc->NODE2
                  );
              }
              trouve = 1;
            }            
          }
          if( !trouve )
          {
            ctcok = 0;
            printf( "   [ 7] CTC %ld.%ld et %ld.%ld non definie sur le noeud %ld.\n",
                     ptctc->SIG1->INDEX,
                     ptctc->NODE1,
                     ptctc->SIG2->INDEX,
                     ptctc->NODE2,
                     ptctc->NODE2
                  );
          }
        }
      }

      for( verifchain = scanchain->NEXT ;
           verifchain ;
           verifchain = verifchain->NEXT
         ) 
      {
        ptctc2 = (loctc_list*)(verifchain->DATA);

        if( ( ptctc->SIG1  == ptctc2->SIG1  &&
              ptctc->NODE1 == ptctc2->NODE1 &&
              ptctc->SIG2  == ptctc2->SIG2  &&
              ptctc->NODE2 == ptctc2->NODE2    ) ||          
            ( ptctc->SIG2  == ptctc2->SIG1  &&
              ptctc->NODE2 == ptctc2->NODE1 &&
              ptctc->SIG1  == ptctc2->SIG2  &&
              ptctc->NODE1 == ptctc2->NODE2    )   )
        {
          ctcok = 0;
          printf( "   [ 8] Plusieurs CTC entre %ld.%ld et %ld.%ld c=%g.\n",
                  ptctc->SIG1->INDEX,
                  ptctc->NODE1,
                  ptctc->SIG2->INDEX,
                  ptctc->NODE2,
                  ptctc->CAPA
                );
        }
      }
    }   

    for( i=1 ; i < ptsig->PRCN->NBNODE ; i++ )
    {
      ptnode = getlonode( ptsig, i );
      if( !ptnode )
        continue;

      for( scanchain = ptnode->CTCLIST ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {
        ptctc = (loctc_list*)scanchain->DATA;
        
        if( ptctc->SIG1 != ptsig && ptctc->SIG2 != ptsig )
        {
          ctcok = 0 ;
          printf( "   [ 9] CTC pas sur le signal entre %ld.%ld et %ld.%ld sur noeud %d.\n",
                  ptctc->SIG1->INDEX,
                  ptctc->NODE1,
                  ptctc->SIG2->INDEX,
                  ptctc->NODE2,
                  i
                );
        }
        
        if( !( (ptctc->SIG1 == ptsig && ptctc->NODE1 == i ) ||
               (ptctc->SIG2 == ptsig && ptctc->NODE2 == i )   )  )
        {
          ctcok = 0 ;
          printf( "   [11] CTC entre %ld.%ld et %ld.%ld sur mauvais noeud %d.\n",
                  ptctc->SIG1->INDEX,
                  ptctc->NODE1,
                  ptctc->SIG2->INDEX,
                  ptctc->NODE2,
                  i
                );
        }
      }
    }

    /* Verification des wires */

    for( scanwire = ptsig->PRCN->PWIRE ; scanwire ; scanwire = scanwire->NEXT )
    {
      if( RCN_GETFLAG( scanwire->FLAG, RCN_FLAG_DEL ) )
        continue;
      ptnode = getlonode( ptsig, scanwire->NODE1 );

      trouve = 0;
      for( scanchain = ptnode->WIRELIST ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {
        if( scanchain->DATA == scanwire )
        {
          if( trouve == 1 )
          {
            wireok = 0;
            printf( "   [12] WIRE entre %ld et %ld existe plusieurs fois sur le noeud %ld.\n",
                    scanwire->NODE1,
                    scanwire->NODE2,
                    ptnode->INDEX
                  );
          }
          trouve = 1;
        }
      }
      
      if( trouve == 0 )
      {
        wireok = 0;
        printf( "   [13] WIRE entre %ld et %ld n'existe pas sur le noeud %ld.\n",
                scanwire->NODE1,
                scanwire->NODE2,
                ptnode->INDEX
              );
      }
      
      ptnode = getlonode( ptsig, scanwire->NODE2 );

      trouve = 0;
      for( scanchain = ptnode->WIRELIST ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {
        if( scanchain->DATA == scanwire )
        {
          if( trouve == 1 )
          {
            wireok = 0;
            printf( "   [14] WIRE entre %ld et %ld existe plusieurs fois sur le noeud %ld.\n",
                    scanwire->NODE1,
                    scanwire->NODE2,
                    ptnode->INDEX
                  );
          }
          trouve = 1;
        }
      }
      
      if( trouve == 0 )
      {
        wireok = 0;
        printf( "   [15] WIRE entre %ld et %ld n'existe pas sur le noeud %ld.\n",
                scanwire->NODE1,
                scanwire->NODE2,
                ptnode->INDEX
              );
      }
    }

    for( i=1 ; i < ptsig->PRCN->NBNODE ; i++ )
    {
      ptnode = getlonode( ptsig, i );
      if( !ptnode )
        continue;

      nbwire = 0;
      for( scanchain = ptnode->WIRELIST ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {
        nbwire++;
        for( scanwire = ptsig->PRCN->PWIRE ;
             scanwire ;
             scanwire = scanwire->NEXT
           )
        {
           if( scanwire == scanchain->DATA )
             break;
        }
        if( !scanwire )
        {
           wireok = 0;
           printf( "   [16] Wire %lX inexistant trouve sur le noeud %d.\n",
                   (long)scanchain->DATA,
                   i
                 );
        }
      }

      switch( nbwire )
      {
      case 0 :
        if( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_ONE   ) ||
            RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_TWO   ) ||
            RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_CROSS )    )
        {
          printf( "   [17] Mauvais indicateur de nombre de wire sur le noeud %ld.\n",
                  ptnode->INDEX
                );
          wireok = 0;
        }
        break;
      case 1 :
        if( !RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_ONE   ) ||
             RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_TWO   ) ||
             RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_CROSS )    )
        {
          printf( "   [18] Mauvais indicateur de nombre de wire sur le noeud %ld.\n",
                  ptnode->INDEX
                );
          wireok = 0;
        }
        break;
      case 2 :
        if(  RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_ONE   ) ||
            !RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_TWO   ) ||
             RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_CROSS )    )
        {
          printf( "   [19] Mauvais indicateur de nombre de wire sur le noeud %ld.\n",
                  ptnode->INDEX
                );
          wireok = 0;
        }
        break;
      default : 
        if(  RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_ONE   ) ||
             RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_TWO   ) ||
            !RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_CROSS )    )
        {
          printf( "   [20] Mauvais indicateur de nombre de wire sur le noeud %ld.\n",
                  ptnode->INDEX
                );
          wireok = 0;
        }
      }
    }

    /* Verification connecteurs */

    ptptype = getptype( ptsig->USER, LOFIGCHAIN );
    if( !ptptype )
      printf( "  Pas de lofigchain -> connecteurs non testes.\n" );
    else
    {
      for( scanchain = (chain_list*)ptptype->DATA ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {
        ptcon = (locon_list*)(scanchain->DATA) ; 
        
        for( scannum = ptcon->PNODE ; scannum ; scannum = scannum->NEXT )
        {
          ptnode = getlonode( ptsig, scannum->DATA );
          if( !RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON ) )
          {
            printf( "   [21] Le noeud %ld devrait porter l'indicateur RCN_FLAG_LOCON pour le connecteur %s.\n",
                    ptnode->INDEX,
                    ptcon->NAME
                  );
            conok = 0;
          }
          
          ptptype = getptype( ptnode->USER, RCN_LOCON );
          if( !ptptype )
            printf( "   [22] Pas de ptype RCN_LOCON sur le noeud %ld (locon %s).\n",
                    ptnode->INDEX,
                    ptcon->NAME
                  );
          else
          {
            trouve = 0;
            for( scanchcon = (chain_list*)ptptype->DATA ;
                 scanchcon ;
                 scanchcon = scanchcon->NEXT
               )
              if( scanchcon->DATA == ptcon )
                trouve = 1;

            if( trouve == 0 )
            {
              printf( "   [24] Locon %s non defini dans le node %ld.\n",
                      ptcon->NAME,
                      ptnode->INDEX
                    );
              conok = 0;
            }
          }
        }
      }

      for( i = 0 ; i < ptsig->PRCN->NBNODE ; i++ )
      {
        ptnode  = getlonode( ptsig, i );
        if( !ptnode )
          continue;

        ptptype = getptype( ptnode->USER, RCN_LOCON );

        if( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON ) && ptptype )
        {
          for( scanchcon = ptptype->DATA ;
               scanchcon ;
               scanchcon = scanchcon->NEXT )
          {
            ptcon = (locon_list*)(scanchcon->DATA);
            trouve = 0;
            for( scannum = ptcon->PNODE; scannum ; scannum = scannum->NEXT )
              if( scannum->DATA == i )
                trouve = 1;
            if( trouve == 0 )
            {
              printf( "   [26] Le noeud %d n'est pas defini dans le locon %s.\n",
                      i,
                      ptcon->NAME
                    );
              conok = 0;
            }
          }
        }

        if( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON ) && ! ptptype )
        {
          printf( "   [27] Pas de champs ptype associe au RCN_FLAG_LOCON.\n" );
          conok = 0;
        }

        if( !RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON ) && ptptype )
        {
          printf( "   [28] Champs ptype non associe au RCN_FLAG_LOCON.\n" );
          conok = 0;
        }
      }
    }
  }
  
  if( wireok )
    printf( " - structures WIRE coherentes.\n" );
  else
    printf( " - structures WIRE incoherentes.\n" );

  if( ctcok )
    printf( " - structures CTC coherentes.\n" );
  else
    printf( " - structures CTC incoherentes.\n" );

  if( conok )
    printf( " - Indications CONNECTEUR coherentes.\n" );
  else
    printf( " - Indications CONNECTEUR incoherentes.\n" );

  if( !wireok || !ctcok || !conok )
    EXIT(1);
}

float rcncalccapa( losig_list *ptsig )
{
  lowire_list *scanwire;
  chain_list  *scanctc;

  if( ! ptsig->PRCN )
  {
    fflush(stdout);
    fprintf(stderr,"*** mbk error *** : buildtable() impossible.\n");
    fprintf(stderr,"No rc data on signal %ld.\n",ptsig->INDEX);
    EXIT(1);
  }

  ptsig->PRCN->CAPA = 0.0;
  for( scanwire = ptsig->PRCN->PWIRE ; scanwire ; scanwire = scanwire->NEXT )
    ptsig->PRCN->CAPA+=scanwire->CAPA;
  for( scanctc = ptsig->PRCN->PCTC ; scanctc ; scanctc = scanctc->NEXT )
    ptsig->PRCN->CAPA+=((loctc_list*)scanctc->DATA)->CAPA;

  return( ptsig->PRCN->CAPA );
}
