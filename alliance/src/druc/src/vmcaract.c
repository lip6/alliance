/*------------------------------------------------------------\
|                                                             |
| Tool    :                                                   |
|                                                             |
| File    :                  VMCARACT.C                       |
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
#include "vmcmesur.h"
#include "vmcaract.h"
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          DrucNotchTeste                     |
|                                                             |
\------------------------------------------------------------*/
boolean DrucNotchTeste ( DrucInstructionCourante,
                         IndexMesure,
                         Mesure
                       )
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;
     long                       Mesure;
{

  switch ( DrucInstructionCourante->OP_COMPARE [ IndexMesure ] )
  {
  case DRUC_OPERATION_INF :
    {
      if ( Mesure >= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        return ( DRUC_FALSE );
      }
      break;
    }
  case DRUC_OPERATION_INFEQ :
    {
      if ( Mesure > DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        return ( DRUC_FALSE );
      }
      break;
    }
  case DRUC_OPERATION_SUP :
    {
      if ( Mesure <= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        return ( DRUC_FALSE );
      }
      break;
    }
  case DRUC_OPERATION_SUPEQ :
    {
      if ( Mesure < DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        return ( DRUC_FALSE );
      }
      break;
    }
  case DRUC_OPERATION_DIFF :
    {
      if ( Mesure == DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        return ( DRUC_FALSE );
      }
      break;
    }
  default :
    {
/*
 *      printf (" DrucNotchTeste merde\n");
 */
    }	
  } /* switch */
  return ( DRUC_TRUE );
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucCaracteriseNotch               |
|                                                             |
\------------------------------------------------------------*/
void DrucCaracteriseNotch ( TmpRectangle,
                            DrucInstructionCourante,
                            IndexMesure
                          )
     rdsrec_list               *TmpRectangle;
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;
{
DrucTypeStructPlusRect *PlusRec;
DrucTypeStructMesure    DrucStructMesure;
rdsrec_list            *RectangleCourant;
long                    PositionX1;
long                    PositionX2;
long                    PositionY1;
long                    PositionY2;
long                    PositionDX1;
long                    PositionDX2;
long                    PositionDY1;
long                    PositionDY2;
long                    DistanceX1DX2;
long                    DistanceX2DX1;
long                    DistanceY1DY2;
long                    DistanceY2DY1;
long                    Mesure;
int                     TmpCas;
boolean                 PerimetreInEqui;

  DrucStructMesure.LAYER_1    = DrucInstructionCourante->LAYER_1;
  DrucStructMesure.LAYER_2    = DrucInstructionCourante->LAYER_1;
  DrucStructMesure.RECTANGLE1 = TmpRectangle;
  PositionX1                  = TmpRectangle->X;
  PositionY1                  = TmpRectangle->Y;
  PositionDX1                 = PositionX1 + TmpRectangle->DX;
  PositionDY1                 = PositionY1 + TmpRectangle->DY;
  PlusRec                     = (DrucTypeStructPlusRect *)
                                  ((char *)(TmpRectangle)+sizeof(rdsrec_list));
  RectangleCourant            = PlusRec->NEXT_EQUI;
  PlusRec                     = (DrucTypeStructPlusRect *)
                               ((char *)(RectangleCourant)+sizeof(rdsrec_list));
  while ( TmpRectangle != RectangleCourant )
  {

    PositionX2  = RectangleCourant->X;
    PositionY2  = RectangleCourant->Y;
    PositionDX2 = PositionX2 + RectangleCourant->DX;
    PositionDY2 = PositionY2 + RectangleCourant->DY;
    if ( PositionDX1 < PositionX2  ||
         PositionDX2 < PositionX1  ||
         PositionDY1 < PositionY2  ||
         PositionDY2 < PositionY1
       )
    {
      DrucStructMesure.BOOLEEN_DISTANCE = DRUC_TRUE;
      DrucStructMesure.RECTANGLE2       = RectangleCourant;
      DistanceX2DX1                     = PositionX2 - PositionDX1;
      DistanceY2DY1                     = PositionY2 - PositionDY1;
      DistanceX1DX2                     = PositionX1 - PositionDX2;
      DistanceY1DY2                     = PositionY1 - PositionDY2;
      Mesure                            = MACRO_MAX ( MACRO_MAX ( DistanceX1DX2 , DistanceX2DX1 ) ,
                                                      MACRO_MAX ( DistanceY1DY2 , DistanceY2DY1 )
                                                    );
      if ( DrucNotchTeste ( DrucInstructionCourante,
                            IndexMesure,
                            Mesure
                          )
           == DRUC_FALSE
         )
      {
        TmpCas = 0;
        if ( PositionX1 >= PositionX2 )
        {
          TmpCas |= 1;
        }
        if ( PositionDX1 <= PositionDX2 )
        {
          TmpCas |= 2;
        }
        if ( PositionY1 >= PositionY2 )
        {
          TmpCas |= 4;
        }
        if ( PositionDY1 <= PositionDY2 )
        {
          TmpCas |= 8;
        }
        DrucStructMesure.TOPOLOGIE = TmpCas;
/*\
 * Pour les cas d'intersection : TmpCas =5 6 9 ou 10
 * il faut tester si les 2 rectangles ont un vis a vis ou si ils sont
 * en coin; dans le cas de vis a vis il faut un rectangle qui recouvre le notch
 * dans le cas de coin il faut soit un rectangle qui recouvre le notch soit 2 rectangles pour recouvrir 2 cotes
 * du perimetre de la surface en erreur
\*/
        switch ( TmpCas )
        {
          case 1:
          {
            DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                          DrucInstructionCourante,
                                          IndexMesure,
                                          PositionDX2,
                                          PositionY2,
                                          PositionX1,
                                          PositionDY2
                                       );
            break;
          }
          case 2:
          {
            DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                          DrucInstructionCourante,
                                          IndexMesure,
                                          PositionDX1,
                                          PositionY2,
                                          PositionX2,
                                          PositionDY2
                                       ); 
            break; 
          }
          case 4:
          { 
            DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                          DrucInstructionCourante,
                                          IndexMesure,
                                          PositionX2,
                                          PositionDY2,
                                          PositionDX2,
                                          PositionY1
                                       ); 
            break; 
          }
          case 5:
          {
            if ( ( PositionDY2 < PositionY1) &&
                 ( PositionDX2 < PositionX1)
               )
            {
              PerimetreInEqui = DrucIsPerimetreInclusInEqui ( &DrucStructMesure,
                                                               DrucInstructionCourante,
                                                               IndexMesure,
                                                               MACRO_MIN ( PositionDX2 , PositionX1 ),
                                                               MACRO_MIN ( PositionDY2 , PositionY1 ),
                                                               MACRO_MAX ( PositionDX2 , PositionX1 ),
                                                               MACRO_MAX ( PositionDY2 , PositionY1 )
                                                            );
              if ( PerimetreInEqui == DRUC_FALSE )
              {
                DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                              DrucInstructionCourante,
                                              IndexMesure,
                                              MACRO_MIN ( PositionDX2 , PositionX1 ),
                                              MACRO_MIN ( PositionDY2 , PositionY1 ),
                                              MACRO_MAX ( PositionDX2 , PositionX1 ),
                                              MACRO_MAX ( PositionDY2 , PositionY1 )
                                           );
              }
            }
            else
            {
              DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                            DrucInstructionCourante,
                                            IndexMesure,
                                            MACRO_MIN ( PositionDX2 , PositionX1 ),
                                            MACRO_MIN ( PositionDY2 , PositionY1 ),
                                            MACRO_MAX ( PositionDX2 , PositionX1 ),
                                            MACRO_MAX ( PositionDY2 , PositionY1 )
                                         );  
            }
            break; 
          }
          case 6:
          {
            if ( ( PositionDY2 < PositionY1) &&
                 ( PositionDX1 < PositionX2)
               )
            {
              PerimetreInEqui = DrucIsPerimetreInclusInEqui ( &DrucStructMesure,
                                                               DrucInstructionCourante,
                                                               IndexMesure,
                                                               MACRO_MIN ( PositionDX1 , PositionX2 ),
                                                               MACRO_MIN ( PositionDY2 , PositionY1 ),
                                                               MACRO_MAX ( PositionDX1 , PositionX2 ),
                                                               MACRO_MAX ( PositionDY2 , PositionY1 )
                                                            );
              if ( PerimetreInEqui == DRUC_FALSE )
              {
                DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                              DrucInstructionCourante,
                                              IndexMesure,
                                              MACRO_MIN ( PositionDX1 , PositionX2 ),
                                              MACRO_MIN ( PositionDY2 , PositionY1 ),
                                              MACRO_MAX ( PositionDX1 , PositionX2 ),
                                              MACRO_MAX ( PositionDY2 , PositionY1 )
                                           );
              }
            }
            else
            {
              DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                            DrucInstructionCourante,
                                            IndexMesure,
                                            MACRO_MIN ( PositionDX1 , PositionX2 ),
                                            MACRO_MIN ( PositionDY2 , PositionY1 ),
                                            MACRO_MAX ( PositionDX1 , PositionX2 ),
                                            MACRO_MAX ( PositionDY2 , PositionY1 )
                                         );
            }
            break;
          }
          case 7:
          {
            DrucIsPositionInclusInEqui ( &DrucStructMesure, 
                                          DrucInstructionCourante, 
                                          IndexMesure, 
                                          PositionX1, 
                                          PositionDY2, 
                                          PositionDX1,
                                          PositionY1
                                       );  
            break;  
          }
          case 8:
          {
            DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                          DrucInstructionCourante,
                                          IndexMesure,
                                          PositionX2,
                                          PositionDY1,
                                          PositionDX2,
                                          PositionY2
                                       ); 
            break; 
          }
          case 9:
          {
            if ( ( PositionDY1 < PositionY2) &&
                 ( PositionDX2 < PositionX1)
               )
            {
              PerimetreInEqui = DrucIsPerimetreInclusInEqui ( &DrucStructMesure,
                                                               DrucInstructionCourante,
                                                               IndexMesure,
                                                               MACRO_MIN ( PositionDX2 , PositionX1 ),
                                                               MACRO_MIN ( PositionDY1 , PositionY2 ),
                                                               MACRO_MAX ( PositionDX2 , PositionX1 ),
                                                               MACRO_MAX ( PositionDY1 , PositionY2 )
                                                            );
              if ( PerimetreInEqui == DRUC_FALSE )
              {
                DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                              DrucInstructionCourante,
                                              IndexMesure,
                                              MACRO_MIN ( PositionDX2 , PositionX1 ),
                                              MACRO_MIN ( PositionDY1 , PositionY2 ),
                                              MACRO_MAX ( PositionDX2 , PositionX1 ),
                                              MACRO_MAX ( PositionDY1 , PositionY2 )
                                           );
              }
            }
            else
            {
              DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                            DrucInstructionCourante,
                                            IndexMesure,
                                            MACRO_MIN ( PositionDX2 , PositionX1 ),
                                            MACRO_MIN ( PositionDY1 , PositionY2 ),
                                            MACRO_MAX ( PositionDX2 , PositionX1 ),
                                            MACRO_MAX ( PositionDY1 , PositionY2 )
                                         );
            }
            break;
          }
          case 10:
          {
            if ( ( PositionDY1 < PositionY2) &&
                 ( PositionDX1 < PositionX2)  
               )
            {
              PerimetreInEqui = DrucIsPerimetreInclusInEqui ( &DrucStructMesure,
                                                               DrucInstructionCourante,
                                                               IndexMesure,
                                                               MACRO_MIN ( PositionDX1 , PositionX2 ),
                                                               MACRO_MIN ( PositionDY1 , PositionY2 ),
                                                               MACRO_MAX ( PositionDX1 , PositionX2 ),
                                                               MACRO_MAX ( PositionDY1 , PositionY2 )
                                                            );
              if ( PerimetreInEqui == DRUC_FALSE )
              {
                DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                              DrucInstructionCourante,
                                              IndexMesure,
                                              MACRO_MIN ( PositionDX1 , PositionX2 ),
                                              MACRO_MIN ( PositionDY1 , PositionY2 ),
                                              MACRO_MAX ( PositionDX1 , PositionX2 ),
                                              MACRO_MAX ( PositionDY1 , PositionY2 )
                                           );
              }
            }
            else
            {
              DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                            DrucInstructionCourante,
                                            IndexMesure,
                                            MACRO_MIN ( PositionDX1 , PositionX2 ),
                                            MACRO_MIN ( PositionDY1 , PositionY2 ),
                                            MACRO_MAX ( PositionDX1 , PositionX2 ),
                                            MACRO_MAX ( PositionDY1 , PositionY2 )
                                         );
            }
            break;
          }
          case 11:
          {
            DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                          DrucInstructionCourante,
                                          IndexMesure,
                                          PositionX1,
                                          PositionDY1,
                                          PositionDX1,
                                          PositionY2
                                       );  
            break;  
          }
          case 13:
          {
            DrucIsPositionInclusInEqui ( &DrucStructMesure,
                                          DrucInstructionCourante,
                                          IndexMesure,
                                          PositionDX2,
                                          PositionY1,
                                          PositionX1,
                                          PositionDY1
                                       );
            break;
          }
          case 14:
          {
            DrucIsPositionInclusInEqui ( &DrucStructMesure, 
                                          DrucInstructionCourante, 
                                          IndexMesure, 
                                          PositionDX1, 
                                          PositionY1, 
                                          PositionX2,
                                          PositionDY1
                                       ); 
            break; 
          }
        }
      }
    }
    RectangleCourant = PlusRec->NEXT_EQUI;
    PlusRec          = (DrucTypeStructPlusRect *)
                            ((char *)(RectangleCourant)+sizeof(rdsrec_list));
  }
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucCaracteriseTeste               |
|                                                             |
\------------------------------------------------------------*/
void DrucCaracteriseTeste ( TmpRectangle,
                            DrucInstructionCourante,
                            IndexMesure,
                            Mesure
                          )
     rdsrec_list               *TmpRectangle;
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;
     long                       Mesure;
{

  switch ( DrucInstructionCourante->OP_COMPARE [ IndexMesure ] )
  {
  case DRUC_OPERATION_INF :
    {
      if ( Mesure >= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurUnCorps ( TmpRectangle,
                            DrucInstructionCourante,
                            IndexMesure
                          );
      }
      break;
    }
  case DRUC_OPERATION_INFEQ :
  case DRUC_OPERATION_MAX :
/*\
 *  a cause de l'unification les 2 sont identiques ici
\*/
    {
      if ( Mesure > DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurUnCorps ( TmpRectangle,
                            DrucInstructionCourante,
                            IndexMesure
                          );
      }
      break;
    }
  case DRUC_OPERATION_SUP :
    {
      if ( Mesure <= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurUnCorps ( TmpRectangle,
                            DrucInstructionCourante,
                            IndexMesure
                          );
      }
      break;
    }
  case DRUC_OPERATION_SUPEQ :
  case DRUC_OPERATION_MIN :
/*\
 *  a cause de l'unification les 2 sont identiques ici
\*/
    {
      if ( Mesure < DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurUnCorps ( TmpRectangle,
                            DrucInstructionCourante,
                            IndexMesure
                          );
      }
      break;
    }
  case DRUC_OPERATION_DIFF :
    {
      if ( Mesure == DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurUnCorps ( TmpRectangle,
                            DrucInstructionCourante,
                            IndexMesure
                          );
      }
      break;
    }
  }
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucCaracteriseTesteInter           |
|                                                             |
\------------------------------------------------------------*/
void DrucCaracteriseTesteSurface ( TmpRectangle,
                                   DrucInstructionCourante,
                                   IndexMesure
                                 )
rdsrec_list *TmpRectangle;
DrucTypeStructInstruction *DrucInstructionCourante;
int                        IndexMesure;

{
DrucTypeStructPlusRect *PlusRec;
DrucTypeStructMesure    DrucStructMesure;
rdsrec_list            *RectangleCourant;

  DrucStructMesure.LAYER_1    = DrucInstructionCourante->LAYER_1;
  DrucStructMesure.LAYER_2    = DrucInstructionCourante->LAYER_1;
  DrucStructMesure.RECTANGLE1 = TmpRectangle;
  PlusRec                     = (DrucTypeStructPlusRect *)((char *)(TmpRectangle)+sizeof(rdsrec_list));
  RectangleCourant            = PlusRec->NEXT_EQUI;
  PlusRec                     = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  while ( TmpRectangle != RectangleCourant )
  {
    if ( DrucIsRectanglesContact ( TmpRectangle,
                                   RectangleCourant
                                 )
         == DRUC_TRUE
       )
    {
      DrucStructMesure.RECTANGLE2 = RectangleCourant;
      DrucRelationMesure ( &DrucStructMesure
                         );
      DrucMesureInter ( &DrucStructMesure,
                         DrucInstructionCourante,
                         IndexMesure,
                         MACRO_SURFACE_INTER_1
                       );
    }
    RectangleCourant = PlusRec->NEXT_EQUI;
    PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  }
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucCaracteriseTesteLargeur         |
|                                                             |
\------------------------------------------------------------*/
void DrucCaracteriseTesteLargeur ( TmpRectangle,
                                   DrucInstructionCourante,
                                   IndexMesure
                                 )
rdsrec_list *TmpRectangle;
DrucTypeStructInstruction *DrucInstructionCourante;
int                        IndexMesure;

{
DrucTypeStructPlusRect *PlusRec;
DrucTypeStructMesure    DrucStructMesure;
rdsrec_list            *RectangleCourant;

  DrucStructMesure.LAYER_1    = DrucInstructionCourante->LAYER_1;
  DrucStructMesure.LAYER_2    = DrucInstructionCourante->LAYER_1;
  DrucStructMesure.RECTANGLE1 = TmpRectangle;
  PlusRec                     = (DrucTypeStructPlusRect *)((char *)(TmpRectangle)+sizeof(rdsrec_list));
  RectangleCourant            = PlusRec->NEXT_EQUI;
  PlusRec                     = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  while ( TmpRectangle != RectangleCourant )
  {
    if ( DrucIsRectanglesContact ( TmpRectangle,
                                   RectangleCourant
                                 )
         == DRUC_TRUE
       )
    {
      DrucStructMesure.RECTANGLE2 = RectangleCourant;
      DrucRelationMesure ( &DrucStructMesure
                         );
      DrucMesureInter ( &DrucStructMesure,
                         DrucInstructionCourante,
                         IndexMesure,
                         MACRO_LARGE_INTER_1
                      );
    }
    RectangleCourant = PlusRec->NEXT_EQUI;
    PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  }
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucCaracteriseTesteLongueur        |
|                                                             |
\------------------------------------------------------------*/
void DrucCaracteriseTesteLongueur ( TmpRectangle,
                                    DrucInstructionCourante,
                                    IndexMesure
                                  )
rdsrec_list *TmpRectangle;
DrucTypeStructInstruction *DrucInstructionCourante;
int                        IndexMesure;

{
DrucTypeStructPlusRect *PlusRec;
DrucTypeStructMesure    DrucStructMesure;
rdsrec_list            *RectangleCourant;

  DrucStructMesure.LAYER_1    = DrucInstructionCourante->LAYER_1;
  DrucStructMesure.LAYER_2    = DrucInstructionCourante->LAYER_1;
  DrucStructMesure.RECTANGLE1 = TmpRectangle;
  PlusRec                     = (DrucTypeStructPlusRect *)((char *)(TmpRectangle)+sizeof(rdsrec_list));
  RectangleCourant            = PlusRec->NEXT_EQUI;
  PlusRec                     = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  while ( TmpRectangle != RectangleCourant )
  {
    if ( DrucIsRectanglesContact ( TmpRectangle,
                                   RectangleCourant
                                 )
         == DRUC_TRUE
       )
    {
      DrucStructMesure.RECTANGLE2 = RectangleCourant;
      DrucRelationMesure ( &DrucStructMesure
                         );
      DrucMesureInter ( &DrucStructMesure,
                         DrucInstructionCourante,
                         IndexMesure,
                         MACRO_LONG_INTER_1
                      );
    }
    RectangleCourant = PlusRec->NEXT_EQUI;
    PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  }
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucCaracterise                    |
|                                                             |
\------------------------------------------------------------*/
void DrucCaracterise ( DrucFigureRds,
                       DrucInstructionCourante
                     )
     rdsfig_list               *DrucFigureRds;
     DrucTypeStructInstruction *DrucInstructionCourante;
{
rdsrec_list *TmpRectangle;
int          IndexMesure;
long         MesureMax;
long         MesureMin;


  for ( TmpRectangle  = DrucFigureRds->LAYERTAB[ DrucInstructionCourante->LAYER_1 ];
        TmpRectangle != (rdsrec_list *)NULL;
        TmpRectangle  = TmpRectangle->NEXT
      )
  {
    MesureMax = ( TmpRectangle->DX > TmpRectangle->DY ?
                  TmpRectangle->DX :
                  TmpRectangle->DY
                );
    MesureMin = ( TmpRectangle->DX < TmpRectangle->DY ?
                  TmpRectangle->DX :
                  TmpRectangle->DY
                );
    IndexMesure = 0;
    while ( ( DrucInstructionCourante->MESURE [ IndexMesure ] != 
              DRUC_UNDEFINED_MESURE
            ) &&
            ( IndexMesure != DRUC_MAX_MESURES
            )
          )
    {
      switch ( DrucInstructionCourante->MESURE [ IndexMesure ]  )
      {
      case DRUC_MESURE_SURFACE_INTER :
        {
          DrucCaracteriseTesteSurface ( TmpRectangle,
                                        DrucInstructionCourante,
                                        IndexMesure
                                      );
          break;
        }
      case DRUC_MESURE_LONGUEUR_INTER :
        {
          DrucCaracteriseTesteLongueur ( TmpRectangle,
                                         DrucInstructionCourante,
                                         IndexMesure
                                       );
          break;
        }
      case DRUC_MESURE_LARGEUR_INTER :
        {
          DrucCaracteriseTesteLargeur ( TmpRectangle,
                                        DrucInstructionCourante,
                                        IndexMesure
                                      );
          break;
        }
      case DRUC_MESURE_LARGEUR :
        {
          DrucCaracteriseTeste ( TmpRectangle,
                                 DrucInstructionCourante,
                                 IndexMesure,
                                 MesureMin
                               );
          break;
        }
      case DRUC_MESURE_LONGUEUR :
        {
          DrucCaracteriseTeste ( TmpRectangle,
                                 DrucInstructionCourante,
                                 IndexMesure,
                                 MesureMax
                               );
          break;
        }
      case DRUC_MESURE_SURFACE :
        {
          DrucCaracteriseTeste ( TmpRectangle,
                                 DrucInstructionCourante,
                                 IndexMesure,
                                 MesureMin * MesureMax
                               );
          break;
        }
      case DRUC_MESURE_NOTCH :
        {
          DrucCaracteriseNotch ( TmpRectangle,
                                 DrucInstructionCourante,
                                 IndexMesure
                               );
          break;
        }
      } /* switch */
      IndexMesure ++;
    } /* while IndexMesure */
  } /* for TmpRectangle */
}
