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
/* file         : vbl_bcomp.yac                                         */
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
#include "vbh.h"

#include "vbl_utype.h"
#include "vbl_bedef.h"
#include "vbl_bspec.h"
#include "vbl_byacc.h"

#if 1
# define my_fprintf fprintf( stdout, "%s %d : ", basename(__FILE__), __LINE__ ); fprintf
# define my_vbl_error(N,V) \
   do { fprintf( stderr, "%s %d : ", basename(__FILE__), __LINE__); vbl_error(N,V); } while(0)
#else
# define my_fprintf   fprintf
# define my_vbl_error vbl_error
#endif

#ifdef VBL_DEBUG
#define YYDEBUG 1
#endif

%}

%union  {
          int                valu;
          char              *text;
          chain_list        *pcha;
          vbl_name           name;
          vbl_vexstr         list;
          struct vbtyp       type;
       };

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
%token <valu> CONSTANT
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

%left		_AND _OR _NAND _NOR _XOR
%left		_EQSym _NESym _LTSym _LESym _GTSym _GESym
%left		Plus Minus Ampersand
%left		Star Slash MOD REM
%left		DoubleStar
%right		ABS _NOT

%type <valu> .SEVERITY__expression.
%type <valu> severity__message
%type <text> .REPORT__expression.
%type <text> report__message

%type <list> .WHEN__condition.
%type <valu> .GUARDED.

%type <list> .VarAsgn__expression.
%type <list> waveform
%type <list> waveform_element
%type <list> condition
%type <list> expression
%type <list> .expression.
%type <list> guard_expression
%type <list> .guard_expression.
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
%type <name> choice
%type <pcha> aggregate
%type <pcha> element_association
%type <pcha> ...element_association..

%type <list> .condition_clause.
%type <list> condition_clause
%type <list> .iteration_scheme.
%type <list> iteration_scheme
%type <list> loop_parameter_specification

%type <type> type_definition
%type <type> scalar_type_definition
%type <type> composite_type_definition
%type <type> enumeration_type_definition
%type <type> subtype_indication
%type <type> unconstrained_array_definition
%type <type> constrained_array_definition
%type <type> array_type_definition

%type <text> library_clause
%type <text> use_clause
%type <text> selected_name
%type <text> ...selected_name..

%type <name> index_subtype_definition
%type <name> index_constraint
%type <name> discrete_range
%type <name> qualified_expression

%type <text> .label.
%type <text> label
%type <valu> time_label
%type <text> a_label
%type <name> target
%type <text> simple_name
%type <text> prefix
%type <text> suffix
%type <text> type_or_function_name
%type <name> name
%type <name> attribute_name
%type <name> indexed_name
%type <name> slice_name
%type <text> .simple_name.
%type <name> .constraint.
%type <name> constraint
%type <name> .timeout_clause.
%type <name> timeout_clause
%type <name> range
%type <name> range_constraint
%type <valu> direction
%type <valu> .mode.
%type <valu> .local_port_mode.
%type <valu> .procedure_parameter_mode.
%type <valu> .procedure_parameter_object_class.
%type <valu> .function_parameter_mode.
%type <valu> .function_parameter_object_class.
%type <name> type_mark
%type <valu> .signal_kind.
%type <valu> attribute_designator
%type <text> designator

%type <list> generation_scheme
%type <list> generate_parameter_specification

%type <valu> entity_class

%type <list> constant_VarAsgn__expression

%start design_file

%%

design_file
        : {
                /* ###----------------------------------------------###  */
                /*    Initializations                                    */
                /* ###----------------------------------------------###  */

            VBL_NM1LST = NULL;
            VBL_NM2LST = NULL;
            VBL_LOOPLBLLST = NULL;
            VBL_LOOPLBL = NULL;
            VBL_INSLST = NULL;
            VBL_GRDLST = NULL;
            VBL_CNDLST = NULL;
            VBL_LINLST = NULL;
            VBL_VALLST = NULL;
            VBL_ENUMVAL = createauthtable( 100 );
            hshtab = initab ();

            VBL_ERRFLG    = 0;
            VBL_LINNUM    = 1;
            VBL_NUMPTP    = 0;
            VBL_NUMCHOICE = 0;

            VBL_MODNAM  = namealloc( "undefined" );
            VBL_PROCNAM = (char *)0;
            VBL_GNRNAM  = (char *)0;
            VBL_FUNCNAM = (char *)0;
            VBL_COMPNAM = (char *)0;
            VBL_BEFUN   = (vbfun_list *)0;

            VBL_HEADFIG = vbh_addvbfig( VBL_HEADFIG, 0);
            VBL_BEFPNT  = VBL_HEADFIG;

            VBL_BEPCS   = (vbpcs_list *)0;
            VBL_BEINS   = (vbins_list *)0;
            VBL_BEGNR   = (vbgnr_list *)0;
            VBL_BEMAP   = (vbmap_list *)0;

            VBL_GNR_BEPCS = (chain_list *)0;
            VBL_GNR_BEINS = (chain_list *)0;
            VBL_GNR_BEGNR = (chain_list *)0;
          }
	  design_unit
	  ..design_unit..
          {
            VBL_BEFPNT->BEPCS = VBL_BEPCS;
            VBL_BEFPNT->BEINS = VBL_BEINS;
            VBL_BEFPNT->BEGNR = VBL_BEGNR;

            fretab( hshtab );
            destroyauthtable( VBL_ENUMVAL );
          }
	;

..design_unit..
	: /*empty*/
	| design_unit
	  ..design_unit..
	;

design_unit
	: context_clause
	  library_unit
          {}
	;

library_unit
	: primary_unit
          {}
	| secondary_unit
          {}
	| error
          {}
	;

primary_unit
	: entity_declaration
          {}
	| configuration_declaration
          {}
	| package_declaration
          {}
	;

secondary_unit
	: architecture_body
          {}
	| package_body
          {}
	;

library_clause
        : _LIBRARY
          logical_name_list
          Semicolon_ERR
          { }
        ;

logical_name_list
        : Identifier
          ...logical_name..
          { }
        ;

...logical_name..
        : /*empty*/
        | ...logical_name..
          Comma
          Identifier
          { }
        ;

/* 11.3 */
context_clause
        : ..context_item..
          { }
        ;

..context_item..
        : /*empty*/
        | ..context_item..
          context_item
          { }
        ;

context_item
        : library_clause
          { }
        | use_clause
          { }
        ;

use_clause
        : USE
          selected_name    /* package simple name */
          ...selected_name..
          Semicolon_ERR
          {
          }
        ;

/* 1.3  */
configuration_declaration
	: CONFIGURATION
	  Identifier
	  OF
	  Identifier
	  IS
	  block_configuration
	  END_ERR
	  .simple_name.
	  Semicolon_ERR
          {}
	| CONFIGURATION error
          {}
	;

/* block configuration */
block_configuration
	: FOR
	  label
	  END_ERR
	  FOR
	  Semicolon_ERR
          {}
	;


/* Packages */

/* 2.5 */
package_declaration
        : _PACKAGE 
          Identifier
          IS
         {
           vbfig_list  *ScanPkgFig;
            chain_list *ScanChain;
            char       *Name;
            char      **FuncArray;
            int         Index;   
            long        Left;
            long        Right;
            long        AttrLeft;
            long        AttrRight;
            long        Dir;
            int         Signed;
            vbcst_list *VbhCst;
            vbaux_list *VbhAux;
            vbmod_list *VbhMod;
            vbpor_list *VbhPor;
            vbgen_list *VbhGen;
            vbtyp_list *VbhType;
            vbpcs_list *VbhPcs;
            vexexpr    *VexAtom;
            long        Dynamic;
            long        Oper;
            chain_list *PackList;

                extern char *vbl_func_std_logic_1164[];
                extern char *vbl_func_std_logic_arith[];
                extern char *vbl_func_std_numeric_std[];

           int size=0;
           struct vbtyp *vbtyp_pnt;
             VBL_BEFPNT->NAME = $2;
             VBL_BEFPNT->IS_PACKAGE = 1;
           VBL_MODNAM = $2;

           vbtyp_pnt = VBL_BEFPNT->BETYP;
           while(vbtyp_pnt)
           {
             size++;
              vbtyp_pnt = vbtyp_pnt->NEXT;
           }

           vbtyp_pnt  = VBL_BEFPNT->BETYP;
           VBL_NUMTYP = 0;
           while(vbtyp_pnt)
           {
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_SYMDFN,VBL_TPEDFN);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_LBLDFN,vbtyp_pnt->CLASS);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_TYPDFN,vbtyp_pnt->INDEX);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_WMNDFN,vbtyp_pnt->LEFT);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_WMXDFN,vbtyp_pnt->RIGHT);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_PNTDFN,(long)vbtyp_pnt);
              vbtyp_pnt = vbtyp_pnt->NEXT;
              VBL_NUMTYP++;
           }

            for ( ScanChain  = VBL_NM2LST;
                  ScanChain != (chain_list *)0;
                  ScanChain  = ScanChain->NEXT )
            {
              FuncArray = (char **)0;

              if ( ! strcmp( ScanChain->DATA, "ieee.std_logic_1164.all" ) )
              {
                FuncArray = vbl_func_std_logic_1164;
              }
              else
              if ( ( ! strcmp( ScanChain->DATA, "ieee.std_logic_arith.all"    ) ) ||
                   ( ! strcmp( ScanChain->DATA, "ieee.std_logic_signed.all"   ) ) ||
                   ( ! strcmp( ScanChain->DATA, "ieee.std_logic_unsigned.all" ) ) )
              {
                FuncArray = vbl_func_std_logic_arith;
              }
              else
              if ( ( ! strcmp( ScanChain->DATA, "ieee.numeric_std.all" ) ) ||
                   ( ! strcmp( ScanChain->DATA, "ieee.numeric_bit.all" ) ) )
              {
                FuncArray = vbl_func_std_numeric_std;
              }
              else
              {
                for ( ScanPkgFig  = VBL_HEADFIG;
                      ScanPkgFig != (vbfig_list *)0;
                      ScanPkgFig  = ScanPkgFig->NEXT )
                {
                  if ( ( ScanPkgFig->IS_PACKAGE                         ) &&
                       ( ScanPkgFig->NAME == (char *)ScanChain->DATA ) )
                  {
                    for ( PackList  = VBL_BEFPNT->PACK_LIST;
                          PackList != (chain_list *)0;
                          PackList  = PackList->NEXT )
                    {
                      if ( PackList->DATA == ScanChain->DATA ) break; /* already included */
                    }

                    if ( PackList == (chain_list *)0 )
                    {
                      vbh_dupvbfig( VBL_BEFPNT, ScanPkgFig );
                    }

                    /* BECST */

                    for ( VbhCst  = ScanPkgFig->BECST;
                          VbhCst != (vbcst_list *)0;
                          VbhCst  = VbhCst->NEXT )
                    {
                      VbhType = vbh_getvbtyp( VBL_BEFPNT, VbhCst->TYPE->NAME );
                      VexAtom = VbhCst->TARGET;

                      if ( ! IsVexNodeAtom( VexAtom ) )
                      {
                        my_fprintf( stdout, "dynamic declaration not supported !\n" );
                        autexit( 1 );
                      }

                      Name = GetVexAtomValue( VexAtom );

		      /*
                      if ( chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN ) )
                      {
                        my_vbl_error( 10, Name );
                      }
		      */

                      if ( IsVexAtomBit( VexAtom ) )
                      {
                        Left  = -1;
                        Right = -1;
                      }
                      else
                      {
                        Left  = VexAtom->LEFT;
                        Right = VexAtom->RIGHT;
                      }

                      Signed    = ( IsVexNodeSigned( VexAtom ) != 0 );
                      AttrLeft  = Left;
                      AttrRight = Right;

                      addtab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN, VBL_CSTDFN     );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_TYPDFN, VbhType->INDEX );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_WMNDFN, Left           );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_WMXDFN, Right          );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_ATLDFN, AttrLeft       );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_ATRDFN, AttrRight      );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_LBLDFN, 0              );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_PNTDFN, (long)VbhCst->VEX  );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_SUNDFN, Signed         );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_DYNDFN, 0              );
                    }

                    /* BEAUX */

                    for ( VbhAux  = ScanPkgFig->BEAUX;
                          VbhAux != (vbaux_list *)0;
                          VbhAux  = VbhAux->NEXT )
                    {
                      VbhType = vbh_getvbtyp( VBL_BEFPNT, VbhAux->TYPE->NAME );
                      VexAtom = VbhAux->TARGET;

                      if ( ! IsVexNodeAtom( VexAtom ) )
                      {
                        my_fprintf( stdout, "dynamic declaration not supported !\n" );
                        autexit( 1 );
                      }

                      Name = GetVexAtomValue( VexAtom );

		      /*
                      if ( chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN ) )
                      {
                        my_vbl_error( 10, Name );
                      }
		      */

                      Signed    = (IsVexNodeSigned( VexAtom ) != 0);
                      Left      = VexAtom->LEFT;
                      Right     = VexAtom->RIGHT;
                      AttrLeft  = Left;
                      AttrRight = Right;

                      addtab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN, VBL_SGNDFN     );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_TYPDFN, VbhType->INDEX );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_WMNDFN, Left           );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_WMXDFN, Right          );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_ATLDFN, AttrLeft       );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_ATRDFN, AttrRight      );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_LBLDFN, VbhAux->KIND   );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_PNTDFN, (long)VexAtom  );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_SUNDFN, Signed         );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_DYNDFN, 0              );
                    }

                    /* BEPCS */

                    for ( VbhPcs  = ScanPkgFig->BEPCS;
                          VbhPcs != (vbpcs_list *)0; 
                          VbhPcs  = VbhPcs->NEXT )
                    {
                      addtab( hshtab, VbhPcs->LABEL, VBL_MODNAM, VBL_SYMDFN, VBL_LBLDFN );
                    }

                    /* BEFUN TO BE DONE */

                    if ( ScanPkgFig->BEFUN != (vbfun_list *)0 )
                    {
                      my_fprintf( stdout, "function in package not supported !\n" );
                      autexit( 1 );
                    }

                    /* BEATR TO BE DONE */

                    if ( ScanPkgFig->BEATR != (vbatr_list *)0 )
                    {
                      my_fprintf( stdout, "attribute in package not supported !\n" );
                      autexit( 1 );
                    }

                    /* BEGNR TO BE DONE ??? */

                    if ( ScanPkgFig->BEGNR != (vbgnr_list *)0 )
                    {
                      my_fprintf( stdout, "generate in package not supported !\n" );
                      autexit( 1 );
                    }

                    /* BEMOD */

                    for ( VbhMod  = ScanPkgFig->BEMOD;
                          VbhMod != (vbmod_list *)0;
                          VbhMod  = VbhMod->NEXT )
                    {
                      addtab(hshtab,VbhMod->NAME,VBL_MODNAM,VBL_SYMDFN,VBL_COMDFN);
                      addtab(hshtab,VbhMod->NAME,VBL_MODNAM,VBL_PNTDFN,(long)VbhMod);

                      for ( VbhGen  = VbhMod->BEGEN;
                            VbhGen != (vbgen_list *)0;
                            VbhGen  = VbhGen->NEXT )
                      {
                        VbhType = vbh_getvbtyp( VBL_BEFPNT, VbhGen->TYPE->NAME );
                        VexAtom = VbhGen->TARGET;
  
                        if ( ! IsVexNodeAtom( VexAtom ) )
                        {
                          my_fprintf( stdout, "dynamic declaration not supported !\n" );
                          autexit( 1 );
                        }
  
                        Name = GetVexAtomValue( VexAtom );
  
			/*
                        if ( chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN ) )
                        {
                          my_vbl_error( 10, Name );
                        }
			*/
  
                        Signed    = (IsVexNodeSigned( VexAtom ) != 0);
                        Left      = VexAtom->LEFT;
                        Right     = VexAtom->RIGHT;
                        AttrLeft  = Left;
                        AttrRight = Right;
  
                        addtab( hshtab, Name, VbhMod->NAME, VBL_SYMDFN, VBL_GENDFN     );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_TYPDFN, VbhType->INDEX );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_WMNDFN, Left           );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_WMXDFN, Right          );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_ATLDFN, AttrLeft       );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_ATRDFN, AttrRight      );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_LBLDFN, VBH_KIND_NONE  );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_PNTDFN, (long)VexAtom  );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_SUNDFN, Signed         );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_DYNDFN, 0              );
                      }

                      for ( VbhPor  = VbhMod->BEPOR;
                            VbhPor != (vbpor_list *)0;
                            VbhPor  = VbhPor->NEXT )
                      {
                        VbhType = vbh_getvbtyp( VBL_BEFPNT, VbhPor->TYPE->NAME );
                        VexAtom = VbhPor->TARGET;
  
                        if ( ! IsVexNodeAtom( VexAtom ) )
                        {
                          Name    = getvexatomname( VexAtom );
                          Dynamic = 0;

                          if ( IsVexNodeOper( VexAtom ) )
                          {
                            Oper = GetVexOperValue( VexAtom );

                            if ( Oper == VEX_DOWNTO ) Dynamic = VBL_DWTDFN;
                            else         
                            if ( Oper == VEX_TO     ) Dynamic = VBL_UPTDFN;

                            Signed    = (IsVexNodeSigned( VexAtom ) != 0);
                            Left      = (long)GetVexOperand( VexAtom->OPERAND );
                            Right     = (long)GetVexOperand( VexAtom->OPERAND->NEXT );
                            AttrLeft  = Left;
                            AttrRight = Right;
                          }

                          if ( ! Dynamic )
                          {
                            my_fprintf( stdout, "such dynamic declaration not supported !\n" );
                            autexit( 1 );
                          }
                        }
                        else
                        {
                          Name = GetVexAtomValue( VexAtom );

                          Signed    = (IsVexNodeSigned( VexAtom ) != 0);
                          Left      = VexAtom->LEFT;
                          Right     = VexAtom->RIGHT;
                          AttrLeft  = Left;
                          AttrRight = Right;
                          Dynamic   = 0;
                        }
  
			/*
                        if ( chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN ) )
                        {
                          my_vbl_error( 10, Name );
                        }
			*/
  
                        if ( VbhPor->DIR == 'I' ) Dir = VBL_ICNDFN;
                        else 
                        if ( VbhPor->DIR == 'O' ) Dir = VBL_OCNDFN;
                        else                      Dir = VBL_BCNDFN;
  
                        addtab( hshtab, Name, VbhMod->NAME, VBL_SYMDFN, Dir            );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_TYPDFN, VbhType->INDEX );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_WMNDFN, Left           );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_WMXDFN, Right          );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_ATLDFN, AttrLeft       );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_ATRDFN, AttrRight      );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_LBLDFN, VbhPor->KIND   );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_PNTDFN, (long)VexAtom  );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_SUNDFN, Signed         );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_DYNDFN, Dynamic        );
                      }
                    }

                    /* BEINS */

                    if ( ScanPkgFig->BEINS != (vbins_list *)0 )
                    {
                      my_fprintf( stdout, "instance in package not supported !\n" );
                      autexit( 1 );
                    }

                    break;
                  }
                }

                if ( ScanPkgFig == (vbfig_list *)0 )
                {
                  my_vbl_error(127,(char *)ScanChain->DATA);
                }
              }

              if ( FuncArray != (char **)0 )
              {
                Index = 0;

                while ( FuncArray[ Index ] != NULL )
                {
                  Name = namealloc( FuncArray[ Index ] );
                  addtab ( hshtab, Name, VBL_MODNAM, VBL_SYMDFN, VBL_FUNDFN );
                  Index++;
                }
              }
            }

            freechain( VBL_NM2LST );
            VBL_NM2LST = NULL;
         }
          package_declarative_part
          END_ERR
          .simple_name.
          Semicolon_ERR
          {
             if (($7 != NULL) && ($7 != $2)) my_vbl_error (128,$7);
          }
        | _PACKAGE 
          error
          { }
        ;

package_declarative_part
        : ..package_declarative_item..
          { }
        ;

..package_declarative_item..
        : /*empty*/
        | ..package_declarative_item..
          package_declarative_item
          { }
        ;

package_declarative_item
        : type_declaration
        | subtype_declaration
        | attribute_declaration
        | constant_declaration
        | subprogram_declaration
        | component_declaration
        | attribute_specification
        | signal_declaration
        | error END_ERR
          Semicolon_ERR
        ;

/* 2.6 */
/* package bodies */

package_body
        : _PACKAGE
          BODY
          Identifier
          IS  
          {
            if ($3 != VBL_MODNAM) my_vbl_error (128,$3);
          }
          package_body_declarative_part
          END_ERR
          .simple_name.
          Semicolon_ERR
          { 
            if (($8 != NULL) && ($8 != $3)) my_vbl_error (128,$8);
          }
        | _PACKAGE
           BODY
           error
           { my_vbl_error ( 129,NULL); }
        ;

package_body_declarative_part
        : ..package_body_declarative_item..
        ;

..package_body_declarative_item..
        : /*empty*/
        | ..package_body_declarative_item..
          package_body_declarative_item
        ;        

package_body_declarative_item
        : subprogram_declaration
        | subprogram_body
        | type_declaration
        | subtype_declaration
        | constant_declaration
        ;



/* 9.6 */
component_instantiation_statement
        : a_label
          /* Identifier */
          name
          {
            if (chktab (hshtab,$1,VBL_MODNAM,VBL_SYMDFN) != 0)
            {
              my_vbl_error (19,$1);
            }

            addtab (hshtab,$1,VBL_MODNAM,VBL_SYMDFN,VBL_LBLDFN);

            if (chktab (hshtab,$2.NAME,VBL_MODNAM,VBL_SYMDFN) != VBL_COMDFN)
            {
              my_vbl_error (125,$2.NAME);
            }

            VBL_BEINS = vbh_addvbins( VBL_BEINS, $1, $2.NAME, VBL_LINNUM );
            VBL_COMPNAM = $2.NAME;
            VBL_BEMAP = (vbmap_list *)0;
          }
          .generic_map_aspect.
          {
            VBL_BEINS->GEN_MAP = VBL_BEMAP;
            VBL_BEMAP = (vbmap_list *)0;
          }
          .port_map_aspect.
          {
            VBL_BEINS->PORT_MAP = VBL_BEMAP;
            VBL_BEMAP = (vbmap_list *)0;
          }
          Semicolon_ERR
          {
            vbmod_list *VbMod;
            vbgen_list *VbGen;
            vbpor_list *VbPor;
            vbins_list *VbIns;
            vbmap_list *VbMap;
            int         Explicit;

            VbMod = (vbmod_list *)chktab(hshtab,$2.NAME,VBL_MODNAM,VBL_PNTDFN);
            VbIns = VBL_BEINS;

            VbIns->PORT_MAP = (vbmap_list *)reverse( (chain_list *)VbIns->PORT_MAP );

            VbPor    = VbMod->BEPOR;
            VbMap    = VbIns->PORT_MAP;
            Explicit = 0;

            while ( ( VbMap != (vbmap_list *)0 ) &&
                    ( VbPor != (vbpor_list *)0 ) )
            {
              if ( VbMap->FORMAL == (vexexpr *)0 )
              {
                if ( Explicit )
                {
                  my_vbl_error (125, VbIns->NAME);
                }

                VbMap->FORMAL = dupvexexpr( VbPor->TARGET );
                VbPor = VbPor->NEXT;
              }
              else
              {
                Explicit = 1;
              }

              VbMap = VbMap->NEXT;
            }

            if ( VbMap != (vbmap_list *)0 )
            {
              my_vbl_error (125, VbIns->NAME);
            }

            VbIns->GEN_MAP = (vbmap_list *)reverse( (chain_list *)VbIns->GEN_MAP );

            VbGen    = VbMod->BEGEN;
            VbMap    = VbIns->GEN_MAP;
            Explicit = 0;

            while ( ( VbMap != (vbmap_list *)0 ) &&
                    ( VbGen != (vbgen_list *)0 ) )
            {
              if ( VbMap->FORMAL == (vexexpr *)0 )
              {
                if ( Explicit )
                {
                  my_vbl_error (125, VbIns->NAME);
                }

                VbMap->FORMAL = dupvexexpr( VbGen->TARGET );
                VbGen = VbGen->NEXT;
              }
              else
              {
                Explicit = 1;
              }

              VbMap = VbMap->NEXT;
            }

            if ( VbMap != (vbmap_list *)0 )
            {
              my_vbl_error (125, VbIns->NAME);
            }

            VBL_COMPNAM = (char *)0;
          }
        ;

.port_map_aspect.
        : /*empty*/
        {}
        | port_map_aspect
        {}
        ;
port_map_aspect
        :
          PORT
          MAP
          LeftParen        /* was: PORT aggregate */
          association_list
          RightParen_ERR
        {}
        ;


.generic_map_aspect.
        : /*empty*/
        {}
        | generic_map_aspect
        {}
        ;
generic_map_aspect
        :
          GENERIC
          MAP
          LeftParen
          association_list
          RightParen_ERR
        {}
        ;

/* 9.7 */
/*
**  To avoid shift/reduce conflicts, define labeled/unlabeled generate
**  statements.
*/
generate_statement
        : a_label
          {
            if ( chktab (hshtab,$1,VBL_MODNAM,VBL_SYMDFN) != 0)
             my_vbl_error (19,$1);

            addtab( hshtab,$1,VBL_MODNAM,VBL_SYMDFN,VBL_LBLDFN);

            VBL_GNRNAM = $1;
          }
          unlabeled_generate_statement
          {
          }
        ;

unlabeled_generate_statement
        : generation_scheme
          GENERATE
          {
            vbl_name      *Range;
            vbvar_list    *VbVar;
            vexexpr       *Target;
            short          Signed;
            long           Left;
            long           Right;
            long           AttrLeft;
            long           AttrRight;
            char           ForUp;

            VBL_BEGNR = vbh_addvbgnr( VBL_BEGNR, VBL_GNRNAM, VBL_LINNUM );

            if ( $1.IDENT != (char *)0 )
            {
              if ( chktab( hshtab,$1.IDENT,VBL_MODNAM,VBL_SYMDFN ) != 0 )
              {
                my_vbl_error (80,$1.IDENT);
              }

              Range = (vbl_name *)$1.VEX;

              if ( Range->DYNAMIC )
              {
                if ( Range->DYNAMIC == VBL_UPTDFN ) ForUp = 1;
                else                                ForUp = 0;

                AttrLeft  = -1;
                AttrRight = -1;
              }
              else
              {
                AttrLeft  = Range->LEFT;
                AttrRight = Range->RIGHT;

                if ( AttrLeft <= AttrRight ) ForUp = 1;
                else                         ForUp = 0;
              }

              VbVar = (vbvar_list *)vbl_addstr( 'f',0,
                                               get_type(VBH_TYPE_INTEGER),VBH_TYPE_INTEGER,
                                              'I',$1.IDENT, -1, -1, NULL, 0, 0 );
              Target = VbVar->TARGET;
              Signed = ( IsVexNodeSigned( VbVar->TARGET ) != 0 );

              if ( IsVexAtomBit( VbVar->TARGET ) )
              {
                Left  = -1;
                Right = -1;
              }
              else
              {
                Left  = VbVar->TARGET->LEFT;
                Right = VbVar->TARGET->RIGHT;
              }

              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_SYMDFN,VBL_VARDFN);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_TYPDFN,VBH_TYPE_INTEGER);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_WMNDFN,Left);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_WMXDFN,Right);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_ATLDFN,AttrLeft);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_ATRDFN,AttrRight);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_LBLDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_PNTDFN,(long)VbVar->TARGET);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_SUNDFN,Signed);

              if ( ! Range->DYNAMIC )
              {
                Range->LEFT  = (long)createvexatomlong( AttrLeft , 0, Signed );
                Range->RIGHT = (long)createvexatomlong( AttrRight, 0, Signed );
              }

              VBL_BEGNR->TYPE = VBH_GENERATE_FOR;
              VBL_BEGNR->FOR_VARIABLE = VbVar->TARGET;
              VBL_BEGNR->FOR_LEFT     = (vexexpr *)Range->LEFT;
              VBL_BEGNR->FOR_RIGHT    = (vexexpr *)Range->RIGHT;
              VBL_BEGNR->FOR_UP       = ForUp;

              autfreeheap( Range, sizeof( vbl_name ) );
            }
            else
            {
              VBL_BEGNR->TYPE = VBH_GENERATE_IF;
              VBL_BEGNR->IF_COND = $1.VEX;
            }

            VBL_GNR_BEINS = addchain( VBL_GNR_BEINS, (void *)VBL_BEINS );
            VBL_GNR_BEPCS = addchain( VBL_GNR_BEPCS, (void *)VBL_BEPCS );
            VBL_GNR_BEGNR = addchain( VBL_GNR_BEGNR, (void *)VBL_BEGNR );

            VBL_BEINS = (vbins_list *)0;
            VBL_BEPCS = (vbpcs_list *)0;
            VBL_BEGNR = (vbgnr_list *)0;

          }
          set_of_statements
          END_ERR
          GENERATE
          .label.
          Semicolon_ERR
          {
            vbgnr_list *BeGnr;

            if ( $1.IDENT != (char *)0 )
            {
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_SYMDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_TYPDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_WMNDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_WMXDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_ATLDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_ATRDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_LBLDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_PNTDFN,0);
              addtab( hshtab,$1.IDENT,VBL_MODNAM,VBL_SUNDFN,0);
            }

            BeGnr = (vbgnr_list *)VBL_GNR_BEGNR->DATA;

            BeGnr->BEINS = VBL_BEINS;
            BeGnr->BEPCS = VBL_BEPCS;
            BeGnr->BEGNR = VBL_BEGNR;

            VBL_BEINS = (vbins_list *)VBL_GNR_BEINS->DATA;
            VBL_BEPCS = (vbpcs_list *)VBL_GNR_BEPCS->DATA;
            VBL_BEGNR = (vbgnr_list *)VBL_GNR_BEGNR->DATA;

            VBL_GNR_BEINS = delchain( VBL_GNR_BEINS, VBL_GNR_BEINS );
            VBL_GNR_BEPCS = delchain( VBL_GNR_BEPCS, VBL_GNR_BEPCS );
            VBL_GNR_BEGNR = delchain( VBL_GNR_BEGNR, VBL_GNR_BEGNR );
          }
        ;

generation_scheme
        : FOR
          generate_parameter_specification
          {
            $$ = $2;
          }
        | IF
          condition
          {
            $$ = $2;
          }
        ;

generate_parameter_specification
        : Identifier
          _IN
          discrete_range
          {
            vbl_name *Range;

            Range  = (vbl_name *)autallocheap( sizeof( vbl_name ) );
            *Range = $3;

            $$.IDENT = $1;
            $$.VEX   = (vexexpr *)Range;
          }
        ;

association_list
        : association_element
          ...association_element..
        {}
        ;

...association_element..
        : /*empty*/
        {}
        | Comma
          association_element
          ...association_element..
           {
             yyerrok;
           }
        ;

association_element
        : name
          Arrow
	  {
	    VBL_SAVCOMPNAM = VBL_COMPNAM;
	    VBL_COMPNAM    = (char *)0;
	  }
          expression
          {
            vexexpr *Formal;
            vexexpr *Actual;

          struct vbl_expr expr1;
          struct vbl_expr expr_name;
          char           *LocalName;
          char            Buffer[ 40 ];
          long            left;
          long            right;
          long            left_bnd;
          long            right_bnd;
          long            in_bound;
          long            out_bound;
          unsigned char   dynamic;
          int             mode;
          int             flag;

	  VBL_COMPNAM = VBL_SAVCOMPNAM;
	  VBL_SAVCOMPNAM = (char *)0;

          flag = $1.FLAG;

          if ( ( flag != 0          ) &&
               ( flag != VBL_EVENT  ) &&
               ( flag != VBL_STABLE ) )
          {
            if ( ( flag != VBL_RANGE     ) &&
                 ( flag != VBL_REV_RANGE ) )
            {
              sprintf( Buffer, "%ld", $1.LEFT );

              expr1.IDENT = Buffer;
              expr1.VEX   = (vexexpr *)0;
              expr1.TYPE  = -1;

              expr_name = vbl_crtvex( NOPS, expr1, VBL_EMPSTR, -1, -1, 0);
            }
            else
            {
              my_vbl_error(124,$1.NAME);
            }
          }
          else
          if ( ( $1.NAME[0] == '"' ) ||
               ( $1.NAME[0] == '\'') )
          {
            my_vbl_error(124,$1.NAME);
          }
          else
          {
            LocalName = VBL_COMPNAM;
            mode = chktab (hshtab,$1.NAME,LocalName,VBL_SYMDFN);

            if( mode == 0 || mode == VBL_PRODFN )
            {
              my_vbl_error (17,$1.NAME);
            }

            /* LUDO_DYNAMIC */
            dynamic   = chktab (hshtab,$1.NAME,LocalName,VBL_DYNDFN);
            left_bnd  = chktab (hshtab,$1.NAME,LocalName,VBL_WMNDFN);
            right_bnd = chktab (hshtab,$1.NAME,LocalName,VBL_WMXDFN);
              
            if ( ( ! $1.DYNAMIC ) &&
                 ( ! dynamic    ) )
            {
              left  = $1.LEFT;
              right = $1.RIGHT;
              
              if ( left_bnd <= right_bnd )
              {
                if (left <= right)
                {
                  in_bound  = left  - left_bnd;
                  out_bound = right - left_bnd;
                  if ((left < left_bnd) || (right > right_bnd))
                  {
                    my_vbl_error (36,$1.NAME);
                  }
                }
                else
                {
                  in_bound  = right - left_bnd;
                  out_bound = left  - left_bnd;
                  if ((left > right_bnd) || (right < left_bnd))
                  {
                    my_vbl_error (36,$1.NAME);
                  }
                }
              }
              else
              {
                if (left <= right)
                {
                  in_bound  = left  - right_bnd;
                  out_bound = right - right_bnd;
                  if ((left < right_bnd) || (right > left_bnd))
                  {
                    my_vbl_error (36,$1.NAME);
                  }
                }
                else
                {
                  in_bound  = right - right_bnd;
                  out_bound = left  - right_bnd;
                  if ((left > left_bnd) || (right < right_bnd))
                  {
                    my_vbl_error (36,$1.NAME);
                  }
                }
              }
            }
            else
            {
              left  = $1.LEFT;
              right = $1.RIGHT;
            }

            {
              int  type;

              expr1.IDENT = $1.NAME;
              type = chktab(hshtab,$1.NAME,LocalName,VBL_TYPDFN);
              expr1.TYPE = type;
              expr1.SIGNED = chktab (hshtab,$1.NAME,LocalName,VBL_SUNDFN);

              if ( $1.DYNAMIC )
              {
                if ( ( mode == VBL_FUNDFN ) ||
                     ( mode == VBL_TPEDFN ) ||
                     ( mode == VBL_PRODFN ) )
                {
                  expr1.VEX = (vexexpr *)left;
                  expr1 = 
                     vbl_crtvex(SUBVAR,expr1,VBL_EMPSTR,expr1.VEX->LEFT,expr1.VEX->RIGHT,0);
                }
                else
                {
                  expr1 = vbl_crtvex (NOPI,expr1,VBL_EMPSTR,left_bnd,right_bnd,dynamic);

                  if ( $1.DYNAMIC == VBL_UPTDFN ) 
                  {
                    if ( ( ! dynamic                                                  ) ||
                         ( ! isvexequalexpr( (vexexpr *)left_bnd, (vexexpr *)left   ) ) ||
                         ( ! isvexequalexpr( (vexexpr *)right_bnd, (vexexpr *)right ) ) )
                    {
                      expr1 = vbl_crtvex( VEX_TO, expr1,VBL_EMPSTR,left,right,0);
                    }
                  }
                  else
                  if ( $1.DYNAMIC == VBL_DWTDFN )
                  {
                    if ( ( ! dynamic                                                  ) ||
                         ( ! isvexequalexpr( (vexexpr *)left_bnd, (vexexpr *)left   ) ) ||
                         ( ! isvexequalexpr( (vexexpr *)right_bnd, (vexexpr *)right ) ) )
                    {
                      expr1 = vbl_crtvex( VEX_DOWNTO, expr1,VBL_EMPSTR,left,right,0);
                    }
                  }
                  else
                  {
                    expr1.VEX = 
                      createvexbinexpr( VEX_INDEX_N, 1, expr1.VEX, (vexexpr *)left );
                    expr1.VEX = 
                      createvexbinexpr( VEX_INDEX_N, 1, expr1.VEX, (vexexpr *)right );
                  }
                }
              }
              else
              {
                expr1 = vbl_crtvex (NOPI,expr1,VBL_EMPSTR,left,right,0);
              }

              if( flag == VBL_EVENT )
              {
                expr_name = vbl_crtvex (VEX_EVENT,expr1,VBL_EMPSTR,left,right,0);
              }
              else
              if( flag == VBL_STABLE )
              {
                expr1 = vbl_crtvex(VEX_EVENT,expr1,VBL_EMPSTR,left,right,0);
                expr_name    = vbl_crtvex (VEX_NOT,expr1,VBL_EMPSTR,-1,-1,0);
              }
              else
              {
                expr_name = expr1;
              }
            }
          }

            Formal = expr_name.VEX;
            Actual = $4.VEX;

          VBL_BEMAP = vbh_addvbmap( VBL_BEMAP, Formal, Actual, VBL_LINNUM);
          }
        | /* actual_part */
          expression   /* can be name also */
          {
            vexexpr *Actual;

            Actual = $1.VEX;

            VBL_BEMAP = vbh_addvbmap( VBL_BEMAP, (vexexpr *)0, Actual, VBL_LINNUM);
          }
        ;

...selected_name..
        : /*empty*/
          { }
        | ...selected_name..
          Comma
          selected_name
          { }
        ;

/* 4.5 */
component_declaration
        : COMPONENT
          Identifier
          {
            VBL_COMPNAM = $2;
            VBL_BEFPNT->BEMOD = vbh_addvbmod( VBL_BEFPNT->BEMOD, VBL_COMPNAM, VBL_LINNUM );
            addtab(hshtab,$2,VBL_MODNAM,VBL_SYMDFN,VBL_COMDFN);
            addtab(hshtab,$2,VBL_MODNAM,VBL_PNTDFN,(long)VBL_BEFPNT->BEMOD);
          }
          .GENERIC__local_generic_list.
          .PORT__local_port_list.
          END_ERR
          COMPONENT
          Semicolon_ERR
          { VBL_COMPNAM = (char *)0;
            VBL_BEFPNT->BEMOD->BEPOR = 
               (vbpor_list *)reverse( (chain_list *)VBL_BEFPNT->BEMOD->BEPOR ); 
            VBL_BEFPNT->BEMOD->BEGEN = 
               (vbgen_list *)reverse( (chain_list *)VBL_BEFPNT->BEMOD->BEGEN ); 
          } 

        ;

.PORT__local_port_list.
        : /*empty*/
        | PORT
          LeftParen
          local_port_list
          RightParen_ERR
          Semicolon_ERR
          { }
        ;

local_port_list
        : local_port_element
          ...local_port_element..
          { }
        | error RightParen_ERR
          { }
        ;


...local_port_element..
        : /*empty*/
        | ...local_port_element..
          Semicolon_ERR
          local_port_element
          { }
        ;


local_port_element
        : .SIGNAL.
          identifier_list
          Colon
          .local_port_mode.
          type_mark
          .constraint.
          .signal_kind.
         {
            char       *signame;
            vbpor_list *VbPort;
            vbtyp_list *VbType;
            short       Signed;
            long        Left;
            long        Right;
            long        AttrLeft;
            long        AttrRight;

            VbType = val_type( $5.NAME );

            if ( $6.DYNAMIC ) /* LUDO_DYNAMIC */
            {
              AttrLeft  = $6.LEFT;
              AttrRight = $6.RIGHT;
              Left      = AttrLeft;
              Right     = AttrRight;
            }
            else
            if ( ( $6.LEFT  == -1 ) &&
                 ( $6.RIGHT == -1 ) )
            {
              AttrLeft  = VbType->LEFT;
              AttrRight = VbType->RIGHT;
            }
            else
            {
              AttrLeft  = $6.LEFT;
              AttrRight = $6.RIGHT;
            }

            VBL_NM1LST = reverse( VBL_NM1LST );

            while (VBL_NM1LST != NULL)
            {
              signame = (char *)VBL_NM1LST->DATA;
              if (chktab (hshtab,signame,VBL_COMPNAM,VBL_SYMDFN) != 0)
                my_vbl_error (4,signame);

              VbPort = (vbpor_list *)vbl_addstr( 'M',$4, VbType,
                                                 $5.TYPE,$5.FLAG,signame,
                                                 AttrLeft, AttrRight,
                                                 (vexexpr *)0,VBH_KIND_NONE, $6.DYNAMIC);

              addtab (hshtab,signame,VBL_COMPNAM,VBL_SYMDFN,$4);

              Signed = ( IsVexNodeSigned( VbPort->TARGET ) != 0 );

              if ( ( $5.FLAG == 'I' ) && ( $6.DYNAMIC ) )
              {
                $6.DYNAMIC = VBL_DWTDFN;
              }

              if ( ! $6.DYNAMIC ) /* LUDO_DYNAMIC */
              {
                if ( IsVexAtomBit( VbPort->TARGET ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VbPort->TARGET->LEFT;
                  Right = VbPort->TARGET->RIGHT;
                }
              }

              addtab( hshtab,signame,VBL_COMPNAM,VBL_TYPDFN,$5.TYPE);
              addtab( hshtab,signame,VBL_COMPNAM,VBL_WMNDFN,Left  );
              addtab( hshtab,signame,VBL_COMPNAM,VBL_WMXDFN,Right );
              addtab( hshtab,signame,VBL_COMPNAM,VBL_ATLDFN,AttrLeft  );
              addtab( hshtab,signame,VBL_COMPNAM,VBL_ATRDFN,AttrRight );
              addtab( hshtab,signame,VBL_COMPNAM,VBL_LBLDFN,VBH_KIND_NONE);
              addtab( hshtab,signame,VBL_COMPNAM,VBL_PNTDFN,(long)VbPort->TARGET);
              addtab( hshtab,signame,VBL_COMPNAM,VBL_SUNDFN,Signed);
              addtab( hshtab,signame,VBL_COMPNAM,VBL_DYNDFN,$6.DYNAMIC);

              VBL_NM1LST = delchain (VBL_NM1LST, VBL_NM1LST);
           }
         }
        ;

.local_port_mode.
        : /*empty*/
          { $$ = VBL_ICNDFN; }
        | _IN
          { $$ = VBL_ICNDFN; }
        | _OUT
          { $$ = VBL_OCNDFN; }
        | _INOUT
          { $$ = VBL_BCNDFN; }
        | _BUFFER
          { $$ = VBL_BCNDFN; }
        | _LINKAGE
          { $$ = VBL_BCNDFN; }
        ;

.GENERIC__local_generic_list.
        : /*empty*/
        | GENERIC
          LeftParen
          local_generic_list
          RightParen_ERR
          Semicolon_ERR
          { }
        ;

local_generic_list
        : local_generic_element
          ...local_generic_element..
          { }
        | error RightParen_ERR
          { }
        ;


...local_generic_element..
        : /*empty*/
          { }
        | ...local_generic_element..
          Semicolon_ERR
          local_generic_element
          { }
        ;


local_generic_element
        : .CONSTANT.
          identifier_list
          Colon
          .IN.
          type_mark
          .constraint.
         .VarAsgn__expression.
         {
            char       *signame;
            vbgen_list *VbGen;
            vbtyp_list *VbType;
            vexexpr    *VexInit;
            short       Signed;
            long        Left;
            long        Right;
            long        AttrLeft;
            long        AttrRight;

            if ( $6.DYNAMIC )
            {
              my_vbl_error (6,NULL);
            }

            VbType = val_type( $5.NAME );

            if ( ( $6.LEFT  == -1 ) &&
                 ( $6.RIGHT == -1 ) )
            {
              AttrLeft  = VbType->LEFT;
              AttrRight = VbType->RIGHT;
            }
            else
            {
              AttrLeft  = $6.LEFT;
              AttrRight = $6.RIGHT;
            }

            VBL_NM1LST = reverse( VBL_NM1LST );

            while (VBL_NM1LST != NULL)
            {
              VexInit = $7.VEX;

              if ( ( VexInit          != (vexexpr    *)0 ) &&
                   ( VBL_NM1LST->NEXT != (chain_list *)0 ) )
              {
                VexInit = dupvexexpr( $7.VEX );
              }

              signame = (char *)VBL_NM1LST->DATA;
              if (chktab (hshtab,signame,VBL_COMPNAM,VBL_SYMDFN) != 0)
                my_vbl_error (4,signame);

              VexInit = simpvexexpr( VexInit );

              VbGen = (vbgen_list *)vbl_addstr( 'g',0, VbType,
                                                 $5.TYPE,$5.FLAG,signame,
                                                 AttrLeft, AttrRight,VexInit,VBH_KIND_NONE, 0);

              addtab (hshtab,signame,VBL_COMPNAM,VBL_SYMDFN,VBL_GENDFN);

              Signed = ( IsVexNodeSigned( VbGen->TARGET ) != 0 );

              if ( IsVexAtomBit( VbGen->TARGET ) )
              {
                Left  = -1;
                Right = -1;
              }
              else
              {
                Left  = VbGen->TARGET->LEFT;
                Right = VbGen->TARGET->RIGHT;
              }

              addtab( hshtab,signame,VBL_COMPNAM,VBL_TYPDFN,$5.TYPE);
              addtab( hshtab,signame,VBL_COMPNAM,VBL_WMNDFN,Left  );
              addtab( hshtab,signame,VBL_COMPNAM,VBL_WMXDFN,Right );
              addtab( hshtab,signame,VBL_COMPNAM,VBL_ATLDFN,AttrLeft  );
              addtab( hshtab,signame,VBL_COMPNAM,VBL_ATRDFN,AttrRight );
              addtab( hshtab,signame,VBL_COMPNAM,VBL_LBLDFN,VBH_KIND_NONE);
              addtab( hshtab,signame,VBL_COMPNAM,VBL_PNTDFN,(long)VbGen->TARGET);
              addtab( hshtab,signame,VBL_COMPNAM,VBL_SUNDFN,Signed);

              VBL_NM1LST = delchain (VBL_NM1LST, VBL_NM1LST);
           }
         }
        ;

/* 4.4 */
attribute_declaration
        : ATTRIBUTE
          Identifier
          Colon
          simple_name
          Semicolon_ERR
          {
            if ( strcmp( $4, "string" ) )
            {
              my_vbl_error(9, $2 );
            }

            addtab(hshtab,$2,VBL_MODNAM,VBL_SYMDFN,VBL_ATTDFN);
          }
        ;

attribute_specification
        : ATTRIBUTE
          simple_name
          {
            if ( ! chktab(hshtab,$2,VBL_MODNAM,VBL_SYMDFN ) )
            {
              my_vbl_error(9,$2);
            }
          }
          OF
          simple_name
          Colon
          entity_class
          IS
          StringLit
          Semicolon_ERR
          {
            char Buffer[ 512 ];
            int  length;

            length = strlen( $9 );

            strcpy( Buffer, $9 + 1);
            Buffer[ length -2 ] = '\0';

            $9 = namealloc( Buffer );

            VBL_BEFPNT->BEATR = vbh_addvbatr( VBL_BEFPNT->BEATR, $2, $5, $7, $9, VBL_LINNUM ); 
          }
        ;

entity_class
        : _ENTITY
        { $$ = VBH_ATTR_ENTITY; }
        | ARCHITECTURE
        { $$ = VBH_ATTR_ARCHITECTURE; }
        | _PACKAGE
        { $$ = VBH_ATTR_PACKAGE; }
        | FUNCTION
        { $$ = VBH_ATTR_FUNCTION; }
        | PROCEDURE
        { $$ = VBH_ATTR_PROCEDURE; }
        | SUBTYPE
        { $$ = VBH_ATTR_SUBTYPE; }
        | CONSTANT
        { $$ = VBH_ATTR_CONSTANT; }
        | _VARIABLE
        { $$ = VBH_ATTR_VARIABLE; }
        | SIGNAL
        { $$ = VBH_ATTR_SIGNAL; }
        | _LABEL
        { $$ = VBH_ATTR_LABEL; }
        | _TYPE
        { $$ = VBH_ATTR_TYPE; }
        | CONFIGURATION
        { $$ = VBH_ATTR_CONFIGURATION; }
        | COMPONENT
        { $$ = VBH_ATTR_COMPONENT; }
        ;

selected_name
        : prefix
          Dot
          suffix
          { char buffer[ 128 ]; sprintf( buffer, "%s.%s", $1, $3 );
            $$ = namealloc( buffer ); 
            VBL_NM2LST = addchain( VBL_NM2LST, $$ );
          }
        ;

suffix
        : simple_name
          { $$ = $1; }
        | ALL
          { $$ = namealloc( "all" ); }
        ;
prefix
        : simple_name
          { $$ = $1; }
        | prefix
          Dot
          simple_name
          { char buffer[ 128 ]; sprintf( buffer, "%s.%s", $1, $3 );
            $$ = namealloc( buffer );  }
        ;

entity_declaration
       : _ENTITY
         simple_name
         IS
         {
           vbfig_list  *ScanPkgFig;
            chain_list *ScanChain;
            char       *Name;
            char      **FuncArray;
            int         Index;   
            long        Left;
            long        Right;
            long        AttrLeft;
            long        AttrRight;
            long        Dir;
            int         Signed;
            vbcst_list *VbhCst;
            vbaux_list *VbhAux;
            vbmod_list *VbhMod;
            vbpor_list *VbhPor;
            vbgen_list *VbhGen;
            vbtyp_list *VbhType;
            vbpcs_list *VbhPcs;
            vexexpr    *VexAtom;
            chain_list *PackList;
            long        Dynamic;
            long        Oper;

                extern char *vbl_func_std_logic_1164[];
                extern char *vbl_func_std_logic_arith[];
                extern char *vbl_func_std_numeric_std[];

           int size=0;
           struct vbtyp *vbtyp_pnt;
             VBL_BEFPNT->NAME = $2;
           VBL_MODNAM = $2;

            for ( ScanChain  = VBL_NM2LST;
                  ScanChain != (chain_list *)0;
                  ScanChain  = ScanChain->NEXT )
            {
              FuncArray = (char **)0;

              if ( ! strcmp( ScanChain->DATA, "ieee.std_logic_1164.all" ) )
              {
                FuncArray = vbl_func_std_logic_1164;
              }
              else
              if ( ( ! strcmp( ScanChain->DATA, "ieee.std_logic_arith.all"    ) ) ||
                   ( ! strcmp( ScanChain->DATA, "ieee.std_logic_signed.all"   ) ) ||
                   ( ! strcmp( ScanChain->DATA, "ieee.std_logic_unsigned.all" ) ) )
              {
                FuncArray = vbl_func_std_logic_arith;
              }
              else
              if ( ( ! strcmp( ScanChain->DATA, "ieee.numeric_std.all" ) ) ||
                   ( ! strcmp( ScanChain->DATA, "ieee.numeric_bit.all" ) ) )
              {
                FuncArray = vbl_func_std_numeric_std;
              }
              else
              {
                for ( ScanPkgFig  = VBL_HEADFIG;
                      ScanPkgFig != (vbfig_list *)0;
                      ScanPkgFig  = ScanPkgFig->NEXT )
                {
                  if ( ( ScanPkgFig->IS_PACKAGE                         ) &&
                       ( ScanPkgFig->NAME == (char *)ScanChain->DATA ) )
                  {
                    for ( PackList  = VBL_BEFPNT->PACK_LIST;
                          PackList != (chain_list *)0;
                          PackList  = PackList->NEXT )
                    {
                      if ( PackList->DATA == ScanChain->DATA ) break; /* already included */
                    }

                    if ( PackList == (chain_list *)0 )
                    {
                      vbh_dupvbfig( VBL_BEFPNT, ScanPkgFig );
                    }

                    /* BECST */

                    for ( VbhCst  = ScanPkgFig->BECST;
                          VbhCst != (vbcst_list *)0;
                          VbhCst  = VbhCst->NEXT )
                    {
                      VbhType = vbh_getvbtyp( VBL_BEFPNT, VbhCst->TYPE->NAME );
                      VexAtom = VbhCst->TARGET;

                      if ( ! IsVexNodeAtom( VexAtom ) )
                      {
                        my_fprintf( stdout, "dynamic declaration not supported !\n" );
                        autexit( 1 );
                      }

                      Name = GetVexAtomValue( VexAtom );

		      /*
                      if ( chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN ) )
                      {
                        my_vbl_error( 10, Name );
                      }
		      */

                      if ( IsVexAtomBit( VexAtom ) )
                      {
                        Left  = -1;
                        Right = -1;
                      }
                      else
                      {
                        Left  = VexAtom->LEFT;
                        Right = VexAtom->RIGHT;
                      }

                      Signed    = ( IsVexNodeSigned( VexAtom ) != 0 );
                      AttrLeft  = Left;
                      AttrRight = Right;

                      addtab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN, VBL_CSTDFN     );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_TYPDFN, VbhType->INDEX );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_WMNDFN, Left           );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_WMXDFN, Right          );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_ATLDFN, AttrLeft       );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_ATRDFN, AttrRight      );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_LBLDFN, 0              );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_PNTDFN, (long)VbhCst->VEX  );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_SUNDFN, Signed         );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_DYNDFN, 0              );
                    }

                    /* BEAUX */

                    for ( VbhAux  = ScanPkgFig->BEAUX;
                          VbhAux != (vbaux_list *)0;
                          VbhAux  = VbhAux->NEXT )
                    {
                      VbhType = vbh_getvbtyp( VBL_BEFPNT, VbhAux->TYPE->NAME );
                      VexAtom = VbhAux->TARGET;

                      if ( ! IsVexNodeAtom( VexAtom ) )
                      {
                        my_fprintf( stdout, "dynamic declaration not supported !\n" );
                        autexit( 1 );
                      }

                      Name = GetVexAtomValue( VexAtom );

		      /*
                      if ( chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN ) )
                      {
                        my_vbl_error( 10, Name );
                      }
		      */

                      Signed    = (IsVexNodeSigned( VexAtom ) != 0);
                      Left      = VexAtom->LEFT;
                      Right     = VexAtom->RIGHT;
                      AttrLeft  = Left;
                      AttrRight = Right;

                      addtab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN, VBL_SGNDFN     );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_TYPDFN, VbhType->INDEX );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_WMNDFN, Left           );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_WMXDFN, Right          );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_ATLDFN, AttrLeft       );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_ATRDFN, AttrRight      );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_LBLDFN, VbhAux->KIND   );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_PNTDFN, (long)VexAtom  );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_SUNDFN, Signed         );
                      addtab( hshtab, Name, VBL_MODNAM, VBL_DYNDFN, 0              );
                    }

                    /* BEPCS */

                    for ( VbhPcs  = ScanPkgFig->BEPCS;
                          VbhPcs != (vbpcs_list *)0; 
                          VbhPcs  = VbhPcs->NEXT )
                    {
                      addtab( hshtab, VbhPcs->LABEL, VBL_MODNAM, VBL_SYMDFN, VBL_LBLDFN );
                    }

                    /* BEFUN TO BE DONE */

                    if ( ScanPkgFig->BEFUN != (vbfun_list *)0 )
                    {
                      my_fprintf( stdout, "function in package not supported !\n" );
                      autexit( 1 );
                    }

                    /* BEATR TO BE DONE */

                    if ( ScanPkgFig->BEATR != (vbatr_list *)0 )
                    {
                      my_fprintf( stdout, "attribute in package not supported !\n" );
                      autexit( 1 );
                    }

                    /* BEGNR TO BE DONE ??? */

                    if ( ScanPkgFig->BEGNR != (vbgnr_list *)0 )
                    {
                      my_fprintf( stdout, "generate in package not supported !\n" );
                      autexit( 1 );
                    }

                    /* BEMOD */

                    for ( VbhMod  = ScanPkgFig->BEMOD;
                          VbhMod != (vbmod_list *)0;
                          VbhMod  = VbhMod->NEXT )
                    {
                      addtab(hshtab,VbhMod->NAME,VBL_MODNAM,VBL_SYMDFN,VBL_COMDFN);
                      addtab(hshtab,VbhMod->NAME,VBL_MODNAM,VBL_PNTDFN,(long)VbhMod);

                      for ( VbhGen  = VbhMod->BEGEN;
                            VbhGen != (vbgen_list *)0;
                            VbhGen  = VbhGen->NEXT )
                      {
                        VbhType = vbh_getvbtyp( VBL_BEFPNT, VbhGen->TYPE->NAME );
                        VexAtom = VbhGen->TARGET;
  
                        if ( ! IsVexNodeAtom( VexAtom ) )
                        {
                          my_fprintf( stdout, "dynamic declaration not supported !\n" );
                          autexit( 1 );
                        }
  
                        Name = GetVexAtomValue( VexAtom );
  
			/*
                        if ( chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN ) )
                        {
                          my_vbl_error( 10, Name );
                        }
			*/
  
                        Signed    = (IsVexNodeSigned( VexAtom ) != 0);
                        Left      = VexAtom->LEFT;
                        Right     = VexAtom->RIGHT;
                        AttrLeft  = Left;
                        AttrRight = Right;
  
                        addtab( hshtab, Name, VbhMod->NAME, VBL_SYMDFN, VBL_GENDFN     );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_TYPDFN, VbhType->INDEX );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_WMNDFN, Left           );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_WMXDFN, Right          );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_ATLDFN, AttrLeft       );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_ATRDFN, AttrRight      );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_LBLDFN, VBH_KIND_NONE  );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_PNTDFN, (long)VexAtom  );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_SUNDFN, Signed         );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_DYNDFN, 0              );
                      }

                      for ( VbhPor  = VbhMod->BEPOR;
                            VbhPor != (vbpor_list *)0;
                            VbhPor  = VbhPor->NEXT )
                      {
                        VbhType = vbh_getvbtyp( VBL_BEFPNT, VbhPor->TYPE->NAME );
                        VexAtom = VbhPor->TARGET;
  
                        if ( ! IsVexNodeAtom( VexAtom ) )
                        {
                          Name    = getvexatomname( VexAtom );
                          Dynamic = 0;

                          if ( IsVexNodeOper( VexAtom ) )
                          {
                            Oper = GetVexOperValue( VexAtom );

                            if ( Oper == VEX_DOWNTO    ) Dynamic = VBL_DWTDFN;
                            else
                            if ( Oper == VEX_TO        ) Dynamic = VBL_UPTDFN;

                            Signed    = (IsVexNodeSigned( VexAtom ) != 0);
                            Left      = (long)GetVexOperand( VexAtom->OPERAND->NEXT );
                            Right     = (long)GetVexOperand( VexAtom->OPERAND->NEXT->NEXT );
                            AttrLeft  = Left;
                            AttrRight = Right;
                          }

                          if ( ! Dynamic )
                          {
                            my_fprintf( stdout, "such dynamic declaration not supported !\n" );
                            autexit( 1 );
                          }
                        }
                        else
                        {
                          Name = GetVexAtomValue( VexAtom );

                          Signed    = (IsVexNodeSigned( VexAtom ) != 0);
                          Left      = VexAtom->LEFT;
                          Right     = VexAtom->RIGHT;
                          AttrLeft  = Left;
                          AttrRight = Right;
                          Dynamic   = 0;
                        }
  
			/*
                        if ( chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN ) )
                        {
                          my_vbl_error( 10, Name );
                        }
			*/
  
                        if ( VbhPor->DIR == 'I' ) Dir = VBL_ICNDFN;
                        else 
                        if ( VbhPor->DIR == 'O' ) Dir = VBL_OCNDFN;
                        else                      Dir = VBL_BCNDFN;
  
                        addtab( hshtab, Name, VbhMod->NAME, VBL_SYMDFN, Dir            );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_TYPDFN, VbhType->INDEX );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_WMNDFN, Left           );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_WMXDFN, Right          );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_ATLDFN, AttrLeft       );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_ATRDFN, AttrRight      );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_LBLDFN, VbhPor->KIND   );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_PNTDFN, (long)VexAtom  );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_SUNDFN, Signed         );
                        addtab( hshtab, Name, VbhMod->NAME, VBL_DYNDFN, Dynamic        );
                      }
                    }

                    /* BEINS */

                    if ( ScanPkgFig->BEINS != (vbins_list *)0 )
                    {
                      my_fprintf( stdout, "instance in package not supported !\n" );
                      autexit( 1 );
                    }

                    break;
                  }
                }

                if ( ScanPkgFig == (vbfig_list *)0 )
                {
                  my_vbl_error(127,(char *)ScanChain->DATA);
                }
              }

              if ( FuncArray != (char **)0 )
              {
                Index = 0;

                while ( FuncArray[ Index ] != NULL )
                {
                  Name = namealloc( FuncArray[ Index ] );
                  addtab ( hshtab, Name, VBL_MODNAM, VBL_SYMDFN, VBL_FUNDFN );
                  Index++;
                }
              }
            }

            freechain( VBL_NM2LST );
            VBL_NM2LST = NULL;

           vbtyp_pnt = VBL_BEFPNT->BETYP;
           while(vbtyp_pnt)
           {
             size++;
              vbtyp_pnt = vbtyp_pnt->NEXT;
           }

           vbtyp_pnt  = VBL_BEFPNT->BETYP;
           VBL_NUMTYP = 0;
           while(vbtyp_pnt)
           {
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_SYMDFN,VBL_TPEDFN);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_LBLDFN,vbtyp_pnt->CLASS);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_TYPDFN,vbtyp_pnt->INDEX);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_WMNDFN,vbtyp_pnt->LEFT);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_WMXDFN,vbtyp_pnt->RIGHT);
              addtab(hshtab,vbtyp_pnt->NAME,VBL_MODNAM,VBL_PNTDFN,(long)vbtyp_pnt);
              vbtyp_pnt = vbtyp_pnt->NEXT;
              VBL_NUMTYP++;
           }

         }
         .generic_clause.
         .port_clause.
         END_ERR
         .simple_name.
         Semicolon_ERR
          {
             if (($8 != NULL) && ($8 != $2)) my_vbl_error (1,$8);
          }
       | _ENTITY
         error
              { my_vbl_error (2,NULL); }
       ;

.constraint.
       : /*empty*/
        {
          $$.FLAG   = 'U';
          $$.LEFT   = -1;
          $$.RIGHT  = -1;
          $$.DYNAMIC = 0;
          $$.AGGREG  = 0;
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
           if ( ( $1.FLAG != VBL_RANGE     ) &&
                ( $1.FLAG != VBL_REV_RANGE ) )
           {
             my_vbl_error (32,NULL);
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

           ErrorL = vbl_vextonum( $1.VEX, &Left  );
           ErrorR = vbl_vextonum( $3.VEX, &Right );

           $$.NAME   = NULL;
           $$.TYPE   = $1.TYPE;
           $$.SIGNED = 0;
           $$.FLAG   = 'A';

           if ( ( ! ErrorL ) && ( ! ErrorR ) )
           {
             if (((Left > Right) && ($2 == VBL_UPTDFN)) ||
                  ((Left < Right) && ($2 == VBL_DWTDFN)))
             {
               my_vbl_error (32,NULL);
             }

             $$.LEFT   = Left;
             $$.RIGHT  = Right;
             $$.DYNAMIC = 0;
             $$.AGGREG  = 0;
           }
           else
           {
             $$.LEFT    = (long)$1.VEX;
             $$.RIGHT   = (long)$3.VEX;
             $$.DYNAMIC = $2;
             $$.AGGREG  = 0;
           }
         }
       ;

direction
       : TO
                { $$ = VBL_UPTDFN; }
        | DOWNTO
                { $$ = VBL_DWTDFN; }
       ;

.generic_clause.
        : /*empty*/
        | generic_clause
          {}
        ;

generic_clause
        : GENERIC
          LeftParen
          formal_generic_list
          RightParen_ERR
          Semicolon_ERR
          {}
        ;

.port_clause.
       : /*empty*/
       | port_clause
       ;

port_clause
       : PORT
         LeftParen
         formal_port_list
         RightParen_ERR
         Semicolon_ERR {}
       | PORT
         error
         Semicolon_ERR
                { my_vbl_error (3,NULL); }
       ;

formal_port_list
       : formal_port_element
         ...formal_port_element..
         {}
       ;

...formal_port_element..
       : /*empty*/
       | ...formal_port_element..
         Semicolon_ERR
         formal_port_element
         {}
       ;

formal_port_element
       : .SIGNAL.
         identifier_list
         Colon
         .mode.
         type_mark
         .constraint.
         .signal_kind.
         .VarAsgn__expression.
         {
            char       *signame;
            vbpor_list *VbPort;
            vbtyp_list *VbType;
            vexexpr    *VexInit;
            char        buffer[ 40 ];
            short       Signed;
            long        Left;
            long        Right;
            long        AttrLeft;
            long        AttrRight;
            struct vbl_expr expr1;

            /* ###----------------------------------------------### */
            /*    First, check the validity of the declaration.     */
            /* Then, for each port, create the apropriate set of    */
            /* structures (vbrin, vbpor, vbout, vbbus)              */
            /* ###----------------------------------------------### */

            VbType = val_type( $5.NAME );

            if ( $6.DYNAMIC ) /* LUDO_DYNAMIC */
            {
              AttrLeft  = $6.LEFT;
              AttrRight = $6.RIGHT;
              Left      = AttrLeft;
              Right     = AttrRight;
            }
            else
            if ( ( $6.LEFT  == -1 ) &&
                 ( $6.RIGHT == -1 ) )
            {
              AttrLeft  = VbType->LEFT;
              AttrRight = VbType->RIGHT;
            }
            else
            {
              AttrLeft  = $6.LEFT;
              AttrRight = $6.RIGHT;
            }

            VBL_NM1LST = reverse (VBL_NM1LST);

            if ( ! $6.DYNAMIC ) /* LUDO_DYNAMIC */
            {
              if ( $8.VEX == (vexexpr *)0 )
              {
                if ( VbType->CLASS == 'I' )
                {
                  sprintf( buffer, "%ld", AttrLeft );
                  expr1.IDENT = buffer;
                  expr1.TYPE  = -1;
                  expr1.VEX   = (vexexpr *)0;
                  expr1 = vbl_crtvex( NOPS, expr1,VBL_EMPSTR,-1,-1,0);

                  $8.VEX = expr1.VEX;
                }
              }
            }

            while (VBL_NM1LST != NULL)
            {
              VexInit = $8.VEX;

              if ( ( VexInit          != (vexexpr    *)0 ) &&
                   ( VBL_NM1LST->NEXT != (chain_list *)0 ) )
              {
                VexInit = dupvexexpr( $8.VEX );
              }

              signame = (char *)VBL_NM1LST->DATA;
              if (chktab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN) != 0)
                my_vbl_error (4,signame);

              VbPort = (vbpor_list *)vbl_addstr( 'P',$4, VbType,
                                                 $5.TYPE,$5.FLAG,signame,
                                                 AttrLeft, AttrRight,VexInit,$7, $6.DYNAMIC);

              addtab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN,$4);

              Signed = ( IsVexNodeSigned( VbPort->TARGET ) != 0 );

              if ( ! $6.DYNAMIC ) /* LUDO_DYNAMIC */
              {
                if ( IsVexAtomBit( VbPort->TARGET ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VbPort->TARGET->LEFT;
                  Right = VbPort->TARGET->RIGHT;
                }
              }

              addtab( hshtab,signame,VBL_MODNAM,VBL_TYPDFN,$5.TYPE);
              addtab( hshtab,signame,VBL_MODNAM,VBL_WMNDFN,Left  );
              addtab( hshtab,signame,VBL_MODNAM,VBL_WMXDFN,Right );
              addtab( hshtab,signame,VBL_MODNAM,VBL_ATLDFN,AttrLeft  );
              addtab( hshtab,signame,VBL_MODNAM,VBL_ATRDFN,AttrRight );
              addtab( hshtab,signame,VBL_MODNAM,VBL_LBLDFN,$7);
              addtab( hshtab,signame,VBL_MODNAM,VBL_PNTDFN,(long)VbPort->TARGET);
              addtab( hshtab,signame,VBL_MODNAM,VBL_SUNDFN,Signed);
              addtab( hshtab,signame,VBL_MODNAM,VBL_DYNDFN,$6.DYNAMIC);

              VBL_NM1LST = delchain (VBL_NM1LST, VBL_NM1LST);
            }
         }
       | error
         {
           my_vbl_error (6,NULL);
         }
       ;

/* 1.1.1.1 */
formal_generic_list
        : formal_generic_element
          ...formal_generic_element..
          {}
        | error RightParen_ERR
          {}
        ;


...formal_generic_element..
        : /*empty*/
        | ...formal_generic_element..
          Semicolon_ERR
          formal_generic_element
          {
          }
        ;

formal_generic_element
        : .CONSTANT.
          identifier_list
          Colon
          .IN.
          type_mark
          .constraint.
          .VarAsgn__expression.
          {
            char       *signame;
            char       *Value;
            vbgen_list *VbGen;
            vbmap_list *VbMap;
            vbtyp_list *VbType;
            vexexpr    *VexInit;
            char        buffer[ 40 ];
            short       Signed;
            long        Left;
            long        Right;
            long        AttrLeft;
            long        AttrRight;
            struct vbl_expr expr1;

            /* ###----------------------------------------------### */
            /*    First, check the validity of the declaration.     */
            /* Then, for each port, create the apropriate set of    */
            /* structures (vbrin, vbgen, vbout, vbbus)              */
            /* ###----------------------------------------------### */

            if ( $6.DYNAMIC )
            {
              my_vbl_error (126,NULL);
            }

            VbType = val_type( $5.NAME );

            if ( ( $6.LEFT  == -1 ) &&
                 ( $6.RIGHT == -1 ) )
            {
              AttrLeft  = VbType->LEFT;
              AttrRight = VbType->RIGHT;
            }
            else
            {
              AttrLeft  = $6.LEFT;
              AttrRight = $6.RIGHT;
            }

            VBL_NM1LST = reverse (VBL_NM1LST);

            if ( $7.VEX == (vexexpr *)0 )
            {
              if ( VbType->CLASS == 'I' )
              {
                sprintf( buffer, "%ld", AttrLeft );
                expr1.IDENT = buffer;
                expr1.TYPE  = -1;
                expr1.VEX   = (vexexpr *)0;
                expr1 = vbl_crtvex( NOPS, expr1,VBL_EMPSTR,-1,-1,0);

                $7.VEX = expr1.VEX;
              }
            }

            while (VBL_NM1LST != NULL)
            {
              VexInit = $7.VEX;

              if ( ( VexInit          != (vexexpr    *)0 ) &&
                   ( VBL_NM1LST->NEXT != (chain_list *)0 ) )
              {
                VexInit = dupvexexpr( $7.VEX );
              }

              signame = (char *)VBL_NM1LST->DATA;
              if (chktab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN) != 0)
                my_vbl_error (4,signame);

              for ( VbMap  = VBL_GENMAP;
                    VbMap != (vbmap_list *)0;
                    VbMap  = VbMap->NEXT )
              {
                Value = GetVexAtomValue( VbMap->FORMAL );

                if ( Value == signame )
                {
                  VexInit = dupvexexpr( VbMap->ACTUAL );

                  break;
                }
              }

              VbGen = (vbgen_list *)vbl_addstr( 'G',0, VbType,
                                                 $5.TYPE,$5.FLAG,signame,
                                                 AttrLeft,
                                                 AttrRight,VexInit,VBH_KIND_NONE, 0);

              addtab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN,VBL_GENDFN);

              Signed = ( IsVexNodeSigned( VbGen->TARGET ) != 0 );

              if ( IsVexAtomBit( VbGen->TARGET ) )
              {
                Left  = -1;
                Right = -1;
              }
              else
              {
                Left  = VbGen->TARGET->LEFT;
                Right = VbGen->TARGET->RIGHT;
              }

              addtab( hshtab,signame,VBL_MODNAM,VBL_TYPDFN,$5.TYPE);
              addtab( hshtab,signame,VBL_MODNAM,VBL_WMNDFN,Left  );
              addtab( hshtab,signame,VBL_MODNAM,VBL_WMXDFN,Right );
              addtab( hshtab,signame,VBL_MODNAM,VBL_ATLDFN,AttrLeft  );
              addtab( hshtab,signame,VBL_MODNAM,VBL_LBLDFN,VBH_KIND_NONE);
              addtab( hshtab,signame,VBL_MODNAM,VBL_ATRDFN,AttrRight );
              addtab( hshtab,signame,VBL_MODNAM,VBL_PNTDFN,(long)VbGen->TARGET);
              addtab( hshtab,signame,VBL_MODNAM,VBL_SUNDFN,Signed);

              VBL_NM1LST = delchain (VBL_NM1LST, VBL_NM1LST);
            }
         }
        ;


.SIGNAL.
       : /*empty*/
       | SIGNAL
         {}
       ;

.CONSTANT.
        : /*empty*/
        | CONSTANT
          {}
        ;

.IN.
        : /*empty*/
        | _IN
          {}
        ;

.mode.
        : /*empty*/
                { $$ = VBL_ICNDFN; }
        | _IN
                { $$ = VBL_ICNDFN; }
        | _OUT
                { $$ = VBL_OCNDFN; }
        | _INOUT
                { $$ = VBL_BCNDFN; }
        ;

architecture_body
       : ARCHITECTURE
         Identifier
         OF
         simple_name
         IS
          {
            if ($4 != VBL_MODNAM) my_vbl_error (1,$4);
          }
         architecture_declarative_part
         _BEGIN
         architecture_statement_part
         END_ERR
         .simple_name.
         Semicolon_ERR
          {
              if (($11 != NULL) && ($11 != $2)) my_vbl_error (7,$11);
          }
       | ARCHITECTURE
         error
                { my_vbl_error (8,NULL); }
       ;

architecture_declarative_part
       : ..block_declarative_item..
       ;

..block_declarative_item..
       : /*empty*/
       | ..block_declarative_item..
         block_declarative_item
       ;

block_declarative_item
       : constant_declaration
       | signal_declaration
       | type_declaration
       | variable_declaration
       | subtype_declaration
       | attribute_declaration
       | attribute_specification
       | component_declaration
       | subprogram_declaration
       | subprogram_body
       | error
         Semicolon_ERR
                { my_vbl_error (9,NULL); }
       ;

designator
        : Identifier
          { $$ = $1; }
        ;

.procedure_parameter_list.
        : /*empty*/
        | LeftParen
          procedure_parameter_element
          ...procedure_parameter_element..
          RightParen_ERR
          { }
        | LeftParen
          error
          RightParen_ERR
          { }
        ;


...procedure_parameter_element..
        : /*empty*/
        | ...procedure_parameter_element..
          Semicolon_ERR
          procedure_parameter_element
        ;

procedure_parameter_element
        : .procedure_parameter_object_class.
          identifier_list
          Colon
          .procedure_parameter_mode.
          type_mark
          .constraint.
          {
            char         *signame;
            char         *codedsigname;
            char          buffer[ 128 ];
            vbtyp_list   *TypeEnum;
            int           EnumSize;
            vbarg_list   *VbPar;
            short         Signed;
            long          Left;
            long          Right;
            long          AttrLeft;
            long          AttrRight;

            if ( $6.DYNAMIC )
            {
              my_vbl_error(86,0);
            }

            if ( $1 == VBH_CLASS_NONE )
            {
              if ( $4 == VBL_ICNDFN ) $1 = VBH_CLASS_CONSTANT;
              else                    $1 = VBH_CLASS_VARIABLE;
            }

            TypeEnum = val_type( $5.NAME );
            EnumSize = vbl_codeSize( TypeEnum->SIZE - 1 );

            if ( ( $6.LEFT  == -1 ) &&
                 ( $6.RIGHT == -1 ) )
            {
              AttrLeft  = TypeEnum->LEFT;
              AttrRight = TypeEnum->RIGHT;
            }
            else
            {
              AttrLeft  = $6.LEFT;
              AttrRight = $6.RIGHT;
            }

            if (chktab( hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_SYMDFN ) == 0)
            {
              VBL_NM1LST = reverse( VBL_NM1LST );
              while (VBL_NM1LST != NULL)
              {
                if ( ( TypeEnum->CLASS == 'E'          ) &&
                     ( TypeEnum->INDEX >= VBH_MAX_TYPE ) )
                {
                  signame = (char *)VBL_NM1LST->DATA;
  
                  if (chktab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN) != 0)
                        my_vbl_error (80,signame);
  
                  sprintf (buffer, "%s.cod.%s", VBL_FUNCNAM, signame);
                  codedsigname = namealloc ( buffer );
  
                  if (chktab (hshtab,codedsigname,VBL_FUNCNAM,VBL_SYMDFN) != 0)
                        my_vbl_error (80,codedsigname);
/*\
                  VbPar = (vbarg_list *)vbl_addstr( 'p',$4,val_type($5.NAME),$5.TYPE,
                          $5.FLAG,codedsigname, AttrLeft,AttrRight,NULL,$1, 0);
  
                  Signed = ( IsVexNodeSigned( VbPar->TARGET ) != 0 );
  
                  if ( IsVexAtomBit( VbPar->TARGET ) )
                  {
                    Left  = -1;
                    Right = -1;
                  }
                  else
                  {
                    Left  = VbPar->TARGET->LEFT;
                    Right = VbPar->TARGET->RIGHT;
                  }
\*/
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_SYMDFN,VBL_PARDFN);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_TYPDFN,$5.TYPE);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_WMNDFN,-1);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_WMXDFN,-1);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_LBLDFN,0);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_PNTDFN,0);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_SUNDFN,0 );
  
                  if ( EnumSize > 1 )
                  {
                    VbPar = (vbarg_list *)vbl_addstr( 'p',$4,
                            get_type(VBH_TYPE_BIT_VECTOR), VBH_TYPE_BIT_VECTOR,
                            'U',signame, 0,EnumSize-1,NULL,$1, 0);
  
                    Signed = ( IsVexNodeSigned( VbPar->TARGET ) != 0 );
  
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN,VBL_PARDFN);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_TYPDFN,VBH_TYPE_BIT_VECTOR);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMNDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMXDFN,EnumSize-1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATLDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATRDFN,EnumSize-1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_LBLDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_PNTDFN,(long)VbPar->TARGET);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_SUNDFN,Signed);
                  }
                  else
                  {
                    VbPar = (vbarg_list *)vbl_addstr( 'p',$4,
                            get_type(VBH_TYPE_BIT),VBH_TYPE_BIT,
                            'S',signame, -1,-1 ,NULL,$1, 0);
  
                    Signed = ( IsVexNodeSigned( VbPar->TARGET ) != 0 );
  
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN,VBL_PARDFN);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_TYPDFN,VBH_TYPE_BIT);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMNDFN,-1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMXDFN,-1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATLDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATRDFN,1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_LBLDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_PNTDFN,(long)VbPar->TARGET);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_SUNDFN,Signed);
                  }
                }
                else
                {
                  signame = (char *)VBL_NM1LST->DATA;
  
                  if (chktab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN) != 0)
                      my_vbl_error (80,signame);
  
                  VbPar = (vbarg_list *)vbl_addstr( 'p',$4,
                          val_type($5.NAME),$5.TYPE, $5.FLAG,signame,
                          AttrLeft,AttrRight,NULL,$1, 0);

                  Signed = ( IsVexNodeSigned( VbPar->TARGET ) != 0 );

                  if ( IsVexAtomVarWidth( VbPar->TARGET ) )
                  {
                    Left  = -2;
                    Right = -2;
                  }
                  else
                  if ( IsVexAtomBit( VbPar->TARGET ) )
                  {
                    Left  = -1;
                    Right = -1;
                  }
                  else
                  {
                    Left  = VbPar->TARGET->LEFT;
                    Right = VbPar->TARGET->RIGHT;
                  }
  
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN,VBL_PARDFN);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_TYPDFN,$5.TYPE);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMNDFN,Left);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMXDFN,Right);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATLDFN,AttrLeft);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATRDFN,AttrRight);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_LBLDFN,0);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_PNTDFN,(long)VbPar->TARGET);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_SUNDFN,Signed);
                }
  
                VBL_NM1LST = delchain (VBL_NM1LST,VBL_NM1LST);
              }
            }
            else
            {
              freechain( VBL_NM1LST );
              VBL_NM1LST = (chain_list *)0;
            }
          }
        ;

.procedure_parameter_object_class.
        : /*empty*/
                { $$ = VBH_CLASS_NONE; }
        | _VARIABLE
                { $$ = VBH_CLASS_VARIABLE; }
        | CONSTANT
                { $$ = VBH_CLASS_CONSTANT; }
        | SIGNAL 
                { $$ = VBH_CLASS_SIGNAL; }
        ;

.procedure_parameter_mode.
        : /*empty*/
                { $$ = VBL_ICNDFN; }
        | _IN
                { $$ = VBL_ICNDFN; }
        | _OUT
                { $$ = VBL_OCNDFN; }
        | _INOUT
                { $$ = VBL_BCNDFN; }
        ;

.function_parameter_list.
        : /*empty*/
        | LeftParen
          function_parameter_element
          ...function_parameter_element..
          RightParen_ERR
          { }
        | LeftParen
          error
          RightParen_ERR
          { }
        ;


...function_parameter_element..
        : /*empty*/
        | ...function_parameter_element..
          Semicolon_ERR
          function_parameter_element
        ;

function_parameter_element
        : .function_parameter_object_class.
          identifier_list
          Colon
          .function_parameter_mode.
          type_mark
          .constraint.
          {
            char         *signame;
            char         *codedsigname;
            char          buffer[ 128 ];
            vbtyp_list   *TypeEnum;
            int           EnumSize;
            vbarg_list   *VbPar;
            short         Signed;
            long          Left;
            long          Right;
            long          AttrLeft;
            long          AttrRight;

            if ( $6.DYNAMIC )
            {
              my_vbl_error(86,0);
            }

            if ( $1 == VBH_CLASS_NONE ) $1 = VBH_CLASS_CONSTANT;

            TypeEnum = val_type( $5.NAME );
            EnumSize = vbl_codeSize( TypeEnum->SIZE - 1 );

            if ( ( $6.LEFT  == -1 ) &&
                 ( $6.RIGHT == -1 ) )
            {
              AttrLeft  = TypeEnum->LEFT;
              AttrRight = TypeEnum->RIGHT;
            }
            else
            {
              AttrLeft  = $6.LEFT;
              AttrRight = $6.RIGHT;
            }

            if (chktab( hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_SYMDFN ) == 0)
            {
              VBL_NM1LST = reverse( VBL_NM1LST );
              while (VBL_NM1LST != NULL)
              {
                if ( ( TypeEnum->CLASS == 'E'          ) &&
                     ( TypeEnum->INDEX >= VBH_MAX_TYPE ) )
                {
                  signame = (char *)VBL_NM1LST->DATA;
  
                  if (chktab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN) != 0)
                        my_vbl_error (80,signame);
  
                  sprintf (buffer, "%s.cod.%s", VBL_FUNCNAM, signame);
                  codedsigname = namealloc ( buffer );
  
                  if (chktab (hshtab,codedsigname,VBL_FUNCNAM,VBL_SYMDFN) != 0)
                        my_vbl_error (80,codedsigname);
/*\
                  VbPar = (vbarg_list *)vbl_addstr('p',$4,
                          val_type($5.NAME),$5.TYPE, 
                          $5.FLAG,codedsigname, AttrLeft,AttrRight, NULL,$1, 0);
  
                  Signed = ( IsVexNodeSigned( VbPar->TARGET ) != 0 );
  
                  if ( IsVexAtomBit( VbPar->TARGET ) )
                  {
                    Left  = -1;
                    Right = -1;
                  }
                  else
                  {
                    Left  = VbPar->TARGET->LEFT;
                    Right = VbPar->TARGET->RIGHT;
                  }
\*/
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_SYMDFN,VBL_PARDFN);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_TYPDFN,$5.TYPE);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_WMNDFN,-1);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_WMXDFN,-1);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_LBLDFN,0);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_PNTDFN,0);
                  addtab(hshtab,codedsigname,VBL_FUNCNAM,VBL_SUNDFN,0);
  
                  if ( EnumSize > 1 )
                  {
                    VbPar = (vbarg_list *)vbl_addstr( 'p',$4,
                            get_type(VBH_TYPE_BIT_VECTOR), VBH_TYPE_BIT_VECTOR,
                            'U',signame, 0,EnumSize-1,NULL,$1, 0);
  
                    Signed = ( IsVexNodeSigned( VbPar->TARGET ) != 0 );
  
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN,VBL_PARDFN);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_TYPDFN,VBH_TYPE_BIT_VECTOR);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMNDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMXDFN,EnumSize-1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATLDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATRDFN,EnumSize-1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_LBLDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_PNTDFN,(long)VbPar->TARGET);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_SUNDFN,Signed);
                  }
                  else
                  {
                    VbPar = (vbarg_list *)vbl_addstr( 'p',$4,
                            get_type(VBH_TYPE_BIT),VBH_TYPE_BIT, 
                            'S',signame, -1,-1 ,NULL,$1, 0);
  
                    Signed = ( IsVexNodeSigned( VbPar->TARGET ) != 0 );
  
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN,VBL_PARDFN);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_TYPDFN,VBH_TYPE_BIT);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMNDFN,-1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMXDFN,-1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATLDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATRDFN,1);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_LBLDFN,0);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_PNTDFN,(long)VbPar->TARGET);
                    addtab (hshtab,signame,VBL_FUNCNAM,VBL_SUNDFN,Signed);
                  }
                }
                else
                {
                  signame = (char *)VBL_NM1LST->DATA;
                  if (chktab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN) != 0)
                      my_vbl_error (80,signame);
  
                  VbPar = (vbarg_list *)vbl_addstr ('p',$4,
                          val_type($5.NAME),$5.TYPE, $5.FLAG,
                          signame, AttrLeft,AttrRight,NULL,$1, 0);
  
                  Signed = ( IsVexNodeSigned( VbPar->TARGET ) != 0 );
  
                  if ( IsVexAtomBit( VbPar->TARGET ) )
                  {
                    Left  = -1;
                    Right = -1;
                  }
                  else
                  {
                    Left  = VbPar->TARGET->LEFT;
                    Right = VbPar->TARGET->RIGHT;
                  }
  
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_SYMDFN,VBL_PARDFN);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_TYPDFN,$5.TYPE);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMNDFN,Left);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_WMXDFN,Right);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATLDFN,AttrLeft);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_ATRDFN,AttrRight);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_LBLDFN,0);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_PNTDFN,(long)VbPar->TARGET);
                  addtab (hshtab,signame,VBL_FUNCNAM,VBL_SUNDFN,Signed);
                }
  
                VBL_NM1LST = delchain (VBL_NM1LST,VBL_NM1LST);
              }
            }
            else
            {
              freechain( VBL_NM1LST );
              VBL_NM1LST = (chain_list *)0;
            }
          }
        ;

.function_parameter_object_class.
        : /*empty*/
                { $$ = VBH_CLASS_NONE; }
        | CONSTANT
                { $$ = VBH_CLASS_CONSTANT; }
        | SIGNAL
                { $$ = VBH_CLASS_SIGNAL; }
        ;

.function_parameter_mode.
        : /*empty*/
                { $$ = VBL_ICNDFN; }
        | _IN
                { $$ = VBL_ICNDFN; }
        ;

subprogram_declarative_part
        : /*empty*/
        | 
          subprogram_declarative_part
          subprogram_declarative_item
        ;

subprogram_declarative_item
        : constant_declaration
        | variable_declaration
        | type_declaration
        | subtype_declaration
        | attribute_declaration
        | attribute_specification
        ;

subprogram_declaration
        : subprogram_specification
          {
            VBL_FUNCNAM = (char *)0;
          }
          Semicolon_ERR
        ;

subprogram_specification
        : PROCEDURE
          designator
          {
            int Mode;

            VBL_FUNCNAM = $2;

            Mode = chktab(hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_SYMDFN);

            if ( Mode == 0 )
            {
              VBL_BEFPNT->BEFUN = vbh_addvbfun( VBL_BEFPNT->BEFUN,
                                                VBL_FUNCNAM, NULL, NULL, NULL, VBL_LINNUM );
              VBL_BEFUN = VBL_BEFPNT->BEFUN;
            }
            else
            if ( Mode != VBL_PRODFN )
            {
              my_vbl_error (117,VBL_FUNCNAM);
            }
            else
            {
              VBL_BEFUN = (vbfun_list *)chktab( hshtab, VBL_FUNCNAM, VBL_MODNAM, VBL_PNTDFN );
            }
          }
          .procedure_parameter_list.
          {
            if ( chktab(hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_SYMDFN) == 0 )
            {
              VBL_BEFUN->ARGUMENT = 
                 (vbarg_list *)reverse( (chain_list *)VBL_BEFUN->ARGUMENT );

              addtab (hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_SYMDFN,VBL_PRODFN);
              addtab (hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_PNTDFN,(long)VBL_BEFUN);
            }
          }
        | FUNCTION
          designator
          {
            int Mode;

            VBL_FUNCNAM = $2;

            Mode = chktab(hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_SYMDFN);

            if ( Mode == 0 )
            {
              VBL_BEFPNT->BEFUN = vbh_addvbfun( VBL_BEFPNT->BEFUN,
                                                VBL_FUNCNAM, NULL, NULL, NULL, VBL_LINNUM );
              VBL_BEFUN = VBL_BEFPNT->BEFUN;
            }
            else
            if ( Mode != VBL_FUNDFN )
            {
              my_vbl_error (116,VBL_FUNCNAM);
            }
            else
            {
              VBL_BEFUN = (vbfun_list *)chktab(hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_PNTDFN);
            }
          }
          .function_parameter_list.
          _RETURN
          type_mark
          {
            char         *signame;
            char          buffer[ 128 ];
            char         *codedsigname;
            vbtyp_list   *TypeEnum;
            short         Signed;
            long          Left;
            long          Right;
            long          AttrLeft;
            long          AttrRight;
            int           EnumSize;
            vbtyp_list   *VbType;
            vexexpr      *VexRet;

            if ( $6.DYNAMIC )
            {
              my_vbl_error(115,0);
            }

            signame = VBL_FUNCNAM;

            TypeEnum = val_type( $6.NAME );
            EnumSize = vbl_codeSize( TypeEnum->SIZE - 1 );

            AttrLeft  = TypeEnum->LEFT;
            AttrRight = TypeEnum->RIGHT;

            if ( chktab(hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_SYMDFN) == 0 )
            {
              if ( ( TypeEnum->CLASS == 'E'          ) &&
                   ( TypeEnum->INDEX >= VBH_MAX_TYPE ) )
              {
                sprintf (buffer, "%s.cod.%s", VBL_MODNAM, signame);
                codedsigname = namealloc ( buffer );
  
                if (chktab (hshtab,codedsigname,VBL_MODNAM,VBL_SYMDFN) != 0)
                    my_vbl_error (116,codedsigname);
/*\
                VexRet = (vexexpr *)vbl_addstr( 'F',0,val_type($6.NAME),$6.TYPE,
                         $6.FLAG,codedsigname,AttrLeft,AttrRight,NULL,0, 0);
  
                Signed = ( IsVexNodeSigned( VexRet ) != 0 );
  
                if ( IsVexAtomBit( VexRet ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VexRet->LEFT;
                  Right = VexRet->RIGHT;
                }
\*/
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_SYMDFN,VBL_FUNDFN);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_TYPDFN,$6.TYPE);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_WMNDFN,-1);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_WMXDFN,-1);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_SUNDFN,0);
  
                if ( EnumSize > 1 )
                {
                  VbType = get_type( VBH_TYPE_BIT_VECTOR );
                  VexRet = (vexexpr *)vbl_addstr('F',0,
                           VbType,VBH_TYPE_BIT_VECTOR,'U',signame, 0,EnumSize-1,NULL,0, 0);
  
                  Signed = ( IsVexNodeSigned( VexRet ) != 0 );
  
                  addtab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN,VBL_FUNDFN);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_TYPDFN,VBH_TYPE_BIT_VECTOR);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_WMNDFN,0);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_WMXDFN,EnumSize-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_ATLDFN,0);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_ATRDFN,TypeEnum->SIZE-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_SUNDFN,Signed);
                }
                else
                {
                  VbType = get_type(VBH_TYPE_BIT);
                  VexRet = (vexexpr *)vbl_addstr ('F',0,
                           VbType,VBH_TYPE_BIT,'S',signame, -1,-1 ,NULL,0, 0);
  
                  Signed = ( IsVexNodeSigned( VexRet ) != 0 );
  
                  addtab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN,VBL_FUNDFN);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_TYPDFN,VBH_TYPE_BIT);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_WMNDFN,-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_WMXDFN,-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_ATLDFN,0);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_ATRDFN,TypeEnum->SIZE-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_SUNDFN,Signed);
                }
              }
              else
              {
                VbType = val_type($6.NAME);
                VexRet = (vexexpr *)vbl_addstr( 'F',0,VbType,$6.TYPE,
                                                $6.FLAG,signame,-1,-1,NULL,0, 0);
  
                Signed = ( IsVexNodeSigned( VexRet ) != 0 );
  
                if ( IsVexAtomBit( VexRet ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VexRet->LEFT;
                  Right = VexRet->RIGHT;
                }
  
                addtab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN,VBL_FUNDFN);
                addtab (hshtab,signame,VBL_MODNAM,VBL_TYPDFN,$6.TYPE);
                addtab (hshtab,signame,VBL_MODNAM,VBL_WMNDFN,Left);
                addtab (hshtab,signame,VBL_MODNAM,VBL_WMXDFN,Right);
                addtab (hshtab,signame,VBL_MODNAM,VBL_ATLDFN,AttrLeft);
                addtab (hshtab,signame,VBL_MODNAM,VBL_ATRDFN,AttrRight);
                addtab (hshtab,signame,VBL_MODNAM,VBL_SUNDFN,Signed);
              }

              VBL_BEFUN->RETURN = vbh_addvbarg( (vbarg_list *)0, VexRet, 'O', 
                                                VBH_CLASS_NONE, VbType, VBL_LINNUM );
              VBL_BEFUN->ARGUMENT = 
                 (vbarg_list *)reverse( (chain_list *)VBL_BEFUN->ARGUMENT );

              addtab (hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_SYMDFN,VBL_FUNDFN);
              addtab (hshtab,VBL_FUNCNAM,VBL_MODNAM,VBL_PNTDFN,(long)VBL_BEFUN);
            }
          }
        ;

subprogram_body
        : subprogram_specification
          IS
          subprogram_declarative_part
          _BEGIN
          {
            void *pnt;
            pnt = &(VBL_BEFUN->INSTRUCTION);
            VBL_INSLST = addchain(NULL,pnt);
          }
          sequence_of_statements
          {
            struct ptype **pnt;
            struct ptype *ptype;
            pnt = (struct ptype**)VBL_INSLST->DATA;
            ptype = reversetype(*pnt);
            *pnt = ptype;
            VBL_INSLST = delchain (VBL_INSLST, VBL_INSLST);
          }
          END_ERR
          .designator.
          {
            VBL_FUNCNAM = (char *)0;
          }
          Semicolon_ERR
        ;

.designator.
        : /*empty*/
        | designator
          { }
        ;

constant_declaration
       : CONSTANT
         Identifier
         Colon
         type_mark
         .constraint.
         constant_VarAsgn__expression
         Semicolon_ERR
        {
          vbtyp_list    *Type;
          vbcst_list    *VbCst;
          vexexpr       *VexValue;
          chain_list    *HeadChain;
          chain_list    *ScanChain;
          vbl_vexstr    *VexStr;
          short          Signed;
          long           Left;
          long           Right;
          short          Width;
          long           AttrLeft;
          long           AttrRight;

          if ( $6.AGGREG )
          {
            HeadChain = reverse( (chain_list *)$6.IDENT );
            ScanChain = HeadChain;

            if ( HeadChain->NEXT != (chain_list *)0 )
            {
              Width    = 0;
              VexValue = createvexoper( VEX_ARRAY, 1 );

              while ( ScanChain != (chain_list *)0 )
              {
                VexStr = (vbl_vexstr *)ScanChain->DATA;
                Width += VexStr->VEX->WIDTH;
                addvexhexpr( VexValue, VexStr->VEX );

                autfreeheap( VexStr, sizeof( vbl_vexstr ) );

                ScanChain = ScanChain->NEXT;
              }

              VexValue->WIDTH = Width;
            }
            else
            {
              VexStr   = (vbl_vexstr *)HeadChain->DATA;
              VexValue = VexStr->VEX;

              autfreeheap( VexStr, sizeof( vbl_vexstr ) );
            }

            freechain( HeadChain );
          }
          else
          {
            VexValue = simpvexexpr( $6.VEX );
          }

          Type = val_type( $4.NAME );


          if ( $5.DYNAMIC )
          {
            AttrLeft  = $5.LEFT;
            AttrRight = $5.RIGHT;
            Left  = AttrLeft;
            Right = AttrRight;
          }
          else
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

          VbCst  = (vbcst_list *)vbl_addstr( 'C',0,val_type($4.NAME),
                 $4.TYPE,$4.FLAG,$2,AttrLeft,AttrRight,VexValue,0, $5.DYNAMIC);

          Signed = ( IsVexNodeSigned( VbCst->TARGET ) != 0 );

          if ( ! $5.DYNAMIC )
          {
            if ( IsVexAtomBit( VbCst->TARGET ) )
            {
              Left  = -1;
              Right = -1;
            }
            else
            {
              Left  = VbCst->TARGET->LEFT;
              Right = VbCst->TARGET->RIGHT;
            }
          }

          addtab(hshtab,$2,VBL_MODNAM,VBL_SYMDFN,VBL_CSTDFN);
          addtab(hshtab,$2,VBL_MODNAM,VBL_TYPDFN,$4.TYPE);
          addtab(hshtab,$2,VBL_MODNAM,VBL_WMNDFN,Left);
          addtab(hshtab,$2,VBL_MODNAM,VBL_WMXDFN,Right);
          addtab(hshtab,$2,VBL_MODNAM,VBL_ATLDFN,AttrLeft);
          addtab(hshtab,$2,VBL_MODNAM,VBL_ATRDFN,AttrRight);
          addtab(hshtab,$2,VBL_MODNAM,VBL_LBLDFN,0);
          addtab(hshtab,$2,VBL_MODNAM,VBL_PNTDFN,(long)VbCst->VEX);
          addtab(hshtab,$2,VBL_MODNAM,VBL_SUNDFN,Signed );
          addtab(hshtab,$2,VBL_MODNAM,VBL_DYNDFN,$5.DYNAMIC );
        }
       ;

constant_VarAsgn__expression
       : VarAsgn
         expression
                { $$ = $2; }
       ;

.VarAsgn__expression.
        : /*empty*/
        {  $$.VEX = NULL ;}
        | VarAsgn
          expression
        {  $$ = $2 ;}
        ;

variable_declaration
        : _VARIABLE
          identifier_list
          Colon
          type_mark
          .constraint.
          .VarAsgn__expression.
          Semicolon_ERR
          {
            char         *LocalName;
            char         *signame;
            char         *codedsigname;
            char          buffer[ 128 ];
            vbtyp_list   *TypeEnum;
            int           EnumSize;
            vbvar_list   *VbVar;
            short         Signed;
            long          Left;
            long          Right;
            long          AttrLeft;
            long          AttrRight;
            char          StrFlag;
            vexexpr      *VexInit;
            struct vbl_expr expr1;

            if ( VBL_FUNCNAM != (char *)0 )
            {
              LocalName = VBL_FUNCNAM;
              StrFlag   = 'v';
            }
            else
            {
              LocalName = VBL_PROCNAM;
              StrFlag   = 'V';
            }

            TypeEnum = val_type( $4.NAME );
            EnumSize = vbl_codeSize( TypeEnum->SIZE - 1 );

            if ( $5.DYNAMIC ) /* LUDO_DYNAMIC */
            {
              AttrLeft  = $5.LEFT;
              AttrRight = $5.RIGHT;
              Left  = AttrLeft;
              Right = AttrRight;
            }
            else
            if ( ( $5.LEFT  == -1 ) &&
                 ( $5.RIGHT == -1 ) )
            {
              AttrLeft  = TypeEnum->LEFT;
              AttrRight = TypeEnum->RIGHT;
            }
            else
            {
              AttrLeft  = $5.LEFT;
              AttrRight = $5.RIGHT;
            }

            VBL_NM1LST = reverse (VBL_NM1LST);

            if ( ! $5.DYNAMIC ) /* LUDO_DYNAMIC */
            {
              if ( $6.VEX == (vexexpr *)0 )
              {
                if ( TypeEnum->CLASS == 'I' )
                {
                  sprintf( buffer, "%ld", AttrLeft );
                  expr1.IDENT = buffer;
                  expr1.TYPE  = -1;
                  expr1.VEX   = (vexexpr *)0;
                  expr1 = vbl_crtvex( NOPS, expr1,VBL_EMPSTR,-1,-1,0);

                  $6.VEX = expr1.VEX;
                }
              }
            }

            while (VBL_NM1LST != NULL)
            {
              VexInit = $6.VEX;

              if ( ( VexInit          != (vexexpr    *)0 ) &&
                   ( VBL_NM1LST->NEXT != (chain_list *)0 ) )
              {
                VexInit = dupvexexpr( $6.VEX );
              }

              if ( ( TypeEnum->CLASS == 'E'          ) &&
                   ( TypeEnum->INDEX >= VBH_MAX_TYPE ) )
              {
                signame = (char *)VBL_NM1LST->DATA;

                if (chktab (hshtab,signame,LocalName,VBL_SYMDFN) != 0)
                      my_vbl_error (80,signame);

                sprintf (buffer, "%s.cod.%s", LocalName, signame);
                codedsigname = namealloc ( buffer );

                if (chktab (hshtab,codedsigname,LocalName,VBL_SYMDFN) != 0)
                      my_vbl_error (80,codedsigname);
/*\
                VbVar = (vbvar_list *)vbl_addstr(StrFlag,0,val_type($4.NAME),$4.TYPE,
                        $4.FLAG,codedsigname, AttrLeft,AttrRight, VexInit, 0 , 0);

                Signed = ( IsVexNodeSigned( VbVar->TARGET ) != 0 );

                if ( IsVexAtomBit( VbVar->TARGET ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VbVar->TARGET->LEFT;
                  Right = VbVar->TARGET->RIGHT;
                }
\*/
                addtab(hshtab,codedsigname,LocalName,VBL_SYMDFN,VBL_VARDFN);
                addtab(hshtab,codedsigname,LocalName,VBL_TYPDFN,$4.TYPE);
                addtab(hshtab,codedsigname,LocalName,VBL_WMNDFN,-1);
                addtab(hshtab,codedsigname,LocalName,VBL_WMXDFN,-1);
                addtab(hshtab,codedsigname,LocalName,VBL_LBLDFN,0);
                addtab(hshtab,codedsigname,LocalName,VBL_PNTDFN,0);
                addtab(hshtab,codedsigname,LocalName,VBL_SUNDFN,0);
                addtab(hshtab,codedsigname,LocalName,VBL_DYNDFN,0);

                if ( VexInit != (vexexpr *)0 ) VexInit = dupvexexpr( VexInit );

                if ( EnumSize > 1 )
                {
                  VbVar = (vbvar_list *)vbl_addstr( StrFlag,0,
                          get_type(VBH_TYPE_BIT_VECTOR),VBH_TYPE_BIT_VECTOR,
                          'U',signame, 0,EnumSize-1 , VexInit, 0, 0 );

                  Signed = ( IsVexNodeSigned( VbVar->TARGET ) != 0 );

                  addtab (hshtab,signame,LocalName,VBL_SYMDFN,VBL_VARDFN);
                  addtab (hshtab,signame,LocalName,VBL_TYPDFN,VBH_TYPE_BIT_VECTOR);
                  addtab (hshtab,signame,LocalName,VBL_WMNDFN,0);
                  addtab (hshtab,signame,LocalName,VBL_WMXDFN,EnumSize-1);
                  addtab (hshtab,signame,LocalName,VBL_ATLDFN,0);
                  addtab (hshtab,signame,LocalName,VBL_ATRDFN,EnumSize-1);
                  addtab (hshtab,signame,LocalName,VBL_LBLDFN,0);
                  addtab (hshtab,signame,LocalName,VBL_PNTDFN,(long)VbVar->TARGET);
                  addtab (hshtab,signame,LocalName,VBL_SUNDFN,Signed);
                  addtab (hshtab,signame,LocalName,VBL_DYNDFN,0);
                }
                else
                {
                  VbVar = (vbvar_list *)vbl_addstr( StrFlag,0,
                          get_type(VBH_TYPE_BIT),VBH_TYPE_BIT,
                          'S',signame, -1,-1 , VexInit, 0, 0 );

                  Signed = ( IsVexNodeSigned( VbVar->TARGET ) != 0 );

                  addtab (hshtab,signame,LocalName,VBL_SYMDFN,VBL_VARDFN);
                  addtab (hshtab,signame,LocalName,VBL_TYPDFN,VBH_TYPE_BIT);
                  addtab (hshtab,signame,LocalName,VBL_WMNDFN,-1);
                  addtab (hshtab,signame,LocalName,VBL_WMXDFN,-1);
                  addtab (hshtab,signame,LocalName,VBL_ATLDFN,0);
                  addtab (hshtab,signame,LocalName,VBL_ATRDFN,1);
                  addtab (hshtab,signame,LocalName,VBL_LBLDFN,0);
                  addtab (hshtab,signame,LocalName,VBL_PNTDFN,(long)VbVar->TARGET);
                  addtab (hshtab,signame,LocalName,VBL_SUNDFN,Signed);
                  addtab (hshtab,signame,LocalName,VBL_DYNDFN,0);
                }
              }
              else
              {
                signame = (char *)VBL_NM1LST->DATA;
                if (chktab (hshtab,signame,LocalName,VBL_SYMDFN) != 0)
                    my_vbl_error (80,signame);

                VbVar = (vbvar_list *)vbl_addstr( StrFlag,0,
                        val_type($4.NAME),$4.TYPE,
                        $4.FLAG,signame, AttrLeft, AttrRight, VexInit, 0, $5.DYNAMIC);

                Signed = ( IsVexNodeSigned( VbVar->TARGET ) != 0 );

              if ( ( $4.FLAG == 'I' ) && ( $5.DYNAMIC ) )
              {
                $5.DYNAMIC = VBL_DWTDFN;
              }


              if ( ! $5.DYNAMIC ) /* LUDO_DYNAMIC */
              {
                if ( IsVexAtomBit( VbVar->TARGET ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VbVar->TARGET->LEFT;
                  Right = VbVar->TARGET->RIGHT;
                }
              }

                addtab (hshtab,signame,LocalName,VBL_SYMDFN,VBL_VARDFN);
                addtab (hshtab,signame,LocalName,VBL_TYPDFN,$4.TYPE);
                addtab (hshtab,signame,LocalName,VBL_WMNDFN,Left);
                addtab (hshtab,signame,LocalName,VBL_WMXDFN,Right);
                addtab (hshtab,signame,LocalName,VBL_ATLDFN,AttrLeft);
                addtab (hshtab,signame,LocalName,VBL_ATRDFN,AttrRight);
                addtab (hshtab,signame,LocalName,VBL_LBLDFN,0);
                addtab (hshtab,signame,LocalName,VBL_PNTDFN,(long)VbVar->TARGET);
                addtab (hshtab,signame,LocalName,VBL_SUNDFN,Signed);
                addtab (hshtab,signame,LocalName,VBL_DYNDFN,$5.DYNAMIC);
              }

              VBL_NM1LST = delchain (VBL_NM1LST,VBL_NM1LST);
            }
          }
        ;

signal_declaration
       : SIGNAL
         identifier_list
         Colon
         type_mark
         .constraint.
         .signal_kind.
         .VarAsgn__expression.
         Semicolon_ERR
          {
            char         *signame;
            char          buffer[ 128 ];
            char         *codedsigname;
            vbtyp_list   *TypeEnum;
            short         Signed;
            long          Left;
            long          Right;
            long          AttrLeft;
            long          AttrRight;
            int           EnumSize;
            vbaux_list   *VbAux;
            vexexpr      *VexInit;
            struct vbl_expr expr1;

            TypeEnum = val_type( $4.NAME );
            EnumSize = vbl_codeSize( TypeEnum->SIZE - 1 );

            if ( (   $4.DYNAMIC ) &&
                 ( ! $5.DYNAMIC ) )
            {
              $5.DYNAMIC = $4.DYNAMIC;
              $5.LEFT    = $4.LEFT;
              $5.RIGHT   = $4.RIGHT;
            }

            if ( $5.DYNAMIC ) /* LUDO_DYNAMIC */
            {
              AttrLeft  = $5.LEFT;
              AttrRight = $5.RIGHT;
              Left = AttrLeft;
              Right = AttrRight;
            }
            else
            if ( ( $5.LEFT  == -1 ) &&
                 ( $5.RIGHT == -1 ) )
            {
              AttrLeft  = TypeEnum->LEFT;
              AttrRight = TypeEnum->RIGHT;
            }
            else
            {
              AttrLeft  = $5.LEFT;
              AttrRight = $5.RIGHT;
            }

            VBL_NM1LST = reverse (VBL_NM1LST);

            if ( ! $5.DYNAMIC ) /* LUDO_DYNAMIC */
            {
              if ( $7.VEX == (vexexpr *)0 )
              {
                if ( TypeEnum->CLASS == 'I' )
                {
                  sprintf( buffer, "%ld", AttrLeft );
                  expr1.IDENT = buffer;
                  expr1.TYPE  = -1;
                  expr1.VEX   = (vexexpr *)0;
                  expr1 = vbl_crtvex( NOPS, expr1,VBL_EMPSTR,-1,-1,0);

                  $7.VEX = expr1.VEX;
                }
              }
            }

            while (VBL_NM1LST != NULL)
            {
              VexInit = $7.VEX;

              if ( ( VexInit          != (vexexpr    *)0 ) &&
                   ( VBL_NM1LST->NEXT != (chain_list *)0 ) )
              {
                VexInit = dupvexexpr( $7.VEX );
              }

              if ( ( TypeEnum->CLASS == 'E'          ) &&
                   ( TypeEnum->INDEX >= VBH_MAX_TYPE ) )
              {
                signame = (char *)VBL_NM1LST->DATA;

                if (chktab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN) != 0)
                    my_vbl_error (10,signame);

                sprintf (buffer, "%s.cod.%s", VBL_MODNAM, signame);
                codedsigname = namealloc ( buffer );

                if (chktab (hshtab,codedsigname,VBL_MODNAM,VBL_SYMDFN) != 0)
                    my_vbl_error (10,codedsigname);
/*\
                VbAux = (vbaux_list *)vbl_addstr( 'S',0,
                        val_type($4.NAME),$4.TYPE,
                        $4.FLAG,codedsigname, AttrLeft, AttrRight,VexInit, $6, 0);

                Signed = ( IsVexNodeSigned( VbAux->TARGET ) != 0 );

                if ( IsVexAtomBit( VbAux->TARGET ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VbAux->TARGET->LEFT;
                  Right = VbAux->TARGET->RIGHT;
                }
\*/
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_SYMDFN,VBL_SGNDFN);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_TYPDFN,$4.TYPE);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_WMNDFN,-1);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_WMXDFN,-1);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_LBLDFN,$6);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_PNTDFN,0);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_PNTDFN,0);
                addtab (hshtab,codedsigname,VBL_MODNAM,VBL_DYNDFN,0);

                if ( VexInit != (vexexpr *)0 ) VexInit = dupvexexpr( VexInit );

                if ( EnumSize > 1 )
                {
                  VbAux = (vbaux_list *)vbl_addstr ('S',0,
                          get_type(VBH_TYPE_BIT_VECTOR), VBH_TYPE_BIT_VECTOR,
                          'U',signame, 0,EnumSize-1 , VexInit, $6, 0);

                  Signed = ( IsVexNodeSigned( VbAux->TARGET ) != 0 );

                  addtab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN,VBL_SGNDFN);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_TYPDFN,VBH_TYPE_BIT_VECTOR);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_WMNDFN,0);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_WMXDFN,EnumSize-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_ATLDFN,0);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_ATRDFN,EnumSize-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_LBLDFN,$6);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_PNTDFN,(long)VbAux->TARGET);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_SUNDFN,Signed);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_DYNDFN,0);
                }
                else
                {
                  VbAux = (vbaux_list *)vbl_addstr ('S',0,
                          get_type(VBH_TYPE_BIT),VBH_TYPE_BIT,
                          'S',signame, -1,-1 , VexInit, $6, 0);

                  Signed = ( IsVexNodeSigned( VbAux->TARGET ) != 0 );

                  addtab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN,VBL_SGNDFN);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_TYPDFN,VBH_TYPE_BIT);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_WMNDFN,-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_WMXDFN,-1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_ATLDFN,0);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_ATRDFN,1);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_LBLDFN,$6);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_PNTDFN,(long)VbAux->TARGET);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_SUNDFN,Signed);
                  addtab (hshtab,signame,VBL_MODNAM,VBL_DYNDFN,0);
                }
              }
              else
              {
                signame = (char *)VBL_NM1LST->DATA;
                if (chktab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN) != 0)
                   my_vbl_error (10,signame);

                VbAux = (vbaux_list *)vbl_addstr( 'S',0,
                        val_type($4.NAME),$4.TYPE,
                        $4.FLAG,signame, AttrLeft,AttrRight,VexInit, $6,
                        $5.DYNAMIC);

                if ( ( $4.FLAG == 'I' ) && ( $5.DYNAMIC ) )
                {
                  $5.DYNAMIC = VBL_DWTDFN;
                }

                Signed = ( IsVexNodeSigned( VbAux->TARGET ) != 0 );

              if ( ! $5.DYNAMIC ) /* LUDO_DYNAMIC */
              {
                if ( IsVexAtomBit( VbAux->TARGET ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VbAux->TARGET->LEFT;
                  Right = VbAux->TARGET->RIGHT;
                }
              }

                addtab (hshtab,signame,VBL_MODNAM,VBL_SYMDFN,VBL_SGNDFN);
                addtab (hshtab,signame,VBL_MODNAM,VBL_TYPDFN,$4.TYPE);
                addtab (hshtab,signame,VBL_MODNAM,VBL_WMNDFN,Left);
                addtab (hshtab,signame,VBL_MODNAM,VBL_WMXDFN,Right);
                addtab (hshtab,signame,VBL_MODNAM,VBL_ATLDFN,AttrLeft);
                addtab (hshtab,signame,VBL_MODNAM,VBL_ATRDFN,AttrRight);
                addtab (hshtab,signame,VBL_MODNAM,VBL_LBLDFN,$6);
                addtab (hshtab,signame,VBL_MODNAM,VBL_PNTDFN,(long)VbAux->TARGET);
                addtab (hshtab,signame,VBL_MODNAM,VBL_SUNDFN,Signed);
                addtab (hshtab,signame,VBL_MODNAM,VBL_DYNDFN,$5.DYNAMIC);
              }

              VBL_NM1LST = delchain (VBL_NM1LST,VBL_NM1LST);
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
           vbtyp_list *VbType;
              VbType = vbh_addvbtyp(VBL_BEFPNT,$2,$4.LEFT,
               $4.RIGHT,$4.VALUE,$4.SIZE,$4.BYTE,$4.BASE,VBL_NUMTYP,$4.RESOLV,$4.CLASS,
               VBL_LINNUM);
              if ( $4.DYNAMIC )
              {
                VbType->DYNAMIC       = $4.DYNAMIC;
                VbType->DYNAMIC_LEFT  = $4.DYNAMIC_LEFT;
                VbType->DYNAMIC_RIGHT = $4.DYNAMIC_RIGHT;
              }
              addtab(hshtab,$2,VBL_MODNAM,VBL_SYMDFN,VBL_TPEDFN);
              addtab(hshtab,$2,VBL_MODNAM,VBL_LBLDFN,$4.CLASS);
              addtab(hshtab,$2,VBL_MODNAM,VBL_TYPDFN,VBL_NUMTYP);
              addtab(hshtab,$2,VBL_MODNAM,VBL_WMNDFN,$4.LEFT);
              addtab(hshtab,$2,VBL_MODNAM,VBL_WMXDFN,$4.RIGHT);
              addtab(hshtab,$2,VBL_MODNAM,VBL_DYNDFN,$4.DYNAMIC);
              addtab(hshtab,$2,VBL_MODNAM,VBL_PNTDFN,(long)VBL_BEFPNT->BETYP);
              VBL_NUMTYP++;
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
           vbtyp_list *VbType;
            VbType = vbh_addvbtyp(VBL_BEFPNT,$2,$4.LEFT,
                   $4.RIGHT,$4.VALUE,$4.SIZE,$4.BYTE,$4.BASE,$4.INDEX,$4.RESOLV,$4.CLASS,
                   VBL_LINNUM);
              if ( $4.DYNAMIC )
              {
                VbType->DYNAMIC       = $4.DYNAMIC;
                VbType->DYNAMIC_LEFT  = $4.DYNAMIC_LEFT;
                VbType->DYNAMIC_RIGHT = $4.DYNAMIC_RIGHT;
              }
            addtab(hshtab,$2,VBL_MODNAM,VBL_SYMDFN,VBL_TPEDFN);
            addtab(hshtab,$2,VBL_MODNAM,VBL_LBLDFN,$4.CLASS);
            addtab(hshtab,$2,VBL_MODNAM,VBL_TYPDFN,$4.INDEX);
            addtab(hshtab,$2,VBL_MODNAM,VBL_WMNDFN,$4.LEFT);
            addtab(hshtab,$2,VBL_MODNAM,VBL_WMXDFN,$4.RIGHT);
            addtab(hshtab,$2,VBL_MODNAM,VBL_DYNDFN,$4.DYNAMIC);
            addtab(hshtab,$2,VBL_MODNAM,VBL_PNTDFN,(long)VBL_BEFPNT->BETYP);
              VBL_NUMTYP++;
          }
          ;

subtype_indication
       : type_or_function_name
         type_mark
         .constraint.
         {
           struct vbtyp *vbtyp_pnt;
           struct vbfun *vbfun_pnt;
           int           mode;
  
           mode = chktab(hshtab,$1,VBL_MODNAM,VBL_SYMDFN);

           vbfun_pnt = (vbfun_list *)chktab(hshtab,$1,VBL_MODNAM,VBL_PNTDFN);
           vbtyp_pnt = (vbtyp_list *)chktab(hshtab,$2.NAME,VBL_MODNAM,VBL_PNTDFN);
  
           if ( ( mode != VBL_FUNDFN ) || ( $3.DYNAMIC ) )
           {
             my_vbl_error(87,0);
           }
  
           $$.NAME = NULL;
           $$.BASE = vbtyp_pnt;
           $$.VALUE = NULL ;
           $$.SIZE = 0 ;
           $$.INDEX = VBL_NUMTYP;
           $$.BYTE = vbtyp_pnt->BYTE;
           $$.LEFT  = $3.LEFT ;
           $$.RIGHT = $3.RIGHT ;
           $$.CLASS = $3.FLAG ;
           $$.RESOLV = vbfun_pnt;
           $$.DYNAMIC = 0;
           $$.DYNAMIC_LEFT  = (vexexpr *)0;
           $$.DYNAMIC_RIGHT = (vexexpr *)0;
         }
       | type_mark
         .constraint.
         {
           struct vbtyp *vbtyp_pnt;
  
           vbtyp_pnt = (vbtyp_list *)chktab(hshtab,$1.NAME,VBL_MODNAM,VBL_PNTDFN);
  
           $$.NAME = NULL;
           $$.BASE = vbtyp_pnt;
           $$.VALUE = NULL ;
           $$.SIZE = 0 ;
           $$.INDEX = VBL_NUMTYP;
           $$.BYTE = vbtyp_pnt->BYTE;
           $$.RESOLV = NULL ;
           $$.CLASS = $2.FLAG;

           if ( ! $2.DYNAMIC )
           {
             $$.LEFT    = $2.LEFT ;
             $$.RIGHT   = $2.RIGHT ;
             $$.DYNAMIC = 0;
             $$.DYNAMIC_LEFT  = (vexexpr *)0;
             $$.DYNAMIC_RIGHT = (vexexpr *)0;
           }
           else
           {
             $$.LEFT  = -2;
             $$.RIGHT = -2;
             $$.DYNAMIC = $2.DYNAMIC;
             $$.DYNAMIC_LEFT  = (vexexpr *)$2.LEFT;
             $$.DYNAMIC_RIGHT = (vexexpr *)$2.RIGHT;
           }
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
                char  buffer[ 128 ];
                char *enumname;
                char *enumval;
              int size;
              int indice;
                int numbit;
                char **pnt;
              chain_list *nm1lst;

                VBL_NM1LST = reverse (VBL_NM1LST);

              nm1lst = VBL_NM1LST;
                size   = 0;

                while (nm1lst != NULL)
              {
                     size++;
                     nm1lst = nm1lst->NEXT;
                }

              if( size != 0)
                pnt = (char **) autallocblock(size * sizeof(char*));

                numbit = vbl_codeSize( size - 1 );
                indice = 0;

                while (VBL_NM1LST != NULL)
                {
                  sprintf ( buffer, "%s.cod.%s", VBL_MODNAM, (char *)VBL_NM1LST->DATA);
                  enumname = namealloc ( buffer );

                  if (chktab (hshtab,enumname,VBL_MODNAM,VBL_SYMDFN) != 0)
                    my_vbl_error (82,enumname);

                  enumval = vbl_enumbitstring(indice,size,numbit);
                 
                  addtab (hshtab,enumname,VBL_MODNAM,VBL_SYMDFN,VBL_CSTDFN);
                  addtab (hshtab,enumname,VBL_MODNAM,VBL_TYPDFN,VBL_NUMTYP);
                  addtab (hshtab,enumname,VBL_MODNAM,VBL_WMNDFN,-1);
                  addtab (hshtab,enumname,VBL_MODNAM,VBL_WMXDFN,-1);
                  addtab (hshtab,enumname,VBL_MODNAM,VBL_LBLDFN,indice);
                  addtab (hshtab,enumname,VBL_MODNAM,VBL_PNTDFN,(long)pnt);
                  addauthelem(VBL_ENUMVAL, enumname, (long)enumval );
                 
                  pnt[indice++] = enumname;
                  VBL_NM1LST = delchain (VBL_NM1LST, VBL_NM1LST);
                }
              $$.VALUE = pnt ;
              $$.SIZE = size ;
              $$.LEFT = 0 ;
              $$.RIGHT = 0 ;
              $$.BYTE = size/256 +1;
              $$.INDEX = VBL_NUMTYP;
              $$.CLASS = 'E' ;
              $$.BASE = 0 ;
              $$.RESOLV = NULL ;
              $$.DYNAMIC = 0;
              $$.DYNAMIC_LEFT = NULL;
              $$.DYNAMIC_RIGHT = NULL;
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
                { VBL_NM1LST = addchain (VBL_NM1LST,$1); }
        | CharacterLit
                { VBL_NM1LST = addchain (VBL_NM1LST,$1); }
        ;

composite_type_definition
        : array_type_definition
              {
               $$ = $1;
              }
        ;

array_type_definition
        : unconstrained_array_definition
              { 
                my_vbl_error(123,NULL);
                $$ = $1;
              }
        | constrained_array_definition
              { $$ = $1; }
        ;

unconstrained_array_definition
        : ARRAY
          LeftParen
          index_subtype_definition
          RightParen_ERR
          OF
          subtype_indication
          {
            $$ = $6;
            $$.CLASS = 'U';
            $$.INDEX = $3.TYPE;
          }
        ;

constrained_array_definition
        : ARRAY
          index_constraint
          OF
          subtype_indication
          {
            vbtyp_list *NewType;
            char       *name;
            char        buffer[ 32 ];

            $$ = $4;

            if ( $4.CLASS == 'U' )
            {
              if ( $4.BASE->CLASS != 'A' )
              {
                my_vbl_error(123,NULL);
              }
            }
            else
            if ( $4.CLASS == 'A' )
            {
              if ( $4.BASE->CLASS != 'U' )
              {
                my_vbl_error(123,NULL);
              }

              sprintf( buffer, "_subtype_%d", VBL_NUMTYP );
              name = namealloc( buffer );

              NewType = vbh_addvbtyp(VBL_BEFPNT,name,$4.LEFT,
                   $4.RIGHT,$4.VALUE,$4.SIZE,$4.BYTE,$4.BASE,$4.INDEX,$4.RESOLV,$4.CLASS,
                   VBL_LINNUM);
              addtab(hshtab,name,VBL_MODNAM,VBL_SYMDFN,VBL_TPEDFN);
              addtab(hshtab,name,VBL_MODNAM,VBL_LBLDFN,$4.CLASS);
              addtab(hshtab,name,VBL_MODNAM,VBL_TYPDFN,$4.INDEX);
              addtab(hshtab,name,VBL_MODNAM,VBL_WMNDFN,$4.LEFT);
              addtab(hshtab,name,VBL_MODNAM,VBL_WMXDFN,$4.RIGHT);
              addtab(hshtab,name,VBL_MODNAM,VBL_PNTDFN,(long)VBL_BEFPNT->BETYP);
              VBL_NUMTYP++;

              $$.BASE = NewType;
            }
            else
            {
              my_vbl_error(123,NULL);
            }

            $$.CLASS = 'A';
            $$.INDEX = $2.TYPE;
            $$.DYNAMIC = $2.DYNAMIC;

            if ( ! $2.DYNAMIC )
            {
              $$.LEFT  = $2.LEFT;
              $$.RIGHT = $2.RIGHT;

              $$.DYNAMIC_LEFT  = (vexexpr *)0;
              $$.DYNAMIC_RIGHT = (vexexpr *)0;
            }
            else
            {
              $$.LEFT  = -2;
              $$.RIGHT = -2;

              $$.DYNAMIC_LEFT  = (vexexpr *)$2.LEFT;
              $$.DYNAMIC_RIGHT = (vexexpr *)$2.RIGHT;
            }

          }
        ;

index_subtype_definition
        : name    /* type_mark  - causes conflict */
          _RANGE
          Box
          {
              $$.NAME = $1.NAME;
              $$.LEFT = $1.LEFT;
              $$.RIGHT = $1.RIGHT;
              $$.DYNAMIC = $1.DYNAMIC;
              $$.AGGREG  = 0;
              $$.TYPE = chktab(hshtab,$1.NAME,VBL_MODNAM,VBL_TYPDFN);
              $$.FLAG = chktab(hshtab,$1.NAME,VBL_MODNAM,VBL_LBLDFN);

              if ( ( $$.TYPE == 0 ) || ( $$.FLAG == 0 ) )
              {
                my_vbl_error(83,$1);
              }
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

type_mark
          :
           simple_name
           {
             vbtyp_list *VbType;
              int type;
              $$.NAME = $1;
              $$.LEFT = -1;
              $$.RIGHT = -1;
              $$.DYNAMIC = 0;
              $$.AGGREG  = 0;
              $$.TYPE = chktab(hshtab,$1,VBL_MODNAM,VBL_TYPDFN);
              $$.FLAG = chktab(hshtab,$1,VBL_MODNAM,VBL_LBLDFN);
              type = chktab(hshtab,$1,VBL_MODNAM,VBL_SYMDFN);

              if (type != VBL_TPEDFN || $$.TYPE ==0)
              {
                my_vbl_error(84,$1);
              }

              VbType = (vbtyp_list *)chktab( hshtab,$1,VBL_MODNAM,VBL_PNTDFN);
              if ( ( VbType != (vbtyp_list *)0 ) &&
                   ( VbType->DYNAMIC           ) )
              {
                $$.DYNAMIC = VbType->DYNAMIC;
                $$.LEFT    = (long)VbType->DYNAMIC_LEFT;
                $$.RIGHT   = (long)VbType->DYNAMIC_RIGHT;
              }
          }
        ;

.signal_kind.
       : /*empty*/
              { $$ = VBH_KIND_NONE; }
       | BUS
              { $$ = VBH_KIND_BUS; }
       | REGISTER
              { $$ = VBH_KIND_REGISTER; }
       ;

architecture_statement_part
       : ..concurrent_statement..
       ;

..concurrent_statement..
       : /*empty*/
       | ..concurrent_statement..
         concurrent_statement
       ;

concurrent_statement
       : block_statement
       | concurrent_assertion_statement
       | process_statement
       | concurrent_signal_assignment_statement
       | component_instantiation_statement
       | generate_statement
       | error
         Semicolon_ERR
                { my_vbl_error (18,NULL); }
       ;

concurrent_assertion_statement
        : a_label
          unlabeled_concurrent_assertion_statement
          {
            if (chktab (hshtab,$1,VBL_MODNAM,VBL_SYMDFN) != 0)
              my_vbl_error (19,$1);

            addtab (hshtab,$1,VBL_MODNAM,VBL_SYMDFN,VBL_LABDFN);
          }
        | unlabeled_concurrent_assertion_statement
        ;

unlabeled_concurrent_assertion_statement
        : assertion_statement
        ;

block_statement
        : a_label
          BLOCK
          .guard_expression.
          _BEGIN
          {
            if (chktab( hshtab, $1,VBL_MODNAM,VBL_SYMDFN) != 0)
              my_vbl_error (19,$1);

            addtab( hshtab,$1,VBL_MODNAM,VBL_SYMDFN,VBL_LABDFN);

            if ($3.VEX != (vexexpr *)0)
            {
              VBL_GRDLST = addchain( VBL_GRDLST, (char *)$3.VEX );
            }

          }
          set_of_statements
          END_ERR
          BLOCK
          .label.
          Semicolon_ERR
          {
            if ( $3.VEX != (vexexpr *)0 )
            {
              VBL_GRDLST = delchain( VBL_GRDLST,VBL_GRDLST );
            }

            if ( ($9 != NULL) &&
                 ($9 != $1  ) ) my_vbl_error (20,$9);
          }
        ;

set_of_statements
        : ..concurrent_statement..
        ;

concurrent_signal_assignment_statement
        : a_label
          unlabeled_conditional_signal_assignment
          {
            if (chktab( hshtab,$1,VBL_MODNAM,VBL_SYMDFN) != 0)
              my_vbl_error (19,$1);
            addtab( hshtab,$1,VBL_MODNAM,VBL_SYMDFN, VBL_LABDFN );
          }
        | unlabeled_conditional_signal_assignment
        | a_label
          unlabeled_selected_signal_assignment
          {
            if (chktab (hshtab,$1,VBL_MODNAM,VBL_SYMDFN) != 0)
                my_vbl_error (19,$1);
            addtab( hshtab,$1,VBL_MODNAM,VBL_SYMDFN,VBL_LABDFN);
          }
        | unlabeled_selected_signal_assignment
        ;

unlabeled_conditional_signal_assignment
        : target
          _LESym
          .GUARDED.
          ..waveform__WHEN__condition__ELSE..
          waveform
          Semicolon_ERR
          {
	    vbl_vexstr  *VexStr;
            char         Buffer[ 128 ];
            char        *CodedSigName;
            char        *Name;
            long         Index;
            vbl_vexstr  *ExprCnd;
            vbl_vexstr  *ExprVal;
            int          ProcessType;
            int          Type;
            int          Mode;
	    int          Width;
            long         Left;
            long         Right;
            long         Left_bnd;
            long         Right_bnd;
            chain_list  *SensList;
            chain_list  *HeadChain;
            chain_list  *ScanChain;
            chain_list  *DelChain;
            chain_list **PrevChain;
            ptype_list **PrevInst;
            ptype_list  *Inst;
            ptype_list  *InstIfs;
            ptype_list  *InstAsg;
            vbtyp_list  *TypeTarget;
            vexexpr     *VexTarget;
            vexexpr     *VexValue;
            vexexpr     *VexGuard;
            long         Line;
            unsigned char Dynamic;

           if ( $1.AGGREG )
           {
             my_vbl_error(114,NULL);
           }

          if ( $5.AGGREG )
          {
            HeadChain = reverse( (chain_list *)$5.IDENT );
            ScanChain = HeadChain;

            if ( HeadChain->NEXT != (chain_list *)0 )
            {
              Width    = 0;
              VexValue = createvexoper( VEX_CONCAT, 1 );

              while ( ScanChain != (chain_list *)0 )
              {
                VexStr = (vbl_vexstr *)ScanChain->DATA;
                Width += VexStr->VEX->WIDTH;
                addvexhexpr( VexValue, VexStr->VEX );

                autfreeheap( VexStr, sizeof( vbl_vexstr ) );

                ScanChain = ScanChain->NEXT;
              }

              VexValue->WIDTH = Width;
            }
            else
            {
              VexStr   = (vbl_vexstr *)HeadChain->DATA;
              VexValue = VexStr->VEX;

              autfreeheap( VexStr, sizeof( vbl_vexstr ) );
            }

            freechain( HeadChain );

	    $5.AGGREG = 0;
	    $5.VEX    = VexValue;
          }

            sprintf( Buffer, "%s.cod.%s", VBL_MODNAM, $1.NAME );
            CodedSigName = namealloc( Buffer );

            Type = chktab( hshtab, CodedSigName, VBL_MODNAM, VBL_TYPDFN );
            ProcessType = 0;

            if ( Type >= VBH_MAX_TYPE )
            {
              Mode    = chktab( hshtab, $1.NAME, VBL_MODNAM, VBL_SYMDFN );
              Dynamic = chktab( hshtab, $1.NAME, VBL_MODNAM, VBL_DYNDFN );

              Left  = $1.LEFT;
              Right = $1.RIGHT;
            }
            else
            {
              Type    = chktab( hshtab, $1.NAME, VBL_MODNAM, VBL_TYPDFN );
              Mode    = chktab( hshtab, $1.NAME, VBL_MODNAM, VBL_SYMDFN );
              Dynamic = chktab( hshtab, $1.NAME, VBL_MODNAM, VBL_DYNDFN );

              Left  = $1.LEFT;
              Right = $1.RIGHT;
            }

            if( Mode == VBL_ICNDFN ) my_vbl_error( 21, $1.NAME );
            if( Mode == 0          ) my_vbl_error( 17, $1.NAME );

            if ( ! $1.DYNAMIC )
            {
              VexTarget = vbl_dynamicvexatom( $1.NAME, $1.LEFT, $1.RIGHT, Dynamic );
            }
            else
            {
              Left_bnd  = chktab (hshtab,$1.NAME,VBL_MODNAM,VBL_WMNDFN);
              Right_bnd = chktab (hshtab,$1.NAME,VBL_MODNAM,VBL_WMXDFN);

              VexTarget = vbl_dynamicvexatom( $1.NAME, Left_bnd, Right_bnd, Dynamic );

              if ( Right == 0 )
              {
                VexTarget = createvexbinexpr( VEX_INDEX, 1, VexTarget, (vexexpr *)Left );
              }
              else
              {
                if ( ( Mode == VBL_FUNDFN ) ||
                     ( Mode == VBL_PRODFN ) ||
                     ( Mode == VBL_TPEDFN ) )
                {
                  freevexexpr( VexTarget );
                  VexTarget = (vexexpr *)Left;
                }
                else
                if ( $1.DYNAMIC == VBL_UPTDFN )
                {
                   if ( ( ! Dynamic                                                  ) ||
                        ( ! isvexequalexpr( (vexexpr *)Left_bnd, (vexexpr *)Left   ) ) ||
                        ( ! isvexequalexpr( (vexexpr *)Right_bnd, (vexexpr *)Right ) ) )
                   {
                     VexTarget = createvexternaryexpr( VEX_TO, 1, VexTarget, 
                                                       (vexexpr *)Left, (vexexpr *)Right );
                   }
                }
                else
                if ( $1.DYNAMIC == VBL_DWTDFN )
                {
                   if ( ( ! Dynamic                                                  ) ||
                        ( ! isvexequalexpr( (vexexpr *)Left_bnd, (vexexpr *)Left   ) ) ||
                        ( ! isvexequalexpr( (vexexpr *)Right_bnd, (vexexpr *)Right ) ) )
                   {
                     VexTarget = createvexternaryexpr( VEX_DOWNTO, 1, VexTarget,
                                                    (vexexpr *)Left, (vexexpr *)Right );
                   }
                }
                else 
                {
                  VexTarget = createvexbinexpr( VEX_INDEX_N, 1, VexTarget, (vexexpr *)Left );
                  VexTarget = createvexbinexpr( VEX_INDEX_N, 1, VexTarget, (vexexpr *)Right );
                }
              }
            }

            if ( $1.SIGNED )
            {
              SetVexNodeSigned( VexTarget );
            }

            TypeTarget = get_type( Type );

            Inst     = (ptype_list *)0;
            PrevInst = &Inst;
            SensList = (chain_list *)0;

            if ( VBL_CNDLST != (chain_list *)0 )
            {
              VBL_CNDLST = reverse( VBL_CNDLST );
              VBL_VALLST = reverse( VBL_VALLST );
              VBL_LINLST = reverse( VBL_LINLST );
            }

            while ( VBL_CNDLST != (chain_list *)0 )
            {
              ProcessType |= VBH_PROCESS_CONDITIONNAL_MASK;

              ExprVal = (vbl_vexstr *)VBL_VALLST->DATA;
              ExprCnd = (vbl_vexstr *)VBL_CNDLST->DATA;
              Line    = (long        )VBL_LINLST->DATA;

              SensList = unionvexexprsupport( SensList, ExprCnd->VEX );
              SensList = unionvexexprsupport( SensList, ExprVal->VEX );

              InstAsg = vbh_addvbasg( (ptype_list *)0, dupvexexpr( VexTarget ), 
                                      simpvexexprothers( ExprVal->VEX, VexTarget->WIDTH ),
                                      TypeTarget, Line );

              InstIfs = vbh_addvbifs( (ptype_list *)0, ExprCnd->VEX, Line );
              ((vbifs_list *)InstIfs->DATA)->CNDTRUE = InstAsg;

              *PrevInst = InstIfs;
              PrevInst  = &((vbifs_list *)InstIfs->DATA)->CNDFALSE;

              autfreeheap( ExprCnd, sizeof( vbl_vexstr ) );
              autfreeheap( ExprVal, sizeof( vbl_vexstr ) );

              VBL_CNDLST = delchain( VBL_CNDLST, VBL_CNDLST );
              VBL_VALLST = delchain( VBL_VALLST, VBL_VALLST );
              VBL_LINLST = delchain( VBL_LINLST, VBL_LINLST );
            }

            SensList  = unionvexexprsupport( SensList, $5.VEX );
            *PrevInst = vbh_addvbasg( (ptype_list *)0, VexTarget,
                                      simpvexexprothers( $5.VEX, VexTarget->WIDTH ),
                                      TypeTarget, VBL_LINNUM );
            if ( $3 == VBL_GRDDFN )
            {
              ProcessType |= VBH_PROCESS_GUARDED_MASK;

              VexGuard = dupvexexpr( (vexexpr *)VBL_GRDLST->DATA );
              SensList = unionvexexprsupport( SensList, VexGuard );

              InstIfs  = vbh_addvbifs( (ptype_list *)0, VexGuard, VBL_LINNUM );
              ((vbifs_list *)InstIfs->DATA)->CNDTRUE = Inst;

              Inst = InstIfs;
            }

            PrevChain = &SensList;
            ScanChain =  SensList;

            while ( ScanChain != (chain_list *)0 )
            {
              Name = getvexvectorname( ScanChain->DATA, &Index );
              Mode = chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN );

              if ( ( Mode == VBL_CSTDFN  ) ||
                   ( Mode == VBL_GENDFN  ) ||
                   ( Mode == VBL_VARDFN  ) )
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
                
            Inst = addptype( Inst, VBH_BEWAS,
                             vbh_addvbwas( SensList, NULL, 0, NULL, VBL_LINNUM ) );
            Inst = reversetype( Inst );

            sprintf( Buffer, "p%ld", VBL_LINNUM );
            VBL_PROCNAM = namealloc( Buffer );
            VBL_BEPCS = vbh_addvbpcs( VBL_BEPCS, VBL_PROCNAM, NULL, Inst, VBL_LINNUM );
            VBL_BEPCS->TYPE = ProcessType;
            VBL_PROCNAM = (char *)0;
          }
        ;

..waveform__WHEN__condition__ELSE..
        : /*empty*/
        | ..waveform__WHEN__condition__ELSE..
          waveform
          WHEN
          expression
          ELSE
          {
            vbl_vexstr *Expr;

            Expr = (vbl_vexstr *)autallocheap( sizeof( vbl_vexstr ) );
            Expr->WIDTH = $4.WIDTH;
            Expr->VEX   = $4.VEX;
            VBL_CNDLST  = addchain( VBL_CNDLST, (char *)Expr );

            Expr = (vbl_vexstr *)autallocheap( sizeof( vbl_vexstr ) );
            Expr->WIDTH = $2.WIDTH;
            Expr->VEX   = $2.VEX;
            VBL_VALLST  = addchain( VBL_VALLST, (char *)Expr );

            VBL_LINLST = addchain( VBL_LINLST, (char *)VBL_LINNUM );
          }
        ;

unlabeled_selected_signal_assignment
        : WITH
          expression
          {
            VBL_CASSIZE = $2.WIDTH - 1;
          }
          SELECT
          target
          _LESym
          .GUARDED.
          waveform__WHEN__choices
          ...waveform__WHEN__choices..
          Semicolon_ERR
          {
            char        *CodedSigName;
            char        *Name;
            long         Index;
            char         Buffer[ 128 ];
            ptype_list  *InstCase;
            ptype_list  *InstAsg;
            ptype_list  *InstIfs;
            ptype_list  *Inst;
            chain_list  *SensList;
            vbl_vexstr  *ExprVal;
            vbcho_list  *VbChoice;
            vbcas_list  *VbCase;
            chain_list  *ScanCond;
            chain_list  *ScanChain;
            chain_list **PrevChain;
            chain_list  *DelChain;
            vbtyp_list  *TypeTarget;
            vexexpr     *VexTarget;
            vexexpr     *VexGuard;
            char        *Value;
            int          SizeValue;
            int          NumberChoice;
            int          Type;
            int          Mode;
            long         Left;
            long         Right;
            long         Left_bnd;
            long         Right_bnd;
            long         Line;
            int          ProcessType;
            unsigned char Dynamic;

           if ( $5.AGGREG )
           {
             my_vbl_error(114,NULL);
           }

            sprintf( Buffer, "%s.cod.%s", VBL_MODNAM, $5.NAME );
            CodedSigName = namealloc( Buffer );
            ProcessType = VBH_PROCESS_WITHSELECT_MASK;

            Type = chktab( hshtab, CodedSigName, VBL_MODNAM, VBL_TYPDFN );

            if ( Type >= VBH_MAX_TYPE )
            {
              Mode    = chktab( hshtab, $5.NAME, VBL_MODNAM, VBL_SYMDFN );
              Dynamic = chktab( hshtab, $5.NAME, VBL_MODNAM, VBL_DYNDFN );
              Left  = $5.LEFT;
              Right = $5.RIGHT;
            }
            else
            {
              Type = chktab( hshtab, $5.NAME, VBL_MODNAM, VBL_TYPDFN );
              Mode = chktab( hshtab, $5.NAME, VBL_MODNAM, VBL_SYMDFN );
              Dynamic = chktab( hshtab, $5.NAME, VBL_MODNAM, VBL_DYNDFN );

              Left  = $5.LEFT;
              Right = $5.RIGHT;
            }

            if( Mode == VBL_ICNDFN ) my_vbl_error( 21, $5.NAME );
            if( Mode == 0          ) my_vbl_error( 17, $5.NAME );

            if ( ! $5.DYNAMIC )
            {
              VexTarget = vbl_dynamicvexatom( $5.NAME, $5.LEFT, $5.RIGHT, Dynamic );
            }
            else
            {
              Left_bnd  = chktab (hshtab,$5.NAME,VBL_MODNAM,VBL_WMNDFN);
              Right_bnd = chktab (hshtab,$5.NAME,VBL_MODNAM,VBL_WMXDFN);

              VexTarget = vbl_dynamicvexatom( $5.NAME, Left_bnd, Right_bnd, Dynamic );

              if ( Right == 0 )
              {
                VexTarget = createvexbinexpr( VEX_INDEX, 1, VexTarget,
                                              (vexexpr *)Left );
              }
              else
              {
                if ( ( Mode == VBL_FUNDFN ) ||
                     ( Mode == VBL_TPEDFN ) ||
                     ( Mode == VBL_PRODFN ) )
                {
                  VexTarget = (vexexpr *)Left;
                }
                else
                if ( $5.DYNAMIC == VBL_UPTDFN )
                {
                   if ( ( ! Dynamic                                                  ) ||
                        ( ! isvexequalexpr( (vexexpr *)Left_bnd, (vexexpr *)Left   ) ) ||
                        ( ! isvexequalexpr( (vexexpr *)Right_bnd, (vexexpr *)Right ) ) )
                   {
                      VexTarget = createvexternaryexpr( VEX_TO, 1, VexTarget,
                                                    (vexexpr *)Left, (vexexpr *)Right );
                   }
                }
                else
                if ( $5.DYNAMIC == VBL_DWTDFN )
                {
                   if ( ( ! Dynamic                                                  ) ||
                        ( ! isvexequalexpr( (vexexpr *)Left_bnd, (vexexpr *)Left   ) ) ||
                        ( ! isvexequalexpr( (vexexpr *)Right_bnd, (vexexpr *)Right ) ) )
                   {
                     VexTarget = createvexternaryexpr( VEX_DOWNTO, 1, VexTarget,
                                                    (vexexpr *)Left, (vexexpr *)Right );
                   }
                }
                else
                {
                 VexTarget = createvexbinexpr( VEX_INDEX_N, 1, VexTarget, (vexexpr *)Left );
                 VexTarget = createvexbinexpr( VEX_INDEX_N, 1, VexTarget, (vexexpr *)Right );
                }
              }
            }

            if ( $5.SIGNED )
            {
              SetVexNodeSigned( VexTarget );
            }

            TypeTarget = get_type( Type );
            SensList   = getvexexprsupport( $2.VEX );

            InstCase = vbh_addvbcas( (ptype_list *)0, $2.VEX, get_type( $2.TYPE ), VBL_LINNUM );
            VbCase   = (vbcas_list *)InstCase->DATA;

            SizeValue    = $2.WIDTH + 1;
            NumberChoice = 0;

            for ( ScanCond  = VBL_CNDLST;
                  ScanCond != (chain_list *)0;
                  ScanCond  = ScanCond->NEXT )
            {
              NumberChoice++;
            }

            VbChoice = (vbcho_list *)autallocblock( sizeof( vbcho_list ) * NumberChoice );

            VbCase->CHOICE = VbChoice;
            VbCase->SIZE   = NumberChoice;

            while ( VBL_CNDLST != (chain_list *)0 )
            {
              NumberChoice = NumberChoice - 1;

              ExprVal = (vbl_vexstr *)VBL_VALLST->DATA;
              Line    = (long        )VBL_LINLST->DATA;

              ExprVal->VEX = simpvexexprothers( ExprVal->VEX, VexTarget->WIDTH ),
              
              InstAsg = vbh_addvbasg( (ptype_list *)0, dupvexexpr( VexTarget ), 
                                      ExprVal->VEX, TypeTarget, Line );

              SensList = unionvexexprsupport( SensList, ExprVal->VEX );

              Value = (char *)VBL_CNDLST->DATA;
              if ( Value == (char *)0 ) Value = namealloc( "others" );

              VbChoice[ NumberChoice ].INSTRUCTION = InstAsg;
              VbChoice[ NumberChoice ].SIZE        = SizeValue;
              VbChoice[ NumberChoice ].VALUES      = addchain( NULL, Value );

              autfreeheap( ExprVal, sizeof( vbl_vexstr ) );

              VBL_CNDLST = delchain( VBL_CNDLST, VBL_CNDLST );
              VBL_VALLST = delchain( VBL_VALLST, VBL_VALLST );
              VBL_LINLST = delchain( VBL_LINLST, VBL_LINLST );
            }

            if ( $7 == VBL_GRDDFN )
            {
              ProcessType |= VBH_PROCESS_GUARDED_MASK;

              VexGuard = dupvexexpr( (vexexpr *)VBL_GRDLST->DATA );
              SensList = unionvexexprsupport( SensList, VexGuard );

              InstIfs  = vbh_addvbifs( (ptype_list *)0, VexGuard, VBL_LINNUM );
              ((vbifs_list *)InstIfs->DATA)->CNDTRUE = InstCase;

              Inst = InstIfs;
            }
            else
            {
              Inst = InstCase;
            }

            PrevChain = &SensList;
            ScanChain =  SensList;

            while ( ScanChain != (chain_list *)0 )
            {
              Name = getvexvectorname( ScanChain->DATA, &Index );
              Mode = chktab( hshtab, Name, VBL_MODNAM, VBL_SYMDFN );

              if ( ( Mode == VBL_CSTDFN  ) ||
                   ( Mode == VBL_GENDFN  ) ||
                   ( Mode == VBL_VARDFN  ) )
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

            Inst->NEXT = addptype( (ptype_list *)0, VBH_BEWAS,
                                   vbh_addvbwas( SensList, NULL, 0, NULL, VBL_LINNUM ) );

            sprintf( Buffer, "p%ld", VBL_LINNUM );
            VBL_PROCNAM = namealloc( Buffer );
            VBL_BEPCS = vbh_addvbpcs( VBL_BEPCS, VBL_PROCNAM, NULL, Inst, VBL_LINNUM );
            VBL_BEPCS->TYPE = ProcessType;
            VBL_PROCNAM = (char *)0;
          }
        ;

...waveform__WHEN__choices..
        : /*empty*/
        | ...waveform__WHEN__choices..
          Comma
          waveform__WHEN__choices
        ;

waveform__WHEN__choices
        : waveform
          WHEN
          choices
          {
            vbl_vexstr *Expr;

            while ( VBL_NUMCHOICE > 0 )
            {
              Expr = (vbl_vexstr *)autallocheap( sizeof( vbl_vexstr ) );
              Expr->WIDTH = $1.WIDTH;
              Expr->VEX   = dupvexexpr( $1.VEX );

              VBL_VALLST  = addchain( VBL_VALLST, (char *)Expr );
              VBL_NUMCHOICE--;
            }

            freevexexpr( $1.VEX );
          }
        ;

process_statement
       : a_label
         {
           if ( chktab (hshtab,$1,VBL_MODNAM,VBL_SYMDFN) != 0)
             my_vbl_error (19,$1);

           addtab( hshtab,$1,VBL_MODNAM,VBL_SYMDFN, VBL_LABDFN);

           VBL_PROCNAM = $1;
         }
         unlabeled_process_statement
       | {
           char buffer[ 20 ];
           sprintf( buffer, "p%ld", VBL_LINNUM );
           VBL_PROCNAM = namealloc( buffer );
         }
         unlabeled_process_statement
       ;

unlabeled_process_statement
       : PROCESS
            { VBL_NM1LST = NULL;}
         .sensitivity_list.
       {
         VBL_BEPCS = vbh_addvbpcs( VBL_BEPCS, VBL_PROCNAM,VBL_NM1LST,NULL,VBL_LINNUM );
         VBL_BEPCS->TYPE = VBH_PROCESS_SEQUENTIAL_MASK;
         VBL_NM1LST = NULL; 
       }
       process_declarative_part
       _BEGIN
       {
         void *pnt;
         pnt = &(VBL_BEPCS->INSTRUCTION);
         VBL_INSLST = addchain(NULL,pnt);
       }
       sequence_of_statements
       {
                  struct ptype **pnt;
                  struct ptype *ptype;
                  pnt = (struct ptype**)VBL_INSLST->DATA;
                  ptype = reversetype(*pnt);
                  *pnt = ptype;
                  VBL_INSLST = delchain (VBL_INSLST, VBL_INSLST);
       }
         END_ERR
         PROCESS
         .label.
         Semicolon_ERR
         { 
           VBL_PROCNAM = (char *)0;
           yyerrok;
         }
       ;

process_declarative_part
        : ..process_declarative_item..
        ;


..process_declarative_item..
        : /*empty*/
        | ..process_declarative_item..
          process_declarative_item
                {
                yyerrok;
                }
        ;

process_declarative_item
        : constant_declaration
        | variable_declaration
        | type_declaration
        | subtype_declaration
        | attribute_declaration
        | attribute_specification
        | subprogram_declaration
        | subprogram_body
        ;

.sensitivity_list.
        : /*empty*/
        | sensitivity_list
        ;

sensitivity_list
       : LeftParen
         signal_list
         RightParen {}
       ;

signal_list
        : name
          ...name..
       {
           vexexpr    *Vex;
           chain_list *Chain;
           char       *LocalName;

           if ( chktab(hshtab,$1.NAME,VBL_FUNCNAM,VBL_SYMDFN ) )
           {
             LocalName = VBL_FUNCNAM;
           }
           else
           if ( chktab(hshtab,$1.NAME,VBL_MODNAM,VBL_SYMDFN ) )
           {
             LocalName = VBL_MODNAM;
           }
           else
           {
             my_vbl_error(17,$1.NAME);
           }

           if ( ! $1.DYNAMIC )
           {
             if ( ( $1.LEFT  == -1 ) &&
                  ( $1.RIGHT == -1 ) )
             {
               Vex = createvexatombit( $1.NAME );
             }
             else
             {
               Vex = createvexatomvec( $1.NAME, $1.LEFT, $1.RIGHT );
             }
           }
           else
           {
             Vex = createvexatomvec( $1.NAME, -1, -1 );
           }

           Chain = getvexatombitname( Vex );
           VBL_NM1LST = append( VBL_NM1LST, Chain );
           freevexexpr( Vex );
       }
        ;

...name..
        : /*empty*/
        | Comma
          name
          ...name..
       {
           vexexpr    *Vex;
           chain_list *Chain;
           char       *LocalName;

           if ( chktab(hshtab,$2.NAME,VBL_FUNCNAM,VBL_SYMDFN ) )
           {
             LocalName = VBL_FUNCNAM;
           }
           else
           if ( chktab(hshtab,$2.NAME,VBL_MODNAM,VBL_SYMDFN ) )
           {
             LocalName = VBL_MODNAM;
           }
           else
           {
             my_vbl_error(17,$2.NAME);
           }

           if ( ! $2.DYNAMIC )
           {
             if ( ( $2.LEFT  == -1 ) &&
                  ( $2.RIGHT == -1 ) )
             {
               Vex = createvexatombit( $2.NAME );
             }
             else
             {
               Vex = createvexatomvec( $2.NAME, $2.LEFT, $2.RIGHT );
             }
           }
           else
           {
             Vex = createvexatomvec( $2.NAME, -1, -1 );
           }

           Chain = getvexatombitname( Vex );
           VBL_NM1LST = append( VBL_NM1LST, Chain );
           freevexexpr( Vex );
       }
        ;

sequence_of_statements
       :
       | sequence_of_statements
         sequential_statement
       ;

sequential_statement
       :  assertion_statement
        | variable_assignment_statement
        | signal_assignment_statement
        | if_statement
        | case_statement
        | loop_statement
        | next_statement
        | exit_statement
        | return_statement
        | null_statement
        | procedure_call_statement
        | wait_statement
        ;

loop_statement
        : a_label
          {
           if ( chktab (hshtab,$1,VBL_MODNAM,VBL_SYMDFN) != 0)
             my_vbl_error (19,$1);

            VBL_LOOPLBLLST = addchain(VBL_LOOPLBLLST,$1 );
          }

          unlabeled_loop_statement
        |
          {
             char buffer[ 20 ];

             sprintf( buffer, "loop_%ld", VBL_LINNUM );
             VBL_LOOPLBLLST = addchain ( VBL_LOOPLBLLST, namealloc( buffer ) );
          }
          unlabeled_loop_statement
        ;

unlabeled_loop_statement
        : .iteration_scheme.
          LOOP
          {
            struct ptype **pnt;
            vbl_name      *Range;
            vbvar_list    *VbVar;
            char          *LocalName;
            vexexpr       *Target;
            short          Signed;
            long           Left;
            long           Right;
            long           AttrLeft;
            long           AttrRight;
            char           StrFlag;
            char           ForUp;

            pnt = (struct ptype**)VBL_INSLST->DATA;
            VBL_LOOPLBL = (char *)VBL_LOOPLBLLST->DATA;

            if ( $1.IDENT != (char *)0 )
            {
              if ( VBL_FUNCNAM != (char *)0 )
              {
                LocalName = VBL_FUNCNAM;
                StrFlag   = 'v';
              }
              else
              {
                LocalName = VBL_PROCNAM;
                StrFlag   = 'V';
              }

              Range = (vbl_name *)$1.VEX;

              if ( Range->DYNAMIC )
              {
                if ( Range->DYNAMIC == VBL_UPTDFN ) ForUp = 1;
                else                                ForUp = 0;

                AttrLeft  = -1;
                AttrRight = -1;
              }
              else
              {
                AttrLeft  = Range->LEFT;
                AttrRight = Range->RIGHT;

                if ( AttrLeft <= AttrRight ) ForUp = 1;
                else                         ForUp = 0;
              }

              if ( chktab( hshtab,$1.IDENT,LocalName,VBL_SYMDFN ) != 0 )
              {
                Target = (vexexpr *)chktab( hshtab, $1.IDENT, LocalName, VBL_PNTDFN );
                Signed = ( IsVexNodeSigned( Target ) != 0 );
              }
              else
              {
                VbVar = (vbvar_list *)vbl_addstr( StrFlag,0,
                                                 get_type(VBH_TYPE_INTEGER),VBH_TYPE_INTEGER,
                                                'I',$1.IDENT, -1, -1, NULL, 0 ,0);
                Target = VbVar->TARGET; 
                Signed = ( IsVexNodeSigned( VbVar->TARGET ) != 0 );

                if ( IsVexAtomBit( VbVar->TARGET ) )
                {
                  Left  = -1;
                  Right = -1;
                }
                else
                {
                  Left  = VbVar->TARGET->LEFT;
                  Right = VbVar->TARGET->RIGHT;
                }

                addtab( hshtab,$1.IDENT,LocalName,VBL_SYMDFN,VBL_VARDFN);
                addtab( hshtab,$1.IDENT,LocalName,VBL_TYPDFN,VBH_TYPE_INTEGER);
                addtab( hshtab,$1.IDENT,LocalName,VBL_WMNDFN,Left);
                addtab( hshtab,$1.IDENT,LocalName,VBL_WMXDFN,Right);
                addtab( hshtab,$1.IDENT,LocalName,VBL_ATLDFN,AttrLeft);
                addtab( hshtab,$1.IDENT,LocalName,VBL_ATRDFN,AttrRight);
                addtab( hshtab,$1.IDENT,LocalName,VBL_LBLDFN,0);
                addtab( hshtab,$1.IDENT,LocalName,VBL_PNTDFN,(long)VbVar->TARGET);
                addtab( hshtab,$1.IDENT,LocalName,VBL_SUNDFN,Signed);
              }

              if ( ! Range->DYNAMIC )
              {
                Range->LEFT  = (long)createvexatomlong( AttrLeft , 0, Signed );
                Range->RIGHT = (long)createvexatomlong( AttrRight, 0, Signed );
              }

              *pnt = vbh_addvbfor(*pnt,VBL_LOOPLBL,dupvexexpr(Target),
                                  Range->LEFT,Range->RIGHT,ForUp, VBL_LINNUM );

              VBL_INSLST = addchain( VBL_INSLST, 
                                     &(((struct vbfor*)(*pnt)->DATA)->INSTRUCTION));

              autfreeheap( Range, sizeof( vbl_name ) );
            }
            else
            if ( $1.VEX != (vexexpr *)0 )
            {
              *pnt = vbh_addvbwhi(*pnt,VBL_LOOPLBL,$1.VEX, VBL_LINNUM );
              VBL_INSLST = addchain( VBL_INSLST, 
                                    &(((struct vbwhi*)(*pnt)->DATA)->INSTRUCTION));
            }
            else
            {
              *pnt = vbh_addvblop(*pnt,VBL_LOOPLBL, VBL_LINNUM );
              VBL_INSLST = addchain( VBL_INSLST, 
                                    &(((struct vblop *)(*pnt)->DATA)->INSTRUCTION));
            }
          }
          sequence_of_statements
          END_ERR
          LOOP
          .label.
          Semicolon_ERR
          {
            struct ptype **pnt;
            struct ptype *ptype;

            pnt = (struct ptype**)VBL_INSLST->DATA;
            ptype = reversetype(*pnt);
            *pnt = ptype;

            if ( $7 != NULL )
            {
              if ( VBL_LOOPLBLLST->DATA != $7 )
              {
                my_vbl_error( 95, $7 );
              }
            }

            VBL_INSLST = delchain( VBL_INSLST, VBL_INSLST);
            VBL_LOOPLBLLST = delchain( VBL_LOOPLBLLST, VBL_LOOPLBLLST );
            VBL_LOOPLBL = (char *)NULL;
          }
        ;

.iteration_scheme.
        : /*empty*/
          {
            $$ = vbl_crtvex( EMPTYOP,NULL ,NULL ,-1,-1,0);
          }
        | iteration_scheme
          {
            $$ = $1;
          }
        ;

iteration_scheme
        : WHILE
          expression
          {
            $$ = $2;
          }
        | FOR
          loop_parameter_specification
          {
            $$ = $2;
          }
        ;

loop_parameter_specification
        : Identifier
          _IN
          discrete_range
          {
            vbl_name *Range;

            Range  = (vbl_name *)autallocheap( sizeof( vbl_name ) );
            *Range = $3;

            $$.IDENT = $1;
            $$.VEX   = (vexexpr *)Range;
          }
        ;

wait_statement
       : WAIT
         {
           if ( ( VBL_BEPCS              != NULL ) &&
                ( VBL_BEPCS->SENSITIVITY != NULL ) )
           {
             my_vbl_error(96,NULL);
           }
         }
         .sensitivity_clause.
         .condition_clause.
         .timeout_clause.
         Semicolon_ERR
         {
           struct ptype **pnt;
           struct ptype  *ptptyp;
           struct vbwas  *ptvbwas;

           ptvbwas = vbh_addvbwas( VBL_NM1LST, $4.VEX, $5.TYPE, 
                                   (vexexpr *)$5.NAME, VBL_LINNUM );

           pnt = (struct ptype **) VBL_INSLST->DATA;
           ptptyp = addptype(*pnt, VBH_BEWAS, ptvbwas);
           *pnt = ptptyp;
           VBL_NM1LST = NULL;
         }
       ;

.sensitivity_clause.
       : /*empty*/
       { VBL_NM1LST = NULL;}
       | sensitivity_clause
       ;

sensitivity_clause
       : ON
       { VBL_NM1LST = NULL;}
         signal_list
       ;

.condition_clause.
       : /*empty*/
                { $$ = vbl_crtvex (EMPTYOP ,NULL ,NULL ,-1,-1,0); }
       | condition_clause
              {
              $$ = $1;
              }
       ;

condition_clause
       : UNTIL
         expression
         {
           $$ = $2;
         }
       ;

.timeout_clause.
       : /*empty*/
         {
           $$.NAME  = NULL;
           $$.LEFT  = 0;
           $$.RIGHT = 0;
           $$.FLAG  = 0;
           $$.TYPE  = 0;
           $$.DYNAMIC = 0;
           $$.AGGREG  = 0;
         }
       | timeout_clause
         {
           $$ = $1;
         }
       ;

timeout_clause
       : FOR
         simple_expression
         time_label
         {
           $$.NAME   = (char *)$2.VEX;
           $$.TYPE   = $3;
           $$.FLAG   = 0;
           $$.SIGNED = 0;
           $$.LEFT   = 0;
           $$.RIGHT  = 0;
           $$.AGGREG  = 0;
           $$.DYNAMIC = 1;

           viewvexexprboundln( $2.VEX );
           fprintf( stdout, " %d\n", $3 );
         }
       ;

time_label
       : _FS
         {
              $$ = VBH_TIME_UNIT_FS;
         }
       | _PS
         {
              $$ = VBH_TIME_UNIT_PS;
         }
       | _NS
         {
              $$ = VBH_TIME_UNIT_NS;
         }
       | _MS
         {
              $$ = VBH_TIME_UNIT_MS;
         }
       ;

next_statement
       : _NEXT
         .label.
         .WHEN__condition.
         Semicolon_ERR
         {
           struct vbnxt  *ptvbnxt;
           struct ptype **pnt;

           pnt = (struct ptype**)VBL_INSLST->DATA;
           ptvbnxt = vbh_addvbnxt($2, $3.VEX,VBL_LINNUM);
           *pnt = addptype( *pnt, VBH_BENXT, ptvbnxt );
         }
       ;

exit_statement
       : _EXIT
         .label.
         .WHEN__condition.
         Semicolon_ERR
         {
           struct vbext  *ptvbext;
           struct ptype **pnt;

           pnt = (struct ptype**)VBL_INSLST->DATA;
           ptvbext = vbh_addvbext($2, $3.VEX,VBL_LINNUM);
           *pnt = addptype( *pnt, VBH_BEEXT, ptvbext );
         }
       ;

.WHEN__condition.
        : /*empty*/
          { $$.VEX = (vexexpr *)0; }
        | WHEN
          condition
          { $$ = $2; }
        ;

return_statement
        : _RETURN
          .expression.
          Semicolon_ERR
          {
            struct vbret  *ptvbret;
            struct ptype **pnt;

            if ( VBL_FUNCNAM == (char *)0 )
            {
              my_vbl_error(120,NULL);
            }

            pnt = (struct ptype**)VBL_INSLST->DATA;
            ptvbret = vbh_addvbret($2.VEX,VBL_LINNUM);
            *pnt = addptype( *pnt, VBH_BERET, ptvbret );
          }
        ;

.expression.
        : /*empty*/
          { $$.VEX = (vexexpr *)0; }
        | expression
          { $$ = $1; }
        ;

assertion_statement
       : ASSERT
         expression
         .REPORT__expression.
         .SEVERITY__expression.
         Semicolon_ERR
         { }
       ;

variable_assignment_statement
        : target
          VarAsgn
          expression
          Semicolon_ERR
        {
           unsigned int  type;
           char         *codedsigname;
           int           mode;
           long          left ,right;
           long          left_bnd ,right_bnd;
           char          buffer[128];
           struct ptype **pnt;
           vexexpr       *vex;
           char          *LocalName;
           vbl_vexstr    *VexStr;
           vbl_vexstr    *VexStr2;
           chain_list    *ScanChain;
           chain_list    *ScanChain2;
           ptype_list    *AsgList;
           unsigned char  dynamic;

           if ( $1.AGGREG )
           {
             if ( ! $3.AGGREG )
             {
               my_vbl_error(114,NULL);
             }

             ScanChain  = (chain_list *)$1.NAME;
             ScanChain2 = (chain_list *)$3.IDENT;
             AsgList    = (ptype_list *)0;

             while ( ( ScanChain  != (chain_list *)0 ) &&
                     ( ScanChain2 != (chain_list *)0 ) )
             {
               VexStr  = (vbl_vexstr *)ScanChain->DATA;
               VexStr2 = (vbl_vexstr *)ScanChain2->DATA;

               AsgList = vbh_addvbvar( AsgList, VexStr->VEX,
               simpvexexprothers( VexStr2->VEX, VexStr->VEX->WIDTH ),
                                       get_type(VexStr->TYPE), VBL_LINNUM );

               autfreeheap( VexStr , sizeof( vbl_vexstr ) );
               autfreeheap( VexStr2, sizeof( vbl_vexstr ) );

               ScanChain  = ScanChain ->NEXT;
               ScanChain2 = ScanChain2->NEXT;
             }

             if ( ( ScanChain  != (chain_list *)0 ) ||
                  ( ScanChain2 != (chain_list *)0 ) )
             {
               my_vbl_error(114,NULL);
             }

             freechain( (chain_list *)$1.NAME );
             freechain( (chain_list *)$3.IDENT );

             pnt = (struct ptype**)(VBL_INSLST->DATA);
             *pnt = (struct ptype *)vbh_addvbagr(*pnt, AsgList, VBL_LINNUM );
           }
           else
           {
             if ( chktab(hshtab,$1.NAME,VBL_FUNCNAM,VBL_SYMDFN ) )
             {
               LocalName = VBL_FUNCNAM;
             }
             else
             if ( chktab(hshtab,$1.NAME,VBL_PROCNAM,VBL_SYMDFN ) )
             {
               LocalName = VBL_PROCNAM;
             }
             else
             {
               my_vbl_error(27,$1.NAME);
             }
  
             sprintf (buffer, "%s.cod.%s", LocalName, $1.NAME);
             codedsigname = namealloc( buffer );
  
             type = chktab(hshtab,codedsigname,LocalName,VBL_TYPDFN);
  
              if ( type >= VBH_MAX_TYPE )
             {
                 mode  = chktab(hshtab,$1.NAME,LocalName,VBL_SYMDFN);
                 dynamic = chktab( hshtab, $1.NAME, LocalName, VBL_DYNDFN );
                 left  = $1.LEFT;
                 right = $1.RIGHT;
             }
             else
             {
                type = chktab(hshtab,$1.NAME,LocalName,VBL_TYPDFN);
                mode = chktab(hshtab,$1.NAME,LocalName,VBL_SYMDFN);
                dynamic = chktab( hshtab, $1.NAME, LocalName, VBL_DYNDFN );
  
                left  = $1.LEFT;
                right = $1.RIGHT;
             }
  
             if ( ! $1.DYNAMIC )
             {
               if ( ( $1.LEFT  == -1 ) &&
                    ( $1.RIGHT == -1 ) )
               {
                 vex = createvexatombit( $1.NAME );
               }
               else
               {
                 vex = createvexatomvec( $1.NAME, $1.LEFT, $1.RIGHT );
               }
             }
             else
             {
               left_bnd  = chktab (hshtab,$1.NAME,LocalName,VBL_WMNDFN);
               right_bnd = chktab (hshtab,$1.NAME,LocalName,VBL_WMXDFN);
  
               vex = vbl_dynamicvexatom( $1.NAME, left_bnd, right_bnd, dynamic );
  
               if ( right == 0 )
               {
                 vex = createvexbinexpr( VEX_INDEX, 1, vex, (vexexpr *)left );
               }
               else
               {
                 if ( ( mode == VBL_FUNDFN ) ||
                      ( mode == VBL_TPEDFN ) ||
                      ( mode == VBL_PRODFN ) )
                 {
                   vex = (vexexpr *)left;
                 }
                 else
                 if ( $1.DYNAMIC == VBL_UPTDFN )
                 {
                   if ( ( ! dynamic                                                  ) ||
                        ( ! isvexequalexpr( (vexexpr *)left_bnd, (vexexpr *)left   ) ) ||
                        ( ! isvexequalexpr( (vexexpr *)right_bnd, (vexexpr *)right ) ) )
                   {
                     vex = createvexternaryexpr( VEX_TO, 1, vex,
                                               (vexexpr *)left, (vexexpr *)right );
                   }
                 }
                 else
                 if ( $1.DYNAMIC == VBL_DWTDFN )
                 {
                   if ( ( ! dynamic                                                  ) ||
                        ( ! isvexequalexpr( (vexexpr *)left_bnd, (vexexpr *)left   ) ) ||
                        ( ! isvexequalexpr( (vexexpr *)right_bnd, (vexexpr *)right ) ) )
                   {
                     vex = createvexternaryexpr( VEX_DOWNTO, 1, vex,
                                               (vexexpr *)left, (vexexpr *)right );
                   }
                 }
                 else
                 {
                   vex = createvexbinexpr( VEX_INDEX_N, 1, vex, (vexexpr *)left );
                   vex = createvexbinexpr( VEX_INDEX_N, 1, vex, (vexexpr *)right );
                 }
               }
             }
  
             pnt = (struct ptype**)(VBL_INSLST->DATA);
             *pnt = (struct ptype *)vbh_addvbvar(*pnt,vex,
             simpvexexprothers( $3.VEX, vex->WIDTH ),get_type(type),VBL_LINNUM);
           }
         }
         ;

procedure_call_statement
        : name
          Semicolon_ERR
          { 
            struct vbcal  *ptvbcal;
            struct ptype **pnt;
            vexexpr       *VexFunc;

            pnt = (struct ptype**)VBL_INSLST->DATA;

            if ( chktab( hshtab,$1.NAME,VBL_MODNAM,VBL_SYMDFN) != VBL_PRODFN )
            {
              my_vbl_error(119,$1.NAME);
            }

            if ( ! $1.DYNAMIC ) VexFunc = createvexfunc( $1.NAME, 0 );
            else                VexFunc = (vexexpr *)$1.LEFT;

            ptvbcal = vbh_addvbcal( VexFunc, VBL_LINNUM );
            *pnt = addptype( *pnt, VBH_BECAL, ptvbcal );
          }
        ;

signal_assignment_statement
       : target
         _LESym
         waveform
         Semicolon_ERR
       {
           unsigned int   type;
           int            mode;
           char           buffer[128];
           char          *codedsigname;
           char          *LocalName;
           long           left ,right;
           long           left_bnd ,right_bnd;
           struct ptype **pnt;
           vexexpr       *vex;
           vbl_vexstr    *VexStr;
           vbl_vexstr    *VexStr2;
           chain_list    *ScanChain;
           chain_list    *ScanChain2;
           ptype_list    *AsgList;
           unsigned char  dynamic;

           if ( $1.AGGREG )
           {
             if ( ! $3.AGGREG )
             {
               my_vbl_error(114,NULL);
             }

             ScanChain  = (chain_list *)$1.NAME;
             ScanChain2 = (chain_list *)$3.IDENT;
             AsgList    = (ptype_list *)0;

             while ( ( ScanChain  != (chain_list *)0 ) &&
                     ( ScanChain2 != (chain_list *)0 ) )
             {
               VexStr  = (vbl_vexstr *)ScanChain->DATA;
               VexStr2 = (vbl_vexstr *)ScanChain2->DATA;

               AsgList = vbh_addvbasg( AsgList, VexStr->VEX,
                                       simpvexexprothers( VexStr2->VEX, VexStr->VEX->WIDTH ),
                                       get_type(VexStr->TYPE), VBL_LINNUM );

               autfreeheap( VexStr , sizeof( vbl_vexstr ) );
               autfreeheap( VexStr2, sizeof( vbl_vexstr ) );

               ScanChain  = ScanChain ->NEXT;
               ScanChain2 = ScanChain2->NEXT;
             }

             if ( ( ScanChain  != (chain_list *)0 ) ||
                  ( ScanChain2 != (chain_list *)0 ) )
             {
               my_vbl_error(114,NULL);
             }

             freechain( (chain_list *)$1.NAME );
             freechain( (chain_list *)$3.IDENT );

             pnt = (struct ptype**)(VBL_INSLST->DATA);
             *pnt = (struct ptype *)vbh_addvbagr(*pnt, AsgList, VBL_LINNUM );
           }
           else
           {
             if ( chktab(hshtab,$1.NAME,VBL_FUNCNAM,VBL_SYMDFN ) )
             {
               LocalName = VBL_FUNCNAM;
             }
             else
             if ( chktab(hshtab,$1.NAME,VBL_MODNAM,VBL_SYMDFN ) )
             {
               LocalName = VBL_MODNAM;
             }
             else
             {
               my_vbl_error(17,$1.NAME);
             }
  
             sprintf (buffer, "%s.cod.%s", LocalName, $1.NAME);
             codedsigname = namealloc( buffer );
  
             type = chktab(hshtab,codedsigname,LocalName,VBL_TYPDFN);
  
             if ( type >= VBH_MAX_TYPE )
             {
               mode  = chktab(hshtab,$1.NAME,LocalName,VBL_SYMDFN);
               dynamic = chktab(hshtab,$1.NAME,LocalName,VBL_DYNDFN);
               left  = $1.LEFT;
               right = $1.RIGHT;
             }
             else
             {
               type = chktab(hshtab,$1.NAME,LocalName,VBL_TYPDFN);
               mode = chktab(hshtab,$1.NAME,LocalName,VBL_SYMDFN);
               dynamic = chktab(hshtab,$1.NAME,LocalName,VBL_DYNDFN);
  
               left  = $1.LEFT;
               right = $1.RIGHT;
             }
  
             if( mode == VBL_ICNDFN) my_vbl_error(21,$1.NAME);
             if( mode == 0         ) my_vbl_error(17,$1.NAME);
  
             if ( ! $1.DYNAMIC )
             {
               if ( ( $1.LEFT  == -1 ) &&
                    ( $1.RIGHT == -1 ) )
               {
                 vex = createvexatombit( $1.NAME );
               }
               else
               {
                 vex = createvexatomvec( $1.NAME, $1.LEFT, $1.RIGHT );
               }
             }
             else
             {
               left_bnd  = chktab (hshtab,$1.NAME,LocalName,VBL_WMNDFN);
               right_bnd = chktab (hshtab,$1.NAME,LocalName,VBL_WMXDFN);
  
               vex = vbl_dynamicvexatom( $1.NAME, left_bnd, right_bnd, dynamic );
  
               if ( right == 0 )
               {
                 vex = createvexbinexpr( VEX_INDEX, 1, vex, (vexexpr *)left );
               }
               else
               {
                 if ( ( mode == VBL_FUNDFN ) ||
                      ( mode == VBL_TPEDFN ) ||
                      ( mode == VBL_PRODFN ) )
                 {
                   vex = (vexexpr *)left;
                 }
                 else
                 if ( $1.DYNAMIC == VBL_UPTDFN )
                 {
                   if ( ( ! dynamic                                                  ) ||
                        ( ! isvexequalexpr( (vexexpr *)left_bnd, (vexexpr *)left   ) ) ||
                        ( ! isvexequalexpr( (vexexpr *)right_bnd, (vexexpr *)right ) ) )
                   {
                      vex = createvexternaryexpr( VEX_TO, 1, vex,
                                                  (vexexpr *)left, (vexexpr *)right );
                   }
                 }
                 else
                 if ( $1.DYNAMIC == VBL_DWTDFN )
                 {
                   if ( ( ! dynamic                                                  ) ||
                        ( ! isvexequalexpr( (vexexpr *)left_bnd, (vexexpr *)left   ) ) ||
                        ( ! isvexequalexpr( (vexexpr *)right_bnd, (vexexpr *)right ) ) )
                   {
                     vex = createvexternaryexpr( VEX_DOWNTO, 1, vex,
                                               (vexexpr *)left, (vexexpr *)right );
                   }
                 }
                 else
                 {
                   vex = createvexbinexpr( VEX_INDEX_N, 1, vex, (vexexpr *)left );
                   vex = createvexbinexpr( VEX_INDEX_N, 1, vex, (vexexpr *)right );
                 }
               }
             }
  
             pnt = (struct ptype**)(VBL_INSLST->DATA);
             *pnt = (struct ptype *)vbh_addvbasg(*pnt,vex,
             simpvexexprothers($3.VEX, vex->WIDTH ),get_type(type), VBL_LINNUM );
           }
       }
       ;

waveform_element
       : expression
              { $$ = $1; }
       ;

if_statement
       : IF
         condition
         THEN
         {
           struct ptype **pnt;

           pnt = (struct ptype**)VBL_INSLST->DATA;
           *pnt = vbh_addvbifs( *pnt,$2.VEX, VBL_LINNUM  );
           VBL_INSLST = addchain(VBL_INSLST,&(((struct vbifs*)(*pnt)->DATA)->CNDTRUE));
         }
         sequence_of_statements
         {
           struct ptype **pnt;
           struct ptype *ptype;

           pnt = (struct ptype**)VBL_INSLST->DATA;
           ptype = reversetype(*pnt);
           *pnt = ptype;
           VBL_INSLST = delchain (VBL_INSLST, VBL_INSLST);
           pnt = (struct ptype**)VBL_INSLST->DATA;
           VBL_INSLST = addchain(VBL_INSLST,&(((struct vbifs*)(*pnt)->DATA)->CNDFALSE));
         }
         ..ELSIF__THEN__seq_of_stmts..
         .ELSE__seq_of_stmts.
         END_ERR
         IF
         Semicolon_ERR
       ;

..ELSIF__THEN__seq_of_stmts..
       : /*empty*/
       | ..ELSIF__THEN__seq_of_stmts..
         ELSIF
         condition
         THEN
         {
           struct ptype **pnt;

           pnt = (struct ptype**)VBL_INSLST->DATA;
           *pnt = vbh_addvbifs(*pnt,$3.VEX, VBL_LINNUM );
           VBL_INSLST = addchain(VBL_INSLST,(void*)&((struct vbifs*)(*pnt)->DATA)->CNDTRUE);
         }
         sequence_of_statements
         {
           struct ptype **pnt;
           struct ptype *ptype;

           pnt = (struct ptype**)VBL_INSLST->DATA;
           ptype = reversetype(*pnt);
           *pnt = ptype;
           VBL_INSLST = delchain (VBL_INSLST, VBL_INSLST);
           pnt = (struct ptype**)VBL_INSLST->DATA;
           VBL_INSLST = delchain (VBL_INSLST, VBL_INSLST);
           VBL_INSLST = addchain(VBL_INSLST,&((struct vbifs*)(*pnt)->DATA)->CNDFALSE);
         }
       ;

.ELSE__seq_of_stmts.
       : /*empty*/
              {
                  VBL_INSLST = delchain (VBL_INSLST, VBL_INSLST);
                yyerrok;
              }
       | ELSE
         sequence_of_statements
              {
                  struct ptype *ptype;
                  struct ptype **pnt;
                  pnt = (struct ptype**)VBL_INSLST->DATA;
                  ptype = reversetype(*pnt);
                  *pnt = ptype;
                  VBL_INSLST = delchain (VBL_INSLST, VBL_INSLST);
                yyerrok;
              }
       ;

case_statement
       : CASE
         expression
         {
           struct ptype **pnt;

           pnt = (struct ptype**)VBL_INSLST->DATA;

           VBL_OTHPNT  = NULL;
           VBL_CASPNT  = NULL;
           VBL_CASSIZE = $2.WIDTH - 1;

           *pnt = vbh_addvbcas(*pnt,$2.VEX,get_type($2.TYPE), VBL_LINNUM );

           VBL_INSLST = addchain( VBL_INSLST,(void*)&(((struct vbcas*)((*pnt)->DATA))->SIZE));
           VBL_INSLST = addchain( VBL_INSLST,(void*)&(((struct vbcas*)(*pnt)->DATA)->CHOICE));
         }
         IS
         case_statement_alternative
         ..case_statement_alternative..
         END_ERR
         CASE
         Semicolon_ERR
         {
           unsigned int size=0;
           unsigned int *size1;
           int indice=0;
           struct choice_chain *ch;
           struct vbcho **pnt;
           struct vbcho *tab;
           struct choice_chain *nm1lst;


           nm1lst = VBL_CASPNT;
           while (nm1lst != NULL)
           {
             size++;
             nm1lst = nm1lst->NEXT;
           }
           if ( VBL_OTHPNT != NULL ) size++;

           pnt = (struct vbcho**)VBL_INSLST->DATA;
           VBL_INSLST = delchain(VBL_INSLST,VBL_INSLST);

           if( size != 0)
             tab  = (struct vbcho*)autallocblock(size*sizeof(struct vbcho));

           while(VBL_CASPNT)
           {
             tab[indice].INSTRUCTION = reversetype(VBL_CASPNT->INSTRUCTION);
             tab[indice].SIZE = VBL_CASPNT->SIZE;
             tab[indice++].VALUES = VBL_CASPNT->VALUES;
             ch = VBL_CASPNT;
             VBL_CASPNT =VBL_CASPNT->NEXT;
             autfreeblock( ch );
             /*free(ch);*/
           }
           if ( VBL_OTHPNT != NULL )
           {
             tab[indice].INSTRUCTION = reversetype(VBL_OTHPNT);
             tab[indice].SIZE = -1;
             tab[indice++].VALUES = addchain( NULL, namealloc("others") );
           }
           (*pnt) = tab;
           size1 = (unsigned int*) VBL_INSLST->DATA;
           *size1 = size;
           VBL_INSLST = delchain(VBL_INSLST,VBL_INSLST);
         }
       ;

..case_statement_alternative..
       : /*empty*/
       | ..case_statement_alternative..
         case_statement_alternative
              { yyerrok; }
       ;

case_statement_alternative
       : WHEN
         choices
         Arrow
         {
           VBL_PTYPE  = NULL;
           VBL_INSLST = addchain(VBL_INSLST,(void*)&VBL_PTYPE);
         }
         sequence_of_statements
         {
           if( VBL_CNDLST == NULL)
             my_vbl_error(85,"case");
           else
           if ( VBL_CNDLST->DATA == NULL)
           {
             VBL_OTHPNT = VBL_PTYPE ;
             VBL_CNDLST = delchain(VBL_CNDLST,VBL_CNDLST);
           }
           else
           {
             struct choice_chain *NewChoice;
             char                *Value;
             int                  Length;

             NewChoice = (struct choice_chain*)autallocblock(sizeof(struct choice_chain));
             NewChoice->VALUES      = (chain_list *)0;
             NewChoice->SIZE        = 0;
             NewChoice->INSTRUCTION = VBL_PTYPE;

             NewChoice->NEXT = VBL_CASPNT;
             VBL_CASPNT      = NewChoice;

             while(VBL_CNDLST)
             {
               Value = (char *)VBL_CNDLST->DATA;
               Length = strlen( Value );

               NewChoice->VALUES = addchain( NewChoice->VALUES, (void *)Value );
               if ( NewChoice->SIZE < Length ) NewChoice->SIZE = Length;

               VBL_CNDLST = delchain(VBL_CNDLST,VBL_CNDLST);
             }
           }
           VBL_INSLST = delchain(VBL_INSLST,VBL_INSLST);
         }
       ;

waveform
       : waveform_element
              { $$ = $1; }
       ;

choices
       : choice
         {
           VBL_LINLST = addchain (VBL_LINLST, (void *)VBL_LINNUM );

           if ( ! strcmp($1.NAME , "others"))
           {
             VBL_CNDLST = addchain (VBL_CNDLST,NULL);
           }
           else
           {
             VBL_CNDLST = addchain (VBL_CNDLST,(void*)$1.NAME);
           }

           VBL_NUMCHOICE = 1;
         }
         ..Bar__choice..
         { yyerrok; }
       ;

null_statement
        : _NULL
          Semicolon_ERR {}
        ;

..Bar__choice..
       : /*empty*/
       | ..Bar__choice..
         Bar
         choice
         {
           VBL_NUMCHOICE++;

           if ((!strcmp($3.NAME , "others"))  || (VBL_CNDLST->DATA == NULL))
              my_vbl_error (31,NULL);

           VBL_CNDLST = addchain (VBL_CNDLST ,(void*)$3.NAME);
           VBL_LINLST = addchain (VBL_LINLST ,(void *)VBL_LINNUM );
         }
       ;

choice
       : literal
         {
           char Buffer[ 512 ];

           Buffer[ 0 ] = '"';
           vbl_tobin( &Buffer[1],$1,0,VBL_CASSIZE );
           Buffer[ VBL_CASSIZE + 2 ] = '"';
           Buffer[ VBL_CASSIZE + 3 ] = '\0';

           $$.NAME  = namealloc( Buffer );
           $$.RIGHT = -1;
           $$.TYPE  = -1;
           $$.LEFT  = -1;
           $$.DYNAMIC = 0;
           $$.AGGREG  = 0;
         }

       | OTHERS
         {
           $$.RIGHT = -1;
           $$.TYPE  = -1;
           $$.LEFT  = -1;
           $$.DYNAMIC = 0;
           $$.AGGREG  = 0;
           $$.NAME  = namealloc("others");
         }
       | name
         {
           vexexpr        *VexCst;
           int             left;
           int             right;
           int             in_bound;
           int             out_bound;
           int             left_bnd;
           int             right_bnd;
           int             sig_conf;

           if ( ( $1.NAME[0] == '"' ) || ( $1.NAME[0] == '\'') )
           {
             $$.NAME  = $1.NAME;
             $$.LEFT  = -1;
             $$.RIGHT = -1;
             $$.TYPE  = -1;
             $$.DYNAMIC = 0;
             $$.AGGREG  = 0;
           }
           else
           {
             sig_conf = chktab (hshtab,$1.NAME,VBL_MODNAM,VBL_SYMDFN);
             VexCst   = (vexexpr *)chktab (hshtab,$1.NAME,VBL_MODNAM,VBL_PNTDFN);

             if (sig_conf == 0)
             {
               my_vbl_error (17,$1.NAME);
             }
             else
             if (sig_conf != VBL_CSTDFN)
             {
               my_vbl_error (76,$1.NAME);
             }

             left_bnd  = chktab (hshtab,$1.NAME,VBL_MODNAM,VBL_WMNDFN);
             right_bnd = chktab (hshtab,$1.NAME,VBL_MODNAM,VBL_WMXDFN);

             left  = $1.LEFT;
             right = $1.RIGHT;

             if (left_bnd <= right_bnd)
             {
               if (left <= right)
               {
                 in_bound = left - left_bnd;
                 out_bound = right - left_bnd;
                 if ((left < left_bnd) || (right > right_bnd))
                 {
                   my_vbl_error (36,$1.NAME);
                 }
               }
               else
               {
                 in_bound = left - right_bnd;
                 out_bound = right - right_bnd;
                 if ((left > right_bnd) || (right < left_bnd))
                 {
                   my_vbl_error (36,$1.NAME);
                 }
               }
             }
             else
             {
               if (left <= right)
               {
                 in_bound = right - left_bnd;
                 out_bound = left - left_bnd;
                 if ((left < right_bnd) || (right > left_bnd))
                 {
                   my_vbl_error (36,$1.NAME);
                 }
               }
               else
               {
                 in_bound = right - right_bnd;
                 out_bound = left - right_bnd;
                 if ((left > left_bnd) || (right < right_bnd))
                 {
                   my_vbl_error (36,$1.NAME);
                 }
               }
             }

             $$.NAME = GetVexAtomValue( VexCst );
             $$.LEFT = -1;
             $$.RIGHT = -1;
             $$.TYPE = -1;
             $$.DYNAMIC = 0;
             $$.AGGREG  = 0;
           }
         }
         ;

condition
        : expression
                { $$ = $1; }
        ;

.REPORT__expression.
       :  /*empty*/
        { $$ = NULL;}
       | REPORT
         report__message
         { $$ = $2;}
       ;

.SEVERITY__expression.
       : /*empty*/
         { $$ = 'E';}
       | SEVERITY
         severity__message
         { $$ = $1;}
       ;

report__message
       : StringLit
         { $$ = $1; }
       ;

severity__message
       : ERROR
         { $$ = 'E';}
       | WARNING
         { $$ = 'W';}
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
                { $$ = vbl_crtvex (VEX_AND ,$1 ,$3 ,-1,-1,0); }
       | relation..AND__relation..
         _AND
         relation
                { $$ = vbl_crtvex (VEX_AND ,$1 ,$3 ,-1,-1,0);}
       ;

relation..OR__relation..
       : relation
         _OR
         relation
                { $$ = vbl_crtvex (VEX_OR ,$1 ,$3 ,-1,-1,0); }
       | relation..OR__relation..
         _OR
         relation
                { $$ = vbl_crtvex (VEX_OR ,$1 ,$3 ,-1,-1,0); }
       ;

relation.NAND_NOR_NXOR_relation.
       : relation
                { $$ = $1; }
       | relation
         _NAND
         relation
                { $$ = vbl_crtvex (VEX_NAND ,$1 ,$3 ,-1,-1,0); }
       | relation
         _NOR
         relation
                { $$ = vbl_crtvex (VEX_NOR ,$1 ,$3 ,-1,-1,0); }
       | relation
         _NXOR
         relation
                { $$ = vbl_crtvex (VEX_NXOR ,$1 ,$3 ,-1,-1,0); }
       | relation
         _IFT
         relation
                { $$ = vbl_crtvex (VEX_IFT,$1 ,$3 ,-1,-1,0); }
       ;

relation..XOR__relation..
       : relation
         _XOR
         relation
                { $$ = vbl_crtvex (VEX_XOR ,$1 ,$3 ,-1,-1,0); }
       | relation..XOR__relation..
         _XOR
         relation
                { $$ = vbl_crtvex (VEX_XOR ,$1 ,$3 ,-1,-1,0); }
       ;

relation
       : simple_expression
                { $$ = $1; }
       | simple_expression
         relational_operator
         simple_expression
                { $$ = vbl_crtvex ($2 ,$1 ,$3 ,-1,-1,0); }
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
                { if ( $1 == VEX_NEG )
                  {
                    $$ = vbl_crtvex( VEX_NEG,$2,NULL,-1,-1,0);
                  }
                  else
                  {
                    $$ = $2;
                  }
                }
        | .sign.term..add_op__term..
          adding_operator
          term
                { $$ = vbl_crtvex($2,$1,$3,-1,-1,0); }
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
                { $$ = vbl_crtvex( $2, $1, $3,-1,-1,0); }
       ;

factor
       : primary
         { $$ = $1; }
       | primary
         DoubleStar
         primary
              { $$ = vbl_crtvex( VEX_EXP,$1,$3,-1,-1,0); }
       | ABS
	 primary
              { 
                 $$ = vbl_crtvex( VEX_ABS,$2,VBL_EMPSTR,-1,-1,0); 
              }
       | _NOT
         primary
              { $$ = vbl_crtvex( VEX_NOT,$2,VBL_EMPSTR,-1,-1,0); }
       ;

primary
       : literal
        {
           struct vbl_expr expr1;

           expr1.IDENT = $1;
           expr1.TYPE  = -1;
           expr1.VEX   = (vexexpr *)0;

           $$ = vbl_crtvex( NOPS,expr1,VBL_EMPSTR,-1,-1,0);
        }
        | aggregate
        {
            vbl_vexstr *VexStr;
            chain_list *ScanChain;

            ScanChain = $1;

            if ( ScanChain == (chain_list *)0 )
            {
              my_vbl_error(114,NULL);
            }

            if ( ScanChain->NEXT == (chain_list *)0 )
            {
              VexStr = (vbl_vexstr *)ScanChain->DATA;

              $$ = *VexStr;

              autfreeheap( VexStr, sizeof( vbl_vexstr ) );
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
          struct vbl_expr expr1;
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
          unsigned char   dynamic;

          flag = $1.FLAG;

          if ( ( flag != 0          ) &&
               ( flag != VBL_EVENT  ) &&
               ( flag != VBL_STABLE ) )
          {
            if ( ( flag != VBL_RANGE     ) &&
                 ( flag != VBL_REV_RANGE ) )
            {
              if ( ! $1.DYNAMIC )
              {
                sprintf( Buffer, "%ld", $1.LEFT );

                expr1.IDENT = Buffer;
                expr1.VEX   = (vexexpr *)0;
                expr1.TYPE  = -1;

                $$ = vbl_crtvex( NOPS, expr1, VBL_EMPSTR, -1, -1,0);
              }
              else
              {
                $$.IDENT = (char *)0;
                $$.VEX   = (vexexpr *)$1.LEFT;
                $$.TYPE  = -1;
                $$.SIGNED = 0;
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

            $$ = vbl_crtvex( NOPS ,expr1, VBL_EMPSTR, -1, -1,0); /* as for literal */
          }
          else
          {
            if  ( ( mode = chktab(hshtab,$1.NAME,VBL_FUNCNAM,VBL_SYMDFN ) ) != 0 )
            {
              LocalName = VBL_FUNCNAM;
            }
            else
            if  ( ( mode = chktab(hshtab,$1.NAME,VBL_PROCNAM,VBL_SYMDFN ) ) != 0 )
            {
              LocalName = VBL_PROCNAM;
            }
            else
            if ( ( VBL_COMPNAM != (char *)0 )  &&
                 ( ( mode = chktab(hshtab,$1.NAME,VBL_COMPNAM,VBL_SYMDFN ) ) != 0 ) )
            {
              LocalName = VBL_COMPNAM;
            }
            else
            {
              LocalName = VBL_MODNAM;
              mode = chktab (hshtab,$1.NAME,LocalName,VBL_SYMDFN);
            }

            /*\  Skipped for out argument parameter
            if ( mode == VBL_OCNDFN )
            {
              my_vbl_error (26,$1.NAME);
            }
            \*/

            /* LUDO ICI
            if( mode == 0 || mode == VBL_TPEDFN || mode == VBL_PRODFN )
            */
            if( mode == 0 || mode == VBL_PRODFN )
            {
              my_vbl_error (17,$1.NAME);
            }

            left_bnd  = chktab (hshtab,$1.NAME,LocalName,VBL_WMNDFN);
            right_bnd = chktab (hshtab,$1.NAME,LocalName,VBL_WMXDFN);
            dynamic   = chktab (hshtab,$1.NAME,LocalName,VBL_DYNDFN);
              
            if ( ( ! $1.DYNAMIC ) &&
                 ( ! dynamic    ) )
            {
              left  = $1.LEFT;
              right = $1.RIGHT;

              if ( ! dynamic )
              {
                if ( VBL_COMPNAM == (char *)0 )
                {
                  if ( left_bnd <= right_bnd )
                  {
                    if (left <= right)
                    {
                      in_bound  = left  - left_bnd;
                      out_bound = right - left_bnd;
                      if ((left < left_bnd) || (right > right_bnd))
                      {
                        my_vbl_error (36,$1.NAME);
                      }
                    }
                    else
                    {
                      in_bound  = right - left_bnd;
                      out_bound = left  - left_bnd;
                      if ((left > right_bnd) || (right < left_bnd))
                      {
                        my_vbl_error (36,$1.NAME);
                      }
                    }
                  }
                  else
                  {
                    if (left <= right)
                    {
                      in_bound  = left  - right_bnd;
                      out_bound = right - right_bnd;
                      if ((left < right_bnd) || (right > left_bnd))
                      {
                        my_vbl_error (36,$1.NAME);
                      }
                    }
                    else
                    {
                      in_bound  = right - right_bnd;
                      out_bound = left  - right_bnd;
                      if ((left > left_bnd) || (right < right_bnd))
                      {
                        my_vbl_error (36,$1.NAME);
                      }
                    }
                  }
                }
              }
            }
            else
            {
              left  = $1.LEFT;
              right = $1.RIGHT;
            }

/*\
            if (mode == VBL_CSTDFN)
            {
              expr1.VEX   = (vexexpr *)chktab(hshtab,$1.NAME,LocalName,VBL_PNTDFN);
              expr1.IDENT = (char *)expr1.VEX->VALUE;
              expr1.TYPE  = chktab(hshtab,$1.NAME,LocalName,VBL_TYPDFN);
              expr1.SIGNED = chktab (hshtab,$1.NAME,LocalName,VBL_SUNDFN);

              $$ = vbl_crtvex (NOPS,expr1,VBL_EMPSTR,in_bound,out_bound,0);
            }
            else
\*/
            {
              int  type;

              expr1.IDENT = $1.NAME;
              type = chktab(hshtab,$1.NAME,LocalName,VBL_TYPDFN);
              expr1.TYPE = type;
              expr1.SIGNED = chktab (hshtab,$1.NAME,LocalName,VBL_SUNDFN);

              if ( $1.DYNAMIC )
              {
                if ( ( mode == VBL_FUNDFN ) ||
                     ( mode == VBL_TPEDFN ) ||
                     ( mode == VBL_PRODFN ) )
                {
                  expr1.VEX = (vexexpr *)left;
                  expr1 = vbl_crtvex(SUBVAR,expr1,VBL_EMPSTR,expr1.VEX->LEFT,expr1.VEX->RIGHT,0);
                }
                else
                {
                  expr1 = vbl_crtvex (NOPI,expr1,VBL_EMPSTR,left_bnd,right_bnd,dynamic);

                  if ( $1.DYNAMIC == VBL_UPTDFN ) 
                  {
                    if ( ( ! dynamic                                                  ) ||
                         ( ! isvexequalexpr( (vexexpr *)left_bnd, (vexexpr *)left   ) ) ||
                         ( ! isvexequalexpr( (vexexpr *)right_bnd, (vexexpr *)right ) ) )
                    {
                      expr1 = vbl_crtvex( VEX_TO, expr1,VBL_EMPSTR,left,right,0);
                    }
                  }
                  else
                  if ( $1.DYNAMIC == VBL_DWTDFN )
                  {
                    if ( ( ! dynamic                                                  ) ||
                         ( ! isvexequalexpr( (vexexpr *)left_bnd, (vexexpr *)left   ) ) ||
                         ( ! isvexequalexpr( (vexexpr *)right_bnd, (vexexpr *)right ) ) )
                    {
                      expr1 = vbl_crtvex( VEX_DOWNTO, expr1,VBL_EMPSTR,left,right,0);
                    }
                  }
                  else
                  {
                    expr1.VEX = 
                      createvexbinexpr( VEX_INDEX_N, 1, expr1.VEX, (vexexpr *)left );
                    expr1.VEX = 
                      createvexbinexpr( VEX_INDEX_N, 1, expr1.VEX, (vexexpr *)right );
                  }
                }
              }
              else
              {
                expr1 = vbl_crtvex (NOPI,expr1,VBL_EMPSTR,left,right,0);
              }

              if( flag == VBL_EVENT )
              {
                $$ = vbl_crtvex (VEX_EVENT,expr1,VBL_EMPSTR,left,right,0);
              }
              else
              if( flag == VBL_STABLE )
              {
                expr1 = vbl_crtvex(VEX_EVENT,expr1,VBL_EMPSTR,left,right,0);
                $$    = vbl_crtvex (VEX_NOT,expr1,VBL_EMPSTR,-1,-1,0);
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
          struct vbl_expr expr1;

          expr1.IDENT  = $1.NAME;
          expr1.TYPE   = $1.TYPE;
          expr1.SIGNED = $1.SIGNED;
          expr1.VEX    = (vexexpr *)$1.LEFT;

          expr1 = vbl_crtvex(SUBVAR,expr1,VBL_EMPSTR,expr1.VEX->LEFT,expr1.VEX->RIGHT,0);

          $$ = expr1;
        }
        ;

qualified_expression
        : name
          Apostrophe
          aggregate
          {
            char       *LocalName;
            vbl_vexstr *VexStr;
            chain_list *ScanChain;
            vexexpr    *VexExpr;
            long        Def;

            ScanChain = $3;

            if ( ( ScanChain       == (chain_list *)0 ) ||
                 ( ScanChain->NEXT != (chain_list *)0 ) )
            {
              my_vbl_error(122,NULL);
            }

            if  ( ( Def = chktab(hshtab,$1.NAME,VBL_FUNCNAM,VBL_SYMDFN ) ) != 0 )
            {
              LocalName = VBL_FUNCNAM;
            }
            else
            if  ( ( Def = chktab(hshtab,$1.NAME,VBL_PROCNAM,VBL_SYMDFN ) ) != 0 )
            {
              LocalName = VBL_PROCNAM;
            }
            else
            {
              LocalName = VBL_MODNAM;
              Def = chktab (hshtab,$1.NAME,LocalName,VBL_SYMDFN);
            }

            if ( Def == VBL_TPEDFN )
            {
              VexStr = (vbl_vexstr *)ScanChain->DATA;

              VexExpr = createvexfunc( $1.NAME, VexStr->VEX->WIDTH );
              addvexhexpr( VexExpr, VexStr->VEX );

              autfreeheap( VexStr, sizeof( vbl_vexstr ) );

              $$.NAME    = $1.NAME;
              $$.TYPE    = chktab (hshtab,$1.NAME,LocalName,VBL_TYPDFN);
              $$.SIGNED  = chktab (hshtab,$1.NAME,LocalName,VBL_SUNDFN);
              $$.FLAG    = 0;
              $$.LEFT    = (long)VexExpr;
              $$.RIGHT   = 0;
              $$.DYNAMIC = 1;
              $$.AGGREG  = 0;

              freechain( $3 );
            }
            else
            {
              my_vbl_error(122, $1.NAME);
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
            vbl_vexstr *VexStr;

            VexStr  = (vbl_vexstr *)autallocheap( sizeof( vbl_vexstr ) );
            *VexStr = $1;

            $$ = addchain( (chain_list *)0, VexStr );
          }
        | OTHERS
          Arrow
          expression
          {
            vbl_vexstr *VexStr;

            VexStr  = (vbl_vexstr *)autallocheap( sizeof( vbl_vexstr ) );
            *VexStr = vbl_crtvex( VEX_OTHERS, $3,VBL_EMPSTR,-1,-1,0);

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

           if ( chktab(hshtab,$1,VBL_FUNCNAM,VBL_SYMDFN ) )
           {
             LocalName = VBL_FUNCNAM;
           }
           else
           if ( chktab(hshtab,$1,VBL_PROCNAM,VBL_SYMDFN ) )
           {
             LocalName = VBL_PROCNAM;
           }
           else
           if ( ( VBL_COMPNAM != (char *)0 ) &&
                ( chktab(hshtab,$1,VBL_COMPNAM,VBL_SYMDFN ) ) )
           {
             LocalName = VBL_COMPNAM;
           }
           else
           {
             LocalName = VBL_MODNAM;
           }

           $$.NAME = $1;
           $$.FLAG = 0;

           sprintf ( buffer, "%s.cod.%s", LocalName, $1);
           codedsigname = namealloc( buffer );
           $$.TYPE = chktab (hshtab,codedsigname,LocalName,VBL_TYPDFN);

           if ( $$.TYPE >= VBH_MAX_TYPE )
           {
             if ((valbitstr = searchauthelem(VBL_ENUMVAL, codedsigname )) != NULL)
             {
               $$.NAME   = namealloc((char *)valbitstr->VALUE);
               $$.TYPE   = VBH_TYPE_BIT_VECTOR ; /* BIT_VECTOR intentionnel */
               $$.LEFT   = -1;
               $$.RIGHT  = -1;
               $$.DYNAMIC = 0;
               $$.AGGREG  = 0;
               $$.SIGNED  = 0;
             }
             else
             {
               $$.TYPE   = VBH_TYPE_BIT_VECTOR ; /* BIT_VECTOR intentionnel */
               $$.LEFT   = chktab (hshtab,$1,LocalName,VBL_WMNDFN);
               $$.RIGHT  = chktab (hshtab,$1,LocalName,VBL_WMXDFN);
               $$.DYNAMIC = chktab(hshtab,$1,LocalName,VBL_DYNDFN);
               $$.AGGREG  = 0;
               $$.SIGNED = 0;
             }
           }
           else
           {
             $$.TYPE   = chktab (hshtab,$1,LocalName,VBL_TYPDFN);
             $$.LEFT   = chktab (hshtab,$1,LocalName,VBL_WMNDFN);
             $$.RIGHT  = chktab (hshtab,$1,LocalName,VBL_WMXDFN);
             $$.SIGNED = chktab (hshtab,$1,LocalName,VBL_SUNDFN);
             $$.DYNAMIC = chktab(hshtab,$1,LocalName,VBL_DYNDFN);
             $$.AGGREG  = 0;

           }
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
            vbl_vexstr *VexStr;
            vbfun_list *VbFunc;
            chain_list *ScanChain;
            vexexpr    *VexExpr;
            vexexpr    *VexRet;
            long        Index;
            int         Error;
            long        Def;

            ScanChain = $2;

            if ( ScanChain == (chain_list *)0 )
            {
              my_vbl_error(114,NULL);
            }

            if  ( ( Def = chktab(hshtab,$1,VBL_FUNCNAM,VBL_SYMDFN ) ) != 0 )
            {
              LocalName = VBL_FUNCNAM;
            }
            else
            if  ( ( Def = chktab(hshtab,$1,VBL_PROCNAM,VBL_SYMDFN ) ) != 0 )
            {
              LocalName = VBL_PROCNAM;
            }
            else
            if ( ( VBL_COMPNAM != (char *)0 ) &&
                 ( ( Def = chktab(hshtab,$1,VBL_COMPNAM,VBL_SYMDFN ) ) != 0 ) )
            {
              LocalName = VBL_COMPNAM;
            }
            else
            {
              LocalName = VBL_MODNAM;
              Def = chktab (hshtab,$1,LocalName,VBL_SYMDFN);
            }

            if ( ( Def == VBL_FUNDFN ) ||
                 ( Def == VBL_TPEDFN ) ||
                 ( Def == VBL_PRODFN ) )
            {
              if ( Def != VBL_TPEDFN )
              {
                VexExpr = createvexfunc( $1, 0 );
              }

              if ( Def == VBL_FUNDFN )
              {
                VbFunc = (vbfun_list *)chktab( hshtab,$1,LocalName,VBL_PNTDFN);

                if ( VbFunc != (vbfun_list *)0 )
                {
                  VexRet = VbFunc->RETURN->TARGET;

                  VexExpr->LEFT  = VexRet->LEFT;
                  VexExpr->RIGHT = VexRet->RIGHT;
                  VexExpr->WIDTH = VexRet->WIDTH;

                  if ( ! IsVexNodeVarWidth( VexRet ) )
                  {
                    ClearVexNodeVarWidth( VexExpr );
                  }
                }
              }

              if ( Def == VBL_TPEDFN )
              {
                if ( ScanChain->NEXT != (chain_list *)0 )
                {
                  my_vbl_error(114,NULL);
                }

                VexStr = (vbl_vexstr *)ScanChain->DATA;

                VexExpr = createvexfunc( $1, VexStr->VEX->WIDTH );
                addvexhexpr( VexExpr, VexStr->VEX );

                autfreeheap( VexStr, sizeof( vbl_vexstr ) );
              }
              else
              {
                while ( ScanChain != (chain_list *)0 )
                {
                  VexStr = (vbl_vexstr *)ScanChain->DATA;
                  addvexqexpr( VexExpr, VexStr->VEX );
  
                  autfreeheap( VexStr, sizeof( vbl_vexstr ) );
                  ScanChain = ScanChain->NEXT;
                }
              }
  
              $$.NAME    = $1;
              $$.TYPE    = chktab (hshtab,$1,LocalName,VBL_TYPDFN);
              $$.SIGNED  = chktab (hshtab,$1,LocalName,VBL_SUNDFN);
              $$.FLAG    = 0;
              $$.LEFT    = (long)VexExpr;
              $$.RIGHT   = -1;
              $$.DYNAMIC = 1;
              $$.AGGREG  = 0;

              freechain( $2 );
            }
            else
            if ( ScanChain->NEXT != (chain_list *)0 )
            {
              if ( ( ! Def                                    ) || 
                   ( ScanChain->NEXT->NEXT != (chain_list *)0 ) )
              {
                my_vbl_error(118,$1);
              }

              /* ICI */

              $$.NAME   = $1;
              $$.TYPE   = chktab (hshtab,$1,LocalName,VBL_TYPDFN);
              $$.SIGNED = chktab (hshtab,$1,LocalName,VBL_SUNDFN);
              $$.FLAG   = 0;

              $$.DYNAMIC = VBL_IDXDFN;
              $$.AGGREG  = 0;

              VexStr  = (vbl_vexstr *)ScanChain->DATA;
              $$.LEFT = (long)VexStr->VEX;
              autfreeheap( VexStr, sizeof( vbl_vexstr ) );

              ScanChain = ScanChain->NEXT;
              VexStr   = (vbl_vexstr *)ScanChain->DATA;
              $$.RIGHT = (long)VexStr->VEX;
              autfreeheap( VexStr, sizeof( vbl_vexstr ) );

              freechain( $2 );
            }
            else
            {
              VexStr = (vbl_vexstr *)ScanChain->DATA;
              Error  = vbl_vextonum( VexStr->VEX, &Index );

              $$.NAME   = $1;
              $$.TYPE   = chktab (hshtab,$1,LocalName,VBL_TYPDFN);
              $$.SIGNED = chktab (hshtab,$1,LocalName,VBL_SUNDFN);
              $$.FLAG   = 0;
              $$.AGGREG  = 0;

              if ( Error )
              {
                $$.LEFT    = (long)VexStr->VEX;
                $$.RIGHT   = 0;
                $$.DYNAMIC = VBL_UPTDFN;
              }
              else
              {
                $$.LEFT   = Index;
                $$.RIGHT  = Index;
                $$.DYNAMIC = 0;
              }

              autfreeheap( VexStr, sizeof( vbl_vexstr ) );
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

           if ( chktab(hshtab,$1,VBL_FUNCNAM,VBL_SYMDFN ) )
           {
             LocalName = VBL_FUNCNAM;
           }
           else
           if ( chktab(hshtab,$1,VBL_PROCNAM,VBL_SYMDFN ) )
           {
             LocalName = VBL_PROCNAM;
           }
           else
           if ( ( VBL_COMPNAM != (char *)0 ) &&
                ( chktab(hshtab,$1,VBL_COMPNAM,VBL_SYMDFN ) ) )
           {
             LocalName = VBL_COMPNAM;
           }
           else
           {
             LocalName = VBL_MODNAM;
           }

           ErrorL = vbl_vextonum( $3.VEX, &Left  );
           ErrorR = vbl_vextonum( $5.VEX, &Right );

           $$.NAME   = $1;
           $$.TYPE   = chktab (hshtab,$1,LocalName,VBL_TYPDFN);
           $$.SIGNED = chktab (hshtab,$1,LocalName,VBL_SUNDFN);
           $$.FLAG   = 0;
           $$.AGGREG = 0;

           if ( ( ! ErrorL ) && ( ! ErrorR ) )
           {
             if (((Left > Right) && ($4 == VBL_UPTDFN)) ||
                  ((Left < Right) && ($4 == VBL_DWTDFN)))
               my_vbl_error (32,$1);

             $$.LEFT   = Left;
             $$.RIGHT  = Right;
             $$.DYNAMIC = 0;
           }
           else
           {
             $$.LEFT    = (long)$3.VEX;
             $$.RIGHT   = (long)$5.VEX;
             $$.DYNAMIC = $4;
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
            unsigned char Dynamic;

            if  ( ( mode = chktab(hshtab,$1.NAME,VBL_FUNCNAM,VBL_SYMDFN ) ) != 0 )
            {
              LocalName = VBL_FUNCNAM;
            }
            else
            if  ( ( mode = chktab(hshtab,$1.NAME,VBL_PROCNAM,VBL_SYMDFN ) ) != 0 )
            {
              LocalName = VBL_PROCNAM;
            }
            else
            {
              LocalName = VBL_MODNAM;
              mode = chktab (hshtab,$1.NAME,LocalName,VBL_SYMDFN);
            }

            type = chktab (hshtab,$1.NAME,LocalName,VBL_TYPDFN);
            flag = $3;

            Dynamic = chktab(hshtab,$1.NAME,LocalName,VBL_DYNDFN);

            $$.NAME   = $1.NAME;
            $$.TYPE   = type;
            $$.LEFT   = -1;
            $$.RIGHT  = -1;
            $$.SIGNED = chktab (hshtab,$1.NAME,LocalName,VBL_SUNDFN);
            $$.FLAG   = $3;
            $$.DYNAMIC = Dynamic;

            if ( ( flag == VBL_STABLE ) ||
                 ( flag == VBL_EVENT  ) )
            {
              if ( LocalName != VBL_MODNAM )
                my_vbl_error (79,$1);
            }
            else
            {
              AttrLeft  = chktab( hshtab,$1.NAME,LocalName,VBL_ATLDFN);
              AttrRight = chktab( hshtab,$1.NAME,LocalName,VBL_ATRDFN);

              if ( ! Dynamic )
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
              else
              {
                if ( Dynamic == VBL_UPTDFN )
                {
                  AttrLow  = AttrLeft;
                  AttrHigh = AttrRight;
                }
                else
                {
                  AttrHigh = AttrLeft;
                  AttrLow  = AttrRight;
                }
              }

              switch ( flag )
              {
                case VBL_LEFT   : $$.LEFT = AttrLeft;
                break;

                case VBL_RIGHT  : $$.LEFT = AttrRight;
                break;

                case VBL_LOW    : $$.LEFT = AttrLow;
                break;

                case VBL_HIGH   : $$.LEFT = AttrHigh;
                break;

                case VBL_LENGTH : $$.LEFT = 1 + (AttrHigh - AttrLow);
                break;

                case VBL_RANGE  : $$.LEFT  = AttrLeft;
                                  $$.RIGHT = AttrRight;
                break;

                case VBL_REV_RANGE : $$.LEFT  = AttrRight;
                                     $$.RIGHT = AttrLeft;
                break;
              }
            }
          }
          ;

attribute_designator
        : _STABLE     { $$ = VBL_STABLE; }
        | _EVENT      { $$ = VBL_EVENT; }
        | _LEFT       { $$ = VBL_LEFT; }
        | _RIGHT      { $$ = VBL_RIGHT; }
        | _LOW        { $$ = VBL_LOW; }
        | _HIGH       { $$ = VBL_HIGH; }
        | _LENGTH     { $$ = VBL_LENGTH; }
        | _RANGE      { $$ = VBL_RANGE; }
        | _REV_RANGE  { $$ = VBL_REV_RANGE; }
        ;

identifier_list
       : Identifier
                { VBL_NM1LST = addchain (VBL_NM1LST,$1); }
         ...identifier..
       ;

...identifier..
       : /*empty*/
       | ...identifier..
         Comma
         Identifier
                { VBL_NM1LST = addchain (VBL_NM1LST,$3); }
       ;

.label.
       : /*empty*/
              { $$ = NULL; }
       | label
              { $$ = $1; }
       ;

.guard_expression.
        : /*empty*/
          { $$.VEX = (vexexpr *)0; }
        | guard_expression
          { $$ = $1; }
        ;

guard_expression
        : LeftParen
          expression
          RightParen_ERR
          { $$ = $2; }
        ;

.GUARDED.
        : /*empty*/
          {
            $$ = VBL_UNGDFN;
          }
        | GUARDED
          {
            $$ = VBL_GRDDFN;

            if ( VBL_GRDLST == (chain_list *)0 )
              my_vbl_error(41,0 );
          }
        ;

.simple_name.
       : /*empty*/
              { $$ = NULL; }
       | simple_name
              { $$ = $1; }
       ;

simple_name
       : Identifier
              { $$ = $1; }
       ;

target
       : name
              { $$ = $1; }
       | aggregate
         {
            $$.NAME    = (char *)$1;
            $$.LEFT    = 0;
            $$.RIGHT   = 0;
            $$.FLAG    = 0;
            $$.TYPE    = 0;
            $$.DYNAMIC = 0;
            $$.SIGNED  = 0;
            $$.AGGREG  = 1;
          }
       ;

a_label
       : label
         Colon
                {
                $$ = $1;
                }
       ;

label
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
  struct dct_entry *entry;
  int              i;

  if (VBL_DCEHED == NULL)
    {
    VBL_DCEHED = (struct dct_entry *)
                 autallocblock (sizeof(struct dct_entry) * VBL_ALODFN);

    entry = VBL_DCEHED;
    for (i=1 ; i<VBL_ALODFN ; i++)
      {
      entry->next = entry + 1;
      entry++;
      }
    entry->next = NULL;
    }

  entry       = VBL_DCEHED;
  VBL_DCEHED  = VBL_DCEHED->next;

  entry->next = head;
  entry->data = NULL;
  entry->key  = key;

  return (entry);
}

static struct dct_recrd *addrcd (head , key)

struct dct_recrd *head;
char             *key;

{
  struct dct_recrd *recrd;
  int               i;

  if (VBL_DCRHED == NULL)
    {
    VBL_DCRHED = (struct dct_recrd *)
                 autallocblock (sizeof(struct dct_recrd) * VBL_ALODFN);

    recrd = VBL_DCRHED;
    for (i=1 ; i<VBL_ALODFN ; i++)
      {
      recrd->next = recrd + 1;
      recrd++;
      }
    recrd->next = NULL;
    }

  recrd           = VBL_DCRHED;
  VBL_DCRHED      = VBL_DCRHED->next;

  recrd->next     = head;
  recrd->fd0_val  = 0;
  recrd->fd1_val  = 0;
  recrd->fd2_val  = 0;
  recrd->fd3_val  = 0;
  recrd->fd4_val  = 0;
  recrd->fd5_val  = 0;
  recrd->fd6_val  = 0;
  recrd->fd7_val  = 0;
  recrd->fd8_val  = 0;
  recrd->pnt_val  = 0;
  recrd->dyn_val  = 0;
  recrd->key      = key;

  return (recrd);
}

static struct dct_entry **initab ()

{
  struct dct_entry **head;
  int                i;

  head = (struct dct_entry **)
         autallocblock (sizeof(struct dct_entry *) * VBL_HSZDFN);

  for (i=0 ; i<VBL_HSZDFN ; i++)
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

  index     = ((unsigned long) key_str) % VBL_HSZDFN;
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
      case VBL_MODDFN :
        recrd_pnt->fd0_val = valu;
        break;
      case VBL_SYMDFN :
        recrd_pnt->fd1_val = valu;
        break;
      case VBL_TYPDFN :
        recrd_pnt->fd2_val = valu;
        break;
      case VBL_SUNDFN :
        recrd_pnt->fd3_val = valu;
        break;
      case VBL_LBLDFN :
        recrd_pnt->fd4_val = valu;
        break;
      case VBL_WMXDFN :
        recrd_pnt->fd5_val = valu;
        break;
      case VBL_WMNDFN :
        recrd_pnt->fd6_val = valu;
        break;
      case VBL_ATLDFN :
        recrd_pnt->fd7_val = valu;
        break;
      case VBL_ATRDFN :
        recrd_pnt->fd8_val = valu;
        break;
      case VBL_PNTDFN :
        recrd_pnt->pnt_val = valu;
        break;
      case VBL_DYNDFN :
        recrd_pnt->dyn_val = valu;
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

  entry_pnt = head [((unsigned long)key_str) % VBL_HSZDFN];

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
        case VBL_MODDFN :
          valu = recrd_pnt->fd0_val;
          break;
        case VBL_SYMDFN :
          valu = recrd_pnt->fd1_val;
          break;
        case VBL_TYPDFN :
          valu = recrd_pnt->fd2_val;
          break;
        case VBL_SUNDFN :
          valu = recrd_pnt->fd3_val;
          break;
        case VBL_LBLDFN :
          valu = recrd_pnt->fd4_val;
          break;
        case VBL_WMXDFN :
          valu = recrd_pnt->fd5_val;
          break;
        case VBL_WMNDFN :
          valu = recrd_pnt->fd6_val;
          break;
        case VBL_ATLDFN :
          valu = recrd_pnt->fd7_val;
          break;
        case VBL_ATRDFN :
          valu = recrd_pnt->fd8_val;
          break;
        case VBL_PNTDFN :
          valu = recrd_pnt->pnt_val;
          break;
        case VBL_DYNDFN :
          valu = recrd_pnt->dyn_val;
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
    for (i=0 ; i<VBL_HSZDFN ; i++)
      {
      if ((pt_entry = pt_hash[i]) != NULL)
        {
        while (pt_entry != NULL)
          {
          pt_record = pt_entry->data;

          while (pt_record->next != NULL)
            pt_record = pt_record->next;

          pt_record->next = VBL_DCRHED;
          VBL_DCRHED      = pt_entry->data;

          pt_nxtentry     = pt_entry->next;
          pt_entry->next  = VBL_DCEHED;
          VBL_DCEHED      = pt_entry;
          pt_entry        = pt_nxtentry;
          }
        }
      }
    autfreeblock(pt_hash);
    }
}

static void *vbl_addstr(object,mode,prtype,type,flag,name,left,right,exp,kind,dynamic)

char          object;
int           mode;
vbtyp_list   *prtype;
unsigned char type;
char          flag;
char         *name;
long          left;
long          right;
vexexpr      *exp;
unsigned char kind;
unsigned char dynamic;

{
  ptype_list *GenVar;
  void  *pnt    = NULL;
  char   porflg = 0;
  char   modflg = 0;
  char   auxflg = 0;
  char   cstflg = 0;
  char   varflg = 0;
  char   funflg = 0;
  char   conflg = 0;
  char   parflg = 0;
  char   genflg = 0;
  char   gnrflg = 0;
  char genmodflg = 0;
  char   lclmod = 'X';
  vexexpr *vex_pnt;
  int      bitsize;

  switch (object)
  {
    case 'M':

       /* ###------------------------------------------------------### */
       /*   if object is a model port ...                                   */
       /* ###------------------------------------------------------### */

      modflg = 1;
      switch (mode)
      {
        case VBL_ICNDFN:
          lclmod = 'I'; break;
        case VBL_OCNDFN:
          lclmod = 'O'; break;
        case VBL_BCNDFN:
          lclmod = 'B'; break;
      }
      break;

    case 'P':

       /* ###------------------------------------------------------### */
       /*   if object is a port ...                                   */
       /* ###------------------------------------------------------### */

      porflg = 1;
      switch (mode)
      {
        case VBL_ICNDFN:
          lclmod = 'I'; break;
        case VBL_OCNDFN:
          lclmod = 'O'; break;
        case VBL_BCNDFN:
          lclmod = 'B'; break;
      }
      break;

    case 'p':

       /* ###------------------------------------------------------### */
       /*   if object is a parameter of a function / procedure       */
       /* ###------------------------------------------------------### */

      parflg = 1;
      switch (mode)
      {
        case VBL_ICNDFN:
          lclmod = 'I'; break;
        case VBL_OCNDFN:
          lclmod = 'O'; break;
        case VBL_BCNDFN:
          lclmod = 'B'; break;
      }
      break;

    case 'S':

       /* ###------------------------------------------------------### */
       /*   if object is a signal ...                                   */
       /* ###------------------------------------------------------### */

        auxflg = 1;

      break;

    case 'v':

        /* ###------------------------------------------------------### */
        /*   if object is a variable in function/procedure              */
        /* ###------------------------------------------------------### */

        funflg = 1;

      break;

    case 'V':

        /* ###------------------------------------------------------### */
        /*   if object is a variable ...                                */
        /* ###------------------------------------------------------### */

        varflg = 1;

      break;

    case 'C':

        /* ###------------------------------------------------------### */
        /*   if object is a constant ...                                */
        /* ###------------------------------------------------------### */

        cstflg = 1;

      break;

    case 'G':

        /* ###------------------------------------------------------### */
        /*   if object is a generic ...                                */
        /* ###------------------------------------------------------### */

        genflg = 1;

      break;

    case 'f':

        /* ###------------------------------------------------------### */
        /*   if object is a for generate variable ...                   */
        /* ###------------------------------------------------------### */

        gnrflg = 1;

      break;

    case 'g':

        /* ###------------------------------------------------------### */
        /*   if object is a generic map ...                                */
        /* ###------------------------------------------------------### */

        genmodflg = 1;

      break;

    case 'F':

        /* ###------------------------------------------------------### */
        /*   if object is a function ...                                */
        /* ###------------------------------------------------------### */

        conflg = 1;

      break;
  }

  if ( (flag == 'A') ||
       (flag == 'U') )
  {
       /* ###------------------------------------------------------### */
       /*   if object is an array ...                                   */
       /* ###------------------------------------------------------### */

/* Original 
    vex_pnt = createvexatomvec( name, left, right );
*/

/* Correction */
    if ( ( flag == 'A' ) && ( ! dynamic ) )
    {
      vex_pnt = createvexatomvec( name, prtype->LEFT, prtype->RIGHT ); 
    }
    else
    {
      if ( dynamic )
      {
        if ( dynamic == VBL_UPTDFN ) vex_pnt = createvexoper( VEX_TO    , 0 );
        else                         vex_pnt = createvexoper( VEX_DOWNTO, 0 );

        addvexqexpr( vex_pnt, createvexatomvec( name, -1, -1 ) );
        addvexqexpr( vex_pnt, dupvexexpr( (vexexpr *)left  ) );
        addvexqexpr( vex_pnt, dupvexexpr( (vexexpr *)right ) );
      }
      else
      {
        vex_pnt = createvexatomvec( name, left, right );
      }
    }
/* Fin Correction */ 

    if ( prtype->INDEX == VBH_TYPE_SIGNED )
    {
      SetVexNodeSigned( vex_pnt );
    }

    if ( auxflg == 1 )
      VBL_BEFPNT->BEAUX = vbh_addvbaux(VBL_BEFPNT->BEAUX, vex_pnt,exp,kind,prtype,
                                       VBL_LINNUM );
    if ( cstflg == 1 )
      VBL_BEFPNT->BECST = vbh_addvbcst(VBL_BEFPNT->BECST, vex_pnt,exp,prtype,
                                       VBL_LINNUM );

    if ( genflg == 1 )
      VBL_BEFPNT->BEGEN = vbh_addvbgen(VBL_BEFPNT->BEGEN, vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (porflg == 1)
      VBL_BEFPNT->BEPOR = vbh_addvbpor(VBL_BEFPNT->BEPOR, vex_pnt,exp,lclmod,kind,prtype,
                                       VBL_LINNUM );

    if (modflg == 1)
      VBL_BEFPNT->BEMOD->BEPOR = 
         vbh_addvbpor(VBL_BEFPNT->BEMOD->BEPOR, vex_pnt,exp,lclmod,kind,prtype,
                                       VBL_LINNUM );
    if (genmodflg == 1)
      VBL_BEFPNT->BEMOD->BEGEN = 
         vbh_addvbgen(VBL_BEFPNT->BEMOD->BEGEN, vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (parflg == 1)
      VBL_BEFUN->ARGUMENT = vbh_addvbarg(VBL_BEFUN->ARGUMENT,vex_pnt,lclmod,kind,prtype,
                                         VBL_LINNUM );
    if (varflg == 1)
      VBL_BEPCS->VARIABLE = vbh_addvbvar(VBL_BEPCS->VARIABLE,vex_pnt,exp,prtype,
                                         VBL_LINNUM );
    if (funflg == 1)
      VBL_BEFUN->VARIABLE = vbh_addvbvar(VBL_BEFUN->VARIABLE,vex_pnt,exp,prtype,
                                         VBL_LINNUM );
    if (gnrflg == 1)
      GenVar = vbh_addvbvar( (vbvar_list *)0,vex_pnt,exp,prtype, VBL_LINNUM );
  }
  else
  if ( (flag == 'I') )
  {
       /* ###------------------------------------------------------### */
       /*   if object is an integer ...                            */
       /* ###------------------------------------------------------### */

      if ( dynamic )
      {
        vexexpr *new_left;
        vexexpr *new_right;

        vex_pnt = createvexoper( VEX_NUM_BIT, 0 );
        addvexqexpr( vex_pnt, dupvexexpr( (vexexpr *)left )  );
        addvexqexpr( vex_pnt, dupvexexpr( (vexexpr *)right ) );

        new_left  = (vexexpr *)vex_pnt;
        new_right = (vexexpr *)createvexatomlit( VEX_ATOM_ZERO );

        vex_pnt = createvexternaryexpr(
            VEX_DOWNTO, -1, createvexatomvec( name, -1, -1 ), 
             dupvexexpr( (vexexpr *)new_left ), dupvexexpr( (vexexpr *)new_right ) );

        /* It's not clean but ... */
        copyvexnode( (vexexpr *)left, new_left );
        copyvexnode( (vexexpr *)right, new_right );
      }
      else
      {
        if ((left == -1 ) && (right == -1))
        {
          left  = prtype->LEFT;
          right = prtype->RIGHT;
        }

        bitsize = vbl_intSize( left, right );

        vex_pnt = createvexatomvec( name, bitsize - 1, 0 );

        if ( ( left  < 0 ) ||
             ( right < 0 ) )
        {
          SetVexNodeSigned( vex_pnt );
        }
      }

    if ( auxflg == 1 )
      VBL_BEFPNT->BEAUX = vbh_addvbaux(VBL_BEFPNT->BEAUX, vex_pnt,exp,kind,prtype,
                                       VBL_LINNUM );
    if ( cstflg == 1 )
      VBL_BEFPNT->BECST = vbh_addvbcst(VBL_BEFPNT->BECST, vex_pnt,exp,prtype,
                                       VBL_LINNUM );

    if ( genflg == 1 )
      VBL_BEFPNT->BEGEN = vbh_addvbgen(VBL_BEFPNT->BEGEN, vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (porflg == 1)
      VBL_BEFPNT->BEPOR = vbh_addvbpor(VBL_BEFPNT->BEPOR, vex_pnt,exp,lclmod,kind,prtype,
                                       VBL_LINNUM );
    if (modflg == 1)
      VBL_BEFPNT->BEMOD->BEPOR = 
        vbh_addvbpor(VBL_BEFPNT->BEMOD->BEPOR, vex_pnt,exp,lclmod,kind,prtype,
                                       VBL_LINNUM );
    if (genmodflg == 1)
      VBL_BEFPNT->BEMOD->BEGEN = 
         vbh_addvbgen(VBL_BEFPNT->BEMOD->BEGEN, vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (parflg == 1)
      VBL_BEFUN->ARGUMENT = vbh_addvbarg(VBL_BEFUN->ARGUMENT, vex_pnt, lclmod,kind,prtype,
                                       VBL_LINNUM );
    if (varflg == 1)
      VBL_BEPCS->VARIABLE = vbh_addvbvar(VBL_BEPCS->VARIABLE,vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (funflg == 1)
      VBL_BEFUN->VARIABLE = vbh_addvbvar(VBL_BEFUN->VARIABLE,vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (gnrflg == 1)
      GenVar = vbh_addvbvar( (vbvar_list *)0,vex_pnt,exp,prtype, VBL_LINNUM );
  }
  else
  {
    vex_pnt = createvexatombit( name );
       /* ###------------------------------------------------------### */
       /*   if object is a scalar ...                                   */
       /* ###------------------------------------------------------### */

    if (porflg == 1)
      VBL_BEFPNT->BEPOR = vbh_addvbpor (VBL_BEFPNT->BEPOR,vex_pnt,exp,lclmod,kind,prtype,
                                       VBL_LINNUM );
    if (modflg == 1)
      VBL_BEFPNT->BEMOD->BEPOR = 
        vbh_addvbpor (VBL_BEFPNT->BEMOD->BEPOR,vex_pnt,exp,lclmod,kind,prtype,
                                       VBL_LINNUM );
    if (genmodflg == 1)
      VBL_BEFPNT->BEMOD->BEGEN = 
         vbh_addvbgen(VBL_BEFPNT->BEMOD->BEGEN, vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (parflg == 1)
      VBL_BEFUN->ARGUMENT = vbh_addvbarg(VBL_BEFUN->ARGUMENT,vex_pnt,lclmod,kind,prtype,
                                       VBL_LINNUM );
    if (auxflg == 1)
      VBL_BEFPNT->BEAUX = vbh_addvbaux (VBL_BEFPNT->BEAUX,vex_pnt,exp,kind,prtype,
                                       VBL_LINNUM );
    if (cstflg == 1)
      VBL_BEFPNT->BECST = vbh_addvbcst (VBL_BEFPNT->BECST,vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (genflg == 1)
      VBL_BEFPNT->BEGEN = vbh_addvbgen (VBL_BEFPNT->BEGEN,vex_pnt,exp,prtype,
                                       VBL_LINNUM );
    if (varflg == 1)
      VBL_BEPCS->VARIABLE = vbh_addvbvar(VBL_BEPCS->VARIABLE,vex_pnt, exp, prtype,
                                       VBL_LINNUM  );
    if (funflg == 1)
      VBL_BEFUN->VARIABLE = vbh_addvbvar(VBL_BEFUN->VARIABLE,vex_pnt, exp, prtype,
                                       VBL_LINNUM  );
    if (gnrflg == 1)
      GenVar = vbh_addvbvar( (vbvar_list *)0,vex_pnt,exp,prtype, VBL_LINNUM );
  }

  if (auxflg == 1)
    pnt = (void *) VBL_BEFPNT->BEAUX;
  if (cstflg == 1)
    pnt = (void *) VBL_BEFPNT->BECST;
  if (genflg == 1)
    pnt = (void *) VBL_BEFPNT->BEGEN;
  if (varflg == 1)
    pnt = (void *) VBL_BEPCS->VARIABLE->DATA;
  if (gnrflg == 1)
  {
    pnt = (void *) GenVar->DATA;
    freeptype( GenVar );
  }
  if (funflg == 1)
    pnt = (void *) VBL_BEFUN->VARIABLE->DATA;
  if (porflg == 1)
    pnt = (void *) VBL_BEFPNT->BEPOR;
  if (modflg == 1)
    pnt = (void *) VBL_BEFPNT->BEMOD->BEPOR;
  if (genmodflg == 1)
    pnt = (void *) VBL_BEFPNT->BEMOD->BEGEN;
  if (parflg == 1)
    pnt = (void *) VBL_BEFUN->ARGUMENT;
  if (conflg == 1)
    pnt = (void *) vex_pnt;

  return (pnt);
}

static vbtyp_list *val_type(name)

 char *name;
{
  return( vbh_getvbtyp( VBL_HEADFIG, name ) );
}

static vbtyp_list *get_type(val)

  int val;
{
  vbtyp_list *Type;

  for ( Type  = VBL_BEFPNT->BETYP;
        Type != (vbtyp_list *)0;
        Type  = Type->NEXT )
  {
    if  ( Type->INDEX == val ) break;
  }

  return( Type );
}

static struct ptype *reversetype(pnt)
 struct ptype *pnt;
{
  return( (ptype_list *)reverse( (chain_list *)pnt ) );
# if 0
    struct ptype* revtype;
    struct ptype* aux2;
    struct ptype* aux1;

    if(pnt == NULL || pnt->NEXT == NULL)
       return(pnt);
                     revtype = pnt;
                   aux1 = pnt->NEXT;
                   revtype->NEXT = NULL;
                while(aux1)
                 {
                   aux2 = revtype;
                   revtype = aux1;
                   aux1 = aux1->NEXT;
                   revtype->NEXT = aux2;
                 }
       return(revtype);
# endif
}
