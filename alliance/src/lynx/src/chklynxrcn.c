/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*************************************************************
*                  SOURCES                                   *
*                                                            *
* Changed by:    Greg                                        *
* Date:          05/07/95                                    *
* New file:      -                                           *
* Comments:                                                  *
* Fonction permettant la verification et le debuggage        *
* des structures.                                            *
*                                                            *
*                                                            *
*************************************************************/

# include "rds.h"
# include "mut.h"
# include "mlo.h"
# include "rcn.h"
# include "mph.h"
# include "rpr.h"
# include "lynxrcn.h"
# include "chklynxrcn.h"
# include "addlynxrcn.h"
# include "mbkrds.h"
# include "extract.h"
# include "via.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/timeb.h>

void GrgChkInConFEqui(fig)
lofig_list      *fig;

/*************************************************************
* Cette procedure verifie la presence d'une equipotentielle  *
* physique pour chaque signal logique et la correspondance   *
* connecteur logique vers connecteur physique.               *
*                                                            *
*************************************************************/
{
  losig_list    *ScanLoSig;     /* Pour parcourir la liste des equipotentielles         */
  rdsrec_list   *ScanPhRec;     /* Pour la meme chose                                   */
  char           EndEqui;       /* Indique la fin du traitement de l'equi               */
  loins_list    *ScanLoIns;
  lotrs_list    *ScanLoTrs;
  locon_list    *ScanLoCon;

  for(ScanLoSig=fig->LOSIG; ScanLoSig; ScanLoSig=ScanLoSig->NEXT)
  {
    EndEqui=0;
    ScanPhRec=(rdsrec_list*)(getptype(ScanLoSig->USER,RCN_FEQUI)->DATA); 
    while(!EndEqui)
    {
      if(ScanPhRec==NULL)
	GrgFatalError(22);
      ScanPhRec->USER=NULL;
      if(IsLynxEndEqui(ScanPhRec))
	EndEqui=1;
      else
	ScanPhRec=LYNX_EQUI(ScanPhRec);
    }
  }
  for(ScanLoCon=fig->LOCON; ScanLoCon; ScanLoCon=ScanLoCon->NEXT)
  {
    if(!getptype(ScanLoCon->USER,RDSLOCON1))
      GrgFatalError(23);
  }
  for(ScanLoIns=fig->LOINS; ScanLoIns; ScanLoIns=ScanLoIns->NEXT)
  {
    for(ScanLoCon=ScanLoIns->LOCON; ScanLoCon; ScanLoCon=ScanLoCon->NEXT)
    {
      if(!getptype(ScanLoCon->USER,RDSLOCON2))
	GrgFatalError(24);
    }
  }
  for(ScanLoTrs=fig->LOTRS; ScanLoTrs; ScanLoTrs=ScanLoTrs->NEXT)
  {
    if(!getptype(ScanLoTrs->DRAIN->USER,RDSLOCON3))
      GrgFatalError(25);
    if(!getptype(ScanLoTrs->GRID->USER,RDSLOCON3))
      GrgFatalError(25);
    if(!getptype(ScanLoTrs->SOURCE->USER,RDSLOCON3))
      GrgFatalError(25);
  }
}





void GrgChkOutConVia(fig,rcnlynxdebug)
lofig_list      *fig;
short rcnlynxdebug;

/*************************************************************
* Cette procedure verifie la correspondance entre connecteur *
* physique vers connecteur logique, et le regroupement des   *
* vias.                                                      *
*                                                            *
*************************************************************/
{
  losig_list    *ScanLoSig;     /* Pour parcourir la liste des equipotentielles         */
  rdsrec_list   *ScanPhRec;     /* Pour la meme chose                                   */
  char           EndEqui;       /* Indique la fin du traitement de l'equi               */

  for(ScanLoSig=fig->LOSIG; ScanLoSig; ScanLoSig=ScanLoSig->NEXT)
  {
    EndEqui=0;
    ScanPhRec=(rdsrec_list*)(getptype(ScanLoSig->USER,RCN_FEQUI)->DATA); 
    while(!EndEqui)
    {
      if(IsRdsConnector(ScanPhRec))
      {
	if(!getptype(ScanPhRec->USER,RDSLOCON))
	  GrgFatalError(26);
      }
      if(IsLynxEndEqui(ScanPhRec))
	EndEqui=1;
      else
	ScanPhRec=LYNX_EQUI(ScanPhRec);
    }
  }
}


void Grgdisplay_datain(fig)
lofig_list   *fig;
{
  rdsrec_list *scan;
  losig_list  *sg;
  chain_list  *name;
  locon_list  *con,*locon;
  loins_list  *ins;
  lotrs_list  *trs;
  GrgPhVia_list *via;
  char EndEqui=0;
  ptype_list *ptptype;

  fprintf(stderr,"Figure name: %s\n",fig->NAME);
  fprintf(stderr,"RDS_PHYSICAL_GRID: %ld\n",RDS_PHYSICAL_GRID);
  fprintf(stderr,"RDS_UNIT: %ld\n",RDS_UNIT);
  fprintf(stderr,"RDS_LAMBDA: %ld\n",RDS_LAMBDA);
  fprintf(stderr,"SCALE_X: %ld\n",SCALE_X);
  fprintf(stderr,"Valeurs techno (deja divisee par RDS_LAMBDA(2)) :\n");
  fprintf(stderr,"Alu1  Capacite surfacique (pf/u^2): %e\n",GET_LYNX_SURFACE_CAPA_LAYER(RDS_ALU1));
  fprintf(stderr,"      Capacite perimetrique (pf/u): %e\n",GET_LYNX_PERIMETER_CAPA_LAYER(RDS_ALU1));
  fprintf(stderr,"      Resistance par carre (om/sq): %e\n",GET_LYNX_RESISTOR_LAYER(RDS_ALU1));
  fprintf(stderr,"Alu2  Capacite surfacique (pf/u^2): %e\n",GET_LYNX_SURFACE_CAPA_LAYER(RDS_ALU2));
  fprintf(stderr,"      Capacite perimetrique (pf/u): %e\n",GET_LYNX_PERIMETER_CAPA_LAYER(RDS_ALU2));
  fprintf(stderr,"      Resistance par carre (om/sq): %e\n",GET_LYNX_RESISTOR_LAYER(RDS_ALU2));
  fprintf(stderr,"Alu3  Capacite surfacique (pf/u^2): %e\n",GET_LYNX_SURFACE_CAPA_LAYER(RDS_ALU3));
  fprintf(stderr,"      Capacite perimetrique (pf/u): %e\n",GET_LYNX_PERIMETER_CAPA_LAYER(RDS_ALU3));
  fprintf(stderr,"      Resistance par carre (om/sq): %e\n",GET_LYNX_RESISTOR_LAYER(RDS_ALU3));

  for(sg=fig->LOSIG;sg;sg=sg->NEXT)
  {
    scan=((rdsrec_list*)(getptype(sg->USER,RCN_FEQUI)->DATA));
    EndEqui=0; 
    fprintf(stderr,"\nNouvelle equi: %ld\n",sg->INDEX);
    fprintf(stderr,"Nom(s) du signal logique: %s\n",(sg->NAMECHAIN!=NULL)?((char*)(sg->NAMECHAIN->DATA)):"-");  
    if(sg->NAMECHAIN!=NULL)
      for(name=sg->NAMECHAIN->NEXT;name;name=name->NEXT)
	fprintf(stderr,"                          %s\n",(char*)name->DATA);
    while(!EndEqui) 
    {
      fprintf(stderr,"  %6ld %6ld %6ld %6ld  | ",scan->X,scan->Y,scan->X+scan->DX,scan->Y+scan->DY);
      fprintf(stderr,"%15s (%3d) | ",RDS_LAYER_NAME[GetRdsLayer(scan)],GetRdsLayer(scan));
      if (IsRdsConnector(scan)) fprintf(stderr,"%20s  | ","Connecteur");
      if (IsRdsSegment(scan))   fprintf(stderr,"%20s  | ","Segment");
      if (IsRdsReference(scan)) fprintf(stderr,"%20s  | ","Reference");
      if (IsRdsRealVia(scan))       fprintf(stderr,"%20s  | ","Via");
      if (IsRdsInstance(scan))  fprintf(stderr,"%20s  | ","Instance");
      if (IsRdsFigure(scan))    fprintf(stderr,"%20s  | ","Figure"); 

      if(scan->NAME!=NULL)
	fprintf(stderr,"  %s",scan->NAME); 

      for(ptptype = scan->USER ; ptptype ; ptptype = ptptype->NEXT )
        printf("[%ld-%08lX] ", ptptype->TYPE, (long)(ptptype->DATA) );
      printf("*\n");
      
      if(getptype(scan->USER,RDSLOCON))
      {
	locon=((locon_list*)(getptype(scan->USER,RDSLOCON)->DATA));
	fprintf(stderr,"\n     -> Logique: %10s signal %ld",locon->NAME,locon->SIG->INDEX);
      }
      if(getptype(scan->USER,GRGVIA))
      {
	via=((GrgPhVia_list*)(getptype(scan->USER,GRGVIA)->DATA));
	fprintf(stderr,"\n     ->Autres couches: %6ld %6ld %6ld %6ld  %s\n",via->LAYER1->X,
									    via->LAYER1->Y,
									    via->LAYER1->X+via->LAYER1->DX,
									    via->LAYER1->Y+via->LAYER1->DY,
									    RDS_LAYER_NAME[GetRdsLayer(via->LAYER1)]);
	if(via->LAYER2)
	  fprintf(stderr,"                       %6ld %6ld %6ld %6ld  %s\n",via->LAYER2->X,
									    via->LAYER2->Y,
									    via->LAYER2->X+via->LAYER2->DX,
									    via->LAYER2->Y+via->LAYER2->DY,
									    RDS_LAYER_NAME[GetRdsLayer(via->LAYER2)]);
	if(via->LAYER3)
	  fprintf(stderr,"                       %6ld %6ld %6ld %6ld  %s\n",via->LAYER3->X,
									    via->LAYER3->Y,
									    via->LAYER3->X+via->LAYER3->DX,
									    via->LAYER3->Y+via->LAYER3->DY,
									    RDS_LAYER_NAME[GetRdsLayer(via->LAYER3)]);
      }
      
      fprintf(stderr,"\n");
    
      if(IsLynxEndEqui(scan))
	EndEqui=1;
      else
	scan=LYNX_EQUI(scan);
    }
  }
  fprintf(stderr,"\n\nListe des connecteurs de la figure:\n");
  for(con=fig->LOCON;con;con=con->NEXT)
  {
    fprintf(stderr,"Connecteur logique: %s -> %ld\n",con->NAME,con->SIG->INDEX);
    Grgdisplayrdscon(con->USER,RDSLOCON1);
  }
  fprintf(stderr,"\n\nListe des instances de la figure:\n");
  for(ins=fig->LOINS;ins;ins=ins->NEXT)
  {
    fprintf(stderr,"Instance: %s basee sur %s\n",ins->INSNAME,ins->FIGNAME);
    for(con=ins->LOCON;con;con=con->NEXT)
    {
      fprintf(stderr,"Connecteur logique: %s -> %ld\n",con->NAME,con->SIG->INDEX);
      Grgdisplayrdscon(con->USER,RDSLOCON2);
    }
  }
  fprintf(stderr,"\n\nListe des transistors de la figure:\n");
  for(trs=fig->LOTRS;trs;trs=trs->NEXT)
  {
    fprintf(stderr,"Transistor: %c  W=%ld L=%ld\n",(trs->TYPE==TRANSN)?'N':'P',trs->WIDTH,trs->LENGTH);
    fprintf(stderr,"Connecteur logique drain: %s -> %ld\n",trs->DRAIN->NAME,trs->DRAIN->SIG->INDEX);
    Grgdisplayrdscon(trs->DRAIN->USER,RDSLOCON3);
    fprintf(stderr,"Connecteur logique grille: %s -> %ld\n",trs->GRID->NAME,trs->GRID->SIG->INDEX);
    Grgdisplayrdscon(trs->GRID->USER,RDSLOCON3);
    fprintf(stderr,"Connecteur logique source: %s -> %ld\n",trs->SOURCE->NAME,trs->SOURCE->SIG->INDEX);
    Grgdisplayrdscon(trs->SOURCE->USER,RDSLOCON3);
  }
}





void Grgdisplayrdscon(ptptype,t)
ptype_list *ptptype;
long t;
{
  rdsrec_list *scan;
  chain_list *scanchain;
  if(getptype(ptptype,t))
  {
    for( scanchain=((chain_list*)(getptype(ptptype,t)->DATA));scanchain;scanchain=scanchain->NEXT)
    {
      scan=((rdsrec_list*)(scanchain->DATA));
      fprintf(stderr," - Connecteur physique:\n");
      fprintf(stderr,"  %6ld %6ld %6ld %6ld  ",scan->X,scan->Y,scan->X+scan->DX,scan->Y+scan->DY);
      fprintf(stderr,"%15s  ",RDS_LAYER_NAME[GetRdsLayer(scan)]);
      if (IsRdsConnector(scan)) fprintf(stderr,"%20s   ","Connecteur");
      if (IsRdsSegment(scan))   fprintf(stderr,"%20s   ","Segment");
      if (IsRdsReference(scan)) fprintf(stderr,"%20s   ","Reference");
      if (IsRdsRealVia(scan))       fprintf(stderr,"%20s   ","Via");
      if (IsRdsInstance(scan))  fprintf(stderr,"%20s   ","Instance");
      if (IsRdsFigure(scan))    fprintf(stderr,"%20s   ","Figure"); 

      if(scan->NAME!=NULL)
	fprintf(stderr,"  %s",scan->NAME); 
      fprintf(stderr,"\n");
    }
  }
}
void dbg_affrec(scan)
rdsrec_list     *scan;
{
  locon_list *locon;

  fprintf(stderr,"(%6ld,%6ld) - (%6ld,%6ld)  ",scan->X,scan->Y,scan->X+scan->DX,scan->Y+scan->DY);
  fprintf(stderr,"%15s (%3d) ",RDS_LAYER_NAME[GetRdsLayer(scan)],GetRdsLayer(scan));
  if (IsRdsConnector(scan)) fprintf(stderr,"%20s  ","Connecteur");
  if (IsRdsSegment(scan))   fprintf(stderr,"%20s  ","Segment");
  if (IsRdsReference(scan)) fprintf(stderr,"%20s  ","Reference");
  if (IsRdsRealVia(scan))       fprintf(stderr,"%20s  ","Via");
  if (IsRdsInstance(scan))  fprintf(stderr,"%20s  ","Instance");
  if (IsRdsFigure(scan))    fprintf(stderr,"%20s  ","Figure");

  if(scan->NAME!=NULL)
    fprintf(stderr,"  %s",scan->NAME);
  if(getptype(scan->USER,RDSLOCON))
  {
    locon=((locon_list*)(getptype(scan->USER,RDSLOCON)->DATA));
    fprintf(stderr,"\n     -> Logique: %10s signal %ld",locon->NAME,locon->SIG->INDEX);
  }
  fprintf(stderr,"\n");
}
