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
/* ###--------------------------------------------------------------### */
/* file         : ctp_bcomp.yac                                         */
/* date         : April 29 1993                                         */
/* version      : v1.0                                                  */
/* content      : yacc rules for behavioural VHDL for Petri Net         */
/* ###--------------------------------------------------------------### */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "aut.h"
#include "vex.h"
#include "ctl.h"

#include "ctp_utype.h"
#include "ctp_bedef.h"
#include "ctp_bspec.h"
#include "ctp_byacc.h"

#if 1
# define my_fprintf fprintf( stdout, "%s.%d:", autbasename(__FILE__,NULL), __LINE__ ); fprintf
# define my_ctp_error(N,V) \
   do { fprintf( stderr, "%s.%d:", autbasename(__FILE__,NULL), __LINE__); ctp_error(N,V); } while(0)
#else
# define my_fprintf   fprintf
# define my_ctp_error ctp_error
#endif

#ifdef CTP_DEBUG
#define YYDEBUG 1
#endif

%}

%union  {
          int           valu;
          char         *text;
          chain_list   *pcha;
          ctp_name      name;
          ctp_vexstr    list;
          ctltype_list  type;
       };

%token <valu> _AF
%token <valu> _AG
%token <valu> _AX
%token <valu> _AU
%token <valu> _EF
%token <valu> _EG
%token <valu> _EX
%token <valu> _EU
%token <valu> Imply
%token <valu> Equiv
%token <valu> _AND
%token <valu> _BEGIN
%token <valu> _END
%token <valu> _EQSym
%token <valu> _EXIT
%token <valu> _FILE
%token <valu> _GESym
%token <valu> _GTSym
%token <valu> _IN
%token <valu> _INOUT
%token <valu> _LABEL
%token <valu> _LESym
%token <valu> _LINKAGE
%token <valu> _LTSym
%token <valu> _NAND
%token <valu> _IFT
%token <valu> _NXOR
%token <valu> _NESym
%token <valu> _NEXT
%token <valu> _NOR
%token <valu> _NOT
%token <valu> _NULL
%token <valu> _OR
%token <valu> _OUT
%token <valu> _XOR
%token <valu> ABS
%token <valu> ACCESS
%token <valu> AFTER
%token <valu> ALIAS
%token <valu> ALL
%token <valu> ARCHITECTURE
%token <valu> ARRAY
%token <valu> ASSERT
%token <valu> ATTRIBUTE
%token <text> AbstractLit
%token <valu> Ampersand
%token <valu> Apostrophe
%token <valu> Arrow
%token <valu> BLOCK
%token <valu> BODY
%token <valu> BUS
%token <valu> _BUFFER
%token <valu> Bar
%token <valu> BasedInt
%token <valu> BasedReal
%token <text> BitStringLit
%token <text> _TRUE
%token <text> _FALSE
%token <valu> Box
%token <valu> CASE
%token <valu> COMPONENT
%token <valu> CONFIGURATION
%token <valu> _DEFINE
%token <valu> _ASSUME
%token <valu> _INITIAL
%token <valu> _RESET_COND
%token <text> CharacterLit
%token <valu> Colon
%token <valu> Comma
%token <valu> DISCONNECT
%token <valu> DOWNTO
%token <valu> DecimalInt
%token <valu> DecimalReal
%token <valu> Dot
%token <valu> DoubleStar
%token <valu> ELSE
%token <valu> ELSIF
%token <valu> _ENTITY
%token <valu> ERROR
%token <valu> _EVENT
%token <valu> _LEFT
%token <valu> _LENGTH
%token <valu> _RIGHT
%token <valu> _HIGH
%token <valu> _LOW
%token <valu> FOR
%token <valu> FUNCTION
%token <valu> GENERATE
%token <valu> GENERIC
%token <valu> GUARDED
%token <valu> IF
%token <valu> IS
%token <valu> _NS
%token <valu> _PS
%token <valu> _FS
%token <valu> _MS
%token <text> Identifier
%token <valu> _LIBRARY
%token <valu> LOOP
%token <valu> LeftParen
%token <valu> MAP
%token <valu> MOD
%token <valu> Minus
%token <valu> NEW
%token <valu> OF
%token <valu> ON
%token <valu> OPEN
%token <valu> OTHERS
%token <valu> _PACKAGE
%token <valu> PORT
%token <valu> PROCEDURE
%token <valu> PROCESS
%token <valu> Plus
%token <valu> _RANGE
%token <valu> _REV_RANGE
%token <valu> RECORD
%token <valu> REGISTER
%token <valu> REM
%token <valu> REPORT
%token <valu> _RETURN
%token <valu> RightParen
%token <valu> SELECT
%token <valu> SEVERITY
%token <valu> SIGNAL
%token <valu> _STABLE
%token <valu> SUBTYPE
%token <valu> Semicolon
%token <valu> Slash
%token <valu> Star
%token <text> StringLit
%token <valu> THEN
%token <valu> TO
%token <valu> TRANSPORT
%token <valu> _TYPE
%token <valu> UNITS
%token <valu> UNTIL
%token <valu> USE
%token <valu> _VARIABLE
%token <valu> VarAsgn
%token <valu> WAIT
%token <valu> WARNING
%token <valu> WHEN
%token <valu> WHILE
%token <valu> WITH

%left                _AND _OR _NAND _NOR _XOR Imply Equiv
%left                _EQSym _NESym _LTSym _LESym _GTSym _GESym
%left                Plus Minus Ampersand
%left                Star Slash MOD REM
%left                DoubleStar
%right               ABS _NOT _AF _AG _AX _EF _EG _EX

%type <list> expression
%type <list> relation..AND__relation..
%type <list> relation..OR__relation..
%type <list> relation.NAND_NOR_NXOR_relation.
%type <list> relation..XOR__relation..
%type <list> relation
%type <list> simple_expression
%type <list> .sign.term..add_op__term..
%type <list> term
%type <list> factor
%type <list> primary
%type <valu> relational_operator
%type <valu> sign
%type <valu> adding_operator
%type <valu> multiplying_operator
%type <text> literal
%type <pcha> aggregate
%type <pcha> element_association
%type <pcha> ...element_association..

%type <type> type_definition
%type <type> scalar_type_definition
%type <type> composite_type_definition
%type <type> enumeration_type_definition
%type <type> subtype_indication
%type <type> constrained_array_definition
%type <type> array_type_definition

%type <name> index_constraint
%type <name> discrete_range
%type <name> qualified_expression

%type <text> simple_name
%type <text> type_or_function_name
%type <name> name
%type <name> attribute_name
%type <name> indexed_name
%type <name> slice_name
%type <name> .constraint.
%type <name> constraint
%type <name> range
%type <name> range_constraint
%type <valu> direction
%type <name> type_mark
%type <valu> attribute_designator

%type <list> constant_VarAsgn__expression

%start ctl_file

%%

ctl_file
        : {
            ctltype_list *CtlType;

            CTP_HEADFIG = addctlfig( CTP_FILENAME );;
            CTP_MODNAM  = CTP_HEADFIG->NAME;
            addctlpredefinedtype( CTP_HEADFIG );

            hshtab = initab ();

            CTP_NM1LST = NULL;
            CTP_INSLST = NULL;
            CTP_LINLST = NULL;
            CTP_VALLST = NULL;

            CTP_ERRFLG    = 0;
            CTP_LINNUM    = 1;
            CTP_NUMTYP    = 0;

            CtlType = CTP_HEADFIG->TYPE;

            while( CtlType )
            {
              addtab(hshtab,CtlType->NAME,CTP_MODNAM,CTP_SYMDFN,CTP_TPEDFN);
              addtab(hshtab,CtlType->NAME,CTP_MODNAM,CTP_LBLDFN,CtlType->CLASS);
              addtab(hshtab,CtlType->NAME,CTP_MODNAM,CTP_TYPDFN,CtlType->INDEX);
              addtab(hshtab,CtlType->NAME,CTP_MODNAM,CTP_WMNDFN,CtlType->LEFT);
              addtab(hshtab,CtlType->NAME,CTP_MODNAM,CTP_WMXDFN,CtlType->RIGHT);
              addtab(hshtab,CtlType->NAME,CTP_MODNAM,CTP_PNTDFN,(long)CtlType);

              CtlType = CtlType->NEXT;
            }

            CTP_NUMTYP = VEX_MAX_TYPE;
          }
          ctl_body
          {
            fretab( hshtab );
          }
        ;

ctl_body
       : ctl_declarative_part
         _BEGIN
         ctl_statement_part
         END_ERR
         Semicolon_ERR
         {
         }
       ;

ctl_declarative_part
       : ..block_declarative_item..
       ;

..block_declarative_item..
       : /*empty*/
       | ..block_declarative_item..
         block_declarative_item
       ;

block_declarative_item
       : constant_declaration
       | variable_declaration
       | assumption_declaration
       | initial_declaration
       | reset_cond_declaration
       | type_declaration
       | subtype_declaration
       | error
         Semicolon_ERR
         {
           my_ctp_error(9,NULL);
         }
       ;

constant_declaration
       : _DEFINE
         Identifier
         Colon
         type_mark
         .constraint.
         constant_VarAsgn__expression
         Semicolon_ERR
        {
          ctltype_list  *Type;
          ctldecl_list  *CtlCst;
          vexexpr       *VexValue;
          chain_list    *HeadChain;
          chain_list    *ScanChain;
          ctp_vexstr    *VexStr;
          short          Signed;
          long           Left;
          long           Right;
          short          Width;
          long           AttrLeft;
          long           AttrRight;

          VexValue = simpvexexpr( $6.VEX );

          Type = val_type( $4.NAME );

          if ( ( $5.LEFT  == -1 ) &&
               ( $5.RIGHT == -1 ) )
          {
            AttrLeft  = Type->LEFT;
            AttrRight = Type->RIGHT;
          }
          else
          {
            AttrLeft  = $5.LEFT;
            AttrRight = $5.RIGHT;
          }

          CtlCst  = (ctldecl_list *)ctp_addstr( 'C',0,val_type($4.NAME),
                 $4.TYPE,$4.FLAG,$2,AttrLeft,AttrRight,VexValue);

          Signed = ( IsVexNodeSigned( CtlCst->VEX_ATOM ) != 0 );

          {
            if ( IsVexAtomBit( CtlCst->VEX_ATOM ) )
            {
              Left  = -1;
              Right = -1;
            }
            else
            {
              Left  = CtlCst->VEX_ATOM->LEFT;
              Right = CtlCst->VEX_ATOM->RIGHT;
            }
          }

          addtab(hshtab,$2,CTP_MODNAM,CTP_SYMDFN,CTP_CSTDFN);
          addtab(hshtab,$2,CTP_MODNAM,CTP_TYPDFN,$4.TYPE);
          addtab(hshtab,$2,CTP_MODNAM,CTP_WMNDFN,Left);
          addtab(hshtab,$2,CTP_MODNAM,CTP_WMXDFN,Right);
          addtab(hshtab,$2,CTP_MODNAM,CTP_ATLDFN,AttrLeft);
          addtab(hshtab,$2,CTP_MODNAM,CTP_ATRDFN,AttrRight);
          addtab(hshtab,$2,CTP_MODNAM,CTP_LBLDFN,0);
          addtab(hshtab,$2,CTP_MODNAM,CTP_PNTDFN,(long)CtlCst->VEX_ATOM);
          addtab(hshtab,$2,CTP_MODNAM,CTP_SUNDFN,Signed );
        }
       ;

assumption_declaration
       : _ASSUME
         Identifier
         constant_VarAsgn__expression
         Semicolon_ERR
        {
          ctltype_list  *Type;
          ctldecl_list  *CtlAss;
          vexexpr       *VexValue;
          chain_list    *HeadChain;
          chain_list    *ScanChain;
          ctp_vexstr    *VexStr;
          short          Signed;
          long           Left;
          long           Right;
          short          Width;
          long           AttrLeft;
          long           AttrRight;

          VexValue = simpvexexpr( $3.VEX );

          Type = val_type( "boolean" );

          AttrLeft  = -1;
          AttrRight = -1;

          CtlAss  = (ctldecl_list *)ctp_addstr( 'A',0, Type,
                 VEX_TYPE_BOOLEAN, 0, $2, AttrLeft,AttrRight,VexValue);

          Signed = 0;
          Left   = -1;
          Right  = -1;

          addtab(hshtab,$2,CTP_MODNAM,CTP_SYMDFN,CTP_CSTDFN);
          addtab(hshtab,$2,CTP_MODNAM,CTP_TYPDFN,VEX_TYPE_BOOLEAN);
          addtab(hshtab,$2,CTP_MODNAM,CTP_WMNDFN,Left);
          addtab(hshtab,$2,CTP_MODNAM,CTP_WMXDFN,Right);
          addtab(hshtab,$2,CTP_MODNAM,CTP_ATLDFN,AttrLeft);
          addtab(hshtab,$2,CTP_MODNAM,CTP_ATRDFN,AttrRight);
          addtab(hshtab,$2,CTP_MODNAM,CTP_LBLDFN,0);
          addtab(hshtab,$2,CTP_MODNAM,CTP_PNTDFN,(long)CtlAss->VEX_ATOM);
          addtab(hshtab,$2,CTP_MODNAM,CTP_SUNDFN,Signed );
        }
       ;

initial_declaration
       : _INITIAL
         Identifier
         constant_VarAsgn__expression
         Semicolon_ERR
        {
          ctltype_list  *Type;
          ctldecl_list  *CtlAss;
          vexexpr       *VexValue;
          chain_list    *HeadChain;
          chain_list    *ScanChain;
          ctp_vexstr    *VexStr;
          short          Signed;
          long           Left;
          long           Right;
          short          Width;
          long           AttrLeft;
          long           AttrRight;

          VexValue = simpvexexpr( $3.VEX );

          Type = val_type( "boolean" );

          AttrLeft  = -1;
          AttrRight = -1;

          CtlAss  = (ctldecl_list *)ctp_addstr( 'I',0, Type,
                 VEX_TYPE_BOOLEAN, 0, $2, AttrLeft,AttrRight,VexValue);

          Signed = 0;
          Left   = -1;
          Right  = -1;

          addtab(hshtab,$2,CTP_MODNAM,CTP_SYMDFN,CTP_CSTDFN);
          addtab(hshtab,$2,CTP_MODNAM,CTP_TYPDFN,VEX_TYPE_BOOLEAN);
          addtab(hshtab,$2,CTP_MODNAM,CTP_WMNDFN,Left);
          addtab(hshtab,$2,CTP_MODNAM,CTP_WMXDFN,Right);
          addtab(hshtab,$2,CTP_MODNAM,CTP_ATLDFN,AttrLeft);
          addtab(hshtab,$2,CTP_MODNAM,CTP_ATRDFN,AttrRight);
          addtab(hshtab,$2,CTP_MODNAM,CTP_LBLDFN,0);
          addtab(hshtab,$2,CTP_MODNAM,CTP_PNTDFN,(long)CtlAss->VEX_ATOM);
          addtab(hshtab,$2,CTP_MODNAM,CTP_SUNDFN,Signed );
        }
       ;

reset_cond_declaration
       : _RESET_COND
         Identifier
         constant_VarAsgn__expression
         Semicolon_ERR
        {
          ctltype_list  *Type;
          ctldecl_list  *CtlAss;
          vexexpr       *VexValue;
          chain_list    *HeadChain;
          chain_list    *ScanChain;
          ctp_vexstr    *VexStr;
          short          Signed;
          long           Left;
          long           Right;
          short          Width;
          long           AttrLeft;
          long           AttrRight;

          VexValue = simpvexexpr( $3.VEX );

          Type = val_type( "boolean" );

          AttrLeft  = -1;
          AttrRight = -1;

          CtlAss  = (ctldecl_list *)ctp_addstr( 'R',0, Type,
                 VEX_TYPE_BOOLEAN, 0, $2, AttrLeft,AttrRight,VexValue);

          Signed = 0;
          Left   = -1;
          Right  = -1;

          addtab(hshtab,$2,CTP_MODNAM,CTP_SYMDFN,CTP_CSTDFN);
          addtab(hshtab,$2,CTP_MODNAM,CTP_TYPDFN,VEX_TYPE_BOOLEAN);
          addtab(hshtab,$2,CTP_MODNAM,CTP_WMNDFN,Left);
          addtab(hshtab,$2,CTP_MODNAM,CTP_WMXDFN,Right);
          addtab(hshtab,$2,CTP_MODNAM,CTP_ATLDFN,AttrLeft);
          addtab(hshtab,$2,CTP_MODNAM,CTP_ATRDFN,AttrRight);
          addtab(hshtab,$2,CTP_MODNAM,CTP_LBLDFN,0);
          addtab(hshtab,$2,CTP_MODNAM,CTP_PNTDFN,(long)CtlAss->VEX_ATOM);
          addtab(hshtab,$2,CTP_MODNAM,CTP_SUNDFN,Signed );
        }
       ;


constant_VarAsgn__expression
       : VarAsgn
         expression
                { $$ = $2; }
       ;

variable_declaration
        : _VARIABLE
          identifier_list
          Colon
          type_mark
          .constraint.
          Semicolon_ERR
          {
            char         *LocalName;
            char         *signame;
            char         *codedsigname;
            char          buffer[ 128 ];
            ctltype_list *Type;
            ctldecl_list *CtlVar;
            short         Signed;
            long          Left;
            long          Right;
            long          AttrLeft;
            long          AttrRight;
            char          StrFlag;
            vexexpr      *VexInit;
            struct ctp_expr expr1;

            LocalName = CTP_MODNAM;
            Type  = val_type( $4.NAME );

            if ( ( $5.LEFT  == -1 ) &&
                 ( $5.RIGHT == -1 ) )
            {
              AttrLeft  = Type->LEFT;
              AttrRight = Type->RIGHT;
            }
            else
            {
              AttrLeft  = $5.LEFT;
              AttrRight = $5.RIGHT;
            }

            CTP_NM1LST = reverse (CTP_NM1LST);

            while (CTP_NM1LST != NULL)
            {
              signame = (char *)CTP_NM1LST->DATA;
              if (chktab (hshtab,signame,LocalName,CTP_SYMDFN) != 0)
                  my_ctp_error (80,signame);

              CtlVar = (ctldecl_list *)ctp_addstr( 'V', 0,
                      val_type($4.NAME),$4.TYPE,
                      $4.FLAG,signame, AttrLeft, AttrRight, NULL );

              Signed = ( IsVexNodeSigned( CtlVar->VEX_ATOM ) != 0 );

              if ( IsVexAtomBit( CtlVar->VEX_ATOM ) )
              {
                Left  = -1;
                Right = -1;
              }
              else
              {
                Left  = CtlVar->VEX_ATOM->LEFT;
                Right = CtlVar->VEX_ATOM->RIGHT;
              }

              addtab (hshtab,signame,LocalName,CTP_SYMDFN,CTP_VARDFN);
              addtab (hshtab,signame,LocalName,CTP_TYPDFN,$4.TYPE);
              addtab (hshtab,signame,LocalName,CTP_WMNDFN,Left);
              addtab (hshtab,signame,LocalName,CTP_WMXDFN,Right);
              addtab (hshtab,signame,LocalName,CTP_ATLDFN,AttrLeft);
              addtab (hshtab,signame,LocalName,CTP_ATRDFN,AttrRight);
              addtab (hshtab,signame,LocalName,CTP_LBLDFN,0);
              addtab (hshtab,signame,LocalName,CTP_PNTDFN,(long)CtlVar->VEX_ATOM);
              addtab (hshtab,signame,LocalName,CTP_SUNDFN,Signed);

              CTP_NM1LST = delchain (CTP_NM1LST,CTP_NM1LST);
            }
          }
        ;


type_declaration
        : full_type_declaration
        ;

full_type_declaration
        : _TYPE
          Identifier
          IS
          type_definition
          Semicolon_ERR
          {
           ctltype_list *CtlType;

           CtlType = addctltype( CTP_HEADFIG,$2, CTP_NUMTYP, $4.LEFT,
               $4.RIGHT,$4.SIZE,$4.VALUE, $4.CLASS, $4.BASE );
           addctlline( CTP_HEADFIG, &CtlType->LINE, CTP_LINNUM);

           addtab(hshtab,$2,CTP_MODNAM,CTP_SYMDFN,CTP_TPEDFN);
           addtab(hshtab,$2,CTP_MODNAM,CTP_LBLDFN,$4.CLASS);
           addtab(hshtab,$2,CTP_MODNAM,CTP_TYPDFN,CTP_NUMTYP);
           addtab(hshtab,$2,CTP_MODNAM,CTP_WMNDFN,$4.LEFT);
           addtab(hshtab,$2,CTP_MODNAM,CTP_WMXDFN,$4.RIGHT);
           addtab(hshtab,$2,CTP_MODNAM,CTP_PNTDFN,(long)CtlType );
           CTP_NUMTYP++;
         }
        ;

type_definition
        : scalar_type_definition
              { $$ = $1; }
        | composite_type_definition
              { $$ = $1; }
        | error Semicolon_ERR
              { yyerrok; }
        ;

subtype_declaration
        : SUBTYPE
          Identifier
          IS
          subtype_indication
          Semicolon_ERR
          {
            ctltype_list *CtlType;

            CtlType = addctltype(CTP_HEADFIG,$2,$4.INDEX,$4.LEFT,
                   $4.RIGHT, $4.SIZE, $4.VALUE, $4.CLASS,$4.BASE );
            addctlline( CTP_HEADFIG, &CtlType->LINE, CTP_LINNUM);
            addtab(hshtab,$2,CTP_MODNAM,CTP_SYMDFN,CTP_TPEDFN);
            addtab(hshtab,$2,CTP_MODNAM,CTP_LBLDFN,$4.CLASS);
            addtab(hshtab,$2,CTP_MODNAM,CTP_TYPDFN,$4.INDEX);
            addtab(hshtab,$2,CTP_MODNAM,CTP_WMNDFN,$4.LEFT);
            addtab(hshtab,$2,CTP_MODNAM,CTP_WMXDFN,$4.RIGHT);
            addtab(hshtab,$2,CTP_MODNAM,CTP_PNTDFN,(long)CTP_HEADFIG->TYPE);
            CTP_NUMTYP++;
          }
          ;

subtype_indication
       : type_or_function_name
         type_mark
         .constraint.
         {
           ctltype_list *ctltype_pnt;
           int           mode;
  
           mode = chktab(hshtab,$1,CTP_MODNAM,CTP_SYMDFN);

           ctltype_pnt = (ctltype_list *)chktab(hshtab,$2.NAME,CTP_MODNAM,CTP_PNTDFN);
  
           $$.NAME = NULL;
           $$.BASE = ctltype_pnt;
           $$.VALUE = NULL ;
           $$.SIZE = 0 ;
           $$.INDEX = CTP_NUMTYP;
           $$.LEFT  = $3.LEFT ;
           $$.RIGHT = $3.RIGHT ;
           $$.CLASS = $3.FLAG ;
         }
       | type_mark
         .constraint.
         {
           ctltype_list *ctltype_pnt;
  
           ctltype_pnt = (ctltype_list *)chktab(hshtab,$1.NAME,CTP_MODNAM,CTP_PNTDFN);
  
           $$.NAME = NULL;
           $$.BASE = ctltype_pnt;
           $$.VALUE = NULL ;
           $$.SIZE = 0 ;
           $$.INDEX = CTP_NUMTYP;
           $$.CLASS = $2.FLAG;

           $$.LEFT    = $2.LEFT ;
           $$.RIGHT   = $2.RIGHT ;
        }
      ;

type_or_function_name
        : simple_name
          { $$ = $1; }
        ;


scalar_type_definition
        : enumeration_type_definition
          {
            $$ = $1;
          }
        ;
enumeration_type_definition
        : LeftParen
          enumeration_literal
          ...enumeration_literal..
          RightParen_ERR
          {
              char *enumname;
              char *enumval;
              int   size;
              int   indice;
              int   numbit;
              char **pnt;
              chain_list *nm1lst;

              CTP_NM1LST = reverse (CTP_NM1LST);

              nm1lst = CTP_NM1LST;
              size   = 0;

              while (nm1lst != NULL)
              {
                size++;
                nm1lst = nm1lst->NEXT;
              }

              pnt = (char **) autallocblock(size * sizeof(char*));

              indice = 0;

              while (CTP_NM1LST != NULL)
              {
                enumname = (char *)CTP_NM1LST->DATA;

                if (chktab (hshtab,enumname,CTP_MODNAM,CTP_SYMDFN) != 0)
                  my_ctp_error (82,enumname);

                addtab (hshtab,enumname,CTP_MODNAM,CTP_SYMDFN,CTP_CSTDFN);
                addtab (hshtab,enumname,CTP_MODNAM,CTP_TYPDFN,CTP_NUMTYP);
                addtab (hshtab,enumname,CTP_MODNAM,CTP_WMNDFN,-1);
                addtab (hshtab,enumname,CTP_MODNAM,CTP_WMXDFN,-1);
                addtab (hshtab,enumname,CTP_MODNAM,CTP_LBLDFN,indice);
                addtab (hshtab,enumname,CTP_MODNAM,CTP_PNTDFN,(long)pnt);
                
                pnt[indice++] = enumname;
                CTP_NM1LST = delchain (CTP_NM1LST, CTP_NM1LST);
              }

              $$.VALUE = pnt ;
              $$.SIZE = size ;
              $$.LEFT = -1 ;
              $$.RIGHT = -1 ;
              $$.INDEX = CTP_NUMTYP;
              $$.CLASS = 'E' ;
              $$.BASE = 0 ;
        }
        ;


...enumeration_literal..
        : /*empty*/
        | ...enumeration_literal..
          Comma
          enumeration_literal
                { yyerrok; }
        ;

enumeration_literal
        : Identifier
          {
            CTP_NM1LST = addchain (CTP_NM1LST,$1);
          }
        | CharacterLit
          {
            CTP_NM1LST = addchain (CTP_NM1LST,$1);
          }
        ;

composite_type_definition
        : array_type_definition
          {
            $$ = $1;
          }
        ;

array_type_definition
        : constrained_array_definition
              { $$ = $1; }
        ;

constrained_array_definition
        : ARRAY
          index_constraint
          OF
          subtype_indication
          {
            ctltype_list *NewType;
            char       *name;
            char        buffer[ 32 ];

            $$ = $4;

            if ( $4.CLASS == 'U' )
            {
              if ( $4.BASE->CLASS != 'A' )
              {
                my_ctp_error(123,NULL);
              }
            }
            else
            if ( $4.CLASS == 'A' )
            {
              if ( $4.BASE->CLASS != 'U' )
              {
                my_ctp_error(123,NULL);
              }

              sprintf( buffer, "_subtype_%d", CTP_NUMTYP );
              name = namealloc( buffer );

              NewType = addctltype(CTP_HEADFIG,name,$4.INDEX, $4.LEFT,
                   $4.RIGHT,$4.SIZE,$4.VALUE,$4.CLASS,$4.BASE );
              addctlline( CTP_HEADFIG, &NewType->LINE, CTP_LINNUM);
              addtab(hshtab,name,CTP_MODNAM,CTP_SYMDFN,CTP_TPEDFN);
              addtab(hshtab,name,CTP_MODNAM,CTP_LBLDFN,$4.CLASS);
              addtab(hshtab,name,CTP_MODNAM,CTP_TYPDFN,$4.INDEX);
              addtab(hshtab,name,CTP_MODNAM,CTP_WMNDFN,$4.LEFT);
              addtab(hshtab,name,CTP_MODNAM,CTP_WMXDFN,$4.RIGHT);
              addtab(hshtab,name,CTP_MODNAM,CTP_PNTDFN,(long)CTP_HEADFIG->TYPE );
              CTP_NUMTYP++;

              $$.BASE = NewType;
            }
            else
            {
              my_ctp_error(123,NULL);
            }

            $$.CLASS = 'A';
            $$.INDEX = $2.TYPE;

            $$.LEFT  = $2.LEFT;
            $$.RIGHT = $2.RIGHT;
          }
        ;

index_constraint
        : LeftParen
          discrete_range
          RightParen_ERR
         { $$ = $2;}
        ;

discrete_range
        : range  /* includes attribute_name */
         { $$ = $1;}
        | name
          range_constraint   /* subtype_indication - causes conflict */
         { $$ = $2;
           $$.NAME = $1.NAME;
         }
       | type_mark     /* type_mark causes 2 r/r conflicts - required for
                           louie's code */
         { $$ = $1;}
        ;

.constraint.
       : /*empty*/
        {
          $$.FLAG   = 'U';
          $$.LEFT   = -1;
          $$.RIGHT  = -1;
        }
        | constraint
        { 
          $$ = $1;
        }
       ;

constraint
       : range_constraint
         { 
           $$ = $1;
         }
        |
         LeftParen
         range
         RightParen_ERR
         {
           $$ = $2;
         }
         ;

range_constraint
       : _RANGE
         range
         { $$ = $2; }
        ;

range
       : attribute_name
         {
           if ( ( $1.FLAG != CTP_RANGE     ) &&
                ( $1.FLAG != CTP_REV_RANGE ) )
           {
             my_ctp_error (32,NULL);
           }

           $$ = $1;
         }
       | simple_expression
         direction
         simple_expression
         {
           long Left;
           long Right;
           int  ErrorL;
           int  ErrorR;

           ErrorL = ctp_vextonum( $1.VEX, &Left  );
           ErrorR = ctp_vextonum( $3.VEX, &Right );

           $$.NAME   = NULL;
           $$.TYPE   = $1.TYPE;
           $$.SIGNED = 0;
           $$.FLAG   = 'A';

           if ( ( ! ErrorL ) && ( ! ErrorR ) )
           {
             if (((Left > Right) && ($2 == CTP_UPTDFN)) ||
                  ((Left < Right) && ($2 == CTP_DWTDFN)))
             {
               my_ctp_error (32,NULL);
             }

             $$.LEFT   = Left;
             $$.RIGHT  = Right;
           }
           else
           {
             $$.LEFT    = (long)$1.VEX;
             $$.RIGHT   = (long)$3.VEX;
           }
         }
       ;

direction
       : TO
                { $$ = CTP_UPTDFN; }
        | DOWNTO
                { $$ = CTP_DWTDFN; }
       ;


type_mark
          :
           simple_name
           {
              int type;
              $$.NAME = $1;
              $$.LEFT = -1;
              $$.RIGHT = -1;
              $$.TYPE = chktab(hshtab,$1,CTP_MODNAM,CTP_TYPDFN);
              $$.FLAG = chktab(hshtab,$1,CTP_MODNAM,CTP_LBLDFN);
              type = chktab(hshtab,$1,CTP_MODNAM,CTP_SYMDFN);

              if (type != CTP_TPEDFN || $$.TYPE ==0)
              {
                my_ctp_error(84,$1);
              }
          }
        ;

ctl_statement_part
       : ..concurrent_statement..
       ;

..concurrent_statement..
       : /*empty*/
       | ..concurrent_statement..
         concurrent_statement
       ;

concurrent_statement
       : formula_assignment_statement
       | error
         Semicolon_ERR
                { my_ctp_error (18,NULL); }
       ;

formula_assignment_statement
       : simple_name
         Colon
         expression
         Semicolon_ERR
         {
           ctlform_list *CtlForm;
           vexexpr      *VexForm;

           VexForm = simpvexexpr( $3.VEX );

           if (chktab (hshtab,$1, CTP_MODNAM,CTP_SYMDFN) != 0)
           {
             my_ctp_error (80,$1);
           }

           CtlForm = addctlform( CTP_HEADFIG, $1, VexForm );
           addctlline( CTP_HEADFIG, &CtlForm->LINE, CTP_LINNUM);

           addtab(hshtab,$1,CTP_MODNAM,CTP_SYMDFN,CTP_LABDFN);
         }
       ;

expression
       : relation..AND__relation..
                { $$ = $1; }
       | relation..OR__relation..
                { $$ = $1; }
       | relation.NAND_NOR_NXOR_relation.
                { $$ = $1; }
       | relation..XOR__relation..
                { $$ = $1; }
       ;

relation..AND__relation..
       : relation
         _AND
         relation
         { 
           $$ = ctp_crtvex (VEX_AND ,$1 ,$3 ,-1,-1);
         } 
       | relation..AND__relation..
         _AND
         relation
         { 
           $$ = ctp_crtvex (VEX_AND ,$1 ,$3 ,-1,-1);
         } 
       ;

relation..OR__relation..
       : relation
         _OR
         relation
         { 
           $$ = ctp_crtvex (VEX_OR ,$1 ,$3 ,-1,-1);
         } 
       | relation..OR__relation..
         _OR
         relation
         { 
           $$ = ctp_crtvex (VEX_OR ,$1 ,$3 ,-1,-1);
         } 
       ;

relation.NAND_NOR_NXOR_relation.
       : relation
         { $$ = $1; }
       | relation
         _NAND
         relation
         { 
           $$ = ctp_crtvex (VEX_NAND ,$1 ,$3 ,-1,-1);
         }
       | relation
         _NOR
         relation
         { 
           $$ = ctp_crtvex (VEX_NOR ,$1 ,$3 ,-1,-1);
         }
       | relation
         _NXOR
         relation
         { 
           $$ = ctp_crtvex (VEX_NXOR ,$1 ,$3 ,-1,-1);
         }
       | relation
         _IFT
         relation
         { 
           $$ = ctp_crtvex (VEX_IFT,$1 ,$3 ,-1,-1);
         }
       | _EU
         LeftParen
         relation
         Comma
         relation
	 RightParen_ERR
         { 
           $$ = ctp_crtvex (VEX_EU,$3 ,$5 ,-1,-1);
         }
       | _AU
         LeftParen
         relation
         Comma
         relation
	 RightParen_ERR
         { 
           $$ = ctp_crtvex (VEX_AU,$3 ,$5 ,-1,-1);
         }
       ;

relation..XOR__relation..
       : relation
         _XOR
         relation
         {
           $$ = ctp_crtvex (VEX_XOR ,$1 ,$3 ,-1,-1);
         }
       | relation..XOR__relation..
         _XOR
         relation
         {
           $$ = ctp_crtvex (VEX_XOR ,$1 ,$3 ,-1,-1);
         }
       ;

relation
       : simple_expression
                { $$ = $1; }
       | simple_expression
         relational_operator
         simple_expression
         { 
           $$ = ctp_crtvex ($2 ,$1 ,$3 ,-1,-1);
         }
       ;

simple_expression
       : .sign.term..add_op__term..
                { $$ = $1; }
       ;

.sign.term..add_op__term..
        : term
          { $$ = $1; }
        | sign
          term
          {
             if ( $1 == VEX_NEG )
             {
               $$ = ctp_crtvex( VEX_NEG,$2,NULL,-1,-1);
             }
             else
             {
               $$ = $2;
             }
          }
        | .sign.term..add_op__term..
          adding_operator
          term
          {
            $$ = ctp_crtvex($2,$1,$3,-1,-1);
          }
        ;

adding_operator
        : Plus
               { $$ = VEX_ADD; }
        | Minus
               { $$ = VEX_SUB; }
        | Ampersand
               { $$ = VEX_CONCAT; }
        ;
sign
        : Plus
               { $$ = 0; }
        | Minus
               { $$ = VEX_NEG; }
        ;

multiplying_operator
        : Star
               { $$ = VEX_MUL; }
        | Slash
               { $$ = VEX_DIV; }
        | MOD
               { $$ = VEX_MOD; }
        | REM
               { $$ = VEX_REM; }
        ;

term
       : factor
         { $$ = $1; }
       | term
         multiplying_operator
         factor
         {
           $$ = ctp_crtvex( $2, $1, $3,-1,-1);
         }
       ;

factor
       : primary
         {
           $$ = $1;
         }
       | primary
         DoubleStar
         primary
         {
           $$ = ctp_crtvex( VEX_EXP,$1,$3,-1,-1);
         }
       | ABS
         primary
         { 
           $$ = ctp_crtvex( VEX_ABS,$2,CTP_EMPSTR,-1,-1); 
         }
       | _NOT
         primary
         { 
           $$ = ctp_crtvex( VEX_NOT,$2,CTP_EMPSTR,-1,-1);
         }
       | _AG
         primary
         { 
           $$ = ctp_crtvex( VEX_AG,$2,CTP_EMPSTR,-1,-1);
         }
       | _AF
         primary
         { 
           $$ = ctp_crtvex( VEX_AF,$2,CTP_EMPSTR,-1,-1);
         }
       | _AX
         primary
         { 
           $$ = ctp_crtvex( VEX_AX,$2,CTP_EMPSTR,-1,-1);
         }
       | _EF
         primary
         { 
           $$ = ctp_crtvex( VEX_EF,$2,CTP_EMPSTR,-1,-1);
         }
       | _EG
         primary
         { 
           $$ = ctp_crtvex( VEX_EG,$2,CTP_EMPSTR,-1,-1);
         }
       | _EX
         primary
         { 
           $$ = ctp_crtvex( VEX_EX,$2,CTP_EMPSTR,-1,-1);
         }
       ;

primary
       : literal
        {
           struct ctp_expr expr1;

           expr1.IDENT = $1;
           expr1.TYPE  = -1;
           expr1.VEX   = (vexexpr *)0;

           $$ = ctp_crtvex( NOPS,expr1,CTP_EMPSTR,-1,-1);
        }
        | aggregate
        {
            ctp_vexstr *VexStr;
            chain_list *ScanChain;

            ScanChain = $1;

            if ( ScanChain == (chain_list *)0 )
            {
              my_ctp_error(114,NULL);
            }

            if ( ScanChain->NEXT == (chain_list *)0 )
            {
              VexStr = (ctp_vexstr *)ScanChain->DATA;

              $$ = *VexStr;

              autfreeheap( VexStr, sizeof( ctp_vexstr ) );
              freechain( ScanChain );
            }
            else
            {
              $$.IDENT  = (char *)$1;
              $$.AGGREG = 1;
            }
        }
        | name
        {
          struct ctp_expr expr1;
          char           *LocalName;
          char            Buffer[ 40 ];
          long            left;
          long            right;
          long            left_bnd;
          long            right_bnd;
          long            in_bound;
          long            out_bound;
          int             mode;
          int             flag;

          flag = $1.FLAG;

          if ( ( flag != 0          ) &&
               ( flag != CTP_EVENT  ) &&
               ( flag != CTP_STABLE ) )
          {
            if ( ( flag != CTP_RANGE     ) &&
                 ( flag != CTP_REV_RANGE ) )
            {
              {
                sprintf( Buffer, "%ld", $1.LEFT );

                expr1.IDENT = Buffer;
                expr1.VEX   = (vexexpr *)0;
                expr1.TYPE  = -1;

                $$ = ctp_crtvex( NOPS, expr1, CTP_EMPSTR, -1, -1);
              }
            }
            else
            {
              autexit( 1 );
            }
          }
          else
          if ( ( $1.NAME[0] == '"' ) ||
               ( $1.NAME[0] == '\'') )
          {
            expr1.IDENT = $1.NAME;
            expr1.VEX   = (vexexpr *)0;
            expr1.TYPE  = -1;

            $$ = ctp_crtvex( NOPS ,expr1, CTP_EMPSTR, -1, -1); /* as for literal */
          }
          else
          {
            LocalName = CTP_MODNAM;
            mode = chktab (hshtab,$1.NAME,LocalName,CTP_SYMDFN);

            if( mode == 0 )
            {
              my_ctp_error (17,$1.NAME);
            }

            left_bnd  = chktab (hshtab,$1.NAME,LocalName,CTP_WMNDFN);
            right_bnd = chktab (hshtab,$1.NAME,LocalName,CTP_WMXDFN);
              
            left  = $1.LEFT;
            right = $1.RIGHT;

            {
              int  type;

              expr1.IDENT = $1.NAME;
              type = chktab(hshtab,$1.NAME,LocalName,CTP_TYPDFN);
              expr1.TYPE = type;
              expr1.SIGNED = chktab (hshtab,$1.NAME,LocalName,CTP_SUNDFN);

              {
                expr1 = ctp_crtvex (NOPI,expr1,CTP_EMPSTR,left,right);
              }

              if( flag == CTP_EVENT )
              {
                $$ = ctp_crtvex (VEX_EVENT,expr1,CTP_EMPSTR,left,right);
              }
              else
              if( flag == CTP_STABLE )
              {
                expr1 = ctp_crtvex(VEX_EVENT,expr1,CTP_EMPSTR,left,right);
                $$    = ctp_crtvex (VEX_NOT,expr1,CTP_EMPSTR,-1,-1);
              }
              else
              {
                $$ = expr1;
              }
            }
          }
        }
        | qualified_expression
        {
          struct ctp_expr expr1;

          expr1.IDENT  = $1.NAME;
          expr1.TYPE   = $1.TYPE;
          expr1.SIGNED = $1.SIGNED;
          expr1.VEX    = (vexexpr *)$1.LEFT;

          expr1 = ctp_crtvex(SUBVAR,expr1,CTP_EMPSTR,expr1.VEX->LEFT,expr1.VEX->RIGHT);

          $$ = expr1;
        }
        ;

qualified_expression
        : name
          Apostrophe
          aggregate
          {
            char       *LocalName;
            ctp_vexstr *VexStr;
            chain_list *ScanChain;
            vexexpr    *VexExpr;
            long        Def;

            ScanChain = $3;

            if ( ( ScanChain       == (chain_list *)0 ) ||
                 ( ScanChain->NEXT != (chain_list *)0 ) )
            {
              my_ctp_error(122,NULL);
            }

            LocalName = CTP_MODNAM;
            Def = chktab (hshtab,$1.NAME,LocalName,CTP_SYMDFN);

            if ( Def == CTP_TPEDFN )
            {
              VexStr = (ctp_vexstr *)ScanChain->DATA;

              VexExpr = createvexfunc( $1.NAME, VexStr->VEX->WIDTH );
              addvexhexpr( VexExpr, VexStr->VEX );

              autfreeheap( VexStr, sizeof( ctp_vexstr ) );

              $$.NAME    = $1.NAME;
              $$.TYPE    = chktab (hshtab,$1.NAME,LocalName,CTP_TYPDFN);
              $$.SIGNED  = chktab (hshtab,$1.NAME,LocalName,CTP_SUNDFN);
              $$.FLAG    = 0;
              $$.LEFT    = (long)VexExpr;
              $$.RIGHT   = 0;

              freechain( $3 );
            }
            else
            {
              my_ctp_error(122, $1.NAME);
            }
          }
        ;

relational_operator
       : _EQSym
              { $$ = VEX_EQ; }
       | _NESym
              { $$ = VEX_NE; }
       | _GESym
              { $$ = VEX_GE; }
       | _GTSym
              { $$ = VEX_GT; }
       | _LESym
              { $$ = VEX_LE; }
       | _LTSym
              { $$ = VEX_LT; }
       | Equiv
              { $$ = VEX_EQUIV; }
       | Imply
              { $$ = VEX_IMPLY; }
       ;

literal
       : AbstractLit
              { $$ = $1; }
       | CharacterLit
              { 
                $$ = $1; }
       | StringLit
              { $$ = $1; }
       | BitStringLit
              { $$ = $1; }
       | _TRUE
              { $$ = namealloc( "'1'" ); }
       | _FALSE
              { $$ = namealloc( "'0'" ); }
       ;

aggregate
        : LeftParen
          element_association
          ...element_association..
          RightParen_ERR
          {
            if ( $3 != (chain_list *)0 )
            {
              $2->NEXT = reverse( $3 );
            }

            $$ = $2;
          }
        ;

...element_association..
        : /* empty */
          { 
            $$ = (chain_list *)0;
          }
        | ...element_association..
          Comma
          element_association
          {
            if ( $1 != (chain_list *)0 )
            {
              $3->NEXT = $1;
            }

            $$ = $3;
          }
        ;

element_association
        : expression
          { 
            ctp_vexstr *VexStr;

            VexStr  = (ctp_vexstr *)autallocheap( sizeof( ctp_vexstr ) );
            *VexStr = $1;

            $$ = addchain( (chain_list *)0, VexStr );
          }
        | OTHERS
          Arrow
          expression
          {
            ctp_vexstr *VexStr;

            VexStr  = (ctp_vexstr *)autallocheap( sizeof( ctp_vexstr ) );
            *VexStr = ctp_crtvex( VEX_OTHERS, $3,CTP_EMPSTR,-1,-1);

            $$ = addchain( (chain_list *)0, VexStr );
          }
        ;

name
       : simple_name
         {
           authelem *valbitstr;
           char     *codedsigname;
           char     *LocalName;
           char      buffer[128];

           LocalName = CTP_MODNAM;

           $$.NAME = $1;
           $$.FLAG = 0;

           $$.TYPE   = chktab (hshtab,$1,LocalName,CTP_TYPDFN);
           $$.LEFT   = chktab (hshtab,$1,LocalName,CTP_WMNDFN);
           $$.RIGHT  = chktab (hshtab,$1,LocalName,CTP_WMXDFN);
           $$.SIGNED = chktab (hshtab,$1,LocalName,CTP_SUNDFN);
         }
       | indexed_name
              { $$ = $1; }
       | slice_name
              { $$ = $1; }
       | attribute_name
              { $$ = $1; }
       ;

indexed_name
       :  simple_name
          aggregate
          {
            char       *LocalName;
            ctp_vexstr *VexStr;
            chain_list *ScanChain;
            vexexpr    *VexExpr;
            vexexpr    *VexRet;
            long        Index;
            int         Error;
            long        Def;

            ScanChain = $2;

            if ( ScanChain == (chain_list *)0 )
            {
              my_ctp_error(114,NULL);
            }

            LocalName = CTP_MODNAM;
            Def = chktab (hshtab,$1,LocalName,CTP_SYMDFN);

            if ( ScanChain->NEXT != (chain_list *)0 )
            {
              if ( ( ! Def                                    ) || 
                   ( ScanChain->NEXT->NEXT != (chain_list *)0 ) )
              {
                my_ctp_error(118,$1);
              }

              /* ICI */

              $$.NAME   = $1;
              $$.TYPE   = chktab (hshtab,$1,LocalName,CTP_TYPDFN);
              $$.SIGNED = chktab (hshtab,$1,LocalName,CTP_SUNDFN);
              $$.FLAG   = 0;

              VexStr  = (ctp_vexstr *)ScanChain->DATA;
              $$.LEFT = (long)VexStr->VEX;
              autfreeheap( VexStr, sizeof( ctp_vexstr ) );

              ScanChain = ScanChain->NEXT;
              VexStr   = (ctp_vexstr *)ScanChain->DATA;
              $$.RIGHT = (long)VexStr->VEX;
              autfreeheap( VexStr, sizeof( ctp_vexstr ) );

              freechain( $2 );
            }
            else
            {
              VexStr = (ctp_vexstr *)ScanChain->DATA;
              Error  = ctp_vextonum( VexStr->VEX, &Index );

              $$.NAME   = $1;
              $$.TYPE   = chktab (hshtab,$1,LocalName,CTP_TYPDFN);
              $$.SIGNED = chktab (hshtab,$1,LocalName,CTP_SUNDFN);
              $$.FLAG   = 0;

              if ( Error )
              {
                my_ctp_error(36, $1);
              }
              else
              {
                $$.LEFT   = Index;
                $$.RIGHT  = Index;
              }

              autfreeheap( VexStr, sizeof( ctp_vexstr ) );
              freechain( ScanChain );
            }
          }
       ;

slice_name
       : simple_name
         LeftParen
         simple_expression
         direction
         simple_expression
         RightParen_ERR
         {
           char *LocalName;
           long  Left;
           long  Right;
           int   ErrorL;
           int   ErrorR;

           LocalName = CTP_MODNAM;

           ErrorL = ctp_vextonum( $3.VEX, &Left  );
           ErrorR = ctp_vextonum( $5.VEX, &Right );

           $$.NAME   = $1;
           $$.TYPE   = chktab (hshtab,$1,LocalName,CTP_TYPDFN);
           $$.SIGNED = chktab (hshtab,$1,LocalName,CTP_SUNDFN);
           $$.FLAG   = 0;

           if ( ( ! ErrorL ) && ( ! ErrorR ) )
           {
             if (((Left > Right) && ($4 == CTP_UPTDFN)) ||
                  ((Left < Right) && ($4 == CTP_DWTDFN)))
               my_ctp_error (32,$1);

             $$.LEFT   = Left;
             $$.RIGHT  = Right;
           }
           else
           {
             my_ctp_error (36,$1);
           }
         }
       ;

attribute_name
        : name
          Apostrophe
          attribute_designator
          {
            char *LocalName;
            int   type;
            int   flag;
            int   mode;
            long  AttrLeft;
            long  AttrRight;
            long  AttrLow;
            long  AttrHigh;

            LocalName = CTP_MODNAM;
            mode = chktab (hshtab,$1.NAME,LocalName,CTP_SYMDFN);
            type = chktab (hshtab,$1.NAME,LocalName,CTP_TYPDFN);
            flag = $3;

            $$.NAME   = $1.NAME;
            $$.TYPE   = type;
            $$.LEFT   = -1;
            $$.RIGHT  = -1;
            $$.SIGNED = chktab (hshtab,$1.NAME,LocalName,CTP_SUNDFN);
            $$.FLAG   = $3;

            if ( ( flag == CTP_STABLE ) ||
                 ( flag == CTP_EVENT  ) )
            {
              if ( LocalName != CTP_MODNAM )
                my_ctp_error (79,$1);
            }
            else
            {
              AttrLeft  = chktab( hshtab,$1.NAME,LocalName,CTP_ATLDFN);
              AttrRight = chktab( hshtab,$1.NAME,LocalName,CTP_ATRDFN);

              {
                if ( AttrLeft > AttrRight )
                {
                  AttrLow  = AttrRight;
                  AttrHigh = AttrLeft;
                }
                else
                {
                  AttrHigh = AttrRight;
                  AttrLow  = AttrLeft;
                }
              }

              switch ( flag )
              {
                case CTP_LEFT   : $$.LEFT = AttrLeft;
                break;

                case CTP_RIGHT  : $$.LEFT = AttrRight;
                break;

                case CTP_LOW    : $$.LEFT = AttrLow;
                break;

                case CTP_HIGH   : $$.LEFT = AttrHigh;
                break;

                case CTP_LENGTH : $$.LEFT = 1 + (AttrHigh - AttrLow);
                break;

                case CTP_RANGE  : $$.LEFT  = AttrLeft;
                                  $$.RIGHT = AttrRight;
                break;

                case CTP_REV_RANGE : $$.LEFT  = AttrRight;
                                     $$.RIGHT = AttrLeft;
                break;
              }
            }
          }
          ;

attribute_designator
        : _STABLE     { $$ = CTP_STABLE; }
        | _EVENT      { $$ = CTP_EVENT; }
        | _LEFT       { $$ = CTP_LEFT; }
        | _RIGHT      { $$ = CTP_RIGHT; }
        | _LOW        { $$ = CTP_LOW; }
        | _HIGH       { $$ = CTP_HIGH; }
        | _LENGTH     { $$ = CTP_LENGTH; }
        | _RANGE      { $$ = CTP_RANGE; }
        | _REV_RANGE  { $$ = CTP_REV_RANGE; }
        ;

identifier_list
       : Identifier
                { CTP_NM1LST = addchain (CTP_NM1LST,$1); }
         ...identifier..
       ;

...identifier..
       : /*empty*/
       | ...identifier..
         Comma
         Identifier
                { 
                  CTP_NM1LST = addchain (CTP_NM1LST,$3);
                }
       ;

simple_name
       : Identifier
              { $$ = $1; }
       ;

RightParen_ERR
       : RightParen
              { yyerrok; }
       ;

Semicolon_ERR
       : Semicolon
              { yyerrok; }
       ;

END_ERR
       : _END
              { yyerrok; }
       ;

%%

static struct dct_entry *addent (head , key)
struct dct_entry *head;
char             *key;

{
  struct dct_entry *entry = NULL;
  int              i;

  if (CTP_DCEHED == NULL)
    {
    CTP_DCEHED = (struct dct_entry *)
                 autallocblock (sizeof(struct dct_entry) * CTP_ALODFN);

    entry = CTP_DCEHED;
    for (i=1 ; i<CTP_ALODFN ; i++)
      {
      entry->next = entry + 1;
      entry++;
      }
    entry->next = NULL;
    }

  entry       = CTP_DCEHED;
  CTP_DCEHED  = CTP_DCEHED->next;

  entry->next = head;
  entry->data = NULL;
  entry->key  = key;

  return (entry);
}

static struct dct_recrd *addrcd (head , key)

struct dct_recrd *head;
char             *key;

{
  struct dct_recrd *recrd = NULL;
  int               i;

  if (CTP_DCRHED == NULL)
    {
    CTP_DCRHED = (struct dct_recrd *)
                 autallocblock (sizeof(struct dct_recrd) * CTP_ALODFN);

    recrd = CTP_DCRHED;
    for (i=1 ; i<CTP_ALODFN ; i++)
      {
      recrd->next = recrd + 1;
      recrd++;
      }
    recrd->next = NULL;
    }

  recrd           = CTP_DCRHED;
  CTP_DCRHED      = CTP_DCRHED->next;

  recrd->next     = head;
  recrd->key      = key;

  return (recrd);
}

static struct dct_entry **initab ()

{
  struct dct_entry **head = NULL;
  int                i;

  head = (struct dct_entry **)
         autallocblock (sizeof(struct dct_entry *) * CTP_HSZDFN);

  for (i=0 ; i<CTP_HSZDFN ; i++)
    head[i] = NULL;

  return (head);
}

static void addtab (head,key_str,ctx_str,field,valu)

struct dct_entry **head;
char              *key_str;
char              *ctx_str;
long               field;
long               valu;

{
  long              found = 0;
  long              index;
  struct dct_entry *entry_pnt;
  struct dct_recrd *recrd_pnt;

  index     = ((unsigned long) key_str) % CTP_HSZDFN;
  entry_pnt = head[index];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->key == key_str)
      {
      found = 1;
      break;
      }
    entry_pnt = entry_pnt->next;
    }

  if (found == 0)
    {
    head[index] = addent (head[index],key_str);
    entry_pnt = head[index];
    }

  found = 0;
  recrd_pnt = entry_pnt->data;
  while (recrd_pnt != NULL)
    {
    if (recrd_pnt->key == ctx_str)
      {
      found = 1;
      break;
      }
    recrd_pnt = recrd_pnt->next;
    }

  if (found == 0)
    {
    entry_pnt->data = addrcd (entry_pnt->data,ctx_str);
    recrd_pnt       = entry_pnt->data ;
    }

  switch (field)
    {
      case CTP_MODDFN :
        recrd_pnt->fd0_val = valu;
        break;
      case CTP_SYMDFN :
        recrd_pnt->fd1_val = valu;
        break;
      case CTP_TYPDFN :
        recrd_pnt->fd2_val = valu;
        break;
      case CTP_SUNDFN :
        recrd_pnt->fd3_val = valu;
        break;
      case CTP_LBLDFN :
        recrd_pnt->fd4_val = valu;
        break;
      case CTP_WMXDFN :
        recrd_pnt->fd5_val = valu;
        break;
      case CTP_WMNDFN :
        recrd_pnt->fd6_val = valu;
        break;
      case CTP_ATLDFN :
        recrd_pnt->fd7_val = valu;
        break;
      case CTP_ATRDFN :
        recrd_pnt->fd8_val = valu;
        break;
      case CTP_PNTDFN :
        recrd_pnt->pnt_val = valu;
        break;
    }
}

static int chktab (head,key_str,ctx_str,field)

struct dct_entry **head;
char              *key_str;
char              *ctx_str;
long               field;

{
  long              found = 0;
  long              valu = 0;
  struct dct_entry *entry_pnt;
  struct dct_recrd *recrd_pnt;

  entry_pnt = head [((unsigned long)key_str) % CTP_HSZDFN];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->key == key_str)
      {
      found = 1;
      break;
      }
    entry_pnt = entry_pnt->next;
    }

  if (found == 1)
    {
    found = 0;
    recrd_pnt = entry_pnt->data;
    while (recrd_pnt != NULL)
      {
      if (recrd_pnt->key == ctx_str)
        {
        found = 1;
        break;
        }
      recrd_pnt = recrd_pnt->next;
      }
    if (found == 1)
      {
      switch (field)
        {
        case CTP_MODDFN :
          valu = recrd_pnt->fd0_val;
          break;
        case CTP_SYMDFN :
          valu = recrd_pnt->fd1_val;
          break;
        case CTP_TYPDFN :
          valu = recrd_pnt->fd2_val;
          break;
        case CTP_SUNDFN :
          valu = recrd_pnt->fd3_val;
          break;
        case CTP_LBLDFN :
          valu = recrd_pnt->fd4_val;
          break;
        case CTP_WMXDFN :
          valu = recrd_pnt->fd5_val;
          break;
        case CTP_WMNDFN :
          valu = recrd_pnt->fd6_val;
          break;
        case CTP_ATLDFN :
          valu = recrd_pnt->fd7_val;
          break;
        case CTP_ATRDFN :
          valu = recrd_pnt->fd8_val;
          break;
        case CTP_PNTDFN :
          valu = recrd_pnt->pnt_val;
          break;
        }
      }
    }

  return (valu);
}

static void fretab (pt_hash)

struct dct_entry **pt_hash;
{
  struct dct_entry *pt_entry;
  struct dct_entry *pt_nxtentry;
  struct dct_recrd *pt_record;
  long              i;

  if (pt_hash != NULL)
    {
    for (i=0 ; i<CTP_HSZDFN ; i++)
      {
      if ((pt_entry = pt_hash[i]) != NULL)
        {
        while (pt_entry != NULL)
          {
          pt_record = pt_entry->data;

          while (pt_record->next != NULL)
            pt_record = pt_record->next;

          pt_record->next = CTP_DCRHED;
          CTP_DCRHED      = pt_entry->data;

          pt_nxtentry     = pt_entry->next;
          pt_entry->next  = CTP_DCEHED;
          CTP_DCEHED      = pt_entry;
          pt_entry        = pt_nxtentry;
          }
        }
      }
    autfreeblock(pt_hash);
    }
}

static void *ctp_addstr(object,mode,prtype,type,flag,name,left,right,exp)

char          object;
int           mode;
ctltype_list *prtype;
unsigned char type;
char          flag;
char         *name;
long          left;
long          right;
vexexpr      *exp;

{ 
  ctlform_list *CtlForm;
  ctldecl_list *CtlDeclar;
  void         *pnt    = NULL;
  vexexpr      *vex_pnt;
  int           bitsize;

  if ( object == 'F' )
  {
    CtlForm = addctlform( CTP_HEADFIG, name, exp );
    addctlline( CTP_HEADFIG, &CtlForm->LINE, CTP_LINNUM );

    return( (void *)CtlForm );
  }


  if ( (flag == 'A') ||
       (flag == 'U') )
  {
       /* ###------------------------------------------------------### */
       /*   if object is an array ...                                   */
       /* ###------------------------------------------------------### */

    if ( flag == 'A' )
    {
      vex_pnt = createvexatomvec( name, prtype->LEFT, prtype->RIGHT ); 
    }
    else
    {
      vex_pnt = createvexatomvec( name, left, right );
    }

    if ( prtype->INDEX == VEX_TYPE_SIGNED )
    {
      SetVexNodeSigned( vex_pnt );
    }

  }
  else
  if ( (flag == 'I') )
  {
       /* ###------------------------------------------------------### */
       /*   if object is an integer ...                            */
       /* ###------------------------------------------------------### */

      if ((left == -1 ) && (right == -1))
      {
        left  = prtype->LEFT;
        right = prtype->RIGHT;
      }

      bitsize = ctp_intSize( left, right );

      vex_pnt = createvexatomvec( name, bitsize - 1, 0 );

      if ( ( left  < 0 ) ||
           ( right < 0 ) )
      {
        SetVexNodeSigned( vex_pnt );
      }
  }
  else
  {
       /* ###------------------------------------------------------### */
       /*   if object is a scalar ...                                   */
       /* ###------------------------------------------------------### */
    vex_pnt = createvexatombit( name );
  }

  if ( object == 'C' )
  {
    CtlDeclar = addctldecldef( CTP_HEADFIG, vex_pnt );
    CtlDeclar->TYPE     = prtype;
    CtlDeclar->VEX_INIT = exp;
    addctlline( CTP_HEADFIG, &CtlDeclar->LINE, CTP_LINNUM );

    pnt = (void *)CtlDeclar;
  }
  else
  if ( object == 'V' )
  {
    CtlDeclar = addctldeclvar( CTP_HEADFIG, vex_pnt );
    CtlDeclar->TYPE     = prtype;
    addctlline( CTP_HEADFIG, &CtlDeclar->LINE, CTP_LINNUM );

    pnt = (void *)CtlDeclar;
  }
  else
  if ( object == 'A' )
  {
    CtlDeclar = addctldeclass( CTP_HEADFIG, vex_pnt );
    CtlDeclar->TYPE     = prtype;
    CtlDeclar->VEX_INIT = exp;
    addctlline( CTP_HEADFIG, &CtlDeclar->LINE, CTP_LINNUM );

    pnt = (void *)CtlDeclar;
  }
  else
  if ( object == 'I' )
  {
    CtlDeclar = addctldeclinit( CTP_HEADFIG, vex_pnt );
    CtlDeclar->TYPE     = prtype;
    CtlDeclar->VEX_INIT = exp;
    addctlline( CTP_HEADFIG, &CtlDeclar->LINE, CTP_LINNUM );

    pnt = (void *)CtlDeclar;
  }
  else
  if ( object == 'R' )
  {
    CtlDeclar = addctldeclreset( CTP_HEADFIG, vex_pnt );
    CtlDeclar->TYPE     = prtype;
    CtlDeclar->VEX_INIT = exp;
    addctlline( CTP_HEADFIG, &CtlDeclar->LINE, CTP_LINNUM );

    pnt = (void *)CtlDeclar;
  }

  return (pnt);
}

static ctltype_list *val_type(name)

 char *name;
{
  return( searchctltype( CTP_HEADFIG, name ) );
}

static ctltype_list *get_type(val)

  long val;
{
  ctltype_list *Type;

  for ( Type  = CTP_HEADFIG->TYPE;
        Type != (ctltype_list *)0;
        Type  = Type->NEXT )
  {
    if  ( Type->INDEX == val ) break;
  }

  return( Type );
}

int ctp_y_wrap ()
{
  return (1);
}

