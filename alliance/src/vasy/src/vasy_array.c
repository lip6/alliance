/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| File    :                  vasy_array.c                     |
|                                                             |
| Authors :                Jacomme Ludovic                    |
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
# include "vex.h"
# include "vpn.h"
# include "vbh.h"

# include "vasy_array.h"
# include "vasy_debug.h"
# include "vasy_error.h"

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

  static authtable   *VasyHashNameArray  = (authtable   *)0; 
  static authtable   *VasyHashNameVector = (authtable   *)0;   
  static authtable   *VasyHashDeclar     = (authtable   *)0;
  static ptype_list **VasyPrevInst       = (ptype_list **)0;
  static vbpcs_list  *VasyProcess        = (vbpcs_list  *)0;
  static long         VasyNumberVar      = 0;
  static vbtyp_list  *VasyTypeNatural    = (vbtyp_list  *)0;

  static char VasyBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayGetIndexName                     |
|                                                             |
\------------------------------------------------------------*/

static char *VasyArrayGetIndexName( Name, Index )

  char *Name;
  int   Index;
{
  sprintf( VasyBuffer, "%s_idx_%d", Name, Index );
  return( namealloc( VasyBuffer ) );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayScanSens                         |
|                                                             |
\------------------------------------------------------------*/

static chain_list *VasyArrayScanSens( SensList )

  chain_list *SensList;
{
  vbtyp_list  *VbhType;
  vbtyp_list  *VbhBase;
  authelem    *Element;
  vexexpr     *VexAtom;
  chain_list  *ScanList;
  chain_list **PrevList;
  chain_list  *DelList;
  char        *Name;
  char        *IndexName;
  char        *NewName;
  long         Index;
  int          Step;
  int          Bit;
  int          ArrayMin;
  int          ArrayStep;
  int          ArrayMax;


  for ( ScanList  = SensList;
        ScanList != (chain_list *)0;
        ScanList  = ScanList->NEXT )
  {
    Name = getvexvectorname( ScanList->DATA, &Index );

    if ( Name == (char *)0 ) continue;

    Element = searchauthelem( VasyHashNameArray, Name );

    if ( Element != (authelem *)0 )
    {
      VbhType = (vbtyp_list *)Element->VALUE;
      VbhBase = VbhType->BASE;

      if ( Index == -1 )
      {
        ArrayMin = VbhType->LEFT;
        ArrayMax = VbhType->RIGHT;

        if ( ArrayMin < ArrayMax ) ArrayStep =  1;
        else                       ArrayStep = -1;
      }
      else
      {
        ArrayMin  = Index;
        ArrayMax  = Index;
        ArrayStep = 1;
      }

      for ( Index = ArrayMin; Index != (ArrayMax + ArrayStep); Index += ArrayStep )
      {
        IndexName = VasyArrayGetIndexName( Name, Index );

        if ( VbhBase->LEFT > VbhBase->RIGHT ) Step = -1;
        else                                  Step =  1;

        for ( Bit = VbhBase->LEFT; Bit != (VbhBase->RIGHT + Step); Bit += Step )
        {
          sprintf( VasyBuffer, "%s(%d)", IndexName, Bit );
          NewName  = namealloc( VasyBuffer );
          SensList = addchain( SensList, (void *)NewName );
        }
      }

      ScanList->DATA = (void *)0;
    }
    else
    if ( Index == -1 )
    {
      Element = searchauthelem( VasyHashDeclar, Name );

      if ( Element != (authelem *)0 )
      {
        VexAtom = (vexexpr *)Element->VALUE;

        ArrayMin = VexAtom->LEFT;
        ArrayMax = VexAtom->RIGHT;

        if ( ArrayMin < ArrayMax ) ArrayStep =  1;
        else                       ArrayStep = -1;

        for ( Index = ArrayMin; Index != (ArrayMax + ArrayStep); Index += ArrayStep )
        {
          sprintf( VasyBuffer, "%s(%ld)", Name, Index );
          NewName  = namealloc( VasyBuffer );
          SensList = addchain( SensList, (void *)NewName );
        }

        ScanList->DATA = (void *)0;
      }
      else
      {
        ScanList->DATA = (void *)Name;
      }
    }
  }

  ScanList = SensList;
  PrevList = &SensList;

  while ( ScanList != (chain_list *)0 )
  {
    if ( ScanList->DATA == (void *)0 )
    {
      DelList   = ScanList;
      ScanList  = ScanList->NEXT;
      *PrevList = ScanList;

      DelList->NEXT = (chain_list *)0;
      freechain( DelList );
    }
    else
    {
      PrevList = &ScanList->NEXT;
      ScanList = ScanList->NEXT;
    }
  }

  return( SensList );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayAddVariable                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyArrayAddVariable( BaseType, Width )

  vbtyp_list *BaseType;
  int         Width;
{
  vexexpr *VexAtom;
  char    *Name;

  sprintf( VasyBuffer, "array_var_%ld", VasyNumberVar++ );
  Name = namealloc( VasyBuffer ); 

  if ( Width == 0 )
  {
    VexAtom = createvexatomvec( Name, BaseType->LEFT, BaseType->RIGHT );
  }
  else
  if ( Width > 0 )
  {
    VexAtom = createvexatomvec( Name, Width - 1, 0 );
  }
  else
  {
    VexAtom = createvexatombit( Name );
  }

  VasyProcess->VARIABLE = 
    vbh_addvbvar( VasyProcess->VARIABLE, VexAtom, (vexexpr *)0, BaseType, VasyProcess->LINE );

  SetVbhProcSequential( VasyProcess );

  return( dupvexexpr( VexAtom ) );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayScanVexAtom                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyArrayScanVexAtom( VexExpr )

  vexexpr *VexExpr;
{
  char       *AtomValue;
  authelem   *Element;
  vbtyp_list *VbhType;
  vbtyp_list *VbhBase;
  vexexpr    *Operand;

  AtomValue = GetVexAtomValue( VexExpr );
  Element   = searchauthelem( VasyHashNameArray, AtomValue );

  if ( Element != (authelem *)0 )
  {
    if ( VexExpr->WIDTH != 1 )
    {
      VasyError( VASY_ERROR_ILLEGAL_USE_OF, "array in expression" );
    }

    VbhType   = (vbtyp_list *)Element->VALUE;
    VbhBase   = VbhType->BASE;
    AtomValue = VasyArrayGetIndexName( AtomValue, VexExpr->LEFT );

    Operand = createvexatomvec( AtomValue, VbhBase->LEFT, VbhBase->RIGHT );
    freevexexpr( VexExpr );

    VexExpr = Operand;
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayScanVexIndex                     |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyArrayScanVexIndex( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr    *VexAtom;
  vexexpr    *VexDeclar;
  vexexpr    *Operand;
  vexexpr    *VexVar;
  vexexpr    *VexData;
  vexexpr    *VexCond;
  vexexpr    *VexValue;
  authelem   *Element;
  char       *AtomValue;
  char       *ChoiceValue;
  vbtyp_list *VbhType;
  vbtyp_list *VbhBase;
  vbcho_list *VbhChoice;
  vbcas_list *VbhCase;
  ptype_list *ScanPType;
  int         ArraySize;
  int         Vector;
  int         Step;
  int         SizeValue;
  int         IntValue;
  int         Index;

  VexAtom   = GetVexOperand( VexExpr->OPERAND       );
  Operand   = GetVexOperand( VexExpr->OPERAND->NEXT );

  AtomValue = GetVexAtomValue( VexAtom );
  Element   = searchauthelem( VasyHashNameArray, AtomValue );

  if ( Element == (authelem *)0 )
  {
    Element = searchauthelem( VasyHashNameVector, AtomValue );

    if ( Element == (authelem *)0 )
    {
      viewvexexprboundln( VexExpr );
      VasyError( VASY_ERROR_ILLEGAL_USE_OF, "slice operator" );
    }

    VbhType = (vbtyp_list *)Element->VALUE;
    VbhBase = VbhType->BASE;

    Element = searchauthelem( VasyHashDeclar, AtomValue );

    if ( Element == (authelem *)0 )
    {
      viewvexexprboundln( VexExpr );
      VasyError( VASY_ERROR_ILLEGAL_USE_OF, "slice operator" );
    }

    VexDeclar = (vexexpr *)Element->VALUE;
    ArraySize = VexDeclar->WIDTH;

    if ( IsVexNodeDown( VexDeclar ) ) Step = -1;
    else                              Step =  1;

    SizeValue = getvexintervalnumbit( VexDeclar->LEFT, VexDeclar->RIGHT );
    Vector    = -1;
  }
  else
  {
    VbhType = (vbtyp_list *)Element->VALUE;
    VbhBase = VbhType->BASE;

    if ( VbhType->LEFT > VbhType->RIGHT )
    {
      ArraySize = VbhType->LEFT  - VbhType->RIGHT + 1;
      Step      = -1;
    }
    else
    {
      ArraySize = VbhType->RIGHT - VbhType->LEFT  + 1;
      Step      = 1;
    }

    SizeValue = getvexintervalnumbit( VbhType->LEFT, VbhType->RIGHT );
    Vector    = 0;
  }

  VexVar = VasyArrayAddVariable( VbhBase, Vector );

  if ( ! IsVexNodeAtom( Operand ) )
  {
    VexCond = VasyArrayAddVariable( VasyTypeNatural, SizeValue );
  }
  else
  {
    VexCond = Operand;
  }

  ScanPType = vbh_addvbcas( *VasyPrevInst, dupvexexpr( VexCond ), 
                             VasyTypeNatural, VasyProcess->LINE );
  *VasyPrevInst = ScanPType;
  VbhCase       = (vbcas_list *)ScanPType->DATA;

  if ( ! IsVexNodeAtom( Operand ) )
  {
    ScanPType = vbh_addvbvar( *VasyPrevInst, VexCond, dupvexexpr( Operand ),
                               VasyTypeNatural, VasyProcess->LINE );
    *VasyPrevInst = ScanPType;
  }

  VbhChoice = (vbcho_list *)autallocblock( sizeof( vbcho_list ) * ArraySize );
  VbhCase->SIZE   = ArraySize;
  VbhCase->CHOICE = VbhChoice;

  IntValue  = VbhType->LEFT;

  for ( Index = 0; Index < ArraySize; Index++ )
  {
    VexValue    = createvexatomlong( IntValue, SizeValue, 0 );
    ChoiceValue = GetVexAtomValue( VexValue );
    freevexexpr( VexValue );

    if ( Vector != -1 )
    {
      VexData = createvexatomvec( 
              VasyArrayGetIndexName( AtomValue, Index ), VbhBase->LEFT, VbhBase->RIGHT );
    }
    else
    {
      VexData = createvexatomvec( AtomValue, Index, Index );
    }

    ScanPType = vbh_addvbvar( (ptype_list *)0, 
          dupvexexpr( VexVar ), VexData, VbhBase, VasyProcess->LINE );

    VbhChoice[ Index ].INSTRUCTION = ScanPType;
    VbhChoice[ Index ].SIZE        = SizeValue;
    VbhChoice[ Index ].VALUES      = addchain( NULL, (void *)ChoiceValue );

    IntValue += Step;
  }

  freevexexpr( VexExpr );

  return( VexVar );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayScanVex                          |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyArrayScanVex( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr    *Operand;
  chain_list *ScanChain;
  long        Oper;

  if ( IsVexNodeAtom( VexExpr ) )
  {
    return( VasyArrayScanVexAtom( VexExpr ) );
  }

  if ( IsVexNodeOper( VexExpr ) )
  {
    Oper = GetVexOperValue( VexExpr );

    if ( Oper == VEX_INDEX )
    {
      VexExpr = VasyArrayScanVexIndex( VexExpr );
    }
    else
    if ( ( Oper == VEX_TO     ) ||
         ( Oper == VEX_DOWNTO ) )
    {
      viewvexexprln( VexExpr );
      VasyError( VASY_ERROR_ILLEGAL_USE_OF, "array slice in expression" );
    }
  }

  for ( ScanChain  = VexExpr->OPERAND;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Operand = GetVexOperand( ScanChain );
    Operand = VasyArrayScanVex( Operand );
    SetVexOperand( ScanChain, Operand );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayScanVexCond                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyArrayScanVexCond( PrevInst, VexExpr )

  ptype_list **PrevInst;
  vexexpr     *VexExpr;
{
  if ( VexExpr != (vexexpr *)0 )
  {
    VasyPrevInst = PrevInst;
    VexExpr = VasyArrayScanVex( VexExpr );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayScanAsg                          |
|                                                             |
\------------------------------------------------------------*/

static void VasyArrayScanAsg( PrevInst, CurInst, Variable )

  ptype_list **PrevInst;
  ptype_list  *CurInst;
  int          Variable;
{
  vbasg_list *ScanAsg;
  vbvar_list *ScanVar;
  vbvar_list *NewVar;
  vbifs_list *ScanIfs;
  vexexpr    *VexTarget;
  vexexpr    *VexDeclar;
  vexexpr    *VexAtom;
  vexexpr    *VexVar;
  vexexpr    *VexExpr;
  vexexpr    *VexValue;
  vexexpr    *VexData;
  vexexpr    *VexCond;
  vexexpr    *Operand;
  vbtyp_list *VbhType;
  vbtyp_list *VbhBase;
  ptype_list *ScanPType;
  char       *AtomValue;
  char       *DataValue;
  char       *Name;
  authelem   *Element;
  long        Oper;
  int         Vector;
  int         ArraySize;
  int         Step;
  int         SizeValue;
  int         IntValue;
  int         Index;
  int         Pos;
  int         PosMin;
  int         PosMax;
  int         PosStep;
  long        Line;


  VasyPrevInst = PrevInst;

  if ( Variable )
  {
    ScanVar   = (vbvar_list *)CurInst->DATA;
    ScanAsg   = (vbasg_list *)0;
    Line      = ScanVar->LINE;
    VexTarget = ScanVar->TARGET;
  }
  else
  {
    ScanAsg   = (vbasg_list *)CurInst->DATA;
    ScanVar   = (vbvar_list *)0;
    Line      = ScanAsg->LINE;
    VexTarget = ScanAsg->TARGET;
  }

  if ( IsVexNodeAtom( VexTarget ) )
  {
    AtomValue = GetVexAtomValue( VexTarget );
    Element   = searchauthelem( VasyHashNameArray, AtomValue );

    if ( Element != (authelem *)0 )
    {
      VbhType = (vbtyp_list *)Element->VALUE;
      VbhBase = VbhType->BASE;

      if ( VexTarget->WIDTH != 1 )
      {
        if ( Variable ) VexExpr = ScanVar->VEX;
        else            VexExpr = ScanAsg->VEX;

        if ( ( ! IsVexNodeAtom( VexExpr   ) ) ||
             (  IsVexAtomLiteral( VexExpr ) ) )
        {
          VasyError( VASY_ERROR_ILLEGAL_USE_OF, "slice assignation" );
        }

        DataValue = GetVexAtomValue( VexExpr );

        PosMin  = VexTarget->WIDTH - 1;
        PosMax  = -1;
        PosStep = -1;

        /* Check order for a same array variable assignation */

        if ( ( Variable               ) &&
             ( DataValue == AtomValue ) )
        {
          if ( ( ( IsVexAtomDown( VexTarget )      ) &&
                 ( VexTarget->LEFT < VexExpr->LEFT ) ) ||
               ( ( IsVexAtomUp( VexTarget )        ) &&
                 ( VexTarget->LEFT > VexExpr->LEFT ) ) )
          {
            PosMin  = 0;
            PosMax  = VexTarget->WIDTH;
            PosStep = 1;
          }
        }
        
        for ( Pos = PosMin; Pos != PosMax; Pos += PosStep )
        {
          Index   = getvexvectorindex( VexTarget, Pos );
          Name    = VasyArrayGetIndexName( AtomValue, Index );
          VexVar  = createvexatomvec( Name, VbhBase->LEFT, VbhBase->RIGHT );
          Index   = getvexvectorindex( VexExpr, Pos );
          Name    = VasyArrayGetIndexName( DataValue, Index );
          VexData = createvexatomvec( Name, VbhBase->LEFT, VbhBase->RIGHT );

          if ( Variable )
          {
            ScanPType = vbh_addvbvar( (ptype_list *)0, VexVar, VexData, VbhBase, Line );
          }
          else
          {
            ScanPType = vbh_addvbasg( (ptype_list *)0, VexVar, VexData, VbhBase, Line );
          }

          if ( CurInst != (ptype_list *)0 )
          {
            CurInst->TYPE = ScanPType->TYPE;
            CurInst->DATA = ScanPType->DATA;

            ScanPType->NEXT = (ptype_list *)0;
            freeptype( ScanPType );

            CurInst = (ptype_list *)0;
          }
          else
          {
            ScanPType->NEXT = *PrevInst;
            *PrevInst       = ScanPType;
          }
        }
      }
      else
      {
        AtomValue = VasyArrayGetIndexName( AtomValue, VexTarget->LEFT );

        Operand = createvexatomvec( AtomValue, VbhBase->LEFT, VbhBase->RIGHT );
        freevexexpr( VexTarget );
        VexTarget = Operand;

        if ( Variable )
        {
          ScanVar->TARGET = VexTarget;
          ScanVar->VEX    = VasyArrayScanVex( ScanVar->VEX );
        }
        else
        {
          ScanAsg->TARGET = VexTarget;
          ScanAsg->VEX    = VasyArrayScanVex( ScanAsg->VEX );
        }
      }
    }
    else
    {
      if ( Variable ) ScanVar->VEX = VasyArrayScanVex( ScanVar->VEX );
      else            ScanAsg->VEX = VasyArrayScanVex( ScanAsg->VEX );
    }
  }
  else
  if ( IsVexNodeOper( VexTarget ) )
  {
    Oper = GetVexOperValue( VexTarget );

    if ( Oper == VEX_INDEX )
    {
      if ( Variable )
      {
        ScanVar->VEX = VasyArrayScanVex( ScanVar->VEX );
        VexExpr = ScanVar->VEX;
      }
      else
      {
        ScanAsg->VEX = VasyArrayScanVex( ScanAsg->VEX );
        VexExpr = ScanAsg->VEX;
      }

      VexAtom   = GetVexOperand( VexTarget->OPERAND       );
      Operand   = GetVexOperand( VexTarget->OPERAND->NEXT );

      AtomValue = GetVexAtomValue( VexAtom );
      Element   = searchauthelem( VasyHashNameArray, AtomValue );

      if ( Element == (authelem *)0 )
      {
        Element = searchauthelem( VasyHashNameVector, AtomValue );
    
        if ( Element == (authelem *)0 )
        {
          viewvexexprboundln( VexTarget );
          VasyError( VASY_ERROR_ILLEGAL_USE_OF, "slice operator" );
        }
    
        VbhType = (vbtyp_list *)Element->VALUE;
        VbhBase = VbhType->BASE;
    
        Element = searchauthelem( VasyHashDeclar, AtomValue );
    
        if ( Element == (authelem *)0 )
        {
          viewvexexprboundln( VexTarget );
          VasyError( VASY_ERROR_ILLEGAL_USE_OF, "slice operator" );
        }
    
        VexDeclar = (vexexpr *)Element->VALUE;
    
        ArraySize = VexDeclar->WIDTH;
    
        if ( IsVexNodeDown( VexDeclar ) ) Step = -1;
        else                              Step =  1;
    
        SizeValue = getvexintervalnumbit( VexDeclar->LEFT, VexDeclar->RIGHT );
        Vector    = -1;
      }
      else
      {
        VbhType   = (vbtyp_list *)Element->VALUE;
        VbhBase   = VbhType->BASE;
  
        if ( VbhType->LEFT > VbhType->RIGHT )
        {
          ArraySize = VbhType->LEFT  - VbhType->RIGHT + 1;
          Step      = -1;
        }
        else
        {
          ArraySize = VbhType->RIGHT - VbhType->LEFT  + 1;
          Step      = 1;
        }

        SizeValue = getvexintervalnumbit( VbhType->LEFT, VbhType->RIGHT );
        Vector    = 0;
      }

      VexVar    = VasyArrayAddVariable( VbhBase, Vector );
      ScanPType = vbh_addvbvar( (ptype_list *)0, VexVar,
                                dupvexexpr( VexExpr ), VbhBase, Line );

      CurInst->TYPE = ScanPType->TYPE;
      CurInst->DATA = ScanPType->DATA;

      ScanPType->NEXT = (ptype_list *)0;
      freeptype( ScanPType );

      NewVar       = (vbvar_list *)CurInst->DATA;
      NewVar->VEX  = VasyArrayScanVex( NewVar->VEX );

      ScanPType = CurInst->NEXT;

      if ( ! IsVexNodeAtom( Operand ) )
      {
        VexCond   = VasyArrayAddVariable( VasyTypeNatural, SizeValue );
        ScanPType = vbh_addvbvar( ScanPType, VexCond,
                                  dupvexexpr( Operand ), VasyTypeNatural, Line );
      }

      if ( Vector == -1 ) VbhBase = VbhType;

      IntValue  = VbhType->LEFT;

      for ( Index = 0; Index < ArraySize; Index++ )
      {
        VexValue = createvexatomlong( IntValue, SizeValue, 0 );
        VexCond  = createvexbinexpr( VEX_EQ, 1, dupvexexpr( Operand ), VexValue );

        if ( Vector != -1 )
        {
          VexData = createvexatomvec( 
                   VasyArrayGetIndexName( AtomValue, Index ), VbhBase->LEFT, VbhBase->RIGHT );
        }
        else
        {
          VexData = createvexatomvec( AtomValue, Index, Index );
        }

        ScanPType = vbh_addvbifs( ScanPType, VexCond, Line );
        ScanIfs = (vbifs_list *)ScanPType->DATA;

        if ( Variable )
        {
          ScanIfs->CNDTRUE = vbh_addvbvar( (ptype_list *)0,
                                VexData, dupvexexpr( VexVar ), VbhBase, Line );
        }
        else
        {
          ScanIfs->CNDTRUE = vbh_addvbasg( (ptype_list *)0,
                                VexData, dupvexexpr( VexVar ), VbhBase, Line );
        }

        IntValue += Step;
      }

      CurInst->NEXT = ScanPType;

      if ( Variable ) vbh_frevbasg( ScanVar );
      else            vbh_frevbasg( ScanAsg );
    }
    else
    if ( ( Oper == VEX_TO     ) ||
         ( Oper == VEX_DOWNTO ) )
    {
      viewvexexprboundln( VexTarget );
      VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "array slice in assignation" );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyArrayScanInstruction                  |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *VasyArrayScanInstruction( Instruction )

  ptype_list *Instruction;
{
  ptype_list  *ScanPType;
  ptype_list **PrevPType;
  vbifs_list  *ScanIfs;
  vbcas_list  *ScanCase;
  vbcho_list  *ScanChoice;
  vblop_list  *ScanLoop;
  vbfor_list  *ScanFor;
  vbwhi_list  *ScanWhile;
  vbwas_list  *ScanWait;
  vbnxt_list  *ScanNext;
  vbext_list  *ScanExit;
  int          Offset;

  ScanPType = Instruction;
  PrevPType = &Instruction;

  while ( ScanPType != (ptype_list *)0 )
  {
    switch ( ScanPType->TYPE )
    {
      case VBH_BEIFS : 

        ScanIfs = (vbifs_list *)ScanPType->DATA;
        ScanIfs->CND      = VasyArrayScanVexCond( PrevPType, ScanIfs->CND );
        ScanIfs->CNDTRUE  = VasyArrayScanInstruction( ScanIfs->CNDTRUE  );
        ScanIfs->CNDFALSE = VasyArrayScanInstruction( ScanIfs->CNDFALSE );

      break;

      case VBH_BECAS : 

        ScanCase = (vbcas_list *)ScanPType->DATA;
        ScanCase->VEX = VasyArrayScanVexCond( PrevPType, ScanCase->VEX );

        for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
        {
          ScanChoice = &ScanCase->CHOICE[ Offset ];
          ScanChoice->INSTRUCTION = VasyArrayScanInstruction( ScanChoice->INSTRUCTION );
        }

      break;

      case VBH_BEFOR : 

        ScanFor = (vbfor_list *)ScanPType->DATA;
        ScanFor->INSTRUCTION = VasyArrayScanInstruction( ScanFor->INSTRUCTION );

        continue;

      break;

      case VBH_BEWHI : 

        ScanWhile = (vbwhi_list *)ScanPType->DATA;
        ScanWhile->CND         = VasyArrayScanVexCond( PrevPType, ScanWhile->CND );
        ScanWhile->INSTRUCTION = VasyArrayScanInstruction( ScanWhile->INSTRUCTION );

      break;

      case VBH_BELOP : 

        ScanLoop = (vblop_list *)ScanPType->DATA;
        ScanLoop->INSTRUCTION = VasyArrayScanInstruction( ScanLoop->INSTRUCTION );

      break;

      case VBH_BEASG : 

        VasyArrayScanAsg( PrevPType, ScanPType, 0 );

      break;

      case VBH_BEVAR :

        VasyArrayScanAsg( PrevPType, ScanPType, 1 );

      break;

      case VBH_BEWAS :

        ScanWait = (vbwas_list *)ScanPType->DATA;
        ScanWait->CND = VasyArrayScanVexCond( PrevPType, ScanWait->CND );
        ScanWait->SEN = VasyArrayScanSens( ScanWait->SEN );

      break;

      case VBH_BENXT :

        ScanNext = (vbnxt_list *)ScanPType->DATA;
        ScanNext->CND = VasyArrayScanVexCond( PrevPType, ScanNext->CND );

      break;

      case VBH_BEEXT :

        ScanExit = (vbext_list *)ScanPType->DATA;
        ScanExit->CND = VasyArrayScanVexCond( PrevPType, ScanExit->CND );

      break;

      case VBH_BEAGR :

        VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "aggregate assignation" );

      break;
    }

    PrevPType = &ScanPType->NEXT;
    ScanPType = ScanPType->NEXT;
  }

  return( Instruction );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyExpandArrayScanProcess            |
|                                                             |
\------------------------------------------------------------*/

static void VasyArrayScanProcess( BePcs )

  vbpcs_list *BePcs;
{
  VasyProcess = BePcs;

  BePcs->INSTRUCTION = VasyArrayScanInstruction( BePcs->INSTRUCTION );
  BePcs->SENSITIVITY = VasyArrayScanSens( BePcs->SENSITIVITY );
}


/*------------------------------------------------------------\
|                                                             |
|                       VasyArrayCheckType                    |
|                                                             |
\------------------------------------------------------------*/

static int VasyArrayCheckType( VexAtom, VbhType )

  vexexpr    *VexAtom;
  vbtyp_list *VbhType;
{
  vbtyp_list *VbhBaseType;
  char       *AtomValue;
  short       Array;
  short       Vector;

  Array  = 0;
  Vector = 0;

  if ( IsVasyVbhTypeArray( VbhType ) )
  {
    Array = 1;
  }
  else
  if ( IsVasyVbhTypeVector( VbhType ) )
  {
    Vector = 1;
  }
  else
  if ( VbhType->INDEX >= VBH_MAX_TYPE )
  {
    if ( VbhType->DYNAMIC )
    {
      VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "dynamic size user type" );
    }

    if ( VbhType->CLASS == 'U' )
    {
      VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "user unconstrained array type" );
    }
    else
    if ( VbhType->CLASS == 'A' )
    {
      VbhBaseType = VbhType->BASE;

      if ( VbhBaseType->INDEX >= VBH_MAX_TYPE   )
      {
         if ( VbhBaseType->DYNAMIC )
         {
           VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "dynamic size user type" );
         }

        if ( VbhBaseType->CLASS == 'A' )
        {
          Array = 1;
          SetVasyVbhTypeArray( VbhType );
        }
        else
        if ( VbhBaseType->CLASS == 'U' )
        {
          VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "user unconstrained array subtype" );
        }
      }
      else
      if ( isvextypevector( VbhType->INDEX ) )
      {
        Array = 1;
        SetVasyVbhTypeArray( VbhType );
      }
      else
      {
        Vector = 1;
        SetVasyVbhTypeVector( VbhType );
      }
    }
  }
  else 
  if ( isvextypevector( VbhType->INDEX ) )
  {
    Vector = 1;
    SetVasyVbhTypeVector( VbhType );
  }

  if ( Array )
  {
    AtomValue = GetVexAtomValue( VexAtom );
    addauthelem( VasyHashNameArray, AtomValue, (long)VbhType );
  }
  else
  if ( Vector )
  {
    AtomValue = GetVexAtomValue( VexAtom );
    addauthelem( VasyHashNameVector, AtomValue, (long)VbhType );
    addauthelem( VasyHashDeclar    , AtomValue, (long)VexAtom );
  }

  return( Array );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyArrayCleanDeclar                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyArrayCleanDeclar( VbhFigure )

  vbfig_list *VbhFigure;
{
  vbcst_list  *BeCst;
  vbcst_list  *DelCst;
  vbcst_list **PrevCst;
  vbaux_list  *BeAux;
  vbaux_list  *DelAux;
  vbaux_list **PrevAux;
  vbpcs_list  *BePcs;
  vbvar_list  *BeVar;
  vbtyp_list  *BeType;
  ptype_list  *ScanPtype;
  ptype_list **PrevPType;
  ptype_list  *DelPType;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyArrayCleanDeclar %s\n", VbhFigure->NAME );
  }

  BeCst   =  VbhFigure->BECST;
  PrevCst = &VbhFigure->BECST;

  while ( BeCst != (vbcst_list *)0 )
  {
    BeType  = BeCst->TYPE;

    if ( IsVasyVbhTypeArray( BeType ) )
    {
      DelCst   = BeCst;
      BeCst    = BeCst->NEXT;
      *PrevCst = BeCst;

      DelCst->NEXT = (vbcst_list *)0;
      vbh_frevbcst( DelCst );
    }
    else
    {
      PrevCst = &BeCst->NEXT;
      BeCst   = BeCst->NEXT;
    }
  }

  BeAux   =  VbhFigure->BEAUX;
  PrevAux = &VbhFigure->BEAUX;

  while ( BeAux != (vbaux_list *)0 )
  {
    BeType  = BeAux->TYPE;

    if ( IsVasyVbhTypeArray( BeType ) )
    {
      DelAux   = BeAux;
      BeAux    = BeAux->NEXT;
      *PrevAux = BeAux;

      DelAux->NEXT = (vbaux_list *)0;
      vbh_frevbaux( DelAux );
    }
    else
    {
      PrevAux = &BeAux->NEXT;
      BeAux   = BeAux->NEXT;
    }
  }

  for ( BePcs  = VbhFigure->BEPCS;
        BePcs != (vbpcs_list *)0;
        BePcs  = BePcs->NEXT )
  {
    ScanPtype =  BePcs->VARIABLE;
    PrevPType = &BePcs->VARIABLE;

    while ( ScanPtype != (ptype_list *)0 )
    {
      BeVar   = (vbvar_list *)ScanPtype->DATA;
      BeType  = BeVar->TYPE;

      if ( IsVasyVbhTypeArray( BeType ) )
      {
        vbh_frevbvar( BeVar );

        DelPType   = ScanPtype;
        ScanPtype  = ScanPtype->NEXT;
        *PrevPType = ScanPtype;

        DelPType->NEXT = (ptype_list *)0;
        freeptype( DelPType );
      }
      else
      {
        PrevPType  = &ScanPtype->NEXT;
        ScanPtype  = ScanPtype->NEXT;
      }
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyArrayCleanDeclar\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyArrayCheckDeclar                  |
|                                                             |
\------------------------------------------------------------*/

static int VasyArrayCheckDeclar( VbhFigure )

  vbfig_list *VbhFigure;
{
  vbpor_list *BePor;
  vbaux_list *BeAux;
  vbcst_list *BeCst;
  vbpcs_list *BePcs;
  vbvar_list *BeVar;
  vbtyp_list *BeType;
  vbtyp_list *BaseType;
  ptype_list *ScanPtype;
  chain_list *ScanOper;
  vexexpr    *VexAtom;
  vexexpr    *NewAtom;
  vexexpr    *VexInit;
  vexexpr    *Operand; 
  char       *AtomValue;
  char       *NewName;
  int         Found;
  int         Step;
  int         Index;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyArrayCheckDeclar %s\n", VbhFigure->NAME );
  }

  Found = 0;

  for ( BePor  = VbhFigure->BEPOR;
        BePor != (vbpor_list *)0;
        BePor  = BePor->NEXT )
  {
    BeType  = BePor->TYPE;
    VexAtom = BePor->TARGET;

    if ( VasyArrayCheckType( VexAtom, BeType ) )
    {
      VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "port: array type" );
    }
  }

  for ( BeAux  = VbhFigure->BEAUX;
        BeAux != (vbaux_list *)0;
        BeAux  = BeAux->NEXT )
  {
    BeType  = BeAux->TYPE;
    VexAtom = BeAux->TARGET;

    if ( VasyArrayCheckType( VexAtom, BeType ) )
    {
      if ( BeAux->VEX != (vexexpr *)0 )
      {
        VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "array initialisation" );
      }

      BaseType  = BeType->BASE;
      AtomValue = GetVexAtomValue( VexAtom );

      if ( BeType->LEFT < BeType->RIGHT ) Step =  1;
      else                                Step = -1;

      for ( Index = BeType->LEFT; Index != (BeType->RIGHT + Step); Index += Step )
      {
        NewName = VasyArrayGetIndexName( AtomValue, Index );
        NewAtom = createvexatomvec( NewName, BaseType->LEFT, BaseType->RIGHT );

        VbhFigure->BEAUX = vbh_addvbaux( VbhFigure->BEAUX, NewAtom, (vexexpr *)0,
                                         BeAux->KIND, BaseType, BeAux->LINE );
      }

      Found = 1;
    }
  }

  for ( BeCst  = VbhFigure->BECST;
        BeCst != (vbcst_list *)0;
        BeCst  = BeCst->NEXT )
  {
    BeType  = BeCst->TYPE;
    VexAtom = BeCst->TARGET;

    if ( VasyArrayCheckType( VexAtom, BeType ) )
    {
      BaseType  = BeType->BASE;
      AtomValue = GetVexAtomValue( VexAtom );
      VexInit   = BeCst->VEX;

      if ( ( VexInit == (vexexpr *)0                 ) ||
           ( ! IsVexNodeOper( VexInit )              ) ||
           ( GetVexOperValue( VexInit ) != VEX_ARRAY ) )
      {
        VasyError( VASY_ERROR_ILLEGAL_USE_OF, "constant array initialisation" );
      }

      ScanOper = VexInit->OPERAND;

      if ( BeType->LEFT < BeType->RIGHT ) Step =  1;
      else                                Step = -1;

      for ( Index = BeType->LEFT; Index != (BeType->RIGHT + Step); Index += Step )
      {
        if ( ScanOper == (chain_list *)0 )
        {
          VasyError( VASY_ERROR_ILLEGAL_USE_OF, "constant array initialisation" );
        }

        Operand = GetVexOperand( ScanOper );
        SetVexOperand( ScanOper, (vexexpr *)0 );

        NewName = VasyArrayGetIndexName( AtomValue, Index );
        NewAtom = createvexatomvec( NewName, BaseType->LEFT, BaseType->RIGHT );

        VbhFigure->BECST = vbh_addvbcst( VbhFigure->BECST, NewAtom, Operand,
                                         BaseType, BeCst->LINE );

        ScanOper = ScanOper->NEXT;
      }

      freevexexpr( VexInit );
      BeCst->VEX = (vexexpr *)0;

      Found = 1;
    }
  }

  for ( BePcs  = VbhFigure->BEPCS;
        BePcs != (vbpcs_list *)0;
        BePcs  = BePcs->NEXT )
  {
    for ( ScanPtype  = BePcs->VARIABLE;
          ScanPtype != (ptype_list *)0;
          ScanPtype  = ScanPtype->NEXT )
    {
      BeVar   = (vbvar_list *)ScanPtype->DATA;
      BeType  = BeVar->TYPE;
      VexAtom = BeVar->TARGET;

      if ( VasyArrayCheckType( BeVar->TARGET, BeVar->TYPE ) )
      {
        BaseType  = BeType->BASE;
        AtomValue = GetVexAtomValue( VexAtom );
  
        if ( BeType->LEFT < BeType->RIGHT ) Step =  1;
        else                                Step = -1;
  
        for ( Index = BeType->LEFT; Index != (BeType->RIGHT + Step); Index += Step )
        {
          NewName = VasyArrayGetIndexName( AtomValue, Index );
          NewAtom = createvexatomvec( NewName, BaseType->LEFT, BaseType->RIGHT );
  
          BePcs->VARIABLE = vbh_addvbvar( BePcs->VARIABLE, NewAtom, (vexexpr *)0,
                                          BaseType, BeVar->LINE );
        }
  
        Found = 1;
      }
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyArrayCheckDeclar %d\n", Found );
  }

  return( Found );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyExpandArrayVbhFig                 |
|                                                             |
\------------------------------------------------------------*/

void VasyExpandArrayVbhFig( VbhFigure )

 vbfig_list *VbhFigure;
{
  vbpcs_list *BePcs;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyExpandArrayVbhFig %s\n", VbhFigure->NAME );
  }

  VasyHashNameArray  = createauthtable( 100 );
  VasyHashNameVector = createauthtable( 100 );
  VasyHashDeclar     = createauthtable( 100 );

  VasyTypeNatural = vbh_getvbtypbyindex( VbhFigure, VBH_TYPE_NATURAL );

  VasyArrayCheckDeclar( VbhFigure );

  for ( BePcs  = VbhFigure->BEPCS;
        BePcs != (vbpcs_list *)0;
        BePcs  = BePcs->NEXT )
  {
    VasyArrayScanProcess( BePcs );
  }

  VasyArrayCleanDeclar( VbhFigure );

  if ( IsVasyDebugLevel1() )
  {
    vbh_viewvbfig( VbhFigure );
  }

  destroyauthtable( VasyHashNameArray  );
  destroyauthtable( VasyHashNameVector );
  destroyauthtable( VasyHashDeclar     );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyExpandArrayVbhFig %s\n\n", VbhFigure->NAME );
  }
}
