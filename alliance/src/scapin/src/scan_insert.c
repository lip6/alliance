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
| Tool    :                     SCAN                          |
|                                                             |
| File    :                  scan_insert.c                    |
|                                                             |
| Author  :                                                   |
|                                                             |
| Date    :                                                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "mlu.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "scan_param.h"
# include "scan_path.h"
# include "scan_insert.h"

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

  static char Buffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       ScanRemoveSignal                      |
|                                                             |
\------------------------------------------------------------*/

static void ScanRemoveSignal( LoFigure, LoSig, FlagVerbose  )

  lofig_list *LoFigure;
  losig_list *LoSig;
  int         FlagVerbose;
{
  ptype_list  *PType;
  chain_list  *ScanChain;
  chain_list  *DelChain;
  chain_list **PrevChain;
  char        *SigName;
  locon_list  *LoCon;
  loins_list  *LoIns;
  locon_list  *LoInsCon;
  losig_list  *LoInsSig;
  int          Delete;
  int          NumberOut;

  PType     = getptype( LoSig->USER, (long)LOFIGCHAIN );
  ScanChain = (chain_list *)PType->DATA;

  if ( ScanChain == (chain_list *)0 )
  {
/*
** Zero connector on this net : should delete the signal
*/
    if ( FlagVerbose )
    {
      if ( LoSig->NAMECHAIN != (chain_list *)0 ) SigName = (char *)LoSig->NAMECHAIN->DATA; 
      else                                       SigName = "unamed";
    
      fprintf( stdout, "\t    > Delete signal %s (%ld)\n", SigName, LoSig->INDEX );
    }

    dellosig( LoFigure, LoSig->INDEX );
  }
  else
  if ( ScanChain->NEXT == (chain_list *)0 )
  {
/*
** Only one connector on this net
*/
    LoCon = (locon_list *)ScanChain->DATA;

    if ( LoCon->TYPE == INTERNAL )
    {
/*
** Internal connector : may be should delete the instance
*/
      LoIns = (loins_list *)LoCon->ROOT; 
      NumberOut = 0;
      Delete    = 1;

      for ( LoInsCon  = LoIns->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        if ( ( LoInsCon->DIRECTION == OUT   ) ||
             ( LoInsCon->DIRECTION == INOUT ) )
        {
          NumberOut++;

          if ( ( LoInsCon->SIG != LoSig ) ||
               ( NumberOut     >  1     ) )
          {
            Delete = 0; break;
          }
        }
      }

      if ( Delete )
      {
        for ( LoInsCon  = LoIns->LOCON;
              LoInsCon != (locon_list *)0;
              LoInsCon  = LoInsCon->NEXT )
        {
          LoInsSig = LoInsCon->SIG;
 
          PType     = getptype( LoInsSig->USER, (long)LOFIGCHAIN );
          ScanChain = (chain_list  *)PType->DATA;
          PrevChain = (chain_list **)&PType->DATA;

          while ( ScanChain != (chain_list *)0 )
          {
            if ( ScanChain->DATA == (void *)LoInsCon )
            {
              DelChain   = ScanChain;
              *PrevChain = DelChain->NEXT;
              ScanChain  = ScanChain->NEXT;

              DelChain->NEXT = (chain_list *)0;
              freechain( DelChain );
            }
            else
            {
              PrevChain = &ScanChain->NEXT;
              ScanChain = ScanChain->NEXT;
            }
          }

          if ( LoInsSig == LoSig ) continue;

          ScanRemoveSignal( LoFigure, LoInsSig, FlagVerbose );
        }

        if ( FlagVerbose )
        {
          if ( LoSig->NAMECHAIN != (chain_list *)0 ) SigName = (char *)LoSig->NAMECHAIN->DATA; 
          else                                       SigName = "unamed";

          fprintf( stdout, "\t    > Delete signal %s (%ld)\n", SigName, LoSig->INDEX );
        }

        dellosig( LoFigure, LoSig->INDEX );

        if ( FlagVerbose )
        {
          fprintf( stdout, "\t    > Delete instance %s (%s)\n",
                   LoIns->INSNAME, LoIns->FIGNAME );
        }

        delloins( LoFigure, LoIns->INSNAME );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       ScanInsertScanPath                    |
|                                                             |
\------------------------------------------------------------*/

void ScanInsertScanPath( LoFigure, ScanParam, PathParam, 
                         FlagVerbose, FlagDebug, FlagRegMux, FlagBuffer )

  lofig_list *LoFigure;
  scanparam  *ScanParam;
  pathparam  *PathParam;
  int         FlagVerbose;
  int         FlagDebug;
  int         FlagRegMux;
  int         FlagBuffer;
{
  lofig_list      *LoModel;
  lofig_list      *LoModel2;
  lofig_list      *LoModel3;
  lofig_list      *LoModel4;
  loins_list      *LoIns;
  chain_list      *ScanChain;
  char            *RegName;
  char            *InsName;
  losig_list      *LoSig;
  losig_list      *LoSigMuxOutput;
  losig_list      *LastLoSig;
  losig_list      *LoSigScanIn;
  losig_list      *LoSigScanTest;
  losig_list      *LoSigScanOut;
  losig_list      *LoSigScanBuf;
  locon_list      *LoCon;
  locon_list      *LoConScanIn;
  locon_list      *LoConScanOut;
  locon_list      *LoConScanTest;
  locon_list      *LoConVdd;
  locon_list      *LoConVss;
  locon_list      *LoInsCon;
  locon_list      *LoInsConClk;
  locon_list      *LoInsConSel;
  locon_list      *LoInsConInput;
  locon_list      *LoInsConOutput;
  locon_list      *LoInsConVdd;
  locon_list      *LoInsConVss;
  locon_list      *LoInsConInputSel;
  locon_list      *LoInsConInputNsel;
  scanreg_list    *ScanReg;
  scanregmux_list *ScanRegMux;
  scanmux_list    *ScanMux;
  scanmux_list    *ScanMux1;
  scanbuf_list    *ScanBuf;
  authtable       *InsHashTable;
  authtable       *RegHashTable;
  authtable       *RegMuxHashTable;
  authtable       *MuxHashTable;
  authelem        *Element;
  chain_list      *SignalList;
  long             LoSigIndex;
  long             LoInsIndex;
  int              Number;
  locon_list       *LoInsConOutputMux;
  losig_list       *LoSigOutputMux;
  
/*
** Compute and Initialize max signal index
*/
  LoInsIndex = 0;
  LoSigIndex = 0;

  for ( LoSig  = LoFigure->LOSIG;
        LoSig != (losig_list *)0;
        LoSig  = LoSig->NEXT )
  {
    if ( LoSig->INDEX > LoSigIndex ) LoSigIndex = LoSig->INDEX;
  }

  LoSigIndex++;
/*
** Inserer eventuellement les connecteurs ScanIn/ScanOut/ScanTest en fonction de PathParam
*/
  LoConScanIn   = (locon_list *)0;
  LoConScanOut  = (locon_list *)0;
  LoConScanTest = (locon_list *)0;
  LoConVdd      = (locon_list *)0;
  LoConVss      = (locon_list *)0;

  for ( LoCon  = LoFigure->LOCON;
        LoCon != (locon_list*)0;
        LoCon  = LoCon->NEXT )
  {    
    if ( LoCon->NAME == PathParam->SCAN_IN   ) LoConScanIn = LoCon;
    else
    if ( LoCon->NAME == PathParam->SCAN_OUT  ) LoConScanOut = LoCon;
    else
    if ( LoCon->NAME == PathParam->SCAN_TEST ) LoConScanTest = LoCon;
    else
    if ( isvdd( LoCon->NAME ) ) LoConVdd = LoCon;
    else
    if ( isvss( LoCon->NAME ) ) LoConVss = LoCon;
  }

  if ( LoConVdd == (locon_list *)0 )
  {
    fprintf( stderr, "\t### Error unable to find connector VDD in figure %s\n",
             LoFigure->NAME );
    autexit( 1 );
  }

  if ( LoConVss == (locon_list *)0 )
  {
    fprintf( stderr, "\t### Error unable to find connector VSS in figure %s\n",
             LoFigure->NAME );
    autexit( 1 );
  }

  if ( LoConScanIn == (locon_list *)0 )
  {
    LoSigScanIn = addlosig( LoFigure, LoSigIndex++, (chain_list *)0, EXTERNAL );
    LoConScanIn = addlocon( LoFigure, PathParam->SCAN_IN, LoSigScanIn, IN );
  }
  else
  {
    LoSigScanIn = LoConScanIn->SIG;
  }

  if ( LoConScanTest == (locon_list *)0 )
  {
    LoSigScanTest = addlosig( LoFigure, LoSigIndex++, (chain_list *)0, EXTERNAL );
    LoConScanTest = addlocon( LoFigure, PathParam->SCAN_TEST, LoSigScanTest, IN );
  }
  else
  {
    LoSigScanTest = LoConScanTest->SIG;
  }

/*
** Create all usefull hash tables (Instance/Reg/Reg Mux etc)
*/
  RegHashTable = createauthtable( 100 );

  for  ( ScanReg  = ScanParam->REG_LIST;
         ScanReg != (scanreg_list *)0;
         ScanReg  = ScanReg->NEXT )
  {
    addauthelem( RegHashTable, ScanReg->MODEL, (long)ScanReg );
  }

  MuxHashTable = createauthtable( 100 );

  for  ( ScanMux  = ScanParam->MUX_LIST;
         ScanMux != (scanmux_list *)0;
         ScanMux  = ScanMux->NEXT )
  {
    addauthelem( MuxHashTable, ScanMux->MODEL, (long)ScanMux );
  }

  RegMuxHashTable = createauthtable( 100 );

  for  ( ScanRegMux  = ScanParam->REG_MUX_LIST;
         ScanRegMux != (scanregmux_list *)0;
         ScanRegMux  = ScanRegMux->NEXT )
  {
    addauthelem( RegMuxHashTable, ScanRegMux->MODEL, (long)ScanRegMux );
  }

  InsHashTable = createauthtable( 1000 );

  for ( LoIns  = LoFigure->LOINS;
        LoIns != (loins_list *)0;
        LoIns  = LoIns->NEXT )
  {
    addauthelem( InsHashTable, LoIns->INSNAME, (long)LoIns );
  }

  if ( FlagDebug )
  {
    fprintf( stdout, "Identify all REG-MUX\n" );
  }
/*
** Identifier dans les LOINS les differents registres-mux de PathParam
*/
  for ( ScanChain  = PathParam->REG_LIST;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    RegName = (char *)ScanChain->DATA;
/*
** On recherche le registre RegName
*/
    Element = searchauthelem( InsHashTable, RegName );

    if ( Element == (authelem *)0 )
    {
      fprintf( stderr, "\t### Error unable to find instance %s\n", RegName );
      autexit( 1 );
    }
    
    LoIns = (loins_list *)Element->VALUE;
/*
** Check register model type : reg_mux or simple reg ?
*/
    Element = searchauthelem( RegMuxHashTable, LoIns->FIGNAME );

    if ( Element != (authelem *)0 )
    {
/*
** Transformation du REG-MUX en MUX + REG
*/
      ScanRegMux = (scanregmux_list *)Element->VALUE;

      if ( FlagVerbose )
      {
        fprintf( stdout, "\t    > Replace register %s (%s) by %s + %s models\n",
                 RegName, LoIns->FIGNAME, ScanRegMux->MUX, ScanRegMux->REG  );
      }

      Number = 0;
     
      for ( LoInsCon  = LoIns->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        if ( LoInsCon->NAME == ScanRegMux->CLK    )      LoInsConClk    = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanRegMux->INPUT_NSEL  ) LoInsConInputNsel  = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanRegMux->INPUT_SEL  )  LoInsConInputSel  = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanRegMux->SEL  )        LoInsConSel  = LoInsCon;    
        else
        if ( LoInsCon->NAME == ScanRegMux->OUTPUT )      LoInsConOutput = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanRegMux->VDD    )      LoInsConVdd    = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanRegMux->VSS    )      LoInsConVss    = LoInsCon;
        else
        {
           fprintf( stderr, "\t### Error unknown port name %s in instance %s\n",
                     LoInsCon->NAME, LoIns->INSNAME );
           autexit( 1 );
        }
                               
        Number++;
      }
         
      if ( Number != 7 )
      {
        fprintf( stderr, "\t### Error missing %d port(s) in instance %s\n",
                 7 - Number, LoIns->INSNAME );
        autexit( 1 );
      }
/*
** Creation du multiplexeur
*/
      Element = searchauthelem( MuxHashTable, ScanRegMux->MUX );
      ScanMux = (scanmux_list *)Element->VALUE;
      
      LoModel = getlofig( ScanMux->MODEL, 'P' );

      LoSigMuxOutput = addlosig( LoFigure, LoSigIndex++, (chain_list *)0, INTERNAL );

      SignalList = (chain_list *)0;
     
      for ( LoInsCon  = LoModel->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        if ( LoInsCon->NAME == ScanMux->INPUT_NSEL ) LoSig = LoInsConInputNsel->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->INPUT_SEL  ) LoSig = LoInsConInputSel->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->SEL        ) LoSig = LoInsConSel->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->VDD        ) LoSig = LoInsConVdd->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->VSS        ) LoSig = LoInsConVss->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->OUTPUT     ) LoSig = LoSigMuxOutput;
        else
        {
          fprintf( stderr, "\t### Error unknown port name %s in model %s\n",
                       LoInsCon->NAME, LoModel->NAME );
                autexit( 1 );
        }

        SignalList = addchain( SignalList, (void *)LoSig );
      }
          
      SignalList = reverse( SignalList );
                
      sprintf( Buffer, "%s_scan_%ld", LoIns->INSNAME, LoInsIndex++ );
      addloins( LoFigure, Buffer, LoModel, SignalList );
/*
** Creation du registre
*/
      Element = searchauthelem( RegHashTable, ScanRegMux->REG );
      ScanReg = (scanreg_list *)Element->VALUE;
      
      LoModel = getlofig( ScanReg->MODEL, 'P' );

      SignalList = (chain_list *)0;

      for ( LoInsCon  = LoModel->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        if ( LoInsCon->NAME == ScanReg->CLK    ) LoSig = LoInsConClk->SIG;
        else
        if ( LoInsCon->NAME == ScanReg->INPUT  ) LoSig = LoSigMuxOutput;
        else
        if ( LoInsCon->NAME == ScanReg->OUTPUT ) LoSig = LoInsConOutput->SIG;
        else
        if ( LoInsCon->NAME == ScanReg->VDD    ) LoSig = LoInsConVdd->SIG;
        else
        if ( LoInsCon->NAME == ScanReg->VSS    ) LoSig = LoInsConVss->SIG;
        else
        {
          fprintf( stderr, "\t### Error unknown port name %s in model %s\n",
                       LoInsCon->NAME, LoModel->NAME );
                autexit( 1 );
        }

        SignalList = addchain( SignalList, (void *)LoSig );
      }
          
      SignalList = reverse( SignalList );
      InsName    = LoIns->INSNAME;
/*
** Destruction du registre-mux
*/
      for ( LoInsCon  = LoIns->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        LoInsCon->SIG = (losig_list *)0;
      }

      delloins( LoFigure, InsName );
      LoIns = addloins( LoFigure, InsName, LoModel, SignalList );

      addauthelem( InsHashTable, LoIns->INSNAME, (long)LoIns );
    }
  }

/*
** Identifier dans les LOINS les differents registres a partir de PathParam
** Creation du chemin de test !
*/
  if ( FlagDebug )
  {
    fprintf( stdout, "Scan Path generation\n" );
  }

  LastLoSig = LoConScanIn->SIG;

  for ( ScanChain  = PathParam->REG_LIST;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    RegName = (char *)ScanChain->DATA;

    if ( FlagDebug )
    {
      fprintf( stdout, "Search %s\n", RegName );
    }
/*
** On recherche le registre RegName
*/
    Element = searchauthelem( InsHashTable, RegName );

    if ( Element == (authelem *)0 )
    {
      fprintf( stderr, "\t### Error unable to find instance %s\n", RegName );
      autexit( 1 );
    }
    
    LoIns = (loins_list *)Element->VALUE;
/*
** Check register model type : reg_mux or simple reg ?
*/
    Element = searchauthelem( RegHashTable, LoIns->FIGNAME );

    if ( Element == (authelem *)0 )
    {
      fprintf( stderr, "\t### Error unknown register model %s in figure %s\n", 
                 LoIns->FIGNAME, LoFigure->NAME );
      autexit( 1 );
    }

    ScanReg = (scanreg_list *)Element->VALUE;
/*
** Ajout d'un mux en entree du registre
*/
/*______________________________________________________________________________*/
/*           cas ou on a un registre et on veut relier avec un mux              */
/*_________________________________________________________________________________*/
    if ( ! FlagRegMux )
    {
      Element = searchauthelem( MuxHashTable, ScanReg->MUX );
      ScanMux = (scanmux_list *)Element->VALUE;
      Number  = 0;

      if ( FlagVerbose )
      {
        fprintf( stdout, "\t    Insert multiplexor (%s) in front of register %s (%s)\n",
                 ScanMux->MODEL, RegName, LoIns->FIGNAME );
      }
  
/*-----------------------------------------------------------------------------*/
/*             on donne des noms pour chaque connecteur du registre    */
/*-----------------------------------------------------------------------------*/
  
      for ( LoInsCon  = LoIns->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        if ( LoInsCon->NAME == ScanReg->CLK    ) LoInsConClk    = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanReg->INPUT  ) LoInsConInput  = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanReg->OUTPUT ) LoInsConOutput = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanReg->VDD    ) LoInsConVdd    = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanReg->VSS    ) LoInsConVss    = LoInsCon;
        else
        {
          fprintf( stderr, "\t### Error unknown port name %s in instance %s\n",
                   LoInsCon->NAME,LoIns->INSNAME);
           autexit( 1 );
        }

        Number++;
      }
    
      if ( Number != 5 )
      {
        fprintf( stderr, "\t### Error missing %d port(s) in instance %s\n",
                                 5 - Number, LoIns->INSNAME );
        autexit( 1 );
      }
 
      LoModel = getlofig( ScanMux->MODEL, 'P' );
/*
** Build the signal list
*/

/*------------------------------------------------------------------------------------------*/
/*  on relie tous les connecteurs du mux  avec les signaux  correspondant sauf */
/*  le connecteur de sortie  */
 /*-----------------------------------------------------------------------------------------*/    
/*
** Creation du signal de sortie du mux : LoSigMuxOutput
*/
      LoSigMuxOutput = addlosig( LoFigure, LoSigIndex++, (chain_list *)0, INTERNAL );

      SignalList = (chain_list *)0;
 
      for ( LoInsCon  = LoModel->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        if ( LoInsCon->NAME == ScanMux->INPUT_NSEL ) LoSig = LoInsConInput->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->INPUT_SEL  ) LoSig = LastLoSig;
        else
        if ( LoInsCon->NAME == ScanMux->SEL        ) LoSig = LoConScanTest->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->VDD        ) LoSig = LoInsConVdd->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->VSS        ) LoSig = LoInsConVss->SIG;
        else
        if ( LoInsCon->NAME == ScanMux->OUTPUT     ) LoSig = LoSigMuxOutput;
        else
        {  
          fprintf( stderr, "\t### Error unknown port name %s in model %s\n",
                   LoInsCon->NAME, LoModel->NAME );
          autexit( 1 );
        }

        SignalList = addchain( SignalList, (void *)LoSig );
      }

      SignalList = reverse( SignalList );

      sprintf( Buffer, "%s_scan_%ld", LoIns->INSNAME, LoInsIndex++ );
      addloins( LoFigure, Buffer, LoModel, SignalList );
/*
** Connection : signal de sortie du mux -> connecteur d'entree du registre
*/
      LoInsConInput->SIG = LoSigMuxOutput;

      freechain( LoInsConInput->SIG->NAMECHAIN );
      LoInsConInput->SIG->NAMECHAIN = (chain_list *)0;
           
      LastLoSig = LoInsConOutput->SIG;
    }
    else
    {
/*
** Substitution d'un registre par un reg-mux
*/
      Element    = searchauthelem( RegMuxHashTable, ScanReg->REG_MUX );
      ScanRegMux = (scanregmux_list *)Element->VALUE;

      if ( FlagVerbose )
      {
        fprintf( stdout, "\t    > Replace register %s (%s) by a reg-mux (%s)\n",
                 RegName, LoIns->FIGNAME, ScanRegMux->MODEL );
      }

      Number = 0;

      for ( LoInsCon  = LoIns->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        if ( LoInsCon->NAME == ScanReg->CLK    ) LoInsConClk    = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanReg->INPUT  ) LoInsConInput  = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanReg->OUTPUT ) LoInsConOutput = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanReg->VDD    ) LoInsConVdd    = LoInsCon;
        else
        if ( LoInsCon->NAME == ScanReg->VSS    ) LoInsConVss    = LoInsCon;
        else
        {
          fprintf( stderr, "\t### Error unknown port name %s in instance %s\n",
                   LoInsCon->NAME, LoIns->INSNAME );
          autexit( 1 );
        }

         Number++;
      }

      if ( Number != 5 )
      {
        fprintf( stderr, "\t### Error missing %d port(s) in instance %s\n",
                   5 - Number, LoIns->INSNAME );
        autexit( 1 );
      }

      LoModel = getlofig( ScanRegMux->MODEL, 'P' );
/*
** Build the signal list
*/
      SignalList = (chain_list *)0;

      for ( LoInsCon  = LoModel->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        if ( LoInsCon->NAME == ScanRegMux->CLK        ) LoSig = LoInsConClk->SIG;
        else
        if ( LoInsCon->NAME == ScanRegMux->INPUT_NSEL ) LoSig = LoInsConInput->SIG;
        else
        if ( LoInsCon->NAME == ScanRegMux->INPUT_SEL  ) LoSig = LastLoSig;
        else
        if ( LoInsCon->NAME == ScanRegMux->SEL        ) LoSig = LoConScanTest->SIG;
        else
        if ( LoInsCon->NAME == ScanRegMux->VDD        ) LoSig = LoInsConVdd->SIG;
        else
        if ( LoInsCon->NAME == ScanRegMux->VSS        ) LoSig = LoInsConVss->SIG;
        else
        if ( LoInsCon->NAME == ScanRegMux->OUTPUT     ) LoSig = LoInsConOutput->SIG;
        else
        {
          fprintf( stderr, "\t### Error unknown port name %s in model %s\n",
                   LoInsCon->NAME, LoModel->NAME );
          autexit( 1 );
        }

        SignalList = addchain( SignalList, (void *)LoSig ); 
      }

      SignalList = reverse( SignalList );

      sprintf( Buffer, "%s_scan_%ld", LoIns->INSNAME, LoInsIndex++ );
      addloins( LoFigure, Buffer, LoModel, SignalList );

      LastLoSig = LoInsConOutput->SIG;

      for ( LoInsCon  = LoIns->LOCON;
            LoInsCon != (locon_list *)0;
            LoInsCon  = LoInsCon->NEXT )
      {
        LoInsCon->SIG = (losig_list *)0;
      }

      delloins( LoFigure, LoIns->INSNAME );
    }
  }
  
/*
** ajout d'un buffer
*/
  if ( FlagBuffer )
  {
    ScanBuf = ScanParam->BUF_LIST;

    if ( FlagVerbose )
    {
      fprintf( stdout, "\t    Insert a buffer (%s) before the port %s\n",
               ScanBuf->MODEL, PathParam->SCAN_OUT );
    }

    LoModel = getlofig( ScanBuf->MODEL, 'P' );

    LoSigScanBuf = addlosig( LoFigure, LoSigIndex++, (chain_list *)0, EXTERNAL );
    SignalList   = (chain_list *)0;

    for ( LoInsCon  = LoModel->LOCON;
          LoInsCon != (locon_list *)0;
          LoInsCon  = LoInsCon->NEXT )
    {
      if ( LoInsCon->NAME == ScanBuf->INPUT ) LoSig = LastLoSig ;
      else
      if ( LoInsCon->NAME ==ScanBuf->VDD ) LoSig =LoConVdd->SIG;
      else
      if ( LoInsCon->NAME == ScanBuf->VSS) LoSig = LoConVss->SIG;
      else
      if ( LoInsCon->NAME == ScanBuf->OUTPUT) LoSig = LoSigScanBuf  ;
      else
      {
        fprintf( stderr, "\t### Error unknown port name %s in model %s\n",
                 LoInsCon->NAME, LoModel->NAME );
        autexit( 1 );
      }

      SignalList = addchain( SignalList, (void *)LoSig ); 
    }

    SignalList = reverse( SignalList );

    sprintf( Buffer, "buf_scan_%ld", LoInsIndex++ );
    addloins( LoFigure, Buffer, LoModel, SignalList );

    LastLoSig = LoSigScanBuf;
  }

  if ( LoConScanOut == (locon_list *)0 )
  {
    LastLoSig->TYPE = EXTERNAL;
    LoConScanOut = addlocon( LoFigure, PathParam->SCAN_OUT, LastLoSig,
                             FlagBuffer ? OUT : INOUT );
  }
  else
  {
    LoSigScanOut = LoConScanOut->SIG;
    LoSigScanOut->TYPE = INTERNAL;
    freechain( LoSigScanOut->NAMECHAIN );
    LoSigScanOut->NAMECHAIN = (chain_list *)0;

    LoConScanOut->SIG =  LastLoSig;
    LastLoSig->TYPE   = EXTERNAL;

    LoConScanOut->DIRECTION = FlagBuffer ?  OUT : INOUT;

    lofigchain( LoFigure );

    if ( FlagVerbose )
    {
      fprintf( stdout, "\t--> Port %s already exists, should clean the netlist\n",
               PathParam->SCAN_OUT );
    }

    ScanRemoveSignal( LoFigure, LoSigScanOut, FlagVerbose );
  }

  destroyauthtable( InsHashTable    );
  destroyauthtable( RegHashTable    );
  destroyauthtable( MuxHashTable    );
  destroyauthtable( RegMuxHashTable );

  if ( FlagDebug )
  {
    fprintf( stdout, "<-- ScanInsertScanPath\n" );
  }
}
