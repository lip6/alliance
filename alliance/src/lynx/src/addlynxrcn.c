/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*************************************************************
*                  SOURCES                                   *
*                                                            *
* Changed by:    Greg                                        *
* Date:          05/07/95                                    *
* New file:      -                                           *
* Comments:                                                  *
*   Fonctions permettant d'ajouter un nouveau reseau dans    *
*   la figure logique existante.                             *
*                                                            *
*                                                            *
*************************************************************/

# include "mut.h"
# include "rds.h"
# include "mlo.h"
# include "rcn.h"
# include "mph.h"
# include "rpr.h"
# include "extract.h"
# include "lynxrcn.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/timeb.h>
# include "addlynxrcn.h"
# include "Windows95.h"
# include "inter.h"
# include "mbkrds.h"
# include "chklynxrcn.h"
# include "buildrcn.h"
# include "via.h"

# include <math.h>

/*************************************************************
* La variable rcnlynxdebug doit etre fixee a 0 pour NE PAS   *
* avoir d'affichage sur la sortie STDERR.                    *
*                                                            *
*                                                            *
*************************************************************/

short rcnlynxdebug=0;
short rcnlynxchk=0;

long GrgNodeINDEX=1L;

/*************************************************************
*                                                            *
* Fonctions utilisees en interne par le programme.           *
*                                                            *
*                                                            *
*************************************************************/

void GrgConPh2ConLo __P((locon_list*, long));
void GrgDualConLoPh __P((lofig_list*));
void GrgGroupVia __P((lofig_list*));
void SigMark __P(( lofig_list*));

/*************************************************************
* SOURCES                                                    *
*                                                            *
*                                                            *
*                                                            *
*************************************************************/

void generate_RCN(fig)
lofig_list *fig;

/*************************************************************

En entree, les rds doivent avoir dans leur champ user une ptype
normalement constituee. Les connecteurs logiques DOIVENT etre
relies aux connecteurs physiques grace a une liste chainee
accessible par un ptype du champ USER des locons. Les valeurs
que doit prendre l'identificateur de la ptype est definie
dans le fichier 'lynxrcn.h'.
A un connecteur physique, il n'y aura qu'un seul connecteur
logique attache, tandis qu'a un connecteur logique, il peut y
avoir plusieurs connecteurs physiques (ex des instances).
Pour les transistors, il n'y a qu'un connecteur physique par
connecteur logique.

*************************************************************/

{
  losig_list    *ScanSignal;    

  fflush(stdout);
  fflush(stderr);
  
  SigMark( fig);

  if(rcnlynxdebug)
  {
    fflush(stderr);
    fprintf(stderr,"\n\n--------- DEBUT DE LA FONCTION generate_RCN -----------\n");
    fflush(stderr);
  }

  printf("\t---> Net extract is running...\n"); fflush(stdout);

  if(rcnlynxdebug)
  {
    fprintf(stderr,"Figure d'entree\n");
    Grgdisplay_datain(fig);
    fprintf(stderr,"____________________________________________\n");
    fflush(stderr);
  }

  if(rcnlynxchk)
  {
    if(rcnlynxdebug)
      fprintf(stderr,
	      "Verification de la conformite des structures d'entree...\n");
    GrgChkInConFEqui(fig);
  }

  /*
  Ajoute pour chaque rectangle representant un connecteur un pointeur vers
  le connecteur logique correspondant. Ce pointeur est accessible par
  la valeur RDSLOCON
  */

  GrgDualConLoPh(fig);

  /*
  Permet de retrouver a partir d'un rectangle constituant un via les deux
  autres rectangles constituant ce via. Le pointeur est accessible par la
  valeur GRGVIA
  */
  GrgGroupVia(fig);

  if(rcnlynxchk)
  {
    if(rcnlynxdebug)
      fprintf(stderr,"Verification de la conformite des structures de travail...\n");
    GrgChkOutConVia(fig,rcnlynxdebug);
  }
  
  if(rcnlynxdebug)
  {
    fprintf(stderr,"Figure apres liaisons connecteur physique-logique\n");
    Grgdisplay_datain(fig);
    fprintf(stderr,"____________________________________________\n");
    fflush(stderr);
  }

  for(ScanSignal=fig->LOSIG; ScanSignal; ScanSignal=ScanSignal->NEXT)
  {
    /* Remise a un du compteur de noeud pour chaque Signal */

    GrgNodeINDEX=1L;

    buildrcn(fig,ScanSignal);

/*
    if(rcnlynxchk)
    {
      if(!rcn_check(getlorcnet(ScanSignal),0))
        printf("RCN error on signal %ld.\n",ScanSignal->INDEX);
    }
*/
  }


  if(rcnlynxdebug)
  {
    fprintf(stderr,"-------- SORTIE DE LA FONCTION generate_RCN -----------\n");
  }

  fflush(stderr);
  fflush(stdout);

}

void GrgDualConLoPh(fig)
lofig_list      *fig;
/*************************************************************
* Creer la relation connecteur physique vers connecteur      *
* logique a partir de la relation connecteur logique vers    *
* connecteur logique existante.                              *
*                                                            *
*************************************************************/
{
  loins_list    *ScanLoIns;     /* Pour parcourir les instances logiques                */
  lotrs_list    *ScanTrs;       /* Pour parcourir les transistors logiques              */

  if(rcnlynxdebug)
  {
    fprintf(stderr,"\nPlace dans le champ USER des rectangles connecteurs\n");
    fprintf(stderr,"le connecteur logique lui correspondant...\n");
  }

  if(rcnlynxdebug)
    fprintf(stderr,"  Traitement des connecteurs externes de la figure...\n");
  GrgConPh2ConLo(fig->LOCON,RDSLOCON1);
  
  if(rcnlynxdebug)
    fprintf(stderr,"  Traitement des connecteurs des instances...\n");
  for(ScanLoIns=fig->LOINS;ScanLoIns;ScanLoIns=ScanLoIns->NEXT)
  {
    GrgConPh2ConLo(ScanLoIns->LOCON,RDSLOCON2);
  }

  if(rcnlynxdebug)
    fprintf(stderr,"  Traitement des connecteurs des transistors...\n");
  for(ScanTrs=fig->LOTRS;ScanTrs;ScanTrs=ScanTrs->NEXT)
  {
    GrgConPh2ConLo(ScanTrs->DRAIN,RDSLOCON3);
    GrgConPh2ConLo(ScanTrs->GRID,RDSLOCON3);
    GrgConPh2ConLo(ScanTrs->SOURCE,RDSLOCON3);
    GrgConPh2ConLo(ScanTrs->BULK,RDSLOCON3);
  }
}





void GrgConPh2ConLo(connector,identificateur)
locon_list      *connector;     /*Structure classique des connecteurs chaines */
long             identificateur;

/*************************************************************
Cette fonction ajoute un ptype dans le champ USER des rectangles
connecteurs qui pointe sur le connecteur logique lui
correspondant. Les parametres de cette fonction sont:
-Une locon_list des connecteurs possedant un ptype repere par
 'identificateur' pointant sur une liste chainee contenant les
 adresses des rdsrec_list connecteur.
-Un identificateur defini dans 'lynxrcn.h'
*************************************************************/

{
  locon_list *ScanLoCon;
  chain_list *ScanChain;        /* liste chainee des rectangles connecteur */
  rdsrec_list *Rec_a;
  ptype_list  *ptl;

  for(ScanLoCon=connector;ScanLoCon;ScanLoCon=ScanLoCon->NEXT)
  {
    ptl = getptype( ScanLoCon->USER, identificateur );
    if( !ptl )
      continue;
    for(ScanChain=((chain_list*)(ptl->DATA));
	ScanChain;
	ScanChain=ScanChain->NEXT)
    {
      Rec_a=((rdsrec_list*)(ScanChain->DATA));
      ptl = getptype( Rec_a->USER, RDSLOCON );
      if( ptl )
        printf( "***LYNX RCN : ptype RDSLOCON found on rectangle.\n" );

      Rec_a->USER=addptype(Rec_a->USER,RDSLOCON,ScanLoCon);

    }
  }
}



void GrgFatalError(code)
int code;
{
  fprintf(stderr,"\n\n********************************************\n");
  fprintf(stderr,"*                                          *\n");
  fprintf(stderr,"*    ERREUR DANS LES STRUCTURES DE DONNEE  *\n");
  fprintf(stderr,"*                                          *\n");
  fprintf(stderr,"********************************************\n\n");

  fprintf(stderr,"Code d'erreur: %d\n",code);
  exit(code);
}



int IsRdsRealVia(r)
rdsrec_list *r;
{
  
  if(IsRdsVia(r))
  {
    if(GetLynxAttribute(r)!= C_X_N && GetLynxAttribute(r)!=C_X_P)
      return(1);
  } 
  return(0);
}

void GrgGroupVia( lofig_list *fig)
{
  losig_list    *ScanLoSig;
  chain_list    *Vias;
  chain_list    *scanchain;
  rdsrec_list   *ScanPhRec;
  int            EndEqui;
  rdsrec_list   *scanvia;
  ptype_list    *ptl;

  for(ScanLoSig=fig->LOSIG; ScanLoSig; ScanLoSig=ScanLoSig->NEXT)
  {
    Vias=NULL;

    for( scanchain = (chain_list*)(getptype(ScanLoSig->USER,RCN_FEQUI)->DATA) ;
         scanchain ;
         scanchain = scanchain->NEXT
       )
    {
      ScanPhRec = (rdsrec_list*)(scanchain->DATA); 
      EndEqui   = 0 ;

      while(!EndEqui)
      {
        if( IsRdsRealVia(ScanPhRec)                                 &&
            (ScanPhRec->FLAGS & RCN_TAGVIA_MASK) != RCN_TAGVIA_MASK 
          )
        {
          Vias=addchain(Vias,ScanPhRec);
          scanvia=ScanPhRec;
          do
          {
            ptl = getptype( scanvia->USER, RCN_SIG_PTR );
            if( !ptl )
            {
              scanvia = LYNX_LINK( scanvia );
              continue;
            }
            if( ptl->DATA == ScanLoSig )
            {
              scanvia->FLAGS = scanvia->FLAGS | RCN_TAGVIA_MASK;
            }
            scanvia = LYNX_LINK( scanvia );
          }
          while( scanvia != ScanPhRec );
        }

        if(IsLynxEndEqui(ScanPhRec))
    	  EndEqui=1;
        else
  	  ScanPhRec=LYNX_EQUI(ScanPhRec);
      }
    }

    ScanLoSig->USER = addptype( ScanLoSig->USER, GRGVIA, Vias );
  }
}

void SigMark( lofig_list *fig)
{
  losig_list    *ScanLoSig;
  chain_list    *scanchain;
  rdsrec_list   *ScanPhRec;
  int            EndEqui;

  for(ScanLoSig=fig->LOSIG; ScanLoSig; ScanLoSig=ScanLoSig->NEXT)
  {
    for( scanchain = (chain_list*)(getptype(ScanLoSig->USER,RCN_FEQUI)->DATA) ;
         scanchain ;
         scanchain = scanchain->NEXT
       )
    {
      ScanPhRec = (rdsrec_list*)(scanchain->DATA); 
      EndEqui   = 0 ;

      while(!EndEqui)
      {
        ScanPhRec->USER = addptype( ScanPhRec->USER, RCN_SIG_PTR, ScanLoSig );
        if(IsLynxEndEqui(ScanPhRec))
    	  EndEqui=1;
        else
  	  ScanPhRec=LYNX_EQUI(ScanPhRec);
      }
    }
  }
}
