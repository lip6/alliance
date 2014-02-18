/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  VMCASMLD.C                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mph.h"
#include "mut.h"
#include "rds.h"
#include "rwi.h"
#include "rut.h"
#include "rtl.h"
#include "rfm.h"
#include "rpr.h"

#include "drucompi_y.h"
#include "drucutil.h"
#include "vmctools.h"
#include "drucompi.h"
#include "druchier.h"
#include "vmcasmld.h"

/*------------------------------------------------------------\
|                                                             |
|                  DRUCVMC   variables                        |
|                                                             |
\------------------------------------------------------------*/

extern long                       DrucCtmMax;
extern DrucTypeStructRuleComment *DrucRuleCommentList;


static char DrucFileBuffer [ DRUC_MAX_FILE_BUFFER + 1 ];

/*------------------------------------------------------------\
|                                                             |
|                  DRUVMC    Fonctions                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                  DrucComputeCtm                             |
|                                                             |
\------------------------------------------------------------*/

void DrucComputeCtm ( DrucInstructionCourante
                    )

DrucTypeStructInstruction *DrucInstructionCourante;

{

long                      Value;
int                       IndexMesure = 0;

  while (  DrucInstructionCourante->RELATION  [ IndexMesure ] 
          != DRUC_UNDEFINED_RELATION 
        )
  {
    if (   ( DrucInstructionCourante->MESURE  [ IndexMesure ]
            == DRUC_MESURE_AXIALE )                  
        ||
           (  DrucInstructionCourante->MESURE  [ IndexMesure ]
            == DRUC_MESURE_GEODESIQUE )
       )
    {
      Value      = DrucInstructionCourante->VALEUR [ IndexMesure ];

      DrucCtmMax = ( DrucCtmMax > Value ?
                     DrucCtmMax :
                     Value
                   );

      DrucStructRdsLayer [ DrucInstructionCourante->LAYER_1 ].CTM  = 
      ( Value > DrucStructRdsLayer [ DrucInstructionCourante->LAYER_1 ].CTM ?
        Value : 
        DrucStructRdsLayer [ DrucInstructionCourante->LAYER_1 ].CTM 
      );

      DrucStructRdsLayer [ DrucInstructionCourante->LAYER_2 ].CTM  = 
      ( Value > DrucStructRdsLayer [ DrucInstructionCourante->LAYER_2 ].CTM ?
        Value : 
        DrucStructRdsLayer [ DrucInstructionCourante->LAYER_2 ].CTM 
      );
    }

    IndexMesure ++;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   DrucReadRulesComment                      |
|                                                             |
\------------------------------------------------------------*/

void DrucReadRulesComment (
                          )

{

long                       Rule;
DrucTypeStructRuleComment *Comment;
char                      *Flag;
char                      *TmpBuffer;

  while ( 1 
        )
  {
    Flag = fgets( DrucFileBuffer, DRUC_MAX_FILE_BUFFER, vrd_y_in              );

    if ( Flag != (char *)NULL 
       )
    {
      if ( strstr( DrucFileBuffer, DRUC_DRC_END_COMMENT ) != (char *)NULL 
         )
      {
        return;
      }

      if (  sscanf ( DrucFileBuffer, "%22ld\n", &Rule ) == EOF
         )
      {
        DRUC_EXIT( DRUC_ERROR_BAD_COMMENT );
      }
      TmpBuffer = strchr ( DrucFileBuffer, ' '
                         );
      TmpBuffer ++;
      Comment = (DrucTypeStructRuleComment *)
                      rdsalloc( sizeof( DrucTypeStructRuleComment ), 
                      1
                );

      Comment->NEXT       = DrucRuleCommentList;
      DrucRuleCommentList = Comment;
      Comment->RULE       = Rule;
      Comment->COMMENT    = (char *)rdsalloc( strlen( DrucFileBuffer ) + 1, 0 );

      strcpy ( Comment->COMMENT, TmpBuffer
             );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  DrucInitInstruction                        |
|                                                             |
| Ouvre le fichier contenant le code assembleur de DRUC       |
| Lit et charge le code dans la pile d'execution              |
| Initialise la structure DrucStructRdsLayer                  |
| Initialise la structure DrucStructDefine                    |
| Initialise la structure DrucStructRegle                     |
\------------------------------------------------------------*/

int DrucInitInstruction ( DrucTabInstruction
                        )

DrucTypeStructInstruction **DrucTabInstruction;

{

int                        TmpIndexMesure;
int                        IndexLayer;
boolean                    FlagBegin;
char                      *FlagBuffer;
DrucTypeStructInstruction *TmpStructInstruction;

  DrucInitStructLayers (
                       );

  vrd_y_in              = fopen ( RDS_TECHNO_NAME ,
                              "r"
                            );

  if ( vrd_y_in              == (FILE *)NULL 
     )
  {
    DRUC_EXIT ( DRUC_ERROR_NO_RULE_FILE 
              );
  }

  FlagBegin  = DRUC_FALSE;
  FlagBuffer = (char *)NULL;

  while ( FlagBegin != DRUC_TRUE 
        )
  {
    FlagBuffer = fgets ( DrucFileBuffer, 
                         DRUC_MAX_FILE_BUFFER, 
                         vrd_y_in              
                       );

    if ( FlagBuffer != (char *)NULL 
       )
    {

      if ( strstr ( DrucFileBuffer, DRUC_DRC_RULES ) != (char *)NULL 
         )
      {
        FlagBegin = DRUC_TRUE;
      }
    }
    else
    {
    DRUC_EXIT ( DRUC_ERROR_NO_RULE_IN_FILE 
              );
    }
  }

  DrucStructInstruction = ( DrucTypeStructInstruction *)NULL;

  vrd_y_parse();
  DrucViewString ("\n");

  if ( DrucStructStat.FLAG_FLATTEN_MODE  == DRUC_HIERARCHIE
     )
  {
    DrucAddABoxRules ( & DrucStructInstruction,
                       & DrucNombreInstructions
                     );
  }

  *DrucTabInstruction =
      ( DrucTypeStructInstruction *)malloc
             (sizeof (  DrucTypeStructInstruction ) * DrucNombreInstructions );

  for ( IndexLayer  = DrucNombreInstructions - 1;
        IndexLayer >= 0;
        IndexLayer --
      )
  {
    (*DrucTabInstruction) [ IndexLayer ].NEXT        = (DrucTypeStructInstruction *)NULL;
    (*DrucTabInstruction) [ IndexLayer ].INSTRUCTION = DrucStructInstruction->INSTRUCTION;
    (*DrucTabInstruction) [ IndexLayer ].LAYER_DEST  = DrucStructInstruction->LAYER_DEST;
    (*DrucTabInstruction) [ IndexLayer ].OP_COMPOSE  = DrucStructInstruction->OP_COMPOSE;
    (*DrucTabInstruction) [ IndexLayer ].LAYER_1     = DrucStructInstruction->LAYER_1;
    (*DrucTabInstruction) [ IndexLayer ].LAYER_2     = DrucStructInstruction->LAYER_2;
    (*DrucTabInstruction) [ IndexLayer ].OP_UNAIRE   = DrucStructInstruction->OP_UNAIRE ;
    (*DrucTabInstruction) [ IndexLayer ].DEF_VALEUR  = DrucStructInstruction->DEF_VALEUR;

    for ( TmpIndexMesure = 0;
          TmpIndexMesure < DRUC_MAX_MESURES;
          TmpIndexMesure ++
        )
    {
      (*DrucTabInstruction) [ IndexLayer ].REGLE_NUM  [ TmpIndexMesure ] =
                               DrucStructInstruction->REGLE_NUM [ TmpIndexMesure ] ;

      (*DrucTabInstruction) [ IndexLayer ].RELATION   [ TmpIndexMesure ] =
                               DrucStructInstruction->RELATION [ TmpIndexMesure ] ;

      (*DrucTabInstruction) [ IndexLayer ].MESURE     [ TmpIndexMesure ] =
                               DrucStructInstruction->MESURE [ TmpIndexMesure ] ;

      (*DrucTabInstruction) [ IndexLayer ].OP_COMPARE [ TmpIndexMesure ] =
                               DrucStructInstruction->OP_COMPARE [ TmpIndexMesure ] ;

      (*DrucTabInstruction) [ IndexLayer ].VALEUR     [ TmpIndexMesure ] =
                               DrucStructInstruction->VALEUR [ TmpIndexMesure ] ;
    }
    TmpStructInstruction  = DrucStructInstruction;
    DrucStructInstruction = DrucStructInstruction->NEXT;

    free ( TmpStructInstruction 
         );
  }

  for ( IndexLayer  = 0;
        IndexLayer  < DrucNombreInstructions -1;
        IndexLayer ++
      )
  {
    (*DrucTabInstruction)[ IndexLayer ].NEXT = (*DrucTabInstruction) +1;
  }

/*\
 *   DrucNombreInstructions = DrucReadAsm ( & DrucTabInstruction
 *                                        );
\*/

  FlagBegin  = DRUC_FALSE;
  FlagBuffer = (char *)NULL;

  while ( FlagBegin != DRUC_TRUE 
        )
  {
    FlagBuffer = fgets ( DrucFileBuffer, 
                         DRUC_MAX_FILE_BUFFER, 
                         vrd_y_in              
                       );

    if ( FlagBuffer != (char *)NULL 
       )
    {
      if ( strstr ( DrucFileBuffer, DRUC_DRC_COMMENT ) != (char *)NULL 
         )
      {
        FlagBegin = DRUC_TRUE;
      }
    }
    else
    {
      DRUC_EXIT ( DRUC_ERROR_COMMENT_FILE 
                );
    }
  }

  DrucReadRulesComment (
                       );
  fclose               ( vrd_y_in              
                       );
  return               ( DrucNombreInstructions 
                       );
}
