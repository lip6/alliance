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
| Tool    :                     Vpn                           |
|                                                             |
| File    :                 Vpn Errors                        |
|                                                             |
| Vpnhors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   04.07.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef VPN_ERROR_H
# define VPN_ERROR_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define VPN_PLACE_EXIST_ERROR             0
# define VPN_TRANS_EXIST_ERROR             1
# define VPN_PORT_EXIST_ERROR              2
# define VPN_ARC_PLACE_EXIST_ERROR         3
# define VPN_ARC_TRANS_EXIST_ERROR         4
# define VPN_SIGNAL_EXIST_ERROR            6
# define VPN_VARIABLE_EXIST_ERROR          7
# define VPN_DEFINE_EXIST_ERROR            8
# define VPN_CONSTANT_EXIST_ERROR          9
# define VPN_TRANS_FATHER_UNKNOWN         10
# define VPN_PROC_EXIST_ERROR             11
# define VPN_DEL_ARC_PLACE_ERROR          12
# define VPN_DEL_ARC_TRANS_ERROR          13
# define VPN_FUNC_EXIST_ERROR             14
# define VPN_UNKNOWN_DECLAR_TYPE          15
# define VPN_ARGUMENT_EXIST_ERROR         16
# define VPN_UNDECLARED_SYMBOL            17
# define VPN_DECLAR_EXIST_ERROR           18
# define VPN_BAD_DECLAR_TYPE              19
# define VPN_BAD_ASSIGN_TYPE              20
# define VPN_ILLEGAL_INDEX_ERROR          21
# define VPN_ILLEGAL_POSITION_ERROR       22
# define VPN_INS_EXIST_ERROR              23
# define VPN_MOD_EXIST_ERROR              24
# define VPN_GENERIC_EXIST_ERROR          25

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

# define vpnerror( E, V1, V2 ) \
   (vpn_error((int)(E),(char*)(long)(V1),(char*)(long)(V2), basename(__FILE__),__LINE__))

# define vpnwarning( E, V1, V2 ) \
   (vpn_warning( (int)(E),(char*)(long)(V1),(char*)(long)(V2), basename(__FILE__),__LINE__))

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void  vpn_error __P((int Error, char *Text1, char *Text2, char *File, long Line));
  extern void  vpn_warning __P((int Warning, char *Text1, char *Text2, char *File, long Line));

# endif
