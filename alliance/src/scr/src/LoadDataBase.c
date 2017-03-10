/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
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

/******************************************************************************/
/*                                                                            */
/*                      Chaine de CAO & VLSI   Alliance                       */
/*                                                                            */
/*    Produit : STANDAD CELLS ROUTER                                          */
/*    Fichier : LoadDataBase.c                                                */
/*                                                                            */
/*    (c) copyright 1991 Laboratoire MASI equipe CAO & VLSI                   */
/*    Tous droits reserves                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                   */
/*                                                                            */
/*    Auteur(s) : El housseine REJOUAN                  le : 11/07/1991       */
/*                                                                            */
/*    Modifie par :                                     le : ../../....       */
/*    Modifie par :                                     le : ../../....       */
/*    Modifie par :                                     le : ../../....       */
/*                                                                            */
/******************************************************************************/
# include <mut.h>
# include <mlo.h> 
# include <mph.h> 
# include <apr.h> 
# include "SCR_Type.h"
# include "ViewDataBase.h"
# include "main.h"

/******************************************************************************/
/*  Fonction orient :  Calcul de l'orientation reelle d'une reference         */
/*   de type quelconque parmis NORTH/SOUTH/EAST/WEST en tenant compte         */
/*   de la transformation de l'instance.                                      */
/******************************************************************************/
char		Orientation(ORIENT,TRANSF)

char		ORIENT;
char		TRANSF;

{
 if (ORIENT == NORTH)
  switch(TRANSF) {
   case NOSYM : return (NORTH);
   case SYM_X : return (NORTH); 
   case SYM_Y : return (SOUTH);
   case SYMXY : return (SOUTH);
   case ROT_P : return (WEST);
   case SY_RP : return (EAST);
   case ROT_M : return (EAST);
   case SY_RM : return (WEST);
  }
 if (ORIENT == SOUTH)
  switch(TRANSF) {
   case NOSYM : return (SOUTH);
   case SYM_X : return (SOUTH);
   case SYM_Y : return (NORTH);
   case SYMXY : return (NORTH);
   case ROT_P : return (EAST);
   case SY_RP : return (WEST);
   case ROT_M : return (WEST);
   case SY_RM : return (EAST);
  }
 if (ORIENT == EAST)
  switch(TRANSF) {
   case NOSYM : return (EAST);
   case SYM_X : return (WEST);
   case SYM_Y : return (EAST);
   case SYMXY : return (WEST);
   case ROT_P : return (NORTH);
   case SY_RP : return (NORTH);
   case ROT_M : return (SOUTH);
   case SY_RM : return (SOUTH);
  }
 if (ORIENT == WEST)
  switch(TRANSF) {
   case NOSYM : return (WEST);
   case SYM_X : return (EAST); 
   case SYM_Y : return (WEST);
   case SYMXY : return (EAST);
   case ROT_P : return (SOUTH);
   case SY_RP : return (SOUTH);
   case ROT_M : return (NORTH);
   case SY_RM : return (NORTH);
  }
}

/******************************************************************************/
/*     Fonction Delta_X :  Calcul de la largeur de l'instance en tenant       */
/*                        compte de sa transformation.                        */
/******************************************************************************/
long		Delta_X(Xab1,Yab1,Xab2,Yab2,X,Y,TRANSF)

long		Xab1,Yab1,Xab2,Yab2,X,Y;
char		TRANSF;

{
 switch(TRANSF) {
  case NOSYM : return(X - Xab1);
  case SYM_X : return(-X + Xab2);
  case SYM_Y : return(X - Xab1);
  case SYMXY : return(Xab2 - Xab1 - Y + Yab1);
  case ROT_P : return(Yab2 - Y); 
  case ROT_M : return(Y - Yab1);
  case SY_RM : return(Yab2 - Y);
  case SY_RP : return(Y - Yab1);
 }
}

/******************************************************************************/
/*     Fonction Delta_Y :  Calcul de la hauteur de l'instance en tenant       */
/*                        compte de sa transformation.                        */
/******************************************************************************/
long		Delta_Y(Xab1,Yab1,Xab2,Yab2,X,Y,TRANSF)

long		Xab1,Yab1,Xab2,Yab2,X,Y;
char		TRANSF;

{
 switch(TRANSF) {
  case NOSYM : return(Y - Yab1);
  case SYM_X : return(Y - Yab1);
  case SYM_Y : return(Yab2 - Y);
  case SYMXY : return(Yab2 - Y);
  case ROT_P : return(X - Xab1);
  case ROT_M : return(Xab2 - X);
  case SY_RM : return(Xab2 - X);
  case SY_RP : return(X - Xab1);
 }
}

/******************************************************************************/
/*     Fonction Deltab_Y :  Calcul de la hauteur de l'instance en tenant      */
/*                        compte de sa transformation.                        */
/******************************************************************************/
long		Deltab_Y(Xab1,Yab1,Xab2,Yab2,TRANSF)

long		Xab1,Yab1,Xab2,Yab2;
char		TRANSF;

{
 switch(TRANSF) {
  case NOSYM : 
  case SYM_X :
  case SYM_Y :
  case SYMXY : return(Yab2 - Yab1); 
  case ROT_P :
  case ROT_M :
  case SY_RM :
  case SY_RP : return(Xab2 - Xab1);
 }
}

/******************************************************************************/
/*     Fonction Deltab_X :  Calcul de la largeur de l'instance en tenant      */
/*                        compte de sa transformation.                        */
/******************************************************************************/
long		Deltab_X(Xab1,Yab1,Xab2,Yab2,TRANSF)

long		Xab1,Yab1,Xab2,Yab2;
char		TRANSF;

{
 switch(TRANSF) {
  case NOSYM :
  case SYM_X :
  case SYM_Y :
  case SYMXY : return(Xab2 - Xab1);
  case ROT_P :
  case ROT_M :
  case SY_RM :
  case SY_RP : return(Yab2 - Yab1);
 }
}

/******************************************************************************/
/*		fonction	CreateInstance()		              */
/******************************************************************************/
Instance	*CreateInstance(InsName,FigName,X,Y,Width,Heigth,Transf)

char		*InsName;
char		*FigName;
long		X;
long		Y;
long		Width;
long		Heigth;
char		Transf;

{
 Instance 	*ptNewIns = NULL;

 ptNewIns = (Instance *) mbkalloc (sizeof(Instance));
 ptNewIns->NEXT    = NULL;
 ptNewIns->NAME    = InsName;
 ptNewIns->FIGNAME = FigName;
 ptNewIns->X       = X;
 ptNewIns->Y       = Y;
 ptNewIns->WIDTH   = Width;
 ptNewIns->HEIGTH  = Heigth;
 ptNewIns->TRANSF  = Transf;
 ptNewIns->CON     = NULL;
 ptNewIns->ALLOW   = NULL;
 
 return (ptNewIns);
}

/******************************************************************************/
/*		fonction	InsertInstance()		              */
/******************************************************************************/
Instance 	*InsertInstance(ptLine,InsName,FigName,X,Y,Width,Heigth,Transf)

Line 		*ptLine;
char		*InsName;
char		*FigName;
long		X;
long		Y;
long		Width;
long		Heigth;
char		Transf;

{

 Instance 	*CurrentIns = NULL;
 Instance 	*PrevIns    = NULL;
 Instance 	*ptNewIns   = NULL;
 long		XIns        = 0;
 long		WidthIns    = 0;

 for (CurrentIns = ptLine->INS; CurrentIns; CurrentIns = CurrentIns->NEXT) {
  XIns = CurrentIns->X;
  WidthIns = CurrentIns->WIDTH;
  if ((XIns == X) || 
      ((XIns < (X + Width)) && (X + Width) < (XIns + WidthIns)) ||
      ((XIns < X) && (X < (XIns + WidthIns)))) {
   fprintf(stderr,"scr_error :Overlaping of cells %s \n",CurrentIns->NAME);
   exit(1);
  }
  else if (X < XIns) break;
       else PrevIns = CurrentIns;
 }
 ptNewIns = CreateInstance(InsName,FigName,X,Y,Width,Heigth,Transf);
 if (!CurrentIns && !PrevIns) ptLine->INS = ptNewIns;
 else if (!CurrentIns && PrevIns) PrevIns->NEXT = ptNewIns;
      else if (CurrentIns && !PrevIns) {
            ptNewIns->NEXT = CurrentIns;
            ptLine->INS    = ptNewIns;
           }
           else {
            ptNewIns->NEXT = CurrentIns;
            PrevIns->NEXT  = ptNewIns;
           } 
 return(ptNewIns);
}
/******************************************************************************/
/*	fonction CreateLine()						      */
/******************************************************************************/
Line		*CreateLine(Type,X1,Y1,X2,Y2)

char		 Type;
long		 X1,Y1,X2,Y2;

{
 Line          *ptNewLine;
 
 ptNewLine = (Line *) mbkalloc (sizeof(Line));
 ptNewLine->NEXT      = NULL;
 ptNewLine->NAME      = NULL;
 ptNewLine->TYPE      = Type;
 ptNewLine->X	      = X1;
 ptNewLine->Y	      = Y1;
 ptNewLine->WIDTH     = X2-X1;
 ptNewLine->HEIGTH    = Y2-Y1;
 ptNewLine->INDEX     = 0;
 ptNewLine->INS	      = NULL;
 ptNewLine->SIG	      = NULL;
 ptNewLine->CON       = NULL; 
 ptNewLine->ALLOWUSED = NULL;
 ptNewLine->ALLOW     = NULL;
 ptNewLine->CHANNEL   = NULL;
 
 return(ptNewLine);
}

/******************************************************************************/
/*       fonction CreateSegment ()                                            */
/******************************************************************************/
Segment		*CreateSegment(Name,X1,Y1,X2,Y2,Width,Type,Layer)

char		*Name;
long		X1;
long		Y1;
long		X2;
long		Y2;
long		Width;
char		Type;
char		Layer;

{
 Segment	*ptNewSeg = NULL;

 ptNewSeg = (Segment *) mbkalloc (sizeof(Segment));
 ptNewSeg->NEXT  = NULL ;
 ptNewSeg->NAME  = Name ;
 ptNewSeg->X1    = X1 ;
 ptNewSeg->X2    = X2 ;
 ptNewSeg->Y1    = Y1 ;
 ptNewSeg->Y2    = Y2 ;
 ptNewSeg->WIDTH = Width ;
 ptNewSeg->TYPE  = Type ;
 ptNewSeg->LAYER = Layer ;

 return (ptNewSeg);
}
/******************************************************************************/
/*        fonction      InsertAllowInIns()                                    */
/******************************************************************************/
Segment *
InsertAllowInIns (ptCell, Xab1, Xab2, Yab1, Yab2, ptphInsfig)

     Instance *ptCell;
     long Xab1;
     long Xab2;
     long Yab1;
     long Yab2;
     phfig_list *ptphInsfig;

{
  Segment *ptNewAllow = NULL;
  phseg_list *ptphSeg = NULL;
  phseg_list *ptphSeg_dejavu = NULL;
  long X1 = 0;
  long X2 = 0;
  long Y1 = 0;
  long Y2 = 0;

  if (ptCell->ALLOW)
    printf ("[SCR] Allow builded twice for %s \n", ptCell->NAME);

  for (ptphSeg = ptphInsfig->PHSEG; ptphSeg; ptphSeg = ptphSeg->NEXT)
  {
    if (ptphSeg->LAYER == TALU2)
    {
      xyflat (&X1, &Y1, ptphSeg->X1, ptphSeg->Y1, ptCell->X, ptCell->Y,
              Xab1, Yab1, Xab2, Yab2, ptCell->TRANSF);

      /* fw : elimination des segments ALLOW empiles */
      for (ptphSeg_dejavu = ptCell->ALLOW;
           ptphSeg_dejavu && (X1 != ptphSeg_dejavu->X1);
           ptphSeg_dejavu = ptphSeg_dejavu->NEXT);

      if (!ptphSeg_dejavu)
      {
        xyflat (&X2, &Y2, ptphSeg->X2, ptphSeg->Y2, ptCell->X, ptCell->Y,
               Xab1, Yab1, Xab2, Yab2, ptCell->TRANSF);
        ptNewAllow = CreateSegment (ptphSeg->NAME, X1, (Y1 - ptCell->Y), X2,
                                    (Y2 - ptCell->Y), ptphSeg->WIDTH,
                                    ptphSeg->TYPE, ptphSeg->LAYER);
        ptNewAllow->NEXT = ptCell->ALLOW;
        ptCell->ALLOW = ptNewAllow;
      }
    }
  }
  return (ptCell->ALLOW);
}
/* forbidden 99/2/28
Segment		*InsertAllowInIns(ptCell,Xab1,Xab2,Yab1,Yab2,ptphInsfig)

Instance	*ptCell;
long      	Xab1;
long      	Xab2;
long      	Yab1;
long      	Yab2;
phfig_list	*ptphInsfig;

{
 Segment	*CurrentAllow = NULL;
 Segment	*PrevAllow    = NULL;
 Segment	*ptNewAllow   = NULL;
 phseg_list	*ptphSeg      = NULL;
 long      	X1   = 0;
 long      	X2   = 0;
 long      	Y1   = 0;
 long      	Y2   = 0;

 for (ptphSeg = ptphInsfig->PHSEG; ptphSeg; ptphSeg = ptphSeg->NEXT) {
  if(ptphSeg->LAYER == TALU2) {
   xyflat(&X1,&Y1,ptphSeg->X1,ptphSeg->Y1,ptCell->X,ptCell->Y,
          Xab1,Yab1,Xab2,Yab2,ptCell->TRANSF);
   xyflat(&X2,&Y2,ptphSeg->X2,ptphSeg->Y2,ptCell->X,ptCell->Y,
          Xab1,Yab1,Xab2,Yab2,ptCell->TRANSF);
   for (PrevAllow = NULL, CurrentAllow = ptCell->ALLOW; CurrentAllow;
        CurrentAllow = CurrentAllow->NEXT) {
    if (X1 > CurrentAllow->X1) break;
    PrevAllow = CurrentAllow;
   } 
   ptNewAllow = CreateSegment(ptphSeg->NAME,X1,(Y1 - ptCell->Y),X2,
                              (Y2 - ptCell->Y),ptphSeg->WIDTH,
                              ptphSeg->TYPE,ptphSeg->LAYER);
   if ((CurrentAllow == NULL) && (PrevAllow == NULL)) 
    ptCell->ALLOW = ptNewAllow;
   else if ((CurrentAllow == NULL) && PrevAllow) 
         PrevAllow->NEXT = ptNewAllow;
        else if (CurrentAllow && (PrevAllow == NULL)) {
              ptNewAllow->NEXT = CurrentAllow;
              ptCell->ALLOW    = ptNewAllow;
             }
             else {
              ptNewAllow->NEXT = CurrentAllow;
              PrevAllow->NEXT  = ptNewAllow;
             }
  }
 }
 return(ptNewAllow);
}
*/

/******************************************************************************/
/*	Fonction	CreateCon()                                           */
/******************************************************************************/
Connector	*CreateCon( Name,Type,Direction,Orient,X,Y,Width,
                           Layer,ptLine,ptIns)

char		*Name;
char		Type;
char		Direction;
char		Orient;
long            X;
long            Y;
long            Width;
char		Layer;
Line		*ptLine;
Instance	*ptIns;

{
 Connector      *ptNewCon = NULL;
 ptNewCon=(Connector *) mbkalloc (sizeof(Connector));
 ptNewCon->NEXT      = NULL;
 ptNewCon->NAME      = Name;
 ptNewCon->TYPE      = Type;
 ptNewCon->DIRECTION = Direction;
 ptNewCon->ORIENT    = Orient;
 ptNewCon->X         = X;
 ptNewCon->Y         = Y;
 ptNewCon->WIDTH     = Width;
 ptNewCon->LAYER     = Layer;
 ptNewCon->ORDER     = 0;	/* indique l'ordre du connecteur sur une face */
 ptNewCon->SIG       = NULL;
 ptNewCon->LINE      = ptLine;
 ptNewCon->INST      = ptIns;
 
 return(ptNewCon);
}

/******************************************************************************/
/*	Fonction InsertConInIns()		                              */
/******************************************************************************/
Connector	*InsertConInIns(
                               Name,Type,Direction,Orient,X,Y,
                               Width,Layer,ptLine,ptIns
                              )

char		*Name;
char		Type;
char		Direction;
char		Orient;
long		X;
long		Y;
long		Width;
char		Layer;
Line		*ptLine;
Instance	*ptIns;

{
 Connector	*ptScrCon = NULL;
 
 ptScrCon = CreateCon(Name,Type,Direction,Orient,X,(Y - ptIns->Y),
                      Width,Layer,ptLine,ptIns);
 if (ptIns->CON == NULL) ptIns->CON = ptScrCon;
 else {
  ptScrCon->NEXT = ptIns->CON;
  ptIns->CON     = ptScrCon;
 }
 return(ptScrCon);
}

/******************************************************************************/
/*      Fonction InsertConInFig()                                             */
/******************************************************************************/
Connector       *InsertConInFig( ptfig,Name,Type,Direction,Orient,X,Y,
                                Width,Layer,ptLine)
 
Figure          *ptfig;
char            *Name;
char            Type;
char            Direction;
char            Orient;
long            X;
long            Y;
long            Width;
char            Layer;
Line            *ptLine;
 
{
 Connector      *ptNewScrCon = NULL;
 Connector      *ptScrCon = NULL;
 
 ptNewScrCon = CreateCon(Name,Type,Direction,Orient,X,Y,Width,Layer,ptLine,NULL);
 if (ptfig->CON == NULL) ptfig->CON = ptNewScrCon;
 else {  
  for(ptScrCon = ptfig->CON; ptScrCon->NEXT; ptScrCon = ptScrCon->NEXT);
  ptScrCon->NEXT = ptNewScrCon;
 }   
 return(ptNewScrCon);
} 

/******************************************************************************/
/*    Fonction	CreateSignal()	                      		              */
/******************************************************************************/
Signal		*CreateSignal(Name,Index,Type)

char		*Name;
long		Index;
char		Type;

{
 Signal		*ptNewSig=NULL;
 
 ptNewSig=(Signal *) mbkalloc (sizeof(Signal));
 ptNewSig->NEXT   = NULL;
 ptNewSig->NAME   = Name;
 ptNewSig->INDEX  = Index;
 ptNewSig->TYPE   = Type;
 ptNewSig->CON    = NULL;
 ptNewSig->LINE   = NULL;
 ptNewSig->MAXCON = 0;
 
 return(ptNewSig);
}

/******************************************************************************/
/*	fonction InsertSignal()		                      	              */
/******************************************************************************/
Signal		*InsertSignal(ptRoot,Name,Index,Type,ptCon,ptLine)

Figure		*ptRoot;
char		*Name;
long		Index;
long		Type;
Connector	*ptCon;
Line		*ptLine;
	
{
 Signal		*ptNewSig  = NULL;
 
 if (ptRoot->SIG == NULL) {
  ptRoot->SIG = ptNewSig = CreateSignal(Name,Index,Type);
  ptNewSig->CON  = addchain(ptNewSig->CON,((char *) ptCon));
  ptNewSig->LINE = addchain(ptNewSig->LINE,((char *) ptLine));
 }
 else {
  ptNewSig = CreateSignal(Name,Index,Type);
  ptNewSig->NEXT = ptRoot->SIG;
  ptRoot->SIG    = ptNewSig;
  ptNewSig->CON  = addchain(ptNewSig->CON,((char *) ptCon));
  ptNewSig->LINE = addchain(ptNewSig->LINE,((char *) ptLine));
 }
 return (ptNewSig);
}

/******************************************************************************/
/* Function : ScrOrganizeXCon()                                               */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*      Input parameters :                                                    */
/*      Input global Variables : none                                         */
/*      Output parameters :  none                                             */
/*      Output global Variables : none                                        */
/*                                                                            */
/******************************************************************************/
void		ScrOrganizeXCon(LeadScrCon,Orient)

Connector	*LeadScrCon;
char		Orient;

{
 Connector	*ptScrphcon = NULL;
 Connector	*NextptScrphcon = NULL;
 long		ConOrder = -1;
 long		NextConOrder = -1;
 long		XCon = 0;
 long		XNextCon = 0;
 long		XAuxCon = 0;

/*
fprintf(stderr,"##### AVANT TRIE #####\n");
for (ptScrphcon = LeadScrCon; ptScrphcon; ptScrphcon = ptScrphcon->NEXT) {
 if ((ptScrphcon->ORIENT == Orient) && (ptScrphcon->ORDER != -1)) 
 fprintf (stderr,"NAME : %s , ORIENT : %c , X : %ld , Y : %ld , ORDER : %ld\n",ptScrphcon->NAME,ptScrphcon->ORIENT,ptScrphcon->X,ptScrphcon->Y,ptScrphcon->ORDER);
}
*/

 for(ptScrphcon = LeadScrCon; ptScrphcon; ptScrphcon = ptScrphcon->NEXT) {
  ConOrder = ptScrphcon->ORDER;
  if ((ptScrphcon->ORIENT == Orient) && (ConOrder != -1)) {
   for(NextptScrphcon = ptScrphcon->NEXT; NextptScrphcon; NextptScrphcon = NextptScrphcon->NEXT) {
    NextConOrder = NextptScrphcon->ORDER;
    if ((NextptScrphcon->ORIENT == Orient) && (NextConOrder != -1)) {
     if (ConOrder < NextConOrder) {
      if ((XCon = ptScrphcon->X) > (XNextCon = NextptScrphcon->X)) {
       XAuxCon  = XCon;
       ptScrphcon->X = XNextCon;
       NextptScrphcon->X = XAuxCon;
      }
     }
    }
   }
  }
 }

/*
fprintf(stderr,"##### APRES TRIE #####\n");
for (ptScrphcon = LeadScrCon; ptScrphcon; ptScrphcon = ptScrphcon->NEXT) {
 if ((ptScrphcon->ORIENT == Orient) && (ptScrphcon->ORDER != -1)) 
 fprintf (stderr,"NAME : %s , ORIENT : %c , X : %ld , Y : %ld , ORDER : %ld\n",ptScrphcon->NAME,ptScrphcon->ORIENT,ptScrphcon->X,ptScrphcon->Y,ptScrphcon->ORDER);
}
*/

}

/******************************************************************************/
/* Function : MakeFatherCon()                                                 */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*      Input parameters :                                                    */
/*      Input global Variables : none                                         */
/*      Output parameters :  none                                             */
/*      Output global Variables : none                                        */
/*                                                                            */
/******************************************************************************/
void		MakeFatherCon(ptfig,ptphfig,ptlofig,Leadphcon)

Figure		*ptfig;
phfig_list	*ptphfig;
lofig_list	*ptlofig;
PlaceConList	*Leadphcon;

{
 CaracConList	*ptphCon       = NULL;
 locon_list	*ptloCon       = NULL;
 losig_list	*ptMbkSig      = NULL;
 Line		*ptLine        = NULL;
 Line		*FirstCellLine = NULL;
 Line		*LastCellLine  = NULL;
 Line		*ptSaveLine    = NULL;
 Line		*DownLine      = NULL;
 Line		*UpLine        = NULL;
 chain_list	*LineList      = NULL;
 chain_list	*ConList       = NULL;
 Connector	*ptNewCon      = NULL;
 Connector	*ptCon         = NULL;
 Connector	*ptScrphcon    = NULL;
 Signal		*ptSigFig      = NULL;
 long		XConSave       = 0;
 long		FirstIndex     = 0;
 long		LastIndex      = 0;
 long		IndexLine      = 0;
 long		RightXCon      = 0;
 long		LeftXCon       = 0;
 long		XConFig        = 0;
 long		YConFig        = 0;
 long		Order          = 0;
 long		Counter        = 0;
 long		ConWidth       = 0;
 char		TypeLine;
 char		ConLayer;
 char		Orient;
 char		*LoNameCon;
 char		*PhNameCon;
 BOOLEAN	NotFindInRight = TRUE;
 BOOLEAN	NotFindInLeft = TRUE;
 BOOLEAN	NotStopLeft = TRUE;
 BOOLEAN	NotStopRight = TRUE;

 /* reperer la 1ere et la derniere ligne */
 DownLine = ptfig->LINE;
 FirstCellLine = DownLine->NEXT->NEXT; 
 FirstIndex = FirstCellLine->INDEX;
 for (LastCellLine = FirstCellLine; LastCellLine->NEXT->NEXT->NEXT;
      LastCellLine = LastCellLine->NEXT->NEXT);
 UpLine = LastCellLine->NEXT->NEXT;
 LastIndex = LastCellLine->INDEX;

 /* on place les connecteurs de la figure mere qui n'ont pas de contraintes 
    physiques */
 for(ptloCon = ptlofig->LOCON; ptloCon; ptloCon = ptloCon->NEXT) {
  LoNameCon = namealloc(ptloCon->NAME);
  if (!(isvdd(LoNameCon) || isvss(LoNameCon))) {
   if (Leadphcon) {
    for(ptphCon = Leadphcon->NORTH_CON; ptphCon; ptphCon = ptphCon->NEXT) {
     PhNameCon = namealloc(ptphCon->NAME);
     if (LoNameCon == PhNameCon) break;
    }
 
    if (ptphCon == NULL) 
     for(ptphCon = Leadphcon->SOUTH_CON; ptphCon; ptphCon = ptphCon->NEXT) {
      PhNameCon = namealloc(ptphCon->NAME);
      if (LoNameCon == PhNameCon) break;
     }
 
    if (ptphCon == NULL) 
     for(ptphCon = Leadphcon->WEST_CON; ptphCon; ptphCon = ptphCon->NEXT) {
      PhNameCon = namealloc(ptphCon->NAME);
      if (LoNameCon == PhNameCon) break;
     }
 
    if (ptphCon == NULL) 
     for(ptphCon = Leadphcon->EAST_CON; ptphCon; ptphCon = ptphCon->NEXT) {
      PhNameCon = namealloc(ptphCon->NAME);
      if (LoNameCon == PhNameCon) break;
     }
   }
 
   if (ptphCon == NULL) { /* n'appartient pas aux connecteurs preplaces */
   /* on le place */
    for (ptSigFig = ptfig->SIG; (ptSigFig && 
         (ptSigFig->INDEX != ptloCon->SIG->INDEX));
         ptSigFig = ptSigFig->NEXT);
    if (ptSigFig) {
     ptSaveLine = NULL;
     for (LineList = ptSigFig->LINE; LineList; LineList = LineList->NEXT) {
      ptLine = ((Line *) LineList->DATA);
      IndexLine = ptLine->INDEX;
      TypeLine  = ptLine->TYPE;
      if (TypeLine == CELL) {
       ptSaveLine = ptLine;
       if (IndexLine == FirstIndex) {
        for (ConList = ptLine->CON; ConList; ConList = ConList->NEXT) {
         ptCon = ((Connector *) ConList->DATA);
         if ((ptCon->ORIENT == SOUTH) && (ptCon->SIG->INDEX == ptSigFig->INDEX)) 
          break;
        }
        ptNewCon = InsertConInFig(ptfig,ptloCon->NAME,ptloCon->TYPE,
                                  ptloCon->DIRECTION,ptCon->ORIENT,ptCon->X,
                                  ptphfig->YAB1,LAYER2WIDTH,ALU2,DownLine);
        ptNewCon->ORDER  = -1;
        ptNewCon->SIG    = ptSigFig;
        DownLine->CON = addchain(DownLine->CON,((Connector *) ptNewCon));
        DownLine->SIG = addchain(DownLine->SIG,((Signal *) ptSigFig));
        ptSigFig->CON    = addchain(ptSigFig->CON,((Connector *) ptNewCon));
        ptSigFig->LINE   = addchain(ptSigFig->LINE,((Line *) DownLine));
        break;
       }
       else if (IndexLine == LastIndex) {
             for (ConList = ptLine->CON; ConList; ConList = ConList->NEXT) {
              ptCon = ((Connector *) ConList->DATA);
              if ((ptCon->ORIENT == NORTH) && 
                  (ptCon->SIG->INDEX == ptSigFig->INDEX)) break;
             }
             ptNewCon = InsertConInFig(ptfig,ptloCon->NAME,ptloCon->TYPE,
                                       ptloCon->DIRECTION,ptCon->ORIENT,
                                       ptCon->X,ptphfig->YAB2,LAYER2WIDTH,
                                       ALU2,UpLine);
             ptNewCon->ORDER  = -1;
             ptNewCon->SIG    = ptSigFig;
             UpLine->CON = addchain(UpLine->CON,((Connector *) ptNewCon));
             UpLine->SIG = addchain(UpLine->SIG,((Signal *) ptSigFig));
             ptSigFig->CON    = addchain(ptSigFig->CON,((Connector *) ptNewCon));
             ptSigFig->LINE   = addchain(ptSigFig->LINE,((Line *) UpLine));
             break;
            }
      }
     }
     if (LineList == NULL) {
      for (ptLine = ptfig->LINE->NEXT; (ptLine->NEXT != ptSaveLine);
           ptLine = ptLine->NEXT->NEXT);
      for (ConList = ptSaveLine->CON; ConList; ConList = ConList->NEXT) {
       ptCon = ((Connector *) ConList->DATA);
       if (ptCon->SIG->INDEX == ptSigFig->INDEX) break;
      }
      if ((ptCon->X - ptphfig->XAB1) >= (ptphfig->XAB2 - ptCon->X)) {
       Orient = EAST;
       XConFig = ptphfig->XAB2;
      }
      else {
       Orient = WEST;
       XConFig = ptphfig->XAB1;
      }
       ptNewCon = InsertConInFig(ptfig,ptloCon->NAME,ptloCon->TYPE,
                                ptloCon->DIRECTION,Orient,
                                XConFig,ptLine->Y,LAYER1WIDTH,ALU1,ptLine);
      ptNewCon->ORDER  = -1;
      ptNewCon->SIG  = ptSigFig;
      ptLine->CON    = addchain(ptLine->CON, ((Connector *) ptNewCon));
      ptLine->SIG    = addchain(ptLine->SIG,((Signal *) ptSigFig));
      ptSigFig->CON  = addchain(ptSigFig->CON,((Connector *) ptNewCon));
      ptSigFig->LINE = addchain(ptSigFig->LINE,((Line *) ptLine));
     }
    }
    else {
     fprintf(stderr,"scr_warning : the connector : %s is not connected to any signal !\n", ptloCon->NAME);
    }
   }
  }
/*
  else if (isvdd(LoNameCon)) NameVdd = LoNameCon;
       else if (isvss(LoNameCon)) NameVss = LoNameCon;
*/
 }

/******************************************************************************/
/* traitement des connecteurs pre-places */
/******************************************************************************/
 if (Leadphcon != NULL) {
/******************************************************************************/
/*	Traitement des connecteurs sur la face SOUTH                          */
/******************************************************************************/
  for(ptphCon = Leadphcon->SOUTH_CON; ptphCon; ptphCon = ptphCon->NEXT) {
   PhNameCon = namealloc(ptphCon->NAME);
   Orient = SOUTH;
   Order = ptphCon->USER;
   if (!(isvdd(PhNameCon) || isvss(PhNameCon))) {
    NotFindInRight = TRUE;
    NotFindInLeft = TRUE;
    NotStopRight = TRUE;
    NotStopLeft = TRUE;
    for(ptloCon = ptlofig->LOCON; ptloCon; ptloCon = ptloCon->NEXT) {
     LoNameCon = namealloc(ptloCon->NAME);
     if (LoNameCon == PhNameCon) break;
    }
    if (ptloCon == NULL) {
     fprintf(stderr,"scr_error : the connector : %s does not exist in the netlist !\n", ptphCon->NAME);
     exit(1);
    }
    for (ptSigFig = ptfig->SIG; (ptSigFig && (ptSigFig->INDEX != ptloCon->SIG->INDEX)); ptSigFig = ptSigFig->NEXT);
    if (ptSigFig == NULL) {
     ptMbkSig = ptloCon->SIG;
     ptSigFig = CreateSignal(getsigname(ptMbkSig),ptMbkSig->INDEX,ptMbkSig->TYPE);
     ptSigFig->NEXT = ptfig->SIG;
     ptfig->SIG     = ptSigFig;
    }
    YConFig = ptphfig->YAB1;
    for (ConList = FirstCellLine->CON; ConList; ConList = ConList->NEXT) {
     ptCon = ((Connector *) ConList->DATA);
     if ((ptCon->ORIENT == SOUTH) && (ptCon->SIG->INDEX == ptSigFig->INDEX)) 
      break;
    }
    if (ConList != NULL) XConSave = ptCon->X;
    else {
     for (LineList = ptSigFig->LINE; LineList; LineList = LineList->NEXT) {
      ptLine = ((Line *) LineList->DATA);
      if (ptLine->TYPE == CELL) {
       for (ConList = ptLine->CON; ConList; ConList = ConList->NEXT) {
        ptCon = ((Connector *) ConList->DATA);
        if ((ptCon->ORIENT == SOUTH) && (ptCon->SIG->INDEX == ptSigFig->INDEX)) {
         RightXCon = LeftXCon = XConSave = ptCon->X;
         LineList == NULL;
         break;
        }
       }
      }
     }
    }
    for(ptScrphcon = ptfig->CON; ptScrphcon; ptScrphcon = ptScrphcon->NEXT) {
     if ((ptScrphcon->ORIENT == SOUTH) && (ptScrphcon->X == XConSave)) 
      break; 
    }
    if (ptScrphcon == NULL) {
     XConFig = XConSave;
    }
    else {
     while (NotFindInRight && NotFindInLeft) {
      if (!((NotStopRight) && (NotStopLeft))) {
       fprintf(stderr,"Impossible configuration : the number of connectors to be placed on south side is greater than the existing number of pitchs. \n");
       exit(1);
      }
      if (NotStopRight) RightXCon += PITCH_X;
      if (NotStopLeft) LeftXCon   -= PITCH_X;
      if (RightXCon >= ptphfig->XAB2) {
       NotStopRight = FALSE;
      }
      if (LeftXCon <= ptphfig->XAB1) {
       NotStopLeft = FALSE;
      }
      for(ptScrphcon = ptfig->CON; (NotStopRight && NotFindInLeft && ptScrphcon); ptScrphcon = ptScrphcon->NEXT) {
       if ((ptScrphcon->ORIENT == SOUTH) && (ptScrphcon->X == RightXCon)) 
        break; 
      }
      if (NotStopRight && NotFindInLeft && (ptScrphcon == NULL)) {
       XConFig = RightXCon;
       NotFindInRight = FALSE;
      }
      for(ptScrphcon = ptfig->CON; (NotStopLeft && NotFindInRight && ptScrphcon); ptScrphcon = ptScrphcon->NEXT) {
       if ((ptScrphcon->ORIENT == SOUTH) && (ptScrphcon->X == LeftXCon)) 
       break; 
      }
      if (NotStopLeft && NotFindInRight && (ptScrphcon == NULL)) {
       XConFig = LeftXCon;
       NotFindInLeft = FALSE;
      }
     }
    }
    ptLine = DownLine;
    ConWidth = LAYER2WIDTH;
    ConLayer = ALU2;
    ptNewCon = InsertConInFig(ptfig,ptloCon->NAME,ptloCon->TYPE,
                              ptloCon->DIRECTION,Orient,
                              XConFig,YConFig,ConWidth,ConLayer,ptLine);
    ptNewCon->ORDER = Order;
    ptNewCon->SIG   = ptSigFig;
    ptLine->CON     = addchain(ptLine->CON, ((Connector *) ptNewCon));
    ptLine->SIG     = addchain(ptLine->SIG,((Signal *) ptSigFig));
    ptSigFig->CON   = addchain(ptSigFig->CON,((Connector *) ptNewCon));
    ptSigFig->LINE  = addchain(ptSigFig->LINE,((Line *) ptLine));
   }
  }
  ScrOrganizeXCon(ptfig->CON,Orient);
/******************************************************************************/
/*	Traitement des connecteurs sur la face NORTH                          */
/******************************************************************************/
  for(ptphCon = Leadphcon->NORTH_CON; ptphCon; ptphCon = ptphCon->NEXT) {
   PhNameCon = namealloc(ptphCon->NAME);
   Orient = NORTH;
   Order = ptphCon->USER;

   if (!(isvdd(PhNameCon) || isvss(PhNameCon))) {
    NotFindInRight = TRUE;
    NotFindInLeft = TRUE;
    NotStopRight = TRUE;
    NotStopLeft = TRUE;
    for(ptloCon = ptlofig->LOCON; ptloCon; ptloCon = ptloCon->NEXT) {
     LoNameCon = namealloc(ptloCon->NAME);
     if (LoNameCon == PhNameCon) break;
    }
    if (ptloCon == NULL) {
     fprintf(stderr,"scr_error : the connector : %s does not exist in the netlist !\n", ptphCon->NAME);
     exit(1);
    }
    for (ptSigFig = ptfig->SIG; (ptSigFig && (ptSigFig->INDEX != ptloCon->SIG->INDEX)); ptSigFig = ptSigFig->NEXT);
    if (ptSigFig == NULL) {
     ptMbkSig = ptloCon->SIG;
     ptSigFig = CreateSignal(getsigname(ptMbkSig),ptMbkSig->INDEX,ptMbkSig->TYPE);
     ptSigFig->NEXT = ptfig->SIG;
     ptfig->SIG     = ptSigFig;
    }
    YConFig = ptphfig->YAB2;
    for (ConList = FirstCellLine->CON; ConList; ConList = ConList->NEXT) {
     ptCon = ((Connector *) ConList->DATA);
     if ((ptCon->ORIENT == NORTH) && (ptCon->SIG->INDEX == ptSigFig->INDEX)) 
      break;
    }
    if (ConList != NULL) XConSave = ptCon->X;
    else {
     for (LineList = ptSigFig->LINE; LineList; LineList = LineList->NEXT) {
      ptLine = ((Line *) LineList->DATA);
      if (ptLine->TYPE == CELL) {
       for (ConList = ptLine->CON; ConList; ConList = ConList->NEXT) {
        ptCon = ((Connector *) ConList->DATA);
        if ((ptCon->ORIENT == NORTH) && (ptCon->SIG->INDEX == ptSigFig->INDEX)) {
         RightXCon = LeftXCon = XConSave = ptCon->X;
         LineList == NULL;
         break;
        }
       }
      }
     }
    }
    for(ptScrphcon = ptfig->CON; ptScrphcon; ptScrphcon = ptScrphcon->NEXT) {
     if ((ptScrphcon->ORIENT == NORTH) && (ptScrphcon->X == XConSave)) 
      break; 
    }
    if (ptScrphcon == NULL) {
     XConFig = XConSave;
    }
    else {
     while (NotFindInRight && NotFindInLeft) {
      if (!((NotStopRight) && (NotStopLeft))) {
       fprintf(stderr,"Impossible configuration : the number of connectors to be placed on north side is greater than the existing number of pitchs.\n");
       exit(1);
      }
      if (NotStopRight) RightXCon += PITCH_X;
      if (NotStopLeft) LeftXCon   -= PITCH_X;
      if (RightXCon >= ptphfig->XAB2) {
       NotStopRight = FALSE;
      }
      if (LeftXCon <= ptphfig->XAB1) {
       NotStopLeft = FALSE;
      }
      for(ptScrphcon = ptfig->CON; (NotStopRight && NotFindInLeft && ptScrphcon); ptScrphcon = ptScrphcon->NEXT) {
       if ((ptScrphcon->ORIENT == NORTH) && (ptScrphcon->X == RightXCon)) 
        break; 
      }
      if (NotStopRight && NotFindInLeft && (ptScrphcon == NULL)) {
       XConFig = RightXCon;
       NotFindInRight = FALSE;
      }
      for(ptScrphcon = ptfig->CON; (NotStopLeft && NotFindInRight && ptScrphcon); ptScrphcon = ptScrphcon->NEXT) {
       if ((ptScrphcon->ORIENT == NORTH) && (ptScrphcon->X == LeftXCon)) 
       break; 
      }
      if (NotStopLeft && NotFindInRight && (ptScrphcon == NULL)) {
       XConFig = LeftXCon;
       NotFindInLeft = FALSE;
      }
     }
    }
    ptLine = UpLine;
    ConWidth = LAYER2WIDTH;
    ConLayer = ALU2;
    ptNewCon = InsertConInFig(ptfig,ptloCon->NAME,ptloCon->TYPE,
                              ptloCon->DIRECTION,Orient,
                              XConFig,YConFig,ConWidth,ConLayer,ptLine);
    ptNewCon->ORDER = Order;
    ptNewCon->SIG   = ptSigFig;
    ptLine->CON     = addchain(ptLine->CON, ((Connector *) ptNewCon));
    ptLine->SIG     = addchain(ptLine->SIG,((Signal *) ptSigFig));
    ptSigFig->CON   = addchain(ptSigFig->CON,((Connector *) ptNewCon));
    ptSigFig->LINE  = addchain(ptSigFig->LINE,((Line *) ptLine));
   }
  }
  ScrOrganizeXCon(ptfig->CON,Orient);
/******************************************************************************/
/*	Traitement des connecteurs sur la face WEST                           */
/******************************************************************************/
  for(ptphCon = Leadphcon->WEST_CON; ptphCon; ptphCon = ptphCon->NEXT) {
   PhNameCon = namealloc(ptphCon->NAME);
   Orient = WEST;
   Order = ptphCon->USER;
   if (!(isvdd(PhNameCon) || isvss(PhNameCon))) {
    for(ptloCon = ptlofig->LOCON; ptloCon; ptloCon = ptloCon->NEXT) {
     LoNameCon = namealloc(ptloCon->NAME);
     if (LoNameCon == PhNameCon) break;
    }
    if (ptloCon == NULL) {
     fprintf(stderr,"scr_error : the connector : %s does not exist in the netlist !\n", ptphCon->NAME);
     exit(1);
    }
    for (ptSigFig = ptfig->SIG; (ptSigFig && (ptSigFig->INDEX != ptloCon->SIG->INDEX)); 
         ptSigFig = ptSigFig->NEXT);
    if (ptSigFig == NULL) {
     ptMbkSig = ptloCon->SIG;
     ptSigFig = CreateSignal(getsigname(ptMbkSig),ptMbkSig->INDEX,ptMbkSig->TYPE);
     ptSigFig->NEXT = ptfig->SIG;
     ptfig->SIG     = ptSigFig;
    }
    for (LineList = ptSigFig->LINE; LineList; LineList = LineList->NEXT) {
     ptLine = ((Line *) (LineList->DATA));
     if (ptLine->TYPE == CELL) {
      ptSaveLine = ptLine;
      break;
     }
    }
    for (ptLine = ptfig->LINE->NEXT; (ptLine->NEXT != ptSaveLine); ptLine = ptLine->NEXT->NEXT);
    ptNewCon = InsertConInFig(ptfig,ptloCon->NAME,ptloCon->TYPE,
                              ptloCon->DIRECTION,Orient,
                              ptphfig->XAB1,ptLine->Y,LAYER1WIDTH,ALU1,ptLine);
    ptNewCon->ORDER  = Order;
    ptNewCon->SIG    = ptSigFig;
    ptLine->CON      = addchain(ptLine->CON, ((Connector *) ptNewCon));
    ptLine->SIG      = addchain(ptLine->SIG,((Signal *) ptSigFig));
    ptSigFig->CON    = addchain(ptSigFig->CON,((Connector *) ptNewCon));
    ptSigFig->LINE   = addchain(ptSigFig->LINE,((Line *) ptLine));
   }
  }
/******************************************************************************/
/*	Traitement des connecteurs sur la face EAST                           */
/******************************************************************************/
  for(ptphCon = Leadphcon->EAST_CON; ptphCon; ptphCon = ptphCon->NEXT) {
   PhNameCon = namealloc(ptphCon->NAME);
   Orient = EAST;
   Order = ptphCon->USER;
   if (!(isvdd(PhNameCon) || isvss(PhNameCon))) {
    for(ptloCon = ptlofig->LOCON; ptloCon; ptloCon = ptloCon->NEXT) {
     LoNameCon = namealloc(ptloCon->NAME);
     if (LoNameCon == PhNameCon) break;
    }
    if (ptloCon == NULL) {
     fprintf(stderr,"scr_error : the connector : %s does not exist in the netlist !\n", ptphCon->NAME);
     exit(1);
    }
    for (ptSigFig = ptfig->SIG; (ptSigFig && (ptSigFig->INDEX != ptloCon->SIG->INDEX)); 
         ptSigFig = ptSigFig->NEXT);
    if (ptSigFig == NULL) {
     ptMbkSig = ptloCon->SIG;
     ptSigFig = CreateSignal(getsigname(ptMbkSig),ptMbkSig->INDEX,ptMbkSig->TYPE);
     ptSigFig->NEXT = ptfig->SIG;
     ptfig->SIG     = ptSigFig;
    }
    for (LineList = ptSigFig->LINE; LineList; LineList = LineList->NEXT) {
     ptLine = ((Line *) (LineList->DATA));
     if (ptLine->TYPE == CELL) {
      ptSaveLine = ptLine;
      break;
     }
    }
    for (ptLine = ptfig->LINE->NEXT; (ptLine->NEXT != ptSaveLine); ptLine = ptLine->NEXT->NEXT);
    ptNewCon = InsertConInFig(ptfig,ptloCon->NAME,ptloCon->TYPE,
                              ptloCon->DIRECTION,Orient,
                              ptphfig->XAB2,ptLine->Y,LAYER1WIDTH,ALU1,ptLine);
    ptNewCon->ORDER  = Order;
    ptNewCon->SIG    = ptSigFig;
    ptLine->CON      = addchain(ptLine->CON, ((Connector *) ptNewCon));
    ptLine->SIG      = addchain(ptLine->SIG,((Signal *) ptSigFig));
    ptSigFig->CON    = addchain(ptSigFig->CON,((Connector *) ptNewCon));
    ptSigFig->LINE   = addchain(ptSigFig->LINE,((Line *) ptLine));
   }
  }
 }
}


/******************************************************************************/
/*	fonction FillFigure()	                 		              */
/******************************************************************************/
void		FillFigure(ptRoot,ptphfig,ptlofig,HeadphconList)

Figure		*ptRoot;
phfig_list	*ptphfig;
lofig_list	*ptlofig;
PlaceConList	*HeadphconList;

{
 phins_list	*ptphIns    = NULL;
 phfig_list	*ptphInsfig = NULL;
 loins_list	*ptloIns    = NULL;
 Line		*ptLine     = NULL;
 Instance	*CurrentIns = NULL;
 Connector	*ptScrCon   = NULL;
 phcon_list	*ptphCon    = NULL;
 locon_list	*ptloCon    = NULL;
 Signal		*ptScrSig   = NULL;
 Signal		*ptRootSig  = NULL;
 chain_list	*LineList   = NULL;
 chain_list	*SigList    = NULL;
 losig_list	*ptSig      = NULL;
 Segment	*ptScrAllow = NULL;
 long		Xab1        = 0;
 long		Xab2        = 0;
 long		Yab1        = 0;
 long		Yab2        = 0;
 long		XlInsFig    = 0;
 long		XrInsFig    = 0;
 long		YuInsFig    = 0;
 long		YdInsFig    = 0;
 long		YuLineFig   = 0;
 long		YdLineFig   = 0;
 long		XConLine    = 0;
 long		YConLine    = 0;
 char		Transf;
 char		*phName;
 char		Orient;

 for (ptphIns = ptphfig->PHINS ; ptphIns; ptphIns = ptphIns->NEXT) {
  ptphInsfig = getphfig(ptphIns->FIGNAME,'A');
  sx2sc (ptphInsfig);
  Xab1 = ptphInsfig->XAB1;
  Yab1 = ptphInsfig->YAB1;
  Xab2 = ptphInsfig->XAB2;
  Yab2 = ptphInsfig->YAB2;
  Transf = ptphIns->TRANSF;
  XlInsFig = ptphIns->XINS;
  YdInsFig = ptphIns->YINS;
  XrInsFig = XlInsFig + Deltab_X(Xab1, Yab1, Xab2, Yab2, Transf);
  YuInsFig = YdInsFig + Deltab_Y(Xab1, Yab1, Xab2, Yab2, Transf);


  for (ptLine = ptRoot->LINE; ptLine; ptLine = ptLine->NEXT) {
   if (ptLine->TYPE == CELL) {
    YdLineFig = ptLine->Y;
    YuLineFig = YdLineFig + ptLine->HEIGTH;
    if ((YdLineFig == YdInsFig) && (YuLineFig == YuInsFig)) break;
   }
  }
  CurrentIns = InsertInstance(ptLine,ptphIns->INSNAME,ptphIns->FIGNAME,
                              XlInsFig,ptLine->Y,(XrInsFig - XlInsFig),
                              (YuInsFig - YdInsFig),Transf);
  ptScrAllow = InsertAllowInIns(CurrentIns,Xab1,Xab2,Yab1,Yab2,ptphInsfig);
  while (ptScrAllow) {
   ptLine->ALLOW = addchain(ptLine->ALLOW,((char *) ptScrAllow));
   ptScrAllow = ptScrAllow->NEXT;
  }

  if (!(incatalogfeed(ptphIns->FIGNAME))) {
   ptloIns = getloins(ptlofig,ptphIns->INSNAME);
   for(ptphCon = ptphInsfig->PHCON ; ptphCon; ptphCon = ptphCon->NEXT) {
    for(ptloCon = ptloIns->LOCON ; 
        (ptloCon && (ptloCon->NAME != ptphCon->NAME));
        ptloCon = ptloCon->NEXT);
    if (ptloCon) {
     ptScrCon = NULL;
     ptScrSig = NULL;
     xyflat(&XConLine,&YConLine,ptphCon->XCON,ptphCon->YCON,
            XlInsFig,YdInsFig,Xab1,Yab1,Xab2,Yab2,Transf);
     Orient = Orientation(ptphCon->ORIENT,Transf);
     phName = namealloc (ptphCon->NAME);
     if ((phName == NameVss) || (phName == NameVdd)) 
      phName = ScrNameIndex(phName,ptLine->INDEX);
     ptScrCon = InsertConInIns(phName,ptloCon->TYPE,ptloCon->DIRECTION,Orient,
                               XConLine,YConLine,ptphCon->WIDTH, ptphCon->LAYER,
                               ptLine,CurrentIns
                              );
     ptLine->CON = addchain(ptLine->CON,((char *) ptScrCon));
     ptSig    = ptloCon->SIG;
     for(ptRootSig = ptRoot->SIG ; (ptRootSig && 
         (ptRootSig->INDEX != ptSig->INDEX)); ptRootSig = ptRootSig->NEXT);
     if (ptRootSig == NULL) {
      ptScrSig = InsertSignal(ptRoot,getsigname(ptSig),ptSig->INDEX,ptSig->TYPE,
                              ptScrCon,ptLine);
      ptScrCon->SIG = ptScrSig;
      ptLine->SIG = addchain(ptLine->SIG,((char *) ptScrSig));
     }
     else {
      ptScrCon->SIG   = ptRootSig;
      ptRootSig->CON  = addchain(ptRootSig->CON,((char *) ptScrCon));
      for (LineList = ptRootSig->LINE; LineList; LineList = LineList->NEXT) 
       if (ptLine->INDEX == ((Line *) LineList->DATA)->INDEX) break;
      if (LineList == NULL)
       ptRootSig->LINE = addchain(ptRootSig->LINE,((char *) ptLine));
      for (SigList = ptLine->SIG; SigList; SigList = SigList->NEXT) 
       if (ptSig->INDEX == ((Signal *) SigList->DATA)->INDEX) break;
      if (SigList == NULL)
       ptLine->SIG = addchain(ptLine->SIG,((char *) ptRootSig));
     }
 
    }
   }
  }
 }
 MakeFatherCon(ptRoot,ptphfig,ptlofig,HeadphconList);

}

/******************************************************************************/
/* Fonction : MakeLine()	                 		              */
/* 	      Creation des differentes lignes et verification                 */
/*            de la topologie du circuit.                                     */
/******************************************************************************/
Line		*MakeLine(ptphfig)

phfig_list	*ptphfig;

{
 Line		*ptLeadLine  = NULL;
 Line		*ptBeforLine = NULL;
 Line		*ptAfterLine = NULL;
 Line		*ptNewLine   = NULL;
 Line		*ptLine      = NULL;
 phins_list	*ptIns       = NULL;
 phfig_list	*ptphInsfig  = NULL;
 BOOLEAN	LineExist    = FALSE;
 long		Xab2 = 0,Xab1 = 0,Yab2 = 0,Yab1 = 0;
 long		Y1Line = 0,Y2Line = 0;
 long		Y1Ins = 0,Y2Ins = 0;
 long		LineIndex = 0;

 Xab1 = ptphfig->XAB1;
 Xab2 = ptphfig->XAB2;
 Yab1 = ptphfig->YAB1;
 Yab2 = ptphfig->YAB2;
 
 /*  building UP line  */
 ptLeadLine = CreateLine(UP,Xab1,Yab2,Xab2,Yab2);
 
 /*  building CELLs lines  */
 for (ptIns = ptphfig->PHINS ; ptIns; ptIns = ptIns->NEXT) {
  ptphInsfig = getphfig(ptIns->FIGNAME,'A');
  sx2sc (ptphInsfig);
  Y1Ins = ptIns->YINS;
  Y2Ins = ptIns->YINS + Deltab_Y(ptphInsfig->XAB1,ptphInsfig->YAB1,
				 ptphInsfig->XAB2,ptphInsfig->YAB2,
				 ptIns->TRANSF);

  ptBeforLine = ptAfterLine = NULL;
  LineExist = FALSE;
  for (ptLine = ptLeadLine; ptLine; ptLine = ptLine->NEXT) {
   Y1Line = ptLine->Y;
   Y2Line = Y1Line + ptLine->HEIGTH;
   if (((Y2Line < Y2Ins) && (Y2Line > Y1Ins)) ||
       ((Y1Line < Y2Ins) && (Y1Line > Y1Ins)) ||
       ((Y2Line < Y2Ins) && (Y1Line > Y1Ins)) ||
       ((Y2Line > Y2Ins) && (Y1Line == Y1Ins)) ||
       ((Y2Line >= Y2Ins) && (Y1Line < Y1Ins)) ||
       ((Y2Line <= Y2Ins) && (Y1Line > Y1Ins) && (Y1Line < Y2Ins)) ||
       ((Y2Line < Y2Ins) && (Y1Line == Y1Ins))) {
    fprintf(stderr,"scr_error,Bad placed instance : %s, or the abutment box is not defined in the genlib file.\n",ptIns->INSNAME);
    exit(1);
   }
   else if ((Y2Line == Y2Ins) && (Y1Line == Y1Ins)) {
         LineExist = TRUE;
         break;
        }
        else if (Y2Line <= Y1Ins) {
              ptBeforLine = ptLine;
             }
             else if (Y1Line >= Y2Ins) {
                   ptAfterLine = ptLine;
                   break;
                  }
  }
  if ((!LineExist) && ptBeforLine && ptAfterLine) {
   ptNewLine = CreateLine(CELL,Xab1,Y1Ins,Xab2,Y2Ins);
   ptNewLine->NEXT = ptAfterLine;
   ptBeforLine->NEXT = ptNewLine;
  }
  else if ((!LineExist) && ptBeforLine) {
        ptNewLine = CreateLine(CELL,Xab1,Y1Ins,Xab2,Y2Ins);
        ptBeforLine->NEXT = ptNewLine;
       }
       else if ((!LineExist) && ptAfterLine) {
             ptNewLine = CreateLine(CELL,Xab1,Y1Ins,Xab2,Y2Ins);
             ptNewLine->NEXT = ptAfterLine;
             ptLeadLine = ptNewLine;
            }
 }



 /*  building DOWN line  */
 ptNewLine = CreateLine(DOWN,Xab1,Yab1,Xab2,Yab1);
 ptNewLine->NEXT = ptLeadLine;
 ptLeadLine = ptNewLine;


 /*  building PATH lines  */
 for (ptBeforLine = ptLeadLine, ptAfterLine = ptLeadLine->NEXT ; ptAfterLine;
      ptBeforLine = ptAfterLine, ptAfterLine = ptAfterLine->NEXT) {
  ptNewLine = CreateLine(PATH,Xab1,(ptBeforLine->Y + ptBeforLine->HEIGTH),
                         Xab2,ptAfterLine->Y);
  ptNewLine->NEXT   = ptAfterLine;
  ptBeforLine->NEXT = ptNewLine;
 }
     
 /*  indexing  lines  */
 for (ptLine = ptLeadLine ; ptLine; ptLine = ptLine->NEXT) {
  ptLine->INDEX = ++LineIndex;
  if (ptLine->TYPE == PATH) 
   ptLine->NAME = ScrNameIndex("SCR_P",LineIndex);
  else if (ptLine->TYPE == CELL)
        ptLine->NAME = ScrNameIndex("SCR_C",LineIndex);
       else if (ptLine->TYPE == UP) 
             ptLine->NAME = ScrNameIndex("SCR_U",LineIndex);
            else if (ptLine->TYPE == DOWN) 
                  ptLine->NAME = ScrNameIndex("SCR_D",LineIndex);
 
 }
 return(ptLeadLine);
} 

/******************************************************************************/
/*		fonction	LoadScrFig()			              */
/******************************************************************************/
Figure		*LoadScrFig(ptphfig, ptlofig,ptphcon)

phfig_list	*ptphfig;
lofig_list	*ptlofig;
PlaceConList	*ptphcon;

{
 Figure    	*ptroot = NULL;

 ptroot = (Figure *) mbkalloc (sizeof(Figure));
 ptroot->LINE  = MakeLine(ptphfig);
 ptroot->SIG   = NULL;
 ptroot->CON   = NULL;
 ptroot->ALLOW = NULL;

 FillFigure(ptroot,ptphfig,ptlofig,ptphcon);
 return (ptroot);
}

