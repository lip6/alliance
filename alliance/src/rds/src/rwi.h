/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rwi110.h                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RWI_110_H
# define RWI_110_H

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define RWI_MAX_REC       8
# define RWI_MAX_LAYER     RDS_ABOX

/*------------------------------------------------------------\
|                                                             |
|                        Flags for window                     |
|                                                             |
\------------------------------------------------------------*/

# define RDS_ONE_WINDOW     (long)0x00010000

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define IsRdsOneWindow( Rectangle )                          \
                                                              \
   ( ( Rectangle )->FLAGS & RDS_ONE_WINDOW                    )

# define SetRdsOneWindow( Rectangle )                         \
                                                              \
   ( ( Rectangle )->FLAGS |= RDS_ONE_WINDOW                   )

# define ClearRdsOneWindow( Rectangle )                       \
                                                              \
   ( ( Rectangle )->FLAGS &= ~RDS_ONE_WINDOW                  )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Rds Window, Rectangle List                 |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdswinrec_list   

  {
    struct rdswinrec_list *NEXT; 
    rdsrec_list           *RECTAB[ RWI_MAX_REC ];

  } rdswinrec_list;

/*------------------------------------------------------------\
|                                                             |
|                      Rds Window list                        |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdswin_list

  {
    struct rdswinrec_list **LAYERTAB;

  } rdswin_list;

/*------------------------------------------------------------\
|                                                             |
|                  Rds rectangle, Window List                 |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdsrecwin_list 

  {
    struct rdsrecwin_list *NEXT;
    struct rdswin_list    *WINDOW;
    
  } rdsrecwin_list;

/*------------------------------------------------------------\
|                                                             |
|                         Rds Window                          |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdswindow

  {
    long          XMIN;
    long          YMIN;
    long          XMAX;
    long          YMAX;
    long          MASK;
    long          DX;
    long          DY;
    long          SIDE;
    long          SIZE;
    rdswin_list  *WINTAB;
    
  } rdswindow;

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
/*------------------------------------------------------------\
|                                                             |
|                       Alloc functions                       |
|                                                             |
\------------------------------------------------------------*/

  extern rdswinrec_list * allocrdswinrec __P(());
  extern rdsrecwin_list * allocrdsrecwin __P(());
  extern   rdswin_list * allocrdswin __P((unsigned long Number));
  extern   rdswin_list * allocrdswinlayer __P((rdswin_list *RdsWin));
  extern     rdswindow * allocrdswindow __P(());

/*------------------------------------------------------------\
|                                                             |
|                        Free functions                       |
|                                                             |
\------------------------------------------------------------*/

  extern           void  freerdswinrec __P((rdswinrec_list *WinRec));
  extern           void  freerdsrecwin __P((rdsrecwin_list *RecWin));
  extern           void  freerdswin __P((rdswin_list *Win));
  extern           void  freerdswinlayer __P((rdswin_list *Win));
  extern           void  freerdswindow __P((rdswindow *RdsWindow));
  
/*------------------------------------------------------------\
|                                                             |
|                     Get functions                           |
|                                                             |
\------------------------------------------------------------*/

  extern    chain_list * getrdswindow __P((long X, long Y, long Dx, long Dy, rdswindow *RdsWindow));
   
/*------------------------------------------------------------\
|                                                             |
|                 Build & Destroy functions                   |
|                                                             |
\------------------------------------------------------------*/
   
  extern           void  addrdsrecwindow __P((rdsrec_list *Rectangle, rdswindow *RdsWindow));
  extern           void  delrdsrecwindow __P((rdsrec_list *Rectangle, rdswindow *RdsWindow));
  extern     rdswindow * buildrdswindow __P((rdsfig_list *Figure));
  extern           void  destroyrdswindow __P((rdsfig_list *Figure, rdswindow *RdsWindow));
  extern           void  viewrdswindow __P((rdswindow *RdsWindow));

# endif
