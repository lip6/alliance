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
/* file         : abe201.h                                              */
/* date         : Feb 15 1995                                           */
/* version      : v201                                                  */
/* contents     : defines and structure definitions used in BEH library */
/*                                                                      */
/* ###--------------------------------------------------------------### */

#ifndef ABE_BEHDEF
#define ABE_BEHDEF

        /* ###------------------------------------------------------### */
        /*    defines                                                   */
        /* ###------------------------------------------------------### */

#define BEH_GENERIC 203698      /* generic ptype code                   */
#define BEH_STABLE  0x01        /* description used STABLE attribute    */

        /* ###------------------------------------------------------### */
        /*    structure definitions                                     */
        /* ###------------------------------------------------------### */

struct beden                    /* dictionary entry point               */
  {
  struct beden *NEXT;           /* next entry                           */
  struct bedrd *DATA;           /* data                                 */
  char         *KEY;            /* key                                  */
  };

struct bedrd                    /* dictionary record                    */
  {
  struct bedrd *NEXT;           /* next record                          */
  char         *KEY;            /* context key                          */
  short         FD0_VAL;        /* data field                           */
  short         FD1_VAL;        /* data field                           */
  short         FD2_VAL;        /* data field                           */
  short         FD3_VAL;        /* data field                           */
  short         FD4_VAL;        /* data field                           */
  short         FD5_VAL;        /* data field                           */
  short         FD6_VAL;        /* data field                           */
  long          PNT_VAL;        /* data field                           */
  };

typedef struct befig            /* behaviour figure                     */
  {
  struct befig      *NEXT;      /* next figure                          */
  char              *NAME;      /* figure's name                        */
  struct bereg      *BEREG;     /* list of internal signals (REGISTER)  */
  struct bemsg      *BEMSG;     /* list of assert instructions          */
  struct berin      *BERIN;     /* list of inputs (registers and ports) */
  struct beout      *BEOUT;     /* list of output ports                 */
  struct bebus      *BEBUS;     /* list of output ports (BUS)           */
  struct beaux      *BEAUX;     /* list of internal signals             */
  struct beaux      *BEDLY;     /* internal signals used for 'DELAYED   */
  struct bebux      *BEBUX;     /* list of internal signals (BUS)       */
  struct bepor      *BEPOR;     /* list of figure's port built in the   */
                                /* reverse order of the port clause     */
  struct begen      *BEGEN;     /* list of generic data                 */
  struct bepgm      *BEPGM;     /* list of pragmas (new field !!)       */

  struct bddcircuit *CIRCUI;    /* Circuit pointer used during ABL to   */
                                /* BDD transformation                   */
  void              *USER;      /* reserved for user's applications     */
  long               FLAGS;
  char               ERRFLG;    /* error flag                           */
  char               TYPE;      /* description type (not used)          */
  }
befig_list;

typedef struct bereg            /* register                             */
  {
  struct bereg  *NEXT;          /* next register                        */
  char          *NAME;          /* register's name                      */
  struct biabl  *BIABL;         /* list of drivers (ABL)                */
  struct binode *BINODE;        /* list of drivers (BDD)                */
  void          *USER;
  long           FLAGS;
  }
bereg_list;

typedef struct bemsg            /* assert instruction                   */
  {
  struct bemsg   *NEXT;          /* next assertion                       */
  char            LEVEL;         /* severity level of the assertion      */
  char           *LABEL;         /* instruction's label                  */
  char           *MESSAGE;       /* assertion's message                  */
  ablexpr        *ABL;           /* assertion's condition (ABL)          */
  struct bddnode *NODE;          /* assertion's condition (BDD)          */
  void          *USER;
  long           FLAGS;
  }
bemsg_list;

typedef struct beout            /* output port                          */
  {
  struct beout   *NEXT;          /* next output                          */
  char           *NAME;          /* port's name                          */
  ablexpr        *ABL;           /* port's equation (ABL)                */
  struct bddnode *NODE;          /* port's equation (BDD)                */
  void           *USER;
  long            FLAGS;
  }
beout_list;

typedef struct bebus            /* output port (BUS)                    */
  {
  struct bebus  *NEXT;          /* next bus port                        */
  char          *NAME;          /* port's name                          */
  struct biabl  *BIABL;         /* list of drivers (ABL)                */
  struct binode *BINODE;        /* list of drivers (BDD)                */
  char           TYPE;          /* port's type : W or M                 */
  void          *USER;
  long           FLAGS;
  }
bebus_list;

typedef struct beaux            /* auxiliary signal                     */
  {     
  struct beaux   *NEXT;          /* next signal                          */
  char           *NAME;          /* signal's name                        */
  ablexpr        *ABL;           /* signal's equation (ABL)              */
  struct bddnode *NODE;          /* signal's equation (BDD)              */
  void           *USER;
  long            FLAGS;
  }
beaux_list;

typedef struct bebux            /* internal signal (BUS)                */
  {     
  struct bebux  *NEXT;          /* next signal                          */
  char          *NAME;          /* signal's name                        */
  struct biabl  *BIABL;         /* list of drivers (ABL)                */
  struct binode *BINODE;        /* list of drivers (BDD)                */
  char           TYPE;          /* signal's type : W or M               */
  void          *USER;
  long           FLAGS;
  }
bebux_list;

typedef struct bepor            /* port                                 */
  {
  struct bepor *NEXT;           /* next port                            */
  char         *NAME;           /* port's name                          */
  char          DIRECTION;      /* port's mode (same as LOCON)          */
  char          TYPE;           /* port's type (B, M or W)              */
  void         *USER;
  long          FLAGS;
  }
bepor_list;

typedef struct biabl            /* couple of ABL                        */
  {
  struct biabl  *NEXT;          /* next couple                          */
  char          *LABEL;         /* driver's label                       */
  ablexpr       *CNDABL;        /* driver's connection condition (ABL)  */
  ablexpr       *VALABL;        /* equation of driven value (ABL)       */
  void          *USER;          /* user field                           */
  long           FLAGS;
  }
biabl_list;

typedef struct binode           /* couple of BDD                        */
  {
  struct binode  *NEXT;          /* next figure                          */
  struct bddnode *CNDNODE;       /* driver's connection condition (BDD)  */
  struct bddnode *VALNODE;       /* equation of driven value (BDD)       */
  void           *USER;
  long            FLAGS;
  }
binode_list;

typedef struct berin            /* primary inputs                       */
  {
  struct berin *NEXT;           /* next input                           */
  char         *NAME;           /* input's name                         */
  struct chain *OUT_REF;        /* outputs  which depends on this input */
  struct chain *BUS_REF;        /* busses   which depends on this input */
  struct chain *AUX_REF;        /* aux sig. which depends on this input */
  struct chain *REG_REF;        /* registers which depends on this input*/
  struct chain *BUX_REF;        /* busses   which depends on this input */
  struct chain *MSG_REF;        /* messages which depends on this input */
  struct chain *DLY_REF;        /* delayed internal signals depending ..*/
  void         *USER;
  long          FLAGS;
  }
berin_list;

typedef struct begen            /* generic                              */
  {
  struct begen *NEXT;           /* next generic                         */
  char         *NAME;           /* generic`s name                       */
  char         *TYPE;           /* generic type`s name                  */
  void         *VALUE;          /* generic`s value                      */
  void         *USER;
  long          FLAGS;
  }
begen_list;

typedef struct begex
  {
  struct chain   *OPERAND;      /* list of operands or NULL for term    */
  unsigned int    TERM   ;      /* operator or terminal's index         */
  unsigned short  TYPE   ;      /* expression's type index              */
  }
begex_list;

typedef struct bepgm            /* pragma                               */
  {
  struct bepgm *NEXT;           /* next pragma                          */
  char         *TYPE;           /* prgma's type                         */
  char         *NAME;           /* object's name                        */
  void         *VALUE;          /* prgma's content                      */
  void         *USER;
  }
bepgm_list;

        /* ###------------------------------------------------------### */
        /*    functions                                                 */
        /* ###------------------------------------------------------### */

extern struct beaux  *beh_addbeaux ();
extern struct bebux  *beh_addbebux ();
extern struct bebus  *beh_addbebus ();
extern struct befig  *beh_addbefig ();
extern struct begen  *beh_addbegen ();
extern struct bemsg  *beh_addbemsg ();
extern struct beout  *beh_addbeout ();
extern struct bepor  *beh_addbepor ();
extern struct bereg  *beh_addbereg ();
extern struct berin  *beh_addberin ();
extern struct bepgm  *beh_addbepgm ();
extern struct biabl  *beh_addbiabl ();
extern struct binode *beh_addbinode ();

extern struct beaux  *beh_delbeaux ();
extern struct bebux  *beh_delbebux ();
extern struct bebus  *beh_delbebus ();
extern struct befig  *beh_delbefig ();
extern struct begen  *beh_delbegen ();
extern struct bemsg  *beh_delbemsg ();
extern struct beout  *beh_delbeout ();
extern struct bepor  *beh_delbepor ();
extern struct bereg  *beh_delbereg ();
extern struct berin  *beh_delberin ();
extern struct bepgm  *beh_delbepgm ();
extern struct biabl  *beh_delbiabl ();
extern struct binode *beh_delbinode ();

extern void           beh_frebeaux ();
extern void           beh_frebebux ();
extern void           beh_frebebus ();
extern void           beh_frebefig ();
extern void           beh_frebegen ();
extern void           beh_frebemsg ();
extern void           beh_frebeout ();
extern void           beh_frebepor ();
extern void           beh_frebereg ();
extern void           beh_freberin ();
extern void           beh_frebepgm ();
extern void           beh_frebiabl ();
extern void           beh_frebinode ();

extern void beh_viewbefig();
extern void beh_viewbiabl();
extern void beh_viewbereg();
extern void beh_viewbebux();
extern void beh_viewbebus();
extern void beh_viewbeout();
extern void beh_viewbeaux();
extern void beh_viewbepor();

extern struct beaux  *beh_rmvbeaux ();
extern struct bebux  *beh_rmvbebux ();
extern struct bebus  *beh_rmvbebus ();
extern struct befig  *beh_rmvbefig ();
extern struct begen  *beh_rmvbegen ();
extern struct bemsg  *beh_rmvbemsg ();
extern struct beout  *beh_rmvbeout ();
extern struct bepor  *beh_rmvbepor ();
extern struct bereg  *beh_rmvbereg ();
extern struct berin  *beh_rmvberin ();
extern struct bepgm  *beh_rmvbepgm ();

extern struct beden **beh_initab   ();
extern void           beh_addtab   ();
extern long           beh_chktab   ();
extern void           beh_fretab   ();

extern int            beh_error    ();
extern void           beh_message  ();
extern void           beh_toolbug  ();

#endif
