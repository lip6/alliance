/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DRUCGRAL_TEST.C                  |
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
#include <string.h>
#include "mph.h"
#include "mut.h"
#include "rds.h"
#include "rut.h"
#include "rtl.h"
#include "rwi.h"
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
#include "drucmin.h"
#include "vmcunify.h"
#include "drucring.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

extern rdsfig_list   *DrucFigureRdsErreur;

/*------------------------------------------------------------\
|                                                             |
|                             Main                            |
|                                                             |
\------------------------------------------------------------*/
void main (argc , argv )
     int   argc;
     char *argv[];
{
phfig_list                *DrucFigureMbk;
rdsfig_list               *DrucFigureRds;
char                      *Name;
char                      *DrucFigureName;
char                      *DrucErrorName           = (char *)NULL;

/*\
 *  lecture des parametres d'entree
\*/
  if ( argc != 2 
     )
  {
    printf         ( "Usage : %s figure_name (without extention)\n",argv [0]
                   );
    exit           ( 1
                   );
  }

/*\
 *  affichage de la banniere standard
\*/
  alliancebanner     ( "DRCGraL", 
                        VERSION, 
                       "Design Rule Checker", 
                       "1993", 
                        ALLIANCE_VERSION
                     );
  DrucStatInit       ( DRUC_FLATTEN,
                       DRUC_SILENT,
                       DRUC_SILENT,
                       DRUC_NO_ERROR_FILE
                     );

  Name               = argv [ 1 ];

  DrucFigureName     = namealloc ( Name 
                                );

  Name               = malloc ( strlen ( DrucFigureName )    +
                                strlen ( DRUC_ERROR_EXTEND) +
                                1
                              );

  strcpy             ( Name ,
                       DrucFigureName
                     );
  strcat             ( Name,
                       DRUC_ERROR_EXTEND
                     );

  DrucErrorName      = namealloc ( Name 
                                 );
  
  free               ( Name
                     );
  mbkenv             (
                     );
  rdsenv             (
                     );
  loadrdsparam       (
                     );
  DrucFigureMbk      = getphfig ( DrucFigureName,
                                'A'
                                );

  DrucViewString     ( "Flatten DRC on: "
                     );
  DrucViewString     ( DrucFigureName
                     );
  DrucViewString     ( "\n"
                     );
  rflattenphfig      ( DrucFigureMbk,
                       YES,
                       NO
                     );
  DrucFigureRds      = figmbkrds ( DrucFigureMbk,
                               MACRO_PLUS_TAILLE,
                               0
                             );

  DrucViewString      ( "Delete MBK figure : "
                      );
  DrucViewString      ( DrucFigureName
                      );
  DrucViewString      ( "\n"
                      );

  delphfig            ( DrucFigureName
                      );

  DrucViewString      ( "Load Flatten Rules : "
                      );
  DrucViewString      ( RDS_TECHNO_NAME
                      );
  DrucViewString      ( "\n"
                      );

  DrucOpenErrorFile   ( DrucErrorName,
                        "w"
                      );
  DrucFigureRdsErreur = drucrdsfig ( DrucFigureRds,
                                     DrucErrorName
                                   );

  DrucStatInit     ( DRUC_FLATTEN,
                     DRUC_VERBOSE,
                     DRUC_NO_ERROR_FILE
                   );
  viewrdsfig       ( DrucFigureRdsErreur
                   );
  DrucViewString   ( "\n"
                   );
  DrucViewRuleComment (
                      );
  DrucViewString   ( "\nnombre d'erreurs rencontrees = valeur de sortie: "
                   );
  DrucViewNumber   ( DrucStructStat.ERROR_NUMBER
                   );
  DrucViewString   ( "\n"
                   );
  exit             ( DrucStructStat.ERROR_NUMBER 
                   );
}



