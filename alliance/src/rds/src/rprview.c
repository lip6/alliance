/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rprview.c                       |
|                                                             |
| Authors :               Jacomme  Ludovic                    |
|                         Picault Stephane       *4p*         |
|                                                             |
| Date    :                   27.06.95                        |
|                             15.07.98           *4p*         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <math.h>
# include <ctype.h>
# include <stdlib.h>
# include <string.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rwi.h"
# include "rut.h"

# include "rprerror.h"
# include "rprview.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Rds view Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void viewrdsparam()

{
  int   Layer;
  int   Index;

  fprintf( stdout, "\n  DEFINE PHYSICAL_GRID %ld", RDS_PHYSICAL_GRID );
  fprintf( stdout, "\n  DEFINE LAMBDA %ld"       , RDS_LAMBDA );
  fprintf( stdout, "\n# DEFINE RDS_UNIT %ld"     , RDS_UNIT );

  fprintf( stdout, "\n\nTABLE MBK_TO_RDS_SEGMENT\n" );

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", MBK_LAYER_NAME[ Layer ] );

    Index = 0;

    while ( GET_SEGMENT_LAYER( Layer, Index ) != RDS_SEGMENT_EMPTY )
    {
      fprintf( stdout, "  %s %s %2ld %2ld %2ld %s", 
               RDS_LAYER_NAME[ GET_SEGMENT_LAYER( Layer, Index ) ],
               RDS_TRANS_NAME[ GET_SEGMENT_TRANS( Layer, Index ) ],
               GET_SEGMENT_DLR   ( Layer, Index ),
               GET_SEGMENT_DWR   ( Layer, Index ),
               GET_SEGMENT_OFFSET( Layer, Index ),
               RDS_USE_NAME[ GET_SEGMENT_USE( Layer, Index ) ] );

      Index = Index + 1;
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE MBK_TO_RDS_CONNECTOR\n" );

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", MBK_LAYER_NAME[ Layer ] );

    if ( GET_CONNECTOR_LAYER( Layer ) != RDS_CONNECTOR_EMPTY )
    {
      fprintf( stdout, " %s %2ld %2ld", 
               RDS_LAYER_NAME[ GET_CONNECTOR_LAYER( Layer ) ],
               GET_CONNECTOR_DER( Layer ),
               GET_CONNECTOR_DWR( Layer ) );
    }
  } 

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE MBK_TO_RDS_REFERENCE\n" );

  for ( Layer = 0; Layer < MBK_MAX_REFERENCE; Layer++ )
  {
    if ( Layer == 0 ) fprintf( stdout, "\nREF_REF" );
    else              fprintf( stdout, "\nREF_CON" );

    Index = 0;

    while ( GET_REFERENCE_LAYER( Layer, Index ) != RDS_REFERENCE_EMPTY )
    {
      fprintf( stdout, " %s %2ld", 
               RDS_LAYER_NAME[ GET_REFERENCE_LAYER( Layer, Index ) ],
               GET_REFERENCE_SIZE( Layer, Index ) );

      Index = Index + 1;
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE MBK_TO_RDS_VIA\n" );

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    Index = 0;

    fprintf( stdout, "\n%s", MBK_VIA_NAME[ Layer ] );

    while ( GET_VIA_LAYER( Layer, Index ) != RDS_VIA_EMPTY )
    {
      fprintf( stdout, "  %s %2ld %s", 
               RDS_LAYER_NAME[ GET_VIA_LAYER( Layer, Index ) ],
               GET_VIA_SIZE( Layer, Index ),
               RDS_USE_NAME[ GET_VIA_USE( Layer, Index ) ] );

      Index = Index + 1;
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE MBK_TO_RDS_BIGVIA_HOLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    Index = 0;

    fprintf( stdout, "\n%s", MBK_VIA_NAME[ Layer ] );

    while ( GET_BIGVIA_HOLE_LAYER( Layer, Index ) != RDS_BIGVIA_HOLE_EMPTY )
    {
      fprintf( stdout, "  %s %2ld %2ld %s", 
               RDS_LAYER_NAME[ GET_BIGVIA_HOLE_LAYER( Layer, Index ) ],
               GET_BIGVIA_HOLE_SIDE( Layer, Index ),
               GET_BIGVIA_HOLE_STEP( Layer, Index ),
               RDS_USE_NAME[ GET_BIGVIA_HOLE_USE( Layer, Index ) ] );

      Index = Index + 1;
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE MBK_TO_RDS_BIGVIA_METAL\n" );

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    Index = 0;

    fprintf( stdout, "\n%s", MBK_VIA_NAME[ Layer ] );

    while ( GET_BIGVIA_METAL_LAYER( Layer, Index ) != RDS_BIGVIA_METAL_EMPTY )
    {
      fprintf( stdout, "  %s %2ld %2ld %s", 
               RDS_LAYER_NAME[ GET_BIGVIA_METAL_LAYER( Layer, Index ) ],
               GET_BIGVIA_METAL_DWR( Layer, Index ),
               GET_BIGVIA_METAL_OVERLAP( Layer, Index ),
               RDS_USE_NAME[ GET_BIGVIA_METAL_USE( Layer, Index ) ] );

      Index = Index + 1;
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE MBK_TO_RDS_TURNVIA\n" );

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    Index = 0;

    fprintf( stdout, "\n%s", MBK_VIA_NAME[ Layer ] );

    while ( GET_TURNVIA_LAYER( Layer, Index ) != RDS_TURNVIA_EMPTY )
    {
      fprintf( stdout, "  %s %2ld %s", 
               RDS_LAYER_NAME[ GET_TURNVIA_LAYER( Layer, Index ) ],
               GET_TURNVIA_DWR( Layer, Index ),
               RDS_USE_NAME[ GET_TURNVIA_USE( Layer, Index ) ] );

      Index = Index + 1;
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE CIF_LAYER\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    if ( GET_CIF_LAYER ( Layer ) != RDS_CIF_LAYER_EMPTY )
    {
      fprintf( stdout, " %s", GET_CIF_LAYER ( Layer ) );
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE GDS_LAYER\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    if ( GET_GDS_LAYER( Layer ) != RDS_GDS_LAYER_EMPTY )
    {
      fprintf( stdout, " %d", GET_GDS_LAYER ( Layer ) );
    }
  }
 
  fprintf( stdout, "\n\nEND" );
 
  fprintf( stdout, "\n\nTABLE S2R_OVERSIZE_DENOTCH\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    if ( GET_S2R_OVERSIZE( Layer ) != RDS_S2R_OVERSIZE_EMPTY )
    {
      fprintf( stdout, " %ld", GET_S2R_OVERSIZE ( Layer ) );
    }
  }
 
  fprintf( stdout, "\n\nEND" );
 
  fprintf( stdout, "\n\nTABLE S2R_BLOC_RING_WIDTH\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    if ( GET_S2R_RING_WIDTH( Layer ) != RDS_S2R_RING_WIDTH_EMPTY )
    {
      fprintf( stdout, " %ld", GET_S2R_RING_WIDTH ( Layer ) );
    }
  }
 
  fprintf( stdout, "\n\nEND" );
 
  fprintf( stdout, "\n\nTABLE S2R_MINIMUM_LAYER_WIDTH\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    if ( GET_S2R_LAYER_WIDTH( Layer ) != RDS_S2R_LAYER_WIDTH_EMPTY )
    {
      fprintf( stdout, " %ld", GET_S2R_LAYER_WIDTH ( Layer ) );
    }
  }
 
  fprintf( stdout, "\n\nEND" );
 
  fprintf( stdout, "\n\nTABLE S2R_POST_TREAT\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    if ( GET_S2R_POST_Y_OR_N( Layer ) != RDS_S2R_POST_TREAT_EMPTY )
    {
      fprintf( stdout, " %d", GET_S2R_POST_Y_OR_N ( Layer ) );

      if ( GET_S2R_POST_COMPLEMENTARY( Layer ) != RDS_S2R_POST_TREAT_EMPTY )
      {
        fprintf( stdout, " %s", 
                 RDS_LAYER_NAME[ GET_S2R_POST_COMPLEMENTARY( Layer ) ] );
      }
    }
  }
 
  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE LYNX_GRAPH\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    Index = 0;

    while ( GET_LYNX_GRAPH_LAYER( Layer, Index ) != RDS_LYNX_GRAPH_EMPTY )
    {
      fprintf( stdout, "  %s ", 
               RDS_LAYER_NAME[ GET_LYNX_GRAPH_LAYER( Layer, Index ) ] );

      Index = Index + 1;
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE LYNX_DIFFUSION\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    Index = 0;

    while ( GET_LYNX_DIFFUSION_LAYER( Layer, Index ) != RDS_LYNX_DIFFUSION_EMPTY )
    {
      fprintf( stdout, "  %s ",
               RDS_LAYER_NAME[ GET_LYNX_DIFFUSION_LAYER( Layer, Index ) ] );
      fprintf( stdout, "  %d ",
               GET_LYNX_DIFFUSION_Y_OR_N( Layer, Index ) );

      Index = Index + 1;
    }
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE LYNX_CAPA\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    fprintf( stdout, "  %g ", GET_LYNX_SURFACE_CAPA_LAYER( Layer ) * RDS_UNIT2  );
    fprintf( stdout, "  %g ", GET_LYNX_PERIMETER_CAPA_LAYER( Layer ) * RDS_UNIT );
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE LYNX_RESISTOR\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    fprintf( stdout, "  %g ", GET_LYNX_RESISTOR_LAYER( Layer ) );
  }

  fprintf( stdout, "\n\nEND" );

  fprintf( stdout, "\n\nTABLE LYNX_TRANSISTOR\n" );

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", MBK_LAYER_NAME[ Layer ] );

    if ( GET_LYNX_TRANSISTOR_TYPE( Layer ) != RDS_LYNX_TRANSISTOR_EMPTY )
    {
      fprintf( stdout, "  %s",
               MBK_LAYER_NAME[ (int)GET_LYNX_TRANSISTOR_TYPE( Layer ) ] );
      fprintf( stdout, "  %s",
               MBK_VIA_NAME[ (int)GET_LYNX_TRANSISTOR_CX( Layer ) ] );
      fprintf( stdout, "  %s",
               RDS_LAYER_NAME[ (int)GET_LYNX_TRANSISTOR_GATE_LAYER( Layer ) ] );
      fprintf( stdout, "  %s",
               RDS_LAYER_NAME[ (int)GET_LYNX_TRANSISTOR_DIFF_LAYER( Layer ) ] );

      if ( GET_LYNX_TRANSISTOR_ACTIV_LAYER( Layer ) != RDS_LYNX_TRANSISTOR_EMPTY )
      {
        fprintf( stdout, "  %s",
                 RDS_LAYER_NAME[ (int)GET_LYNX_TRANSISTOR_ACTIV_LAYER( Layer ) ] );
      }
      else
      {
        fprintf( stdout, "  NULL" );
      }
/*4p*=============================================================================*4p*/
      if ( GET_LYNX_TRANSISTOR_BULK_LAYER( Layer ) != RDS_LYNX_TRANSISTOR_EMPTY )
      {
        fprintf( stdout, "  %s",
                  RDS_LAYER_NAME[(int)GET_LYNX_TRANSISTOR_BULK_LAYER( Layer ) ] );
      }
      else
      {
        fprintf( stdout, "  NULL" );
      }
/*4p*=============================================================================*4p*/
    }
  }

  fprintf( stdout, "\n\nEND\n\n" );

/*4p*=============================================================================*4p*/
 
  fprintf( stdout, "\n\nTABLE LYNX_BULK_IMPLICIT\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\n%s", RDS_LAYER_NAME[ Layer ] );

    if ( GET_LYNX_BULK_IMPLICIT ( Layer ) != RDS_LYNX_BULK_IMPLICIT_EMPTY )
    {
      fprintf( stdout, " %d", GET_LYNX_BULK_IMPLICIT ( Layer ) );
    }
  }

  fprintf( stdout, "\n\nEND" );
/*4p*=============================================================================*4p*/

}
