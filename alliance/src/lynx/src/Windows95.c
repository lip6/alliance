/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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

#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "rds.h"
#include "mut.h"
#include "mlo.h"
#include "mph.h"
#include "Windows95.h"
#include "lynxrcn.h"
#include "inter.h"
#include "mbkrds.h"
#include "extract.h"

char GrgWinDebug=0;
GrgWinChRec *FREE_GrgWinChRec=NULL;

#define MXFREE 5

typedef struct smystack
{
  struct smystack *NEXT;
  ptype_list	*PILE[MAXSTACK];
  long int	 CASEIN;
} mystack;

mystack* creatmystack __P((void));
void freemystack __P((mystack*));
mystack* addinmystack __P((mystack*, ptype_list*));
GrgWinChRec* addwinchrec __P((GrgWinChRec*, rdsrec_list*));


GrgWindow* GrgCreatWindows(Signal)
losig_list *Signal;
{
  rdsrec_list *Rec,fenetre;
  char End_Equi=0;
  GrgWindow *Win=NULL;
  long	nbrec=0,N,k;		/* N: nombre de fenetre sur un cote du
                                   fenetrage */
  long  x1,x2,y1,y2,x,y;
  GrgWinChRec *ptwinchrec;
  ptype_list *ptGrgWin;
  chain_list *sl;
  long i;
  ptype_list    *ptdebug;
  chain_list    *scanchain;

  if(GrgWinDebug)
    fprintf(stderr,"Création du fenetrage.\n");

  for( scanchain = (chain_list*)(getptype( Signal->USER, RCN_FEQUI )->DATA ) ;
       scanchain ;
       scanchain = scanchain->NEXT
     )
    if( scanchain->DATA )
      break;

  if( scanchain )
  {
    /*
       Calcul de la taille occupee par le signal et du nombre
       de rectangle le constituant.
    */

    Win=(GrgWindow*)mbkalloc(sizeof(GrgWindow));
    Win->mx= INT_MAX;
    Win->my= INT_MAX;
    Win->Mx= INT_MIN;
    Win->My= INT_MIN;

    for( scanchain = (chain_list*)(getptype( Signal->USER, RCN_FEQUI )->DATA ) ;
         scanchain ;
         scanchain = scanchain->NEXT
       )
    {    

      Rec = (rdsrec_list*)(scanchain->DATA );
      if( !Rec )
        continue;
      End_Equi = 0;

      while(!End_Equi)
      {
        nbrec++;
        if(Rec->X < Win->mx) Win->mx = Rec->X; 
        if(Rec->Y < Win->my) Win->my = Rec->Y; 
        if(Rec->X+Rec->DX > Win->Mx) Win->Mx = Rec->X+Rec->DX;
        if(Rec->Y+Rec->DY > Win->My) Win->My = Rec->Y+Rec->DY;

        if(IsLynxEndEqui(Rec))
          End_Equi=1;
        else
          Rec=LYNX_EQUI(Rec);
      }
    }


    N=(long)(sqrt((double)nbrec));

    Win->Px=(Win->Mx-Win->mx)/N;
    Win->Py=(Win->My-Win->my)/N;

    if( Win->Px == 0 )
      Win->Px = 1;

    if( Win->Py == 0 )
      Win->Py = 1;
    
    Win->nx=(Win->Mx-Win->mx)/Win->Px+1;
    Win->ny=(Win->My-Win->my)/Win->Py+1;

    if(GrgWinDebug)
    {
      fprintf(stderr,"Taille du signal: (%ld;%ld)-(%ld;%ld)\n",Win->mx,
                                                               Win->my,
                                                               Win->Mx,
                                                               Win->My);
      fprintf(stderr,"Nombre de rectangles: %ld\n",nbrec);
      fprintf(stderr,"Nombre de fenetre X: %ld   Y: %ld\n",Win->nx,Win->ny);
      fprintf(stderr,"Taille de fenetres: %ldx%ld\n",Win->Px,Win->Py);
    }
    Win->Windows=(GrgWinChRec**)mbkalloc( Win->nx*Win->ny*sizeof(GrgWinChRec*));
    k=Win->nx*Win->ny;
    for(x=0;x<k;x++)
      *(Win->Windows+x)=NULL;  
    
    /* Rangement des rectangles dans leur fenetre */

    for( scanchain = (chain_list*)(getptype( Signal->USER, RCN_FEQUI )->DATA ) ;
         scanchain ;
         scanchain = scanchain->NEXT
       )
    {    
      Rec = (rdsrec_list*)(scanchain->DATA );
      if( !Rec )
        continue;
      End_Equi = 0;
      while(!End_Equi)
      {
        if(GrgWinDebug>=2)
        {
          fprintf(stderr,"Rectangle: (%6ld;%6ld)-(%6ld;%6ld) ",Rec->X,
                                                              Rec->Y,
                                                              Rec->X+Rec->DX,
                                                              Rec->Y+Rec->DY);
          for( ptdebug = Rec->USER ; ptdebug ; ptdebug = ptdebug->NEXT )
            printf("[%5ld-%08X] ", ptdebug->TYPE, (int)(ptdebug->DATA) );
          printf("\n");
        }
  
        x1=(Rec->X-Win->mx)/Win->Px;
        x2=(Rec->X+Rec->DX-Win->mx)/Win->Px;
        y1=(Rec->Y-Win->my)/Win->Py;
        y2=(Rec->Y+Rec->DY-Win->my)/Win->Py;
  
        x=x1; y=y1;
  
        /* Verification adjacence en bas et a gauche */
        if(x1>=1 && y1>=1)
        {
          fenetre.X= (x1-1)*Win->Px + Win->mx;
          fenetre.DX= Win->Px;
          fenetre.Y= (y1-1)*Win->Py + Win->my;
          fenetre.DY= Win->Py;
          fenetre.FLAGS= Rec->FLAGS;
  
          if(Intersect(&fenetre,Rec)!=NON_INTERSECT)
          {
            y=y1-1;
            x=x1-1;
          }
        }
  
        /* Verification adjacence à gauche */
        if(x1>=1)
        {
          fenetre.X= (x1-1)*Win->Px + Win->mx;
          fenetre.DX= Win->Px;
          fenetre.Y= y1*Win->Py + Win->my;
          fenetre.DY= Win->Py;
          fenetre.FLAGS= Rec->FLAGS;
  
          if(Intersect(&fenetre,Rec)!=NON_INTERSECT)
            x=x1-1;
        }
  
        /* Verification adjacence en bas */
        if(y1>=1)
        {
          fenetre.X= x1*Win->Px + Win->mx;
          fenetre.DX= Win->Px;
          fenetre.Y= (y1-1)*Win->Py + Win->my;
          fenetre.DY= Win->Py;
          fenetre.FLAGS= Rec->FLAGS;
  
          if(Intersect(&fenetre,Rec)!=NON_INTERSECT)
            y=y1-1;
        }
  
        x1=x;
        y1=y;
  
        if(GrgWinDebug>=2)
          fprintf(stderr,"x1=%ld; y1=%ld     x2=%ld; y2=%ld\n",x1,y1,x2,y2);
  
        for(x=x1;x<=x2 && x<Win->nx;x++)
          for(y=y1;y<=y2 && y<Win->ny;y++)
          {
            if(GrgWinDebug>=2)
              fprintf(stderr,"Fenetre: %ld,%ld\n",x,y);
            ptwinchrec=*(Win->Windows+y*Win->nx+x);
            ptwinchrec=addwinchrec(ptwinchrec,Rec);
            *(Win->Windows+y*Win->nx+x)=ptwinchrec;
  
            if( (ptGrgWin=getptype(Rec->USER,GrgWin)) )
              ptGrgWin->DATA=addchain(ptGrgWin->DATA,Win->Windows+y*Win->nx+x);
            else
              Rec->USER=addptype(Rec->USER,GrgWin,addchain(NULL,Win->Windows+y*Win->nx+x));
          }
  
        if(IsLynxEndEqui(Rec))
          End_Equi=1;
        else
          Rec=LYNX_EQUI(Rec);
      }
    }

    if(GrgWinDebug)
    {
      fprintf(stderr,"\nRectangle par fenetre.\n");
      for(x=0;x<k;x++)
      {
        fprintf(stderr,"Fenetre: %ld\n",x);
        ptwinchrec=*(Win->Windows+x);
        for(;ptwinchrec;ptwinchrec=ptwinchrec->NEXT)
        {
          for(i=0;i<ptwinchrec->POS;i++)
          {
            Rec=ptwinchrec->REC[i];
            fprintf( stderr,"Rectangle: (%6ld;%6ld)-(%6ld;%6ld) %s\n",
                     Rec->X,
                     Rec->Y,
                     Rec->X+Rec->DX,
                     Rec->Y+Rec->DY,
                     RDS_LAYER_NAME[ GetRdsLayer( Rec ) ]
                   );
           }
        }
      }

      fprintf(stderr,"\nFenetre par rectangle.\n");
      for( scanchain = (chain_list*)(getptype( Signal->USER, RCN_FEQUI )->DATA ) ;
           scanchain ;
           scanchain = scanchain->NEXT
         )
      {    
        Rec = (rdsrec_list*)(scanchain->DATA );
        if( !Rec )
          continue;
        End_Equi = 0;
        while(!End_Equi)
        { 
          fprintf(stderr,"Rectangle: (%6ld;%6ld)-(%6ld;%6ld)\n",Rec->X,
                                                              Rec->Y,
                                                              Rec->X+Rec->DX,
                                                              Rec->Y+Rec->DY);
          if(! (ptGrgWin=getptype(Rec->USER,GrgWin)))
          {
            fprintf(stderr,"ERREUR: Pas de fenetre associee à ce rectangle\n");
            exit(1745); 
          }

          for(sl=(chain_list*)(ptGrgWin->DATA);
              sl;
              sl=sl->NEXT)
            fprintf(stderr,"Fenetre: %ld\n",(((long)(sl->DATA))-((long)(Win->Windows)))/sizeof(void*));

          if(IsLynxEndEqui(Rec))
            End_Equi=1;
          else
            Rec=LYNX_EQUI(Rec);
        }
        fprintf(stderr,"Fin du fenetrage.\n");
      }
    }
  }

  /* Win=NULL si le signal n'a pas de rectangle */
  return(Win);
} 

chain_list* GrgGetInterWindows(Rec,WinInfo)
rdsrec_list	*Rec;
GrgWindow	*WinInfo;
{
  ptype_list *pt;
  chain_list *ScanchWin,*InterRec=NULL,*sc;
  GrgWinChRec *ScanchRec;
  mystack *pile,*scanmystack;
  rdsrec_list *R;
  long i;

  if(GrgWinDebug)
  {
    fprintf(stderr,"Debut procedure de recherche des intersections pour le\n");
    fprintf(stderr,"Rectangle: (%6ld;%6ld)-(%6ld;%6ld)\n",Rec->X,
                                                        Rec->Y,
                                                        Rec->X+Rec->DX,
                                                        Rec->Y+Rec->DY);
  }

  pt=getptype(Rec->USER,GrgWin);
  if(pt)
  {
    /* Pour eviter d'entrer deux fois un rectangle dans la liste des
       rectangles de sortie, on met le champs USER a la valeur 1 (
       valeur non cadree, impossible en fonctionnement normal). A la
       fin, on remet les bonnes valeurs que l'on avait sauvegardees 
       dans une pile.
    */

    pile=creatmystack();

    for(ScanchWin=(chain_list*)(pt->DATA);
        ScanchWin;
        ScanchWin=ScanchWin->NEXT)
    {
      if(GrgWinDebug)
        fprintf(stderr,"Fenetre: %ld\n",
  (((long)(ScanchWin->DATA))-((long)(WinInfo->Windows)))/sizeof(chain_list*));

      for(ScanchRec=*((GrgWinChRec**)(ScanchWin->DATA));
          ScanchRec;
          ScanchRec=ScanchRec->NEXT)
      {
        for(i=0;i<ScanchRec->POS;i++)
        {
          if(Intersect(Rec,ScanchRec->REC[i])!=NON_INTERSECT && 
             Rec!=ScanchRec->REC[i])
          {

            /* Ajout du nouveau rectangle dans la chaine InterRec si il
               n'y est deja pas present */

            if(GrgWinDebug)
              fprintf(stderr," (%6ld;%6ld)-(%6ld;%6ld) USERS: %X\n",
                             ScanchRec->REC[i]->X,
                             ScanchRec->REC[i]->Y,
                             ScanchRec->REC[i]->X+ScanchRec->REC[i]->DX,
                             ScanchRec->REC[i]->Y+ScanchRec->REC[i]->DY,
                             (int)(ScanchRec->REC[i]->USER) );

            R=ScanchRec->REC[i];
            if(R->USER!=(ptype_list*)1)
            {
              pile=addinmystack(pile,R->USER);
              InterRec=addchain(InterRec,R);
              R->USER=(ptype_list*)1; 
            }

          }
        }
      }
    } 
    if(GrgWinDebug)
      fprintf(stderr,"Fin de la procedure de recherche des intersections.\n");
    fflush(stderr); 

    /* Avant de renvoyer cette liste, on remet les (bonnes!) valeurs
       dans le champs USER des rectangles */

    scanmystack=pile;
    for(sc=InterRec;sc;sc=sc->NEXT)
    {
      scanmystack->CASEIN--;
      R=((rdsrec_list*)(sc->DATA));
      R->USER=scanmystack->PILE[scanmystack->CASEIN];

      if(GrgWinDebug)
        fprintf(stderr," (%6ld;%6ld)-(%6ld;%6ld) USERS: %X\n",
                       R->X,R->Y,R->X+R->DX,R->Y+R->DY,(int)(R->USER) );

      if(scanmystack->CASEIN==0)
        scanmystack=scanmystack->NEXT;
    }

    if(InterRec && scanmystack)
    {
      fprintf(stderr,"Windows95 :\n");
      fprintf(stderr,"Pas de concordance entre les rectangles et la pile.\n");
      exit(1);
    }
    freemystack(pile);
    return(InterRec);
  }
  fprintf(stderr,"ERREUR: Un rectangle n'est associe a aucune fenetre.\n");
  fprintf(stderr,"%s (%ld,%ld)-(%ld,%ld)\n", RDS_LAYER_NAME[ GetRdsLayer(Rec) ],
                                             Rec->X,
                                             Rec->Y,
                                             Rec->DX,
                                             Rec->DY
         );
  fflush(stderr);
  exit(4594);
}

void GrgClearWindows(WinInfo,Signal)
GrgWindow *WinInfo;
losig_list *Signal;
{
  rdsrec_list *Rec;
  char End_Equi;
  long pt; 
  GrgWinChRec *sc,*old;
  ptype_list *ptl;
  chain_list *scanchain;

  if(GrgWinDebug)
    fprintf(stderr,"Debut du nettoyage des fenetres.\n");

  for( scanchain = (chain_list*)(getptype( Signal->USER, RCN_FEQUI )->DATA ) ;
       scanchain ;
       scanchain = scanchain->NEXT
     )
  {    

    Rec = (rdsrec_list*)(scanchain->DATA );
    if( !Rec )
      continue;
    End_Equi=0;

    while(!End_Equi)
    {
      ptl=getptype(Rec->USER,GrgWin);
      if(!ptl)
      {
        fprintf(stderr,"Un rectangle n'appartient a aucune fenetre (1).\n");
        fprintf(stderr,"Rectangle: (%6ld;%6ld)-(%6ld;%6ld)\n",Rec->X,
                                                              Rec->Y,
                                                              Rec->X+Rec->DX,
                                                              Rec->Y+Rec->DY);
        exit(-1);
      }
      if(!ptl->DATA)
      {
        fprintf(stderr,"Un rectangle n'appartient a aucune fenetre (2).\n");
        fprintf(stderr,"Rectangle: (%6ld;%6ld)-(%6ld;%6ld)\n",Rec->X,
                                                              Rec->Y,
                                                              Rec->X+Rec->DX,
                                                              Rec->Y+Rec->DY);
        exit(-1);
      }
      freechain((chain_list*)(ptl->DATA));
      Rec->USER=delptype(Rec->USER,GrgWin);
      if(IsLynxEndEqui(Rec))
        End_Equi=1;
      else
        Rec=LYNX_EQUI(Rec);
    }
  }

  for(pt=0;pt<WinInfo->nx*WinInfo->ny;pt++)
  {
    old=NULL;
    for(sc= *(WinInfo->Windows+pt);sc;sc=sc->NEXT)
    {
      if(old)
      {
        old->NEXT=FREE_GrgWinChRec;
        FREE_GrgWinChRec=old;
      }
      old=sc;
    }
    if(old)
    {
      old->NEXT=FREE_GrgWinChRec;
      FREE_GrgWinChRec=old;
    }
  }

  mbkfree(WinInfo->Windows);
  mbkfree(WinInfo);

  if(GrgWinDebug)
    fprintf(stderr,"Nettoyage des fenetres termine.\n");
} 



/* Fonction de gestion de piles */

mystack* creatmystack()
{
  mystack *new;

  new=(mystack*)mbkalloc(sizeof(mystack));
  new->NEXT=NULL;
  new->CASEIN=0;
  return(new);
}

mystack* addinmystack(head,data)
mystack *head;
ptype_list *data;
{
  mystack *new;
  if(head->CASEIN==MAXSTACK)
  {
    new=creatmystack();
    new->NEXT=head;
    head=new;
  }

  head->PILE[head->CASEIN]=data;
  head->CASEIN++;

  return(head);
}

void freemystack(head)
mystack *head;
{
  mystack *scan,*old;

  old=NULL;
  for(scan=head;scan;scan=scan->NEXT)
  {
    if(old)
      mbkfree(old);
    old=scan;
  }
  if(old)
    mbkfree(old);
}

GrgWinChRec* addwinchrec(head,rec)
GrgWinChRec	*head;
rdsrec_list	*rec;
{
  GrgWinChRec *new;
  int i;

  if(!head)
  {
    if(FREE_GrgWinChRec)
    {
      new=FREE_GrgWinChRec;
      FREE_GrgWinChRec=FREE_GrgWinChRec->NEXT;
    }
    else
    {
      FREE_GrgWinChRec=(GrgWinChRec*)mbkalloc(sizeof(GrgWinChRec)*MXFREE);

      for(i=0;i<MXFREE-2;i++)
        FREE_GrgWinChRec[i].NEXT=FREE_GrgWinChRec+i+1;
      FREE_GrgWinChRec[MXFREE-2].NEXT=NULL;

      new=FREE_GrgWinChRec+MXFREE-1;
      new->NEXT=NULL;
    }
    new->NEXT=head;
    new->POS=0;
    head=new;
  }
  
  if(head->POS==GRGWINMAXREC)
  {
    if(FREE_GrgWinChRec)
    {
      new=FREE_GrgWinChRec;
      FREE_GrgWinChRec=FREE_GrgWinChRec->NEXT;
    }
    else
    {
      FREE_GrgWinChRec=(GrgWinChRec*)mbkalloc(sizeof(GrgWinChRec)*MXFREE);

      for(i=0;i<MXFREE-2;i++)
        FREE_GrgWinChRec[i].NEXT=FREE_GrgWinChRec+i+1;
      FREE_GrgWinChRec[MXFREE-2].NEXT=NULL;

      new=FREE_GrgWinChRec+MXFREE-1;
      new->NEXT=NULL;
    }

    new->NEXT=head;
    new->POS=0;
    head=new;
  }

  head->REC[head->POS]=rec;
  head->POS++;

  return(head);
}
