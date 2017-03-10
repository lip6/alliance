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
/*    Fichier : ViewDataBase.c                                                */
/*                                                                            */
/*    (c) copyright 1991 Laboratoire MASI equipe CAO & VLSI                   */
/*    Tous droits reserves                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                   */
/*                                                                            */
/*    Auteur(s) : El housseine REJOUAN                  le : 26/10/1991       */
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

/******************************************************************************/
/* function DecodLayer()                                                       /
/******************************************************************************/
char *DecodLayer(index)
char index;
{
	switch (index) {
		case NWELL :
			return "nwell";
		case PWELL :
			return "pwell";
		case NTIE :
			return "ntie";
		case PTIE :
			return "ptie";
		case NDIF :
			return "ndif";
		case PDIF :
			return "pdif";
		case NTRANS :
			return "ntrans";
		case PTRANS :
			return "ptrans";
		case POLY :
			return "poly";
		case ALU1 :
			return "metal1";
		case ALU2 :
			return "metal2";
		case ALU3 :
			return "metal3";
		case TPOLY :
			return "allowP";
		case TALU1 :
			return "allowM1";
		case TALU2 :
			return "allowM2";
		case TALU3 :
			return "allowM3";
	}
	return NULL; /* makes lint silent */
}

/******************************************************************************/
/*	Function : ViewConList()                                              */
/******************************************************************************/
void		ViewConList(ptLeadList)

ConnectorList	*ptLeadList;

{
 ConnectorList	*CurrentCon = NULL;

 for (CurrentCon = ptLeadList; CurrentCon; CurrentCon = CurrentCon->NextCon) 
  fprintf(stdout,"|____(NAME,INDEX) : ( %ld , %ld ) \n",
          CurrentCon->ConName,CurrentCon->Mark);
}
  
/******************************************************************************/
/*	Function : ViewScrChannel()                                           */
/******************************************************************************/
void		ViewScrChannel(ptfig)

Figure		*ptfig;

{
 Line		*ptLine = NULL;
 Channel	*ptChannel = NULL;

 for(ptLine = ptfig->LINE->NEXT; ptLine->NEXT->NEXT;
     ptLine = ptLine->NEXT->NEXT) {
  ptChannel = ptLine->CHANNEL;
  if (ptChannel) {
   fprintf(stdout,"|__ChannelName : %s \n",ptLine->NAME);
   fprintf(stdout,"|__________|__NorthConList : \n");
   ViewConList(ptChannel->NORTH_LIST);
   fprintf(stdout,"|__________|__SouthConList : \n");
   ViewConList(ptChannel->SOUTH_LIST);
   fprintf(stdout,"|__________|__WestConList  : \n");
   ViewConList(ptChannel->WEST_LIST);
   fprintf(stdout,"|__________|__EastConList  : \n");
   ViewConList(ptChannel->EAST_LIST);
  }
 }
}

/******************************************************************************/
/*   fonction ViewScrSegment()                                                */
/******************************************************************************/
void		ViewScrSegment(ptSeg)

Segment		*ptSeg;

{
 Segment	*CurrentSeg;
 
 for (CurrentSeg = ptSeg ; CurrentSeg; CurrentSeg = CurrentSeg->NEXT) {
   printf("|__SEG.NAME________________: %s \n",CurrentSeg->NAME);
   printf("| |__SEG.(X1,Y1) : (%ld , %ld ) \n",CurrentSeg->X1,CurrentSeg->Y1);
   printf("| |__SEG.(X2,Y2) : (%ld , %ld ) \n",CurrentSeg->X2,CurrentSeg->Y2);
   printf("| |__SEG.(WIDTH) : %ld \n",CurrentSeg->WIDTH);
   printf("| |__SEG.(TYPE)  : ( %c ) \n",CurrentSeg->TYPE);
   printf("| |__SEG.(LAYER) : ( %s ) \n",DecodLayer(CurrentSeg->LAYER));
 }
}
 
/******************************************************************************/
/*  fonction ViewScrCon()                                                     */
/******************************************************************************/
void		ViewScrCon(ptCon)

Connector	*ptCon;

{
 Connector	*CurrentCon = NULL;
 
 for ( CurrentCon = ptCon ; CurrentCon; CurrentCon = CurrentCon->NEXT ) {
  printf("|__CON.NAME____________________: %s \n",CurrentCon->NAME);
  printf("| |__CON.(TYPE)      : ( %c )\n",CurrentCon->TYPE);
  printf("| |__CON.(DIRECT)    : ( %c )\n",CurrentCon->DIRECTION);
  printf("| |__CON.(ORIENT)    : ( %c )\n",CurrentCon->ORIENT);
  printf("| |__CON.(X,Y)       : ( %ld, %ld )\n",CurrentCon->X,CurrentCon->Y);
  printf("| |__CON.(WIDTH)     : ( %ld )\n",CurrentCon->WIDTH);
  printf("| |__CON.(LAYER)     : ( %s )\n",DecodLayer(CurrentCon->LAYER));
  printf("| |__CON.(SIG_INDEX) : ( %ld )\n",CurrentCon->SIG->INDEX);
  printf("| |__CON.(LINE_INDEX): ( %ld )\n",CurrentCon->LINE->INDEX);
  if (CurrentCon->INST != NULL)
   printf("| |__CON.(LINE_INST) : ( %s )\n",CurrentCon->INST->NAME);
 }
}

/******************************************************************************/
/*   fonction ViewScrInstance()                                               */
/******************************************************************************/
void		ViewScrInstance(ptLeadIns)

Instance		*ptLeadIns;

{
 Instance		*ptIns = NULL;
 
  for ( ptIns = ptLeadIns ; ptIns; ptIns = ptIns->NEXT ) {
   printf("|  |__INS.INSNAME__________: %s \n",ptIns->NAME );
   printf("|  |  |__INS.FIGNAME   : %s \n", ptIns->FIGNAME );
   printf("|  |  |__INS.(X,WIDTH) : ( %ld , %ld ) \n",ptIns->X,ptIns->WIDTH);
   printf("|  |  |__INS.(Y,HEIGTH): ( %ld , %ld ) \n",ptIns->Y,ptIns->HEIGTH);
   printf("|  |  |__INS.TRANSF    : ( %c ) \n",ptIns->TRANSF );
   printf("|  |  |__Connecteurs  de l'instance : \n");
   ViewScrCon(ptIns->CON);
   printf("|  |  |__Transparences  de la cellule : \n");
   if (ptIns->ALLOW == NULL) 
    fprintf(stdout,"Pas de TRANSPARENCES dans cette cellule.\n");
   else ViewScrSegment(ptIns->ALLOW);
  }
}  
 
/******************************************************************************/
/*     fonction ViewScrSignal()                                               */
/******************************************************************************/
void		ViewScrSignal(ptLeadSig)

Signal		*ptLeadSig;

{
 Signal		*ptSig    = NULL;
 chain_list	*ConList  = NULL;
 chain_list	*LineList = NULL;
 
 for (ptSig = ptLeadSig; ptSig; ptSig = ptSig->NEXT ) {
  printf("|____SIG.NAME__________________: %s \n",ptSig->NAME);
  printf("| |__SIG.(INDEX,TYPE) : ( %ld , %c ) \n",ptSig->INDEX,ptSig->TYPE);
  printf("|       |__Connecteurs du signal : \n");
  for (ConList = ptSig->CON; ConList; ConList = ConList->NEXT ) 
   printf("|      |__SIG.(CON_X) : (  %ld ) \n",
          ((Connector *) ConList->DATA)->X);
  printf("|       |__Lignes du signal : \n");
  for (LineList = ptSig->LINE; LineList; LineList = LineList->NEXT ) 
   printf("|      |__SIG.(INDEX_LINE) : ( %ld ) \n",
          ((Line *) LineList->DATA)->INDEX);
  printf("| |__SIG.(MAXCON) : ( %ld ) \n",ptSig->MAXCON);
 }
}

/******************************************************************************/
/*    fonction ViewScrLine()                                                  */
/******************************************************************************/
void		ViewScrLine(ptLeadLine)

Line		*ptLeadLine;

{
 Line		*ptLine    = NULL;
 chain_list	*SigList   = NULL;
 chain_list	*ConList   = NULL;
 chain_list	*AllowList = NULL;
 Segment	*ptAllow   = NULL;

 for (ptLine = ptLeadLine; ptLine; ptLine = ptLine->NEXT) {
  printf("|_LIN.Name____________________: %s \n", ptLine->NAME);
  printf("|  |__LIN.(TYPE,INDEX) :( %c, %ld )\n", ptLine->TYPE, ptLine->INDEX);
  printf("|  |__LIN.(X,WIDTH)    :( %ld, %ld )\n", ptLine->X , ptLine->WIDTH);
  printf("|  |__LIN.(Y,HEIGTH)   :( %ld, %ld )\n", ptLine->Y, ptLine->HEIGTH);
  if (ptLine->TYPE == 'C') {
   printf("|_________Cellules de la ligne    : \n");
   ViewScrInstance(ptLine->INS);
  }
  printf("|       |__signaux de la ligne : \n");
  for (SigList = ptLine->SIG; SigList; SigList = SigList->NEXT)
   printf("| |__LIN.(SIG_INDEX) : ( %ld )\n",
          ((Signal *) SigList->DATA)->INDEX);
  printf("|       |__connecteurs de la ligne : \n");
  for (ConList = ptLine->CON; ConList; ConList = ConList->NEXT)
   printf("| |__LIN.(CON_NAME) : ( %s )\n",
          ((Connector *) ConList->DATA)->NAME);
  printf("|       |__transprences utilisees de la ligne : \n");
  for (ptAllow = ptLine->ALLOWUSED; ptAllow; ptAllow = ptAllow->NEXT) {
   printf("| |__LIN.(ALLOWUSED_X1_Y1) : ( %ld , %ld )\n",
          ptAllow->X1,ptAllow->Y1);
   printf("| |__LIN.(ALLOWUSED_X2_Y2) : ( %ld , %ld )\n",
          ptAllow->X2,ptAllow->Y2);
  }
  printf("|       |__transprences de la ligne : \n");
  for (AllowList = ptLine->ALLOW; AllowList; AllowList = AllowList->NEXT) {
   printf("| |__LIN.(ALLOW_X1_Y1) : ( %ld , %ld )\n",
          ((Segment *) AllowList->DATA)->X1,((Segment *) AllowList->DATA)->Y1);
   printf("| |__LIN.(ALLOW_X2_Y2) : ( %ld , %ld )\n",
          ((Segment *) AllowList->DATA)->X2,((Segment *) AllowList->DATA)->Y2);
  }
 }
}

/******************************************************************************/
/*    fonction ViewScrFigure()                                                */
/******************************************************************************/
void		ViewScrFigure(ptRoot)
 
Figure		*ptRoot;

{
 fprintf(stdout,"Les LIGNES de la figure : \n\n");
 ViewScrLine(ptRoot->LINE);
 fprintf(stdout,"Les SIGNAUX de la figure : \n\n");
 ViewScrSignal(ptRoot->SIG);
 fprintf(stdout,"Les CONNECTEURS de la figure : \n\n");
 ViewScrCon(ptRoot->CON);
 fprintf(stdout,"Les TRANSPARENCES de la figure : \n\n");
 ViewScrSegment(ptRoot->ALLOW);
}
