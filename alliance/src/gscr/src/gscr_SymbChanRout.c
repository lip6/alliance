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
 
/******************************************************************************/
/*                             STANDARD CELLS ROUTER                          */
/*      FILE : SymbChanRout.c    LAST MODIFICATIOIN : JUL/25/1991 AT 9:23     */
/******************************************************************************/


# include <stdio.h>
# include <string.h>
# include <mut.h>
# include <mlo.h> 
# include <mph.h> 
# include "gscr_DataBase.h"
# include "apr.h"
# include "gscr_main.h"
# include "gscr_AllowFree.h"
# include "gscr_StaticUtil.h"
# include "gscr_GreedyRout.h"

StaticColumn	*Lead_NS_Column     = NULL;
StaticColumn	*End_NS_Column      = NULL;
StaticColumn	*LeadEastColumn     = NULL;

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void   EliminateOnlyCon(LeadList1,LeadList2,LeadList3,LeadList4)

ConnectorList   **LeadList1;
ConnectorList   **LeadList2;
ConnectorList   **LeadList3;
ConnectorList   **LeadList4;

{
 ConnectorList  *ptList  = NULL;
 ConnectorList  *ptList1 = NULL;
 ConnectorList  *ptList2 = NULL;
 ConnectorList  *ptList3 = NULL;
 ConnectorList  *ptList4 = NULL;
 ConnectorList  *PrevList1 = NULL;
 ConnectorList  *ptEliminate = NULL;
 long           ConName = 0;

 ptList1 = PrevList1 = *LeadList1;
 while (ptList1) {
  ConName = ptList1->ConName;
  for (ptList = *LeadList1; ptList; ptList = ptList->NextCon)
   if ((ptList != ptList1) && (ConName == ptList->ConName)) break;

  if (ptList == NULL)
   for (ptList2 = *LeadList2; ptList2; ptList2 = ptList2->NextCon)
    if (ConName == ptList2->ConName) break;

  if ((ptList == NULL) && (ptList2 == NULL))
   for (ptList3 = *LeadList3; ptList3; ptList3 = ptList3->NextCon)
    if (ConName == ptList3->ConName) break;

  if ((ptList == NULL) && (ptList2 == NULL) && (ptList3 == NULL))
   for (ptList4 = *LeadList4; ptList4; ptList4 = ptList4->NextCon)
    if (ConName == ptList4->ConName) break;

  if ((ptList == NULL) && (ptList2 == NULL) &&
      (ptList3 == NULL)  && (ptList4 == NULL)) {
   ptEliminate = ptList1;
   ptList1 = ptList1->NextCon;
   if (ptList1 == (*LeadList1)->NextCon) *LeadList1 = ptList1;
   else if (ptList1 == NULL) PrevList1->NextCon = NULL;
        else PrevList1->NextCon = ptList1;
   mbkfree((void *) ptEliminate);
  }
  else {
   PrevList1 = ptList1;
   ptList1   = ptList1->NextCon;
  }
 }
}

/******************************************************************************/
/*            cette fonction elimine les connecteurs redondant dans la liste  */
/*                         EAST et dans la liste WEST.                        */
/******************************************************************************/
void	 SCR_Eliminate(LeadConList)

ConnectorList	**LeadConList;

{
 ConnectorList	*EliminateCon        = NULL;
 ConnectorList	*CurrentCon          = NULL;
 ConnectorList	*CurrentConList      = NULL;
 ConnectorList	*BeforCurrentConList = NULL;

 if (*LeadConList)
  for(CurrentCon = *LeadConList; CurrentCon;
      CurrentCon = CurrentCon->NextCon) {
   BeforCurrentConList = CurrentCon;
   CurrentConList = CurrentCon->NextCon;
   while(CurrentConList) 
    if(CurrentCon->ConName == CurrentConList->ConName) {
     EliminateCon = CurrentConList;
     if (CurrentConList->NextCon) {
      CurrentConList = CurrentConList->NextCon;
      BeforCurrentConList->NextCon = CurrentConList;
     }
     else {
      CurrentConList = NULL;
      BeforCurrentConList->NextCon = NULL;
     }
     mbkfree(((void *) EliminateCon));
    }
    else {
     CurrentConList = CurrentConList->NextCon;
     BeforCurrentConList = BeforCurrentConList->NextCon;
    }
  }
}

/******************************************************************************/
/*                         CALCUL DE LA NOUVELLE LARGEUR DU CANAL             */
/******************************************************************************/
long	SCR_NewChannelWidth(LeadWest, LeadEast, Width)

ConnectorList	*LeadWest;
ConnectorList	*LeadEast;
long	Width;

{
 ConnectorList	*CurrentList = NULL;

 for (CurrentList = LeadWest; CurrentList; Width++,
      CurrentList = CurrentList->NextCon);
 for (CurrentList = LeadEast; CurrentList; Width++,
      CurrentList = CurrentList->NextCon);
 return(Width);
}

/******************************************************************************/
/*        Creation de la base de donnee du routeur canal symbolique           */
/******************************************************************************/

StaticColumn	*SCR_LoadStaticColumn(LeadColumn, LeadNorth,LeadSouth,
				      LeadWest, LeadEast,  Width)

StaticColumn	*LeadColumn;
ConnectorList	*LeadNorth;
ConnectorList	*LeadSouth;
ConnectorList	*LeadWest;
ConnectorList	*LeadEast;
long		 Width;

{
 StaticColumn	*CurrentColumn    = NULL;
 ConnectorList	*CurrentList      = NULL;
 ConnectorList	*CurrentNorthList = NULL;
 ConnectorList	*CurrentSouthList = NULL;
 long		 Indice = 0;



 CurrentColumn = LeadColumn;

 /* Transfert de la liste WEST - si elle n'est pas vide - vers la liste des */
 /* colonnes                                                                */ 

 if (LeadWest) 
  for (CurrentList = LeadWest; CurrentList;
   CurrentList = CurrentList->NextCon, CurrentColumn = CurrentColumn->NextCol) {
   CurrentColumn->NorthCon = CurrentList->ConName;
   CurrentColumn->SouthCon = 0;
  }

 /* Transfert de la liste NORTH et SOUTH vers la liste des colonnes        */

 
 for (Lead_NS_Column   = CurrentColumn, CurrentNorthList = LeadNorth,
      CurrentSouthList = LeadSouth, Indice = 1; Indice <= Width;
      CurrentColumn = CurrentColumn->NextCol, Indice++) {
  if (CurrentNorthList) 
   if (CurrentNorthList->Mark == Indice) {
    CurrentColumn->NorthCon = CurrentNorthList->ConName;
    CurrentNorthList = CurrentNorthList->NextCon;
   }
   else CurrentColumn->NorthCon = 0;
  else CurrentColumn->NorthCon = 0;

  if (CurrentSouthList) 
   if (CurrentSouthList->Mark == Indice) {
    CurrentColumn->SouthCon = CurrentSouthList->ConName;
    CurrentSouthList = CurrentSouthList->NextCon;
   }
   else CurrentColumn->SouthCon = 0;
  else CurrentColumn->SouthCon = 0;

  End_NS_Column = CurrentColumn; 
 }

 /* Transfert de la liste EAST - si elle n'est pas vide - vers la liste des */
 /* colonnes  en creant au fur et a mesure les colonnes necessaires         */ 

 if (LeadEast) 
  for (CurrentList = LeadEast, LeadEastColumn = CurrentColumn; CurrentList;
   CurrentList = CurrentList->NextCon, CurrentColumn = CurrentColumn->NextCol) {
   CurrentColumn->NorthCon = CurrentList->ConName;
   CurrentColumn->SouthCon = 0;
  }

 return(LeadColumn);
}

/******************************************************************************/
/*                                      BELONG                                */
/******************************************************************************/
BOOLEAN SCR_Belong(Element, LeadList, EndList, ControlCon)

StaticColumn	*LeadList;
StaticColumn	*EndList;
long		Element;
char		ControlCon;

{
 StaticColumn	*CurrentColumn = NULL;
 BOOLEAN		Find           = FALSE;
 long		ConName        = 0;

 CurrentColumn = LeadList;
 while (CurrentColumn != EndList) {
  if (ControlCon == 'N') ConName = CurrentColumn->NorthCon;
   else ConName = CurrentColumn->SouthCon;
  if (Element == ConName) {
   Find = TRUE;
   break;
  }
  CurrentColumn = CurrentColumn->NextCol;
 }
 return(Find);
}

/******************************************************************************/
/*                         CALCUL DE LA DENSITE DU CANAL                      */
/******************************************************************************/
long	SCR_ChannelDensity(LeadStaticColumn, Height)
 
StaticColumn	*LeadStaticColumn;
long		*Height;

{
 StaticColumn	*CurrentColumn       = NULL;
 StaticColumn 	*BeforCurrentColumn  = NULL;
 StaticColumn 	*AfterCurrentColumn  = NULL;
 long		BeforCurrentNorthCon = 0;
 long		BeforCurrentSouthCon = 0;
 long		LocalDensity         = 0;
 long		Density              = 0;

 for (CurrentColumn = Lead_NS_Column; CurrentColumn != LeadEastColumn;
      CurrentColumn = CurrentColumn->NextCol) {
  AfterCurrentColumn = CurrentColumn->NextCol;
  for (BeforCurrentColumn = LeadStaticColumn;
       BeforCurrentColumn != AfterCurrentColumn; 
       BeforCurrentColumn = BeforCurrentColumn->NextCol) {
   BeforCurrentNorthCon = BeforCurrentColumn->NorthCon;
   BeforCurrentSouthCon = BeforCurrentColumn->SouthCon;
   if (BeforCurrentNorthCon != BeforCurrentSouthCon) {
    if (BeforCurrentNorthCon != 0)
     if (!(SCR_Belong(BeforCurrentNorthCon, LeadStaticColumn,
                      BeforCurrentColumn, 'N') ||
          SCR_Belong(BeforCurrentNorthCon, LeadStaticColumn,
                     BeforCurrentColumn, 'S'))) 
      if (SCR_Belong(BeforCurrentNorthCon, CurrentColumn, 
		     ((StaticColumn *) NULL), 'N') ||
          SCR_Belong(BeforCurrentNorthCon, CurrentColumn, 
		     ((StaticColumn *) NULL), 'S'))
       LocalDensity++;  
    if (BeforCurrentSouthCon != 0)
     if (!(SCR_Belong(BeforCurrentSouthCon, LeadStaticColumn,
		      BeforCurrentColumn, 'N') ||
          SCR_Belong(BeforCurrentSouthCon, LeadStaticColumn,
		     BeforCurrentColumn, 'S'))) 
      if (SCR_Belong(BeforCurrentSouthCon, CurrentColumn, 
		     ((StaticColumn *) NULL), 'N') ||
          SCR_Belong(BeforCurrentSouthCon, CurrentColumn, 
		     ((StaticColumn *) NULL), 'S'))
       LocalDensity++;  
   }
   else if (CurrentColumn != BeforCurrentColumn) {
         if (BeforCurrentNorthCon != 0)
          if (!(SCR_Belong(BeforCurrentNorthCon, LeadStaticColumn,
			   BeforCurrentColumn, 'N') ||
               SCR_Belong(BeforCurrentNorthCon, LeadStaticColumn,
			  BeforCurrentColumn, 'S'))) 
           if (SCR_Belong(BeforCurrentNorthCon, CurrentColumn, 
		          ((StaticColumn *) NULL), 'N') ||
               SCR_Belong(BeforCurrentNorthCon, CurrentColumn, 
		          ((StaticColumn *) NULL), 'S'))
         LocalDensity++;  
        }
        else
         if (BeforCurrentNorthCon != 0)
          if (!(SCR_Belong(BeforCurrentNorthCon, LeadStaticColumn,
			   BeforCurrentColumn, 'N') ||
               SCR_Belong(BeforCurrentNorthCon, LeadStaticColumn,
			  BeforCurrentColumn, 'S'))) 
           if (SCR_Belong(BeforCurrentNorthCon, CurrentColumn->NextCol, 
			  ((StaticColumn *) NULL), 'N') ||
               SCR_Belong(BeforCurrentNorthCon, CurrentColumn->NextCol, 
			  ((StaticColumn *) NULL), 'S'))
         LocalDensity++;  

  }
  Density = MAX(Density, LocalDensity);
  LocalDensity = 0;
 }
 return((*Height = MAX(Density, *Height)));
}

/******************************************************************************/
/*                    INSERER UNE NOUVELLE COLONNE                            */
/******************************************************************************/
void SCR_InsertNewColumn(EndRealChannel, LeadEastColumn, Density, NumberColumn)

StaticColumn	*EndRealChannel;
StaticColumn	*LeadEastColumn;
long		Density;
long		NumberColumn;

{
 StaticColumn	*NewSetColumn   = NULL;
 StaticColumn	*LeadNewChannel = NULL;
 StaticColumn	*EndNewChannel  = NULL;
 StaticColumn	*CurrentColumn  = NULL;
 StaticPoint	*LeadNewPoint   = NULL;
 StaticPoint	*EndNewPoint    = NULL;
 StaticPoint	*OldPoint       = NULL;
 StaticPoint	*EastPoint      = NULL;

 NewSetColumn   = SCR_AllowColumn(NumberColumn);
 LeadNewChannel = SCR_AllowChannel(NewSetColumn, (Density + 2), NumberColumn);
 for (CurrentColumn = LeadNewChannel; CurrentColumn;
      CurrentColumn = CurrentColumn->NextCol)
  EndNewChannel = CurrentColumn;
 if (LeadEastColumn) {
  EndNewChannel->NextCol  = LeadEastColumn;
  LeadEastColumn->PrevCol = EndNewChannel;
  EndRealChannel->NextCol = LeadNewChannel;
  LeadNewChannel->PrevCol = EndRealChannel;
  for (OldPoint = EndRealChannel->PointList,
       EastPoint = LeadEastColumn->PointList,
       LeadNewPoint = LeadNewChannel->PointList,
       EndNewPoint = EndNewChannel->PointList; OldPoint;
       OldPoint = OldPoint->NextPoint,
       EastPoint = EastPoint->NextPoint,
       LeadNewPoint = LeadNewPoint->NextPoint,
       EndNewPoint = EndNewPoint->NextPoint )  {
    EndNewPoint->RightPoint = EastPoint;
    OldPoint->RightPoint = LeadNewPoint;
  }
 }
 else {
  EndRealChannel->NextCol = LeadNewChannel;
  LeadNewChannel->PrevCol = EndRealChannel;
  for (OldPoint = EndRealChannel->PointList,
       LeadNewPoint = LeadNewChannel->PointList; OldPoint;
       OldPoint = OldPoint->NextPoint,
       LeadNewPoint = LeadNewPoint->NextPoint )  
   OldPoint->RightPoint = LeadNewPoint;
 }
} 

/******************************************************************************/
/*                  INSERER UNE NOUVELLE PISTE                                */
/******************************************************************************/
void	 SCR_InsertNewTrack(LeadDataBase, Position, Width)

StaticColumn	*LeadDataBase;
long		Position;
long		Width;

{
 StaticPoint	*CurrentPoint     = NULL;
 StaticPoint	*NextCurrentPoint = NULL;
 StaticPoint	*NewTrack         = NULL;
 long		Counter           = 1;

 for (CurrentPoint = LeadDataBase->PointList; Counter < Position; Counter++,
      CurrentPoint = CurrentPoint->NextPoint);
 NewTrack = SCR_AllowOneTrack(Width); 
 for (NextCurrentPoint = CurrentPoint->NextPoint; CurrentPoint;
      CurrentPoint = CurrentPoint->RightPoint,
      NextCurrentPoint = NextCurrentPoint->RightPoint,
      NewTrack = NewTrack->RightPoint) {
  NewTrack->NextPoint = NextCurrentPoint;
  CurrentPoint->NextPoint = NewTrack;
 }
}

/******************************************************************************/
/*                 POST TREATMENT OF THE WEST CONNECTORS                      */
/******************************************************************************/
StaticColumn	*SCR_WestPostAnalysis(Lead_NS_Column, LeadWestCon, Density)

StaticColumn	*Lead_NS_Column;
ConnectorList	*LeadWestCon;
long		Density;

{
 StaticColumn	*CurrentColumn = NULL;
 StaticColumn	*EndWestColumn = NULL;
 StaticPoint	*CurrentPoint  = NULL;
 StaticPoint	*PointList     = NULL;
 ConnectorList	*CurrentList   = NULL;
 long		TrackCounter   = 0;
 long		Counter;

 EndWestColumn = Lead_NS_Column->PrevCol;
 EndWestColumn->PointList->Via    = FALSE;
 EndWestColumn->PointList->Layer1 = NOP;
 EndWestColumn->PointList->Layer2 = NOP;
 for (CurrentColumn = EndWestColumn; CurrentColumn;
      CurrentColumn = CurrentColumn->PrevCol) {
  for (TrackCounter = Density, 
       CurrentPoint = CurrentColumn->PointList->NextPoint;
       ((CurrentPoint) && (CurrentPoint->Layer1 != RIGHT));
       CurrentPoint = CurrentPoint->NextPoint, TrackCounter--);
  if (! CurrentPoint) return NULL;
  for (Counter = Density, PointList = EndWestColumn->PointList->NextPoint;
       Counter > TrackCounter; PointList = PointList->NextPoint, Counter--);
  if (! PointList) return NULL;
  PointList->Via    = FALSE;
  PointList->Layer1 = RIGHT;
  PointList->Layer2 = NOP;
  for (CurrentList = LeadWestCon; 
       CurrentList->ConName != CurrentPoint->PointName; 
       CurrentList = CurrentList->NextCon);
  if (! CurrentList) return NULL;
   CurrentList->Mark = TrackCounter;
 }
 return (EndWestColumn);
}

/******************************************************************************/
/*                 POST TREATMENT OF THE EAST CONNECTORS                      */
/******************************************************************************/
StaticColumn	*SCR_EastPostAnalysis(LeadEastColumn, LeadEastCon, Density)

StaticColumn	*LeadEastColumn;
ConnectorList	*LeadEastCon;
long		Density;

{
 StaticColumn	*CurrentColumn = NULL;
 StaticPoint	*CurrentPoint  = NULL;
 StaticPoint	*CurrentEastPoint  = NULL;
 StaticPoint	*EastPoint     = NULL;
 ConnectorList	*CurrentList   = NULL;
 long		TrackCounter   = 0;
 BOOLEAN	NetNotTreated  = TRUE;
 BOOLEAN	FirstColumn    = TRUE;

 LeadEastColumn->PointList->Via    = FALSE;
 LeadEastColumn->PointList->Layer1 = NOP;
 LeadEastColumn->PointList->Layer2 = NOP;

 for (CurrentColumn = LeadEastColumn; CurrentColumn;
      CurrentColumn = CurrentColumn->NextCol) {

  for (TrackCounter = Density,
       CurrentPoint = CurrentColumn->PointList->NextPoint,
       CurrentEastPoint = LeadEastColumn->PointList->NextPoint; CurrentPoint;
       CurrentEastPoint = CurrentEastPoint->NextPoint,
       CurrentPoint = CurrentPoint->NextPoint, TrackCounter--) {
   if (CurrentPoint->Layer1 == LEFT) {
    NetNotTreated = TRUE;
    if (FirstColumn) {
     for (CurrentList = LeadEastCon;
          CurrentList->ConName != CurrentPoint->PointName;        
          CurrentList = CurrentList->NextCon);
      CurrentList->Mark = TrackCounter;
    }
    else {
     for (EastPoint = LeadEastColumn->PointList->NextPoint; EastPoint;
          EastPoint = EastPoint->NextPoint)
      if (EastPoint->PointName == CurrentPoint->PointName) {
       NetNotTreated = FALSE;
       break;
      }
     if (NetNotTreated) {
      CurrentEastPoint->PointName = CurrentPoint->PointName;
      CurrentEastPoint->Layer1 = LEFT;
      for (CurrentList = LeadEastCon;
           CurrentList->ConName != CurrentPoint->PointName;        
           CurrentList = CurrentList->NextCon);
       CurrentList->Mark = TrackCounter;
     }
    }
   }
   else
    if (CurrentEastPoint->Layer1 != LEFT) {
     CurrentEastPoint->PointName = ((long ) 0);
     CurrentEastPoint->Layer1 = NOP;
    }
   CurrentEastPoint->Via    = FALSE;
   CurrentEastPoint->Layer2 = NOP;
  }
  FirstColumn = FALSE;
 }
 return (LeadEastColumn);
}

/******************************************************************************/
/*                     SAUVEGARDE DU RESULTAT DE ROUTAGE                      */
/******************************************************************************/
int	SCR_SaveResult( 
                       LeadDataBase, EndDataBase, LeadH_Segment,
                       LeadV_Segment, LeadVia, Density, LeadWestCon
                      )

StaticColumn	*LeadDataBase;
StaticColumn	*EndDataBase;
SegmentList	**LeadH_Segment;
SegmentList	**LeadV_Segment;
ViasList	**LeadVia;
long		Density;
ConnectorList   *LeadWestCon;

{
 StaticColumn	*CurrentColumnList = NULL;
 StaticPoint	*CurrentPoint      = NULL;
 StaticPoint	*RightCurrentPoint = NULL;
 StaticPoint	*NextCurrentPoint = NULL;
 SegmentList	*CurrentH_Segment  = NULL;
 SegmentList	*CurrentV_Segment  = NULL;
 ViasList	*CurrentVia        = NULL;
 SegmentList	*BeforCurrentH_Seg = NULL;
 SegmentList	*BeforCurrentV_Seg = NULL;
 ViasList	*BeforCurrentVia   = NULL;
 long		CounterTrack       = 0;
 long		Counter            = 0;
 long		CounterColumn      = 0;
 BOOLEAN	FirstVia           = TRUE;
 BOOLEAN	FirstH_Segment     = TRUE;
 BOOLEAN	FirstV_Segment     = TRUE;
 char		NodeName[15];
 char		Buffer[10];

 if (LeadWestCon) CounterColumn = 0; 
 else CounterColumn = 1; 
 if (!LeadDataBase) return -1;
 for (CurrentColumnList = LeadDataBase; (CurrentColumnList != EndDataBase);
      CurrentColumnList = CurrentColumnList->NextCol, CounterColumn++) 
  for (CurrentPoint = CurrentColumnList->PointList, 
       CounterTrack = (Density + 1); CurrentPoint;
       CurrentPoint = CurrentPoint->NextPoint, CounterTrack--) {

   sprintf(Buffer,"%ld",CurrentPoint->PointName);
   strcpy(NodeName,"Seg_");
   strcat(NodeName,Buffer);

   if (CurrentPoint->Via) {
    if (FirstVia) {
     *LeadVia = BeforCurrentVia = SCR_AllowViasList();
     BeforCurrentVia->XVia = CounterColumn;
     BeforCurrentVia->YVia = CounterTrack;
     FirstVia = FALSE;
    }
    else {
     CurrentVia = SCR_AllowViasList();
     BeforCurrentVia->NextVia = CurrentVia;
     CurrentVia->XVia = CounterColumn;
     CurrentVia->YVia = CounterTrack;
     BeforCurrentVia = CurrentVia;
    }
   }

   if (CurrentPoint->Layer1 == RIGHT) {
    if (FirstH_Segment) {
     *LeadH_Segment = BeforCurrentH_Seg = SCR_AllowSegmentList();
     BeforCurrentH_Seg->SegName = namealloc(NodeName);
     BeforCurrentH_Seg->X1Seg = CounterColumn;
     BeforCurrentH_Seg->Y1Seg = BeforCurrentH_Seg->Y2Seg = CounterTrack;
     for (Counter = CounterColumn, RightCurrentPoint = CurrentPoint->RightPoint;
          ((RightCurrentPoint) && (RightCurrentPoint->Layer1 != LEFT));
          RightCurrentPoint = RightCurrentPoint->RightPoint, Counter++);
     BeforCurrentH_Seg->X2Seg = ++Counter;
     FirstH_Segment = FALSE;
    }
    else {
     CurrentH_Segment = SCR_AllowSegmentList();
     BeforCurrentH_Seg->NextSeg = CurrentH_Segment;
     CurrentH_Segment->SegName = namealloc(NodeName);
     CurrentH_Segment->X1Seg = CounterColumn;
     CurrentH_Segment->Y1Seg = CurrentH_Segment->Y2Seg = CounterTrack;
     for (Counter = CounterColumn, RightCurrentPoint = CurrentPoint->RightPoint;
          ((RightCurrentPoint) && (RightCurrentPoint->Layer1 != LEFT));
          RightCurrentPoint = RightCurrentPoint->RightPoint, Counter++);
     CurrentH_Segment->X2Seg = ++Counter;
     BeforCurrentH_Seg = CurrentH_Segment;
    }
   }

   if (CurrentPoint->Layer2 == DOWN) {
    if (FirstV_Segment) {
     *LeadV_Segment = BeforCurrentV_Seg = SCR_AllowSegmentList();
     BeforCurrentV_Seg->SegName = namealloc(NodeName);
     BeforCurrentV_Seg->X1Seg = BeforCurrentV_Seg->X2Seg = CounterColumn;
     BeforCurrentV_Seg->Y2Seg = CounterTrack;
     for (Counter = CounterTrack, NextCurrentPoint = CurrentPoint->NextPoint;
          ((NextCurrentPoint) && (NextCurrentPoint->Layer2 != UP));
          NextCurrentPoint = NextCurrentPoint->NextPoint, Counter--);
     BeforCurrentV_Seg->Y1Seg = --Counter;
     FirstV_Segment = FALSE;
    }
    else {
     CurrentV_Segment = SCR_AllowSegmentList();
     BeforCurrentV_Seg->NextSeg = CurrentV_Segment;
     CurrentV_Segment->SegName = namealloc(NodeName);
     CurrentV_Segment->X1Seg = CurrentV_Segment->X2Seg = CounterColumn;
     CurrentV_Segment->Y2Seg = CounterTrack;
     for (Counter = CounterTrack, NextCurrentPoint = CurrentPoint->NextPoint;
          ((NextCurrentPoint) && (NextCurrentPoint->Layer2 != UP));
          NextCurrentPoint = NextCurrentPoint->NextPoint, Counter--);
     CurrentV_Segment->Y1Seg = --Counter;
     BeforCurrentV_Seg = CurrentV_Segment;
    }
   }
  } 
 return 0;
}

/******************************************************************************/
/*                              SYMBOLIC CHANNEL ROUTER                       */
/******************************************************************************/
BOOLEAN		SymbolicChannelRouter(
			      LeadNorthCon, LeadSouthCon,
			      LeadWestCon, LeadEastCon,
			      ChannelWidth, ChannelHeight,
			      LeadH_Segment, LeadV_Segment, LeadVia
			     )

ConnectorList	**LeadNorthCon;
ConnectorList	**LeadSouthCon;
ConnectorList	**LeadWestCon;
ConnectorList	**LeadEastCon;
long		*ChannelWidth;
long		*ChannelHeight;
SegmentList	**LeadH_Segment;
SegmentList	**LeadV_Segment;
ViasList	**LeadVia;

{
 long	 	OldChannelWidth     = 0;
 long		NewChannelWidth     = 0;
 long		ChannelDensity      = 0;
 StaticColumn	*LeadStaticColumn   = NULL;
 StaticColumn	*LeadColumnList     = NULL;
 StaticColumn	*LeadStaticDataBase = NULL;
 StaticColumn	*LeadRealChannel    = NULL;
 StaticColumn	*EndRealChannel    = NULL;
 BOOLEAN        Change = FALSE;

 Lead_NS_Column = NULL;
 End_NS_Column  = NULL;
 LeadEastColumn = NULL;
 OldChannelWidth = *ChannelWidth;
  
# ifdef SCR_DEBUG
  fprintf(stderr,"Befor elimination : \n");
  fprintf(stderr,"Northlist : \n");
  U_PrintList(*LeadNorthCon);
  fprintf(stderr,"Southlist : \n");
  U_PrintList(*LeadSouthCon);
  fprintf(stderr,"Westlist : \n");
  U_PrintList(*LeadWestCon);
  fprintf(stderr,"Eastlist : \n");
  U_PrintList(*LeadEastCon);
# endif

# ifdef SCR_DEBUG
 fprintf(stderr,"Eliminate a Multiply Connector on West Edge if necessary \n");
# endif

 if (LeadWestCon) SCR_Eliminate(LeadWestCon);

# ifdef SCR_DEBUG
 fprintf(stderr,"Eliminate a Multiply Connector on East Edge if necessary \n");
# endif

 if (LeadEastCon) SCR_Eliminate(LeadEastCon);

 EliminateOnlyCon(LeadNorthCon,LeadSouthCon,LeadWestCon,LeadEastCon);
 EliminateOnlyCon(LeadSouthCon,LeadNorthCon,LeadWestCon,LeadEastCon);
 if (LeadWestCon) EliminateOnlyCon(LeadWestCon,LeadNorthCon,LeadSouthCon,LeadEastCon);
 if (LeadEastCon) EliminateOnlyCon(LeadEastCon,LeadNorthCon,LeadSouthCon,LeadWestCon);

# ifdef SCR_DEBUG
  fprintf(stderr,"After elimination : \n");
  fprintf(stderr,"Northlist : \n");
  U_PrintList(*LeadNorthCon);
  fprintf(stderr,"Southlist : \n");
  U_PrintList(*LeadSouthCon);
  fprintf(stderr,"Westlist : \n");
  U_PrintList(*LeadWestCon);
  fprintf(stderr,"Eastlist : \n");
  U_PrintList(*LeadEastCon);
# endif

# ifdef SCR_DEBUG
 fprintf(stderr,"Computing a New Channel Width \n");
# endif

 NewChannelWidth  = SCR_NewChannelWidth(*LeadWestCon,*LeadEastCon,*ChannelWidth);

# ifdef SCR_DEBUG
 fprintf(stderr,"Allow Static Column \n");
# endif

 LeadStaticColumn = SCR_AllowColumn(NewChannelWidth);

# ifdef SCR_DEBUG
 fprintf(stderr,"Loading Static Data Base \n");
# endif

 LeadColumnList = SCR_LoadStaticColumn(LeadStaticColumn,
				       *LeadNorthCon,*LeadSouthCon,
                                       *LeadWestCon,*LeadEastCon,
				       OldChannelWidth);

# ifdef SCR_DEBUG
 fprintf(stderr,"Column Static Data Base \n");
 U_PrintColumn(LeadColumnList);
 fprintf(stderr,"Computing Channel Density \n");
# endif

 ChannelDensity = SCR_ChannelDensity(LeadColumnList, ChannelHeight);

# ifdef SCR_DEBUG
 fprintf(stderr,"Channel Density  =  %ld \n", ChannelDensity);
 fprintf(stderr,"Allow Memory of a Channel \n");
# endif

 LeadStaticDataBase = SCR_AllowChannel( LeadColumnList, ChannelDensity + 2,
				       NewChannelWidth);

 LeadRealChannel = LeadStaticDataBase;

# ifdef SCR_DEBUG
 fprintf(stderr,"U_PrintStaticDataBase Befor Routing\n");
 U_PrintStaticDataBase(LeadStaticDataBase);
 fprintf(stderr,"Greedy Router \n");
# endif

 *ChannelWidth = OldChannelWidth;
 SCR_GreedyRouter( LeadStaticDataBase, End_NS_Column, LeadEastColumn,
                  &ChannelDensity, NewChannelWidth, ChannelWidth);

 *ChannelHeight = ChannelDensity;

 if (OldChannelWidth != *ChannelWidth) Change = TRUE;

# ifdef SCR_DEBUG
 fprintf(stderr,"ChannelDensity = %ld \n", ChannelDensity);
 fprintf(stderr,"U_PrintStaticDataBase \n");
 U_PrintStaticDataBase(LeadStaticDataBase);
# endif

 if (*LeadWestCon) {
  LeadRealChannel = SCR_WestPostAnalysis( Lead_NS_Column,*LeadWestCon,
				         ChannelDensity);
  if(!LeadRealChannel) return -1;
 }

 if (*LeadEastCon) {
  EndRealChannel = SCR_EastPostAnalysis( LeadEastColumn,*LeadEastCon,
		       		        ChannelDensity );
  if(!EndRealChannel) return -1;
 }

# ifdef SCR_DEBUG
 fprintf(stderr,"U_PrintStaticDataBase \n");
 U_PrintStaticDataBase(LeadRealChannel);
 fprintf(stderr,"Saving a Channel Router Result \n");
# endif

 if(SCR_SaveResult(LeadRealChannel, EndRealChannel, LeadH_Segment, 
	        LeadV_Segment, LeadVia, ChannelDensity,*LeadWestCon) < 0) return -1;;

# ifdef SCR_DEBUG
 fprintf(stderr,"Print Horizontal Segment : \n");
 U_PrintSegmentList(*LeadH_Segment);
 fprintf(stderr,"Print Vertical Segment : \n");
 U_PrintSegmentList(*LeadV_Segment);
 fprintf(stderr,"Print Via List : \n");
 U_PrintViasList(*LeadVia);
 fprintf(stderr,"Free of Memory \n");
# endif

 SCR_FreeDataBase(LeadStaticDataBase);

 return (Change);
}
