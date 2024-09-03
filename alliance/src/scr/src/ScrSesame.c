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
/*    Fichier : ScrSesame.c                                                   */
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

# include <string.h>
# include <mut.h>
# include <mlo.h> 
# include <mph.h> 

# define XNEG 1
# define YNEG 2

extern int SXMODE;
  
/******************************************************************************/
/*	Function : UpDateNegatifCoordinate()                                  */
/******************************************************************************/
void		UpDateNegatifCoordinate(ptphfig,XYflag)
phfig_list	*ptphfig;
int		XYflag;
 
{
 phcon_list     *CurrentphCon = NULL;
 phins_list     *CurrentphIns = NULL;

 if (XYflag == XNEG) {
   for (CurrentphCon = ptphfig->PHCON; CurrentphCon; CurrentphCon = CurrentphCon->NEXT) {
     CurrentphCon->XCON += (ptphfig->XAB1 * (-1));
   }
   for (CurrentphIns = ptphfig->PHINS; CurrentphIns; CurrentphIns = CurrentphIns->NEXT) {
     CurrentphIns->XINS += (ptphfig->XAB1 * (-1));
   }
 }
 else {
   if (XYflag == YNEG) {
     for (CurrentphCon = ptphfig->PHCON; CurrentphCon; CurrentphCon = CurrentphCon->NEXT) {
       CurrentphCon->YCON += (ptphfig->YAB1 * (-1));
     }
     for (CurrentphIns = ptphfig->PHINS; CurrentphIns; CurrentphIns = CurrentphIns->NEXT) {
       CurrentphIns->YINS += (ptphfig->YAB1 * (-1));
     }
   }
 }

}

/******************************************************************************/
/*	Function : InsCompare()                                               */
/******************************************************************************/
void		InsCompare(ptphIns,ptloIns)

phins_list	*ptphIns;
loins_list	*ptloIns;
 
{
 phins_list	*CurrentphIns = NULL;
 phins_list	*ptCurrentph  = NULL;
 loins_list	*CurrentloIns = NULL;
 loins_list	*ptCurrentlo  = NULL;
 char		*InsName;

 for (CurrentloIns = ptloIns; CurrentloIns; CurrentloIns = CurrentloIns->NEXT) {
  InsName = CurrentloIns->INSNAME;
  for (ptCurrentlo = ptloIns; ptCurrentlo; ptCurrentlo = ptCurrentlo->NEXT) 
   if ((ptCurrentlo != CurrentloIns) && (InsName == ptCurrentlo->INSNAME)) {
    fprintf(stderr,"scr_error : Logic instance : %s is redeclared !!\n", InsName);
    exit(1);
   }
 }

 for (CurrentphIns = ptphIns; CurrentphIns; CurrentphIns = CurrentphIns->NEXT) {
  InsName = CurrentphIns->INSNAME;
  for (ptCurrentph = ptphIns; ptCurrentph; ptCurrentph = ptCurrentph->NEXT) 
   if ((ptCurrentph != CurrentphIns) && (InsName == ptCurrentph->INSNAME)) {
    fprintf(stderr,"scr_error : Physical instance : %s is redeclared !!\n", InsName);
    exit(1);
   }
 }

 for (CurrentphIns = ptphIns; CurrentphIns; CurrentphIns = CurrentphIns->NEXT) {
  /*if (!(incatalogfeed(CurrentphIns->FIGNAME))) {*/
  if (strcmp(CurrentphIns->FIGNAME,(SXMODE ? "rowend_x0":"tie_y"))) {
//  if (strcmp(CurrentphIns->FIGNAME,(SXMODE ? "tie_x0":"tie_y"))) {
   for (CurrentloIns = ptloIns; CurrentloIns; CurrentloIns = CurrentloIns->NEXT)
    if (CurrentloIns->INSNAME == CurrentphIns->INSNAME) break;
   if (CurrentloIns == NULL) {
    fprintf(stderr,"scr_error : Physical instance : %s does not exist in logic net-list !! \n", CurrentphIns->INSNAME);
    exit(1);
   }
  }
 }

 for (CurrentloIns = ptloIns; CurrentloIns; CurrentloIns = CurrentloIns->NEXT) {
  for (CurrentphIns = ptphIns; CurrentphIns; CurrentphIns = CurrentphIns->NEXT)
   if (CurrentloIns->INSNAME == CurrentphIns->INSNAME) break;
  if (CurrentphIns == NULL) {
    fprintf(stderr,"scr_error : Logic instance : %s does not exist in physical net-list !! \n", CurrentloIns->INSNAME);
    exit(1);
  }
 }
}

/******************************************************************************/
/*	Function : ConCompare()                                               */
/******************************************************************************/
void		ConCompare(ptphCon,ptloCon)

phcon_list	*ptphCon;
locon_list	*ptloCon;

{
 phcon_list	*CurrentphCon = NULL;
 locon_list	*CurrentloCon = NULL;

 for (CurrentphCon = ptphCon; CurrentphCon; CurrentphCon = CurrentphCon->NEXT) {
  for (CurrentloCon = ptloCon; CurrentloCon; CurrentloCon = CurrentloCon->NEXT) 
   if (CurrentphCon->NAME == CurrentloCon->NAME) break;
  if (CurrentloCon == NULL) {
   fprintf(stderr,"scr_error : Physical connector : %s does not exist in logic connector list\n", CurrentphCon->NAME);
   exit(1);
  }
 }
}

/******************************************************************************/
/*	Function : Sesame()                                                   */
/******************************************************************************/
void		Sesame(ptphfig,ptlofig)

phfig_list	*ptphfig;
lofig_list	*ptlofig;

{
 if (ptphfig->PHSEG || ptphfig->PHVIA) {
  fprintf(stderr,"scr_error : Segments and Vias list must be NULL !!\n");
  exit(1);
 }
 if (ptphfig->XAB1 < 0) {
  UpDateNegatifCoordinate(ptphfig,XNEG);
  ptphfig->XAB2 += (ptphfig->XAB1 * (-1));
  ptphfig->XAB1 += (ptphfig->XAB1 * (-1));
 }
 if (ptphfig->YAB1 < 0) {
  UpDateNegatifCoordinate(ptphfig,YNEG);
  ptphfig->YAB2 += (ptphfig->YAB1 * (-1));
  ptphfig->YAB1 += (ptphfig->YAB1 * (-1));
 }
 InsCompare(ptphfig->PHINS,ptlofig->LOINS);
 ConCompare(ptphfig->PHCON,ptlofig->LOCON);
}
 
