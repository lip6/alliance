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
| File    :                  vasy_simul.c                     |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   28.08.97                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_error.h"
# include "vasy_debug.h"
# include "vasy_simul.h"

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

  static vpnfig_list  *VasyFigure     = (vpnfig_list  *)0;
  static vpnline_list *VasyVpnLine    = (vpnline_list *)0;

  static vasysimul *VasySimulateVex();

  static vasysimul *VasySimulateVexConcat();
  static vasysimul *VasySimulateVexLogic();
  static vasysimul *VasySimulateVexEvent();
  static vasysimul *VasySimulateVexNeg();
  static vasysimul *VasySimulateVexAbs();
  static vasysimul *VasySimulateVexSum();
  static vasysimul *VasySimulateVexEqual();
  static vasysimul *VasySimulateVexCompare();
  static vasysimul *VasySimulateVexSlice();
  static vasysimul *VasySimulateVexDriver();

  static vasysimul *(*VasySimulOperFunc[ VEX_MAX_OPERATOR ])() =
  {
     VasySimulateVexConcat,     /* VEX_CONCAT    */
     VasySimulateVexLogic,      /* VEX_NOT       */
     VasySimulateVexNeg,        /* VEX_NEG       */
     VasySimulateVexEvent,      /* VEX_EVENT     */
     VasySimulateVexLogic,      /* VEX_OR        */
     VasySimulateVexLogic,      /* VEX_AND       */
     VasySimulateVexLogic,      /* VEX_XOR       */
     VasySimulateVexLogic,      /* VEX_NOR       */
     VasySimulateVexLogic,      /* VEX_NAND      */
     VasySimulateVexLogic,      /* VEX_NXOR      */
     VasySimulateVexEqual,      /* VEX_EQ        */
     VasySimulateVexEqual,      /* VEX_NE        */
     VasySimulateVexCompare,    /* VEX_LT        */
     VasySimulateVexCompare,    /* VEX_LE        */
     VasySimulateVexCompare,    /* VEX_GT        */
     VasySimulateVexCompare,    /* VEX_GE        */
     VasySimulateVexSum,        /* VEX_ADD       */
     VasySimulateVexSum,        /* VEX_SUB       */
     NULL,                      /* VEX_MUL       */
     NULL,                      /* VEX_DIV       */
     NULL,                      /* VEX_EXP       */
     NULL,                      /* VEX_MOD       */
     NULL,                      /* VEX_REM       */
     VasySimulateVexSlice,      /* VEX_TO        */
     VasySimulateVexSlice,      /* VEX_DOWNTO    */
     VasySimulateVexSlice,      /* VEX_INDEX     */
     NULL,                      /* VEX_LEFT      */
     NULL,                      /* VEX_RIGHT     */
     NULL,                      /* VEX_LOW       */
     NULL,                      /* VEX_HIGH      */
     NULL,                      /* VEX_LENGTH    */
     NULL,                      /* VEX_RANGE     */
     NULL,                      /* VEX_REV_RANGE */
     VasySimulateVexDriver,     /* VEX_DRIVER    */
     VasySimulateVexLogic,      /* VEX_IFT       */
     NULL,                      /* VEX_ARRAY     */
     NULL,                      /* VEX_INDEX_N   */
     NULL,                      /* VEX_OTHERS    */
     NULL,                      /* VEX_NUM_BIT   */
     VasySimulateVexAbs,        /* VEX_ABS       */
     NULL,                      /* AF            */
     NULL,                      /* AG            */
     NULL,                      /* AX            */
     NULL,                      /* AU            */
     NULL,                      /* EF            */
     NULL,                      /* EG            */
     NULL,                      /* EX            */
     NULL,                      /* EU            */
     NULL,                      /* EQUIV         */
     NULL                       /* IMPLY         */ 
  };

  static char VasyTruthTableAnd[ VEX_MAX_ID ][ VEX_MAX_ID ] =
  {
    /*  --------------------------------------------------
    **  |  U    X    0    1    Z    W    L    H    -
    **  --------------------------------------------------
    */
        {  0 ,  0 ,  2 ,  0 ,  0 ,  0 ,  2 ,  0 ,  0  },  /* u */
        {  0 ,  1 ,  2 ,  1 ,  1 ,  1 ,  2 ,  1 ,  1  },  /* x */
        {  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2  },  /* 0 */
        {  0 ,  1 ,  2 ,  3 ,  1 ,  1 ,  2 ,  3 ,  1  },  /* 1 */
        {  0 ,  1 ,  2 ,  1 ,  1 ,  1 ,  2 ,  1 ,  1  },  /* z */
        {  0 ,  1 ,  2 ,  1 ,  1 ,  1 ,  2 ,  1 ,  1  },  /* w */
        {  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2  },  /* l */
        {  0 ,  1 ,  2 ,  3 ,  1 ,  1 ,  2 ,  3 ,  1  },  /* h */
        {  0 ,  1 ,  2 ,  1 ,  1 ,  1 ,  2 ,  1 ,  1  }   /* - */
  };

  static char VasyTruthTableOr[ VEX_MAX_ID ][ VEX_MAX_ID ] =
  {
    /*  ----------------------------------------------------
    **  |  u    x    0    1    z    w    l    h    -
    **  ----------------------------------------------------
    */
        {  0 ,  0 ,  0 ,  3 ,  0 ,  0 ,  0 ,  3 ,  0  },  /* u */
        {  0 ,  1 ,  1 ,  3 ,  1 ,  1 ,  1 ,  3 ,  1  },  /* x */
        {  0 ,  1 ,  2 ,  3 ,  1 ,  1 ,  2 ,  3 ,  1  },  /* 0 */
        {  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3  },  /* 1 */
        {  0 ,  1 ,  1 ,  3 ,  1 ,  1 ,  1 ,  3 ,  1  },  /* z */
        {  0 ,  1 ,  1 ,  3 ,  1 ,  1 ,  1 ,  3 ,  1  },  /* w */
        {  0 ,  1 ,  2 ,  3 ,  1 ,  1 ,  2 ,  3 ,  1  },  /* l */
        {  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3  },  /* h */
        {  0 ,  1 ,  1 ,  3 ,  1 ,  1 ,  1 ,  3 ,  1  }   /* - */
  };

  static char VasyTruthTableXor[ VEX_MAX_ID ][ VEX_MAX_ID ] =
  {
    /*  ----------------------------------------------------
    **  |  u    x    0    1    z    w    l    h    -
    **  ----------------------------------------------------
    */
        {  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0  },  /* u */
        {  0 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1  },  /* x */
        {  0 ,  1 ,  2 ,  3 ,  1 ,  1 ,  2 ,  3 ,  1  },  /* 0 */
        {  0 ,  1 ,  3 ,  2 ,  1 ,  1 ,  3 ,  2 ,  1  },  /* 1 */
        {  0 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1  },  /* z */
        {  0 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1  },  /* w */
        {  0 ,  1 ,  2 ,  3 ,  1 ,  1 ,  2 ,  3 ,  1  },  /* l */
        {  0 ,  1 ,  3 ,  2 ,  1 ,  1 ,  3 ,  2 ,  1  },  /* h */
        {  0 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1  }   /* - */
  };

  static char VasyTruthTableNot[ VEX_MAX_ID ] =
  {
    /*  ----------------------------------------------------
    **  |  u    x    0    1    z    w    l    h    -
    **  ----------------------------------------------------
    */
            0 ,  1 ,  3 ,  2 ,  1 ,  1 ,  3 ,  2 ,  1 
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateViewSimul                   |
|                                                             |
\------------------------------------------------------------*/

static void VasySimulateViewSimul( ScanSimul )

  vasysimul *ScanSimul;
{
  char    *Value;
  int      Position;
  int      Width;

  VasyPrintf( stdout, "    --> VasySimulateViewSimul %d %d\n",
             ScanSimul->WIDTH, ScanSimul->SIGNED );

  Value   = ScanSimul->VALUE;
  Width   = ScanSimul->WIDTH;

  for ( Position = 0; Position < Width; Position++ )
  {
    VasyPrintf( stdout, "    +++ Simul[ %d ] > %c\n", 
               Position, VEX_LITERAL_BY_ID[ (int)Value[ Position ] ] );
  }

  VasyPrintf( stdout, "    <-- VasySimulateViewSimul\n" );
}
 
/*------------------------------------------------------------\
|                                                             |
|                      VasySimulateAddSimul                   |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateAddSimul( Width, Signed )

  short Width;
  short Signed;
{
  vasysimul *NewSimul;

  NewSimul = (vasysimul *)autallocheap( sizeof( vasysimul ) );

  NewSimul->VALUE  = autallocheap( Width );
  NewSimul->WIDTH  = Width;
  NewSimul->SIGNED = Signed;

  return( NewSimul );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasySimulateDelSimul                   |
|                                                             |
\------------------------------------------------------------*/

void VasySimulateDelSimul( DelSimul )

  vasysimul *DelSimul;
{
  autfreeheap( DelSimul->VALUE, DelSimul->WIDTH );
  autfreeheap( DelSimul, sizeof( vasysimul ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasySimulateSignSimul                  |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateSignSimul( Simul )

  vasysimul *Simul;
{
  vasysimul *NewSimul;
  char     *NewValue; 
  char     *Value; 
  int       Position;

  Value = Simul->VALUE;

  if ( ! Simul->SIGNED )
  {
    if ( IsVasyDebugLevel2() )
    {
      VasyPrintf( stdout, "  --> VasySimulateSignSimul\n" );
      VasySimulateViewSimul( Simul );
    }

    if ( Value[ 0 ] == VEX_ONE_ID )
    {
      NewSimul = VasySimulateAddSimul( Simul->WIDTH + 1, 1 );
      NewValue = NewSimul->VALUE;

      NewValue[ 0 ] = VEX_ZERO_ID;

      for ( Position = 0; Position < Simul->WIDTH; Position++ )
      {
        NewValue[ Position + 1 ] = Value[ Position ];
      }

      VasySimulateDelSimul( Simul );
      Simul = NewSimul;
    }

    Simul->SIGNED = 1;

    if ( IsVasyDebugLevel2() )
    {
      VasySimulateViewSimul( Simul );
      VasyPrintf( stdout, "  <-- VasySimulateSignSimul\n" );
    }
  }

  return( Simul );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySimulateIncWidthSimul                |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateCarryOutSimul( Simul, CarryOut )

  vasysimul *Simul;
  char      CarryOut;
{
  vasysimul *NewSimul;
  char     *NewValue; 
  char     *Value; 
  int       Position;

  NewSimul = VasySimulateAddSimul( Simul->WIDTH + 1, Simul->SIGNED );

  NewValue = NewSimul->VALUE;
  Value    = Simul->VALUE;

  for ( Position = 0; Position < Simul->WIDTH; Position++ )
  {
    NewValue[ Position + 1 ] = Value[ Position ];
  }

  NewValue[ 0 ] = CarryOut;
  VasySimulateDelSimul( Simul );

  return( NewSimul );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySimulateExtendSimul                  |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateExtendSimul( Simul, Width )

  vasysimul *Simul;
  short     Width;
{
  vasysimul *NewSimul;
  char     *NewValue; 
  char     *Value; 
  int       Position;
  int       Delta;
  char      BitSign;

  Simul = VasySimulateSignSimul( Simul );
  Delta = Width - Simul->WIDTH;

  if ( Delta > 0 )
  {
    if ( IsVasyDebugLevel2() )
    {
      VasyPrintf( stdout, "  --> VasySimulateExtendSimul (%d %d)\n",
                 Simul->WIDTH, Width );
      VasySimulateViewSimul( Simul );
    }

    Value   = Simul->VALUE;
    BitSign = Value[ 0 ];

    NewSimul = VasySimulateAddSimul( Width, 1 );
    NewValue = NewSimul->VALUE;

    for ( Position = 0; Position < Delta; Position++ )
    {
      NewValue[ Position ] = BitSign;
    }

    for ( Position = 0; Position < Simul->WIDTH; Position++ )
    {
      NewValue[ Position + Delta ] = Value[ Position ];
    }

    VasySimulateDelSimul( Simul );
    Simul = NewSimul;

    if ( IsVasyDebugLevel2() )
    {
      VasySimulateViewSimul( Simul );
      VasyPrintf( stdout, "  <-- VasySimulateExtendSimul\n" );
    }
  }

  return( Simul );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySimulateResizeSimul                  |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateResizeSimul( Simul, Width, Signed )

  vasysimul *Simul;
  short     Width;
  short     Signed;
{
  vasysimul *NewSimul;
  char     *NewValue; 
  char     *Value; 
  int       Position;
  int       Delta;
  char      BitSign;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasySimulateResizeSimul > (%d %d)\n", Width, Signed );
    VasySimulateViewSimul( Simul );
  }

  Value = Simul->VALUE;
  Delta = Width - Simul->WIDTH;

  if ( Delta == 0 )
  {
/*
**  Target Width is equal => must be convertible
*/
    if ( ( Simul->SIGNED != Signed     ) &&
         ( Value[ 0 ]   == VEX_ONE_ID ) )
    {
      VasyWarningLine( VASY_WARNING_IN_SIMULATION, VasyVpnLine, "unconvertible value" );
    }
  }
  else
  if ( Delta > 0 )
  {
/*
**  Target Width is greater => must extend the sign 
*/
    if ( ( Simul->SIGNED             ) &&
         ( ! Signed                 ) &&
         ( Value[ 0 ] == VEX_ONE_ID ) )
    {
      VasyErrorLine( VASY_WARNING_IN_SIMULATION, VasyVpnLine, "unconvertible value" );
    }

    if ( ( Signed       ) &&
         ( Simul->SIGNED ) ) BitSign = Value[ 0 ];
    else                     BitSign = VEX_ZERO_ID;

    NewSimul  = VasySimulateAddSimul( Width, Signed );
    NewValue = NewSimul->VALUE;

    for ( Position = 0; Position < Delta; Position++ )
    {
      NewValue[ Position ] = BitSign;
    }

    for ( Position = 0; Position < Simul->WIDTH; Position++ )
    {
      NewValue[ Position + Delta ] = Value[ Position ];
    }

    VasySimulateDelSimul( Simul );
    Simul = NewSimul;
  }
  else
  {
/*
**  Target Width is lower => must verify the sign 
*/
    Delta = - Delta;

    if ( ( Signed        ) &&
         ( Simul->SIGNED ) ) BitSign = Value[ 0 ];
    else                     BitSign = VEX_ZERO_ID;

    for ( Position = 0; Position < Delta; Position++ )
    {
      if ( Value[ Position ] != BitSign )
      {
        VasyWarningLine( VASY_WARNING_IN_SIMULATION, VasyVpnLine, "unconvertible value" );
      }
    }

    NewSimul  = VasySimulateAddSimul( Width, Signed );
    NewValue = NewSimul->VALUE;

    for ( Position = 0; Position < Width; Position++ )
    {
      NewValue[ Position ] = Value[ Position + Delta ];
    }

    VasySimulateDelSimul( Simul );
    Simul = NewSimul;
  }

  if ( IsVasyDebugLevel2() )
  {
    VasySimulateViewSimul( Simul );
    VasyPrintf( stdout, "  <-- VasySimulateResizeSimul\n" );
  }

  return( Simul );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySimulateIntegerSimul                 |
|                                                             |
\------------------------------------------------------------*/

static long VasySimulateIntegerSimul( SimulExpr )

  vasysimul *SimulExpr;
{
  char *SimulValue;
  int   Width;
  int   Position;
  long  Value;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasySimulateIntegerSimul\n" );
    VasySimulateViewSimul( SimulExpr );
  }

  SimulValue = SimulExpr->VALUE;
  Width      = SimulExpr->WIDTH;
  Position   = 0;
  Value      = 0;

  if ( ( SimulExpr->SIGNED             ) &&
       ( SimulValue[ 0 ] == VEX_ONE_ID ) )
  {
    Value = -1;
  }

  for ( Position = 0; Position < Width; Position++ )
  {
    Value = Value << 1;

    if ( SimulValue[ Position ] == VEX_ONE_ID )
    {
      Value |= 1;
    }
    else
    if ( SimulValue[ Position ] != VEX_ZERO_ID )
    {
      VasyErrorLine( VASY_ERROR_IN_SIMULATION, VasyVpnLine, "not integer value" );
    }
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  <-- VasySimulateNegSimul %ld\n", Value );
  }

  return( Value );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySimulateNegSimul                     |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateNegSimul( SimulExpr )

  vasysimul *SimulExpr;
{
  char *SimulValue;
  int   Position;
  char  Value1;
  char  Value;
  char  Carry;
  char  Sign;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasySimulateNegSimul\n" );
    VasySimulateViewSimul( SimulExpr );
  }

  SimulExpr  = VasySimulateSignSimul( SimulExpr );
  SimulValue = SimulExpr->VALUE;

  Sign  = SimulValue[ 0 ];
  Carry = 1;

  for ( Position = SimulExpr->WIDTH - 1; Position >= 0; Position-- )
  {
    if ( SimulValue[ Position ] == VEX_ONE_ID ) Value1 = 0;
    else                                       Value1 = 1;

    Value = Value1 ^ Carry;
    Carry = Value1 & Carry;

    if ( Value ) Value = VEX_ONE_ID;
    else         Value = VEX_ZERO_ID;

    SimulValue[ Position ] = Value;
  }

  if ( ( Sign            == VEX_ONE_ID  ) &&
       ( SimulValue[ 0 ] != VEX_ZERO_ID ) )
  {
    SimulExpr = VasySimulateCarryOutSimul( SimulExpr, VEX_ZERO_ID );
  }

  if ( IsVasyDebugLevel2() )
  {
    VasySimulateViewSimul( SimulExpr );
    VasyPrintf( stdout, "  <-- VasySimulateNegSimul\n" );
  }

  return( SimulExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySimulateSumSimul                     |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateSumSimul( SimulExpr1, SimulExpr2 )

  vasysimul *SimulExpr1;
  vasysimul *SimulExpr2;
{
  char    *SimulValue1;
  char    *SimulValue2;
  char     Negative1;
  char     Negative2;
  char     Value1;
  char     Value2;
  char     Value;
  char     Carry;
  int      Position;

  SimulExpr1 = VasySimulateSignSimul( SimulExpr1 );
  SimulExpr2 = VasySimulateSignSimul( SimulExpr2 );

  if ( SimulExpr1->WIDTH > SimulExpr2->WIDTH )
  {
    SimulExpr2 = VasySimulateExtendSimul( SimulExpr2, SimulExpr1->WIDTH );
  }
  else
  if ( SimulExpr2->WIDTH > SimulExpr1->WIDTH )
  {
    SimulExpr1 = VasySimulateExtendSimul( SimulExpr1, SimulExpr2->WIDTH );
  }

  Carry       = 0;
  SimulValue1 = SimulExpr1->VALUE;
  SimulValue2 = SimulExpr2->VALUE;

  Negative1 = ( SimulValue1[ 0 ] == VEX_ONE_ID );
  Negative2 = ( SimulValue2[ 0 ] == VEX_ONE_ID );

  for ( Position = SimulExpr1->WIDTH - 1; Position >= 0; Position-- )
  {
    Value1 = SimulValue1[ Position ];
    Value2 = SimulValue2[ Position ];

    if ( Value1 == VEX_ONE_ID ) Value1 = 1;
    else                        Value1 = 0;

    if ( Value2 == VEX_ONE_ID ) Value2 = 1;
    else                        Value2 = 0;

    Value = Value1 ^ Value2 ^ Carry;
    Carry = ( Value1 & Value2 ) |
            ( Value1 & Carry  ) |
            ( Value2 & Carry  ) ;

    if ( Value ) Value = VEX_ONE_ID;
    else         Value = VEX_ZERO_ID;

    SimulValue1[ Position ] = Value;
  }

  if ( ( Negative1 ) &&
       ( Negative2 ) )
  {
    if ( SimulValue1[ 0 ] == VEX_ZERO_ID )
    {
      SimulExpr1 = VasySimulateCarryOutSimul( SimulExpr1, VEX_ONE_ID );
    }
  }
  else
  if ( ( ! Negative1 ) &&
       ( ! Negative2 ) )
  {
    if ( SimulValue1[ 0 ] == VEX_ONE_ID )
    {
      SimulExpr1 = VasySimulateCarryOutSimul( SimulExpr1, VEX_ZERO_ID );
    }
  }

  VasySimulateDelSimul( SimulExpr2 );

  return( SimulExpr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySimulateAbsSimul                     |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateAbsSimul( SimulExpr )

  vasysimul *SimulExpr;
{
  char *SimulValue;
  int   Position;
  char  Value1;
  char  Value;
  char  Carry;
  char  Sign;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasySimulateAbsSimul\n" );
    VasySimulateViewSimul( SimulExpr );
  }

  SimulValue = SimulExpr->VALUE;

  Sign = SimulValue[ 0 ];

  if ( ( Sign == VEX_ONE_ID ) &&
       ( SimulExpr->SIGNED  ) )
  {
    Carry = 1;

    for ( Position = SimulExpr->WIDTH - 1; Position >= 0; Position-- )
    {
      if ( SimulValue[ Position ] == VEX_ONE_ID ) Value1 = 0;
      else                                        Value1 = 1;
  
      Value = Value1 ^ Carry;
      Carry = Value1 & Carry;
  
      if ( Value ) Value = VEX_ONE_ID;
      else         Value = VEX_ZERO_ID;
  
      SimulValue[ Position ] = Value;
    }
  
    if ( ( Sign            == VEX_ONE_ID  ) &&
         ( SimulValue[ 0 ] != VEX_ZERO_ID ) )
    {
      SimulExpr = VasySimulateCarryOutSimul( SimulExpr, VEX_ZERO_ID );
    }
  }

  if ( IsVasyDebugLevel2() )
  {
    VasySimulateViewSimul( SimulExpr );
    VasyPrintf( stdout, "  <-- VasySimulateAbsSimul\n" );
  }

  return( SimulExpr );
}


/*------------------------------------------------------------\
|                                                             |
|                      VasySimulateVexAbs                     |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexAbs( Expr )

  vexexpr *Expr;
{
  vasysimul *SimulExpr;
  vexexpr *Operand;

  Operand   = GetVexOperand( Expr->OPERAND );
  SimulExpr  = VasySimulateVex( Operand );
  SimulExpr  = VasySimulateAbsSimul( SimulExpr );

  return( SimulExpr );
}


/*------------------------------------------------------------\
|                                                             |
|                      VasySimulateVexNeg                     |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexNeg( Expr )

  vexexpr *Expr;
{
  vasysimul *SimulExpr;
  vexexpr *Operand;

  Operand   = GetVexOperand( Expr->OPERAND );
  SimulExpr  = VasySimulateVex( Operand );
  SimulExpr  = VasySimulateNegSimul( SimulExpr );

  return( SimulExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexSum                      |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexSum( Expr )

  vexexpr *Expr;
{
  vasysimul        *SimulExpr1;
  vasysimul        *SimulExpr2;
  chain_list     *ScanChain;
  vexexpr        *Operand;
  long            Oper;

  Oper = GetVexOperValue( Expr );

  ScanChain = Expr->OPERAND;
  Operand   = GetVexOperand( ScanChain );
  SimulExpr1 = VasySimulateVex( Operand );
  ScanChain = ScanChain->NEXT;

  while ( ScanChain != (chain_list *)0 )
  {
    Operand   = GetVexOperand( ScanChain );
    SimulExpr2 = VasySimulateVex( Operand );

    if ( Oper == VEX_SUB )
    {
      SimulExpr2 = VasySimulateNegSimul( SimulExpr2 );
    }

    SimulExpr1 = VasySimulateSumSimul( SimulExpr1, SimulExpr2 );

    ScanChain = ScanChain->NEXT;
  }

  return( SimulExpr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexEqual                    |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexEqual( Expr )

  vexexpr *Expr;
{
  vasysimul       *SimulExpr1;
  vasysimul       *SimulExpr2;
  char           *SimulValue1;
  char           *SimulValue2;
  chain_list     *ScanChain;
  vexexpr        *Operand;
  char            Equal;
  long            Oper;
  int             Position;

  Oper = GetVexOperValue( Expr );

  ScanChain  = Expr->OPERAND;
  Operand    = GetVexOperand( ScanChain );
  SimulExpr1  = VasySimulateVex( Operand );

  ScanChain  = ScanChain->NEXT;
  Operand    = GetVexOperand( ScanChain );
  SimulExpr2  = VasySimulateVex( Operand );

  if ( SimulExpr1->WIDTH > SimulExpr2->WIDTH )
  {
    SimulExpr2 = VasySimulateExtendSimul( SimulExpr2, SimulExpr1->WIDTH );
  }
  else
  if ( SimulExpr2->WIDTH > SimulExpr1->WIDTH )
  {
    SimulExpr1 = VasySimulateExtendSimul( SimulExpr1, SimulExpr2->WIDTH );
  }

  SimulValue1 = SimulExpr1->VALUE;
  SimulValue2 = SimulExpr2->VALUE;

  if ( Oper == VEX_EQ ) Equal = 1;
  else                  Equal = 0;

  for ( Position = 0; Position < SimulExpr2->WIDTH; Position++ )
  {
    if ( SimulValue1[ Position ] != SimulValue2[ Position ] )
    {
      Equal = ! Equal; break;
    }
  }

  VasySimulateDelSimul( SimulExpr1 );
  VasySimulateDelSimul( SimulExpr2 );

  SimulExpr1 = VasySimulateAddSimul( 1, 0 );

  if ( Equal ) SimulExpr1->VALUE[ 0 ] = VEX_ONE_ID;
  else         SimulExpr1->VALUE[ 0 ] = VEX_ZERO_ID;

  return( SimulExpr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexCompare                  |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexCompare( Expr )

  vexexpr *Expr;
{
  vasysimul       *SimulExpr1;
  vasysimul       *SimulExpr2;
  char           *SimulValue1;
  chain_list     *ScanChain;
  vexexpr        *Operand;
  long            Oper;
  short           Compare;
  short           Negative;
  short           Zero;
  int             Position;

  Oper = GetVexOperValue( Expr );

  ScanChain = Expr->OPERAND;
  Operand   = GetVexOperand( ScanChain );
  SimulExpr1 = VasySimulateVex( Operand );

  ScanChain = ScanChain->NEXT;
  Operand   = GetVexOperand( ScanChain );
  SimulExpr2 = VasySimulateVex( Operand );

  SimulExpr2 = VasySimulateNegSimul( SimulExpr2 );
  SimulExpr1 = VasySimulateSumSimul( SimulExpr1, SimulExpr2 );

  SimulValue1 = SimulExpr1->VALUE;

  Negative = ( SimulValue1[ 0 ] == VEX_ONE_ID );
  Zero     = 1;

  for ( Position = SimulExpr1->WIDTH - 1; Position >= 0; Position-- )
  {
    if ( SimulValue1[ Position ] != VEX_ZERO_ID )
    {
      Zero = 0; break;
    }
  }

  if ( Oper == VEX_LT ) Compare = (   Negative ) && ( ! Zero );
  else
  if ( Oper == VEX_GT ) Compare = ( ! Negative ) && ( ! Zero );
  else
  if ( Oper == VEX_LE ) Compare = (   Negative ) || (   Zero );
  else
                        Compare = ( ! Negative ) || (   Zero );

  VasySimulateDelSimul( SimulExpr1 );

  SimulExpr1 = VasySimulateAddSimul( 1, 0 );

  if ( Compare ) SimulExpr1->VALUE[ 0 ] = VEX_ONE_ID;
  else           SimulExpr1->VALUE[ 0 ] = VEX_ZERO_ID;

  return( SimulExpr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexAtom                     |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexAtom( Atom, Effec )

  vexexpr *Atom;
  short    Effec;
{
  vpndecl_list *VpnDeclar;
  vpnsym       *VpnSymbol;
  vasysimul     *SimulAtom;
  char         *SimulValue;
  char         *AtomValue;
  int           Position;
  int           Delta;
  short         Signed;

  AtomValue = GetVexAtomValue( Atom );

  if ( IsVexAtomSigned( Atom ) ) Signed = 1;
  else                           Signed = 0;

  SimulAtom  = VasySimulateAddSimul( Atom->WIDTH, Signed );
  SimulValue = SimulAtom->VALUE;

  if ( IsVexAtomLiteral( Atom ) )
  {
    for ( Position = 0; Position < Atom->WIDTH; Position++ )
    {
      SimulValue[ Position ] = getvexliteralid( AtomValue[ Position + 1 ] );
    }
  }
  else
  {
    VpnDeclar = searchvpndeclall( VasyFigure, AtomValue );
    Delta     = getvexvectorpos( VpnDeclar->VEX_ATOM, Atom->LEFT );

    for ( Position = 0; Position < Atom->WIDTH; Position++ )
    {
      VpnSymbol = &VpnDeclar->DECL_SYM[ Position + Delta ];

      if ( Effec ) SimulValue[ Position ] = VpnSymbol->EFFEC;
      else         SimulValue[ Position ] = VpnSymbol->DRIVE;
    }
  }

  return( SimulAtom );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexSlice                    |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexSlice( Expr )

  vexexpr *Expr;
{
  vasysimul   *SimulExpr1;
  vasysimul   *SimulExpr2;
  char       *SimulValue1;
  char       *SimulValue2;
  chain_list *ScanChain;
  vexexpr    *VexAtom;
  vexexpr    *Operand;
  long        Oper;
  int         IndexFrom;
  int         IndexTo;
  int         IndexMin;
  int         IndexMax;
  int         Index;
  int         Width;
  int         Position;
  int         ScanPos;
  short       Signed;

  Oper = GetVexOperValue( Expr );

  ScanChain   = Expr->OPERAND;
  VexAtom     = GetVexOperand( ScanChain );
  SimulExpr1  = VasySimulateVex( VexAtom );
  SimulValue1 = SimulExpr1->VALUE;

  ScanChain  = ScanChain->NEXT;
  Operand    = GetVexOperand( ScanChain );
  SimulExpr2 = VasySimulateVex( Operand );

  IndexFrom = VasySimulateIntegerSimul( SimulExpr2 );
  VasySimulateDelSimul( SimulExpr2 );

  if ( Oper != VEX_INDEX )
  {
    ScanChain  = ScanChain->NEXT;
    Operand    = GetVexOperand( ScanChain );
    SimulExpr2 = VasySimulateVex( Operand );

    IndexTo = VasySimulateIntegerSimul( SimulExpr2 );
    VasySimulateDelSimul( SimulExpr2 );
  }
  else
  {
    IndexTo = IndexFrom;
  }

  if ( IsVexAtomSigned( VexAtom ) ) Signed = 1;
  else                              Signed = 0;

  if ( Oper == VEX_DOWNTO )
  { 
    IndexMin = IndexTo;
    IndexMax = IndexFrom;
  }
  else
  {
    IndexMin = IndexFrom;
    IndexMax = IndexTo;
  }

  Width = ( IndexMax - IndexMin ) + 1;

  if ( Width <= 0 )
  {
    VasyErrorLine( VASY_ERROR_IN_SIMULATION, VasyVpnLine, "negative width" );
  }

  SimulExpr2  = VasySimulateAddSimul( Width, Signed );
  SimulValue2 = SimulExpr2->VALUE;

  Position = 0;

  for ( Index = IndexMin; Index <= IndexMax; Index++ )
  {
    ScanPos = getvexvectorpos( VexAtom, Index );

    if ( ScanPos == -1 )
    {
      VasyErrorLine( VASY_ERROR_IN_SIMULATION, VasyVpnLine, "index out of bound" );
    }

    SimulValue2[ Position ] = SimulValue1[ ScanPos ];
  }

  VasySimulateDelSimul( SimulExpr1 );

  return( SimulExpr2 );
}


/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexConcat                   |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexConcat( Expr )

  vexexpr *Expr;
{
  vasysimul        *SimulExpr;
  vasysimul        *SimulOper;
  char           *SimulValue;
  chain_list     *ScanChain;
  vexexpr        *Operand;
  int             Position;
  int             ScanPos;
  int             Width;

  SimulExpr  = VasySimulateAddSimul( Expr->WIDTH, 0 );
  SimulValue = SimulExpr->VALUE;
  Position  = 0;

  for ( ScanChain  = Expr->OPERAND;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Operand  = GetVexOperand( ScanChain );
    Width    = Operand->WIDTH;
    SimulOper = VasySimulateVex( Operand );

    for ( ScanPos = 0; ScanPos < Width; ScanPos++ )
    {
      SimulValue[ Position++ ] = SimulOper->VALUE[ ScanPos ];
    }

    VasySimulateDelSimul( SimulOper );
  }

  return( SimulExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexDriver                   |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexDriver( Expr )

  vexexpr *Expr;
{
  vexexpr  *Atom;
  vasysimul *SimulExpr;

  Atom = GetVexOperand( Expr->OPERAND );
  SimulExpr  = VasySimulateVexAtom( Atom, 0 );

  return( SimulExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexEvent                    |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexEvent( Expr )

  vexexpr *Expr;
{
  vpndecl_list   *VpnDeclar;
  vpnsym         *VpnSymbol;
  vexexpr        *Atom;
  vasysimul        *SimulExpr;
  char           *SimulValue;
  char           *AtomValue;
  int             Delta;

  SimulExpr  = VasySimulateAddSimul( 1, 0 );
  SimulValue = SimulExpr->VALUE;

  Atom      = GetVexOperand( Expr->OPERAND );
  AtomValue = GetVexAtomValue( Atom );

  VpnDeclar = searchvpndeclall( VasyFigure, AtomValue );
  Delta     = getvexvectorpos( VpnDeclar->VEX_ATOM, Atom->LEFT );
  VpnSymbol = &VpnDeclar->DECL_SYM[ Delta ];

  if ( VpnSymbol->EVENT ) SimulValue[ 0 ] = VEX_ONE_ID;
  else                    SimulValue[ 0 ] = VEX_ZERO_ID;

  return( SimulExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexLogic                    |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVexLogic( Expr )

  vexexpr *Expr;
{
  vasysimul   *SimulExpr1;
  vasysimul   *SimulExpr2;
  char       *SimulValue1;
  char       *SimulValue2;
  chain_list *ScanChain;
  vexexpr    *Operand;
  int         Value1;
  int         Value2;
  long        Oper;
  short       Width;
  int         Position;
  int         Negative;

  Oper  = GetVexOperValue( Expr );
  Width = Expr->WIDTH;

  Negative = isvexnegativeoper( Oper );

  if ( Negative ) Oper = getvexnotoper( Oper );

  if ( Oper == VEX_IFT ) Oper = VEX_AND;

  ScanChain  = Expr->OPERAND;
  Operand    = GetVexOperand( ScanChain );
  SimulExpr1  = VasySimulateVex( Operand );
  SimulValue1 = SimulExpr1->VALUE;

  ScanChain = ScanChain->NEXT;

  while ( ScanChain != (chain_list *)0 )
  {
    Operand    = GetVexOperand( ScanChain );
    SimulExpr2  = VasySimulateVex( Operand );
    SimulValue2 = SimulExpr2->VALUE;

    for ( Position = 0; Position < Width; Position++ )
    {
      Value1 = SimulValue1[ Position ];
      Value2 = SimulValue2[ Position ];

      if ( Oper == VEX_AND )
      {
        Value1 = VasyTruthTableAnd[ Value1 ][ Value2 ];
      }
      else
      if ( Oper == VEX_OR )
      {
        Value1 = VasyTruthTableOr[ Value1 ][ Value2 ];
      }
      else
      {
        Value1 = VasyTruthTableXor[ Value1 ][ Value2 ];
      }

      SimulValue1[ Position ] = Value1;
    }

    VasySimulateDelSimul( SimulExpr2 );
    ScanChain = ScanChain->NEXT;
  }

  if ( Negative )
  {
    for ( Position = 0; Position < Width; Position++ )
    {
      Value1 = SimulValue1[ Position ];

      SimulValue1[ Position ] = VasyTruthTableNot[ Value1 ];
    }
  }

  return( SimulExpr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVex                         |
|                                                             |
\------------------------------------------------------------*/

static vasysimul *VasySimulateVex( Expr )

  vexexpr *Expr;
{
  vasysimul  *SimulExpr;
  long       Oper;

  SimulExpr = (vasysimul *)0;

  if ( IsVexNodeAtom( Expr ) )
  {
    SimulExpr = VasySimulateVexAtom( Expr, 1 );
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( VasySimulOperFunc[ Oper ] == NULL )
    {
      VasyErrorLine( VASY_NOT_YET_IMPLEMENTED_ERROR, VasyVpnLine, "Operator" );
    }

    SimulExpr = (*VasySimulOperFunc[ Oper ])( Expr );
  }
  else
  {
    VasyErrorLine( VASY_NOT_YET_IMPLEMENTED_ERROR, VasyVpnLine,  "Function" );
  }

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  +++ VasySimulateVex " );
    viewvexexprboundln( Expr );
  }

  if ( IsVasyDebugLevel2() )
  {
    VasySimulateViewSimul( SimulExpr );
  }

  return( SimulExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexGuard                    |
|                                                             |
\------------------------------------------------------------*/

static int VasySimulateVexGuard( VpnFigure, VexGuard )

  vpnfig_list  *VpnFigure;
  vexexpr      *VexGuard;
{
  vasysimul *GuardSimul;
  char    *GuardValue;
  int      Guard;

  VasyFigure  = VpnFigure;
  GuardSimul  = VasySimulateVex( VexGuard );
  GuardValue = GuardSimul->VALUE;

  if ( GuardValue[ 0 ] == VEX_ONE_ID ) Guard = 1;
  else                                 Guard = 0;

  VasySimulateDelSimul( GuardSimul );

  return( Guard );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVexExpr                     |
|                                                             |
\------------------------------------------------------------*/

vasysimul *VasySimulateVexExpr( VpnFigure, VexAtom, VexExpr )

  vpnfig_list  *VpnFigure;
  vexexpr      *VexAtom;
  vexexpr      *VexExpr;
{
  vasysimul *SimulExpr;
  short    Signed;

  VasyFigure  = VpnFigure;
  SimulExpr   = VasySimulateVex( VexExpr );

  if ( IsVexAtomSigned( VexAtom ) ) Signed = 1;
  else                              Signed = 0;

  SimulExpr = VasySimulateResizeSimul( SimulExpr, VexAtom->WIDTH, Signed );

  return( SimulExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySimulateGetVpnActAtom                  |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasySimulateGetVpnActAtom( VpnFigure, VpnAction )

  vpnfig_list *VpnFigure;
  vpnact_list *VpnAction;
{
  vasysimul   *SimulExpr1;
  chain_list *ScanChain;
  vexexpr    *VexExpr;
  vexexpr    *VexAtom;
  vexexpr    *Operand;
  long        Oper;
  int         IndexFrom;
  int         IndexTo;
  int         IndexMin;
  int         IndexMax;
  int         Width;
  int         PosMin;
  int         PosMax;

  VexExpr = VpnAction->VEX_ATOM;

  if ( IsVexNodeAtom( VexExpr ) ) return( VexExpr );

  if ( ! IsVexNodeOper( VexExpr ) )
  {
    VasyErrorLine( VASY_ERROR_IN_SIMULATION, VpnAction->LINE, "bad assigment" );
  }

  Oper = GetVexOperValue( VexExpr );

  if ( ( Oper != VEX_INDEX  ) &&
       ( Oper != VEX_DOWNTO ) &&
       ( Oper != VEX_TO     ) )
  {
    VasyErrorLine( VASY_ERROR_IN_SIMULATION, VpnAction->LINE, "bad assigment" );
  }

  VasyFigure  = VpnFigure;
  VasyVpnLine = VpnAction->LINE;

  ScanChain  = VexExpr->OPERAND;
  VexAtom    = GetVexOperand( ScanChain );

  ScanChain  = ScanChain->NEXT;
  Operand    = GetVexOperand( ScanChain );
  SimulExpr1 = VasySimulateVex( Operand );

  IndexFrom  = VasySimulateIntegerSimul( SimulExpr1 );
  VasySimulateDelSimul( SimulExpr1 );

  if ( Oper != VEX_INDEX )
  {
    ScanChain  = ScanChain->NEXT;
    Operand    = GetVexOperand( ScanChain );
    SimulExpr1 = VasySimulateVex( Operand );

    IndexTo = VasySimulateIntegerSimul( SimulExpr1 );
    VasySimulateDelSimul( SimulExpr1 );
  }
  else
  {
    IndexTo = IndexFrom;
  }

  VexExpr = dupvexnode( VexAtom );

  if ( Oper == VEX_DOWNTO )
  {
    IndexMin = IndexTo;
    IndexMax = IndexFrom;

    SetVexAtomDown( VexExpr );
  }
  else
  {
    IndexMin = IndexFrom;
    IndexMax = IndexTo;

    SetVexAtomUp( VexExpr );
  }

  Width = ( IndexMax - IndexMin ) + 1;

  if ( Width <= 0 )
  {
    VasyErrorLine( VASY_ERROR_IN_SIMULATION, VpnAction->LINE, "negative width" );
  }

  PosMin = getvexvectorpos( VexAtom, IndexMin );
  PosMax = getvexvectorpos( VexAtom, IndexMax );

  if ( ( PosMin == -1 ) ||
       ( PosMax == -1 ) )
  {
    VasyErrorLine( VASY_ERROR_IN_SIMULATION, VpnAction->LINE, "index out of bound" );
  }

  VexExpr->LEFT  = IndexFrom;
  VexExpr->RIGHT = IndexTo;
  VexExpr->WIDTH = Width;

  SetVexAtomStaWidth( VexExpr );

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVpnAct                      |
|                                                             |
\------------------------------------------------------------*/

static void VasySimulateVpnAct( VpnFigure, VpnProc, VpnAction )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  vpnact_list  *VpnAction;
{
  vpndecl_list *AsgDeclar;
  vpnsym       *AsgSymbol;
  vexexpr      *AsgAtom;
  vexexpr      *AsgExpr;
  vasysimul     *AsgSimul;
  char         *AsgValue;
  char         *AtomValue;
  int           AsgWidth;
  int           AsgIndex;
  int           Position;

  VasyVpnLine = VpnAction->LINE;

  AsgAtom = VasySimulateGetVpnActAtom( VpnFigure, VpnAction );
  AsgExpr = VpnAction->VEX_EXPR;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, ">> Simulation Of " );
    viewvexexprbound( AsgAtom );
    fprintf( stdout, " : " );
    viewvexexprboundln( AsgExpr );
  }

  AsgSimul = VasySimulateVexExpr( VpnFigure, AsgAtom, AsgExpr );
  AsgValue = AsgSimul->VALUE;

  AtomValue = GetVexAtomValue( AsgAtom );
  AsgDeclar = searchvpndeclall( VpnFigure, AtomValue );
  AsgWidth  = AsgAtom->WIDTH;

  for ( Position = 0; Position < AsgWidth; Position++ )
  {
    AsgIndex  = getvexvectorindex( AsgAtom, Position );
    AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );

    if ( ( AsgDeclar->TYPE == VPN_DECLAR_DEFINE   ) ||
         ( AsgDeclar->TYPE == VPN_DECLAR_VARIABLE ) )
    {
      AsgSymbol->EFFEC = AsgValue[ Position ];
    }
    else
    {
      AsgSymbol->DRIVE = AsgValue[ Position ];
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    VasySimulateViewSimul( AsgSimul );
  }

  VasySimulateDelSimul( AsgSimul );

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "<<\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasySimulateVpnPlace                      |
|                                                             |
\------------------------------------------------------------*/

static int VasySimulateVpnPlace( VpnFigure, VpnProc, BeginPlace )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpnplace_list *BeginPlace;
{
  chain_list     *ScanTrans;
  vpnarc         *VpnArc;
  vpntrans_list  *VpnTrans;
  vpnplace_list  *VpnPlace;
  vpnact_list    *VpnAct;

  VpnPlace = (vpnplace_list *)0;

  do
  {
    if ( IsVasySimulateLoop( BeginPlace ) )
    {
      VasyErrorLine( VASY_LOOP_IN_SIMULATION, BeginPlace->LINE, BeginPlace->NAME );
    }

    IncVasySimulateLoop( BeginPlace );

    VpnTrans = (vpntrans_list *)0;

    for ( ScanTrans  = BeginPlace->TRANS_OUT;
          ScanTrans != (chain_list *)0;
          ScanTrans  = ScanTrans->NEXT )
    {
      VpnArc   = GetVpnArc( ScanTrans );
      VpnTrans = GetVpnArcTargetTrans( VpnArc );
      VpnArc   = GetVpnArc( VpnTrans->PLACE_OUT );
      VpnPlace = GetVpnArcTargetPlace( VpnArc );

      if ( VpnTrans->VEX_GUARD != (vexexpr *)0 )
      {
        VasyVpnLine = VpnPlace->LINE;

        if ( VasySimulateVexGuard( VpnFigure, VpnTrans->VEX_GUARD ) )
        {
          break;
        }
      }
    }

    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT )
    {
      VpnProc->ELABO = VpnTrans;

      BeginPlace->TOKEN = 1;

      break;
    }

    for ( VpnAct  = VpnTrans->ACT;
          VpnAct != (vpnact_list *)0;
          VpnAct  = VpnAct->NEXT )
    {
      VasySimulateVpnAct( VpnFigure, VpnProc, VpnAct );
    }

    BeginPlace = VpnPlace;
  }
  while ( 1 );

  for ( VpnPlace  = VpnProc->PLACE;
        VpnPlace != (vpnplace_list *)0;
        VpnPlace  = VpnPlace->NEXT )
  {
    ClearVasySimulateLoop( VpnPlace );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasySimulateVpnProc                     |
|                                                             |
\------------------------------------------------------------*/

void VasySimulateVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vpnplace_list *VpnPlace;
  vpnplace_list *ElaboPlace;
  vpntrans_list *ElaboTrans;
/*\
  vpntrans_list *VpnTrans;
  chain_list    *ScanTrans;
\*/
  vpnarc        *VpnArc;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasySimulateVpnProc %s\n", VpnProc->NAME );
  }

  ElaboTrans = VpnProc->ELABO;
  VpnArc     = GetVpnArc( ElaboTrans->PLACE_IN );
  ElaboPlace = GetVpnArcSourcePlace( VpnArc );

  VpnArc   = GetVpnArc( ElaboTrans->PLACE_OUT );
  VpnPlace = GetVpnArcTargetPlace( VpnArc );
/*
**  Change the first Sup_Wait transition
*/
  ElaboPlace->TOKEN = 0;

  VpnProc->ELABO = (vpntrans_list *)0;

  if ( ElaboTrans->TYPE == VPN_TRANS_SUP_WAIT )
  {
    ElaboTrans->TYPE = VPN_TRANS_IMMEDIATE;
  }
/*\
  for ( ScanTrans  = DelPlace->TRANS_IN;
        ScanTrans != (chain_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcSourceTrans( VpnArc );

    addvpnarctrans( VpnFigure, VpnTrans, VpnPlace );
  }

  delvpntrans( VpnFigure, ElaboTrans );
  delvpnplace( VpnFigure, ElaboPlace );

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }
\*/
/*
**  Start elaboration from the first process place
*/
  VasySimulateVpnPlace( VpnFigure, VpnProc, VpnPlace );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasySimulateVpnProc %s\n", VpnProc->NAME );
  }
}
