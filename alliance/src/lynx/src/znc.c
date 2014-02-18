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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

#include "mut.h"
#include "mlo.h"
#include "rcn.h"
#include "rds.h"
#include "lynxrcn.h"
#include "Windows95.h"
#include "znc.h"

znc	*HEAD_FREE_ZNC;
#define BUFZNCSIZE 5
/*
#define ZNCDEBUG
*/
void freeznc __P((znc*));			/* Libere une structure znc. Cette structure
				   est remise dans la liste des structures
				   libres */
znc* getznc __P((void));			/* Recupere une structure znc par allocation
				   par tas */
void addwinznc __P((znc*, WinZnc*));		/* Cette fonction ajoute une znc dans le 
				   fenetrage prevus a cet effet */
void delwinznc __P((znc*,WinZnc*));		/* Efface un znc du fenetrage */
long getmx __P((znc*,WinZnc*));
long getmy __P((znc*,WinZnc*));
long getMx __P((znc*,WinZnc*));
long getMy __P((znc*,WinZnc*));

long addznc(x1,y1,x2,y2,layer,winznc)
long		 x1;
long		 y1;
long		 x2;
long		 y2;
long		 layer;
WinZnc		*winznc;
{
  chain_list	*interznc,*schinter;
  znc		*nouveau,*ancien,*recouvre;
  znc		 last;

  nouveau=getznc();
  nouveau->x1=x1;
  nouveau->x2=x2;
  nouveau->y1=y1;
  nouveau->y2=y2;
  nouveau->layer=layer;

#ifdef ZNCDEBUG
fprintf(stderr,"*** addznc ***\n");
fprintf(stderr,"Zone ( %ld; %ld)-( %ld; %ld) sur layer %ld \n",x1,y1,x2,y2,nouveau->layer);
#endif

  do
  {
    last=*nouveau;

    interznc=getwininterznc(nouveau,winznc,1);

    for(schinter=interznc;schinter;schinter=schinter->NEXT)
    {
      recouvre=(znc*)(schinter->DATA);

      if(recouvre->x1 < nouveau->x1) nouveau->x1=recouvre->x1;
      if(recouvre->y1 < nouveau->y1) nouveau->y1=recouvre->y1;
      if(recouvre->x2 > nouveau->x2) nouveau->x2=recouvre->x2;
      if(recouvre->y2 > nouveau->y2) nouveau->y2=recouvre->y2;
    }

#ifdef ZNCDEBUG
fprintf(stderr,"Nouvelle znc apres recouvrements\n");
fprintf(stderr,"Zone ( %ld; %ld)-( %ld; %ld) sur layer %ld \n",nouveau->x1,nouveau->y1,nouveau->x2,nouveau->y2,nouveau->layer);
fprintf(stderr,"\n\nAjout de la nouvelle zone dans le fenetrage\n");
#endif


#ifdef ZNCDEBUG
fprintf(stderr,"\nEffacement eventuel des zones recouvrantes\n\n");
#endif

    if(interznc)
    {
      ancien=NULL;
      for(schinter=interznc;schinter;schinter=schinter->NEXT)
      {
        if(ancien)
        {
          delwinznc(ancien,winznc);
          freeznc(ancien);
        }
        ancien=(znc*)(schinter->DATA);
      }

      if(ancien)
      {
        delwinznc(ancien,winznc);
        freeznc(ancien);
      }
      freechain(interznc);
    }
  }
  while(last.x1!=nouveau->x1 ||
        last.y1!=nouveau->y1 ||
        last.x2!=nouveau->x2 ||
        last.y2!=nouveau->y2   );

  /* Attention lors de l'utilisation du connect : il ne devrait plus y
     avoir de recouvrement sur cette znc. L'utilisateur doit prendre ces
     precautions */

  addwinznc(nouveau,winznc);  
  return(nouveau->node);
}

znc* getznc()
{
  int	 i;
  znc	*newznc;

  if(HEAD_FREE_ZNC==NULL)
  {
    HEAD_FREE_ZNC=(znc*)mbkalloc(sizeof(znc)*BUFZNCSIZE);
    newznc=HEAD_FREE_ZNC;
    for(i=1;i<BUFZNCSIZE;i++)
    {
      newznc->NEXT=newznc+1;
      newznc++;
    }
    newznc->NEXT=NULL;
  }
  newznc=HEAD_FREE_ZNC;
  HEAD_FREE_ZNC=HEAD_FREE_ZNC->NEXT;
  newznc->NEXT=NULL;
  newznc->Win=NULL;
  newznc->rec=NULL;
  newznc->node=0;
  return(newznc);
}

void freeznc(z)
znc *z;
{
  if(z->rec) freechain(z->rec);
  if(z->Win) freechain(z->Win);
  z->NEXT=HEAD_FREE_ZNC;
  HEAD_FREE_ZNC=z;
}

void addwinznc(elem,wininfo)
znc		*elem;
WinZnc		*wininfo;
{
  long		 x1,y1,x2,y2;
  long		 nx1,ny1;
  chain_list	*ptchain;


  x1=getmx(elem,wininfo);
  y1=getmy(elem,wininfo);
  x2=getMx(elem,wininfo);
  y2=getMy(elem,wininfo);

  /* Ajout de la znc dans les ChZnc qui vont bien */

  for(nx1=x1;nx1<=x2;nx1++)
  {
    for(ny1=y1;ny1<=y2;ny1++)
    {
      /* Ajoute l'element dans le fenetrage */
#ifdef ZNCDEBUG
fprintf(stderr,"Ajout de l'element dans la fenetre %ld\n",ny1*wininfo->nx + nx1);
#endif

      ptchain=*(wininfo->ChZnc + ny1*wininfo->nx + nx1);
      ptchain=addchain(ptchain,elem);
      *(wininfo->ChZnc + ny1*wininfo->nx + nx1)=ptchain;

      /* Fait pointer cet element vers ses fenetrages */

     elem->Win=addchain(elem->Win,wininfo->ChZnc + ny1*wininfo->nx + nx1);
    }
  } 
}

chain_list* getwininterznc(elem,wininfo,chklayer)
znc	*elem;
WinZnc	*wininfo;
short	 chklayer;	/* si chklayer != 0, alors une verification de layer
			   est effectuee */
{
  chain_list	*fenetre;
  chain_list	*candidats;
  znc		*zonecandidate;
  chain_list	*elus,*cleanelus;
  long		 x1,y1,x2,y2,x,y;

  elus=NULL;
#ifdef ZNCDEBUG
fprintf(stderr,"Debut de recherche des intersections\n");
#endif

  x1=getmx(elem,wininfo);
  y1=getmy(elem,wininfo);
  x2=getMx(elem,wininfo);
  y2=getMy(elem,wininfo);

  for(x=x1;x<=x2;x++)
  {
    for(y=y1;y<=y2;y++)
    {
      fenetre=*(wininfo->ChZnc + y*wininfo->nx + x);

#ifdef ZNCDEBUG
fprintf(stderr,"Examine fenetre %ld\n",y*wininfo->nx + x);
#endif

      for(candidats=fenetre;
          candidats;
          candidats=candidats->NEXT)
      {
        zonecandidate=(znc*)(candidats->DATA);

#ifdef ZNCDEBUG
fprintf(stderr,"Zone : (%ld ; %ld)-(%ld ; %ld) layer %ld\n",zonecandidate->x1,zonecandidate->y1,zonecandidate->x2,zonecandidate->y2,zonecandidate->layer);
#endif

        /* On regarde les intersections */
        if( zonecandidate->x1 <= elem->x2 &&
            elem->x1 <= zonecandidate->x2 &&
            elem->y1 <= zonecandidate->y2 &&
            zonecandidate->y1 <= elem->y2 &&
            ( !chklayer || (chklayer && (zonecandidate->layer==elem->layer)))   )
        {
          if(zonecandidate->NEXT==NULL)
          {
            elus=addchain(elus,zonecandidate);
            zonecandidate->NEXT=(znc*)1;
#ifdef ZNCDEBUG
fprintf(stderr,"---> OK\n");
#endif
          }
        }
      }
    }
  }
  for(cleanelus=elus;cleanelus;cleanelus=cleanelus->NEXT)
    ((znc*)(cleanelus->DATA))->NEXT=NULL;

  return(elus);
}

WinZnc* creatwinznc(mx,my,MX,MY,PX,PY,nx,ny)
long mx,my,MX,MY,PX,PY,nx,ny;
{
  WinZnc	*wznc;
  long i,j;

  wznc		= (WinZnc*)mbkalloc(sizeof(WinZnc));
  wznc->mx	= mx;
  wznc->my	= my;
  wznc->MX	= MX;
  wznc->MY	= MY;
  wznc->PX	= PX;
  wznc->PY	= PY;
  wznc->nx	= nx;
  wznc->ny	= ny;
  wznc->ChZnc	= (chain_list**)mbkalloc(sizeof(chain_list*)*nx*ny);

  j=nx*ny;
  for(i=0;i<j;i++)
    wznc->ChZnc[i]=NULL;

#ifdef ZNCDEBUG
fprintf(stderr,"Creation du fenetrage znc\n");
fprintf(stderr,"Nombre des fenetres  : %ld x %ld\n",wznc->nx,wznc->ny);
fprintf(stderr,"Taille des fenetres  : %ld x %ld\n",wznc->PX,wznc->PY);
fprintf(stderr,"Origine des fenetres : %ld x %ld\n",wznc->mx,wznc->my);
#endif
  return(wznc);
}

void freewinznc(winfo)
WinZnc *winfo;
{
  long i,j;
  
  j=winfo->nx*winfo->ny;

  for(i=0;i<j;i++)
    if(winfo->ChZnc[i]!=NULL)
      freechain(winfo->ChZnc[i]);

  mbkfree(winfo->ChZnc);
  mbkfree(winfo);  
  
}

void delwinznc(elem,info)
znc	*elem;
WinZnc	*info;
{
  chain_list	 *chl,*chlw,*last=NULL;
  chain_list	**fenetre;

#ifdef ZNCDEBUG
fprintf(stderr,"Elimination de fenetrage de la znc ( %ld; %ld)-( %ld; %ld) sur layer %ld\n",elem->x1,elem->y1,elem->x2,elem->y2,elem->layer);
#endif

  for(chl=elem->Win;chl;chl=chl->NEXT)
  {
    fenetre=(chain_list**)(chl->DATA);

#ifdef ZNCDEBUG
fprintf(stderr," -> fenetre : %ld\n",(((long)fenetre)-((long)(info->ChZnc)))/sizeof(void*));
#endif

    /* Libere l'element dans la fenetre pointant vers elem */

    if( (*fenetre)->DATA == elem )
    {
      last=*fenetre;
      *fenetre=last->NEXT;
      last->NEXT=NULL;
      freechain(last); 
    }
    else
    {
      for(chlw=*fenetre;chlw;chlw=chlw->NEXT)
      {
        if(chlw->DATA == elem)
          break;
        last=chlw;
      }
      if(!chlw)
      {
        fprintf(stderr,"Erreur impossible (1).\n");
        exit(-2);
      }
      last->NEXT=chlw->NEXT;
      chlw->NEXT=NULL;
      freechain(chlw);
    }
  }
  freechain(elem->Win);
  elem->Win=NULL;
}

long getMx(elem,wininfo)
znc	*elem;
WinZnc	*wininfo;
{
  long x2;
  x2=(elem->x2-wininfo->mx)/wininfo->PX;
  return(x2);
}

long getMy(elem,wininfo)
znc	*elem;
WinZnc	*wininfo;
{
  long y2;
  y2=(elem->y2-wininfo->my)/wininfo->PY;
  return(y2);
}

long getmx(elem,wininfo)
znc	*elem;
WinZnc	*wininfo;
{
  long x1,y1,nx1;
  long ix1,ix2,iy1,iy2;

  x1=(elem->x1-wininfo->mx)/wininfo->PX;
  y1=(elem->y1-wininfo->my)/wininfo->PY;
  nx1=x1;
  if(x1>=1)
  {
    ix1=(x1-1)*wininfo->PX+wininfo->mx;
    iy1=y1*wininfo->PY+wininfo->my;
    ix2=ix1+wininfo->PX;
    iy2=iy1+wininfo->PY;
  
    /* verifie l'intersection des deux rectangles */ 
    if( elem->x1<=ix2 && ix1<=elem->x2 && iy1<=elem->y2 && elem->y1<=iy2 )
      nx1=x1-1;
  } 
  return(nx1);
}

long getmy(elem,wininfo)
znc	*elem;
WinZnc	*wininfo;
{
  long x1,y1,ny1;
  long ix1,ix2,iy1,iy2;

  y1=(elem->y1-wininfo->my)/wininfo->PY;
  x1=(elem->x1-wininfo->mx)/wininfo->PX;
  ny1=y1;
  if(y1>=1)
  {
    ix1=x1*wininfo->PX+wininfo->mx;
    iy1=(y1-1)*wininfo->PY+wininfo->my;
    ix2=ix1+wininfo->PX;
    iy2=iy1+wininfo->PY;
  
    /* verifie l'intersection des deuy rectangles */ 
    if( elem->x1<=ix2 && ix1<=elem->x2 && iy1<=elem->y2 && elem->y1<=iy2 )
      ny1=y1-1;
  } 
  return(ny1);
}

void makezncnode(win,ptsig)
WinZnc		*win;
losig_list	*ptsig;
{
  /* Pour parcourir toutes les znc, on se sert du fenetrage */
  chain_list	*scanwin;
  znc		*zone;
  long		 i;

  for(i=0;i<(win->nx*win->ny);i++)
  {
    for(scanwin=*(win->ChZnc+i);scanwin;scanwin=scanwin->NEXT)
    {
      zone=(znc*)(scanwin->DATA);
      if(! zone->node)
        zone->node=addlonode(ptsig,NULL);
    }
  }
}

void linklonodelocon(connect, ptsig, wz, gwin)
chain_list	*connect;
losig_list	*ptsig;
WinZnc		*wz;
GrgWindow       *gwin;
{
  chain_list	*scanchain;
  rdsrec_list	*rec;
  rdsrec_list	*zncrec;
  znc		 test;
  chain_list	*interznc;
  long		 noeud;
  locon_list	*connecteur;
  locon_list	*tstlocon;
  ptype_list    *ptrdslocon;
  chain_list    *scanzncrec;
  chain_list    *headzncrec;

  for(scanchain=connect;scanchain;scanchain=scanchain->NEXT)
  {
    rec=(rdsrec_list*)(scanchain->DATA);
    ptrdslocon = getptype( rec->USER, RDSLOCON );
    connecteur = ((locon_list*)(ptrdslocon->DATA));
    if( !connecteur )
      continue; /* cas ou plusieurs rectangles connecteurs sont superposes */

    /* La znc test ne sert qu'à récupérer la 'vraie' qui regroupe celles
       qu'il faut pour avoir le lonode */

    test.x1=rec->X;
    test.y1=rec->Y;
    test.x2=rec->X+rec->DX;
    test.y2=rec->Y+rec->DY;
    test.layer = (GetRdsLayer(rec)==RDS_GATE) ? RDS_POLY : GetRdsLayer(rec);
    
    interznc=getwininterznc(&test,wz,1);

    if(interznc->NEXT)
    {
      chain_list	*scanznc;
      fprintf(stderr,"Un locon correspond à plusieurs ZNC.\n");
      fprintf(stderr,"Rectangle : (%ld,%ld)-(%ld,%ld) [%s]\n",test.x1,test.y1,
                                                              test.x2,test.y2,
                                                   RDS_LAYER_NAME[test.layer]);
      for(scanznc=interznc;scanznc;scanznc=scanznc->NEXT)
        fprintf(stderr,"ZNC : (%ld,%ld)-(%ld,%ld) [%s]\n",
                ((znc*)(scanznc->DATA))->x1,
                ((znc*)(scanznc->DATA))->y1,
                ((znc*)(scanznc->DATA))->x2,
                ((znc*)(scanznc->DATA))->y2,
                RDS_LAYER_NAME[((znc*)(scanznc->DATA))->layer]);
      exit(-1);
    }

    noeud=((znc*)(interznc->DATA))->node; 

    setloconnode(connecteur, noeud);

    headzncrec = GrgGetInterWindows( rec, gwin );
    for( scanzncrec = headzncrec ; scanzncrec ; scanzncrec = scanzncrec->NEXT )
    {
      zncrec = (rdsrec_list*)( scanzncrec->DATA );
      ptrdslocon = getptype( zncrec->USER, RDSLOCON );
      if( ptrdslocon )
      {
        tstlocon = ((locon_list*)(ptrdslocon->DATA));
        if( tstlocon == connecteur )
          ptrdslocon->DATA = NULL;
      }
    }
    freechain( headzncrec );
    /* pour pouvoir trier les connecteurs dans l'ordre */

    rec->USER=addptype(rec->USER,TMPLONODE,(void*)noeud);
  }
}

/* =============================================================================
!                    FONCTION DE DEBUGGAGE                                     !
==============================================================================*/

void dbg_affznc(winznc)
WinZnc	*winznc;
{
  chain_list	*scanwin;
  znc		*zone;
  long		 i;

  for(i=0;i<(winznc->nx*winznc->ny);i++)
  {
    fprintf(stderr,"fenetre %ld\n",i);
    for(scanwin=*(winznc->ChZnc+i);scanwin;scanwin=scanwin->NEXT)
    {
      zone=(znc*)(scanwin->DATA);
      fprintf(stderr,"\t%lX : (%ld,%ld)-(%ld,%ld) [%s]\n", (long)zone,
                                                          zone->x1,
                                                          zone->y1,
                                                          zone->x2,
                                                          zone->y2,
                                                 RDS_LAYER_NAME[zone->layer]);
    }
  }
}
