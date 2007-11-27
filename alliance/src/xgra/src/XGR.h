/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                    XGRA                           |
|                                                             |
| File    :                   XFS.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef XGRA_XFS
# define XGRA_XFS 

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define XGRA_OBJECT_CIRCLE     0x0000
# define XGRA_OBJECT_LINE       0x0001
# define XGRA_OBJECT_RECTANGLE  0x0002
# define XGRA_OBJECT_ARROW      0x0003
# define XGRA_OBJECT_TEXT       0x0004
# define XGRA_OBJECT_TYPE       0x000F

# define XGRA_OBJECT_NODE      0x0010
# define XGRA_OBJECT_ARC      0x0020

# define XGRA_OBJECT_LINE_DOWN  0x0100
# define XGRA_OBJECT_LINE_LEFT  0x0200

# define XGRA_OBJECT_SELECT     0x0400
# define XGRA_OBJECT_ACCEPT     0x0800
# define XGRA_OBJECT_CONNECT    0x1000

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

# define GetXgraObjectType( X ) ( ( X )->TYPE & XGRA_OBJECT_TYPE )

# define IsXgraLineDown( X ) ( ( X )->TYPE & XGRA_OBJECT_LINE_DOWN )
# define IsXgraLineLeft( X ) ( ( X )->TYPE & XGRA_OBJECT_LINE_LEFT )
# define IsXgraNode( X )    ( ( X )->TYPE & XGRA_OBJECT_NODE     )
# define IsXgraArc( X )    ( ( X )->TYPE & XGRA_OBJECT_ARC     )

# define SetXgraLineDown( X ) ( ( X )->TYPE |= XGRA_OBJECT_LINE_DOWN )
# define SetXgraLineLeft( X ) ( ( X )->TYPE |= XGRA_OBJECT_LINE_LEFT )
# define SetXgraNode( X )    ( ( X )->TYPE |= XGRA_OBJECT_NODE     )
# define SetXgraArc( X )    ( ( X )->TYPE |= XGRA_OBJECT_ARC     )

# define ClearXgraLineDown( X ) ( ( X )->TYPE &= ~XGRA_OBJECT_LINE_DOWN )
# define ClearXgraLineLeft( X ) ( ( X )->TYPE &= ~XGRA_OBJECT_LINE_LEFT )
# define ClearXgraNode( X )    ( ( X )->TYPE &= ~XGRA_OBJECT_NODE     )
# define ClearXgraArc( X )    ( ( X )->TYPE &= ~XGRA_OBJECT_ARC     )

/*------------------------------------------------------------\
|                                                             |
|                           Select                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXgraSelect( X )    ( ( X )->TYPE & XGRA_OBJECT_SELECT   )
# define SetXgraSelect( X )   ( ( X )->TYPE |= XGRA_OBJECT_SELECT  )
# define ClearXgraSelect( X ) ( ( X )->TYPE &= ~XGRA_OBJECT_SELECT )

/*------------------------------------------------------------\
|                                                             |
|                           Accept                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXgraAccept( X )    ( ( X )->TYPE & XGRA_OBJECT_ACCEPT   )
# define SetXgraAccept( X )   ( ( X )->TYPE |= XGRA_OBJECT_ACCEPT  )
# define ClearXgraAccept( X ) ( ( X )->TYPE &= ~XGRA_OBJECT_ACCEPT )

/*------------------------------------------------------------\
|                                                             |
|                           Connect                           |
|                                                             |
\------------------------------------------------------------*/

# define IsXgraConnect( X )    ( ( X )->TYPE & XGRA_OBJECT_CONNECT   )
# define SetXgraConnect( X )   ( ( X )->TYPE |= XGRA_OBJECT_CONNECT  )
# define ClearXgraConnect( X ) ( ( X )->TYPE &= ~XGRA_OBJECT_CONNECT )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Point                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xgrapoint
  {
    long  X;
    long  Y;

  } xgrapoint;

/*------------------------------------------------------------\
|                                                             |
|                            Object                           |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xgraobj_list
  {
    struct xgraobj_list *NEXT;
    struct xgraobj_list *LINK;
    char                *NAME;
    short                LAYER;
    long                 TYPE;
    long                 X;
    long                 Y;
    long                 DX;
    long                 DY;
    void                *USER;

  } xgraobj_list;

  typedef struct xautgraph
  {
    struct xautgraph *NEXT;
    char                *NAME;
    xgraobj_list        *OBJECT [ XGRA_MAX_LAYER ];
    xgraobj_list        *TEXT   [ XGRA_MAX_LAYER ];

  } xautgraph;

/*------------------------------------------------------------\
|                                                             |
|                            Select                           |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xgraselect_list 
  {
    struct xgraselect_list *NEXT;
    xgraobj_list           *OBJECT;

  } xgraselect_list;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern long            XgraBoundXmin;
  extern long            XgraBoundYmin;
  extern long            XgraBoundXmax;
  extern long            XgraBoundYmax; 
  extern char            XgraRecomputeBound;

  extern xautgraph    *XgraFigure;
  extern autgraph     *XgraFigureGra;

  extern xgraselect_list *XgraHeadSelect;
  extern xgraselect_list *XgraHeadConnect;

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

  extern xautgraph   *XgraAddFigure();
  extern xgraobj_list   *XgraAddCircle();
  extern xgraobj_list   *XgraAddLine();
  extern xgraobj_list   *XgraAddText();

/*------------------------------------------------------------\
|                                                             |
|                          Del Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraDelFigure();

/*------------------------------------------------------------\
|                                                             |
|                   Flag Set & Clear Functions                |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraSelectObject();
  extern void XgraUnselectObject();
  extern void XgraAcceptObject();
  extern void XgraRejectObject();
  extern void XgraConnectObject();
  extern void XgraDisconnectObject();

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraAddSelect();
  extern void XgraDelSelect();
  extern void XgraPurgeSelect();

/*------------------------------------------------------------\
|                                                             |
|                          Connect Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraAddConnect();
  extern void XgraDelConnect();

/*------------------------------------------------------------\
|                                                             |
|                          Bound Compute                      |
|                                                             |
\------------------------------------------------------------*/

  extern char XgraComputeBound();

/*------------------------------------------------------------\
|                                                             |
|                        Load Functions                       |
|                                                             |
\------------------------------------------------------------*/

 extern void XgraLoadFigure();

/*------------------------------------------------------------\
|                                                             |
|                    Error Message Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern char *XgraGetErrorMessage();

/*------------------------------------------------------------\
|                                                             |
|                    Informations Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern char *XgraGetInformations();

# endif
