/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for SCL         |
|                                                       |
|  File    :            sch105.h                        |
|                                                       |
|  Date    :            04.03.98                        |
|                                                       |
\------------------------------------------------------*/

# ifndef SCL_105_H
# define SCL_105_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                Box & Connectors Size / Step           |
|                                                       |
\------------------------------------------------------*/

# define SCP_BOX_MIN_DX              8
                                     
# define SCP_BOX_CON_BASE_Y          2
# define SCP_BOX_CON_STEP_Y          4
# define SCP_BOX_CON_X               0
                                     
# define SCP_BOX_STEP_Y              5
# define SCP_CON_STEP_Y              3
# define SCP_CELL_MIN_DX             2

# define SCP_PRIMARY_CON_DX          3
# define SCP_PRIMARY_CON_DY          2
# define SCP_CON_DY                  2
                                     
# define SCP_CLUSTER_ROUTE_STEP_X    2

/*------------------------------------------------------\
|                                                       |
|                      From Type                        |
|                                                       |
\------------------------------------------------------*/

# define SCH_FROM_LOFIG     0x0001
# define SCH_FROM_BEFIG     0x0002

/*------------------------------------------------------\
|                                                       |
|                       P Type Id                       |
|                                                       |
\------------------------------------------------------*/

# define SCH_NET_TYPE       0x90000020
# define SCH_BOX_TYPE       0x90000200

/*------------------------------------------------------\
|                                                       |
|                        Box Mask                       |
|                                                       |
\------------------------------------------------------*/

# define SCH_BOX_TAGED         0x0001 
# define SCH_BOX_CLUSTERED     0x0002
# define SCH_BOX_CLUSTER       0x0004
# define SCH_BOX_CLUSTER_NET   0x0008

# define SCH_BOX_TRANSPARENCE  0x0010
# define SCH_BOX_RETAGED       0x0020 
# define SCH_BOX_PLACED        0x0040

/*------------------------------------------------------\
|                                                       |
|                        Con Mask                       |
|                                                       |
\------------------------------------------------------*/

# define SCH_CON_IN         0x0001
# define SCH_CON_OUT        0x0002
# define SCH_CON_VEC        0x0004
# define SCH_CON_EXTERNAL   0x0008
# define SCH_CON_INTERNAL   0x0010
# define SCH_CON_VEC_FIRST  0x0020
# define SCH_CON_VEC_LAST   0x0040

# define SCH_CON_TAGED      0x0100
# define SCH_CON_PLACED     0x0200

# define SCH_NET_CANAL      0x0F

# define SCH_NET_MAXDIR     0x20
# define SCH_NET_MINDIR     0x10

# define SCH_ROOT_CON_BOX   0x0001
# define SCH_ROOT_CON_FIG   0x0002


# define SCH_SOURCE_LOSIG   1 
# define SCH_SOURCE_LOINS   2
# define SCH_SOURCE_LOCON   3
# define SCH_SOURCE_LOFIG   4
# define SCH_SOURCE_LOTRS   5

# define SCH_SOURCE_BEPOR   6
# define SCH_SOURCE_BEAUX   7
# define SCH_SOURCE_BEREG   8
# define SCH_SOURCE_BEBUX   9
# define SCH_SOURCE_BEBUS  10
# define SCH_SOURCE_BEOUT  11
# define SCH_SOURCE_BEFIG  12

/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                         Box                           |
|                                                       |
\------------------------------------------------------*/

# define IsSchBoxClustered( X )     ( ( X )->FLAGS & SCH_BOX_CLUSTERED )
# define SetSchBoxClustered( X )    ( ( X )->FLAGS |= SCH_BOX_CLUSTERED )
# define ClearSchBoxClustered( X )  ( ( X )->FLAGS &= ~SCH_BOX_CLUSTERED )

# define IsSchBoxTaged( X )         ( ( X )->FLAGS & SCH_BOX_TAGED )
# define SetSchBoxTaged( X )        ( ( X )->FLAGS |= SCH_BOX_TAGED )
# define ClearSchBoxTaged( X )      ( ( X )->FLAGS &= ~SCH_BOX_TAGED )

# define IsSchBoxReTaged( X )       ( ( X )->FLAGS & SCH_BOX_RETAGED )
# define SetSchBoxReTaged( X )      ( ( X )->FLAGS |= SCH_BOX_RETAGED )
# define ClearSchBoxReTaged( X )    ( ( X )->FLAGS &= ~SCH_BOX_RETAGED )

# define IsSchBoxCluster( X )       ( ( X )->FLAGS & SCH_BOX_CLUSTER )
# define SetSchBoxCluster( X )      ( ( X )->FLAGS |= SCH_BOX_CLUSTER )
# define ClearSchBoxCluster( X )    ( ( X )->FLAGS &= ~SCH_BOX_CLUSTER )

# define IsSchBoxTransparence( X )  ( ( X )->FLAGS & SCH_BOX_TRANSPARENCE )
# define SetSchBoxTransparence( X ) ( ( X )->FLAGS |= SCH_BOX_TRANSPARENCE )
# define ClearSchBoxTransparence( X ) ( ( X )->FLAGS &= ~SCH_BOX_TRANSPARENCE )

# define IsSchBoxPlaced( X )        ( ( X )->FLAGS & SCH_BOX_PLACED )
# define SetSchBoxPlaced( X )       ( ( X )->FLAGS |= SCH_BOX_PLACED )
# define ClearSchBoxPlaced( X )     ( ( X )->FLAGS &= ~SCH_BOX_PLACED )

# define IsSchBoxClusterNet( X )    ( ( X )->FLAGS & SCH_BOX_CLUSTER_NET )
# define SetSchBoxClusterNet( X )   ( ( X )->FLAGS |= SCH_BOX_CLUSTER_NET )
# define ClearSchBoxClusterNet( X ) ( ( X )->FLAGS &= ~SCH_BOX_CLUSTER_NET )

/*------------------------------------------------------\
|                                                       |
|                       Connector                       |
|                                                       |
\------------------------------------------------------*/

# define IsSchConIn( X )          ( ( X )->TYPE & SCH_CON_IN )
# define SetSchConIn( X )         ( ( X )->TYPE |= SCH_CON_IN )
# define ClearSchConIn( X )       ( ( X )->TYPE &= ~SCH_CON_IN )

# define IsSchConOut( X )         ( ( X )->TYPE & SCH_CON_OUT )
# define SetSchConOut( X )        ( ( X )->TYPE |= SCH_CON_OUT )
# define ClearSchConOut( X )       ( ( X )->TYPE &= ~SCH_CON_OUT )

# define IsSchConVec( X )         ( ( X )->TYPE & SCH_CON_VEC )
# define SetSchConVec( X )        ( ( X )->TYPE |= SCH_CON_VEC )
# define ClearSchConVec( X )      ( ( X )->TYPE &= ~SCH_CON_VEC )

# define IsSchConVecFirst( X )    ( ( X )->TYPE & SCH_CON_VEC_FIRST )
# define SetSchConVecFirst( X )   ( ( X )->TYPE |= SCH_CON_VEC_FIRST )
# define ClearSchConVecFirst( X ) ( ( X )->TYPE &= ~SCH_CON_VEC_FIRST )

# define IsSchConVecLast( X )    ( ( X )->TYPE & SCH_CON_VEC_LAST )
# define SetSchConVecLast( X )   ( ( X )->TYPE |= SCH_CON_VEC_LAST )
# define ClearSchConVecLast( X ) ( ( X )->TYPE &= ~SCH_CON_VEC_LAST )

# define IsSchConExternal( X )    ( ( X )->TYPE & SCH_CON_EXTERNAL )
# define SetSchConExternal( X )   ( ( X )->TYPE |= SCH_CON_EXTERNAL )
# define ClearSchConExternal( X ) ( ( X )->TYPE &= ~SCH_CON_EXTERNAL )

# define IsSchConInternal( X )    ( ( X )->TYPE & SCH_CON_INTERNAL )
# define SetSchConInternal( X )   ( ( X )->TYPE |= SCH_CON_INTERNAL )
# define ClearSchConInternal( X ) ( ( X )->TYPE &= ~SCH_CON_INTERNAL )

# define IsSchConPlaced( X )      ( ( X )->FLAGS & SCH_CON_PLACED )
# define SetSchConPlaced( X )     ( ( X )->FLAGS |= SCH_CON_PLACED )
# define ClearSchConPlaced( X )   ( ( X )->FLAGS &= ~SCH_CON_PLACED )

# define IsSchConTaged( X )       ( ( X )->FLAGS & SCH_CON_TAGED )
# define SetSchConTaged( X )      ( ( X )->FLAGS |= SCH_CON_TAGED )
# define ClearSchConTaged( X )    ( ( X )->FLAGS &= ~SCH_CON_TAGED )

/*------------------------------------------------------\
|                                                       |
|                           Net                         |
|                                                       |
\------------------------------------------------------*/

# define IsSchNetCanal( X )        ( ( X )->FLAGS & SCH_NET_CANAL )
# define GetSchNetDirType( X )     ( ( X )->FLAGS & (SCH_NET_MAXDIR|SCH_NET_MINDIR) )
# define SetSchNetMaxOutput( X )     ( ( X )->FLAGS |= SCH_NET_MAXDIR )
# define SetSchNetMinOutput( X )     ( ( X )->FLAGS |= SCH_NET_MINDIR )

# define SetSchNetCanal( X )       ( ( X )->FLAGS |= SCH_NET_CANAL )

# define ClearSchNetCanal( X )     ( ( X )->FLAGS &= ~SCH_NET_CANAL )
# define ClearSchNetDir( X )       ( ( X )->FLAGS &= ~(SCH_NET_MINDIR|SCH_NET_MAXDIR) )

/*------------------------------------------------------\
|                                                       |
|                       Add Connector                   |
|                                                       |
\------------------------------------------------------*/

# define addschfigconin( F, N )  (addschfigcon( (F), (N), SCH_CON_IN ))
# define addschfigconout( F, N ) (addschfigcon( (F), (N), SCH_CON_OUT ))

# define addschboxconin( F, B, N )  (addschboxcon( (F), (B), (N), SCH_CON_IN ))
# define addschboxconout( F, B, N ) (addschboxcon( (F), (B), (N), SCH_CON_OUT ))

/*------------------------------------------------------\
|                                                       |
|                      Structures                       |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                        Wires                          |
|                                                       |
\------------------------------------------------------*/

  typedef struct schwir_list
  {
    struct schwir_list  *NEXT;
    struct schnet_list  *NET;
    long                 X;
    long                 Y;
    long                 DX;
    long                 DY;
    long                 FLAGS;
    void                *USER;

  } schwir_list;

/*------------------------------------------------------\
|                                                       |
|                        Nets (Signals)                 |
|                                                       |
\------------------------------------------------------*/

  typedef struct schnet_list 
  {
    struct schnet_list  *NEXT;
    chain_list          *CON_NET;
    struct schwir_list  *WIRE;
    void                *SOURCE;
    unsigned char        SOURCE_TYPE;
    long                 FLAGS;
    long                 NUMBER_IN;
    long                 NUMBER_OUT;
    long                 CANAL;
    long                 YMIN;
    long                 YMAX;
    void                *USER;
    
  } schnet_list;

/*------------------------------------------------------\
|                                                       |
|                      Connectors                       |
|                                                       |
\------------------------------------------------------*/

  typedef struct schcon_list 
  {
    struct schcon_list  *NEXT;
    char                *NAME;
    unsigned short       TYPE;
    unsigned char        DIR;
    unsigned char        ROOT_TYPE;
    void                *ROOT;
    schnet_list         *NET;
    void                *SOURCE;
    unsigned char        SOURCE_TYPE;
    long                 X_REL;
    long                 Y_REL;
    long                 FLAGS;
    void                *USER;
    
  } schcon_list;


/*------------------------------------------------------\
|                                                       |
|                        Box (Instance)                 |
|                                                       |
\------------------------------------------------------*/

  typedef struct schbox_list 
  {
    struct schbox_list  *NEXT;
    char                *NAME;
    struct schcon_list  *CON_IN;
    long                 NUMBER_IN;
    struct schcon_list  *CON_OUT;
    long                 NUMBER_OUT;
    void                *SOURCE;
    unsigned char        SOURCE_TYPE;    /* type de la source */ 
    long                 X;
    long                 Y;
    long                 DX;
    long                 DY;
    long                 FLAGS;
    void                *USER;
   
  } schbox_list;

/*------------------------------------------------------\
|                                                       |
|                        Figure                         |
|                                                       |
\------------------------------------------------------*/

  typedef struct schfig_list 
  {
    struct schfig_list  *NEXT;
    char                *NAME;    
    struct schcon_list  *CON_IN;
    long                 NUMBER_IN;
    struct schcon_list  *CON_OUT;
    long                 NUMBER_OUT;
    schbox_list         *BOX;
    schnet_list         *NET;
    void                *SOURCE;
    unsigned char        SOURCE_TYPE;
    long                 X;
    long                 Y;
    long                 FLAGS;
    void                *USER;
   
  } schfig_list;

/*------------------------------------------------------\
|                                                       |
|                     Global Variables                  |
|                                                       |
\------------------------------------------------------*/

 extern schfig_list   *HEAD_SCHFIG;
 extern char          *SCH_IN;

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                       Env Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern void schenv();

/*------------------------------------------------------\
|                                                       |
|                    Alloc Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern schfig_list    *allocschfig();
  extern schcon_list    *allocschcon();
  extern schbox_list    *allocschbox();
  extern schwir_list    *allocschwir();
  extern schnet_list    *allocschnet();


/*------------------------------------------------------\
|                                                       |
|                      Free Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern void  freeschfig();
  extern void  freeschcon();
  extern void  freeschbox();
  extern void  freeschwir();
  extern void  freeschnet();


/*------------------------------------------------------\
|                                                       |
|                      Add Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern schnet_list    *addschnetcon();
  extern schfig_list    *addschfig();
  extern schcon_list    *addschfigcon();
  extern schcon_list    *addschboxcon();
  extern schbox_list    *addschbox();
  extern schwir_list    *addschwir();
  extern schnet_list    *addschnet();
  

/*------------------------------------------------------\
|                                                       |
|                      Del Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern int   delschfig();

/*------------------------------------------------------\
|                                                       |
|                     View Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern void viewschfig();
  extern void viewschbox();
  extern void viewschcon();
  extern void viewschwir();
  extern void viewschnet();

/*------------------------------------------------------\
|                                                       |
|                       Get Functions                   |
|                                                       |
\------------------------------------------------------*/

 extern char *getschvectorname();

/*------------------------------------------------------\
|                                                       |
|                   Translate Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern schfig_list *lofig2schfig();
  extern schfig_list *befig2schfig();

/*------------------------------------------------------\
|                                                       |
|                  Place / Route Functions              |
|                                                       |
\------------------------------------------------------*/

  extern void placerouteschfig();

# endif 
