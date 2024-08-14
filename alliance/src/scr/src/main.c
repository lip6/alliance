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
/*    Produit : STANDARD CELLS ROUTER                                         */
/*    Fichier : main.c                                                        */
/*                                                                            */
/*    (c) copyright 1991 Laboratoire MASI equipe CAO & VLSI                   */
/*    Tous droits reserves                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                   */
/*                                                                            */
/*    Auteur(s) : El housseine REJOUAN                  le : 26/10/1991       */
/*                                                                            */
/*    Modifie par : El housseine REJOUAN                le : 04/08/1992       */
/*    Modifie par : El housseine REJOUAN                le : 04/06/1993       */
/*    Modifie par :                                     le : ../../....       */
/*                                                                            */
/******************************************************************************/
#define NOL3DEBUG
#ifdef  L3DEBUG
#define LOGld(t,v) fprintf(stderr,"%30s%30s : %ld\n",t,#v,v)
#define LOGs(t,v) fprintf(stderr,"%30s%30s : %s\n",t,#v,v)
#define LOGf(t,v) fprintf(stderr,"%30s%30s : %f\n",t,#v,v)
#else
#define LOGld(t,v) 
#define LOGs(t,v) 
#define LOGf(t,v) 
#endif

# include <string.h>
# include <mut.h>
# include <mlo.h> 
# include <mph.h> 
# include <apr.h> 
# include "SCR_Type.h"
# include "main.h"
# include "LoadDataBase.h"
# include "ViewDataBase.h"

PlaceConList   *LeadPlaceConList = NULL;
extern int L3MODE=0;
extern int SXMODE;
extern int SCR_RATIO;
char *NameVdd=NULL;
char *NameVss=NULL;


 void rdsenv ();
 void loadrdsparam();

/******************************************************************************/
/* Function : ScrUsage()                                                      */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*	Input parameters :                                                    */
/*      Input global Variables : none                                         */
/*	Output parameters :  none                                             */
/*      Output global Variables : none                                        */
/*                                                                            */
/******************************************************************************/
void	ScrUsage(Execut)
char	*Execut;

{
 fprintf(stderr,"scr_error : Syntax error on command line\n");
 fprintf(stderr,"usage : %s [options...] <source>\n",Execut);
 fprintf(stderr,"where valid options are :\n");
 fprintf(stderr," <source> : netlist file and placed layout (same name)\n");
 fprintf(stderr," [-c ChannelName]     : if the option ChannelName is used\n");
 fprintf(stderr,"                        then the result will be hierarchical\n");
 fprintf(stderr,"                        otherwise the result will be flattened\n");
 fprintf(stderr," [-o OutputFileName]  : renames the output file name to FileName\n");
 fprintf(stderr," [-p]                 : This option invokes the placement process\n");
 fprintf(stderr," [-r]                 : This option invokes the routing process\n");
 fprintf(stderr," [-i IterationNumber] : This action is used to improve the quality of the placement\n");
 fprintf(stderr," [-l SliceNumber]     : This option allows the designer to set the number of rows\n");
 fprintf(stderr," [-a SupplyNumber]    : This option allows the designer to set the number of power supply\n");
 fprintf(stderr,"for more informations use man scr\n");
 exit(1);
}

/******************************************************************************/
/* Function : GetOptions()                                                    */
/*                                                                            */
/* Description : getting all used options on commad line                      */
/*                                                                            */
/*	Input parameters : argument count and argument value                  */
/*      Input global Variables : none                                         */
/*	Output parameters : option list pointer                               */
/*      Output global Variables : none                                        */
/*                                                                            */
/******************************************************************************/
OptionList	*GetOptions(ArgCount,ArgValue)
int		ArgCount;
char		*ArgValue[];

{
 OptionList	*ptOption = (OptionList *) mbkalloc (sizeof(OptionList));
 int		ArgNumber = 0;

 ptOption->ChannelName  = NULL;
 ptOption->InputFileName = NULL;
 ptOption->OutputFileName = NULL;
 ptOption->Placer       = FALSE;
 ptOption->Router       = FALSE;
 ptOption->SupplyRecall = 1;
 ptOption->Row          = 0;
 ptOption->Iteration    = 0;
 
 L3MODE = (getenv ("SCR_L3MODE")) ? 1 : 0;
 SXMODE = (getenv ("SCR_SCLIB")) ? 0 : 1;
 if (L3MODE) SXMODE=1;
 if (getenv ("SCR_RATIO"))
  SCR_RATIO = atol(getenv ("SCR_RATIO"));
 else
  SCR_RATIO = L3MODE?70:100;
 for (ArgNumber = 1; ArgNumber < ArgCount; ArgNumber++) {
  int AuxArgNumber = ArgNumber;
  char *ArgV = ArgValue[ArgNumber];
  if (*ArgV == '-') {
   switch (*++ArgV) {

    case 'c' : if (ArgValue[++AuxArgNumber] == NULL) 
                ScrUsage(ArgValue[0]);
               else
                ptOption->ChannelName = ArgValue[++ArgNumber];
               continue;
    case 'o' : if (ArgValue[++AuxArgNumber] == NULL) 
                ScrUsage(ArgValue[0]);
               else
                ptOption->OutputFileName = ArgValue[++ArgNumber];
               continue;
    case 'p' : if (*++ArgV == '\0') ptOption->Placer = TRUE;
               else ScrUsage(ArgValue[0]);
               continue;

    case 'r' : if (*++ArgV == '\0') ptOption->Router = TRUE;
               else ScrUsage(ArgValue[0]);
               continue;

    case 'l' : if ((*++ArgV) == '\0') 
                if (ArgValue[++AuxArgNumber] == NULL) 
                 ScrUsage(ArgValue[0]);
                else
                 ptOption->Row = atoi(ArgValue[++ArgNumber]);
               else ScrUsage(ArgValue[0]);
               continue;

    case 'a' : if ((*++ArgV) == '\0') 
                if (ArgValue[++AuxArgNumber] == NULL) 
                 ScrUsage(ArgValue[0]);
                else
                 ptOption->SupplyRecall = atoi(ArgValue[++ArgNumber]);
               else ScrUsage(ArgValue[0]);
               continue;

    case 'i' : if (*++ArgV == '\0') 
                if (ArgValue[++AuxArgNumber] == NULL) 
                 ScrUsage(ArgValue[0]);
                else
                 ptOption->Iteration = atoi(ArgValue[++ArgNumber]);
               else ScrUsage(ArgValue[0]);
               continue;

    default  : ScrUsage(ArgValue[0]);
   }
  }
  else ptOption->InputFileName = ArgV;
 }
 return(ptOption);
}

/*******************************************************************************
* function ScrNameIndex()                                                         *
* return a string that is the concatenation of the name argument, the mbk      *
* separator, and an index                                                      *
*******************************************************************************/
char *ScrNameIndex(name, index)
char *name;
long index;
{
char str[100];

        (void)sprintf(str,"%s%ld", name, index);
        return namealloc(str);
}

/******************************************************************************/
/* Function : PreparVerticalChannel()                                         */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*      Input parameters :                                                    */
/*      Input global Variables :                                              */
/*      Output parameters :                                                   */
/*      Output global Variables :                                             */
/*                                                                            */
/******************************************************************************/
XSupplyRecallList   *PreparVerticalChannel(ptScrFig, ptMbkTie, Number, Counter)
Figure  	*ptScrFig;
phfig_list	*ptMbkTie;
int		Number;
long    	*Counter;

{
 long		Interval = (long ) (((ptScrFig->LINE->WIDTH) / (Number + 1))
                                    + 0.5);
 long		XBreak   = ptScrFig->LINE->X;
 Line           *ptLine = NULL;
 Instance       *CurrentInst = NULL;
 Instance       *PrevInst = NULL;
 Instance       *ptNewInst = NULL;
 long           XMin    = 0;
 long           XMax    = 0;
 long           NewXMin = 0;
 long           WidthShift = 0;
 long           TieNumber = 0;
 long           TieNumberSave = 0;
 long           XInsert = 0;
 long           XInsertSouth = 0;
 Connector      *CurrentCon = NULL;
 chain_list     *ConList = NULL;
 XSupplyRecallList        *CurrentX = NULL;
 XSupplyRecallList        *ptXSave = NULL;
 XSupplyRecallList        *ptXNewSave = NULL;
 Segment        *CurrentSeg = NULL;
 Segment        *ptNewAllow = NULL;
 long		TieWidth = (ptMbkTie->XAB2 - ptMbkTie->XAB1);
 long		TieHeight  = (ptMbkTie->YAB2 - ptMbkTie->YAB1);

 while (Number > 0) {
  XBreak += (Interval + WidthShift);
  XMin = XBreak;
  XMax = ptScrFig->LINE->X;

  /* CALCUL DE LA LARGEUR DE DECALAGE */
  for (ptLine = ptScrFig->LINE; ptLine; ptLine = ptLine->NEXT) {
   if (ptLine->TYPE == CELL) {
    for (PrevInst = NULL, CurrentInst = ptLine->INS; 
         (CurrentInst && ((NewXMin = CurrentInst->X) < XBreak));
         PrevInst = CurrentInst, CurrentInst = CurrentInst->NEXT);
    if ((XBreak - PrevInst->X) <= (NewXMin - XBreak))
     NewXMin = PrevInst->X;
    if (NewXMin < XMin) XMin = NewXMin;
    if (NewXMin > XMax) XMax = NewXMin;
   }
  } 
  WidthShift = (XMax - XMin);
  if ((WidthShift % TieWidth) >= (TieWidth / (2 * SCALE_X))) {
   TieNumber = (WidthShift / TieWidth) + 1; 
  }
  else {
   TieNumber = (WidthShift / TieWidth); 
  }
  WidthShift = TieNumber * TieWidth;
  TieNumberSave = TieNumber; 

/* SAUVEGARDE DES POSITIONS DES LIGNES DE COUPE */
 ptXNewSave = (XSupplyRecallList *) mbkalloc (sizeof(XSupplyRecallList));
 ptXNewSave->NEXT = NULL;
 ptXNewSave->X = XMax;
 if (ptXSave == NULL) ptXSave = ptXNewSave;
 else {
  for (CurrentX = ptXSave; CurrentX->NEXT; CurrentX = CurrentX->NEXT);
  CurrentX->NEXT = ptXNewSave;
 }
  for (ptLine = ptScrFig->LINE; ptLine; ptLine = ptLine->NEXT) {
   ptLine->WIDTH += WidthShift;
   if (ptLine->TYPE == CELL) {
    for (PrevInst = NULL, CurrentInst = ptLine->INS;
         (CurrentInst && (CurrentInst->X < XBreak));
         PrevInst = CurrentInst, CurrentInst = CurrentInst->NEXT);
    if ((XBreak - PrevInst->X) <= (CurrentInst->X - XBreak))
     CurrentInst = PrevInst;
    XInsert = CurrentInst->X;
    if (ptLine == (ptScrFig->LINE->NEXT->NEXT)) XInsertSouth = XInsert;
    if (CurrentInst != ptLine->INS) {
     for (PrevInst = ptLine->INS; PrevInst->NEXT != CurrentInst;
          PrevInst = PrevInst->NEXT);
    }
    else PrevInst = NULL;
    TieNumber = TieNumberSave; 
    while (TieNumber > 0) {
     ptNewInst = CreateInstance(ScrNameIndex("feed",(*Counter)++),
                                ptMbkTie->NAME,XInsert,ptLine->Y,
                                TieWidth,TieHeight,NOSYM);
     if (PrevInst) {
      ptNewInst->NEXT = CurrentInst;
      PrevInst->NEXT = ptNewInst;
     }
     else {
      ptNewInst->NEXT = CurrentInst;
      ptLine->INS = ptNewInst;
     }

     ptNewAllow = InsertAllowInIns(ptNewInst,ptMbkTie->XAB1,ptMbkTie->XAB2,
                                 ptMbkTie->YAB1,ptMbkTie->YAB2,ptMbkTie);
     ptLine->ALLOW = addchain(ptLine->ALLOW,(char *)ptNewAllow);
     PrevInst = ptNewInst;
     XInsert += TieWidth;
     TieNumber--;
    }
    for (CurrentSeg = ptLine->ALLOWUSED; CurrentSeg;
         CurrentSeg = CurrentSeg->NEXT)
     if (CurrentSeg->X1 >= XInsert) {
      CurrentSeg->X1 += WidthShift;
      CurrentSeg->X2 += WidthShift;
     }
  /*MISE A JOUR DES CONNECTEURS DES LIGNES */
    for (ConList = ptLine->CON; ConList; ConList = ConList->NEXT){
     CurrentCon = ((Connector *)ConList->DATA);
     if ((!CurrentCon->INST) && (CurrentCon->X >= XInsert))
      CurrentCon->X += WidthShift;
    }
  /*MISE A JOUR DES CONNECTEURS INTERNES ET DES SEGMENT DES INSTANCES */
    for (; CurrentInst; CurrentInst = CurrentInst->NEXT) {
     CurrentInst->X += WidthShift;
     for (CurrentCon = CurrentInst->CON; CurrentCon;
          CurrentCon = CurrentCon->NEXT)
      CurrentCon->X += WidthShift;
     for (CurrentSeg = CurrentInst->ALLOW; CurrentSeg;
          CurrentSeg = CurrentSeg->NEXT) {
      CurrentSeg->X1 += WidthShift;
      CurrentSeg->X2 += WidthShift;
     }
    } 
   }
  } 
/* MISE A JOUR DES CONNECTEURS EXTERNES */
  XInsert -= WidthShift;
  for(CurrentCon = ptScrFig->CON; CurrentCon; CurrentCon = CurrentCon->NEXT) {
   if (CurrentCon->ORIENT == EAST)
     CurrentCon->X += WidthShift;
   else
    if ((CurrentCon->ORIENT == NORTH) && (CurrentCon->X >= XInsert))
     CurrentCon->X += WidthShift;
    else
     if ((CurrentCon->ORIENT == SOUTH) && (CurrentCon->X >= XInsertSouth))
      CurrentCon->X += WidthShift;
  } 
  Number--;
 }
 return(ptXSave);
}

/******************************************************************************/
/* Function : MakeVerticalChannel()                                           */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*	Input parameters :                                                    */
/*      Input global Variables :                                              */
/*	Output parameters :                                                   */
/*      Output global Variables :                                             */
/*                                                                            */
/******************************************************************************/
void			MakeVerticalChannel(ptFigure, ptXInsert, ChannelWidth)
Figure			*ptFigure;
XSupplyRecallList	*ptXInsert;
long			ChannelWidth;

{
 XSupplyRecallList	*ptXBreak = NULL;
 XSupplyRecallList	*ptXBreakList = NULL;
 Line           	*ptLine = NULL;
 Instance       	*ptInst = NULL;
 Instance       	*PrevInst = NULL;
 Connector      	*ptCon = NULL;
 Segment        	*ptSeg = NULL;
 SegmentList    	*ptSegList = NULL;
 ViasList       	*ptViaList = NULL;
 long           	WidthShiftSymb = ChannelWidth / PITCH_X;
 long           	XOpen = 0;
 long           	XOpenSymb = 0;

 for (ptXBreak = ptXInsert; ptXBreak; ptXBreak = ptXBreak->NEXT) {
  XOpen = ptXBreak->X;
  XOpenSymb = (XOpen / PITCH_X) + 1;
  for (ptLine = ptFigure->LINE; ptLine; ptLine = ptLine->NEXT) {
       ptLine->WIDTH += ChannelWidth;
   if (ptLine->TYPE == CELL) {
    for (PrevInst = NULL, ptInst = ptLine->INS; 
         (ptInst && (ptInst->X < XOpen));
         PrevInst = ptInst, ptInst = ptInst->NEXT);
    for (; ptInst; ptInst = ptInst->NEXT) {
     ptInst->X += ChannelWidth;
     for (ptCon = ptInst->CON; ptCon; ptCon = ptCon->NEXT)
          ptCon->X += ChannelWidth;
     for (ptSeg = ptInst->ALLOW; ptSeg; ptSeg = ptSeg->NEXT) {
      ptSeg->X1 += ChannelWidth;
      ptSeg->X2 += ChannelWidth;
     }
    } 
   } 
   else {
    if (ptLine->TYPE == PATH) {
     ptLine->CHANNEL->WIDTH += WidthShiftSymb;
     for (ptSegList = ptLine->CHANNEL->H_SEGMENT; ptSegList;
          ptSegList = ptSegList->NextSeg) {
      if (ptSegList->X1Seg >= XOpenSymb) ptSegList->X1Seg += WidthShiftSymb;
      if (ptSegList->X2Seg >= XOpenSymb) ptSegList->X2Seg += WidthShiftSymb;
     }
     for (ptSegList = ptLine->CHANNEL->V_SEGMENT; ptSegList;
          ptSegList = ptSegList->NextSeg) {
      if (ptSegList->X1Seg >= XOpenSymb) {
       ptSegList->X1Seg += WidthShiftSymb;
       ptSegList->X2Seg += WidthShiftSymb;
      }
     }
     for (ptViaList = ptLine->CHANNEL->VIA; ptViaList;
          ptViaList = ptViaList->NextVia) {
      if (ptViaList->XVia >= XOpenSymb) {
       ptViaList->XVia += WidthShiftSymb;
      }
     }
    }
   }
  }
  
  /* MISE A JOUR DES TRANSPARENCES VERTICALES */
  for (ptSeg = ptFigure->ALLOW; ptSeg; ptSeg = ptSeg->NEXT) {
   if (ptSeg->X1 >= XOpen) {
    ptSeg->X1 += ChannelWidth;
    ptSeg->X2 += ChannelWidth;
   }
  }

  /* MISE A JOUR DES CONNECTEURS EXTERNES */
  for (ptCon = ptFigure->CON; ptCon; ptCon = ptCon->NEXT) {
   if (ptCon->ORIENT == EAST) ptCon->X += ChannelWidth;
   else if ((ptCon->ORIENT == NORTH) && (ptCon->X >= XOpen))
         ptCon->X += ChannelWidth;
        else if ((ptCon->ORIENT == SOUTH) && (ptCon->X >= XOpen))
         ptCon->X += ChannelWidth;
  }
  
  /* MISE A JOUR DES SEGMENTS DE LA FIGURE MERE */
  for (ptLine = ptFigure->LINE; ptLine; ptLine = ptLine->NEXT) {
   if (ptLine->TYPE == CELL) {
    for (ptSeg = ptLine->ALLOWUSED; ptSeg; ptSeg = ptSeg->NEXT) {
     if (ptSeg->X1 >= XOpen) {
      ptSeg->X1 += ChannelWidth;
      ptSeg->X2 += ChannelWidth;
     }
    }
   }
  }
  for (ptXBreakList = ptXBreak->NEXT; ptXBreakList; 
       ptXBreakList = ptXBreakList->NEXT) {
   ptXBreakList->X += ChannelWidth;
  }
 }
}
 
/******************************************************************************/
/*      Function : CompleteLine()                                             */
/******************************************************************************/
void		CompleteLine(ptFig,NewphIns,Counter)
Figure		*ptFig;
phfig_list	*NewphIns;
long		*Counter;

{
 Line		*CurrentLine = NULL;
 Instance	*CurrentInst = NULL;
 Instance	*NextInst = NULL;
 Instance	*PrevInst = NULL;
 long		NewInstWidth = 0;
 long		NewInstHeight = 0;
 Segment	*ptNewAllow = NULL;
 long		Xab1Figure = ptFig->LINE->X;
 long		Xab2Figure = ptFig->LINE->WIDTH + Xab1Figure;

 NewInstWidth = ((NewphIns->XAB2) - (NewphIns->XAB1));
 NewInstHeight = ((NewphIns->YAB2) - (NewphIns->YAB1));
 for (CurrentLine = ptFig->LINE; CurrentLine; CurrentLine = CurrentLine->NEXT) {
  if (CurrentLine->TYPE == CELL) {
   CurrentInst = CurrentLine->INS;
   if (Xab1Figure + NewInstWidth <= CurrentInst->X) {
    CurrentInst =  CreateInstance(ScrNameIndex("row",(*Counter)++),
                                  NewphIns->NAME,Xab1Figure,CurrentLine->Y,
                                  NewInstWidth,NewInstHeight,NOSYM);
    CurrentInst->NEXT = CurrentLine->INS;
    CurrentLine->INS = CurrentInst;
    ptNewAllow = InsertAllowInIns(CurrentInst,NewphIns->XAB1,NewphIns->XAB2,
                                  NewphIns->YAB1,NewphIns->YAB2,NewphIns);
    CurrentLine->ALLOW = addchain(CurrentLine->ALLOW,(char *)ptNewAllow);
   }
   while (CurrentInst->NEXT) {
    if ((CurrentInst->X + CurrentInst->WIDTH) + NewInstWidth <=
        (NextInst = CurrentInst->NEXT)->X) {
     PrevInst = CurrentInst;
     CurrentInst = CreateInstance(ScrNameIndex("row",(*Counter)++),
                                  NewphIns->NAME,
                                  CurrentInst->X + CurrentInst->WIDTH,
                                  CurrentLine->Y,
                                  NewInstWidth,NewInstHeight,NOSYM);
     CurrentInst->NEXT = NextInst;
     PrevInst->NEXT = CurrentInst;
     ptNewAllow = InsertAllowInIns(CurrentInst,NewphIns->XAB1,NewphIns->XAB2,
                                  NewphIns->YAB1,NewphIns->YAB2,NewphIns);
     CurrentLine->ALLOW = addchain(CurrentLine->ALLOW,(char *)ptNewAllow);
    }
    else  CurrentInst = CurrentInst->NEXT;
   }
   while ((CurrentInst->X + CurrentInst->WIDTH) + NewInstWidth <= Xab2Figure) {
    CurrentInst->NEXT =  CreateInstance(ScrNameIndex("row",(*Counter)++),
                                        NewphIns->NAME,
                                        CurrentInst->X + CurrentInst->WIDTH,
                                        CurrentLine->Y,NewInstWidth,
                                        NewInstHeight,NOSYM);
    CurrentInst = CurrentInst->NEXT;
    ptNewAllow = InsertAllowInIns(CurrentInst,NewphIns->XAB1,NewphIns->XAB2,
                                  NewphIns->YAB1,NewphIns->YAB2,NewphIns);
    CurrentLine->ALLOW = addchain(CurrentLine->ALLOW,(char *)ptNewAllow);
   }
  }
 }
}

/******************************************************************************/
/* Function : UpDateWidthLine()                                               */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*	Input parameters :                                                    */
/*      Input global Variables :                                              */
/*	Output parameters :                                                   */
/*      Output global Variables :                                             */
/*                                                                            */
/******************************************************************************/
void		UpDateWidthLine(ptFig,NewphIns,Counter)
Figure		*ptFig;
phfig_list	*NewphIns;
long		*Counter;

{
 Line		*CurrentLine = NULL;
 Instance	*CurrentInst = NULL;
 Segment	*ptNewAllow  = NULL;
 SegmentList	*ptSegment  = NULL;
 Channel	*ptChannel  = NULL;
 Connector	*ptScrCon = NULL;
 long		NewWidth     = 0;
 long		LineWidth    = 0;
 long           NewInstWidth = 0;
 long           NewInstHeight = 0;
 long           NextXInst = 0;
 long           Xab1 = 0;

 NewInstWidth  = ((NewphIns->XAB2) - (NewphIns->XAB1));
 NewInstHeight = ((NewphIns->YAB2) - (NewphIns->YAB1));

 for (CurrentLine = ptFig->LINE; CurrentLine; CurrentLine = CurrentLine->NEXT) {
  if (CurrentLine->TYPE == PATH) {
   NewWidth = MAX(NewWidth,CurrentLine->WIDTH);
  }
 }
 NewWidth += (Xab1 = (CurrentLine = ptFig->LINE)->X);
 for (; CurrentLine; CurrentLine = CurrentLine->NEXT) {
  if ((LineWidth = CurrentLine->WIDTH) != NewWidth) {
   CurrentLine->WIDTH = NewWidth - Xab1;
   if (CurrentLine->TYPE == CELL) {
    for (CurrentInst = CurrentLine->INS;CurrentInst->NEXT;
         CurrentInst = CurrentInst->NEXT);
    while ((NextXInst = CurrentInst->X + CurrentInst->WIDTH) != NewWidth) {
     CurrentInst->NEXT = CreateInstance(ScrNameIndex("feed",(*Counter)++),
                                        NewphIns->NAME,NextXInst,
                                        CurrentLine->Y,NewInstWidth,
                                        NewInstHeight,NOSYM);
     CurrentInst = CurrentInst->NEXT;
     ptNewAllow = InsertAllowInIns(CurrentInst,NewphIns->XAB1,NewphIns->XAB2,
                                   NewphIns->YAB1,NewphIns->YAB2,NewphIns);
     CurrentLine->ALLOW = addchain(CurrentLine->ALLOW,(char *)ptNewAllow);
    }
   }
   else {
    if (CurrentLine->TYPE == PATH) {
     ptChannel = CurrentLine->CHANNEL;
/*
     ptChannel->WIDTH = NewWidth / PITCH_X;
*/
     for (ptSegment = ptChannel->H_SEGMENT; ptSegment;
          ptSegment = ptSegment->NextSeg) {
      if (ptSegment->X2Seg == (ptChannel->WIDTH + 1)) {
       ptSegment->X2Seg += ((NewWidth - LineWidth + Xab1) / PITCH_X);
      }
     }
    }
   }
  }
 }
 for (ptScrCon = ptFig->CON; ptScrCon; ptScrCon = ptScrCon->NEXT) 
  if (ptScrCon->ORIENT == EAST) ptScrCon->X = NewWidth;

}
/******************************************************************************/
/* Function : Scr_GetSigName()          4 juin 1993                           */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*      Input parameters :                                                    */
/*      Input global Variables :                                              */
/*      Output parameters :                                                   */
/*      Output global Variables :                                             */
/*                                                                            */
/******************************************************************************/
char	*ScrGetSigName(ptSig, SigIndex)
Signal	*ptSig;
long	SigIndex;

{
 for (;ptSig; ptSig = ptSig->NEXT) {
  if (ptSig->INDEX == SigIndex) {
   return(ptSig->NAME);
  }
  if (ptSig == NULL) return NULL;
 }
} 
/******************************************************************************/
/*                              Placer2Scr                                    */
/* ecriture du placement dans mbk physique                                    */
/*     Modified by : Xavier Picat                       Date : 09/07/1993     */
/******************************************************************************/
phfig_list	*Placer2Scr(Nom,FigurePlacee)
char		*Nom;
placement_fig	*FigurePlacee;

{
	phfig_list   *ptphfig;
	int           iCell, iRow;
	int           Width;
	int           Height; 
	row_elt      *Row;
	cell_list    *Cell;
	long          step, percent, i;
	long          NombreLignes, NombreCellules, Hauteur;

	fprintf (stderr,"Saving placement ");
	/******** recuperation de la hauteur d'UNE cellule ***************/
	Row  = FigurePlacee->Rows;
	ptphfig = getphfig (Row[0].head->ins->FIGNAME,'A');
    sx2sc (ptphfig);
	Hauteur = (ptphfig->YAB2 - ptphfig->YAB1) / SCALE_X;

	/******************  chargement du pointeur figure ********************/
	ptphfig = addphfig(Nom);
	NombreCellules = FigurePlacee->NbCells;
	NombreLignes   = FigurePlacee->NbRows;

	/**************** calcul de la taille de la figure ********************/
	Width = 0;
	for (iRow=0;iRow<NombreLignes;iRow++)		/* bande la plus large */
		if (Row[iRow].length>Width) 
			Width=Row[iRow].length;	
	Height = NombreLignes*Hauteur;
	ptphfig->XAB1 = 0;
	ptphfig->YAB1 = 0; 
	ptphfig->XAB2 = Width*SCALE_X;
	ptphfig->YAB2 = Height*SCALE_X; 

	step = NombreCellules/100;
	percent = 0;
	i = step - 1;
	/********************* placement des cellules *************************/
	Cell = FigurePlacee->Cells;
	for (iCell=0;iCell < NombreCellules;iCell++)
	{
		i++;
		if (i == step)
		{
			i = 0;
			fprintf (stderr,"%3d%%\b\b\b\b", percent);
			fflush (stderr);
			percent++;
		}
		addphins (ptphfig,
		          Cell->ins->FIGNAME,
		          Cell->ins->INSNAME,
		          NOSYM,
		          (Cell->x-(Cell->width>>0 /* 99/9/21 1 */))*SCALE_X,
		          Cell->row*Hauteur*SCALE_X
		         );
		Cell++;
	}
	fprintf (stderr,"100%\n");
	mbkfree (FigurePlacee->Rows);
	mbkfree (FigurePlacee->Cells);
	return(ptphfig);
} /* fin de Placer2Scr */

/******************************************************************************/
/* Function : Scr2Mbk                                                         */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*	Input parameters :                                                    */
/*      Input global Variables :                                              */
/*	Output parameters :                                                   */
/*      Output global Variables :                                             */
/*                                                                            */
/******************************************************************************/
void			Scr2Mbk(ptfig,FigName,ptOptionList,
                                ptXInsert,VerticalChannelWidth)
Figure			*ptfig;
char			*FigName;
OptionList		*ptOptionList;
XSupplyRecallList	*ptXInsert;
long			VerticalChannelWidth;

{
 phfig_list	*ptMbkRoot = NULL;
 Channel        *ptScrChannel = NULL;
 Line           *ptScrLine    = NULL;
 Instance       *ptScrIns     = NULL;
 Connector      *ptScrCon     = NULL;
 Segment        *ptScrSeg     = NULL;
 chain_list     *ConList      = NULL;

 XSupplyRecallList	*ptXBreak = NULL;
 ConnectorList  *ptConList      = NULL;
 long           NewYNextLine    = 0;
 SegmentList    *CurrentSeg = NULL;
 ViasList       *CurrentVia = NULL;
 long           X1Symbolic  = 0;
 long           X2Symbolic  = 0;
 long           Y1Symbolic  = 0;
 long           Y2Symbolic  = 0;
 long           X1Segment   = 0;
 long           X2Segment   = 0;
 long           Y1Segment   = 0;
 long           Y2Segment   = 0;
 long           Y1 = 0;
 long           Y2 = 0;
 long           X1 = 0;
 long           X2 = 0;
 long           X3 = 0;
 long           X4 = 0;
 long           Xab1 = 0;
 long           Xab2 = 0;
 long           Yab1 = 0;
 long           Yab2 = 0;
 long           YLine = 0;
 long           VddWidth = 0;
 long           VssWidth = 0;
 long           ChannelWidth = 0;
 long           ChannelHeight = 0;
 long           CellLineNumber = 0;
 char           *NameSeg;
 char           Buffer[10];
 long           IndexSignal = 0;
 long           ChannelNumber = 0;
 static long    Counter = 0;

 for (ptScrLine = ptfig->LINE; ptScrLine; ptScrLine = ptScrLine->NEXT)
 {
  if (ptScrLine->TYPE == PATH)
   ChannelNumber ++;
 }
 ptScrLine = ptfig->LINE;
 NewYNextLine = ptScrLine->HEIGTH;
 LOGld("premier canal",ptScrLine->HEIGTH);
 for (CellLineNumber = 0; ptScrLine; ptScrLine = ptScrLine->NEXT) {
  if (ptScrLine->TYPE == PATH) {
   ChannelWidth = ((ptScrLine->WIDTH - WESTOFFSET - EASTOFFSET) / PITCH_X) + 1;
   ChannelHeight = ((ptScrLine->HEIGTH - SOUTHOFFSET - NORTHOFFSET) / 
                    PITCH_Y) + 1;
   if (L3MODE) /* pour que le canal ait au moins la taille d'une cellule */
   {
    LOGld("",ptScrLine->HEIGTH);
    if ((CellLineNumber == 0) || (CellLineNumber == (ChannelNumber-1)))
    {
     LOGld("numero canal",CellLineNumber);
     LOGld("hauteur",ptScrLine->HEIGTH/SCALE_X);
     if ((ptScrLine->HEIGTH) < (CELLAB/2)) 
      ptScrLine->HEIGTH = CELLAB/2;
    }
    else
    {
     if ((ptScrLine->HEIGTH) < CELLAB) 
      ptScrLine->HEIGTH = CELLAB;
    }
   }
   ptMbkRoot = addphfig(ptScrLine->NAME);
   ptScrChannel = ptScrLine->CHANNEL;
   if (L3MODE && (CellLineNumber != 0))
   {
    NewYNextLine -= CELLAB;
    ptScrLine->Y += NewYNextLine;
   }
   else
    ptScrLine->Y += NewYNextLine;
   LOGld("hau",ptScrLine->Y);
   YLine = ptScrLine->Y;
   Xab1 = ptMbkRoot->XAB1 = ptScrLine->X; 
   Xab2 = ptMbkRoot->XAB2 = (Xab1 + (ptScrLine->WIDTH));
   Yab1 = ptMbkRoot->YAB1 = YLine;
   Yab2 = ptMbkRoot->YAB2 = (Yab1 + (ptScrLine->HEIGTH));

/******************************************************************************/
/*              SAVING HORIZONTAL SEGMENT                                     */
/******************************************************************************/
   for (CurrentSeg = ptScrChannel->H_SEGMENT; CurrentSeg;
        CurrentSeg = CurrentSeg->NextSeg) {
    if ((X1Symbolic = CurrentSeg->X1Seg) == 0)
     X1Segment = Xab1;
    else
     X1Segment = ((X1Symbolic - 1) * PITCH_X) + WESTOFFSET + Xab1;
    if ((X2Symbolic = CurrentSeg->X2Seg) == (ChannelWidth + 1))
     X2Segment = Xab2;
    else
     X2Segment = ((X2Symbolic - 1) * PITCH_X) + WESTOFFSET + Xab1;	
    Y1Segment = Y2Segment = (((CurrentSeg->Y1Seg - 1) * PITCH_Y) + SOUTHOFFSET)
                            + YLine;
    if (CurrentSeg->SegName != namealloc("HOR_FEED")) {
     strcpy(Buffer,CurrentSeg->SegName);
     strtok(Buffer,TRICE);
     IndexSignal = atol(strtok(NULL,TRICE));
     if ((NameSeg = ScrGetSigName(ptfig->SIG, IndexSignal)) == NULL)
      NameSeg = CurrentSeg->SegName;
     ptMbkRoot->PHSEG = addphseg(ptMbkRoot, ((char )(L3MODE?ALU3:ALU1)), LAYER1WIDTH, 
                                X1Segment, Y1Segment, X2Segment , Y2Segment, 
                                NameSeg); 
    }
    else ptMbkRoot->PHSEG = addphseg(ptMbkRoot, ((char )(L3MODE?TALU3:TALU1)), LAYER1WIDTH,
                                     X1Segment, Y1Segment, X2Segment , Y2Segment,
                                     NULL);
   }

/******************************************************************************/
/*              SAVING VERTICAL SEGMENT                                       */
/******************************************************************************/
   for (CurrentSeg = ptScrChannel->V_SEGMENT; CurrentSeg;
        CurrentSeg = CurrentSeg->NextSeg) {
    if ((Y1Symbolic = CurrentSeg->Y1Seg) == 0)
     Y1Segment = Yab1 - ((SXMODE && !L3MODE) ? CELLAB*40/50:0);
    else
     Y1Segment = ((Y1Symbolic - 1) * PITCH_Y) + SOUTHOFFSET + YLine;
    if ((Y2Symbolic = CurrentSeg->Y2Seg) == ChannelHeight + 1)
     Y2Segment = Yab2 + ((SXMODE && !L3MODE)? CELLAB*40/50:0);
    else
     Y2Segment = ((Y2Symbolic - 1) * PITCH_Y) + SOUTHOFFSET + YLine;
    X1Segment = X2Segment = ((CurrentSeg->X1Seg - 1) * PITCH_X) + WESTOFFSET + Xab1;
    strcpy(Buffer,CurrentSeg->SegName);
    strtok(Buffer,TRICE);
    IndexSignal = atol(strtok(NULL,TRICE));
    if ((NameSeg = ScrGetSigName(ptfig->SIG, IndexSignal)) == NULL)
     NameSeg = CurrentSeg->SegName;
    ptMbkRoot->PHSEG = addphseg(ptMbkRoot, ((char )(L3MODE?ALU2:ALU2)), LAYER2WIDTH, 
                                X1Segment, Y1Segment, X2Segment, Y2Segment, 
                                NameSeg);
   }

/******************************************************************************/
/*              SAVING VIA                                                    */
/******************************************************************************/
   for (CurrentVia = ptScrChannel->VIA; CurrentVia;
        CurrentVia = CurrentVia->NextVia) {
    ptMbkRoot->PHVIA = addphvia(ptMbkRoot, ((char ) (L3MODE?CONT_VIA2:CONT_VIA)), 
                                ((CurrentVia->XVia - 1) * PITCH_X) + WESTOFFSET
                                + Xab1,
                                ((CurrentVia->YVia - 1) * PITCH_Y) + 
                                SOUTHOFFSET + YLine,0,0,NULL);
   }

/******************************************************************************/
/*              SAVING CHANNEL CONNECTOR IF FIGURE MUST BE FLATTENED          */
/******************************************************************************/
/* A REVOIR


 if (ptOptionList->ChannelName) {
  for (CurrentCon = ptScrChannel->NORTH_LIST; CurrentCon; 
       CurrentCon = CurrentCon->NextCon) {
   if ((ConName = CurrentCon->ConName) != NONET) {
    sprintf(Buffer,"%ld",CurrentCon->ConName);
    addphcon(ptMbkRoot, NORTH, Buffer, 
             (((CurrentCon->Mark) * PITCH_X) - WESTOFFSET + Xab1),
             Yab2, ((char ) ALU2), LAYER2WIDTH);
   }
  }
  for (CurrentCon = ptScrChannel->SOUTH_LIST; CurrentCon; 
       CurrentCon = CurrentCon->NextCon) {
   if ((ConName = CurrentCon->ConName) != NONET) {
    sprintf(Buffer,"%ld",CurrentCon->ConName);
    addphcon(ptMbkRoot, SOUTH, Buffer, 
             (((CurrentCon->Mark) * PITCH_X) - WESTOFFSET + Xab1),
             Yab1, ((char ) ALU2), LAYER2WIDTH);
   }
  }
  for (CurrentCon = ptScrChannel->WEST_LIST; 
       CurrentCon; CurrentCon = CurrentCon->NextCon) {
   sprintf(Buffer,"%ld",CurrentCon->ConName);
   addphcon(ptMbkRoot, WEST, Buffer, Xab1,
            ((((CurrentCon->Mark) - 1) * PITCH_Y) + SOUTHOFFSET),
            ((char ) ALU1), LAYER1WIDTH);
  }
  for (CurrentCon = ptScrChannel->EAST_LIST; CurrentCon; 
       CurrentCon = CurrentCon->NextCon) {
   sprintf(Buffer,"%ld",CurrentCon->ConName);
   addphcon(ptMbkRoot, EAST, Buffer, Xab2,
            ((((CurrentCon->Mark) - 1) * PITCH_Y) + SOUTHOFFSET),
            ((char ) ALU1), LAYER1WIDTH);
  }
 }
*/

/******************************************************************************/
/*              UP DATE THE X FATHER CONNECTOR                                */
/******************************************************************************/
    if (ptScrChannel->WEST_LIST)
     for (ptConList = ptScrChannel->WEST_LIST; ptConList;
          ptConList = ptConList->NextCon) {
      for (ptScrCon = ptfig->CON; ptScrCon; ptScrCon = ptScrCon->NEXT) {
       if ((ptScrCon->ORIENT == WEST) &&
           (ptScrCon->SIG->INDEX == ptConList->ConName)) {
        ptScrCon->Y = (((ptConList->Mark - 1) * PITCH_Y) + SOUTHOFFSET + YLine);
        break;
       }
      }
     }
 
    if (ptScrChannel->EAST_LIST)
     for (ptConList = ptScrChannel->EAST_LIST; ptConList;
          ptConList = ptConList->NextCon) {
      for (ptScrCon = ptfig->CON; ptScrCon; ptScrCon = ptScrCon->NEXT) {
       if ((ptScrCon->ORIENT == EAST) &&
           (ptScrCon->SIG->INDEX == ptConList->ConName)) {
        ptScrCon->Y = (((ptConList->Mark - 1) * PITCH_Y) + SOUTHOFFSET + YLine);
        break;
       }
      }
     }

    if (ptOptionList->ChannelName) savephfig(ptMbkRoot);
    LOGld("CANAL",ptScrLine->HEIGTH);
    NewYNextLine += ptScrLine->HEIGTH;
    LOGld("TOTAL",NewYNextLine);
   }
   else if (ptScrLine->TYPE == CELL) {
    CellLineNumber++;
    ptMbkRoot = addphfig(ptScrLine->NAME);
    ptScrLine->Y += (NewYNextLine - (L3MODE ? CELLAB/2 : 0));
    ptMbkRoot->XAB1 = ptScrLine->X; 
    ptMbkRoot->YAB1 = ptScrLine->Y;
    ptMbkRoot->XAB2 = ((ptScrLine->X) + (ptScrLine->WIDTH));
    ptMbkRoot->YAB2 = ((ptScrLine->Y) + (ptScrLine->HEIGTH));
 
    for (ptScrIns = ptScrLine->INS; ptScrIns; ptScrIns = ptScrIns->NEXT) {
     ptMbkRoot->PHINS = addphins(ptMbkRoot,ptScrIns->FIGNAME,ptScrIns->NAME,
                                 ptScrIns->TRANSF,ptScrIns->X,ptScrLine->Y);
    }
    if (!L3MODE)
    {
     for (ConList = ptScrLine->CON; ConList; ConList = ConList->NEXT) {
      ptScrCon = ((Connector *) ConList->DATA);
      if (ptScrCon->ORIENT == NORTH) 
       ptMbkRoot->PHCON = addphcon(ptMbkRoot,ptScrCon->ORIENT,ptScrCon->NAME,
                                   ptScrCon->X,ptMbkRoot->YAB2,
                                   ptScrCon->LAYER,ptScrCon->WIDTH);
      else 
       if (ptScrCon->ORIENT == SOUTH) 
        ptMbkRoot->PHCON = addphcon(ptMbkRoot,ptScrCon->ORIENT,ptScrCon->NAME,
                                    ptScrCon->X,ptMbkRoot->YAB1,
                                    ptScrCon->LAYER,ptScrCon->WIDTH);
     } 
    }
    if (SXMODE) 
    {
     phfig_list * phfig;
     phref_list * phref;
     phseg_list * phseg;
     phins_list * phins;
     
     for (phins = ptMbkRoot->PHINS; phins; phins = phins->NEXT)
     {
      phfig = getphfig (phins->FIGNAME, 'A');
      LOGs("",phins->FIGNAME);
      for (phref = phfig->PHREF; phref; phref = phref->NEXT)
      {
       if (phref->NAME && strstr (phref->NAME, "_25"))
       {
        long xvia, yvia;
        LOGld("tra",phins->TRANSF);
        LOGs("name",phref->NAME);
        LOGld("",phref->XREF);
        LOGld("",phref->YREF);
        LOGld("",phins->XINS);
        LOGld("",phins->YINS);
        LOGld("",ptMbkRoot->XAB1);
        LOGld("",ptMbkRoot->YAB1);
        LOGld("",ptMbkRoot->XAB2);
        LOGld("",ptMbkRoot->YAB2);
        xyflat (&xvia, &yvia, phref->XREF, phref->YREF, phins->XINS, phins->YINS,
                ptMbkRoot->XAB1, ptMbkRoot->YAB1, ptMbkRoot->XAB2, ptMbkRoot->YAB2,
                phins->TRANSF);
        LOGld("",xvia);
        LOGld("",yvia);
        addphvia (ptMbkRoot, ((char ) CONT_VIA), xvia, yvia+ptMbkRoot->YAB1,0,0,NULL);
        /*
        if (L3MODE)
          addphvia (ptMbkRoot, ((char ) CONT_VIA2), xvia, yvia+ptMbkRoot->YAB1,0,0,NULL);
        */
       }
      }
     }
    } 
    if (SXMODE==0)
    {
     for(ptScrSeg = ptScrLine->ALLOWUSED; ptScrSeg; ptScrSeg = ptScrSeg->NEXT) {
      ptMbkRoot->PHSEG = addphseg(ptMbkRoot,ptScrSeg->LAYER,ptScrSeg->WIDTH,
                                  ptScrSeg->X1,ptScrSeg->Y1 + ptScrLine->Y,
                                  ptScrSeg->X2,ptScrSeg->Y2 + ptScrLine->Y,
                                  ptScrSeg->NAME);
     }
    }
#ifdef L3DEBUG
    savephfig(ptMbkRoot);
#endif    
    if (ptOptionList->ChannelName && !SXMODE) savephfig(ptMbkRoot);
   }
   else if (ptScrLine->TYPE == UP) ptScrLine->Y = ptScrLine->Y + NewYNextLine;
   
  }

 VddWidth = (CellLineNumber * VDDWIDTH) / 2;
 VssWidth = (CellLineNumber * VSSWIDTH) / 2;

 ptMbkRoot = addphfig(FigName);

 ptMbkRoot->XAB1 = (ptScrLine = ptfig->LINE)->X; 
 ptMbkRoot->YAB1 = ptScrLine->Y;
for (; ptScrLine->NEXT; ptScrLine = ptScrLine->NEXT);
 ptMbkRoot->XAB2 = ptScrLine->X + ptScrLine->WIDTH; 
 ptMbkRoot->YAB2 = ptScrLine->Y + ptScrLine->HEIGTH;

 for (CellLineNumber = 0,ptScrLine = ptfig->LINE->NEXT; 
      ptScrLine->NEXT; 
      CellLineNumber++, ptScrLine = ptScrLine->NEXT) {
  if (L3MODE && ((CellLineNumber%4)==3))
  { 
   ptMbkRoot->PHINS = addphins(ptMbkRoot,ptScrLine->NAME,ptScrLine->NAME,
                               SYM_Y,
                               ptScrLine->X,ptScrLine->Y);
  }                             
  else 
   ptMbkRoot->PHINS = addphins(ptMbkRoot,ptScrLine->NAME,ptScrLine->NAME,
                               NOSYM,
                               ptScrLine->X,ptScrLine->Y);
 }

/******************************************************************************/
/*              SAVING FATHER CONNECTOR                                       */
/******************************************************************************/

 for (ptScrCon = ptfig->CON; ptScrCon; ptScrCon = ptScrCon->NEXT) {
  if ((ptScrCon->ORIENT == WEST) || (ptScrCon->ORIENT == EAST))
  {
   ptMbkRoot->PHCON = addphcon(ptMbkRoot,ptScrCon->ORIENT,ptScrCon->NAME,
                               ptScrCon->X,ptScrCon->Y,
                               (L3MODE?ALU3:ptScrCon->LAYER),ptScrCon->WIDTH);
  }                             
  else if (ptScrCon->ORIENT == NORTH)
  {      
   ptMbkRoot->PHCON = addphcon(ptMbkRoot,ptScrCon->ORIENT,ptScrCon->NAME,
                               ptScrCon->X,ptMbkRoot->YAB2,(L3MODE?ALU2:ptScrCon->LAYER),
                               ptScrCon->WIDTH);
  }                                  
  else if (ptScrCon->ORIENT == SOUTH)
  {           
   ptMbkRoot->PHCON = addphcon(ptMbkRoot,ptScrCon->ORIENT,
                               ptScrCon->NAME,ptScrCon->X,
                               ptMbkRoot->YAB1,(L3MODE?ALU2:ptScrCon->LAYER),
                               ptScrCon->WIDTH);
  } 
 }
 /* connecteurs alimentations */
 for (ptScrLine = ptfig->LINE; ptScrLine; ptScrLine = ptScrLine->NEXT) {
  if (ptScrLine->TYPE == CELL) {
   if (ptOptionList->SupplyRecall == 0) {
    ptMbkRoot->PHCON = addphcon(ptMbkRoot,WEST,
                                ScrNameIndex(NameVdd,ptScrLine->INDEX),
                                ptMbkRoot->XAB1,
                                (ptScrLine->Y + ptScrLine->HEIGTH - VDDOFFSET),
                                ALU1,VDDWIDTH);
    ptMbkRoot->PHCON = addphcon(ptMbkRoot,WEST,
                                ScrNameIndex(NameVss,ptScrLine->INDEX),
                                ptMbkRoot->XAB1,
                                (ptScrLine->Y + VSSOFFSET),
                                ALU1,VSSWIDTH);
    ptMbkRoot->PHCON = addphcon(ptMbkRoot,EAST,
                                ScrNameIndex(NameVdd,ptScrLine->INDEX),
                                ptMbkRoot->XAB2,
                                (ptScrLine->Y + ptScrLine->HEIGTH - VDDOFFSET),
                                ALU1,VDDWIDTH);
    ptMbkRoot->PHCON = addphcon(ptMbkRoot,EAST,
                                ScrNameIndex(NameVss,ptScrLine->INDEX),
                                ptMbkRoot->XAB2,
                                (ptScrLine->Y + VSSOFFSET),
                                ALU1,VSSWIDTH);
   }
   else {
    ptMbkRoot->PHCON = addphcon(ptMbkRoot,WEST, NameVdd, ptMbkRoot->XAB1,
                                (ptScrLine->Y + ptScrLine->HEIGTH - VDDOFFSET),
                                ALU1,VDDWIDTH);
    ptMbkRoot->PHCON = addphcon(ptMbkRoot,WEST, NameVss, ptMbkRoot->XAB1,
                                (ptScrLine->Y + VSSOFFSET),
                                ALU1,VSSWIDTH);
    ptMbkRoot->PHCON = addphcon(ptMbkRoot,EAST, NameVdd, ptMbkRoot->XAB2,
                               (ptScrLine->Y + ptScrLine->HEIGTH - VDDOFFSET),
                                ALU1,VDDWIDTH);
    ptMbkRoot->PHCON = addphcon(ptMbkRoot,EAST, NameVss, ptMbkRoot->XAB2,
                                (ptScrLine->Y + VSSOFFSET),
                                ALU1,VSSWIDTH);
 /* BARRES D'ALIMENTATIONS HORIZONTALES */
    for (ptXBreak = ptXInsert; ptXBreak; ptXBreak = ptXBreak->NEXT) {
     long xins;
     phfig_list *feed1, *feed2;
     long feed1_width, feed2_width;
     X1 = ptXBreak->X;
     X2 = X1 + VerticalChannelWidth;
     X3 = X1 + RIGHTSUPPLY + (VddWidth / 2);
     X4 = X2 - LEFTSUPPLY - (VssWidth / 2);
     Y1 = ptScrLine->Y + VSSOFFSET;
     Y2 = ptScrLine->Y + ptScrLine->HEIGTH - VDDOFFSET;
     ptMbkRoot->PHSEG = addphseg(ptMbkRoot,ALU1,VSSWIDTH,X1,Y1,X2,Y1,NameVss);
     ptMbkRoot->PHSEG = addphseg(ptMbkRoot,ALU1,VDDWIDTH,X1,Y2,X2,Y2,NameVdd);
     bigvia(ptMbkRoot, CONT_VIA,X3, Y2, VddWidth, VDDWIDTH);
     bigvia(ptMbkRoot, CONT_VIA,X4, Y1, VssWidth, VSSWIDTH);
     feed1 = getphfig("tie_x0",'A');
     feed2 = getphfig("rowend_x0",'A');
     feed1_width=feed1->XAB2-feed1->XAB1;
     feed2_width=feed2->XAB2-feed2->XAB1;
     for(xins=X1; xins+feed1_width*SCALE_X <= X2; xins += feed1_width*SCALE_X) {
        addphins(ptMbkRoot,"tie_x0",ScrNameIndex("tiefeed",Counter++),
                               NOSYM,
                               xins,ptScrLine->Y);
     }
     for(; xins+feed2_width*SCALE_X <= X2; xins += feed2_width*SCALE_X) {
        addphins(ptMbkRoot,"rowend_x0",ScrNameIndex("tiefeed",Counter++),
                               NOSYM,
                               xins,ptScrLine->Y);
     }
     /*
     if (L3MODE)
     {
      bigvia(ptMbkRoot, CONT_VIA2,X3, Y2, VddWidth, VDDWIDTH);
      bigvia(ptMbkRoot, CONT_VIA2,X4, Y1, VssWidth, VSSWIDTH);
     }
     */
    }
   }
  }
 }
 /* BARRES D'ALIMENTATIONS VERTICALES */
 Yab1 = ptMbkRoot->YAB1;
 Yab2 = ptMbkRoot->YAB2;
 if (ptOptionList->SupplyRecall != 0) {
  for (ptXBreak = ptXInsert; ptXBreak; ptXBreak = ptXBreak->NEXT) {
   X1 = ptXBreak->X;
   X2 = X1 + VerticalChannelWidth;
   X3 = X1 + RIGHTSUPPLY + (VddWidth / 2);
   X4 = X2 - LEFTSUPPLY - (VssWidth / 2);

   ptMbkRoot->PHSEG = addphseg(ptMbkRoot,(L3MODE?ALU2:ALU2),VddWidth,X3,Yab1,X3,Yab2,NameVdd);
   ptMbkRoot->PHSEG = addphseg(ptMbkRoot,(L3MODE?ALU2:ALU2),VssWidth,X4,Yab1,X4,Yab2,NameVss);
   ptMbkRoot->PHCON = addphcon(ptMbkRoot,NORTH, NameVdd, X3,Yab2,(L3MODE?ALU2:ALU2),VddWidth);
   ptMbkRoot->PHCON = addphcon(ptMbkRoot,NORTH, NameVss, X4,Yab2,(L3MODE?ALU2:ALU2),VssWidth);
   ptMbkRoot->PHCON = addphcon(ptMbkRoot,SOUTH, NameVdd, X3,Yab1,(L3MODE?ALU2:ALU2),VddWidth);
   ptMbkRoot->PHCON = addphcon(ptMbkRoot,SOUTH, NameVss, X4,Yab1,(L3MODE?ALU2:ALU2),VssWidth); 
  }
 }

/* TRANSPARENCES DE LA FIGURE */

 for(ptScrSeg = ptfig->ALLOW; ptScrSeg; ptScrSeg = ptScrSeg->NEXT) {
  ptMbkRoot->PHSEG = addphseg(ptMbkRoot,ptScrSeg->LAYER,ptScrSeg->WIDTH,
                              ptScrSeg->X1,Yab1,ptScrSeg->X2,Yab2, 
                              ptScrSeg->NAME);
 }

 if (!(ptOptionList->ChannelName) || SXMODE) rflattenphfig(ptMbkRoot,'N','Y');
 
 /* on rabote les fils vertical trop grand, le flaten est alors obligatoire */
 if (SXMODE && !L3MODE) 
 {
  phseg_list * phseg;
  for (phseg = ptMbkRoot->PHSEG; phseg; phseg = phseg->NEXT)
  {
   if (phseg->Y1 < ptMbkRoot->YAB1)
     phseg->Y1 = ptMbkRoot->YAB1;
   if (phseg->Y2 > ptMbkRoot->YAB2)
     phseg->Y2 = ptMbkRoot->YAB2;
  }
 }
 savephfig(ptMbkRoot);
}

/******************************************************************************/
/* Function : MakeVerticalFeed()                                              */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*      Input parameters :                                                    */
/*      Input global Variables : none                                         */
/*      Output parameters :  none                                             */
/*      Output global Variables : none                                        */
/*                                                                            */
/******************************************************************************/
void			MakeVerticalFeed(ptFig,ptMbkTie,FeedNumber,
                                         SupplyNumber,ptXInsert,Counter)
Figure			*ptFig;
phfig_list		*ptMbkTie;
long			FeedNumber;
int			SupplyNumber;
XSupplyRecallList       *ptXInsert;
long			*Counter;

{	
 XSupplyRecallList	*ptXBreak = NULL;
 XSupplyRecallList	*ptXBreakList = NULL;
 Line           	*ptLine = NULL;
 Instance       	*ptInst = NULL;
 Instance       	*PrevInst = NULL;
 Instance       	*ptNewInst = NULL;
 Connector      	*ptCon = NULL;
 Segment        	*ptSeg = NULL;
 Segment        	*ptNewAllow = NULL;
 phseg_list     	*ptphSeg = NULL;
 SegmentList    	*ptSegList = NULL;
 ViasList       	*ptViaList = NULL;
 long           	ChannelWidth = 0;
 long           	WidthShiftSymb = 0;
 long           	XOpen = 0;
 long           	XOpenSymb = 0;
 long           	TieWidth = (ptMbkTie->XAB2 - ptMbkTie->XAB1);
 long           	TieHeight  = (ptMbkTie->YAB2 - ptMbkTie->YAB1);
 long           	X1   = 0;
 long           	X2   = 0;
 long           	Y1   = 0;
 long           	Y2   = 0;
 long           	FeedToPlace = 0;
 long           	PositionCounter = 0;
 long           	Divide   = 0;
 long           	Reste   = 0;

 ChannelWidth = TieWidth;
 WidthShiftSymb = ChannelWidth / PITCH_X;
 if (FeedNumber <= (long) SupplyNumber) {
  for (ptXBreak = ptXInsert; ptXBreak && (FeedNumber != 0); ptXBreak = ptXBreak->NEXT) {
   FeedNumber--;
   XOpen = ptXBreak->X;
   XOpenSymb = (XOpen / PITCH_X) + 1;
   for (ptLine = ptFig->LINE; ptLine; ptLine = ptLine->NEXT) {
    ptLine->WIDTH += ChannelWidth;
    if (ptLine->TYPE == CELL) {
     for (PrevInst = NULL, ptInst = ptLine->INS; (ptInst && (ptInst->X < XOpen));
          PrevInst = ptInst, ptInst = ptInst->NEXT);
     ptNewInst = CreateInstance(ScrNameIndex("verfeed",(*Counter)++),
                                ptMbkTie->NAME,XOpen,ptLine->Y,
                                TieWidth,TieHeight,NOSYM);
     if (PrevInst) {
      ptNewInst->NEXT = ptInst;
      PrevInst->NEXT = ptNewInst;
     }
     else {
      ptNewInst->NEXT = ptInst;
      ptLine->INS = ptNewInst;
     }
     for (; ptInst; ptInst = ptInst->NEXT) {
      ptInst->X += ChannelWidth;
      for (ptCon = ptInst->CON; ptCon; ptCon = ptCon->NEXT)
       ptCon->X += ChannelWidth;
      for (ptSeg = ptInst->ALLOW; ptSeg; ptSeg = ptSeg->NEXT) {
       ptSeg->X1 += ChannelWidth;
       ptSeg->X2 += ChannelWidth;
      }
     } 
    } 
    else {
     if (ptLine->TYPE == PATH) {
      ptLine->CHANNEL->WIDTH += WidthShiftSymb;
      for (ptSegList = ptLine->CHANNEL->H_SEGMENT; ptSegList;
           ptSegList = ptSegList->NextSeg) {
       if (ptSegList->X1Seg >= XOpenSymb) ptSegList->X1Seg += WidthShiftSymb;
       if (ptSegList->X2Seg >= XOpenSymb) ptSegList->X2Seg += WidthShiftSymb;
      }
      for (ptSegList = ptLine->CHANNEL->V_SEGMENT; ptSegList;
           ptSegList = ptSegList->NextSeg) {
       if (ptSegList->X1Seg >= XOpenSymb) {
        ptSegList->X1Seg += WidthShiftSymb;
        ptSegList->X2Seg += WidthShiftSymb;
       }
      }
      for (ptViaList = ptLine->CHANNEL->VIA; ptViaList;
           ptViaList = ptViaList->NextVia) {
       if (ptViaList->XVia >= XOpenSymb) 
        ptViaList->XVia += WidthShiftSymb;
      }
     }
    }
   }

/* PLACEMENT DES TRANSPARENCES VERTICALES */
   for (ptphSeg = ptMbkTie->PHSEG; ptphSeg; ptphSeg = ptphSeg->NEXT) {
    if(ptphSeg->LAYER == TALU2) {
     xyflat(&X1,&Y1,ptphSeg->X1,ptphSeg->Y1,ptNewInst->X,ptNewInst->Y,
            ptMbkTie->XAB1,ptMbkTie->YAB1,ptMbkTie->XAB2,ptMbkTie->YAB2,ptNewInst->TRANSF);
     xyflat(&X2,&Y2,ptphSeg->X2,ptphSeg->Y2,ptNewInst->X,ptNewInst->Y,
            ptMbkTie->XAB1,ptMbkTie->YAB1,ptMbkTie->XAB2,ptMbkTie->YAB2,ptNewInst->TRANSF);
     ptNewAllow = CreateSegment(ptphSeg->NAME,X1,(Y1 - ptNewInst->Y),X2,
                                (Y2 - ptNewInst->Y),ptphSeg->WIDTH,
                                ptphSeg->TYPE,ptphSeg->LAYER);
     if (ptFig->ALLOW == NULL) ptFig->ALLOW = ptNewAllow;
     else {
      ptNewAllow->NEXT = ptFig->ALLOW;
      ptFig->ALLOW = ptNewAllow;
     }
    }
   }

   /* MISE A JOUR DES CONNECTEURS EXTERNES */
   for (ptCon = ptFig->CON; ptCon; ptCon = ptCon->NEXT) {
    if (ptCon->ORIENT == EAST) ptCon->X += ChannelWidth;
    else if ((ptCon->ORIENT == NORTH) && (ptCon->X >= XOpen))
          ptCon->X += ChannelWidth;
         else if ((ptCon->ORIENT == SOUTH) && (ptCon->X >= XOpen))
           ptCon->X += ChannelWidth;
   }
   
   /* MISE A JOUR DES SEGMENTS DE LA FIGURE MERE */
   for (ptLine = ptFig->LINE; ptLine; ptLine = ptLine->NEXT) {
    if (ptLine->TYPE == CELL) {
     for (ptSeg = ptLine->ALLOWUSED; ptSeg; ptSeg = ptSeg->NEXT) {
      if (ptSeg->X1 >= XOpen) {
       ptSeg->X1 += ChannelWidth;
       ptSeg->X2 += ChannelWidth;
      }
     }
    }
   }
   for (ptXBreakList = ptXBreak->NEXT; ptXBreakList; 
        ptXBreakList = ptXBreakList->NEXT) {
    ptXBreakList->X += ChannelWidth;
   }
  }
 }
/*#################################################*/
 else {
  Divide = FeedNumber / ((long ) SupplyNumber);
  Reste = FeedNumber % ((long ) SupplyNumber);
  for (PositionCounter = 1, ptXBreak = ptXInsert; ptXBreak ; ptXBreak = ptXBreak->NEXT) {
   XOpen = ptXBreak->X;
   XOpenSymb = (XOpen / PITCH_X) + 1;
   if (PositionCounter <= Reste) {
    FeedToPlace = Divide + 1;
    PositionCounter++;
   }
   else FeedToPlace = Divide;
   while (FeedToPlace != 0) {
    for (ptLine = ptFig->LINE; ptLine; ptLine = ptLine->NEXT) {
     ptLine->WIDTH += ChannelWidth;
     if (ptLine->TYPE == CELL) {
      for (PrevInst = NULL, ptInst = ptLine->INS; (ptInst && (ptInst->X < XOpen));
           PrevInst = ptInst, ptInst = ptInst->NEXT);
      ptNewInst = CreateInstance(ScrNameIndex("verfeed",(*Counter)++),
                                 ptMbkTie->NAME,XOpen,ptLine->Y,
                                 TieWidth,TieHeight,NOSYM);
      if (PrevInst) {
       ptNewInst->NEXT = ptInst;
       PrevInst->NEXT = ptNewInst;
      }
      else {
       ptNewInst->NEXT = ptInst;
       ptLine->INS = ptNewInst;
      }
      for (; ptInst; ptInst = ptInst->NEXT) {
       ptInst->X += ChannelWidth;
       for (ptCon = ptInst->CON; ptCon; ptCon = ptCon->NEXT)
        ptCon->X += ChannelWidth;
       for (ptSeg = ptInst->ALLOW; ptSeg; ptSeg = ptSeg->NEXT) {
        ptSeg->X1 += ChannelWidth;
        ptSeg->X2 += ChannelWidth;
       }
      } 
     } 
     else {
      if (ptLine->TYPE == PATH) {
       ptLine->CHANNEL->WIDTH += WidthShiftSymb;
       for (ptSegList = ptLine->CHANNEL->H_SEGMENT; ptSegList;
            ptSegList = ptSegList->NextSeg) {
        if (ptSegList->X1Seg >= XOpenSymb) ptSegList->X1Seg += WidthShiftSymb;
        if (ptSegList->X2Seg >= XOpenSymb) ptSegList->X2Seg += WidthShiftSymb;
       }
       for (ptSegList = ptLine->CHANNEL->V_SEGMENT; ptSegList;
            ptSegList = ptSegList->NextSeg) {
        if (ptSegList->X1Seg >= XOpenSymb) {
         ptSegList->X1Seg += WidthShiftSymb;
         ptSegList->X2Seg += WidthShiftSymb;
        }
       }
       for (ptViaList = ptLine->CHANNEL->VIA; ptViaList;
            ptViaList = ptViaList->NextVia) {
        if (ptViaList->XVia >= XOpenSymb) 
         ptViaList->XVia += WidthShiftSymb;
       }
      }
     }
    }

/* PLACEMENT DES TRANSPARENCES VERTICALES */
    for (ptphSeg = ptMbkTie->PHSEG; ptphSeg; ptphSeg = ptphSeg->NEXT) {
     if(ptphSeg->LAYER == TALU2) {
      xyflat(&X1,&Y1,ptphSeg->X1,ptphSeg->Y1,ptNewInst->X,ptNewInst->Y,
             ptMbkTie->XAB1,ptMbkTie->YAB1,ptMbkTie->XAB2,ptMbkTie->YAB2,ptNewInst->TRANSF);
      xyflat(&X2,&Y2,ptphSeg->X2,ptphSeg->Y2,ptNewInst->X,ptNewInst->Y,
             ptMbkTie->XAB1,ptMbkTie->YAB1,ptMbkTie->XAB2,ptMbkTie->YAB2,ptNewInst->TRANSF);
      ptNewAllow = CreateSegment(ptphSeg->NAME,X1,(Y1 - ptNewInst->Y),X2,
                                 (Y2 - ptNewInst->Y),ptphSeg->WIDTH,
                                 ptphSeg->TYPE,ptphSeg->LAYER);
      if (ptFig->ALLOW == NULL) ptFig->ALLOW = ptNewAllow;
      else {
       ptNewAllow->NEXT = ptFig->ALLOW;
       ptFig->ALLOW = ptNewAllow;
      }
     }
    }

   /* MISE A JOUR DES CONNECTEURS EXTERNES */
    for (ptCon = ptFig->CON; ptCon; ptCon = ptCon->NEXT) {
     if (ptCon->ORIENT == EAST) ptCon->X += ChannelWidth;
     else if ((ptCon->ORIENT == NORTH) && (ptCon->X >= XOpen))
           ptCon->X += ChannelWidth;
          else if ((ptCon->ORIENT == SOUTH) && (ptCon->X >= XOpen))
            ptCon->X += ChannelWidth;
    }
    
   /* MISE A JOUR DES SEGMENTS DE LA FIGURE MERE */
    for (ptLine = ptFig->LINE; ptLine; ptLine = ptLine->NEXT) {
     if (ptLine->TYPE == CELL) {
      for (ptSeg = ptLine->ALLOWUSED; ptSeg; ptSeg = ptSeg->NEXT) {
       if (ptSeg->X1 >= XOpen) {
        ptSeg->X1 += ChannelWidth;
        ptSeg->X2 += ChannelWidth;
       }
      }
     }
    }
    for (ptXBreakList = ptXBreak->NEXT; ptXBreakList; 
         ptXBreakList = ptXBreakList->NEXT) {
     ptXBreakList->X += ChannelWidth;
    }
    XOpen += ChannelWidth;
    XOpenSymb = (XOpen / PITCH_X) + 1;
    FeedToPlace--;
   }


  }
 }
/*#################################################*/

}

/******************************************************************************/
/* Function : MakeHorizFeed()                                                 */
/*                                                                            */
/* Description :                                                              */
/*                                                                            */
/*      Input parameters :                                                    */
/*      Input global Variables : none                                         */
/*      Output parameters :  none                                             */
/*      Output global Variables : none                                        */
/*                                                                            */
/******************************************************************************/
void		MakeHorizFeed(ptFig,Number)
Figure		*ptFig;
long		Number;

{
 Line		*ptLine = NULL;
 long		Counter = 0;
 long		MiddleChannel = 0;
 ConnectorList	*ptCon = NULL;
 SegmentList	*ptSegment = NULL;
 Channel	*ptChannel = NULL;
 long		ChannelHeigth = 0;

 for (ptLine = ptFig->LINE; ptLine; ptLine = ptLine->NEXT) 
  if (ptLine->TYPE == PATH) Counter++;
 if (IMPAIRE(Counter)) MiddleChannel = (Counter/2) + 1;
 else MiddleChannel = (Counter/2);
 Counter = 0;
 for (ptLine = ptFig->LINE; ptLine; ptLine = ptLine->NEXT) {
  if (ptLine->TYPE == PATH) Counter++;
  if (Counter == MiddleChannel) break;
 }
 ptChannel = ptLine->CHANNEL;
 ChannelHeigth = ptChannel->HEIGTH;
 for (ptCon = ptChannel->NORTH_LIST; ptCon; ptCon = ptCon->NextCon) {
  if (ptCon->Mark >= ChannelHeigth) ptCon->Mark += Number;
 }
 for (ptSegment = ptChannel->V_SEGMENT; ptSegment; ptSegment = ptSegment->NextSeg) {
  if (ptSegment->Y2Seg > ChannelHeigth) ptSegment->Y2Seg += Number;
 }
 ptChannel->HEIGTH += Number;
 ptLine->HEIGTH = ((ptChannel->HEIGTH - 1) * PITCH_Y) + SOUTHOFFSET + NORTHOFFSET;
 for (Counter = 1; Counter <= Number; Counter++) {
  ptSegment = (SegmentList *) mbkalloc (sizeof(SegmentList));
  ptSegment->SegName = namealloc("HOR_FEED");
  ptSegment->X1Seg = 0;
  ptSegment->Y1Seg = ChannelHeigth + Counter;
  ptSegment->X2Seg = (ptChannel->WIDTH) + 1;
  ptSegment->Y2Seg = ChannelHeigth + Counter;
  ptSegment->NextSeg = ptChannel->H_SEGMENT;
  ptChannel->H_SEGMENT = ptSegment;
 }
}

/******************************************************************************/
/*	Function : main()                                                     */
/******************************************************************************/
int	main(argc,argv)

int	argc;
char	*argv[];

{
 Figure		*ptfig   = NULL;
 Line		*ptLine  = NULL;
 Channel	*ptChannel = NULL;
 phfig_list	*ptphfig = NULL;
 lofig_list	*ptlofig = NULL;
 phfig_list	*NewphIns = NULL;
 XSupplyRecallList	*ptXInsertList = NULL;
 BOOLEAN        LineWidthChange  = FALSE;
 long		Counter  = 0;
 long		CounterInst  = 0;
 OptionList	*ptOption = NULL;
 long		ChannelWidth  = 0;
 long		CellLineNumber = 0;
 extern FILE	*yyin;

 rdsenv ();
 loadrdsparam();
 mbkenv();
 NameVdd = namealloc(VDD);
 NameVss = namealloc(VSS);
 alliancebanner("SCR","5.3","Standard Cell router","1991",ALLIANCE_VERSION);
 if (argc < 3) ScrUsage(argv[0]);
 else
  if (*argv[1] == '-') ptOption = GetOptions(argc,argv);
  else ScrUsage(argv[0]);

 if (!((ptOption->Placer) || (ptOption->Router)))
  ScrUsage(argv[0]);

 if ((yyin = mbkfopen(ptOption->InputFileName,"scr","r")) != NULL) {
  yyparse();
  fclose(yyin);
 }

 fprintf(stderr,"Loading logical view : %s\n",ptOption->InputFileName);
 ptlofig = getlofig(ptOption->InputFileName,'A');
 rflattenlofig(ptlofig,'Y','Y');

 if (ptlofig->LOTRS) {
  fprintf(stderr,"scr_error : Check that the catalogue file existes and that it contains all the models instanciated in the figure.\n");
  exit(1);
 }

 if (ptOption->Placer) {
  fprintf(stderr,"Placing logical view : %s\n",ptOption->InputFileName);
  ptphfig =
  Placer2Scr(ptlofig->NAME,Place(ptlofig,ptOption->Iteration,ptOption->Row,0,7,5,5,NULL,SCR_RATIO)); 
 }
 else {
  fprintf(stderr,"Loading physical view : %s\n",ptOption->InputFileName);
  ptphfig = getphfig(ptOption->InputFileName,'A');
  rflattenphfig(ptphfig,'Y','Y');
 }
 if (ptOption->Router) {

  fprintf(stderr,"Checking consistency between logical and physical views\n");
  Sesame(ptphfig, ptlofig);

  fprintf(stderr,"Loading SCR data base ...\n");
  ptfig = LoadScrFig(ptphfig, ptlofig,LeadPlaceConList);

# ifdef SCR_DEBUG   
  fprintf(stderr,"In ViewDataBase \n");
  ViewScrFigure(ptfig);
  ViewScrChannel(ptfig);
  fprintf(stderr,"Out ViewDataBase \n");
# endif

  fprintf(stderr,"Deleting MBK data base ...\n");
  dellofig(ptlofig->NAME);
  delphfig(ptphfig->NAME);

# ifdef SCR_DEBUG   
  fprintf(stderr,"Get the transparencies model ...\n");
# endif

  NewphIns = getphfig((SXMODE ? "tie_x0":"tie_y"),'A');
  sx2sc (NewphIns);


# ifdef SCR_DEBUG
  fprintf(stderr,"Complete rows ...\n");
# endif

  CompleteLine(ptfig,NewphIns,&CounterInst);

  NewphIns = getphfig((SXMODE ? "rowend_x0":"tie_y"),'A');
  sx2sc (NewphIns);


# ifdef SCR_DEBUG
  fprintf(stderr,"Complete rows ...\n");
# endif

  CompleteLine(ptfig,NewphIns,&CounterInst);

  NewphIns = getphfig((SXMODE ? "tie_x0":"tie_y"),'A');
  sx2sc (NewphIns);
# ifdef SCR_DEBUG   
  fprintf(stderr,"In ViewDataBase \n");
  ViewScrFigure(ptfig);
  ViewScrChannel(ptfig);
  fprintf(stderr,"Out ViewDataBase \n");
# endif

  if (ptOption->SupplyRecall != 0)
   ptXInsertList = PreparVerticalChannel(ptfig, NewphIns, ptOption->SupplyRecall, &CounterInst);
  else
   if (LeadPlaceConList && (LeadPlaceConList->VER_FEED != 0)) 
    ptXInsertList = PreparVerticalChannel(ptfig, NewphIns, 1, &CounterInst);

  NewphIns = getphfig((SXMODE ? "rowend_x0":"tie_y"),'A');
  sx2sc (NewphIns);

  if (ptOption->SupplyRecall != 0)
   ptXInsertList = PreparVerticalChannel(ptfig, NewphIns, ptOption->SupplyRecall, &CounterInst);
  else
   if (LeadPlaceConList && (LeadPlaceConList->VER_FEED != 0)) 
    ptXInsertList = PreparVerticalChannel(ptfig, NewphIns, 1, &CounterInst);

  fprintf(stderr,"Global routing ...\n");
  GlobalRoute(ptfig,NewphIns,ptXInsertList);

  fprintf(stderr,"Channel routing ...\n");
  for (ptLine = ptfig->LINE; ptLine; ptLine = ptLine->NEXT) {
   if (ptLine->TYPE == PATH) {
    CellLineNumber++;
    Counter++;
    if (ptOption->ChannelName) 
     ptLine->NAME = ScrNameIndex(ptOption->ChannelName,Counter);
    fprintf(stderr,"|_____Routing Channel : %s \n",ptLine->NAME);
    ptChannel  = ptLine->CHANNEL;
    if (SymbolicChannelRouter(&ptChannel->NORTH_LIST,&ptChannel->SOUTH_LIST,
                              &ptChannel->WEST_LIST,&ptChannel->EAST_LIST,
                              &(ptChannel->WIDTH),&(ptChannel->HEIGTH),
                              &(ptChannel->H_SEGMENT),&(ptChannel->V_SEGMENT),
                              &(ptChannel->VIA))) 
     LineWidthChange = TRUE;
    ptLine->WIDTH = ((ptChannel->WIDTH - 1) * PITCH_X) + WESTOFFSET + EASTOFFSET;
    ptLine->HEIGTH = ((ptChannel->HEIGTH - 1) * PITCH_Y) + SOUTHOFFSET + NORTHOFFSET;
   }
  }
  CellLineNumber--;
 
  if (LineWidthChange) {

# ifdef SCR_DEBUG   
   fprintf(stderr,"Up date the width rows ... \n");
# endif

   UpDateWidthLine(ptfig,NewphIns,&CounterInst);
  }

# ifdef SCR_DEBUG   
  fprintf(stderr,"In ViewDataBase \n");
   ViewScrFigure(ptfig);
   ViewScrChannel(ptfig);
  fprintf(stderr,"Out ViewDataBase \n");
# endif

  if (LeadPlaceConList && (LeadPlaceConList->VER_FEED != 0)) {
   fprintf(stderr,"Making vertical feed \n");
   if (ptOption->SupplyRecall != 0) 
    MakeVerticalFeed(ptfig,NewphIns,LeadPlaceConList->VER_FEED,
                     ptOption->SupplyRecall,ptXInsertList,&CounterInst);
   else
    MakeVerticalFeed(ptfig,NewphIns,LeadPlaceConList->VER_FEED,
                     1,ptXInsertList,&CounterInst);
  }

  if (LeadPlaceConList && (LeadPlaceConList->HOR_FEED != 0)) {
   fprintf(stderr,"Making horizontal feed \n");
   MakeHorizFeed(ptfig,LeadPlaceConList->HOR_FEED);
  }

  if (ptOption->SupplyRecall != 0) {
   ChannelWidth = RIGHTSUPPLY + LEFTSUPPLY + MIDDLESUPPLY +
                  (long )((CellLineNumber * (VSSWIDTH + VDDWIDTH)) / 2 + 0.5);
   if (ChannelWidth % PITCH_X) {
    ChannelWidth = ((ChannelWidth / PITCH_X) + 1) * PITCH_X;
   }
   fprintf(stderr,"Making vertical power and ground wires  \n");
   MakeVerticalChannel(ptfig, ptXInsertList, ChannelWidth);
  }

  if (ptOption->OutputFileName == NULL) {
   fprintf(stderr,"Saving layout : %s\n",ptOption->InputFileName);
   Scr2Mbk(ptfig,ptOption->InputFileName,ptOption, ptXInsertList,ChannelWidth);
  }
  else {
   fprintf(stderr,"Saving layout : %s\n",ptOption->OutputFileName);
   Scr2Mbk(ptfig,ptOption->OutputFileName,ptOption, ptXInsertList,ChannelWidth);
  }
 }
 else 
  if (!(ptOption->Placer)) ScrUsage(argv[0]);
  else {
   fprintf(stderr,"Saving layout : %s\n",ptOption->InputFileName);
   savephfig(ptphfig);
  }
 return(0);
}
