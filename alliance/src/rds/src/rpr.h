/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rpr214.h                        |
|                                                             |
| Authors :               Jacomme  Ludovic                    |
|                         Picault Stephane          *4p*      |
|                                                             |
| Date    :                   27.06.95                        |
|                             15.07.98              *4p*      |
|                                                             |
\------------------------------------------------------------*/

# ifndef RPR_214_H
# define RPR_214_H

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
/*------------------------------------------------------------\
|                                                             |
|                          Lynx Define                        |
|                                                             |
\------------------------------------------------------------*/

# define RDS_LYNX_GRAPH_FIELD       25
# define RDS_LYNX_GRAPH_EMPTY       (unsigned char)255
# define RDS_LYNX_CAPA_FIELD        2
# define RDS_LYNX_CAPA_EMPTY        0.0
# define RDS_LYNX_RESISTOR_FIELD    1
# define RDS_LYNX_RESISTOR_EMPTY    0.0
# define RDS_LYNX_TRANSISTOR_FIELD  6 
# define RDS_LYNX_TRANSISTOR_EMPTY  (unsigned char)255
# define RDS_LYNX_DIFFUSION_FIELD   25
# define RDS_LYNX_DIFFUSION_EMPTY   (unsigned char)255

# define RDS_IMPLICIT  1                                 /*4p*/
# define RDS_EXPLICIT  (unsigned char)255                /*4p*/

# define RDS_LYNX_BULK_IMPLICIT_FIELD   1                /*4p*/
# define RDS_LYNX_BULK_IMPLICIT_EMPTY   RDS_EXPLICIT     /*4p*/

/*------------------------------------------------------------\
|                                                             |
|                          S2R Define                         |
|                                                             |
\------------------------------------------------------------*/

# define S2R_NOTREAT                (unsigned char)255
# define S2R_TREAT                  1

# define RDS_S2R_POST_TREAT_FIELD   2
# define RDS_S2R_OVERSIZE_FIELD     1
# define RDS_S2R_RING_WIDTH_FIELD   1
# define RDS_S2R_LAYER_WIDTH_FIELD  1

# define RDS_S2R_POST_TREAT_EMPTY   S2R_NOTREAT
# define RDS_S2R_OVERSIZE_EMPTY     (long)0
# define RDS_S2R_RING_WIDTH_EMPTY   (long)0
# define RDS_S2R_LAYER_WIDTH_EMPTY  (long)0

/*------------------------------------------------------------\
|                                                             |
|                          Mbk Define                         |
|                                                             |
\------------------------------------------------------------*/

# define RDS_TRANS_VW         (unsigned char)0
# define RDS_TRANS_LCW        (unsigned char)1
# define RDS_TRANS_RCW        (unsigned char)2

# define RDS_MAX_TRANS        ( RDS_TRANS_RCW + 1 )

# define MBK_REF_REF          0
# define MBK_REF_CON          1

# define MBK_MAX_LAYER        ( LAST_LAYER   + 1 )
# define MBK_MAX_VIA          ( LAST_CONTACT + 1 )
# define MBK_MAX_REFERENCE    2
# define MBK_MAX_SYMMETRY     ( SY_RP        + 1 )
# define MBK_MAX_ORIENT       4

# define RDS_SEGMENT_FIELD    120
# define RDS_VIA_FIELD        60
# define RDS_CONNECTOR_FIELD  3
# define RDS_REFERENCE_FIELD  60

# define RDS_SEGMENT_EMPTY    (long)-1
# define RDS_VIA_EMPTY        (long)-1
# define RDS_CONNECTOR_EMPTY  (long)-1
# define RDS_REFERENCE_EMPTY  (long)-1

# define RDS_USE_ALL          (unsigned char)0
# define RDS_USE_EXTRACT      (unsigned char)1
# define RDS_USE_DRC          (unsigned char)2

# define RDS_MAX_USE          ( RDS_USE_DRC + 1 )

# define RDS_BIGVIA_HOLE_FIELD    80
# define RDS_BIGVIA_METAL_FIELD   80
# define RDS_TURNVIA_FIELD        60

# define RDS_BIGVIA_HOLE_EMPTY    (long)-1
# define RDS_BIGVIA_METAL_EMPTY   (long)-1
# define RDS_TURNVIA_EMPTY        (long)-1

/*------------------------------------------------------------\
|                                                             |
|                         Parser Define                       |
|                                                             |
\------------------------------------------------------------*/

# define RDS_CIF_LAYER_FIELD  1
# define RDS_GDS_LAYER_FIELD  2

# define RDS_CIF_LAYER_EMPTY  (char *)NULL
# define RDS_GDS_LAYER_EMPTY  (unsigned char)255

/*------------------------------------------------------------\
|                                                             |
|                          Lynx Macros                        |
|                                                             |
\------------------------------------------------------------*/

# define GET_LYNX_GRAPH_LAYER( Layer, Index )                 \
                                                              \
  ( ( Index >= RDS_LYNX_GRAPH_FIELD ) ?                       \
    ( RDS_LYNX_GRAPH_EMPTY          ) :                       \
    ( RDS_LYNX_GRAPH_TABLE [ (int)(Layer) ][ ( Index ) ] ) )

# define GET_LYNX_SURFACE_CAPA_LAYER( Layer )                 \
                                                              \
  ( RDS_LYNX_CAPA_TABLE [ (int)(Layer) ][ 0 ]                    )

# define GET_LYNX_PERIMETER_CAPA_LAYER( Layer )               \
                                                              \
  ( RDS_LYNX_CAPA_TABLE [ (int)(Layer) ][ 1 ]                    )

# define GET_LYNX_DIFFUSION_LAYER( Layer, Index )             \
                                                              \
  ( ( (int)(Index) * 2 >= RDS_LYNX_DIFFUSION_FIELD ) ?             \
    ( RDS_LYNX_DIFFUSION_EMPTY ) :                            \
    ( RDS_LYNX_DIFFUSION_TABLE [ (int)(Layer) ][ (int)(Index)*2 + 0 ] ) )

# define GET_LYNX_DIFFUSION_Y_OR_N( Layer, Index )            \
                                                              \
  ( ( (int)(Index) * 2 >= RDS_LYNX_DIFFUSION_FIELD ) ?             \
    ( RDS_LYNX_DIFFUSION_EMPTY ) :                            \
    ( RDS_LYNX_DIFFUSION_TABLE [ (int)(Layer) ][ (int)(Index)*2 + 1 ] ) )

# define GET_LYNX_RESISTOR_LAYER( Layer )                     \
                                                              \
  ( RDS_LYNX_RESISTOR_TABLE [ (int)(Layer) ][ 0 ]                )

# define GET_LYNX_TRANSISTOR_TYPE( Layer )                    \
                                                              \
  ( RDS_LYNX_TRANSISTOR_TABLE [ (int)(Layer) ][ 0 ]              )

# define GET_LYNX_TRANSISTOR_CX( Layer )                      \
                                                              \
  ( RDS_LYNX_TRANSISTOR_TABLE [ (int)(Layer) ][ 1 ]              )

# define GET_LYNX_TRANSISTOR_GATE_LAYER( Layer )              \
                                                              \
  ( RDS_LYNX_TRANSISTOR_TABLE [ (int)(Layer) ][ 2 ]              )

# define GET_LYNX_TRANSISTOR_DIFF_LAYER( Layer )              \
                                                              \
  ( RDS_LYNX_TRANSISTOR_TABLE [ (int)(Layer) ][ 3 ]              )

# define GET_LYNX_TRANSISTOR_ACTIV_LAYER( Layer )             \
                                                              \
  ( RDS_LYNX_TRANSISTOR_TABLE [ (int)(Layer) ][ 4 ]              )

/*4p*/
# define GET_LYNX_TRANSISTOR_BULK_LAYER( Layer )              \
                                                              \
  ( RDS_LYNX_TRANSISTOR_TABLE [ (int)(Layer) ][ 5 ] ) 

/*4p*/
# define GET_LYNX_BULK_IMPLICIT( Layer )                      \
                                                              \
  ( RDS_LYNX_BULK_IMPLICIT_TABLE [ (int)(Layer) ][ 0 ] )     


/*------------------------------------------------------------\
|                                                             |
|                          S2R Macros                         |
|                                                             |
\------------------------------------------------------------*/
 
# define GET_S2R_POST_Y_OR_N( Layer )                         \
                                                              \
        ( RDS_S2R_POST_TREAT_TABLE [ (int)(Layer) ][ 0 ] )

# define GET_S2R_POST_COMPLEMENTARY( Layer )                  \
                                                              \
        ( RDS_S2R_POST_TREAT_TABLE [ (int)(Layer) ][ 1 ] )

# define GET_S2R_OVERSIZE( Layer )                            \
                                                              \
        ( RDS_S2R_OVERSIZE_TABLE   [ (int)(Layer) ][ 0 ] )

# define GET_S2R_RING_WIDTH( Layer )                          \
                                                              \
        ( RDS_S2R_RING_WIDTH_TABLE [ (int)(Layer) ][ 0 ] )

# define GET_S2R_LAYER_WIDTH( Layer )                         \
                                                              \
        ( RDS_S2R_LAYER_WIDTH_TABLE [ (int)(Layer) ][ 0 ] )

/*------------------------------------------------------------\
|                                                             |
|                          Mbk Macros                         |
|                                                             |
\------------------------------------------------------------*/

# define GET_SEGMENT_LAYER( Layer, Index )                    \
                                                              \
      ( ( (int)(Index) * 6 >= RDS_SEGMENT_FIELD ) ?            \
        ( RDS_SEGMENT_EMPTY                ) :                \
        ( RDS_SEGMENT_TABLE [ (int)(Layer) ][ (int)(Index) * 6 + 0 ] )  )

# define GET_SEGMENT_TRANS( Layer, Index )                    \
                                                              \
      ( ( (int)(Index) * 6 >= RDS_SEGMENT_FIELD ) ?            \
        ( RDS_SEGMENT_EMPTY              )   :                \
        ( RDS_SEGMENT_TABLE [ (int)(Layer) ][ (int)(Index) * 6 + 1 ] )  )

# define GET_SEGMENT_DLR( Layer, Index )                      \
                                                              \
      ( ( (int)(Index) * 6 >= RDS_SEGMENT_FIELD ) ?                \
        ( RDS_SEGMENT_EMPTY                ) :                \
        ( RDS_SEGMENT_TABLE [ (int)(Layer) ][ (int)(Index) * 6 + 2 ] )  )

# define GET_SEGMENT_DWR( Layer, Index )                      \
                                                              \
      ( ( (int)(Index) * 6 >= RDS_SEGMENT_FIELD ) ?                \
        ( RDS_SEGMENT_EMPTY                ) :                \
        ( RDS_SEGMENT_TABLE [ (int)(Layer) ][ (int)(Index) * 6 + 3 ] )  )

# define GET_SEGMENT_OFFSET( Layer, Index )                   \
                                                              \
      ( ( (int)(Index) * 6 >= RDS_SEGMENT_FIELD ) ?                \
        ( RDS_SEGMENT_EMPTY                ) :                \
        ( RDS_SEGMENT_TABLE [ (int)(Layer) ][ (int)(Index) * 6 + 4 ] )  )

# define GET_SEGMENT_USE( Layer, Index )                      \
                                                              \
      ( ( (int)(Index) * 6 >= RDS_SEGMENT_FIELD ) ?                \
        ( RDS_SEGMENT_EMPTY                ) :                \
        ( RDS_SEGMENT_TABLE [ (int)(Layer) ][ (int)(Index) * 6 + 5 ] )  )

# define GET_CONNECTOR_LAYER( Layer )                         \
                                                              \
        ( RDS_CONNECTOR_TABLE [ (int)(Layer) ][ 0 ] )

# define GET_CONNECTOR_DER( Layer )                           \
                                                              \
        ( RDS_CONNECTOR_TABLE [ (int)(Layer) ][ 1 ] )

# define GET_CONNECTOR_DWR( Layer )                           \
                                                              \
        ( RDS_CONNECTOR_TABLE [ (int)(Layer) ][ 2 ] )

# define GET_VIA_LAYER( Layer, Index )                        \
                                                              \
      ( ( (int)(Index) * 3 >= RDS_VIA_FIELD ) ?                    \
        ( RDS_VIA_EMPTY                ) :                    \
        ( RDS_VIA_TABLE [ (int)(Layer) ][ (int)(Index) * 3 + 0 ] )      )

# define GET_VIA_SIZE( Layer, Index )                         \
                                                              \
      ( ( (int)(Index) * 3 >= RDS_VIA_FIELD ) ?                    \
        ( RDS_VIA_EMPTY                ) :                    \
        ( RDS_VIA_TABLE [ (int)(Layer) ][ (int)(Index) * 3 + 1 ] )      )

# define GET_VIA_USE( Layer, Index )                          \
                                                              \
      ( ( (int)(Index) * 3 >= RDS_VIA_FIELD ) ?                    \
        ( RDS_VIA_EMPTY                    ) :                \
        ( RDS_VIA_TABLE [ (int)(Layer) ][ (int)(Index) * 3 + 2 ] )      )

# define GET_REFERENCE_LAYER( Layer, Index )                  \
                                                              \
      ( ( (int)(Index) * 2 >= RDS_REFERENCE_FIELD ) ?              \
        ( RDS_REFERENCE_EMPTY          ) :                    \
        ( RDS_REFERENCE_TABLE [ (int)(Layer) ][ (int)(Index) * 2 + 0 ] ))

# define GET_REFERENCE_SIZE( Layer, Index )                   \
                                                              \
      ( ( (int)(Index) * 2 >= RDS_REFERENCE_FIELD ) ?              \
        ( RDS_REFERENCE_EMPTY          ) :                    \
        ( RDS_REFERENCE_TABLE [ (int)(Layer) ][ (int)(Index) * 2 + 1 ] ))

/*------------------------------------------------------------\
|                                                             |
|                          Big Via Macros                     |
|                                                             |
\------------------------------------------------------------*/

# define GET_BIGVIA_HOLE_LAYER( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 4 >= RDS_BIGVIA_HOLE_FIELD ) ?  \
        ( RDS_BIGVIA_HOLE_EMPTY ) :                      \
        ( RDS_BIGVIA_HOLE_TABLE [ (int)(Layer) ][ (int)(Index) * 4 + 0 ] ) )

# define GET_BIGVIA_HOLE_SIDE( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 4 >= RDS_BIGVIA_HOLE_FIELD ) ?  \
        ( RDS_BIGVIA_HOLE_EMPTY ) :                      \
        ( RDS_BIGVIA_HOLE_TABLE [ (int)(Layer) ][ (int)(Index) * 4 + 1 ] ) )

# define GET_BIGVIA_HOLE_STEP( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 4 >= RDS_BIGVIA_HOLE_FIELD ) ?  \
        ( RDS_BIGVIA_HOLE_EMPTY ) :                      \
        ( RDS_BIGVIA_HOLE_TABLE [ (int)(Layer) ][ (int)(Index) * 4 + 2 ] ) )

# define GET_BIGVIA_HOLE_USE( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 4 >= RDS_BIGVIA_HOLE_FIELD ) ?  \
        ( RDS_BIGVIA_HOLE_EMPTY ) :                      \
        ( RDS_BIGVIA_HOLE_TABLE [ (int)(Layer) ][ (int)(Index) * 4 + 3 ] ) )

# define GET_BIGVIA_METAL_LAYER( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 4 >= RDS_BIGVIA_METAL_FIELD ) ?  \
        ( RDS_BIGVIA_METAL_EMPTY ) :                      \
        ( RDS_BIGVIA_METAL_TABLE [ (int)(Layer) ][ (int)(Index) * 4 + 0 ] ) )

# define GET_BIGVIA_METAL_DWR( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 4 >= RDS_BIGVIA_METAL_FIELD ) ?  \
        ( RDS_BIGVIA_METAL_EMPTY ) :                      \
        ( RDS_BIGVIA_METAL_TABLE [ (int)(Layer) ][ (int)(Index) * 4 + 1 ] ) )

# define GET_BIGVIA_METAL_OVERLAP( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 4 >= RDS_BIGVIA_METAL_FIELD ) ?  \
        ( RDS_BIGVIA_METAL_EMPTY ) :                      \
        ( RDS_BIGVIA_METAL_TABLE [ (int)(Layer) ][ (int)(Index) * 4 + 2 ] ) )

# define GET_BIGVIA_METAL_USE( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 4 >= RDS_BIGVIA_METAL_FIELD ) ?  \
        ( RDS_BIGVIA_METAL_EMPTY ) :                      \
        ( RDS_BIGVIA_METAL_TABLE [ (int)(Layer) ][ (int)(Index) * 4 + 3 ] ) )

/*------------------------------------------------------------\
|                                                             |
|                          Turn Via Macros                    |
|                                                             |
\------------------------------------------------------------*/

# define GET_TURNVIA_LAYER( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 3 >= RDS_TURNVIA_FIELD ) ?  \
        ( RDS_TURNVIA_EMPTY ) :                      \
        ( RDS_TURNVIA_TABLE [ (int)(Layer) ][ (int)(Index) * 3 + 0 ] ) )

# define GET_TURNVIA_DWR( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 3 >= RDS_TURNVIA_FIELD ) ?  \
        ( RDS_TURNVIA_EMPTY ) :                      \
        ( RDS_TURNVIA_TABLE [ (int)(Layer) ][ (int)(Index) * 3 + 1 ] ) )

# define GET_TURNVIA_USE( Layer, Index )           \
                                                         \
      ( ( (int)(Index) * 3 >= RDS_TURNVIA_FIELD ) ?  \
        ( RDS_TURNVIA_EMPTY ) :                      \
        ( RDS_TURNVIA_TABLE [ (int)(Layer) ][ (int)(Index) * 3 + 2 ] ) )

/*------------------------------------------------------------\
|                                                             |
|                         Parser Macros                       |
|                                                             |
\------------------------------------------------------------*/

# define GET_CIF_LAYER( Layer )                               \
                                                              \
        ( RDS_CIF_LAYER_TABLE [ (int)(Layer) ][ 0 ] )

# define GET_GDS_LAYER( Layer )                               \
                                                              \
        ( RDS_GDS_LAYER_TABLE [ (int)(Layer) ][ 0 ] )

# define GET_GDS_CONNECTOR_LAYER( Layer )                               \
                                                              \
        ( RDS_GDS_LAYER_TABLE [ (int)(Layer) ][ 1 ] )

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

 extern long   RDS_PHYSICAL_GRID;
 extern long   RDS_LAMBDA;
 extern long   RDS_LAMBDA2;
 extern long   RDS_UNIT;
 extern long   RDS_UNIT2;

 extern unsigned char RDS_S2R_POST_TREAT_TABLE  [ RDS_ALL_LAYER ][ RDS_S2R_POST_TREAT_FIELD  ];
 extern long          RDS_S2R_OVERSIZE_TABLE    [ RDS_ALL_LAYER ][ RDS_S2R_OVERSIZE_FIELD    ];
 extern long          RDS_S2R_RING_WIDTH_TABLE  [ RDS_ALL_LAYER ][ RDS_S2R_RING_WIDTH_FIELD  ];
 extern long          RDS_S2R_LAYER_WIDTH_TABLE [ RDS_ALL_LAYER ][ RDS_S2R_LAYER_WIDTH_FIELD ];

 extern long   RDS_SEGMENT_TABLE   [ MBK_MAX_LAYER     ][ RDS_SEGMENT_FIELD   ];
 extern long   RDS_VIA_TABLE       [ MBK_MAX_VIA       ][ RDS_VIA_FIELD       ];
 extern long   RDS_CONNECTOR_TABLE [ MBK_MAX_LAYER     ][ RDS_CONNECTOR_FIELD ];
 extern long   RDS_REFERENCE_TABLE [ MBK_MAX_REFERENCE ][ RDS_REFERENCE_FIELD ];

 extern long   RDS_BIGVIA_HOLE_TABLE[ MBK_MAX_VIA ][ RDS_BIGVIA_HOLE_FIELD ];
 extern long   RDS_BIGVIA_METAL_TABLE[ MBK_MAX_VIA ][ RDS_BIGVIA_METAL_FIELD ];
 extern long   RDS_TURNVIA_TABLE[ MBK_MAX_VIA ][ RDS_TURNVIA_FIELD ];

 extern unsigned char  RDS_GDS_LAYER_TABLE [ RDS_ALL_LAYER ][ RDS_GDS_LAYER_FIELD ];
 extern char          *RDS_CIF_LAYER_TABLE [ RDS_ALL_LAYER ][ RDS_CIF_LAYER_FIELD ];

 extern unsigned char  RDS_LYNX_BULK_IMPLICIT_TABLE [ RDS_ALL_LAYER ][ RDS_LYNX_BULK_IMPLICIT_FIELD ];

 extern float  RDS_LYNX_CAPA_TABLE [ RDS_ALL_LAYER ][ RDS_LYNX_CAPA_FIELD ];
 extern float  RDS_LYNX_RESISTOR_TABLE [ RDS_ALL_LAYER ][ RDS_LYNX_RESISTOR_FIELD ];

 extern unsigned char RDS_LYNX_GRAPH_TABLE [ RDS_ALL_LAYER ][ RDS_LYNX_GRAPH_FIELD ];
 extern unsigned char RDS_LYNX_TRANSISTOR_TABLE [ MBK_MAX_LAYER ][ RDS_LYNX_TRANSISTOR_FIELD ];
 extern unsigned char RDS_LYNX_DIFFUSION_TABLE [ RDS_ALL_LAYER ][ RDS_LYNX_DIFFUSION_FIELD ];

 extern char   MBK_LAYER_NAME[ MBK_MAX_LAYER ][ 12 ];
 extern char   MBK_VIA_NAME  [ MBK_MAX_VIA   ][ 12 ];
 extern char   RDS_TRANS_NAME [ RDS_MAX_TRANS ][ 4 ];
 extern char   RDS_USE_NAME [ RDS_MAX_USE ][ 4 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern           void  loadrdsparam __P(());
  extern           void  viewrdsparam __P(());

# endif 
