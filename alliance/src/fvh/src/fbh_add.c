/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
/*                                                                      */
/* file        : fbh_addfbasg.c                                     */
/* date        : Aug 28 1992                                            */
/* version     : v102                                                   */
/* authors     : Pirouz BAZARGAN SABET                                  */
/* content     : contains a specific function used to create a          */
/*            fbhaviour data structure                                  */
/*                                                                      */
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "abl.h" 
#include "bdd.h"
#include "fvh.h"

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbasg                                      */
/* description     : create a BEASG structure at the top the list       */
/* called func.     : addptype, mbkalloc, namealloc                     */
/* ###--------------------------------------------------------------### */

struct ptype *fbh_addfbasg (lastptype, name, abl, type)

struct ptype  *lastptype;     /* pointer on the last ptype structure     */
char          *name;          /* signal's name                           */
struct chain  *abl;          /* condition's expression                   */
unsigned char  type;          /* signal's type (cannot be 0)             */

  {
  struct ptype *ptptype;
  struct fbasg *ptfbasg;

  name              = namealloc (name);

  ptfbasg           = (struct fbasg *) mbkalloc (sizeof(struct fbasg));
  ptfbasg->NAME     = name;
  ptfbasg->ABL      = abl;
  ptfbasg->TYPE     = type;

  ptptype           = addptype (lastptype, FBH_BEASG, (void *)ptfbasg);

  return (ptptype);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbaux                              */
/* description     : create a BEAUX structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbaux *fbh_addfbaux (lastfbaux, name, abl_expr, bdd_expr, type)

struct fbaux *lastfbaux;     /* pointer on the last fbaux structure     */
char         *name;          /* signal's name               */
struct chain *abl_expr;          /* signal's expression (ABL)          */
bddnode  *bdd_expr;          /* signal's expression (BDD)          */
unsigned char  type;          /* signal's type                */

  {
  struct fbaux *ptaux;

  name        = namealloc (name);

  ptaux       = (struct fbaux *) mbkalloc (sizeof(struct fbaux));
  ptaux->NAME = name;
  ptaux->ABL  = abl_expr;
  ptaux->NODE = bdd_expr;
  ptaux->TYPE = type;
  ptaux->NEXT = lastfbaux;

  return (ptaux);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbbus                              */
/* description     : create a BEBUS structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbbus *fbh_addfbbus (lastfbbus, name, fbbiabl, fbbinode, type)

struct fbbus  *lastfbbus;     /* pointer on the last fbbus structure     */
char          *name;          /* port's name                    */
struct fbbiabl  *fbbiabl;          /* port's drivers (ABL)               */
struct fbbinode *fbbinode;          /* port's drivers (BDD)               */
char           type;          /* port type mark (M or W)          */

  {
  struct fbbus *ptbus;

  name          = namealloc (name);

  ptbus         = (struct fbbus *) mbkalloc (sizeof(struct fbbus));
  ptbus->NAME   = name;
  ptbus->BIABL  = fbbiabl;
  ptbus->BINODE = fbbinode;
  ptbus->TYPE   = type;
  ptbus->NEXT   = lastfbbus;

  return (ptbus);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbbux                              */
/* description     : create a BEBUX structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbbux *fbh_addfbbux (lastfbbux, name, fbbiabl, fbbinode, type)

struct fbbux  *lastfbbux;     /* pointer on the last fbbux structure     */
char          *name;          /* signal's name               */
struct fbbiabl  *fbbiabl;          /* signal's expression (ABL)          */
struct fbbinode *fbbinode;          /* signal's expression (BDD)          */
char           type;          /* signal's type mark (M or W)          */

  {
  struct fbbux *ptbux;

  name          = namealloc (name);

  ptbux         = (struct fbbux *) mbkalloc (sizeof(struct fbbux));
  ptbux->NAME   = name;
  ptbux->BIABL  = fbbiabl;
  ptbux->BINODE = fbbinode;
  ptbux->TYPE   = type;
  ptbux->NEXT   = lastfbbux;

  return (ptbux);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbcas                              */
/* description     : create a BECAS structure at the top the list          */
/* called func.     : addptype, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct ptype *fbh_addfbcas (lastptype, abl,type)

struct ptype *lastptype;     /* pointer on the last ptype structure     */
struct chain *abl;          /* condition's expression          */
unsigned char type;          /*  expression's type     i               */

  {
  struct ptype *ptptype;
  struct fbcas *ptfbcas;

  ptfbcas           = (struct fbcas *) mbkalloc (sizeof(struct fbcas));
  ptfbcas->ABL      = abl;
  ptfbcas->CHOICE   = 0;
  ptfbcas->USER     = 0;
  ptfbcas->SIZE     =  0;
  ptfbcas->TYPE     = type;

  ptptype           = addptype (lastptype, FBH_BECAS, (void *)ptfbcas);

  return (ptptype);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbfig                              */
/* description     : create an empty BEFIG structure at the top of the list*/
/*            The FBH_HEDFIG list is not modified.               */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbfig *fbh_addfbfig (lastfbfig, name)

struct fbfig *lastfbfig;     /* pointer on the last fbfig structure     */
char         *name;          /* figure's name               */

  {
  struct fbfig *ptfig;

  name          = namealloc (name);

  ptfig         = (struct fbfig *) mbkalloc (sizeof(struct fbfig));
  ptfig->NAME   = name;
  ptfig->BEREG  = 0;
  ptfig->BEMSG  = 0;
  ptfig->BERIN  = 0;
  ptfig->BEOUT  = 0;
  ptfig->BEBUS  = 0;
  ptfig->BEAUX  = 0;
  ptfig->BEDLY  = 0;
  ptfig->BEBUX  = 0;
  ptfig->BEPOR  = 0;
  ptfig->BEPCS  = 0;
  ptfig->BETYP  = 0;
  ptfig->BEPGM  = 0;
  ptfig->CIRCUI = 0;
  ptfig->USER   = 0;
  ptfig->FLAG   = 0;
  ptfig->NEXT   = lastfbfig;

  return (ptfig);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbgen                              */
/* description     : create a BEGEN structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbgen *fbh_addfbgen (lastfbgen, name, type, value)

struct fbgen   *lastfbgen;     /* pointer on the last fbgen structure     */
char           *name;          /* generic's name               */
char           *type;          /* generic's type                */
void           *value;          /* generic's value               */

  {

  struct fbgen *ptgen;

  name         = namealloc (name);
  
  ptgen        = (struct fbgen *) mbkalloc (sizeof(struct fbgen));
  ptgen->NAME  = name;
  ptgen->TYPE  = type;
  ptgen->VALUE = value;
  ptgen->NEXT  = lastfbgen;

  return (ptgen);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbifs                              */
/* description     : create a BEIFS structure at the top the list          */
/* called func.     : addptype, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct ptype *fbh_addfbifs (lastptype, abl)

struct ptype *lastptype;     /* pointer on the last ptype structure     */
struct chain *abl;          /* condition's expression          */

  {
  struct ptype *ptptype;
  struct fbifs *ptfbifs;

  ptfbifs           = (struct fbifs *) mbkalloc (sizeof(struct fbifs));
  ptfbifs->CND      = abl;
  ptfbifs->CNDTRUE  = 0;
  ptfbifs->CNDFALSE = 0;
  ptptype           = addptype (lastptype, FBH_BEIFS, (void *)ptfbifs);

  return (ptptype);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbmsg                              */
/* description     : create BEMSG structure at the top the list          */
/* called func.     : mbkalloc, namealloc                         */
/* ###--------------------------------------------------------------### */

struct fbmsg *fbh_addfbmsg (lastfbmsg,label,level,message,abl_expr,bdd_expr)

struct fbmsg *lastfbmsg;     /* pointer on the last fbmsg structure     */
char         *label;          /* assert's label               */
char          level;          /* severity level (E or W)          */
char         *message;          /* reported message               */
struct chain *abl_expr;          /* assert's condition (ABL)          */
bddnode  *bdd_expr;          /* assert's condition (BDD)          */

  {

  struct fbmsg *ptmsg;

  label          = namealloc (label);

  ptmsg          = (struct fbmsg *) mbkalloc (sizeof(struct fbmsg));
  ptmsg->LABEL   = label;
  ptmsg->LEVEL   = level;
  ptmsg->MESSAGE = message;
  ptmsg->ABL     = abl_expr;
  ptmsg->NODE    = bdd_expr;
  ptmsg->NEXT    = lastfbmsg;

  return (ptmsg);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbout                              */
/* description     : create a BEOUT structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbout *fbh_addfbout (lastfbout, name, abl_expr, bdd_expr,type)

struct fbout *lastfbout;     /* pointer on the last fbout structure     */
char         *name;          /* port's name                    */
struct chain *abl_expr;          /* port's expression (ABL)          */
bddnode  *bdd_expr;          /* port's expression (BDD)          */
unsigned char type;          /* port's type                     */

  {
  struct fbout *ptout;

  name         = namealloc (name);

  ptout        = (struct fbout *) mbkalloc (sizeof(struct fbout));
  ptout->NAME  = name;
  ptout->ABL   = abl_expr;
  ptout->NODE  = bdd_expr;
  ptout->TYPE  = type;
  ptout->NEXT  = lastfbout;

  return (ptout);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbpcs                              */
/* description     : create a BEPCS structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbpcs *fbh_addfbpcs (lastfbpcs, label, sens, inst, outs, flag)

struct fbpcs *lastfbpcs;     /* pointer on the last fbpcs structure     */
char         *label;          /* process's label               */
struct chain *sens;          /* sensitivity list               */
struct ptype *inst;          /* list of instructions               */
struct chain *outs;          /* list of outputs               */
char          flag;          /* flag                         */

  {
  struct fbpcs *ptfbpcs;

  label                = namealloc (label);

  ptfbpcs              = (struct fbpcs *) mbkalloc (sizeof(struct fbpcs));
  ptfbpcs->LABEL       = label;
  ptfbpcs->SENSITIVITY = sens;
  ptfbpcs->OUTPUTS     = outs;
  ptfbpcs->INSTRUCTION = inst;
  ptfbpcs->FLAG        = flag;

  ptfbpcs->NEXT        = lastfbpcs;

  return (ptfbpcs);
  }
/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbpgm                              */
/* description     : add an BEPGM structure at the top of a list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbpgm *fbh_addfbpgm (lastfbpgm, type , name , value)
struct fbpgm  *lastfbpgm;     /* pointer on the last fbpgm          */
char          *type;          /* pragma's type                         */
char          *name;          /* pragma's name               */
void          *value;          /* pragma's content                              */
  {
  struct fbpgm *ptfbpgm;

  if(type)
       type            = namealloc (type);
  if(name)
       name            = namealloc (name);

  ptfbpgm         = (struct fbpgm *) mbkalloc (sizeof(struct fbpgm));

  ptfbpgm->TYPE   = type;
  ptfbpgm->NAME   = name;
  ptfbpgm->VALUE  = value;

  ptfbpgm->NEXT   = lastfbpgm;

  return (ptfbpgm);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbpor                              */
/* description     : create a BEPOR structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbpor *fbh_addfbpor (lastfbpor, name, dir, type)

struct fbpor  *lastfbpor;     /* pointer on the last fbpor structure     */
char          *name;          /* signal's name               */
char           dir;              /* signal's mode (I, O, B, Z, or T)     */
unsigned char  type;          /* signal's type (M or W)          */

  {
  struct fbpor *ptfbpor;

  name               = namealloc (name);

  ptfbpor            = (struct fbpor *) mbkalloc (sizeof(struct fbpor));
  ptfbpor->NAME      = name;
  ptfbpor->DIRECTION = dir;
  ptfbpor->TYPE      = type;
  ptfbpor->NEXT      = lastfbpor;

  return (ptfbpor);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbreg                              */
/* description     : create a BEREG structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbreg *fbh_addfbreg (lastfbreg, name, fbbiabl, fbbinode)

struct fbreg  *lastfbreg;     /* pointer on the last fbreg structure     */
char          *name;          /* register's name               */
struct fbbiabl  *fbbiabl;          /* register's drivers (ABL)          */
struct fbbinode *fbbinode;          /* register's drivers (BDD)          */
/*unsigned char  type;           register's type               */

  {
  struct fbreg *ptreg;

  name          = namealloc (name);

  ptreg         = (struct fbreg *) mbkalloc (sizeof(struct fbreg));
  ptreg->NAME   = name;
  ptreg->BIABL  = fbbiabl;
  ptreg->BINODE = fbbinode;
  ptreg->NEXT   = lastfbreg;

  return (ptreg);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbrin                              */
/* description     : create a BERIN structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbrin *fbh_addfbrin (lastfbrin, name)

struct fbrin *lastfbrin;     /* pointer on the last fbrin structure     */
char         *name;          /* signal's name               */

  {
  struct fbrin *ptrin;

  name         = namealloc (name);

  ptrin        = (struct fbrin *) mbkalloc (sizeof(struct fbrin));
  ptrin->NAME  = name;
  ptrin->NEXT  = lastfbrin;

  return (ptrin);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addfbtyp                              */
/* description     : add an BETYP structure at the top of a list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbtyp *fbh_addfbtyp (lastfbtyp, name , left , right , value ,
                            size     , byte , base , index, resolv, class)

struct fbtyp  *lastfbtyp;     /* pointer on the last fbtyp          */
char          *name;          /* type's name                    */
int            left;          /* left bound                    */
int            right;          /* right bound                    */
char         **value;          /* list of values               */
unsigned int   size;          /* number of values               */
unsigned int   byte;          /* number of bytes needed to code values*/
unsigned char  base;          /* the base type or element's type     */
unsigned char  index;          /* the type of the index for an array     */
unsigned char  resolv;          /* index of resolution function          */
char           class;          /* type's class (E, I, A, S)          */

  {
  struct fbtyp *ptfbtyp;

  name            = namealloc (name);

  ptfbtyp         = (struct fbtyp *) mbkalloc (sizeof(struct fbtyp));

  ptfbtyp->CLASS  = class;
  ptfbtyp->NAME   = name;
  ptfbtyp->LEFT   = left;
  ptfbtyp->RIGHT  = right;
  ptfbtyp->VALUE  = value;
  ptfbtyp->SIZE   = size;
  ptfbtyp->BYTE   = byte;
  ptfbtyp->BASE   = base;
  ptfbtyp->INDEX  = index;
  ptfbtyp->RESOLV = resolv;

  ptfbtyp->NEXT   = lastfbtyp;

  return (ptfbtyp);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addbiabl                              */
/* description     : create a BIABL structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbbiabl *fbh_addbiabl (lastbiabl, label, condition, value)

struct fbbiabl *lastbiabl;     /* pointer on the last fbbiabl structure     */
char         *label;          /* block's label               */
struct chain *condition;     /* guard expression (ABL)          */
struct chain *value;          /* value expression (ABL)          */

  {
  struct fbbiabl  *ptbiabl;

  ptbiabl         = (struct fbbiabl *) mbkalloc (sizeof(struct fbbiabl));
  label           = namealloc (label);

  ptbiabl->LABEL  = label;
  ptbiabl->CNDABL = condition;
  ptbiabl->VALABL = value;
  ptbiabl->NEXT   = lastbiabl;

  return (ptbiabl);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_addbinode                              */
/* description     : create a BINODE structure at the top the list          */
/* called func.     : namealloc, mbkalloc                         */
/* ###--------------------------------------------------------------### */

struct fbbinode *fbh_addbinode (lastbinode, condition, value)

struct fbbinode *lastbinode;     /* pointer on the last fbbinode structure     */
bddnode   *condition;     /* guard expression (BDD)          */
bddnode   *value;          /* value expression (BDD)          */

  {
  struct fbbinode *ptbinode;

  ptbinode          = (struct fbbinode *) mbkalloc (sizeof(struct fbbinode));
  ptbinode->CNDNODE = condition;
  ptbinode->VALNODE = value;
  ptbinode->NEXT    = lastbinode;

  return (ptbinode);
  }

/* ###--------------------------------------------------------------### */
/* function     : fbh_prdeftyp                              */
/* description     : create BETYPs for predefined types               */
/* called func.     : fbh_addfbtyp, namealloc                    */
/* ###--------------------------------------------------------------### */

struct fbtyp *fbh_prdeftyp ()

{
  static char         *bit_vl[2];
  static char         *boolean_vl[2];
  static char         *severity_vl[2];
  struct fbtyp        *t = 0;

  boolean_vl[0] = namealloc ("false");
  boolean_vl[1] = namealloc ("true");

  bit_vl[0] = namealloc ("'0'");
  bit_vl[1] = namealloc ("'1'");

  severity_vl[0] = namealloc ("warning");
  severity_vl[1] = namealloc ("error");

     /* ###------------------------------------------------------### */
     /*    Enumeration types                              */
     /* ###------------------------------------------------------### */

  t = fbh_addfbtyp (t,"severity_level",0,1,severity_vl,2,1,0,0,0,'E');
  t = fbh_addfbtyp (t,"boolean"       ,0,1,boolean_vl ,2,1,0,0,0,'E');
  t = fbh_addfbtyp (t,"bit"           ,0,1,bit_vl     ,2,1,0,0,0,'E');

     /* ###------------------------------------------------------### */
     /*    Integer types                              */
     /* ###------------------------------------------------------### */

  t = fbh_addfbtyp (t,"integer",0x80000000,0x7fffffff,0, 0,0,0,0,0,'I');

     /* ###------------------------------------------------------### */
     /*    Subtypes                                   */
     /* ###------------------------------------------------------### */

  t = fbh_addfbtyp (t,"mux_bit",0,1         ,0,0,1,2,0,1,'S');
  t = fbh_addfbtyp (t,"wor_bit",0,1         ,0,0,1,2,0,2,'S');
  t = fbh_addfbtyp (t,"reg_bit",0,1         ,0,0,1,2,0,3,'S');
  t = fbh_addfbtyp (t,"natural",0,0x7fffffff,0,0,1,3,0,0,'S');

     /* ###------------------------------------------------------### */
     /*    Unconstraint array types                         */
     /* ###------------------------------------------------------### */

  t = fbh_addfbtyp (t,"bit_vector",0,0x7fffffff,0,0,1,2,7,0,'U');
  t = fbh_addfbtyp (t,"mux_vector",0,0x7fffffff,0,0,1,4,7,0,'U');
  t = fbh_addfbtyp (t,"wor_vector",0,0x7fffffff,0,0,1,5,7,0,'U');
  t = fbh_addfbtyp (t,"reg_vector",0,0x7fffffff,0,0,1,6,7,0,'U');

  return (t);
}
