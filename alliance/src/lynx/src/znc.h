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

#define ZNCMAXREC 5

typedef struct sznc
{
  struct sznc	*NEXT;	/* seulement pour l'allocation par tas */
  long		 x1;
  long		 y1;
  long		 x2;
  long		 y2;
  long		 layer;
  chain_list	*Win;
  chain_list	*rec;
  long		 node;
} znc;

typedef struct sWinZnc
{
  long		  mx,my,MX,MY;
  long		  PX,PY,nx,ny;
  chain_list	**ChZnc;
}
WinZnc;

long addznc __P((long,long,long,long,long,WinZnc*));
WinZnc* creatwinznc __P((long,long,long,long,long,long,long,long));	/* long mx,my,MX,MY,PX,PY,nx,ny */
void freewinznc __P((WinZnc*));	/* WinZnc* */
chain_list* getwininterznc __P((znc*, WinZnc*,short));
void makezncnode __P((WinZnc*, losig_list*));
void linklonodelocon __P((chain_list*, losig_list*, WinZnc*, GrgWindow* ));	/* chain_list*, lorcnet_list*, WinZnc* */
