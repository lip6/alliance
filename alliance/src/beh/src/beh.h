/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: beh111.h						*/
/* date		: Mar  8 2000						*/
/* version	: v111							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* contents	: low level library for behavioural description		*/
/* ###--------------------------------------------------------------### */

#ifndef BEH_BEHDEF
#define BEH_BEHDEF

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	/* ###------------------------------------------------------### */
	/*    defines							*/
	/* ###------------------------------------------------------### */

#define BEH_GENERIC            203698	/* generic ptype code		*/
#define BEH_STABLE               0x01	/* description using STABLE	*/

#define BEH_NODE_BDD             0x01	/* fig TYPE : expr. BDD		*/
#define BEH_NODE_GEX             0x02	/* fig TYPE : expr. GEX		*/
#define BEH_NODE_VARLIST         0x04	/* fig TYPE : list of variables	*/
#define BEH_NODE_DERIVATE        0x08	/* fig TYPE : derivatives	*/
#define BEH_NODE_USER            0x10	/* fig TYPE : user expression	*/
#define BEH_NODE_QUAD            0x20	/* fig TYPE : QUAD structure	*/

#define BEH_CHK_DRIVERS    0x00000001	/* check output drivers		*/
#define BEH_CHK_EMPTY      0x00000002	/* check for empty architecture	*/

#define BEH_ARC_VHDL             0x01	/* fig FLAG : VHDL architecture	*/
#define BEH_ARC_C                0x02	/* fig FLAG : C    architecture	*/
#define BEH_FIG_DEPEND           0x04	/* fig FLAG : forward depend.	*/
#define BEH_FIG_ZERODELAY	 0x08	/* fig FLAG : no delay specified*/

#define BEH_CND_UNKNOWN          0x00	/* cond. precedence is unknown	*/
#define BEH_CND_PRECEDE          0x01	/* cond. has a precedence	*/
#define BEH_CND_NOPRECEDE        0x02	/* cond. has no precedence	*/

#define BEH_ASG_INERTIAL         0x10	/* assignment FLAG : inertial	*/
#define BEH_ASG_TRANSPORT        0x20	/* assignment FLAG : transport	*/

#define BEH_FIG__TIMEUNIT        0x07	/* fig TIME UNIT		*/
#define BEH_TU__FS                  1	/* fig TIME UNIT : fs		*/
#define BEH_TU__PS                  2	/* fig TIME UNIT : ps		*/
#define BEH_TU__NS                  3	/* fig TIME UNIT : ns		*/
#define BEH_TU__US                  4	/* fig TIME UNIT : us		*/
#define BEH_TU__MS                  5	/* fig TIME UNIT : ms		*/

	/* ###------------------------------------------------------### */
	/*    global variables						*/
	/* ###------------------------------------------------------### */

extern unsigned int   BEH_LINNUM  ;

extern unsigned char  BEH_TIMEUNIT;
extern unsigned int   BEH_TIMESTEP;

	/* ###------------------------------------------------------### */
	/*    structure definitions					*/
	/* ###------------------------------------------------------### */

struct beden			/* dictionary entry point		*/
  {
  struct beden *NEXT;		/* next entry				*/
  struct bedrd *DATA;		/* data					*/
  char         *KEY ;		/* key					*/
  };

struct bedrd			/* dictionary record			*/
  {
  struct bedrd *NEXT   ;	/* next record				*/
  char         *KEY    ;	/* context key				*/
  short         FD0_VAL;	/* data field				*/
  short         FD1_VAL;	/* data field				*/
  short         FD2_VAL;	/* data field				*/
  short         FD3_VAL;	/* data field				*/
  short         FD4_VAL;	/* data field				*/
  short         FD5_VAL;	/* data field				*/
  short         FD6_VAL;	/* data field				*/
  long          PNT_VAL;	/* data field				*/
  };

typedef struct befig		/* behaviour figure			*/
  {
  struct befig   *NEXT     ;	/* next figure				*/
  char           *NAME     ;	/* figure's name			*/
  struct bereg   *BEREG    ;	/* list of internal signals (REGISTER)	*/
  struct bemsg   *BEMSG    ;	/* list of assert instructions		*/
  struct berin   *BERIN    ;	/* list of inputs (registers and ports)	*/
  struct beout   *BEOUT    ;	/* list of output ports			*/
  struct bebus   *BEBUS    ;	/* list of output ports (BUS)		*/
  struct beaux   *BEAUX    ;	/* list of internal signals		*/
  struct beaux   *BEDLY    ;	/* internal signals used for 'DELAYED	*/
  struct bebux   *BEBUX    ;	/* list of internal signals (BUS)	*/
  struct bepor   *BEPOR    ;	/* list of ports in reverse order	*/
  struct begen   *BEGEN    ;	/* list of generic data			*/
  struct circuit *CIRCUI   ;	/* Pointer on Circuit for BDDs		*/
  struct ptype   *USER     ;	/* reserved for user's applications	*/
  char            ERRFLG   ;	/* error flag				*/
  char            TYPE     ;	/* description type			*/
  unsigned char   FLAG     ;	/* description flag			*/
  unsigned char   TIME_UNIT;	/* time unit				*/
  unsigned int    TIME_STEP;	/* minimum time step			*/
  }
befig_list;

typedef struct bereg		/* register				*/
  {
  struct bereg  *NEXT  ;	/* next register			*/
  char          *NAME  ;	/* register's name			*/
  struct biabl  *BIABL ;	/* list of drivers (ABL)		*/
  struct binode *BINODE;	/* list of drivers (BDD)		*/
  }
bereg_list;

typedef struct bemsg		/* assert instruction			*/
  {
  struct bemsg	*NEXT   ;	/* next assertion			*/
  char		*LABEL  ;	/* instruction's label			*/
  char		*MESSAGE;	/* assertion's message			*/
  struct chain	*ABL    ;	/* assertion's condition (ABL)		*/
  struct node	*NODE   ;	/* assertion's condition (BDD)		*/
  char           LEVEL  ;	/* severity level of the assertion	*/
  }
bemsg_list;

typedef struct beout		/* simple output port			*/
  {
  struct beout	*NEXT;		/* next output				*/
  char		*NAME;		/* port's name				*/
  struct chain	*ABL ;		/* port's equation (ABL)		*/
  struct node	*NODE;		/* port's equation (BDD)		*/
  unsigned int   TIME;		/* driver's delay			*/
  unsigned char  FLAG;		/* driver's flag			*/
  }
beout_list;

typedef struct bebus		/* bussed output port			*/
  {
  struct bebus	*NEXT  ;	/* next bus port			*/
  char		*NAME  ;	/* port's name				*/
  struct biabl	*BIABL ;	/* list of drivers (ABL)		*/
  struct binode	*BINODE;	/* list of drivers (BDD)		*/
  char           TYPE  ;	/* port's type : W or M			*/
  }
bebus_list;

typedef struct beaux		/* simple internal signal		*/
  {	
  struct beaux	*NEXT;		/* next signal				*/
  char		*NAME;		/* signal's name			*/
  struct chain	*ABL ;		/* signal's equation (ABL)		*/
  struct node	*NODE;		/* signal's equation (BDD)		*/
  unsigned int   TIME;		/* driver's delay			*/
  unsigned char  FLAG;		/* driver's flag			*/
  }
beaux_list;

typedef struct bebux		/* bussed internal signal		*/
  {	
  struct bebux	*NEXT  ;	/* next signal				*/
  char		*NAME  ;	/* signal's name			*/
  struct biabl	*BIABL ;	/* list of drivers (ABL)		*/
  struct binode	*BINODE;	/* list of drivers (BDD)		*/
  char           TYPE  ;	/* signal's type : W or M		*/
  }
bebux_list;

typedef struct bepor		/* port					*/
  {
  struct bepor *NEXT     ;	/* next port				*/
  char         *NAME     ;	/* port's name				*/
  char          DIRECTION;	/* port's mode (I, O, B, Z, T)		*/
  char          TYPE     ;	/* port's type (B, M or W)		*/
  }
bepor_list;

typedef struct biabl		/* couple of ABL			*/
  {
  struct biabl	*NEXT  ;	/* next couple				*/
  char          *LABEL ;	/* driver's label			*/
  struct chain	*CNDABL;	/* driver's connection condition (ABL)	*/
  struct chain	*VALABL;	/* equation of driven value (ABL)	*/
  struct ptype  *USER  ;	/* user field				*/
  unsigned int   TIME  ;	/* driver's delay			*/
  unsigned char  FLAG  ;	/* condition and driver's flag		*/
  }
biabl_list;

typedef struct binode		/* couple of BDD			*/
  {
  struct binode	*NEXT   ;	/* next figure				*/
  struct node	*CNDNODE;	/* driver's connection condition (BDD)	*/
  struct node	*VALNODE;	/* equation of driven value (BDD)	*/
  unsigned int   TIME   ;	/* driver's delay			*/
  unsigned char  FLAG  ;	/* driver's flag			*/
  }
binode_list;

typedef struct berin		/* primary inputs			*/
  {
  struct berin *NEXT;		/* next input				*/
  char         *NAME;		/* input's name				*/
  struct chain *OUT_REF;        /* dependent simple output		*/
  struct chain *BUS_REF;        /* dependent bussed output		*/
  struct chain *AUX_REF;        /* dependent simple internal signal	*/
  struct chain *REG_REF;        /* dependent internal register		*/
  struct chain *BUX_REF;        /* dependent bussed internal signal	*/
  struct chain *MSG_REF;        /* dependent assertion			*/
  struct chain *DLY_REF;        /* dependent delayed internal signal	*/
  }
berin_list;

typedef struct begen		/* generic				*/
  {
  struct begen *NEXT ;		/* next generic				*/
  char         *NAME ;		/* generic`s name			*/
  char         *TYPE ;		/* generic type`s name			*/
  void         *VALUE;		/* generic`s value			*/
  }
begen_list;

typedef struct bequad
  {
  struct node  *BDD     ;	/* expression using BDD format		*/
  struct begex *GEX     ;	/* expression using GEX format		*/
  struct chain *VARLIST ;	/* variable list			*/
  struct beder *DERIVATE;	/* list of derivate BDDs		*/
  void         *USER    ;	/* user defined form of the expression	*/
  }
bequad_list;

typedef struct beder
  {
  struct beder *NEXT ;		/* next derivative			*/
  struct node  *BDD  ;		/* derivative BDD			*/
  short         INDEX;		/* index of the variable		*/
  }
beder_list;

typedef struct begex
  {
  struct chain   *OPERAND;	/* list of operands or NULL for term	*/
  unsigned int    TERM   ;	/* operator or terminal's index		*/
  unsigned short  TYPE   ;	/* expression's type index		*/
  }
begex_list;

	/* ###------------------------------------------------------### */
	/*    functions							*/
	/* ###------------------------------------------------------### */

extern struct beaux  *beh_addbeaux   ();
extern struct bebux  *beh_addbebux   ();
extern struct bebus  *beh_addbebus   ();
extern struct befig  *beh_addbefig   ();
extern struct begen  *beh_addbegen   ();
extern struct bemsg  *beh_addbemsg   ();
extern struct beout  *beh_addbeout   ();
extern struct bepor  *beh_addbepor   ();
extern struct bereg  *beh_addbereg   ();
extern struct berin  *beh_addberin   ();
extern struct biabl  *beh_addbiabl   ();
extern struct binode *beh_addbinode  ();

extern struct beaux  *beh_apdbeaux   ();
extern struct berin  *beh_apdberin   ();

extern struct beder  *beh_addbeder   ();
extern struct beaux  *beh_delbeaux   ();
extern struct bebux  *beh_delbebux   ();
extern struct bebus  *beh_delbebus   ();
extern struct befig  *beh_delbefig   ();
extern struct begen  *beh_delbegen   ();
extern struct bemsg  *beh_delbemsg   ();
extern struct beout  *beh_delbeout   ();
extern struct bepor  *beh_delbepor   ();
extern struct bereg  *beh_delbereg   ();
extern struct berin  *beh_delberin   ();
extern struct biabl  *beh_delbiabl   ();
extern struct binode *beh_delbinode  ();
extern struct beder  *beh_delbeder   ();
extern void           beh_delbequad  ();

extern void           beh_frebeaux   ();
extern void           beh_frebebux   ();
extern void           beh_frebebus   ();
extern void           beh_frebefig   ();
extern void           beh_frebegen   ();
extern void           beh_frebemsg   ();
extern void           beh_frebeout   ();
extern void           beh_frebepor   ();
extern void           beh_frebereg   ();
extern void           beh_freberin   ();
extern void           beh_frebiabl   ();
extern void           beh_frebinode  ();
extern void           beh_frebeder   ();

extern struct beaux  *beh_rmvbeaux   ();
extern struct bebux  *beh_rmvbebux   ();
extern struct bebus  *beh_rmvbebus   ();
extern struct befig  *beh_rmvbefig   ();
extern struct begen  *beh_rmvbegen   ();
extern struct bemsg  *beh_rmvbemsg   ();
extern struct beout  *beh_rmvbeout   ();
extern struct bepor  *beh_rmvbepor   ();
extern struct bereg  *beh_rmvbereg   ();
extern struct berin  *beh_rmvberin   ();
extern struct beder  *beh_rmvbeder   ();

extern struct chain  *beh_expstable  ();
extern struct chain  *beh_expdelayed ();

extern struct chain  *beh_rplstable  ();
extern struct chain  *beh_rpldelayed ();

extern struct beden **beh_initab     ();
extern void           beh_addtab     ();
extern long           beh_chktab     ();
extern void           beh_fretab     ();

extern unsigned int   beh_error      ();
extern void           beh_message    ();
extern void           beh_toolbug    ();

extern char          *beh_vhdlname   ();
extern char          *beh_vhvector   ();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
