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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/* ###--------------------------------------------------------------### */
/*                                                                      */
/* file     : fbl_bcomp.yac                                             */
/* date     : NOV 5 1992                                                */
/* version  : v100                                                      */
/* author   : P. BAZARGAN, M. HANAFI                                    */
/*            Modified by Ludovic Jacome                                */
/* content  : yacc rules for fbhavioural VHDL                           */
/*                                                                      */
/* ###--------------------------------------------------------------### */

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "fvh.h"

#include "fbl_utype.h"
#include "fbl_bedef.h"
#include "fbl_bspec.h"
#include "fbl_byacc.h"

static struct dct_entry *addent ();
static struct dct_recrd *addrcd ();
static struct dct_entry **initab ();
static void addtab ();
static long chktab ();
static void fretab ();
void *fbl_addstr ();
long fbl_chkdcl ();
struct ptype *reversetype();
struct choice_chain *order_choice();
%}

%union  {
	long		 valu;
	char		*text;
	struct ptype	*ptyp;
	struct fbpcs	proc;
        struct g_type dble;
        fbl_name  	name;   
	fbl_ablstr	list;
	struct fbtyp 	type;
	};

%token	<valu> tok_AND
%token	<valu> Pragma
%token	<valu> _BEGIN
%token	<valu> _END
%token	<valu> _EQSym
%token	<valu> _EXIT
%token  <valu> _FILE
%token	<valu> _GESym
%token	<valu> _GTSym
%token	<valu> _IN
%token	<valu> _INOUT
%token	<valu> _LABEL
%token	<valu> _LESym
%token  <valu> _LINKAGE
%token	<valu> _LTSym
%token	<valu> _NAND
%token	<valu> _NESym
%token	<valu> _NEXT
%token	<valu> _NOR
%token	<valu> _NOT
%token	       tok_NULL
%token	<valu> _OR
%token	<valu> _OUT
%token  <valu> _XOR
%token	<valu> ABS
%token  <valu> ACCESS
%token	<valu> AFTER
%token	<valu> ALIAS
%token	<valu> ALL
%token	<valu> ARCHITECTURE
%token	<valu> ARRAY
%token	<valu> ASSERT
%token	<valu> ATTRIBUTE
%token	<text> AbstractLit
%token	<valu> Ampersand
%token	<valu> Apostrophe
%token	<valu> Arrow
%token	<valu> _BLOCK
%token	<valu> BODY
%token	<valu> BUFFER
%token  <valu> BUS
%token	<valu> Bar
%token	<valu> BasedInt
%token	<valu> BasedReal
%token	<text> BitStringLit
%token	<valu> Box
%token	<valu> CASE
%token	<valu> COMPONENT
%token	<valu> CONFIGURATION
%token	<valu> CONSTANT
%token	<text> CharacterLit
%token	<valu> Colon
%token	<valu> Comma
%token	<valu> DISCONNECT
%token	<valu> DOWNTO
%token	<valu> DecimalInt
%token	<valu> DecimalReal
%token	<valu> Dot
%token	<valu> DoubleStar
%token	<valu> ELSE
%token	<valu> ELSIF
%token	<valu> ENTITY
%token  <valu> ERROR
%token	<valu> FOR
%token	<valu> FUNCTION
%token	<valu> GENERATE
%token	<valu> GENERIC
%token  <valu> GUARDED
%token  <valu> IF
%token	<valu> IS
%token	<text> Identifier
%token  <valu> LIBRARY
%token	<valu> LOOP
%token	       LeftParen
%token	<valu> MAP
%token	<valu> MOD
%token	<valu> Minus
%token  <valu> NEW
%token	<valu> OF
%token  <valu> ON
%token	<valu> OPEN
%token	<valu> OTHERS
%token	<valu> _PACKAGE
%token	       PORT
%token	<valu> PROCEDURE
%token	<valu> PROCESS
%token	<valu> Plus
%token	<valu> RANGE
%token	<valu> RECORD
%token  <valu> REGISTER
%token	<valu> REM
%token	<valu> REPORT
%token	<valu> RETURN
%token	<valu> RightParen
%token	<valu> SELECT
%token	<valu> SEVERITY
%token	       SIGNAL
%token	       _STABLE
%token	       _EVENT
%token	<valu> SUBTYPE
%token	<valu> Semicolon
%token	<valu> Star
%token	<text> StringLit
%token	<valu> THEN
%token  <valu> TO
%token	<valu> TRANSPORT
%token	<valu> FBHTYPE
%token	<valu> UNITS
%token  <valu> UNTIL
%token	<valu> USE
%token  <valu> VARIABLE
%token	<valu> VarAsgn
%token  <valu> WAIT
%token  <valu> WARNING
%token  <valu> WHEN
%token  <valu> WHILE
%token  <valu> WITH

%left         tok_AND _OR _NAND _NOR _XOR
%left         _EQSym _NESym
%left         _NOT


%type  <valu> .SEVERITY__expression.
%type  <valu> severity__message
%type  <text> .REPORT__expression.
%type  <text> report__message
%type  <text> .guard_expression.
%type  <text> guard_expression


%type <list> waveform
%type <list> waveform_element
%type <list> ...waveform__WHEN__choices..
%type <list> waveform__WHEN__choices
%type <list> condition
%type <list> expression
%type <list> relation..AND__relation..
%type <list> relation..OR__relation..
%type <list> relation.NAND_NOR__relation.
%type <list> relation..XOR__relation..
%type <list> relation
%type <list> simple_expression
%type <list> .sign.term..add_op__term..
%type <list> term
%type <list> factor
%type <list> primary
%type <valu> relational_operator
%type <text> literal
%type <name> choice
%type <list> aggregate

%type <valu> .GUARDED.

%type <type> type_definition
%type <type> scalar_type_definition
%type <type> composite_type_definition
%type <type> enumeration_type_definition
%type <type> subtype_indication
%type <type> unconstrained_array_definition
%type <type> constrained_array_definition
%type <type> array_type_definition

%type <name> index_subtype_definition
%type <name> index_constraint
%type <name> discrete_range

%type <text> .label.
%type <text> label
%type <text> a_label
%type <name> target
%type <text> simple_name
%type <name> name
%type <name> attribute_name
%type <name> indexed_name
%type <name> slice_name
%type <text> .simple_name.
%type <name> .constraint.
%type <name> constraint
%type <name> range
%type <name> range_constraint
%type <valu> direction
%type <valu> abstractlit
%type <valu> .mode.
%type <name> type_mark
%type <valu> .signal_kind.
%type <valu> attribute_designator


%type <text> constant_VarAsgn__expression

%start design_file

%%

design_file
	: /* empty */ 
                {  
                /* ###----------------------------------------------###  */
                /*    Initializations					 */
                /* ###----------------------------------------------###  */
                FBL_NM1LST = 0;
                FBL_GRDLST = 0;
                FBL_CNDLST = 0;
                FBL_VALLST = 0;

                hshtab     = initab ();

                FBL_ERRFLG  = 0;
                FBL_LINNUM  = 1;
                FBL_NUMPTP  = 0;
                FBL_HEADFIG = (fbfig_list *)0;

		FBL_MODNAM  = 0;
                FBL_BEFPNT  = fbh_addfbfig (FBL_HEADFIG,0);
                FBL_HEADFIG = FBL_BEFPNT;
                FBL_HASH_ASG_FSM = createauthtable( 1000 );
                }
                design_unit
                ..design_unit..
                {
                struct fbbus *fbbus_pnt;
                struct fbreg *fbreg_pnt;
                struct fbout *fbout_pnt;
                struct fbaux *fbaux_pnt;
                authelem *Element;
 
                /* ###----------------------------------------------### */
                /*    Checking that each output have at least one driver*/
                /* ###----------------------------------------------### */

		/*printf("liste des types\n");
                fbtyp_pnt = FBL_BEFPNT->BETYP;
                while (fbtyp_pnt != 0)
                  {
		   printf("name : %s [%d]\n",fbtyp_pnt->NAME,j);
		   printf("class : %c \n",fbtyp_pnt->CLASS);
		   printf("BASE : %d \n",fbtyp_pnt->BASE);
		   printf("INDEX : %d \n",fbtyp_pnt->INDEX);
		   printf("TYPE : %d \n",chktab (hshtab,fbtyp_pnt->NAME,FBL_MODNAM,FBL_TYPDFN) );
		   if(fbtyp_pnt->VALUE != 0)
		   for ( i= 0; i< fbtyp_pnt->SIZE ; i++)
		   	  printf("VAL : %s \n",*(fbtyp_pnt->VALUE + i));
                  fbtyp_pnt = fbtyp_pnt->NEXT;
				  j++;
                  }*/
	
                fbout_pnt = FBL_BEFPNT->BEOUT;
                while (fbout_pnt != 0)
                  {
                    Element = searchauthelem( FBL_HASH_ASG_FSM, fbout_pnt->NAME );
                    if ( Element != (authelem *)0 ) SetFbhAssignByFsm( fbout_pnt );

                  /* LUDO
                  if ( (fbout_pnt->ABL == 0) &&
                       (! IsFbhAssignByFsm( fbout_pnt ) ) )
                  {
                    fbl_error (40,fbout_pnt->NAME);
                  }
                  */
                  fbout_pnt = fbout_pnt->NEXT;
                  }

                /*fbrin_pnt = FBL_BEFPNT->BERIN;
                while (fbout_pnt != 0)
                  {
		   printf("%s \n",fbrin_pnt->NAME);
                  if (fbout_pnt->ABL == 0)
                    fbl_error (40,fbrin_pnt->NAME);
                  fbrin_pnt = fbrin_pnt->NEXT;
                  }*/
 
                /*fbpcs_pnt = FBL_BEFPNT->BEPCS;
                while (fbpcs_pnt != 0)
                  {
		  struct fbpgm *fbpgm;
		  struct ptype *fbtyp;
		   printf("label : %s \n",fbpcs_pnt->LABEL);
			fbpgm = FBL_BEFPNT->BEPGM;
			while(fbpgm)
			{
			  fbpgm = fbpgm->NEXT;
			}
			fbtyp = fbpcs_pnt->INSTRUCTION;
			while(fbtyp)
			{
			  display_fbh(fbtyp); 
			  fbtyp = fbtyp->NEXT;
			}
                  fbpcs_pnt = fbpcs_pnt->NEXT;
                  }*/

                fbaux_pnt = FBL_BEFPNT->BEAUX;
                while (fbaux_pnt != 0)
                  {
                    Element = searchauthelem( FBL_HASH_ASG_FSM, fbaux_pnt->NAME );
                    if ( Element != (authelem *)0 ) SetFbhAssignByFsm( fbaux_pnt );

                  if ( (fbaux_pnt->ABL == 0) &&
                       (! IsFbhAssignByFsm( fbaux_pnt ) ) )
                  {
                    fbl_error (40,fbaux_pnt->NAME);
                  }
                  fbaux_pnt = fbaux_pnt->NEXT;
                  }
 
                fbbus_pnt = FBL_BEFPNT->BEBUS;
                while (fbbus_pnt != 0)
                  {
                  if (fbbus_pnt->BIABL == 0)
                  {
                    fbl_error (40,fbbus_pnt->NAME);
                  }
                  fbbus_pnt = fbbus_pnt->NEXT;
                  }
 
                fbreg_pnt = FBL_BEFPNT->BEREG;
                while (fbreg_pnt != 0)
                  {
                  if (fbreg_pnt->BIABL == 0)
                  {
                    fbl_error (40,fbreg_pnt->NAME);
                  }
                  fbreg_pnt = fbreg_pnt->NEXT;
                  }
                fretab (hshtab);
                destroyauthtable( FBL_HASH_ASG_FSM );
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
        ;

library_unit
        : primary_unit
	  { }
        | secondary_unit
	  { }
        | error
	  { }
        ;

primary_unit
        : entity_declaration
	  { }
        ;

secondary_unit
        : architecture_body
	  { }
        ;

library_clause
        : LIBRARY
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

...selected_name..
        : /*empty*/
	  { }
        | ...selected_name..
          Comma
          selected_name
	  { }
        ;
        
selected_name
        : prefix
          Dot
          suffix
	  {
	  }
        ;

suffix
        : simple_name
	  { }
        | ALL
	  { }
        ;

prefix
	: simple_name
	  {  }
	| prefix
	  Dot
	  simple_name
	  { }
	;

entity_declaration
	: ENTITY
	  simple_name
	  IS
        {
		long type,size=0;
		struct fbtyp *fbtyp_pnt;
                char   *name;
                char   *name_bit;
                char   *name_bit_vector;
                char   *name_std_logic;
                char   *name_std_ulogic;
                char   *name_std_logic_vector;
                char   *name_std_ulogic_vector;
                FBL_BEFPNT->NAME = $2;
		FBL_MODNAM = $2;

		fbtyp_pnt = fbh_prdeftyp();
		 FBL_BEFPNT->BETYP = fbtyp_pnt;
		 FBL_NUMTYP =1;
		 while(fbtyp_pnt)
		 {
		   size++;
		    fbtyp_pnt = fbtyp_pnt->NEXT;
		  }

                  name_bit = namealloc( "bit" );
                  name_bit_vector = namealloc( "bit_vector" );
                  name_std_logic = namealloc( "std_logic" );
                  name_std_ulogic = namealloc( "std_ulogic" );
                  name_std_logic_vector = namealloc( "std_logic_vector" );
                  name_std_ulogic_vector = namealloc( "std_ulogic_vector" );

		 fbtyp_pnt =FBL_BEFPNT->BETYP ;
		while(fbtyp_pnt)
		  {
		type = val_type(fbtyp_pnt->NAME);
   	    addtab(hshtab,fbtyp_pnt->NAME,FBL_MODNAM,FBL_SIGDFN,FBL_TPEDFN);
   	    addtab(hshtab,fbtyp_pnt->NAME,FBL_MODNAM,FBL_USRDFN,type);
   	    addtab(hshtab,fbtyp_pnt->NAME,FBL_MODNAM,FBL_LBLDFN,fbtyp_pnt->CLASS);
   	    addtab(hshtab,fbtyp_pnt->NAME,FBL_MODNAM,FBL_TYPDFN,size +1 - FBL_NUMTYP);
            addtab(hshtab,fbtyp_pnt->NAME,FBL_MODNAM,FBL_PNTDFN,(long)fbtyp_pnt);
                    if ( ( fbtyp_pnt->NAME == name_bit        ) ||
                         ( fbtyp_pnt->NAME == name_bit_vector ) )
                    {
                      if ( fbtyp_pnt->NAME == name_bit ) name = name_std_logic;
                      else                               name = name_std_logic_vector;

   	    addtab(hshtab,name,FBL_MODNAM,FBL_SIGDFN,FBL_TPEDFN);
   	    addtab(hshtab,name,FBL_MODNAM,FBL_USRDFN,type);
   	    addtab(hshtab,name,FBL_MODNAM,FBL_LBLDFN,fbtyp_pnt->CLASS);
   	    addtab(hshtab,name,FBL_MODNAM,FBL_TYPDFN,size +1 - FBL_NUMTYP);
            addtab(hshtab,name,FBL_MODNAM,FBL_PNTDFN,(long)fbtyp_pnt);

                      if ( fbtyp_pnt->NAME == name_bit ) name = name_std_ulogic;
                      else                               name = name_std_ulogic_vector;

   	    addtab(hshtab,name,FBL_MODNAM,FBL_SIGDFN,FBL_TPEDFN);
   	    addtab(hshtab,name,FBL_MODNAM,FBL_USRDFN,type);
   	    addtab(hshtab,name,FBL_MODNAM,FBL_LBLDFN,fbtyp_pnt->CLASS);
   	    addtab(hshtab,name,FBL_MODNAM,FBL_TYPDFN,size +1 - FBL_NUMTYP);
            addtab(hshtab,name,FBL_MODNAM,FBL_PNTDFN,(long)fbtyp_pnt);
                    }
		    fbtyp_pnt = fbtyp_pnt->NEXT;
		    FBL_NUMTYP++;
		  }
                }
	  .port_clause.
	  END_ERR
	  .simple_name.
	  Semicolon_ERR
               {
                if (($7 != 0) && ($7 != $2))
                {
                  fbl_error (1,$7);
                }
                }
	| ENTITY
	  error
		{ fbl_error (2,0); }
	;

.constraint.
	: /*empty*/
               {  
                $$.FLAG  = 'U';
                $$.LEFT  = -1;
                $$.RIGHT = -1;
                }
        | constraint
                { $$ = $1; }
	;

constraint
	: LeftParen
	  range
	  RightParen_ERR
		{ $$ = $2; }
	;

range_constraint
	: RANGE
          range
	  { $$ = $2; }
        ;

range
	: abstractlit
	  direction
	  abstractlit
                {    
                $$.FLAG  = 'A';
                $$.LEFT  = $1;
                $$.RIGHT = $3;

                if ((($1 > $3) && ($2 == FBL_UPTDFN)) ||
                    (($1 < $3) && ($2 == FBL_DWTDFN)) ||
                    (($1 < 0 ) || ($2 < 0 )))
                  {
                  fbl_error (32,0);
                  }
                }  
	;

direction
	: TO
                { $$ = FBL_UPTDFN; }
        | DOWNTO
                { $$ = FBL_DWTDFN; }
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
	  Semicolon_ERR
	| PORT
	  error
	  Semicolon_ERR
                { fbl_error (3,0); }
	;

formal_port_list
	: formal_port_element
	  ...formal_port_element..
	;

...formal_port_element..
	: /*empty*/
	| ...formal_port_element..
	  Semicolon_ERR
	  formal_port_element
	;

formal_port_element
	: .SIGNAL.
	  identifier_list
	  Colon
	  .mode.
	  type_mark
	  .constraint.
	  .signal_kind.
                {
                char *signame;
                long   sigconf;
                void *pnt;
 
                /* ###----------------------------------------------### */
                /*    First, check the validity of the declaration.     */
                /* Then, for each port, create the apropriate set of    */
                /* structures (fbrin, fbpor, fbout, fbbus)              */
                /* ###----------------------------------------------### */
 
                fbl_chkdcl ('P', $4, val_type($5.NAME), $5.FLAG, $7, $6.FLAG,&sigconf);

                FBL_NM1LST = reverse (FBL_NM1LST);
                while (FBL_NM1LST != 0)
                  {
                  signame = (char *)FBL_NM1LST->DATA;
                  if (chktab (hshtab,signame,FBL_MODNAM,FBL_SIGDFN) != 0)
                  {
                    fbl_error (4,signame);
                  }

                  pnt =  fbl_addstr (FBL_BEFPNT,'P',$4,val_type($5.NAME),$5.TYPE,$5.FLAG,signame,
                                $6.LEFT,$6.RIGHT);
 
                  addtab (hshtab,signame,FBL_MODNAM,FBL_SIGDFN,sigconf);
                  addtab (hshtab,signame,FBL_MODNAM,FBL_TYPDFN,$5.TYPE);
                  addtab (hshtab,signame,FBL_MODNAM,FBL_WMNDFN,$6.LEFT);
                  addtab (hshtab,signame,FBL_MODNAM,FBL_WMXDFN,$6.RIGHT);
                  addtab (hshtab,signame,FBL_MODNAM,FBL_LBLDFN,$7);
                  addtab (hshtab,signame,FBL_MODNAM,FBL_PNTDFN,(long)pnt);
 
                  FBL_NM1LST = delchain (FBL_NM1LST, FBL_NM1LST);
                  }
                }
	| error
                {
                /* ###----------------------------------------------### */
                /*   The following 3 lines reject tokens until the	*/
                /* sync. token 'Semicolon' is found			*/
                /* ###----------------------------------------------### */
 
                /*do
                  yychar = yylex ();
                while ((yychar != Semicolon) && (yychar != 0));           
                yyerrok;
*/ 
                fbl_error (6,0);
                }
	;

.SIGNAL.
	: /*empty*/
	| SIGNAL
	;

.mode.
        : /*empty*/
                { $$ = FBL_ICNDFN; }
        | _IN   
                { $$ = FBL_ICNDFN; }
        | _OUT  
                { $$ = FBL_OCNDFN; }
        | _INOUT
                { $$ = FBL_BCNDFN; }
        ;

architecture_body
	: ARCHITECTURE
	  Identifier
	  OF
	  simple_name
	  IS
                {
                if ($4 != FBL_MODNAM)
                {
                  fbl_error (1,$4);
                }
                }
	  architecture_declarative_part
		{
		  struct fbtyp *listfbtyp; 
		  struct fbtyp *fbtypaux; 
		  struct fbtyp *revfbtyp = 0; 
			listfbtyp = FBL_BEFPNT->BETYP;
		     revfbtyp = listfbtyp;
		     listfbtyp = listfbtyp->NEXT;		
		     revfbtyp->NEXT = 0;	
		  while(listfbtyp != 0)
		   {
		     fbtypaux = revfbtyp;	
		     revfbtyp = listfbtyp;
		     listfbtyp = listfbtyp->NEXT;		
		     revfbtyp->NEXT = fbtypaux;		
		   }
			
                        FBL_BEFPNT->BETYP = fbh_crtfbtyp(revfbtyp);
		}
	  _BEGIN
	  architecture_statement_part
	  END_ERR
	  .simple_name.
	  Semicolon_ERR
                {
                if (($12 != 0) && ($12 != $2))
                {
                  fbl_error (7,$12);
                }
                }
	| ARCHITECTURE
	  error
                { fbl_error (8,0); }
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
	: signal_declaration
        | type_declaration
	| subtype_declaration
	| constant_declaration
	| pragma_declaration 
	| error
	  Semicolon_ERR
                { fbl_error (9,0); }
	;

pragma_declaration
	:  Pragma
		{
		char pragma[30];
		char type[128];
		char name[128];
		char value[128];
		char *pt = NULL;
		long  field;

		 field = sscanf((char *)$1,"-- %s %s %s %s", pragma,type,name,value);

		if ( field == 3 ) pt = (void *)0;
		else
		if ( field == 4 ) pt = namealloc( value );
		else
		{
                  fbl_error (86,0);
		}
		
                        FBL_BEFPNT->BEPGM = fbh_addfbpgm(FBL_BEFPNT->BEPGM,type,name,pt);
                        FBL_BEFPNT->BEPGM->LINE_NUM = FBL_LINNUM;
		}
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
                long sigconf;

                if (fbl_chkdcl ('C',0,val_type($4.NAME),$4.FLAG,0,$5.FLAG,&sigconf) == 0)
                {
                  addtab (hshtab,$2,FBL_MODNAM,FBL_WMNDFN,$5.LEFT);
                  addtab (hshtab,$2,FBL_MODNAM,FBL_WMXDFN,$5.RIGHT);
                  addtab (hshtab,$2,FBL_MODNAM,FBL_SIGDFN,sigconf);
                  addtab (hshtab,$2,FBL_MODNAM,FBL_TYPDFN,-1);
                  addtab (hshtab,$2,0,FBL_PNTDFN,$6);
                }
                }  
	;

constant_VarAsgn__expression
	: VarAsgn
	  literal
                { $$ = $2; }
	;

signal_declaration
	: SIGNAL
	  identifier_list
	  Colon
	  type_mark
	  .constraint.
	  .signal_kind.
	  Semicolon_ERR
                {
                char *signame;
                long   sigconf;
                void *pnt;
                long   errflg;
 
                errflg = fbl_chkdcl ('S',0,val_type($4.NAME),$4.FLAG,$6,$5.FLAG,&sigconf);
 
              /* ###----------------------------------------------### */
            /*    First, check the validity of the declaration.     */
          /* Then, for each signal, create the apropriate set of  */
        /* structures (fbrin, fbreg, fbaux, fbbux)
                */
                /* ###----------------------------------------------### */
 
	  /*if( val_type($4.NAME) != -1)
		fbl_error(81,$4.NAME);*/
	  /*else */
	  {
              FBL_NM1LST = reverse (FBL_NM1LST);
                while (FBL_NM1LST != 0)
                  {
                  signame = (char *)FBL_NM1LST->DATA;
                  if (chktab (hshtab,signame,FBL_MODNAM,FBL_SIGDFN) != 0)
                  {
                  fbl_error (10,signame);
                  }

                  pnt = fbl_addstr (FBL_BEFPNT,'S',0,val_type($4.NAME),$4.TYPE,
			$4.FLAG,signame, $5.LEFT,$5.RIGHT);
 
                  addtab (hshtab,signame,FBL_MODNAM,FBL_SIGDFN,sigconf);
                  addtab (hshtab,signame,FBL_MODNAM,FBL_TYPDFN,$4.TYPE);
                addtab (hshtab,signame,FBL_MODNAM,FBL_WMNDFN,$5.LEFT);
              addtab (hshtab,signame,FBL_MODNAM,FBL_WMXDFN,$5.RIGHT);
              addtab (hshtab,signame,FBL_MODNAM,FBL_LBLDFN,$6);
            addtab (hshtab,signame,FBL_MODNAM,FBL_PNTDFN,(long)pnt);
 
          FBL_NM1LST = delchain (FBL_NM1LST,FBL_NM1LST);
        
                  }
           }  
	}
	;

type_declaration
        : full_type_declaration
        ;

full_type_declaration
        : FBHTYPE
          Identifier
          IS
          type_definition
		{
		FBL_BEFPNT->BETYP =  fbh_addfbtyp(FBL_BEFPNT->BETYP,$2,$4.LEFT,
		 $4.RIGHT,$4.VALUE,$4.SIZE,$4.BYTE,$4.BASE,$4.INDEX,$4.RESOLV,$4.CLASS); 
                FBL_BEFPNT->BETYP->LINE_NUM = FBL_LINNUM;
   		addtab(hshtab,$2,FBL_MODNAM,FBL_SIGDFN,FBL_TPEDFN);
   		addtab(hshtab,$2,FBL_MODNAM,FBL_LBLDFN,$4.CLASS);
   	        addtab(hshtab,$2,FBL_MODNAM,FBL_TYPDFN,FBL_NUMTYP);
                addtab (hshtab,$2,FBL_MODNAM,FBL_PNTDFN,(long)FBL_BEFPNT->BETYP);
				FBL_NUMTYP++;
		}
          Semicolon_ERR
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
		{
		FBL_BEFPNT->BETYP =  fbh_addfbtyp(FBL_BEFPNT->BETYP,$2,$4.LEFT,
		 $4.RIGHT,$4.VALUE,$4.SIZE,$4.BYTE,$4.BASE,$4.INDEX,$4.RESOLV,$4.CLASS); 
                FBL_BEFPNT->BETYP->LINE_NUM = FBL_LINNUM;
   		addtab(hshtab,$2,FBL_MODNAM,FBL_SIGDFN,FBL_TPEDFN);
   		addtab(hshtab,$2,FBL_MODNAM,FBL_LBLDFN,$4.CLASS);
   	        addtab(hshtab,$2,FBL_MODNAM,FBL_TYPDFN,FBL_NUMTYP);
                addtab (hshtab,$2,FBL_MODNAM,FBL_PNTDFN,(long)FBL_BEFPNT->BETYP);
				FBL_NUMTYP++;
		}
          Semicolon_ERR
        ;

subtype_indication
	: type_mark
          .constraint.
		{ struct fbtyp *fbtyp_pnt;
		fbtyp_pnt = (struct fbtyp*)chktab(hshtab,$1,FBL_MODNAM,FBL_PNTDFN);
		$$.BASE = $1.TYPE ; 
		$$.VALUE = 0 ;
		$$.SIZE = 0 ;
		$$.BYTE = fbtyp_pnt->BYTE  ;
		$$.LEFT = $2.LEFT ;
		$$.RIGHT = $2.RIGHT ;
		$$.CLASS = 'S' ;
		$$.RESOLV = 0 ;
		}
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
		long size=0;
		long indice=0;
                char **pnt = NULL;
		chain_list *nm1lst;
 
                FBL_NM1LST = reverse (FBL_NM1LST);

		nm1lst = FBL_NM1LST;
                while (nm1lst != 0)
		  {
			size++;
			nm1lst = nm1lst->NEXT;
	  	  }
		if( size != 0)
		  pnt = (char **) malloc(size * sizeof(char*));
	         
                while (FBL_NM1LST != 0)
                  {
                  enumname = namealloc((char *)FBL_NM1LST->DATA);
                  if (chktab (hshtab,enumname,FBL_MODNAM,FBL_SIGDFN) != 0)
                  {
                    fbl_error (82,enumname);
                  }

                  addtab (hshtab,enumname,FBL_MODNAM,FBL_SIGDFN,FBL_CSTDFN);
                  addtab (hshtab,enumname,FBL_MODNAM,FBL_TYPDFN,FBL_NUMTYP);
                  addtab (hshtab,enumname,FBL_MODNAM,FBL_USRDFN,size);
                  addtab (hshtab,enumname,FBL_MODNAM,FBL_WMNDFN,-1);
                  addtab (hshtab,enumname,FBL_MODNAM,FBL_WMXDFN,-1);
                  addtab (hshtab,enumname,FBL_MODNAM,FBL_LBLDFN,indice);
                  addtab (hshtab,enumname,FBL_MODNAM,FBL_PNTDFN,(long)pnt);
 
		  pnt[indice++] = enumname;
                  FBL_NM1LST = delchain (FBL_NM1LST, FBL_NM1LST);
                  }
		$$.VALUE = pnt ;
		$$.SIZE = size ;
		$$.LEFT = 0 ;
		$$.RIGHT = 0 ;
		$$.BYTE = size/256 +1;
		$$.CLASS = 'E' ;
		$$.BASE = 0 ;
		$$.RESOLV = 0 ;
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
                { FBL_NM1LST = addchain (FBL_NM1LST,$1); }
        | CharacterLit
                { FBL_NM1LST = addchain (FBL_NM1LST,$1); }
        ;
 

composite_type_definition
        : array_type_definition
		{ 
		 $$ = $1; 
		}
        ;
 
array_type_definition
        : unconstrained_array_definition
		{ $$ = $1; }
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
		{ $$ = $4; 
		$$.CLASS = 'A'; 
		$$.INDEX = $2.TYPE; 
		}
        ;
 
index_subtype_definition
        : name    /* type_mark  - causes conflict */
          RANGE
          Box  
              { 
		$$.NAME = $1.NAME;
		$$.LEFT = $1.LEFT;
		$$.RIGHT = $1.RIGHT;
		/*$$.TYPE = chktab(hshtab,$1,FBL_MODNAM,FBL_TYPDFN); */
		$$.TYPE = $1.TYPE; 
		$$.FLAG = chktab(hshtab,$1,FBL_MODNAM,FBL_LBLDFN);
		if ($$.TYPE == 0 || $$.FLAG ==0)
                {
		  fbl_error(83,$1.NAME);
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
   	: simple_name
              { 
		long type;
		$$.NAME = $1;
		$$.LEFT = -1;
		$$.RIGHT = -1;
		$$.TYPE = chktab(hshtab,$1,FBL_MODNAM,FBL_TYPDFN);
		$$.FLAG = chktab(hshtab,$1,FBL_MODNAM,FBL_LBLDFN);
		type = chktab(hshtab,$1,FBL_MODNAM,FBL_SIGDFN); 
		if (type != FBL_TPEDFN || $$.TYPE ==0)
                {
			fbl_error(84,$1);
                }
	      }
        ;


.signal_kind.
	: /*empty*/
		{ $$ = 0; }
	| REGISTER
		{ $$ = REGISTER; }
	| BUS
		{ $$ = BUS; }
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
	: process_statement
        | block_statement
	| concurrent_assertion_statement
	| concurrent_signal_assignment_statement
	| error
	  Semicolon_ERR
                { fbl_error (18,0); }
	;

/* DEBUT LUDO */

block_statement
	: a_label
	  _BLOCK
	  .guard_expression.
	  _BEGIN
		{
		if (chktab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN) != 0)
                {
		  fbl_error (19,$1);
                }

		addtab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN,1);
		if ($3 != NULL)
		  FBL_GRDLST = addchain (FBL_GRDLST,(char *)$3);
		}
	  set_of_statements
	  END_ERR
	  _BLOCK
	  .label.
	  Semicolon_ERR
		{
		if ($3 != NULL)
		  FBL_GRDLST = delchain (FBL_GRDLST,FBL_GRDLST);
		if (($9 != NULL) && ($9 != $1))
                {
		  fbl_error (20,$9);
                }
		}
	;

set_of_statements
	: ..concurrent_statement..
	;

concurrent_assertion_statement
	: a_label
	  unlabeled_concurrent_assertion_statement
		{
		if (chktab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN) != 0)
                {
		  fbl_error (19,$1);
                }

		addtab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN,1);
		if (FBL_ERRFLG == 0)
		  FBL_BEFPNT->BEMSG->LABEL = $1;
		}
	| unlabeled_concurrent_assertion_statement
	;

concurrent_signal_assignment_statement
	: a_label
	  unlabeled_conditional_signal_assignment
		{
		if (chktab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN) != 0)
                {
		  fbl_error (19,$1);
                }
		addtab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN,1);
		}
	| unlabeled_conditional_signal_assignment
	| a_label
	  unlabeled_selected_signal_assignment
		{
		if (chktab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN) != 0)
                {
                  {
		  fbl_error (19,$1);
                  }
                }
		addtab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN,1);
		}
	| unlabeled_selected_signal_assignment
	;

unlabeled_concurrent_assertion_statement
	: assertion_statement
	;

unlabeled_conditional_signal_assignment
	: target
	  _LESym
	  .GUARDED.
	  ..waveform__WHEN__condition__ELSE..
	  waveform
	  Semicolon_ERR
		{
		long             i;
		struct fbout   *fbout_pnt;
		struct fbbus   *fbbus_pnt;
		struct fbreg   *fbreg_pnt;
		struct fbaux   *fbaux_pnt;
		struct fbbux   *fbbux_pnt;
		struct chain   *abl_pnt;
		struct fbl_expr expr0;
		struct fbl_expr expr1;
		struct fbl_expr expr2;
		struct fbl_expr expr3;
		struct fbl_expr expr4;
		struct fbl_expr expr5;
		struct fbl_expr expr6;
		long             rev_flg = 0;
		long             left_bnd;
		long             right_bnd;
		long             left;
		long             right;
		long             in_bound;
		long             out_bound;
		long             sig_width;
		long             sig_conf;

		expr4 = $5;

		while (FBL_CNDLST != NULL)
		  {
		  expr5 = *((fbl_ablstr *) FBL_CNDLST->DATA);
		  expr6 = *((fbl_ablstr *) FBL_VALLST->DATA);

		  expr0 = fbl_cpyablstr (expr5);
		  expr1 = fbl_crtabl (ABL_NOT , expr5, FBL_EMPSTR,-1,-1);
		  expr5 = expr0;
		  expr2 = fbl_crtabl (ANDM, expr4, expr1     ,-1,-1);
		  expr3 = fbl_crtabl (ANDM, expr6, expr5     ,-1,-1);
		  expr4 = fbl_crtabl (ABL_OR  , expr2, expr3     ,-1,-1);

		  FBL_CNDLST = delchain (FBL_CNDLST,FBL_CNDLST);
		  FBL_VALLST = delchain (FBL_VALLST,FBL_VALLST);
		  }

		left_bnd  = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_WMNDFN);
		right_bnd = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_WMXDFN);
		sig_conf  = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_SIGDFN);

		left  = $1.LEFT;
		right = $1.RIGHT;

		if (left_bnd <= right_bnd)
		  {
		  sig_width = right_bnd - left_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 0;
		    in_bound  = right_bnd - right;
		    out_bound = right_bnd - left;
		    if ((left < left_bnd) || (right > right_bnd))
                    {
		      fbl_error (36,$1.NAME);
                    }
		    }
		  else
		    {
		    rev_flg   = 1;
		    in_bound  = right_bnd - left;
		    out_bound = right_bnd - right;
		    if ((left > right_bnd) || (right < left_bnd))
                    {
		      fbl_error (36,$1.NAME);
                    }
		    }
		  }
		else
		  {
		  sig_width = left_bnd - right_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 1;
		    in_bound  = left  - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
                    {
		      fbl_error (36,$1.NAME);
                    }
		    }
		  else
		    {
		    rev_flg   = 0;
		    in_bound  = right - right_bnd;
		    out_bound = left  - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
                    {
		      fbl_error (36,$1.NAME);
                    }
		    }
		  }

		if ((out_bound - in_bound + 1) != expr4.WIDTH)
                {
		  fbl_error (35,$1.NAME);
                }

		if (rev_flg == 1)
		  expr4.LIST_ABL = reverse (expr4.LIST_ABL);

		abl_pnt = expr4.LIST_ABL;

		switch (sig_conf)
		  {
		  case (FBL_ICNDFN + FBL_BITDFN + FBL_NORDFN) :
                    {
		    fbl_error (21,$1.NAME);
                    }
		    break;

		  case (FBL_OCNDFN + FBL_BITDFN + FBL_NORDFN) :
		  case (FBL_BCNDFN + FBL_BITDFN + FBL_NORDFN) :
		    if ($3 == FBL_GRDDFN)
                    {
		      fbl_error (22,$1.NAME);
                    }
		    fbout_pnt = (struct fbout *)
		                 chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (fbout_pnt->ABL != NULL)
                          {
		            fbl_error (39,fbout_pnt->NAME);
                          }
		          else
		            fbout_pnt->ABL = (struct chain *)abl_pnt->DATA;
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbout_pnt = fbout_pnt->NEXT;
		        }
		      }
		    break;

		  case (FBL_OCNDFN + FBL_MUXDFN + FBL_BUSDFN) :
		  case (FBL_OCNDFN + FBL_WORDFN + FBL_BUSDFN) :
		  case (FBL_BCNDFN + FBL_MUXDFN + FBL_BUSDFN) :
		  case (FBL_BCNDFN + FBL_WORDFN + FBL_BUSDFN) :
		    if (($3 != FBL_GRDDFN) || (FBL_GRDLST == NULL))
                    {
		      fbl_error (23,$1.NAME);
                    }
		    fbbus_pnt = (struct fbbus *)
		                 chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          fbbus_pnt->BINODE = fbh_addbinode (fbbus_pnt->BINODE,
		                                             NULL,NULL);
		          fbbus_pnt->BIABL  = fbh_addbiabl  (fbbus_pnt->BIABL,
		                                             FBL_LBLNAM,
		                                             dupablexpr(FBL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbbus_pnt = fbbus_pnt->NEXT;
		        }
		      }
		    break;

		  case (FBL_BITDFN + FBL_NORDFN):
		    if ($3 == FBL_GRDDFN)
                    {
		      fbl_error (22,$1.NAME);
                    }
		    fbaux_pnt = (struct fbaux *)
		                chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (fbaux_pnt->ABL != NULL)
                          {
		            fbl_error (39,fbaux_pnt->NAME);
                          }
		          else
		            fbaux_pnt->ABL = (struct chain *)abl_pnt->DATA;
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbaux_pnt = fbaux_pnt->NEXT;
		        }
		      }
		    break;

		  case (FBL_RBIDFN + FBL_REGDFN):
		    if (($3 != FBL_GRDDFN) || (FBL_GRDLST == NULL))
                    {
		      fbl_error (23,$1.NAME);
                    }
		    fbreg_pnt = (struct fbreg *)
		                chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          fbreg_pnt->BINODE = fbh_addbinode (fbreg_pnt->BINODE,
		                                             NULL,NULL);
		          fbreg_pnt->BIABL  = fbh_addbiabl  (fbreg_pnt->BIABL,
		                                             FBL_LBLNAM,
		                                             dupablexpr(FBL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbreg_pnt = fbreg_pnt->NEXT;
		        }
		      }
		    break;

		  case (FBL_MUXDFN + FBL_BUSDFN) :
		  case (FBL_WORDFN + FBL_BUSDFN) :
		    if (($3 != FBL_GRDDFN) || (FBL_GRDLST == NULL))
                    {
		      fbl_error (23,$1.NAME);
                    }
		    fbbux_pnt = (struct fbbux *)
		                chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          fbbux_pnt->BINODE = fbh_addbinode (fbbux_pnt->BINODE,
		                                             NULL,NULL);
		          fbbux_pnt->BIABL  = fbh_addbiabl  (fbbux_pnt->BIABL,
		                                             FBL_LBLNAM,
		                                             dupablexpr(FBL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbbux_pnt = fbbux_pnt->NEXT;
		        }
		      }
		    break;

		  default :
                    {
		    fbl_error (17,$1.NAME);
                    }
		    break;
		  }
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
		struct fbl_expr *expr_pnt;

		expr_pnt           = (fbl_ablstr *)mbkalloc(sizeof(fbl_ablstr));
		expr_pnt->WIDTH    = $4.WIDTH;
		expr_pnt->LIST_ABL = $4.LIST_ABL;
		FBL_CNDLST         = addchain (FBL_CNDLST,(char *) expr_pnt);

		expr_pnt           = (fbl_ablstr *)mbkalloc(sizeof(fbl_ablstr));
		expr_pnt->WIDTH    = $2.WIDTH;
		expr_pnt->LIST_ABL = $2.LIST_ABL;
		FBL_VALLST         = addchain (FBL_VALLST,(char *) expr_pnt);
		}
	;

unlabeled_selected_signal_assignment
	: WITH
	  expression
		{
		FBL_SLCEXP = $2;
		FBL_BDDPNT = NULL;
		}
	  SELECT
	  target
	  _LESym
	  .GUARDED.
	  waveform__WHEN__choices
	  ...waveform__WHEN__choices..
	  Semicolon_ERR
		{
		long             i;
		struct fbout   *fbout_pnt;
		struct fbbus   *fbbus_pnt;
		struct fbreg   *fbreg_pnt;
		struct fbaux   *fbaux_pnt;
		struct fbbux   *fbbux_pnt;
		struct chain   *abl_pnt;
		struct fbl_expr expr1;
		long             rev_flg = 0;
		long             left_bnd;
		long             right_bnd;
		long             left;
		long             right;
		long             in_bound;
		long             out_bound;
		long             sig_width;
		long             sig_conf;

		expr1 = fbl_crtabl (ABL_OR ,$8 ,$9,-1,-1);
		if (FBL_BDDPNT != getlogbddnodeone())
                {
		  fbl_error (25,NULL);
                }
		left_bnd  = chktab (hshtab,$5.NAME,FBL_MODNAM,FBL_WMNDFN);
		right_bnd = chktab (hshtab,$5.NAME,FBL_MODNAM,FBL_WMXDFN);
		sig_conf  = chktab (hshtab,$5.NAME,FBL_MODNAM,FBL_SIGDFN);

		left  = $5.LEFT;
		right = $5.RIGHT;
		if (left_bnd <= right_bnd)
		  {
		  sig_width = right_bnd - left_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 0;
		    in_bound  = right_bnd - right;
		    out_bound = right_bnd - left;
		    if ((left < left_bnd) || (right > right_bnd))
                    {
		      fbl_error (36,$5.NAME);
                    }
		    }
		  else
		    {
		    rev_flg   = 1;
		    in_bound  = right_bnd - left;
		    out_bound = right_bnd - right;
		    if ((left > right_bnd) || (right < left_bnd))
                    {
		      fbl_error (36,$5.NAME);
                    }
		    }
		  }
		else
		  {
		  sig_width = left_bnd - right_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 1;
		    in_bound  = left  - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
                    {
		      fbl_error (36,$5.NAME);
                    }
		    }
		  else
		    {
		    rev_flg   = 0;
		    in_bound  = right - right_bnd;
		    out_bound = left  - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
                    {
		      fbl_error (36,$5.NAME);
                    }
		    }
		  }

		if ((out_bound - in_bound + 1) != expr1.WIDTH)
                {
		  fbl_error (35,$5.NAME);
                }

		if (rev_flg == 1)
		  expr1.LIST_ABL = reverse (expr1.LIST_ABL);

		abl_pnt = expr1.LIST_ABL;

		switch (sig_conf)
		  {
		  case (FBL_ICNDFN + FBL_BITDFN + FBL_NORDFN) :
                    {
		    fbl_error (21,$5.NAME);
                    }
		    break;

		  case (FBL_OCNDFN + FBL_BITDFN + FBL_NORDFN) :
		  case (FBL_BCNDFN + FBL_BITDFN + FBL_NORDFN) :
		    if ($7 == FBL_GRDDFN)
                    {
		      fbl_error (22,$5.NAME);
                    }
		    fbout_pnt = (struct fbout *)
		                chktab (hshtab,$5.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (fbout_pnt->ABL != NULL)
                          {
		            fbl_error (39,fbout_pnt->NAME);
                          }
		          else
		            fbout_pnt->ABL = (struct chain *)abl_pnt->DATA;
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbout_pnt = fbout_pnt->NEXT;
		        }
		      }
		    break;

		  case (FBL_OCNDFN + FBL_MUXDFN + FBL_BUSDFN) :
		  case (FBL_OCNDFN + FBL_WORDFN + FBL_BUSDFN) :
		  case (FBL_BCNDFN + FBL_MUXDFN + FBL_BUSDFN) :
		  case (FBL_BCNDFN + FBL_WORDFN + FBL_BUSDFN) :
		    if (($7 != FBL_GRDDFN) || (FBL_GRDLST == NULL))
                    {
		      fbl_error (23,$5.NAME);
                    }
		    fbbus_pnt = (struct fbbus *)
		                chktab (hshtab,$5.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          fbbus_pnt->BINODE = fbh_addbinode (fbbus_pnt->BINODE,
		                                             NULL,NULL);
		          fbbus_pnt->BIABL  = fbh_addbiabl  (fbbus_pnt->BIABL,
		                                             FBL_LBLNAM,
		                                             dupablexpr(FBL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbbus_pnt = fbbus_pnt->NEXT;
		        }
		      }
		    break;

		  case (FBL_BITDFN + FBL_NORDFN):
		    if ($7 == FBL_GRDDFN)
                    {
		      fbl_error (22,$5.NAME);
                    }
		    fbaux_pnt = (struct fbaux *)
		                chktab (hshtab,$5.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (fbaux_pnt->ABL != NULL)
                          {
		            fbl_error (39,fbaux_pnt->NAME);
                          }
		          else
		           {
		            fbaux_pnt->ABL = (struct chain *)abl_pnt->DATA;
		            }
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbaux_pnt = fbaux_pnt->NEXT;
		        }
		      }
		    break;

		  case (FBL_RBIDFN + FBL_REGDFN):
		    if (($7 != FBL_GRDDFN) || (FBL_GRDLST == NULL))
                    {
		      fbl_error (23,$5.NAME);
                    }
		    fbreg_pnt = (struct fbreg *)
		                chktab (hshtab,$5.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          fbreg_pnt->BINODE = fbh_addbinode (fbreg_pnt->BINODE,
		                                             NULL,NULL);
		          fbreg_pnt->BIABL  = fbh_addbiabl  (fbreg_pnt->BIABL,
		                                             FBL_LBLNAM,
		                                             dupablexpr(FBL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbreg_pnt = fbreg_pnt->NEXT;
		        }
		      }
		    break;

		  case (FBL_MUXDFN + FBL_BUSDFN) :
		  case (FBL_WORDFN + FBL_BUSDFN) :
		    if (($7 != FBL_GRDDFN) || (FBL_GRDLST == NULL))
                    {
		      fbl_error (23,$5.NAME);
                    }
		    fbbux_pnt = (struct fbbux *)
		                chktab (hshtab,$5.NAME,FBL_MODNAM,FBL_PNTDFN);
		    if (FBL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          fbbux_pnt->BINODE = fbh_addbinode (fbbux_pnt->BINODE,
		                                             NULL,NULL);
		          fbbux_pnt->BIABL  = fbh_addbiabl  (fbbux_pnt->BIABL,
		                                             FBL_LBLNAM,
		                                             dupablexpr(FBL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        fbbux_pnt = fbbux_pnt->NEXT;
		        }
		      }
		    break;

		  default :
                    {
		    fbl_error (17,$5.NAME);
                    }
		    break;
		  }
		}
	;
/* FIN LUDO */

process_statement
	: a_label
		{
		  FBL_LBLNAM = namealloc($1); 
		}
	  unlabeled_process_statement
	|	{
                  char buffer[ 64 ];

                  sprintf( buffer, "process_%ld", FBL_LINNUM );
		  FBL_LBLNAM = namealloc( buffer ); 
		}
	  unlabeled_process_statement
	;

unlabeled_process_statement
	: PROCESS
	{FBL_NM1LST = 0 ;}
	  .sensitivity_list.
		{ FBL_BEFPNT->BEPCS = fbh_addfbpcs(FBL_BEFPNT->BEPCS,FBL_LBLNAM,FBL_NM1LST,0,0,0); 
                  FBL_BEFPNT->BEPCS->LINE_NUM = FBL_LINNUM;
		  FBL_NM1LST = 0; }	
          process_declarative_part
	  _BEGIN
		{  
		void *pnt;
		pnt = (void *)(&(FBL_BEFPNT->BEPCS->INSTRUCTION));
                   FBL_NM1LST = addchain(0,pnt);
		}

	  sequence_of_statements
		  {
		    struct ptype **pnt;
		    struct ptype *ptype;
		    pnt = (struct ptype**)FBL_NM1LST->DATA;
		    ptype = reversetype(*pnt); 
		    *pnt = ptype; 
                  FBL_NM1LST = delchain (FBL_NM1LST, FBL_NM1LST);
		  }
	  END_ERR
	  PROCESS
	  .label.
	  Semicolon_ERR
		{ 
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
        | type_declaration
        | subtype_declaration
        ;

.sensitivity_list.
        : /*empty*/
        | sensitivity_list
        ;

sensitivity_list
	: LeftParen
	  signal_list
	  RightParen
	;

signal_list
        : name
          ...name..
		{ 
		if($1.TYPE == 0)
                {
			fbl_error(17,$1.NAME) ;
                }
		FBL_NM1LST = addchain (FBL_NM1LST,$1.NAME); }
        ;    
 
...name..
        : /*empty*/
        | Comma 
          name 
          ...name..
		{ 
		if($2.TYPE == 0)
                {
			fbl_error(17,$2.NAME) ;
                }
		FBL_NM1LST = addchain (FBL_NM1LST,$2.NAME); }
        ;

sequence_of_statements
	: ..sequential_statement..
	;

..sequential_statement..
	: /*empty*/
	| ..sequential_statement..
	  sequential_statement
	;

sequential_statement
	: assertion_statement
	| signal_assignment_statement
	| if_statement
	| case_statement
	| null_statement
	;


assertion_statement
	: ASSERT
	  expression
	  .REPORT__expression.
	  .SEVERITY__expression.
	  Semicolon_ERR
		{   
                    FBL_BEFPNT->BEMSG  = fbh_addfbmsg(FBL_BEFPNT->BEMSG,0,$4,$3,$2.LIST_ABL->DATA,0);
                    FBL_BEFPNT->BEMSG->LINE_NUM = FBL_LINNUM;
		}
	;

signal_assignment_statement
	: target
	  _LESym
	  waveform
	  Semicolon_ERR
		{   unsigned long type; 
		    long sig_conf,i; 	
		    long left_bnd,right_bnd;
		    long in_bound,out_bound;
		    long left ,right;
	            long rev_flg, debut;
		    long sig_width;
		    struct chain   *abl_pnt;
		    char extname[100];
		    struct fbl_expr expr;
		    struct ptype **pnt;

		    type = chktab(hshtab,$1.NAME,FBL_MODNAM,FBL_TYPDFN);
		    sig_conf = chktab(hshtab,$1.NAME,FBL_MODNAM,FBL_SIGDFN);

	 	if( type == 0)
                {
			fbl_error(17,$1.NAME);
                }

                addauthelem( FBL_HASH_ASG_FSM, $1.NAME, 1 );

 	    left_bnd  = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_WMNDFN);
            right_bnd = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_WMXDFN);
 
                left  = $1.LEFT;
                right = $1.RIGHT;

		expr = $3; 

                if (left_bnd <= right_bnd)
                  {
                  sig_width = right_bnd - left_bnd + 1;
                  if (left <= right)
                    {
                    rev_flg   = 0;
                    in_bound  = right_bnd - right;
                    out_bound = right_bnd - left;
                    if ((left < left_bnd) || (right > right_bnd))
                    {
                      fbl_error (36,$1.NAME);
                    }
		    debut = left;  
                    }

                    else
                    {
                    rev_flg   = 1;
                    in_bound  = right_bnd - left;
                    out_bound = right_bnd - right;
                    if ((left > right_bnd) || (right < left_bnd))
                    {
                      fbl_error (36,$1.NAME);
                    }
		    debut = right;  
                    }
                  }
                else
                  {
                  sig_width = left_bnd - right_bnd + 1;
                  if (left <= right)
                    {
                    rev_flg   = 1;
                    in_bound  = left  - right_bnd;
                    out_bound = right - right_bnd;
                    if ((left < right_bnd) || (right > left_bnd))
                    {
                      fbl_error (36,$1.NAME);
                    }
		    debut = left;  
                    }
                  else
                    {
                    rev_flg   = 0;
                    in_bound  = right - right_bnd;
                    out_bound = left  - right_bnd;
                    if ((left > left_bnd) || (right < right_bnd))
                    {
                      fbl_error (36,$1.NAME);
                    }
		    debut = right;  
                    }
                  }
                if ((out_bound - in_bound + 1) != expr.WIDTH)
		{
                  fbl_error (35,$1.NAME);
		}
 
                if (rev_flg == 1)
                  expr.LIST_ABL = reverse (expr.LIST_ABL);
 
                abl_pnt = expr.LIST_ABL;
 

		    if( sig_conf == FBL_ICNDFN)
                    {
			fbl_error(21,$1.NAME);
                    }
                    /* LUDO
		    else if( sig_conf == 0)
                    {
			fbl_error(17,$1.NAME);
                    }
                    */
			else
			{
		      pnt = (struct ptype**)(FBL_NM1LST->DATA);
                  if(sig_width >1) 
		  {
                      for (i=0 ; i<sig_width ; i++)
                        {
                          char *newname;

      			sprintf (extname,"%s %ld",$1.NAME,i+debut-in_bound);
                        newname = namealloc( extname );
                        if (i >= in_bound)
                          {
                         *pnt =fbh_addfbasg(*pnt,newname, (struct chain *)abl_pnt->DATA,type);
                         ((struct fbasg *)((*pnt)->DATA))->LINE_NUM = FBL_LINNUM;
                          abl_pnt = abl_pnt->NEXT;
                
                          addauthelem( FBL_HASH_ASG_FSM, newname, 1 );
                          }
                        if (i >= out_bound)
                          break;
                        } 
                  }
		else
                  {
                    *pnt = fbh_addfbasg(*pnt,$1.NAME,abl_pnt->DATA,type);
                    ((struct fbasg *)((*pnt)->DATA))->LINE_NUM = FBL_LINNUM;
                  }
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
		    pnt = (struct ptype**)FBL_NM1LST->DATA;
		    *pnt = fbh_addfbifs(*pnt,$2.LIST_ABL->DATA);
                    ((struct fbifs *)((*pnt)->DATA))->LINE_NUM = FBL_LINNUM;
	            FBL_NM1LST = addchain(FBL_NM1LST,&(((struct fbifs*)(*pnt)->DATA)->CNDTRUE));
		}
	  sequence_of_statements
		{
		    struct ptype **pnt;
		    struct ptype *ptype;
		    pnt = (struct ptype**)FBL_NM1LST->DATA;
		    ptype = reversetype(*pnt); 
		    *pnt = ptype; 
                  FBL_NM1LST = delchain (FBL_NM1LST, FBL_NM1LST);
		    pnt = (struct ptype**)FBL_NM1LST->DATA;
	            FBL_NM1LST = addchain(FBL_NM1LST,&(((struct fbifs*)(*pnt)->DATA)->CNDFALSE));	
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
		    pnt = (struct ptype**)FBL_NM1LST->DATA;
		    *pnt = fbh_addfbifs(*pnt,$3.LIST_ABL->DATA);
                    ((struct fbifs *)((*pnt)->DATA))->LINE_NUM = FBL_LINNUM;
	            FBL_NM1LST = addchain(FBL_NM1LST,(void*)&((struct fbifs*)(*pnt)->DATA)->CNDTRUE);	
		}
	  sequence_of_statements
		{
		    struct ptype **pnt;
		    struct ptype *ptype;
		    pnt = (struct ptype**)FBL_NM1LST->DATA;
		    ptype = reversetype(*pnt); 
		    *pnt = ptype; 
                    FBL_NM1LST = delchain (FBL_NM1LST, FBL_NM1LST);
		    pnt = (struct ptype**)FBL_NM1LST->DATA;
                    FBL_NM1LST = delchain (FBL_NM1LST, FBL_NM1LST);
	            FBL_NM1LST = addchain(FBL_NM1LST,&((struct fbifs*)(*pnt)->DATA)->CNDFALSE);	
		yyerrok; }
	;

.ELSE__seq_of_stmts.
	: /*empty*/
		{ 
                  FBL_NM1LST = delchain (FBL_NM1LST, FBL_NM1LST);
		  yyerrok; 
		}
	| ELSE
	  sequence_of_statements
		{ 
		    struct ptype *ptype;
		    struct ptype **pnt;
		    pnt = (struct ptype**)FBL_NM1LST->DATA;
		    ptype = reversetype(*pnt); 
		    *pnt = ptype; 
                  FBL_NM1LST = delchain (FBL_NM1LST, FBL_NM1LST);
		  yyerrok; 
		}
	;

case_statement
	: CASE
	  expression
	 	{	
		    struct ptype **pnt;

		    pnt = (struct ptype**)FBL_NM1LST->DATA;
		    
		    /*fbcas = (struct fbcas *)mbkalloc(sizeof(struct fbcas));
		    fbcas->CHOICE = 0;
		    fbcas->USER   = 0;		
		    fbcas->LINE_NUM   = 0;		
		    fbcas->SIZE   = 0;		
		    fbcas->TYPE = $2.TYPE;
		    fbcas->ABL = $2.LIST_ABL->DATA;*/
		    FBL_OTHPNT  = 0;	
		*pnt = fbh_addfbcas(*pnt,$2.LIST_ABL->DATA,$2.TYPE);	
                    ((struct fbcas *)((*pnt)->DATA))->LINE_NUM = FBL_LINNUM;
	            FBL_NM1LST = addchain(FBL_NM1LST,(void*)&(((struct fbcas*)((*pnt)->DATA))->SIZE));
	            FBL_NM1LST = addchain(FBL_NM1LST,(void*)&(((struct fbcas*)(*pnt)->DATA)->CHOICE));
		} 
	  IS
	  case_statement_alternative
	  ..case_statement_alternative..
	  END_ERR
	  CASE
	  Semicolon_ERR
		{
		unsigned long size=1;
		unsigned long *size1;
		long indice=0;
		struct choice_chain *ch;
                struct fbcho **pnt;
                struct fbcho *tab = NULL;
		struct choice_chain *nm1lst;
 

		nm1lst = FBL_CASPNT;
                while (nm1lst != 0)
		  {
			size++;
			nm1lst = nm1lst->NEXT;
	  	  }
		pnt = (struct fbcho**)FBL_NM1LST->DATA;
		  FBL_NM1LST = delchain(FBL_NM1LST,FBL_NM1LST);
		if( size != 0)
		  tab  = (struct fbcho*)mbkalloc(size*sizeof(struct fbcho));
		 while(FBL_CASPNT)
		 {
		  tab[indice].INSTRUCTION = reversetype(FBL_CASPNT->INSTRUCTION);
		  tab[indice].SIZE = FBL_CASPNT->SIZE;
		  tab[indice++].VALUE = FBL_CASPNT->VALUE;
		  ch = FBL_CASPNT;
		  FBL_CASPNT =FBL_CASPNT->NEXT;
		  /*free(ch);*/
		  }
		  tab[indice].INSTRUCTION = reversetype(FBL_OTHPNT);
		  tab[indice].SIZE = -1;
		  tab[indice++].VALUE = namealloc("others");
		  (*pnt) = tab;
		  size1 = (unsigned long*) FBL_NM1LST->DATA;
		  *size1 = size;
		  FBL_NM1LST = delchain(FBL_NM1LST,FBL_NM1LST);
		  FBL_OTHPNT = 0;
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
	  Arrow { 
		 FBL_PTYPE = 0;
		 FBL_NM1LST = addchain(FBL_NM1LST,(void*)&FBL_PTYPE);
		 }
	  sequence_of_statements
		{ 
		 if( FBL_CNDLST == 0)
                 {
		     fbl_error(85,"case");
                 }
		 else if ( FBL_CNDLST->DATA == 0)
                 {
		     FBL_OTHPNT = FBL_PTYPE ;
                     while (FBL_CNDLST )
                     {
		       FBL_CNDLST = delchain(FBL_CNDLST,FBL_CNDLST);
                     }
                 }
		 else 
		 while(FBL_CNDLST)
		 {
		  char* val;
		  val = (char *)FBL_CNDLST->DATA;
		  FBL_CASPNT = order_choice(FBL_CASPNT,FBL_PTYPE,val,strlen(val));
		  FBL_CNDLST = delchain(FBL_CNDLST,FBL_CNDLST);
		 }
		  FBL_NM1LST = delchain(FBL_NM1LST,FBL_NM1LST);
		}
	;

/* DEBUT LUDO */

...waveform__WHEN__choices..
	: /*empty*/
	  {
            $$ = FBL_EMPSTR;
          }
	| ...waveform__WHEN__choices..
	  Comma
	  waveform__WHEN__choices
		{ 
                  $$ = fbl_crtabl (ABL_OR,$1,$3,-1,-1);
                }
	;

waveform__WHEN__choices
	: waveform
	  WHEN
	  choices2
		{
		  fbl_ablstr expr1;

		fbl_select (&expr1,FBL_NM1LST,&FBL_BDDPNT,&FBL_SLCEXP);
		freechain (FBL_NM1LST);
		FBL_NM1LST = NULL;

		$$ = fbl_crtabl(ANDM,$1,expr1,-1,-1);
		}
	;

choices2
	: choice
		{
                  FBL_NM1LST = addchain (NULL ,$1.NAME);
                }
	  ..Bar__choice2..
		{ yyerrok; }
	;

..Bar__choice2..
	: /*empty*/
	| ..Bar__choice2..
	  Bar
	  choice
		{
          if (!strncmp($3.NAME,"others",7))
                {
		  fbl_error (30,NULL);
                }
		FBL_NM1LST = addchain (FBL_NM1LST ,$3.NAME);
		fprintf( stdout, "%s%d: %s\n", basename(__FILE__),__LINE__,$3.NAME );
		}
	;


/* FIN LUDO */

waveform
	: waveform_element
		{ $$ = $1; }
	;

choices
	: choice
                {
		 if (strcmp($1.NAME , "others"))
                 	FBL_CNDLST = addchain (0 ,(void*)$1.NAME); 
		 else 
                 	FBL_CNDLST = addchain (0 ,0); 
		}
	  ..Bar__choice..
		{ yyerrok; }
	;

null_statement
        : tok_NULL
          Semicolon_ERR
        ;

..Bar__choice..
	: /*empty*/
	| ..Bar__choice..
	  Bar
	  choice
                {
                if (!strcmp($3.NAME , "others")  || FBL_CNDLST->DATA == 0)
                {
                  fbl_error (31,0);
                }
                FBL_CNDLST = addchain (FBL_CNDLST ,(void*)$3.NAME);
                }
	;

choice
	: literal 
		{
                  $$.NAME  = $1;
		  $$.RIGHT = -1;
		  $$.TYPE  = -1;
		  $$.LEFT  = -1;}
	| OTHERS
		{
		  $$.RIGHT = -1;
		  $$.TYPE  = -1;
		  $$.LEFT  = -1;
		  $$.NAME  = namealloc("others");}
	| name
                {
                char           *val;
                char            val2[256];
                long             left;
                long             right;
                long             in_bound;
                long             out_bound;
                long             left_bnd;
                long             right_bnd;
                long             sig_conf;
 
                strcpy (val2,"B\"");
                sig_conf = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_SIGDFN);
                val      = (char *) chktab (hshtab,$1.NAME,0,FBL_PNTDFN);
                if (sig_conf == 0)
                {
                  fbl_error (17,$1.NAME);
                }
                else
                  {
                  if (sig_conf != FBL_CSTDFN)
                  {
                    fbl_error (76,$1.NAME);
                  }
                  }
 
                left_bnd  = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_WMNDFN);
                right_bnd = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_WMXDFN);
 
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
                      fbl_error (36,$1.NAME);
                    }
                    }
                  else
                    {
                    in_bound = left - right_bnd;
                    out_bound = right - right_bnd;
                    if ((left > right_bnd) || (right < left_bnd))
                    {
                      fbl_error (36,$1.NAME);
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
                      fbl_error (36,$1.NAME);
                    }
                    }
                  else
                    {
                    in_bound = right - right_bnd;
                    out_bound = left - right_bnd;
                    if ((left > left_bnd) || (right < right_bnd))
                    {
                      fbl_error (36,$1.NAME);
                    }
                    }
                  }
		if( chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_TYPDFN) == -1) 
		{
                  fbl_tobin (&val2[2],val,in_bound,out_bound);
                  strcat (val2,"\"");
                  $$.NAME = namealloc (val2);
                  $$.LEFT = -1;
                  $$.RIGHT = -1;
                  $$.TYPE = -1;
		}
		else 
		 {
                  $$.LEFT = $1.LEFT;
                  $$.RIGHT = $1.RIGHT;
                  $$.TYPE = $1.TYPE;
		  /* sig_conf = chktab(hshtab,$1.NAME,FBL_MODNAM,FBL_LBLDFN) ;
		      $$.NAME = (char *)fbl_numtobin(sig_conf); */
			$$.NAME = $1.NAME;
		 }
		}
	;

condition 
        : expression
                { $$ = $1; }
        ;

.REPORT__expression.
	:  /*empty*/
	 { $$ = 0;}
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
	  { fbl_stostr($1);}
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
	| relation.NAND_NOR__relation.
                { $$ = $1; }
	| relation..XOR__relation..
                { $$ = $1; }
	;

relation..AND__relation..
	: relation
	  tok_AND
	  relation
                { $$ = fbl_crtabl (ABL_AND ,$1 ,$3 ,-1,-1); }
	| relation..AND__relation..
	  tok_AND
	  relation
                { $$ = fbl_crtabl (ABL_AND ,$1 ,$3 ,-1,-1);} 
	;

relation..OR__relation..
	: relation
	  _OR
	  relation
                { $$ = fbl_crtabl (ABL_OR ,$1 ,$3 ,-1,-1); }
	| relation..OR__relation..
	  _OR
	  relation
                { $$ = fbl_crtabl (ABL_OR ,$1 ,$3 ,-1,-1); }
	;

relation.NAND_NOR__relation.
	: relation
                { $$ = $1; }
	| relation
	  _NAND
	  relation
                { $$ = fbl_crtabl (ABL_NAND ,$1 ,$3 ,-1,-1); }
	| relation
	  _NOR
	  relation
                { $$ = fbl_crtabl (ABL_NAND ,$1 ,$3 ,-1,-1); }
	;

relation..XOR__relation..
	: relation
	  _XOR
	  relation
                { $$ = fbl_crtabl (ABL_XOR ,$1 ,$3 ,-1,-1); }
	| relation..XOR__relation..
	  _XOR
	  relation
                { $$ = fbl_crtabl (ABL_XOR ,$1 ,$3 ,-1,-1); }
	;

relation
	: simple_expression
                { $$ = $1; }
	| simple_expression
	  relational_operator
	  simple_expression
                { $$ = fbl_crtabl ($2 ,$1 ,$3 ,-1,-1); }
	;

simple_expression
	: .sign.term..add_op__term..
                { $$ = $1; }
	;

.sign.term..add_op__term..
	: term
                { $$ = $1; }
	| .sign.term..add_op__term..
	  Ampersand
	  term
                { $$ = fbl_crtabl (CONC,$1,$3,-1,-1); }
	;

term
	: factor
                { $$ = $1; }
	;

factor
	: primary
                { $$ = $1; }
	| _NOT
	  primary
              { $$ = fbl_crtabl (ABL_NOT,$2,FBL_EMPSTR,-1,-1); }
	;

primary
	: literal
                {
                struct fbl_expr expr1;
 
                expr1.IDENT = $1;
                expr1.TYPE = -1;
                $$ = fbl_crtabl (NOPS,expr1,FBL_EMPSTR,-1,-1);
                }
        | aggregate
                { $$ = $1; }
        | name
                {
                struct fbl_expr expr1;
                long             left;
                long             right;
                long             left_bnd;
                long             right_bnd;
                long             in_bound;
                long             out_bound;
                long             mode;
		long  prtype;
		long  type;

                mode = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_SIGDFN);
		type = chktab(hshtab,$1.NAME,FBL_MODNAM,FBL_TYPDFN);
		if( mode == FBL_OCNDFN)
		{
                  {
                    fbl_error (26,$1.NAME);
                  }
	            $$.IDENT = 0;	
	            $$.LIST_ABL = 0;	
	            $$.WIDTH = 0;	
	            $$.TYPE = 0;	
		}
		else if( type == 0 || mode == FBL_TPEDFN)
		{
                  {
                    fbl_error (17,$1.NAME);
                  }
	            $$.IDENT = 0;	
	            $$.LIST_ABL = 0;	
	            $$.WIDTH = 0;	
	            $$.TYPE = 0;	
		}
	       else
               { 
                left_bnd  = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_WMNDFN);
                right_bnd = chktab (hshtab,$1.NAME,FBL_MODNAM,FBL_WMXDFN);
 
                left  = $1.LEFT;
                right = $1.RIGHT;
 
                if (left_bnd <= right_bnd)
                  {
                  if (left <= right)
                    {
                    in_bound  = left  - left_bnd;
                    out_bound = right - left_bnd;
                    if ((left < left_bnd) || (right > right_bnd))
                    {
                      fbl_error (36,$1.NAME);
                    }
                    }
                  else
                    {
                    in_bound  = right - left_bnd;
                    out_bound = left  - left_bnd;
                    if ((left > right_bnd) || (right < left_bnd))
                    {
                      fbl_error (36,$1.NAME);
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
                      fbl_error (36,$1.NAME);
                    }
                    }
                  else
                    {
                    in_bound  = right - right_bnd;
                    out_bound = left  - right_bnd;
                    if ((left > left_bnd) || (right < right_bnd))
                    {
                      fbl_error (36,$1.NAME);
                    }
                    }
                  }

                /* LUDO
                if ( mode == FBL_VARDFN)
                  {
                  expr1.IDENT = 0;
                  expr1.TYPE = chktab(hshtab,$1.NAME,FBL_MODNAM,FBL_TYPDFN);

                  expr1.LIST_ABL = (struct chain *)chktab(hshtab,$1.NAME,FBL_MODNAM,FBL_PNTDFN);
                  $$ = fbl_crtabl (SUBVAR,expr1,FBL_EMPSTR,in_bound,out_bound);
                  }
                else
                */
                if ((mode != FBL_CSTDFN)||
		   (chktab(hshtab,$1.NAME,FBL_MODNAM,FBL_TYPDFN) != -1))
                  {
                  expr1.IDENT = $1.NAME;
		prtype = chktab(hshtab,$1.NAME,FBL_MODNAM,FBL_USRDFN);
		if(val_type((FBL_BEFPNT->BETYP + type -1)->NAME) !=-1)
		  expr1.TYPE = chktab(hshtab,namealloc("bit"),FBL_MODNAM,FBL_TYPDFN);
		else
		  expr1.TYPE = type;
              if($1.FLAG == 'X')
                        {
                        expr1 = fbl_crtabl (NOPI,expr1,FBL_EMPSTR,left,right);  
                        $$    = fbl_crtabl (ABL_STABLE,expr1,FBL_EMPSTR,left,right);   
                        }
              else
              if($1.FLAG == 'x')
              {
                        expr1 = fbl_crtabl (NOPI,expr1,FBL_EMPSTR,left,right);  
                        expr1 = fbl_crtabl (ABL_STABLE,expr1,FBL_EMPSTR,left,right);   
                        $$    = fbl_crtabl (ABL_NOT,expr1,FBL_EMPSTR,left,right);   
              }
		  else
                  {
                  $$ = fbl_crtabl (NOPI,expr1,FBL_EMPSTR,left,right);
                  }
                 }
                else
                 {
                 expr1.IDENT = (char *)chktab(hshtab,$1.NAME,0,FBL_PNTDFN);
		 expr1.TYPE =(long)-1;/*dec, cast long */
                  $$ = fbl_crtabl (NOPS,expr1,FBL_EMPSTR,in_bound,out_bound);
                 }
		}
               }
	;

relational_operator
	: _EQSym
		{ $$ = EQ; }
	| _NESym
		{ $$ = NE; }
	| _GESym
		{ $$ = GE; }
	| _GTSym
		{ $$ = GT; }
	| _LESym
		{ $$ = LE; }
	| _LTSym
		{ $$ = LT; }
	;

literal
	: CharacterLit
		{ $$ = $1; }
	| StringLit
		{ $$ = $1; }
	| BitStringLit
		{ $$ = $1; }
	;

aggregate
	: LeftParen
	  expression
	  RightParen
		{ $$ = $2; }
	;

name
	: simple_name
               {
                $$.NAME  = $1;
                $$.TYPE  = chktab (hshtab,$1,FBL_MODNAM,FBL_TYPDFN);
              	$$.LEFT  = chktab (hshtab,$1,FBL_MODNAM,FBL_WMNDFN);
                $$.RIGHT = chktab (hshtab,$1,FBL_MODNAM,FBL_WMXDFN);
                }
	| indexed_name
		{ $$ = $1; }
	| slice_name
		{ $$ = $1; }
        | attribute_name
                { $$ = $1; }
	;

indexed_name
	: simple_name
	  LeftParen
	  abstractlit
	  RightParen_ERR
                {
                $$.NAME  = $1;
                $$.LEFT  = $3;
                $$.TYPE  = chktab (hshtab,$1,FBL_MODNAM,FBL_TYPDFN);
                $$.RIGHT = $3;
                }
	;

slice_name
	: simple_name
	  LeftParen
	  abstractlit
	  direction
	  abstractlit
	  RightParen_ERR
                {
                if ((($3 > $5) && ($4 == FBL_UPTDFN)) ||
                    (($3 < $5) && ($4 == FBL_DWTDFN)))
                {
                  fbl_error (32,$1);
                }
 
                $$.NAME  = $1;
                $$.TYPE  = chktab (hshtab,$1,FBL_MODNAM,FBL_TYPDFN);
                $$.LEFT  = $3;
                $$.RIGHT = $5;
                }
	;

attribute_name
        : simple_name
          Apostrophe
          attribute_designator
                {
                char             extname[100];
                char            *lclname;
                long              sig_conf;
                long              type;
		long              kind;
                struct fbl_expr  expr1;
                struct fbl_expr  expr2;
                struct chain    *ptabl;

                sprintf (extname,"%s'delayed",$1);
                lclname = namealloc (extname);

                if (chktab (hshtab,lclname,FBL_MODNAM,FBL_SIGDFN) == 0 )
                {
		sig_conf = chktab (hshtab,$1,FBL_MODNAM,FBL_SIGDFN);
		switch (sig_conf)
		  {
		  case (FBL_ICNDFN + FBL_BITDFN + FBL_NORDFN) :
		  case (FBL_BCNDFN + FBL_BITDFN + FBL_NORDFN) :
		  case (FBL_BCNDFN + FBL_MUXDFN + FBL_BUSDFN) :
		  case (FBL_BCNDFN + FBL_WORDFN + FBL_BUSDFN) :
		  case (FBL_BITDFN + FBL_NORDFN):
		  case (FBL_MUXDFN + FBL_BUSDFN):
		  case (FBL_WORDFN + FBL_BUSDFN):
		  case (FBL_RBIDFN + FBL_REGDFN):
		    break;
		  default :
                    {
		    fbl_error (79,$1);
                    }
		  }
                type = chktab (hshtab,$1,FBL_MODNAM,FBL_TYPDFN);
                kind = chktab (hshtab,$1,FBL_MODNAM,FBL_LBLDFN);
 
                if (chktab (hshtab,$1,FBL_MODNAM,FBL_WMNDFN) != -1)
                {
                  fbl_error (79,$1);
                }

                if (FBL_ERRFLG == 0)
                  {
                    expr1.IDENT = $1;
                    expr1.WIDTH  = 1;
                    expr2 = fbl_crtabl (NOPI,expr1,FBL_EMPSTR,-1,-1);
                    ptabl = (struct chain*)expr2.LIST_ABL->DATA;
                    FBL_BEFPNT->BEDLY = fbh_addfbaux(FBL_BEFPNT->BEDLY,lclname,
                                                     ptabl,0,type);
                    FBL_BEFPNT->BEDLY->LINE_NUM = FBL_LINNUM;
                    /* LUDO addtab (hshtab,$1,FBL_MODNAM,FBL_STBDFN,1); */
                    addtab (hshtab,lclname,FBL_MODNAM,FBL_WMNDFN,-1);
                    addtab (hshtab,lclname,FBL_MODNAM,FBL_WMXDFN,-1);
                    addtab (hshtab,lclname,FBL_MODNAM,FBL_TYPDFN, type);
                    addtab (hshtab,lclname,FBL_MODNAM,FBL_LBLDFN, kind);
                    addtab (hshtab,lclname,FBL_MODNAM,FBL_SIGDFN,
                    (FBL_ICNDFN + FBL_BITDFN + FBL_NORDFN));
                    }
                 }
                $$.NAME = $1;
                $$.LEFT = -1;
                $$.RIGHT = -1;
                if ( $3 == FBL_STABLE ) $$.FLAG  = 'X';
                else                    $$.FLAG  = 'x';
             }
        ;
 
attribute_designator
        : _STABLE { $$ = FBL_STABLE; }
        | _EVENT  { $$ = FBL_EVENT;  }
        ;

identifier_list
	: Identifier
                { FBL_NM1LST = addchain (FBL_NM1LST,$1); }
	  ...identifier..
	;

...identifier..
	: /*empty*/
	| ...identifier..
	  Comma
	  Identifier
                { FBL_NM1LST = addchain (FBL_NM1LST,$3); }
	;

.label.
	: /*empty*/
		{ $$ = 0; }
	| label
		{ $$ = $1; }
	;

/* DEBUT LUDO */

.guard_expression.
	: /*empty*/
		{ $$ = NULL; }
	| guard_expression
		{ $$ = $1; }
	;

guard_expression
	: LeftParen
	  expression
	  RightParen_ERR
		{ $$ = (char *)$2.LIST_ABL->DATA; }
	;

.GUARDED.
	: /*empty*/
		{ $$ = FBL_UNGDFN ; }
	| GUARDED
		{ $$ = FBL_GRDDFN; }
	;

/* FIN LUDO */

.simple_name.
	: /*empty*/
		{ $$ = 0; }
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
	;

a_label
	: label
	  Colon
                {
                FBL_LBLNAM = $1;
                $$ = $1;
                }
	;

label
	: Identifier
		{ $$ = $1; }
	;

abstractlit
	: AbstractLit
		{ $$ = atoi($1); }
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
  long              i;

  if (FBL_DCEHED == 0)
    {
    FBL_DCEHED = (struct dct_entry *)
                 mbkalloc (sizeof(struct dct_entry) * FBL_ALODFN);

    entry = FBL_DCEHED;
    for (i=1 ; i<FBL_ALODFN ; i++)
      {
      entry->next = entry + 1;
      entry++;
      }
    entry->next = 0;
    }

  entry       = FBL_DCEHED;
  FBL_DCEHED  = FBL_DCEHED->next;

  entry->next = head;
  entry->data = 0;
  entry->key  = key;

  return (entry);
  }

static struct dct_recrd *addrcd (head , key)

struct dct_recrd *head;
char             *key;

  {
  struct dct_recrd *recrd;
  long               i;

  if (FBL_DCRHED == 0)
    {
    FBL_DCRHED = (struct dct_recrd *)
                 mbkalloc (sizeof(struct dct_recrd) * FBL_ALODFN);

    recrd = FBL_DCRHED;
    for (i=1 ; i<FBL_ALODFN ; i++)
      {
      recrd->next = recrd + 1;
      recrd++;
      }
    recrd->next = 0;
    }

  recrd           = FBL_DCRHED;
  FBL_DCRHED      = FBL_DCRHED->next;

  recrd->next     = head;
  recrd->fd0_val  = 0;
  recrd->fd1_val  = 0;
  recrd->fd2_val  = 0;
  recrd->fd3_val  = 0;
  recrd->fd4_val  = 0;
  recrd->fd5_val  = 0;
  recrd->fd6_val  = 0;
  recrd->pnt_val  = 0;
  recrd->key      = key;

  return (recrd);
  }

static struct dct_entry **initab ()

  {
  struct dct_entry **head;
  long                i;

  head = (struct dct_entry **)
         mbkalloc (sizeof(struct dct_entry *) * FBL_HSZDFN);

  for (i=0 ; i<FBL_HSZDFN ; i++)
    head[i] = 0;

  return (head);
  }

static void addtab (head,key_str,ctx_str,field,valu)

struct dct_entry **head;
char              *key_str;
char              *ctx_str;
long                field;
long               valu;

  {
  long               found = 0;
  long               index;
  struct dct_entry *entry_pnt;
  struct dct_recrd *recrd_pnt;

  index     = ((unsigned long) key_str) % FBL_HSZDFN;
  entry_pnt = head[index];

  while (entry_pnt != 0)
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
  while (recrd_pnt != 0)
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
    case 0 :
      recrd_pnt->fd0_val = valu;
      break;
    case 1 :
      recrd_pnt->fd1_val = valu;
      break;
    case 2 :
      recrd_pnt->fd2_val = valu;
      break;
    case 3 :
      recrd_pnt->fd3_val = valu;
      break;
    case 4 :
      recrd_pnt->fd4_val = valu;
      break;
    case 5 :
      recrd_pnt->fd5_val = valu;
      break;
    case 6 :
      recrd_pnt->fd6_val = valu;
      break;
    case 7 :
      recrd_pnt->pnt_val = valu;
      break;
    }

  }

static long chktab (head,key_str,ctx_str,field)

struct dct_entry **head;
char              *key_str;
char              *ctx_str;
long                field;

  {
  long               found = 0;
  long               valu = 0;
  struct dct_entry *entry_pnt;
  struct dct_recrd *recrd_pnt;

  entry_pnt = head [((unsigned long)key_str) % FBL_HSZDFN];

  while (entry_pnt != 0)
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
    while (recrd_pnt != 0)
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
        case 0 :
          valu = recrd_pnt->fd0_val;
          break;
        case 1 :
          valu = recrd_pnt->fd1_val;
          break;
        case 2 :
          valu = recrd_pnt->fd2_val;
          break;
        case 3 :
          valu = recrd_pnt->fd3_val;
          break;
        case 4 :
          valu = recrd_pnt->fd4_val;
          break;
        case 5 :
          valu = recrd_pnt->fd5_val;
          break;
        case 6 :
          valu = recrd_pnt->fd6_val;
          break;
        case 7 :
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
  long               i;

  if (pt_hash != 0)
    {
    for (i=0 ; i<FBL_HSZDFN ; i++)
      {
      if ((pt_entry = pt_hash[i]) != 0)
        {
        while (pt_entry != 0)
          {
          pt_record = pt_entry->data;

          while (pt_record->next != 0)
            pt_record = pt_record->next;

          pt_record->next = FBL_DCRHED;
          FBL_DCRHED      = pt_entry->data;

          pt_nxtentry     = pt_entry->next;
          pt_entry->next  = FBL_DCEHED;
          FBL_DCEHED      = pt_entry;
          pt_entry        = pt_nxtentry;
          }
        }
      }
    free (pt_hash);
    }
  }

void *fbl_addstr (ptfig,object,mode,prtype,type,flag,name,left,right)

struct fbfig *ptfig;
char          object;
long           mode;
long           prtype;
unsigned char type;
char          flag;
char         *name;
short         left;
short         right;

{
  void  *pnt    = 0;
  char   porflg = 0;
  char   rinflg = 0;
  char   outflg = 0;
  char   busflg = 0;
  char   auxflg = 0;
  char   buxflg = 0;
  char   regflg = 0;
  char   lclmod = 'B';
  char   lcltyp = type;
  char   extname[100];
  short  i;
  short  inc = 1;

  switch (object)
    {
    case 'P':

	/* ###------------------------------------------------------### */
	/*   if object is a port ...					*/
	/* ###------------------------------------------------------### */

      porflg = 1;
      switch (mode)
        {
        case FBL_ICNDFN:
          lclmod = 'I'; lcltyp = type ; rinflg = 1; break;
        case FBL_OCNDFN:
          switch (prtype)
            {
            case BIT :
              lclmod = 'O'; lcltyp = type ; outflg = 1; break;
            case MUX_BIT :
              lclmod = 'Z'; lcltyp = type; busflg = 1; break;
            case WOR_BIT :
              lclmod = 'Z'; lcltyp = type; busflg = 1; break;
            }
          break;
        case FBL_BCNDFN:
          rinflg = 1;
          switch (prtype)
            {
            case BIT:
              lclmod = 'B'; lcltyp = type; outflg = 1; rinflg =1;break;
            case MUX_BIT:
              lclmod = 'T'; lcltyp = type; busflg = 1;rinflg =1; break;
            case WOR_BIT:
              lclmod = 'T'; lcltyp = type; busflg = 1; rinflg =1;break;
            }
          break;
        }
      break;

    case 'S':

	/* ###------------------------------------------------------### */
	/*   if object is a signal ...					*/
	/* ###------------------------------------------------------### */

      switch (prtype)
        {
        case BIT:
          lcltyp = type; auxflg = 1; break;
        case REG_BIT :
          rinflg = 1; regflg = 1; break;
        case MUX_BIT :
          lcltyp = type; rinflg = 1; buxflg = 1; break;
        case WOR_BIT :
          lcltyp = type; rinflg = 1; buxflg = 1; break;
	default : lcltyp = type ; auxflg =1;break;
        }
      break;
    }

  if (flag == 'A' || flag == 'U')
    {
	/* ###------------------------------------------------------### */
	/*   if object is an array ...					*/
	/* ###------------------------------------------------------### */

    if (left >= right)
      inc = -1;

    for (i=left ; i!=(right+inc) ; i+=inc)
      {
      sprintf (extname,"%s %d",name,i);

      if (porflg == 1)
      {
        ptfig->BEPOR = fbh_addfbpor (ptfig->BEPOR,extname,lclmod,lcltyp);
        ptfig->BEPOR->LINE_NUM = FBL_LINNUM;
      }
      if (rinflg == 1)
      {
        ptfig->BERIN = fbh_addfbrin (ptfig->BERIN,extname);
        ptfig->BERIN->LINE_NUM = FBL_LINNUM;
      }
      if (outflg == 1)
      {
        ptfig->BEOUT = fbh_addfbout (ptfig->BEOUT,extname,0,0,lcltyp);
        ptfig->BEOUT->LINE_NUM = FBL_LINNUM;
      }
      if (busflg == 1)
      {
        ptfig->BEBUS = fbh_addfbbus (ptfig->BEBUS,extname,0,0,lcltyp);
        ptfig->BEBUS->LINE_NUM = FBL_LINNUM;
      }
      if (auxflg == 1)
      {
        ptfig->BEAUX = fbh_addfbaux (ptfig->BEAUX,extname,0,0,lcltyp);
        ptfig->BEAUX->LINE_NUM = FBL_LINNUM;
      }
      if (buxflg == 1)
      {
        ptfig->BEBUX = fbh_addfbbux (ptfig->BEBUX,extname,0,0,lcltyp);
        ptfig->BEBUX->LINE_NUM = FBL_LINNUM;
      }
      if (regflg == 1)
      {
        ptfig->BEREG = fbh_addfbreg (ptfig->BEREG,extname,0,0);
        ptfig->BEREG->LINE_NUM = FBL_LINNUM;
      }
      }
    }
  else
    {
	/* ###------------------------------------------------------### */
	/*   if object is a scalar ...					*/
	/* ###------------------------------------------------------### */

    if (porflg == 1)
    {
      ptfig->BEPOR = fbh_addfbpor (ptfig->BEPOR,name,lclmod,lcltyp);
      ptfig->BEPOR->LINE_NUM = FBL_LINNUM;
    }
    if (rinflg == 1)
    {
      ptfig->BERIN = fbh_addfbrin (ptfig->BERIN,name);
      ptfig->BERIN->LINE_NUM = FBL_LINNUM;
    }
    if (outflg == 1)
    {
      ptfig->BEOUT = fbh_addfbout (ptfig->BEOUT,name,0,0,lcltyp);
      ptfig->BEOUT->LINE_NUM = FBL_LINNUM;
    }
    if (busflg == 1)
    {
      ptfig->BEBUS = fbh_addfbbus (ptfig->BEBUS,name,0,0,lcltyp);
      ptfig->BEBUS->LINE_NUM = FBL_LINNUM;
    }
    if (auxflg == 1)
    {
      ptfig->BEAUX = fbh_addfbaux (ptfig->BEAUX,name,0,0,lcltyp);
      ptfig->BEAUX->LINE_NUM = FBL_LINNUM;
    }
    if (buxflg == 1)
    {
      ptfig->BEBUX = fbh_addfbbux (ptfig->BEBUX,name,0,0,lcltyp);
      ptfig->BEBUX->LINE_NUM = FBL_LINNUM;
    }
    if (regflg == 1)
    {
      ptfig->BEREG = fbh_addfbreg (ptfig->BEREG,name,0,0);
      ptfig->BEREG->LINE_NUM = FBL_LINNUM;
    }

    }

  if (outflg == 1)
  {
    pnt = (void *) ptfig->BEOUT;
  }
  if (busflg == 1)
  {
    pnt = (void *) ptfig->BEBUS;
  }
  if (auxflg == 1)
  {
    pnt = (void *) ptfig->BEAUX;
  }
  if (buxflg == 1)
  {
    pnt = (void *) ptfig->BEBUX;
  }
  if (regflg == 1)
  {
    pnt = (void *) ptfig->BEREG;
  }

  return (pnt);
}

long fbl_chkdcl (object,mode,type,flag,kind,constraint,conf)

char object;
long  mode;
long  type;
char flag;
long  kind;
char constraint;
long  *conf;

{
  long errflg = 0;
  long lclcnf = 0;

  if (flag != constraint && constraint !='U'  && flag !='U')
    {
    errflg = 1;
    {
    fbl_error (33,0);
    }
    }
  else
    {
    switch (object)
      {
      case 'P':
	/* ###------------------------------------------------------### */
	/*    If object is a port :					*/
	/*       - if type is bit, no guard indication can be used	*/
	/*       - if type is wor_bit or mux_bit, bus kind must be used	*/
	/*       - other types are illegal				*/
	/* ###------------------------------------------------------### */

        switch (type)
          {
          case BIT :

            lclcnf += FBL_BITDFN + FBL_NORDFN;
            switch (mode)
              {
              case _IN :
                lclcnf += FBL_ICNDFN; break;
              case _OUT:
                lclcnf += FBL_OCNDFN; break;
              case _INOUT :
                lclcnf += FBL_BCNDFN; break;
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != 0)
              errflg = 1;
            break;

          case MUX_BIT :

            lclcnf += FBL_MUXDFN + FBL_BUSDFN;
            switch (mode)
              {
              case _OUT :
                lclcnf += FBL_OCNDFN; break;
              case _INOUT :
                lclcnf += FBL_BCNDFN; break;
              case _IN :
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != BUS)
              errflg = 1;
            break;

          case WOR_BIT :

            lclcnf += FBL_WORDFN + FBL_BUSDFN;
            switch (mode)
              {
              case _OUT :
                lclcnf += FBL_OCNDFN; break;
              case _INOUT :
                lclcnf += FBL_BCNDFN; break;
              case _IN :
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != BUS)
              errflg = 1;
            break;

          case REG_BIT :
          case NATURAL:

            errflg = 1;
            break;
          }
        if (errflg == 1)
        {
          fbl_error (5,0);
        }
        break;

      case 'G':
	/* ###------------------------------------------------------### */
	/*    If object is a generic :					*/
	/*       - only natural type is allowed				*/
	/* ###------------------------------------------------------### */

        if ((type != NATURAL) || (mode != 0) || (kind != 0))
          {
          errflg = 1;
          {
          fbl_error (77,0);
          }
          }
        break;

      case 'S':
	/* ###------------------------------------------------------### */
	/*    If object is a signal :					*/
	/*       - no mode can be specified				*/
	/*       - if type is bit no guard indication can be used	*/
	/*       - if type is wor_bit or mux_bit, bus kind must be used	*/
	/*       - if type is reg_bit, register kind must be used	*/
	/*       - other types are illegal				*/
	/* ###------------------------------------------------------### */

       switch (type)
          {
          case BIT:
            lclcnf += FBL_BITDFN + FBL_NORDFN;
            if ((mode != 0) || (kind != 0))
              errflg = 1;
            break;
          case MUX_BIT:
            lclcnf += FBL_MUXDFN + FBL_BUSDFN;
            if ((mode != 0) || (kind != BUS))
              errflg = 1;
            break;
          case WOR_BIT:
            lclcnf += FBL_WORDFN + FBL_BUSDFN;
            if ((mode != 0) || (kind != BUS))
              errflg = 1;
            break;
          case REG_BIT:
            lclcnf += FBL_RBIDFN + FBL_REGDFN;
            if ((mode != 0) || (kind != REGISTER))
              errflg = 1;
            break;
          case NATURAL:
            errflg = 1; break;
          }

        if (mode != 0)
          errflg = 1;

        if (errflg == 1)
        {
          fbl_error (11,NULL);
        }
        break;

      case 'C':
	/* ###------------------------------------------------------### */
	/*    If object is a constant :					*/
	/*       - only bit type without any other indication is legal	*/
	/* ###------------------------------------------------------### */

        lclcnf += FBL_CSTDFN;
        if ((type != BIT) || (mode != 0) || (kind != 0))
          {
          errflg = 1;
          {
          fbl_error (78,NULL);
          }
          }
        break;
      }
    }

  *conf = lclcnf;
  return (errflg);
}

long val_type(name)
 char *name;
{
    if(!name)
	   return(-1);
	if (!strcmp(name,"severity_level"))
          return(SEVERITY_LEVEL);
	if (!strcmp(name,"boolean"))
             return(BOOLEAN);	
	if (!strcmp(name,"bit"))
             return(BIT);	
	if (!strcmp(name,"mux_bit"))
             return(MUX_BIT);	
	if (!strcmp(name,"wor_bit"))
             return(WOR_BIT);	
	if (!strcmp(name,"reg_bit"))
             return(REG_BIT);	
	if (!strcmp(name,"bit_vector"))
             return(BIT);
	if (!strcmp(name,"mux_vector"))
             return(MUX_BIT);	
	if (!strcmp(name,"wor_vector"))
             return(WOR_BIT);	
	if (!strcmp(name,"reg_vector"))
             return(REG_BIT);	
	if (!strcmp(name,"integer"))
             return(INTEGER);	
	if (!strcmp(name,"natural"))
             return(NATURAL);	
	if (!strcmp(name,"std_logic"))
             return(BIT);	
	if (!strcmp(name,"std_ulogic"))
             return(BIT);	
	if (!strcmp(name,"std_logic_vector"))
             return(BIT);
	if (!strcmp(name,"std_ulogic_vector"))
             return(BIT);
        return( -1 );
}
 struct ptype *reversetype(pnt)
 struct ptype *pnt;
  {
    struct ptype* revtype;
    struct ptype* aux2;
    struct ptype* aux1;

    if(pnt == 0 || pnt->NEXT == 0)
	return(pnt);
			revtype = pnt;
		     aux1 = pnt->NEXT;		
		     revtype->NEXT = 0;	
		  while(aux1)
		   {
		     aux2 = revtype;	
		     revtype = aux1;
		     aux1 = aux1->NEXT;		
		     revtype->NEXT = aux2;	
		   }
	return(revtype);
 }
	
 struct choice_chain *addchoice(lastpnt,type,val,size)
 struct choice_chain *lastpnt;
 struct ptype *type;
 char    *val;
 unsigned long size;
 {
    struct choice_chain *pnt;	

    pnt = (struct choice_chain*)mbkalloc(sizeof(struct choice_chain));
    pnt->VALUE = val;
    pnt->SIZE = size;
    pnt->INSTRUCTION  = type;
    pnt->NEXT   = lastpnt;
	return(pnt);
 }

 struct choice_chain *order_choice(pnt,type,val,size)
 struct choice_chain *pnt;
 struct ptype *type;
 char    *val;
 unsigned long size;
 {
    struct choice_chain *aux1;
    struct choice_chain *aux2;
   
      if(!pnt)
	{
    	 	aux1 = addchoice (pnt,type,val,size);
		return(aux1);
	}

	 if(strcmp(pnt->VALUE,val)>0) 
		{
	  aux1 =addchoice(pnt,type,val,size);
               return(aux1);
		}
	
    aux1 =pnt; 
    aux2 =pnt->NEXT; 
	while(aux2)
	{
	 if(strcmp(aux2->VALUE, val)>0) 
	  {
    	 	aux2 = addchoice (aux2,type,val,size);
		aux1->NEXT = aux2;
		return(pnt);
  	  }
	 aux1 = aux2;
	 aux2 = aux2->NEXT;
	}
    	 	aux2 = addchoice (aux2,type,val,size);
		aux1->NEXT = aux2;
		return(pnt);
 } 
          
