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
#ifndef __IOC_HEADER_H
#define __IOC_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct connector
{
    struct connector	*NEXT	;
    char		ORIENT	;
    char		*NAME	;
    int			VALUE	; // in case of SPACE
} con_list;

extern con_list* add_con(con_list* ptlist, char orient, char* name);
extern con_list* add_space(con_list* ptlist, char orient, char* value);
extern con_list* iocparse(char *file);

#ifdef __cplusplus
}
#endif

#endif /* __IOC_HEADER_H */
