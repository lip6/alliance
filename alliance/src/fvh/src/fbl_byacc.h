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
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/* ###--------------------------------------------------------------### */
/*									*/
/* file		: fbl_byacc.h						*/
/* date		: Jun 15 1992						*/
/* version	: v102							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: declaration of external functions and global variables*/
/*		  used by yacc						*/
/*								 	*/
/* ###--------------------------------------------------------------### */

# if 0
typedef struct fbl_expr
  {
  char          *IDENT;			/* identifier or constant name	*/
  struct chain  *LIST_ABL;		/* pointer on fbl_abllst list	*/
  char 		 TYPE;
  short          WIDTH;			/* width of bit vector		*/
  }
fbl_ablstr;
# endif

typedef struct
  {
  char          *NAME;			/* identifier name		*/
  short          LEFT;			/* vector's left index		*/
  short          RIGHT;			/* vector's right index		*/
  char           FLAG;			/* scalar ('S') or array 'A'	*/
  char           TYPE;			/* scalar ('S') or array 'A'	*/
  }
fbl_name;

struct g_type
  {
  long  VALU;
  char FLAG;
  };

struct choice_chain
  {
  struct choice_chain* NEXT;	
  char  *VALUE;
  unsigned long  SIZE;
  struct ptype *INSTRUCTION;
  };

char         *FBL_MODNAM;		/* curnennt model name		*/
fbl_ablstr    FBL_SLCEXP;		/* structure filled with the	*/
fbl_ablstr    FBL_EMPSTR;		/* empty structure used with NOT*/
 
struct bddnode *FBL_BDDPNT = NULL;		/* BDD pointer			*/
static char         *FBL_LBLNAM = NULL;	/* label			*/
static long           FBL_NUMTYP = 0;	/* nombre de type			*/
static struct chain *FBL_NM1LST = NULL;	/* 1-st name liste		*/
static struct chain *FBL_GRDLST = NULL;	/* list of guard's ABL		*/
static struct chain *FBL_CNDLST = NULL;	/* list of conditions (ABL)	*/
static struct chain *FBL_VALLST = NULL;	/* list of waveforms (ABL)	*/
static struct fbfig *FBL_BEFPNT = NULL;	/* current BEFIG pointer	*/

static struct choice_chain  *FBL_CASPNT = NULL;
static struct ptype *FBL_OTHPNT = NULL;
static struct ptype *FBL_PTYPE = NULL;

static struct dct_entry  *FBL_DCEHED;	/* free dct_entry's head	*/
static struct dct_recrd  *FBL_DCRHED;	/* free dct_recrd's head	*/

static authtable *FBL_HASH_ASG_FSM = (authtable *)0;

extern char          FBL_ERRFLG;	/* set to 1 in case of error	*/
extern struct fbfig *FBL_HEADFIG;	/* head of fbfigs		*/
extern long           FBL_LINNUM;
extern long           FBL_NUMPTP;


static struct dct_entry **hshtab;
 
  extern  struct fbgen * fbl_addgen __P((struct fbgen *lastgeneric, struct chain *nam_lst, struct chain *nat_lst, char *type, short left, short right));
  extern            long  val_type __P((char *name));

  extern            long  fbl_tobin __P((char *trg, char *src, long left, long right));
  extern            long  fbl_bintonum __P((char *str));
  extern     fbl_ablstr  fbl_crtabl __P((short oper, fbl_ablstr expr1, fbl_ablstr expr2, long left, long right));
  extern     fbl_ablstr  fbl_cpyablstr __P((fbl_ablstr ablstr));
extern void         fbl_select();
  extern          char * fbl_stostr __P((char *str));

  extern           void  fbl_y_error __P((char *str));
  extern            long  fbl_y_lex __P(());

