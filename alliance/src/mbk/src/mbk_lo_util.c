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

/* 
 * Purpose : utilities for logical structures
 * Date    : 05/11/92
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 */

#ident "$Id: mbk_lo_util.c,v 1.5 2009/06/14 13:51:52 ludo Exp $"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include "mut.h"
#include "mlo.h"
#include "rcn.h"
#include "mlu.h"
#include "mbk_lo_util.h"
#include "rcn_lo.h"

/* local for flattenlofig */
#define FLATTEN_CTC 19971215
#define FLATTEN_POSTRAIT 19971216

void debugctc ( losig_list*, int );
void debugctc2 ( lofig_list* );

/*******************************************************************************
* function getlofig()                                                          *
* if mode == 'A'  all the figure is needed                                     *
* if mode == 'P'  interface only is needed                                     *
*******************************************************************************/
lofig_list *getlofig(const char *fname, char mode)
{
lofig_list *ptfig;
char *figname = namealloc(fname);
   /* scan figure list */
   for (ptfig = HEAD_LOFIG; ptfig; ptfig = ptfig->NEXT)
      if (ptfig->NAME == figname)
         break;

   if (ptfig == NULL) { /* figure doesn't exist */
      ptfig = (lofig_list *)mbkalloc(sizeof(lofig_list));
      ptfig->NAME   = figname;
      ptfig->MODELCHAIN  = NULL;
      ptfig->LOINS  = NULL;
      ptfig->LOTRS  = NULL;
      ptfig->LOCAP  = NULL;
      ptfig->LORES  = NULL;
      ptfig->LOSELF = NULL;
      ptfig->LOCON  = NULL;
      ptfig->LOSIG  = NULL;
      ptfig->BKSIG  = NULL;
      ptfig->USER   = NULL;
      ptfig->NEXT   = HEAD_LOFIG;
      HEAD_LOFIG = ptfig;
      ptfig->MODE = mode;
      loadlofig(ptfig, figname, mode );
      return ptfig;
   }

   return ptfig;
}

/*******************************************************************************
* function givelosig()                                                         *
*******************************************************************************/
losig_list *givelosig(lofig_list *ptfig, long index)
{
losig_list *ptsig = NULL;
ptype_list *pt = NULL;
int low;
int type;
int sigsize;
        sigsize = getsigsize( ptfig );
   low = (index % sigsize);
   type = (index / sigsize);
   for (pt = ptfig->BKSIG; pt; pt = pt->NEXT)
   if (pt->TYPE == type)
      break;
   if (pt != NULL) {
      ptsig = (losig_list *)(pt->DATA) + low;
      if (ptsig->INDEX == index)
         return ptsig;
   }
   return addlosig(ptfig, index, (chain_list *)NULL, 'I' );
}

/*******************************************************************************
* function  flattenlofig()                                                     *
*                                                                              *
* the instance "insname" is suppressed as a hierarchical level                 *
* in figure pointed by "ptfig". Capacitances and netlist                       *
* consistency is preserved.                                                    *
* Modified by Gregoire AVOT for rcn support.                                   *
*******************************************************************************/
void flattenlofig(lofig_list *ptfig, const char *iname, char concat)
{
  struct st_interf
  {
    struct st_interf    *next;
    int                  base;
    losig_list          *sig;
  };
  
  struct st_contact
  {
    struct st_contact   *next;
    locon_list          *con1;  /* on instance */
    int                  node1;
    locon_list          *con2;  /* on lofig of instance */
    int                   node2;
    int                  interf;
  };

  chain_list            *nextscanctc;
  loins_list           *ptins;
  lofig_list           *figins;
  losig_list           *scanlosig;
  int             maxsig;
  locon_list            *ptcon;
  chain_list            *scanchain;
  ptype_list            *ptptype;
  struct st_interf      *interf;
  struct st_interf      *ptinterf;
  struct st_contact     *contact;
  struct st_contact     *ptcontact;
  int                    nbinterf;
  num_list              *sn1;
  num_list              *sn2;
  lowire_list           *scanwire;
  int                    node1;
  struct st_interf      *ctinterf;
  int       *newnode;
  losig_list            *newsig;
  locon_list            *scanlocon;
  int                    maxnode;
  chain_list            *scanctc;
  locon_list            *nextlocon;
  void         *forfree;
  loctc_list            *ptctc;
  loctc_list            *ptctc2;
  chain_list            *prevctc;
  chain_list            *prevctc2;
  chain_list            *scanctc2;
  losig_list            *sigctc2;
  int                    othernode;
  int                    scanctcnode;
  losig_list            *othersig;
  chain_list            *posttreatsig=NULL;
  ptype_list            *ptbksig=NULL;
  int                    maxnode_itrf;
  int                    newnode_itrf;
  chain_list            *scanmodel;
  chain_list            *lofigc;
  chain_list            *chainhtab;
  chain_list            *chainht;
  chain_list            *loconchain;
  chain_list            *chain;
  htitem                *ptitem ;
  long                  nbi;

  int                    verif;

  lotrs_list            *scantrs;
  locap_list            *scancap;
  lores_list            *scanres;
  loself_list           *scanself;
  loins_list            *scanins;

  int                    i;
  char                   all;
  long                   key ;
  ht                     *htab ;
  const char             *insname;

  /* recupere l'index maximum des signaux sur la figure */
  maxsig = -1 ;
  for( ptptype = ptfig->BKSIG ; ptptype ; ptptype = ptptype->NEXT )
  {
    if( ptptype->TYPE > maxsig )
    {
      maxsig = ptptype->TYPE;
      ptbksig = ptptype;
    }
  }
  
  scanlosig = (losig_list*)(ptbksig->DATA);
  verif = getsigsize( ptfig );
  for( i=0 ; i< verif ; i++ )
  {
    if( scanlosig[i].INDEX > maxsig )
      maxsig = scanlosig[i].INDEX;
  }
  
  maxsig++;

 /* si insname == NULL on met a plat au niveau transistor */

  if(iname == NULL)
    all = 'Y' ;
  else
   all = 'N' ;

  /* On verifie que le lofigchain est bien a jour. Tous les signaux d'une
   * lofig ont le ptype dans leur champ USER, meme si il est vide */

  if (!getptype(ptfig->LOSIG->USER, LOFIGCHAIN)) {
#if 0
    fflush( stdout );
    fprintf( stderr, "*** mbk warning ***\n" );
    fprintf( stderr, "lofigchain is missing on lofig %s.\n", ptfig->NAME );
#endif

    lofigchain( ptfig );
  }
  
while(ptfig->LOINS != NULL)
{
  if(all == 'Y')
   {
    insname = ptfig->LOINS->INSNAME ;
   }
  else
     insname = iname;

  ptins  = getloins( ptfig, insname );
  figins = rduplofig( getlofig( ptins->FIGNAME, 'A' ) );

  lofigchain( figins );

  for( ptcon = ptins->LOCON ; ptcon ; ptcon = ptcon->NEXT )
    if( ptcon->SIG->PRCN && gettabnode( ptcon->SIG->PRCN ) )
      freetable( ptcon->SIG );
 
  /* Si un seul signal a un RC, on en cree un sur tous les autres */

  for( ptcon = ptins->LOCON ; ptcon ; ptcon = ptcon->NEXT )
    if( ptcon->SIG->PRCN )
      break;

  if( !ptcon )
    for( scanlosig = figins->LOSIG ; scanlosig ; scanlosig = scanlosig->NEXT )
      if( scanlosig->PRCN )
        break;

  if( scanlosig || ptcon )
  {
    for( ptcon = ptins->LOCON ; ptcon ; ptcon = ptcon->NEXT )
    {
      if( ! ptcon->SIG->PRCN )
      {
        addlorcnet( ptcon->SIG );
      }

      if( gettabnode( ptcon->SIG->PRCN ) )
        freetable( ptcon->SIG );
    }

    for( scanlosig = figins->LOSIG ; scanlosig ; scanlosig = scanlosig->NEXT )
    {
      if( ! scanlosig->PRCN )
        addlorcnet( scanlosig );

      if( gettabnode( scanlosig->PRCN ) )
        freetable( scanlosig );
    }
  }
  
  /* Au boulot */  
  for( scanlosig = figins->LOSIG ; scanlosig ; scanlosig = scanlosig->NEXT )
  {
    if( scanlosig->TYPE == INTERNAL )
    {
      newsig = addlosig( ptfig, maxsig, NULL, INTERNAL );
      maxsig++;   

      newsig->USER = addptype( newsig->USER, LOFIGCHAIN, NULL );

      /* concatene les nom si necessaire */
      if( concat != NO )
        for( scanchain = scanlosig->NAMECHAIN ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
          scanchain->DATA = concatname( insname, (char*)(scanchain->DATA) );
      newsig->NAMECHAIN = scanlosig->NAMECHAIN;
      scanlosig->NAMECHAIN = NULL;
      
      ptptype       = getptype( newsig->USER, LOFIGCHAIN );
      for( scanchain = getptype( scanlosig->USER, LOFIGCHAIN )->DATA ;
           scanchain ;
           scanchain = scanchain->NEXT )
      {
        ptcon         = (locon_list*)(scanchain->DATA);
        ptcon->SIG    = newsig;
        ptptype->DATA = (void*)addchain( (chain_list*)ptptype->DATA, ptcon );
      }

      if( scanlosig->PRCN )
      {
        duplorcnet( newsig, scanlosig );

        /* Duplication des CTC */

        for( scanctc = scanlosig->PRCN->PCTC ;
             scanctc ;
             scanctc = scanctc->NEXT )
        {
          ptctc = (loctc_list*)scanctc->DATA;
          if( !ptctc )
          {
            fflush( stdout );
            fprintf( stderr, "*** mbk error ***\n" );
            fprintf( stderr,
                     "Null CTC on signal %ld (1) in instance %s.\n",
                     scanlosig->INDEX,
                     insname
                   );
          }

          if( ptctc->SIG1 == scanlosig )
          {
            newsig->PRCN->PCTC = addchain( newsig->PRCN->PCTC, ptctc );
            ptctc->SIG1 = newsig;
          }

          if( ptctc->SIG2 == scanlosig )
          {
            newsig->PRCN->PCTC = addchain( newsig->PRCN->PCTC, ptctc );
            ptctc->SIG2 = newsig;
          }
        }
      }
    }
    else /* scanlosig->TYPE == EXTERNAL */ 
    {
      /* cree la liste interf */
      interf = (struct st_interf*)mbkalloc( sizeof( struct st_interf ) );
      interf->base = 0;
      interf->next = NULL;
      interf->sig  = scanlosig;
      
      loconchain = NULL ;

      for( scanchain = 
                  (chain_list*)(getptype( scanlosig->USER, LOFIGCHAIN )->DATA) ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {
        ptcon = (locon_list*)(scanchain->DATA);
        if( ptcon->ROOT == figins )
        {
          for( scanlocon = ptins->LOCON ;
               scanlocon ;
               scanlocon = scanlocon->NEXT
             )
          {
            if( scanlocon->NAME == ptcon->NAME )
              break;
          }
          
          if( ! scanlocon )
          {
            fflush( stdout );
            fprintf( stderr, "*** mbk error ***\n" );
            fprintf( stderr,
                     "flattenlofig : Connector %s exist only in instance %s.\n",
                     ptcon->NAME,
                     insname
                   );
            EXIT(1);
          }

          loconchain = addchain(loconchain,scanlocon) ;

          ptinterf = (struct st_interf*)
                          mbkalloc( sizeof( struct st_interf ) );
          ptinterf->base = 0;
          ptinterf->next = interf;
          ptinterf->sig  = scanlocon->SIG;
          interf         = ptinterf;

          if( ! scanlocon->PNODE && ptcon->PNODE )
          {
            for( maxnode_itrf = 0, sn1 = ptcon->PNODE ; sn1 ; sn1 = sn1->NEXT, maxnode_itrf++ );
            
            newnode_itrf = addlonode( scanlocon->SIG, NULL );

            for( i = 1 ; i <= maxnode_itrf ; i++ )
              setloconnode( scanlocon, newnode_itrf );
          }
          
          if( scanlocon->PNODE && ! ptcon->PNODE )
          {
            for( maxnode_itrf = 0, sn1 = scanlocon->PNODE ; sn1 ; sn1 = sn1->NEXT, maxnode_itrf++ );
            
            newnode_itrf = addlonode( ptcon->SIG, NULL );

            for( i = 1 ; i <= maxnode_itrf ; i++ )
              setloconnode( ptcon, newnode_itrf );
          }

        }
      }
      
      for( ptinterf = interf ; ptinterf ; ptinterf = ptinterf->next )
         if(ptinterf->sig != scanlosig)
          break ;

      if(ptinterf)
       {
        newsig = ptinterf->sig ;
        ptptype = getptype(newsig->USER,LOFIGCHAIN) ;
        for(chain = loconchain ; chain != NULL ; chain = chain->NEXT)
         {
          scanchain = (chain_list*)ptptype->DATA ;
          while( scanchain )
           {
            ptcon = (locon_list*)(scanchain->DATA);
  
            if((locon_list *)chain->DATA == ptcon)
             {
              if(scanchain == (chain_list*)ptptype->DATA)
               {
                ptptype->DATA = delchain((chain_list*)ptptype->DATA,
                                         scanchain) ;
                scanchain =  ptptype->DATA ;
               }
              else
               {
                lofigc->NEXT = delchain(lofigc->NEXT,
                                         scanchain) ;
                scanchain = lofigc->NEXT ;
               }
              break ;
             }
            else
             {
              lofigc = scanchain ;
              scanchain = scanchain->NEXT ;
             }
           }
         }
       }
      else
       {
        newsig = addlosig( ptfig, maxsig, NULL, INTERNAL );
        maxsig++; 
        newsig->USER = addptype( newsig->USER, LOFIGCHAIN, NULL );
        ptptype = newsig->USER ;
       }

      freechain(loconchain) ;

      if(getptype(newsig->USER,FLATTEN_POSTRAIT) == NULL)
       {
        posttreatsig = addchain( posttreatsig, newsig );
        newsig->USER = addptype(newsig->USER,FLATTEN_POSTRAIT,NULL) ;
       }

      /* cree la liste contact */
      contact  = NULL;
      if( newsig->PRCN )
        nbinterf = newsig->PRCN->NBNODE ;
      else
        nbinterf = 1;
      
      for( scanchain = 
                  (chain_list*)(getptype( scanlosig->USER, LOFIGCHAIN )->DATA) ;
           scanchain ;
           scanchain = scanchain->NEXT )
      {
        ptcon = (locon_list*)(scanchain->DATA);
        if( ptcon->ROOT == figins )
        {
          for( scanlocon = ptins->LOCON ; 
               scanlocon ;
               scanlocon = scanlocon->NEXT
             )
          {
            if( scanlocon->NAME == ptcon->NAME )
              break;
          }

          if( !scanlocon )
          {
            fflush( stdout );
            fprintf( stderr, "*** mbk error ***\n" );
            fprintf( stderr,
                     "flattenlofig : Connector %s exist only in instance %s.\n",
                     ptcon->NAME,
                     ptfig->NAME
                   );
            EXIT(1);
          }

          if( scanlocon->PNODE || ptcon->PNODE )
          {
            for( sn1 = scanlocon->PNODE, sn2 = ptcon->PNODE ;
                 sn1 && sn2 ;
                 sn1 = sn1->NEXT, sn2 = sn2->NEXT 
               )
            {
              ptcontact =
                    (struct st_contact*)mbkalloc( sizeof( struct st_contact ) );
            
              ptcontact->next   = contact;
              ptcontact->con1   = scanlocon;
              ptcontact->con2   = ptcon;
              ptcontact->node1  = sn1->DATA;
              ptcontact->node2  = sn2->DATA;
              if(scanlocon->SIG == newsig)
               ptcontact->interf = sn1->DATA ;
              else
               {
                ptcontact->interf = nbinterf++;
               }

              contact = ptcontact;
            }

            if( sn1 || sn2 )
            {
              fflush( stdout );
              fprintf( stderr, "*** mbk error ***\n" );
              fprintf( stderr,
                       "flattenlofig : Connector %s in instance %s : Number of physical node differ.\n",
                       ptcon->NAME,
                       figins->NAME
                     );
              EXIT(1);
            }
          }
          else
          {
            ptcontact =
                    (struct st_contact*)mbkalloc( sizeof( struct st_contact ) );
            
            ptcontact->next   = contact;
            ptcontact->con1   = scanlocon;
            ptcontact->con2   = ptcon;
            ptcontact->node1  = 0;
            ptcontact->node2  = 0;
            ptcontact->interf = 0;

            contact = ptcontact;
          }          
        }
      }

      /* Met a jour le champs base des structures inter */      
      if( interf->sig->PRCN )
      {
        if(!newsig->PRCN)
     addlorcnet( newsig );

        maxnode = nbinterf ;
        
        for( ptinterf = interf ; ptinterf ; ptinterf = ptinterf->next )
        {
          if(ptinterf->sig != newsig)
           {
            ptinterf->base = maxnode - 1 ;
            maxnode        = maxnode + ptinterf->sig->PRCN->NBNODE - 1 ;

       newsig->PRCN->CAPA += ptinterf->sig->PRCN->CAPA;
           }
          else
            ptinterf->base = 0 ;
        }
      }

      /* calcul le type de newsig */

      for( ptinterf = interf ; ptinterf ; ptinterf = ptinterf->next )
        if( ptinterf->sig != scanlosig && ptinterf->sig->TYPE == EXTERNAL )
          break;
      
      if( ptinterf )
        newsig->TYPE = EXTERNAL;
      else
        newsig->TYPE = INTERNAL;

      /* met a jour le nom de newsig */
      for( ptinterf = interf ; ptinterf ; ptinterf = ptinterf->next )
      {
        if(ptinterf->sig == newsig)
          continue ;
        if( concat != NO )
        {
          if( ptinterf->sig == scanlosig )
            for( scanchain = ptinterf->sig->NAMECHAIN ;
                 scanchain ;
                 scanchain = scanchain->NEXT
               )
              scanchain->DATA = concatname( insname, (char*)scanchain->DATA );
        }
        newsig->NAMECHAIN = append( ptinterf->sig->NAMECHAIN, newsig->NAMECHAIN );
        ptinterf->sig->NAMECHAIN = NULL;
      }

      /* Pour chaque signal S de interf */
      for( ptinterf = interf ; ptinterf ; ptinterf = ptinterf->next )
      {
        if(ptinterf->sig == newsig)
          continue ;
        /* Pour chaque connecteur C de S */
        for( scanchain = 
               (chain_list*)(getptype( ptinterf->sig->USER, LOFIGCHAIN )->DATA);
             scanchain ;
             scanchain = scanchain->NEXT )
        {
          ptcon = (locon_list*)(scanchain->DATA);
          
          for( ptcontact = contact ; ptcontact ; ptcontact = ptcontact->next )
            if( ptcon == ptcontact->con1 || ptcon == ptcontact->con2 )
              break;

          /* Si C n'est pas sur l'interface */
          if( !ptcontact )
          {
            ptptype->DATA = (void*)addchain( (chain_list*)ptptype->DATA,ptcon);

            for( sn1 = ptcon->PNODE ; sn1 ; sn1 = sn1->NEXT )
            {
              for( ptcontact = contact ;
                   ptcontact ;
                   ptcontact = ptcontact->next
                 )
              {
                if( ptcontact->node1     == sn1->DATA  &&
                    ptcontact->con1->SIG == ptcon->SIG
                  )
                  break;
                if( ptcontact->node2     == sn1->DATA  &&
                    ptcontact->con2->SIG == ptcon->SIG
                  )
                  break;
              }

              if( ptcontact )
                /* cas particulier : si un noeud appartient aussi a un
                 * connecteur de l'interface */
                sn1->DATA = ptcontact->interf;
              else
                sn1->DATA = sn1->DATA + ptinterf->base ;
            }

            ptcon->SIG = newsig;
            for(sn1 = ptcon->PNODE ; sn1 ; sn1 = sn1->NEXT)
              if(ptcon->SIG->PRCN->NBNODE <= sn1->DATA)  
                ptcon->SIG->PRCN->NBNODE = sn1->DATA + 1 ;
          }
        }

        if( ptinterf->sig->PRCN )
        {
          /* Pour chaque noeud de S, on calcule son nouvel index dans
           * un tableau newnode */
          newnode = mbkalloc( sizeof(int) * ( ptinterf->sig->PRCN->NBNODE + 1 ));
          bzero( newnode, sizeof(int) * ( ptinterf->sig->PRCN->NBNODE + 1 ) ); 

          for( ptcontact = contact ; ptcontact ; ptcontact = ptcontact->next )
          {
            if(ptinterf->sig == ptcontact->con1->SIG)
             {
              newnode[ptcontact->node1] = ptcontact->interf;
             }
            else if(ptinterf->sig == ptcontact->con2->SIG)
             {
              newnode[ptcontact->node2] = ptcontact->interf;
             }
          }

          for( node1 = 1 ; node1 <= ptinterf->sig->PRCN->NBNODE ; node1++ )
            if( newnode[node1] == 0 )
              newnode[node1] = node1 + ptinterf->base;
           
          /* Pour chaque wire W de S */
          for( scanwire = ptinterf->sig->PRCN->PWIRE ;
               scanwire ;
               scanwire = scanwire->NEXT
             )
          {
            addlowire( newsig,
                       scanwire->LAYER,
                       scanwire->FLAG,
                       scanwire->RESI,
                       scanwire->CAPA,
                       scanwire->X,
                       scanwire->Y,
                       scanwire->DX,
                       scanwire->DY,
                       newnode[ scanwire->NODE1 ],
                       newnode[ scanwire->NODE2 ]
                     );
          }

          /* Pour chaque CTC de S */
          for( scanctc = ptinterf->sig->PRCN->PCTC ;
               scanctc ;
               scanctc = scanctc->NEXT
             )
          {
           
            ptctc = (loctc_list*)scanctc->DATA;

            if( ptctc->SIG1 == ptinterf->sig )
            {
              ptctc->SIG1  = newsig;
              if( ptctc->NODE1 )
                ptctc->NODE1 = newnode[ ptctc->NODE1 ];
            }
            else
            {
              for( ctinterf = interf ; ctinterf ; ctinterf = ctinterf->next )
              {
                if( ptctc->SIG1 == ctinterf->sig )
                {
                  ptctc->SIG1 = newsig;
               
                  if( ptctc->NODE1 )
                  {
                    for( ptcontact = contact ;
                         ptcontact ;
                         ptcontact = ptcontact->next
                       )
                    {
                      if( ( ctinterf->sig   == ptcontact->con1->SIG &&
                            ptctc->NODE1    == ptcontact->node1          ) ||
                          ( ctinterf->sig   == ptcontact->con2->SIG &&
                            ptctc->NODE1    == ptcontact->node2          )    )
                      {
                        ptctc->NODE1 = ptcontact->interf;
                        break;
                      }
                    }
                    if( !ptcontact )
                      ptctc->NODE1 = ptctc->NODE1 + ctinterf->base;
                    break;
                  }
                }
              }
            }

            if( ptctc->SIG2 == ptinterf->sig )
            {
              ptctc->SIG2  = newsig;
              if( ptctc->NODE2 )
                ptctc->NODE2 = newnode[ ptctc->NODE2 ];
            }
            else
            {
              for( ctinterf = interf ; ctinterf ; ctinterf = ctinterf->next )
              {
                if( ptctc->SIG2 == ctinterf->sig )
                {
                  ptctc->SIG2 = newsig;
                
                  if( ptctc->NODE2 )
                  {
                    for( ptcontact = contact ;
                         ptcontact ;
                         ptcontact = ptcontact->next
                       )
                    {
                      if( ( ctinterf->sig   == ptcontact->con1->SIG &&
                            ptctc->NODE2    == ptcontact->node1          ) ||
                          ( ctinterf->sig   == ptcontact->con2->SIG &&
                            ptctc->NODE2    == ptcontact->node2          )    )
                      {
                        ptctc->NODE2 = ptcontact->interf;
                        break;
                      }
                    }
                    if( !ptcontact )
                      ptctc->NODE2 = ptctc->NODE2 + ctinterf->base;
                    break;
                  }
                }
              }
            }

            if(!ptctc)
            {
              fflush( stdout );
              fprintf( stderr, "*** mbk error ***\n" );
              fprintf( stderr,
                       "Null CTC on signal %ld (2) in instance %s.\n",
                       scanlosig->INDEX,
                       insname
                     );
            }
            newsig->PRCN->PCTC = addchain( newsig->PRCN->PCTC, ptctc );
       
          }

          mbkfree( newnode );

        }
      }
      
      for( ptinterf = interf ; ptinterf ; ptinterf = ptinterf->next )
      {
        if(( ptinterf->sig != scanlosig ) && ( ptinterf->sig != newsig ))
        {
          ptptype = getptype( ptinterf->sig->USER, LOFIGCHAIN );
          freechain( (chain_list*)ptptype->DATA );
          ptptype->DATA = NULL;
          if( ptinterf->sig->PRCN && ptinterf->sig->PRCN->PCTC )
          {
            freechain( ptinterf->sig->PRCN->PCTC );
            ptinterf->sig->PRCN->PCTC = NULL;
          }
          dellosig( ptfig, ptinterf->sig->INDEX );
        }
      }

      forfree = NULL;
      for( ptcontact = contact ; ptcontact ; ptcontact=ptcontact->next )
      {
        if( forfree )
     mbkfree( forfree );
   forfree = ptcontact;
      }
      if( forfree )
        mbkfree( forfree );

      forfree = NULL;
      for( ptinterf = interf ; ptinterf ; ptinterf=ptinterf->next )
      {
        if( forfree )
     mbkfree( forfree );
   forfree = ptinterf;
      }
      if( forfree )
        mbkfree( forfree );
    }
    /* vire scanlosig */
    ptptype = getptype( scanlosig->USER, LOFIGCHAIN );
    freechain( (chain_list*)ptptype->DATA );
    ptptype->DATA = NULL;
    
    if( scanlosig->PRCN && scanlosig->PRCN->PCTC )
    {
      freechain( scanlosig->PRCN->PCTC );
      scanlosig->PRCN->PCTC = NULL;
    }

    dellosig( figins, scanlosig->INDEX );


  }

  delloins( ptfig, ptins->INSNAME );

  if( concat != NO )
  {
    for( scanins = figins->LOINS ; scanins ; scanins = scanins->NEXT )
    {
      scanins->INSNAME = concatname( insname, scanins->INSNAME );

      for( scanlocon = scanins->LOCON ; scanlocon ; scanlocon = scanlocon->NEXT)
      {
        ptptype = getptype(scanlocon->USER, PNODENAME);
        if( ptptype )
        {
          for( scanchain = (chain_list*)(ptptype->DATA) ;
               scanchain ;
               scanchain = scanchain->NEXT
             )
            scanchain->DATA = concatname( insname, (char*)(scanchain->DATA) );
        }
      }
    }

    for( scantrs = figins->LOTRS ; scantrs ; scantrs = scantrs->NEXT )
    {
      if( scantrs->TRNAME )
        scantrs->TRNAME = concatname( insname, scantrs->TRNAME );
      
      ptptype = getptype( scantrs->DRAIN->USER, PNODENAME);
      if( ptptype )
      {
        for( scanchain = (chain_list*)(ptptype->DATA) ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
          scanchain->DATA = concatname( insname, (char*)(scanchain->DATA) );
      }
      
      ptptype = getptype( scantrs->GRID->USER, PNODENAME);
      if( ptptype )
      {
        for( scanchain = (chain_list*)(ptptype->DATA) ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
          scanchain->DATA = concatname( insname, (char*)(scanchain->DATA) );
      }
      
      ptptype = getptype( scantrs->SOURCE->USER, PNODENAME);
      if( ptptype )
      {
        for( scanchain = (chain_list*)(ptptype->DATA) ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
          scanchain->DATA = concatname( insname, (char*)(scanchain->DATA) );
      }
      
      ptptype = getptype( scantrs->BULK->USER, PNODENAME);
      if( ptptype )
      {
        for( scanchain = (chain_list*)(ptptype->DATA) ;
             scanchain ;
             scanchain = scanchain->NEXT
           )
          scanchain->DATA = concatname( insname, (char*)(scanchain->DATA) );
      }
    }

    for(scancap = figins -> LOCAP ; scancap != NULL ; scancap = scancap -> NEXT)
      {
        if(scancap -> NAME != NULL)
	  {
            scancap -> NAME = concatname(insname,scancap -> NAME) ;
	  }

        ptptype = getptype(scancap -> TCON -> USER,PNODENAME) ;

        if(ptptype)
          {
             for(scanchain = (chain_list *)(ptptype -> DATA) ; scanchain ; scanchain = scanchain->NEXT)
	       {
                 scanchain -> DATA = concatname(insname,(char *)(scanchain -> DATA)) ;
	       }
	  }
      
	ptptype = getptype( scancap -> BCON -> USER,PNODENAME) ;

	if(ptptype)
	  {
	    for(scanchain = (chain_list *)(ptptype -> DATA) ; scanchain ; scanchain = scanchain->NEXT)
	      {
		scanchain -> DATA = concatname(insname,(char *)(scanchain -> DATA)) ;
	      }
	  }
      }

    for(scanres = figins -> LORES ; scanres != NULL ; scanres = scanres -> NEXT)
      {
        if(scanres -> NAME != NULL)
	  {
            scanres -> NAME = concatname(insname,scanres -> NAME) ;
	  }

        ptptype = getptype(scanres -> RCON1 -> USER,PNODENAME) ;

        if(ptptype)
          {
             for(scanchain = (chain_list *)(ptptype -> DATA) ; scanchain ; scanchain = scanchain->NEXT)
	       {
                 scanchain -> DATA = concatname(insname,(char *)(scanchain -> DATA)) ;
	       }
	  }
      
	ptptype = getptype( scanres -> RCON2 -> USER,PNODENAME) ;

	if(ptptype)
	  {
	    for(scanchain = (chain_list *)(ptptype -> DATA) ; scanchain ; scanchain = scanchain->NEXT)
	      {
		scanchain -> DATA = concatname(insname,(char *)(scanchain -> DATA)) ;
	      }
	  }
      }

    for(scanself = figins -> LOSELF ; scanself != NULL ; scanself = scanself -> NEXT)
      {
        if(scanself -> NAME != NULL)
	  {
            scanself -> NAME = concatname(insname,scanself -> NAME) ;
	  }

        ptptype = getptype(scanself -> SCON1 -> USER,PNODENAME) ;

        if(ptptype)
          {
             for(scanchain = (chain_list *)(ptptype -> DATA) ; scanchain ; scanchain = scanchain->NEXT)
	       {
                 scanchain -> DATA = concatname(insname,(char *)(scanchain -> DATA)) ;
	       }
	  }
      
	ptptype = getptype( scanself -> SCON2 -> USER,PNODENAME) ;

	if(ptptype)
	  {
	    for(scanchain = (chain_list *)(ptptype -> DATA) ; scanchain ; scanchain = scanchain->NEXT)
	      {
		scanchain -> DATA = concatname(insname,(char *)(scanchain -> DATA)) ;
	      }
	  }
      }
  }
 
  for( scanins = figins->LOINS ; scanins ; scanins = scanins->NEXT )
  {
    for( scanmodel = ptfig->MODELCHAIN ;
         scanmodel ;
         scanmodel = scanmodel->NEXT )
      if( (char*)scanmodel->DATA == scanins->FIGNAME )
        break;
    if( !scanmodel )
      ptfig->MODELCHAIN = addchain( ptfig->MODELCHAIN, scanins->FIGNAME );
  }

  ptfig->LOINS = (loins_list*)append( (chain_list*)figins->LOINS,
                                      (chain_list*)ptfig->LOINS );
  ptfig->LOTRS = (lotrs_list*)append( (chain_list*)figins->LOTRS,
                                      (chain_list*)ptfig->LOTRS );

  ptfig->LOCAP = (locap_list*)append( (chain_list*)figins->LOCAP,
                                      (chain_list*)ptfig->LOCAP );

  ptfig->LORES = (lores_list*)append( (chain_list*)figins->LORES,
                                      (chain_list*)ptfig->LORES );

  ptfig->LOSELF = (loself_list*)append( (chain_list*)figins->LOSELF,
                                      (chain_list*)ptfig->LOSELF );

  figins->LOINS = NULL;
  figins->LOTRS = NULL;

  figins->LOCAP = NULL;
  figins->LORES = NULL;
  figins->LOSELF = NULL;


  for( scanlocon = figins->LOCON ; scanlocon ; scanlocon = nextlocon )
  {
    nextlocon = scanlocon->NEXT;
    delloconuser( scanlocon );
    mbkfree( (void*)scanlocon );
  }
  figins->LOCON = NULL;

  for( ptptype = figins->BKSIG ; ptptype ; ptptype = ptptype->NEXT )
    mbkfree( (void*)ptptype->DATA );
  freeptype( figins->BKSIG );
  figins->BKSIG = NULL;
  figins->LOSIG = NULL;

  freechain( figins->MODELCHAIN );
  figins->MODELCHAIN = NULL;

  /* Les seules valeurs qui peuvent arriver sont : PTSIGSIZE */

  dellofiguser( figins );

  if( figins->MODELCHAIN     ||
      figins->LOCON          ||
      figins->LOSIG          ||
      figins->BKSIG          ||
      figins->LOINS          ||
      figins->LOTRS          ||
      figins->LOCAP          ||
      figins->LORES          ||
      figins->LOSELF         ||
      figins->USER              )
  {
    fflush( stdout );
    fprintf( stderr,
             "*** mbk warning *** : figure %s not empty (type=%ld)\n",
             figins->NAME,
             figins->USER->TYPE
           );
  }
  mbkfree( (void*)figins );

  if(all == 'N')
   break ;
}

  /* Post traitement sur les CTC dont un des noeuds est a 0 */

  /* Variable de debuggage */
  verif = 0;

  /* Regroupe les CTC en double */
  for( scanchain = posttreatsig ; scanchain ; scanchain = scanchain->NEXT )
  {
    scanlosig = (losig_list*)(scanchain->DATA);
    if(getptype(scanlosig->USER,FLATTEN_POSTRAIT) != NULL)
      scanlosig->USER = delptype(scanlosig->USER,FLATTEN_POSTRAIT) ;
      
    if((scanlosig->INDEX == 0) || ( ! scanlosig->PRCN ))
      continue;

    htab = addht(scanlosig->PRCN->NBNODE + 100) ;
   
    for( scanctc = scanlosig->PRCN->PCTC ; scanctc ; scanctc = scanctc->NEXT )
    {
      ptctc = (loctc_list*)( scanctc->DATA );

      if( ptctc->CAPA >= 0.0 )
      {
        if( ptctc->SIG1 == scanlosig )
        {
          othersig    = ptctc->SIG2  ;
          othernode   = ptctc->NODE2 ;
          scanctcnode = ptctc->NODE1 ;
        }
        else
        {
          othersig    = ptctc->SIG1  ;
          othernode   = ptctc->NODE1 ;
          scanctcnode = ptctc->NODE2 ;
        }
     if( othersig == scanlosig && othernode == scanctcnode )
        {
          ptctc->CAPA = -1.0 ;
          verif = verif + 2 ;
        }
      else
        {
         key = othersig->INDEX << 11 ;
         key = (key | othernode) << 11 ;
         key = (key | scanctcnode) ;

         if((chainhtab = (chain_list *)gethtitem(htab,(void *)key)) == (chain_list *)EMPTYHT)
          {
           chainhtab = addchain(NULL,ptctc) ;
           addhtitem(htab,(void *)key,(long)chainhtab) ;
          }
         else
          {
           for(chainht = chainhtab ; chainht ; chainht = chainht->NEXT)
            {
             ptctc2 = (loctc_list*)(chainht->DATA);

             if( ptctc2->CAPA < 0.0 )
              continue;

             if((( ptctc2->SIG1  == scanlosig && ptctc2->SIG2 == othersig) &&
                (ptctc2->NODE1 == scanctcnode && ptctc2->NODE2 == othernode)) ||
               ((ptctc2->SIG2  == scanlosig && ptctc2->SIG1 == othersig) &&
                (ptctc2->NODE2 == scanctcnode && ptctc2->NODE1 == othernode)))
                {
                  ptctc->CAPA+= ptctc2->CAPA;
                  ptctc2->CAPA = -1.0;
                  verif = verif + 2 ;
                }
            }
           chainhtab = addchain(chainhtab,ptctc) ;
           addhtitem(htab,(void *)key,(long)chainhtab) ;
          }
        } 
     }
    }
   for( nbi = 0 ; nbi < htab->length ; nbi++)
    if((((ptitem = htab->pElem + nbi)->value) != (long)EMPTYHT) && 
       ((htab->pElem + nbi)->value != (long)DELETEHT))
      freechain((chain_list *)ptitem->value) ;
   delht(htab) ;
  }

  /* On fait le menage en virant les CTC dont la capa vaut -1 */

  for( scanchain = posttreatsig ; scanchain ; scanchain = scanchain->NEXT )
  {
    scanlosig = ( losig_list*)( scanchain->DATA );
    if((scanlosig->INDEX == 0) || ( ! scanlosig->PRCN ))
     continue ;

    prevctc = NULL;
    for( scanctc = scanlosig->PRCN->PCTC ; scanctc ; scanctc = nextscanctc )
    {
      nextscanctc = scanctc->NEXT;

      ptctc = (loctc_list*)( scanctc->DATA );

      if( ptctc->CAPA < 0.0 )
      {
        if( prevctc )
          prevctc->NEXT = nextscanctc ;
        else
          scanlosig->PRCN->PCTC = nextscanctc ;

        scanctc->NEXT = NULL;
        freechain( scanctc );
        
        prevctc2 = NULL;
        sigctc2 = ( ptctc->SIG1 == scanlosig ? ptctc->SIG2 : ptctc->SIG1 );
        for( scanctc2 = sigctc2->PRCN->PCTC ;                 
             scanctc2 ;
             scanctc2 = scanctc2->NEXT
           )
        {
          if( scanctc2->DATA == ptctc )
          {
            /* cas particulier : une CTC sur un  meme signal, dont les
               chain_list sont consecutifs */
            if( scanctc2 == nextscanctc )
              nextscanctc = nextscanctc->NEXT;

            if( prevctc2 )
              prevctc2->NEXT = scanctc2->NEXT;
            else
              sigctc2->PRCN->PCTC = scanctc2->NEXT;

            scanctc2->NEXT = NULL;
            freechain( scanctc2 );
            break;
          }
          prevctc2 = scanctc2;
        }

        freeloctc( ptctc );
      }
      else
        prevctc = scanctc;
    }
  }
  
  freechain( posttreatsig );
}

void debugctc2 ( lofig_list *ptfig )
{
   losig_list   *sig;
   chain_list   *scanchain;
   loctc_list   *ptctc;

   printf( "Information sur la figure %s.\n", ptfig->NAME );
   for( sig = ptfig->LOSIG ; sig ; sig = sig->NEXT )
   {
     printf( "Signal %2ld (%08X)\n", sig->INDEX, (unsigned long) sig );
     if( sig->PRCN )
     {
       for( scanchain = sig->PRCN->PCTC ;
            scanchain ;
            scanchain = scanchain->NEXT
          )
       {
         ptctc = ( loctc_list* )( scanchain->DATA );
         printf(
                "  CTC (%lX) entre le signal %2ld (%lX) et le signal %2ld (%lX).\n",
                 (unsigned long)ptctc,
                 ptctc->SIG1->INDEX,
                 (unsigned long)ptctc->SIG1,
                 ptctc->SIG2->INDEX,
                 (unsigned long)ptctc->SIG2
               );
       }
     }
   }
}

void debugctc ( losig_list *headlosig, int niveau )
{
  losig_list            *scanlosig;
  chain_list            *scanctc;
  chain_list            *scanctc2;
  loctc_list            *ptctc;
  
  /* Boucle de debuggage */
  for( scanlosig = headlosig ; scanlosig ; scanlosig = scanlosig->NEXT )
  {
    if( ! scanlosig->PRCN )
      continue;
    
    for( scanctc = scanlosig->PRCN->PCTC ; scanctc ; scanctc = scanctc->NEXT )
    {
      ptctc = (loctc_list*)( scanctc->DATA );
      if( getptype( ptctc->USER, FLATTEN_CTC ) )
      {
        printf( "(%d) FLATTEN_CTC trouve dans la CTC (%lX)  entre %ld.%ld et %ld.%ld.\n",
                niveau,
                (unsigned long)ptctc,
                ptctc->SIG1->INDEX,
                ptctc->NODE1,
                ptctc->SIG2->INDEX,
                ptctc->NODE2
              );
        exit(-1);
      }
      
      for( scanctc2 = ( ptctc->SIG1 == scanlosig ? ptctc->SIG2 : ptctc->SIG1 )->PRCN->PCTC ;
           scanctc2 ;
           scanctc2 = scanctc2->NEXT )
      {
        if( scanctc2->DATA == ptctc )
          break;
      }
      if( !scanctc2 )
      {
        printf( "(%d) CTC (%lX) entre %ld:%ld et %ld:%ld sur signal %ld non trouvee sur l'autre signal.\n",
                niveau,
                (unsigned long)ptctc,
                ptctc->SIG1->INDEX, ptctc->NODE1,
                ptctc->SIG2->INDEX, ptctc->NODE2,
                scanlosig->INDEX
              );
        exit(-1);
      }
    }
  }
}

/*******************************************************************************
* function loadlofig                                                           *
*******************************************************************************/
void loadlofig(lofig_list *ptfig, const char *name, char mode)
{
   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout,
                     "--- mbk --- loadlofig : reading file %s.%s mode %c\n",
                     name, IN_LO, mode);
   if ((!strcmp(IN_LO, "hns")) || (!strcmp(IN_LO, "fne"))
         || (!strcmp(IN_LO, "hdn")) || (!strcmp(IN_LO, "fdn")))
      vtiloadlofig(ptfig, name, mode);
   else if ((!strcmp(IN_LO, "al")) || (!strcmp(IN_LO, "alx")))
      alcloadlofig(ptfig, name, mode);
   else if (!strcmp(IN_LO, "spi") || !strcmp( IN_LO, "sp" ) ||
                !strcmp(IN_LO, "cir"))
      spiceloadlofig(ptfig, name, mode);
   else if (!strcmp(IN_LO, "edi"))
      edifloadlofig(ptfig, name, mode);
   else if (!strcmp(IN_LO, "vst"))
      vhdlloadlofig(ptfig, name, mode);
   else {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "MBK_IN_LO : '%s' unknown format\n", IN_LO);
      EXIT(1);
   }
   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout,
                     "--- mbk --- loadlofig : done reading file %s.%s\n",
                     name, IN_LO);
}

/*******************************************************************************
* function savelofig                                                           *
*******************************************************************************/
void savelofig(lofig_list *ptfig)
{
   if ((!strcmp(OUT_LO, "hns")) || (!strcmp(OUT_LO, "fne"))
         || (!strcmp(OUT_LO, "hdn")) || (!strcmp(OUT_LO, "fdn")))
      vtisavelofig(ptfig);
   else if ((!strcmp(OUT_LO, "al")) || (!strcmp(OUT_LO, "alx")))
      alcsavelofig(ptfig);
   else if (!strcmp(OUT_LO, "spi")  || !strcmp( OUT_LO, "sp" ) ||
                !strcmp(OUT_LO, "cir"))
      spicesavelofig(ptfig);
   else if (!strcmp(OUT_LO, "edi"))
      edifsavelofig(ptfig);
   else if (!strcmp(OUT_LO, "vst"))
      vhdlsavelofig(ptfig);
   else if (!strcmp(OUT_LO, "cct"))
      hilosavelofig(ptfig);
        else if (!strcmp(OUT_LO, "vlg"))
                vlogsavelofig(ptfig);
   else {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "MBK_OUT_LO : %s unknown format\n", OUT_LO);
      EXIT(1);
   }
}

/*******************************************************************************
* function rflattenlofig()                                                     *
* flatten recursif sur une figure logique ptfig.                               *
* concat permet de generer les cheminons (concat != 'N')                       *
* la mise a plat s'effectue jusqu'au cellules du catalogue                     *
* non comprises.                                                               *
* catal = `y` ou catal=`Y` indique qu`il faut tenir compte du                  *
* catalogue. Une autre valeur implique un flatten complet.                     *
*******************************************************************************/
void rflattenlofig(lofig_list *ptfig, char concat, char catal)
{
loins_list *p;
chain_list *c;

   catal = catal == NO ? 0 : 1;
   for (p = ptfig->LOINS; p != NULL;) { 
      if (!catal || !incatalog(p->FIGNAME) || incatalogdelete(p->FIGNAME)) {
         flattenlofig(ptfig, p->INSNAME, concat);
         p = ptfig->LOINS;
      }
      else
         p = p->NEXT;
   }
   freechain(ptfig->MODELCHAIN);
   ptfig->MODELCHAIN = NULL;
   p = ptfig->LOINS;
   while (p != NULL) {
      c = ptfig->MODELCHAIN;
      while (c != NULL) {
         if ((char *)c->DATA == p->FIGNAME)
            break;
         c = c->NEXT;
      }
      if (c == NULL)
         ptfig->MODELCHAIN = addchain(ptfig->MODELCHAIN, (void *)p->FIGNAME);
      p = p->NEXT;
   } 
}

/*
UNFLATLOFIG

II. Algorithme
--------------

1. On fait un LOFIGCHAIN sur 'ptfig'.

2. On veut construire deux 'chain_list' :
   LI : signaux internes a la nouvelle figure
   LX : signaux externes a la nouvelle figure

   Pour la figure 'ptfig', on parcourt la liste des signaux :
      Pour chaque signal, on parcourt la liste des connecteurs qui lui
      sont associes (par LOFIGCHAIN):
        * Si AUCUN connecteur n'appartient a une instance presente dans
          la liste 'list', on passe au signal suivant (pas d'insertion)

        * Sinon, si TOUS les connecteurs appartiennent a une instance
          presente dans la liste 'list', le signal est insere dans la
          liste LI
        * Sinon, le signal est insere dans la liste LX
          (au moins un connecteur n'appartient pas a la nouvelle figure)

3. On construit la nouvelle figure. Il faut creer :
   - une liste des signaux
   - une liste des connecteurs externes
   - une liste des instances, chacune contenant une liste de connecteurs

 a. liste des signaux
    on parcourt la liste LI et la liste LX : on ajoute chaque signal a
    la liste des signaux de la nouvelle figure.
    On construit une structure ptype (de nom UNFLATLOFIG), donnant a
    chaque signal de LI ou LX un pointeur vers le signal cree dans la
    nouvelle figure.

 b. liste des connecteurs externes
    on parcourt la liste LX :
    pour chaque signal, on cherche le premier connecteur interne (a la
    nouvelle figure) et le premier connecteur externe (a la nouvelle
    figure), de preference un terminal de la figure pere: s'il en
    existe un (terminal) on prend celui-la, sinon on prend le
    premier externe a la nouvelle figure, mais non terminal dans
    la figure pere.
    On ajoute un connecteur externe a la nouvelle figure. Il pointe
    sur le signal courant (dans la nouvelle). Sa direction est:
    - si le connecteur n'appartenant pas a la nouvelle figure est
      un terminal du pere (externe), le nouveau connecteur prend la
      direction de ce terminal.
    - si le connecteur n'appartenant pas a la nouvelle figure est un
      connecteur d'instance, le nouveau connecteur prend la direction
      du premier connecteur INTERNE a la nouvelle figure trouve 
      precedemment. 
    Le nom du nouveau connecteur est:
    - le nom du signal auquel il est connecte si ce dernier en a un.
    - sinon :
      . si le premier connecteur n'appartenant pas a la nouvelle
        figure est un terminal, le nouveau prend ce nom,
      . si le premier connecteur n'appartenant pas a la nouvelle
        figure est un connecteur d'instance, le nouveau prend le
        nom du connecteur INTERNE a la nouvelle figure (comme pour
        la direction).
    Dans tous les cas, on concatene au nom un compteur (comptant le
    nombre de connecteurs externes crees) pour assurer l'unicite.

 c. liste des instances
    on parcourt la liste 'list' :
    on insere chaque instance dans la liste d'instances de la nouvelle
    figure. Pour chaque connecteur d'une instance de 'list', on
    ajoute un connecteur dans la liste des connecteurs de l'instance
    dans la nouvelle figure, dont le signal dans la nouvelle figure est
    obtenu en consultant la structure ptype du signal dans la figure pere.
    Les connecteurs prennent les directions des connecteurs dans les
    instances de la figure pere.

4. On modifie la figure pere.
    On instancie la nouvelle figure.
    On detruit les signaux internes (LI).
    On detruit les instances de la liste 'list'.
*/
/*******************************************************************************
* function unflattenlofig                                                      *
*******************************************************************************/
lofig_list *unflattenlofig(lofig_list *ptfig, const char  *figname, const char  *insname, chain_list *list)
{
lofig_list *newlofig = NULL;
chain_list *li_head = NULL, *lx_head = NULL, *ptchain;
losig_list *ptsig;

   if (ptfig == NULL) 
      return NULL;
   lofigchain(ptfig);
   /* scan signals:
      construct LI and LX. */
   for (ptsig = ptfig->LOSIG; ptsig != NULL; ptsig = ptsig->NEXT) {
   chain_list *loconchain;
   int found_int = 0, found_ext = 0;
      /* scan connectors of signal */
      for (loconchain =
            (chain_list * )((getptype (ptsig->USER, (long)LOFIGCHAIN))->DATA);
            loconchain != NULL; loconchain = loconchain->NEXT) {
      locon_list *ptcon;
      chain_list *loinschain;
         ptcon = (locon_list * )(loconchain->DATA);
         for (loinschain = list; loinschain != NULL;
               loinschain = loinschain->NEXT)
            if ((void *)(loinschain->DATA) == ptcon->ROOT) 
               break;
         if (loinschain == NULL) 
            found_ext ++; 
         else 
            found_int ++;
      }
      if (found_int == 0) 
         continue; /* no insertion */
      if (found_ext == 0) 
         li_head = addchain(li_head, (void *)(ptsig)); /* insert LI */
      else 
         lx_head = addchain(lx_head, (void *)(ptsig)); /* insert LX */
   } /* endfor ptsig */

   /***** add new figure : */
   /* create signals list */
   newlofig = addlofig(figname);
   for (ptchain = li_head; ptchain != NULL; ptchain = ptchain->NEXT) {
   losig_list * ptlosig;
   ptype_list * ptype;
   chain_list * namechain = NULL, *ptaux;

      ptlosig = (losig_list *)(ptchain->DATA);
      for (ptaux = ptlosig->NAMECHAIN; ptaux != NULL; ptaux =
          ptaux->NEXT)
         namechain = addchain(namechain, ptaux->DATA);
      newlofig->LOSIG = addlosig(newlofig, ptlosig->INDEX, namechain, INTERNAL );
         if( ptlosig->PRCN ) {
            addlorcnet( newlofig->LOSIG );
            addcapa( newlofig->LOSIG, ptlosig->PRCN->CAPA );
         }
      ptlosig->USER = addptype(ptlosig->USER, (long)UNFLATLOFIG,
           (void * )NULL);
      ptype = getptype(ptlosig->USER, (long)UNFLATLOFIG);
      ptype->DATA = (void *)addchain((chain_list * )ptype->DATA,
           (void *)(newlofig->LOSIG));
   }
   for (ptchain = lx_head; ptchain != NULL; ptchain = ptchain->NEXT) {
   losig_list *ptlosig;
   ptype_list *ptype;
   chain_list *namechain = NULL, *ptaux;

      ptlosig = (losig_list *)(ptchain->DATA);
      for (ptaux = ptlosig->NAMECHAIN; ptaux != NULL; ptaux =
          ptaux->NEXT)
         namechain = addchain(namechain, ptaux->DATA);
      newlofig->LOSIG = addlosig(newlofig, ptlosig->INDEX, namechain, EXTERNAL );
      if( ptlosig->PRCN ) {
         addlorcnet( newlofig->LOSIG );
         addcapa( newlofig->LOSIG, ptlosig->PRCN->CAPA );
      }

      ptlosig->USER = addptype(ptlosig->USER, (long)UNFLATLOFIG,
           (void * )NULL);
      ptype = getptype (ptlosig->USER, (long)UNFLATLOFIG);
      ptype->DATA = (void *)addchain((chain_list *)ptype->DATA,
           (void *)(newlofig->LOSIG));
   }

   /* create terminals list */
   for (ptchain = lx_head; ptchain != NULL; ptchain = ptchain->NEXT) {
   locon_list *intlocon = NULL, *extlocon = NULL;
   losig_list *ptlosig, *newlosig;
   ptype_list *ptype;
   chain_list *loconchain, *auxchain;
   static char loconname[BUFSIZE];
   char  *name; 
   char  locondir; 
   int   i, foundterm = 0;

      /* scan connectors list of the current signal */
      ptlosig = (losig_list *)(ptchain->DATA);
      for (loconchain =
            (chain_list *)((getptype (ptlosig->USER, (long)LOFIGCHAIN))->DATA);
            loconchain != NULL; loconchain = loconchain->NEXT) {
      locon_list *ptlocon;
      chain_list *loinschain;

         ptlocon = (locon_list *)(loconchain->DATA);
         for (loinschain = list; loinschain != NULL; loinschain =
             loinschain->NEXT)
            if ((void *)(loinschain->DATA) == ptlocon->ROOT) 
               break;
         if (loinschain == NULL) { /* external connector of new figure */
            if (ptlocon->TYPE == EXTERNAL) /* external con. in root figure too */ { 
               foundterm++; 
               extlocon = ptlocon; 
            } else if (extlocon == NULL) 
               extlocon = ptlocon;
         } else if (intlocon == NULL) 
            intlocon = ptlocon; /* internal con. in new */
      } /* endfor loconchain */

      /* create new external connector for the new figure */
      locondir = foundterm ? extlocon->DIRECTION : intlocon->DIRECTION;
      name = (ptlosig->NAMECHAIN != NULL) ?  getsigname(ptlosig) :
          foundterm ? extlocon->NAME : intlocon->NAME;
      i = 0;
      /*
      while ( (name[i] != '\0') && (name[i] != ' ') ) { 
         loconname [i] = name[i]; 
         i++; 
      }
      loconname[i] = '\0';
      */
      /* Franck */
      strcpy(loconname, name);
      if ((name = strchr(loconname, ' ')) != NULL)
         *name = '_';

      /* the format used to be %s%lu%s but it generates vectors with a
         unique base name. 
         sprintf(loconname, "%s%lu%s", loconname, newtermcount++, &(name[i]));
         Since the interface of the resulting figure is not supposed to be
         used by humans, let's make simple names.
         Hey Hey, this was true a while ago, but as always, now we want
         humans to predict the ouput of the function. Fun, ain'it? */
      /* !Franck
      sprintf(loconname, "%s_%lu", loconname, newtermcount++);
      */
      ptype = getptype(ptlosig->USER, (long)UNFLATLOFIG);
      auxchain = (chain_list *)(ptype->DATA);
      newlosig = (losig_list *)(auxchain->DATA);
      newlofig->LOCON = addlocon(newlofig, loconname, newlosig, locondir);
   } /* endfor ptchain */


   /* create instances list */
   for (ptchain = list; ptchain != NULL; ptchain = ptchain->NEXT) {
   loins_list *ptloins;
   locon_list *ptlocon, *ptaux, *con;
   chain_list *signal = NULL;
   lofig_list *model;
   /* Why this model ?
      we need a name matching and not an order matching.
      we have to read in the file, because we need to have a lofig in order
      to be able to call addloins.
      The matching is done trivially, using a n^2 loop: since the number of
      connectors on an instance is small, an unflattenlofig very seldom
      called :-) it's not worth some malloc + qsort, and furthermore, I'm
      lazy today! */
   int i = 0, j = 0;

      ptloins = (loins_list *)(ptchain->DATA);
      model = getlofig(ptloins->FIGNAME, 'P');
      for (con = model->LOCON; con != NULL; con = con->NEXT)
         i++;
      for (ptlocon = ptloins->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT)
         j++;
      if (i != j) {
         fflush(stdout);
         fputs("*** mbk error ***\n", stderr);
         fputs("unflattenlofig : connector number inconsistency between model",
               stderr);
         fprintf(stderr, " '%s' and instance '%s'\n",
               model->NAME, ptloins->INSNAME);
         EXIT(1);
      }
      for (con = model->LOCON; con != NULL; con = con->NEXT) {
         for (ptlocon = ptloins->LOCON; ptlocon != NULL;
            ptlocon = ptlocon->NEXT) {
         chain_list *sigchain;
            if (ptlocon->NAME == con->NAME) {
               sigchain = (chain_list *)((getptype(ptlocon->SIG->USER,
                                           (long)UNFLATLOFIG))->DATA);
               signal = addchain(signal, (void *)(sigchain->DATA));
               break;
            } 
         }
      }
      signal = reverse(signal);
      newlofig->LOINS = addloins(newlofig, ptloins->INSNAME, model, signal);
      for (ptlocon = newlofig->LOINS->LOCON, ptaux = ptloins->LOCON;
          (ptlocon != NULL) && (ptaux != NULL);  ptlocon = ptlocon->NEXT,
           ptaux = ptaux->NEXT)
         ptlocon->DIRECTION = ptaux->DIRECTION;
   }

   /***** free ptype lists (lofigchain and unflatlofig) */
   /* free lofigchain */
   for (ptsig = ptfig->LOSIG; ptsig != NULL; ptsig = ptsig->NEXT) {
   ptype_list *ptype;

      ptype = getptype(ptsig->USER, (long)LOFIGCHAIN);
      freechain ((chain_list *)ptype->DATA);
      ptype->DATA = (void *)NULL;
      ptsig->USER = delptype(ptsig->USER, (long)LOFIGCHAIN);
   }

   /* free unflatlofig in lx signals */
   for (ptchain = lx_head; ptchain != NULL; ptchain = ptchain->NEXT) {
   ptype_list *ptype;
   losig_list *ptlosig;

      ptlosig = (losig_list *)(ptchain->DATA);
      ptype = getptype(ptlosig->USER, (long)UNFLATLOFIG);
      freechain ((chain_list *)ptype->DATA);
      ptype->DATA = (void *)NULL;
      ptlosig->USER = delptype(ptlosig->USER, (long)UNFLATLOFIG);
   }

   /* free unflatlofig in li signals */
   for (ptchain = li_head; ptchain != NULL; ptchain = ptchain->NEXT) {
   ptype_list *ptype;
   losig_list *ptlosig;

      ptlosig = (losig_list *)(ptchain->DATA);
      ptype = getptype (ptlosig->USER, (long)UNFLATLOFIG);
      freechain ((chain_list *)ptype->DATA);
      ptype->DATA = (void *)NULL;
      ptlosig->USER = delptype(ptlosig->USER, (long)UNFLATLOFIG);
   }

   /***** modify the root figure :*/
   /* add new instancied figure */
   lx_head = reverse (lx_head);
   ptfig->LOINS = addloins(ptfig, insname, getlofig(figname, 'P'), lx_head);

   /* delete signals */
   for (ptchain = li_head; ptchain != NULL; ptchain = ptchain->NEXT)
      dellosig(ptfig, ((losig_list *)(ptchain->DATA))->INDEX);

   /* delete instances */
   for (ptchain = list; ptchain != NULL; ptchain = ptchain->NEXT)
      delloins(ptfig, ((loins_list *)(ptchain->DATA))->INSNAME);
   return newlofig;
}

#define OK      1
#define PROBLEM 0
#define NOVBE  -1

int restoredirvbe(lofig_list *lof)
{
FILE *f = NULL;
char line[BUFSIZ];
int p, q, r, from, to, inc, nbcon;
int run = 0; /* shall we interpret what we read? */
char string[128], hook[128];
locon_list * fc, *fct;
char dir;

   if (!lof) {
      fprintf(stderr, "*** mbk error *** restoredirvbe : no figure given\n");
      return PROBLEM;
   }
   if ((f = mbkfopen(lof->NAME, "vbe", "r")) == NULL)
      if ((f = mbkfopen(lof->NAME, "vst", "r")) == NULL)
         return NOVBE;
   lof->LOCON = (locon_list * )reverse((chain_list * )lof->LOCON);
   nbcon = 0;
   while (1) {
      if (fgets(line, BUFSIZ, f) == NULL)
         break;
      for (p = 0; p < BUFSIZ && line[p] && line[p] != '\n'; p++)
         if (isupper((int)line[p]))
            line[p] = tolower(line[p]);
      p = 0;
      while (isspace((int)line[p]))
         p++;
      switch (line[p]) {
      case 'e': /* either entity or end for us */
         if (!strncmp("entity", &line[p], 6)) /* we shall treat now */
            run = 1;
         if (!strncmp("end", &line[p], 3)) /* we shall exit gracefully */ {
            fclose(f);
            for (p = 0, fct = lof->LOCON; fct; fct = fct->NEXT)
               p++;
            if (p != nbcon) {
               fprintf(stderr, "*** mbk error *** restoredirvbe : figure %s have more connectors than vbe\n",
                    lof->NAME);
               return PROBLEM;
            }
            return OK;
         }
         continue; /* next line guys */
      case 'p': /* port or nothing! */
         if (!run)
            continue;
         if (!strncmp("port", &line[p], 4)) {
            p += 4;
            while (isspace((int)line[p])) {
               if (line[p] == '\n') {
                  fgets(line, BUFSIZ, f);
                  p = 0;
               } else
                  p++;
            }
            if (line[p] != '(') {
               fprintf(stderr, "*** mbk error *** restoredirvbe : cannot read port well!\n");
               fclose(f);
               return PROBLEM;
            }

            while (1) {
               fgets(line, BUFSIZ, f);
               for (p = 0; p < BUFSIZ && line[p]; p++)
                  if (isupper((int)line[p]))
                     line[p] = tolower(line[p]);
               p = 0;
               while (isspace((int)line[p]))
                  p++;
               if (line[p] == ')')
                  break;
               q = p;
               while (!isspace((int)line[p]) && line[p] != ':')
                  p++;
               strncpy(hook, &line[q], p - q);
               hook[p - q] = '\0';

               while (isspace((int)line[p]) || line[p] == ':')
                  p++;
               q = p;
               while (!isspace((int)line[p]))
                  p++;
               strncpy(string, &line[q], p - q);
               string[p - q] = '\0';
               while (isspace((int)line[p]))
                  p++;
               if (line[p] == 'b' || line[p] == 'r') /* finished, dir ok */ {
                  if (!strcmp("in", string))
                     dir = IN;
                  else if (!strcmp("out", string))
                     dir = OUT;
                  else
                     dir = UNKNOWN;
               } else {
                  if (!strcmp("inout", string))
                     dir = TRANSCV;
                  else if (!strcmp("out", string))
                     dir = TRISTATE;
                  else
                     dir = UNKNOWN;
               }
               while (line[p] != '(' && line[p] != '\0')
                  p++;
               if (line[p] == '\0') {
                  fc = getlocon(lof, hook);
                  fc->DIRECTION = dir;
                  if (lof->LOCON != fc) {
                     for (fct = lof->LOCON; fct->NEXT != fc; fct = fct->NEXT)
                        ;
                     fct->NEXT = fc->NEXT;
                     fc->NEXT = lof->LOCON;
                     lof->LOCON = fc;
                  }
                  nbcon++;
               } else { /* bit vector indeed */
                  p++; /* skip ( */
                  from = atoi(&line[p]);
                  while (isdigit((int)line[p]))
                     p++;
                  while (isspace((int)line[p]))
                     p++;
                  if (line[p] == 'd')
                     inc = -1;
                  else if (line[p] == 't')
                     inc = 1;
                  else
                   {
                     fprintf(stderr, "*** mbk error *** restoredirvbe : bit_vect neither to nor downto\n");
                     fclose(f);
                     return PROBLEM;
                  }
                  while (!isspace((int)line[p]))
                     p++;
                  to = atoi(&line[p]);

                  for (r = from; r <= to; r += inc) {
                     sprintf(string, "%s %d", hook, r);
                     fc = getlocon(lof, string);
                     fc->DIRECTION = dir;
                     if (lof->LOCON != fc) {
                        for (fct = lof->LOCON; fct->NEXT != fc; fct = fct->NEXT)
/* code folded from here */
   ;
/* unfolding */
                        fct->NEXT = fc->NEXT;
                        fc->NEXT = lof->LOCON;
                        lof->LOCON = fc;
                     }
                     nbcon++;
                  }
               }
               continue;
            }
         }
         continue;
      default :
         continue;
      }
   }
   fclose(f);
   for (p = 0, fct = lof->LOCON; fct; fct = fct->NEXT)
      p++;
   if (p != nbcon) {
      fprintf(stderr, "*** mbk error *** restoredirvbe : figure %s have more connectors than vbe\n", lof->NAME);
      return PROBLEM;
   }
   return OK;
}

int guessextdir(lofig_list *lof)
{
int ret = OK;
char dir;
locon_list *ec;
chain_list *pch;
ptype_list *getptype();

   lofigchain(lof);
   for (ec = lof->LOCON; ec; ec = ec->NEXT) {
      dir = UNKNOWN;
      for (pch = (chain_list * )(getptype(ec->SIG->USER, (long)LOFIGCHAIN)->DATA);           pch; pch = pch->NEXT) {
         if (((locon_list * )(pch->DATA))->TYPE != INTERNAL)
            continue;
         switch (((locon_list * )(pch->DATA))->DIRECTION) {
         case IN :
            switch (dir) {
            case IN :
               dir = IN;
               break;
            case OUT :
               dir = INOUT;
               break;
            case INOUT :
               dir = INOUT;
               break;
            case UNKNOWN :
               dir = IN;
               break;
            case TRISTATE :
               dir = TRANSCV;
               break;
            case TRANSCV  :
            case TRANSCV2 :
            case TRANSCV3 :
            case TRANSCV4 :
               dir = TRANSCV;
               break;
            }
            break;
         case OUT :
            switch (dir) {
            case IN :
               dir = INOUT;
               break;
            case OUT :
               dir = OUT;
               fprintf(stderr, "Warning : signal %s have two OUT",
                        getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case INOUT :
               dir = INOUT;
               fprintf(stderr, "Warning : signal %s have an OUT and an INOUT",
                        getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case UNKNOWN :
               dir = OUT;
               break;
            case TRISTATE :
               dir = OUT;
               fprintf(stderr, "Warning : signal %s have an OUT and a TRISTATE",
                        getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case TRANSCV  :
            case TRANSCV2 :
            case TRANSCV3 :
            case TRANSCV4 :
               dir = TRANSCV;
               fprintf(stderr, "Warning : signal %s have an OUT and a TRANSCV",
                        getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            }
            break;
         case INOUT :
            switch (dir) {
            case IN :
               dir = INOUT;
               break;
            case OUT :
               dir = INOUT;
               fprintf(stderr, "Warning : signal %s have an INOUT and an OUT",
                        getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case INOUT :
               dir = INOUT;
               fprintf(stderr, "Warning : signal %s have two INOUT",
                        getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case UNKNOWN :
               dir = INOUT;
               break;
            case TRISTATE :
               dir = INOUT;
               fprintf(stderr, "Warning : signal %s have an INOUT and a TRISTATE",
                        getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case TRANSCV  :
            case TRANSCV2 :
            case TRANSCV3 :
            case TRANSCV4 :
               dir = INOUT;
               fprintf(stderr, "Warning : signal %s have an INOUT and a TRANSCV",
                        getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            }
            break;
         case UNKNOWN :
            switch (dir) {
            case IN :
               dir = IN;
               break;
            case OUT :
               dir = OUT;
               break;
            case INOUT :
               dir = INOUT;
               break;
            case UNKNOWN :
               dir = UNKNOWN;
               break;
            case TRISTATE :
               dir = TRISTATE;
               break;
            case TRANSCV :
            case TRANSCV2 :
            case TRANSCV3 :
            case TRANSCV4 :
               dir = TRANSCV;
               break;
            }
            break;
         case TRISTATE :
            switch (dir) {
            case IN :
               dir = TRANSCV;
               break;
            case OUT :
               dir = OUT;
               fprintf(stderr, "Warning : signal %s have a TRISTATE and an OUT", getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case INOUT :
               dir = INOUT;
               fprintf(stderr, "Warning : signal %s have a TRISTATE and an INOUT", getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case UNKNOWN :
               dir = TRISTATE;
               break;
            case TRISTATE :
               dir = TRISTATE;
               break;
            case TRANSCV  :
            case TRANSCV2 :
            case TRANSCV3 :
            case TRANSCV4 :
               dir = TRANSCV;
               break;
            }
            break;
         case TRANSCV  :
         case TRANSCV2 :
         case TRANSCV3 :
         case TRANSCV4 :

            switch (dir) {
            case IN :
               dir = TRANSCV;
               break;
            case OUT :
               dir = TRANSCV;
               fprintf(stderr, "Warning : signal %s have a TRANSCV and an OUT", getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case INOUT :
               dir = INOUT;
               fprintf(stderr, "Warning : signal %s have a TRANSCV and an INOUT", getsigname(ec->SIG));
               ret = PROBLEM;
               break;
            case UNKNOWN :
               dir = TRANSCV;
               break;
            case TRISTATE :
               dir = TRANSCV;
               break;
            case TRANSCV  :
            case TRANSCV2 :
            case TRANSCV3 :
            case TRANSCV4 :
               dir = TRANSCV;
               break;
            }
            break;
         }
      }
      if (dir != UNKNOWN)
         ec->DIRECTION = dir;
   }
   return ret;
}

int restorealldir(lofig_list *lf)
{
int ret = OK, res;
lofig_list *lof;
loins_list *li;
locon_list *ic, *fc;

   for (li = lf->LOINS; li; li = li->NEXT) {
      /* get model of the instance and restore connector direction in it */
      for (lof = HEAD_LOFIG; lof; lof = lof->NEXT)
         if (lof->NAME == li->FIGNAME)
            break;
      if (!lof) {
         lof = getlofig(li->FIGNAME, 'P');
         res = restoredirvbe(lof);
         if (res == PROBLEM)
            ret = PROBLEM;
         else if (res == NOVBE) {
            lof = getlofig(lof->NAME, 'A');
            res = restorealldir(lof);
            if (res == PROBLEM)
               ret = PROBLEM;
            res = guessextdir(lof);
            if (res == PROBLEM)
               ret = PROBLEM;
         }
      } else if (lof->LOCON->DIRECTION == UNKNOWN) {
         res = restoredirvbe(lof);
         if (res == PROBLEM)
            ret = PROBLEM;
         else if (res == NOVBE) {
            if (lof->MODE != 'A')
               lof = getlofig(lof->NAME, 'A');
            res = restorealldir(lof);
            if (res == PROBLEM)
               ret = PROBLEM;
            res = guessextdir(lof);
            if (res == PROBLEM)
               ret = PROBLEM;
         }
      }

      /* restore connector direction in the instance */
      for (ic = li->LOCON; ic; ic = ic->NEXT) {
         fc = getlocon(lof, ic->NAME);
         ic->DIRECTION = fc->DIRECTION;
      }
   }
   return ret;
}

struct typoin           /* structure used by dast_dbg */
  {               /* to store its state      */
  short  type;          /* code of the current structure*/
  short  mark;          /* stop mark         */
  void  *data;          /* pointer of the structure   */
  };

#define VHD_ERRDFN  -1
#define VHD__XTDFN  0
#define VHD__UPDFN  1
#define VHD__TPDFN  2
#define VHD__SPDFN  3
#define VHD__BKDFN  4

#define VHD_lofigDFN 5
#define VHD_nextDFN 6
#define VHD_modelchainDFN 7
#define VHD_loconDFN 8
#define VHD_losigDFN 9
#define VHD_bksigDFN 10
#define VHD_loinsDFN 11
#define VHD_lotrsDFN 13
#define VHD_nameDFN 14
#define VHD_modeDFN 15
#define VHD_userDFN 16
#define VHD_drainDFN 17
#define VHD_gridDFN 18
#define VHD_sourceDFN 19
#define VHD_bulkDFN 36
#define VHD_widthDFN 20
#define VHD_lengthDFN 21
#define VHD_xDFN 22
#define VHD_yDFN 23
#define VHD_typeDFN 24
#define VHD_insnameDFN 25
#define VHD_fignameDFN 26
#define VHD_sigDFN 27
#define VHD_rootDFN 28
#define VHD_directionDFN 29
#define VHD_valDFN 30
#define VHD_namechainDFN 31
#define VHD_capaDFN 32
#define VHD_indexDFN 33
#define VHD_ptypeDFN 34
#define VHD_dataDFN 35

#define VHD_locapDFN 36
#define VHD_loresDFN 37
#define VHD_loselfDFN 38
#define VHD_tconDFN 39
#define VHD_bconDFN 40
#define VHD_rcon1DFN 41
#define VHD_rcon2DFN 42
#define VHD_scon1DFN 43
#define VHD_scon2DFN 44

#define VHD_MAXDFN 45

static int vhd_getcmd();
static int vhd_hash();

void mlodebug (head_pnt,stru_name)

void  *head_pnt;
char  *stru_name;

  {

  int     i;
  int     state;
  int     newsta0;
  int     readcmd = 0;
  char       readtab[3][20];
  int     stk_pnt = 0;
  int            type = VHD__XTDFN;
  struct chain  *ptchain;

  struct typoin  current_pnt;
  struct typoin  stk[200];

  char           empty[4];
  char           avail[12];

  int     key[VHD_MAXDFN];
  char      *tab[VHD_MAXDFN];
  void      *nxt[VHD_MAXDFN];
  short      typ[VHD_MAXDFN];

  struct lofig *lofig_pnt;
  struct locon *locon_pnt;
  struct losig *losig_pnt;
  struct ptype *ptype_pnt;
  struct loins *loins_pnt;
  struct lotrs *lotrs_pnt;

  struct locap  *locap_pnt;
  struct lores  *lores_pnt;
  struct loself *loself_pnt;

key[VHD_lofigDFN]      = vhd_hash ("lofig");
key[VHD_nextDFN]       = vhd_hash ("next");
key[VHD_modelchainDFN] = vhd_hash ("modelchain");
key[VHD_loconDFN]      = vhd_hash ("locon");
key[VHD_losigDFN]      = vhd_hash ("losig");
key[VHD_bksigDFN]      = vhd_hash ("bksig");
key[VHD_loinsDFN]      = vhd_hash ("loins");
key[VHD_lotrsDFN]      = vhd_hash ("lotrs");
key[VHD_nameDFN]       = vhd_hash ("name");
key[VHD_modeDFN]       = vhd_hash ("mode");
key[VHD_userDFN]       = vhd_hash ("user");
key[VHD_drainDFN]      = vhd_hash ("drain");
key[VHD_gridDFN]       = vhd_hash ("grid");
key[VHD_sourceDFN]     = vhd_hash ("source");
key[VHD_bulkDFN]       = vhd_hash ("bulk");
key[VHD_widthDFN]      = vhd_hash ("width");
key[VHD_lengthDFN]     = vhd_hash ("length");
key[VHD_xDFN]          = vhd_hash ("x");
key[VHD_yDFN]          = vhd_hash ("y");
key[VHD_typeDFN]       = vhd_hash ("type");
key[VHD_insnameDFN]    = vhd_hash ("insname");
key[VHD_fignameDFN]    = vhd_hash ("figname");
key[VHD_sigDFN]        = vhd_hash ("sig");
key[VHD_rootDFN]       = vhd_hash ("root");
key[VHD_directionDFN]  = vhd_hash ("direction");
key[VHD_valDFN]        = vhd_hash ("val");
key[VHD_namechainDFN]  = vhd_hash ("namechain");
key[VHD_capaDFN]       = vhd_hash ("capa");
key[VHD_indexDFN]      = vhd_hash ("index");
key[VHD_ptypeDFN]      = vhd_hash ("ptype");

key[VHD_locapDFN]      = vhd_hash ("locap");
key[VHD_loresDFN]      = vhd_hash ("lores");
key[VHD_loselfDFN]     = vhd_hash ("loself");
key[VHD_tconDFN]       = vhd_hash ("tcon");
key[VHD_bconDFN]       = vhd_hash ("bcon");
key[VHD_rcon1DFN]      = vhd_hash ("rcon1");
key[VHD_rcon2DFN]      = vhd_hash ("rcon2");
key[VHD_scon1DFN]      = vhd_hash ("scon1");
key[VHD_scon2DFN]      = vhd_hash ("scon2");

   /* ###------------------------------------------------------### */
   /*    Set of predefined commands          */
   /* ###------------------------------------------------------### */

  key[0] = vhd_hash ("_exit");
  key[1] = vhd_hash ("_up");
  key[2] = vhd_hash ("_top");
  key[3] = vhd_hash ("_stop");
  key[4] = vhd_hash ("_back");

  (void)strcpy (empty,"");
  (void)strcpy (avail,"AVAILABLE");

  for (i=0 ; i<VHD_MAXDFN ; i++)
    typ[i] = i;

   /* ###------------------------------------------------------### */
   /*    Find the type of the head structure       */
   /* ###------------------------------------------------------### */

  readcmd = vhd_hash (stru_name);
  for (i=0 ; i<VHD_MAXDFN ; i++)
    {
    if (readcmd == key[i])
      {
      type = typ[i];
      break;
      }
    }

   /* ###------------------------------------------------------### */
   /*    Exit if the head structure is empty       */
   /* ###------------------------------------------------------### */

  if (head_pnt == NULL)
    type = VHD__XTDFN;

  current_pnt.data = head_pnt;
  current_pnt.type = type;

  state            = type;

  while (state != VHD__XTDFN)
    {
   /* ###------------------------------------------------------### */
   /*    Print structure's field until the exit command is read   */
   /* ###------------------------------------------------------### */

    for (i=0 ; i<VHD_MAXDFN ; i++)
      {
      tab[i] = empty;
      nxt[i] = NULL;
      }

   /* ###------------------------------------------------------### */
   /*    _exit and _stop commands are allways available     */
   /* ###------------------------------------------------------### */

    tab[VHD__XTDFN] = avail;
    tab[VHD__SPDFN] = avail;

   /* ###------------------------------------------------------### */
   /*    _up, _top, and _back commands are available only if the  */
   /* stack is not empty                  */
   /* ###------------------------------------------------------### */

    if (stk_pnt != 0)
      {
      tab[VHD__UPDFN] = avail;
      tab[VHD__TPDFN] = avail;
      tab[VHD__BKDFN] = avail;
      }

    switch (state)
      {

    case (VHD_lofigDFN):

      /* ###--------- lofig ---------### */

      lofig_pnt = (struct lofig *)(current_pnt.data);

      if (lofig_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)lofig_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_lofigDFN;
        }
      if (lofig_pnt->LOCON != NULL)
        {
        tab[VHD_loconDFN] = avail;
        nxt[VHD_loconDFN] = (void *)lofig_pnt->LOCON;
        typ[VHD_loconDFN] = VHD_loconDFN;
        }
      if (lofig_pnt->LOSIG != NULL)
        {
        tab[VHD_losigDFN] = avail;
        nxt[VHD_losigDFN] = (void *)lofig_pnt->LOSIG;
        typ[VHD_losigDFN] = VHD_losigDFN;
        }
      if (lofig_pnt->BKSIG != NULL)
        {
        tab[VHD_bksigDFN] = avail;
        nxt[VHD_bksigDFN] = (void *)lofig_pnt->BKSIG;
        typ[VHD_bksigDFN] = VHD_ptypeDFN;
        }
      if (lofig_pnt->LOINS != NULL)
        {
        tab[VHD_loinsDFN] = avail;
        nxt[VHD_loinsDFN] = (void *)lofig_pnt->LOINS;
        typ[VHD_loinsDFN] = VHD_loinsDFN;
        }
      if (lofig_pnt->LOTRS != NULL)
        {
        tab[VHD_lotrsDFN] = avail;
        nxt[VHD_lotrsDFN] = (void *)lofig_pnt->LOTRS;
        typ[VHD_lotrsDFN] = VHD_lotrsDFN;
        }

      if (lofig_pnt->LOCAP != NULL)
        {
        tab[VHD_locapDFN] = avail;
        nxt[VHD_locapDFN] = (void *)lofig_pnt->LOCAP;
        typ[VHD_locapDFN] = VHD_locapDFN;
        }
      if (lofig_pnt->LORES != NULL)
        {
        tab[VHD_loresDFN] = avail;
        nxt[VHD_loresDFN] = (void *)lofig_pnt->LORES;
        typ[VHD_loresDFN] = VHD_loresDFN;
        }
      if (lofig_pnt->LOSELF != NULL)
        {
        tab[VHD_loselfDFN] = avail;
        nxt[VHD_loselfDFN] = (void *)lofig_pnt->LOSELF;
        typ[VHD_loselfDFN] = VHD_loselfDFN;
        }


      if (lofig_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)lofig_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      ptchain = lofig_pnt->MODELCHAIN;
      (void)printf ("   modelchain  :\n");
      while (ptchain != NULL)
        {
        (void)printf ("                 %s\n",(char *)ptchain->DATA);
        ptchain = ptchain->NEXT;
        }

      (void)printf ("   name        : %s\n",lofig_pnt->NAME);
      (void)printf ("   mode        : %c\n",lofig_pnt->MODE);
      (void)printf ("-> locon       : %s\n",tab[VHD_loconDFN]);
      (void)printf ("-> losig       : %s\n",tab[VHD_losigDFN]);
      (void)printf ("-> bksig       : %s\n",tab[VHD_bksigDFN]);
      (void)printf ("-> loins       : %s\n",tab[VHD_loinsDFN]);
      (void)printf ("-> lotrs       : %s\n",tab[VHD_lotrsDFN]);

      (void)printf ("-> locap       : %s\n",tab[VHD_locapDFN]);
      (void)printf ("-> lores       : %s\n",tab[VHD_loresDFN]);
      (void)printf ("-> loself      : %s\n",tab[VHD_loselfDFN]);

      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_lotrsDFN):

      /* ###--------- lotrs ---------### */

      lotrs_pnt = (struct lotrs *)(current_pnt.data);

      if (lotrs_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)lotrs_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_lotrsDFN;
        }
      if (lotrs_pnt->DRAIN != NULL)
        {
        tab[VHD_drainDFN] = avail;
        nxt[VHD_drainDFN] = (void *)lotrs_pnt->DRAIN;
        typ[VHD_drainDFN] = VHD_loconDFN;
        }
      if (lotrs_pnt->GRID != NULL)
        {
        tab[VHD_gridDFN] = avail;
        nxt[VHD_gridDFN] = (void *)lotrs_pnt->GRID;
        typ[VHD_gridDFN] = VHD_loconDFN;
        }
      if (lotrs_pnt->SOURCE != NULL)
        {
        tab[VHD_sourceDFN] = avail;
        nxt[VHD_sourceDFN] = (void *)lotrs_pnt->SOURCE;
        typ[VHD_sourceDFN] = VHD_loconDFN;
        }
      if (lotrs_pnt->BULK != NULL)
        {
        tab[VHD_sourceDFN] = avail;
        nxt[VHD_sourceDFN] = (void *)lotrs_pnt->BULK;
        typ[VHD_sourceDFN] = VHD_loconDFN;
        }
      if (lotrs_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)lotrs_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("-> drain       : %s\n",tab[VHD_drainDFN]);
      (void)printf ("-> grid        : %s\n",tab[VHD_gridDFN]);
      (void)printf ("-> source      : %s\n",tab[VHD_sourceDFN]);
      (void)printf ("-> bulk        : %s\n",tab[VHD_bulkDFN]);
      (void)printf ("   length      : %ld\n",lotrs_pnt->LENGTH);
      (void)printf ("   width       : %ld\n",lotrs_pnt->WIDTH);
      (void)printf ("   y           : %ld\n",lotrs_pnt->Y);
      (void)printf ("   x           : %ld\n",lotrs_pnt->X);
      (void)printf ("   type        : %c\n",lotrs_pnt->TYPE);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;


    case (VHD_locapDFN):

      /* ###--------- locap ---------### */

      locap_pnt = (struct locap *)(current_pnt.data);

      if (locap_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)locap_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_locapDFN;
        }
      if (locap_pnt->TCON != NULL)
        {
        tab[VHD_tconDFN] = avail;
        nxt[VHD_tconDFN] = (void *)locap_pnt->TCON;
        typ[VHD_tconDFN] = VHD_loconDFN;
        }
      if (locap_pnt->BCON != NULL)
        {
        tab[VHD_bconDFN] = avail;
        nxt[VHD_bconDFN] = (void *)locap_pnt->BCON;
        typ[VHD_bconDFN] = VHD_loconDFN;
        }
      if (locap_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)locap_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("-> tcon        : %s\n",tab[VHD_tconDFN]);
      (void)printf ("-> bcon        : %s\n",tab[VHD_bconDFN]);
      (void)printf ("   type        : %c\n",locap_pnt->TYPE);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;


    case (VHD_loresDFN):

      /* ###--------- lores ---------### */

      lores_pnt = (struct lores *)(current_pnt.data);

      if (lores_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)lores_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_loresDFN;
        }
      if (lores_pnt->RCON1 != NULL)
        {
        tab[VHD_rcon1DFN] = avail;
        nxt[VHD_rcon1DFN] = (void *)lores_pnt->RCON1;
        typ[VHD_rcon1DFN] = VHD_loconDFN;
        }
      if (lores_pnt->RCON2 != NULL)
        {
        tab[VHD_rcon2DFN] = avail;
        nxt[VHD_rcon2DFN] = (void *)lores_pnt->RCON2;
        typ[VHD_rcon2DFN] = VHD_loconDFN;
        }
      if (lores_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)lores_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("-> rcon1       : %s\n",tab[VHD_rcon1DFN]);
      (void)printf ("-> rcon2       : %s\n",tab[VHD_rcon2DFN]);
      (void)printf ("   type        : %c\n",lores_pnt->TYPE);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_loselfDFN):

      /* ###--------- loself ---------### */

      loself_pnt = (struct loself *)(current_pnt.data);

      if (loself_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)loself_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_loselfDFN;
        }
      if (loself_pnt->SCON1 != NULL)
        {
        tab[VHD_scon1DFN] = avail;
        nxt[VHD_scon1DFN] = (void *)loself_pnt->SCON1;
        typ[VHD_scon1DFN] = VHD_loconDFN;
        }
      if (loself_pnt->SCON2 != NULL)
        {
        tab[VHD_scon2DFN] = avail;
        nxt[VHD_scon2DFN] = (void *)loself_pnt->SCON2;
        typ[VHD_scon2DFN] = VHD_loconDFN;
        }
      if (loself_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)loself_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("-> scon1       : %s\n",tab[VHD_scon1DFN]);
      (void)printf ("-> scon2       : %s\n",tab[VHD_scon2DFN]);
      (void)printf ("   type        : %c\n",loself_pnt->TYPE);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;


    case (VHD_loinsDFN):

      /* ###--------- loins ---------### */

      loins_pnt = (struct loins *)(current_pnt.data);

      if (loins_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)loins_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_loinsDFN;
        }
      if (loins_pnt->LOCON != NULL)
        {
        tab[VHD_loconDFN] = avail;
        nxt[VHD_loconDFN] = (void *)loins_pnt->LOCON;
        typ[VHD_loconDFN] = VHD_loconDFN;
        }
      if (loins_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)loins_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("   insname     : %s\n",loins_pnt->INSNAME);
      (void)printf ("   figname     : %s\n",loins_pnt->FIGNAME);
      (void)printf ("-> locon       : %s\n",tab[VHD_loconDFN]);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_loconDFN):

      /* ###--------- locon ---------### */

      locon_pnt = (struct locon *)(current_pnt.data);

      if (locon_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)locon_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_loconDFN;
        }
      if (locon_pnt->SIG != NULL)
        {
        tab[VHD_sigDFN] = avail;
        nxt[VHD_sigDFN] = (void *)locon_pnt->SIG;
        typ[VHD_sigDFN] = VHD_losigDFN;
        }
      if (locon_pnt->ROOT != NULL)
        {
        tab[VHD_rootDFN] = avail;
        nxt[VHD_rootDFN] = (void *)locon_pnt->ROOT;
        if (locon_pnt->TYPE == 'I')
          typ[VHD_rootDFN] = VHD_loinsDFN;
        else
          typ[VHD_rootDFN] = VHD_lofigDFN;
        }
      if (locon_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)locon_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("   name        : %s\n",locon_pnt->NAME);
      (void)printf ("-> sig         : %s\n",tab[VHD_sigDFN]);
      (void)printf ("-> root        : %s\n",tab[VHD_rootDFN]);
      (void)printf ("   direction   : %c\n",locon_pnt->DIRECTION);
      (void)printf ("   type        : %c\n",locon_pnt->TYPE);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_losigDFN):

      /* ###--------- losig ---------### */

      losig_pnt = (struct losig *)(current_pnt.data);

      if (losig_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)losig_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_losigDFN;
        }
      if (losig_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)losig_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      ptchain = losig_pnt->NAMECHAIN;
      (void)printf ("   namechain   :\n");
      while (ptchain != NULL)
        {
        (void)printf ("                 %s\n",(char *)ptchain->DATA);
        ptchain = ptchain->NEXT;
        }

      (void)printf ("   capa        : %f\n",(losig_pnt->PRCN->CAPA?losig_pnt->PRCN->CAPA:0.0) );
      (void)printf ("   index       : %ld\n",losig_pnt->INDEX);
      (void)printf ("   type        : %c\n",losig_pnt->TYPE);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_ptypeDFN):

      /* ###--------- ptype ---------### */

      ptype_pnt = (struct ptype *)(current_pnt.data);

      if (ptype_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)ptype_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_ptypeDFN;
        }
      if (ptype_pnt->DATA != NULL)
        {
        tab[VHD_dataDFN] = avail;
        nxt[VHD_dataDFN] = (void *)ptype_pnt->DATA;
        typ[VHD_dataDFN] = VHD_ERRDFN;
/*......typ[VHD_dataDFN] = .......................*/
        }

      (void)printf ("   type        : %ld\n",ptype_pnt->TYPE);
      (void)printf ("-> data        : %s\n",tab[VHD_dataDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;


      }

   /* ###------------------------------------------------------### */
   /*    Reading new command              */
   /* ###------------------------------------------------------### */

    readcmd = vhd_getcmd (readtab);

    for (i=0 ; i<VHD_MAXDFN ; i++)
      {
      if (readcmd == key[i])
        break;
      }
    if ((i != VHD_MAXDFN+1) && (tab[i] == avail))
      newsta0 = i;
    else
      newsta0 = -1;

   /* ###------------------------------------------------------### */
   /*    Analysing new command               */
   /* ###------------------------------------------------------### */

    switch (newsta0)
      {
      case (VHD__UPDFN):
        current_pnt        = stk[--stk_pnt] ;
        break;

      case (VHD__BKDFN):
        while ((--stk_pnt != 0) && (stk[stk_pnt].mark != 1));
        current_pnt        = stk[stk_pnt] ;
        break;

      case (VHD__SPDFN):
        current_pnt.mark   = 1;
        break;

      case (VHD__TPDFN):
        current_pnt        = stk[0] ;
        stk_pnt        = 0 ;
        break;

      case (VHD__XTDFN):
        current_pnt.data   = NULL;
        current_pnt.type   = VHD__XTDFN;
        current_pnt.mark   = 0;
        break;

      case (VHD_ERRDFN):
        break;

      default:
        stk[stk_pnt++] = current_pnt;
        current_pnt.type   = typ[newsta0];
        current_pnt.mark   = 0;
        current_pnt.data   = nxt[newsta0];
        break;

      }

    state  = current_pnt.type;
    }
  }


static int vhd_getcmd (prvcmd)

char prvcmd[3][20];

  {
  char readstr[60];
  char comd0[20];
  char comd1[20];
  char comd2[20];
  int  code;

  (void)printf ("\n\n\n COMMAND >> ");
  comd0[0] = '\0';
  comd1[0] = '\0';
  comd2[0] = '\0';
  (void)fgets( readstr, 60, stdin );
  (void)sscanf (readstr,"%s%s%s",comd0,comd1,comd2);

  if (strcmp(comd0,"."))
    {
    (void)strcpy (prvcmd[0], comd0);
    (void)strcpy (prvcmd[1], comd1);
    (void)strcpy (prvcmd[2], comd2);
    }
  code = vhd_hash (prvcmd[0]);

  (void)printf ("\n\n\n\n\n");
  return (code);
  }



static int vhd_hash (str)

char *str;

  {
  int code = 0;

  while(*str != '\0')
    code =  code<<1 ^ *str++;

  return (code);
  }

/*##------------------------------------------------------------------##*/
/*  Author : VUONG H.N.                   */
/*  Date   : Dec 11 1991                  */
/*  Cont.  : Essentially functions to duplicate lofig structures  */
/*           such a complete LOFIG or just a chain_list.    */
/*##------------------------------------------------------------------##*/

/*##------------------------------------------------------------------##*/
/*  Function : dupchainlst()                 */
/*  contents : duplicate a chain list and return a pointer on the new   */
/*             structure.                 */
/*  called func. : reverse(), mbkalloc(),          */
/*##------------------------------------------------------------------##*/

chain_list *dupchainlst(chain_ptr)
chain_list *chain_ptr;
  {
  chain_list *chain_rpt = NULL;     /* Returned chain pointer  */

  while(chain_ptr != NULL)
    {
    chain_rpt = addchain(chain_rpt, chain_ptr->DATA);
    chain_ptr     = chain_ptr->NEXT;
    }
  chain_rpt = reverse(chain_rpt);
  return(chain_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duploconlst()                 */
/*  contents : duplicate a locon list and return a pointer on the new   */
/*             structure.                 */
/*  called func. : duplocon(), reverse(),          */
/*##------------------------------------------------------------------##*/

locon_list *duploconlst(locon_ptr)
locon_list *locon_ptr;
  {
  locon_list *locon_rpt = NULL;     /* Returned locon pointer  */
  locon_list *locon_tmpptr = NULL;  /* Temporary pointer       */

  while(locon_ptr != NULL)
    {
    locon_tmpptr       = duplocon(locon_ptr);
    locon_tmpptr->NEXT = locon_rpt;
    locon_rpt          = locon_tmpptr;

    locon_ptr          = locon_ptr->NEXT;
    }
  locon_rpt = (locon_list *)reverse((chain_list *)locon_rpt);
  return(locon_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duplocon()                 */
/*  contents : duplicate a locon and return a pointer on the new  */
/*        structure.                      */
/*  called func. : mbkalloc(),                  */
/*  note : the ROOT and the SIG pointer are also duplicated    */
/*##------------------------------------------------------------------##*/

locon_list *duplocon(locon_ptr)
locon_list *locon_ptr;
  {
  locon_list *locon_rpt = NULL;     /* Returned locon pointer  */
  num_list   *sn;
  ptype_list *ptnodename;

  locon_rpt             = (locon_list *)(mbkalloc(sizeof(locon_list)));
  locon_rpt->NEXT       = NULL;
  locon_rpt->NAME    = locon_ptr->NAME;
  locon_rpt->SIG        = locon_ptr->SIG;
  locon_rpt->ROOT       = locon_ptr->ROOT;
  locon_rpt->DIRECTION  = locon_ptr->DIRECTION;
  locon_rpt->TYPE       = locon_ptr->TYPE;

  locon_rpt->PNODE = NULL;
  for( sn = locon_ptr->PNODE ; sn ; sn = sn->NEXT )
    locon_rpt->PNODE = addnum( locon_rpt->PNODE, sn->DATA );
  locon_rpt->PNODE = (num_list*)reverse( (chain_list*)locon_rpt->PNODE );

  locon_rpt->USER       = NULL;     /* The ptype_list is not duplicated */

  if( ( ptnodename = getptype( locon_ptr->USER, PNODENAME ) ) )
    locon_rpt->USER = addptype( locon_rpt->USER,
                                PNODENAME,
                                dupchainlst( ptnodename->DATA )
                              );

  return(locon_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duploinslst()                 */
/*  contents : duplicate a loins list and return a pointer on the new   */
/*             structure.                 */
/*  called func. : duploins(), reverse(),          */
/*##------------------------------------------------------------------##*/

loins_list *duploinslst(loins_ptr)
loins_list *loins_ptr;
  {
  loins_list *loins_rpt = NULL;     /* Returned loins pointer  */
  loins_list *loins_tmpptr = NULL;  /* Temporary pointer       */

  while(loins_ptr != NULL)
    {
    loins_tmpptr       = duploins(loins_ptr);
    loins_tmpptr->NEXT = loins_rpt;
    loins_rpt          = loins_tmpptr;

    loins_ptr          = loins_ptr->NEXT;
    }
  loins_rpt = (loins_list *)reverse((chain_list *)loins_rpt);
  return(loins_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duploins()                 */
/*  contents : duplicate a loins and return a pointer on the new  */
/*        structure.                      */
/*  called func. : mbkalloc(),                  */
/*  note : the LOCON pointer is also duplicated          */
/*##------------------------------------------------------------------##*/

loins_list *duploins(loins_ptr)
loins_list *loins_ptr;
  {
  loins_list *loins_rpt = NULL;     /* Returned loins pointer  */

  loins_rpt             = (loins_list *)(mbkalloc(sizeof(loins_list)));
  loins_rpt->NEXT       = NULL;
  loins_rpt->LOCON   = loins_ptr->LOCON;
  loins_rpt->INSNAME    = loins_ptr->INSNAME;
  loins_rpt->FIGNAME    = loins_ptr->FIGNAME;
  loins_rpt->USER       = NULL;     /* The ptype_list is not duplicated */

  return(loins_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duplotrslst()                 */
/*  contents : duplicate a lotrs list and return a pointer on the new   */
/*             structure.                 */
/*  called func. : duplotrs(), reverse(),          */
/*##------------------------------------------------------------------##*/

lotrs_list *duplotrslst(lotrs_ptr)
lotrs_list *lotrs_ptr;
  {
  lotrs_list *lotrs_rpt = NULL;     /* Returned lotrs pointer  */
  lotrs_list *lotrs_tmpptr = NULL;  /* Temporary pointer       */

  while(lotrs_ptr != NULL)
    {
    lotrs_tmpptr       = duplotrs(lotrs_ptr);
    lotrs_tmpptr->NEXT = lotrs_rpt;
    lotrs_rpt          = lotrs_tmpptr;

    lotrs_ptr          = lotrs_ptr->NEXT;
    }
  lotrs_rpt = (lotrs_list *)reverse((chain_list *)lotrs_rpt);
  return(lotrs_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duplotrs()                 */
/*  contents : duplicate a lotrs and return a pointer on the new  */
/*        structure.                      */
/*  called func. : mbkalloc(), duplocon()             */
/*  note : the DRAIN, SOURCE, GRID, BULK pointers are also duplicated   */
/*##------------------------------------------------------------------##*/

lotrs_list *duplotrs(lotrs_ptr)
lotrs_list *lotrs_ptr;
  {
  lotrs_list *lotrs_rpt = NULL;     /* Returned lotrs pointer  */

  lotrs_rpt             = (lotrs_list *)mbkalloc(sizeof(lotrs_list));
  lotrs_rpt->NEXT       = NULL;
  lotrs_rpt->TRNAME             = lotrs_ptr->TRNAME;
  lotrs_rpt->DRAIN   = lotrs_ptr->DRAIN;
  lotrs_rpt->GRID       = lotrs_ptr->GRID;
  lotrs_rpt->SOURCE     = lotrs_ptr->SOURCE;
  lotrs_rpt->BULK       = lotrs_ptr->BULK;
  lotrs_rpt->WIDTH      = lotrs_ptr->WIDTH;
  lotrs_rpt->LENGTH     = lotrs_ptr->LENGTH;
  lotrs_rpt->PS         = lotrs_ptr->PS;
  lotrs_rpt->PD         = lotrs_ptr->PD;
  lotrs_rpt->XS         = lotrs_ptr->XS;
  lotrs_rpt->XD         = lotrs_ptr->XD;
  lotrs_rpt->X          = lotrs_ptr->X;
  lotrs_rpt->Y          = lotrs_ptr->Y;
  lotrs_rpt->TYPE       = lotrs_ptr->TYPE;
  lotrs_rpt->USER       = NULL;     /* The ptype_list is not duplicated */

  return(lotrs_rpt);
  }      

/*##------------------------------------------------------------------##*/
/*  Function : duplosiglst()                 */
/*  contents : duplicate a losig_list and return a pointer on the new   */
/*             structure, a pointer on BKSIG must be given.    */
/*  called func. :duplosig(), reverse(),           */
/*##------------------------------------------------------------------##*/

losig_list *duplosiglst(losig_ptr,BKSIG_ptr,sigsize)
losig_list *losig_ptr;
ptype_list **BKSIG_ptr;
int sigsize;
  {
  losig_list *losig_rpt = NULL;     /* Returned losig pointer  */
  losig_list *losig_tmpptr = NULL;  /* Temporary pointer       */
  ptype_list *BKSIG_rpt = NULL;        /* Returned BKSIG pointer  */

  while(losig_ptr != NULL)
    {
    losig_tmpptr       = duplosig(losig_ptr, &BKSIG_rpt, sigsize);
    losig_tmpptr->NEXT = losig_rpt;
    losig_rpt          = losig_tmpptr;

    losig_ptr          = losig_ptr->NEXT;
    }
  losig_rpt = (losig_list *)reverse((chain_list *)losig_rpt);
  *BKSIG_ptr = BKSIG_rpt;
  return(losig_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duplosig()                                               */
/*  contents : duplicate a losig and return a pointer on the new        */
/*             structure, a pointer on the BKSIG must be given.         */
/*  called func. : mbkalloc, addptype, dupchainlst       */
/*##------------------------------------------------------------------##*/

losig_list *duplosig(losig_ptr, BKSIG_ptr, sigsize)
losig_list *losig_ptr;
ptype_list **BKSIG_ptr;
int sigsize;
  {
  ptype_list *ptype_ptr = NULL;
  ptype_list *pt = NULL;
  long index = losig_ptr->INDEX;
  int i;
  int low, high;
  losig_list *ptsig = NULL;
  losig_list *pthead = NULL;

  ptype_ptr = *BKSIG_ptr;
  low = (index % sigsize);
  high = (index / sigsize);
  for (pt = ptype_ptr; pt; pt = pt->NEXT)
    if (pt->TYPE == high)
      break;
  if (pt == NULL) 
    {
    /* Initialization of the array of losig */
    pthead = (losig_list *)mbkalloc(sigsize * sizeof(losig_list));
    ptsig = pthead;
    for (i = 0; i < sigsize; i++) 
      {
      ptsig->INDEX = 0;
      ptsig++;
      }
    pt = ptype_ptr = (ptype_list *)addptype(ptype_ptr,(long)high,(void *)pthead);
    }
  ptsig = (losig_list *)(pt->DATA) + low;

  /* check index  unicity */
  if (ptsig->INDEX != 0L) 
    {
   (void)fflush(stdout);
    (void)fprintf(stderr, "*** mbk error ***\n");
    (void)fprintf(stderr, "duplosig impossible :\n");
    (void)fprintf(stderr, "signal %ld already exist\n", 
           index);
    EXIT(1);
    }

  ptsig->NEXT      = NULL;
  ptsig->NAMECHAIN = dupchainlst(losig_ptr->NAMECHAIN);
  ptsig->PRCN      = NULL;
  ptsig->INDEX     = losig_ptr->INDEX;
  ptsig->USER      = NULL;
  ptsig->PRCN      = NULL;    /* The rc data is not duplicated    */
  ptsig->TYPE      = losig_ptr->TYPE;  /* The ptype_list is not duplicated */

  if (TRACE_MODE == 'Y') {
    (void)fprintf(stdout, "--- mbk --- duplosig : %ld ", index);
    if (ptsig->NAMECHAIN)
      if (ptsig->NAMECHAIN->DATA)
   (void)fprintf(stdout, "name : %s", 
                      (char *)((chain_list *)(ptsig->NAMECHAIN))->DATA);
    (void)fprintf(stdout, "\n");
    }
   
  *BKSIG_ptr = ptype_ptr;
  return ptsig;
  }

/*##------------------------------------------------------------------##*/
/*  Function : duplofiglst()                 */
/*  contents : duplicate a lofig list and return a pointer on the new   */
/*             structure.                 */
/*  called func. : duplofig(), reverse(),          */
/*##------------------------------------------------------------------##*/

lofig_list *duplofiglst(lofig_ptr)
lofig_list *lofig_ptr;
  {
  lofig_list *lofig_rpt = NULL;     /* Returned lofig pointer  */
  lofig_list *lofig_tmpptr = NULL;  /* Temporary pointer       */

  while(lofig_ptr != NULL)
    {
    lofig_tmpptr       = duplofig(lofig_ptr);
    lofig_tmpptr->NEXT = lofig_rpt;
    lofig_rpt          = lofig_tmpptr;

    lofig_ptr          = lofig_ptr->NEXT;
    }
  lofig_rpt = (lofig_list *)reverse((chain_list *)lofig_rpt);
  return(lofig_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duplofig()                 */
/*  contents : duplicate a lofig and return a pointer on the new  */
/*        structure.                      */
/*  called func. : mbkalloc(), dupchainlst(),            */
/*  note : the LOCON, LOSIG, BKSIG, LOINS, LOTRS, LOCAP, LORES, LOSELF pointers are     */
/*         also duplicated                */
/*##------------------------------------------------------------------##*/

lofig_list *duplofig(lofig_ptr)
lofig_list *lofig_ptr;
  {
  lofig_list *lofig_rpt = NULL;     /* Returned lofig pointer  */

  lofig_rpt             = (lofig_list *)(mbkalloc(sizeof(lofig_list)));
  lofig_rpt->BKSIG      = NULL;
  lofig_rpt->USER       = NULL;     /* The ptype_list is not duplicated */
  setsigsize( lofig_rpt, getsigsize( lofig_ptr ));
  lofig_rpt->NEXT       = NULL;
  lofig_rpt->MODELCHAIN = dupchainlst(lofig_ptr->MODELCHAIN);
  lofig_rpt->LOCON      = lofig_ptr->LOCON;
  lofig_rpt->LOSIG      = lofig_ptr->LOSIG;
  lofig_rpt->BKSIG      = lofig_ptr->BKSIG;
  lofig_rpt->LOINS      = lofig_ptr->LOINS;
  lofig_rpt->LOTRS      = lofig_ptr->LOTRS;
  lofig_rpt->LOCAP      = lofig_ptr->LOCAP;
  lofig_rpt->LORES      = lofig_ptr->LORES;
  lofig_rpt->LOSELF     = lofig_ptr->LOSELF;
  lofig_rpt->NAME       = lofig_ptr->NAME;
  lofig_rpt->MODE       = lofig_ptr->MODE;

  return(lofig_rpt);
  }      

/*##------------------------------------------------------------------##*/
/*  Function : rduplofig()                */
/*  contents : recursively duplicate a lofig and return a pointer on    */
/*             the new structure. This structure is the entirely  */
/*             double of the lofig duplicated. Except, that all the  */
/*             USER fields are empty.                 */
/*  called func. : mbkalloc(),                  */
/*##------------------------------------------------------------------##*/

lofig_list *rduplofig(lofig_ptr)
lofig_list *lofig_ptr;
  {
  lofig_list *lofig_rpt = NULL;     /* Returned lofig pointer  */
  ptype_list *ptype_ptr = NULL;
  locon_list *locon_pt = NULL;
  loins_list *loins_pt = NULL;
  lotrs_list *lotrs_pt = NULL;
  locap_list *locap_pt = NULL;
  lores_list *lores_pt = NULL;
  loself_list *loself_pt = NULL;
  losig_list *losig_pt = NULL;
  chain_list *headctc;
  chain_list *scanchain;
  loctc_list *ptctc;
  long index = 0;
  
  lofig_rpt             = duplofig(lofig_ptr);
  lofig_rpt->LOSIG      = duplosiglst(lofig_rpt->LOSIG, &ptype_ptr,getsigsize(lofig_rpt));
  lofig_rpt->BKSIG      = ptype_ptr;
  lofig_rpt->LOCON      = duploconlst(lofig_rpt->LOCON);
  lofig_rpt->LOINS      = duploinslst(lofig_rpt->LOINS);
  lofig_rpt->LOTRS      = duplotrslst(lofig_rpt->LOTRS);

  lofig_rpt->LOCAP      = duplocaplst(lofig_rpt->LOCAP);
  lofig_rpt->LORES      = duploreslst(lofig_rpt->LORES);
  lofig_rpt->LOSELF     = duploselflst(lofig_rpt->LOSELF);

  /* Now verify and restitute the coherence of the figure   */

  /* Restitute coherence in the LOTRS List */ 
  lotrs_pt = lofig_rpt->LOTRS;
  while(lotrs_pt != NULL)
    {
    lotrs_pt->GRID   = duplocon(lotrs_pt->GRID);
    locon_pt = lotrs_pt->GRID;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)lotrs_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    lotrs_pt->DRAIN  = duplocon(lotrs_pt->DRAIN);
    locon_pt = lotrs_pt->DRAIN;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)lotrs_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    lotrs_pt->SOURCE    = duplocon(lotrs_pt->SOURCE);
    locon_pt = lotrs_pt->SOURCE;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)lotrs_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    lotrs_pt->BULK   = duplocon(lotrs_pt->BULK);
    locon_pt = lotrs_pt->BULK;
    while(locon_pt != NULL)
      {
      if( locon_pt->SIG )
        {
        index     = (locon_pt->SIG)->INDEX;
        locon_pt->SIG   = getlosig(lofig_rpt, index);
   }
      else
        {
   locon_pt->SIG   = NULL;
   }
      locon_pt->ROOT = (void *)lotrs_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    lotrs_pt = lotrs_pt->NEXT;
    }


  /* Restitute coherence in the LOCAP List */ 
  locap_pt = lofig_rpt->LOCAP;
  while(locap_pt != NULL)
    {
    locap_pt->TCON   = duplocon(locap_pt->TCON);
    locon_pt = locap_pt->TCON;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)locap_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    locap_pt->BCON  = duplocon(locap_pt->BCON);
    locon_pt = locap_pt->BCON;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)locap_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    while(locon_pt != NULL)
      {
      if( locon_pt->SIG )
        {
        index     = (locon_pt->SIG)->INDEX;
        locon_pt->SIG   = getlosig(lofig_rpt, index);
   }
      else
        {
   locon_pt->SIG   = NULL;
   }
      locon_pt->ROOT = (void *)locap_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    locap_pt = locap_pt->NEXT;
    }


  /* Restitute coherence in the LORES List */ 
  lores_pt = lofig_rpt->LORES;
  while(lores_pt != NULL)
    {
    lores_pt->RCON1   = duplocon(lores_pt->RCON1);
    locon_pt = lores_pt->RCON1;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)lores_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    lores_pt->RCON2  = duplocon(lores_pt->RCON2);
    locon_pt = lores_pt->RCON2;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)lores_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    while(locon_pt != NULL)
      {
      if( locon_pt->SIG )
        {
        index     = (locon_pt->SIG)->INDEX;
        locon_pt->SIG   = getlosig(lofig_rpt, index);
   }
      else
        {
   locon_pt->SIG   = NULL;
   }
      locon_pt->ROOT = (void *)lores_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    lores_pt = lores_pt->NEXT;
    }


  /* Selftitute coherence in the LOSELF List */ 
  loself_pt = lofig_rpt->LOSELF;
  while(loself_pt != NULL)
    {
    loself_pt->SCON1   = duplocon(loself_pt->SCON1);
    locon_pt = loself_pt->SCON1;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)loself_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    loself_pt->SCON2  = duplocon(loself_pt->SCON2);
    locon_pt = loself_pt->SCON2;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)loself_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    while(locon_pt != NULL)
      {
      if( locon_pt->SIG )
        {
        index     = (locon_pt->SIG)->INDEX;
        locon_pt->SIG   = getlosig(lofig_rpt, index);
   }
      else
        {
   locon_pt->SIG   = NULL;
   }
      locon_pt->ROOT = (void *)loself_pt;
      
      locon_pt = locon_pt->NEXT;
      }

    loself_pt = loself_pt->NEXT;
    }



  /* Restitute coherence in the LOINS List */ 
  loins_pt = lofig_rpt->LOINS;
  while(loins_pt != NULL)
    {
    loins_pt->LOCON  = duploconlst(loins_pt->LOCON);
    locon_pt = loins_pt->LOCON;
    while(locon_pt != NULL)
      {
      index       = (locon_pt->SIG)->INDEX;
      locon_pt->SIG  = getlosig(lofig_rpt, index);
      locon_pt->ROOT = (void *)loins_pt;
      
      locon_pt = locon_pt->NEXT;
      }
    loins_pt = loins_pt->NEXT;
    }

  /* Restitute coherence in the LOCON List */ 
  locon_pt = lofig_rpt->LOCON;
  while(locon_pt != NULL)
    {
    index      = (locon_pt->SIG)->INDEX;
    locon_pt->SIG    = getlosig(lofig_rpt, index);
    locon_pt->ROOT   = (void *)lofig_rpt;
    
    locon_pt = locon_pt->NEXT;
    }
  
  /* Duplicate rc data */
  for( losig_pt = lofig_ptr->LOSIG ; losig_pt ; losig_pt = losig_pt->NEXT )
     duplorcnet( getlosig( lofig_rpt, losig_pt->INDEX ), losig_pt );

  headctc = getallctc( lofig_ptr );
  for( scanchain = headctc ; scanchain ; scanchain = scanchain->NEXT )
  {
    ptctc = (loctc_list*)scanchain->DATA;
    addloctc( getlosig( lofig_rpt, ptctc->SIG1->INDEX ), ptctc->NODE1,
              getlosig( lofig_rpt, ptctc->SIG2->INDEX ), ptctc->NODE2,
              ptctc->CAPA );
  }
  freechain( headctc );
  
  return(lofig_rpt);
  }

/* sort routines :
   connectors and signals may have to be sorted by name for some
   language purposes. */

static char Buffer0[255]; /* may be of use for other functions */

static int
   connectorcmp(flc, slc)
locon_list **flc, **slc;
{
   return naturalstrcmp((*slc)->NAME, (*flc)->NAME);
}

void
   sortlocon(connectors)
locon_list **connectors;
{
long n, i;
locon_list *lc, **tc;

   if (!connectors || !(*connectors)) 
      return;

   /* first :
      count connectors. */
   for (n = 0, lc = *connectors; lc; lc = lc->NEXT)
      n++;
   tc = (locon_list **)mbkalloc(n * sizeof(locon_list *));
   for (n = 0, lc = *connectors; lc; lc = lc->NEXT)
      tc[n++] = lc;
   /* second :
      sort them. */
   qsort((void *)tc, n, sizeof(locon_list *), connectorcmp);
   /* tables :
      the last element of the table is t[n - 1].
      relink the list in the sorted order. */
   n--;
   *connectors = tc[0];
   for (i = 0; i < n; i++)
      tc[i]->NEXT = tc[i + 1];
   tc[n]->NEXT = (locon_list *)NULL;
   mbkfree((void *)tc);
}

static int
   signalcmp(fls, sls)
losig_list **fls, **sls;
{
   return naturalstrcmp(getsigname(*fls), getsigname(*sls));
}

void
   sortlosig(signals)
losig_list **signals;
{
long n, i;
losig_list *lc, **tc;

   if (!signals || !(*signals)) 
      return;

   /* first :
      count signals. */
   for (n = 0, lc = *signals; lc; lc = lc->NEXT)
      n++;
   tc = (losig_list **)mbkalloc(n * sizeof(losig_list *));
   for (n = 0, lc = *signals; lc; lc = lc->NEXT)
      tc[n++] = lc;
   /* second :
      sort them. */
   qsort((void *)tc, n, sizeof(losig_list *), signalcmp);
   /* tables :
      the last element of the table is t[n - 1].
      relink the list in the sorted order. */
   n--;
   *signals = tc[0];
   for (i = 0; i < n; i++)
      tc[i]->NEXT = tc[i + 1];
   tc[n]->NEXT = (losig_list *)NULL;
   mbkfree((void *)tc);
}

/* checking :
   the validity of a list of connectors for vectorization purposes
   is to be checked for some file formats, so let's do it.
   The algorithm checks that :
      vectorized radical do not appear on themselves
      vectorized connectors are all together and ordered properly */
#define HTSIZE 200
int
   checkloconorder(c)
locon_list *c;
{
char *s=NULL, *t=NULL;
ht *table;
int first = 1, previous=0, delta=0, current=0;

   table = addht(HTSIZE);
   while (c) {
      /* vectorized ?
         no. */
      if ((s = strchr(c->NAME, ' ')) == NULL) {
avoid_redundancy :
         if (sethtitem(table, (void *)c->NAME, 0)) {
            (void)fflush(stdout);
            (void)fprintf(stderr, "*** mbk error ***\n");
            (void)fprintf(stderr,
                           "the radical %s is already used in a vector\n",
                           c->NAME);
            delht(table);
            EXIT(1);
            return 1;
         }
         first = 1;
         c = c->NEXT;
      } else {
         while (c) {
         int i = 1;
            /* redundancy :
               I'ven't found an elegant way to do that, too bad. */
            if (!s)
               if ((s = strchr(c->NAME, ' ')) == NULL)
                  goto avoid_redundancy;
            /* Temporary change :
               alter the string contents just a microsecond. */
            *s = '\0';
            strcpy(Buffer0, c->NAME);
            *s = ' ';
            while (*(s + i) && isdigit((int)*(s + i)))
               i++;
            if (*(s + i)) {
               (void)fflush(stdout);
               (void)fprintf(stderr, "*** mbk error ***\n");
               (void)fprintf(stderr,
                     "the radical %s has a spurious vectorized value %s (%s)\n",
                     Buffer0, s + 1, c->NAME);
               delht(table);
               EXIT(1);
               return 1;
            }
            current = atoi(s + 1);
            s = NULL;
            /* first :
               the name is not yet known. so I store it.
               we do that each time a new vector is encountered. */
            if (first) {
its_first:
               if (sethtitem(table, (void *)(t = namealloc(Buffer0)), 0)) {
                  (void)fflush(stdout);
                  (void)fprintf(stderr, "*** mbk error ***\n");
                  (void)fprintf(stderr,
                                 "the radical %s is already used in a vector\n",
                                 Buffer0);
                  delht(table);
                  EXIT(1);
                  return 1;
               }
               delta = first = 0;
               previous = current;
               c = c->NEXT;
            } else { /* let's see if it's a good vhdl legal vector */
               /* its a new vector :
                  in fact this vector follows an other vector! */
               if (t != namealloc(Buffer0))
                  goto its_first;
               if (!delta)
                  delta = previous - current;
               else if (delta != previous - current) {
                  (void)fflush(stdout);
                  (void)fprintf(stderr, "*** mbk error ***\n");
                  (void)fprintf(stderr,
                              "the radical %s is not vectorized properly,",
                              Buffer0);
                  (void)fprintf(stderr,
                              " %s %d follows %s %d\n", Buffer0, previous,
                                                         Buffer0, current);
                  delht(table);
                  EXIT(1);
                  return 1;
               }
               if (delta != 1 && delta != -1) {
                  (void)fflush(stdout);
                  (void)fprintf(stderr, "*** mbk error ***\n");
                  (void)fprintf(stderr,
                              "the radical %s is not vectorized properly,",
                              c->NAME);
                  (void)fprintf(stderr,
                              " %s %d follows %s %d\n", Buffer0, previous,
                                                         Buffer0, current);
                  delht(table);
                  EXIT(1);
                  return 1;
               }
               previous = current;
               c = c->NEXT;
            }
         }
      }
   }
   delht(table);
   return 0;
}

/*************************** Analogical world ***************************************/

/*##------------------------------------------------------------------##*/
/*  Function : duplocaplst()                 */
/*  contents : duplicate a locap list and return a pointer on the new   */
/*             structure.                 */
/*  called func. : duplocap(), reverse(),          */
/*##------------------------------------------------------------------##*/

locap_list *duplocaplst(locap_ptr)
locap_list *locap_ptr;
  {
  locap_list *locap_rpt = NULL;     /* Returned locap pointer  */
  locap_list *locap_tmpptr = NULL;  /* Temporary pointer       */

  while(locap_ptr != NULL)
    {
    locap_tmpptr       = duplocap(locap_ptr);
    locap_tmpptr->NEXT = locap_rpt;
    locap_rpt          = locap_tmpptr;

    locap_ptr          = locap_ptr->NEXT;
    }
  locap_rpt = (locap_list *)reverse((chain_list *)locap_rpt);
  return(locap_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duplocap()                 */
/*  contents : duplicate a locap and return a pointer on the new  */
/*        structure.                      */
/*  called func. : mbkalloc(), duplocon()             */
/*  note : the TCON, BCON pointers are also duplicated   */
/*##------------------------------------------------------------------##*/

locap_list *duplocap(locap_ptr)
locap_list *locap_ptr;
  {
  locap_list *locap_rpt = NULL;     /* Returned locap pointer  */

  locap_rpt             = (locap_list *)mbkalloc(sizeof(locap_list));
  locap_rpt->NEXT       = NULL;
  locap_rpt->NAME       = locap_ptr->NAME;
  locap_rpt->TCON       = locap_ptr->TCON;
  locap_rpt->BCON       = locap_ptr->BCON;
  locap_rpt->TYPE       = locap_ptr->TYPE;
  locap_rpt->CAPA       = locap_ptr->CAPA;
  locap_rpt->USER       = NULL;     /* The ptype_list is not duplicated */

  return(locap_rpt);
  }      


/*##------------------------------------------------------------------##*/
/*  Function : duploreslst()                 */
/*  contents : duplicate a lores list and return a pointer on the new   */
/*             structure.                 */
/*  called func. : duplores(), reverse(),          */
/*##------------------------------------------------------------------##*/

lores_list *duploreslst(lores_ptr)
lores_list *lores_ptr;
  {
  lores_list *lores_rpt = NULL;     /* Returned lores pointer  */
  lores_list *lores_tmpptr = NULL;  /* Temporary pointer       */

  while(lores_ptr != NULL)
    {
    lores_tmpptr       = duplores(lores_ptr);
    lores_tmpptr->NEXT = lores_rpt;
    lores_rpt          = lores_tmpptr;

    lores_ptr          = lores_ptr->NEXT;
    }
  lores_rpt = (lores_list *)reverse((chain_list *)lores_rpt);
  return(lores_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duplores()                 */
/*  contents : duplicate a lores and return a pointer on the new  */
/*        structure.                      */
/*  called func. : mbkalloc(), duplocon()             */
/*  note : the RCON1, RCON2 pointers are also duplicated   */
/*##------------------------------------------------------------------##*/

lores_list *duplores(lores_ptr)
lores_list *lores_ptr;
  {
  lores_list *lores_rpt = NULL;     /* Returned lores pointer  */

  lores_rpt             = (lores_list *)mbkalloc(sizeof(lores_list));
  lores_rpt->NEXT       = NULL;
  lores_rpt->NAME       = lores_ptr->NAME;
  lores_rpt->RCON1      = lores_ptr->RCON1;
  lores_rpt->RCON2      = lores_ptr->RCON2;
  lores_rpt->TYPE       = lores_ptr->TYPE;
  lores_rpt->RESI       = lores_ptr->RESI;
  lores_rpt->USER       = NULL;     /* The ptype_list is not duplicated */

  return(lores_rpt);
  }      


/*##------------------------------------------------------------------##*/
/*  Function : duploselflst()                 */
/*  contents : duplicate a loself list and return a pointer on the new   */
/*             structure.                 */
/*  called func. : duploself(), reverse(),          */
/*##------------------------------------------------------------------##*/

loself_list *duploselflst(loself_ptr)
loself_list *loself_ptr;
  {
  loself_list *loself_rpt = NULL;     /* Returned loself pointer  */
  loself_list *loself_tmpptr = NULL;  /* Temporary pointer       */

  while(loself_ptr != NULL)
    {
    loself_tmpptr       = duploself(loself_ptr);
    loself_tmpptr->NEXT = loself_rpt;
    loself_rpt          = loself_tmpptr;

    loself_ptr          = loself_ptr->NEXT;
    }
  loself_rpt = (loself_list *)reverse((chain_list *)loself_rpt);
  return(loself_rpt);
  }

/*##------------------------------------------------------------------##*/
/*  Function : duploself()                 */
/*  contents : duplicate a loself and return a pointer on the new  */
/*        structure.                      */
/*  called func. : mbkalloc(), duplocon()             */
/*  note : the RCON1, RCON2 pointers are also duplicated   */
/*##------------------------------------------------------------------##*/

loself_list *duploself(loself_ptr)
loself_list *loself_ptr;
  {
  loself_list *loself_rpt = NULL;     /* Returned loself pointer  */

  loself_rpt             = (loself_list *)mbkalloc(sizeof(loself_list));
  loself_rpt->NEXT       = NULL;
  loself_rpt->NAME       = loself_ptr->NAME;
  loself_rpt->SCON1      = loself_ptr->SCON1;
  loself_rpt->SCON2      = loself_ptr->SCON2;
  loself_rpt->TYPE       = loself_ptr->TYPE;
  loself_rpt->SELF       = loself_ptr->SELF;
  loself_rpt->USER       = NULL;     /* The ptype_list is not duplicated */

  return(loself_rpt);
  }      





