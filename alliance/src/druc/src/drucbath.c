/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DRUCBATH.C                       |
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
#include <string.h>
#include "mut.h"
#include "mph.h"
#include "mpu.h"
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
#include "drucmin.h"
#include "vmcunify.h"
#include "drucring.h"
#include "drucbath.h"
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

extern rdsfig_list   *DrucFigureRdsErreur;
extern FILE          *DrucErrorFile;

/*------------------------------------------------------------\
|                                                             |
|                          DrucConcateneErrorFiles            |
|                                                             |
\------------------------------------------------------------*/

void DrucConcateneErrorFiles ( DrucFigureRds,
                               DrucErrorFile
                             )
     rdsfig_list *DrucFigureRds;
     FILE        *DrucErrorFile;

{

rdsins_list *InstanceCourante;
char        *TmpErrorFileName;
FILE        *TmpErrorFile;
char         DrucTmpFileBuffer [ DRUC_MAX_STRING_BUFFER + 1 ];

  fputs ( "\n" ,
          DrucErrorFile 
        );
  fputs ( DrucFigureRds->NAME , 
          DrucErrorFile 
        );
  fputs ( "\n" , 
          DrucErrorFile 
        );

DrucViewString      ( "Merge Errorfiles: "
                    );
DrucViewString      ( "\n"
                    );
        
  for ( InstanceCourante  = DrucFigureRds->INSTANCE;
        InstanceCourante != (rdsins_list *)NULL;
        InstanceCourante  = InstanceCourante->NEXT
      )
  {
    TmpErrorFileName = DrucAddSuffixeName ( InstanceCourante->FIGNAME,
                                            DRUC_ERROR_FILE_SUFFIXE
                                          );

    DrucViewString ("\r");
    DrucViewString ( TmpErrorFileName);
    DrucViewString ("                      ");
       
    TmpErrorFile = fopen ( TmpErrorFileName,
                           "r"
                         );

    if ( TmpErrorFile != NULL ) 
    {
      while ( fgets( DrucTmpFileBuffer,
                     DRUC_MAX_FILE_BUFFER,
                     TmpErrorFile
                   )
              != (char *)NULL
            )
      {
        fputs ( DrucTmpFileBuffer , 
                DrucErrorFile 
              );
      }

      fclose  ( TmpErrorFile
              );
/*\
      unlink  ( TmpErrorFileName
              );
\*/
    }
  }
  DrucViewString      ( "\n");
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucConcateneErrorFigures          |
|                                                             |
\------------------------------------------------------------*/

void DrucConcateneErrorFigures ( DrucFigureRds,
                                 DrucFigureRdsErreur
                               )
     rdsfig_list *DrucFigureRds;
     rdsfig_list *DrucFigureRdsErreur;

{

rdsfig_list *InstanceModele;
rdsins_list *InstanceCourante;
rdsrec_list *RectangleCourant;
rdsrec_list *NouveauRectangle;
char        *TmpName;
int          IndexLayer;

DrucViewString      ( "Merge Error Instances:"
                    );
    DrucViewString      ( "\n"
                        );
        
  for ( InstanceCourante  = DrucFigureRds->INSTANCE;
        InstanceCourante != (rdsins_list *)NULL;
        InstanceCourante  = InstanceCourante->NEXT
      )
  {
/*\
 * pour toutes les instances de la figure inserer les rectangles 
 * du modele de l'instance dans le bloc
\*/
    TmpName = DrucAddSuffixeName ( InstanceCourante->FIGNAME,
                                   DRUC_ERROR_EXTEND
                                 );

    InstanceModele = searchrdsfig( TmpName
                                 );

    if ( InstanceModele == (rdsfig_list *)NULL
       )
    {
      continue;
    }
    DrucViewString      ( TmpName 
                        );
    DrucViewString      ( "\n"
                        );
        
    for ( IndexLayer  = 0;
          IndexLayer  < RDS_MAX_LAYER;
          IndexLayer ++
        )
    {
      if ( ( IndexLayer == DRUC_BBOX ) ||
           ( IndexLayer == RDS_USER0 ) ||
           ( IndexLayer == RDS_USER1 ) ||
           ( IndexLayer == RDS_USER2 ) ||
           ( IndexLayer == RDS_USER3 ) ||
           ( IndexLayer == RDS_USER4 ) ||
           ( IndexLayer == RDS_USER5 ) ||
           ( IndexLayer == RDS_USER6 ) ||
           ( IndexLayer == RDS_USER7 ) ||
           ( IndexLayer == RDS_USER8 ) ||
           ( IndexLayer == RDS_USER9 ) 
         )
      {
        continue;
      }

      for ( RectangleCourant  = InstanceModele->LAYERTAB [ IndexLayer ];
            RectangleCourant != (rdsrec_list *)NULL;
            RectangleCourant  = RectangleCourant->NEXT
          )
      {
        NouveauRectangle = addrdsfigrec ( DrucFigureRdsErreur,
                                          RectangleCourant->NAME,
                                          IndexLayer,
                                          RectangleCourant->X,
                                          RectangleCourant->Y,
                                          RectangleCourant->DX,
                                          RectangleCourant->DY
                                        );

        applyrdssym ( & NouveauRectangle->X,
                      & NouveauRectangle->Y,
                      & NouveauRectangle->DX,
                      & NouveauRectangle->DY,
                      InstanceCourante->X,
                      InstanceCourante->Y,
                      InstanceCourante->TRANSF
                    );
      } /* for tous les rectangles */
    }/* for tous les layers */
  }/* for toutes les instances */
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucSaveFgureErreur                 |
|                                                             |
\------------------------------------------------------------*/

void DrucSaveFigureErreur ( DrucFigureRdsErreur
                          )
     rdsfig_list *DrucFigureRdsErreur;

{

rdsrec_list *RectangleCourant;

  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER0
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER1
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER2
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER3
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER4
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER5
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER6
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER7
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER8
                   );
  DrucDelLayer     ( DrucFigureRdsErreur,
                     RDS_USER9
                   );
/*\
viewrdsfig ( DrucFigureRdsErreur
           );
\*/
  saverdsfig       ( DrucFigureRdsErreur
                   );
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucBloc                            |
|                                                             |
\------------------------------------------------------------*/

void DrucBloc ( DrucFigureName
              )
char *DrucFigureName;

{

extern DrucTypeStructRdsLayer  DrucStructRdsLayer[];
rdswindow                 *DrucRdsWindow;
DrucTypeStructInstruction *DrucTabInstruction = ( DrucTypeStructInstruction *)NULL;
phfig_list                *DrucFigureMbk;
rdsfig_list               *DrucFigureRds;
rdsins_list               *InstanceCourante;
rdsfig_list               *InstanceModele;
char                      *TmpName;
char                      *DrucErrorName;
char                      *DrucCouronneName;

  DrucFigureMbk = getphfig ( DrucFigureName,
                             'A'
                           );

  if ( incatalog ( DrucFigureName) ||
       DrucStructStat.FLAG_FLATTEN_MODE == DRUC_FLATTEN 
     )
  {
    DrucViewString ( "Flatten DRC on: "
                   );
    DrucViewString ( DrucFigureName
                   );
    DrucViewString ( "\n"
                   );
    rflattenphfig ( DrucFigureMbk,
                    YES,
                    NO
                  );
    
    DrucFigureRds = figmbkrds ( DrucFigureMbk,
                                MACRO_PLUS_TAILLE,
                                0
                              );

/*\
viewrdsfig ( DrucFigureRds );
\*/
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
       
    DrucInitInstruction ( & DrucTabInstruction
                        );
    DrucViewString      ( "Unify : "
                        );
    DrucViewString      ( DrucFigureName
                        );
    DrucViewString      ( "\n"
                        );
    DrucUnifyFigure     (   DrucFigureRds,
                            DrucStructRdsLayer
                        );

    DrucRdsWindow    = buildrdswindow ( DrucFigureRds
                                      );

    DrucComputeEquiFigure ( DrucFigureRds
                          );
  }
  else
  {
/*\
 *  le bloc n'est pas dans le catalogue alors on fait sa verification hierarchique
\*/
    DrucViewString      ( "Hierarchical DRC on : "
                        );
    DrucViewString      ( DrucFigureName
                        );
    DrucViewString      ( "\n"
                        );
       
    DrucFigureRds = figmbkrds ( DrucFigureMbk,
                                MACRO_PLUS_TAILLE,
                                0
                              );

/*\
viewrdsfig ( DrucFigureRds );
\*/

    if ( DrucFigureRds->LAYERTAB [ DRUC_BBOX ]
        != (rdsrec_list *)NULL 
       )
    {
      delrdsfigrec ( DrucFigureRds,
                     DrucFigureRds->LAYERTAB [ DRUC_BBOX ]
                   );
    }
 
    DrucViewString      ( "Delete MBK figure : "
                        );
    DrucViewString      ( DrucFigureName 
                        );
    DrucViewString      ( "\n"
                        );
    delphfig            ( DrucFigureName
                        );

    for ( InstanceCourante  = DrucFigureRds->INSTANCE;
          InstanceCourante != (rdsins_list *)NULL;
          InstanceCourante  = InstanceCourante->NEXT
        )
    {
/*\
 *  pour toutes les instances de la figure lancer la verif hierarchique
\*/
      TmpName = DrucAddSuffixeName ( InstanceCourante->FIGNAME,
                                     DRUC_COURONNE_EXTEND
                                   );

      InstanceModele = searchrdsfig ( TmpName
                                    );
/*\
 *  si la couronne de l'instance n'exite pas lancer la verif hierarchique
\*/
      if ( InstanceModele == ( rdsfig_list *)NULL
         )
      {
        DrucBloc ( InstanceCourante->FIGNAME
                 );
      }
    }
    DrucViewString      ( "Load Hierarchical Rules"
                        );
    DrucViewString      ( "\n"
                        );
       
    DrucInitInstruction ( & DrucTabInstruction
                        );

    DrucViewString           ( "Unify : "
                             );
    DrucViewString           ( DrucFigureName
                             );
    DrucViewString           ( "\n"
                             );
    DrucUnifyFigure          (   DrucFigureRds,
                                 DrucStructRdsLayer
                             );
    DrucFlattenInstanceRings ( DrucFigureRds 
                             );

    InstanceCourante        = DrucFigureRds->INSTANCE;
    DrucFigureRds->INSTANCE = (rdsins_list *)NULL;
    DrucRdsWindow           = buildrdswindow ( DrucFigureRds
                                             );
    DrucFigureRds->INSTANCE = InstanceCourante;

    DrucComputeEquiFigure ( DrucFigureRds
                          );
  }/* else */
/*\
 *  la figure RDS est chargee soit a plat soit avec ses couronnes on n'a plus besoin de MBK
\*/
  DrucCouronneName = DrucAddSuffixeName ( DrucFigureName,
                                          DRUC_COURONNE_EXTEND
                                        );

  DrucViewString      ( "\nCreate Ring : "
                      );
  DrucViewString      ( DrucCouronneName
                      );
  DrucViewString      ( "\n"
                      );

  DrucCreateCouron    ( DrucFigureRds,
                        DrucRdsWindow,
                        DrucCouronneName
                      );
/*\
 *  ici se colle la concatenation des fichiers d'erreurs
 *  ici se colle la verification
\*/

  DrucErrorName = DrucAddSuffixeName ( DrucFigureName,
                                       DRUC_ERROR_FILE_SUFFIXE
                                     );

  DrucOpenErrorFile       ( DrucErrorName,
                            "w"
                          );
  DrucConcateneErrorFiles ( DrucFigureRds,
                            DrucErrorFile
                          );
/*\
 *  concatener les figures des erreurs
\*/
  DrucConcateneErrorFigures ( DrucFigureRds,
                              DrucFigureRdsErreur
                            );
/*\
 * envoyer la verif des regles
\*/
  DrucMin ( DrucFigureRds,
            DrucRdsWindow,
            DrucTabInstruction ,
            DrucNombreInstructions
          );

/*\
 * fermer le fichier des erreurs
 * sauver la figure des erreurs
\*/
  DrucSaveFigureErreur ( DrucFigureRdsErreur
                       );
  DrucDelLayer         ( DrucFigureRds,
                         DRUC_BBOX
                       );
  DrucDelLayer         ( DrucFigureRds,
                         RDS_USER0
                       );
  DrucDelLayer         ( DrucFigureRds,
                         RDS_USER1
                       );
  DrucDelLayer         ( DrucFigureRds,
                         RDS_USER2
                       );
  DrucDelLayer         ( DrucFigureRdsErreur,
                         RDS_USER3
                       );
  DrucDelLayer         ( DrucFigureRdsErreur,
                         RDS_USER4
                       );
  DrucDelLayer         ( DrucFigureRdsErreur,
                         RDS_USER5
                       );
  DrucDelLayer         ( DrucFigureRdsErreur,
                         RDS_USER6
                       );
  DrucDelLayer         ( DrucFigureRdsErreur,
                         RDS_USER7
                       );
  DrucDelLayer         ( DrucFigureRdsErreur,
                         RDS_USER8
                       );
  DrucDelLayer         ( DrucFigureRdsErreur,
                         RDS_USER9
                       );
  InstanceCourante        = DrucFigureRds->INSTANCE;
  DrucFigureRds->INSTANCE = (rdsins_list *)NULL;


  DrucFigureRds->INSTANCE = InstanceCourante;
/*\
\*/
  DrucFigureRds->INSTANCE = (rdsins_list *)NULL;
 
  destroyrdswindow   ( DrucFigureRds,
                       DrucRdsWindow
                     );
  delrdsfig          ( DrucFigureName
                     ); 
  DrucViewString     ( "End DRC on: "
                     );
  DrucViewString     ( DrucFigureName 
                     );
  DrucViewString     ( "\n"
                     );
  DrucCloseErrorFile (
                     );
}

/*------------------------------------------------------------\
|                                                             |
|                             Main                            |
|                                                             |
\------------------------------------------------------------*/
int main (argc , argv )
     int   argc;
     char *argv[];
{
phfig_list                *DrucFigureMbk;
rdsfig_list               *DrucFigureRds;
rdswindow                 *DrucRdsWindow;
char                      *Name;
char                      *DrucFigureName;
char                      *DrucErrorName           = (char *)NULL;
char                      *DrucCouronneRdsName;
char                      *DrucNetworkName;
DrucTypeStructInstruction *DrucTabInstruction      = ( DrucTypeStructInstruction *)NULL;
int                        IndexLayer;
int                        Resultat;
int                        IndexName = 1;

  int i;
  boolean A,B;

  mbkenv         (
                 );

/*\
 *  affichage de la banniere standard
\*/
  alliancebanner ( "DRuC", 
                    VERSION, 
                   "Design Rule Checker", 
                   "1993", 
                    ALLIANCE_VERSION
                 );

if ( argc < 2 || argc > 4
   )
{
  printf       ( "Usage : %s figure_name (without extention) [-v] [-h]\n",argv [0]);
/*\
\*/
  exit         ( 2 );
}
  A = DRUC_SILENT;
  B = DRUC_FLATTEN;

  for ( i =  1  ;
        i <  argc;
        i ++
      )
  {
    if (strcmp ( argv [i], "-v" )==0) 
    { 
      A = DRUC_VERBOSE; 
    }
    else
    {
      if (strcmp ( argv [i], "-h" )==0) 
      { 
        B = DRUC_HIERARCHIE; 
      }
      else
      {
        IndexName =  i;
      }
    }
  }    
    DrucStatInit   ( B,
                     DRUC_VERBOSE,
                     A,
                     DRUC_ERROR_FILE
                   );

  Name           = argv [ IndexName ];

  DrucFigureName = namealloc ( Name 
                             );
  Name           = malloc ( strlen ( DrucFigureName )    +
                            strlen ( DRUC_ERROR_EXTEND) +
                            1
                 );

  strcpy         ( Name ,
                   DrucFigureName
                 );
  strcat         ( Name,
                   DRUC_ERROR_EXTEND
                 );
  DrucErrorName = namealloc ( Name );
  free           ( Name
                 );
  rdsenv         (
                 );
  loadrdsparam   (
                 );
/*\
 *  creer la figure des erreurs locales
\*/
  DrucFigureRdsErreur    = addrdsfig ( DrucErrorName,
                                       MACRO_PLUS_TAILLE
                                     );

  DrucBloc       ( DrucFigureName
                 );
  DrucViewString ( "Saving the Error file figure\n"
                 );
  DrucDelLayer   ( DrucFigureRdsErreur,
                   DRUC_BBOX 
                 );
  DrucViewString ( "Done\n"
                 );
  DrucViewNumber   ( DrucStructStat.ERROR_NUMBER
                   );
  DrucViewString   ( "\n"
                   );

  if ( DrucStructStat.ERROR_NUMBER == 0
     )
  {
    fprintf ( stdout,
              "\nFile: %s%s is empty: no errors detected.\n",
              DrucFigureName,
              namealloc ( DRUC_ERROR_FILE_SUFFIXE
                        )
            );
  }
  else
  {
    fprintf ( stdout,
              "\nSome errors have been detected, see file: %s%s for detailled\n",
              DrucFigureName,
              namealloc ( DRUC_ERROR_FILE_SUFFIXE
                        )
            );
  }
  exit      ( DrucStructStat.ERROR_NUMBER
            );
}



