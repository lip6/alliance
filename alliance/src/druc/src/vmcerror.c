/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  VMCERROR.C                       |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   25/01/94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Modifs                              |
| 25/01/94: ligne 407 rajoutee (elle etait oubliee)           |
|           char        *DrucFigureRdsErrorName;              |
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
#include "vmcasmld.h"
#include "vmcaract.h"
#include "defdefin.h"
#include "vmcmesur.h"
#include "vmcrelat.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list   *DrucFigureRdsErreur;
FILE          *DrucErrorFile;


/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                          DrucOpenErrorFile                  |
|                                                             |
\------------------------------------------------------------*/

void DrucOpenErrorFile ( DrucErrorFileName,
                         Mode
                       )
char *DrucErrorFileName;
char *Mode;

{
  if ( DrucStructStat.FILE_ERROR == DRUC_ERROR_FILE
     )
  {
    DrucErrorFile = fopen ( DrucErrorFileName, 
                            Mode
                          );
  }
}
  
/*------------------------------------------------------------\
|                                                             |
|                          DrucCloseErrorFile                 |
|                                                             |
\------------------------------------------------------------*/

void DrucCloseErrorFile (
                        ) 
{

  if ( DrucStructStat.FILE_ERROR == DRUC_ERROR_FILE
     )
  {
    fclose ( DrucErrorFile
           );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucErreurOutFileRectangle         |
|                                                             |
\------------------------------------------------------------*/

void DrucErreurOutFileRectangle ( RectangleErreur
                                )
     rdsrec_list *RectangleErreur;

{
  if ( DrucStructStat.FILE_ERROR == DRUC_ERROR_FILE
     )
  {
    fprintf ( DrucErrorFile , 
              "\tX = %.1f  \t"   , 
              RectangleErreur->X / (float)RDS_LAMBDA );
    fprintf ( DrucErrorFile ,     
              "Y = %.1f  \t"   , 
              RectangleErreur->Y / (float)RDS_LAMBDA );
    fprintf ( DrucErrorFile ,     
              "DX = %.1f  \t"   , 
              RectangleErreur->DX / (float)RDS_LAMBDA);
    fprintf ( DrucErrorFile ,     
              "DY = %.1f  \t\n" , 
              RectangleErreur->DY / (float)RDS_LAMBDA);
    fflush (DrucErrorFile);
  }
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucErreurOutFileRegle             |
|                                                             |
\------------------------------------------------------------*/

void DrucErreurOutFileRegle ( Regle
                            )
     int                     Regle;

{
  extern DrucTypeStructRuleComment *DrucRuleCommentList;
  DrucTypeStructRuleComment *Comment;

  if ( DrucStructStat.FILE_ERROR == DRUC_ERROR_FILE
     )
  {
    fprintf ( DrucErrorFile , 
              "\nERROR Code %4d: ",
              Regle
            );
    for (Comment = DrucRuleCommentList;
	 Comment && (Comment->RULE != Regle);
	 Comment = Comment->NEXT);
    if (Comment)
      fprintf ( DrucErrorFile , 
                "%s\n",
                Comment->COMMENT
              );
    else
      fprintf ( DrucErrorFile , 
                "NO MESSAGE\n"
              );

  }
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucErreurOutFileMesure            |
|                                                             |
\------------------------------------------------------------*/

void DrucErreurOutFileMesure ( Mesure
                             )
     float                       Mesure;

{

  if ( DrucStructStat.FILE_ERROR == DRUC_ERROR_FILE
     )
  {
    fprintf ( DrucErrorFile , 
              "Mesure Effectuee :%.1f\n", 
              Mesure
            );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucErreurOutFileLayer             |
|                                                             |
\------------------------------------------------------------*/

void DrucErreurOutFileLayer ( IndexLayer
                            )
     int                        IndexLayer;

{

  if ( DrucStructStat.FILE_ERROR == DRUC_ERROR_FILE
     )
  {
    fprintf ( DrucErrorFile , 
              "\tLayer %s :",
              ( RDS_LAYER_NAME [ IndexLayer ]) + 4
            );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucErreurUnRectangle              |
| si on a une composition se composition il faut              |
| re une boucle qui balaye toutes les compositions            |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list * DrucErreurUnRectangle ( DrucRectangle,
                                      DrucInstructionCourante,
                                      IndexMesure,
                                      IndexLayer
                                    )
     rdsrec_list               *DrucRectangle;
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;
     int                        IndexLayer;

{

register rdsrec_list            *RectangleOrigine;
register rdsrec_list            *RectangleErreurPrec;
register rdsrec_list            *RectangleErreur;
register DrucTypeStructPlusRect *TmpPlusRec;

/*\
DrucViewString ("\nDrucErreurUnRectangle\n");
DrucViewRectangle (&DrucRectangle);
\*/

  TmpPlusRec       = MACRO_NEXT_DEFINE ( DrucRectangle );

  if ( TmpPlusRec != ( DrucTypeStructPlusRect *)NULL
     )
  {
    RectangleErreur = allocrdsrec ( sizeof ( DrucTypeStructPlusRect )
                                  );

    RectangleOrigine    = TmpPlusRec->NEXT_EQUI;
    RectangleErreur->X  = RectangleOrigine->X;
    RectangleErreur->Y  = RectangleOrigine->Y;
    RectangleErreur->DX = RectangleOrigine->DX;
    RectangleErreur->DY = RectangleOrigine->DY;

    SetRdsLayer ( RectangleErreur, 
                  GetRdsLayer ( RectangleOrigine )
                );

    MACRO_REGLE_NUM (RectangleErreur ) = DrucInstructionCourante->REGLE_NUM [ IndexMesure ];
    MACRO_NEXT_EQUI (RectangleErreur ) = RectangleErreur;

/*\
 *  ca fait voir et mettre dans la figure des erreurs le rectangle 
 *  user ce qui n'est pas utile
 *
    DrucErreurOutFileRectangle ( RectangleErreur
                               );
\*/
    DrucAddRecFigure           ( RectangleErreur,
                                 DrucFigureRdsErreur,
                                 TmpPlusRec->NOMBRE
                               );
    /*\
     *  recopie de la liste des rectangles origines dans la rdsErreur
    \*/
/*\
  modif 08 03 94 : la ligne est reportee dans la boucle while 
    TmpPlusRec          = TmpPlusRec->NEXT_DEFINE;
\*/

    while ( TmpPlusRec != ( DrucTypeStructPlusRect *)NULL 
          )
    {
      /*\
       *  chainage circulaire des rectangles composant le RECTANGLE1
      \*/
      RectangleErreurPrec = RectangleErreur;
      RectangleErreur     = allocrdsrec ( sizeof
                                          ( DrucTypeStructPlusRect
                                          )
                                        );

      MACRO_NEXT_EQUI (RectangleErreur )     = MACRO_NEXT_EQUI ( RectangleErreurPrec );
      MACRO_NEXT_EQUI (RectangleErreurPrec ) = RectangleErreur;
      RectangleOrigine                       = TmpPlusRec->NEXT_EQUI;
      RectangleErreur->X                     = RectangleOrigine->X;
      RectangleErreur->Y                     = RectangleOrigine->Y;
      RectangleErreur->DX                    = RectangleOrigine->DX;
      RectangleErreur->DY                    = RectangleOrigine->DY;

      SetRdsLayer ( RectangleErreur, 
                    TmpPlusRec->NOMBRE
                  );

      MACRO_REGLE_NUM (RectangleErreur ) = DrucInstructionCourante->REGLE_NUM [ IndexMesure ];

      DrucErreurOutFileLayer  ( TmpPlusRec->NOMBRE
                              );

      DrucErreurOutFileRectangle ( RectangleErreur
                                 );
      DrucAddRecFigure           ( RectangleErreur,
                                   DrucFigureRdsErreur,
                                   TmpPlusRec->NOMBRE
                                 );
/*\
  modif 08 03 94 : la ligne suivante est rajoutee
\*/
    TmpPlusRec          = TmpPlusRec->NEXT_DEFINE;
    }
  }

  else
  {
    /*\
     *  recopie des infos du rectangle origine dans la rdsErreur
    \*/
    RectangleErreur = allocrdsrec ( sizeof ( DrucTypeStructPlusRect )
                                  );

    RectangleErreur->X  = DrucRectangle->X;
    RectangleErreur->Y  = DrucRectangle->Y;
    RectangleErreur->DX = DrucRectangle->DX;
    RectangleErreur->DY = DrucRectangle->DY;

    SetRdsLayer ( RectangleErreur, 
                  IndexLayer 
                );

    MACRO_REGLE_NUM (RectangleErreur )  = 
              DrucInstructionCourante->REGLE_NUM [ IndexMesure ];

    MACRO_NEXT_EQUI ( RectangleErreur ) = RectangleErreur;

    DrucErreurOutFileLayer  ( IndexLayer
                            );

    DrucErreurOutFileRectangle ( RectangleErreur
                               );
    DrucAddRecFigure           ( RectangleErreur,
                                 DrucFigureRdsErreur,
                                 IndexLayer
                               );
  }

  return ( RectangleErreur 
         );
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucErreurUnCorps                  |
|                                                             |
\------------------------------------------------------------*/

void DrucErreurUnCorps ( DrucRectangle,
                         DrucInstructionCourante,
                         IndexMesure
                       )
     rdsrec_list               *DrucRectangle;
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;

{

  DrucStructStat.ERROR_NUMBER ++;

  DrucErreurOutFileRegle  ( DrucInstructionCourante->REGLE_NUM[IndexMesure]
                          );
/*\
 * l'instruction est reportee dans l'affichage des rectangles
  DrucErreurOutFileLayer  ( DrucInstructionCourante->LAYER_1
                          );
\*/

  DrucErreurUnRectangle   ( DrucRectangle,
                            DrucInstructionCourante,
                            IndexMesure,
                            DrucInstructionCourante->LAYER_1
                          );
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucErreurDeuxCorps                |
|                                                             |
\------------------------------------------------------------*/

void DrucErreurDeuxCorps ( DrucStructMesure,
                           DrucInstructionCourante,
                           IndexMesure
                         )
     DrucTypeStructInstruction *DrucInstructionCourante;
     DrucTypeStructMesure      *DrucStructMesure;
     int                        IndexMesure;

{

rdsrec_list *First;
rdsrec_list *Second;
rdsrec_list *Link;

  DrucStructStat.ERROR_NUMBER ++;

  DrucErreurOutFileRegle  ( DrucInstructionCourante->REGLE_NUM[IndexMesure] 
                          );
/*\
modif 21/01/94 a la demande d'alain:
suppression de la mesure
  DrucErreurOutFileMesure ( DrucStructMesure->MESURE_EFFECTIVE / (float)RDS_LAMBDA 
                          );

suppression de l'affichage du layer user
la fonction est reportee dans l'affichage d'un rectangle
  DrucErreurOutFileLayer  ( DrucStructMesure->LAYER_1
                          );
\*/

  First = DrucErreurUnRectangle ( DrucStructMesure->RECTANGLE1,
                                  DrucInstructionCourante,
                                  IndexMesure,
                                  DrucStructMesure->LAYER_1
                                );

/*\
suppression de l'affichage du layer user
la fonction est reportee dans l'affichage d'un rectangle
  DrucErreurOutFileLayer ( DrucStructMesure->LAYER_2
                         );
\*/

  Second = DrucErreurUnRectangle  ( DrucStructMesure->RECTANGLE2,
                                    DrucInstructionCourante,
                                    IndexMesure,
                                    DrucStructMesure->LAYER_2
                                  );

  Link                       = MACRO_NEXT_EQUI ( First );
  MACRO_NEXT_EQUI ( First  ) = MACRO_NEXT_EQUI ( Second );
  MACRO_NEXT_EQUI ( Second ) = Link;
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucErreurCleanFigureErreur        |
|                                                             |
\------------------------------------------------------------*/

void DrucErreurCleanFigureErreur ( DrucFigureRdsErreur,
                                   DrucFigureRdsErrorName
                                 )
     rdsfig_list *DrucFigureRdsErreur;
     char        *DrucFigureRdsErrorName;

{

  if ( DrucFigureRdsErreur != ( rdsfig_list *)NULL 
     )
  {
    delrdsfig ( DrucFigureRdsErrorName 
              );

    DrucFigureRdsErreur = ( rdsfig_list *)NULL;
  }
}



