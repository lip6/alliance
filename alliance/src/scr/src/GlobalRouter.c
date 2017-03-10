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
/*    Fichier : GlobalRouter.c                                                */
/*                                                                            */
/*    (c) copyright 1991 Laboratoire MASI equipe CAO & VLSI                   */
/*    Tous droits reserves                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                   */
/*                                                                            */
/*    Auteur(s) : El housseine REJOUAN                  le : 26/10/1991       */
/*                                                                            */
/*    Modifie par : El housseine REJOUAN                le : 25/07/1992       */
/*    Modifie par :                                     le : ../../....       */
/*    Modifie par :                                     le : ../../....       */
/*                                                                            */
/******************************************************************************/
# include <mut.h>
# include <mlo.h>
# include <mph.h>
# include <apr.h>
# include "SCR_Type.h"
# include "LoadDataBase.h"
# include "ViewDataBase.h"
# include "ScrDataBase.h"
# include "main.h"

/******************************************************************************/
/* Function : EliminateMultipleConnection()                                   */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*	Input parameters :                                                    */
/*      Input global Variables :                                              */
/*	Output parameters :                                                   */
/*      Output global Variables :                                             */
/*                                                                            */
/******************************************************************************/
Figure	*EliminateMultipleConnection(ptFig)
Figure	*ptFig;

{
 Line		*PathLine = NULL;
 Line		*CentralChannel = NULL;
 Channel	*ptChannel = NULL;
 Signal		*ptSignal = NULL;
 ConnectorList	*ptNorthCon = NULL;
 ConnectorList	*ptSouthCon = NULL;
 ConnectorList	*ptWestCon = NULL;
 ConnectorList	*ptEastCon = NULL;
 ConnectorList	*ptDeleteCon = NULL;
 ConnectorList	*ptConList = NULL;
 ConnectorList	*PrevConList = NULL;
 ConnectorList	*CurrentCon = NULL;
 ConnectorList	*ptSaveCon = NULL;
 long		CounterCon = 0;
 long		IndexSignal = 0;
 long		IndexCentralChannel = 0;
 long		GravityCentre = 0;
 long		AddMarkCon = 0;
 long		MinLength = 0;
 long		NewLength = 0;
 BOOLEAN	NetInChannel = FALSE;

/******************************************************************************/
 for (ptSignal = ptFig->SIG; ptSignal; ptSignal = ptSignal->NEXT) {
  if (!(isvdd(ptSignal->NAME) || isvss(ptSignal->NAME))) {
   ptSignal->MAXCON = 0;
   IndexSignal = ptSignal->INDEX;
   for (PathLine = ptFig->LINE; PathLine; PathLine = PathLine->NEXT) {
    if (PathLine->TYPE == PATH) {
     ptChannel = PathLine->CHANNEL;
     CounterCon = 0;
     for (ptNorthCon = ptChannel->NORTH_LIST; ptNorthCon; 
          ptNorthCon = ptNorthCon->NextCon) 
      if (IndexSignal == ptNorthCon->ConName) CounterCon++;
     for (ptSouthCon = ptChannel->SOUTH_LIST; ptSouthCon; 
          ptSouthCon = ptSouthCon->NextCon) 
      if (IndexSignal == ptSouthCon->ConName) CounterCon++;
     for (ptWestCon = ptChannel->WEST_LIST; ptWestCon; 
          ptWestCon = ptWestCon->NextCon) 
      if (IndexSignal == ptWestCon->ConName) CounterCon++;
     for (ptEastCon = ptChannel->EAST_LIST; ptEastCon; 
          ptEastCon = ptEastCon->NextCon) 
      if (IndexSignal == ptEastCon->ConName) CounterCon++;
     if (CounterCon > ptSignal->MAXCON) {
      CentralChannel = PathLine;
      ptSignal->MAXCON = CounterCon;
     }
    }
   }
   ptSignal->MAXCON = CentralChannel->INDEX;
  }
 }
/******************************************************************************/
 for (ptSignal = ptFig->SIG; ptSignal; ptSignal = ptSignal->NEXT) {
if ((NameVdd != ptSignal->NAME) && (NameVss != ptSignal->NAME)) {
  IndexSignal = ptSignal->INDEX;
  IndexCentralChannel = ptSignal->MAXCON;
  for (PathLine = ptFig->LINE; PathLine; PathLine = PathLine->NEXT) {
if (PathLine->TYPE == PATH) {
   if (PathLine->INDEX != IndexCentralChannel) {
    ptChannel = PathLine->CHANNEL;
    CounterCon = AddMarkCon = GravityCentre = 0;
    NetInChannel = FALSE;
    for (ptNorthCon = ptChannel->NORTH_LIST; ptNorthCon; 
         ptNorthCon = ptNorthCon->NextCon) 
     if (IndexSignal == ptNorthCon->ConName) {
      NetInChannel = TRUE;
      CounterCon++;
      AddMarkCon += ptNorthCon->Mark;
     }
    for (ptSouthCon = ptChannel->SOUTH_LIST; ptSouthCon; 
         ptSouthCon = ptSouthCon->NextCon) 
     if (IndexSignal == ptSouthCon->ConName) {
      NetInChannel = TRUE;
      CounterCon++;
      AddMarkCon += ptSouthCon->Mark; 
     }
    for (ptWestCon = ptChannel->WEST_LIST; ptWestCon; 
         ptWestCon = ptWestCon->NextCon) 
     if (IndexSignal == ptWestCon->ConName) {
      NetInChannel = TRUE;
      CounterCon++;
      AddMarkCon += ptWestCon->Mark;
     }
    for (ptEastCon = ptChannel->EAST_LIST; ptEastCon; 
         ptEastCon = ptEastCon->NextCon) 
     if (IndexSignal == ptEastCon->ConName) {
      NetInChannel = TRUE;
      CounterCon++;
      AddMarkCon += ptChannel->WIDTH;
     }
if (NetInChannel) {
   if (PathLine->INDEX < IndexCentralChannel) 
    ptConList = ptChannel->NORTH_LIST;
   else
    if (PathLine->INDEX > IndexCentralChannel) 
     ptConList = ptChannel->SOUTH_LIST;
    GravityCentre = ((long ) ((AddMarkCon / CounterCon) + 0.5));
    MinLength = ptChannel->WIDTH;
    ptSaveCon = NULL;
    for (CurrentCon = ptConList; CurrentCon; CurrentCon = CurrentCon->NextCon) {
     if (CurrentCon->ConName == IndexSignal) {
      NewLength = LABS(GravityCentre,CurrentCon->Mark);
      if (NewLength <= MinLength) {
       MinLength = NewLength;
       ptSaveCon = CurrentCon;
      }
     }
    }
    PrevConList = NULL;
    while (ptConList) {
     if ((ptConList->ConName == IndexSignal) && (ptConList != ptSaveCon)) {
      ptDeleteCon = ptConList;
      ptConList = ptConList->NextCon;
      if (PrevConList) {
       PrevConList->NextCon = ptConList;
      }
      else {
       if (PathLine->INDEX < IndexCentralChannel) 
        ptChannel->NORTH_LIST = ptConList;
       else
        if (PathLine->INDEX > IndexCentralChannel) 
         ptChannel->SOUTH_LIST = ptConList;
      }
      mbkfree((void *) ptDeleteCon);
     }
     else {
      PrevConList = ptConList;
      ptConList = ptConList->NextCon;
     }
    }
}
   }
}
  }
}
 }
 return(ptFig);
}

/******************************************************************************/
/*	Function : AutoAllowInsert()                                          */
/******************************************************************************/
Segment		*AutoAllowInsert(ptfig,ptLine,Xinf,Xsup,NewphIns,ptXInsert,Counter)
Figure		*ptfig;
Line		*ptLine;
long		Xinf;
long		Xsup;
phfig_list	*NewphIns;
XSupplyRecallList       *ptXInsert;
long		Counter;

{
 XSupplyRecallList      *ptXBreakList = NULL;
 long		Xinsert = 0;
 long		XInsertSouth = 0;
 long		Xmoy    = 0;
 Line		*CurrentLine = NULL;
 Instance	*CurrentInst = NULL;
 Instance	*PrevInst = NULL;
 Instance	*ptNewIns = NULL;
 long		Xinst = 0;
 long		XWinst = 0;
 long		NewWidth = 0;
 long		phNewHeight = 0;
 Connector	*CurrentCon = NULL;
 Segment	*CurrentSeg = NULL;
 Segment	*ptNewAllow = NULL;
 Segment	*ptSaveAllow = NULL;
 chain_list	*ConList = NULL;
 
 Xmoy = ((Xsup + Xinf) / 2);
 NewWidth  = NewphIns->XAB2 - NewphIns->XAB1;
 phNewHeight = NewphIns->YAB2 - NewphIns->YAB1;
 for (CurrentLine = ptfig->LINE; CurrentLine; CurrentLine = CurrentLine->NEXT) {
  if (CurrentLine->TYPE == CELL) {
   for (PrevInst = NULL, CurrentInst = CurrentLine->INS; CurrentInst;
        CurrentInst = CurrentInst->NEXT) {
    if ((Xmoy >= (Xinst = CurrentInst->X)) &&
        (Xmoy <= (XWinst = Xinst + CurrentInst->WIDTH))) break;
    PrevInst = CurrentInst;
   }
   Xinsert = INF(Xmoy,Xinst,XWinst);
   if (CurrentLine == (ptfig->LINE->NEXT->NEXT)) XInsertSouth = Xinsert;
   if (Xinsert == XWinst) {
    PrevInst = CurrentInst;
    CurrentInst = CurrentInst->NEXT;
   }
   ptNewIns = CreateInstance(nameindex(CurrentLine->NAME,Counter),
                             NewphIns->NAME,Xinsert,CurrentLine->Y,
                             NewWidth,phNewHeight,NOSYM);
   if (PrevInst) {
    ptNewIns->NEXT = CurrentInst;
    PrevInst->NEXT = ptNewIns;
   }
   else {
    ptNewIns->NEXT = CurrentInst;
    CurrentLine->INS = ptNewIns;
   }

   CurrentLine->WIDTH += NewWidth;
   ptNewAllow = InsertAllowInIns(ptNewIns,NewphIns->XAB1,NewphIns->XAB2,
                                 NewphIns->YAB1,NewphIns->YAB2,NewphIns);
   if (CurrentLine == ptLine)
    ptSaveAllow = ptNewAllow;
   CurrentLine->ALLOW = addchain(CurrentLine->ALLOW,(char *)ptNewAllow);
   for (CurrentSeg = CurrentLine->ALLOWUSED; CurrentSeg;
        CurrentSeg = CurrentSeg->NEXT) 
    if (CurrentSeg->X1 >= Xinsert) {
     CurrentSeg->X1 += NewWidth;
     CurrentSeg->X2 += NewWidth;
    }
/* Mise a jour des connecteurs des lignes */
   for (ConList = CurrentLine->CON; ConList; ConList = ConList->NEXT){
    CurrentCon = ((Connector *)ConList->DATA);
    if ((!CurrentCon->INST) && (CurrentCon->X >= Xinsert)) 
     CurrentCon->X += NewWidth;
   }
/* Mise a jour des connecteurs internes et des segment des instances */
   for (; CurrentInst; CurrentInst = CurrentInst->NEXT) {
    CurrentInst->X += NewWidth;
    for (CurrentCon = CurrentInst->CON; CurrentCon;
         CurrentCon = CurrentCon->NEXT)
     CurrentCon->X += NewWidth;
    for (CurrentSeg = CurrentInst->ALLOW; CurrentSeg;
         CurrentSeg = CurrentSeg->NEXT) {
     CurrentSeg->X1 += NewWidth;
     CurrentSeg->X2 += NewWidth;
    }
   }
  }
  else
   CurrentLine->WIDTH += NewWidth;
 }
/* Mise a jour des connecteurs externes */
 for (CurrentCon = ptfig->CON; CurrentCon; CurrentCon = CurrentCon->NEXT) {
  if (CurrentCon->ORIENT == EAST)
   CurrentCon->X += NewWidth;
  else
   if ((CurrentCon->ORIENT == NORTH) && (CurrentCon->X >= Xinsert))
    CurrentCon->X += NewWidth;
   else
    if ((CurrentCon->ORIENT == SOUTH) && (CurrentCon->X >= XInsertSouth))
     CurrentCon->X += NewWidth;
 }
 /* MISE A JOUR DES POSITIONS D'OUVERTURES DES ALIM. */
  for (ptXBreakList = ptXInsert; ptXBreakList; ptXBreakList = ptXBreakList->NEXT) {
   if (ptXBreakList->X >= Xinsert) ptXBreakList->X += NewWidth;
  }
 /* MISE A JOUR DES POSITIONS D'OUVERTURES DES ALIM. */

 return(ptSaveAllow);
}

/******************************************************************************/
/*	Function : MakeConList()                                              */
/******************************************************************************/
ConnectorList	*MakeConList(LeadConList,IndexSig,IndexCol)

ConnectorList	*LeadConList;
long		IndexSig;
long		IndexCol;

{
 ConnectorList	*ptNewCon   = NULL;
 ConnectorList	*CurrentCon = NULL;
 ConnectorList	*PrevCon    = NULL;

 ptNewCon = (ConnectorList *) mbkalloc (sizeof(ConnectorList));
 ptNewCon->NextCon = NULL;
 ptNewCon->ConName = IndexSig;
 ptNewCon->Mark    = IndexCol;
 
 if (LeadConList == NULL) LeadConList = ptNewCon;
 else {
  PrevCon = CurrentCon = LeadConList;
  while (CurrentCon) {
   if (CurrentCon->Mark > IndexCol) {
    ptNewCon->NextCon = CurrentCon;
    if (CurrentCon == LeadConList) LeadConList = ptNewCon;
    else PrevCon->NextCon  = ptNewCon;
    break;
   }
   PrevCon = CurrentCon;
   CurrentCon = CurrentCon->NextCon;
  }
  if (CurrentCon == NULL) PrevCon->NextCon = ptNewCon;
 }
 
 return(LeadConList);
}

/******************************************************************************/
/*	Function : CreateChannel()                                            */
/******************************************************************************/
Channel		*CreateChannel( LeadNorth,LeadSouth,LeadWest,LeadEast,
                               Width,Heigth)

ConnectorList	*LeadNorth;
ConnectorList	*LeadSouth;
ConnectorList	*LeadWest;
ConnectorList	*LeadEast;
long		Width;
long		Heigth;

{
 Channel	*ptNewChannel = NULL;

 ptNewChannel = (Channel *) mbkalloc (sizeof(Channel));
 ptNewChannel->NORTH_LIST = LeadNorth;
 ptNewChannel->SOUTH_LIST = LeadSouth;
 ptNewChannel->WEST_LIST  = LeadWest;
 ptNewChannel->EAST_LIST  = LeadEast;
 ptNewChannel->H_SEGMENT  = NULL;
 ptNewChannel->V_SEGMENT  = NULL;
 ptNewChannel->VIA	  = NULL;
 ptNewChannel->WIDTH      = Width;
 ptNewChannel->HEIGTH     = Heigth;

 return (ptNewChannel);
}

/******************************************************************************/
/*	Function : FillChannel()                                              */
/******************************************************************************/
void		FillChannel(ptfig)

Figure		*ptfig;

{
 Line		*PathLine   = NULL;
 Line		*NorthLine  = NULL;
 Line		*SouthLine  = NULL;
 Connector	*CurrentCon = NULL;
 chain_list	*ConList    = NULL;
 ConnectorList	*ptNorthList  = NULL;
 ConnectorList	*ptSouthList  = NULL;
 ConnectorList	*ptEastList   = NULL;
 ConnectorList	*ptWestList   = NULL;

 for (SouthLine = ptfig->LINE; SouthLine->NEXT;
      SouthLine = SouthLine->NEXT->NEXT) {

  ptNorthList  = NULL;
  ptSouthList  = NULL;
  ptEastList   = NULL;
  ptWestList   = NULL;
  
  PathLine  = SouthLine->NEXT;
  NorthLine = PathLine->NEXT;
  for (ConList = PathLine->CON; ConList; ConList = ConList->NEXT) {
   CurrentCon = ((Connector *) ConList->DATA);
   if (CurrentCon->ORIENT == WEST) 
    ptWestList = MakeConList(ptWestList,CurrentCon->SIG->INDEX,0);
   else if (CurrentCon->ORIENT == EAST) 
         ptEastList = MakeConList(ptEastList,CurrentCon->SIG->INDEX,0);
  }
  if (SouthLine->TYPE == DOWN) {
   for (ConList = SouthLine->CON; ConList; ConList = ConList->NEXT) {
    CurrentCon = ((Connector *) ConList->DATA);
    ptSouthList = MakeConList(ptSouthList,CurrentCon->SIG->INDEX,
                              ((CurrentCon->X - WESTOFFSET) / PITCH_X) + 1);
   }
  }
  else {
   for (ConList = SouthLine->CON; ConList; ConList = ConList->NEXT) {
    CurrentCon = ((Connector *) ConList->DATA);
    if (CurrentCon->ORIENT == NORTH) 
     ptSouthList = MakeConList(ptSouthList,CurrentCon->SIG->INDEX,
                               ((CurrentCon->X - WESTOFFSET) / PITCH_X) + 1);
   }
  }
  if (NorthLine->TYPE == UP) {
   for (ConList = NorthLine->CON; ConList; ConList = ConList->NEXT) {
    CurrentCon = ((Connector *) ConList->DATA);
    ptNorthList = MakeConList(ptNorthList,CurrentCon->SIG->INDEX,
                              ((CurrentCon->X - WESTOFFSET) / PITCH_X) + 1);
   }
  }
  else {
   for (ConList = NorthLine->CON; ConList; ConList = ConList->NEXT) {
    CurrentCon = ((Connector *) ConList->DATA);
    if (CurrentCon->ORIENT == SOUTH) 
     ptNorthList = MakeConList(ptNorthList,CurrentCon->SIG->INDEX,
                               ((CurrentCon->X - WESTOFFSET) / PITCH_X) + 1);
   }
  }
 
 PathLine->CHANNEL = CreateChannel(ptNorthList,ptSouthList,ptWestList,
                                   ptEastList,
                                   ((PathLine->WIDTH) / PITCH_X),
                                   ((PathLine->HEIGTH) / PITCH_Y));
 }
}

/******************************************************************************/
/*	fonction	MaxConInColumn()	                              */
/******************************************************************************/
long		MaxConInColumn(ptSig)

Signal		*ptSig;

{
 chain_list	*ConList1   = NULL;
 chain_list	*ConList2   = NULL;
 Connector	*CurrentCon = NULL;
 Connector	*NextCon    = NULL;
 long		MaxConInCol = 0 ;
 long		ConInColumn = 0 ;

 for (ConList1 = ptSig->CON; ConList1; ConList1 = ConList1->NEXT) {
  CurrentCon  = ((Connector *) ConList1->DATA);
  ConInColumn = 1;
  for (ConList2 = ConList1->NEXT; ConList2; ConList2 = ConList2->NEXT) {
   NextCon  = ((Connector *) ConList2->DATA);
   if (NextCon->X == CurrentCon->X) ConInColumn++;
  }
  if (MaxConInCol < ConInColumn) MaxConInCol = ConInColumn;
 }
 return (MaxConInCol);
}
 
/******************************************************************************/
/*		fonction	SortSignal()                                  */
/******************************************************************************/
Signal		*SortSignal(ptLeadSig)

Signal		*ptLeadSig;

{
 Signal		*CurrentSig = NULL;
 Signal		*ptNextSig  = NULL;
 Signal		*ptPrevSig  = NULL;
 BOOLEAN	EndingSort  = FALSE;
 long		SecondSig   = 0;
 

 for (CurrentSig = ptLeadSig; CurrentSig; CurrentSig = CurrentSig->NEXT) 
  CurrentSig->MAXCON = MaxConInColumn(CurrentSig);

 while (!EndingSort) {
  EndingSort = TRUE;
  SecondSig  = 0;
  for (ptPrevSig = CurrentSig = ptLeadSig; CurrentSig->NEXT; 
       CurrentSig = CurrentSig->NEXT) {
   ptNextSig = CurrentSig->NEXT;
   if (SecondSig < 2) SecondSig++;
   else ptPrevSig = ptPrevSig->NEXT;
   if (CurrentSig->MAXCON < ptNextSig->MAXCON) {
    EndingSort = FALSE;

    if (CurrentSig == ptLeadSig) {
     if (ptNextSig->NEXT == NULL) {
      ptLeadSig = ptNextSig;
      ptNextSig->NEXT = CurrentSig;
      CurrentSig->NEXT = NULL;
      break;
     }
     else {
      ptLeadSig = ptNextSig;
      CurrentSig->NEXT = ptNextSig->NEXT;
      ptNextSig->NEXT  = CurrentSig;
     }
    }
    else {
     if (ptNextSig->NEXT == NULL) {
      ptPrevSig->NEXT = ptNextSig;
      ptNextSig->NEXT = CurrentSig;
      CurrentSig->NEXT = NULL;
      break;
     }
     else {
      CurrentSig->NEXT = ptNextSig->NEXT;
      ptNextSig->NEXT  = CurrentSig;
      ptPrevSig->NEXT  = ptNextSig;
      CurrentSig       = ptNextSig;
     }
    }
   }
  }
 } 
 return (ptLeadSig);
}

/******************************************************************************/
/*	Function : SpreadSignal()                                             */
/******************************************************************************/
void			SpreadSignal(ptScrRoot,NewphIns,ptXInsert)
Figure			*ptScrRoot;
phfig_list		*NewphIns;
XSupplyRecallList       *ptXInsert;

{
 Signal		*CurrentSig = NULL;
 Signal		*ptSignal   = NULL;
 Connector	*ptCon      = NULL;
 Connector	*ptNewCon   = NULL;
 Segment	*ptAllow    = NULL;
 Segment	*ptSaveAllow = NULL;
 Segment        *ptNewAllow = NULL;
 Line		*FirstLine  = NULL;
 Line		*LastLine   = NULL;
 Line		*CellLine   = NULL;
 Line		*PrevLine   = NULL;
 Line		*ptSaveLine = NULL;
 Line		*ptLine     = NULL;
 chain_list	*LineList   = NULL;
 chain_list	*SigList    = NULL;
 chain_list	*ConList    = NULL;
 chain_list	*AllowList  = NULL;
 chain_list	*PrevAllow  = NULL;
 char           *SigName    = NULL;
 long		MinLength   = 0;
 long		NewLength   = 0;
 long		Xinf = 0;
 long		Xsup = 0;
 long		Counter = 0;

 for (CurrentSig = ptScrRoot->SIG; CurrentSig; CurrentSig = CurrentSig->NEXT) {
  SigName = namealloc(CurrentSig->NAME);
  if (!(isvdd(SigName) || isvss(SigName))) {
   FirstLine = LastLine = ((Line *) (CurrentSig->LINE)->DATA);
   for (LineList = CurrentSig->LINE; LineList; LineList = LineList->NEXT) {
    ptLine = ((Line *) LineList->DATA);
    if(ptLine->INDEX < FirstLine->INDEX) FirstLine = ptLine;
    else if (ptLine->INDEX > LastLine->INDEX) LastLine = ptLine;
   }
   if ((FirstLine != LastLine) && (FirstLine->NEXT != LastLine)) {
    for (ptSaveLine = PrevLine = FirstLine, CellLine = PrevLine->NEXT;
         (CellLine != LastLine); ptSaveLine = PrevLine, 
         PrevLine = CellLine, CellLine = CellLine->NEXT) {
     if (CellLine->TYPE == CELL) {
      for (SigList = CellLine->SIG; SigList; SigList = SigList->NEXT) 
       if (CurrentSig->INDEX == ((Signal *) SigList->DATA)->INDEX) break;
      if (SigList == NULL) { /* signal n'appartient pas a la ligne */
       if (CellLine->ALLOW) {
        for (SigList = PrevLine->SIG; SigList; SigList = SigList->NEXT) {
         ptSignal = ((Signal *) SigList->DATA);
         if (CurrentSig->INDEX == ptSignal->INDEX) break;
        }
        if (SigList == NULL) {
         for (SigList = ptSaveLine->SIG; SigList; SigList = SigList->NEXT) {
          ptSignal = ((Signal *) SigList->DATA);
          if (CurrentSig->INDEX == ptSignal->INDEX) break;
         }
        }
        for (ConList = ptSignal->CON; ConList; ConList = ConList->NEXT) {
         ptCon = ((Connector *) ConList->DATA);
         if ( ((ptCon->LINE->INDEX == PrevLine->INDEX) &&
                (ptCon->ORIENT == NORTH)) 
            ||((ptCon->LINE->INDEX == ptSaveLine->INDEX) &&
	       (ptCon->ORIENT == NORTH)) )
          break;
        }
        MinLength = CellLine->WIDTH;
        ptSaveAllow = NULL;
        for (AllowList = CellLine->ALLOW; AllowList;
             AllowList = AllowList->NEXT) {
         ptAllow = ((Segment *) AllowList->DATA);
         NewLength = LABS(ptAllow->X1,ptCon->X);
         if (MinLength > NewLength) {
          MinLength = NewLength;
          ptSaveAllow = ptAllow;
         }
        }  
       }
       else {
        Xinf = CellLine->WIDTH;
        Xsup = CellLine->X;
        for (ConList = CurrentSig->CON; ConList; ConList = ConList->NEXT) {
         ptCon = ((Connector *)ConList->DATA);
         if (Xinf > ptCon->X) Xinf = ptCon->X;
         if (Xsup < ptCon->X) Xsup = ptCon->X;
        }
        ptSaveAllow = AutoAllowInsert(ptScrRoot,CellLine,Xinf,Xsup,
                                      NewphIns,ptXInsert,++Counter);
       }
       /* Insert ALLOW in Line at ALLOWUSED */
       if (ptSaveAllow) {
        ptNewAllow = CreateSegment(namealloc(CurrentSig->NAME),ptSaveAllow->X1,
                                   ptSaveAllow->Y1,ptSaveAllow->X2,
                                   ptSaveAllow->Y2,LAYER2WIDTH,VER,ALU2);
        if (CellLine->ALLOWUSED == NULL) CellLine->ALLOWUSED = ptNewAllow; 
        else {
         for (ptAllow = CellLine->ALLOWUSED; ptAllow->NEXT; 
              ptAllow = ptAllow->NEXT);
         ptAllow->NEXT = ptNewAllow; 
        }
        /* delete ALLOW from Line */
        PrevAllow = AllowList = CellLine->ALLOW;
        while (AllowList) {
         if (ptSaveAllow == ((Segment *) AllowList->DATA)) {
          if (AllowList == CellLine->ALLOW) CellLine->ALLOW = AllowList->NEXT;
          else if (AllowList->NEXT) PrevAllow->NEXT = AllowList->NEXT; 
               else PrevAllow->NEXT = NULL;
          /* FREE(AllowList) */
          break;
         }
         PrevAllow = AllowList;
         AllowList = AllowList->NEXT;
        }
 
        CellLine->SIG = addchain(CellLine->SIG,CurrentSig);
        CurrentSig->LINE = addchain(CurrentSig->LINE,CellLine);
        ptNewCon = CreateCon(CurrentSig->NAME,'I',UNKNOWN,NORTH,
                             ptSaveAllow->X1,ptSaveAllow->Y2,
                             LAYER2WIDTH,ALU2,CellLine,NULL);
        ptNewCon->SIG = CurrentSig;
        ptNewCon->LINE = CellLine;
        CellLine->CON = addchain(CellLine->CON,ptNewCon);
        CurrentSig->CON = addchain(CurrentSig->CON,ptNewCon);
        ptNewCon = CreateCon(CurrentSig->NAME,'I',UNKNOWN,SOUTH,
                             ptSaveAllow->X1,ptSaveAllow->Y1,
                             LAYER2WIDTH,ALU2,CellLine,NULL);
        ptNewCon->SIG = CurrentSig;
        ptNewCon->LINE = CellLine;
        CellLine->CON = addchain(CellLine->CON,ptNewCon);
        CurrentSig->CON = addchain(CurrentSig->CON,ptNewCon);
       }
       else {
        fprintf(stderr,"scr_error : in Spread Signal !!! \n");
        exit(1);
       }
      }
     }
    }
   }
  }
 }
}

/******************************************************************************/
/*		fonction GlobalRoute()                                        */
/******************************************************************************/
void			GlobalRoute(ptfig,NewphIns,ptXInsert)
Figure			*ptfig;
phfig_list		*NewphIns;
XSupplyRecallList       *ptXInsert;

{

 ptfig->SIG = SortSignal(ptfig->SIG);
 SpreadSignal(ptfig,NewphIns,ptXInsert);
 FillChannel(ptfig);

# ifdef SCR_DEBUG
  fprintf(stderr,"Eliminate the multiple connections ...\n");
# endif

  ptfig = EliminateMultipleConnection(ptfig);

}
/******************************************************************************/
