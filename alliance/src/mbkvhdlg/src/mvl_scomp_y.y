
/* ###--------------------------------------------------------------### */
/*                                                                      */
/* file         : mvl_scomp.yac                                         */
/* date         : Sep 24 1993                                           */
/* version      : v4.06                                                 */
/* author       : P. BAZARGAN, L.A. TABUSSE, VUONG H.N., D.HOMMAIS      */
/* content      : yacc rules for structural VHDL                        */
/* last modifie : Mar 7 1997                                            */
/*                                                                      */
/* ###--------------------------------------------------------------### */

%{
#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "mlo.h"
#include "mvl_stype.h"
#include "mvl_syacc.h"
#include "mvl_stdef.h"
#include "gen_generic.h"


static int           MVL_GENFLG = 0   ; /* Indicates if generic         */
static int           MVL_CONRNK = 0   ; /* rank of an implicit connexion*/
static int           MVL_SIGIDX = 1   ; /* structural's signal index    */
static int           MVL_SIMFLG = 0   ; /* simple flag                  */
static int           MVL_CONCNT = 0   ; /* connector's count            */
static char         *MVL_MODNAM ;       /* current model name           */
static char         *MVL_CHDNAM ;       /* child's model name           */
static char         *MVL_LOCNAM ;       /* instance's name              */
static struct chain *MVL_NM1LST = NULL; /* 1-st name liste              */
static struct chain *MVL_NM2LST = NULL; /* 2-nd name liste              */
static struct lofig *MVL_CHDPNT = NULL; /* current child pointer        */
static struct lofig *MVL_CHDHED = NULL; /* childs list                  */
static struct ptype *MVL_PTPPNT = NULL;
static struct chain *MVL_GENLST = NULL;
static struct logen *MVL_GENPNT = NULL;
static struct logen  MVL_LOGEN;
static long              base ;

%}
%union
  {
  int         valu;
  char       *text;
  char        flag;
  mvl_name    name;
  mvl_expr    expr;
  };

%token        Ampersand
%token        Apostrophe
%token        LeftParen
%token        RightParen
%token        DoubleStar
%token        Star
%token        Plus
%token        Comma
%token        Minus
%token        VarAsgn
%token        Colon
%token        Semicolon
%token        _LESym
%token        Box
%token        _LTSym
%token        Arrow
%token        _EQSym
%token        _GESym
%token        _GTSym
%token        Bar
%token        _NESym
%token        Dot
%token        Slash
%token <text> Identifier
%token        DecimalInt
%token        DecimalReal
%token <text> AbstractLit
%token        BasedInt
%token        BasedReal
%token <text> CharacterLit
%token <text> StringLit
%token <text> BitStringLit
%token        ABS
%token        ACCESS
%token        AFTER
%token        ALIAS
%token        ALL
%token        tok_AND
%token        ARCHITECTURE
%token        ARRAY
%token        ARG  
%token        ASSERT
%token        ATTRIBUTE
%token        _BEGIN
%token        BIT
%token        BIT_VECTOR
%token        BLOCK
%token        BODY
%token        BUFFER
%token        BUS
%token        CASE
%token        CLOCK
%token        COMPONENT
%token        CONFIGURATION
%token        CONSTANT
%token        CONVERT
%token        DISCONNECT
%token        DOWNTO
%token        ELSE
%token        ELSIF
%token        _END
%token        ENTITY
%token        ERROR
%token        _EXIT
%token        _FILE
%token        FOR
%token        FUNCTION
%token        GENERATE
%token        GENERIC
%token        GUARDED
%token        IF
%token        _IN
%token        _INOUT
%token        INTEGER
%token        IS
%token        _LABEL
%token        LIBRARY
%token        _LINKAGE
%token        _LIST
%token        LOOP
%token        MAP
%token        MOD
%token        MUX_BIT
%token        MUX_VECTOR
%token        _NAND
%token        NATURAL
%token        NATURAL_VECTOR
%token        NEW
%token        _NEXT
%token        _NOR
%token        _NOT
%token        tok_NULL
%token        OF
%token        ON
%token        OPEN
%token        _OR
%token        OTHERS
%token        _OUT
%token        _PACKAGE
%token        PORT
%token        POSITIVE
%token        PROCEDURE
%token        PROCESS
%token        RANGE
%token        RECORD
%token        REG_BIT
%token        REG_VECTOR
%token        REGISTER
%token        REM
%token        REPORT
%token        RETURN
%token        SELECT
%token        SEVERITY
%token        SIGNAL
%token        _STABLE
%token        STRING
%token        SUBTYPE
%token        THEN
%token        TO
%token        TRANSPORT
%token        _TYPE
%token        UNITS
%token        UNTIL
%token        USE
%token        VARIABLE
%token        WAIT
%token        WARNING
%token        WHEN
%token        WHILE
%token        WITH
%token        WOR_BIT
%token        WOR_VECTOR
%token        _XOR

%type  <valu> .mode.
%type  <valu> .local_port_mode.
%type  <valu>  type_mark
%type  <valu> .BUS.
%type  <text> .simple_name.
%type  <text>  simple_name
%type  <text>  a_label
%type  <name>  formal_port_name
%type  <expr>  actual_port_name
%type  <expr>  expression
%type  <expr>  relation
%type  <expr>  simple_expression
%type  <expr> .sign.term..add_op__term..
%type  <expr>  term
%type  <expr>  factor
%type  <expr>  primary
%type  <expr>  aggregate
%type  <expr>  type_convertion
%type  <name> .constraint.
%type  <name>  constraint
%type  <name>  range
%type  <valu>  direction
%type  <valu>  abstractlit
%type  <name>  name
%type  <name>  slice_name
%type  <name>  indexed_name

%type  <text>  formal_generic_name
%type  <text>  generic_name
%type  <flag>  .sign.

%start design_file

%%
design_file
        : entity_declaration
          architecture_body
                {
                freelomodel (MVL_CHDHED);
                mvl_fretab  (MVL_HSHTAB);
                }
        ;

entity_declaration
        : ENTITY
          .simple_name.
          IS
                {
                MVL_HSHTAB = mvl_initab ();
                MVL_SIMFLG = 0   ;      /* simple flag                  */
                MVL_NM1LST = NULL;      /* 1-st name liste              */
                MVL_NM2LST = NULL;      /* 2-nd name liste              */
                MVL_CHDPNT = NULL;      /* current child pointer        */
                MVL_CHDHED = NULL;      /* childs list                  */

                MVL_MODNAM = $2;
                mvl_addtab (MVL_HSHTAB,MVL_MODNAM,MVL_MODNAM,MVL_MODDFN,
                            MVL_ROTDFN);
                }
          .generic_clause.
          .port_clause.
           END_ERR
          .simple_name.
           Semicolon_ERR
                {
                if (($8 != NULL) && ($8 != $2))
                  mvl_error (1, $8);
                mvl_addtab (MVL_HSHTAB,MVL_MODNAM,MVL_MODNAM,MVL_CCNDFN,
                            MVL_CONCNT);
                MVL_CONCNT = 0;
                }
        | ENTITY
          error
                { mvl_error (2, NULL); }
        ;

.generic_clause.
        : /*empty*/
        | generic_clause
        ;

generic_clause
        : GENERIC
            {                 /* NULL means we're in the entity declaration */
               if (!MVL_CHDHED) {
                  MVL_LOFPNT->USER=addptype(MVL_LOFPNT->USER,LOGEN,NULL);
                  MVL_PTPPNT=MVL_LOFPNT->USER;
               } else {
                  MVL_CHDPNT->USER=addptype(MVL_LOFPNT->USER,LOGEN,NULL);
                  MVL_PTPPNT=MVL_CHDPNT->USER;
               }
            }
          LeftParen
          formal_generic_list
          RightParen_ERR
          Semicolon_ERR
        | GENERIC
          error
          Semicolon_ERR
        ;

formal_generic_list
        : formal_generic_element
          ...formal_generic_element..
        ;

...formal_generic_element..
        : /*empty*/
        | ...formal_generic_element..
          Semicolon_ERR
          formal_generic_element
        ;

formal_generic_element
        : CONSTANT
          identifier_list
          Colon
          type_mark
          .constraint.
          generic_VarAsgn__expression
             {
             struct chain *l;
                
                for (l=MVL_NM1LST;l;l=l->NEXT) {
                   MVL_PTPPNT->DATA=duplogen(&MVL_LOGEN,
                                             MVL_PTPPNT->DATA,l->DATA);
                }
                switch ($4) {
                   case MVL_ARGDFN:
                   {
                      logen_list *gen = (logen_list *)MVL_PTPPNT->DATA;
                      gen->TYPE=GENTYPE_ARG;
                   }
                   break;
                   case MVL_BITDFN:
                   {
                      logen_list *gen = (logen_list *)MVL_PTPPNT->DATA;
                      gen->TYPE=GENTYPE_BIT;
                   }
                   break;
                }

                MVL_LOGEN.TYPE=GENTYPE_EMPTY;
                freechain (MVL_NM1LST);
                MVL_NM1LST = NULL;
             }
        | error
        ;

generic_VarAsgn__expression
        : VarAsgn
          generic_expression
        ;

.constraint.
        : /*empty*/
                {
                $$.LEFT  = -1;
                $$.RIGHT = -1;
                $$.ERR_FLG = 0;
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

range
        : abstractlit
          direction
          abstractlit
                {
                $$.LEFT  = $1;
                $$.RIGHT = $3;

                if ((($1 > $3) && ($2 == MVL_UPTDFN)) ||
                    (($1 < $3) && ($2 == MVL_DWTDFN)))
                  {
                  mvl_error (32, NULL);
                  $$.ERR_FLG = 1;
                  }
                else
                  $$.ERR_FLG = 0;
                }
        ;

direction
        : TO
                { $$ = MVL_UPTDFN; }
        | DOWNTO
                { $$ = MVL_DWTDFN; }
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
                { mvl_error (3, NULL); }
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
          .BUS.
                {
                struct locon *locon_pnt;
                struct losig *losig_pnt;
                int           sig_width;
                int           sig_conf;
                char         *sig_name;
                struct chain *sig_list;
                int           local_err;

                local_err = $6.ERR_FLG;
                if ( ($6.LEFT == -1) && (($5 == MVL_BTVDFN) ||
                                         ($5 == MVL_MXVDFN) ||
                                         ($5 == MVL_WRVDFN)))
                  {
                  mvl_error (33, NULL);
                  }

                if ($6.LEFT <= $6.RIGHT)
                  sig_width = $6.RIGHT - $6.LEFT  + 1;
                else
                  sig_width = $6.LEFT  - $6.RIGHT + 1;

                sig_conf   = $4 + $5 + $7;
                MVL_NM1LST = reverse (MVL_NM1LST);
                sig_list   = MVL_NM1LST;
                while ((sig_list != NULL) && (local_err == 0))
                  {
                  sig_name = (char *)sig_list->DATA;

                  if (mvl_chktab (MVL_HSHTAB,sig_name,MVL_MODNAM,
                                  MVL_SIGDFN) != 0)
                    mvl_error (4, sig_name);
                  else
                    {
                    switch (sig_conf)
                      {
                      case (MVL_ICNDFN + MVL_BITDFN + MVL_NORDFN) :
                      case (MVL_ICNDFN + MVL_BTVDFN + MVL_NORDFN) :
                        losig_pnt = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,'E','0',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        locon_pnt = mvl_addlocon (MVL_LOFPNT,losig_pnt,'I',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_OCNDFN + MVL_BITDFN + MVL_NORDFN) :
                      case (MVL_OCNDFN + MVL_BTVDFN + MVL_NORDFN) :
                        losig_pnt = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,'E','0',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        locon_pnt = mvl_addlocon (MVL_LOFPNT,losig_pnt,'O',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_OCNDFN + MVL_MUXDFN + MVL_BUSDFN) :
                      case (MVL_OCNDFN + MVL_MXVDFN + MVL_BUSDFN) :
                        losig_pnt = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,'E','M',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        locon_pnt = mvl_addlocon (MVL_LOFPNT,losig_pnt,'Z',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_OCNDFN + MVL_WORDFN + MVL_BUSDFN) :
                      case (MVL_OCNDFN + MVL_WRVDFN + MVL_BUSDFN) :
                        losig_pnt = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,'E','W',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        locon_pnt = mvl_addlocon (MVL_LOFPNT,losig_pnt,'Z',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_BCNDFN + MVL_BITDFN + MVL_NORDFN) :
                      case (MVL_BCNDFN + MVL_BTVDFN + MVL_NORDFN) :
                        losig_pnt = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,'E','0',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        locon_pnt = mvl_addlocon (MVL_LOFPNT,losig_pnt,'B',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_BCNDFN + MVL_MUXDFN + MVL_BUSDFN) :
                      case (MVL_BCNDFN + MVL_MXVDFN + MVL_BUSDFN) :
                        losig_pnt = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,'E','M',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        locon_pnt = mvl_addlocon (MVL_LOFPNT,losig_pnt,'T',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_BCNDFN + MVL_WORDFN + MVL_BUSDFN) :
                      case (MVL_BCNDFN + MVL_WRVDFN + MVL_BUSDFN) :
                        losig_pnt = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,'E','W',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        locon_pnt = mvl_addlocon (MVL_LOFPNT,losig_pnt,'T',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_XCNDFN + MVL_BITDFN + MVL_NORDFN) :
                      case (MVL_XCNDFN + MVL_BTVDFN + MVL_NORDFN) :
                        losig_pnt = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,'E','0',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        locon_pnt = mvl_addlocon (MVL_LOFPNT,losig_pnt,'X',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;
                      default :
                        mvl_error (5, sig_name);
                        local_err = 1;
                        break;
                      }

                    if (local_err == 0)
                      {
                      mvl_addtab (MVL_HSHTAB,sig_name,MVL_MODNAM,MVL_PNTDFN,
                                  (int)losig_pnt);
                      mvl_addtab (MVL_HSHTAB,sig_name,MVL_MODNAM,MVL_SIGDFN,
                                  sig_conf);
                      mvl_addtab (MVL_HSHTAB,sig_name,MVL_MODNAM,MVL_LFTDFN,
                                  $6.LEFT);
                      mvl_addtab (MVL_HSHTAB,sig_name,MVL_MODNAM,MVL_RGTDFN,
                                  $6.RIGHT);
                      MVL_CONCNT += sig_width;
                      MVL_SIGIDX += sig_width;
                      }
                    }

                  sig_list = sig_list->NEXT;
                  }
                freechain (MVL_NM1LST);
                MVL_NM1LST = NULL;
                }
        | error
                {

                /* ###----------------------------------------------### */
                /*   The following 3 lines reject tokens until the      */
                /* sync. token ('Semicolon') is found                   */
                /* ###----------------------------------------------### */

                do
                  yychar = yylex ();
                while ((yychar != Semicolon) && (yychar != 0));
                yyerrok;

                mvl_error (6, NULL);
                }
        ;

architecture_body
        : ARCHITECTURE
                {
                if (!strcmp (IN_LO, "vbe"))
                  YYACCEPT;
                }
          simple_name
          OF
          simple_name
          IS
                {
                if ($5 != MVL_MODNAM)
                  mvl_error (1, $5);
                }
          architecture_declarative_part
          _BEGIN
          architecture_statement_part
          END_ERR
          .simple_name.
          Semicolon_ERR
                {
                if (($12 != NULL) && ($12 != $3))
                  mvl_error (7, $12);
                }
        | ARCHITECTURE
          error
                { mvl_error (8, NULL); }
        ;

architecture_declarative_part
        : ..block_declaration_item..
        ;

..block_declaration_item..
        : /*empty*/
        | ..block_declaration_item..
          block_declaration_item
        ;

block_declaration_item
        : signal_declaration
        | component_declaration
        | error
          Semicolon_ERR
                { mvl_error (9, NULL); }
        ;

signal_declaration
        : SIGNAL
          identifier_list
          Colon
          type_mark
.constraint.
          .BUS.
          Semicolon_ERR
                {
                struct losig *losig_pnt;
                int           sig_width;
                int           sig_conf;
                char         *sig_name;
                struct chain *sig_list;
                int           local_err;

                local_err = $5.ERR_FLG;
                if (($5.LEFT==-1) && (($4==MVL_BTVDFN) || ($4==MVL_RGVDFN)))
                  {
                  mvl_error (33, NULL);
                  }
                if ($5.LEFT <= $5.RIGHT)
                  sig_width = $5.RIGHT - $5.LEFT  + 1;
                else
                  sig_width = $5.LEFT  - $5.RIGHT + 1;

                sig_conf   = $4 + $6;
                MVL_NM1LST = reverse (MVL_NM1LST);
                sig_list   = MVL_NM1LST;
                while ((sig_list != NULL) && (local_err == 0))
                  {
                  sig_name = (char *)sig_list->DATA;

                  if (mvl_chktab (MVL_HSHTAB,sig_name,MVL_MODNAM,
                                  MVL_SIGDFN) != 0)
                    mvl_error (10, sig_name);
                  else
                    {
                    switch (sig_conf)
                      {
                      case (MVL_BITDFN + MVL_NORDFN):
                      case (MVL_BTVDFN + MVL_NORDFN):
                        losig_pnt = mvl_addlosig(MVL_LOFPNT,MVL_SIGIDX,'I','0',
                                                 sig_name,$5.LEFT,$5.RIGHT);
                        break;

                      case (MVL_MUXDFN + MVL_BUSDFN):
                      case (MVL_MXVDFN + MVL_BUSDFN):
                        losig_pnt = mvl_addlosig(MVL_LOFPNT,MVL_SIGIDX,'I','M',
                                                 sig_name,$5.LEFT,$5.RIGHT);
                        break;

                      case (MVL_WORDFN + MVL_BUSDFN):
                      case (MVL_WRVDFN + MVL_BUSDFN):
                        losig_pnt = mvl_addlosig(MVL_LOFPNT,MVL_SIGIDX,'I','W',
                                                 sig_name,$5.LEFT,$5.RIGHT);
                        break;

                      default:
                        mvl_error (11, sig_name);
                        local_err = 1;
                        break;
                      }

                    if (local_err == 0)
                      {
                      mvl_addtab (MVL_HSHTAB,sig_name,MVL_MODNAM,MVL_PNTDFN,
                                  (int)losig_pnt);
                      mvl_addtab (MVL_HSHTAB,sig_name,MVL_MODNAM,MVL_SIGDFN,
                                  sig_conf);
                      mvl_addtab (MVL_HSHTAB,sig_name,MVL_MODNAM,MVL_LFTDFN,
                                  $5.LEFT);
                      mvl_addtab (MVL_HSHTAB,sig_name,MVL_MODNAM,MVL_RGTDFN,
                                  $5.RIGHT);
                      MVL_SIGIDX += sig_width;
                      }
                    }

                  sig_list = sig_list->NEXT;
                  }
                freechain (MVL_NM1LST);
                MVL_NM1LST = NULL;
                }
        ;

component_declaration
        : COMPONENT
          Identifier
                {
                MVL_CHDNAM = $2;

                if (mvl_chktab (MVL_HSHTAB,MVL_CHDNAM,MVL_MODNAM,MVL_MODDFN) != 0)
                  {
                  mvl_error (12, MVL_CHDNAM);
                  MVL_CHDPNT = NULL;
                  }
                else
                  {
                  MVL_CHDHED = addlomodel (MVL_CHDHED,MVL_CHDNAM);

                  MVL_CHDPNT = MVL_CHDHED;
                  mvl_addtab (MVL_HSHTAB,MVL_CHDNAM,MVL_MODNAM,MVL_MODDFN,MVL_CHDDFN);
                  }
                }
          .generic_clause.
          .PORT__local_port_list.
          END_ERR
          COMPONENT
          Semicolon_ERR
                {
                if (MVL_CHDPNT != NULL)
                  {
                  mvl_addtab (MVL_HSHTAB,$2,MVL_MODNAM,MVL_CCNDFN,MVL_CONCNT);
                  MVL_LOFPNT->MODELCHAIN = addchain (MVL_LOFPNT->MODELCHAIN,$2);
                  }
                MVL_CONCNT = 0;
                }
        ;

.PORT__local_port_list.
        : /*empty*/
        | PORT
          LeftParen
          local_port_list
          RightParen_ERR
          Semicolon_ERR
        ;

local_port_list
        : local_port_element
          ...local_port_element..
        ;

...local_port_element..
        : /*empty*/
        | ...local_port_element..
          Semicolon_ERR
          local_port_element
        ;

local_port_element
        : .SIGNAL.
          identifier_list
          Colon
          .local_port_mode.
          type_mark
.constraint.
          .BUS.
                {
                struct locon *locon_pnt;
                int           sig_width;
                int           sig_conf;
                char         *sig_name;
                struct chain *sig_list;
                int           local_err = 0;

                if (($6.ERR_FLG != 0) || (MVL_CHDPNT == NULL))
                  local_err = 1;

                if (($6.LEFT == -1) && (($5 == MVL_BTVDFN) ||
                                        ($5 == MVL_MXVDFN) ||
                                        ($5 == MVL_WRVDFN))  )
                  {
                  mvl_error (33, NULL);
                  }
                if ($6.LEFT <= $6.RIGHT)
                  sig_width = $6.RIGHT - $6.LEFT  + 1;
                else
                  sig_width = $6.LEFT  - $6.RIGHT + 1;

                sig_conf   = $4 + $5 + $7;
                MVL_NM1LST = reverse (MVL_NM1LST);
                sig_list   = MVL_NM1LST;
                while ((sig_list != NULL) && (local_err == 0))
                  {
                  sig_name = (char *)sig_list->DATA;
                  if (mvl_chktab (MVL_HSHTAB,sig_name,MVL_CHDNAM,
                                  MVL_SIGDFN) != 0)
                    mvl_error (4, sig_name);
                  else
                    {
                    switch (sig_conf)
                      {
                      case (MVL_ICNDFN + MVL_BITDFN + MVL_NORDFN) :
                      case (MVL_ICNDFN + MVL_BTVDFN + MVL_NORDFN) :
                        locon_pnt = mvl_addlocon (MVL_CHDPNT,NULL,'I',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_OCNDFN + MVL_BITDFN + MVL_NORDFN) :
                      case (MVL_OCNDFN + MVL_BTVDFN + MVL_NORDFN) :
                        locon_pnt = mvl_addlocon (MVL_CHDPNT,NULL,'O',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_OCNDFN + MVL_MUXDFN + MVL_BUSDFN) :
                      case (MVL_OCNDFN + MVL_MXVDFN + MVL_BUSDFN) :
                        locon_pnt = mvl_addlocon (MVL_CHDPNT,NULL,'Z',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_OCNDFN + MVL_WORDFN + MVL_BUSDFN) :
                      case (MVL_OCNDFN + MVL_WRVDFN + MVL_BUSDFN) :
                        locon_pnt = mvl_addlocon (MVL_CHDPNT,NULL,'Z',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_BCNDFN + MVL_BITDFN + MVL_NORDFN) :
                      case (MVL_BCNDFN + MVL_BTVDFN + MVL_NORDFN) :
                        locon_pnt = mvl_addlocon (MVL_CHDPNT,NULL,'B',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_BCNDFN + MVL_MUXDFN + MVL_BUSDFN) :
                      case (MVL_BCNDFN + MVL_MXVDFN + MVL_BUSDFN) :
                        locon_pnt = mvl_addlocon (MVL_CHDPNT,NULL,'T',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_BCNDFN + MVL_WORDFN + MVL_BUSDFN) :
                      case (MVL_BCNDFN + MVL_WRVDFN + MVL_BUSDFN) :
                        locon_pnt = mvl_addlocon (MVL_CHDPNT,NULL,'T',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      case (MVL_XCNDFN + MVL_BITDFN + MVL_NORDFN) :
                      case (MVL_XCNDFN + MVL_BTVDFN + MVL_NORDFN) :
                        locon_pnt = mvl_addlocon (MVL_CHDPNT,NULL,'X',
                                                  sig_name,$6.LEFT,$6.RIGHT);
                        break;

                      default :
                        mvl_error (5, sig_name);
                        local_err = 1;
                        break;
                      }

                    if (local_err == 0)
                      {
                      mvl_addtab (MVL_HSHTAB,MVL_CONCNT,MVL_CHDNAM,MVL_PNTDFN,
                                  sig_name);
                      mvl_addtab (MVL_HSHTAB,sig_name,  MVL_CHDNAM,MVL_SIGDFN,
                                  sig_conf);
                      mvl_addtab (MVL_HSHTAB,sig_name,  MVL_CHDNAM,MVL_RNGDFN,
                                  MVL_CONCNT);
                      mvl_addtab (MVL_HSHTAB,sig_name,  MVL_CHDNAM,MVL_LFTDFN,
                                  $6.LEFT);
                      mvl_addtab (MVL_HSHTAB,sig_name,  MVL_CHDNAM,MVL_RGTDFN,
                                  $6.RIGHT);
                      MVL_CONCNT += sig_width;
                      }
                    }
                  sig_list = sig_list->NEXT;
                  }
                freechain (MVL_NM1LST);
                MVL_NM1LST = NULL;
                }
        | error
                {

                /* ###----------------------------------------------### */
                /*   The following 3 lines reject tokens until the      */
                /* sync. token ('Semicolon') is found                   */
                /* ###----------------------------------------------### */

                do
                  yychar = yylex ();
                while ((yychar != Semicolon) && (yychar != 0));
                yyerrok;

                mvl_error (6, NULL);
                }
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
        : component_instantiation_statement
        | error
          Semicolon_ERR
                { mvl_error (18, NULL); }
        ;

component_instantiation_statement
        : a_label
          simple_name
                {
                int con_count;
                int local_err = 0;

                MVL_CONRNK = 0;
                MVL_CHDNAM = $2;
                MVL_LOCNAM = $1;

                if (mvl_chktab (MVL_HSHTAB,$1,MVL_MODNAM,MVL_LBLDFN) != 0)
                  {
                  mvl_error (13, $1);
                  local_err = 1;
                  }

                if (mvl_chktab (MVL_HSHTAB,$2,MVL_MODNAM,
                                MVL_MODDFN) != MVL_CHDDFN)
                  {
                  mvl_error (14, $2);
                  local_err = 1;
                  }

                if (local_err == 0)
                  {
                  mvl_addtab (MVL_HSHTAB,$1,MVL_MODNAM,MVL_LBLDFN,1);
                  con_count  = mvl_chktab (MVL_HSHTAB,$2,MVL_MODNAM,MVL_CCNDFN);
                  MVL_NM1LST = NULL;
                  while (con_count != 0)
                    {
                    MVL_NM1LST = addchain (MVL_NM1LST,NULL);
                    con_count--;
                    }
                  MVL_CHDPNT = MVL_CHDHED;
                  while (MVL_CHDPNT != NULL)
                    {
                    if (MVL_CHDPNT->NAME == $2)
                      break;
                    MVL_CHDPNT = MVL_CHDPNT->NEXT;
                    }
                  }
                else
                  MVL_CHDPNT = NULL;

                }
          .generic_map_aspect.
          .port_map_aspect.
          Semicolon_ERR
                {
                struct loins *loins_pnt    ;
                struct locon *locon_pnt    ;
                struct chain *chain_pnt    ;
                struct logen *genmod       ;
                char         *prt_name     ;
                char          tampon [256] ;
                int           i            = 0;

                if (MVL_CHDPNT != NULL)
                  {
                  MVL_NM1LST = reverse (MVL_NM1LST);

                  chain_pnt  = MVL_NM1LST;
                  locon_pnt  = MVL_CHDPNT->LOCON;

                  while (chain_pnt != NULL)
                    {
                    if (chain_pnt->DATA == NULL)
                      {
                      MVL_SIGIDX++;
                      sprintf(tampon,"%s:%s.%s",MVL_CHDNAM,$1,locon_pnt->NAME);
                      chain_pnt->DATA = mvl_addlosig (MVL_LOFPNT,MVL_SIGIDX,
                                                      'I','0',tampon,-1,-1);
                      }
                    chain_pnt = chain_pnt->NEXT;
                    locon_pnt = locon_pnt->NEXT;
                    }

                  if (MVL_ERRFLG == 0) {
                    loins_pnt = addloins (MVL_LOFPNT, $1  , MVL_CHDPNT,
                                          MVL_NM1LST );
                     if (MVL_CHDPNT->USER) {
                        if (!MVL_PTPPNT)
                           MVL_PTPPNT = addptype(NULL,LOGEN,NULL);  
                        loins_pnt->USER=MVL_PTPPNT;
                        MVL_PTPPNT->DATA
                                = chklogen(loins_pnt->USER->DATA,
                                             MVL_CHDPNT->USER->DATA,
                                             MVL_CHDNAM, loins_pnt->INSNAME);
                     } else {
                        if (MVL_PTPPNT) {
                           fprintf(stderr,"No generic for model %s, NULLed !\n",
                                MVL_CHDPNT->NAME);
                           loins_pnt->USER=NULL;
                        }
                     }

                    }
                  }

                freechain (MVL_NM1LST);
                MVL_NM1LST = NULL;
                MVL_SIMFLG = MVL_UKNDFN;
                }
        ;

/* ###--------------------------------------------------------------------### */
/* #   GENERIC MAP                                                          # */
/* ###--------------------------------------------------------------------### */

.generic_map_aspect.
        : /*empty*/
          {
             MVL_PTPPNT = NULL;
          }
        | generic_map_aspect
        ;

generic_map_aspect
        : GENERIC
          MAP
          LeftParen
          generic_association_list
          RightParen_ERR
          {
             MVL_PTPPNT = addptype(NULL,LOGEN,MVL_GENPNT);  
             MVL_GENPNT = NULL;
          }
        ;

generic_association_list
        : generic_association_element
          ...generic_association_element..
        ;

...generic_association_element..
        : /*empty*/
        | ...generic_association_element..
          Comma
          generic_association_element
        ;

generic_association_element
        : formal_generic_name
          Arrow
          actual_generic_name
          {
             MVL_GENPNT = duplogen(&MVL_LOGEN,MVL_GENPNT,$1);
             MVL_LOGEN.TYPE=GENTYPE_EMPTY;
          }
        | actual_generic_name
          {
             MVL_GENPNT = duplogen(&MVL_LOGEN,MVL_GENPNT,NULL);
             MVL_LOGEN.TYPE=GENTYPE_EMPTY;
          }
        | error
        ;

formal_generic_name
        : simple_name
        ;

actual_generic_name
        : generic_expression
        ;

generic_expression
        : generic_relation
        ;

generic_relation
        : generic_simple_expression
        ;

generic_simple_expression
        : .sign.
          generic_term
                {
                   if ($1)
                      if (MVL_LOGEN.TYPE!=GENTYPE_VAL) {
                         fputs("Don't know how to change Identifier sign\n",
                               stderr);
                      } else
                         MVL_LOGEN.VALUE.VAL = -MVL_LOGEN.VALUE.VAL;
                }
          ...generic_term..
        ;

.sign.
        : /*empty*/             { $$ = 0; }
        | Plus                  { $$ = 0; }
        | Minus                 { $$ = 1; }
        ;

generic_term
        : generic_factor
        ;

...generic_term..
        : /*empty*/
       /*  | ...generic_term..
          generic_adding_operator
          generic_term */
        ;

generic_factor
        : generic_primary
        ;

generic_primary
        : generic_name
          {
             MVL_LOGEN.VALUE.TEXT=namealloc($1);
             MVL_LOGEN.TYPE=GENTYPE_TEXT;
          }
        | generic_literal
        | generic_aggregate
          {
             MVL_LOGEN.VALUE.LIST = (logen_list *)reverse(MVL_GENLST->DATA);
             MVL_LOGEN.TYPE = GENTYPE_LIST;
             MVL_GENLST = delchain(MVL_GENLST, MVL_GENLST);
          }
        ;

generic_aggregate
        : LeftParen
          {
             MVL_GENLST = addchain(MVL_GENLST, NULL);
          }
          generic_element_association
          ...generic_element_association..
          RightParen_ERR
        ;

...generic_element_association..
        : /*empty*/
        | ...generic_element_association..
          Comma
          generic_element_association
        ;

generic_element_association
        : .generic_choices.
          generic_expression
          {
             ((logen_list *)MVL_GENLST->DATA)
                    = duplogen (&MVL_LOGEN,
                                ((logen_list *)MVL_GENLST->DATA), NULL);
             MVL_LOGEN.TYPE = GENTYPE_EMPTY;
          }
        ;

.generic_choices.
        : /*empty*/
        ;

/*
generic_adding_operator
        : Ampersand
        ;
*/

generic_name
        : simple_name           { $$ = $1; }
        ;

generic_literal
        : abstractlit
          {
             MVL_LOGEN.VALUE.VAL = $1;
             if (base==16)
                MVL_LOGEN.TYPE = GENTYPE_HEX ;
             else
                MVL_LOGEN.TYPE = GENTYPE_VAL ;
          }
        | StringLit
          {
             MVL_LOGEN.VALUE.TEXT = strdup($1);
             MVL_LOGEN.TYPE = GENTYPE_TEXT;
          }
        | CharacterLit
          {
             MVL_LOGEN.VALUE.CHAR = $1[1];
             MVL_LOGEN.TYPE = GENTYPE_BIT;
          }
        ;

/* ###--------------------------------------------------------------------### */
/*     PORT MAP                                                               */
/* ###--------------------------------------------------------------------### */

.port_map_aspect.
        : /*empty*/
        | port_map_aspect
        ;

port_map_aspect
        : PORT
          MAP
          LeftParen
          association_list
          RightParen_ERR
        ;

association_list
        : association_element
          ...association_element..
        ;

...association_element..
        : /*empty*/
        | ...association_element..
          Comma
          association_element
        ;

association_element
        : formal_port_name
          Arrow
          actual_port_name
                {
                int           sig_width;
                int           con_pos;
                struct chain *sig_list1;
                struct chain *sig_list2;
                int           local_err = 0;

                if (MVL_CHDPNT == NULL)
                  local_err = 1;
                sig_list1 = MVL_NM1LST;

                if (MVL_SIMFLG == MVL_IMPDFN)
                  sig_list1 = MVL_NM2LST;
                MVL_SIMFLG = MVL_EXPDFN;

                if ($1.LEFT <= $1.RIGHT)
                  sig_width = $1.RIGHT - $1.LEFT + 1;
                else
                  sig_width = $1.LEFT - $1.RIGHT + 1;

                if (local_err == 0)
                  {
                  if (sig_width != $3.WIDTH)
                    mvl_error (38, NULL);
                  else
                    {
                    if (MVL_ERRFLG == 0)
                      {
                      con_pos = mvl_chktab (MVL_HSHTAB, $1.NAME, MVL_CHDNAM,
                                            MVL_RNGDFN);
                      while (con_pos != 0)
                        {
                        sig_list1 = sig_list1->NEXT;
                        con_pos--;
                        }
                      sig_list2 = $3.LIST;
                      while (sig_list2 != NULL)
                        {
                        if (sig_list1->DATA != NULL)
                          {
                          mvl_error (41, $1.NAME);
                          break;
                          }
                        sig_list1->DATA = sig_list2->DATA;
                        sig_list2       = sig_list2->NEXT;
                        sig_list1       = sig_list1->NEXT;
                        }
                      }
                    }
                  }
                freechain ($3.LIST);
                }
        | actual_port_name
                {
                int           sig_width ;
                struct chain *sig_list1 ;
                char         *prt_name  ;
                int           prt_width ;
                int           left_bnd  ;
                int           right_bnd ;
                int           local_err = 0;
                int           i         ;

                if (MVL_CHDPNT == NULL)
                  local_err = 1;

                if (MVL_SIMFLG == MVL_UKNDFN)
                  {
                  MVL_SIMFLG = MVL_IMPDFN;
                  MVL_NM2LST = MVL_NM1LST;
                  }
                if (MVL_SIMFLG != MVL_IMPDFN)
                  {
                  mvl_error (15, NULL);
                  local_err = 1;
                  }

                prt_name  = (char *) mvl_chktab (MVL_HSHTAB,MVL_CONRNK,
                                                 MVL_CHDNAM,MVL_PNTDFN);
                if (prt_name == NULL)
                  {
                  local_err = 1;
                  mvl_error (76, MVL_LOCNAM);
                  }

                right_bnd = mvl_chktab (MVL_HSHTAB,prt_name,MVL_CHDNAM,
                                        MVL_LFTDFN);
                left_bnd  = mvl_chktab (MVL_HSHTAB,prt_name,MVL_CHDNAM,
                                        MVL_RGTDFN);

                if (left_bnd <= right_bnd)
                  prt_width = right_bnd - left_bnd  + 1;
                else
                  prt_width = left_bnd  - right_bnd + 1;

                if (local_err == 0)
                  {
                  if (prt_width != $1.WIDTH)
                    {
                    mvl_error (38, NULL);
                    for (i=0 ; i<prt_width ; i++)
                      MVL_NM2LST = MVL_NM2LST->NEXT;
                    }
                  else
                    {
                    sig_list1 = $1.LIST;
                    while (sig_list1 != NULL)
                      {
                      MVL_NM2LST->DATA = sig_list1->DATA;
                      sig_list1        = sig_list1->NEXT;
                      MVL_NM2LST       = MVL_NM2LST->NEXT;
                      }
                    }
                  MVL_CONRNK += prt_width;
                  }
                freechain ($1.LIST);
                }
        | error
                {

                /* ###----------------------------------------------### */
                /*   The following 3 lines reject tokens until the      */
                /* sync. token ('Comma') is found                       */
                /* ###----------------------------------------------### */

                do
                  yychar = yylex ();
                while ((yychar != Comma) && (yychar != 0));
                yyerrok;

                MVL_CONRNK++;
                mvl_error (31, NULL);
                }
        ;

formal_port_name
        : name
                {
                if (MVL_CHDPNT != NULL)
                  {
                  if (mvl_chktab(MVL_HSHTAB,$1.NAME,MVL_CHDNAM,MVL_SIGDFN) != 0)
                    {
                    $$.LEFT  = mvl_chktab (MVL_HSHTAB,$1.NAME,MVL_CHDNAM,
                                           MVL_LFTDFN);
                    $$.RIGHT = mvl_chktab (MVL_HSHTAB,$1.NAME,MVL_CHDNAM,
                                           MVL_RGTDFN);
                    }
                  else
                    {
                    mvl_error (16, $1.NAME);
                    $$ = $1;
                    }
                  }
                else
                  $$ = $1;
                }
        ;

actual_port_name
        : expression
                { $$ = $1; }
        ;

name
        : simple_name
                {
                $$.NAME  = $1;
                $$.LEFT  = -1;
                $$.RIGHT = -1;
                }
        | indexed_name
                { $$ = $1; }
        | slice_name
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
                $$.RIGHT = $3;
                }
        ;

slice_name
        : simple_name
          constraint
                {
                $$.NAME  = $1;
                $$.LEFT  = $2.LEFT;
                $$.RIGHT = $2.RIGHT;
                }
        ;


expression
        : relation
                { $$ = $1; }
        ;

relation
        : simple_expression
                { $$ = $1; }
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
                {
                struct chain *list1;

                list1 = $1.LIST;
                while (list1->NEXT != NULL)
                  list1 = list1->NEXT;

                list1->NEXT = $3.LIST;
                $$.LIST     = $1.LIST;
                $$.WIDTH    = $1.WIDTH + $3.WIDTH;
                }

        ;

term
        : factor
                { $$ = $1; }
        ;


factor
        : primary
                { $$ = $1; }
        ;

primary
        : aggregate
                { $$ = $1; }
        | type_convertion
                { $$ = $1; }
        | name
                {
                mvl_expr      expr;
                struct losig *losig_pnt;
                int           i;
                int           left;
                int           right;
                int           left_bnd;
                int           right_bnd;
                int           sig_width;
                int           rev_flg;
                int           in_bound;
                int           out_bound;

                if (mvl_chktab (MVL_HSHTAB,$1.NAME,MVL_MODNAM,MVL_SIGDFN) != 0)
                  {
                  left_bnd  = mvl_chktab (MVL_HSHTAB,$1.NAME,MVL_MODNAM,
                                          MVL_LFTDFN);
                  right_bnd = mvl_chktab (MVL_HSHTAB,$1.NAME,MVL_MODNAM,
                                          MVL_RGTDFN);

                  if ($1.LEFT == -1)
                    {
                    left  = left_bnd;
                    right = right_bnd;
                    }
                  else
                    {
                    left  = $1.LEFT;
                    right = $1.RIGHT;
                    }

                  if (left_bnd <= right_bnd)
                    {
                    sig_width = right_bnd - left_bnd + 1;
                    if (left <= right)
                      {
                      expr.WIDTH   = right - left + 1;
                      rev_flg    = 0;
                      in_bound   = right_bnd - right;
                      out_bound  = right_bnd - left;
                      if ((left < left_bnd) || (right > right_bnd))
                        mvl_error (36, $1.NAME);
                      }
                    else
                      {
                      expr.WIDTH   = left - right + 1;
                      rev_flg    = 1;
                      in_bound   = right_bnd - left;
                      out_bound  = right_bnd - right;
                      if ((left > right_bnd) || (right < left_bnd))
                        mvl_error (36, $1.NAME);
                      }
                    }
                  else
                    {
                    sig_width = left_bnd - right_bnd + 1;
                    if (left <= right)
                      {
                      expr.WIDTH   = right - left + 1;
                      rev_flg    = 1;
                      in_bound   = left  - right_bnd;
                      out_bound  = right - right_bnd;
                      if ((left < right_bnd) || (right > left_bnd))
                        mvl_error (36, $1.NAME);
                      }
                    else
                      {
                      expr.WIDTH   = left - right + 1;
                      rev_flg    = 0;
                      in_bound   = right - right_bnd;
                      out_bound  = left  - right_bnd;
                      if ((left > left_bnd) || (right < right_bnd))
                        mvl_error (36, $1.NAME);
                      }
                    }

                  losig_pnt = (struct losig *)
                              mvl_chktab (MVL_HSHTAB,$1.NAME,MVL_MODNAM,
                                          MVL_PNTDFN);
                  expr.LIST = NULL;
                  for (i=0 ; i<sig_width ; i++)
                    {
                    if (i >= in_bound)
                      expr.LIST = addchain (expr.LIST,losig_pnt);
                    if (i >= out_bound)
                      break;
                    losig_pnt = losig_pnt->NEXT;
                    }
                  if (rev_flg == 1)
                    expr.LIST = reverse (expr.LIST);
                  }
                else
                  {
                  mvl_error (17, $1.NAME);
                  expr.WIDTH = 1;
                  expr.LIST  = addchain (NULL,NULL);
                  }
                $$ = expr;
                }
        ;

aggregate
        : LeftParen
          expression
          RightParen_ERR
                { $$ = $2; }
        ;

type_convertion
        : CONVERT
          LeftParen
          expression
          RightParen_ERR
                { $$ = $3; }
        ;


.SIGNAL.
        : /*empty*/
        | SIGNAL
        ;

.local_port_mode.
        : /*empty*/
                { $$ = MVL_ICNDFN; }
        | _IN
                { $$ = MVL_ICNDFN; }
        | _OUT
                { $$ = MVL_OCNDFN; }
        | _INOUT
                { $$ = MVL_BCNDFN; }
        | _LINKAGE
                {
                mvl_warning (2, NULL);
                $$ = MVL_XCNDFN;
                }
        ;

.mode.
        : /*empty*/
                { $$ = MVL_ICNDFN; }
        | _IN
                { $$ = MVL_ICNDFN; }
        | _OUT
                { $$ = MVL_OCNDFN; }
        | _INOUT
                { $$ = MVL_BCNDFN; }
        | _LINKAGE
                { $$ = MVL_XCNDFN; }
        ;

type_mark
        : BIT
                { $$ = MVL_BITDFN; }
        | WOR_BIT
                { $$ = MVL_WORDFN; }
        | MUX_BIT
                { $$ = MVL_MUXDFN; }
        | BIT_VECTOR
                { $$ = MVL_BTVDFN; }
        | WOR_VECTOR
                { $$ = MVL_WRVDFN; }
        | MUX_VECTOR
                { $$ = MVL_MXVDFN; }
        | INTEGER
                { $$ = MVL_NATDFN; }
        | NATURAL
                { $$ = MVL_NATDFN; }
        | NATURAL_VECTOR
                { $$ = MVL_NAVDFN; }
        | POSITIVE
                { $$ = MVL_NATDFN; }
        | STRING
                { $$ = MVL_STRDFN; }
        | _LIST
                { $$ = MVL_LSTDFN; }
        | ARG
                { $$ = MVL_ARGDFN; }
        ;

.BUS.
        : /*empty*/
                { $$ = MVL_NORDFN; }
        | BUS
                { $$ = MVL_BUSDFN; }
        ;

identifier_list
        : Identifier
                {
                  MVL_NM1LST = addchain (MVL_NM1LST,$1);
                }
          ...identifier..
        ;

...identifier..
        : /*empty*/
        | ...identifier..
          Comma
          Identifier
                {
                  MVL_NM1LST = addchain (MVL_NM1LST,$3);
                }
        ;

a_label
        : Identifier
          Colon
                { $$ = $1; }
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

Semicolon_ERR
        : Semicolon
                { yyerrok; }
        ;

abstractlit
        : AbstractLit
                {
                  char *absend, *absbase;

                  base = strtoul ($1, &absbase, 10);
                  if (*absbase=='\0')
                     $$ = base;
                  else
                     $$ = strtoul(absbase+1, &absend, base);
                }
        ;

RightParen_ERR
        : RightParen
                { yyerrok; }
        ;

END_ERR
        : _END
                { yyerrok; }
        ;

%%
