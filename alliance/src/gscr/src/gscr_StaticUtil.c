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
/*                                                                            */
/*                    CAO & VLSI's cad tools chain Alliance                   */
/*                                                                            */
/*     Product  : Symbolic Channel Router                                     */
/*     File     : @(#)                                                        */
/*     Contents :                                                             */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   : El housseine REJOUAN               Date : ../../....     */
/*     Modified by : El housseine REJOUAN               Date : 29/07/1992     */
/*     Modified by :                                    Date : ../../....     */
/*     Modified by :                                    Date : ../../....     */
/*                                                                            */
/******************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <mut.h>
# include <mlo.h>
# include <mph.h>
# include <mpu.h>
# include "apr.h"
# include "gscr_main.h"
# include "gscr_DataBase.h"

int L3MODE;

/***********************************************************************/
/*       CHARGEMENT DES LISTES NORTH, SOUTH, WEST ET EAST              */
/***********************************************************************/
ConnectorList   *U_GetConList(ptfich)

FILE            *ptfich;

{
 ConnectorList  *LeadConList = NULL;
 ConnectorList  *CurrentList = NULL;
 ConnectorList  *PrevCurrentList = NULL;
 long           Mark = 1;
 char		car;
 char		string[10];
 int		i=0;


 if ((car = getc(ptfich)) == '{') 
  if ((car = getc(ptfich)) != '}') {
   while ((car != ' ') && (car != ',') && (car != '}')) {
    if (car != '\n') string[i++] = car;
    car = getc(ptfich);
   }
   string[i] = '\0';
   LeadConList = (ConnectorList *) mbkalloc (sizeof(ConnectorList));
   PrevCurrentList = LeadConList;
   LeadConList->NextCon = NULL;
   LeadConList->ConName = atol(string);
   LeadConList->Mark = Mark;
   Mark++;
   
   while (car != '}') {
    i = 0;
    car = getc(ptfich);
    while ((car != ' ') && (car != ',') && (car != '}')) {
     if (car != '\n') string[i++] = car;
     car = getc(ptfich);
    }
    string[i] = '\0';
    CurrentList = (ConnectorList *) mbkalloc (sizeof(ConnectorList));
    PrevCurrentList->NextCon = CurrentList;
    PrevCurrentList = CurrentList;
    CurrentList->NextCon = NULL;
    CurrentList->ConName = atol(string);
    CurrentList->Mark = Mark;
    Mark++;
   }
  }
  else LeadConList = NULL;
 car = getc(ptfich);
 return (LeadConList);
}
/******************************************************************************/
/*	Function : U_GetChannel()                                             */
/******************************************************************************/
void	U_GetChannel(FichName, LeadNorthList, LeadSouthList, 
                     LeadWestList, LeadEastList)

char		*FichName;
ConnectorList   **LeadNorthList;
ConnectorList   **LeadSouthList;
ConnectorList   **LeadWestList;
ConnectorList   **LeadEastList;

{
 FILE	*ptfich;
 
 if ((ptfich = fopen(FichName,"r")) == NULL)  {
  fprintf(stdout,"fichier inexistant \n");
  exit(0);
 }
 else {
  *LeadNorthList = U_GetConList(ptfich);
  *LeadSouthList = U_GetConList(ptfich);
  *LeadWestList  = U_GetConList(ptfich);
  *LeadEastList  = U_GetConList(ptfich);
  fclose(ptfich);
 }
}
/******************************************************************************/
/*                  VISUALISATION DE LA LISTE DES CONNECTEURS                 */
/******************************************************************************/
void  U_PrintList(LeadConList)

ConnectorList	*LeadConList;

{
 ConnectorList	*CurrentConList;
 CurrentConList = LeadConList;

 if (LeadConList)
  while (CurrentConList) {
   fprintf(stderr, "ConName= %ld \t  Mark= %ld \n", CurrentConList->ConName,
                                                    CurrentConList->Mark);
   CurrentConList = CurrentConList->NextCon;
  }
}

/******************************************************************************/
/*              VISUALISATION DE LA LISTE DES COLONNES                        */
/******************************************************************************/
void U_PrintColumn(LeadColumn)

StaticColumn	*LeadColumn;
{
 StaticColumn	*CurrentColumn = NULL;

 CurrentColumn = LeadColumn;
 while (CurrentColumn) {
  fprintf(stderr, "NorthCon = %ld \t SouthCon = %ld \n",CurrentColumn->NorthCon,
         CurrentColumn->SouthCon);
  CurrentColumn = CurrentColumn->NextCol;
 }
}

/******************************************************************************/
/*                        LARGEUR DU CANAL                                    */
/******************************************************************************/
long	U_ChannelWidth(LeadNorth, LeadSouth)

StaticColumn	*LeadNorth;
StaticColumn	*LeadSouth;

{
 StaticColumn	*CurrentColumn = NULL;
 long		NorthWidth = 0;
 long		SouthWidth = 0;

 for (CurrentColumn = LeadNorth; CurrentColumn;
      CurrentColumn = CurrentColumn->NextCol, NorthWidth++);
 for (CurrentColumn = LeadSouth; CurrentColumn;
      CurrentColumn = CurrentColumn->NextCol, SouthWidth++);
 return(MAX(NorthWidth, SouthWidth));
}

/******************************************************************************/
/*                  VISUALISATION DE LA BASE DE DONNEE STATIQUE               */
/******************************************************************************/
void U_PrintStaticDataBase(LeadColumn)

StaticColumn	*LeadColumn;

{
 StaticColumn	*CurrentColumn = NULL;
 StaticPoint	*CurrentPoint  = NULL;
  
 for (CurrentColumn = LeadColumn;CurrentColumn;
      CurrentColumn = CurrentColumn->NextCol) {
  fprintf(stderr, "NorthCon = %ld \t SouthCon = %ld \n",CurrentColumn->NorthCon,
         CurrentColumn->SouthCon);
  for (CurrentPoint = CurrentColumn->PointList; CurrentPoint;
       CurrentPoint = CurrentPoint->NextPoint) 
   fprintf(stderr, "Via = %d \t Layer1 = %c \t Layer2 = %c \t Name = %ld \n",
          CurrentPoint->Via, CurrentPoint->Layer1, CurrentPoint->Layer2,
          CurrentPoint->PointName);
 }
}
/******************************************************************************/
/*               VISUALISATION DE LA LISTE DES  SEGMENTS                      */
/******************************************************************************/
void	U_PrintSegmentList(LeadList)

SegmentList	*LeadList;

{
 SegmentList	*CurrentList = NULL;

 for (CurrentList = LeadList; CurrentList; CurrentList = CurrentList->NextSeg) {
  fprintf(stderr,"Name = %s \t X1Seg = %3ld \t Y1Seg = %3ld \t ",
          CurrentList->SegName, CurrentList->X1Seg, CurrentList->Y1Seg);
  fprintf(stderr,"X2Seg= %3ld \t Y2Seg= %3ld \n",
          CurrentList->X2Seg, CurrentList->Y2Seg);
 }
          
}

/******************************************************************************/
/*               VISUALISATION DE LA LISTE DES  SEGMENTS                      */
/******************************************************************************/
void	U_PrintViasList(LeadList)

ViasList	*LeadList;

{
 ViasList	*CurrentList = NULL;

 for (CurrentList = LeadList; CurrentList; CurrentList = CurrentList->NextVia)
  fprintf(stderr,"XVia = %ld \t YVia = %ld \t \n",
          CurrentList->XVia, CurrentList->YVia);
}

/******************************************************************************/
/* Function :                                                                 */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*	Input parameters :                                                    */
/*      Input global Variables :                                              */
/*	Output parameters :                                                   */
/*      Output global Variables :                                             */
/*                                                                            */
/******************************************************************************/
void		gscr2mbk(FigName,LeadHorSeg,LeadVerSeg,LeadVia,
                         LeadNorthCon,LeadSouthCon,LeadWestCon,
                         LeadEastCon,ChannelWidth,ChannelHeight)
char            *FigName;
SegmentList     *LeadHorSeg;
SegmentList     *LeadVerSeg;
ViasList        *LeadVia;
ConnectorList   *LeadNorthCon;
ConnectorList   *LeadSouthCon;
ConnectorList   *LeadWestCon;
ConnectorList   *LeadEastCon;
long            ChannelWidth;
long            ChannelHeight;

{
 phfig_list     *ptFigure   = NULL;
 SegmentList    *CurrentSeg = NULL;
 ViasList       *CurrentVia = NULL;
 ConnectorList	*CurrentCon = NULL;
 long		X1Symbolic  = 0;
 long		X2Symbolic  = 0;
 long		Y1Symbolic  = 0;
 long		Y2Symbolic  = 0;
 long		X1Segment   = 0;
 long		X2Segment   = 0;
 long		Y1Segment   = 0;
 long		Y2Segment   = 0;
 long		Xab1 = 0;
 long		Xab2 = 0;
 long		Yab1 = 0;
 long		Yab2 = 0;
 long		ConName = 0;
 char		Buffer[10];

 ptFigure = addphfig(FigName);
 Xab1 = Yab1 = ptFigure->XAB1 = ptFigure->YAB1 = 0;
 Xab2 = ptFigure->XAB2 = 
        ((ChannelWidth - 1) * PITCH_X) + WESTOFFSET + EASTOFFSET;
 Yab2 = ptFigure->YAB2 = 
        ((ChannelHeight - 1) * PITCH_Y) + SOUTHOFFSET + NORTHOFFSET;

/******************************************************************************/
/*              SAVING HORIZONTAL SEGMENT                                     */
/******************************************************************************/
 for (CurrentSeg = LeadHorSeg; CurrentSeg; CurrentSeg = CurrentSeg->NextSeg) {
  if ((X1Symbolic = CurrentSeg->X1Seg) == 0) 
   X1Segment = Xab1;
  else 
   X1Segment = ((X1Symbolic - 1) * PITCH_X) + WESTOFFSET;
  if ((X2Symbolic = CurrentSeg->X2Seg) == (ChannelWidth + 1)) 
   X2Segment = Xab2;
  else 
   X2Segment = ((X2Symbolic - 1) * PITCH_X) + WESTOFFSET;
  Y1Segment = Y2Segment = (((CurrentSeg->Y1Seg - 1) * PITCH_Y) + SOUTHOFFSET);
  addphseg(ptFigure, ((char )ALU1), LAYER1WIDTH, X1Segment, Y1Segment,
           X2Segment , Y2Segment, CurrentSeg->SegName);
 }

/******************************************************************************/
/*              SAVING VERTICAL SEGMENT                                       */
/******************************************************************************/
 for (CurrentSeg = LeadVerSeg; CurrentSeg; CurrentSeg = CurrentSeg->NextSeg) {
  if ((Y1Symbolic = CurrentSeg->Y1Seg) == 0)
   Y1Segment = Yab1;
  else 
   Y1Segment = ((Y1Symbolic - 1) * PITCH_Y) + SOUTHOFFSET;
  if ((Y2Symbolic = CurrentSeg->Y2Seg) == ChannelHeight + 1) 
   Y2Segment = Yab2;
  else 
   Y2Segment = ((Y2Symbolic - 1) * PITCH_Y) + SOUTHOFFSET;
  X1Segment = X2Segment = ((CurrentSeg->X1Seg - 1) * PITCH_X) + WESTOFFSET;
  addphseg(ptFigure, ((char )ALU2), LAYER2WIDTH, X1Segment, Y1Segment, 
           X2Segment, Y2Segment, CurrentSeg->SegName);
 }

/******************************************************************************/
/*              SAVING VIA                                                    */
/******************************************************************************/
 for (CurrentVia = LeadVia; CurrentVia; CurrentVia = CurrentVia->NextVia) {
  addphvia(ptFigure, ((char ) CONT_VIA), 
           ((CurrentVia->XVia - 1) * PITCH_X) + WESTOFFSET,
           ((CurrentVia->YVia - 1) * PITCH_Y) + SOUTHOFFSET,0,0,NULL);
 }

/******************************************************************************/
/*              SAVING CONNECTOR                                              */
/******************************************************************************/
 for (CurrentCon = LeadNorthCon; CurrentCon; CurrentCon = CurrentCon->NextCon) {
  if ((ConName = CurrentCon->ConName) != NONET) {
   sprintf(Buffer,"%ld",ConName);
   addphcon(ptFigure, NORTH, Buffer, 
            ((CurrentCon->Mark - 1) * PITCH_X) + WESTOFFSET,
            Yab2, ((char ) ALU2), LAYER2WIDTH);
  }
 }
 for (CurrentCon = LeadSouthCon; CurrentCon; CurrentCon = CurrentCon->NextCon) {
  if ((ConName = CurrentCon->ConName) != NONET) {
   sprintf(Buffer,"%ld",ConName);
   addphcon(ptFigure, SOUTH, Buffer, 
            ((CurrentCon->Mark - 1) * PITCH_X) + WESTOFFSET,
            Yab1, ((char ) ALU2), LAYER2WIDTH);
  }
 }
 for (CurrentCon = LeadWestCon; CurrentCon; CurrentCon = CurrentCon->NextCon) {
  sprintf(Buffer,"%ld",CurrentCon->ConName);
  addphcon(ptFigure, WEST, Buffer, Xab1,
           ((CurrentCon->Mark - 1) * PITCH_Y) + SOUTHOFFSET,
           ((char ) ALU1), LAYER1WIDTH);
 }
 for (CurrentCon = LeadEastCon; CurrentCon; CurrentCon = CurrentCon->NextCon) {
  sprintf(Buffer,"%ld",CurrentCon->ConName);
  addphcon(ptFigure, EAST, Buffer, Xab2,
           ((CurrentCon->Mark - 1) * PITCH_Y) + SOUTHOFFSET,
           ((char ) ALU1), LAYER1WIDTH);
 }
/******************************************************************************/
/*              SAVING FIGURE                                                 */
/******************************************************************************/
 savephfig(ptFigure);
}

/******************************************************************************/
/*              SAUVEGARDE DU RESULTAT DE ROUTAGE                             */
/******************************************************************************/
void	U_SaveChannel(
                      FigName, LeadH_Segment, LeadV_Segment,
                      LeadVia, Width, Height,
		      LeadWestList, LeadEastList
                     )

char		*FigName;
SegmentList	*LeadH_Segment;
SegmentList	*LeadV_Segment;
ViasList	*LeadVia;
long		Width;
long		Height;
ConnectorList   *LeadWestList;
ConnectorList   *LeadEastList;

{
 phfig_list	*ptfig = NULL;
 SegmentList	*CurrentSegment = NULL;
 SegmentList	*CurrentSeg = NULL;
 ViasList	*CurrentVia     = NULL;
 long		Layer1Length    = 0; 
 long		Layer2Length    = 0; 
 long		ViasNumber      = 0;
 long           X1Symbolic  = 0;
 long           X2Symbolic  = 0;
 long           Y1Symbolic  = 0;
 long           Y2Symbolic  = 0;
 long           X1Segment  = 0;
 long           X2Segment  = 0;
 long           Y1Segment  = 0;
 long           Y2Segment  = 0;
 long           Xab1  = 0;
 long           Xab2  = 0;
 long           Yab1  = 0;
 long           Yab2  = 0;

 ptfig = addphfig(FigName);

fprintf(stdout,"Width = %ld \n",Width);
fprintf(stdout,"Height = %ld \n",Height);

 Xab1 = Yab1 = ptfig->XAB1 = ptfig->YAB1 = ((long ) 0);
/*
 ptfig->XAB2 = ((Width + 1) * PITCH_X);
 ptfig->YAB2 = ((Height + 1 )  * PITCH_Y);
*/

 Xab2 = ptfig->XAB2 = ((Width - 1) * PITCH_X) + WESTOFFSET + EASTOFFSET;
 Yab2 = ptfig->YAB2 = ((Height - 1) * PITCH_Y) + SOUTHOFFSET + NORTHOFFSET;

fprintf(stderr,"XAB1 = %ld \n",ptfig->XAB1);
fprintf(stderr,"XAB2 = %ld \n",ptfig->XAB2);
fprintf(stderr,"YAB1 = %ld \n",ptfig->YAB1);
fprintf(stderr,"YAB2 = %ld \n",ptfig->YAB2);

fflush(stderr);

/******************************************************************************/
/*              SAVING HORIZONTAL SEGMENT                                     */
/******************************************************************************/
 for (CurrentSeg = LeadH_Segment; CurrentSeg; CurrentSeg = CurrentSeg->NextSeg) {
  if ((X1Symbolic = CurrentSeg->X1Seg) == 0) 
   X1Segment = Xab1;
  else 
   X1Segment = ((X1Symbolic - 1) * PITCH_X) + WESTOFFSET;
  if ((X2Symbolic = CurrentSeg->X2Seg) == (Width + 1)) 
   X2Segment = Xab2;
  else 
   X2Segment = ((X2Symbolic - 1) * PITCH_X) + WESTOFFSET;
  Y1Segment = Y2Segment = (((CurrentSeg->Y1Seg - 1) * PITCH_Y) + SOUTHOFFSET),
  addphseg(ptfig, ((char )ALU1), LAYER1WIDTH, X1Segment,
           Y1Segment, X2Segment , Y2Segment, CurrentSeg->SegName);
 }

/******************************************************************************/

/*
 for (CurrentSegment = LeadH_Segment; CurrentSegment;
      CurrentSegment = CurrentSegment->NextSeg) {
  addphseg(ptfig, ((char ) ALU1), LAYER1WIDTH,
           ((CurrentSegment->X1Seg) * PITCH_X), 
           ((CurrentSegment->Y1Seg) * PITCH_Y), 
           ((CurrentSegment->X2Seg) * PITCH_X), 
           ((CurrentSegment->Y2Seg) * PITCH_Y), 
	   CurrentSegment->SegName);
  Layer1Length = Layer1Length + (CurrentSegment->X2Seg - CurrentSegment->X1Seg);
 }
 Layer1Length = Layer1Length * PITCH_X;
*/
                    
 for (CurrentSegment = LeadV_Segment; CurrentSegment;
      CurrentSegment = CurrentSegment->NextSeg) {
  addphseg(ptfig, ((char ) ALU2), LAYER2WIDTH,
           ((CurrentSegment->X1Seg) * PITCH_X), 
           ((CurrentSegment->Y1Seg) * PITCH_Y), 
           ((CurrentSegment->X2Seg) * PITCH_X), 
           ((CurrentSegment->Y2Seg) * PITCH_Y), 
	   CurrentSegment->SegName);
  Layer2Length = Layer2Length + (CurrentSegment->Y2Seg - CurrentSegment->Y1Seg);
 }
 Layer2Length = Layer2Length * PITCH_Y;

 for (CurrentVia = LeadVia; CurrentVia; CurrentVia = CurrentVia->NextVia) {
  addphvia(ptfig, ((char ) CONT_VIA), 
           ((CurrentVia->XVia) * PITCH_X),
           ((CurrentVia->YVia) * PITCH_Y),0,0,NULL);
  ViasNumber = ViasNumber + 1;
 }

 for (CurrentSegment = LeadV_Segment; CurrentSegment;
      CurrentSegment = CurrentSegment->NextSeg) {
  if (CurrentSegment->Y1Seg == 0) 
   addphcon(ptfig, SOUTH, CurrentSegment->SegName, 
            ((CurrentSegment->X1Seg) * PITCH_X),
            ((CurrentSegment->Y1Seg) * PITCH_Y),
            ((char ) ALU2), (LAYER2WIDTH));
  if (CurrentSegment->Y2Seg == (Height + 1)) 
   addphcon(ptfig, NORTH, CurrentSegment->SegName, 
            ((CurrentSegment->X2Seg) * PITCH_X),
            ((CurrentSegment->Y2Seg) * PITCH_Y),
            ((char ) ALU2), (LAYER2WIDTH));
 }

 if (LeadWestList && LeadEastList) {
  for (CurrentSegment = LeadH_Segment; CurrentSegment;
       CurrentSegment = CurrentSegment->NextSeg) {
   if (CurrentSegment->X1Seg == 0) 
    addphcon(ptfig, WEST, CurrentSegment->SegName,
             ((CurrentSegment->X1Seg) * PITCH_X),
             ((CurrentSegment->Y1Seg) * PITCH_Y),
             ((char ) ALU1), (LAYER1WIDTH));
   if (CurrentSegment->X2Seg == (Width - 1)) 
    addphcon(ptfig, EAST, CurrentSegment->SegName,
             ((CurrentSegment->X2Seg) * PITCH_X),
             ((CurrentSegment->Y2Seg) * PITCH_Y),
             ((char ) ALU1), (LAYER1WIDTH));
  }
 }
 else
  if (LeadWestList) {
   for (CurrentSegment = LeadH_Segment; CurrentSegment;
       CurrentSegment = CurrentSegment->NextSeg) 
   if (CurrentSegment->X1Seg == 0) 
    addphcon(ptfig, WEST, CurrentSegment->SegName,
             ((CurrentSegment->X1Seg) * PITCH_X),
             ((CurrentSegment->Y1Seg) * PITCH_Y),
             ((char ) ALU1), (LAYER1WIDTH));
  }
  else 
   if (LeadEastList) {
    for (CurrentSegment = LeadH_Segment; CurrentSegment; 
        CurrentSegment = CurrentSegment->NextSeg) 
    if (CurrentSegment->X2Seg == (Width - 1)) 
     addphcon(ptfig, EAST, CurrentSegment->SegName, 
              ((CurrentSegment->X2Seg) * PITCH_X), 
              ((CurrentSegment->Y2Seg) * PITCH_Y),
              ((char ) ALU1), (LAYER1WIDTH));
   }

fprintf(stdout,"Layer1 Length = %ld \n",Layer1Length);
fprintf(stdout,"Layer2 Length = %ld \n",Layer2Length);
fprintf(stdout,"ViasNumber = %ld \n",ViasNumber);
 savephfig(ptfig);
}
/******************************************************************************/
