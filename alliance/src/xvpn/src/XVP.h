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
| Tool    :                    XVPN                           |
|                                                             |
| File    :                   XVP.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef XVPN_XVP
# define XVPN_XVP 

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define XVPN_OBJECT_CIRCLE     0x00000
# define XVPN_OBJECT_LINE       0x00001
# define XVPN_OBJECT_RECTANGLE  0x00002
# define XVPN_OBJECT_ARROW      0x00003
# define XVPN_OBJECT_TEXT       0x00004
# define XVPN_OBJECT_TYPE       0x0000F

# define XVPN_OBJECT_PLACE      0x00010
# define XVPN_OBJECT_TRANS      0x00020
# define XVPN_OBJECT_ARC_TRANS  0x00040
# define XVPN_OBJECT_ARC_PLACE  0x00080
# define XVPN_OBJECT_PROC       0x00100
# define XVPN_OBJECT_FUNC       0x00200

# define XVPN_OBJECT_LINE_DOWN  0x01000
# define XVPN_OBJECT_LINE_LEFT  0x02000

# define XVPN_OBJECT_SELECT     0x04000
# define XVPN_OBJECT_ACCEPT     0x08000
# define XVPN_OBJECT_CONNECT    0x10000

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Object                            |
|                                                             |
\------------------------------------------------------------*/

# define GetXvpnObjectType( X ) ( ( X )->TYPE & XVPN_OBJECT_TYPE )

# define IsXvpnLineDown( X ) ( ( X )->TYPE & XVPN_OBJECT_LINE_DOWN )
# define IsXvpnLineLeft( X ) ( ( X )->TYPE & XVPN_OBJECT_LINE_LEFT )
# define IsXvpnPlace( X )    ( ( X )->TYPE & XVPN_OBJECT_PLACE     )
# define IsXvpnTrans( X )    ( ( X )->TYPE & XVPN_OBJECT_TRANS     )
# define IsXvpnArcTrans( X ) ( ( X )->TYPE & XVPN_OBJECT_ARC_TRANS )
# define IsXvpnArcPlace( X ) ( ( X )->TYPE & XVPN_OBJECT_ARC_PLACE )
# define IsXvpnProc( X )     ( ( X )->TYPE & XVPN_OBJECT_PROC      )
# define IsXvpnFunc( X )     ( ( X )->TYPE & XVPN_OBJECT_FUNC      )

# define SetXvpnLineDown( X ) ( ( X )->TYPE |= XVPN_OBJECT_LINE_DOWN )
# define SetXvpnLineLeft( X ) ( ( X )->TYPE |= XVPN_OBJECT_LINE_LEFT )
# define SetXvpnPlace( X )    ( ( X )->TYPE |= XVPN_OBJECT_PLACE     )
# define SetXvpnTrans( X )    ( ( X )->TYPE |= XVPN_OBJECT_TRANS     )
# define SetXvpnArcTrans( X ) ( ( X )->TYPE |= XVPN_OBJECT_ARC_TRANS )
# define SetXvpnArcPlace( X ) ( ( X )->TYPE |= XVPN_OBJECT_ARC_PLACE )
# define SetXvpnProc( X )     ( ( X )->TYPE |= XVPN_OBJECT_PROC      )
# define SetXvpnFunc( X )     ( ( X )->TYPE |= XVPN_OBJECT_FUNC      )

# define ClearXvpnLineDown( X ) ( ( X )->TYPE &= ~XVPN_OBJECT_LINE_DOWN )
# define ClearXvpnLineLeft( X ) ( ( X )->TYPE &= ~XVPN_OBJECT_LINE_LEFT )
# define ClearXvpnPlace( X )    ( ( X )->TYPE &= ~XVPN_OBJECT_PLACE     )
# define ClearXvpnTrans( X )    ( ( X )->TYPE &= ~XVPN_OBJECT_TRANS     )
# define ClearXvpnArcTrans( X ) ( ( X )->TYPE &= ~XVPN_OBJECT_ARC_TRANS )
# define ClearXvpnArcPlace( X ) ( ( X )->TYPE &= ~XVPN_OBJECT_ARC_PLACE )
# define ClearXvpnProc( X )     ( ( X )->TYPE &= ~XVPN_OBJECT_PROC      )
# define ClearXvpnFunc( X )     ( ( X )->TYPE &= ~XVPN_OBJECT_FUNC      )

/*------------------------------------------------------------\
|                                                             |
|                           Select                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXvpnSelect( X )    ( ( X )->TYPE & XVPN_OBJECT_SELECT   )
# define SetXvpnSelect( X )   ( ( X )->TYPE |= XVPN_OBJECT_SELECT  )
# define ClearXvpnSelect( X ) ( ( X )->TYPE &= ~XVPN_OBJECT_SELECT )

/*------------------------------------------------------------\
|                                                             |
|                           Accept                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXvpnAccept( X )    ( ( X )->TYPE & XVPN_OBJECT_ACCEPT   )
# define SetXvpnAccept( X )   ( ( X )->TYPE |= XVPN_OBJECT_ACCEPT  )
# define ClearXvpnAccept( X ) ( ( X )->TYPE &= ~XVPN_OBJECT_ACCEPT )

/*------------------------------------------------------------\
|                                                             |
|                           Connect                           |
|                                                             |
\------------------------------------------------------------*/

# define IsXvpnConnect( X )    ( ( X )->TYPE & XVPN_OBJECT_CONNECT   )
# define SetXvpnConnect( X )   ( ( X )->TYPE |= XVPN_OBJECT_CONNECT  )
# define ClearXvpnConnect( X ) ( ( X )->TYPE &= ~XVPN_OBJECT_CONNECT )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Object                           |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xvpnobj_list
  {
    struct xvpnobj_list *NEXT;
    struct xvpnobj_list *LINK;
    char                *NAME;
    short                LAYER;
    long                 TYPE;
    long                 X;
    long                 Y;
    long                 DX;
    long                 DY;
    void                *USER;

  } xvpnobj_list;

  typedef struct xvpnfig_list
  {
    struct xvpnfig_list *NEXT;
    char                *NAME;
    xvpnobj_list        *OBJECT [ XVPN_MAX_LAYER ];
    xvpnobj_list        *TEXT   [ XVPN_MAX_LAYER ];

  } xvpnfig_list;

/*------------------------------------------------------------\
|                                                             |
|                            Select                           |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xvpnselect_list 
  {
    struct xvpnselect_list *NEXT;
    xvpnobj_list           *OBJECT;

  } xvpnselect_list;

/*------------------------------------------------------------\
|                                                             |
|                          Vpn Type                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern long            XvpnBoundXmin;
  extern long            XvpnBoundYmin;
  extern long            XvpnBoundXmax;
  extern long            XvpnBoundYmax; 
  extern char            XvpnRecomputeBound;

  extern xvpnfig_list    *XvpnFigure;
  extern vpnfig_list     *XvpnFigureVpn;

  extern xvpnselect_list *XvpnHeadSelect;
  extern xvpnselect_list *XvpnHeadConnect;

  extern int              XvpnReduce;
  extern int              XvpnReduceAfter;
  extern int              XvpnElaborate;
  extern int              XvpnBdd;
  extern int              XvpnBound;
  extern int              XvpnSimp;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Add Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern xvpnfig_list   *XvpnAddFigure();
  extern xvpnobj_list   *XvpnAddCircle();
  extern xvpnobj_list   *XvpnAddLine();
  extern xvpnobj_list   *XvpnAddText();

/*------------------------------------------------------------\
|                                                             |
|                          Del Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnDelFigure();

/*------------------------------------------------------------\
|                                                             |
|                     Elaborate Functions                     |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnElaborateFigure();

/*------------------------------------------------------------\
|                                                             |
|                   Flag Set & Clear Functions                |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnSelectObject();
  extern void XvpnUnselectObject();
  extern void XvpnAcceptObject();
  extern void XvpnRejectObject();
  extern void XvpnConnectObject();
  extern void XvpnDisconnectObject();

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnAddSelect();
  extern void XvpnDelSelect();
  extern void XvpnPurgeSelect();

/*------------------------------------------------------------\
|                                                             |
|                          Connect Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnAddConnect();
  extern void XvpnDelConnect();

/*------------------------------------------------------------\
|                                                             |
|                          Bound Compute                      |
|                                                             |
\------------------------------------------------------------*/

  extern char XvpnComputeBound();

/*------------------------------------------------------------\
|                                                             |
|                        Load Functions                       |
|                                                             |
\------------------------------------------------------------*/

 extern void  XvpnLoadFigure();

/*------------------------------------------------------------\
|                                                             |
|                         Vex Functions                       |
|                                                             |
\------------------------------------------------------------*/

  extern char *XvpnVex2String();

/*------------------------------------------------------------\
|                                                             |
|                    Error Message Functions                  |
|                                                             |
\------------------------------------------------------------*/
 
  extern void  XvpnInitializeErrorMessage();

  extern char *XvpnGetErrorMessage();
  extern void  XvpnExitErrorMessage();

/*------------------------------------------------------------\
|                                                             |
|                    Informations Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern char *XvpnGetInformations();

# endif
