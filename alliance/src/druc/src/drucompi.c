/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  DRUCOMPI.C                       |
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
/*------------------------------------------------------------\
|                                                             |
|                  DRUCOMPI  variables                        |
|                                                             |
\------------------------------------------------------------*/
DrucTypeStructDefine       DrucStructDefine;
DrucTypeStructRegle        DrucStructRegle;
DrucTypeStructInstruction *DrucStructInstruction;


int                        DrucNombreInstructions;
int                        CurrentIndexDrucLayer;
long                       DrucCtmMax;
   
FILE                      *TmpFileAssembler;
char                       DRUC_OUTPUT_NAME[256];
char                       DRUC_TMP_OUTPUT_NAME[256];

/*------------------------------------------------------------\
|                                                             |
|                  DRUCOMPI  Fonctions                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc vrd_y_error                               |
|                                                             |
\------------------------------------------------------------*/
void vrd_y_error(s)
  char *s;
{
  fprintf (stdout,"\nErreur Langage ligne %d : %s\n",vrd_y_lineno,s);
  EXIT(1);
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitCompiler                           |
|                                                             |
| Ouvre le fichier de destination du code assembleur de DRUC  |
| Initialise la structure DrucStructRdsLayer                  |
| Initialise la structure DrucStructDefine                    |
| Initialise la structure DrucStructRegle                     |
\------------------------------------------------------------*/
void DrucInitCompiler ()
{
  int      TmpIndexMesure;
  int      IndexLayer;
  CurrentIndexDrucLayer = DRUC_UNDEFINED_LAYER;
  DrucCtmMax            = DRUC_UNDEFINED_CTM;

  for ( IndexLayer  = 0;
        IndexLayer  < RDS_MAX_LAYER;
        IndexLayer ++
      )
  {
    DrucStructRdsLayer [ IndexLayer ].RDS_INDEX  = DRUC_UNDEFINED_LAYER;
    DrucStructRdsLayer [ IndexLayer ].WIDTH      = DRUC_UNDEFINED_WIDTH;
    DrucStructRdsLayer [ IndexLayer ].CTM        = DRUC_UNDEFINED_CTM;
    DrucStructRdsLayer [ IndexLayer ].FLAGS      = DRUC_FLAG_LAYER_INTERDIT;
  }
  DrucStructRdsLayer [ DRUC_USER_LAYER_0 ].FLAGS = DRUC_FLAG_LAYER_LIBRE;
  DrucStructRdsLayer [ DRUC_USER_LAYER_1 ].FLAGS = DRUC_FLAG_LAYER_LIBRE;
  DrucStructRdsLayer [ DRUC_USER_LAYER_2 ].FLAGS = DRUC_FLAG_LAYER_LIBRE;

  DrucStructDefine.LAYER_DEST                    = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.OP_COMPOSE                    = DRUC_UNDEFINED_OP_COMPOSE;
  DrucStructDefine.OP_UNAIRE                     = DRUC_UNDEFINED_OP_COMPOSE;
  DrucStructDefine.LAYER_1                       = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.LAYER_2                       = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.USER_LAYER_0                  = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.USER_LAYER_1                  = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.USER_LAYER_2                  = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.USER_LAYER_NAME_0             = (char *)NULL;
  DrucStructDefine.USER_LAYER_NAME_1             = (char *)NULL;
  DrucStructDefine.USER_LAYER_NAME_2             = (char *)NULL;

  DrucStructRegle.LIST_REGLE_NUM                 = (DataList *)NULL;
  DrucStructRegle.INSTRUCTION                    = DRUC_UNDEFINED_INSTRUCTION;
  DrucStructRegle.LAYER_1                        = DRUC_UNDEFINED_LAYER;
  DrucStructRegle.LAYER_2                        = DRUC_UNDEFINED_LAYER;
  for ( TmpIndexMesure              = 0;
        TmpIndexMesure  < DRUC_MAX_MESURES;
        TmpIndexMesure ++
      )
  {
    DrucStructRegle.RELATION   [TmpIndexMesure ] = DRUC_UNDEFINED_RELATION;
    DrucStructRegle.REGLE_NUM  [TmpIndexMesure ] = DRUC_UNDEFINED_REGLE;
    DrucStructRegle.MESURE     [TmpIndexMesure ] = DRUC_UNDEFINED_MESURE;
    DrucStructRegle.OP_COMPARE [TmpIndexMesure ] = DRUC_UNDEFINED_OPERATEUR;
    DrucStructRegle.VALEUR     [TmpIndexMesure ] = DRUC_UNDEFINED_VALEUR;
  }
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucCloseCompiler                          |
|                                                             |
| Ferme le fichier de destination du code assembleur de DRUC  |
\------------------------------------------------------------*/
void DrucCloseCompiler ()
{
  if ( fclose( TmpFileAssembler ) == EOF )
  {
    DRUC_EXIT ( DRUC_ERROR_CLOSE_DRC_FILE );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRdsLayerName                       |
|                                                             |
| Trouve le Name dans la liste des layers RDS ou sort         |
| Range l'index associe dans DrucStructRdsLayer.RDS_INDEX     |
\------------------------------------------------------------*/
void DrucInitRdsLayerName (Name)
     char *Name;
{
  int IndexLayer = 0;

  CurrentIndexDrucLayer  = DRUC_UNDEFINED_LAYER;
  while( IndexLayer < RDS_MAX_LAYER )
  {
    if ( strncmp ( RDS_LAYER_NAME[ IndexLayer ], 
                   Name ,
                   strlen (Name)
                 )
         == 0
       )
    {
      if ( DrucStructRdsLayer[ IndexLayer ].RDS_INDEX == DRUC_UNDEFINED_LAYER )
      {
        DrucStructRdsLayer[ IndexLayer ].RDS_INDEX = IndexLayer;
        DrucStructRdsLayer[ IndexLayer ].FLAGS     = DRUC_FLAG_LAYER_OCCUPE;
        CurrentIndexDrucLayer                      = IndexLayer;
        return;
      }
    }
    IndexLayer ++ ;
  }
  DRUC_EXIT( DRUC_ERROR_UNDEFINED_LAYER );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRdsLayerDef                        |
|                                                             |
| Range la Value associe dans DrucStructRdsLayer.WIDTH        |
| dont l'index est dans CurrentIndexDrucLayer                 |
\------------------------------------------------------------*/
void DrucInitRdsLayerDef (Value)
     int Value;
{
  if ( CurrentIndexDrucLayer == DRUC_UNDEFINED_LAYER)
  {
    DRUC_EXIT( DRUC_ERROR_UNDEFINED_LAYER );
  }
/*\
 * DrucViewString ("DrucInitRdsLayerDef");
 * DrucViewNumber (Value);
\*/
  DrucStructRdsLayer[ CurrentIndexDrucLayer ].WIDTH = Value;
  CurrentIndexDrucLayer = DRUC_UNDEFINED_LAYER;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucStoreRdsLayer                          |
|                                                             |
| Ecrit dans le fichier *TmpFileAssembler, les layers         |
| contenus dans DrucStructRdsLayer[] avec leur valeur min     |
| pour la verification des regles et l'unification            |
| Stocke le checksumm des ecritures dans DrucCheckSumm        |
\------------------------------------------------------------*/

void DrucStoreRdsLayer ()
{
  register int IndexLayer;
 
   fprintf( TmpFileAssembler,
            "%d ",
            DRUC_ASM_BEGIN_LAYER
          );
 
   for( IndexLayer  = 0;
        IndexLayer != RDS_MAX_LAYER;
        IndexLayer ++
      )
   {
     if ((DrucStructRdsLayer[ IndexLayer ].RDS_INDEX != DRUC_UNDEFINED_LAYER ) &&
         (DrucStructRdsLayer[ IndexLayer ].CTM == DRUC_UNDEFINED_LAYER )
        )
     {
       DrucStructRdsLayer[ IndexLayer ].CTM = DrucCtmMax;
     } 
     fprintf( TmpFileAssembler,
             "%d %d %d %d ",
              DrucStructRdsLayer[ IndexLayer ].RDS_INDEX,
              DrucStructRdsLayer[ IndexLayer ].WIDTH,
              DrucStructRdsLayer[ IndexLayer ].CTM,
              DrucStructRdsLayer[ IndexLayer ].FLAGS
           );
  }
  fprintf( TmpFileAssembler,
           "%d\n",
           DRUC_ASM_END_LAYER
         );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitDefineLayerDest                    |
|                                                             |
| Verifie que:                                                |
|   soit le layer NAME est vide                               |
|   soir le layer NAME est le meme que le layer A             |
|                                                             |
\------------------------------------------------------------*/
void DrucInitDefineLayerDest (Name)
     char *Name;
{
  int IndexLayer = 0;

  while( IndexLayer < RDS_MAX_LAYER )
  {
    if ( strstr( RDS_LAYER_NAME[ IndexLayer ] , Name ) != ( char *) NULL )
    {
      /*\
       *  Le nom existe dans la liste des layers RDS qui sont interdits
      \*/
      if ( DrucStructRdsLayer [ IndexLayer ].FLAGS == DRUC_FLAG_LAYER_INTERDIT )
      {
        DRUC_EXIT( DRUC_ERROR_NOT_DECLARED_LAYER);
      }
      /*\
       *  Le nom existe dans la liste des layers RDS qui sont occupes
      \*/
      if ( DrucStructRdsLayer [ IndexLayer ].FLAGS == DRUC_FLAG_LAYER_OCCUPE && 
           DrucStructDefine.OP_COMPOSE             != DRUC_COMPOSE_RESIZE 
         )
      {
        DRUC_EXIT( DRUC_ERROR_NOT_EMPTY_LAYER);
      }
      else
      {
        DrucStructDefine.LAYER_DEST = IndexLayer; 
        DrucStructRdsLayer [ IndexLayer ].FLAGS = DRUC_FLAG_LAYER_OCCUPE;
        return;
      }
    }
    IndexLayer ++ ;
  }
  /*\
   *  Le nom existe dans la liste des layers USER en usage
  \*/
  if ( DrucStructDefine.USER_LAYER_NAME_0 != (char *)NULL )
  {
    if ( strstr( Name , DrucStructDefine.USER_LAYER_NAME_0 ) != (char *)NULL )
    {
      DRUC_EXIT( DRUC_ERROR_DEFINE_USER_LAYER);
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_1 != (char *)NULL )
  {
    if ( strstr( Name , DrucStructDefine.USER_LAYER_NAME_1 ) != (char *)NULL )
    {
      DRUC_EXIT( DRUC_ERROR_DEFINE_USER_LAYER);
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_2 != (char *)NULL )
  {
    if ( strstr( Name , DrucStructDefine.USER_LAYER_NAME_2 ) != (char *)NULL )
    {
      DRUC_EXIT( DRUC_ERROR_DEFINE_USER_LAYER);
    }
  }
  /*\
   *  Les layers USER ne sont pas tous en usage
  \*/
  if ( DrucStructDefine.USER_LAYER_NAME_0 != (char *)NULL )
  {
    if ( DrucStructDefine.USER_LAYER_NAME_1 != (char *)NULL )
    {
      if ( DrucStructDefine.USER_LAYER_NAME_2 != (char *)NULL )
      {
        DRUC_EXIT( DRUC_ERROR_DEFINE_USER_LAYER );
      }
      DrucStructRdsLayer [ DRUC_USER_LAYER_2 ].FLAGS = DRUC_FLAG_LAYER_OCCUPE;
      DrucStructDefine.LAYER_DEST        = DRUC_USER_LAYER_2;
      DrucStructDefine.USER_LAYER_2      = DRUC_USER_LAYER_2;
      DrucStructDefine.USER_LAYER_NAME_2 = (char *)malloc ( strlen ( Name ) +1 );
      strcpy ( DrucStructDefine.USER_LAYER_NAME_2 ,Name );
      return;
    }
    DrucStructRdsLayer [ DRUC_USER_LAYER_1 ].FLAGS = DRUC_FLAG_LAYER_OCCUPE;
    DrucStructDefine.LAYER_DEST        = DRUC_USER_LAYER_1;
    DrucStructDefine.USER_LAYER_1      = DRUC_USER_LAYER_1;
    DrucStructDefine.USER_LAYER_NAME_1 = (char *)malloc ( strlen ( Name ) +1 );
    strcpy ( DrucStructDefine.USER_LAYER_NAME_1 ,Name );
    return;
  }
  DrucStructRdsLayer [ DRUC_USER_LAYER_0 ].FLAGS = DRUC_FLAG_LAYER_OCCUPE;
  DrucStructDefine.LAYER_DEST        = DRUC_USER_LAYER_0;
  DrucStructDefine.USER_LAYER_0      = DRUC_USER_LAYER_0;
  DrucStructDefine.USER_LAYER_NAME_0 = (char *)malloc ( strlen ( Name ) +1 );
  strcpy ( DrucStructDefine.USER_LAYER_NAME_0 ,Name );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucStructDefineOpCompose                  |
|                                                             |
\------------------------------------------------------------*/
void DrucStructDefineOpCompose ( Operateur )
     int Operateur;
{
  DrucStructDefine.OP_COMPOSE = Operateur;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitOpUnaire                           |
|                                                             |
\------------------------------------------------------------*/
void DrucInitOpUnaire ( )
{
  DrucStructDefine.OP_UNAIRE  = DrucStructDefine.OP_COMPOSE;
  DrucStructDefine.OP_COMPOSE = DRUC_UNDEFINED_OP_COMPOSE;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitDefineValeur                       |
|                                                             |
\------------------------------------------------------------*/
void DrucInitDefineValeur ( Valeur )
     long Valeur;
{
  DrucStructDefine.VALEUR = Valeur;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitDefineLayerA                       |
|                                                             |
| Verifie que le Name est dans la liste des layers RDS        |
| ou sort                                                     |
| Verifie que le layer correspondant est initialise           |
| ou sort                                                     |
| Range l'index associe dans                                  |
|    DrucTypeStructDefine.USER_LAYER_SOURCE_A                 |
\------------------------------------------------------------*/
void DrucInitDefineLayerA (Name)
     char *Name;
{
  int IndexLayer = 0;

  /*\
   *  Le nom existe dans la liste des layers RDS
  \*/
  while( IndexLayer < RDS_MAX_LAYER )
  {
    if ( strncmp ( RDS_LAYER_NAME[ IndexLayer ], 
                   Name , 
                   strlen (Name) 
                 ) 
         == 0
       )
    if ( DrucStructRdsLayer[ IndexLayer ].RDS_INDEX == IndexLayer 
       ) 
    {
      DrucStructRegle.LAYER_1  = DrucStructRdsLayer[ IndexLayer ].RDS_INDEX;
      DrucStructDefine.LAYER_1 = DrucStructRdsLayer[ IndexLayer ].RDS_INDEX;
      return;
    }
    IndexLayer ++ ;
  }
  /*\
   *  Le nom existe dans la liste des layers USER
  \*/
  if ( DrucStructDefine.USER_LAYER_NAME_0 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_0, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_1  = DRUC_USER_LAYER_0;
      DrucStructDefine.LAYER_1 = DRUC_USER_LAYER_0;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_1 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_1, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_1  = DRUC_USER_LAYER_1;
      DrucStructDefine.LAYER_1 = DRUC_USER_LAYER_1;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_2 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_2, Name ) !=
     (char *)NULL )
    {
      DrucStructRegle.LAYER_1  = DRUC_USER_LAYER_2;
      DrucStructDefine.LAYER_1 = DRUC_USER_LAYER_2;
      return;
    }
  }
  DRUC_EXIT( DRUC_ERROR_UNDEFINED_LAYER );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitDefineLayerB                       |
|                                                             |
| Verifie que le Name est dans la liste des layers RDS        |
| ou sort                                                     |
| Verifie que le layer correspondant est initialise           |
| ou sort                                                     |
| Range l'index associe dans                                  |
|    DrucTypeStructDefine.USER_LAYER_SOURCE_B                 |
\------------------------------------------------------------*/
void DrucInitDefineLayerB (Name)
     char *Name;
{
  int IndexLayer = 0;

  /*\
   *  Le nom existe dans la liste des layers RDS
  \*/
  while( IndexLayer < RDS_MAX_LAYER )
  {
    if ( strncmp ( RDS_LAYER_NAME[ IndexLayer ], 
                   Name , 
                   strlen (Name) 
                 ) 
         == 0
       )
    if ( DrucStructRdsLayer[ IndexLayer ].RDS_INDEX == IndexLayer 
       ) 
    {
      DrucStructRegle.LAYER_2  = DrucStructRdsLayer[ IndexLayer ].RDS_INDEX;
      DrucStructDefine.LAYER_2 = DrucStructRdsLayer[ IndexLayer ].RDS_INDEX;
      return;
    }
    IndexLayer ++ ;
  }
  /*\
   *  Le nom existe dans la liste des layers USER
  \*/
  if ( DrucStructDefine.USER_LAYER_NAME_0 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_0, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_2  = DRUC_USER_LAYER_0;
      DrucStructDefine.LAYER_2 = DRUC_USER_LAYER_0;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_1 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_1, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_2  = DRUC_USER_LAYER_1;
      DrucStructDefine.LAYER_2 = DRUC_USER_LAYER_1;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_2 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_2, Name ) !=
     (char *)NULL )
    {
      DrucStructRegle.LAYER_2  = DRUC_USER_LAYER_2;
      DrucStructDefine.LAYER_2 = DRUC_USER_LAYER_2;
      return;
    }
  }
  DRUC_EXIT( DRUC_ERROR_UNDEFINED_LAYER );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucStoreInstructionDefine                 |
|                                                             |
| Ecrit dans le fichier *TmpFileAssembler, l'instruction      |
| contenue dans DrucStructDefine avec ses valeurs             |
| Stocke le checksumm des ecritures dans DrucCheckSumm        |
| DrucStructDefine.USER_LAYER_NAME_0        n'est pas ecrit   |
| DrucStructDefine.USER_LAYER_NAME_1        n'est pas ecrit   |
| DrucStructDefine.USER_LAYER_NAME_2        n'est pas ecrit   |
| Seuls les numeros de layer associes sont ecrits             |
\------------------------------------------------------------*/
void DrucStoreInstructionDefine ()
{
  fprintf( TmpFileAssembler,
           "%d ",
           DRUC_ASM_BEGIN_DEFINE
         );

  fprintf( TmpFileAssembler,
           "%d %d %d %d ",
            DrucStructDefine.LAYER_DEST,
            DrucStructDefine.OP_COMPOSE,
            DrucStructDefine.LAYER_1,
            DrucStructDefine.LAYER_2
         );

  fprintf( TmpFileAssembler,
           "%d\n",
           DRUC_ASM_END_DEFINE
         );

  DrucStructDefine.LAYER_DEST        = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.OP_COMPOSE        = DRUC_UNDEFINED_OP_COMPOSE;
  DrucStructDefine.LAYER_1           = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.LAYER_2           = DRUC_UNDEFINED_LAYER;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucLoadInstructionDefine                  |
|                                                             |
| Transfert le contenu de DrucStructDefine dans une nouvelle  |
| instruction de TmpStructInstruction.                        |
| DrucStructDefine.USER_LAYER_NAME_0        n'est pas ecrit   |
| DrucStructDefine.USER_LAYER_NAME_1        n'est pas ecrit   |
| DrucStructDefine.USER_LAYER_NAME_2        n'est pas ecrit   |
| Seuls les numeros de layer associes sont ecrits             |
\------------------------------------------------------------*/
void DrucLoadInstructionDefine ()
{

int TmpIndexMesure;
DrucTypeStructInstruction *TmpStructInstruction;

  TmpStructInstruction =
     ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));

  if ( TmpStructInstruction == (DrucTypeStructInstruction *)NULL
     )
  {
    DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
  }

  TmpStructInstruction->NEXT        = DrucStructInstruction;
  TmpStructInstruction->INSTRUCTION = DRUC_ASM_BEGIN_DEFINE;
  TmpStructInstruction->LAYER_DEST  = DrucStructDefine.LAYER_DEST;
  TmpStructInstruction->LAYER_1     = DrucStructDefine.LAYER_1;
  TmpStructInstruction->LAYER_2     = DrucStructDefine.LAYER_2;
  TmpStructInstruction->OP_COMPOSE  = DrucStructDefine.OP_COMPOSE;
  TmpStructInstruction->OP_UNAIRE   = DrucStructDefine.OP_UNAIRE; 
  TmpStructInstruction->DEF_VALEUR  = DrucStructDefine.VALEUR;

  for ( TmpIndexMesure = 0;
        TmpIndexMesure < DRUC_MAX_MESURES;
        TmpIndexMesure ++
      )
  {
    ( TmpStructInstruction->REGLE_NUM ) [ TmpIndexMesure ] = DRUC_UNDEFINED_REGLE;
    ( TmpStructInstruction->RELATION  ) [ TmpIndexMesure ] = DRUC_UNDEFINED_RELATION;
    ( TmpStructInstruction->MESURE    ) [ TmpIndexMesure ] = DRUC_UNDEFINED_MESURE;
    ( TmpStructInstruction->OP_COMPARE) [ TmpIndexMesure ] = DRUC_UNDEFINED_OPERATEUR;
    ( TmpStructInstruction->VALEUR    ) [ TmpIndexMesure ] = DRUC_UNDEFINED_VALEUR;
  }

  DrucStructInstruction             = TmpStructInstruction;
  DrucNombreInstructions ++;

  DrucStructDefine.LAYER_DEST        = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.LAYER_1           = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.LAYER_2           = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.OP_COMPOSE        = DRUC_UNDEFINED_OP_COMPOSE;
  DrucStructDefine.OP_UNAIRE         = DRUC_UNDEFINED_OP_COMPOSE;
  DrucStructDefine.VALEUR            = 0;
/*\
 * DrucViewInstruction ( TmpStructInstruction );
\*/
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitUndefineUserLayer                  |
|                                                             |
| Trouve Name dans DrucStructDefine.USER_LAYER_i  ou sort     |
| Range son index dans DrucStructDefine.USER_LAYER_i          |
\------------------------------------------------------------*/
void DrucInitUndefineUserLayer (Name)
     char *Name;
{

int IndexLayer = 0;

  /*\
   *  Le Name n'existe pas dans la liste des layers user
  \*/

  while( IndexLayer < RDS_MAX_LAYER )
  {
    if ( strstr( RDS_LAYER_NAME[ IndexLayer ] , Name ) != ( char *) NULL )
    {
      /*\
       *  Le nom existe dans la liste des layers RDS qui sont interdits
      \*/
      if ( DrucStructRdsLayer [ IndexLayer ].FLAGS == DRUC_FLAG_LAYER_INTERDIT )
      {
        DRUC_EXIT( DRUC_ERROR_NOT_DECLARED_LAYER);
      }
      /*\
       *  Le nom existe dans la liste des layers RDS qui sont libres
      \*/
      if ( DrucStructRdsLayer [ IndexLayer ].FLAGS == DRUC_FLAG_LAYER_LIBRE )
      {
        DRUC_EXIT( DRUC_ERROR_EMPTY_LAYER);
      }
      else
      {
        DrucStructDefine.LAYER_DEST = IndexLayer; 
        DrucStructRdsLayer [ IndexLayer ].FLAGS = DRUC_FLAG_LAYER_LIBRE;
        return;
      }
    }
    IndexLayer ++ ;
  }
  /*\
   *  Le nom existe dans la liste des layers USER en usage
  \*/
  if ( DrucStructDefine.USER_LAYER_NAME_0 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_0, Name ) != (char *)NULL )
    {  
      DrucStructRdsLayer [ DRUC_USER_LAYER_0 ].FLAGS = DRUC_FLAG_LAYER_LIBRE;
      DrucStructDefine.LAYER_DEST        = DRUC_USER_LAYER_0;
      DrucStructDefine.USER_LAYER_0      = DRUC_UNDEFINED_LAYER;
      free ( DrucStructDefine.USER_LAYER_NAME_0 );
      DrucStructDefine.USER_LAYER_NAME_0 = (char *)NULL;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_1 != (char *)NULL )
  {
    if ( strcmp( DrucStructDefine.USER_LAYER_NAME_1, Name ) == 0 )
    {
      DrucStructRdsLayer [ DRUC_USER_LAYER_1 ].FLAGS = DRUC_FLAG_LAYER_LIBRE;
      DrucStructDefine.LAYER_DEST        = DRUC_USER_LAYER_1;
      DrucStructDefine.USER_LAYER_1      = DRUC_UNDEFINED_LAYER;
      free ( DrucStructDefine.USER_LAYER_NAME_1 );
      DrucStructDefine.USER_LAYER_NAME_1 = (char *)NULL;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_2 != (char *)NULL )
  {
    if ( strcmp( DrucStructDefine.USER_LAYER_NAME_2, Name ) == 0 )
    {
      DrucStructRdsLayer [ DRUC_USER_LAYER_2 ].FLAGS = DRUC_FLAG_LAYER_LIBRE;
      DrucStructDefine.LAYER_DEST        = DRUC_USER_LAYER_2;
      DrucStructDefine.USER_LAYER_2      = DRUC_UNDEFINED_LAYER;
      free ( DrucStructDefine.USER_LAYER_NAME_2 );
      DrucStructDefine.USER_LAYER_NAME_2 = (char *)NULL;
      return;
    }
  }
  DRUC_EXIT( DRUC_ERROR_UNDEFINED_LAYER );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucLoadInstructionUndefine                |
|                                                             |
| Ecrit dans DrucStructInstruction         l'instruction      |
| contenue dans DrucStructDefine avec ses valeurs             |
| Seuls le numero de layer user est ecrit                     |
\------------------------------------------------------------*/
void DrucLoadInstructionUndefine ()
{

int TmpIndexMesure;
DrucTypeStructInstruction *TmpStructInstruction;

  TmpStructInstruction =
           ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));

  if ( TmpStructInstruction == (DrucTypeStructInstruction *)NULL
     )
  {
    DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
  }

  TmpStructInstruction->NEXT        = DrucStructInstruction;
  DrucStructInstruction             = TmpStructInstruction;
  TmpStructInstruction->INSTRUCTION = DRUC_ASM_BEGIN_UNDEFINE;
  TmpStructInstruction->LAYER_DEST  = DRUC_UNDEFINED_LAYER;
  TmpStructInstruction->OP_COMPOSE  = DRUC_UNDEFINED_OP_COMPOSE;
  TmpStructInstruction->LAYER_1     = DrucStructDefine.LAYER_DEST;
  TmpStructInstruction->LAYER_2     = DRUC_UNDEFINED_LAYER;
  TmpStructInstruction->OP_UNAIRE   = DRUC_UNDEFINED_OP_COMPOSE;
  TmpStructInstruction->DEF_VALEUR  = 0;


  for ( TmpIndexMesure = 0;
        TmpIndexMesure < DRUC_MAX_MESURES;
        TmpIndexMesure ++
      )
  {
    ( TmpStructInstruction->REGLE_NUM ) [ TmpIndexMesure ] = DRUC_UNDEFINED_REGLE;
    ( TmpStructInstruction->RELATION  ) [ TmpIndexMesure ] = DRUC_UNDEFINED_RELATION;
    ( TmpStructInstruction->MESURE    ) [ TmpIndexMesure ] = DRUC_UNDEFINED_MESURE;
    ( TmpStructInstruction->OP_COMPARE) [ TmpIndexMesure ] = DRUC_UNDEFINED_OPERATEUR;
    ( TmpStructInstruction->VALEUR    ) [ TmpIndexMesure ] = DRUC_UNDEFINED_VALEUR;
  }

  DrucNombreInstructions ++;

  DrucStructDefine.LAYER_DEST = DRUC_UNDEFINED_LAYER;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRegleNumero                        |
|                                                             |
\------------------------------------------------------------*/
void DrucInitRegleNumero ( Value,
                           DrucIndexMesure
                         )
     int Value;
     int DrucIndexMesure;
{
  DataList *TmpListReglNum;

  if ( DrucIndexMesure >= DRUC_MAX_MESURES )
  {
    DRUC_EXIT( DRUC_ERROR_MAX_OPERATOR );
  }
  for( TmpListReglNum  = DrucStructRegle.LIST_REGLE_NUM;
       TmpListReglNum != (DataList *)NULL;
       TmpListReglNum  = TmpListReglNum->NEXT
     )
  {
    if ( TmpListReglNum->REGLE_NUM == Value )
    {
      DRUC_EXIT( DRUC_ERROR_RULE_NUMBER );
    }
  }
  DrucStructRegle.REGLE_NUM [ DrucIndexMesure ] = Value;
  TmpListReglNum                                = DrucStructRegle.LIST_REGLE_NUM;
  DrucStructRegle.LIST_REGLE_NUM                = (DataList *)malloc( sizeof( DataList ));
  ( DrucStructRegle.LIST_REGLE_NUM )->NEXT      = TmpListReglNum;
  ( DrucStructRegle.LIST_REGLE_NUM )->REGLE_NUM = Value;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRegleInstruction                   |
|                                                             |
\------------------------------------------------------------*/
void DrucInitRegleInstruction ( Instruction )
      int Instruction;
{
  DrucStructRegle.INSTRUCTION = Instruction;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRegleRelation                      |
|                                                             |
\------------------------------------------------------------*/
void DrucInitRegleRelation ( Relation,
                             DrucIndexMesure
                           )
     int Relation;
     int DrucIndexMesure;
{
  DrucStructRegle.RELATION [ DrucIndexMesure ] = Relation;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRegleLayerA                        |
|                                                             |
| Verifie que le Name est dans la liste des layers RDS        |
| ou                                                          |
| Verifie que le Name est dans la liste des layers            |
| user deja declares pour DRUC                                |
| ou sort                                                     |
|Range le numero de layer RDS dans DrucStructRegle.LAYER_1    |
\------------------------------------------------------------*/
void DrucInitRegleLayerA ( Name )
     char *Name;
{
  int IndexLayer = 0;

  /*\
   *  Le nom existe dans la liste des layers RDS
  \*/
  while( IndexLayer < RDS_MAX_LAYER )
  {
    if ( strncmp ( RDS_LAYER_NAME[ IndexLayer ],
                   Name ,
                   strlen (Name)
                 )
         == 0
       )
    if ( DrucStructRdsLayer[ IndexLayer ].RDS_INDEX == IndexLayer 
       )
    {
      DrucStructRegle.LAYER_1 = DrucStructRdsLayer[ IndexLayer ].RDS_INDEX;
      return;
    }
    IndexLayer ++ ;
  }
  /*\
   *  Le nom existe dans la liste des layers USER
  \*/
  if ( DrucStructDefine.USER_LAYER_NAME_0 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_0, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_1 = DRUC_USER_LAYER_0;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_1 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_1, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_1 = DRUC_USER_LAYER_1;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_2 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_2, Name ) !=
     (char *)NULL )
    {
      DrucStructRegle.LAYER_1 = DRUC_USER_LAYER_2;
      return;
    }
  }
  DRUC_EXIT( DRUC_ERROR_UNDEFINED_LAYER );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRegleLayerB                        |
|                                                             |
| Verifie que le Name est dans la liste des layers RDS        |
| ou                                                          |
| Verifie que le Name est dans la liste des layers            |
| user deja declares pour DRUC                                |
| ou sort                                                     |
|Range le numero de layer RDS dans DrucStructRegle.LAYER_2    |
\------------------------------------------------------------*/
void DrucInitRegleLayerB ( Name )
     char *Name;
{
  int IndexLayer = 0;

  /*\
   *  Le nom existe dans la liste des layers RDS
  \*/
  while( IndexLayer < RDS_MAX_LAYER )
  {
    if ( strncmp ( RDS_LAYER_NAME[ IndexLayer ], 
                   Name , 
                   strlen (Name) 
                 ) 
         == 0
       )
    if ( DrucStructRdsLayer[ IndexLayer ].RDS_INDEX == IndexLayer 
       ) 
    {
      DrucStructRegle.LAYER_2 = DrucStructRdsLayer[ IndexLayer ].RDS_INDEX;
      return;
    }
    IndexLayer ++ ;
  }
  /*\
   *  Le nom existe dans la liste des layers USER
  \*/
  if ( DrucStructDefine.USER_LAYER_NAME_0 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_0, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_2 = DRUC_USER_LAYER_0;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_1 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_1, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_2 = DRUC_USER_LAYER_1;
      return;
    }
  }
  if ( DrucStructDefine.USER_LAYER_NAME_2 != (char *)NULL )
  {
    if ( strstr( DrucStructDefine.USER_LAYER_NAME_2, Name ) != (char *)NULL )
    {
      DrucStructRegle.LAYER_2 = DRUC_USER_LAYER_2;
      return;
    }
  }
  DRUC_EXIT( DRUC_ERROR_UNDEFINED_LAYER );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRegleMesure                        |
|                                                             |
\------------------------------------------------------------*/
void DrucInitRegleMesure ( Mesure,
                           DrucIndexMesure
                         )
     int Mesure;
     int DrucIndexMesure;
{
  DrucStructRegle.MESURE [ DrucIndexMesure ] = Mesure;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRegleValeur                        |
|                                                             |
\------------------------------------------------------------*/
void DrucInitRegleValeur ( Value,
                           DrucIndexMesure
                         )
     long Value;
     int  DrucIndexMesure;
{
/*\
 * DrucViewString ("DrucInitRegleValeur");
 * DrucViewNumber (Value);
\*/

  if ( ( DrucStructRegle.MESURE [ DrucIndexMesure ] == DRUC_MESURE_SURFACE )      ||  
       ( DrucStructRegle.MESURE [ DrucIndexMesure ] == DRUC_MESURE_SURFACE_INTER )
     )
  {
    Value  = ( RprTranslateParam ( 1.0 ) * Value );
  }
  DrucStructRegle.VALEUR [ DrucIndexMesure ] = Value;
  if ( ( DrucStructRegle.MESURE [ DrucIndexMesure ] == DRUC_MESURE_AXIALE )      ||
       ( DrucStructRegle.MESURE [ DrucIndexMesure ] == DRUC_MESURE_GEODESIQUE )
     )
  {  
/*\
    DrucCtmMax = ( DrucCtmMax > Value ?
                   DrucCtmMax :
                   Value
                 );
 * la ctm max est calculee par le parser 
 * comme ca on peut charger en tete les layers valides 
 * suivis des regles et de leure duplications
\*/
    DrucStructRdsLayer[ DrucStructRegle.LAYER_1 ].CTM = Value;
  }
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitRegleOpCompare                     |
|                                                             |
\------------------------------------------------------------*/
void DrucInitRegleOpCompare ( Operateur,
                              DrucIndexMesure
                            )
     int Operateur;
     int DrucIndexMesure;
{
  DrucStructRegle.OP_COMPARE [ DrucIndexMesure ] = Operateur;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucLoadInstructionRegle                   |
|                                                             |
| Ecrit dans le fichier *TmpFileAssembler, l'instruction      |
| contenue dans DrucStructRegle avec ses valeurs              |
| Stocke le checksumm des ecritures dans DrucCheckSumm        |
| Une instruction de relation est caracterisee par un code    |
| RELATION dans ce cas deux layers sont enregistres au lieu   |
| d'un seul. Les autre valeurs sont toutes enregistrees       |
\------------------------------------------------------------*/
void DrucLoadInstructionRegle ()
{
int TmpIndexMesure;
DrucTypeStructInstruction *TmpStructInstruction;

  TmpStructInstruction =
           ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));

  if ( TmpStructInstruction == (DrucTypeStructInstruction *)NULL
     )
  {
    DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
  }

  TmpStructInstruction->NEXT        = DrucStructInstruction;
  DrucStructInstruction             = TmpStructInstruction;
  TmpStructInstruction->INSTRUCTION = DRUC_ASM_BEGIN_REGLE;
  TmpStructInstruction->LAYER_DEST  = DRUC_UNDEFINED_LAYER;
  TmpStructInstruction->OP_COMPOSE  = DrucStructRegle.INSTRUCTION;
  TmpStructInstruction->LAYER_1     = DrucStructRegle.LAYER_1;
  TmpStructInstruction->LAYER_2     = DrucStructRegle.LAYER_2;
  TmpStructInstruction->OP_UNAIRE   = DRUC_UNDEFINED_OP_COMPOSE;
  TmpStructInstruction->DEF_VALEUR  = 0;

  for ( TmpIndexMesure = 0;
        TmpIndexMesure < DRUC_MAX_MESURES;
        TmpIndexMesure ++
      )
  {
    ( TmpStructInstruction->REGLE_NUM ) [ TmpIndexMesure ] = 
                                DrucStructRegle.REGLE_NUM [ TmpIndexMesure ];
    ( TmpStructInstruction->RELATION  ) [ TmpIndexMesure ] = 
                                DrucStructRegle.RELATION [ TmpIndexMesure ];
    ( TmpStructInstruction->MESURE    ) [ TmpIndexMesure ] = 
                                DrucStructRegle.MESURE [ TmpIndexMesure ];
    ( TmpStructInstruction->OP_COMPARE) [ TmpIndexMesure ] = 
                                DrucStructRegle.OP_COMPARE [ TmpIndexMesure ];
    ( TmpStructInstruction->VALEUR    ) [ TmpIndexMesure ] = 
                                DrucStructRegle.VALEUR [ TmpIndexMesure ];

    DrucStructRegle.REGLE_NUM  [ TmpIndexMesure ] = DRUC_UNDEFINED_REGLE;
    DrucStructRegle.RELATION   [ TmpIndexMesure ] = DRUC_UNDEFINED_RELATION;
    DrucStructRegle.MESURE     [ TmpIndexMesure ] = DRUC_UNDEFINED_MESURE;
    DrucStructRegle.OP_COMPARE [ TmpIndexMesure ] = DRUC_UNDEFINED_OPERATEUR;
    DrucStructRegle.VALEUR     [ TmpIndexMesure ] = DRUC_UNDEFINED_VALEUR;
  }

  DrucStructRegle.LAYER_1 = DRUC_UNDEFINED_LAYER;
  DrucStructRegle.LAYER_2 = DRUC_UNDEFINED_LAYER;
  DrucStructDefine.OP_UNAIRE        = DRUC_UNDEFINED_OP_COMPOSE;
  TmpStructInstruction->DEF_VALEUR  = 0;

  DrucNombreInstructions ++;

  DrucComputeCtm ( TmpStructInstruction
                 );

  if ( DrucStructStat.FLAG_FLATTEN_MODE  == DRUC_HIERARCHIE
     )
  {
    DrucDuplicateHierarchicalRule ( & DrucStructInstruction,
                                    & DrucNombreInstructions
                                  );
  }
}
