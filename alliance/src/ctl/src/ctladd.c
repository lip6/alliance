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
| Tool    :                     Ctl                           |
|                                                             |
| File    :                   ctladd.c                        |
|                                                             |
| Date    :                   18.04.02                        |
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
# include "ctl.h"

# include <stdio.h>
# include "ctladd.h"
# include "ctlerror.h"

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

  ctlfig_list *HEAD_CTLFIG = (ctlfig_list *)0;

  char *CTL_DECLAR_TYPE[ CTL_MAX_DECLAR_TYPE ] =
  {
    "ALL",
    "VARIABLE",
    "DEFINE",
    "ASSUME",
    "INITIAL",
    "RESET"
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Ctl Add Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Ctl Add Figure                       |
|                                                             |
\------------------------------------------------------------*/

ctlfig_list *addctlfig( Name )

   char *Name;
{
  ctlfig_list *Figure;
  int          Type;

  Figure = allocctlfig();

  Figure->NAME = namealloc( Name );
  Figure->NEXT = HEAD_CTLFIG;
  HEAD_CTLFIG  = Figure;

  for ( Type = 0; Type < CTL_MAX_DECLAR_TYPE; Type++ )
  {
    Figure->HASH_DECLAR[ Type ] = createauthtable( 50 );
  }

  Figure->HASH_FORM = createauthtable( 50 );
  Figure->HASH_TYPE = createauthtable( 50 );
  Figure->LAST_FORM = &Figure->FORM;

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                       Ctl Add Declaration                   |
|                                                             |
\------------------------------------------------------------*/

static ctldecl_list *loc_addctldecl( Figure, Expr, DeclType )

  ctlfig_list   *Figure;
  vexexpr       *Expr;
  unsigned char  DeclType;
{
  ctldecl_list  *Decl;
  ctldecl_list **PrevDecl;
  vexexpr       *Atom;
  ctlsym        *Sym;
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

  if ( ( DeclType <= CTL_DECLAR_ALL      ) ||
       ( DeclType >= CTL_MAX_DECLAR_TYPE ) )
  {
    ctlerror( CTL_UNKNOWN_DECLAR_TYPE, DeclType, Name );
  }

  Decl = allocctldecl();

  Width = Atom->WIDTH;
  Sym   = allocctlsym( Width );

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

  PrevDecl = &Figure->DECLAR[ DeclType ];

  Decl->VEX_ATOM  = Expr;
  Decl->DECL_SYM  = Sym;
  Decl->DECL_TYPE = DeclType;
  Decl->PREV      = &Figure->DECLAR[ DeclType ];
  Decl->NEXT      = *(Decl->PREV);

  if ( Decl->NEXT != (ctldecl_list *)0 )
  {
    Decl->NEXT->PREV = &Decl->NEXT;
  }

  *(Decl->PREV) = Decl;

  addauthelem( Figure->HASH_DECLAR[ DeclType       ], Name, (long)Decl );
  addauthelem( Figure->HASH_DECLAR[ CTL_DECLAR_ALL ], Name, (long)Decl );

  return( Decl );
}

/*------------------------------------------------------------\
|                                                             |
|                       Ctl Add Declaration                   |
|                                                             |
\------------------------------------------------------------*/

ctldecl_list *addctldecl( Figure, Atom, Type )

  ctlfig_list   *Figure;
  vexexpr       *Atom;
  unsigned char  Type;
{
  ctldecl_list *Decl;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Decl = searchctldeclall( Figure, Name );

  if ( Decl != (ctldecl_list *)0 )
  {
    ctlerror( CTL_DECLAR_EXIST_ERROR, Name, 0 );
  }

  Decl = loc_addctldecl( Figure, Atom, Type );

  return( Decl );
}

/*------------------------------------------------------------\
|                                                             |
|                     Ctl Add Declaration Variable            |
|                                                             |
\------------------------------------------------------------*/

ctldecl_list *addctldeclvar( Figure, Atom )

   ctlfig_list   *Figure;
   vexexpr       *Atom;
{
  ctldecl_list *Variable;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Variable = searchctldeclall( Figure, Name );

  if ( Variable != (ctldecl_list *)0 )
  {
    ctlerror( CTL_DECLAR_EXIST_ERROR, Name, 0 );
  }

  Variable = loc_addctldecl( Figure, Atom, CTL_DECLAR_VARIABLE );

  return( Variable );
}

/*------------------------------------------------------------\
|                                                             |
|                     Ctl Add Declaration Define              |
|                                                             |
\------------------------------------------------------------*/

ctldecl_list *addctldecldef( Figure, Atom )

   ctlfig_list   *Figure;
   vexexpr       *Atom;
{
  ctldecl_list *Define;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Define = searchctldeclall( Figure, Name );

  if ( Define != (ctldecl_list *)0 )
  {
    ctlerror( CTL_DECLAR_EXIST_ERROR, Name, 0 );
  }

  Define = loc_addctldecl( Figure, Atom, CTL_DECLAR_DEFINE );

  return( Define );
}

/*------------------------------------------------------------\
|                                                             |
|                     Ctl Add Declaration Assume              |
|                                                             |
\------------------------------------------------------------*/

ctldecl_list *addctldeclass( Figure, Atom )

   ctlfig_list   *Figure;
   vexexpr       *Atom;
{
  ctldecl_list *Assume;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Assume = searchctldeclall( Figure, Name );

  if ( Assume != (ctldecl_list *)0 )
  {
    ctlerror( CTL_DECLAR_EXIST_ERROR, Name, 0 );
  }

  Assume = loc_addctldecl( Figure, Atom, CTL_DECLAR_ASSUME );

  return( Assume );
}

/*------------------------------------------------------------\
|                                                             |
|                     Ctl Add Declaration Initial             |
|                                                             |
\------------------------------------------------------------*/

ctldecl_list *addctldeclinit( Figure, Atom )

   ctlfig_list   *Figure;
   vexexpr       *Atom;
{
  ctldecl_list *Initial;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Initial = searchctldeclall( Figure, Name );

  if ( Initial != (ctldecl_list *)0 )
  {
    ctlerror( CTL_DECLAR_EXIST_ERROR, Name, 0 );
  }

  Initial = loc_addctldecl( Figure, Atom, CTL_DECLAR_INITIAL );

  return( Initial );
}

/*------------------------------------------------------------\
|                                                             |
|                     Ctl Add Declaration Reset               |
|                                                             |
\------------------------------------------------------------*/

ctldecl_list *addctldeclreset( Figure, Atom )

   ctlfig_list   *Figure;
   vexexpr       *Atom;
{
  ctldecl_list *Initial;
  char         *Name;

  if ( ! IsVexNodeAtom( Atom ) ) Name = getvexarrayname( Atom );
  else                           Name = GetVexAtomValue( Atom );

  Initial = searchctldeclall( Figure, Name );

  if ( Initial != (ctldecl_list *)0 )
  {
    ctlerror( CTL_DECLAR_EXIST_ERROR, Name, 0 );
  }

  Initial = loc_addctldecl( Figure, Atom, CTL_DECLAR_RESET );

  return( Initial );
}

/*------------------------------------------------------------\
|                                                             |
|                         Ctl Add File Line                   |
|                                                             |
\------------------------------------------------------------*/

ctlline_list *addctlfileline( Figure, HeadLine, File, Line )

  ctlfig_list   *Figure;
  ctlline_list **HeadLine;
  char          *File;
  long           Line;
{
  ctlline_list **PrevLine;
  ctlline_list  *CtlLine;

  File = autnamealloc( File );

  PrevLine = HeadLine;

  for ( CtlLine  = *PrevLine;
        CtlLine != (ctlline_list *)0;
        CtlLine  = CtlLine->NEXT )
  {
    if ( CtlLine->NAME == File )
    {
      if ( CtlLine->LINE == Line ) return( CtlLine );
      if ( CtlLine->LINE <  Line ) PrevLine = &CtlLine->NEXT;
    }
  }

  CtlLine = allocctlline();

  CtlLine->LINE = Line;
  CtlLine->NAME = File;
  CtlLine->NEXT = *PrevLine;
  *PrevLine     = CtlLine;

  return( CtlLine );
}

/*------------------------------------------------------------\
|                                                             |
|                         Ctl Add Line                        |
|                                                             |
\------------------------------------------------------------*/

ctlline_list *addctlline( Figure, HeadLine, Line )

  ctlfig_list   *Figure;
  ctlline_list **HeadLine;
  long           Line;
{
  return( addctlfileline( Figure, HeadLine, (char *)0, Line ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Add Form                         |
|                                                             |
\------------------------------------------------------------*/

ctlform_list *addctlform( Figure, Name, Expr )

  ctlfig_list   *Figure;
  char          *Name;
  vexexpr       *Expr;
{
  ctlform_list *Form;

  Form = allocctlform();
  Name = namealloc( Name );

  Form->NAME     = Name;
  Form->VEX_EXPR = Expr;

  *Figure->LAST_FORM = Form;
  Form->PREV         = Figure->LAST_FORM;
  Figure->LAST_FORM  = &Form->NEXT;

  addauthelem( Figure->HASH_FORM, Name, (long)Form );

  return( Form );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Add Type                         |
|                                                             |
\------------------------------------------------------------*/

ctltype_list *addctltype( Figure, Name, Index, 
                          Left, Right, Size, Value, Class, Base )

  ctlfig_list    *Figure;
  char           *Name;
  unsigned long   Index;
  long            Left;
  long            Right;
  unsigned long   Size;
  char          **Value;
  char            Class;
  ctltype_list   *Base;
{
  ctltype_list *Type;

  Type = allocctltype();

  Name         = namealloc( Name );
  Type->NAME   = Name;
  Type->NEXT   = Figure->TYPE;
  Type->LEFT   = Left;
  Type->RIGHT  = Right;
  Type->SIZE   = Size;
  Type->VALUE  = Value;
  Type->CLASS  = Class;
  Type->INDEX  = Index;
  Type->BASE   = Base;
  Figure->TYPE = Type;

  addauthelem( Figure->HASH_TYPE, Name, (long)Type );

  return( Type );
}

/*------------------------------------------------------------\
|                                                             |
|                  Ctl Add Predefined Type                    |
|                                                             |
\------------------------------------------------------------*/

void addctlpredefinedtype( Figure )

  ctlfig_list *Figure;
{
  static char         *bit_vl[2];
  static char         *boolean_vl[2];
  static char         *severity_vl[2];
  static char         *std_ulogic_vl[9];

  ctltype_list        *TypeBit;
  ctltype_list        *TypeReg;
  ctltype_list        *TypeMux;
  ctltype_list        *TypeWor;
  ctltype_list        *TypeInt;
  ctltype_list        *TypeStd_logic;
  ctltype_list        *TypeStd_ulogic;
  ctltype_list        *TypeCharacter;

  boolean_vl[0] = namealloc ("false");
  boolean_vl[1] = namealloc ("true");

  bit_vl[0] = namealloc ("'0'");
  bit_vl[1] = namealloc ("'1'");

  severity_vl[0] = namealloc ("warning");
  severity_vl[1] = namealloc ("error");

  std_ulogic_vl[ 0 ] = namealloc("'u'");
  std_ulogic_vl[ 1 ] = namealloc("'x'");
  std_ulogic_vl[ 2 ] = namealloc("'0'");
  std_ulogic_vl[ 3 ] = namealloc("'1'");
  std_ulogic_vl[ 4 ] = namealloc("'z'");
  std_ulogic_vl[ 5 ] = namealloc("'w'");
  std_ulogic_vl[ 6 ] = namealloc("'l'");
  std_ulogic_vl[ 7 ] = namealloc("'h'");
  std_ulogic_vl[ 8 ] = namealloc("'-'");

  addctltype( Figure, "severity_level",
              VEX_TYPE_SEVERITY, 0, 1, 2, severity_vl, 'E', NULL );

  addctltype( Figure, "boolean",
              VEX_TYPE_BOOLEAN , 0, 1, 2, boolean_vl, 'E', NULL );

  TypeBit = addctltype( Figure, "bit",
              VEX_TYPE_BIT     , 0, 1, 2, bit_vl    , 'E', NULL );


  TypeCharacter = addctltype( Figure, "character",
              VEX_TYPE_CHARACTER, 0, 127,  0, NULL  , 'E', NULL );

  addctltype( Figure, "string",
              VEX_TYPE_STRING, 0, 0x7fffffff, 0, NULL,  'U', TypeCharacter );

  TypeInt = addctltype( Figure, "integer",
              VEX_TYPE_INTEGER, 0x80000000, 0x7fffffff, 0, NULL, 'I', NULL );

  addctltype( Figure, "natural",
              VEX_TYPE_NATURAL, 0 ,0x7fffffff, 0, NULL, 'I', NULL );

  addctltype( Figure, "bit_vector",
              VEX_TYPE_BIT_VECTOR, 0, 0x7fffffff, 0, NULL, 'U', TypeBit );

  TypeStd_ulogic = addctltype( Figure, "std_ulogic",
              VEX_TYPE_STD_ULOGIC, 0, 8, 9, std_ulogic_vl, 'E', NULL );

  TypeStd_logic = addctltype( Figure, "std_logic",
              VEX_TYPE_STD_LOGIC , 0, 8, 9, std_ulogic_vl, 'E', NULL );

  addctltype( Figure, "std_ulogic_vector",
              VEX_TYPE_STD_ULOGIC_VECTOR,
              0,0x7fffffff, 0, NULL, 'U', TypeStd_ulogic );

  addctltype( Figure,"std_logic_vector",
              VEX_TYPE_STD_LOGIC_VECTOR, 0,0x7fffffff, 0, NULL,
              'U',TypeStd_logic );

  addctltype( Figure,"unsigned",
              VEX_TYPE_UNSIGNED, 0,0x7fffffff, 0, NULL, 'U', TypeStd_logic );

  addctltype( Figure,"signed",
              VEX_TYPE_SIGNED, 0,0x7fffffff, 0, NULL, 'U', TypeStd_logic );
}
