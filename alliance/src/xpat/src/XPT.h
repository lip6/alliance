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
| Tool    :                    XPAT                           |
|                                                             |
| File    :                   XPT.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef XPAT_XPT
# define XPAT_XPT 

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Objects                           |
|                                                             |
\------------------------------------------------------------*/

# define XPAT_OBJECT_CIRCLE     0x0000
# define XPAT_OBJECT_LINE       0x0001
# define XPAT_OBJECT_RECTANGLE  0x0002
# define XPAT_OBJECT_ARROW      0x0003
# define XPAT_OBJECT_TEXT       0x0004
# define XPAT_OBJECT_HEXAGON    0x0005
# define XPAT_OBJECT_TYPE       0x000F

# define XPAT_OBJECT_VECTOR     0x0010
# define XPAT_OBJECT_IO         0x0020
# define XPAT_OBJECT_CURSOR     0x0040

# define XPAT_OBJECT_LINE_DOWN  0x0100
# define XPAT_OBJECT_LINE_LEFT  0x0200

# define XPAT_OBJECT_SELECT     0x0400
# define XPAT_OBJECT_ACCEPT     0x0800
# define XPAT_OBJECT_CONNECT    0x1000
# define XPAT_OBJECT_DELETE     0x2000

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

# define GetXpatObjectType( X ) ( ( X )->TYPE & XPAT_OBJECT_TYPE )

# define IsXpatLineDown( X ) ( ( X )->TYPE & XPAT_OBJECT_LINE_DOWN )
# define IsXpatLineLeft( X ) ( ( X )->TYPE & XPAT_OBJECT_LINE_LEFT )
# define IsXpatVector( X )  ( ( X )->TYPE & XPAT_OBJECT_VECTOR   )
# define IsXpatIO( X )       ( ( X )->TYPE & XPAT_OBJECT_IO        )
# define IsXpatCursor( X )   ( ( X )->TYPE & XPAT_OBJECT_CURSOR    )

# define SetXpatLineDown( X ) ( ( X )->TYPE |= XPAT_OBJECT_LINE_DOWN )
# define SetXpatLineLeft( X ) ( ( X )->TYPE |= XPAT_OBJECT_LINE_LEFT )
# define SetXpatVector( X )  ( ( X )->TYPE |= XPAT_OBJECT_VECTOR   )
# define SetXpatIO( X )       ( ( X )->TYPE |= XPAT_OBJECT_IO        )
# define SetXpatCursor( X )  ( ( X )->TYPE |= XPAT_OBJECT_CURSOR   )

# define ClearXpatLineDown( X ) ( ( X )->TYPE &= ~XPAT_OBJECT_LINE_DOWN )
# define ClearXpatLineLeft( X ) ( ( X )->TYPE &= ~XPAT_OBJECT_LINE_LEFT )
# define ClearXpatVector( X )  ( ( X )->TYPE &= ~XPAT_OBJECT_VECTOR   )
# define ClearXpatIO( X )       ( ( X )->TYPE &= ~XPAT_OBJECT_IO        )
# define ClearXpatCursor( X )  ( ( X )->TYPE &= ~XPAT_OBJECT_CURSOR   )

/*------------------------------------------------------------\
|                                                             |
|                           Select                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXpatSelect( X )    ( ( X )->TYPE & XPAT_OBJECT_SELECT   )
# define SetXpatSelect( X )   ( ( X )->TYPE |= XPAT_OBJECT_SELECT  )
# define ClearXpatSelect( X ) ( ( X )->TYPE &= ~XPAT_OBJECT_SELECT )

/*------------------------------------------------------------\
|                                                             |
|                           Accept                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXpatAccept( X )    ( ( X )->TYPE & XPAT_OBJECT_ACCEPT   )
# define SetXpatAccept( X )   ( ( X )->TYPE |= XPAT_OBJECT_ACCEPT  )
# define ClearXpatAccept( X ) ( ( X )->TYPE &= ~XPAT_OBJECT_ACCEPT )

/*------------------------------------------------------------\
|                                                             |
|                           Connect                           |
|                                                             |
\------------------------------------------------------------*/

# define IsXpatConnect( X )    ( ( X )->TYPE & XPAT_OBJECT_CONNECT   )
# define SetXpatConnect( X )   ( ( X )->TYPE |= XPAT_OBJECT_CONNECT  )
# define ClearXpatConnect( X ) ( ( X )->TYPE &= ~XPAT_OBJECT_CONNECT )

/*------------------------------------------------------------\
|                                                             |
|                           Delete                            |
|                                                             |
\------------------------------------------------------------*/

# define IsXpatDelete( X )    ( ( X )->TYPE & XPAT_OBJECT_DELETE   )
# define SetXpatDelete( X )   ( ( X )->TYPE |= XPAT_OBJECT_DELETE  )
# define ClearXpatDelete( X ) ( ( X )->TYPE &= ~XPAT_OBJECT_DELETE )

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

  typedef struct xpatpoint
  {
    long  X;
    long  Y;

  } xpatpoint;

/*------------------------------------------------------------\
|                                                             |
|                            Object                           |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xpatobj_list
  {
    struct xpatobj_list *NEXT;
    struct xpatobj_list *LINK;
    char                *NAME;
    short                LAYER;
    long                 TYPE;
    long                 X;
    long                 Y;
    long                 DX;
    long                 DY;
    void                *USER;

  } xpatobj_list;

  typedef struct xpatfig_list
  {
    struct xpatfig_list *NEXT;
    char                *NAME;
    xpatobj_list        *OBJECT [ XPAT_MAX_LAYER ];

  } xpatfig_list;

/*------------------------------------------------------------\
|                                                             |
|                         Pattern Figure                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct patfig_list
  {
    struct patfig_list *NEXT;
    char               *NAME;
    char              **IO_NAME_ARRAY;
    char              **IO_MODE_ARRAY;
    char               *IO_FORMAT_ARRAY;
    long               *IO_WIDTH_ARRAY;
    long                IO_NUMBER;
    long                IO_NUMBER_BIT;
    paseq_list         *SEQUENCE;
    unsigned long       TIME_SCALE;
    unsigned long       TIME_DELTA;
    char                TIME_UNIT;

  } patfig_list;

/*------------------------------------------------------------\
|                                                             |
|                            Select                           |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xpatselect_list 
  {
    struct xpatselect_list *NEXT;
    xpatobj_list           *OBJECT;

  } xpatselect_list;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern long            XpatBoundXmin;
  extern long            XpatBoundYmin;
  extern long            XpatBoundXmax;
  extern long            XpatBoundYmax; 
  extern char            XpatRecomputeBound;

  extern xpatfig_list    *XpatFigure;
  extern patfig_list     *XpatFigurePat;

  extern xpatselect_list *XpatHeadSelect;
  extern xpatselect_list *XpatHeadConnect;

  extern long XPAT_PATTERN_STEP_X;
  extern long XPAT_PATTERN_STEP_Y;

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

  extern xpatfig_list   *XpatAddFigure();
  extern xpatobj_list   *XpatAddCircle();
  extern xpatobj_list   *XpatAddLine();
  extern xpatobj_list   *XpatAddRectangle();
  extern xpatobj_list   *XpatAddHexagon();
  extern xpatobj_list   *XpatAddArrow();
  extern xpatobj_list   *XpatAddText();

  extern double          XpatReScaleFigure();

/*------------------------------------------------------------\
|                                                             |
|                          Del Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatDelFigure();

/*------------------------------------------------------------\
|                                                             |
|                   Flag Set & Clear Functions                |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatSelectObject();
  extern void XpatUnselectObject();
  extern void XpatAcceptObject();
  extern void XpatRejectObject();
  extern void XpatConnectObject();
  extern void XpatDisconnectObject();

/*------------------------------------------------------------\
|                                                             |
|                          Select Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatAddSelect();
  extern void XpatDelSelect();
  extern void XpatPurgeSelect();

/*------------------------------------------------------------\
|                                                             |
|                          Connect Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatAddConnect();
  extern void XpatDelConnect();

/*------------------------------------------------------------\
|                                                             |
|                          Cursor  Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern xpatobj_list *XpatAddCursor();
  extern void          XpatDelCursor();

/*------------------------------------------------------------\
|                                                             |
|                          Bound Compute                      |
|                                                             |
\------------------------------------------------------------*/

  extern char XpatComputeBound();

/*------------------------------------------------------------\
|                                                             |
|                        Load Functions                       |
|                                                             |
\------------------------------------------------------------*/

 extern void XpatLoadFigure();

/*------------------------------------------------------------\
|                                                             |
|                    Error Message Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern char *XpatGetErrorMessage();
  extern void  XpatExitErrorMessage();

/*------------------------------------------------------------\
|                                                             |
|                    Informations Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern char *XpatGetInformations();

# endif
