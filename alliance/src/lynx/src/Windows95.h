/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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

#define GrgWin 1983
#define GRGWINMAXREC 8
#define MAXSTACK 100

typedef struct stwin
{
  long mx,my,Mx,My;	/* Coordonnées maximum du chip */
  long Px,Py;		/* Taille des fenetres */
  long nx,ny;		/* Nombre de fenetre */
  struct sGrgWinChRec **Windows; /* Pointeur vers liste chainee des fenetres */
} GrgWindow;

typedef struct sGrgWinChRec
{
  struct sGrgWinChRec	*NEXT;
  long			 POS;
  rdsrec_list		*REC[GRGWINMAXREC];
} GrgWinChRec;

GrgWindow*  GrgCreatWindows __P((losig_list*));
chain_list* GrgGetInterWindows __P((rdsrec_list*, GrgWindow*));
void        GrgClearWindows __P((GrgWindow*, losig_list*));

