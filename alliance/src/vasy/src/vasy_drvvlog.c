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
| Tool    :                     VASY                          |
|                                                             |
| File    :                 vasy_drvvlog.c                    |
|                                                             |
| Authors :                Frédéric Pétrot                    |
|           This file is a basic modification of vasy_drvsyn  |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "rtn.h"

# include "vasy_debug.h"
# include "vasy_error.h"
# include "vasy_simprtl.h"
# include "vasy_shared.h"
# include "vasy_drvvex.h"
# include "vasy_drvvlog.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/

  static FILE      *VasyFile = (FILE *)NULL;

  static char *VERILOG_DIR_TYPE[ RTL_MAX_DIR_TYPE ] =
  {
    "input",
    "output",
    "inout"
  };

  static long VasyVerilogDef = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Ln2p                              |
|                                                             |
\------------------------------------------------------------*/

/* Ln2p: computes ln2(n) by excess : for example ln2(512) = 9, ln2(513) = 10 */ 

static long Ln2p( Value )

  long Value;
{  
  long i = 0L, j = ( Value & 1 ) ? 1L : 0L;
   
  if ( Value )
  {
    for ( i = -1;
          Value > 0L;
          Value >>= 1 )
    {
      if ( Value & 1L )
      {
         j++;
      }
      i++;
    }
  }
  return i + ( j > 1L ? 1L : 0L );
}

/*------------------------------------------------------------\
|                                                             |
|                         VasyVerilogBinary                   |
|                                                             |
\------------------------------------------------------------*/

static char *VasyVerilogBinary ( Range, Value )
  
  long Range;
  long Value;
{
  static char Buffer[ 256 ];
  int Index;
  int Shift;
  
  sprintf( Buffer, "%ld'b", Range );
  
  /* Do not use the system depend result of sprintf */
  Shift = strlen( Buffer );
  
  for ( Index = Range - 1;
        Index >= 0;
        Index -- )
  {
    Buffer[ Shift + Range - 1 - Index ] = (( Value << Index ) & 1) + '0' ;
  }
  
  Buffer[ Shift + Range ] = (char)0;

  return Buffer;
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyVerilogBinaryZXU                    |
|                                                             |
\------------------------------------------------------------*/

static char *VasyVerilogBinaryZXU ( Range, Zxu )
  
  long Range;
  char Zxu;
{
  static char Buffer[ 256 ];
  int Index;
  int Shift;
  
  sprintf( Buffer, "%ld'b", Range );
  
  /* Do not use the system depend result of sprintf */
  Shift = strlen( Buffer );
  
  for ( Index = Range - 1;
        Index >= 0;
        Index -- )
  {
    Buffer[ Shift + Range - 1 - Index ] = Zxu ;
  }

  Buffer[ Shift + Range ] = (char)0;

  return Buffer;
}

static char *VasyGetVerilogNetType( RtlDeclar )
  
  rtldecl_list *RtlDeclar;
{
  char *NetType;

  if ( IsVasyRtlDeclVerilogReg( RtlDeclar ) ) NetType = "reg";
  else                                        NetType = "wire";

  return( NetType );
}

/*------------------------------------------------------------\
|                                                             |
|               VasyDriveVerilogAddRtlDefine                  |
|                                                             |
\------------------------------------------------------------*/

vexexpr *VasyDriveVerilogAddRtlDefine( RtlFigure, VexCond )

  rtlfig_list  *RtlFigure;
  vexexpr      *VexCond;
{
  rtldecl_list *DefDeclar;
  rtlasg_list  *DefAssign;
  vexexpr      *VexAtom;
  char          Buffer[ 32 ];

  sprintf( Buffer, "rtlvlog_%ld", VasyVerilogDef++ );
  VexAtom = createvexatombit( Buffer );

  DefDeclar = addrtldecl( RtlFigure, VexAtom, RTL_DECLAR_SIGNAL );
  SetVasyRtlDeclarRead( DefDeclar );
  SetVasyRtlDeclarAsg( DefDeclar );

  DefDeclar->BASE = VEX_TYPE_BOOLEAN;
  DefDeclar->KIND = RTL_KIND_NONE;
  DefDeclar->DIR  = RTL_DIR_INOUT;

  DefAssign = addrtlasg( RtlFigure, dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
  DefAssign->VEX_DATA = VexCond;

  return( dupvexexpr( VexAtom ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyVerilogTreatMap                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyVerilogTreatMap( RtlFigure, RtlInst )

  rtlfig_list *RtlFigure;
  rtlins_list *RtlInst;
{
  rtlmap_list  *RtlMap;
  rtlport_list *RtlPort;
  rtldecl_list *RtlDeclar;
  rtlasg_list  *RtlAssign;
  vexexpr      *VexFormal;
  vexexpr      *VexActual;
  vexexpr      *VexAtom;
  char         *AtomValue;
  char          Buffer[ 64 ];
  int           Type;

  for ( RtlMap  = RtlInst->MAP;
        RtlMap != (rtlmap_list *)0;
        RtlMap  = RtlMap->NEXT )
  {
    VexFormal = RtlMap->VEX_FORMAL;
    AtomValue = GetVexAtomValue( VexFormal ); 
    RtlPort   = searchrtlmodport( RtlFigure, RtlInst->MODEL, AtomValue );
    VexAtom   = RtlPort->VEX_ATOM;
  
    VexActual = RtlMap->VEX_ACTUAL;
  
    if ( ( ! IsVexNodeAtom( VexActual )    ) ||
         (   IsVexAtomLiteral( VexActual ) ) )
    {
      sprintf( Buffer, "rtl_map_%ld", VasyVerilogDef++ );
    
      if ( isvextypevector( Type ) )
      {
        VexAtom = createvexatomvec( Buffer, VexActual->WIDTH - 1, 0 );
      }
      else
      {
        VexAtom = createvexatombit( Buffer );
      }
    
      RtlDeclar = addrtldecl( RtlFigure, dupvexexpr( VexAtom ), RTL_DECLAR_SIGNAL );
      RtlDeclar->BASE = RtlPort->BASE;
      RtlDeclar->KIND = RTL_KIND_NONE;
      RtlDeclar->DIR  = RTL_DIR_INOUT;
    
      SetVasyRtlDeclarRead( RtlDeclar );
      SetVasyRtlDeclarAsg( RtlDeclar );
  
      RtlAssign = addrtlasg( RtlFigure, dupvexexpr( VexAtom ), RTL_ASG_COMBINATORIAL );
      RtlAssign->VEX_DATA = VexActual;
  
      VexActual = VexAtom;
    }

    RtlMap->VEX_ACTUAL = VexActual;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyVerilogTreatAsg                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyVerilogTreatAsg( RtlFigure, RtlAsg )

  rtlfig_list *RtlFigure;
  rtlasg_list *RtlAsg;
{
  rtldecl_list  *RtlDeclar;
  rtlbivex_list *RtlBiVex;
  chain_list    *AllName;
  chain_list    *ScanChain;
  ptype_list    *SensList;
  char          *SensName;
  vexexpr       *VexAtom;
  vexexpr       *VexCond;
  vexexpr       *Operand;
  long           Oper;
  long           SensType;
  int            RtlType;

  SensType = 0;

  VexAtom   = RtlAsg->VEX_ATOM;
  RtlDeclar = searchrtldecl( RtlFigure, GetVexAtomValue( VexAtom ) );
/*
** Compute the sensisitivity list for each register assignment
*/
  if ( RtlAsg->TYPE == RTL_ASG_REGISTER )
  {
    SetVasyRtlDeclVerilogReg( RtlDeclar ); 

    SensList = (ptype_list *)0;

    if ( RtlAsg->REG_TYPE == RTL_ASG_REGISTER_MIXED )
    {
      for ( RtlBiVex  = RtlAsg->BIVEX;
            RtlBiVex != (rtlbivex_list *)0;
            RtlBiVex  = RtlBiVex->NEXT )
      {
        RtlType  = RtlBiVex->TYPE;
        VexCond  = RtlBiVex->VEX_COND;
        SensName = (char *)0;
/*
** Special treatments for register's asynchronous conditions
*/
        if ( ( RtlType == RTL_BIVEX_ASYNC_SET   ) ||
             ( RtlType == RTL_BIVEX_ASYNC_RESET ) )
        {
          if ( ! IsVexNodeAtom( VexCond ) )
          {
            if ( IsVexNodeOper( VexCond ) )
            {
              Oper    = GetVexOperValue( VexCond );
              Operand = GetVexOperand( VexCond->OPERAND );

              if ( ( Oper == VEX_NOT          ) &&
                   ( IsVexNodeAtom( Operand ) ) )
              {
                SensType = VASY_VERILOG_SENS_NEGEDGE;
                SensName = GetVexAtomValue( Operand );
              }
            }

            if ( SensName == (char *)0 )
            {
              VexCond = VasyDriveVerilogAddRtlDefine( RtlFigure, VexCond );
              RtlBiVex->VEX_COND = VexCond;

              SensType = VASY_VERILOG_SENS_POSEDGE;
              SensName = GetVexAtomValue( VexCond );
            }
          }
          else
          {
            SensType = VASY_VERILOG_SENS_POSEDGE;
            SensName = GetVexAtomValue( VexCond );
          }

          SensList = addptype( SensList, SensType, SensName );
        }
        else
        if ( RtlType == RTL_BIVEX_ASYNC_WEN )
        {
          VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "Driver Verilog: ASYNC WEN" );
/*
** Should be transformed to an equivalent Set/Reset condition TO BE DONE
*/
          VasyPrintf( stdout, "Must be transformed to an equivalent Set/Reset condition\n" );
        }
        else
        if ( ( RtlType == RTL_BIVEX_RISING_EDGE  ) ||
             ( RtlType == RTL_BIVEX_FALLING_EDGE ) )
        {
          if ( RtlType == RTL_BIVEX_FALLING_EDGE ) SensType = VASY_VERILOG_SENS_NEGEDGE;
          else                                     SensType = VASY_VERILOG_SENS_POSEDGE;

          SensName = GetVexAtomValue( VexCond );
          SensList = addptype( SensList, SensType, SensName );
        }
        else break;
      }
    }
    else
    {
      AllName  = (chain_list *)0;

      for ( RtlBiVex  = RtlAsg->BIVEX;
            RtlBiVex != (rtlbivex_list *)0;
            RtlBiVex  = RtlBiVex->NEXT )
      {
        RtlType = RtlBiVex->TYPE;

        if ( ( RtlType == RTL_BIVEX_ASYNC_RESET  ) ||
             ( RtlType == RTL_BIVEX_ASYNC_SET    ) ||
             ( RtlType == RTL_BIVEX_ASYNC_WEN    ) )
        {
          AllName = unionvexexprallname( AllName, RtlBiVex->VEX_COND );
          AllName = unionvexexprallname( AllName, RtlBiVex->VEX_DATA );
        }
        else
        if ( ( RtlType == RTL_BIVEX_RISING_EDGE  ) ||
             ( RtlType == RTL_BIVEX_FALLING_EDGE ) )
        {
          VexCond  = RtlBiVex->VEX_COND;

          SensName = GetVexAtomValue( VexCond );

          if ( RtlType == RTL_BIVEX_FALLING_EDGE ) SensType = VASY_VERILOG_SENS_NEGEDGE;
          else                                     SensType = VASY_VERILOG_SENS_POSEDGE;

          SensList = addptype( SensList, SensType, SensName );
        }
      }

      for ( ScanChain  = AllName;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        SensList = addptype( SensList, VASY_VERILOG_SENS_NOEDGE, ScanChain->DATA );
      }

      freechain( AllName );
    }

    SensList = (ptype_list *)reverse( (chain_list *)SensList );
    SetVasyRtlAsgVerilogSens( RtlAsg, SensList );
  }
  else
/*
** Compute the sensisitivity list for each tristate assignment
*/
  if ( ( RtlAsg->TYPE == RTL_ASG_TRISTATE    ) ||
       ( RtlAsg->TYPE == RTL_ASG_PULL_UP     ) ||
       ( RtlAsg->TYPE == RTL_ASG_PULL_DOWN   ) ||
       ( RtlAsg->TYPE == RTL_ASG_CONDITIONAL ) ||
       ( RtlAsg->TYPE == RTL_ASG_MULTIPLEXOR ) )
  {
    SetVasyRtlDeclVerilogReg( RtlDeclar ); 

    SensList = (ptype_list *)0;
    AllName  = (chain_list *)0;

    for ( RtlBiVex  = RtlAsg->BIVEX;
          RtlBiVex != (rtlbivex_list *)0;
          RtlBiVex  = RtlBiVex->NEXT )
    {
      AllName = unionvexexprallname( AllName, RtlBiVex->VEX_COND );
      AllName = unionvexexprallname( AllName, RtlBiVex->VEX_DATA );
    }

    for ( ScanChain  = AllName;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      SensList = addptype( SensList, VASY_VERILOG_SENS_NOEDGE, ScanChain->DATA );
    }

    freechain( AllName );

    SensList = (ptype_list *)reverse( (chain_list *)SensList );
    SetVasyRtlAsgVerilogSens( RtlAsg, SensList );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveVerilogTreatFigure               |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogTreatFigure( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlasg_list *RtlAsg;
  rtlins_list *RtlInst;

  for ( RtlInst  = RtlFigure->INSTANCE;
        RtlInst != (rtlins_list *)0;
        RtlInst  = RtlInst->NEXT )
  {
    VasyVerilogTreatMap( RtlFigure, RtlInst );
  }

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)NULL;
        RtlAsg  = RtlAsg->NEXT )
  {
    VasyVerilogTreatAsg( RtlFigure, RtlAsg );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveVerilogPort                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogPort( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list *RtlDeclar;
  vexexpr      *VexPort;
  char         *PortName;
  char         *PortDir;

  fprintf( VasyFile, "(");

  for ( RtlDeclar = RtlFigure->DECLAR[ RTL_DECLAR_PORT ];
        RtlDeclar != (rtldecl_list *)NULL;
        RtlDeclar = RtlDeclar->NEXT )
  {
    VexPort  = RtlDeclar->VEX_ATOM;
    PortName = GetVexAtomValue( VexPort );
    fprintf( VasyFile, "%s", PortName );
    if ( RtlDeclar->NEXT != NULL )
    {
       fprintf( VasyFile, ", " );
    }
  }

  fprintf( VasyFile, ");\n\n" );
    
  for ( RtlDeclar = RtlFigure->DECLAR[ RTL_DECLAR_PORT ];
        RtlDeclar != (rtldecl_list *)NULL;
        RtlDeclar = RtlDeclar->NEXT )
  {
    VexPort  = RtlDeclar->VEX_ATOM;
    PortName = GetVexAtomValue( VexPort );
    PortDir  = VERILOG_DIR_TYPE[ RtlDeclar->DIR ];

    fprintf( VasyFile, "  %-6s ", PortDir );
 
    if ( IsVexAtomVector( VexPort ) )
    {
      fprintf( VasyFile, "[%d:%d]", VexPort->LEFT, VexPort->RIGHT );
    }

    fprintf( VasyFile, "\t%s;\n", PortName );
  }

  /* Beware: from what I understood from verilog, if a port is a
   * register or three state, it should also be redefined afterwood 
   * with the correct type. */

  for ( RtlDeclar = RtlFigure->DECLAR[ RTL_DECLAR_PORT ];
        RtlDeclar != (rtldecl_list *)NULL;
        RtlDeclar = RtlDeclar->NEXT )
  {
	  if ( IsVasyRtlDeclType( RtlDeclar, VASY_RTL_DECL_REGISTER )
	    || IsVasyRtlDeclType( RtlDeclar, VASY_RTL_DECL_TRISTATE ) )
     {
       VexPort  = RtlDeclar->VEX_ATOM;
       PortName = GetVexAtomValue( VexPort );

       fprintf( VasyFile, "  %-6s ", VasyGetVerilogNetType( RtlDeclar ) );
       if ( IsVexAtomVector( VexPort ) )
       {
         fprintf( VasyFile, "[%d:%d]", VexPort->LEFT, VexPort->RIGHT );
       }
       fprintf( VasyFile, "\t%s;\n", PortName );
     }
  }
  fprintf( VasyFile, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyDriveVerilogDeclar                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogDeclar( RtlFigure, Type )

  rtlfig_list *RtlFigure;
  int          Type;
{
  rtldecl_list *RtlDeclar;
  vexexpr      *VexDeclar;
  vexexpr      *VexInit;   
  char         *DeclarName;
  char         *DeclarDir;
  char         *DeclarKind;

  for ( RtlDeclar  = RtlFigure->DECLAR[ Type ];
        RtlDeclar != (rtldecl_list *)NULL;
        RtlDeclar  = RtlDeclar->NEXT )
  {
    VexDeclar  = RtlDeclar->VEX_ATOM;
    VexInit    = RtlDeclar->VEX_INIT;
    DeclarName = GetVexAtomValue( VexDeclar );
    DeclarDir  = VERILOG_DIR_TYPE[ RtlDeclar->DIR ];
    DeclarKind = VasyGetVerilogNetType( RtlDeclar );

    if ( ! IsVasyRtlDeclarUsed( RtlDeclar ) ) continue;

    fprintf( VasyFile, "  %-4s\t", DeclarKind );

    if ( IsVexAtomVector( VexDeclar ) )
    {
      fprintf( VasyFile, "[%d:%d]", VexDeclar->LEFT, VexDeclar->RIGHT );
    }

    fprintf( VasyFile, "  %s;\n", DeclarName );

    if ( Type == RTL_DECLAR_CONSTANT )
    {
      fprintf( VasyFile, "  assign %s = \t", DeclarName );
      VasyDriveVexExprVerilog( VasyFile, RtlFigure, VexInit );
      fprintf( VasyFile, ";\n" );
    }

    if ( RtlDeclar->NEXT == (rtldecl_list *)NULL )
    {
      fprintf( VasyFile, "\n" );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasyDriveVerilogFsmDeclar                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogFsmDeclar( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlfsm_list      *RtlFsm;
  rtlfsmstate_list *RtlState;
  int               StateNumber; /* No enumeration in verilog ? */
  int               StateRegisterRange;

  for ( RtlFsm  = RtlFigure->FSM;
        RtlFsm != (rtlfsm_list *)NULL;
        RtlFsm  = RtlFsm->NEXT )
  {
    
    for ( RtlState  = RtlFsm->STATE, StateNumber = 0;
          RtlState != (rtlfsmstate_list *)NULL;
          RtlState  = RtlState->NEXT, StateNumber++ );

    StateRegisterRange = Ln2p( StateNumber );

    for ( RtlState  = RtlFsm->STATE, StateNumber = 0;
          RtlState != (rtlfsmstate_list *)NULL;
          RtlState  = RtlState->NEXT, StateNumber++ )
    {
      fprintf( VasyFile,
               "parameter %s_%s = %s;\n",
               RtlFsm->NAME,
               RtlState->NAME,
               VasyVerilogBinary( StateRegisterRange, StateNumber )
             );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyDriveVerilogEntity                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogEntity( RtlFigure )

  rtlfig_list *RtlFigure;
{
  VasyDriveVerilogPort( RtlFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveVerilogVexCond                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogVexCond( RtlFigure, VexExpr, Positive )

  rtlfig_list *RtlFigure;
  vexexpr     *VexExpr;
  int          Positive;

{
  vexexpr *VexCond;
  long     Oper;
  int      NoEqual;

  VexCond = VexExpr;
  NoEqual = 0;

  if ( IsVexNodeOper( VexCond ) )
  {
    Oper = GetVexOperValue( VexCond );

    if ( ( Oper == VEX_EQ ) ||
         ( Oper == VEX_NE ) ||
         ( Oper == VEX_LT ) ||
         ( Oper == VEX_LE ) ||
         ( Oper == VEX_GT ) ||
         ( Oper == VEX_GE ) )
    {
      NoEqual = 1;

      if ( ! Positive )
      {
        Oper      = getvexnotoper( Oper );
        VexCond   = dupvexexpr( VexCond );
        SetVexOperValue( VexCond, Oper );
      }
    }
    else
    if ( Oper == VEX_NOT )
    {
      Positive = ! Positive;
      VexCond  = dupvexexpr( GetVexOperand( VexCond->OPERAND ) );
    }
    else
    if ( isvexnegativeoper( Oper ) )
    {
      Positive  = ! Positive;
      Oper      = getvexnotoper( Oper );
      VexCond   = dupvexexpr( VexCond );

      SetVexOperValue( VexCond, Oper );
    }
  }

  if ( ! NoEqual ) fprintf( VasyFile, "(" );
  VasyDriveVexExprVerilog( VasyFile, RtlFigure, VexCond );
  if ( ! NoEqual ) fprintf( VasyFile, " == 1'b%c)", ( Positive ) ? '1' : '0' );

  fprintf( VasyFile, " " );

  if ( VexCond != VexExpr )
  {
    freevexexpr( VexCond );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyDriveVerilogAssign                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogAssign( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlasg_list   *RtlAsg;
  rtlbivex_list *RtlBiVex;
  ptype_list    *SensList;
  ptype_list    *ScanPtype;
  int            RtlType;
  int            ElseIf;
  int            Edge;

  for ( RtlAsg  = RtlFigure->ASSIGN;
        RtlAsg != (rtlasg_list *)NULL;
        RtlAsg  = RtlAsg->NEXT )
  {
    if ( RtlAsg->TYPE == RTL_ASG_COMBINATORIAL )
    {
      fprintf( VasyFile, "  " );
      fprintf( VasyFile, "assign\t" );
      VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
      fprintf( VasyFile, " = " );
      VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlAsg->VEX_DATA );
      fprintf( VasyFile, ";\n" );
    }
    else
    if ( ( RtlAsg->TYPE == RTL_ASG_REGISTER    ) ||
         ( RtlAsg->TYPE == RTL_ASG_PULL_UP     ) ||
         ( RtlAsg->TYPE == RTL_ASG_PULL_DOWN   ) ||
         ( RtlAsg->TYPE == RTL_ASG_TRISTATE    ) ||
         ( RtlAsg->TYPE == RTL_ASG_CONDITIONAL ) ||
         ( RtlAsg->TYPE == RTL_ASG_MULTIPLEXOR ) )
    {
      fprintf( VasyFile, "\n  always @ ( " );
      SensList = GetVasyRtlAsgVerilogSens( RtlAsg );
      
      for ( ScanPtype  = SensList;
            ScanPtype != (ptype_list *)0;
            ScanPtype  = ScanPtype->NEXT )
      {
        if ( ScanPtype->TYPE == VASY_VERILOG_SENS_POSEDGE )
        {
          fprintf( VasyFile, "posedge %s", (char *)ScanPtype->DATA );
        }
        else
        if ( ScanPtype->TYPE == VASY_VERILOG_SENS_NEGEDGE )
        {
          fprintf( VasyFile, "negedge %s", (char *)ScanPtype->DATA );
        }
        else
        {
          fprintf( VasyFile, "%s", (char *)ScanPtype->DATA );
        }
  
        if ( ScanPtype->NEXT != (ptype_list *)NULL )
        {
          fprintf( VasyFile, " or " );
        }
      }
  
      freeptype( SensList );
      SetVasyRtlAsgVerilogSens( RtlAsg, NULL );
  
      fprintf( VasyFile, " )\n" );
    }

    if ( RtlAsg->TYPE == RTL_ASG_REGISTER )
    {
      ElseIf = 0;
      Edge   = 0;

      for ( RtlBiVex  = RtlAsg->BIVEX;
            RtlBiVex != (rtlbivex_list *)0;
            RtlBiVex  = RtlBiVex->NEXT )
      {
        RtlType = RtlBiVex->TYPE;

        if ( Edge ) fprintf( VasyFile, "  " );

        if ( RtlBiVex->VEX_COND != (vexexpr *)0 )
        {
          if ( ( RtlType != RTL_BIVEX_RISING_EDGE  ) &&
               ( RtlType != RTL_BIVEX_FALLING_EDGE ) )
          {
            if ( ElseIf == 1 ) fprintf( VasyFile, "    else if " );
            else               fprintf( VasyFile, "    if "      );
          }

          if ( ( RtlType == RTL_BIVEX_RISING_EDGE  ) ||
               ( RtlType == RTL_BIVEX_FALLING_EDGE ) )
          {
            if ( ElseIf == 1 ) fprintf( VasyFile, "    else\n" );

            fprintf( VasyFile, "    begin\n" );

            ElseIf = 0;
            Edge   = 1;
          }
          else
          {
            VasyDriveVerilogVexCond( RtlFigure, RtlBiVex->VEX_COND, 1 );

            ElseIf = 1;
          }
        }

        if ( ( RtlType != RTL_BIVEX_FALLING_EDGE ) &&
             ( RtlType != RTL_BIVEX_RISING_EDGE  ) )
        {
          if ( ( RtlType == RTL_BIVEX_SYNC_WEN      ) &&
               ( RtlBiVex->VEX_COND == (vexexpr *)0 ) )
          {
            fprintf( VasyFile, "    " );
          }

          VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
        }

        if ( ( RtlType == RTL_BIVEX_ASYNC_SET ) ||
             ( RtlType == RTL_BIVEX_SYNC_SET  ) )
        {
          fprintf( VasyFile, " = %s;\n",
                   VasyVerilogBinary( RtlAsg->VEX_ATOM->WIDTH, 1 ) );
        }
        else
        if ( ( RtlType == RTL_BIVEX_ASYNC_RESET ) ||
             ( RtlType == RTL_BIVEX_SYNC_RESET  ) )
        {
          fprintf( VasyFile, " = %s;\n",
                   VasyVerilogBinary( RtlAsg->VEX_ATOM->WIDTH, 0 ) );
        }
        else
        if ( ( RtlType == RTL_BIVEX_ASYNC_WEN ) ||
             ( RtlType == RTL_BIVEX_SYNC_WEN  ) )
        {
          fprintf( VasyFile, " = " );
          VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlBiVex->VEX_DATA );
          fprintf( VasyFile, ";\n" );
        }
      }

      if ( Edge )
      {
        fprintf( VasyFile, "    end\n" );
      }
    }
    else
    if ( ( RtlAsg->TYPE == RTL_ASG_TRISTATE  ) ||
         ( RtlAsg->TYPE == RTL_ASG_PULL_UP   ) ||
         ( RtlAsg->TYPE == RTL_ASG_PULL_DOWN ) )
    {
      RtlBiVex = RtlAsg->BIVEX;

      fprintf( VasyFile, "    if " );
      VasyDriveVerilogVexCond( RtlFigure, RtlBiVex->VEX_COND, 1 );
      VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
      fprintf( VasyFile, " = " );
      VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlBiVex->VEX_DATA );
      fprintf( VasyFile, ";\n    else " );
      VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
      fprintf( VasyFile, " = %s;\n",
               VasyVerilogBinaryZXU( RtlAsg->VEX_ATOM->WIDTH, 'z' ) );
    }
    else
    if ( RtlAsg->TYPE == RTL_ASG_CONDITIONAL )
    {
      ElseIf = 0;

      for ( RtlBiVex  = RtlAsg->BIVEX;
            RtlBiVex != (rtlbivex_list *)0;
            RtlBiVex  = RtlBiVex->NEXT )
      {
        RtlType = RtlBiVex->TYPE;

        if ( RtlBiVex->VEX_COND != (vexexpr *)0 )
        {
          if ( ElseIf == 1 ) fprintf( VasyFile, "    else if " );
          else               fprintf( VasyFile, "    if "      );

          VasyDriveVerilogVexCond( RtlFigure, RtlBiVex->VEX_COND, 1 );

          ElseIf = 1;
        }
        else
        if ( ElseIf == 1 )
        {
          fprintf( VasyFile, "    else " );
        }

        VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlAsg->VEX_ATOM );
        fprintf( VasyFile, " = " );
        VasyDriveVexExprVerilog( VasyFile, RtlFigure, RtlBiVex->VEX_DATA );
        fprintf( VasyFile, ";\n" );
      }
    }
    else
    if ( RtlAsg->TYPE == RTL_ASG_MULTIPLEXOR )
    {
      /* TO BE DONE */
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyDriveVerilogFsm                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogFsm( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlfsm_list      *RtlFsm;
  /*
  rtlfsmstate_list *FsmState;
  rtlfsmtrans_list *FsmTrans;
  int               ElseIf;
  rtlfsmasg_list   *FsmAsg;
  chain_list       *ScanChain;
  */

  for ( RtlFsm  = RtlFigure->FSM;
        RtlFsm != (rtlfsm_list *)NULL;
        RtlFsm  = RtlFsm->NEXT )
  {
    VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "Driver Verilog: FSM" );
/*
**  Drive The transition process
*/
    /* TO BE DONE 

    fprintf( VasyFile, "  always @ ( " );

    if ( RtlFsm->VEX_EDGE != (vexexpr *)NULL )
    {
      Support = getvexexprsupport( RtlFsm->VEX_EDGE );
      if ( VasyVerilogEdgeDirection( RtlFsm->VEX_COND ) == EDGE_UP )
      {
        fprintf( VasyFile, "posedge %s", (char *)Support->DATA );
      }
      else
      {
        fprintf( VasyFile, "negedge %s", (char *)Support->DATA );
      }
      Support = NULL;
    }
    else
    {
      Support = getvexexprsupport( RtlFsm->VEX_COND );
    }

    Support = unionvexexprsupport( Support, RtlFsm->VEX_WEN );

    for ( ScanChain  = Support;
          ScanChain != (chain_list *)NULL;
          ScanChain  = ScanChain->NEXT )
    {
      fprintf( VasyFile, "%s", (char *)ScanChain->DATA );

      if ( ScanChain->NEXT != (chain_list *)NULL )
      {
        fprintf( VasyFile, " or " );
      }
    }

    freechain( Support );

    fprintf( VasyFile, " )\n" );
    fprintf( VasyFile, "  begin\n" );

    if ( RtlFsm->VEX_WEN != (vexexpr *)NULL )
    {
      fprintf( VasyFile, "\n" );
      fprintf( VasyFile, "      if " );
      VasyDriveVerilogVexCond( RtlFsm->VEX_WEN );
      fprintf( VasyFile, "\n" );
      fprintf( VasyFile, "      begin\n" );
    }

    fprintf( VasyFile, "      case (%s)\n", RtlFsm->NAME );

    for ( FsmState  = RtlFsm->STATE;
          FsmState != (rtlfsmstate_list *)NULL;
          FsmState  = FsmState->NEXT )
    {
      fprintf( VasyFile, "\n        %s_%s:\n", RtlFsm->NAME, FsmState->NAME );
      fprintf( VasyFile, "          begin\n" );

      ElseIf = 0;

      for ( ScanChain  = FsmState->FROM;
            ScanChain != (chain_list *)NULL;
            ScanChain  = ScanChain->NEXT )
      {
        FsmTrans = GetRtlFsmTrans( ScanChain );
        
        if ( FsmTrans->VEX_COND != (vexexpr *)NULL )
        {
          if ( ! ElseIf )
          {
            fprintf( VasyFile, "          if " );
          }
          else
          if ( ScanChain->NEXT != (chain_list *)NULL )
          {
            fprintf( VasyFile, "          else if " );
          }
          else
          {
            fprintf( VasyFile, "          else\n" );
          }

          if ( ScanChain->NEXT != (chain_list *)NULL )
          {
            VasyDriveVerilogVexCond( FsmTrans->VEX_COND, 1 );
            fprintf( VasyFile, "\n          begin\n" );
          }

          ElseIf = 1;
        }

        fprintf( VasyFile, "            %s = %s;\n", RtlFsm->NAME, FsmTrans->TO->NAME );

        for ( FsmAsg  = FsmTrans->ASSIGN;
              FsmAsg != (rtlfsmasg_list *)NULL;
              FsmAsg  = FsmAsg->NEXT )
        {
          fprintf( VasyFile, "            " );
          VasyDriveVexExprVerilog( VasyFile, RtlFigure, FsmAsg->VEX_ATOM );
          fprintf( VasyFile, " = " );
          VasyDriveVexExprVerilog( VasyFile, RtlFigure, FsmAsg->VEX_DATA );
          fprintf( VasyFile, ";\n" );
        }
      }

      if ( ElseIf )
      {
        fprintf( VasyFile, "          end\n" );
      }
      fprintf( VasyFile, "          end\n" );
    }

    fprintf( VasyFile, "\n      endcase\n" );

    if ( RtlFsm->VEX_WEN != (vexexpr *)NULL )
    {
      fprintf( VasyFile, "      end\n" );
    }

    fprintf( VasyFile, "  end\n\n" );
    */
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasyDriveVerilogPortMap                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogPortMap( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlmod_list *RtlModel;
  rtlins_list *RtlInst;
  rtlmap_list *RtlMap;
  vexexpr     *VexFormal;
  vexexpr     *VexActual;

  for ( RtlInst  = RtlFigure->INSTANCE;
        RtlInst != (rtlins_list *)0;
        RtlInst  = RtlInst->NEXT )
  {
    RtlModel = RtlInst->MODEL;

    fprintf( VasyFile, "  %s %s (\n", RtlModel->NAME, RtlInst->NAME );
    RtlMap = RtlInst->MAP;

    while ( RtlMap != (rtlmap_list *)0 )
    {
      VexFormal = RtlMap->VEX_FORMAL;
      VexActual = RtlMap->VEX_ACTUAL;

      fprintf( VasyFile, "   .%s(", GetVexAtomValue( VexFormal ) );
      VasyDriveVexExprVerilog( VasyFile, RtlFigure, VexActual );
      fprintf( VasyFile, ")" );

      if ( RtlMap->NEXT != (rtlmap_list *)0 )
      {
        fprintf( VasyFile, ",\n" );
      }
  
      RtlMap = RtlMap->NEXT;
    }

    fprintf( VasyFile, "\n  );\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyDriveVerilogArchi                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveVerilogArchi( RtlFigure )

  rtlfig_list *RtlFigure;
{
  VasyDriveVerilogDeclar( RtlFigure, RTL_DECLAR_CONSTANT );
  VasyDriveVerilogDeclar( RtlFigure, RTL_DECLAR_SIGNAL   );
  VasyDriveVerilogFsmDeclar( RtlFigure );

  VasyDriveVerilogAssign( RtlFigure );
  VasyDriveVerilogFsm( RtlFigure );
  VasyDriveVerilogPortMap( RtlFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyDriveVerilogRtlFig                   |
|                                                             |
\------------------------------------------------------------*/

void VasyDriveVerilogRtlFig( RtlFigure, FileName )

  rtlfig_list  *RtlFigure;
  char         *FileName;
{
  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyDriveVerilogRtlFig %s\n", FileName );
  }

  if ( IsVasyDebugDriveStdout() )
  {
    VasyFile = stdout;
  }
  else
  {
    if ( ! VasyFlagOver )
    {
/*
** First checks to avoid overwriting an existing file !
*/
      VasyFile = mbkfopen( FileName, "v", "r" );

      if ( VasyFile != (FILE *)0 )
      {
        fclose( VasyFile );
        VasyPrintf( stdout, "ERROR file %s.v already exists, remove it first !\n", 
                    FileName );
        autexit( 1 );
      }
    }

    VasyFile = mbkfopen( FileName, "v", "w" );
  }

  if ( VasyFile != (FILE *)NULL )
  {
/*
** Special treatments for verilog
*/
    VasyDriveVerilogTreatFigure( RtlFigure );

    fprintf( VasyFile, "module %s ", RtlFigure->NAME );
    VasyDriveVerilogEntity( RtlFigure );
    VasyDriveVerilogArchi( RtlFigure );
    fprintf( VasyFile, "\nendmodule\n" );

    if ( ! IsVasyDebugDriveStdout() )
    {
      fclose( VasyFile );
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyDriveVerilogRtlFig %s\n\n", FileName );
  }
}
