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
| Tool    :                   XVPN                            |
|                                                             |
| File    :                   XME.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef XVPN_XME
# define XVPN_XME 

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

  extern XvpnMenuItem  XvpnEditMenu[];

  extern XvpnPanelItem XvpnEditSearchViewPanel;
  extern XvpnPanelItem XvpnEditSelectPanel;
  extern XvpnPanelItem XvpnEditIdentifyPanel;
  extern XvpnPanelItem XvpnEditDescriptionPanel;

  extern int XvpnEditSearchViewDefaultValues[ 5 ];
  extern int XvpnEditIdentifyDefaultValues[ 5 ];
  extern int XvpnEditDescriptionDefaultValues[ 5 ];
  extern int XvpnEditSelectDefaultValues[ 5 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnBuildEditDialog();

  extern void XvpnEditSearch();
  extern void XvpnEditIdentify();
  extern void XvpnEditConnected();
  extern void XvpnEditUnselectAll();

  extern void XvpnDisplayEditIdentify();
  extern void XvpnDisplayEditDescription();

# endif
