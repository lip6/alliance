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
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for ABL         |
|                                                       |
|  Date    :            12.06.95                        |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef ABL_102_H
# define ABL_102_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

# ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
# endif

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                   Logical Operators                   |
|                                                       |
\------------------------------------------------------*/

# define ABL_OR             0
# define ABL_AND            1
# define ABL_XOR            2
# define ABL_NOT            3
# define ABL_NOR            4
# define ABL_NAND           5
# define ABL_NXOR           6
# define ABL_STABLE         7

/*------------------------------------------------------\
|                                                       |
|                       CTL Operators                   |
|                                                       |
\------------------------------------------------------*/

# define ABL_AF             8
# define ABL_AG             9
# define ABL_AX            10
# define ABL_AU            11
# define ABL_EF            12
# define ABL_EG            13
# define ABL_EX            14
# define ABL_EU            15

# define ABL_MAX_OPERATOR  16

/*------------------------------------------------------\
|                                                       |
|                       Polarity                        |
|                                                       |
\------------------------------------------------------*/

# define ABL_POLAR_NEGATIVE 0
# define ABL_POLAR_POSITIVE 1

/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                         View                          |
|                                                       |
\------------------------------------------------------*/

# define ABL_VIEW_INFIX     0
# define ABL_VIEW_PREFIX    1
# define ABL_VIEW_POSTFIX   2
# define ABL_VIEW_VHDL      3

/*------------------------------------------------------\
|                                                       |
|                       Support Type                    |
|                                                       |
\------------------------------------------------------*/

# define ABL_SUPPORT_CHAIN     0
# define ABL_SUPPORT_PTYPE     1

/*------------------------------------------------------\
|                                                       |
|                 Lisp Like Macros                      |
|                                                       |
\------------------------------------------------------*/

# define ABL_CDR( E )        ( ( E )->NEXT )
# define ABL_CAR( E )        ( (ablexpr *)( E )->DATA )
# define ABL_CADR( E )       ( ABL_CAR( ABL_CDR( ( E ) ) ) )
# define ABL_CDDR( E )       ( ABL_CDR( ABL_CDR( ( E ) ) ) )
# define ABL_ATOM( E )       ( ( E )->NEXT == (ablexpr *)0 )
# define ABL_ATOM_VALUE( E ) ( (char *)( ( E )->DATA ) )
# define ABL_OPER( E )       ( (long)( ( ABL_CAR( ( E ) ) )->DATA ) )
# define SET_ABL_OPER( E,O ) ( ( ABL_CAR( (E) )->DATA = (void *)(O) ) )

/*------------------------------------------------------\
|                                                       |
|                     Get Macros                        |
|                                                       |
\------------------------------------------------------*/

# define getablatomone()       ( ABL_ATOM_NAME_ONE      )
# define getablatomzero()      ( ABL_ATOM_NAME_ZERO     )
# define getablatomdc()        ( ABL_ATOM_NAME_DC       )
# define getablatomtristate()  ( ABL_ATOM_NAME_TRISTATE )

/*------------------------------------------------------\
|                                                       |
|                         Type                          |
|                                                       |
\------------------------------------------------------*/

  typedef  chain_list    ablexpr;

/*------------------------------------------------------\
|                                                       |
|                     Global Variables                  |
|                                                       |
\------------------------------------------------------*/

 extern char *ABL_OPERATOR_NAME[ ABL_MAX_OPERATOR ];
 extern char *ABL_OPERATOR_UPPER_NAME[ ABL_MAX_OPERATOR ];
 extern char *ABL_ATOM_NAME_ZERO;
 extern char *ABL_ATOM_NAME_ONE;
 extern char *ABL_ATOM_NAME_DC;
 extern char *ABL_ATOM_NAME_TRISTATE;

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                Initialize Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  ablenv __P(());

/*------------------------------------------------------\
|                                                       |
|                      Free Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  freeablexpr __P((ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                    Create Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * createablatom __P((char *Atom));
  extern       ablexpr * createablatomone __P(());
  extern       ablexpr * createablatomzero __P(());
  extern       ablexpr * createablatomdc __P(());
  extern       ablexpr * createablatomtristate __P(());

  extern       ablexpr * createabloper __P((long Oper));
  extern       ablexpr * createablbinexpr __P((long Oper, ablexpr *Expr1, ablexpr *Expr2));
  extern       ablexpr * createablnotexpr __P((ablexpr *Expr));
  extern       ablexpr * createablxorbinexpr __P((long Oper, ablexpr *Expr1, ablexpr *Expr2));
  extern       ablexpr * createablunaryexpr __P((long Oper, ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                    Optim Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * optimablbinexpr __P((long Oper, ablexpr *Expr1, ablexpr *Expr2));
  extern       ablexpr * optimablnotexpr __P((ablexpr *Expr));
  extern       ablexpr * optimablxorbinexpr __P((long Oper, ablexpr *Expr1, ablexpr *Expr2));
  extern       ablexpr * optimablunaryexpr __P((long Oper, ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                      Add Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern           void  addablqexpr __P((ablexpr *Expr1, ablexpr *Expr2));
  extern           void  addablhexpr __P((ablexpr *Expr1, ablexpr *Expr2));

/*------------------------------------------------------\
|                                                       |
|                    Duplicate Functions                |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * dupablexpr __P((ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                      Del Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern           void  delablexpr __P((ablexpr *Expr));
  extern           void  delablexprnum __P((ablexpr *Expr, int Number));

/*------------------------------------------------------\
|                                                       |
|                      Get Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern            int  getabloperpolar __P((long Oper));
  extern            int  getablopernot __P((long Oper));
  extern          char * getablopername __P((long Oper));
  extern          char * getabloperuppername __P((long Oper));
  extern           long  getabloperbyname __P((char *Name));

  extern           long  getablexprdepth __P((ablexpr *Expr));
  extern           long  getablexprlength __P((ablexpr *Expr));
  extern           long  getablexprnumatom __P((ablexpr *Expr));
  extern           long  getablexprnumbinoper __P((ablexpr *Expr));
  extern           long  getablexprnumocc __P((ablexpr *Expr, char *Name));

  extern       ablexpr * getablexprnum __P((ablexpr *Expr, int Number));
  extern       ablexpr * getablexprmin __P((int (*Function)(), ablexpr *Expr));
  extern       ablexpr * getablexprmax __P((int (*Function)(), ablexpr *Expr));

  extern          void * getablexprsupport __P((ablexpr *Expr, int Mode));
  extern          void * unionablexprsupport __P((void *Support, ablexpr *Expr, int Mode));

/*------------------------------------------------------\
|                                                       |
|                      Is Functions                     |
|                                                       |
\------------------------------------------------------*/

  extern            int  isablunaryoper __P((long Oper));
  extern            int  isablbinaryoper __P((long Oper));
  extern            int  isabloperinexpr __P((ablexpr *Expr, long Oper));
  extern            int  isablnameinexpr __P((ablexpr *Expr, char *Name));
  extern            int  isablequalexpr __P((ablexpr *Expr1, ablexpr *Expr2));
  extern            int  isablsimilarexpr __P((ablexpr *Expr1, ablexpr *Expr2));
  extern            int  isablnotexpr __P((ablexpr *Expr1, ablexpr *Expr2));

/*------------------------------------------------------\
|                                                       |
|                     Vhdl Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern          char * vhdlablname __P((char *Name));
  extern          char * vhdlablvector __P((char *Name, long *Index));

/*------------------------------------------------------\
|                                                       |
|                      Map Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern      void  mapablexpr __P((void (*Function)(), ablexpr *Expr));
  extern  ablexpr * mapabloperexpr __P((ablexpr *(*Function)(), long Oper, ablexpr *Expr));
  extern       int  mapablanyexpr __P((int (*Function)(), ablexpr *Expr));
  extern       int  mapableveryexpr __P((int (*Function)(), ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                   Substitute Functions                |
|                                                       |
\------------------------------------------------------*/

  extern  ablexpr * substablexpr __P((ablexpr *Expr1, char *Name, ablexpr *Expr2));
  extern  ablexpr * substdupablexpr __P((ablexpr *Expr1, char *Name, ablexpr *Expr2));

/*------------------------------------------------------\
|                                                       |
|                   Polar Functions                     |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * polarablexpr __P((ablexpr *Expr, int Polar));
  extern       ablexpr * polardupablexpr __P((ablexpr *Expr, int Polar));

/*------------------------------------------------------\
|                                                       |
|                    Develop Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * devdupablxorexpr __P((ablexpr *Expr));
  extern       ablexpr * devablxorexpr __P((ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                    Simplify Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * simpablexpr __P((ablexpr *Expr));
  extern       ablexpr * simpdupablexpr __P((ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                    Simplify CTL Functions             |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * simpablctlexpr __P((ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|               Normalisation CTL Functions             |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * normablctlexpr __P((ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                    Flatten Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern           void  flatablexpr __P((ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                    UnFlatten Functions                |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * unflatablexpr __P((ablexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                    String Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern       ablexpr * strablexpr __P((char *String, int Mode));

/*------------------------------------------------------\
|                                                       |
|                      View Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  viewablexpr __P((ablexpr *Expr, int Mode));
  extern           void  viewablexprln __P((ablexpr *Expr, int Mode));
  extern           void  viewablexprfile __P((FILE *AblFile, ablexpr *Expr, int Mode));
  extern           void  viewablexprfileln __P((FILE *AblFile, ablexpr *Expr, int Mode));
  extern          char * viewablexprstr __P((ablexpr *Expr, int Mode));
  extern          char * viewablexprstrln __P((ablexpr *Expr, int Mode));

#ifdef __cplusplus
}
#endif /* __cplusplus */

# endif
