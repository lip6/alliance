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
/*									*/
/* file		: vbh.h							*/
/* date		: Apr 13 1993						*/
/* version	: 104							*/
/*									*/
/* ###--------------------------------------------------------------### */

#ifndef VBH_DEFINE_H
#define VBH_DEFINE_H

	/* ###------------------------------------------------------### */
	/*    defines							*/
	/* ###------------------------------------------------------### */

#define VBH_BEIFS     0       /* if     statement ptype code		*/
#define VBH_BECAS     1	      /* case   statement ptype code		*/
#define VBH_BEASG     2	      /* assign statement ptype code		*/
#define VBH_BEVAR     3	      /* var assign statement ptype code	*/
#define VBH_BEFOR     4	      /* for    statement ptype code		*/
#define VBH_BENXT     5	      /* next   statement ptype code		*/
#define VBH_BEEXT     6	      /* exit   statement ptype code		*/
#define VBH_BEWAS     7	      /* exit   statement ptype code		*/
#define VBH_BEWHI     8       /* while  statement ptype code            */
#define VBH_BERET     9       /* return statement ptype code            */
#define VBH_BECAL    10       /* call   statement ptype code            */
#define VBH_BELOP    11       /* loop   statement ptype code            */
#define VBH_BEAGR    12       /* agregate statement ptype code          */

# define VBH_TIME_UNIT_UNKNOWN    0
# define VBH_TIME_UNIT_FS         1
# define VBH_TIME_UNIT_PS         2
# define VBH_TIME_UNIT_NS         3
# define VBH_TIME_UNIT_MS         4
# define VBH_MAX_TIME_UNIT        5


#define VBH_ATTR_ENTITY         0
#define VBH_ATTR_ARCHITECTURE   1
#define VBH_ATTR_PACKAGE        2
#define VBH_ATTR_FUNCTION       3
#define VBH_ATTR_PROCEDURE      4
#define VBH_ATTR_SUBTYPE        5
#define VBH_ATTR_CONSTANT       6
#define VBH_ATTR_VARIABLE       7
#define VBH_ATTR_SIGNAL         8
#define VBH_ATTR_LABEL          9
#define VBH_ATTR_TYPE          10
#define VBH_ATTR_CONFIGURATION 11
#define VBH_ATTR_COMPONENT     12

# define VBH_TYPE_SEVERITY            VEX_TYPE_SEVERITY
# define VBH_TYPE_BOOLEAN             VEX_TYPE_BOOLEAN
# define VBH_TYPE_CHARACTER           VEX_TYPE_CHARACTER
# define VBH_TYPE_STRING              VEX_TYPE_STRING
# define VBH_TYPE_BIT                 VEX_TYPE_BIT
# define VBH_TYPE_INTEGER             VEX_TYPE_INTEGER
# define VBH_TYPE_NATURAL             VEX_TYPE_NATURAL
# define VBH_TYPE_BIT_VECTOR          VEX_TYPE_BIT_VECTOR
# define VBH_TYPE_STD_ULOGIC          VEX_TYPE_STD_ULOGIC
# define VBH_TYPE_STD_LOGIC           VEX_TYPE_STD_LOGIC
# define VBH_TYPE_STD_ULOGIC_VECTOR   VEX_TYPE_STD_ULOGIC_VECTOR
# define VBH_TYPE_STD_LOGIC_VECTOR    VEX_TYPE_STD_LOGIC_VECTOR
# define VBH_TYPE_X01                 VEX_TYPE_X01
# define VBH_TYPE_X01Z                VEX_TYPE_X01Z
# define VBH_TYPE_UX01                VEX_TYPE_UX01
# define VBH_TYPE_UX01Z               VEX_TYPE_UX01Z
# define VBH_TYPE_UNSIGNED            VEX_TYPE_UNSIGNED
# define VBH_TYPE_SIGNED              VEX_TYPE_SIGNED
# define VBH_TYPE_SMALL_INT           VEX_TYPE_SMALL_INT
# define VBH_TYPE_REG_BIT             VEX_TYPE_REG_BIT
# define VBH_TYPE_REG_VECTOR          VEX_TYPE_REG_VECTOR
# define VBH_TYPE_MUX_BIT             VEX_TYPE_MUX_BIT
# define VBH_TYPE_MUX_VECTOR          VEX_TYPE_MUX_VECTOR
# define VBH_TYPE_WOR_BIT             VEX_TYPE_WOR_BIT
# define VBH_TYPE_WOR_VECTOR          VEX_TYPE_WOR_VECTOR
# define VBH_MAX_TYPE                 VEX_TYPE_ENUMERATE

# define VBH_KIND_NONE           0
# define VBH_KIND_BUS            1
# define VBH_KIND_REGISTER       2
# define VBH_MAX_KIND            3

# define VBH_CLASS_NONE            0
# define VBH_CLASS_CONSTANT        1
# define VBH_CLASS_VARIABLE        2
# define VBH_CLASS_SIGNAL          3
# define VBH_MAX_CLASS             4

# define VBH_PROCESS_SEQUENTIAL_MASK       0x01
# define VBH_PROCESS_WITHSELECT_MASK       0x02
# define VBH_PROCESS_GUARDED_MASK          0x04
# define VBH_PROCESS_CONDITIONNAL_MASK     0x08

# define VBH_GENERATE_IF                   0x00
# define VBH_GENERATE_FOR                  0x01

# define VBH_DYNAMIC_TO                    0x1
# define VBH_DYNAMIC_DOWNTO                0x2

# define IsVbhProcSequential( P )      ( (P)->TYPE & VBH_PROCESS_SEQUENTIAL_MASK     )
# define IsVbhProcWithSelect( P )      ( (P)->TYPE & VBH_PROCESS_WITHSELECT_MASK     )
# define IsVbhProcConditionnal( P )    ( (P)->TYPE & VBH_PROCESS_CONDITIONNAL_MASK   )
# define IsVbhProcGuarded( P )         ( (P)->TYPE & VBH_PROCESS_GUARDED_MASK        )
                                       
# define SetVbhProcSequential( P )     ( (P)->TYPE |= VBH_PROCESS_SEQUENTIAL_MASK    )
# define SetVbhProcWithSelect( P )     ( (P)->TYPE |= VBH_PROCESS_WITHSELECT_MASK    )
# define SetVbhProcConditionnal( P )   ( (P)->TYPE |= VBH_PROCESS_CONDITIONNAL_MASK  )
# define SetVbhProcGuarded( P )        ( (P)->TYPE |= VBH_PROCESS_GUARDED_MASK       )

# define ClearVbhProcSequential( P )   ( (P)->TYPE &= ~VBH_PROCESS_SEQUENTIAL_MASK   )
# define ClearVbhProcWithSelect( P )   ( (P)->TYPE &= ~VBH_PROCESS_WITHSELECT_MASK   )
# define ClearVbhProcConditionnal( P ) ( (P)->TYPE &= ~VBH_PROCESS_CONDITIONNAL_MASK )
# define ClearVbhProcGuarded( P )      ( (P)->TYPE &= ~VBH_PROCESS_GUARDED_MASK      )

	/* ###------------------------------------------------------### */
	/*    structure definitions					*/
	/* ###------------------------------------------------------### */

typedef struct vbtyp		/* type or subtype			*/
{
  struct vbtyp   *NEXT;		/* next vbtyp				*/
  char           *NAME;		/* type's name				*/
  long            LEFT;		/* left bound				*/
  long            RIGHT;	/* right bound				*/
  char          **VALUE;	/* list of values			*/
  unsigned int    SIZE;		/* number of values			*/
  unsigned int    BYTE;		/* number of bytes needed to code values*/ 
  char            CLASS;	/* type's class (E, I, A, S)		*/
  unsigned char   INDEX;    	/* the type id number                	*/
  struct vbtyp   *BASE;		/* the base type or element's type	*/
  struct vbfun   *RESOLV;	/* resolution function		        */
  unsigned char   DYNAMIC;
  vexexpr        *DYNAMIC_LEFT;
  vexexpr        *DYNAMIC_RIGHT;
  long            FLAGS;
  long            LINE;
}
vbtyp_list;

typedef struct vbfig		/* behaviour figure			*/
  {
  struct vbfig   *NEXT;		/* next figure				*/
  char           *NAME;		/* model or package's name		*/
  struct vbaux   *BEAUX;	/* internal signals			*/
  struct vbcst   *BECST;	/* internal constants			*/
  struct vbpor   *BEPOR;	/* ports (reverse order of declaration)	*/
  struct vbgen   *BEGEN;        /* generics declarations                */
  struct vbpcs   *BEPCS;	/* list of processes			*/
  struct vbfun   *BEFUN;	/* list of functions	 		*/
  struct vbtyp   *BETYP;	/* list of declared types		*/
  struct vbatr   *BEATR;        /* list of declared attributes          */
  struct vbgnr   *BEGNR;        /* list of generates                    */
  authtable      *HASH_TYPE;    /* hash table for types                 */
  struct vbmod   *BEMOD;        /* list of declared model               */
  struct vbins   *BEINS;        /* list of instance                     */
  struct ptype   *USER;		/* reserved for user's applications	*/
  chain_list     *PACK_LIST;    /* list of used packages                */
  unsigned char   IS_PACKAGE;      /* package figure ?                     */
  char            FLAG;		/* error flags				*/
  }
vbfig_list;

typedef struct vbgnr		/* generate statements                  */
  {
  struct vbgnr   *NEXT;		/* next figure				*/
  char           *LABEL;	/* generate's label                     */
  char            TYPE;         /* generate's type (FOR/IF)             */

  vexexpr        *FOR_VARIABLE; /* for variable                         */
  vexexpr        *FOR_LEFT;     /* for left bound                       */
  vexexpr        *FOR_RIGHT;    /* for right bound                      */
  char            FOR_UP;	/* for to or downto                     */
  vexexpr        *IF_COND;      /* if condition                         */

  struct vbpcs   *BEPCS;	/* list of processes			*/
  struct vbins   *BEINS;        /* list of instance                     */
  struct vbgnr   *BEGNR;        /* list of generates                    */
  long            LINE;
  }
vbgnr_list;

typedef struct vbmod
{
  struct vbmod *NEXT;
  char         *NAME;		/* Name of the model    */
  struct vbpor *BEPOR;          /* Ports of the model   */
  struct vbgen *BEGEN;          /* Generic declarations */
  long          LINE;
}
vbmod_list;

typedef struct vbins
{
  struct vbins *NEXT;
  char         *NAME;		/* Name of instance             */
  char         *MODEL;          /* Name of the model            */
  struct vbmap *PORT_MAP;       /* List of port association     */
  struct vbmap *GEN_MAP;        /* List of generics association */
  long          LINE;
}
vbins_list;

typedef struct vbmap
{
  struct vbmap *NEXT;
  vexexpr      *FORMAL;
  vexexpr      *ACTUAL;
  long          LINE;
}
vbmap_list;

typedef struct vbgen		/* generic         			*/
{	
  struct vbgen	*NEXT;		/* next constant			*/
  vexexpr       *TARGET;	/* generic  name			*/
  vexexpr     	*VEX;		/* generic  init (VEX)			*/
  vbtyp_list    *TYPE;		/* generic  type			*/
  long           LINE;
}
vbgen_list;

typedef struct vbatr
{
  struct vbatr *NEXT;
  char         *NAME;		
  char         *ENTITY;
  unsigned int  TYPE;
  char         *VALUE;
  long          LINE;
}
vbatr_list;

typedef struct vbaux		/* auxiliary signal			*/
  {	
  struct vbaux	*NEXT;		/* next signal				*/
  vexexpr       *TARGET;	/* signal's name			*/
  vexexpr     	*VEX;		/* signal's init (VEX)			*/
  unsigned char  KIND;          /* signal's kind                        */
  vbtyp_list    *TYPE;		/* signal's type			*/
  long           LINE;
  }
vbaux_list;

typedef struct vbcst		/* constant        			*/
  {	
  struct vbcst	*NEXT;		/* next constant			*/
  vexexpr       *TARGET;	/* constant name			*/
  vexexpr     	*VEX;		/* constant init (VEX)			*/
  vbtyp_list    *TYPE;		/* constant type			*/
  long           LINE;
  }
vbcst_list;

typedef struct vbpor		/* port					*/
  {
  struct vbpor *NEXT;		/* next port				*/
  vexexpr      *TARGET;		/* port's name  			*/
  vexexpr      *VEX;		/* port's init (VEX)			*/
  unsigned char DIR;		/* port's mode (same as LOCON)		*/
  unsigned char KIND;           /* port's kind                          */
  vbtyp_list   *TYPE;		/* port's type (B, M or W)		*/
  long          LINE;
  }
vbpor_list;

typedef struct vbpcs		/* process				*/
{
  struct vbpcs *NEXT;		/* next process				*/
  char         *LABEL;		/* process's label			*/
  struct chain *SENSITIVITY;	/* list of sensitivity			*/
  struct ptype *VARIABLE;       /* list of variable                     */
  struct ptype *INSTRUCTION;	/* list of instructions			*/
  unsigned char TYPE;           /* process type                         */
  long          LINE;
}
vbpcs_list;

typedef struct vbifs		/* if statement				*/
{
  vexexpr      *CND;		/* condition (vex)			*/
  struct ptype *CNDTRUE;	/* list of instructions when true	*/
  struct ptype *CNDFALSE;	/* list of instructions when false	*/
  long          LINE;
}
vbifs_list;

typedef struct vbasg		/* signal assignment statement		*/
{
  vexexpr       *TARGET;	/* target  (vex)			*/
  vexexpr       *VEX;		/* condition (vex)			*/
  vbtyp_list    *TYPE;		/* signal's type			*/
  long           LINE;
}
vbasg_list;

typedef struct vbagr
{
  struct ptype *ASSIGN;
  long          LINE;
}
vbagr_list;

typedef struct vbvar		/* variable assignment statement	*/
{
  vexexpr      *TARGET;	        /* target's name			*/
  vexexpr      *VEX;		/* condition (vex)			*/
  vbtyp_list   *TYPE;		/* signal's type			*/
  long          LINE;
}
vbvar_list;

typedef struct vbcas		/* case statement			*/
{
  vexexpr       *VEX;	        /* list of expression (VEX)		*/
  struct vbcho  *CHOICE;	/* table of choices			*/
  vbtyp_list    *TYPE;		/* expression's type			*/
  unsigned int   SIZE;		/* size of choices' table		*/
  void          *USER;
  long           LINE;
}
vbcas_list;

typedef struct vbcho		/* a choice in a case instruction	*/
{
  chain_list   *VALUES;		/* the value's list of the choice       */
  unsigned int  SIZE;		/* size of the value in byte		*/
  struct ptype *INSTRUCTION;	/* list of instructions			*/
  long          LINE;
}
vbcho_list;

typedef struct vbwhi		/* a loop instruction			*/
{
  char         *LABEL;		/* the name of the loop			*/
  vexexpr      *CND;            /* condition clause                     */
  struct ptype *INSTRUCTION;	/* list of instructions			*/
  long          LINE;
}
vbwhi_list;

typedef struct vblop		/* a loop instruction			*/
{
  char         *LABEL;		/* the name of the loop			*/
  struct ptype *INSTRUCTION;	/* list of instructions			*/
  long          LINE;
}
vblop_list;

typedef struct vbfor		/* a loop instruction			*/
{
  char         *LABEL;		/* the name of the loop			*/
  vexexpr      *VARIABLE;       /* the name of the variable             */
  vexexpr      *LEFT;
  vexexpr      *RIGHT;
  char          UP;		/* to or downto                         */
  struct ptype *INSTRUCTION;	/* list of instructions			*/
  long          LINE;
}
vbfor_list;

typedef struct vbwas          /* sequential wait statement            */
{
  struct chain *SEN;          /* senisitvity list                     */
  vexexpr      *CND;          /* condition clause                     */
  unsigned char TIME_UNIT;    /* name of the time unit		      */
  vexexpr      *TIMEOUT;      /* timeout clause                       */
  long          LINE;
}
vbwas_list;

typedef struct vbext		/* exit statement			*/
{
  char	       *LABEL;	        /* label of the targeted loop		*/
  vexexpr      *CND;		/* condition (vex)			*/
  long          LINE;
}
vbext_list;

typedef struct vbnxt		/* next statement			*/
{
  char	       *LABEL;		/* label of the targeted loop		*/
  vexexpr      *CND;		/* condition (vex)			*/
  long          LINE;
}
vbnxt_list;

typedef struct vbret		/* return statement			*/
{
  vexexpr      *RET;		/* expression (vex)			*/
  long          LINE;
}
vbret_list;

typedef struct vbcal		/* call statement			*/
{
  vexexpr      *CALL;		/* procedure call (vex)			*/
  long          LINE;
}
vbcal_list;

typedef struct vbarg
{
  struct vbarg *NEXT;		/* next param				*/
  vexexpr      *TARGET;		/* param's name  			*/
  char          DIR;		/* param's mode (same as LOCON)		*/
  char          CLASS;          /* param's class C,V,S                  */
  vbtyp_list   *TYPE;		/* param's type (B, M or W)		*/
  long          LINE;
}
vbarg_list;

typedef struct vbfun		/* function statement                   */
{
  struct vbfun *NEXT;		/* next process				*/
  char         *NAME;		/* function's name 			*/
  struct vbarg *ARGUMENT;	/* list of arguments  			*/
  struct vbarg *RETURN;         /* return value                         */
  struct ptype *VARIABLE;       /* list of variable                     */
  struct ptype *INSTRUCTION;	/* list of instructions			*/
  unsigned char TYPE;           /* function type                        */
  long          LINE;
}
vbfun_list;

	/* ###------------------------------------------------------### */
	/*    low-level functions					*/
	/* ###------------------------------------------------------### */

extern struct vbaux  *vbh_addvbaux ();
extern struct vbatr  *vbh_addvbatr ();
extern struct vbcst  *vbh_addvbcst ();
extern struct vbgen  *vbh_addvbgen ();
extern struct vbfig  *vbh_addvbfig ();
extern struct vbpor  *vbh_addvbpor ();
extern struct ptype  *vbh_addvbifs ();
extern struct ptype  *vbh_addvbasg ();
extern struct ptype  *vbh_addvbcas ();
extern struct ptype  *vbh_addvbfor ();
extern struct ptype  *vbh_addvbwhi ();
extern struct ptype  *vbh_addvblop ();
extern struct vbpcs  *vbh_addvbpcs ();
extern struct vbarg  *vbh_addvbarg ();
extern ptype_list    *vbh_addvbagr ();
extern struct vbfun  *vbh_addvbfun ();
extern struct vbnxt  *vbh_addvbnxt ();
extern struct vbret  *vbh_addvbret ();
extern struct vbcal  *vbh_addvbcal ();
extern struct vbext  *vbh_addvbext ();
extern struct vbwas  *vbh_addvbwas ();
extern ptype_list    *vbh_addvbvar ();
extern struct vbtyp  *vbh_addvbtyp ();
extern struct vbmap  *vbh_addvbmap ();
extern struct vbmod  *vbh_addvbmod ();
extern struct vbins  *vbh_addvbins ();
extern struct vbgnr  *vbh_addvbgnr ();

extern struct vbtyp  *vbh_crtvbtyp ();

extern void           vbh_delvbfig ();
extern void           vbh_frevbfig ();
extern void           vbh_frevbaux ();
extern void           vbh_frevbatr ();
extern void           vbh_frevbcst ();
extern void           vbh_frevbgen ();
extern void           vbh_frevbpor ();
extern void           vbh_frevbpcs ();
extern void           vbh_frevbarg ();
extern void           vbh_frevbfun ();
extern void           vbh_frevbifs ();
extern void           vbh_frevbasg ();
extern void           vbh_frevbcas ();
extern void           vbh_frevbwhi ();
extern void           vbh_frevblop ();
extern void           vbh_frevbfor ();
extern void           vbh_frevbpcs ();
extern void           vbh_frevbnxt ();
extern void           vbh_frevbret ();
extern void           vbh_frevbcal ();
extern void           vbh_frevbext ();
extern void           vbh_frevbwas ();
extern void           vbh_frevbvar ();
extern void           vbh_frevbagr ();
extern void           vbh_frevbtyp ();
extern void           vbh_frevbinst ();
extern void           vbh_frevbmod ();
extern void           vbh_frevbins ();
extern void           vbh_frevbmap ();
extern void           vbh_frevbgnr ();

extern void           vbh_viewvbfig();
extern void           vbh_viewvbtyp();
extern void           vbh_viewvbinst();

extern void           vbh_simpvbfig();
extern void           vbh_forallvexvbfig();
extern void           vbh_simpvbinst();

extern vbtyp_list   *vbh_prdeftyp();
extern vbtyp_list   *vbh_getvbtyp();
extern vbtyp_list   *vbh_getvbtypbyindex(); 

extern void          vbh_dupvbfig();
extern ptype_list   *vbh_dupvbinst();

	/* ###------------------------------------------------------### */
	/*    parser-driver functions					*/
	/* ###------------------------------------------------------### */

#endif
