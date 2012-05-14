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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/


/*************************************************************
*                  HEADERS                                   *
*                                                            *
* Changed by:    Greg                                        *
* Date:          05/07/95                                    *
* New file:      -                                           *
* Comments:                                                  *
*   Fonctions permettant d'ajouter un nouveau reseau dans    *
*   la figure logique existante.                             *
*                                                            *
*                                                            *
*************************************************************/

void GrgFatalError __P((int));
int IsRdsRealVia __P((rdsrec_list*));
void generate_RCN __P((lofig_list*));

/************************************************************
Codes de sortie en cas d'erreur:
Le nom de fichier entre parenthese indique l'origine
probable de l'erreur.
  10: Il existe deja un reseau RC dans la figure.
  22: Un signal logique n'est relie a aucune equipotentielle
      physique (netlist.c: LynxBuildLogicalSignal() );
  23: Un connecteur physique n'est pas relie a un connecteur
      logique (netlist.c: LynxBuildLogicalSignal());
  24: Un connecteur physique n'est pas relie a un connecteur
      logique (netlist.c: LynxBuildLogicalInstance());
  25: Un connecteur physique n'est pas relie a un connecteur
      logique (netlist.c: LynxBuildLogicalTransistor());
  26: Un connecteur logique n'est pas relie a un connecteur
      physique (addlynx.c);
  28: Une structure GrgPhVia possede un de ces trois champs
      NULL (addlynx.c).
  29: Un rectangle n'appartient a aucune des deux structures
      GrgGroup1 ou GrgGroup2 alors qu'il DOIT l'etre
  30: Type d'intersection entre deux rectangles non prevu
      (Intersect).  

************************************************************/

#define min(a,b) ((a>b)?b:a)
#define max(a,b) ((a>b)?a:b)

#define RCN_TAGVIA_MASK 0x80000000
