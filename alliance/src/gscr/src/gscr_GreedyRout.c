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
/*    FILE : SCR_GreedyRouter.c   LAST MODIFICATION : JUL/11/1991             */
/******************************************************************************/

# include <stdio.h>
# include <mut.h>
# include <mlo.h>
# include <mph.h>
# include "gscr_DataBase.h"
# include "gscr_greedy.h"
# include "gscr_SymbChanRout.h"
# include "gscr_DynamicUtil.h"

long EndPattern = LEADPATTERN;
/******************************************************************************/
/*                SCR_SplitNet Modul of Initialisation                        */
/******************************************************************************/
void SCR_SplitNet(Density)

long    Density;

{
 long   Line = 0;
 long   Track = 0;
 long   Mark = 0;
 long   Counter = 0;
 
 for (Line = 1; Line <= Density; Line++) SPLIT(Line) = 0;
 for (Line = 1; Line <= Density; Line++) 
  if ((HNET(Line) != 0) && (SPLIT(Line) == 0)) {
   for (Track = Line, Counter = 0; Track <= Density; Track++) 
    if (HNET(Track) == HNET(Line)) Counter++;  
   for (Mark = Line; Mark <= Density; Mark++)
    if (HNET(Mark) == HNET(Line))
     SPLIT(Mark) = Counter;
  }
}

/******************************************************************************/
/*                             BELONG IN WINDOW                               */
/******************************************************************************/
BOOLEAN SCR_BelongInWindow(Element, LeadList, ControlCon)

StaticColumn	*LeadList;
long		Element;
char		ControlCon;

{
 StaticColumn	*CurrentColumn = NULL;
 BOOLEAN	Find           = FALSE;
 long 		ConName        = 0;
 long		Counter        = 0;

 for (CurrentColumn = LeadList; ((CurrentColumn) && (Counter <= SNC));
      Counter++, CurrentColumn = CurrentColumn->NextCol) {
  if (ControlCon == 'N') ConName = CurrentColumn->NorthCon;
   else ConName = CurrentColumn->SouthCon;
  if (Element == ConName) {
   Find = TRUE;
   break;
  }
 }
 return(Find);
}

/******************************************************************************/
/*         TERMINAL, NON TERMINAL, STEADY, FALLING, RISING NET                */
/******************************************************************************/
void	SCR_SFRT_Net(Column, Density)

StaticColumn	*Column;
long		Density;

{
 StaticColumn	*CurrentColumn = NULL;
 long		Line           = 0;
 BOOLEAN	BelongNorth    = FALSE;
 BOOLEAN	BelongSouth    = FALSE;
 BOOLEAN	NonTerminalNet = FALSE;

 CurrentColumn = Column->NextCol;
 for (Line = 1;Line <= Density; Line++) 
  if (HNET(Line) == 0) SFRT(Line) = TERMINAL;
  else {
   BelongNorth = SCR_BelongInWindow(HNET(Line), CurrentColumn, 'N');
   BelongSouth = SCR_BelongInWindow(HNET(Line), CurrentColumn, 'S');
   NonTerminalNet = (SCR_Belong(HNET(Line), CurrentColumn, NULL, 'N') ||
                     SCR_Belong(HNET(Line), CurrentColumn, NULL, 'S'));
   if (BelongNorth && BelongSouth) SFRT(Line) = STEADY;
    else if ((!BelongNorth) && BelongSouth) SFRT(Line) = FALLING;
          else if ((!BelongSouth) && BelongNorth) SFRT(Line) = RISING;
                else if (NonTerminalNet) SFRT(Line) = NOTTERMINAL;
                      else SFRT(Line) = TERMINAL;
  }
}

/******************************************************************************/
/*                     EVALUATION FUNCTION : WEIGHT                           */
/******************************************************************************/
void	SCR_WeightTracks(Density)

long	Density;

{
 long	 Line    = 0;
 long	 Weight  = 0;
 long	 Height  = 0;
 BOOLEAN Impaire = FALSE;

 Impaire = IMPAIRE(Density);
 if (Impaire) Weight = Height = ((Density + 1) >> 1);
 else {
  Weight = (Density >> 1);
  Height = (Density >> 1) + 1;
 }
 for (Line = 1; Line < Height ; Line++, Weight--)
  WEIGHT(Line) = Weight;
 if (!(Impaire)) Weight++;
 for (; Line <= Density ; Line++, Weight++)
  WEIGHT(Line) = Weight;
}

/******************************************************************************/
/*                     INSERT NEW LINE IN DYNAMIC DATA BASE                   */
/******************************************************************************/
void	SCR_InsertNewLine(Position, Density)

long	Position;
long	Density;

{
 long	Line     = 0;
 long	Column   = 0;
 long	NextLine = 0;

 for (Line = Density + 1; Line >= Position; Line--) {
  NextLine = Line + 1;
  for (Column = 0; Column <= EndPattern; Column++) 
   TabRout[Column][NextLine] = TabRout[Column][Line]; 
 }
}

/******************************************************************************/
/*                                UPDATE SFRT                                 */
/******************************************************************************/
void	SCR_UpDateSFRT(Column, Density)

StaticColumn	*Column;
long		Density;

{
 long	MarkLine = 0;
 long	Line = 0;
 long	Track = 0;

 SCR_SplitNet(Density);
 SCR_SFRT_Net(Column, Density);
 SCR_WeightTracks(Density);

# ifdef SCR_DEBUG
 U_PrintDynamicDataBase(Density);
# endif

 for (Line = 1; Line <= Density; Line++)
  if (SPLIT(Line) > 1) {
   for (Track = Line + 1; ((Track <= Density) && (HNET(Track) != HNET(Line)));
        Track++);
   for (MarkLine = Line; ((MarkLine <= Track) &&
        (VNET(MarkLine) == (HNET(Line)))); MarkLine++);
   if (SFRT(Line) == RISING) {
    if (MarkLine > Track) {
     SFRT(Track) = TERMINAL;
     SPLIT(Track) = ((long ) 1);
     if (SPLIT(Line) > 1) {
      for (Track = Line + 1; ((Track <= Density) &&
           (HNET(Track) != HNET(Line))); Track++);
      SPLIT(Track) = SPLIT(Line) - 2;
     }
    }
    else SPLIT(Track) = --SPLIT(Line);
   }
   else
    if (SFRT(Line) == FALLING) { 
     if (MarkLine > Track) {
      SFRT(Line) = TERMINAL;
      SPLIT(Track) = --SPLIT(Line);
      SPLIT(Line) = ((long ) 1);
     }
     else SPLIT(Track) = --SPLIT(Line);
    }
    else
     if (SFRT(Line) == TERMINAL) {
      if (MarkLine <= Track) {
       if (Track == Density + 1) Track--;
       SFRT(Line) = SFRT(Track) = NOTTERMINAL;
       for (MarkLine = Track; ((MarkLine <= Density) &&
            (VNET(MarkLine) == (HNET(Line)))); MarkLine++)
        if (HNET(MarkLine) == HNET(Line)) SPLIT(MarkLine) = ((long ) 1);
      }
      else SPLIT(Track) = --SPLIT(Line);
     }
     else
      if ((SFRT(Line) == STEADY) || (SFRT(Line) == NOTTERMINAL)) {
       if (MarkLine > Track) 
        if (WEIGHT(Line) > WEIGHT(Track)) {
         SFRT(Line) = TERMINAL;
         SPLIT(Track) = --SPLIT(Line);
        }
        else {
         SFRT(Track) = TERMINAL;
         SPLIT(Track) = ((long ) 1);
        }
       else SPLIT(Track) = --SPLIT(Line);
      }
  }

# ifdef SCR_DEBUG
 U_PrintDynamicDataBase(Density);
# endif
}

/******************************************************************************/
/*                       UPDATE STATIC DATA BASE                              */
/******************************************************************************/
void	SCR_UpDateDataBase(Column, Density)

StaticColumn	*Column;
long 		Density;

{
 StaticPoint	*CurrentPoint  = NULL;
 long		Line           = 0;


 SCR_UpDateSFRT(Column, Density);
 CurrentPoint = Column->PointList;
 if (SCR_TOP != 0)  {
  CurrentPoint->PointName = SCR_TOP;
  CurrentPoint->Via    = FALSE;
  CurrentPoint->Layer1 = NOP;
  CurrentPoint->Layer2 = DOWN;
 }

# ifdef SCR_DEBUG
 fprintf(stdout,"NorthCon = %ld \n",SCR_TOP);
 fprintf(stdout,"Via = %d \t Layer1 = %c \t Layer2 = %c \t Name = %ld \n",
         CurrentPoint->Via, CurrentPoint->Layer1, CurrentPoint->Layer2,
         CurrentPoint->PointName);
# endif

 for (CurrentPoint = CurrentPoint->NextPoint, Line = 1; ((CurrentPoint) &&
      (Line <= Density)); CurrentPoint = CurrentPoint->NextPoint, Line++) {
  CurrentPoint->PointName = HNET(Line);
  if (HNET(Line) == 0) {
   if ((PREVHNET(Line) == VNET(Line)) && (PREVHNET(Line) != 0)) {
    CurrentPoint->Layer1 = LEFT;
    CurrentPoint->Layer2 = NOP;
    CurrentPoint->Via    = TRUE;
   }
  }
  else {

 /*-------------------------------------------------------*/
 /*                  TRAITEMENT DES LAYER1                */
 /*-------------------------------------------------------*/
   if (SFRT(Line) != TERMINAL) 
    if (HNET(Line) == PREVHNET(Line)) CurrentPoint->Layer1 = NOP;
    else CurrentPoint->Layer1 = RIGHT;
   else
    if ((HNET(Line) == PREVHNET(Line)) || (VNET(Line) == PREVHNET(Line)))
     CurrentPoint->Layer1 = LEFT;
 /*-------------------------------------------------------*/
 /*                  TRAITEMENT DES VIAS                  */
 /*-------------------------------------------------------*/
   if (HNET(Line) == VNET(Line))
    if (SFRT(Line) == TERMINAL) 
     if (HNET(Line) == PREVHNET(Line)) CurrentPoint->Via = TRUE;
     else CurrentPoint->Via = FALSE;
    else CurrentPoint->Via = TRUE;
   else
    if (SFRT(Line) == TERMINAL) 
     if (VNET(Line) == PREVHNET(Line)) CurrentPoint->Via = TRUE;
 /*-------------------------------------------------------*/
 /*                  TRAITEMENT DES LAYER2                */
 /*-------------------------------------------------------*/
  if (VNET(Line) != 0)
   if (VNET(Line) == SCR_TOP)
    if (VNET(Line) != VNET(Line + 1)) CurrentPoint->Layer2 = UP;
    else CurrentPoint->Layer2 = NOP;
   else /* # SCR_TOP */
    if (VNET(Line) == SCR_BOTTOM(Density + 1))
     if (VNET(Line) != VNET(Line - 1)) CurrentPoint->Layer2 = DOWN;
     else CurrentPoint->Layer2 = NOP;
    else /* # SCR_BOTTOM */
     if (VNET(Line) != VNET(Line - 1)) CurrentPoint->Layer2 = DOWN;
     else
      if (VNET(Line) != VNET(Line + 1)) CurrentPoint->Layer2 = UP;
      else CurrentPoint->Layer2 = NOP;
  }  
# ifdef SCR_DEBUG
 fprintf(stdout,"Via = %d \t Layer1 = %c \t Layer2 = %c \t Name = %ld \n",
         CurrentPoint->Via, CurrentPoint->Layer1, CurrentPoint->Layer2,
         CurrentPoint->PointName);
# endif

 }
 if (SCR_BOTTOM(Density + 1) != 0)  {
  CurrentPoint->PointName = SCR_BOTTOM(Density + 1);
  CurrentPoint->Via    = FALSE;
  CurrentPoint->Layer1 = NOP;
  CurrentPoint->Layer2 = UP;
 }

# ifdef SCR_DEBUG
 fprintf(stdout,"Via = %d \t Layer1 = %c \t Layer2 = %c \t Name = %ld \n",
         CurrentPoint->Via, CurrentPoint->Layer1, CurrentPoint->Layer2,
         CurrentPoint->PointName);
 fprintf(stdout,"SouthCon = %ld \n",SCR_BOTTOM(Density + 1));
# endif

}

/******************************************************************************/
/*         SCR_InitCombi Modul of Initialisation                              */
/******************************************************************************/
void	SCR_InitCombi(Density)

long		Density;

{
 long	 Column     = LEADPATTERN;
 long	 MarkLine   = 1;

 for (MarkLine = 1; MarkLine <= (Density + 1) ; MarkLine++)
  for (Column = LEADPATTERN; Column <= EndPattern; Column++) 
   COMBI(Column,MarkLine) = 0;
 EndPattern = LEADPATTERN;
}

/******************************************************************************/
/*         SCR_InitTabRout Modul of Initialisation                            */
/******************************************************************************/
void SCR_InitTabRout()
{
long	I=0; 
long	J=0;

for (J = 0; J < MAXLINE; J++) {
 for (I = 0; I < MAXCOL; I++) {
  TabRout[I][J] = 0;
 }
}
}

/******************************************************************************/
/* Step 1 : Make Feasible Top and Bottom Connetions :                         */
/*	for each pin connection existing at the column being processed,       */
/*	connect it to an empty track or to a track occupied by the same net,  */
/*	whichever uses the least vertical wire. If the channel is fully       */
/*	occupied, bringing a new net is deferred until step 5. If two nets,   */
/*	one from the bottom and one from the top, create a confilct due to    */
/*	overlap,bring the one that uses the least wire, deferrin the other    */
/*	until step 5. If there are no empty tracks, a vertical straight-      */
/*	through connection is permissible.                                    */
/******************************************************************************/
void	Step1(Column, Density)

StaticColumn	*Column;
long	Density;

{
 long	Track = 0;
 long	Line = 0;
 long	MarkLine = 0;

 for (Line = 0; Line <= (Density + 1); Line++)
  VNET(Line) = ((long) 0);

 if (SCR_TOP == SCR_BOTTOM(Density + 1)) {
  if (SCR_TOP != 0) {
   for (Line = 1; ((HNET(Line) != 0) && (HNET(Line) != SCR_TOP) &&
        (Line <= Density)); Line++);
   if (Line > Density) /* pas piste libre ni meme noeud */
    if (!(SCR_Belong(SCR_TOP, Column->NextCol, NULL, 'N') ||
        SCR_Belong(SCR_TOP, Column->NextCol, NULL, 'S'))) { /* noeud terminal */
     for (Line = 0; Line <= (Density + 1); Line++)
      VNET(Line) = SCR_TOP;
      TOPNOTROUT = FALSE;
      BOTNOTROUT = FALSE;
    }
    else { /* on attend l'etape 5 */
     TOPNOTROUT = TRUE;
     BOTNOTROUT = FALSE;
    }
   else { /* existance piste libre ou meme noeud */
    HNET(Line) = SCR_TOP;
    for (Track = 0; Track <= Line; Track++)
     VNET(Track) = SCR_TOP;
    TOPNOTROUT = FALSE;
    for (Line = Density; ((HNET(Line) != 0) &&
         (HNET(Line) != SCR_BOTTOM(Density + 1)) && (Line >= 1)); Line--);
    HNET(Line) = SCR_BOTTOM(Density + 1);
    for (Track = (Density + 1); Track >= Line; Track--)
     VNET(Track) = SCR_BOTTOM(Density + 1);
    BOTNOTROUT = FALSE;
   }
  }
  else { /* Top = Bottom = 0 */
   TOPNOTROUT = FALSE;
   BOTNOTROUT = FALSE;
  }
 }
 else { /* Top != Bottom */
  if (SCR_TOP != 0) {
   for (Line = 1; ((HNET(Line) != 0) && (HNET(Line) != SCR_TOP) &&
        (Line <= Density)); Line++);
    if (Line > Density) TOPNOTROUT = TRUE;
    else {
     HNET(Line) = SCR_TOP;
     for (Track = 0; Track <= Line; Track++)
      VNET(Track) = SCR_TOP;
     TOPNOTROUT = FALSE; 
    }
  }
  else TOPNOTROUT = FALSE;  

  if (SCR_BOTTOM(Density + 1) != 0) {
   for (MarkLine = Density; ((VNET(MarkLine) == 0) && (MarkLine >= 1));
        MarkLine--);
   for (Line = Density; ((HNET(Line) != 0) &&
        (HNET(Line) != SCR_BOTTOM(Density + 1)) && (Line >= 1)
        && (Line > MarkLine)); Line--);
   if ((Line == 0) || (Line == MarkLine))  BOTNOTROUT = TRUE;
   else {
    HNET(Line) = SCR_BOTTOM(Density + 1);
    for (Track = (Density + 1); Track >= Line; Track--)
     VNET(Track) = SCR_BOTTOM(Density + 1);
    BOTNOTROUT = FALSE; 
   }
  } 
  else BOTNOTROUT = FALSE; 
 }
}

/******************************************************************************/
/* Step 2 : Free up As Many Tracks As Possible By Collapsing Split Nets       */
/*	Collapse as many split nets as possible. This an important step in    */
/*	the algorithm since it makes avaible to nets arriving at the          */
/*	channel to the right. A collapsing segment is a piece of vertical     */
/*	wire that connects two adjacent tracks occupied by the same net.      */
/*	A pattern consists of a set of collapsing segments where the          */
/*	segments for different nets do not overlap and no segment overlaps    */
/*	the routing placed in step 1. Each collapsing segment has a weight    */
/*	of either 1 or 2, depending on whether or not the net continues to    */
/*	the right beyond the current column. The weight represents the        */
/*	number of tracks freed due to collapse. The "winning" pattern is      */
/*	found by a combinatrial search that maximizes the weighted sum.       */
/*	If there is a tie, the pattern that leaves the outermost uncollapsed  */
/*	split net as far as possible from the channel edge is chosen. The     */
/*	idea is to keep the free area as close to the edges as possible.      */
/*	If necessary, the second outermost net is considered, and so on.      */
/*	If there are still remaining ties, use the pattern that maximizes     */
/*	the amount of vertical wire. The idea is to minimize the adverse      */
/*	effects on the future pattern due to large collapsing segments.       */
/*	The lists of tracks occupied by a net are updateto reflect the        */
/*	track merging for the pattern selected. If the collapsed track        */
/*	continues to the right, it will do so along the track that is closest */
/*	to the target edge (the side of the channel where the next terminal   */
/*	connection).                                                          */
/******************************************************************************/
void	Step2(Column, Density)

StaticColumn	*Column;
long		Density;

{
 long		MarkColumn  = 0;
 long		MarkLine    = 0;
 long		Track       = 0;
 long		Line        = 0;
 long		SplitNet    = 0;
 long		NotLayer2   = 0;
 BOOLEAN	NotFind = TRUE;
 long		Weight     = 0;
 long		MaxiWeight     = 0;
 long		LeadLayer2 = 0;
 long		EndLayer2 = 0;
 long		WeightTerminal = 0;

 if ((SCR_TOP == SCR_BOTTOM(Density + 1)) && (SCR_TOP != 0)) {
  for (Track = 1;Track <= Density; Track++)
   VNET(Track) = SCR_TOP;
 return;
 }
 else { /* Top != Bottom, Fusion noeuds multiple */

  SCR_SplitNet(Density);
  SCR_SFRT_Net(Column, Density);
  SCR_WeightTracks(Density);
  SCR_InitCombi(Density);

  for (Line = 1; Line <= Density; Line++) 
   if (SPLIT(Line) > 1) {
    for (SplitNet = SPLIT(Line); SplitNet > 1; SplitNet--) {
     for (Track = Line + 1; Track <= Density; Track++) {
      if (HNET(Line) == HNET(Track)) 
       if (SPLIT(Line) == SPLIT(Track)) break;
     }
     SPLIT(Track) = SPLIT(Line) - 1;
     for (NotLayer2 = Line; ((NotLayer2 <= Track) && ((VNET(NotLayer2) == 0)
          || (VNET(NotLayer2) == HNET(Line))));
          NotLayer2++); 
      if (NotLayer2 > Track) /* pas de Layer2 */
       if ((Track - Line) >= MINJOG_LENGTH) {
        NotFind = TRUE;
        for (MarkColumn = LEADPATTERN; MarkColumn <= EndPattern; MarkColumn++)
         if (COMBI(MarkColumn,Line) == 0) {
          NotFind = FALSE;
          for (MarkLine = Line; MarkLine <= Track; MarkLine++)
           COMBI(MarkColumn,MarkLine) = HNET(Line);
         }
        if (NotFind) {
         for (MarkLine = Line; MarkLine <= Track; MarkLine++)
          COMBI(MarkColumn,MarkLine) = HNET(Line);
         EndPattern = MarkColumn;
        }
       }
    }
    --SPLIT(Line);
   }
 }

 for (MarkColumn = LEADPATTERN; MarkColumn <= EndPattern;
      MarkColumn++, Weight = 0) {
  for (Track = 1; Track <= Density; Track++)
   if (COMBI(MarkColumn, Track) != 0) {
    LeadLayer2 = Track;
    for (Line = Track + 1;
         (COMBI(MarkColumn, Line) == COMBI(MarkColumn, LeadLayer2)); Line++);
    Track = EndLayer2 = --Line;
    if (SFRT(Track) == TERMINAL) 
     WeightTerminal = MAX(WEIGHT(Track),WEIGHT(Line));
    else WeightTerminal = 0;
    Weight = Weight + (EndLayer2 - LeadLayer2) + WeightTerminal;
  }
  COMBI(MarkColumn, (Density + 1)) = Weight;
 }

 for (MarkColumn = LEADPATTERN; MarkColumn <= (EndPattern + 1); MarkColumn += 2)
   MaxiWeight = MAX(MaxiWeight,MAX(COMBI(MarkColumn,(Density + 1)),
                                   COMBI((MarkColumn + 1),(Density + 1))));
  for (MarkColumn = LEADPATTERN;
       (MaxiWeight != COMBI(MarkColumn, (Density + 1))); MarkColumn++);
  for (Track = 1; Track <= Density; Track++)
    if (COMBI(MarkColumn, Track) != 0) {
     LeadLayer2 = Track;
     for (Track = Track + 1;
          ((COMBI(MarkColumn, Track) == COMBI(MarkColumn, LeadLayer2)) &&
          (Track <= Density)); Track++);
     EndLayer2 = Track - 1;
     for (Line = LeadLayer2; Line <= EndLayer2; Line++)
      VNET(Line) = COMBI(MarkColumn, EndLayer2);
     if (PREVHNET(LeadLayer2) == 0) HNET(LeadLayer2) = 0;
     else if (PREVHNET(EndLayer2) == 0) HNET(EndLayer2) = 0;
     Track--;
    }
}


/******************************************************************************/
/* Step 3 : Add Doglegs To Reduce The Range of Split Nets                     */
/*	Add jogs to reduce the range of split nets. For each uncollapsed      */
/*	split net, additional jogs are added so that the track in the highest */
/*	level goes as far down as possible and the one at the lowest level    */
/*	goes as far up as possible if such jogs are permissible. No jogs can  */
/*	be shorter than the minimum jog length discussed earlier.             */
/******************************************************************************/
void	Step3(Column, Density)

StaticColumn	*Column ;
long		Density;

{
long	Line = 0; 
long	Length = 0; 
long	Counter = 0;
long	MarkLine = 0; 
long	MarkLine1 = 0; 
long	Track = 0; 
long	ColumnPattern = LEADPATTERN;

/*****************************************************************************/
/*         Step3: 3.0     Search Nets to Reduce the Range (Top to Bottom)    */
/*****************************************************************************/
SCR_SplitNet(Density);
SCR_SFRT_Net(Column,Density);
SCR_InitCombi(Density);

for (Line = 1; Line <= Density ; Line++) {
 if (SPLIT(Line) > 1) {
  if ((SFRT(Line) == FALLING) || (SFRT(Line) == STEADY)) {
   for (Track = Line + 1; ((HNET(Track) != HNET(Line)) &&
        (Track <= Density));Track++) ;   
   if ((Track <= Density ) && ((Track-Line) >= JOG_LENGTH(Density))) {
    for (MarkLine = Line + 1; ((VNET(MarkLine) ==0) &&
         (MarkLine <= Track));MarkLine++) ;   
    if ((MarkLine-Line) >= JOG_LENGTH(Density)) { 
     for (MarkLine1 = MarkLine; ((HNET(MarkLine1) != 0) &&
          (MarkLine1 >= Line)); MarkLine1--);
     if ((MarkLine1-Line) >= JOG_LENGTH(Density)) {            
      Length=0;
      for (ColumnPattern = LEADPATTERN, MarkLine = Line;
           MarkLine<= MarkLine1 ;MarkLine++) {
       COMBI(ColumnPattern,MarkLine)=HNET(Line);
       Length++;
      }
      COMBI(ColumnPattern,Density+1)=Length;
      if (ColumnPattern <= MAXCOL ) ColumnPattern++;
      else  {
       fprintf(stdout,"MAXCOL insuffisant !!\n");
       exit(0);
      }
      EndPattern= ColumnPattern;
     }
    }
   }
  }
/*****************************************************************************/
/*         Step3: 3.1     Search Nets to reduce the Range (Bottom to Top)    */
/*****************************************************************************/
  if (SFRT(Line) == RISING) {
   for (Track = Line - 1; ((HNET(Track) != HNET(Line)) && 
        (Track >= 1));Track--);   
   if ((Track >= 1 ) && ((Line-Track) >= JOG_LENGTH(Density))) { 
    for (MarkLine = Line - 1; ((VNET(MarkLine) ==0) &&
         (MarkLine >= Track));MarkLine--) ;   
     if ((Line-MarkLine) >= JOG_LENGTH(Density)) { 
      for (MarkLine1 = MarkLine; ((HNET(MarkLine1) != 0) &&
           (MarkLine1 <= Line)); MarkLine1++) ;
      if ((Line-MarkLine1) >= JOG_LENGTH(Density)) {            
       Length=0;
       for (MarkLine = MarkLine1; MarkLine<= Line ;MarkLine++) {
        COMBI(ColumnPattern,MarkLine)=HNET(Line);
        Length++;
       }
       COMBI(ColumnPattern,Density+1)=Length;
       if (ColumnPattern <= MAXCOL ) ColumnPattern++;
       else {
        fprintf(stdout,"MAXCOL insuffisant !!\n");
        exit(0);
       } 
       EndPattern= ColumnPattern;
      }
     }
    }
   }
  }
 }
/**********************************************************************/
/*       Step3: 3.2     Select Patterns                               */
/**********************************************************************/

 if ( COMBI(LEADPATTERN,Density+1)> 0) { 
 for (ColumnPattern = LEADPATTERN, Track = 1, Length = 0;
      (ColumnPattern <= EndPattern); ColumnPattern++) {
  for (Counter=Track; Counter <= Density; Counter++) {
   if (COMBI(ColumnPattern, Counter) != 0) {
    COMBI((EndPattern+1), Counter) = COMBI(ColumnPattern, Counter); 
    Length++;
    Track = Counter;
   }
  }
 }
 COMBI((EndPattern+1),(Density+1)) = Length; 

 for (ColumnPattern = EndPattern, Track=Density, Length=0;
  ColumnPattern >= LEADPATTERN; ColumnPattern--) {
  for (Counter=Track; Counter >0; Counter--) {
   if (COMBI(ColumnPattern, Counter) != 0) {
    COMBI((EndPattern+2), Counter) = COMBI(ColumnPattern, Counter); 
    Length++;
    Track = Counter;
   }
  }
 }
 COMBI((EndPattern+2),(Density+1)) = Length; 

 if (COMBI((EndPattern+1),(Density+1)) > COMBI((EndPattern+2),(Density+1)))
  ColumnPattern=EndPattern+1;
 else ColumnPattern=EndPattern+2;
 if (COMBI(ColumnPattern, 1) != 0) { 
   VNET(1) = COMBI(ColumnPattern, 1); 
   HNET(1) = VNET(1) ;
 } 
 if (COMBI(ColumnPattern, Density) != 0) { 
  VNET(Density) = COMBI(ColumnPattern, Density); 
  HNET(Density) = VNET(Density) ;
 } 
 for(Track=2;Track<Density;Track++) {
  if (COMBI(ColumnPattern, Track) != 0) {
   if (COMBI(ColumnPattern, Track-1) == 0) { 
    VNET(Track) = COMBI(ColumnPattern, Track); 
    HNET(Track) = VNET(Track) ;
   } 
   else
    if (COMBI(ColumnPattern, Track+1) == 0) { /* Noeud finissant */
     VNET(Track) = COMBI(ColumnPattern, Track); 
     HNET(Track) = VNET(Track) ;
    } 
    else VNET(Track) = COMBI(ColumnPattern, Track);
  }
 } 
 EndPattern=EndPattern+2;
 } 
} 

/******************************************************************************/
/* Step 4 : Add Doglegs to Raise Rising and Lower Failling Nets               */
/*	Add jogs to raise nets and lower failling nets if such jogs are       */
/*	permissible and if the length of such jogs is greater that the        */
/*	minimum jog length.                                                   */
/******************************************************************************/
void	Step4(CurrentColumn, Density)

StaticColumn	*CurrentColumn;
long		Density;

{
 long	 Column     = LEADPATTERN;
 long	 MarkLine   = 1;
 long	 Line       = 1;
 long	 Track      = 1;
 BOOLEAN NotFind    = TRUE;
 long    MaxiWeight = 0;
 long    Weight     = 0;
 long	 LeadLayer2 = 0;
 long	 EndLayer2  = 0;

 SCR_InitCombi(Density);
 SCR_SFRT_Net(CurrentColumn, Density);
 for (MarkLine = 1; MarkLine <= Density; MarkLine++) 
  if (SFRT(MarkLine) == FALLING) {
   for (Track = MarkLine + 1; ((VNET(Track) == 0) && (Track <= Density));
        Track++) 
    if (HNET(Track) == 0) {
     NotFind = TRUE;
     if ((Track - MarkLine) >= JOG_LENGTH(Density)) {
      for (Column = LEADPATTERN; Column <= EndPattern; Column++) 
       if (COMBI(Column,MarkLine) == 0) {
        NotFind = FALSE;
        for (Line = MarkLine; Line <= Track; Line++)
         COMBI(Column,Line) = HNET(MarkLine);
       }
       if (NotFind) {
        for (Line = MarkLine; Line <= Track; Line++)
         COMBI(Column,Line) = HNET(MarkLine);
        EndPattern = Column;
       }
     }
    }
  } 
  else {
   if (SFRT(MarkLine) == RISING) {
    for (Track = MarkLine - 1; ((VNET(Track) == 0) && (Track >= 1)); Track-- ) 
     if (HNET(Track) == 0) {
      NotFind = TRUE;
      if ((MarkLine - Track) >= JOG_LENGTH(Density)) {
       for (Column = LEADPATTERN; Column <= EndPattern; Column++) 
        if (COMBI(Column,MarkLine) == 0) {
         for (Line = Track;((COMBI(Column,Line) == 0) && (Line <= MarkLine));
              Line++);
          if (Line > MarkLine) {
           NotFind = FALSE;
           for (Line = Track; Line <= MarkLine; Line++)
            COMBI(Column,Line) = HNET(MarkLine);
          }
        }
       if (NotFind) {
        for (Line = Track; Line <= MarkLine; Line++)
         COMBI(Column,Line) = HNET(MarkLine);
        EndPattern = Column;
       }
      }
     }
   }
  }
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*//*                                CHOICE PATTERN                              *//*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
 for (Column = LEADPATTERN; Column <= EndPattern; Column++, Weight = 0) {
  for (Track = 1; Track <= Density; Track++)
   if (COMBI(Column, Track) != 0) {
    LeadLayer2 = Track;
    for (Track = Track + 1;
         (COMBI(Column, Track) == COMBI(Column, LeadLayer2)); Track++);
    EndLayer2 = Track - 1;
    Weight = Weight + (EndLayer2 - LeadLayer2);
  }
  COMBI(Column, (Density + 1)) = Weight;
 }

 for (Column = LEADPATTERN; Column <= (EndPattern + 1); Column += 2)
  MaxiWeight = MAX(MaxiWeight,MAX(COMBI(Column,(Density + 1)),
                                  COMBI((Column + 1),(Density + 1))));
 for (Column = LEADPATTERN; (MaxiWeight != COMBI(Column, (Density + 1)));
      Column++);

 for (Track = 1; Track <= Density; Track++)
   if (COMBI(Column, Track) != 0) {
    LeadLayer2 = Track;
    for (Track = Track + 1;((Track <= Density) &&
         (COMBI(Column, Track) == COMBI(Column, LeadLayer2))); Track++);
    EndLayer2 = --Track;

    if (SFRT(LeadLayer2) == FALLING) {
     HNET(EndLayer2) = COMBI(Column, EndLayer2);
     if (SCR_TOP == COMBI(Column, LeadLayer2))
      HNET(LeadLayer2) = 0;
    }
    if (SFRT(EndLayer2) == RISING) {
     HNET(LeadLayer2) = COMBI(Column, LeadLayer2);
     if (SCR_BOTTOM(Density + 1) == COMBI(Column, LeadLayer2))
      HNET(EndLayer2) = 0;
    }
    for (MarkLine = LeadLayer2; MarkLine <= EndLayer2; MarkLine++)
     VNET(MarkLine) = COMBI(Column, EndLayer2);
   }
}

/******************************************************************************/
/* Step 5 : Widen Channel If Needed To Make Previously Infeasible Top or      */
/*          Bottom Connecions                                                 */
/*	If the nets in the current column could not be routed in step 1,      */
/*	add new tracks and bring them to these tracks. Such new tracks        */
/*	must be placed as near the center of the channel as possible if       */
/*	they do no conflict with existing wiring.                             */
/******************************************************************************/
void	Step5(LeadDataBase, Density, Width)

StaticColumn	*LeadDataBase;
long		*Density;
long		Width;

{
 long	Position = 0;
 long	Line     = 0;

 if (((*Density) + 2) >= MAXLINE) {
  fprintf(stdout,"\n IMPOSSIBLE DE CONTINUER PAR MANQUE DE LIGNE \n \n");
  fprintf(stdout," AUGMENTER MAXLINE DANS MAKEFILE !!! \n");
  exit(0);
 }
 else {
  if (TOPNOTROUT) {
   for (Position = 1; ((VNET(Position) == 0) &&
        (Position <= ((long ) ((*Density) >> 1)))); Position++);
   SCR_InsertNewTrack(LeadDataBase, Position, Width);
   SCR_InsertNewLine(Position, *Density);
   (*Density)++;
   PREVHNET(Position) = ((long ) 0);
   HNET(Position) = SCR_TOP;
   for (Line = 0; Line <= Position; Line++)
    VNET(Line) = SCR_TOP;
   TOPNOTROUT = FALSE;
  }
  if (BOTNOTROUT) {
   for (Position = (*Density); ((VNET(Position) == 0) && 
        (Position >= ((long ) ((*Density) >> 1)))); Position--);
   Position++;
   SCR_InsertNewTrack(LeadDataBase, Position, Width);
   SCR_InsertNewLine(Position, *Density);
   (*Density)++;
   PREVHNET(Position) = ((long ) 0);
   HNET(Position) = SCR_BOTTOM(*Density + 1);
   for (Line = Position; Line <= (*Density + 1); Line++)
    VNET(Line) = SCR_BOTTOM(*Density + 1);
   BOTNOTROUT = FALSE;
  }
 }
}
/******************************************************************************/
/* Step 6 : Extend To Next Column                                             */
/*	For each unsplit net that ended in the current column, delete the     */
/*	list of tracks occupied by the net. extend all tracks occupied by     */
/*	unfinished nets and split nets to the next column.                    */
/******************************************************************************/
void	Step6(Density)

long 		Density;

{
 long	Line = 0;

 for (Line = 1; Line <= Density; Line++) 
  if (SFRT(Line) == TERMINAL) {
   PREVHNET(Line) = 0;
   HNET(Line) = 0;
  }
  else PREVHNET(Line) = HNET(Line);
  VNET(Line) = 0;
}

/******************************************************************************/
/*                   ENDING NOT COLLAPSING SPLIT NETS                         */
/******************************************************************************/
void	SCR_EndingNotCollapsingNets(
				    EndRealChannel, LeadEastColumn,
				    Density, ChannelWidth
				   )

StaticColumn	*EndRealChannel;
StaticColumn	*LeadEastColumn;
long		Density;
long		*ChannelWidth;

{
 long		ColumnNumber   = 0;
 long		MarkColumn = 0;
 long		MarkLine = 0;
 long		Line     = 0;
 long		Track    = 0;
 BOOLEAN	NotFind = TRUE;
 BOOLEAN	InsertMode = FALSE;

 SCR_SplitNet(Density);
 SCR_InitCombi(Density);

# ifdef SCR_DEBUG
 fprintf(stdout,"In SCR_EndingNotCollapsingNets \n");
 U_PrintDynamicDataBase(Density);
# endif

 for (Line = 1; Line <= Density; Line++)
  if  (SPLIT(Line) > 1) {
   InsertMode = TRUE;
   break; 
  }
 if (InsertMode) {
  for (Line = 1; Line <= Density; Line++)
   if ((HNET(Line) != 0) && (SPLIT(Line) > 1)) {
    SCR_TOP = ((long ) 0);
    SCR_BOTTOM(Density + 1) = ((long ) 0);
    NotFind = TRUE;
    for (Track = Line + 1; ((SPLIT(Line) > 1) && (Track <= Density)); Track++)
     if (HNET(Track) == HNET(Line)) {
      SPLIT(Track) = ((long ) 1);
      SPLIT(Line)--;
     }
    for (MarkColumn = LEADPATTERN; MarkColumn <= EndPattern; MarkColumn++)
     if (COMBI(MarkColumn,Line) == 0) {
     NotFind = FALSE;
      for (MarkLine = Line; MarkLine < Track; MarkLine++)
       COMBI(MarkColumn,MarkLine) = HNET(Line);
      break;
     }
    if (NotFind) {
     for (MarkLine = Line; MarkLine < Track; MarkLine++)
      COMBI(MarkColumn,MarkLine) = HNET(Line);
     EndPattern = MarkColumn;
    }
   }
  
# ifdef SCR_DEBUG
 fprintf(stdout,"In SCR_EndingNotCollapsingNets \n");
 U_PrintDynamicDataBase(Density);
# endif
 
  ColumnNumber = (EndPattern - LEADPATTERN) + 1;
  SCR_InsertNewColumn(EndRealChannel, LeadEastColumn, Density, ColumnNumber);
  *ChannelWidth += ColumnNumber;
  for (MarkColumn = LEADPATTERN; MarkColumn <= EndPattern; MarkColumn++) {
   for (Line = 0; Line <= (Density + 1); Line++)
    VNET(Line) = ((long) 0);
   for (Line = 1; Line <= Density; Line++)
    VNET(Line) = COMBI(MarkColumn,Line);
   SCR_UpDateDataBase(EndRealChannel->NextCol, Density);
 
# ifdef SCR_DEBUG
 fprintf(stdout,"In SCR_EndingNotCollapsingNets befor Step6\n");
 U_PrintDynamicDataBase(Density);
# endif
 
   Step6(Density);
 
   EndRealChannel = EndRealChannel->NextCol;
 
# ifdef SCR_DEBUG
 fprintf(stdout,"In SCR_EndingNotCollapsingNets \n");
 fprintf(stdout,"In SCR_EndingNotCollapsingNets after Step6\n");
 U_PrintDynamicDataBase(Density);
# endif
  }
 }
}

/******************************************************************************/
/*                        GREEDY ROUTER ALGORITHM                             */
/******************************************************************************/
void	SCR_GreedyRouter(
			 LeadStaticDataBase, EndRealChannel,
			 LeadEastColumn, Density, Width, ChannelWidth
			)

StaticColumn	*LeadStaticDataBase;
StaticColumn	*EndRealChannel;
StaticColumn	*LeadEastColumn;
long		*Density;
long		Width;
long		*ChannelWidth;

{
 StaticColumn	*CurrentColumn = NULL;

 SCR_InitTabRout();

 for(CurrentColumn = LeadStaticDataBase; CurrentColumn; 
     CurrentColumn = CurrentColumn->NextCol) {
  if (CurrentColumn == LeadEastColumn) {
   SCR_EndingNotCollapsingNets( 
			       EndRealChannel, LeadEastColumn, 
			       *Density, ChannelWidth
			      );
  }
  SCR_TOP = CurrentColumn->NorthCon;
  SCR_BOTTOM(*Density + 1) = CurrentColumn->SouthCon;

# ifdef SCR_DEBUG
 fprintf(stdout,"Befor Routing Column \n");
 U_PrintDynamicDataBase(*Density);
 fprintf(stdout,"In Step1 \n");
# endif

  Step1(CurrentColumn, *Density);
 
# ifdef SCR_DEBUG
 U_PrintDynamicDataBase(*Density);
 fprintf(stdout,"In Step2 \n");
# endif

  Step2(CurrentColumn, *Density);

# ifdef SCR_DEBUG
 U_PrintDynamicDataBase(*Density);
/* 
fprintf(stdout,"In Step3 \n");
*/
# endif
/*
  Step3(CurrentColumn, *Density);
*/

# ifdef SCR_DEBUG
 /*
U_PrintDynamicDataBase(*Density);
*/
 fprintf(stdout,"In Step4 \n");
# endif

  Step4(CurrentColumn, *Density);

# ifdef SCR_DEBUG
 U_PrintDynamicDataBase(*Density);
 fprintf(stdout,"In Step5 \n");
# endif

  Step5(LeadStaticDataBase, Density, Width);

# ifdef SCR_DEBUG
 U_PrintDynamicDataBase(*Density);
# endif

  SCR_UpDateDataBase(CurrentColumn, *Density);

# ifdef SCR_DEBUG
 fprintf(stdout,"In Step6 \n");
# endif

  Step6(*Density);

# ifdef SCR_DEBUG
 U_PrintDynamicDataBase(*Density);
# endif
 }
 SCR_EndingNotCollapsingNets(EndRealChannel, LeadEastColumn, 
                             *Density, ChannelWidth);
}

/******************************************************************************/
