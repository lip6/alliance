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

/* Construction du reseau RC correspondant aux rectangles de 
   la figure */


# include "mut.h"
# include "rds.h"
# include "mlo.h"
# include "rcn.h"
# include "mph.h"
# include "rpr.h"
# include "Windows95.h"
# include "znc.h"
# include "buildrcn.h"
# include "lynxrcn.h"
# include "extract.h"
# include "mbkrds.h"
# include "inter.h"
# include "via.h"

/* A mettre en commentaire pour ne pas avoir de message de debuggage */
/*
# define BUILDRCNDEBUG
*/

/*
#define BUILDRCNCTL
*/

int RectangleValide __P((rdsrec_list*));
int RectangleDiff __P((rdsrec_list*));
void traiteinterparra __P((rdsrec_list*, chain_list*, WinZnc*));
void traiteinterperpen __P((rdsrec_list*, chain_list*, WinZnc*));
void addzncvia __P((chain_list*,WinZnc*,GrgWindow*,losig_list*));
void znc2rcn __P((WinZnc*, losig_list*));
chain_list* sortchain __P((chain_list*, char));
void sortloconnode __P((chain_list*, WinZnc*));
void sortbullelocon __P((locon_list*, chain_list*));
int isviacontact( int );
void affrec( rdsrec_list* );

#define min(a,b) ((a>b)?b:a)
#define max(a,b) ((a>b)?a:b)

/******************************************************************************/



void buildrcn(fig,scansignal)
lofig_list	*fig;
losig_list	*scansignal;
{
  GrgWindow	*WindowsInfo;
  rdsrec_list	*scanrec,*irec;
  int		 EndEqui;
  WinZnc	*zncwininfo;
  chain_list	*inter_list,*parra,*perpen,*scanchain;
  ptype_list	*ptlvia;
  chain_list	*connect;
  long		 layer;
  chain_list    *scanchrec;

#ifdef BUILDRCNCTL
fprintf(stderr,"Traitement du signal %ld [%s]\n",
               scansignal->INDEX,
               getsigname(scansignal)?getsigname(scansignal):"no name");
#endif

#ifdef BUILDRCNDEBUG
fprintf(stderr,"Traitement du signal %ld [%s]\n",
               scansignal->INDEX,
               getsigname(scansignal)?getsigname(scansignal):"no name");
#endif

#ifdef BUILDRCNDEBUG
fprintf(stderr,"Construction des fenetrages\n");
#endif
#ifdef BUILDRCNCTL
fprintf(stderr,"Construction du fenetrage...                                 ");
#endif

  WindowsInfo=GrgCreatWindows(scansignal);

  zncwininfo=creatwinznc( WindowsInfo->mx,
                          WindowsInfo->my,
                          WindowsInfo->Mx,
                          WindowsInfo->My,
                          WindowsInfo->Px,
                          WindowsInfo->Py,
                          WindowsInfo->nx,
                          WindowsInfo->ny  );

  connect=NULL;
  for( scanchrec = (chain_list*)(getptype(scansignal->USER,RCN_FEQUI)->DATA) ;
       scanchrec ;
       scanchrec = scanchrec->NEXT
     )
  {
    scanrec=(rdsrec_list*)(scanchrec->DATA);

    EndEqui=0;
  
  
  #ifdef BUILDRCNCTL
  fprintf(stderr,"\rTraitement de l'equipotentielle...                         ");
  #endif
  
    while(!EndEqui)
    {
      if(RectangleValide(scanrec))
      {
  #ifdef BUILDRCNDEBUG
  fprintf(stderr,"\n\n******** Rectangle traité :\n");
  dbg_affrec(scanrec);
  #endif
        inter_list=GrgGetInterWindows(scanrec,WindowsInfo);
        parra=NULL;
        perpen=NULL;
  
        for(scanchain=inter_list;scanchain;scanchain=scanchain->NEXT)
        {
          irec=(rdsrec_list*)(scanchain->DATA);
  
          if(RectangleValide(irec))
          {
            if(Intersect(scanrec,irec)==PERPEN)
              perpen=addchain(perpen,scanchain->DATA);
            else
              parra=addchain(parra,scanchain->DATA);
          }
        }
  
        /* Traite les extremités du rectangle */
  
        if(GETDIRECTION(scanrec)==HORIZ_DIR) /* horizontal */
        {
          addznc(scanrec->X,scanrec->Y,scanrec->X,scanrec->Y+scanrec->DY,
                 GetRdsLayer(scanrec),zncwininfo);
          addznc(scanrec->X+scanrec->DX,scanrec->Y,
                 scanrec->X+scanrec->DX,scanrec->Y+scanrec->DY,
                 GetRdsLayer(scanrec),zncwininfo);
        }
        else
        {
          addznc(scanrec->X,scanrec->Y,scanrec->X+scanrec->DX,scanrec->Y,
                 GetRdsLayer(scanrec),zncwininfo);
          addznc(scanrec->X,scanrec->Y+scanrec->DY,
                 scanrec->X+scanrec->DX,scanrec->Y+scanrec->DY,
                 GetRdsLayer(scanrec),zncwininfo);
        }
  
        /* Traite les rectangles en intersection perpendiculaire */
  
        traiteinterperpen(scanrec,perpen,zncwininfo);
  
        /* Traite les rectangles en intersection parralelles */
  
        traiteinterparra(scanrec,parra,zncwininfo);
  
  
        if(perpen) freechain(perpen);
        if(parra) freechain(parra);
        if(inter_list) freechain(inter_list);
      }
      /* On ajoute les CxP et les CxN qui servent dans les transistors
         coud\'es. Ils sont fait en GATE, que l'on traite pour l'extraction
         comme du POLY. Ces morceaux seront toujours isoles a ce stade,
         on a juste qu'a ajouter une ZNC le representant */
  
      if(    IsRdsVia(scanrec) &&
           ( GetLynxAttribute(scanrec) == C_X_N ||
             GetLynxAttribute(scanrec) == C_X_P    )
        ) 
        addznc(scanrec->X,scanrec->Y,
               scanrec->X+scanrec->DX,scanrec->Y+scanrec->DY,
               RDS_POLY,zncwininfo);
  
      if(getptype(scanrec->USER,RDSLOCON))
      {
        connect=addchain(connect,scanrec);
      }
      else /* plus clean : on traite les connecteurs plus loin */
        if(RectangleDiff(scanrec))
        {
          addznc(scanrec->X,scanrec->Y,
                 scanrec->X+scanrec->DX,scanrec->Y+scanrec->DY,
                 GetRdsLayer(scanrec),zncwininfo);
        }
  
      if(IsLynxEndEqui(scanrec))
        EndEqui=1;
      else
        scanrec=LYNX_EQUI(scanrec);
    }
  }

  /* Ajoute les vias dans les znc */
  if( (ptlvia=getptype(scansignal->USER,GRGVIA)) )
    addzncvia((chain_list*)(ptlvia->DATA),zncwininfo,WindowsInfo,scansignal);


#ifdef BUILDRCNDEBUG
fprintf(stderr,"ZNC apres le traitement des vias\n");
dbg_affznc(zncwininfo);
#endif

  /* Traitement des connecteurs */

  for(scanchain=connect;scanchain;scanchain=scanchain->NEXT)
  {

    scanrec=(rdsrec_list*)(scanchain->DATA);
    layer=GetRdsLayer(scanrec);

    if(layer==RDS_GATE)
    {
      addznc(scanrec->X,scanrec->Y,scanrec->X+scanrec->DX,
             scanrec->Y+scanrec->DY,RDS_POLY,zncwininfo);
    }
    else
    { 
      addznc(scanrec->X,scanrec->Y,scanrec->X+scanrec->DX,
             scanrec->Y+scanrec->DY,layer,zncwininfo);
    }
  }

#ifdef BUILDRCNDEBUG
fprintf(stderr,"ZNC apres le traitement des connecteurs\n");
dbg_affznc(zncwininfo);
#endif


#ifdef BUILDRCNCTL
fprintf(stderr,"\rCreation des lonodes...                                    ");
#endif

  makezncnode(zncwininfo,scansignal);

#ifdef BUILDRCNCTL
fprintf(stderr,"\rConstruction de la table...                                ");
#endif

  buildtable(scansignal);

#ifdef BUILDRCNCTL
fprintf(stderr,"\rConstruction des Wires...                                  ");
#endif

  znc2rcn(zncwininfo,scansignal);

#ifdef BUILDRCNCTL
fprintf(stderr,"\rAjout des lonodes dans les locons...                       ");
#endif

  linklonodelocon( connect, scansignal, zncwininfo, WindowsInfo );

#ifdef BUILDRCNCTL
fprintf(stderr,"\rLiberation de la table...                                  ");
#endif


  freetable(scansignal);


#ifdef BUILDRCNCTL
fprintf(stderr,"\rTrie des connecteurs d'instance...                         ");
#endif

  sortloconnode(connect,zncwininfo);

  if(connect)
    freechain(connect);



#ifdef BUILDRCNDEBUG
fprintf(stderr,"Liberation du fenetrage\n");
#endif

#ifdef BUILDRCNCTL
fprintf(stderr,"\rLiberation des fenetrages...                               ");
#endif


  GrgClearWindows(WindowsInfo,scansignal);
  freewinznc(zncwininfo);

#ifdef BUILDRCNCTL
fprintf(stderr,"\rOk.                                                      \n");
#endif



#ifdef BUILDRCNDEBUG
fprintf(stderr,"fin du traitement des rectangles du signal %ld\n\n\n",
               scansignal->INDEX);
#endif
}

int IsRcn( R )
rdsrec_list	*R;
{
  /* Layers pour lesquels les interconnexions sont extraites */
  if(    GetRdsLayer(R)==RDS_ALU1
      || GetRdsLayer(R)==RDS_ALU2
      || GetRdsLayer(R)==RDS_ALU3
      || GetRdsLayer(R)==RDS_ALU4
      || GetRdsLayer(R)==RDS_ALU5
      || GetRdsLayer(R)==RDS_ALU6
      || GetRdsLayer(R)==RDS_POLY )
    return( 1 );
  return(0);
}

int isviacontact( int layer )
{
  /* On ne peut prendre un ieme element de la table des vias car les elements
   * C_X_N et C_X_P posent des problèmes */

  if( layer == RDS_CONT ||
      layer == RDS_VIA1 ||
      layer == RDS_VIA2 ||
      layer == RDS_VIA3 ||
      layer == RDS_VIA4 ||
      layer == RDS_VIA5
    )
    return(1);
  return(0);
}

int RectangleValide(R)
rdsrec_list	*R;
{
  if(    !IsRdsVia(R)
      && !IsRdsConnector(R)
      &&  IsRcn( R )
    )
  {
    return(1);
  }
  return(0);
}

int RectangleDiff(R)
rdsrec_list	*R;
{
  /* Layer dont on extrait pas les resistances mais qu'on considere pour
   * recuperer les locon correspondants */
  if( GetRdsLayer(R)==RDS_PDIF || 
      GetRdsLayer(R)==RDS_NDIF   )
    return(1);
  return(0);
}

void traiteinterperpen(scanrec,perpen,zncwininfo)
rdsrec_list	*scanrec;
chain_list	*perpen;
WinZnc		*zncwininfo;
{
  chain_list	*scanchain;
  rdsrec_list	*irec;
  
  for(scanchain=perpen;scanchain;scanchain=scanchain->NEXT)
  {
    irec=(rdsrec_list*)(scanchain->DATA);

    if(RectangleValide(irec))
    {
#ifdef BUILDRCNDEBUG
fprintf(stderr,"Intersection perpendiculaire : \n");
dbg_affrec(irec);
#endif

      if(GETDIRECTION(scanrec)==HORIZ_DIR) /* horizontal */
        addznc(irec->X,scanrec->Y,irec->X+irec->DX,scanrec->Y+scanrec->DY,
             GetRdsLayer(scanrec),zncwininfo);
      else /* vertical */
        addznc(scanrec->X,irec->Y,scanrec->X+scanrec->DX,irec->Y+irec->DY,
             GetRdsLayer(scanrec),zncwininfo);
    }          
  }
}

void traiteinterparra(scanrec,parra,zncwininfo)
rdsrec_list	*scanrec;
chain_list	*parra;
WinZnc		*zncwininfo;
{
  chain_list	*scanchain,*scanchain2;
  rdsrec_list	*irec,*irec2;
  long		 mx,my,Mx,My;

  for(scanchain=parra;scanchain;scanchain=scanchain->NEXT)
  {
    irec=(rdsrec_list*)(scanchain->DATA);

    if(GETDIRECTION(scanrec)==HORIZ_DIR)
    {
      if(irec->X>=scanrec->X)
      {
        my=min(irec->Y,scanrec->Y);
        My=max(irec->Y+irec->DY,scanrec->Y+scanrec->DY);
        for(scanchain2=parra;scanchain2;scanchain2=scanchain2->NEXT)
        {
          irec2=(rdsrec_list*)(scanchain2->DATA);
          if(irec2->X<=irec->X && irec2->X+irec2->DX>=irec->X)
          {
            my=min(my,irec2->Y);
            My=max(My,irec2->Y+irec2->DY);
          }
        }
        addznc(irec->X,my,irec->X,My,GetRdsLayer(scanrec),zncwininfo);
      }

      if(irec->X+irec->DX<=scanrec->X+scanrec->DX)
      {
        my=min(irec->Y,scanrec->Y);
        My=max(irec->Y+irec->DY,scanrec->Y+scanrec->DY);
        for(scanchain2=parra;scanchain2;scanchain2=scanchain2->NEXT)
        {
          irec2=(rdsrec_list*)(scanchain2->DATA);
          if( irec2->X<=irec->X+irec->DX &&
              irec2->X+irec2->DX>=irec->X+irec->DX )
          {
            my=min(my,irec2->Y);
            My=max(My,irec2->Y+irec2->DY);
          }
        }
        addznc(irec->X+irec->DX,my,irec->X+irec->DX,My,
               GetRdsLayer(scanrec),zncwininfo);
      }
    }
    else
    {
      if(irec->Y>=scanrec->Y)
      {
        mx=min(irec->X,scanrec->X);
        Mx=max(irec->X+irec->DX,scanrec->X+scanrec->DX);
        for(scanchain2=parra;scanchain2;scanchain2=scanchain2->NEXT)
        {
          irec2=(rdsrec_list*)(scanchain2->DATA);
          if(irec2->Y<=irec->Y && irec2->Y+irec2->DY>=irec->Y)
          {
            mx=min(mx,irec2->X);
            Mx=max(Mx,irec2->X+irec2->DX);
          }
        }
        addznc(mx,irec->Y,Mx,irec->Y,GetRdsLayer(scanrec),zncwininfo);
      }

      if(irec->Y+irec->DY<=scanrec->Y+scanrec->DY)
      {
        mx=min(irec->X,scanrec->X);
        Mx=max(irec->X+irec->DX,scanrec->X+scanrec->DX);
        for(scanchain2=parra;scanchain2;scanchain2=scanchain2->NEXT)
        {
          irec2=(rdsrec_list*)(scanchain2->DATA);
          if( irec2->Y<=irec->Y+irec->DY &&
              irec2->Y+irec2->DY>=irec->Y+irec->DY )
          {
            mx=min(mx,irec2->X);
            Mx=max(Mx,irec2->X+irec2->DX);
          }
        }
        addznc(mx,irec->Y+irec->DY,Mx,irec->Y+irec->DY,
               GetRdsLayer(scanrec),zncwininfo);
      }
    }
  }
}

void addzncvia(headvia,winznc,winrec, ptsig )
chain_list	*headvia;
WinZnc		*winznc;
GrgWindow	*winrec;
losig_list      *ptsig;
{
  chain_list	*intervia;
  chain_list	*scaninter;
  long		 mx=0,my=0,Mx=0,My=0,layer=0;
  rdsrec_list	*rec;
  rdsrec_list	*scanrec;
  chain_list	*scanvia;
  ptype_list    *ptl;

#ifdef BUILDRCNDEBUG
fprintf(stderr,"Traitement des vias\n");
#endif

  for(scanvia=headvia;scanvia;scanvia=scanvia->NEXT)
  {
    intervia=NULL;
    rec = (rdsrec_list*)(scanvia->DATA);
    do
    {
      ptl = getptype( rec->USER, RCN_SIG_PTR );
      if( !ptl )
      {
        rec = LYNX_LINK( rec );
        continue;
      }
      
      if( ptl->DATA == ptsig )
      {
        layer = GetRdsLayer( rec );
        if( !isviacontact( layer ) &&
            ( IsRcn( rec ) || RectangleDiff( rec ) )
          )
        {

          mx       = rec->X;
          my       = rec->Y;
          Mx       = rec->X+rec->DX;
          My       = rec->Y+rec->DY;
          intervia = GrgGetInterWindows(rec,winrec);

          if(intervia)
          {
            for(scaninter=intervia;scaninter;scaninter=scaninter->NEXT)
            {
              scanrec=(rdsrec_list*)(scaninter->DATA);
              /*
              if(IsRcn(scanrec) || RectangleDiff( rec ) )
              */
              {
                if(GETDIRECTION(scanrec)==HORIZ_DIR)
                {
                  my=min(my,scanrec->Y);
                  My=max(My,scanrec->Y+scanrec->DY);
                }
                else
                {
                  mx=min(mx,scanrec->X);
                  Mx=max(Mx,scanrec->X+scanrec->DX);
                }
              }
            }
            addznc(mx,my,Mx,My,layer,winznc);
            freechain(intervia);
          }
          else
            addznc(mx,my,Mx,My,layer,winznc);
        }
      }
      rec = LYNX_LINK( rec );
    }
    while( rec != scanvia->DATA );
  }
}

void znc2rcn(win,sig)
WinZnc		*win;
losig_list	*sig;
{
  rdsrec_list	*scanrec,*r;
  short		 EndEqui;
  znc		 elem,*noeud1,*noeud2=NULL;
  chain_list	*interznc,*scaninter;
  chain_list	*scanchain,*scanchain1,*scanchain2;
  long		 n1=0,n2=0;
  long		 N1,N2;
  long		 rcntype;
  float		 rcnresi=0.0,rcncapa;
  long		 x,y,dx,dy,mx1,my1,mx2,my2;
  ptype_list	*ptl;
  lowire_list	*w;
  chain_list    *scanchrec;
  chain_list    *scanvia;
  int            layer;
  rdsrec_list   *rec;
  ptype_list    *ptl2;

#ifdef BUILDRCNDEBUG
  fprintf(stderr,"\n\nConstruction des wires \n\n");
#endif

/* On place dans les intersections les rectangles qui les composent */

  for( scanchrec = (chain_list*)(getptype(sig->USER,RCN_FEQUI)->DATA) ;
       scanchrec ;
       scanchrec = scanchrec->NEXT
     )
  {
    scanrec=(rdsrec_list*)(scanchrec->DATA);

    EndEqui=0;
    while(!EndEqui)
    {
      if(RectangleValide(scanrec))
      {
        elem.x1=scanrec->X;
        elem.y1=scanrec->Y;
        elem.x2=scanrec->X+scanrec->DX;
        elem.y2=scanrec->Y+scanrec->DY;
        elem.layer=GetRdsLayer(scanrec);
        elem.NEXT=NULL;
  
        interznc=getwininterznc(&elem,win,1);
        for(scaninter=interznc;scaninter;scaninter=scaninter->NEXT)
        {
          noeud1=(znc*)(scaninter->DATA);
          for(scanchain=noeud1->rec;scanchain;scanchain=scanchain->NEXT)
            if(scanchain->DATA==scanrec)
              break;
          if(!scanchain)
            noeud1->rec=addchain(noeud1->rec,scanrec);
        }
      }
  
      if(IsLynxEndEqui(scanrec))
        EndEqui=1;
      else
        scanrec=LYNX_EQUI(scanrec);
    }
  }
  
  for( scanchrec = (chain_list*)(getptype(sig->USER,RCN_FEQUI)->DATA) ;
       scanchrec ;
       scanchrec = scanchrec->NEXT
     )
  {
    scanrec=(rdsrec_list*)(scanchrec->DATA);

    EndEqui=0;

    while(!EndEqui)
    {
      if(RectangleValide(scanrec))
      {
        elem.x1=scanrec->X;
        elem.y1=scanrec->Y;
        elem.x2=scanrec->X+scanrec->DX;
        elem.y2=scanrec->Y+scanrec->DY;
        elem.layer=GetRdsLayer(scanrec);
        elem.NEXT=NULL;
  
        interznc=getwininterznc(&elem,win,1);
  
  
        if(interznc)
        {
  #ifdef   BUILDRCNDEBUG
  fprintf(stderr,"Traite   le rectangle (%ld,%ld)-(%ld,%ld)\n",
            scanrec->X,scanrec->Y,scanrec->X+scanrec->DX,scanrec->Y+scanrec->DY);
  #endif
  
          if(GETDIRECTION(scanrec)==HORIZ_DIR)
            interznc=sortchain(interznc,'X');
          else
            interznc=sortchain(interznc,'Y');
  
          n1=0l;
          n2=0l;
  
          for(scaninter=interznc;scaninter;scaninter=scaninter->NEXT)
          {
            noeud1=(znc*)(scaninter->DATA);
            n1=noeud1->node;
      
            if(n1 && n2 && getlowire(sig,n1,n2)==NULL)
            {
              /* Place les rectangles communs au noeud1 et au noeud2 dans une
                 chain_list */
  
              mx1=scanrec->X;
              mx2=mx1+scanrec->DX;
              my1=scanrec->Y;
              my2=my1+scanrec->DY;
              for(scanchain1=noeud1->rec;scanchain1;scanchain1=scanchain1->NEXT)
              {
                for(scanchain2=noeud2->rec;scanchain2;scanchain2=scanchain2->NEXT)
                  if(scanchain1->DATA==scanchain2->DATA)
                  {
                    r=(rdsrec_list*)(scanchain1->DATA);
                    mx1=min(mx1,r->X);
                    mx2=max(mx2,r->X+r->DX);
                    my1=min(my1,r->Y);
                    my2=max(my2,r->Y+r->DY);
                  }
              }
  
              if(GETDIRECTION(scanrec)==HORIZ_DIR)
              {
                x=(noeud2->x2-noeud2->x1)/2 + noeud2->x1;
                dx=(noeud1->x2-noeud1->x1)/2 + noeud1->x1 - x;
                y=scanrec->Y+ scanrec->DY/2;
                dy=my2-my1;
                rcnresi=dx/dy*GET_LYNX_RESISTOR_LAYER(GetRdsLayer(scanrec));
                rcncapa=2*dx*GET_LYNX_PERIMETER_CAPA_LAYER(GetRdsLayer(scanrec))+
                        dx*dy*GET_LYNX_SURFACE_CAPA_LAYER(GetRdsLayer(scanrec));
              }
              else
              {
                x=scanrec->X+scanrec->DX/2;
                dx=mx2-mx1;
                y=(noeud2->y2-noeud2->y1)/2 + noeud2->y1;
                dy=(noeud1->y2-noeud1->y1)/2 + noeud1->y1 -y;
     
                rcnresi=dy/dx*GET_LYNX_RESISTOR_LAYER(GetRdsLayer(scanrec));
                rcncapa=2*dy*GET_LYNX_PERIMETER_CAPA_LAYER(GetRdsLayer(scanrec))+
                        dx*dy*GET_LYNX_SURFACE_CAPA_LAYER(GetRdsLayer(scanrec));
              }
  
              rcntype=RCN_WIRE_UNKNOW;
  
              if( GetRdsLayer(scanrec)==RDS_ALU1 ) rcntype=RCN_WIRE_ALU1;
              if( GetRdsLayer(scanrec)==RDS_ALU2 ) rcntype=RCN_WIRE_ALU2;
              if( GetRdsLayer(scanrec)==RDS_POLY ) rcntype=RCN_WIRE_POLY;
  
            
  #ifdef   BUILDRCNDEBUG
  fprintf(stderr,"Ajout   d'un wire en (%ld,%ld) de dimensions (%ld,%ld)\n",
                   x,y,dx,dy);
  #endif
  
              if(dx>dy)
              {
                if( noeud1->x1 < noeud2->x1 )
                {
                  N1=n1;
                  N2=n2;
                }
                else
                {
                  N1=n2;
                  N2=n1;
                }
              }
              else
              {
                if(noeud1->y1 < noeud2->y1)
                {
                  N1=n1;
                  N2=n2;
                }
                else
                {
                  N1=n2;
                  N2=n1;
                }
              } 
              addlowire( sig, rcntype, 0, rcnresi, rcncapa, x, y, dx, dy, N1, N2);
            }
            n2=n1;
            noeud2=noeud1;
          }
  
          freechain(interznc);
        }
      }
  
      if(IsLynxEndEqui(scanrec))
        EndEqui=1;
      else
        scanrec=LYNX_EQUI(scanrec);
    }
  }

  /* On s'occupe maintenant des vias */
  if( (ptl=getptype(sig->USER,GRGVIA)) )
  {
    for(scanvia=(chain_list*)(ptl->DATA);scanvia;scanvia=scanvia->NEXT)
    {
      layer=-1;
      rec = (rdsrec_list*)(scanvia->DATA);
      n1 = 0;
      n2 = 0;
      do
      {
        ptl2 = getptype( rec->USER, RCN_SIG_PTR );
        if( !ptl2 )
        {
          rec = LYNX_LINK( rec );
          continue;
        }
        if( ptl2->DATA == sig )
        {
          layer = GetRdsLayer( rec );
          if( !isviacontact( layer )                     && 
              ( IsRcn( rec ) || RectangleDiff( rec ) )
            )
          {
            
            elem.x1    = rec->X ;
            elem.y1    = rec->Y ;
            elem.x2    = rec->DX + elem.x1 ;
            elem.y2    = rec->DY + elem.y1 ;
            elem.layer = layer ;
            scanchain1 = getwininterznc( &elem, win, 1 ) ;
            if( n1 == 0 )
              n1=((znc*)(scanchain1->DATA))->node;
            else
              n2=((znc*)(scanchain1->DATA))->node;
            freechain(scanchain1);
          }
          else
          {
            if( isviacontact( layer ) )
              rcnresi=GET_LYNX_RESISTOR_LAYER(layer);
          }
        }
        rec = LYNX_LINK( rec );
      }
      while( rec != scanvia->DATA );

      if( n1>0 && n2>0 )
      {
        if((w=getlowire(sig,n1,n2))==NULL)
        {
          addlowire(sig,RCN_WIRE_CONT_VIA,0,rcnresi,0.0,
                    (elem.x2-elem.x1)/2+elem.x1,
                    (elem.y2-elem.y1)/2+elem.y1,0,0,n1,n2);
        }
        else
        {
          addpararesiwire(w,rcnresi);
        }
      }
    }
  }
}

chain_list* sortchain(entree,champ)
chain_list	*entree;
char		 champ;
{
  chain_list	*scanchain,*sortie,*last,*prevmax,*eff;
  znc		*maxi,*z;

  /* tri a bulle de la liste des znc */

  /* On veut faire un tri par ordre croissant en utilisant la fonction
     chain_list qui ajoute les elements a l'envers : on va donc faire
     un tri par ordre decroissant */

  if(!entree)
  {
    fprintf(stderr,"Trie de chaine (null). \n");
    exit(-1);
  }

  if(entree->NEXT==NULL)
    return(entree);

  sortie=NULL;

  while(entree)
  {
    /* il faut debuter avec qqchose */
    maxi=(znc*)entree->DATA;

    /* recherche de l'element le plus grand dans la liste */
    last=NULL;
    prevmax=NULL;

    for(scanchain=entree;scanchain;scanchain=scanchain->NEXT)
    {
      z=(znc*)(scanchain->DATA);
 
      if( ( champ=='X' && z->x1 > maxi->x1 ) ||
          ( champ=='Y' && z->y1 > maxi->y1 )    )
      { 
        maxi = z;
        prevmax=last;
      }
      last=scanchain;
    }

    /* maitenant qu'on l'a trouve, on le met dans la chainlist de sortie */
    sortie=addchain(sortie,maxi);

    /* efface l'element de la liste chainee d'entree */
    if(prevmax)
    {
      eff=prevmax->NEXT;
      prevmax->NEXT=prevmax->NEXT->NEXT;
    }
    else
    {
      eff=entree;
      entree=entree->NEXT;
    }
    if(!eff)
    {
      fprintf(stderr,"eff=null\n");  
      exit(-1);
    }
    eff->NEXT=NULL;
    freechain(eff);
  }

#ifdef BUILDRCNDEBUG
{
  chain_list	*scan;
  for(scan=sortie;scan;scan=scan->NEXT)
    fprintf(stderr,"znc : (%ld,%ld)-(%ld,%ld) [%ld] lonode :%ld\n",
      ((znc*)(scan->DATA))->x1,
      ((znc*)(scan->DATA))->y1,
      ((znc*)(scan->DATA))->x2,
      ((znc*)(scan->DATA))->y2,
      ((znc*)(scan->DATA))->layer,
      ((znc*)(scan->DATA))->node);
}
#endif
  return(sortie);
}

void sortloconnode(connect,winznc)
chain_list	*connect;
WinZnc		*winznc;
{

  chain_list	*scanchain;
  rdsrec_list	*rec;
  ptype_list	*ptl;
  locon_list	*logical_connector;

  struct ssort
  {
    struct ssort	*NEXT;
    locon_list		*connector;
    chain_list		*conlofig;
  } *sort,*scansort;


  /* Recupere la liste des connecteurs de la figure */
  sort=NULL;
  for(scanchain=connect;scanchain;scanchain=scanchain->NEXT)
  {
    rec=(rdsrec_list*)(scanchain->DATA);
    if( (ptl=getptype(rec->USER,RDSLOCON)) )
    {
      logical_connector=(locon_list*)(ptl->DATA);
      if( !logical_connector )
        continue; /* cas des connecteurs superposes */
      
      if(logical_connector->TYPE=='I' ||
         logical_connector->TYPE=='E'    )
      {

        for(scansort=sort;scansort;scansort=scansort->NEXT)
        {
          if(scansort->connector == logical_connector)
            break;
        }

        if(scansort)
        {
          scansort->conlofig=addchain(scansort->conlofig,rec);
        }
        else
        {
          scansort=mbkalloc(sizeof(struct ssort));
          scansort->NEXT=sort;
          scansort->connector=logical_connector;
          scansort->conlofig=addchain(NULL,rec);

          sort=scansort;
        }
      }
    }
  }

  for(scansort=sort;scansort;scansort=scansort->NEXT)
  {
    if(! scansort->conlofig) 
    {
      fprintf(stderr,"Connecteur-> %s\n",scansort->connector->NAME);
      exit(1);
    }
    sortbullelocon(scansort->connector,scansort->conlofig);
  }
}

void sortbullelocon(logical_connector,conlofig)
locon_list	*logical_connector;
chain_list	*conlofig;
{
  chain_list	*scanchain,*trie;
  rdsrec_list	*rec,*maxi;
  chain_list	*prevmax,*last;
  chain_list	*eff;
  ptype_list	*ptl;
  num_list	*node;

  /* Dans la chain_list conlofig on a la liste des rectangles connecteurs 
     de la figure */

  /* on effectue un trie a bulle sur le champ Y */  

  if(conlofig->NEXT!=NULL)
  {
    trie=NULL;
    while(conlofig)
    {
      maxi=(rdsrec_list*)conlofig->DATA;
      last=NULL;
      prevmax=NULL;

      for(scanchain=conlofig;scanchain;scanchain=scanchain->NEXT)
      {
        rec=(rdsrec_list*)(scanchain->DATA);
        if( rec->Y < maxi->Y )
        {
          maxi=rec;
          prevmax=last;
        }
        else
        {
          if(rec->Y == maxi->Y)
            if(rec->X < maxi->X)
            {
              maxi=rec;
              prevmax=last;
            }
        }
        
        last=scanchain;
      }

      trie=addchain(trie,maxi);

      if(prevmax)
      {
        eff=prevmax->NEXT;
        prevmax->NEXT=prevmax->NEXT->NEXT;
      }
      else
      {
        eff=conlofig;
        conlofig=conlofig->NEXT;
      }
      eff->NEXT=NULL;
      freechain(eff);
    }
  }
  else
    trie=conlofig;

  node=NULL;
  for(scanchain=trie;scanchain;scanchain=scanchain->NEXT)
  {
    rec=((rdsrec_list*)(scanchain->DATA));
    ptl=getptype(rec->USER,TMPLONODE);
    if(!ptl)
    {
      fprintf(stderr,"Un connecteur (%s) n'est relie a aucun lonode. %c\n",
                     logical_connector->NAME,
                     logical_connector->TYPE);
      exit(1);
    }
    node=addnum(node,((long)(ptl->DATA)));
  }

  /* on a la liste des connecteurs triee dans la chain_list trie */
  
  freenum(logical_connector->PNODE);
  logical_connector->PNODE = node;
  freechain(trie);
}

void affrec( rdsrec_list *r)
{
  printf( "%s (%ld,%ld)-(%ld,%ld) Cont=%d Rcn=%d Diff=%d Val=%d\n",
          RDS_LAYER_NAME[GetRdsLayer(r)],
          r->X,
          r->Y,
          r->X+r->DX,
          r->Y+r->DY,
          isviacontact( GetRdsLayer(r) ),
          IsRcn(r),
          RectangleDiff(r),
          RectangleValide(r)
        );
}
