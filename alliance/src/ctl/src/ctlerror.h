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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Ctl                           |
|                                                             |
| File    :                 Ctl Errors                        |
|                                                             |
| Ctlhors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   04.07.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef CTL_ERROR_H
# define CTL_ERROR_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define CTL_PLACE_EXIST_ERROR             0
# define CTL_TRANS_EXIST_ERROR             1
# define CTL_PORT_EXIST_ERROR              2
# define CTL_ARC_PLACE_EXIST_ERROR         3
# define CTL_ARC_TRANS_EXIST_ERROR         4
# define CTL_SIGNAL_EXIST_ERROR            6
# define CTL_VARIABLE_EXIST_ERROR          7
# define CTL_DEFINE_EXIST_ERROR            8
# define CTL_CONSTANT_EXIST_ERROR          9
# define CTL_TRANS_FATHER_UNKNOWN         10
# define CTL_PROC_EXIST_ERROR             11
# define CTL_DEL_ARC_PLACE_ERROR          12
# define CTL_DEL_ARC_TRANS_ERROR          13
# define CTL_FUNC_EXIST_ERROR             14
# define CTL_UNKNOWN_DECLAR_TYPE          15
# define CTL_ARGUMENT_EXIST_ERROR         16
# define CTL_UNDECLARED_SYMBOL            17
# define CTL_DECLAR_EXIST_ERROR           18
# define CTL_BAD_DECLAR_TYPE              19
# define CTL_BAD_ASSIGN_TYPE              20
# define CTL_ILLEGAL_INDEX_ERROR          21
# define CTL_ILLEGAL_POSITION_ERROR       22
# define CTL_INS_EXIST_ERROR              23
# define CTL_MOD_EXIST_ERROR              24
# define CTL_GENERIC_EXIST_ERROR          25

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define ctlerror( E, V1, V2 ) \
   (ctl_error((int)(E),(char*)(long)(V1),(char*)(long)(V2),__FILE__,__LINE__))

# define ctlwarning( E, V1, V2 ) \
   (ctl_warning( (int)(E),(char*)(long)(V1),(char*)(long)(V2),__FILE__,__LINE__))

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void  ctl_error __P((int Error, char *Text1, char *Text2, char *File, long Line));
  extern void  ctl_warning __P((int Warning, char *Text1, char *Text2, char *File, long Line));

# endif
