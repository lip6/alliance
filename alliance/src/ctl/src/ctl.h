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
|                       Ctl Declar Type                 |
|                                                       |
\------------------------------------------------------*/

# define CTL_DECLAR_ALL             0
# define CTL_DECLAR_VARIABLE        1
# define CTL_DECLAR_DEFINE          2
# define CTL_DECLAR_ASSUME          3
# define CTL_DECLAR_INITIAL         4
# define CTL_DECLAR_RESET           5
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
|                        Ctl Get Number                 |
|                                                       |
\------------------------------------------------------*/

# define GetCtlNumDecl( F )      ((F)->HASH_DECLAR[ CTL_DECLAR_ALL      ]->NUMBER_ELEM)
# define GetCtlNumDeclVar( F )   ((F)->HASH_DECLAR[ CTL_DECLAR_VARIABLE ]->NUMBER_ELEM)
# define GetCtlNumDeclDef( F )   ((F)->HASH_DECLAR[ CTL_DECLAR_DEFINE   ]->NUMBER_ELEM)
# define GetCtlNumDeclAss( F )   ((F)->HASH_DECLAR[ CTL_DECLAR_ASSUME   ]->NUMBER_ELEM)
# define GetCtlNumDeclInit( F )  ((F)->HASH_DECLAR[ CTL_DECLAR_INITIAL  ]->NUMBER_ELEM)
# define GetCtlNumDeclReset( F ) ((F)->HASH_DECLAR[ CTL_DECLAR_RESET    ]->NUMBER_ELEM)

/*------------------------------------------------------\
|                                                       |
|                    Ctl Search Symbol                  |
|                                                       |
\------------------------------------------------------*/

# define searchctlsymvar( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_VARIABLE ))
# define searchctlsymdef( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_DEFINE ))
# define searchctlsymass( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_ASSUME   ))
# define searchctlsyminit( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_INITIAL  ))
# define searchctlsymreset( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_RESET ))
# define searchctlsymall( F, N, I ) \
    (searchctlsym( (F), (N), (I), CTL_DECLAR_ALL ))

/*------------------------------------------------------\
|                                                       |
|                    Ctl Search Declar                  |
|                                                       |
\------------------------------------------------------*/

# define searchctldeclvar( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_VARIABLE ))
# define searchctldecldef( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_DEFINE ))
# define searchctldeclass( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_ASSUME   ))
# define searchctldeclinit( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_INITIAL  ))
# define searchctldeclreset( F, N ) \
    (searchctldecl( (F), (N), CTL_DECLAR_RESET  ))
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
|                       Ctl Type                        |
|                                                       |
\------------------------------------------------------*/

  typedef struct ctltype_list
  {
    struct ctltype_list *NEXT;
    char                *NAME;
    long                 LEFT;
    long                 RIGHT;
    unsigned long        SIZE;
    char               **VALUE;
    char                 CLASS;
    unsigned long        INDEX;
    struct ctltype_list *BASE;

    ctlline_list        *LINE;
    long                 FLAGS;
    void                *USER;

  } ctltype_list;

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
    ctltype_list         *TYPE;
    unsigned char         DECL_TYPE;
    ctlline_list         *LINE;
    long                  FLAGS;
    void                 *USER;

  } ctldecl_list;

/*------------------------------------------------------\
|                                                       |
|                    Ctl Formula                        |
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
    ctltype_list       *TYPE;
    authtable          *HASH_TYPE;
    ctlform_list       *FORM;
    ctlform_list      **LAST_FORM;
    authtable          *HASH_FORM;
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
  extern  ctltype_list *allocctltype __P(());
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
  extern ctltype_list *addctltype __P((ctlfig_list *Figure, char *Name, unsigned long Index, long Left, long Right, unsigned long Size, char **Value, char Class, ctltype_list *Base));

  extern ctldecl_list *addctldecl __P((ctlfig_list *Figure, vexexpr *Atom, unsigned char Type ));
  extern ctldecl_list *addctldeclvar __P((ctlfig_list *Figure, vexexpr *Atom));
  extern ctldecl_list *addctldecldef __P((ctlfig_list *Figure, vexexpr *Atom));
  extern ctldecl_list *addctldeclass __P((ctlfig_list *Figure, vexexpr *Atom));
  extern ctldecl_list *addctldeclinit __P((ctlfig_list *Figure, vexexpr *Atom));
  extern ctldecl_list *addctldeclreset __P((ctlfig_list *Figure, vexexpr *Atom));

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
  extern  ctltype_list *searchctltype __P((ctlfig_list *Figure, char *Name));
  extern        ctlsym *searchctlsym __P((ctlfig_list *Figure, char *Name, short Index, unsigned char Type));
  extern  ctldecl_list *searchctldecl __P((ctlfig_list *Figure, char *Name, unsigned char Type));

/*------------------------------------------------------\
|                                                       |
|                    Delete Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern int  delctlfig __P((char *Name));
  extern int  delctlline __P((ctlfig_list *Figure, ctlline_list **PrevLine, ctlline_list *Line));

/*------------------------------------------------------\
|                                                       |
|                     View Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern void  viewctlfig __P((ctlfig_list *Figure));
  extern void  viewctlsym __P((ctlsym *Symbol));
  extern void  viewctldecl __P((ctldecl_list *Decl));
  extern void  viewctlform __P((ctlform_list *Form));
  extern void  viewctltype __P((ctltype_list *Type));
  extern void  viewctlline __P((ctlline_list *Line));

# endif
