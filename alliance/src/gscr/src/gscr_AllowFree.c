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
/*                              STANDARD CELLS ROUTER                         */
/*       FILE : SCR_AllowFree.c   LAST MODIFICATION : JUL/8/1991              */
/******************************************************************************/

# include "gscr_DataBase.h"
# include "apr.h"
# include <mut.h>
# include <mlo.h>
# include <mph.h>

/******************************************************************************/
/*                      ALLOCATION D'UNE PISTE                                */
/******************************************************************************/
StaticPoint	*SCR_AllowOneTrack(Width)

long		Width;

{
 StaticPoint	*LeadTrack        = NULL;
 StaticPoint	*CurrentPoint     = NULL;
 StaticPoint	*PrevCurrentPoint = NULL;
 long		Size              = 0;

 LeadTrack = (StaticPoint *) mbkalloc (sizeof(StaticPoint));
 LeadTrack->NextPoint = NULL;
 LeadTrack->RightPoint = NULL;
 LeadTrack->PointName = ((long ) 0);
 LeadTrack->Via    = FALSE;
 LeadTrack->Layer1 = NOP;
 LeadTrack->Layer2 = NOP;
 PrevCurrentPoint = LeadTrack;
 for (Size = 1; Size < Width; Size++) {
  CurrentPoint = (StaticPoint *) mbkalloc (sizeof(StaticPoint));
  PrevCurrentPoint->RightPoint = CurrentPoint;
  PrevCurrentPoint = CurrentPoint;
  CurrentPoint->NextPoint = NULL;
  CurrentPoint->RightPoint = NULL;
  CurrentPoint->PointName = ((long ) 0);
  CurrentPoint->Via    = FALSE;
  CurrentPoint->Layer1 = NOP;
  CurrentPoint->Layer2 = NOP;
 }
 return(LeadTrack);
}

/******************************************************************************/
/*              ALLOCATION D'UN ENSEMBLE DE PISTES                            */
/******************************************************************************/
StaticPoint	*SCR_AllowSetTrack(Density, Width)

long		Density;
long		Width;

{
 StaticPoint	*LeadSetTrack     = NULL;
 StaticPoint	*CurrentTrack     = NULL;
 StaticPoint	*NewTrack         = NULL;
 StaticPoint	*PrevCurrentTrack = NULL;
 long		Size              = 0;

 LeadSetTrack = SCR_AllowOneTrack(Width);
 PrevCurrentTrack = LeadSetTrack;
 for (Size = 1; Size < Density; Size++) {
  for (NewTrack = CurrentTrack = SCR_AllowOneTrack(Width); CurrentTrack;
       CurrentTrack = CurrentTrack->RightPoint,
       PrevCurrentTrack = PrevCurrentTrack->RightPoint)
   PrevCurrentTrack->NextPoint = CurrentTrack;
  PrevCurrentTrack = NewTrack;
 }
 return(LeadSetTrack);
}

/******************************************************************************/
/*              ALLOCATION DE COLONNE SANS POINTs                             */
/******************************************************************************/
StaticColumn	*SCR_AllowColumn(Width)

long		Width;

{
 StaticColumn	*LeadColumn        = NULL;
 StaticColumn	*CurrentColumn     = NULL;
 StaticColumn	*PrevCurrentColumn = NULL;
 long		Size               = 0;

 LeadColumn = (StaticColumn *) mbkalloc (sizeof(StaticColumn));
 LeadColumn->NextCol   = NULL;
 LeadColumn->PrevCol   = NULL;
 LeadColumn->NorthCon  = ((long ) 0);
 LeadColumn->SouthCon  = ((long ) 0);
 LeadColumn->PointList = NULL;
 PrevCurrentColumn     = LeadColumn;
 for (Size = 1; Size < Width ; Size++) {
  CurrentColumn = (StaticColumn *) mbkalloc (sizeof(StaticColumn));
  PrevCurrentColumn->NextCol = CurrentColumn;
  CurrentColumn->PrevCol = PrevCurrentColumn;
  PrevCurrentColumn = CurrentColumn;
  CurrentColumn->NextCol   = NULL;
  CurrentColumn->NorthCon  = ((long ) 0);
  CurrentColumn->SouthCon  = ((long ) 0);
  CurrentColumn->PointList = NULL;
 }
 return(LeadColumn);
}

/******************************************************************************/
/*               ALLOCATION D'UN CANAL                                        */
/******************************************************************************/
StaticColumn	*SCR_AllowChannel(LeadStaticDataBase, Density, Width)

StaticColumn	*LeadStaticDataBase;
long		Density;
long		Width;

{
 StaticColumn	*CurrentColumn   = NULL;
 StaticPoint	*CurrentSetTrack = NULL;

 CurrentSetTrack = SCR_AllowSetTrack(Density, Width);
 for (CurrentColumn = LeadStaticDataBase; CurrentColumn;
      CurrentColumn = CurrentColumn->NextCol,
      CurrentSetTrack = CurrentSetTrack->RightPoint) 
  CurrentColumn->PointList = CurrentSetTrack;
 return(LeadStaticDataBase);
}

/*----------------------------------------------------------------------------*/
/* Les fonctions d'allocation suivantes seront utlisees au cas ou             */
/* il faut ajouter une nouvelle colonne                                       */
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/*              ALLOCATION D'UNE LISTE DE POINTs                              */
/******************************************************************************/
StaticPoint	*SCR_AllowStaticPoint(Density)

long	Density;

{
 StaticPoint	*LeadStaticPoint  = NULL;
 StaticPoint	*CurrentPoint     = NULL;
 StaticPoint	*PrevCurrentPoint = NULL;
 long		Size              = 0;

 LeadStaticPoint = (StaticPoint *) mbkalloc (sizeof(StaticPoint));
 LeadStaticPoint->NextPoint = NULL;
 LeadStaticPoint->RightPoint = NULL;
 LeadStaticPoint->PointName = ((long ) 0);
 LeadStaticPoint->Via    = FALSE;
 LeadStaticPoint->Layer1 = NOP;
 LeadStaticPoint->Layer2 = NOP;
 PrevCurrentPoint = LeadStaticPoint;
 for (Size = 1; Size < Density; Size++) {
  CurrentPoint = (StaticPoint *) mbkalloc (sizeof(StaticPoint));
  PrevCurrentPoint->NextPoint = CurrentPoint;
  PrevCurrentPoint = CurrentPoint;
  CurrentPoint->NextPoint = NULL;
  CurrentPoint->RightPoint = NULL;
  CurrentPoint->PointName = ((long ) 0);
  CurrentPoint->Via    = FALSE;
  CurrentPoint->Layer1 = NOP;
  CurrentPoint->Layer2 = NOP;
 }
 return(LeadStaticPoint);
}

/******************************************************************************/
/*               ALLOCATION D'UNE COLONNE AVEC POINTs                         */
/******************************************************************************/
StaticColumn	*SCR_AllowOneColumn(Density)

long	Density;
{
 StaticColumn	*Column = NULL;

 Column = (StaticColumn *) mbkalloc (sizeof(StaticColumn));
 Column->NextCol = NULL;
 Column->PrevCol = NULL;
 Column->NorthCon = ((long ) 0);
 Column->SouthCon = ((long ) 0);
 Column->PointList = SCR_AllowStaticPoint(Density);
 return(Column);
}

/******************************************************************************/
/*              LIBERATION DE LA MEMOIRE                                      */
/******************************************************************************/
void	SCR_FreeDataBase(LeadDataBase)

StaticColumn	*LeadDataBase;

{
 StaticColumn	*CurrentColumn      = NULL;
 StaticColumn	*AfterCurrentColumn = NULL;
 StaticPoint	*CurrentPoint       = NULL;
 StaticPoint	*AfterCurrentPoint  = NULL;

 CurrentColumn = LeadDataBase;
 AfterCurrentColumn = LeadDataBase->NextCol;
 while(CurrentColumn->NextCol) {
  CurrentPoint = CurrentColumn->PointList;
  AfterCurrentPoint = CurrentPoint->NextPoint;
  while (CurrentPoint->NextPoint) {
   mbkfree(((void * ) CurrentPoint));
   CurrentPoint = AfterCurrentPoint;
   AfterCurrentPoint = CurrentPoint->NextPoint;
  }
  mbkfree(((void * ) CurrentPoint));
  mbkfree(((void * ) CurrentColumn));
  CurrentColumn = AfterCurrentColumn;
  AfterCurrentColumn = CurrentColumn->NextCol;
 }
 mbkfree(((void * ) CurrentColumn));
} 

/******************************************************************************/
/*          ALLOCATION MEMOIRE POUR LA LISTE DES SEGMENTs                     */
/******************************************************************************/
SegmentList	*SCR_AllowSegmentList()

{
 SegmentList	*LeadSegmentList = NULL;

 LeadSegmentList = (SegmentList *) mbkalloc (sizeof(SegmentList));
 LeadSegmentList->NextSeg = NULL;
 LeadSegmentList->SegName = '\0';
 LeadSegmentList->X1Seg = ((long ) 0);
 LeadSegmentList->Y1Seg = ((long ) 0);
 LeadSegmentList->X2Seg = ((long ) 0);
 LeadSegmentList->Y2Seg = ((long ) 0);
 return (LeadSegmentList);
} 

/******************************************************************************/
/*          ALLOCATION MEMOIRE POUR LA LISTE DES VIAs                         */
/******************************************************************************/
ViasList	*SCR_AllowViasList()

{
 ViasList	*LeadViaList = NULL;

 LeadViaList = (ViasList *) mbkalloc (sizeof(ViasList));
 LeadViaList->NextVia = NULL;
 LeadViaList->XVia = ((long ) 0);
 LeadViaList->YVia = ((long ) 0);
 return (LeadViaList);
} 

/******************************************************************************/
/*                           FREE OF SEGMENT DATA BASE                        */
/******************************************************************************/
void	SCR_FreeSegmentList(LeadSegList)

SegmentList	*LeadSegList;

{
 SegmentList	*CurrentSeg      = NULL;
 SegmentList	*AfterCurrentSeg = NULL;

 CurrentSeg = LeadSegList;
 AfterCurrentSeg = LeadSegList->NextSeg;
 while (CurrentSeg->NextSeg) {
  mbkfree(((void * ) CurrentSeg));
  CurrentSeg = AfterCurrentSeg;
  AfterCurrentSeg = CurrentSeg->NextSeg;
 }
 mbkfree(((void * ) CurrentSeg));
}

/******************************************************************************/
/*                           FREE OF  VIA DATA BASE                           */
/******************************************************************************/
void	SCR_FreeViasList(LeadViaList)

ViasList	*LeadViaList;

{
 ViasList	*CurrentVia      = NULL;
 ViasList	*AfterCurrentVia = NULL;

 CurrentVia = LeadViaList;
 AfterCurrentVia = LeadViaList->NextVia;
 while(CurrentVia->NextVia) {
  mbkfree(((void * ) CurrentVia));
  CurrentVia = AfterCurrentVia;
  AfterCurrentVia = CurrentVia->NextVia;
 }
  mbkfree(((void *) CurrentVia));
}

/******************************************************************************/
