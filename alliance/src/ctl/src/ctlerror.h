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

# define CTL_UNKNOWN_DECLAR_TYPE           1
# define CTL_UNDECLARED_SYMBOL             2
# define CTL_DECLAR_EXIST_ERROR            3
# define CTL_BAD_DECLAR_TYPE               4
# define CTL_ILLEGAL_INDEX_ERROR           5
# define CTL_ILLEGAL_POSITION_ERROR        6

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
   (ctl_error((int)(E),(char*)(long)(V1),(char*)(long)(V2),autbasename(__FILE__,NULL),__LINE__))

# define ctlwarning( E, V1, V2 ) \
   (ctl_warning( (int)(E),(char*)(long)(V1),(char*)(long)(V2),autbasename(__FILE__,NULL),__LINE__))

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void  ctl_error __P((int Error, char *Text1, char *Text2, char *File, long Line));
  extern void  ctl_warning __P((int Warning, char *Text1, char *Text2, char *File, long Line));

# endif
