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

#ident "$Id: rcn_lo_util.c,v 1.5 2012/05/14 14:20:24 alliance Exp $"

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

typedef struct s_todo
{
  struct s_todo       *next;
  lonode_list         *ptnode;
  chain_list          *head_wire;
} todolist ;

todolist*        breakloop2 __P(( losig_list*,
                                  lonode_list*,
                                  lowire_list*,
                                  todolist*,
                                  chain_list**
                               ));

void connexe_rec __P(( losig_list*,
                       lowire_list*,
                       lonode_list*,
                       long
                    ));

/******************************************************************************/

void		duplorcnet( ptsig_dest, ptsig_src )
losig_list	*ptsig_dest;
losig_list	*ptsig_src;
{
  lowire_list	*scanlowire;
  lowire_list	*newwire;

  if( ptsig_dest->PRCN )
  {
    fflush( stdout );
    fprintf( stderr,
       "*** MBK error in duplorcnet *** : ptsig_dest have already a rc datas.\n"
           );
    EXIT(1);
  }
  
  if( !ptsig_src->PRCN )
    return;

  addlorcnet( ptsig_dest );
  
  /* Capacité du signal */
  ptsig_dest->PRCN->CAPA = ptsig_src->PRCN->CAPA;
  
  /* Wire constituant le signal */
  for( scanlowire = ptsig_src->PRCN->PWIRE ;
       scanlowire ;
       scanlowire = scanlowire->NEXT
     )
  {

    if( RCN_GETFLAG( scanlowire->FLAG, RCN_FLAG_DEL ) )
      continue;
    newwire = heaplowire();
    
    newwire->NEXT  = ptsig_dest->PRCN->PWIRE;
    newwire->RESI  = scanlowire->RESI;
    newwire->CAPA  = scanlowire->CAPA;
    newwire->X     = scanlowire->X;
    newwire->Y     = scanlowire->Y;
    newwire->DX    = scanlowire->DX;
    newwire->DY    = scanlowire->DY;
    newwire->NODE1 = scanlowire->NODE1;
    newwire->NODE2 = scanlowire->NODE2;
    newwire->LAYER = scanlowire->LAYER;
    newwire->FLAG  = scanlowire->FLAG;
    newwire->USER  = NULL;
      
    ptsig_dest->PRCN->PWIRE = newwire;
  }

  /* CTC sur le signal : non gere */
  ptsig_dest->PRCN->PCTC = NULL;

  /* NBNODE */
  ptsig_dest->PRCN->NBNODE = ptsig_src->PRCN->NBNODE ;

  /* USER */
  ptsig_dest->PRCN->USER = NULL;
}

/******************************************************************************/

todolist*        breakloop2( ptsig, ptnode, curwire, headtodo, cleanwire )
losig_list      *ptsig;
lonode_list     *ptnode;
lowire_list     *curwire;
todolist        *headtodo;
chain_list      **cleanwire;
{
  lowire_list   *tmpwire;
  chain_list    *scanchain;
  todolist      *newtodo;

  if( curwire )
  {
    if( RCN_GETFLAG( curwire->FLAG, RCN_FLAG_PASS ) )
    {
      fflush( stdout );
      fprintf( stderr, "*** rcn error ***\n" );
      fprintf( stderr, "Internal error in function breakloop2().\n" );
      EXIT(1);
    }

    RCN_SETFLAG( curwire->FLAG, RCN_FLAG_PASS );
  }
  
  do
  {
    /* On trouve une boucle */
    if( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_PASS ) )
    {
      tmpwire = addlowire( ptsig,
                           curwire->LAYER,
                           curwire->FLAG,
                           curwire->RESI,
                           curwire->CAPA,
                           curwire->X,
                           curwire->Y,
                           curwire->DX,
                           curwire->DY,
                           curwire->NODE1,
                           addlonode( ptsig, NULL )
                         );
      *cleanwire = addchain( *cleanwire, curwire );
      RCN_SETFLAG( tmpwire->FLAG, RCN_FLAG_PASS );
      RCN_SETFLAG( curwire->FLAG, RCN_FLAG_PASS );
    }
    else
    {
      RCN_SETFLAG( ptnode->FLAG, RCN_FLAG_PASS );
    }
   
    /* Recherche un wire non parcouru sur le noeud courant */
    for( scanchain = ptnode->WIRELIST ;
         scanchain ;
         scanchain = scanchain->NEXT
       )
    {
      curwire = (lowire_list*)(scanchain->DATA);
      if( ! RCN_GETFLAG( curwire->FLAG, RCN_FLAG_PASS ) )
        break;
    }

    if( ! scanchain )
      break; /* on quitte le while */
    
    /* Ajoute les autre wire non parcouru sur le noeud dans une
     * structure todo */
    
    newtodo = NULL;

    if( scanchain )
    {
      for( scanchain = scanchain->NEXT ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {
        tmpwire = (lowire_list*)(scanchain->DATA);
  
        if( ! RCN_GETFLAG( tmpwire->FLAG, RCN_FLAG_PASS ) )
        {
          if( !newtodo )
          {
            newtodo            = (todolist*)(mbkalloc( sizeof(todolist) ) );
            newtodo->next      = headtodo ;
            headtodo           = newtodo;
            newtodo->ptnode    = ptnode;
            newtodo->head_wire = NULL;
          }
          newtodo->head_wire = addchain( newtodo->head_wire, tmpwire );
        }
      }
    }

    ptnode = getlonode( ptsig,
                        ( curwire->NODE1 == ptnode->INDEX ) ? curwire->NODE2 :
                                                              curwire->NODE1
                      );
    
    RCN_SETFLAG( curwire->FLAG, RCN_FLAG_PASS );
  }
  while(1); /* c'est ben vrai! */

  return( headtodo );
}

int              breakloop( ptsig )
losig_list      *ptsig;
{
  
  todolist              *headtodo ;  
  todolist              *curtodo ;  
  todolist              *scantodo ;  
  todolist              *newtodo ;  
  lowire_list           *firstwire;
  lowire_list           *curwire;
  lonode_list           *ptnode;
  chain_list            *cleanwire;
  chain_list            *scanchain;
  
  if( !ptsig->PRCN || !gettabnode( ptsig->PRCN ) )
  {
    fflush( stdout );
    fprintf( stderr, "*** rcn error *** : breakloop() impossible.\n" );
    fprintf( stderr, "No rc data on signal %ld.\n", ptsig->INDEX );
    EXIT( 1 );
  }
  
  clearallwireflag( ptsig->PRCN, RCN_FLAG_PASS );
  cleanwire = NULL;
  
  /* Boucle pour parcourir les differentes composantes connexes */

  for( firstwire  = ptsig->PRCN->PWIRE ;
       firstwire ;
       firstwire = firstwire->NEXT
     )
  {
    if( RCN_GETFLAG( firstwire->FLAG, RCN_FLAG_DEL ) )
      continue;

    ptnode = getlonode( ptsig, firstwire->NODE1 );
    
    if( ! RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_PASS ) )
    {
      /* Corps d'une des composante connexe du reseau */
     
      headtodo = breakloop2( ptsig, ptnode, NULL, NULL, &cleanwire );

      while( headtodo )
      {
        newtodo = NULL;
        
        for( scantodo = headtodo ; scantodo ; scantodo = scantodo->next )
        {
          for( scanchain = scantodo->head_wire ;
               scanchain ;
               scanchain = scanchain->NEXT
             )
          {
          
            curwire = (lowire_list*)( scanchain->DATA );
          
            if( ! RCN_GETFLAG( curwire->FLAG, RCN_FLAG_PASS ) )
            {
               
              newtodo = breakloop2(  ptsig,
                                     getlonode( ptsig,
                                                ( scantodo->ptnode->INDEX ==
                                                              curwire->NODE1 ) ?
                                                                curwire->NODE2 :
                                                                curwire->NODE1
                                             ),
                                      curwire,
                                      newtodo,
                                     &cleanwire
                                   );
            }
          }
        }
        
        while( headtodo )
        {
          curtodo = headtodo->next;
          freechain( headtodo->head_wire );
          mbkfree( headtodo );
          headtodo = curtodo;
        }

        headtodo = newtodo;
      }
    }
  }

  for( scanchain = cleanwire ; scanchain ; scanchain = scanchain->NEXT )
  {
    curwire = (lowire_list*)( scanchain->DATA );
    dellowire( ptsig, curwire->NODE1, curwire->NODE2 );    
  }

  freechain ( cleanwire );

  return(1);
}

int reduce_rcn( ptsig )
losig_list *ptsig;
{
  lowire_list *scanwire;
  lonode_list *ptnode;
  chain_list  *todo;
  chain_list  *doing;
  chain_list  *scanctc;
  loctc_list  *ptctc;
  float        resi;
  int          node1;
  int          node2;
  lowire_list *wire1;
  lowire_list *wire2;
  int          another;

  if( !gettabnode( ptsig->PRCN ) )
  {
    fflush( stdout );
    fprintf( stderr, "*** Reduce RCN error\n" );
    fprintf( stderr, "Node table not built.\n" );
    EXIT(1);
  }

  do
  {
    another = 0;

    todo = NULL;
    ptsig->PRCN->CAPA = 0.0;
  
    for( scanwire = ptsig->PRCN->PWIRE ; scanwire ; scanwire = scanwire->NEXT )
    {
      if( RCN_GETFLAG( scanwire->FLAG, RCN_FLAG_DEL ) )
        continue;
  
      ptsig->PRCN->CAPA = ptsig->PRCN->CAPA + scanwire->CAPA;
      scanwire->CAPA = 0.0;
      ptnode = getlonode( ptsig, scanwire->NODE1 );
      if(   RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_TWO )   &&
          ! RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON ) &&
          ! RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_PASS )
        )
      {
        todo = addchain( todo, ptnode );
        RCN_SETFLAG( ptnode->FLAG, RCN_FLAG_PASS );
      }
      ptnode = getlonode( ptsig, scanwire->NODE2 );
      if(   RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_TWO )   &&
          ! RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON ) &&
          ! RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_PASS )
        )
      {
        todo = addchain( todo, ptnode );
        RCN_SETFLAG( ptnode->FLAG, RCN_FLAG_PASS );
      }
    }
  
    for( scanctc = ptsig->PRCN->PCTC ; scanctc ; scanctc = scanctc->NEXT )
    {
      ptctc = (loctc_list*)( scanctc->DATA );
      ptsig->PRCN->CAPA = ptsig->PRCN->CAPA + ptctc->CAPA;
    }
  
    for( doing = todo ; doing ; doing = doing->NEXT )
    {
      ptnode = (lonode_list*)( doing->DATA );
      if( ptnode->CTCLIST )
        continue;
 
      another = 1; 
      wire1 = (lowire_list*)( ptnode->WIRELIST->DATA );
      wire2 = (lowire_list*)( ptnode->WIRELIST->NEXT->DATA );
  
      if( ptnode->WIRELIST->NEXT->NEXT )
      {
        fprintf( stderr, "Anomalie.\n" );
      }
  
      resi = wire1->RESI + wire2->RESI ;
      node1 = ( wire1->NODE1 == ptnode->INDEX ) ? wire1->NODE2 : wire1->NODE1 ;
      node2 = ( wire2->NODE1 == ptnode->INDEX ) ? wire2->NODE2 : wire2->NODE1 ;
     
      setdellowire( ptsig, wire1 ); 
      setdellowire( ptsig, wire2 ); 
      wire1 = NULL;
      wire2 = NULL;
      
      scanwire = getlowire( ptsig, node1, node2 );
      if( scanwire )
        addpararesiwire( scanwire, resi );
      else
        addlowire( ptsig,
                   RCN_WIRE_UNKNOW,
                   0,
                   resi,
                   0.0,
                   0l,
                   0l,
                   0l,
                   0l,
                   node1,
                   node2
                 );
    }
    freechain( todo );
  
    mergedellowire( ptsig );
  }
  while( another );

  return(1);
}

void connexe_rec( losig_list *ptsig,
                  lowire_list *from,
                  lonode_list *ptnode,
                  long index
                )
{
  ptype_list  *ptl;
  chain_list  *scanwire;
  lonode_list *next;

  /* Le noeud par lequel on arrive est deja numerote */

  /* Propagation sur les branches droites */
  while( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_TWO ) )
  {
    if( (lowire_list*)(ptnode->WIRELIST->DATA) == from )
      from = (lowire_list*)(ptnode->WIRELIST->NEXT->DATA);
    else
      from = (lowire_list*)(ptnode->WIRELIST->DATA);
    
    if( from->NODE1 == ptnode->INDEX )
      ptnode = getlonode( ptsig, from->NODE2 );
    else
      ptnode = getlonode( ptsig, from->NODE1 );

    ptl = getptype( ptnode->USER, CONNEXE );
    if( (long)(ptl->DATA) != 0 )
    {
      if( (long)(ptl->DATA) != index )
      {
        fflush( stdout );
        fprintf( stderr, "RCN ERROR in connexe()\n" );
        fprintf( stderr, "index differs (1).\n" );
        exit( 1 );
      }
      else
        return;
    }
    else
      ptl->DATA = (void*)(index);
  }

  /* On arrive soit sur un noeud terminal, soit sur un embranchement */
  for( scanwire = ptnode->WIRELIST ; scanwire ; scanwire = scanwire->NEXT )
  {
    from = ( lowire_list* )( scanwire->DATA );
    if( from->NODE1 == ptnode->INDEX )
      next = getlonode( ptsig, from->NODE2 );
    else
      next = getlonode( ptsig, from->NODE1 );

    ptl = getptype( next->USER, CONNEXE );
    if( (long)(ptl->DATA) == 0 )
    {
      ptl->DATA = (void*)(index);
      connexe_rec( ptsig, from, next, index ); 
    }
    else
    {
      if( (long)(ptl->DATA) != index )
      {
        fflush( stdout );
        fprintf( stderr, "RCN ERROR in connexe()\n" );
        fprintf( stderr, "index differs (2).\n" );
        exit( 1 );
      }
    }
  }

  return;
}

int connexe( losig_list *ptsig )
{
  lowire_list     *scanlowire;
  ptype_list      *ptl;
  lonode_list     *ptnode;
  long             index;

  if( !gettabnode( ptsig->PRCN ) )
  {
    fflush( stdout );
    fprintf( stderr, "*** Reduce RCN error\n" );
    fprintf( stderr, "Node table not built.\n" );
    EXIT(1);
  }

  for( scanlowire = ptsig->PRCN->PWIRE ;
       scanlowire ;
       scanlowire = scanlowire->NEXT
     )
  {
    ptnode = getlonode( ptsig, scanlowire->NODE1 );
    ptl = getptype( ptnode->USER, CONNEXE );
    if( !ptl )
      ptnode->USER = addptype( ptnode->USER, CONNEXE, (void*)0 );
    else
      ptl->DATA = (void*)0;

    ptnode = getlonode( ptsig, scanlowire->NODE2 );
    ptl = getptype( ptnode->USER, CONNEXE );
    if( !ptl )
      ptnode->USER = addptype( ptnode->USER, CONNEXE, (void*)0 );
    else
      ptl->DATA = (void*)0;
  }

  index = 0;

  for( scanlowire = ptsig->PRCN->PWIRE ;
       scanlowire ;
       scanlowire = scanlowire->NEXT
     )
  {
    ptnode = getlonode( ptsig, scanlowire->NODE1 );
    ptl = getptype( ptnode->USER, CONNEXE );
    
    if( (long)(ptl->DATA) != 0 )
      continue;
 
    index++;

    ptl->DATA = (void*)index;
    connexe_rec( ptsig, scanlowire, ptnode, index );

    ptnode = getlonode( ptsig, scanlowire->NODE2 );
    ptl = getptype( ptnode->USER, CONNEXE );
    ptl->DATA = (void*)index;
    connexe_rec( ptsig, scanlowire, ptnode, index );

  }

  return( index );
}

int cleanrcnet( losig_list *ptsig )
{
  lowire_list   *scanwire;
  lowire_list   *ptwire;
  lowire_list   *parawire;
  float          capa;
  lonode_list   *ptnode;
  lonode_list   *fixnode;
  chain_list    *scanchain;
  chain_list    *next;
  loctc_list    *ptctc;
  loctc_list    *paractc;
  int            node;
  chain_list    *prev;
  
  if( !ptsig->PRCN )
    return(0);

  if( !gettabnode( ptsig->PRCN ) )
  {
    fflush( stdout );
    fprintf( stderr, "*** Reduce RCN error\n" );
    fprintf( stderr, "Node table not built.\n" );
    EXIT(1);
  }

  for( scanwire = ptsig->PRCN->PWIRE ; scanwire ; scanwire = scanwire->NEXT )
  {
    if( RCN_GETFLAG( scanwire->FLAG, RCN_FLAG_DEL ) )
      continue;

    if( scanwire->RESI == 0.0 )
    {
      if( scanwire->CAPA > 0.0 )
      {
        capa = 0.0;

        /* Calcul de la capacité totale de chaque côtés de la résistance */

        ptnode = getlonode( ptsig, scanwire->NODE1 );
        for( scanchain = ptnode->WIRELIST ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
        {
          ptwire = (lowire_list*)scanchain->DATA;
          if( ptwire != scanwire )
            capa = capa + ptwire->CAPA;
        }
        
        ptnode = getlonode( ptsig, scanwire->NODE2 );
        for( scanchain = ptnode->WIRELIST ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
        {
          ptwire = (lowire_list*)scanchain->DATA;
          if( ptwire != scanwire )
            capa = capa + ptwire->CAPA;
        }

        /* On transfert la capacité de la résistance proportionellement à la
         * capacité des éléments voisins de cette résistance */
        ptnode = getlonode( ptsig, scanwire->NODE1 );
        for( scanchain = ptnode->WIRELIST ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
        {
          ptwire = (lowire_list*)scanchain->DATA;
          if( ptwire != scanwire )
            ptwire->CAPA = ptwire->CAPA*(1+scanwire->CAPA/capa);
        }
        
        ptnode = getlonode( ptsig, scanwire->NODE2 );
        for( scanchain = ptnode->WIRELIST ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
        {
          ptwire = (lowire_list*)scanchain->DATA;
          if( ptwire != scanwire )
            ptwire->CAPA = ptwire->CAPA*(1+scanwire->CAPA/capa);
        }

        scanwire->CAPA = 0.0;
      }

      /* On choisi le neoud qui regroupera les 2 autres arbitrairement, sauf
       * si on a un connecteur sur un des 2 côtés.
       * On ne prend pas en compte le cas où on a une résistance en
       * parallèle sur scanwire */

      ptnode  = getlonode( ptsig, scanwire->NODE2 );
      fixnode = getlonode( ptsig, scanwire->NODE1 );

      if(  RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON )  && 
          !RCN_GETFLAG( fixnode->FLAG, RCN_FLAG_LOCON )     )
      {
        lonode_list *tmp;
        
        tmp     = ptnode ;
        ptnode  = fixnode;
        fixnode = tmp;
      }
      else
      if( RCN_GETFLAG( ptnode->FLAG, RCN_FLAG_LOCON )  && 
          RCN_GETFLAG( fixnode->FLAG, RCN_FLAG_LOCON )     )
      {
        chain_list *sc2;
        ptype_list *ptlfix;
        ptype_list *ptlpt;
        locon_list *ptlocon;
        num_list   *scannode;

        ptlfix = getptype( fixnode->USER, RCN_LOCON );
        ptlpt  = getptype( ptnode->USER, RCN_LOCON );

        /* Le noeud ptnode qui va disparaitre est un connecteur : on 
         * le transfert sur fixnode */

        for( scanchain = (chain_list*)(ptlpt->DATA) ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
        {
          ptlocon = (locon_list*)(scanchain->DATA);
          for( scannode = ptlocon->PNODE ;
               scannode ;
               scannode = scannode->NEXT
              )
          {
            if( scannode->DATA == ptnode->INDEX )
              scannode->DATA = fixnode->INDEX;
          }
          for( sc2 =(chain_list*)(ptlfix->DATA); sc2 ; sc2 = sc2->NEXT )
          {
            if( sc2->DATA == ptlocon )
              break;
          }
          if( !sc2 )
            ptlfix->DATA = (void*)addchain( (chain_list*)ptlfix->DATA,
                                            (void*)ptlocon
                                          );
        }

      
      }
         
      prev = NULL; 
      for( scanchain = ptnode->WIRELIST ;
           scanchain ;
           scanchain = next
          )
      {
        next = scanchain->NEXT;
        ptwire = (lowire_list*) scanchain->DATA;
        if( ptwire == scanwire )
        {
          prev = scanchain;
          continue;
        }

        if( ptwire->NODE1 == ptnode->INDEX )
          node = ptwire->NODE2;
        else
          node = ptwire->NODE1;
      
        parawire = getlowire( ptsig, fixnode->INDEX, node );
        if( parawire )
        {
          addpararesiwire( parawire, ptwire->RESI );
          addcapawire( parawire, ptwire->CAPA );
       
          setdellowire( ptsig, ptwire );
        }
        else
        {
          if( ptwire->NODE1 == ptnode->INDEX )
            ptwire->NODE1 = fixnode->INDEX ;
          else
            ptwire->NODE2 = fixnode->INDEX ;

          fixnode->WIRELIST = addchain( fixnode->WIRELIST, ptwire );
          switch( RCN_GETFLAG( fixnode->FLAG,
                               RCN_FLAG_ONE|RCN_FLAG_TWO|RCN_FLAG_CROSS
                             )
                )
          {
          case RCN_FLAG_ONE:
            RCN_CLEARFLAG( fixnode->FLAG, RCN_FLAG_ONE );
            RCN_SETFLAG( fixnode->FLAG, RCN_FLAG_TWO );
            break;
          case RCN_FLAG_TWO:
            RCN_CLEARFLAG( fixnode->FLAG, RCN_FLAG_TWO );
            RCN_SETFLAG( fixnode->FLAG, RCN_FLAG_CROSS );
            break;
          case RCN_FLAG_CROSS:
            break;
          default:
            fprintf( stderr,"*** RCN internal error in cleanrcnet [1].\n");
            EXIT(1);
          }

          switch( RCN_GETFLAG( ptnode->FLAG,
                               RCN_FLAG_ONE|RCN_FLAG_TWO|RCN_FLAG_CROSS
                             )
                )
          {
          case RCN_FLAG_ONE:
            RCN_CLEARFLAG( ptnode->FLAG, RCN_FLAG_ONE );
            break;
          case RCN_FLAG_TWO:
            RCN_CLEARFLAG( ptnode->FLAG, RCN_FLAG_TWO );
            RCN_SETFLAG( ptnode->FLAG, RCN_FLAG_ONE );
            break;
          case RCN_FLAG_CROSS:
            if( ptnode->WIRELIST->NEXT->NEXT->NEXT == NULL )
            {
              RCN_CLEARFLAG( ptnode->FLAG, RCN_FLAG_CROSS );
              RCN_SETFLAG( ptnode->FLAG, RCN_FLAG_TWO );
            }
            break;
          default:
            fprintf( stderr,"*** RCN internal error in cleanrcnet [1].\n");
            EXIT(1);
          }

          if( !prev )
            ptnode->WIRELIST = next;
          else
            prev->NEXT = next;
          scanchain->NEXT=NULL;
          freechain( scanchain );

        }
      }

      /* On supprime éventuellement la CTC entre les deux extrémités de
       * la résistance qu'on est en train de supprimer */
      ptctc = getloctc( ptsig, fixnode->INDEX, ptsig, ptnode->INDEX );
      if( ptctc )
        delloctc( ptsig, fixnode->INDEX, ptsig, ptnode->INDEX );
     
      for( scanchain = ptnode->CTCLIST ;
           scanchain ;
           scanchain = next
         )
      {
        next = scanchain->NEXT;
        ptctc = (loctc_list*)scanchain->DATA;
        if( ptctc->SIG1 == ptsig && ptctc->NODE1 == ptnode->INDEX )
        {
          paractc = getloctc( ptsig, fixnode->INDEX,
                              ptctc->SIG2, ptctc->NODE2
                            );
          if( paractc )
          {
            paractc->CAPA = paractc->CAPA + ptctc->CAPA;
            delloctc( ptctc->SIG1, ptctc->NODE1, ptctc->SIG2, ptctc->NODE2 );
          }
          else
          {
            ptctc->NODE1 = fixnode->INDEX;
            fixnode->CTCLIST = addchain( fixnode->CTCLIST, ptctc );
          }
        }
        else
        {
          paractc = getloctc( ptsig, fixnode->INDEX,
                              ptctc->SIG1, ptctc->NODE1
                            );
          if( paractc )
          {
            paractc->CAPA = paractc->CAPA + ptctc->CAPA;
            delloctc( ptctc->SIG1, ptctc->NODE1, ptctc->SIG2, ptctc->NODE2 );
          }
          else
          {
            ptctc->NODE2 = fixnode->INDEX;
            fixnode->CTCLIST = addchain( fixnode->CTCLIST, ptctc );
          }
        }
      }
      if( ptnode->CTCLIST )
      {
        freechain(ptnode->CTCLIST);
        ptnode->CTCLIST = NULL;
      }

      setdellowire( ptsig, scanwire );
      RCN_CLEARFLAG( ptnode->FLAG, RCN_FLAG_TRUE );
    }

  }
  mergedellowire( ptsig );

  return(1);
}
