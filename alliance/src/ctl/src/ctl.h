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
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for CTL         |
|                                                       |
|  Date    :            04.18.2002                      |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef CTL_H
# define CTL_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                      Ctl Direction                    |
|                                                       |
\------------------------------------------------------*/

# define CTL_DIR_IN         0
# define CTL_DIR_OUT        1
# define CTL_DIR_INOUT      2
# define CTL_MAX_DIR_TYPE   3

/*------------------------------------------------------\
|                                                       |
|                      Ctl Kind                         |
|                                                       |
\------------------------------------------------------*/

# define CTL_KIND_NONE       0
# define CTL_KIND_BUS        1
# define CTL_KIND_REGISTER   2
# define CTL_MAX_KIND_TYPE   3

/*------------------------------------------------------\
|                                                       |
|                       Ctl Declar Type                 |
|                                                       |
\------------------------------------------------------*/

# define CTL_DECLAR_ALL             0
# define CTL_DECLAR_PORT            1
# define CTL_DECLAR_SIGNAL          2
# define CTL_DECLAR_VARIABLE        3
# define CTL_DECLAR_DEFINE          4
# define CTL_DECLAR_CONSTANT        5
# define CTL_MAX_DECLAR_TYPE        6

/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                      Ctl Get Symbol                   |
|                                                       |
\------------------------------------------------------*/

# define GetCtlSymbol( C ) ((ctlsym       *)( (C)->DATA ))
# define GetCtlDeclar( C ) ((ctldecl_list *)( (C)->DATA ))

/*------------------------------------------------------\
|                                                       |
|                        Ctl Get Arc                    |
|                                                       |
\------------------------------------------------------*/

# define GetCtlArc( C ) \
   ((ctlarc *)( (C)->DATA ))

# define GetCtlArcTargetPlace( A ) \
   ((ctlplace_list *)( (ctlarc *)(A)->TARGET ))

# define GetCtlArcSourcePlace( A ) \
   ((ctlplace_list *)( (ctlarc *)(A)->SOURCE ))

# define GetCtlArcTargetTrans( A ) \
   ((ctltrans_list *)( (ctlarc *)(A)->TARGET ))

# define GetCtlArcSourceTrans( A ) \
   ((ctltrans_list *)( (ctlarc *)(A)->SOURCE ))

/*------------------------------------------------------\
|                                                       |
|                        Ctl Add Arc                    |
|                                                       |
\------------------------------------------------------*/

# define addctlarctrans( F, T, P ) \
  (addctlarc( (F), (T), (P), CTL_ARC_TRANS_PLACE ))

# define addctlarcplace( F, P, T ) \
  (addctlarc( (F), (P), (T), CTL_ARC_PLACE_TRANS ))

/*------------------------------------------------------\
|                                                       |
|                        Ctl Add Action                 |
|                                                       |
\------------------------------------------------------*/

# define addctlactcall( F, T, E )  \
  (addctlact( (F), (T), (vexexpr *)0, (E), CTL_ACT_CALL ))

# define addctlactreturn( F, T, E ) \
  (addctlact( (F), (T), (vexexpr *)0, (E), CTL_ACT_RETURN ))

# define addctlactcallafter( F, T, P, E )  \
  (addctlactafter( (F), (T), (P), (vexexpr *)0, (E), CTL_ACT_CALL ))

# define addctlactreturnafter( F, T, P, E ) \
  (addctlactafter( (F), (T), (P), (vexexpr *)0, (E), CTL_ACT_RETURN ))

# define addctlactcallbefore( F, T, P, E )  \
  (addctlactbefore( (F), (T), (P), (vexexpr *)0, (E), CTL_ACT_CALL ))

# define addctlactreturnbefore( F, T, P, E ) \
  (addctlactafter( (F), (T), (P), (vexexpr *)0, (E), CTL_ACT_RETURN ))

# define addctlactcallfirst( F, T, E )  \
  (addctlactfirst( (F), (T), (vexexpr *)0, (E), CTL_ACT_CALL ))

# define addctlactreturnfirst( F, T, E ) \
  (addctlactfirst( (F), (T), (vexexpr *)0, (E), CTL_ACT_RETURN ))


/*------------------------------------------------------\
|                                                       |
|                        Ctl Get Number                 |
|                                                       |
\------------------------------------------------------*/

# define GetCtlNumProc( F )     ((F)->HASH_PROCESS->NUMBER_ELEM)
# define GetCtlNumFunc( F )     ((F)->HASH_FUNCTION->NUMBER_ELEM)
# define GetCtlNumTrans( F )    ((F)->HASH_TRANS->NUMBER_ELEM)
# define GetCtlNumPlace( F )    ((F)->HASH_PLACE->NUMBER_ELEM)
# define GetCtlNumArc( F )      ((F)->HASH_ARC->NUMBER_ELEM)

# define GetCtlNumDecl( F )     ((F)->HASH_DECLAR[ CTL_DECLAR_ALL      ]->NUMBER_ELEM)
# define GetCtlNumDeclPort( F ) ((F)->HASH_DECLAR[ CTL_DECLAR_PORT     ]->NUMBER_ELEM)
# define GetCtlNumDeclSig( F )  ((F)->HASH_DECLAR[ CTL_DECLAR_SIGNAL   ]->NUMBER_ELEM)
# define GetCtlNumDeclVar( F )  ((F)->HASH_DECLAR[ CTL_DECLAR_VARIABLE ]->NUMBER_ELEM)
# define GetCtlNumDeclCst( F )  ((F)->HASH_DECLAR[ CTL_DECLAR_CONSTANT ]->NUMBER_ELEM)
# define GetCtlNumDeclDef( F )  ((F)->HASH_DECLAR[ CTL_DECLAR_DEFINE   ]->NUMBER_ELEM)
# define GetCtlNumDeclArg( F )  ((F)->HASH_DECLAR[ CTL_DECLAR_ARGUMENT ]->NUMBER_ELEM)

/*------------------------------------------------------\
|                                                       |
|                    Ctl Search Symbol                  |
|                                                       |
\------------------------------------------------------*/

# define searchctlsymport( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_PORT ))
# define searchctlsymsig( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_SIGNAL ))
# define searchctlsymvar( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_VARIABLE ))
# define searchctlsymcst( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_CONSTANT ))
# define searchctlsymdef( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_DEFINE ))
# define searchctlsymarg( F, N, I ) \
    (searchctlsym( (F), (N) (I), CTL_DECLAR_ARGUMENT ))
# define searchctlsymgen( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_GENERIC ))
# define searchctlsymall( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_ALL ))

/*------------------------------------------------------\
|                                                       |
|                    Ctl Search Declar                  |
|                                                       |
\------------------------------------------------------*/

# define searchctldeclport( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_PORT ))
# define searchctldeclsig( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_SIGNAL ))
# define searchctldeclvar( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_VARIABLE ))
# define searchctldeclcst( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_CONSTANT ))
# define searchctldecldef( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_DEFINE ))
# define searchctldeclarg( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_ARGUMENT ))
# define searchctldeclgen( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_GENERIC ))
# define searchctldeclall( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_ALL ))


/*------------------------------------------------------\
|                                                       |
|                         Type                          |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                        Ctl Line                       |
|                                                       |
\------------------------------------------------------*/

  typedef struct ctlline_list
  {
    struct ctlline_list *NEXT;
    char                *NAME;
    long                 LINE;

  } ctlline_list;

/*------------------------------------------------------\
|                                                       |
|                        Ctl Symbol                     |
|                                                       |
\------------------------------------------------------*/

  typedef struct ctlsym
  {
    char                  *NAME;
    struct ctldecl_list   *DECL;
    short                  INDEX;
    long                   FLAGS;
    void                  *USER;

  } ctlsym;

/*------------------------------------------------------\
|                                                       |
|                     Ctl Declaration                   |
|                                                       |
\------------------------------------------------------*/

  typedef struct ctldecl_list
  {
    struct ctldecl_list  *NEXT;
    struct ctldecl_list **PREV;
    vexexpr              *VEX_ATOM;
    vexexpr              *VEX_INIT;
    ctlsym               *DECL_SYM;
    unsigned char         DIR;
    unsigned char         TYPE;
    unsigned char         KIND;
    unsigned char         BASE;
    ctlline_list         *LINE;
    long                  FLAGS;
    void                 *USER;

  } ctldecl_list;

/*------------------------------------------------------\
|                                                       |
|                    Ctl Formulae                       |
|                                                       |
\------------------------------------------------------*/

  typedef struct ctlform_list
  {
    struct ctlform_list   *NEXT;
    struct ctlform_list  **PREV;
    char                  *NAME;
    vexexpr               *VEX_EXPR;
    ctlline_list          *LINE;
    long                   FLAGS;
    void                  *USER;

  } ctlform_list;

/*------------------------------------------------------\
|                                                       |
|                        Ctl Figure                     |
|                                                       |
\------------------------------------------------------*/

  typedef struct ctlfig_list
  {
    struct ctlfig_list *NEXT;
    char               *NAME;
    ctlform_list       *FORM;
    ctldecl_list       *DECLAR[ CTL_MAX_DECLAR_TYPE ];
    authtable          *HASH_DECLAR[ CTL_MAX_DECLAR_TYPE ];
    void               *CIRCUIT;
    long                FLAGS;
    void               *USER;

  } ctlfig_list;

/*------------------------------------------------------\
|                                                       |
|                     Global Variables                  |
|                                                       |
\------------------------------------------------------*/

  extern ctlfig_list *HEAD_CTLFIG;

  extern char  *CTL_DECLAR_TYPE   [ CTL_MAX_DECLAR_TYPE ];
  extern char  *CTL_DIR_TYPE      [ CTL_MAX_DIR_TYPE    ];
  extern char  *CTL_KIND_TYPE     [ CTL_MAX_KIND_TYPE   ];

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                       Env Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern void ctlenv __P(());

/*------------------------------------------------------\
|                                                       |
|                    Alloc Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern   ctlfig_list *allocctlfig __P(());
  extern  ctlform_list *allocctlform __P(());
  extern  ctldecl_list *allocctldecl __P(());
  extern        ctlsym *allocctlsym __P((unsigned int Width));
  extern  ctlline_list *allocctlline __P(());

/*------------------------------------------------------\
|                                                       |
|                      Free Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern void  freectlfig __P((ctlfig_list *Figure));
  extern void  freectlform __P((ctlform_list *Form));
  extern void  freectldecl __P((ctldecl_list *Decl));
  extern void  freectlsym __P((ctlsym *Sym));
  extern void  freectlline __P((ctlline_list *Line));

/*------------------------------------------------------\
|                                                       |
|                      Add Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern  ctlfig_list *addctlfig __P((char *Name));
  extern ctlform_list *addctlform __P((ctlfig_list *Figure, char *Name, vexexpr *Expr));

  extern ctldecl_list *addctldecl __P((ctlfig_list *Figure, vexexpr *Atom, unsigned char Type, unsigned char Base));
  extern ctldecl_list *addctldeclport __P((ctlfig_list *Figure, vexexpr *Atom, unsigned char Base, unsigned char Dir, unsigned char Kind));
  extern ctldecl_list *addctldeclsig __P((ctlfig_list *Figure, vexexpr *Atom, unsigned char Base, unsigned char Kind));
  extern ctldecl_list *addctldeclcst __P((ctlfig_list *Figure, vexexpr *Atom, unsigned char Base));

  extern  ctlline_list *addctlline __P((ctlfig_list *Figure, ctlline_list **HeadLine, long Line));
  extern  ctlline_list *addctlfileline __P((ctlfig_list *Figure, ctlline_list **HeadLine, char *File, long Line));

/*------------------------------------------------------\
|                                                       |
|                       Get Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern        ctlsym *getctlsymdecl __P((ctldecl_list *Declar, short Index));
  extern        ctlsym *getctlsymdeclpos __P((ctldecl_list *Declar, int Position));

/*------------------------------------------------------\
|                                                       |
|                    Search Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern   ctlfig_list *searchctlfig __P((char *Name));
  extern  ctlform_list *searchctlform __P((ctlfig_list *Figure, char *Name));
  extern        ctlsym *searchctlsym __P((ctlfig_list *Figure, char *Name, short Index, unsigned char Type));
  extern  ctldecl_list *searchctldecl __P((ctlfig_list *Figure, char *Name, unsigned char Type));

/*------------------------------------------------------\
|                                                       |
|                    Delete Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern int  delctlfig __P((char *Name));
  extern int  delctlline __P((ctlfig_list *Figure, ctlline_list **PrevLine, ctlline_list *Line));
  extern int  delctlform __P((ctlfig_list *Figure, ctlform_list *Form));

/*------------------------------------------------------\
|                                                       |
|                     View Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern void  viewctlfig __P((ctlfig_list *Figure));
  extern void  viewctlsym __P((ctlsym *Symbol));
  extern void  viewctldecl __P((ctldecl_list *Decl));
  extern void  viewctlform __P((ctlform_list *Form));
  extern void  viewctlline __P((ctlline_list *Line));

# endif
