/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
| Tool    :                     Rtl                           |
|                                                             |
| File    :                   rtnadd.c                        |
|                                                             |
| Date    :                   04.07.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "rtn.h"

# include <stdio.h>
# include "rtnadd.h"
# include "rtnerror.h"

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

  rtlfig_list *HEAD_RTLFIG = (rtlfig_list *)0;

  char *RTL_DECLAR_TYPE[ RTL_MAX_DECLAR_TYPE ] =
  {
    "PORT",
    "SIGNAL",
    "CONSTANT",
    "GENERIC"
  };

  char *RTL_KIND_TYPE[ RTL_MAX_KIND_TYPE ] =
  {
    "NONE",
    "BUS",
    "REGISTER"
  };

  char *RTL_DIR_TYPE[ RTL_MAX_DIR_TYPE ] =
  {
    "IN",
    "OUT",
    "INOUT"
  };

  char *RTL_BIVEX_TYPE[ RTL_MAX_BIVEX_TYPE ] =
  {
    "NONE",
    "FALLING_EDGE",
    "RISING_EDGE",
    "ASYNC_RESET",
    "ASYNC_SET",
    "ASYNC_WEN",
    "SYNC_RESET",
    "SYNC_SET",
    "SYNC_WEN",
    "MULTIPLEXOR",
    "CONDITIONAL"
  };

  char *RTL_ASG_TYPE[ RTL_MAX_ASG_TYPE ] =
  {
    "COMBINATORIAL",
    "MULTIPLEXOR",
    "CONDITIONAL",
    "REGISTER",
    "TRISTATE",
    "PULL_UP",
    "PULL_DOWN"
  };

  char *RTL_ASG_REGISTER_TYPE[ RTL_MAX_ASG_REGISTER_TYPE ] =
  {
    "NONE",
    "SYNC",
    "ASYNC",
    "MIXED"
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Rtl Add Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Figure                       |
|                                                             |
\------------------------------------------------------------*/

rtlfig_list *addrtlfig( Name )

   char *Name;
{
  rtlfig_list *Figure;

  Name   = namealloc( Name );
  Figure = allocrtlfig();

  Figure->NAME          = Name;
  Figure->HASH_DECLAR   = createauthtable( 50 );
  Figure->HASH_MODEL    = createauthtable( 50 );
  Figure->HASH_INSTANCE = createauthtable( 50 );

  Figure->NEXT = HEAD_RTLFIG;
  HEAD_RTLFIG  = Figure;

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Declaration                  |
|                                                             |
\------------------------------------------------------------*/

rtldecl_list *addrtldecl( Figure, Atom, Type )

  rtlfig_list *Figure;
  vexexpr     *Atom;
  int          Type;
{
  rtldecl_list  *Decl;
  rtldecl_list **PrevDecl;
  rtlsym        *Sym;
  char          *Name;
  int            Width;
  int            Index;
  int            IndexStep;
  int            Scan;

  Name = GetVexAtomValue( Atom );
  Decl = searchrtldecl( Figure, Name );

  if ( Decl != (rtldecl_list *)0 )
  {
    rtlerror( RTL_DECLAR_EXIST_ERROR, Name, 0 );
  }

  if ( ( Type <  0                   ) ||
       ( Type >= RTL_MAX_DECLAR_TYPE ) )
  {
    rtlerror( RTL_UNKNOWN_DECLAR_TYPE, Type, Name );
  }

  Decl = allocrtldecl();

  Width = Atom->WIDTH;
  Sym   = allocrtlsym( Width );

  if ( ( ! IsVexNodeVarWidth( Atom ) ) &&
       ( IsVexAtomVector( Atom )     ) )
  {
    Index     = Atom->LEFT;
    IndexStep = IsVexAtomDown( Atom ) ? -1 : 1;
  }
  else
  {
    Index     = -1;
    IndexStep = 0;
  }

  for ( Scan = 0; Scan < Width; Scan++ )
  {
    Sym[ Scan ].NAME  = Name;
    Sym[ Scan ].INDEX = Index;
    Sym[ Scan ].DECL  = Decl;

    Index += IndexStep;
  }

  PrevDecl = &Figure->DECLAR[ Type ];

  Decl->VEX_ATOM  = Atom;
  Decl->DECL_SYM  = Sym;
  Decl->TYPE      = Type;

  Decl->NEXT             = Figure->DECLAR[ Type ];
  Figure->DECLAR[ Type ] = Decl;

  addauthelem( Figure->HASH_DECLAR, Name, (long)Decl );

  return( Decl );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Assign                       |
|                                                             |
\------------------------------------------------------------*/

rtlasg_list *addrtlasg( Figure, VexAtom, Type )

  rtlfig_list *Figure;
  vexexpr     *VexAtom;
  int          Type;
{
  rtlasg_list *Assign;

  Assign = allocrtlasg();

  Assign->VEX_ATOM   = VexAtom;
  Assign->TYPE       = Type;
  Assign->LAST_BIVEX = &Assign->BIVEX;

  Assign->NEXT   = Figure->ASSIGN;
  Assign->PREV   = &Figure->ASSIGN;

  if ( Figure->ASSIGN != (rtlasg_list *)0 )
  {
    Figure->ASSIGN->PREV = &Assign->NEXT;
  }

  Figure->ASSIGN = Assign;

  return( Assign );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Assign After                 |
|                                                             |
\------------------------------------------------------------*/

rtlasg_list *addrtlasgafter( Figure, PrevAssign, VexAtom, Type )

  rtlfig_list *Figure;
  rtlasg_list *PrevAssign;
  vexexpr     *VexAtom;
  int          Type;
{
  rtlasg_list *NextAssign;
  rtlasg_list *Assign;

  Assign = allocrtlasg();

  Assign->VEX_ATOM   = VexAtom;
  Assign->TYPE       = Type;
  Assign->LAST_BIVEX = &Assign->BIVEX;

  if ( PrevAssign == (rtlasg_list *)0 )
  {
    NextAssign     = Figure->ASSIGN;
    Figure->ASSIGN = Assign;
    Assign->PREV   = &Figure->ASSIGN;
  }
  else
  {
    NextAssign       = PrevAssign->NEXT;
    PrevAssign->NEXT = Assign;
    Assign->PREV     = &PrevAssign->NEXT;
  }

  Assign->NEXT = NextAssign;

  if ( NextAssign != (rtlasg_list *)0 )
  {
    NextAssign->PREV = &Assign->NEXT;
  }

  return( Assign );
}

/*------------------------------------------------------------\
|                                                             |
|                  Rtl Add Assign Bi Vex                      |
|                                                             |
\------------------------------------------------------------*/

rtlbivex_list *addrtlasgbivex( Figure, Assign, VexCond, VexData, Type  )

  rtlfig_list *Figure;
  rtlasg_list *Assign;
  vexexpr     *VexCond;
  vexexpr     *VexData;
  int          Type;
{
  rtlbivex_list *BiVex;

  BiVex = allocrtlbivex();

  BiVex->VEX_COND = VexCond;
  BiVex->VEX_DATA = VexData;
  BiVex->TYPE     = Type;

  *Assign->LAST_BIVEX = BiVex;
  Assign->LAST_BIVEX  = &BiVex->NEXT;

  return( BiVex );
}

/*------------------------------------------------------------\
|                                                             |
|                    Rtl Add Fsm State Assign                 |
|                                                             |
\------------------------------------------------------------*/

rtlfsmasg_list *addrtlfsmstateasg( Figure, Fsm, State,  VexAtom, VexData )

  rtlfig_list      *Figure;
  rtlfsm_list      *Fsm;
  rtlfsmstate_list *State;
  vexexpr          *VexAtom;
  vexexpr          *VexData;
{
  rtlfsmasg_list *Assign;

  Assign = allocrtlfsmasg();

  Assign->VEX_ATOM = VexAtom;
  Assign->VEX_DATA = VexData;

  Assign->NEXT  = State->ASSIGN;
  State->ASSIGN = Assign;

  return( Assign );
}

/*------------------------------------------------------------\
|                                                             |
|                    Rtl Add Fsm Trans Assign                 |
|                                                             |
\------------------------------------------------------------*/

rtlfsmasg_list *addrtlfsmtransasg( Figure, Fsm, Trans,  VexAtom, VexData )

  rtlfig_list      *Figure;
  rtlfsm_list      *Fsm;
  rtlfsmtrans_list *Trans;
  vexexpr          *VexAtom;
  vexexpr          *VexData;
{
  rtlfsmasg_list *Assign;

  Assign = allocrtlfsmasg();

  Assign->VEX_ATOM = VexAtom;
  Assign->VEX_DATA = VexData;

  Assign->NEXT  = Trans->ASSIGN;
  Trans->ASSIGN = Assign;

  return( Assign );
}

/*------------------------------------------------------------\
|                                                             |
|                          Rtl Add Fsm Trans                  |
|                                                             |
\------------------------------------------------------------*/

rtlfsmtrans_list *addrtlfsmtrans( Figure, Fsm, StateFrom, StateTo, VexCond )

  rtlfig_list      *Figure;
  rtlfsm_list      *Fsm;
  rtlfsmstate_list *StateFrom;
  rtlfsmstate_list *StateTo;
  vexexpr          *VexCond;
{
  rtlfsmtrans_list *Trans;

  Trans = allocrtlfsmtrans();

  Trans->FROM     = StateFrom;
  Trans->TO       = StateTo;
  Trans->VEX_COND = VexCond;

  StateFrom->FROM = addchain( StateFrom->FROM, (void *)Trans );
  StateTo->TO     = addchain( StateTo->TO    , (void *)Trans );

  Trans->NEXT = Fsm->TRANS;
  Fsm->TRANS  = Trans;

  return( Trans );
}

/*------------------------------------------------------------\
|                                                             |
|                          Rtl Add Fsm State                  |
|                                                             |
\------------------------------------------------------------*/

rtlfsmstate_list *addrtlfsmstate( Figure, Fsm, Name )

  rtlfig_list  *Figure;
  rtlfsm_list  *Fsm;
  char         *Name;
{
  rtlfsmstate_list *State;

  Name  = namealloc( Name );
  State = allocrtlfsmstate();

  State->NAME = Name;

  addauthelem( Fsm->HASH_STATE, Name, (long)State );

  State->NEXT = Fsm->STATE;
  Fsm->STATE  = State;

  return( State );
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl Add Fsm Bi Vex                      |
|                                                             |
\------------------------------------------------------------*/

rtlbivex_list *addrtlfsmbivex( Figure, Fsm, VexCond, VexData, Type )

  rtlfig_list *Figure;
  rtlfsm_list *Fsm;
  vexexpr     *VexCond;
  vexexpr     *VexData;
  int          Type;
{
  rtlbivex_list *BiVex;

  BiVex = allocrtlbivex();

  BiVex->VEX_COND = VexCond;
  BiVex->VEX_DATA = VexData;
  BiVex->TYPE     = Type;

  *Fsm->LAST_BIVEX = BiVex;
  Fsm->LAST_BIVEX  = &BiVex->NEXT;

  return( BiVex );
}

/*------------------------------------------------------------\
|                                                             |
|                          Rtl Add Fsm                        |
|                                                             |
\------------------------------------------------------------*/

rtlfsm_list *addrtlfsm( Figure, Name )

  rtlfig_list  *Figure;
  char         *Name;
{
  rtlfsm_list *Fsm;

  Name = namealloc( Name );
  Fsm  = allocrtlfsm();

  Fsm->NAME       = Name;
  Fsm->HASH_STATE = createauthtable( 50 );
  Fsm->LAST_BIVEX = &Fsm->BIVEX;

  Fsm->NEXT   = Figure->FSM;
  Figure->FSM = Fsm;

  return( Fsm );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Model                        |
|                                                             |
\------------------------------------------------------------*/

rtlmod_list *addrtlmod( Figure, Name )

  rtlfig_list   *Figure;
  char          *Name;
{
  rtlmod_list *Model;

  Name  = namealloc( Name );
  Model = searchrtlmod( Figure, Name );

  if ( Model != (rtlmod_list *)0 )
  {
    rtlerror( RTL_MOD_EXIST_ERROR, Name, 0 );
  }

  Model = allocrtlmod();

  Model->NAME = Name;

  Model->NEXT = Figure->MODEL;
  Figure->MODEL = Model;

  addauthelem( Figure->HASH_MODEL, Name, (long)Model );

  return( Model );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Model Port                   |
|                                                             |
\------------------------------------------------------------*/

rtlport_list *addrtlmodport( Figure, Model, Atom, Base, Dir )

  rtlfig_list   *Figure;
  rtlmod_list   *Model;
  vexexpr       *Atom;
  unsigned char  Base;
  unsigned char  Dir;
{
  rtlport_list *Port;

  Port = allocrtlport();

  Port->VEX_ATOM = Atom;
  Port->DIR      = Dir;
  Port->BASE     = Base;


  Port->NEXT  = Model->PORT;
  Model->PORT = Port;

  return( Port );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Model Generic                |
|                                                             |
\------------------------------------------------------------*/

rtlgen_list *addrtlmodgen( Figure, Model, Atom, Expr, Base )

  rtlfig_list   *Figure;
  rtlmod_list   *Model;
  vexexpr       *Atom;
  vexexpr       *Expr;
  unsigned char  Base;
{
  rtlgen_list *Generic;

  Generic = allocrtlgen();

  Generic->VEX_ATOM = Atom;
  Generic->VEX_EXPR = Expr;
  Generic->BASE     = Base;

  Generic->NEXT     = Model->GENERIC;
  Model->GENERIC = Generic;

  return( Generic );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Instance                     |
|                                                             |
\------------------------------------------------------------*/

rtlins_list *addrtlins( Figure, Name, Model )

  rtlfig_list   *Figure;
  char          *Name;
  rtlmod_list   *Model;
{
  rtlins_list *Instance;

  Name     = namealloc( Name );
  Instance = searchrtlins( Figure, Name );

  if ( Instance != (rtlins_list *)0 )
  {
    rtlerror( RTL_INS_EXIST_ERROR, Name, 0 );
  }

  Instance = allocrtlins();

  Instance->NAME  = Name;
  Instance->MODEL = Model;

  Instance->NEXT   = Figure->INSTANCE;
  Figure->INSTANCE = Instance;

  addauthelem( Figure->HASH_INSTANCE, Name, (long)Instance );

  return( Instance );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Instance Map                 |
|                                                             |
\------------------------------------------------------------*/

rtlmap_list *addrtlinsmap( Figure, Instance, Formal, Actual )

  rtlfig_list   *Figure;
  rtlins_list   *Instance;
  vexexpr       *Formal;
  vexexpr       *Actual;
{
  rtlmap_list *Map;

  Map = allocrtlmap();

  Map->VEX_FORMAL = Formal;
  Map->VEX_ACTUAL = Actual;

  Map->NEXT     = Instance->MAP;
  Instance->MAP = Map;

  return( Map );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Add Instance Generic             |
|                                                             |
\------------------------------------------------------------*/

rtlgen_list *addrtlinsgen( Figure, Instance, Atom, Expr )

  rtlfig_list   *Figure;
  rtlins_list   *Instance;
  vexexpr       *Atom;
  vexexpr       *Expr;
{
  rtlgen_list *Generic;

  Generic = allocrtlgen();

  Generic->VEX_ATOM = Atom;
  Generic->VEX_EXPR = Expr;

  Generic->NEXT         = Instance->GENERIC;
  Instance->GENERIC = Generic;

  return( Generic );
}
