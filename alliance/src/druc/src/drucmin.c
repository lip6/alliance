/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DRUCMIN.C                        |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   03/07/95                        |
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
#include "vmcasmld.h"
#include "vmcaract.h"
#include "defdefin.h"
#include "vmcrelat.h"
#include "drucmin.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Fonctions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrucMin                             |
|                                                             |
\------------------------------------------------------------*/

void DrucMin ( DrucFigureRds,
               DrucRdsWindow,
               DrucTabInstruction ,
               DrucNombreInstructions
             )
     rdsfig_list               *DrucFigureRds;
     rdswindow                 *DrucRdsWindow;
     DrucTypeStructInstruction *DrucTabInstruction;
     int                        DrucNombreInstructions;

{

int             InstructionCourante;

DrucViewString ( "instructionCourante : 000"
               );

/*\
 * cette sauvegarde est pour cacher des pb de compilation de graal
 * enlever quand graal link correctement
 * saverdsfig ( DrucFigureRds
 *            );
\*/

  for ( InstructionCourante = 0;
        InstructionCourante != DrucNombreInstructions;
        InstructionCourante = InstructionCourante + 1
      )
  {

    DrucViewString       ( "\b\b\b"
                         );
    DrucViewNumber       ( InstructionCourante
                         );

/*\
  DrucViewInstruction ( DrucTabInstruction [InstructionCourante]
                      );
\*/

    if ( MACRO_REGLE_COURANTE.LAYER_1 == DRUC_UNDEFINED_LAYER 
       )
    {
/*\
 * DrucViewString ("\ninstruction non verifiee\n)");
\*/
      continue;
    }

    switch ( MACRO_REGLE_COURANTE.INSTRUCTION 
           )
    {
    case  DRUC_ASM_BEGIN_REGLE :
      {
        switch ( MACRO_REGLE_COURANTE.OP_COMPOSE 
               )
        {
        case DRUC_INSTR_RELATION :
          {
            DrucRelation (   DrucFigureRds,
                             DrucRdsWindow,
                           & MACRO_REGLE_COURANTE
                         );
            break;
          }
        case DRUC_INSTR_CARACTERISE :
          {

            DrucCaracterise (   DrucFigureRds,
                              & MACRO_REGLE_COURANTE
                            );
            break;
          }
        }
        break;
      }
    case  DRUC_ASM_BEGIN_DEFINE :
      {
        DrucDefine (   DrucFigureRds,
                       DrucRdsWindow ,
                     & MACRO_REGLE_COURANTE
                   );
        break;
      }
    case  DRUC_ASM_BEGIN_UNDEFINE:
      {
        DrucUndefine (   DrucFigureRds,
                         DrucRdsWindow,
                       & MACRO_REGLE_COURANTE
                     );
        break;
      }
    }/* switch */
  }/* for InstructionCourante */

DrucViewString ( "\n"
               );

/*\
saverdsfig (DrucFigureRds);
\*/
}/* DrucVerif */
