/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  DRUCUTIL.C                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/

#include <stdio.h>
#include "mph.h"
#include "mut.h"
#include "rds.h"
#include "rwi.h"
#include "rut.h"
#include "rtl.h"
#include "rfm.h"
#include "rpr.h"
#include "drucutil.h"

/*------------------------------------------------------------\
|                                                             |
|                  DRUCSTAT  variables                        |
|                                                             |
\------------------------------------------------------------*/
DrucTypeStructStat         DrucStructStat;

/*------------------------------------------------------------\
|                                                             |
|                  DRUCUTIL  variables                        |
|                                                             |
\------------------------------------------------------------*/

DrucTypeStructRuleComment *DrucRuleCommentList = NULL;
DrucTypeStructRdsLayer     DrucStructRdsLayer [ RDS_ALL_LAYER ];
long                       DrucCtmMax;

int                        DrucNombreInstructions;

char                      *DrucCompilErrorMessage [] =
                           {
  "\nDRUC Compiler terminated :OK  \n",
  "\nDRUC ERR: undefined user layer \n",
  "\nDRUC ERR: Undefined RDS LAYER \n",
  "\nDRUC ERR: Missing Out Name    \n",
  "\nDRUC ERR: Can't open Rules File \n",
  "\nDRUC ERR: Rules File not found \n",
  "\nDRUC ERR: Can't open Input Rules File  \n",
  "\nDRUC ERR: checksumm absent in compiled rules table \n",
  "\nDRUC ERR: Input Rules File does'nt contain Rules Table \n",
  "\nDRUC ERR: Rule Number         \n",
  "\nDRUC ERR: Invalid User Layer  \n",
  "\nDRUC ERR: memory allocation error\n",
  "\nDRUC ERR: checksumm error in compiled rules  \n",
  "\nDRUC ERR: too much rules in relation \n",
  "\nDRUC ERR: invalid arguments   \n",
  "\nDRUC ERR: il manque les commentaires dans le fichier technologique \n",
  "\nDRUC ERR: unable to close outfile rules\n",
  "\nDRUC ERR: Can't parse rules: I/O error  during reading Techno File\n",
  "\nDRUC ERR: bad read param in compiled rules \n",
  "\nDRUC ERR: Destination RDS layer not empty \n",
  "\nDRUC ERR: Destination layer empty \n",
  "\nDRUC ERR: Destination RDS layer not declared \n",
  "\n                              \n"
                            };
  static char *DrucNoComment = "No comment";

/*------------------------------------------------------------\
|                                                             |
|                  DRUCUTIL  Fonctions                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         DrucViewString                      |
|                                                             |
\------------------------------------------------------------*/

void DrucViewString ( String
                    )
   char *String;

{

  if ( DrucStructStat.FLAG_VERBOSE_MODE == DRUC_VERBOSE
     )
  {
    fprintf ( stdout,
              "%s",
              String
            );
    fflush  ( stdout
            );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucViewNumber                      |
|                                                             |
\------------------------------------------------------------*/

void DrucViewNumber ( Number
                    )
   long Number;

{

  if ( DrucStructStat.FLAG_VERBOSE_MODE == DRUC_VERBOSE
     )
  {
    fprintf ( stdout,
              "%3ld",
              Number
            );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  DrucComputeCheckSumm                       |
|                                                             |
| Calcule et met a jour le checksumm des instructions         |
| assembleur ecrites dans le fichier  DRUC_OUTPUT_NAME;       |

void DrucComputeCheckSumm ( Value
                          )
     int Value;

{

unsigned int TmpCheckSumm;

  TmpCheckSumm  = DrucCheckSumm << 1;
  TmpCheckSumm  = TmpCheckSumm & -1;
  TmpCheckSumm  = TmpCheckSumm ^ Value;
  DrucCheckSumm = TmpCheckSumm;
}
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                       DrucGetRuleComment                    |
|                                                             |
\------------------------------------------------------------*/

char *DrucGetRuleComment ( Rule
                         )
     long Rule;

{

DrucTypeStructRuleComment *Scan;

  for ( Scan  = DrucRuleCommentList;
        Scan != (DrucTypeStructRuleComment *)NULL;
        Scan  = Scan->NEXT
      )
  {
    if ( Scan->RULE == Rule
       )
    {
      return ( Scan->COMMENT
             );
    }
  }

  return ( DrucNoComment
         );
}

/*------------------------------------------------------------\
|                                                             |
|                      DrucViewRuleComment                    |
|                                                             |
\------------------------------------------------------------*/

void DrucViewRuleComment()

{

DrucTypeStructRuleComment *Scan;

  for ( Scan  = DrucRuleCommentList;
        Scan != (DrucTypeStructRuleComment *)NULL;
        Scan  = Scan->NEXT
      )
  {
    DrucViewNumber ( Scan->RULE
                   );
    DrucViewString ( " : "
                   );
    DrucViewString ( Scan->COMMENT
                   );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  DRUCSTAT  Fonctions                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         DrucInitStructStat                  |
|                                                             |
\------------------------------------------------------------*/

void DrucStatInit ( Flatten,
                    Verbose,
                    View_Rules,
                    FileType
                  )
     boolean        Flatten;
     boolean        Verbose;
     boolean        View_Rules;
     boolean        FileType;

 {

   DrucStructStat.ERROR_NUMBER          = 0;
   DrucStructStat.FLAG_FLATTEN_MODE     = Flatten;
   DrucStructStat.FLAG_VERBOSE_MODE     = Verbose;
   DrucStructStat.FLAG_VIEW_RULE_MODE   = View_Rules;
   DrucStructStat.FILE_ERROR            = FileType;
 }

