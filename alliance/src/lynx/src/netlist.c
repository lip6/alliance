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

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  netlist.c                        |
|                                                             |
| Authors :               Jacomme  Ludovic                    |
|                         Picault Stephane    *4p*            | 
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>

# include "mut.h"
# include "mlo.h"
# include "mlu.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "rfm.h"

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/
# include "rcn.h"
# include "lynxrcn.h"
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

# include "parse.h"
# include "mbkrds.h"
# include "extract.h"
# include "netlist.h"
# include "error.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     LynxBuildLogicalSignal                  |
|                                                             |
\------------------------------------------------------------*/

long LynxBuildLogicalSignal( FigureRds, HeadEqui, LogicalFigure, Core, Real, Rcnet, Warning )

   rdsfig_list *FigureRds;
   rdsrec_list *HeadEqui;
   lofig_list  *LogicalFigure;
   char         Core;
   char         Real;
   char		Rcnet;
   char         Warning;
{
   void        *FigureDumpFloat;
   void        *FigureDumpEqui;
   void        *FigureDumpScan;

   losig_list  *Signal;
   chain_list  *SignalChain;
   float        SignalCapa;
   float        Capa;
   char         SignalFloat;
   char         FoundFloat;
   char         FoundScan;
   long         SignalIndex;
   char         SignalType;

   chain_list  *ScanSignalChain;
   chain_list  *DeleteChain;
   ptype_list  *SignalPtype;
   ptype_list  *ScanPtype;

   char         LayerMbk;
   char         LayerRds;
   char         Scan;

   char        *ConnectorName=NULL;
   locon_list  *LogicalConnector;

   rdsrec_list *ScanEqui;
   rdsrec_list *FirstEqui;
   rdsrec_list *IndexEqui;

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/

/*
Cette liste contient la liste des rectangles connecteurs. */
  chain_list   *RecConnector=NULL;
  ptype_list   *PtypeRec;
  ptype_list   *PrevFEQUI;
  int           DispWarning=0;

/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

   rdsbegin();

   SignalIndex = 1;
   SignalPtype = (ptype_list *)0;
   Signal      = (losig_list *)0;
   SignalType  = INTERNAL;
   SignalCapa  = 0.0;
   SignalFloat = Core;
   FoundFloat  = 0;
   FoundScan   = 0;

   if ( ! Real )
   {
     FigureDumpFloat = (void *)addphfig( "core_float" );
     FigureDumpEqui  = (void *)addphfig( "core_equi"  );
     FigureDumpScan  = (void *)addphfig( "trace_equi" );
   }
   else
   {
     FigureDumpFloat = (void *)addrdsfig( "core_float", 0 );
     FigureDumpEqui  = (void *)addrdsfig( "core_equi" , 0 );
     FigureDumpScan  = (void *)addrdsfig( "trace_equi", 0 );
   }

   Lynxrecrdsmbk( FigureDumpFloat, FigureRds->LAYERTAB[ RDS_ABOX ], Real );
   Lynxrecrdsmbk( FigureDumpEqui,  FigureRds->LAYERTAB[ RDS_ABOX ], Real );
   Lynxrecrdsmbk( FigureDumpScan,  FigureRds->LAYERTAB[ RDS_ABOX ], Real );

   for ( FirstEqui = ScanEqui = HeadEqui;
         ScanEqui != (rdsrec_list *)0;
         ScanEqui  = LYNX_EQUI( ScanEqui ) )
   {
     if ( ! IsRdsTransGate( ScanEqui ) )
     {
       Capa = GET_LYNX_SURFACE_CAPA_LAYER( GetRdsLayer( ScanEqui ) );

       if ( Capa != RDS_LYNX_CAPA_EMPTY ) 
       {
         SignalCapa += ( ScanEqui->DX * ScanEqui->DY ) * Capa;
       }

       Capa = GET_LYNX_PERIMETER_CAPA_LAYER( GetRdsLayer( ScanEqui ) );

       if ( Capa != RDS_LYNX_CAPA_EMPTY ) 
       {
         if ( ScanEqui->DX > ScanEqui->DY )
         {
           SignalCapa += ( ScanEqui->DX << 1 ) * Capa;
         }
         else
         {
           SignalCapa += ( ScanEqui->DY << 1 ) * Capa;
         }
       }
     }

     if ( ScanEqui->NAME != (char *)0 )
     {
       if ( ( ! IsRdsConInter( ScanEqui ) ) &&
            ( ( IsRdsFigRec( ScanEqui ) ) ||
              ( ! IsRdsRefCon( ScanEqui ) ) ) )
       {
         for ( ScanPtype  = SignalPtype;
               ScanPtype != (ptype_list *)0;
               ScanPtype  = ScanPtype->NEXT )
         {
           if ( (char *)(ScanPtype->DATA) == ScanEqui->NAME ) break;
         }

         if ( ( IsRdsConExter( ScanEqui ) ) ||
              ( ( IsRdsFigRec( ScanEqui ) ) &&
                ( IsRdsRefCon( ScanEqui ) ) ) )
         {
           SignalType    = EXTERNAL;
           ConnectorName = ScanEqui->NAME;

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/
           if( !IsRdsRefCon( ScanEqui ) )
           {
             RecConnector=addchain(RecConnector,ScanEqui);
           }
           else
           {
             if( DispWarning==0 && Rcnet == 'r' )
             {
               DispWarning=1;
               fprintf( stderr,"\n\tWarning : Lynx found a refcon on a figure extracted with the option -ar.\n");
               fprintf( stderr,"\tit will not be possible to correctly use the extracted figure.\n" );
             }
           }
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

           if ( ScanPtype == (ptype_list *)0 )
           {
             SignalPtype = addptype
             (
               SignalPtype, LYNX_ONLY_ONE,
               (void *)( ScanEqui->NAME )
             );
           }
           else
           {
             ScanPtype->TYPE = LYNX_ONLY_ONE;
           }
         }
         else
         {
           if ( ScanPtype == (ptype_list *)0 )
           {
             SignalPtype = addptype
             (
               SignalPtype, LYNX_GOOD,
               (void *)( ScanEqui->NAME )
             );
           }
         }
       }
     }

     if ( Core )
     {
       if ( IsRdsSegment( ScanEqui ) )
       {
         if ( ( IsRdsConInter( ScanEqui ) ) ||
              ( IsRdsConExter( ScanEqui ) ) )
         {
           SignalFloat = 0;
         }
         else
         {
           LayerMbk  = GetLynxAttribute( ScanEqui );
           LayerRds  = GetRdsLayer( ScanEqui );

           if ( GET_LYNX_TRANSISTOR_TYPE( LayerMbk ) != 
                RDS_LYNX_TRANSISTOR_EMPTY )
           {
             SignalFloat = 0;
           }
         }
       }
       else
       if ( ( IsRdsConnector( ScanEqui ) ) ||
            ( IsRdsReference( ScanEqui ) ) )
       {
         SignalFloat = 0;
       }
     }

     if ( IsLynxEndEqui( ScanEqui ) )
     {
       if ( SignalFloat )
       {
         LynxDumpEqui( FigureDumpFloat, FirstEqui, Real );
         FoundFloat = 1;
       }
 
       if ( SignalPtype != (ptype_list *)0 )
       {
         for ( Signal  = LogicalFigure->LOSIG;
               Signal != (losig_list *)0;
               Signal  = Signal->NEXT )
         {
           for ( ScanPtype  = SignalPtype;
                 ScanPtype != (ptype_list *)0;
                 ScanPtype  = ScanPtype->NEXT )
           {
             ScanSignalChain = Signal->NAMECHAIN;

             while ( ScanSignalChain != (chain_list *)0 )
             {
               if ( ScanPtype->DATA == ScanSignalChain->DATA )
               {
                  DeleteChain     = ScanSignalChain;
                  ScanSignalChain = ScanSignalChain->NEXT;
                  ScanPtype->TYPE = LYNX_BAD;

                  Signal->NAMECHAIN = delchain
                  (
                    Signal->NAMECHAIN,
                    DeleteChain
                  );
               }
               else
               {
                 ScanSignalChain  = ScanSignalChain->NEXT;
               }
             }
           }
         }
       }

       SignalChain = (chain_list *)0;

       if ( SignalType == EXTERNAL )
       {
         for ( ScanPtype  = SignalPtype;
               ScanPtype != (ptype_list *)0;
               ScanPtype  = ScanPtype->NEXT )
         {
           if ( ScanPtype->TYPE == LYNX_ONLY_ONE )
           {
             SignalChain = addchain( SignalChain, ScanPtype->DATA ); 

             break;
           }
         }
       }

       if ( SignalChain == (chain_list *)0 )
       {
         for ( ScanPtype  = SignalPtype;
               ScanPtype != (ptype_list *)0;
               ScanPtype  = ScanPtype->NEXT )
         {
           if ( ScanPtype->TYPE == LYNX_GOOD )
           {
             SignalChain = addchain( SignalChain, ScanPtype->DATA );
           }
         }
       }

       freeptype ( SignalPtype );

       Signal  = addlosig
       (
         LogicalFigure,
         SignalIndex,
         SignalChain,
         SignalType
       );
       /* ajoute capa si Rcnet SignalCapa */

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/
       Signal->USER=addptype(Signal->USER,RCN_FEQUI,addchain(NULL,FirstEqui) );
       
       if( Rcnet == 'r' || Rcnet == 'c' )
       {
         addlorcnet( Signal );
         setcapa( Signal, SignalCapa );
       }
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

       if ( ( Signal->NAMECHAIN       != (chain_list *)0 ) &&
            ( Signal->NAMECHAIN->NEXT != (chain_list *)0 ) )
       {
         Signal->NAMECHAIN->DATA = (void *)getsigname( Signal );
         freechain( Signal->NAMECHAIN->NEXT );
         Signal->NAMECHAIN->NEXT = (chain_list *)0;
       }

       if ( SignalType == EXTERNAL )
       {
         for ( LogicalConnector  = LogicalFigure->LOCON;
               LogicalConnector != (locon_list *)0;
               LogicalConnector  = LogicalConnector->NEXT )
         {
           if ( LogicalConnector->NAME == ConnectorName ) break;
         }

         if ( LogicalConnector == (locon_list *)0)
         {
           LogicalConnector = addlocon
           (
             LogicalFigure, ConnectorName,
             Signal, UNKNOWN
           );

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/

/* REPLACE 
           LogicalConnector->USER = (ptype_list *)FirstEqui;
  WITH */

           LogicalConnector->USER = addptype(LogicalConnector->USER,LYNX_DUMP,FirstEqui);
           LogicalConnector->USER = addptype(LogicalConnector->USER,RDSLOCON1,RecConnector);
           RecConnector=NULL;

/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

         }
         else
         {
           if ( Core )
           {
             LynxDumpEqui( FigureDumpEqui,
                           FirstEqui, Real );

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/

/* REPLACE
             LynxDumpEqui( FigureDumpEqui, 
                           (rdsrec_list *)(LogicalConnector->USER), Real );
  WITH */
             LynxDumpEqui( FigureDumpEqui, 
                           (rdsrec_list *)(getptype(LogicalConnector->USER,LYNX_DUMP)->DATA), Real );
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

           }

           if ( Warning || isvdd(ConnectorName) || isvss(ConnectorName) )
           {
             if( !isvdd(ConnectorName) && !isvss(ConnectorName) )
               LynxWarning( LYNX_WARNING_PHYSICAL_CONNECTOR, ConnectorName );

             lofigchain( LogicalFigure );

             freechain(  ((chain_list*)( getptype( Signal->USER, RCN_FEQUI )->DATA ) ) );
             delptype( Signal->USER, RCN_FEQUI );

             dellosig( LogicalFigure, SignalIndex );

             Signal = LogicalConnector->SIG;
             Signal->NAMECHAIN = addchain( Signal->NAMECHAIN, ConnectorName );


             PtypeRec = getptype( LogicalConnector->USER, RDSLOCON1 );
             (chain_list*)( PtypeRec->DATA) = append( (chain_list*)(PtypeRec->DATA), RecConnector );
             RecConnector = NULL;

             if( Rcnet == 'c' || Rcnet == 'r' )
             {
               Signal->PRCN->CAPA = Signal->PRCN->CAPA + SignalCapa;
             }

             PrevFEQUI = getptype( Signal->USER, RCN_FEQUI );
             ((chain_list*)(PrevFEQUI->DATA)) = addchain( ((chain_list*)(PrevFEQUI->DATA)), FirstEqui );
             
           }
           else
           {
             if ( Core )
             {
               LynxWarning( LYNX_WARNING_CORE_FILE, "core_equi" );

               LynxSaveFigure( FigureDumpEqui, Real );
             }

             LynxError( LYNX_ERROR_PHYSICAL_CONNECTOR, ConnectorName, 0 );
           }
         }

         IndexEqui = FirstEqui;

         Scan = (LynxScanEqui != (chain_list *)0);

         for ( IndexEqui  = FirstEqui;
               IndexEqui != LYNX_EQUI( ScanEqui );
               IndexEqui  = LYNX_EQUI( IndexEqui ) )
         {
           if ( ( IsRdsConExter( IndexEqui ) ) ||
                ( ( IsRdsFigRec( IndexEqui ) ) &&
                  ( IsRdsRefCon( IndexEqui ) ) ) )
           {
             if ( Scan )
             {
               if ( IsLynxScanEqui( LynxScanEqui, IndexEqui->NAME ) )
               {
                 LynxDumpEqui( FigureDumpScan, FirstEqui, Real );
                 Scan      = 0;
                 FoundScan = 1;
               }
             }

             if ( IndexEqui->NAME != ConnectorName )
             {
               if ( Core )
               {
                 LynxWarning( LYNX_WARNING_CORE_FILE, "core_equi" );

                 LynxDumpEqui( FigureDumpEqui, FirstEqui, Real );

                 LynxSaveFigure( FigureDumpEqui, Real );
               }

               LynxError( LYNX_ERROR_PHYSICAL_CONNECTORS, IndexEqui->NAME, ConnectorName );
             }
           }

           IndexEqui->USER = addptype( IndexEqui->USER, RDSSIGNAL, (void *)Signal );
         }
       }
       else
       {
         for ( IndexEqui  =  FirstEqui;
               IndexEqui != LYNX_EQUI( ScanEqui );
               IndexEqui  = LYNX_EQUI( IndexEqui ) )
         {
           IndexEqui->USER = addptype( IndexEqui->USER, RDSSIGNAL, (void *)Signal );
         }
       }

       SignalCapa  = 0.0;
       SignalPtype = (ptype_list *)0;
       SignalType  = INTERNAL;
       SignalFloat = Core;
       SignalIndex = SignalIndex + 1;

       FirstEqui = LYNX_EQUI( ScanEqui );
     }
   }
 
   SignalIndex = SignalIndex - 1;

   if ( FoundScan )
   {
     LynxSaveFigure( FigureDumpScan, Real );
   }

   if ( FoundFloat )
   {
     LynxSaveFigure( FigureDumpFloat, Real );

     SignalIndex = - SignalIndex;
   }

   rdsend();
   return( SignalIndex );
}

/*------------------------------------------------------------\
|                                                             |
|                     LynxBuildLogicalInstance                |
|                                                             |
\------------------------------------------------------------*/

long LynxBuildLogicalInstance( FigureRds, LogicalFigure )

   rdsfig_list *FigureRds; 
   lofig_list  *LogicalFigure;
{
   chain_list  *SignalChain;
   chain_list  *ScanChain;
   chain_list  *NameChain;

   lofig_list  *Model;
   lofig_list  *HeadModel;
   loins_list  *LogicalInstance;
   rdsins_list *InstanceRds;

   rdsrec_list *Rec;
   char        *RecName;

   int          Layer;
   char         BuildModel;
   long         Counter;
/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/
   chain_list  *RDSConChain=NULL;
   chain_list  *HEADRDSConChain=NULL;
   chain_list  *ScanRDSConChain=NULL;
   locon_list  *ScanLocon=NULL;
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

   rdsbegin();
   Counter = 0;

   for ( InstanceRds  = FigureRds->INSTANCE;
         InstanceRds != (rdsins_list *)0;
         InstanceRds  = InstanceRds->NEXT )
   {
     SignalChain  = (chain_list *)0;
     NameChain    = (chain_list *)0;
     Model        = (lofig_list *)0;
     HeadModel    = (lofig_list *)0;

     if ( ! ( BuildModel = ( HeadModel == (lofig_list *)0 )))
     {
       Model = getlomodel
       (  
         HeadModel, 
         InstanceRds->FIGNAME 
       );

       BuildModel = ( Model == (lofig_list *)0);
     }

     if ( BuildModel )
     {
       Model = HeadModel = addlomodel 
       (  
         HeadModel, 
         InstanceRds->FIGNAME 
       );
     }
    
     for ( Layer = 0; Layer < RWI_MAX_LAYER; Layer++ )
     {
       for ( Rec  = InstanceRds->LAYERTAB[ Layer ];
             Rec != (rdsrec_list *)0;
             Rec  = Rec->NEXT )
       {

         if ( ( ! IsRdsConInter( Rec ) ) &&
              ( ! IsRdsRefCon( Rec )   ) )   continue; 

         RecName = Rec->NAME;
/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/

/* REPLACE
         for ( ScanChain  = NameChain;
               ScanChain != (chain_list *)0;
               ScanChain  = ScanChain->NEXT )
         {
             if ( (char *)ScanChain->DATA  == RecName ) break;
         }
  WITH */
         for ( ScanChain  = NameChain, ScanRDSConChain=RDSConChain;
               ScanChain != (chain_list *)0;
               ScanChain  = ScanChain->NEXT, ScanRDSConChain=ScanRDSConChain->NEXT )
         {
             if ( ( (char *)ScanChain->DATA  == RecName  ) &&
                  ! IsRdsRefCon( Rec )
                )
             {
               ScanRDSConChain->DATA=addchain(ScanRDSConChain->DATA,Rec);
             }
             if ( (char *)ScanChain->DATA  == RecName ) break;
         }
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

         if ( ScanChain == (chain_list *)0 )
         {
           NameChain = addchain
           ( 
             NameChain, 
             RecName
           );

           if ( BuildModel )
           {
             addlocon
             ( 
               Model, RecName, 
               (losig_list *)0,
               UNKNOWN
             );
           }

           SignalChain = addchain
           ( 
             SignalChain, 
             getptype( Rec->USER , RDSSIGNAL )->DATA
           );
/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/
           if( ! IsRdsRefCon( Rec ) )
             RDSConChain = addchain(RDSConChain,addchain(NULL,Rec));
           else
             RDSConChain = addchain(RDSConChain,NULL);
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/
         }
       }
     }

     Counter         = Counter + 1;
     LogicalInstance = addloins
     (
       LogicalFigure,
       InstanceRds->INSNAME,
       Model, SignalChain
     );

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/
     HEADRDSConChain=RDSConChain;
     for(ScanLocon=LogicalInstance->LOCON; ScanLocon; ScanLocon=ScanLocon->NEXT)
     {
       ScanLocon->USER=addptype(ScanLocon->USER,RDSLOCON2,RDSConChain->DATA);
       RDSConChain=RDSConChain->NEXT;
     }
     freechain(HEADRDSConChain);
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

     freechain( NameChain );
  }

  rdsend();
  return( Counter );
}


/*------------------------------------------------------------\
|                                                             |
|                      LynxTreatTransistor                    |
|                                                             |
\------------------------------------------------------------*/

void LynxTreatTransistor( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = Rectangle;

  do
  {
    SetLynxTreatTransistor( ScanRec );

    ScanRec = LYNX_LINK( ScanRec );
  }
  while ( ScanRec != Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     LynxBuildLogicalTransistor              |
|                                                             |
\------------------------------------------------------------*/


long LynxBuildLogicalTransistor( FigureRds, LogicalFigure )

   rdsfig_list *FigureRds;
   lofig_list  *LogicalFigure;
{
  rdsrec_list *Gate;
  rdsrec_list *Source;
  rdsrec_list *Drain;
  rdsrec_list *Diff;
  rdsrec_list *Bulk;           /*4p*/
  int          BulkLayer;      /*4p*/
  int          GateLayer;
  int          DiffLayer;
  int          RdsLayer;
  int          MbkLayer;
  int          Orient;
  long         Width;
  long         Length;
  long         Ps;
  long         Pd;
  long         Xs;
  long         Xd;
  long         Counter;
  long         DiffMin;
  long         DiffMax;
  long         GateMin;
  long         GateMax;
  int          TransLayer;
  int          TransType;
  int          tridx = 1;
  char         trname[16];
  ptype_list   *ptlbulk;
 /*************************************************************
* MODIFICATIONS                                              *
*************************************************************/
  lotrs_list   *xxx_trs;
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/
 
  rdsbegin();

  Counter   = 0;

  for ( TransLayer = 0; TransLayer < MBK_MAX_LAYER; TransLayer++ )
  {
    TransType = GET_LYNX_TRANSISTOR_TYPE( TransLayer );
    GateLayer = GET_LYNX_TRANSISTOR_GATE_LAYER( TransLayer );
    DiffLayer = GET_LYNX_TRANSISTOR_DIFF_LAYER( TransLayer );
    BulkLayer = GET_LYNX_TRANSISTOR_BULK_LAYER( TransLayer );     /*4p*/

    if ( ( TransType == RDS_LYNX_TRANSISTOR_EMPTY ) ||
         ( GateLayer == RDS_LYNX_TRANSISTOR_EMPTY ) ||
         ( DiffLayer == RDS_LYNX_TRANSISTOR_EMPTY ) ||
         ( BulkLayer == RDS_LYNX_TRANSISTOR_EMPTY )) continue;     /*4p*/


    if ( TransType == NTRANS ) TransType = TRANSN;
    else                       TransType = TRANSP;

    for ( Gate  = FigureRds->LAYERTAB[ GateLayer ];
          Gate != (rdsrec_list *)0;
          Gate  = Gate->NEXT )
    {
      if ( ( ! IsLynxTreatTransistor( Gate ) ) &&
           (   IsRdsSegment( Gate )          ) )
      {
        MbkLayer = GetLynxAttribute( Gate );

        if ( MbkLayer == TransLayer )
        {
          LynxTreatTransistor( Gate );

          Orient = GetLynxOrient( Gate );
          Source = (rdsrec_list *)0;
          Drain  = (rdsrec_list *)0;
          /* Modif Ludo : 4p is a b... */
          Bulk   = (rdsrec_list *)0;

          if ( ( Orient == LYNX_NORTH ) || 
               ( Orient == LYNX_SOUTH ) )
          {
            GateMin = Gate->X;
            GateMax = GateMin + Gate->DX;
            DiffMin = 0;
            DiffMax = 0;

            Diff = LYNX_LINK( Gate );

            while( Diff != Gate )
            {
              RdsLayer = GetRdsLayer( Diff );

              if ( RdsLayer == DiffLayer ) 
              {
                if ( Source == (rdsrec_list *)0 ) 
                {
                  DiffMin = Diff->Y;
                  DiffMax = DiffMin + Diff->DY;
                  Source  = Diff;
                }
                else
                if ( Drain == (rdsrec_list *)0 )
                {
                  Drain = Diff;
                }

                if ( DiffMin > Diff->Y                ) DiffMin = Diff->Y;
                if ( DiffMax < ( Diff->Y + Diff->DY ) ) DiffMax = Diff->Y + Diff->DY;
              } 
              else
              if ( RdsLayer == GateLayer )
              {
                if ( GateMin > Diff->X                ) GateMin = Diff->X;
                if ( GateMax < ( Diff->X + Diff->DX ) ) GateMax = Diff->X + Diff->DX;
              }
              else
              if ( RdsLayer == BulkLayer )
              {
                 Bulk = Diff;
              }
              Diff = LYNX_LINK( Diff );
            }
          }
          else
          {
            GateMin = Gate->Y;
            GateMax = GateMin + Gate->DY;
            DiffMin = 0;
            DiffMax = 0;

            Diff   = LYNX_LINK( Gate );

            while( Diff != Gate )
            {
             RdsLayer = GetRdsLayer( Diff );

              if ( RdsLayer == DiffLayer ) 
              {
                if ( Source == (rdsrec_list *)0 ) 
                {
                  DiffMin = Diff->X;
                  DiffMax = DiffMin + Diff->DX;
                  Source = Diff;
                }
                else
                if ( Drain == (rdsrec_list *)0 )
                {
                  Drain = Diff;
                }

                if ( DiffMin > Diff->X                ) DiffMin = Diff->X;
                if ( DiffMax < ( Diff->X + Diff->DX ) ) DiffMax = Diff->X + Diff->DX;
              } 
              else
              if ( RdsLayer == GateLayer )
              {
                if ( GateMin > Diff->Y                ) GateMin = Diff->Y;
                if ( GateMax < ( Diff->Y + Diff->DY ) ) GateMax = Diff->Y + Diff->DY;
              }
              else
              if ( RdsLayer == BulkLayer )
              {
                 Bulk = Diff;
              }
 
              Diff = LYNX_LINK( Diff );
            }
          }

          Length = GateMax - GateMin;
          Width  = DiffMax - DiffMin;

          if ( ( Source != (rdsrec_list *)0 ) &&
               ( Drain  != (rdsrec_list *)0 ) )
          {
            Ps = ( Source->DX << 1 ) + ( Source->DY << 1 );
            Pd = ( Drain->DX  << 1 ) + ( Drain->DY  << 1 );
            Xs = ( Source->DX * Source->DY ) / Width;
            Xd = ( Drain->DX  * Drain->DY  ) / Width;

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/


            sprintf( trname, "tr_%05d", tridx++ );
            {
              /* Modif Ludo : 4p is a b... */
              if ( Bulk != (rdsrec_list *)0 ) ptlbulk = getptype( Bulk->USER, RDSSIGNAL );
              else                            ptlbulk = (ptype_list *)0;
               
              xxx_trs=addlotrs
              (
                LogicalFigure,
                TransType,
                ( Gate->X + ( Gate->DX >> 1 ) ) * SCALE_X / RDS_UNIT,
                ( Gate->Y + ( Gate->DY >> 1 ) ) * SCALE_X / RDS_UNIT,
                Width  * SCALE_X / RDS_UNIT,
                Length * SCALE_X / RDS_UNIT,
                Ps     * SCALE_X / RDS_UNIT,
                Pd     * SCALE_X / RDS_UNIT,
                Xs     * SCALE_X / RDS_UNIT,
                Xd     * SCALE_X / RDS_UNIT,
                (losig_list*)(getptype( Gate->USER, RDSSIGNAL )->DATA),
                (losig_list*)(getptype( Source->USER, RDSSIGNAL )->DATA),
                (losig_list*)(getptype( Drain->USER, RDSSIGNAL )->DATA),
                ptlbulk ? (losig_list*)( ptlbulk->DATA ) : NULL , 
	        (Gate->NAME) ? Gate->NAME : namealloc(trname)
              );
            }
            
/* En attendant la rectification de mbk_lo.c */
            xxx_trs->DRAIN->USER=NULL;
            xxx_trs->GRID->USER=NULL;
            xxx_trs->SOURCE->USER=NULL;
            xxx_trs->DRAIN->NEXT=NULL;
            xxx_trs->GRID->NEXT=NULL;
            xxx_trs->SOURCE->NEXT=NULL;
            xxx_trs->BULK->USER=NULL;
            xxx_trs->BULK->NEXT=NULL;

/* Ajoute dans une ptype les rectangles connecteur */
            xxx_trs->DRAIN->USER=addptype(xxx_trs->DRAIN->USER,RDSLOCON3,addchain(NULL,Drain));
            xxx_trs->GRID->USER=addptype(xxx_trs->GRID->USER,RDSLOCON3,addchain(NULL,Gate));
            xxx_trs->SOURCE->USER=addptype(xxx_trs->SOURCE->USER,RDSLOCON3,addchain(NULL,Source));
            /* Modif Ludo : 4p is a b... */
            if ( Bulk != (rdsrec_list *)0 )
            {
              xxx_trs->BULK->USER=addptype(xxx_trs->BULK->USER,RDSLOCON3,addchain(NULL,Bulk));
            }
/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

            Counter = Counter + 1;
          }
          else
          {
            LynxError( LYNX_ERROR_TRANSISTOR_LOGICAL, 
                (char *)( ( Gate->X + ( Gate->DX >> 1 ) ) / RDS_UNIT ),
                (char *)( ( Gate->Y + ( Gate->DY >> 1 ) ) / RDS_UNIT ) );
          }
        }
      }
    }
  }

  rdsend();
  return( Counter );
}

/*------------------------------------------------------------\
|                                                             |
|                       LynxVectorizeConnector                |
|                                                             |
\------------------------------------------------------------*/

char *LynxVectorizeConnector( LogicalFigure )
 
   lofig_list *LogicalFigure;
 {
   loins_list *Instance;

   rdsbegin();

   if ( LogicalFigure->LOCON == (locon_list *)0 ) 
   {
     rdsend();
     return( LogicalFigure->NAME );
   }

   sortlocon( &LogicalFigure->LOCON );
   LogicalFigure->LOCON = (locon_list *)reverse( (chain_list *)LogicalFigure->LOCON );

   for ( Instance  = LogicalFigure->LOINS;
         Instance != (loins_list *)0;
         Instance  = Instance->NEXT )
   {
     if ( Instance->LOCON == (locon_list *)0 ) 
     {
       rdsend();
       return( Instance->INSNAME );
     }

     sortlocon( &Instance->LOCON );
     Instance->LOCON = (locon_list *)reverse( (chain_list *)Instance->LOCON );
   }

   rdsend();
   return( (char *)0 );
 }

float LynxCapaInfo( LogicalFigure )
  lofig_list *LogicalFigure;
{
  losig_list *scanlosig;
  float       cumulcapa;

  cumulcapa = 0.0;
  for( scanlosig = LogicalFigure->LOSIG ;
       scanlosig ;
       scanlosig = scanlosig->NEXT
     )
  {
    if( scanlosig->PRCN )
    {
      if( scanlosig->PRCN->PWIRE || scanlosig->PRCN->PCTC ) 
        rcncalccapa( scanlosig );
      cumulcapa+=scanlosig->PRCN->CAPA;
    }
  }

  return( cumulcapa );
}
