#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rprparse.h                      |
|                                                             |
| Authors :               Jacomme  Ludovic                    |
|                         Picault Stephane     *4p*           |
|                                                             |
| Date    :                   27.06.95                        |
|                             15.07.98         *4p*           |
|                                                             |
\------------------------------------------------------------*/

# ifndef RPR_PARSE_H
# define RPR_PARSE_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define RPR_MAX_RDS_UNIT      100000L
# define RPR_EPSILON           ((double)0.001)
# define RPR_MAX_BUFFER        512
# define RPR_MAX_KEYWORD       155

# define MBK_MAX_WIRESETTING	30
# define RPR_SEPARATORS_STRING " \t\n"
# define RPR_COMMENT_CHAR      '#'

/*------------------------------------------------------------\
|                                                             |
|                        Keyword Constants                    |
|                                                             |
\------------------------------------------------------------*/

# define DEFINE_KEYWORD              "DEFINE"
# define TABLE_KEYWORD               "TABLE" 
# define END_TABLE_KEYWORD           "END"
# define END_RECORD_KEYWORD           NULL
# define LAMBDA_KEYWORD              "LAMBDA"
# define PHYSICAL_GRID_KEYWORD       "PHYSICAL_GRID"
# define SEGMENT_KEYWORD             "MBK_TO_RDS_SEGMENT"
# define VIA_KEYWORD                 "MBK_TO_RDS_VIA"
# define BIGVIA_HOLE_KEYWORD         "MBK_TO_RDS_BIGVIA_HOLE"
# define BIGVIA_METAL_KEYWORD        "MBK_TO_RDS_BIGVIA_METAL"
# define TURNVIA_KEYWORD             "MBK_TO_RDS_TURNVIA"
# define CONNECTOR_KEYWORD           "MBK_TO_RDS_CONNECTOR"
# define REFERENCE_KEYWORD           "MBK_TO_RDS_REFERENCE"
# define WIRESETTING_KEYWORD                "MBK_WIRESETTING"
# define CIF_LAYER_KEYWORD           "CIF_LAYER"
# define GDS_LAYER_KEYWORD           "GDS_LAYER" 
# define S2R_POST_TREAT_KEYWORD      "S2R_POST_TREAT"
# define S2R_OVERSIZE_KEYWORD        "S2R_OVERSIZE_DENOTCH"
# define S2R_RING_WIDTH_KEYWORD      "S2R_BLOC_RING_WIDTH"
# define S2R_LAYER_WIDTH_KEYWORD     "S2R_MINIMUM_LAYER_WIDTH"
# define LYNX_GRAPH_KEYWORD          "LYNX_GRAPH"
# define LYNX_CAPA_KEYWORD           "LYNX_CAPA"
# define LYNX_RESISTOR_KEYWORD       "LYNX_RESISTOR"
# define LYNX_TRANSISTOR_KEYWORD     "LYNX_TRANSISTOR"
# define LYNX_DIFFUSION_KEYWORD      "LYNX_DIFFUSION"
# define LYNX_BULK_IMPLICIT_KEYWORD  "LYNX_BULK_IMPLICIT"     /*4p*/

# define DRC_RULES_KEYWORD           "DRC_RULES"


/*------------------------------------------------------------\
|                                                             |
|                            Mask                             |
|                                                             |
\------------------------------------------------------------*/

# define RPR_LAMBDA_MASK              0x000001
# define RPR_PHYSICAL_GRID_MASK       0x000002
# define RPR_SEGMENT_MASK             0x000004
# define RPR_VIA_MASK                 0x000008
# define RPR_CONNECTOR_MASK           0x000010
# define RPR_REFERENCE_MASK           0x000020
# define RPR_CIF_LAYER_MASK           0x000040
# define RPR_GDS_LAYER_MASK           0x000080
# define RPR_S2R_POST_TREAT_MASK      0x000100
# define RPR_S2R_OVERSIZE_MASK        0x000200
# define RPR_S2R_RING_WIDTH_MASK      0x000400
# define RPR_S2R_LAYER_WIDTH_MASK     0x000800
# define RPR_LYNX_GRAPH_MASK          0x001000
# define RPR_LYNX_CAPA_MASK           0x002000
# define RPR_LYNX_TRANSISTOR_MASK     0x004000
# define RPR_LYNX_RESISTOR_MASK       0x008000
# define RPR_LYNX_DIFFUSION_MASK      0x010000
# define RPR_LYNX_BULK_IMPLICIT_MASK  0x020000
# define RPR_BIGVIA_HOLE_MASK         0x040000
# define RPR_BIGVIA_METAL_MASK        0x080000
# define RPR_TURNVIA_MASK             0x100000
# define RPR_WIRESETTING_MASK         0x200000

# define RPR_ALL_DEFINED_MASK         0x3FFFFF
# define RPR_ALL_REQUIRED_MASK        0x1FFFFF


/*------------------------------------------------------------\
|                                                             |
|                           Macros                            |
|                                                             |
\------------------------------------------------------------*/

# define RprEqual(a,b)                                       \
                                                             \
     ((((a)-RPR_EPSILON) <= (b)) && (((b)) <= (a)+RPR_EPSILON))

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct keyword
  {
    char *NAME;
    char  VALUE;

  } keyword;

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


# endif
