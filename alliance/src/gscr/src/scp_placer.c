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
/*     Product  : Standard Cell Placer                                        */
/*     File     : scp_Placer.c                                                */
/*     Contents :                                                             */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   :                                    Date : ..........     */
/*     Modified by : Xavier Picat                       Date : 08/07/1993     */
/*                                                                            */
/******************************************************************************/

/*---------------------------------------------------------\
					Les includes
\---------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include <mut.h>
#include <mlo.h>
#include <mph.h>
#include "scp_placer.h"
#include "scp_scp2mbk.h"

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
 fprintf(stdout,"Syntax error on command line\n");
 fprintf(stdout,"usage : %s <source> [options...]\n",Execut);
 fprintf(stdout,"where valid options are :\n");
 fprintf(stdout," <source> : netlist file and placed layout (same name)\n");
 fprintf(stdout," [-i IterationNumber] : This action is used to improve the quality of the placement\n");
 fprintf(stdout," [-s SliceNumber]     : This option allows the designer to set the number of slices\n");
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
 char		car;

 ptOption->ChannelName  = NULL;
 ptOption->FileName     = NULL;
 ptOption->Placer       = 0;
 ptOption->Router       = 0;
 ptOption->SupplyRecall = 1;
 ptOption->Row          = 0;
 ptOption->Iteration    = 0;
 
 for (ArgNumber = 2; ArgNumber < ArgCount; ArgNumber++) {
  char *ArgV = ArgValue[ArgNumber];
  if (*ArgV == '-') {
   switch (*++ArgV) {

    case 's' : if ((car = *++ArgV) == '\0') 
                ptOption->Row = atoi(ArgValue[++ArgNumber]);
               else ScrUsage(ArgValue[0]);
               continue;

    case 'i' : if (*++ArgV == '\0') 
                ptOption->Iteration = atoi(ArgValue[++ArgNumber]);
               else ScrUsage(ArgValue[0]);
               continue;

    default  : ScrUsage(ArgValue[0]);
   }
  }
 }
 return(ptOption);
}

/******************************************************************************/
/*	Function : main()                                                     */
/******************************************************************************/
int	main(argc,argv)

int	argc;
char	*argv[];

{
 phfig_list	*ptphfig = NULL;
 lofig_list	*ptlofig = NULL;
 OptionList	*ptOption = NULL;

 mbkenv();

 ptOption = GetOptions(argc,argv);

 fprintf(stdout,"Loading logical view : %s\n",argv[1]);
 ptlofig = getlofig(argv[1],'A');
 rflattenlofig(ptlofig,'Y','Y');

 if (ptlofig->LOTRS) {
  fprintf(stdout,"scp_error : Check that the catalogue file existes and that it contains all the models instanciated in the figure.\n");
  exit(1);
 }

 fprintf(stdout,"Placing logical view : %s\n",argv[1]);
 ptphfig = Placer2Scr( ptlofig->NAME, Place( ptlofig, ptOption->Iteration, ptOption->Row, 0, 7, 5, 5, NULL)); 

 savephfig(ptphfig);

 return(0);
}
