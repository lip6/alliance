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
| Tool    :                    XSCH                           |
|                                                             |
| File    :                   XFS.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef XSCH_XFS
# define XSCH_XFS 

# include "scl.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define XSCH_OBJECT_CIRCLE      0x00000
# define XSCH_OBJECT_LINE        0x00001
# define XSCH_OBJECT_RECTANGLE   0x00002
# define XSCH_OBJECT_ARROW       0x00003
# define XSCH_OBJECT_PENTAGON    0x00004
# define XSCH_OBJECT_TEXT_CENTER 0x00005
# define XSCH_OBJECT_TEXT_LEFT   0x00006
# define XSCH_OBJECT_TEXT_RIGHT  0x00007
# define XSCH_OBJECT_TRIANGLE    0x00008
# define XSCH_OBJECT_CROSS_BOX   0x00009
# define XSCH_OBJECT_BUFFER      0x0000A
# define XSCH_OBJECT_AND         0x0000B
# define XSCH_OBJECT_OR          0x0000C
# define XSCH_OBJECT_XOR         0x0000D
# define XSCH_OBJECT_TRISTATE    0x0000E
# define XSCH_OBJECT_REGISTER    0x0000F
# define XSCH_OBJECT_TRANSISTOR  0x00010
# define XSCH_OBJECT_CONSTANT    0x00011
# define XSCH_OBJECT_ANDOR       0x00012
# define XSCH_OBJECT_ORAND       0x00013
# define XSCH_OBJECT_MUX         0x00014
                                     
# define XSCH_OBJECT_TYPE        0x000FF

# define XSCH_OBJECT_SCHCON      0x00100
# define XSCH_OBJECT_SCHNET      0x00200
# define XSCH_OBJECT_SCHBOX      0x00400
# define XSCH_OBJECT_SCHCON_EXT  0x00800

# define XSCH_OBJECT_LINE_DOWN   0x01000
# define XSCH_OBJECT_LINE_LEFT   0x02000

# define XSCH_OBJECT_SELECT      0x04000
# define XSCH_OBJECT_ACCEPT      0x08000
# define XSCH_OBJECT_CONNECT     0x10000

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

# define GetXschObjectType( X ) ( ( X )->TYPE & XSCH_OBJECT_TYPE )

# define IsXschLineDown( X )  ( ( X )->TYPE & XSCH_OBJECT_LINE_DOWN  )
# define IsXschLineLeft( X )  ( ( X )->TYPE & XSCH_OBJECT_LINE_LEFT  )
# define IsXschSchCon( X )    ( ( X )->TYPE & XSCH_OBJECT_SCHCON     )
# define IsXschSchConExt( X ) ( ( X )->TYPE & XSCH_OBJECT_SCHCON_EXT )
# define IsXschSchNet( X )    ( ( X )->TYPE & XSCH_OBJECT_SCHNET     )
# define IsXschSchBox( X )    ( ( X )->TYPE & XSCH_OBJECT_SCHBOX     )

# define SetXschLineDown( X )  ( ( X )->TYPE |= XSCH_OBJECT_LINE_DOWN  )
# define SetXschLineLeft( X )  ( ( X )->TYPE |= XSCH_OBJECT_LINE_LEFT  )
# define SetXschSchCon( X )    ( ( X )->TYPE |= XSCH_OBJECT_SCHCON     )
# define SetXschSchConExt( X ) ( ( X )->TYPE |= XSCH_OBJECT_SCHCON_EXT )
# define SetXschSchNet( X )    ( ( X )->TYPE |= XSCH_OBJECT_SCHNET     )
# define SetXschSchBox( X )    ( ( X )->TYPE |= XSCH_OBJECT_SCHBOX     )

# define ClearXschLineDown( X )  ( ( X )->TYPE &= ~XSCH_OBJECT_LINE_DOWN  )
# define ClearXschLineLeft( X )  ( ( X )->TYPE &= ~XSCH_OBJECT_LINE_LEFT  )
# define ClearXschSchCon( X )    ( ( X )->TYPE &= ~XSCH_OBJECT_SCHCON     )
# define ClearXschSchConExt( X ) ( ( X )->TYPE &= ~XSCH_OBJECT_SCHCON_EXT )
# define ClearXschSchNet( X )    ( ( X )->TYPE &= ~XSCH_OBJECT_SCHNET     )
# define ClearXschSchBox( X )    ( ( X )->TYPE &= ~XSCH_OBJECT_SCHBOX     )

/*------------------------------------------------------------\
|                                                             |
|                           Select                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXschSelect( X )    ( ( X )->TYPE & XSCH_OBJECT_SELECT   )
# define SetXschSelect( X )   ( ( X )->TYPE |= XSCH_OBJECT_SELECT  )
# define ClearXschSelect( X ) ( ( X )->TYPE &= ~XSCH_OBJECT_SELECT )

/*------------------------------------------------------------\
|                                                             |
|                           Accept                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXschAccept( X )    ( ( X )->TYPE & XSCH_OBJECT_ACCEPT   )
# define SetXschAccept( X )   ( ( X )->TYPE |= XSCH_OBJECT_ACCEPT  )
# define ClearXschAccept( X ) ( ( X )->TYPE &= ~XSCH_OBJECT_ACCEPT )

/*------------------------------------------------------------\
|                                                             |
|                           Connect                           |
|                                                             |
\------------------------------------------------------------*/

# define IsXschConnect( X )    ( ( X )->TYPE & XSCH_OBJECT_CONNECT   )
# define SetXschConnect( X )   ( ( X )->TYPE |= XSCH_OBJECT_CONNECT  )
# define ClearXschConnect( X ) ( ( X )->TYPE &= ~XSCH_OBJECT_CONNECT )

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

  typedef struct xschobj_list
  {
    struct xschobj_list *NEXT;
    struct xschobj_list *LINK;
    char                *NAME;
    short                LAYER;
    long                 TYPE;
    long                 X;
    long                 Y;
    long                 DX;
    long                 DY;
    short                ARG1;
    short                ARG2;
    short                ARG3;
    short                ARG4;
    short                ARG5;
    short                ARG6;
    void                *USER;

  } xschobj_list;

  typedef struct xschfig_list
  {
    struct xschfig_list *NEXT;
    char                *NAME;
    xschobj_list        *OBJECT [ XSCH_MAX_LAYER ];
    void                *USER;

  } xschfig_list;

/*------------------------------------------------------------\
|                                                             |
|                            Select                           |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xschselect_list 
  {
    struct xschselect_list *NEXT;
    xschobj_list           *OBJECT;

  } xschselect_list;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern long            XschBoundXmin;
  extern long            XschBoundYmin;
  extern long            XschBoundXmax;
  extern long            XschBoundYmax; 
  extern char            XschRecomputeBound;

  extern xschfig_list    *XschFigure;
  extern schfig_list     *XschFigureSch;

  extern xschselect_list *XschHeadSelect;
  extern xschselect_list *XschHeadConnect;

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

  extern xschfig_list   *XschAddFigure();
  extern xschobj_list   *XschAddCircle();
  extern xschobj_list   *XschAddLine();
  extern xschobj_list   *XschAddRectangle();
  extern xschobj_list   *XschAddTriangle();
  extern xschobj_list   *XschAddPentagon();
  extern xschobj_list   *XschAddText();
  extern xschobj_list   *XschAddTextWidth();
  extern xschobj_list   *XschAddCrossBox();

  extern xschobj_list   *XschAddBuffer();
  extern xschobj_list   *XschAddAnd();
  extern xschobj_list   *XschAddOr();
  extern xschobj_list   *XschAddXor();
  extern xschobj_list   *XschAddTristate();
  extern xschobj_list   *XschAddRegister();
  extern xschobj_list   *XschAddTransistor();
  extern xschobj_list   *XschAddConstant();
  extern xschobj_list   *XschAddAndOr();
  extern xschobj_list   *XschAddOrAnd();
  extern xschobj_list   *XschAddMux();

/*------------------------------------------------------------\
|                                                             |
|                          Del Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XschDelFigure();

/*------------------------------------------------------------\
|                                                             |
|                   Flag Set & Clear Functions                |
|                                                             |
\------------------------------------------------------------*/

  extern void XschSelectObject();
  extern void XschUnselectObject();
  extern void XschAcceptObject();
  extern void XschRejectObject();
  extern void XschConnectObject();
  extern void XschDisconnectObject();

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XschAddSelect();
  extern void XschDelSelect();
  extern void XschPurgeSelect();
  extern void XschFreeSelect();

/*------------------------------------------------------------\
|                                                             |
|                          Connect Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern void XschAddConnect();
  extern void XschDelConnect();

/*------------------------------------------------------------\
|                                                             |
|                          Bound Compute                      |
|                                                             |
\------------------------------------------------------------*/

  extern char XschComputeBound();

/*------------------------------------------------------------\
|                                                             |
|                        Load Functions                       |
|                                                             |
\------------------------------------------------------------*/

 extern void XschLoadFigure();

/*------------------------------------------------------------\
|                                                             |
|                    Error Message Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern char *XschGetErrorMessage();
  extern void  XschExitErrorMessage();
  extern void  XschInitializeErrorMessage();

/*------------------------------------------------------------\
|                                                             |
|                    Informations Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern char *XschGetInformations();

# endif
