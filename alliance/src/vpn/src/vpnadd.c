/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| Tool    :                     Vpn                           |
|                                                             |
| File    :                   vpnadd.c                        |
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
# include "vpn.h"

# include <stdio.h>
# include "vpnadd.h"
# include "vpnerror.h"

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

  vpnfig_list *HEAD_VPNFIG = (vpnfig_list *)0;

  char *VPN_FATHER_TYPE[ VPN_MAX_FATHER_TYPE ] =
  {
    "PROCESS",
    "FUNCTION"
  };

  char *VPN_ACT_TYPE[ VPN_MAX_ACT_TYPE ] =
  {
    "UNKNOWN",
    "ASG_PORT",
    "ASG_SIGNAL",
    "ASG_VARIABLE",
    "ASG_DEFINE",
    "ASG_ARG_SIG",
    "ASG_ARG_VAR",
    "CALL",
    "RETURN"
  };

  char *VPN_TRANS_TYPE[ VPN_MAX_TRANS_TYPE ] =
  {
    "UNKNOWN",
    "ACT_EXEC",
    "INF_WAIT",
    "SUP_WAIT",
    "GUARDED",
    "IMMEDIATE",
    "ACT_GUARDED",
    "ACT_EXEC_CONC",
    "ACT_GUARDED_CONC"
  };

  char *VPN_PLACE_TYPE[ VPN_MAX_PLACE_TYPE ] =
  {
    "UNKNOWN",
    "ASSIGN",
    "IF",
    "END_IF",
    "CASE",
    "END_CASE",
    "WAIT",
    "UNTIL",
    "UNTIL_TEST",
    "END_UNTIL",
    "WHILE",
    "WHILE_TEST",
    "END_WHILE",
    "FOR",
    "FOR_LOOP",
    "FOR_TEST",
    "FOR_INC",
    "END_FOR",
    "NEXT",
    "EXIT",
    "CALL",
    "RETURN",
    "PROCESS",
    "FUNCTION",
    "LOOP",
    "END_LOOP",
    "END_EXIT",
    "END_NEXT",
    "GOTO",
    "LABEL",
    "DO",
    "DO_TEST",
    "DO_WHILE",
    "END_DO",
    "SWITCH",
    "END_SWITCH",
    "C_CASE",
    "BREAK",
    "C_RETURN"
  };

  char *VPN_DECLAR_TYPE[ VPN_MAX_DECLAR_TYPE ] =
  {
    "ALL",
    "PORT",
    "SIGNAL",
    "VARIABLE",
    "DEFINE",
    "ARGUMENT",
    "CONSTANT",
    "GENERIC"
  };

  char *VPN_ARC_TYPE[ VPN_MAX_ARC_TYPE ] =
  {
    "PLACE_TRANS",
    "TRANS_PLACE"
  };

  char *VPN_KIND_TYPE[ VPN_MAX_KIND_TYPE ] =
  {
    "NONE",
    "BUS",
    "REGISTER"
  };

  char *VPN_CLASS_TYPE[ VPN_MAX_CLASS_TYPE ] =
  {
    "NONE",
    "CONSTANT",
    "VARIABLE",
    "SIGNAL"
  };

  char *VPN_DIR_TYPE[ VPN_MAX_DIR_TYPE ] =
  {
    "IN",
    "OUT",
    "INOUT"
  };

  char *VPN_ATTRIBUTE_TYPE[ VPN_MAX_ATTR_TYPE ] =
  {
    "NONE",
    "ENTITY",
    "ARCHITECTURE",
    "PACKAGE",
    "FUNCTION",
    "PROCEDURE",
    "SUBTYPE",
    "CONSTANT",
    "VARIABLE",
    "SIGNAL",
    "LABEL",
    "TYPE",
    "CONFIGURATION",
    "COMPONENT"
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Vpn Add Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Figure                       |
|                                                             |
\------------------------------------------------------------*/

vpnfig_list *addvpnfig( Name )

   char *Name;
{
  vpnfig_list *Figure;
  int          Type;

  Figure = allocvpnfig();

  Figure->NAME = namealloc( Name );
  Figure->NEXT = HEAD_VPNFIG;
  HEAD_VPNFIG  = Figure;

  Figure->HASH_PROCESS  = createauthtable( 50 );
  Figure->HASH_FUNCTION = createauthtable( 50 );
  Figure->HASH_MODEL    = createauthtable( 50 );
  Figure->HASH_INSTANCE = createauthtable( 50 );
  Figure->HASH_TRANS    = createauthtable( 50 );
  Figure->HASH_PLACE    = createauthtable( 50 );
  Figure->HASH_ARC      = createauth2table( 50 );

  for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ )
  {
    Figure->HASH_DECLAR[ Type ] = createauthtable( 50 );
  }

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Attribute                    |
|                                                             |
\------------------------------------------------------------*/

vpnattr_list *addvpnattr( Figure, Name, Entity, Type, Value )

  vpnfig_list   *Figure;
  char          *Name;
  char          *Entity;
  unsigned char  Type;
  char          *Value;
{
  vpnattr_list *Attr;

  Name   = namealloc( Name   );
  Entity = namealloc( Entity );
  Value  = namealloc( Value  );

  Attr = allocvpnattr();

  Attr->NAME   = Name;
  Attr->ENTITY = Entity;
  Attr->TYPE   = Type;
  Attr->VALUE  = Value;

  Attr->NEXT        = Figure->ATTRIBUTE;
  Figure->ATTRIBUTE = Attr;

  return( Attr );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Process Place                |
|                                                             |
\------------------------------------------------------------*/

vpnplace_list *addvpnprocplace( Figure, Process, Name )

   vpnfig_list   *Figure;
   vpnproc_list  *Process;
   char          *Name;
{
  vpnplace_list  *Place;

  Name  = namealloc( Name );
  Place = searchvpnplace( Figure, Name );

  if ( Place != (vpnplace_list *)0 )
  {
    vpnerror( VPN_PLACE_EXIST_ERROR, Name, 0 );
  }

  Place = allocvpnplace();

  Place->NAME   = Name;
  Place->FATHER = VPN_FATHER_PROCESS;
  Place->NEXT   = Process->PLACE;
  Place->PREV   = &Process->PLACE;

  if ( Process->PLACE != (vpnplace_list *)0 )
  {
    Process->PLACE->PREV = &Place->NEXT;
  }

  Process->PLACE = Place;

  addauthelem( Figure->HASH_PLACE, Name, (long)Place );

  return( Place );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Function Place               |
|                                                             |
\------------------------------------------------------------*/

vpnplace_list *addvpnfuncplace( Figure, Function, Name )

   vpnfig_list   *Figure;
   vpnfunc_list  *Function;
   char          *Name;
{
  vpnplace_list  *Place;

  Name  = namealloc( Name );
  Place = searchvpnplace( Figure, Name );

  if ( Place != (vpnplace_list *)0 )
  {
    vpnerror( VPN_PLACE_EXIST_ERROR, Name, 0 );
  }

  Place = allocvpnplace();

  Place->NAME   = Name;
  Place->FATHER = VPN_FATHER_FUNCTION;
  Place->NEXT   = Function->PLACE;
  Place->PREV   = &Function->PLACE;

  if ( Function->PLACE != (vpnplace_list *)0 )
  {
    Function->PLACE->PREV = &Place->NEXT;
  }

  Function->PLACE = Place;

  addauthelem( Figure->HASH_PLACE, Name, (long)Place );

  return( Place );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Add Process Trans                   |
|                                                             |
\------------------------------------------------------------*/

vpntrans_list *addvpnproctrans( Figure, Process, Name )

   vpnfig_list   *Figure;
   vpnproc_list  *Process;
   char          *Name;
{
  vpntrans_list  *Trans;

  Name  = namealloc( Name );
  Trans = searchvpntrans( Figure, Name );

  if ( Trans != (vpntrans_list *)0 )
  {
    vpnerror( VPN_TRANS_EXIST_ERROR, Name, 0 );
  }

  Trans = allocvpntrans();

  Trans->NAME     = Name;
  Trans->FATHER   = VPN_FATHER_PROCESS;
  Trans->LAST_ACT = &Trans->ACT;
  Trans->NEXT     = Process->TRANS;
  Trans->PREV     = &Process->TRANS;

  if ( Process->TRANS != (vpntrans_list *)0 )
  {
    Process->TRANS->PREV = &Trans->NEXT;
  }

  Process->TRANS = Trans;

  addauthelem( Figure->HASH_TRANS, Name, (long)Trans );

  return( Trans );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Add Function Trans                  |
|                                                             |
\------------------------------------------------------------*/

vpntrans_list *addvpnfunctrans( Figure, Function, Name )

   vpnfig_list   *Figure;
   vpnfunc_list  *Function;
   char          *Name;
{
  vpntrans_list  *Trans;

  Name  = namealloc( Name );
  Trans = searchvpntrans( Figure, Name );

  if ( Trans != (vpntrans_list *)0 )
  {
    vpnerror( VPN_TRANS_EXIST_ERROR, Name, 0 );
  }

  Trans = allocvpntrans();

  Trans->NAME     = Name;
  Trans->FATHER   = VPN_FATHER_FUNCTION;
  Trans->LAST_ACT = &Trans->ACT;
  Trans->NEXT     = Function->TRANS;
  Trans->PREV     = &Function->TRANS;

  if ( Function->TRANS != (vpntrans_list *)0 )
  {
    Function->TRANS->PREV = &Trans->NEXT;
  }

  Function->TRANS = Trans;

  addauthelem( Figure->HASH_TRANS, Name, (long)Trans );

  return( Trans );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Arc                          |
|                                                             |
\------------------------------------------------------------*/

vpnarc *addvpnarc( Figure, Source, Target, ArcType )

   vpnfig_list   *Figure;
   void          *Source;
   void          *Target;
   unsigned char  ArcType;
{
  vpnplace_list *Place;
  vpntrans_list *Trans;
  vpnarc        *Arc;

  if ( ArcType == VPN_ARC_PLACE_TRANS )
  {
    Place = (vpnplace_list *)Source;
    Trans = (vpntrans_list *)Target;
  }
  else
  {
    Place = (vpnplace_list *)Target;
    Trans = (vpntrans_list *)Source;
  }

  Arc = searchvpnarc( Figure, Source, Target );

  if ( Arc != (vpnarc *)0 )
  {
    if ( ArcType == VPN_ARC_PLACE_TRANS )
    {
      vpnerror( VPN_ARC_PLACE_EXIST_ERROR, Place->NAME, Trans->NAME );
    }
    else
    {
      vpnerror( VPN_ARC_TRANS_EXIST_ERROR, Trans->NAME, Place->NAME );
    }
  }

  Arc = allocvpnarc();

  if ( ArcType == VPN_ARC_PLACE_TRANS )
  {
    Place->NUMBER_OUT++;
    Place->TRANS_OUT = addchain( Place->TRANS_OUT, (void *)Arc );
    Trans->NUMBER_IN++;
    Trans->PLACE_IN  = addchain( Trans->PLACE_IN,  (void *)Arc );
  }
  else
  {
    Trans->NUMBER_OUT++;
    Trans->PLACE_OUT = addchain( Trans->PLACE_OUT, (void *)Arc );
    Place->NUMBER_IN++;
    Place->TRANS_IN  = addchain( Place->TRANS_IN,  (void *)Arc );
  }

  Arc->SOURCE = Source;
  Arc->TARGET = Target;
  Arc->TYPE   = ArcType;

  addauth2elem( Figure->HASH_ARC, Source, Target, (long)Arc );

  return( Arc );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Add Declaration                   |
|                                                             |
\------------------------------------------------------------*/

static vpndecl_list *loc_addvpndecl( Figure, Expr, Type, Base )

  vpnfig_list   *Figure;
  vexexpr       *Expr;
  unsigned char  Type;
  unsigned char  Base;
{
  vpndecl_list  *Decl;
  vpndecl_list **PrevDecl;
  vexexpr       *Atom;
  vpnsym        *Sym;
  char          *Name;
  int            Width;
  int            Index;
  int            IndexStep;
  int            Scan;

  if ( ! IsVexNodeAtom( Expr ) )
  {
    Atom = getvexarrayatom( Expr );
  }
  else
  {
    Atom = Expr;
  }

  Name = GetVexAtomValue( Atom );

  if ( ( Type <= VPN_DECLAR_ALL      ) ||
       ( Type >= VPN_MAX_DECLAR_TYPE ) )
  {
    vpnerror( VPN_UNKNOWN_DECLAR_TYPE, Type, Name );
  }

  Decl = allocvpndecl();

  Width = Atom->WIDTH;
  Sym   = allocvpnsym( Width );

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

  Decl->VEX_ATOM  = Expr;
  Decl->DECL_SYM  = Sym;
  Decl->TYPE      = Type;
  Decl->BASE      = Base;
  Decl->PREV      = &Figure->DECLAR[ Type ];
  Decl->NEXT      = *(Decl->PREV);

  if ( Decl->NEXT != (vpndecl_list *)0 )
  {
    Decl->NEXT->PREV = &Decl->NEXT;
  }

  *(Decl->PREV) = Decl;

  addauthelem( Figure->HASH_DECLAR[ Type           ], Name, (long)Decl );
  addauthelem( Figure->HASH_DECLAR[ VPN_DECLAR_ALL ], Name, (long)Decl );

  return( Decl );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Add Declaration                   |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndecl( Figure, Atom, Type, Base )

  vpnfig_list   *Figure;
  vexexpr       *Atom;
  unsigned char  Type;
  unsigned char  Base;
{
  vpndecl_list *Decl;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Decl = searchvpndeclall( Figure, Name );

  if ( Decl != (vpndecl_list *)0 )
  {
    vpnerror( VPN_DECLAR_EXIST_ERROR, Name, 0 );
  }

  Decl = loc_addvpndecl( Figure, Atom, Type, Base );

  return( Decl );
}

/*------------------------------------------------------------\
|                                                             |
|                      Vpn Add Declaration Port               |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclport( Figure, Atom, Base, Dir, Kind )

   vpnfig_list   *Figure;
   vexexpr       *Atom;
   unsigned char  Base;
   unsigned char  Dir;
   unsigned char  Kind;
{
  vpndecl_list *Port;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Port = searchvpndeclall( Figure, Name );

  if ( Port != (vpndecl_list *)0 )
  {
    vpnerror( VPN_PORT_EXIST_ERROR, Name, 0 );
  }

  Port = loc_addvpndecl( Figure, Atom, VPN_DECLAR_PORT, Base );
  Port->DIR  = Dir;
  Port->KIND = Kind;

  return( Port );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Add Declaration Signal              |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclsig( Figure, Atom, Base, Kind )

   vpnfig_list   *Figure;
   vexexpr       *Atom;
   unsigned char  Base;
   unsigned char  Kind;
{
  vpndecl_list *Signal;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Signal = searchvpndeclall( Figure, Name );

  if ( Signal != (vpndecl_list *)0 )
  {
    vpnerror( VPN_SIGNAL_EXIST_ERROR, Name, 0 );
  }

  Signal = loc_addvpndecl( Figure, Atom, VPN_DECLAR_SIGNAL, Base );
  Signal->DIR  = VPN_DIR_INOUT;
  Signal->KIND = Kind;

  return( Signal );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Add Declaration Constant            |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclcst( Figure, Atom, Base )

   vpnfig_list   *Figure;
   vexexpr       *Atom;
   unsigned char  Base;
{
  vpndecl_list *Constant;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Constant = searchvpndeclall( Figure, Name );

  if ( Constant != (vpndecl_list *)0 )
  {
    vpnerror( VPN_CONSTANT_EXIST_ERROR, Name, 0 );
  }

  Constant = loc_addvpndecl( Figure, Atom, VPN_DECLAR_CONSTANT, Base );
  Constant->DIR  = VPN_DIR_IN;

  return( Constant );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Add Declaration Generic             |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclgen( Figure, Atom, Base )

   vpnfig_list   *Figure;
   vexexpr       *Atom;
   unsigned char  Base;
{
  vpndecl_list *Generic;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Generic = searchvpndeclall( Figure, Name );

  if ( Generic != (vpndecl_list *)0 )
  {
    vpnerror( VPN_GENERIC_EXIST_ERROR, Name, 0 );
  }

  Generic = loc_addvpndecl( Figure, Atom, VPN_DECLAR_GENERIC, Base );
  Generic->DIR  = VPN_DIR_IN;

  return( Generic );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Add Declar Variable                 |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclvar( Figure, Atom, Base )

   vpnfig_list   *Figure;
   vexexpr       *Atom;
   unsigned char  Base;
{
  vpndecl_list *Variable;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Variable = searchvpndeclall( Figure, Name );

  if ( Variable != (vpndecl_list *)0 )
  {
    vpnerror( VPN_VARIABLE_EXIST_ERROR, Name, 0 );
  }

  Variable = loc_addvpndecl( Figure, Atom, VPN_DECLAR_VARIABLE, Base );
  Variable->DIR = VPN_DIR_INOUT;

  Figure->VAR_DECL = addchain( Figure->VAR_DECL, Variable );

  return( Variable );
}

/*------------------------------------------------------------\
|                                                             |
|                 Vpn Add Declar Process Variable             |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclprocvar( Figure, Process, Atom, Base )

   vpnfig_list   *Figure;
   vpnproc_list  *Process;
   vexexpr       *Atom;
   unsigned char  Base;
{
  vpndecl_list *Variable;
  char        *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Variable = searchvpndeclall( Figure, Name );

  if ( Variable != (vpndecl_list *)0 )
  {
    vpnerror( VPN_VARIABLE_EXIST_ERROR, Name, 0 );
  }

  Variable = loc_addvpndecl( Figure, Atom, VPN_DECLAR_VARIABLE, Base );
  Variable->DIR = VPN_DIR_INOUT;

  Process->VAR_DECL = addchain( Process->VAR_DECL, Variable );

  return( Variable );
}

/*------------------------------------------------------------\
|                                                             |
|              Vpn Add Declar Function Variable               |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclfuncvar( Figure, Function, Atom, Base )

   vpnfig_list   *Figure;
   vpnfunc_list  *Function;
   vexexpr       *Atom;
   unsigned char  Base;
{
  vpndecl_list *Variable;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Variable = searchvpndeclall( Figure, Name );

  if ( Variable != (vpndecl_list *)0 )
  {
    vpnerror( VPN_VARIABLE_EXIST_ERROR, Name, 0 );
  }

  Variable = loc_addvpndecl( Figure, Atom, VPN_DECLAR_VARIABLE, Base );
  Variable->DIR = VPN_DIR_INOUT;

  Function->VAR_DECL = addchain( Function->VAR_DECL, Variable );

  return( Variable );
}

/*------------------------------------------------------------\
|                                                             |
|                  Vpn Add Declaration Define                 |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndecldef( Figure, Atom, Base )

   vpnfig_list   *Figure;
   vexexpr       *Atom;
   unsigned char  Base;
{
  vpndecl_list *Define;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Define = searchvpndeclall( Figure, Name );

  if ( Define != (vpndecl_list *)0 )
  {
    vpnerror( VPN_DEFINE_EXIST_ERROR, Name, 0 );
  }

  Define = loc_addvpndecl( Figure, Atom, VPN_DECLAR_DEFINE, Base );
  Define->DIR = VPN_DIR_INOUT;

  Figure->DEF_DECL = addchain( Figure->DEF_DECL, Define );

  return( Define );
}

/*------------------------------------------------------------\
|                                                             |
|               Vpn Add Declaration Process Define            |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclprocdef( Figure, Process, Atom, Base )

   vpnfig_list   *Figure;
   vpnproc_list  *Process;
   vexexpr       *Atom;
   unsigned char  Base;
{
  vpndecl_list *Define;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Define = searchvpndeclall( Figure, Name );

  if ( Define != (vpndecl_list *)0 )
  {
    vpnerror( VPN_DEFINE_EXIST_ERROR, Name, 0 );
  }

  Define = loc_addvpndecl( Figure, Atom, VPN_DECLAR_DEFINE, Base );
  Define->DIR = VPN_DIR_INOUT;

  Process->DEF_DECL = addchain( Process->DEF_DECL, Define );

  return( Define );
}

/*------------------------------------------------------------\
|                                                             |
|               Vpn Add Declaration Function Define           |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclfuncdef( Figure, Function, Atom, Base )

   vpnfig_list   *Figure;
   vpnfunc_list  *Function;
   vexexpr       *Atom;
   unsigned char  Base;
{
  vpndecl_list *Define;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Define = searchvpndeclall( Figure, Name );

  if ( Define != (vpndecl_list *)0 )
  {
    vpnerror( VPN_DEFINE_EXIST_ERROR, Name, 0 );
  }

  Define = loc_addvpndecl( Figure, Atom, VPN_DECLAR_DEFINE, Base );
  Define->DIR = VPN_DIR_INOUT;

  Function->DEF_DECL = addchain( Function->DEF_DECL, Define );

  return( Define );
}

/*------------------------------------------------------------\
|                                                             |
|                   Vpn Add Declaration Argument              |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *addvpndeclfuncarg( Figure, Function, Atom, Base, Dir, Class )

   vpnfig_list   *Figure;
   vpnfunc_list  *Function;
   vexexpr       *Atom;
   unsigned char  Base;
   unsigned char  Dir;
   unsigned char  Class;
{
  vpndecl_list *Argument;
  chain_list   *NewChain;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Argument = searchvpndeclall( Figure, Name );

  if ( Argument != (vpndecl_list *)0 )
  {
    vpnerror( VPN_ARGUMENT_EXIST_ERROR, Name, 0 );
  }

  Argument = loc_addvpndecl( Figure, Atom, VPN_DECLAR_ARGUMENT, Base );
  Argument->DIR = Dir;
  Argument->CLASS = Class;

  NewChain = addchain( (chain_list *)0, Argument );

  *Function->LAST_ARG = NewChain;
  Function->LAST_ARG  = &NewChain->NEXT;

  return( Argument );
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Add Process                     |
|                                                             |
\------------------------------------------------------------*/

vpnproc_list *addvpnproc( Figure, Name )

   vpnfig_list   *Figure;
   char          *Name;
{
  vpnproc_list *Process;

  Name    = namealloc( Name );
  Process = searchvpnproc( Figure, Name );

  if ( Process != (vpnproc_list *)0 )
  {
    vpnerror( VPN_PROC_EXIST_ERROR, Name, 0 );
  }

  Process = allocvpnproc();

  Process->NAME = Name;
  Process->PREV = &Figure->PROCESS;
  Process->NEXT = Figure->PROCESS;

  if ( Figure->PROCESS != (vpnproc_list *)0 )
  {
    Figure->PROCESS->PREV = &Process->NEXT;
  }

  Figure->PROCESS = Process;

  addauthelem( Figure->HASH_PROCESS, Name, (long)Process );

  return( Process );
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Add Function                    |
|                                                             |
\------------------------------------------------------------*/

vpnfunc_list *addvpnfunc( Figure, Name )

   vpnfig_list   *Figure;
   char          *Name;
{
  vpnfunc_list *Function;

  Name     = namealloc( Name );
  Function = searchvpnfunc( Figure, Name );

  if ( Function != (vpnfunc_list *)0 )
  {
    vpnerror( VPN_FUNC_EXIST_ERROR, Name, 0 );
  }

  Function = allocvpnfunc();

  Function->NAME     = Name;
  Function->LAST_ARG = &Function->ARG_DECL;
  Function->PREV     = &Figure->FUNCTION;
  Function->NEXT     = Figure->FUNCTION;

  if ( Figure->FUNCTION != (vpnfunc_list *)0 )
  {
    Figure->FUNCTION->PREV = &Function->NEXT;
  }

  Figure->FUNCTION = Function;

  addauthelem( Figure->HASH_FUNCTION, Name, (long)Function );

  return( Function );
}

/*------------------------------------------------------------\
|                                                             |
|                    Local Vpn Add Action                     |
|                                                             |
\------------------------------------------------------------*/

static vpnact_list *loc_addvpnact( Atom, Expr, Type )

  vexexpr       *Atom;
  vexexpr       *Expr;
  unsigned char  Type;
{
  vpnact_list *Action;

  Action = allocvpnact();

  Action->VEX_ATOM = Atom;
  Action->VEX_EXPR = Expr;
  Action->TYPE     = Type;

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Add Action                      |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *addvpnact( Figure, Trans, Atom, Expr, Type )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vexexpr       *Atom;
   vexexpr       *Expr;
   unsigned char  Type;
{
  vpnact_list *Action;

  Action = loc_addvpnact( Atom, Expr, Type );

  *Trans->LAST_ACT = Action;
  Action->PREV     = Trans->LAST_ACT;
  Trans->LAST_ACT  = &Action->NEXT;

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Add After Action                |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *addvpnactafter( Figure, Trans, PrevAction, Atom, Expr, Type )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vpnact_list   *PrevAction;
   vexexpr       *Atom;
   vexexpr       *Expr;
   unsigned char  Type;
{
  vpnact_list *Action;
  vpnact_list *NextAction;

  Action = loc_addvpnact( Atom, Expr, Type );

  if ( PrevAction == (vpnact_list *)0 )
  {
    NextAction   = Trans->ACT;
    Trans->ACT   = Action;
    Action->PREV = &Trans->ACT;
  }
  else
  {
    NextAction       = PrevAction->NEXT;
    PrevAction->NEXT = Action;
    Action->PREV     = &PrevAction->NEXT;
  }

  Action->NEXT = NextAction;

  if ( NextAction != (vpnact_list *)0 )
  {
    NextAction->PREV = &Action->NEXT;
  }
  else
  {
    Trans->LAST_ACT = &Action->NEXT;
  }

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Before Action                |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *addvpnactbefore( Figure, Trans, NextAction, Atom, Expr, Type )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vpnact_list   *NextAction;
   vexexpr       *Atom;
   vexexpr       *Expr;
   unsigned char  Type;
{
  vpnact_list *Action;

  Action = loc_addvpnact( Atom, Expr, Type );

  if ( NextAction == (vpnact_list *)0 )
  {
    *Trans->LAST_ACT = Action;
    Action->PREV     = Trans->LAST_ACT;
    Trans->LAST_ACT  = &Action->NEXT;
  }
  else
  {
    *NextAction->PREV = Action;
    Action->PREV      = NextAction->PREV;
    Action->NEXT      = NextAction;
    NextAction->PREV  = &Action->NEXT;
  }

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add First Action                 |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *addvpnactfirst( Figure, Trans, Atom, Expr, Type )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vexexpr       *Atom;
   vexexpr       *Expr;
   unsigned char  Type;
{
  vpnact_list *Action;

  Action = loc_addvpnact( Atom, Expr, Type );

  Action->NEXT = Trans->ACT;
  Action->PREV = &Trans->ACT;
  Trans->ACT   = Action;

  if ( Action->NEXT != (vpnact_list *)0 )
  {
    Action->NEXT->PREV = &Action->NEXT;
  }
  else
  {
    Trans->LAST_ACT = &Action->NEXT;
  }

  return( Action );
}


/*------------------------------------------------------------\
|                                                             |
|                   Local Vpn Add Action Assign               |
|                                                             |
\------------------------------------------------------------*/

static vpnact_list *loc_addvpnactasg( Figure, Atom, Expr )

  vpnfig_list *Figure;
  vexexpr     *Atom;
  vexexpr     *Expr;
{
  vpnact_list   *Action;
  vpndecl_list  *Declar;
  char          *Name;
  unsigned char  Type;

  Name   = getvexatomname( Atom );
  Declar = searchvpndeclall( Figure, Name );

  if ( Declar == (vpndecl_list *)0 )
  {
    vpnerror( VPN_UNDECLARED_SYMBOL, Name, 0 );
  }

  Type = -1;

  switch ( Declar->TYPE )
  {
    case VPN_DECLAR_PORT     :
      Type = VPN_ACT_ASG_PORT;
    break;
    case VPN_DECLAR_SIGNAL   :
      Type = VPN_ACT_ASG_SIGNAL;
    break;
    case VPN_DECLAR_VARIABLE :
      Type = VPN_ACT_ASG_VARIABLE;
    break;
    case VPN_DECLAR_DEFINE   :
      Type = VPN_ACT_ASG_DEFINE;
    break;
    case VPN_DECLAR_ARGUMENT : 
      if ( Declar->CLASS == VPN_CLASS_SIGNAL ) Type = VPN_ACT_ASG_ARG_SIG;
      else                                     Type = VPN_ACT_ASG_ARG_VAR;
    break;
    default : 
      vpnerror( VPN_BAD_ASSIGN_TYPE, Name, Type );
  }

  Action = loc_addvpnact( Atom, Expr, Type );

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                    Vpn add after Action Assign              |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *addvpnactasgafter( Figure, Trans, PrevAction, Atom, Expr )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vpnact_list   *PrevAction;
   vexexpr       *Atom;
   vexexpr       *Expr;
{
  vpnact_list *Action;
  vpnact_list *NextAction;

  Action = loc_addvpnactasg( Figure, Atom, Expr );

  if ( PrevAction == (vpnact_list *)0 )
  {
    NextAction   = Trans->ACT;
    Trans->ACT   = Action;
    Action->PREV = &Trans->ACT;
  }
  else
  {
    NextAction       = PrevAction->NEXT;
    PrevAction->NEXT = Action;
    Action->PREV     = &PrevAction->NEXT;
  }

  Action->NEXT = NextAction;

  if ( NextAction != (vpnact_list *)0 )
  {
    NextAction->PREV = &Action->NEXT;
  }
  else
  {
    Trans->LAST_ACT = &Action->NEXT;
  }

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                    Vpn add before Action Assign             |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *addvpnactasgbefore( Figure, Trans, NextAction, Atom, Expr )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vpnact_list   *NextAction;
   vexexpr       *Atom;
   vexexpr       *Expr;
{
  vpnact_list *Action;

  Action = loc_addvpnactasg( Figure, Atom, Expr );

  if ( NextAction == (vpnact_list *)0 )
  {
    *Trans->LAST_ACT = Action;
    Action->PREV     = Trans->LAST_ACT;
    Trans->LAST_ACT  = &Action->NEXT;
  }
  else
  {
    *NextAction->PREV = Action;
    Action->PREV      = NextAction->PREV;
    Action->NEXT      = NextAction;
    NextAction->PREV  = &Action->NEXT;
  }

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                    Vpn Add First Action Assign              |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *addvpnactasgfirst( Figure, Trans, Atom, Expr )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vexexpr       *Atom;
   vexexpr       *Expr;
{
  vpnact_list *Action;

  Action = loc_addvpnactasg( Figure, Atom, Expr );

  Action->NEXT = Trans->ACT;
  Action->PREV = &Trans->ACT;
  Trans->ACT   = Action;

  if ( Action->NEXT != (vpnact_list *)0 )
  {
    Action->NEXT->PREV = &Action->NEXT;
  }
  else
  {
    Trans->LAST_ACT = &Action->NEXT;
  }

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Add Action Assign               |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *addvpnactasg( Figure, Trans, Atom, Expr )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vexexpr       *Atom;
   vexexpr       *Expr;
{
  vpnact_list   *Action;

  Action = loc_addvpnactasg( Figure, Atom, Expr );

  *Trans->LAST_ACT = Action;
  Action->PREV     = Trans->LAST_ACT;
  Trans->LAST_ACT  = &Action->NEXT;

  return( Action );
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Add File Line                   |
|                                                             |
\------------------------------------------------------------*/

vpnline_list *addvpnfileline( Figure, HeadLine, File, Line )

  vpnfig_list   *Figure;
  vpnline_list **HeadLine;
  char          *File;
  long           Line;
{
  vpnline_list **PrevLine;
  vpnline_list  *VpnLine;

  File = autnamealloc( File );

  PrevLine = HeadLine;

  for ( VpnLine  = *PrevLine;
        VpnLine != (vpnline_list *)0;
        VpnLine  = VpnLine->NEXT )
  {
    if ( VpnLine->NAME == File )
    {
      if ( VpnLine->LINE == Line ) return( VpnLine );
      if ( VpnLine->LINE <  Line ) PrevLine = &VpnLine->NEXT;
    }
  }

  VpnLine = allocvpnline();

  VpnLine->LINE = Line;
  VpnLine->NAME = File;
  VpnLine->NEXT = *PrevLine;
  *PrevLine     = VpnLine;

  return( VpnLine );
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Add Line                        |
|                                                             |
\------------------------------------------------------------*/

vpnline_list *addvpnline( Figure, HeadLine, Line )

  vpnfig_list   *Figure;
  vpnline_list **HeadLine;
  long           Line;
{
  return( addvpnfileline( Figure, HeadLine, (char *)0, Line ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Model                        |
|                                                             |
\------------------------------------------------------------*/

vpnmod_list *addvpnmod( Figure, Name )

  vpnfig_list   *Figure;
  char          *Name;
{
  vpnmod_list *Model;

  Name  = namealloc( Name );
  Model = searchvpnmod( Figure, Name );

  if ( Model != (vpnmod_list *)0 )
  {
    vpnerror( VPN_MOD_EXIST_ERROR, Name, 0 );
  }

  Model = allocvpnmod();

  Model->NAME = Name;

  Model->NEXT = Figure->MODEL;
  Figure->MODEL = Model;

  addauthelem( Figure->HASH_MODEL, Name, (long)Model );

  return( Model );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Model Port                   |
|                                                             |
\------------------------------------------------------------*/

vpnport_list *addvpnmodport( Figure, Model, Atom, Base, Dir )

  vpnfig_list   *Figure;
  vpnmod_list   *Model;
  vexexpr       *Atom;
  unsigned char  Base;
  unsigned char  Dir;
{
  vpnport_list *Port;

  Port = allocvpnport();

  Port->VEX_ATOM = Atom;
  Port->DIR      = Dir;
  Port->BASE     = Base;


  Port->NEXT  = Model->PORT;
  Model->PORT = Port;

  return( Port );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Model Generic                |
|                                                             |
\------------------------------------------------------------*/

vpngen_list *addvpnmodgen( Figure, Model, Atom, Expr, Base )

  vpnfig_list   *Figure;
  vpnmod_list   *Model;
  vexexpr       *Atom;
  vexexpr       *Expr;
  unsigned char  Base;
{
  vpngen_list *Generic;

  Generic = allocvpngen();

  Generic->VEX_ATOM = Atom;
  Generic->VEX_EXPR = Expr;
  Generic->BASE     = Base;

  Generic->NEXT  = Model->GENERIC;
  Model->GENERIC = Generic;

  return( Generic );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Instance                     |
|                                                             |
\------------------------------------------------------------*/

vpnins_list *addvpnins( Figure, Name, Model )

  vpnfig_list   *Figure;
  char          *Name;
  vpnmod_list   *Model;
{
  vpnins_list *Instance;

  Name     = namealloc( Name );
  Instance = searchvpnins( Figure, Name );

  if ( Instance != (vpnins_list *)0 )
  {
    vpnerror( VPN_INS_EXIST_ERROR, Name, 0 );
  }

  Instance = allocvpnins();

  Instance->NAME  = Name;
  Instance->MODEL = Model;

  Instance->NEXT   = Figure->INSTANCE;
  Figure->INSTANCE = Instance;

  addauthelem( Figure->HASH_INSTANCE, Name, (long)Instance );

  return( Instance );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Instance Map                 |
|                                                             |
\------------------------------------------------------------*/

vpnmap_list *addvpninsmap( Figure, Instance, Formal, Actual )

  vpnfig_list   *Figure;
  vpnins_list   *Instance;
  vexexpr       *Formal;
  vexexpr       *Actual;
{
  vpnmap_list *Map;

  Map = allocvpnmap();

  Map->VEX_FORMAL = Formal;
  Map->VEX_ACTUAL = Actual;

  Map->NEXT     = Instance->MAP;
  Instance->MAP = Map;

  return( Map );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Add Instance Generic             |
|                                                             |
\------------------------------------------------------------*/

vpngen_list *addvpninsgen( Figure, Instance, Atom, Expr )

  vpnfig_list   *Figure;
  vpnins_list   *Instance;
  vexexpr       *Atom;
  vexexpr       *Expr;
{
  vpngen_list *Generic;

  Generic = allocvpngen();

  Generic->VEX_ATOM = Atom;
  Generic->VEX_EXPR = Expr;

  Generic->NEXT     = Instance->GENERIC;
  Instance->GENERIC = Generic;

  return( Generic );
}
