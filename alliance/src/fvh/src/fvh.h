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
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: fvh.h							*/
/* date		: Aug 28 1992						*/
/* version	: 102							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* contents	: defines and structure definitions used in FBH library	*/
/*									*/
/* ###--------------------------------------------------------------### */

#ifndef FBH_DEFINE
#define FBH_DEFINE

	/* ###------------------------------------------------------### */
	/*    defines							*/
	/* ###------------------------------------------------------### */

#define FBH_GENERIC 203698	/* generic ptype code			*/
#define FBH_BEIFS   205635	/* if     statement ptype code		*/
#define FBH_BECAS   423240	/* case   statement ptype code		*/
#define FBH_BEMSG   153676	/* assert statement ptype code		*/
#define FBH_BEASG   967324	/* assign statement ptype code		*/

#define FBH_TYPE_SEVERITY  1
#define FBH_TYPE_BOOLEAN   2	 
#define FBH_TYPE_BIT       3
#define FBH_TYPE_INTEGER   4
#define FBH_TYPE_MUX_BIT   5
#define FBH_TYPE_WOR_BIT   6
#define FBH_TYPE_REG_BIT   7
#define FBH_TYPE_NATURAL   8
#define FBH_TYPE_BIT_VEC   9
#define FBH_TYPE_MUX_VEC   10
#define FBH_TYPE_WOR_VEC   11
#define FBH_TYPE_REG_VEC   12

# define FBH_ASSIGN_BY_FSM_MASK     0x1

# define SetFbhAssignByFsm( O )   ( (O)->FLAG |=  FBH_ASSIGN_BY_FSM_MASK )
# define ClearFbhAssignByFsm( O ) ( (O)->FLAG &= ~FBH_ASSIGN_BY_FSM_MASK )
# define IsFbhAssignByFsm( O )    ( (O)->FLAG  &  FBH_ASSIGN_BY_FSM_MASK )

	/* ###------------------------------------------------------### */
	/*    structure definitions					*/
	/* ###------------------------------------------------------### */

typedef struct fbfig		/* fbhaviour figure			*/
  {
  struct fbfig   *NEXT;		/* next figure				*/
  char           *NAME;		/* figure's name			*/
  struct fbreg   *BEREG;	/* internal signals (REGISTER)		*/
  struct fbmsg   *BEMSG;	/* concurrent assert instructions	*/
  struct fbrin   *BERIN;	/* inputs (registers and ports)		*/
  struct fbout   *BEOUT;	/* output ports				*/
  struct fbbus   *BEBUS;	/* output ports (BUS)			*/
  struct fbaux   *BEAUX;	/* internal signals			*/
  struct fbaux   *BEDLY;	/* internal delayed signals			*/
  struct fbbux   *BEBUX;	/* internal signals (BUS)		*/
  struct fbpor   *BEPOR;	/* ports (reverse order of declaration)	*/
  struct fbpcs   *BEPCS;	/* list of processes			*/
  struct bddcircuit  *CIRCUI;	/* Circuit's pointer used for BDDs	*/
  struct ptype   *USER;		/* reserved for user's applications	*/
  unsigned long   FLAG;		/* error flags				*/
  unsigned char   TYPE;		/* type of the description (not used)	*/
  struct fbtyp   *BETYP;	/* list of declared types		*/
  struct fbpgm   *BEPGM;	/* list of pragmas			*/
  }
fbfig_list;

typedef struct fbreg		/* register				*/
  {
  struct fbreg  *NEXT;		/* next register			*/
  char          *NAME;		/* register's name			*/
  struct fbbiabl  *BIABL;		/* list of drivers (ABL)		*/
  struct fbbinode *BINODE;	/* list of drivers (BDD)		*/
  unsigned long  FLAG;
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbreg_list;

typedef struct fbmsg		/* assert instruction			*/
  {
  struct fbmsg 	*NEXT;		/* next assertion			*/
  char		 LEVEL;		/* severity level of the assertion	*/
  char		*LABEL;		/* instruction's label			*/
  char		*MESSAGE;	/* assertion's message			*/
  struct chain	*ABL;		/* assertion's condition (ABL)		*/
  bddnode 	*NODE;		/* assertion's condition (BDD)		*/
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbmsg_list;

typedef struct fbout		/* output port				*/
  {
  struct fbout	*NEXT;		/* next output				*/
  char		*NAME;		/* port's name				*/
  struct chain	*ABL;		/* port's equation (ABL)		*/
  bddnode 	*NODE;		/* port's equation (BDD)		*/
  unsigned char  TYPE;		/* port's type				*/
  unsigned long  FLAG;
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbout_list;

typedef struct fbbus		/* output port (BUS)			*/
  {
  struct fbbus	*NEXT;		/* next bus port			*/
  char		*NAME;		/* port's name				*/
  struct fbbiabl	*BIABL;		/* list of drivers (ABL)		*/
  struct fbbinode	*BINODE;	/* list of drivers (BDD)		*/
  unsigned char  TYPE;		/* port's type : W or M			*/
  unsigned long  FLAG;
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbbus_list;

typedef struct fbaux		/* auxiliary signal			*/
  {	
  struct fbaux	*NEXT;		/* next signal				*/
  char		*NAME;		/* signal's name			*/
  struct chain	*ABL;		/* signal's equation (ABL)		*/
  bddnode 	*NODE;		/* signal's equation (BDD)		*/
  unsigned char  TYPE;		/* signal's type			*/
  unsigned long  FLAG;
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbaux_list;

typedef struct fbbux		/* internal signal (BUS)		*/
  {	
  struct fbbux	*NEXT;		/* next signal				*/
  char		*NAME;		/* signal's name			*/
  struct fbbiabl	*BIABL;		/* list of drivers (ABL)		*/
  struct fbbinode	*BINODE;	/* list of drivers (BDD)		*/
  unsigned char  TYPE;		/* signal's type : W or M		*/
  unsigned long  FLAG;
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbbux_list;

typedef struct fbpor		/* port					*/
  {
  struct fbpor *NEXT;		/* next port				*/
  char         *NAME;		/* port's name				*/
  char          DIRECTION;	/* port's mode (same as LOCON)		*/
  unsigned char TYPE;		/* port's type (B, M or W)		*/
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbpor_list;

typedef struct fbbiabl		/* couple of ABL			*/
  {
  struct fbbiabl	*NEXT;		/* next couple				*/
  char          *LABEL;		/* driver's label			*/
  struct chain	*CNDABL;	/* driver's connection condition (ABL)	*/
  struct chain	*VALABL;	/* equation of driven value (ABL)	*/
  unsigned long  LINE_NUM;
  void         *USER;
  }
fbbiabl_list;

typedef struct fbbinode		/* couple of BDD			*/
  {
  struct fbbinode	*NEXT;	 	/* next figure				*/
  bddnode 	*CNDNODE;	/* driver's connection condition (BDD)	*/
  bddnode 	*VALNODE;	/* equation of driven value (BDD)	*/
  unsigned long  LINE_NUM;
  void         *USER;
  }
fbbinode_list;

typedef struct fbrin		/* primary inputs			*/
  {
  struct fbrin *NEXT;		/* next input				*/
  char         *NAME;		/* input's name				*/
  unsigned long  LINE_NUM;
  void         *USER;
  }
fbrin_list;

typedef struct fbgen		/* generic				*/
  {
  struct fbgen  *NEXT;		/* next generic				*/
  char 	        *NAME;		/* generic's name			*/
  char          *TYPE;		/* generic type's name			*/
  void          *VALUE;		/* generic's value			*/
  unsigned long  LINE_NUM;
  void         *USER;
  }
fbgen_list;

typedef struct fbpcs		/* process				*/
  {
  struct fbpcs *NEXT;		/* next process				*/
  char         *LABEL;		/* process's label			*/
  struct chain *SENSITIVITY;	/* list of sensitivity			*/
  struct ptype *INSTRUCTION;	/* list of instructions			*/
  struct chain *OUTPUTS;	/* list of assigned signals		*/
  unsigned long LINE_NUM;
  unsigned long FLAG;		/* flag (not used)			*/
  void         *USER;
  }
fbpcs_list;

typedef struct fbifs		/* if statement				*/
  {
  struct chain *CND;		/* condition (abl)			*/
  struct ptype *CNDTRUE;	/* list of instructions when true	*/
  struct ptype *CNDFALSE;	/* list of instructions when false	*/
  unsigned long  LINE_NUM;
  void         *USER;
  }
fbifs_list;

typedef struct fbasg		/* signal assignment statement		*/
  {
  char          *NAME;		/* target's name			*/
  struct chain  *ABL;		/* condition (abl)			*/
  unsigned char  TYPE;		/* signal's type			*/
  unsigned long  LINE_NUM;
  void         *USER;
  }
fbasg_list;

typedef struct fbcas		/* case statement			*/
  {
  struct chain  *ABL;		/* expression (ABL)			*/
  struct fbcho  *CHOICE;	/* table of choices			*/
  unsigned char  TYPE;		/* expression's type			*/
  unsigned int   SIZE;		/* size of choices' table		*/
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbcas_list;

typedef struct fbcho		/* a choice in a case instruction	*/
  {
  char         *VALUE;		/* the value of the choice		*/
  unsigned int  SIZE;		/* size of the value in byte		*/
  struct ptype *INSTRUCTION;	/* list of instructions			*/
  unsigned long  LINE_NUM;
  void          *USER;
  }
fbcho_list;

typedef struct fbtyp		/* type or subtype			*/
  {
  struct fbtyp   *NEXT;		/* next fbtyp				*/
  char           *NAME;		/* type's name				*/
  int             LEFT;		/* left bound				*/
  int             RIGHT;	/* right bound				*/
  char          **VALUE;	/* list of values			*/
  unsigned int    SIZE;		/* number of values			*/
  unsigned int    BYTE;		/* number of bytes needed to code values*/ 
  char            CLASS;	/* type's class (E, I, A, S)		*/
  unsigned char   INDEX;	/* the type of the index for an array	*/
  unsigned char   BASE;		/* the base type or element's type	*/
  unsigned char   RESOLV;	/* index of resolution function		*/
  unsigned long  LINE_NUM;
  void           *USER;
  }
fbtyp_list;

typedef struct fbpgm		/* pragma				*/
  {
  struct fbpgm *NEXT;		/* next pragma				*/
  char         *TYPE;		/* prgma's type				*/
  char         *NAME;		/* object's name			*/
  void         *VALUE;		/* prgma's content			*/
  unsigned long  LINE_NUM;
  void         *USER;
  }
fbpgm_list;


	/* ###------------------------------------------------------### */
	/*    low-level functions					*/
	/* ###------------------------------------------------------### */

extern fbfig_list *FBL_HEADFIG;

  extern  struct fbaux * fbh_addfbaux __P((struct fbaux *lastfbaux, char *name, struct chain *abl_expr, bddnode *bdd_expr, unsigned char type));
  extern  struct fbbux * fbh_addfbbux __P((struct fbbux *lastfbbux, char *name, struct fbbiabl *biabl, struct fbbinode *binode, char type));
  extern  struct fbbus * fbh_addfbbus __P((struct fbbus *lastfbbus, char *name, struct fbbiabl *biabl, struct fbbinode *binode, char type));
  extern  struct fbfig * fbh_addfbfig __P((struct fbfig *lastfbfig, char *name));
  extern  struct fbgen * fbh_addfbgen __P((struct fbgen *lastfbgen, char *name, char *type, void *value ));
  extern  struct fbmsg * fbh_addfbmsg __P((struct fbmsg *lastfbmsg, char *label, char level, char *message, struct chain *abl_expr, bddnode *bdd_expr));
  extern  struct fbout * fbh_addfbout __P((struct fbout *lastfbout, char *name, struct chain *abl_expr, bddnode *bdd_expr, unsigned char type));
  extern  struct fbpor * fbh_addfbpor __P((struct fbpor *lastfbpor, char *name, char dir, unsigned char type));
  extern  struct fbreg * fbh_addfbreg __P((struct fbreg *lastfbreg, char *name, struct fbbiabl *biabl, struct fbbinode *binode));
  extern  struct fbrin * fbh_addfbrin __P((struct fbrin *lastfbrin, char *name));
  extern  struct fbbiabl * fbh_addbiabl __P((struct fbbiabl *lastbiabl, char *label,struct chain *condition, struct chain *value));
  extern struct fbbinode * fbh_addbinode __P((struct fbbinode *lastbinode, bddnode *condition, bddnode *value));
  extern  struct ptype * fbh_addfbifs __P((struct ptype *lastptype, struct chain *abl));
  extern  struct ptype * fbh_addfbasg __P((struct ptype *lastptype, char *name, struct chain *abl, unsigned char type));
  extern  struct ptype * fbh_addfbcas __P((struct ptype *lastptype, struct chain *abl, unsigned char type));
  extern  struct fbpcs * fbh_addfbpcs __P((struct fbpcs *lastfbpcs, char *label, struct chain *sens, struct ptype *inst, struct chain *outs, char flag));
  extern  struct fbtyp * fbh_addfbtyp __P((struct fbtyp *lastfbtyp, char *name, int left, int right, char **value, unsigned int size, unsigned int byte, unsigned char base, unsigned char index, unsigned char resolv, char class));
  extern  struct fbpgm * fbh_addfbpgm __P((struct fbpgm *lastfbpgm, char *type, char *name, void *value));
  extern  struct fbtyp * fbh_crtfbtyp __P((struct fbtyp *listfbtyp));
  extern  struct fbaux * fbh_delfbaux __P((struct fbaux *listfbaux, struct fbaux *ptfbaux, char mode));
  extern  struct fbbux * fbh_delfbbux __P((struct fbbux *listfbbux, struct fbbux *ptfbbux, char mode));
  extern  struct fbbus * fbh_delfbbus __P((struct fbbus *listfbbus, struct fbbus *ptfbbus, char mode));
  extern  struct fbfig * fbh_delfbfig __P((struct fbfig *listfbfig, struct fbfig *ptfbfig, char mode));
  extern  struct fbgen * fbh_delfbgen __P((struct fbgen *listfbgen, struct fbgen *ptfbgen, char mode));
  extern  struct fbmsg * fbh_delfbmsg __P((struct fbmsg *listfbmsg, struct fbmsg *ptfbmsg, char mode));
  extern  struct fbout * fbh_delfbout __P((struct fbout *listfbout, struct fbout *ptfbout, char mode));
  extern  struct fbpor * fbh_delfbpor __P((struct fbpor *listfbpor, struct fbpor *ptfbpor));
  extern  struct fbreg * fbh_delfbreg __P((struct fbreg *listfbreg, struct fbreg *ptfbreg, char mode));
  extern  struct fbrin * fbh_delfbrin __P((struct fbrin *listfbrin, struct fbrin *ptfbrin));
  extern  struct fbbiabl * fbh_delbiabl __P((struct fbbiabl *listbiabl, struct fbbiabl *ptbiabl, char mode));
  extern struct fbbinode * fbh_delbinode __P((struct fbbinode *listbinode, struct fbbinode *ptbinode, char mode));
  extern           void  fbh_frefbaux __P((struct fbaux *listfbaux));
  extern           void  fbh_frefbbux __P((struct fbbux *listfbbux));
  extern           void  fbh_frefbbus __P((struct fbbus *listfbbus));
  extern           void  fbh_frefbfig __P((struct fbfig *listfbfig));
  extern           void  fbh_frefbgen __P((struct fbgen *listfbgen));
  extern           void  fbh_frefbmsg __P((struct fbmsg *listfbmsg));
  extern           void  fbh_frefbout __P((struct fbout *listfbout));
  extern           void  fbh_frefbpor __P((struct fbpor *listfbpor));
  extern           void  fbh_frefbreg __P((struct fbreg *listfbreg));
  extern           void  fbh_frefbrin __P((struct fbrin *listfbrin));
  extern           void  fbh_frefbpgm __P((struct fbpgm *listfbpgm));
  extern           void  fbh_frebiabl __P((struct fbbiabl *listbiabl));
  extern           void  fbh_frebinode __P((struct fbbinode *listbinode));
  extern           void  fbh_frefbtyp __P((struct fbtyp *listfbtyp));
  extern          void * fbh_getgenval __P((struct fbgen *listfbgen, char *fbgenname));
  extern  struct fbaux * fbh_rmvfbaux __P((struct fbaux *listfbaux, char *fbauxname, char mode));
  extern  struct fbbux * fbh_rmvfbbux __P((struct fbbux *listfbbux, char *fbbuxname, char mode));
  extern  struct fbbus * fbh_rmvfbbus __P((struct fbbus *listfbbus, char *fbbusname, char mode));
  extern  struct fbfig * fbh_rmvfbfig __P((struct fbfig *listfbfig, char *fbfigname, char mode));
  extern  struct fbgen * fbh_rmvfbgen __P((struct fbgen *listfbgen, char *fbgenname, char mode));
  extern  struct fbmsg * fbh_rmvfbmsg __P((struct fbmsg *listfbmsg, char *fbmsglabl, char mode));
  extern  struct fbout * fbh_rmvfbout __P((struct fbout *listfbout, char *fboutname, char mode));
  extern  struct fbpor * fbh_rmvfbpor __P((struct fbpor *listfbpor, char *fbporname));
  extern  struct fbreg * fbh_rmvfbreg __P((struct fbreg *listfbreg, char *fbregname, char mode));
  extern  struct fbrin * fbh_rmvfbrin __P((struct fbrin *listfbrin, char *fbrinname));

  extern  struct fbtyp * fbh_prdeftyp __P(());
	/* ###------------------------------------------------------### */
	/*    high-level functions					*/
	/* ###------------------------------------------------------### */

extern void           fbh_debug ();

	/* ###------------------------------------------------------### */
	/*    system-level functions					*/
	/* ###------------------------------------------------------### */

extern FILE          *fbh_opnfile ();

	/* ###------------------------------------------------------### */
	/*    parser-driver functions					*/
	/* ###------------------------------------------------------### */

extern struct fbfig  *fbh_prsvhdl ();
extern struct fbfig  *vhdlloadfbfig();
  extern           void  vhdlsavefbfig __P((fbfig_list *pthedfbfig, char *FileName, int synopsys));

extern void vhdlsavefbdecl __P(( fbfig_list *pthedfbfig, int synopsys ));
extern void vhdlsavefbport __P(( fbfig_list *pthedfbfig, int synopsys ));
extern void vhdlsavefbarchi __P(( fbfig_list *pthedfbfig, int synopsys ));
  extern FILE *FblDriveFile;

	/* ###------------------------------------------------------### */
	/*    Tools */
	/* ###------------------------------------------------------### */

extern void FbhScanExprProcess __P(( fbpcs_list *Process, void (*FuncLeft)(), void (*FuncRight)()));

#endif
