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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  GRM101.h                         |
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

# ifndef GRAAL_GRM
# define GRAAL_GRM 

# define GRAAL_SIZE (sizeof( graalrec ))

# define GRAAL_MAX_REC       8
# define GRAAL_ONE_WINDOW    (long)0x00010000
# define GRAAL_DELETED       (long)0x00020000
# define GRAAL_ACCEPTED      (long)0x00040000
# define GRAAL_TREATED       (long)0x00080000
# define GRAAL_FLATTEN       (long)0x00100000
# define GRAAL_PEEK          (long)0x00200000
# define GRAAL_DRUC          (long)0x00400000
# define GRAAL_INVISIBLE     (long)0x00800000
# define GRAAL_CUT           (long)0x01000000
# define GRAAL_SELECTED      (long)0x02000000
# define GRAAL_DRAW_TEXT     (long)0x04000000
# define GRAAL_SEARCH        (long)0x08000000

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_PREVIOUS( R )                                  \
                                                              \
    (((graalrec *)((char *)(R)+sizeof(rdsrec_list)))->PREVIOUS)

# define GRAAL_MBK( R )                                       \
                                                              \
    (((graalrec *)((char *)(R)+sizeof(rdsrec_list)))->MBK     )

# define GRAAL_WINDOW( R )                                    \
                                                              \
    (((graalrec *)((char *)(R)+sizeof(rdsrec_list)))->WINDOW  )

# define GRAAL_EQUI( R )                                      \
                                                              \
    (((graalrec *)((char *)(R)+sizeof(rdsrec_list)))->EQUI    )

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
 
# define IsGraalOneWindow( Rectangle )         \
                                               \
   ( ( Rectangle )->FLAGS & GRAAL_ONE_WINDOW   )

# define SetGraalOneWindow( Rectangle )        \
                                               \
   ( ( Rectangle )->FLAGS |= GRAAL_ONE_WINDOW  )
 
# define ClearGraalOneWindow( Rectangle )      \
                                               \
   ( ( Rectangle )->FLAGS &= ~GRAAL_ONE_WINDOW )

/*------------------------------------------------------------\
|                                                             |
|                             Search                          |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalSearch( Rectangle )        \
                                            \
   ( ( Rectangle )->FLAGS & GRAAL_SEARCH   )

# define SetGraalSearch( Rectangle )       \
                                            \
   ( ( Rectangle )->FLAGS |= GRAAL_SEARCH  )
 
# define ClearGraalSearch( Rectangle )     \
                                            \
   ( ( Rectangle )->FLAGS &= ~GRAAL_SEARCH )

/*------------------------------------------------------------\
|                                                             |
|                            Deleted                          |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalDeleted( Rectangle )        \
                                            \
   ( ( Rectangle )->FLAGS & GRAAL_DELETED   )

# define SetGraalDeleted( Rectangle )       \
                                            \
   ( ( Rectangle )->FLAGS |= GRAAL_DELETED  )
 
# define ClearGraalDeleted( Rectangle )     \
                                            \
   ( ( Rectangle )->FLAGS &= ~GRAAL_DELETED )

/*------------------------------------------------------------\
|                                                             |
|                            Accepted                         |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalAccepted( Rectangle )        \
                                             \
   ( ( Rectangle )->FLAGS & GRAAL_ACCEPTED  )

# define SetGraalAccepted( Rectangle )       \
                                             \
   ( ( Rectangle )->FLAGS |= GRAAL_ACCEPTED )

# define ClearGraalAccepted( Rectangle )     \
                                             \
   ( ( Rectangle )->FLAGS &= ~GRAAL_ACCEPTED)

/*------------------------------------------------------------\
|                                                             |
|                            Selected                         |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalSelected( Rectangle )        \
                                             \
   ( ( Rectangle )->FLAGS & GRAAL_SELECTED  )

# define SetGraalSelected( Rectangle )       \
                                             \
   ( ( Rectangle )->FLAGS |= GRAAL_SELECTED )

# define ClearGraalSelected( Rectangle )     \
                                             \
   ( ( Rectangle )->FLAGS &= ~GRAAL_SELECTED)

/*------------------------------------------------------------\
|                                                             |
|                            Treated                          |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalTreated( Rectangle )       \
                                           \
   ( ( Rectangle )->FLAGS & GRAAL_TREATED  )

# define SetGraalTreated( Rectangle )      \
                                           \
   ( ( Rectangle )->FLAGS |= GRAAL_TREATED )

# define ClearGraalTreated( Rectangle )     \
                                            \
   ( ( Rectangle )->FLAGS &= ~GRAAL_TREATED )

/*------------------------------------------------------------\
|                                                             |
|                           DrawText                          |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalDrawText( Rectangle )         \
                                              \
   ( ( Rectangle )->FLAGS & GRAAL_DRAW_TEXT   )

# define SetGraalDrawText( Rectangle )        \
                                              \
   ( ( Rectangle )->FLAGS |= GRAAL_DRAW_TEXT  )

# define ClearGraalDrawText( Rectangle )      \
                                              \
   ( ( Rectangle )->FLAGS &= ~GRAAL_DRAW_TEXT )

/*------------------------------------------------------------\
|                                                             |
|                            Flatten                          |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalFlatten( Rectangle )       \
                                           \
   ( ( Rectangle )->FLAGS & GRAAL_FLATTEN  )

# define SetGraalFlatten( Rectangle )      \
                                           \
   ( ( Rectangle )->FLAGS |= GRAAL_FLATTEN )

# define ClearGraalFlatten( Rectangle )     \
                                            \
   ( ( Rectangle )->FLAGS &= ~GRAAL_FLATTEN )

/*------------------------------------------------------------\
|                                                             |
|                              Peek                           |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalPeek( Rectangle )        \
                                         \
   ( ( Rectangle )->FLAGS & GRAAL_PEEK   )

# define SetGraalPeek( Rectangle )       \
                                         \
   ( ( Rectangle )->FLAGS |= GRAAL_PEEK  )

# define ClearGraalPeek( Rectangle )     \
                                         \
   ( ( Rectangle )->FLAGS &= ~GRAAL_PEEK )

/*------------------------------------------------------------\
|                                                             |
|                              Druc                           |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalDruc( Rectangle )        \
                                         \
   ( ( Rectangle )->FLAGS & GRAAL_DRUC   )

# define SetGraalDruc( Rectangle )       \
                                         \
   ( ( Rectangle )->FLAGS |= GRAAL_DRUC  )

# define ClearGraalDruc( Rectangle )     \
                                         \
   ( ( Rectangle )->FLAGS &= ~GRAAL_DRUC )

/*------------------------------------------------------------\
|                                                             |
|                           Invisible                         |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalInvisible( Rectangle )        \
                                              \
   ( ( Rectangle )->FLAGS & GRAAL_INVISIBLE   )

# define SetGraalInvisible( Rectangle )       \
                                              \
   ( ( Rectangle )->FLAGS |= GRAAL_INVISIBLE  )

# define ClearGraalInvisible( Rectangle )     \
                                              \
   ( ( Rectangle )->FLAGS &= ~GRAAL_INVISIBLE )

/*------------------------------------------------------------\
|                                                             |
|                               Cut                           |
|                                                             |
\------------------------------------------------------------*/

# define IsGraalCut( Rectangle )              \
                                              \
   ( ( Rectangle )->FLAGS & GRAAL_CUT         )

# define SetGraalCut( Rectangle )             \
                                              \
   ( ( Rectangle )->FLAGS |= GRAAL_CUT        )

# define ClearGraalCut( Rectangle )           \
                                              \
   ( ( Rectangle )->FLAGS &= ~GRAAL_CUT       )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Graal Windows                         |
|                                                             |
\------------------------------------------------------------*/

  typedef struct graalwinrec   

  {
    struct graalwinrec *NEXT; 
    rdsrec_list        *RECTAB[ GRAAL_MAX_REC ];

  } graalwinrec;

  typedef struct graalwin

  {
    struct graalwinrec **LAYERTAB;

  } graalwin;

  typedef struct graalrecwin 

  {
    struct graalrecwin *NEXT;
    graalwin           *WINDOW;
    
  } graalrecwin;

/*------------------------------------------------------------\
|                                                             |
|                         Graal Undo                          |
|                                                             |
\------------------------------------------------------------*/

  typedef struct graalundorec 

  {
    struct graalundorec *NEXT;
    rdsrec_list         *RECTANGLE;

  } graalundorec;

  typedef struct graalundo

  {
    struct graalundo *NEXT;
    graalundorec     *UNDO;
    
  } graalundo;

/*------------------------------------------------------------\
|                                                             |
|                         Graal Connector                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct graalconrec

  {
    struct graalconrec *NEXT;
    rdsrec_list        *RECTANGLE;

  } graalconrec;

/*------------------------------------------------------------\
|                                                             |
|                         Graal Select                        |
|                                                             |
\------------------------------------------------------------*/
 
  typedef struct graalselect
 
  {
    struct graalselect  *NEXT;
    rdsrec_list         *RECTANGLE;
 
  } graalselect ;

/*------------------------------------------------------------\
|                                                             |
|                         Graal Search                        |
|                                                             |
\------------------------------------------------------------*/

  typedef struct graalsearch

  {
    struct graalsearch *NEXT;
    rdsrec_list        *RECTANGLE;

  } graalsearch;

/*------------------------------------------------------------\
|                                                             |
|                         Graal Peek                          |
|                                                             |
\------------------------------------------------------------*/
 
  typedef struct graalpeek

  {
    struct graalpeek *NEXT;
    rdsrec_list      *RECTANGLE;

  } graalpeek;

/*------------------------------------------------------------\
|                                                             |
|                         X Rectangle                         |
|                                                             |
\------------------------------------------------------------*/

  typedef struct graalrec

  {
    rdsrec_list **PREVIOUS;
    void         *MBK;
    graalrecwin  *WINDOW;
    rdsrec_list  *EQUI;

  } graalrec;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern long          GraalBoundXmin;
  extern long          GraalBoundYmin;
  extern long          GraalBoundXmax;
  extern long          GraalBoundYmax; 
  extern char          GraalRecomputeBound;

  extern long          GraalWindowXmin;
  extern long          GraalWindowYmin;
  extern long          GraalWindowXmax;
  extern long          GraalWindowYmax;
  extern long          GraalWindowDx;
  extern long          GraalWindowDy;
  extern long          GraalWindowSide;
  extern long          GraalWindowSize;
  extern graalwin     *GraalWindowTable;

  extern rdsfig_list  *GraalFigureRds;
  extern rdsfig_list  *GraalFigureDruc;
  extern rdsrec_list  *GraalRectangleDruc;
  extern rdsrec_list  *GraalCurrentDruc;
  extern phfig_list   *GraalFigureMbk;

  extern graalundo    *GraalHeadRedo;
  extern graalundo    *GraalHeadUndo;
  extern rdsrec_list  *GraalHeadEqui;
  extern graalselect  *GraalHeadSelect;
  extern graalsearch  *GraalHeadSearch;
  extern graalpeek    *GraalHeadPeek;
  extern graalconrec  *GraalHeadConRec;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Translator MBK => RDS                   |
|                                                             |
\------------------------------------------------------------*/

  extern rdsfig_list *Graaldrucrdsfig();
  extern char         Graalsavephfig();
  extern phfig_list  *Graalgetphfig();
  extern phfig_list  *Graalrflattenphfig();
  extern phfig_list  *Graalflattenphfig();
  extern rdsfig_list *Graalgetrdsfig();
  extern rdsfig_list *Graalrflattenrdsfig();
  extern rdsfig_list *GraalAddFigure();
  extern rdsins_list *GraalAddInstance();
  extern rdsins_list *GraalFlatInstance();
  extern rdsins_list *GraalUnflatInstance();
  extern char         GraalRealFlatInstance();
  extern rdsrec_list *GraalAddSegment();
  extern rdsrec_list *GraalAddConnector();
  extern rdsrec_list *GraalAddReference();
  extern rdsrec_list *GraalAddVia();
  extern rdsrec_list *GraalAddAbox();
  extern void         GraalDelFigure();
  extern void         GraalDelInstance();
  extern void         GraalDelRectangle();
  extern char         GraalSaveFigure();

/*------------------------------------------------------------\
|                                                             |
|                      Undo Acces Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalAddUndo();
  extern void GraalDelUndo();
  extern void GraalAddUndoRec();
  extern void GraalInitializeUndo();

/*------------------------------------------------------------\
|                                                             |
|                   Flag Set & Clear Functions                |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalDeleteRectangle();
  extern void GraalUndeleteRectangle();
  extern void GraalSelectRectangle();
  extern void GraalUnselectRectangle();
  extern void GraalAcceptRectangle();
  extern void GraalRejectRectangle();

/*------------------------------------------------------------\
|                                                             |
|                      Select Acces Functions                 |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalAddSelect();
  extern void GraalDelSelect();
  extern void GraalPurgeSelect();

/*------------------------------------------------------------\
|                                                             |
|                      Search Acces Functions                 |
|                                                             |
\------------------------------------------------------------*/
 
  extern void GraalAddSearch();
  extern void GraalDelSearch();

/*------------------------------------------------------------\
|                                                             |
|                      Peek Acces Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalPeekInstance();
  extern void GraalAddPeek();
  extern void GraalDelPeek();

/*------------------------------------------------------------\
|                                                             |
|                      Equi Extract Functions                 |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalExtractEqui();
  extern void GraalDelEqui();

/*------------------------------------------------------------\
|                                                             |
|                      Druc Acces Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern rdsrec_list *GraalAddDruc();
  extern void         GraalDelDruc();
  extern void         GraalDelDrucFigure();

/*------------------------------------------------------------\
|                                                             |
|                          Bound Compute                      |
|                                                             |
\------------------------------------------------------------*/

  extern char GraalComputeBound();

/*------------------------------------------------------------\
|                                                             |
|                    Window Acces Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalInitializeWindowTable();

/*------------------------------------------------------------\
|                                                             |
|                    Error Message Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern char *GraalGetErrorMessage();
  extern void  GraalExitErrorMessage();

/*------------------------------------------------------------\
|                                                             |
|                    Informations Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern char *GraalGetInformations();

# endif
