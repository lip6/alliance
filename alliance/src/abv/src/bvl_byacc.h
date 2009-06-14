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
/* file		: bvl_byacc.h						*/
/* date		: Oct  4 1993						*/
/* version	: v108							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: declaration of external functions and global variables*/
/*		  used by yacc						*/
/*								 	*/
/* ###--------------------------------------------------------------### */

typedef struct bvl_expr
  {
  char          *IDENT;			/* identifier or constant name	*/
  struct chain  *LIST_ABL;		/* pointer on bvl_abllst list	*/
  short          WIDTH;			/* width of bit vector		*/
  }
bvl_ablstr;

typedef struct
  {
  char          *NAME;			/* identifier name		*/
  short          LEFT;			/* vector's left index		*/
  short          RIGHT;			/* vector's right index		*/
  char           FLAG;			/* scalar ('S') or array 'A'	*/
  }
bvl_name;

struct g_type
  {
  long  VALU;
  char FLAG;
  };

struct bddnode *BVL_BDDPNT;		/* BDD pointer			*/
char         *BVL_MODNAM;		/* curnennt model name		*/
bvl_ablstr    BVL_SLCEXP;		/* structure filled with the	*/
bvl_ablstr    BVL_EMPSTR;		/* empty structure used with NOT*/

extern long    BVL_AUXMOD;		/* simplify internal sig (= 1)	*/
 
static char         *BVL_LBLNAM = NULL;	/* label			*/
static struct chain *BVL_NM1LST = NULL;	/* 1-st name liste		*/
static struct chain *BVL_GRDLST = NULL;	/* list of guard's ABL		*/
static struct chain *BVL_CNDLST = NULL;	/* list of conditions (ABL)	*/
static struct chain *BVL_VALLST = NULL;	/* list of waveforms (ABL)	*/
static struct befig *BVL_BEFPNT = NULL;	/* current BEFIG pointer	*/

static struct beden **dic = NULL;	/* dictionary			*/

static long chkdcl ( char object, long  mode, long  type, char flag, long  kind, char constraint, long  *conf  );
struct chain *BVL_INTLST = NULL;
struct begen *BVL_GENPNT = NULL;

extern char          BVL_ERRFLG;	/* set to 1 in case of error	*/
extern struct befig *BVL_HEDFIG;	/* head of befigs		*/

extern struct begen      *bvl_addgen();

extern bvl_ablstr bvl_crtabl ( short      oper, bvl_ablstr expr1, bvl_ablstr expr2, long        left, long        right );
extern bvl_ablstr bvl_cpyablstr ( bvl_ablstr ablstr );

extern void bvl_select ( 
bvl_ablstr   *result,
struct chain *pt_str,		/* pointer on a list of choices		*/
bddnode     **pt_bdd,		/* used to check if a choice is legal	*/
bvl_ablstr   pt_ablstr );	/* tested expression			*/
extern void bvl_error ( long   code, char *str1 );
extern void *addstr ( 
struct befig *ptfig,
char          object,
long           mode,
long           type,
char          flag,
char         *name,
short         left,
short         right );
