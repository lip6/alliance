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
| Tool    :                    XFSM                           |
|                                                             |
| File    :                   XFS.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef XFSM_XFS
# define XFSM_XFS 

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define XFSM_OBJECT_CIRCLE     0x0000
# define XFSM_OBJECT_LINE       0x0001
# define XFSM_OBJECT_RECTANGLE  0x0002
# define XFSM_OBJECT_ARROW      0x0003
# define XFSM_OBJECT_TEXT       0x0004
# define XFSM_OBJECT_TYPE       0x000F

# define XFSM_OBJECT_STATE      0x0010
# define XFSM_OBJECT_TRANS      0x0020

# define XFSM_OBJECT_LINE_DOWN  0x0100
# define XFSM_OBJECT_LINE_LEFT  0x0200

# define XFSM_OBJECT_SELECT     0x0400
# define XFSM_OBJECT_ACCEPT     0x0800
# define XFSM_OBJECT_CONNECT    0x1000

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

# define GetXfsmObjectType( X ) ( ( X )->TYPE & XFSM_OBJECT_TYPE )

# define IsXfsmLineDown( X ) ( ( X )->TYPE & XFSM_OBJECT_LINE_DOWN )
# define IsXfsmLineLeft( X ) ( ( X )->TYPE & XFSM_OBJECT_LINE_LEFT )
# define IsXfsmState( X )    ( ( X )->TYPE & XFSM_OBJECT_STATE     )
# define IsXfsmTrans( X )    ( ( X )->TYPE & XFSM_OBJECT_TRANS     )

# define SetXfsmLineDown( X ) ( ( X )->TYPE |= XFSM_OBJECT_LINE_DOWN )
# define SetXfsmLineLeft( X ) ( ( X )->TYPE |= XFSM_OBJECT_LINE_LEFT )
# define SetXfsmState( X )    ( ( X )->TYPE |= XFSM_OBJECT_STATE     )
# define SetXfsmTrans( X )    ( ( X )->TYPE |= XFSM_OBJECT_TRANS     )

# define ClearXfsmLineDown( X ) ( ( X )->TYPE &= ~XFSM_OBJECT_LINE_DOWN )
# define ClearXfsmLineLeft( X ) ( ( X )->TYPE &= ~XFSM_OBJECT_LINE_LEFT )
# define ClearXfsmState( X )    ( ( X )->TYPE &= ~XFSM_OBJECT_STATE     )
# define ClearXfsmTrans( X )    ( ( X )->TYPE &= ~XFSM_OBJECT_TRANS     )

/*------------------------------------------------------------\
|                                                             |
|                           Select                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXfsmSelect( X )    ( ( X )->TYPE & XFSM_OBJECT_SELECT   )
# define SetXfsmSelect( X )   ( ( X )->TYPE |= XFSM_OBJECT_SELECT  )
# define ClearXfsmSelect( X ) ( ( X )->TYPE &= ~XFSM_OBJECT_SELECT )

/*------------------------------------------------------------\
|                                                             |
|                           Accept                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXfsmAccept( X )    ( ( X )->TYPE & XFSM_OBJECT_ACCEPT   )
# define SetXfsmAccept( X )   ( ( X )->TYPE |= XFSM_OBJECT_ACCEPT  )
# define ClearXfsmAccept( X ) ( ( X )->TYPE &= ~XFSM_OBJECT_ACCEPT )

/*------------------------------------------------------------\
|                                                             |
|                           Connect                           |
|                                                             |
\------------------------------------------------------------*/

# define IsXfsmConnect( X )    ( ( X )->TYPE & XFSM_OBJECT_CONNECT   )
# define SetXfsmConnect( X )   ( ( X )->TYPE |= XFSM_OBJECT_CONNECT  )
# define ClearXfsmConnect( X ) ( ( X )->TYPE &= ~XFSM_OBJECT_CONNECT )

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

  typedef struct xfsmobj_list
  {
    struct xfsmobj_list *NEXT;
    struct xfsmobj_list *LINK;
    char                *NAME;
    short                LAYER;
    long                 TYPE;
    long                 X;
    long                 Y;
    long                 DX;
    long                 DY;
    void                *USER;

  } xfsmobj_list;

  typedef struct xfsmfig_list
  {
    struct xfsmfig_list *NEXT;
    char                *NAME;
    xfsmobj_list        *OBJECT [ XFSM_MAX_LAYER ];
    xfsmobj_list        *TEXT   [ XFSM_MAX_LAYER ];

  } xfsmfig_list;

/*------------------------------------------------------------\
|                                                             |
|                            Select                           |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xfsmselect_list 
  {
    struct xfsmselect_list *NEXT;
    xfsmobj_list           *OBJECT;

  } xfsmselect_list;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern long            XfsmBoundXmin;
  extern long            XfsmBoundYmin;
  extern long            XfsmBoundXmax;
  extern long            XfsmBoundYmax; 
  extern char            XfsmRecomputeBound;

  extern xfsmfig_list    *XfsmFigure;
  extern fsmfig_list     *XfsmFigureFsm;

  extern xfsmselect_list *XfsmHeadSelect;
  extern xfsmselect_list *XfsmHeadConnect;

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

  extern xfsmfig_list   *XfsmAddFigure();
  extern xfsmobj_list   *XfsmAddCircle();
  extern xfsmobj_list   *XfsmAddLine();
  extern xfsmobj_list   *XfsmAddText();

/*------------------------------------------------------------\
|                                                             |
|                          Del Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmDelFigure();

/*------------------------------------------------------------\
|                                                             |
|                   Flag Set & Clear Functions                |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmSelectObject();
  extern void XfsmUnselectObject();
  extern void XfsmAcceptObject();
  extern void XfsmRejectObject();
  extern void XfsmConnectObject();
  extern void XfsmDisconnectObject();

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmAddSelect();
  extern void XfsmDelSelect();
  extern void XfsmPurgeSelect();

/*------------------------------------------------------------\
|                                                             |
|                          Connect Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmAddConnect();
  extern void XfsmDelConnect();

/*------------------------------------------------------------\
|                                                             |
|                          Bound Compute                      |
|                                                             |
\------------------------------------------------------------*/

  extern char XfsmComputeBound();

/*------------------------------------------------------------\
|                                                             |
|                        Load Functions                       |
|                                                             |
\------------------------------------------------------------*/

 extern void XfsmLoadFigure();

/*------------------------------------------------------------\
|                                                             |
|                    Error Message Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern char *XfsmGetErrorMessage();
  extern void  XfsmExitErrorMessage();

/*------------------------------------------------------------\
|                                                             |
|                    Informations Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern char *XfsmGetInformations();

# endif
