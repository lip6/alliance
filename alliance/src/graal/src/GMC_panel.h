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

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Panel.h                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_CREATE_PANEL 
# define GRAAL_CREATE_PANEL 

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_CREATE_SEGMENT_X             790
# define GRAAL_CREATE_SEGMENT_Y             230
# define GRAAL_CREATE_VIA_X                 790
# define GRAAL_CREATE_VIA_Y                 330
# define GRAAL_CREATE_BIGVIA_X              790
# define GRAAL_CREATE_BIGVIA_Y              330
# define GRAAL_CREATE_TRANSISTOR_X          790
# define GRAAL_CREATE_TRANSISTOR_Y          330
# define GRAAL_CREATE_CONNECTOR_X           790
# define GRAAL_CREATE_CONNECTOR_Y           330
# define GRAAL_CREATE_REFERENCE_X           790
# define GRAAL_CREATE_REFERENCE_Y           330
# define GRAAL_CREATE_INSTANCE_X            790
# define GRAAL_CREATE_INSTANCE_Y            330

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void CallbackChangeSegmentLayer();
  extern void CallbackChangeSegmentWidth();
  extern void CallbackChangeSegmentName();
  extern void CallbackChangeSegmentWire();
  extern void CallbackChangeSegmentClose();

  extern void CallbackChangeViaType();
  extern void CallbackChangeViaName();
  extern void CallbackChangeViaClose();

  extern void CallbackChangeBigViaType();
  extern void CallbackChangeBigViaName();
  extern void CallbackChangeBigViaClose();

  extern void CallbackChangeTransistorType();
  extern void CallbackChangeTransistorWidth();
  extern void CallbackChangeTransistorName();
  extern void CallbackChangeTransistorWire();
  extern void CallbackChangeTransistorClose();

  extern void CallbackChangeConnectorLayer();
  extern void CallbackChangeConnectorWidth();
  extern void CallbackChangeConnectorOrient();
  extern void CallbackChangeConnectorClose();

  extern void CallbackChangeReferenceType();
  extern void CallbackChangeReferenceClose();

  extern void CallbackChangeInstanceSym();
  extern void CallbackChangeInstanceClose();

# endif 
