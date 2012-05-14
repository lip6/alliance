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

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Create.h                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_GMC
# define GRAAL_GMC

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

  extern GraalMenuItem  GraalCreateMenu[];
  extern GraalPanelItem GraalCreateSegmentPanel;
  extern GraalPanelItem GraalCreateViaPanel;
  extern GraalPanelItem GraalCreateBigViaPanel;
  extern GraalPanelItem GraalCreateTransistorPanel;
  extern GraalPanelItem GraalCreateConnectorPanel;
  extern GraalPanelItem GraalCreateReferencePanel;
  extern GraalPanelItem GraalCreateInstancePanel;

  extern int GraalCreateSegmentDefaultValues[ 5 ];
  extern int GraalCreateViaDefaultValues[ 5 ];
  extern int GraalCreateBigViaDefaultValues[ 5 ];
  extern int GraalCreateTransistorDefaultValues[ 5 ];
  extern int GraalCreateConnectorDefaultValues[ 5 ];
  extern int GraalCreateReferenceDefaultValues[ 5 ];
  extern int GraalCreateInstanceDefaultValues[ 5 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalBuildPanelCreate();
  extern void GraalBuildCreateDialog();
  extern void GraalCreateSegmentMbk();
  extern void GraalCreateViaMbk();
  extern void GraalCreateBigViaMbk();
  extern void GraalCreateTransistorMbk();
  extern void GraalCreateConnectorMbk();
  extern void GraalCreateReferenceMbk();
  extern void GraalCreateInstanceMbk();
  extern void GraalCreateAbutmentBoxMbk();

  extern void GraalChangeSegmentLayer();
  extern void GraalChangeViaType();
  extern void GraalChangeBigViaType();
  extern void GraalChangeTransistorType();
  extern void GraalChangeConnectorLayer();
  extern void GraalChangeReferenceType();

# endif

