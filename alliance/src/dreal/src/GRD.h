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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   GRD101.h                        |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef DREAL_GRD
# define DREAL_GRD 

# define DREAL_SIZE (sizeof( drealrec ))

# define DREAL_MAX_REC       8
# define DREAL_ONE_WINDOW    (long)0x00010000
# define DREAL_DELETED       (long)0x00020000
# define DREAL_ACCEPTED      (long)0x00040000
# define DREAL_SELECTED      (long)0x00100000
# define DREAL_DRAW_TEXT     (long)0x00200000

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_PREVIOUS( R )                                  \
                                                              \
    ((rdsrec_list **)((R)->USER))

# define DREAL_PREVIOUS_L( R )                                \
                                                              \
    ((R)->USER)

# define DREAL_WINDOW( R )                                    \
                                                              \
    (((drealrec *)((char *)(R)+sizeof(rdsrec_list)))->WINDOW  )

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Windows                          |
|                                                             |
\------------------------------------------------------------*/
 
# define IsDrealOneWindow( Rectangle )         \
                                               \
   ( ( Rectangle )->FLAGS & DREAL_ONE_WINDOW   )

# define SetDrealOneWindow( Rectangle )        \
                                               \
   ( ( Rectangle )->FLAGS |= DREAL_ONE_WINDOW  )
 
# define ClearDrealOneWindow( Rectangle )      \
                                               \
   ( ( Rectangle )->FLAGS &= ~DREAL_ONE_WINDOW )

/*------------------------------------------------------------\
|                                                             |
|                            Deleted                          |
|                                                             |
\------------------------------------------------------------*/

# define IsDrealDeleted( Rectangle )        \
                                            \
   ( ( Rectangle )->FLAGS & DREAL_DELETED   )

# define SetDrealDeleted( Rectangle )       \
                                            \
   ( ( Rectangle )->FLAGS |= DREAL_DELETED  )

# define ClearDrealDeleted( Rectangle )     \
                                            \
   ( ( Rectangle )->FLAGS &= ~DREAL_DELETED )

/*------------------------------------------------------------\
|                                                             |
|                            Accepted                         |
|                                                             |
\------------------------------------------------------------*/

# define IsDrealAccepted( Rectangle )        \
                                             \
   ( ( Rectangle )->FLAGS & DREAL_ACCEPTED  )

# define SetDrealAccepted( Rectangle )       \
                                             \
   ( ( Rectangle )->FLAGS |= DREAL_ACCEPTED )

# define ClearDrealAccepted( Rectangle )     \
                                             \
   ( ( Rectangle )->FLAGS &= ~DREAL_ACCEPTED)

/*------------------------------------------------------------\
|                                                             |
|                            Selected                         |
|                                                             |
\------------------------------------------------------------*/

# define IsDrealSelected( Rectangle )        \
                                             \
   ( ( Rectangle )->FLAGS & DREAL_SELECTED  )

# define SetDrealSelected( Rectangle )       \
                                             \
   ( ( Rectangle )->FLAGS |= DREAL_SELECTED )

# define ClearDrealSelected( Rectangle )     \
                                             \
   ( ( Rectangle )->FLAGS &= ~DREAL_SELECTED)

/*------------------------------------------------------------\
|                                                             |
|                           DrawText                          |
|                                                             |
\------------------------------------------------------------*/

# define IsDrealDrawText( Rectangle )         \
                                              \
   ( ( Rectangle )->FLAGS & DREAL_DRAW_TEXT   )

# define SetDrealDrawText( Rectangle )        \
                                              \
   ( ( Rectangle )->FLAGS |= DREAL_DRAW_TEXT  )

# define ClearDrealDrawText( Rectangle )      \
                                              \
   ( ( Rectangle )->FLAGS &= ~DREAL_DRAW_TEXT )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Dreal Windows                         |
|                                                             |
\------------------------------------------------------------*/

  typedef struct drealwinrec   

  {
    struct drealwinrec *NEXT; 
    rdsrec_list        *RECTAB[ DREAL_MAX_REC ];

  } drealwinrec;

  typedef struct drealwin
  {
    drealwinrec **LAYERTAB;

  } drealwin;

  typedef struct drealrecwin 

  {
    struct drealrecwin *NEXT;
    drealwin           *WINDOW;
    
  } drealrecwin;

/*------------------------------------------------------------\
|                                                             |
|                         Dreal Undo                          |
|                                                             |
\------------------------------------------------------------*/

  typedef struct drealundorec

  {
    struct drealundorec *NEXT;
    rdsrec_list         *RECTANGLE;

  } drealundorec;

  typedef struct drealundo

  {
    struct drealundo *NEXT;
    drealundorec     *UNDO;

  } drealundo;

/*------------------------------------------------------------\
|                                                             |
|                         Dreal Select                        |
|                                                             |
\------------------------------------------------------------*/

  typedef struct drealselect

  {
    struct drealselect  *NEXT;
    rdsrec_list         *RECTANGLE;

  } drealselect ;

/*------------------------------------------------------------\
|                                                             |
|                         Dreal Search                        |
|                                                             |
\------------------------------------------------------------*/

  typedef struct drealsearch

  {
    struct drealsearch *NEXT;
    rdsrec_list        *RECTANGLE;

  } drealsearch;

/*------------------------------------------------------------\
|                                                             |
|                         X Rectangle                         |
|                                                             |
\------------------------------------------------------------*/

  typedef struct drealrec

  {
    drealrecwin  *WINDOW;

  } drealrec;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern char         DrealRecomputeBound;
  extern long         DrealBoundXmin;
  extern long         DrealBoundYmin;
  extern long         DrealBoundXmax;
  extern long         DrealBoundYmax; 
  extern long         DrealWindowXmin;
  extern long         DrealWindowYmin;
  extern long         DrealWindowXmax;
  extern long         DrealWindowYmax;
  extern long         DrealWindowDx;
  extern long         DrealWindowDy;
  extern long         DrealWindowSide;
  extern long         DrealWindowSize;
  extern drealwin    *DrealWindowTable;
  extern rdsfig_list *DrealFigureRds;

  extern long         DrealFigureXAB1;
  extern long         DrealFigureXAB2;
  extern long         DrealFigureYAB1;
  extern long         DrealFigureYAB2;

  extern drealundo    *DrealHeadRedo;
  extern drealundo    *DrealHeadUndo;
  extern drealselect  *DrealHeadSelect;
  extern drealsearch  *DrealHeadSearch;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void         DrealAddFigure();
  extern rdsrec_list *DrealAddRectangle();
  extern rdsrec_list *DrealAddAbox();

  extern void         DrealNewFigure();

  extern void         DrealDelFigure();
  extern void         DrealDelRectangle();
 
  extern char         DrealSaveFigure();
  extern void         DrealFlattenFigure();

  extern char         Drealsaverdsfig();
  extern rdsfig_list *Drealgetrdsfig();

/*------------------------------------------------------------\
|                                                             |
|                      Undo Acces Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealAddUndo();
  extern void DrealDelUndo();
  extern void DrealAddUndoRec();
  extern void DrealInitializeUndo();

/*------------------------------------------------------------\
|                                                             |
|                   Flag Set & Clear Functions                |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealDeleteRectangle();
  extern void DrealUndeleteRectangle();
  extern void DrealSelectRectangle();
  extern void DrealUnselectRectangle();
  extern void DrealAcceptRectangle();
  extern void DrealRejectRectangle();

/*------------------------------------------------------------\
|                                                             |
|                      Select Acces Functions                 |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealAddSelect();
  extern void DrealDelSelect();
  extern void DrealPurgeSelect();

/*------------------------------------------------------------\
|                                                             |
|                      Search Acces Functions                 |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealAddSearch();
  extern void DrealDelSearch();

/*------------------------------------------------------------\
|                                                             |
|                          Bound Compute                      |
|                                                             |
\------------------------------------------------------------*/

  extern char DrealComputeBound();

/*------------------------------------------------------------\
|                                                             |
|                    Window Acces Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealInitializeWindowTable();

/*------------------------------------------------------------\
|                                                             |
|                    Error Message Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern char *DrealGetErrorMessage();
  extern void  DrealExitErrorMessage();

/*------------------------------------------------------------\
|                                                             |
|                    Informations Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern char *DrealGetInformations();

# endif
