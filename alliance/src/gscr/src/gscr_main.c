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
/* Cette fonction est provisoire, elle fait just appel a la fonction routeur  */
/******************************************************************************/

# include <stdio.h>
# include "apr.h"
# include "gscr_DataBase.h"
# include "gscr_StaticUtil.h"
# include "gscr_SymbChanRout.h"

main(argc,argv)

int	argc;
char	*argv[];

{
 ConnectorList	*LeadNorthList = NULL;
 ConnectorList	*LeadSouthList = NULL;
 ConnectorList	*LeadWestList  = NULL;
 ConnectorList	*LeadEastList  = NULL;
 SegmentList	*LeadH_Segment = NULL;
 SegmentList	*LeadV_Segment = NULL;
 ViasList	*LeadViaList   = NULL;
 long		ChannelWidth   = 0;
 long		ChannelHeight  = 0;
 BOOLEAN	Change = FALSE;

 mbkenv();
 if (argc == 1) fprintf(stdout,"Erreur manque du nom de fichier \n");
 else {
  U_GetChannel(argv[1], &LeadNorthList, &LeadSouthList, 
               &LeadWestList, &LeadEastList);

# ifdef SCR_DEBUG
  fprintf(stdout,"Northlist : \n");
  U_PrintList(LeadNorthList);
  fprintf(stdout,"Southlist : \n");
  U_PrintList(LeadSouthList);
  fprintf(stdout,"Westlist : \n");
  U_PrintList(LeadWestList);
  fprintf(stdout,"Eastlist : \n");
  U_PrintList(LeadEastList);
# endif
 
  ChannelWidth = U_ChannelWidth(LeadNorthList, LeadSouthList);

# ifdef SCR_DEBUG
  fprintf(stdout,"ChannelWidth = %ld \n",ChannelWidth);
# endif

 Change = SymbolicChannelRouter(LeadNorthList,LeadSouthList, LeadWestList, 
                        LeadEastList, &ChannelWidth, &ChannelHeight,     
                        &LeadH_Segment, &LeadV_Segment, &LeadViaList);

  gscr2mbk(argv[2],LeadH_Segment,LeadV_Segment,LeadViaList,
                         LeadNorthList,LeadSouthList,LeadWestList,
                         LeadEastList,ChannelWidth,ChannelHeight);

 if (LeadH_Segment) SCR_FreeSegmentList(LeadH_Segment);
 if (LeadV_Segment) SCR_FreeSegmentList(LeadV_Segment);
 if (LeadViaList) SCR_FreeViasList(LeadViaList);

# ifdef SCR_DEBUG
  fprintf(stdout,"ChannelWidth = %ld \n",ChannelWidth);
  fprintf(stdout,"ChannelHeight = %ld \n",ChannelHeight);
  fprintf(stdout,"NorthList : \n");
  U_PrintList(LeadNorthList);
  fprintf(stdout,"SouthList : \n");
  U_PrintList(LeadSouthList);
  fprintf(stdout,"Westlist : \n");
  U_PrintList(LeadWestList);
  fprintf(stdout,"Eastlist : \n");
  U_PrintList(LeadEastList);
# endif

 }
}
 	

