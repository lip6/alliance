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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     C                           |
|                                                             |
| File    :                  c_treecompat.h                 |
|                                                             |
| Date    :                   09.07.99                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/

# ifndef C_TREE_COMPAT_H
# define C_TREE_COMPAT_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define C_NULL_TREE (c_tree_node *)0

# define TREE_TYPE(N)  CTreeType(N)
# define TREE_CHAIN(N) CTreeChain(N)

# define TREE_INT_CST_LOW(N)      CTreeIntCstLow(N)
# define TREE_INT_CST_HIGH(N)     CTreeIntCstHigh(N)

# define TYPE_MIN_VALUE(N) CTreeTypeMinValue( N )
# define TYPE_MAX_VALUE(N) CTreeTypeMaxValue( N )

typedef union c_tree_node *tree; 

# define NULL_TREE     (tree)0

#define INT_CST_LT(A, B)  \
(CTreeIntCstHigh(A) < CTreeIntCstHigh(B)       \
 || ( CTreeIntCstHigh(A) == CTreeIntCstHigh(B) \
     && ((unsigned int) CTreeIntCstLow(A)      \
         < (unsigned int) CTreeIntCstLow(B))))


#define INT_CST_LT_UNSIGNED(A, B)  \
(((unsigned int) CTreeIntCstHigh(A)        \
  < (unsigned int) CTreeIntCstHigh(B))        \
 || (((unsigned int) CTreeIntCstHigh(A)        \
      == (unsigned int ) CTreeIntCstHigh(B)) \
     && (((unsigned int) CTreeIntCstLow(A)        \
          < (unsigned int) CTreeIntCstLow(B)))))

# define INTEGRAL_TYPE_P(TYPE)  \
  ( CTreeTypeCode(TYPE) == C_INTEGER_TYPE ||  \
    CTreeTypeCode(TYPE) == C_ENUMERAL_TYPE  ||  \
    CTreeTypeCode(TYPE) == C_BOOLEAN_TYPE || \
    CTreeTypeCode(TYPE) == C_CHAR_TYPE)

#define POINTER_TYPE_P(TYPE) \
  (CTreeTypeCode(TYPE) == C_POINTER_TYPE ||  \
   CTreeTypeCode(TYPE) == C_REFERENCE_TYPE)

 
# define REAL_VALUE_TYPE double

#define REAL_VALUE_NEGATE(N)  (double)(-N)
#define REAL_VALUE_NEGATIVE(N)  ((N) < 0.0)
#define REAL_VALUES_EQUAL(A,B) ((A) == (B))
#define REAL_VALUES_LESS(A,B) ((A) < (B))

#define FLOAT_TYPE_P(TYPE)		\
  ( CTreeTypeCode(TYPE) == C_REAL_TYPE	\
   || ( CTreeTypeCode(TYPE) == C_COMPLEX_TYPE \
       &&  CTreeTypeCode( CTreeType(TYPE)) == C_REAL_TYPE))
# define REAL_VALUES_IDENTICAL(A, B) (A == B)

# endif
