/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DRUCGRAAL.C                      |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "mph.h"
#include "mut.h"
#include "rds.h"
#include "rwi.h"
#include "rut.h"
#include "rtl.h"
#include "rfm.h"
#include "rpr.h"

#include "drucutil.h"
#include "vmctools.h"
#include "vmcerror.h"
#include "druchier.h"
#include "vmcasmld.h"
#include "vmcaract.h"
#include "defdefin.h"
#include "vmcrelat.h"
#include "vmcunify.h"
#include "drucmin.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

extern rdsfig_list            *DrucFigureRdsErreur;
extern DrucTypeStructRdsLayer  DrucStructRdsLayer[];

/*------------------------------------------------------------\
|                                                             |
|                          Fonctions                          |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         drucgeterror                        |
|                                                             |
\------------------------------------------------------------*/

char *drucgeterror( Rule 
                  )
      long  Rule;

{

  return ( DrucGetRuleComment ( Rule
                              )
         );
}

/*------------------------------------------------------------\
|                                                             |
|                             drucrdsfig                      |
|                                                             |
\------------------------------------------------------------*/
rdsfig_list *drucrdsfig ( DrucFigureRds,
                          DrucFigureRdsErrorName
                        )
    rdsfig_list *DrucFigureRds;
    char        *DrucFigureRdsErrorName;

{

rdswindow                 *DrucRdsWindow;
static DrucTypeStructInstruction *DrucTabInstruction = ( DrucTypeStructInstruction *)NULL;
int                        LayerCourant;
int                        IndexLayer;

  DrucStatInit          ( DRUC_FLATTEN,
                          DRUC_SILENT,
                          DRUC_SILENT,
                          DRUC_NO_ERROR_FILE
                        );

  DrucFigureRdsErreur    = addrdsfig ( DrucFigureRdsErrorName,
                                       MACRO_PLUS_TAILLE
                                     );

/*
** Modified by L.Jacomme the 14/10/95
*/
  if ( DrucTabInstruction == ( DrucTypeStructInstruction * )NULL )
  {
    DrucInitInstruction ( & DrucTabInstruction
                        );
  }
/*
** End
*/

  DrucRdsWindow          = buildrdswindow ( DrucFigureRds
                                          );

  for ( IndexLayer = 0;
        IndexLayer < RDS_MAX_LAYER -1;
        IndexLayer ++
      )
  {
    LayerCourant = DrucStructRdsLayer [ IndexLayer ].RDS_INDEX ;

    if ( LayerCourant != DRUC_UNDEFINED_LAYER
       )
    {
      if ( DrucFigureRds->LAYERTAB [ LayerCourant ] != ( rdsrec_list * ) NULL
         )
      {
        DrucComputeEquiLayer ( DrucFigureRds ,
                               LayerCourant
                             );
      }
    }
  }

  destroyrdswindow ( DrucFigureRds,
                     DrucRdsWindow
                   );
/*\
 * le parametre DrucRdsWindow etait ooublie dans l'appel de DrucUnify
 * thnaks to Xavier
\*/
  DrucUnify        ( DrucFigureRds,
                     DrucRdsWindow
                   );

  DrucRdsWindow    = buildrdswindow ( DrucFigureRds
                                    );
  for ( IndexLayer = 0;
        IndexLayer < RDS_MAX_LAYER -1;
        IndexLayer ++
      )
  {
    LayerCourant = DrucStructRdsLayer [ IndexLayer ].RDS_INDEX ;

    if ( LayerCourant != DRUC_UNDEFINED_LAYER
       )
    {
      if ( DrucFigureRds->LAYERTAB [ LayerCourant ] != ( rdsrec_list * ) NULL
         )
      {
        DrucComputeEquiLayer ( DrucFigureRds ,
                               LayerCourant
                             );
      }
    }
  }

  DrucMin          ( DrucFigureRds,
                     DrucRdsWindow,
                     DrucTabInstruction ,
                     DrucNombreInstructions
                   );
  destroyrdswindow ( DrucFigureRds,
                     DrucRdsWindow
                   );
  delrdsfig        ( DrucFigureRds->NAME
                   );
  return           ( DrucFigureRdsErreur
                   );
}


