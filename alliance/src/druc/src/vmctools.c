/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  VMCTOOLS.C                       |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   25/01/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Modifs                              |
| 25/01/94: DrucIsPerimetreInclusInEqui retourne boolean      |
|           ne fait pas de traitement d'erreur                |
|           retourne vrai ou faux                             |
|           lignes 310 a 317                                  |
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
#include "rwi.h"
#include "rut.h"
#include "rtl.h"
#include "rfm.h"
#include "rpr.h"

#include "drucutil.h"
#include "vmctools.h"
#include "vmcerror.h"
/*------------------------------------------------------------\
|                                                             |
|                         Druc Fonctions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          DrucAddSuffixeName                 |
|                                                             |
\------------------------------------------------------------*/
char *DrucAddSuffixeName ( Name,
                           Suffixe
                         )
     char *Name;
     char *Suffixe;
{
char TmpName [DRUC_MAX_STRING_BUFFER];

  strcpy ( (char *)TmpName ,
           Name
           );
  strcat ( (char *)TmpName,
           Suffixe
         );
return ( namealloc ( TmpName ));
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucViewLayers                      |
|                                                             |
\------------------------------------------------------------*/
void DrucViewLayers ()
{
int IndexLayer;

  for ( IndexLayer  = 0;
        IndexLayer  < RDS_MAX_LAYER;
        IndexLayer ++
      )
  {
    fprintf ( stdout ,"LAYER %d\tWIDTH %d\tCTM %d\n",
                      DrucStructRdsLayer [ IndexLayer ].RDS_INDEX,
                      DrucStructRdsLayer [ IndexLayer ].WIDTH,
                      DrucStructRdsLayer [ IndexLayer ].CTM
            );
  }
  fflush ( stdout );
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucInitStructLayers                |
|                                                             |
\------------------------------------------------------------*/
void DrucInitStructLayers ()
{
int IndexLayer;

  for ( IndexLayer  = 0;
        IndexLayer != RDS_MAX_LAYER;
        IndexLayer ++
      )
  {
    DrucStructRdsLayer [ IndexLayer ].RDS_INDEX = DRUC_UNDEFINED_LAYER;
    DrucStructRdsLayer [ IndexLayer ].WIDTH     = DRUC_UNDEFINED_WIDTH;
    DrucStructRdsLayer [ IndexLayer ].CTM       = DRUC_UNDEFINED_CTM;
    DrucStructRdsLayer [ IndexLayer ].FLAGS     = DRUC_UNDEFINED_FLAGS;
  }
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucViewInstruction                |
|                                                             |
\------------------------------------------------------------*/
void DrucViewInstruction ( DrucInstructionCourante 
                         )
     DrucTypeStructInstruction *DrucInstructionCourante;
{
int IndexMesure;

  fprintf ( stdout , "INSTRUCTION       \t%d\n", 
                     DrucInstructionCourante->INSTRUCTION );
  fprintf ( stdout , "LAYER DESTINATION \t%s %d\n", 
                     RDS_LAYER_NAME[ DrucInstructionCourante->LAYER_DEST ],
                     DrucInstructionCourante->LAYER_DEST );
  fprintf ( stdout , "LAYER SOURCE 1    \t%s %d\n", 
                     RDS_LAYER_NAME[ DrucInstructionCourante->LAYER_1 ],
                     DrucInstructionCourante->LAYER_1 );
  fprintf ( stdout , "LAYER SOURCE 2    \t%s %d\n", 
                     RDS_LAYER_NAME[ DrucInstructionCourante->LAYER_2 ],
                     DrucInstructionCourante->LAYER_2 );
/*\
  fprintf ( stdout , "LAYER DESTINATION \t%d\n", 
                     DrucInstructionCourante->LAYER_DEST );
  fprintf ( stdout , "lAYER SOURCE 1    \t%d\n", 
                     DrucInstructionCourante->LAYER_1 );
  fprintf ( stdout , "LAYER SOURCE 2    \t%d\n", 
                     DrucInstructionCourante->LAYER_2 );
\*/
  fprintf ( stdout , "REL du CONSTRUCTEUR\t%d\n", 
                     DrucInstructionCourante->OP_COMPOSE );
  fprintf ( stdout , "OPERATION UNAIRE   \t%d\n", 
                     DrucInstructionCourante->OP_UNAIRE );
  fprintf ( stdout , "VALEUR    UNAIRE   \t%ld\n", 
                     DrucInstructionCourante->DEF_VALEUR );
  for ( IndexMesure = 0;
        IndexMesure < DRUC_MAX_MESURES;
        IndexMesure ++
      )
  {
    if ( ( ( DrucInstructionCourante->REGLE_NUM ) [ IndexMesure ] )
         != DRUC_UNDEFINED_REGLE
       )
    {
      fprintf ( stdout , "REGLE NUMERO %d\t", 
                     ( DrucInstructionCourante->REGLE_NUM ) [ IndexMesure ] );
      fprintf ( stdout , "OPERATEUR %d\t", 
                     ( DrucInstructionCourante->RELATION ) [ IndexMesure ] );
      fprintf ( stdout , "MESURE %d\t", 
                     ( DrucInstructionCourante->MESURE ) [ IndexMesure ] );
      fprintf ( stdout , "COMPARATEUR %d\t", 
                     ( DrucInstructionCourante->OP_COMPARE ) [ IndexMesure ] );
      fprintf ( stdout , "VALEUR %d\n", 
                     ( DrucInstructionCourante->VALEUR ) [ IndexMesure ] );
    }
  }
DrucViewString ("");
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucViewRectangle                   |
|                                                             |
\------------------------------------------------------------*/
void DrucViewRectangle ( Rectangle
                       )
     rdsrec_list **Rectangle;
{
DrucTypeStructPlusRect *PlusRec;

    PlusRec = (DrucTypeStructPlusRect *)
                       ((char*)(*Rectangle)+sizeof(rdsrec_list));
    printf ( "RECTANGLE \n");
    printf ( "%p\t" , 
              *Rectangle);
    printf ( "%p  " , 
              (*Rectangle)->NEXT);
    printf ( "\t%.1f\t"   , 
              (*Rectangle)->X / (float)RDS_LAMBDA );
    printf ( "%.1f\t"   , 
              (*Rectangle)->Y / (float)RDS_LAMBDA );
    printf ( "%.1f\t"   , 
              (*Rectangle)->DX / (float)RDS_LAMBDA);
    printf ( "%.1f\t" , 
              (*Rectangle)->DY / (float)RDS_LAMBDA);
    printf ( "INSTANCE %p\t",
              PlusRec->INSTANCE_NUM);
    printf ( "EQUI %p\t",
              PlusRec->NEXT_EQUI);
    printf ( "%p\t" , 
              (*Rectangle)->USER );
    printf ( "%lx\t\n" , 
              (*Rectangle)->FLAGS );
    fflush (stdout);
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucViewEqui                        |
|                                                             |
\------------------------------------------------------------*/
void DrucViewEqui ( Rectangle )
    rdsrec_list **Rectangle;
{
DrucTypeStructPlusRect *PlusRec1;
rdsrec_list            *RectangleCourant;
 
  RectangleCourant         = *Rectangle;
  PlusRec1                 = (DrucTypeStructPlusRect *)
                               ((char *)(RectangleCourant)+sizeof(rdsrec_list));
fprintf ( stdout,
          "\nEqui:\n"
        );
  while ( PlusRec1->NEXT_EQUI != *Rectangle
        )
  {
    RectangleCourant->FLAGS |= DRUC_FLAG_REC_VISITE;

    DrucViewRectangle (  & RectangleCourant
                      );
    RectangleCourant     = PlusRec1->NEXT_EQUI;
    PlusRec1             = (DrucTypeStructPlusRect *)
                               ((char *)(RectangleCourant)+sizeof(rdsrec_list));
  }
  RectangleCourant->FLAGS |= DRUC_FLAG_REC_VISITE;
  DrucViewRectangle (  & RectangleCourant
                    );
fprintf ( stdout,
          "\nFin Equi:\n"
        );
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucIsPerimetreInclusInEqui        |
| pour bien faire il faudrait calculer le vecteur separant    |
| les deux rectangles en faute puis couvrir ce vecteur par    |
| le balayage de l'equi et eliminer par morceaux ce vecteur   |
| avec chaque rectangle de l'equi qui recouvre ces morceaux   |
|                                                             |
\------------------------------------------------------------*/
boolean DrucIsPerimetreInclusInEqui ( DrucStructMesure,
                                      DrucInstructionCourante,
                                      IndexMesure,
                                      PositionX,
                                      PositionY,
                                      PositionDX,
                                      PositionDY
                                    )
DrucTypeStructMesure      *DrucStructMesure;
DrucTypeStructInstruction *DrucInstructionCourante;
int                        IndexMesure;
long                       PositionX;
long                       PositionY;
long                       PositionDX;
long                       PositionDY;
{
DrucTypeStructPlusRect *PlusRec;
rdsrec_list            *RectangleTete;
rdsrec_list            *RectangleCourant;
boolean                 PerimetreXCouvert = DRUC_FALSE;
boolean                 PerimetreYCouvert = DRUC_FALSE;

/*
printf ("NOTCH %d %d %d %d \n",PositionX,PositionY,PositionDX,PositionDY);
*/
  RectangleTete    = DrucStructMesure->RECTANGLE1;
  PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleTete)+sizeof(rdsrec_list));
  RectangleCourant = PlusRec->NEXT_EQUI;
  PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  while ( RectangleCourant != RectangleTete )
  {
    if ( RectangleCourant == DrucStructMesure->RECTANGLE2 )
    {
      RectangleCourant = PlusRec->NEXT_EQUI;
      PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
      continue;
    }
    /*\
     *  couverture du perimetre vertical gauche avec un rectangle a gauche  droit avec un rectangle a droite
     *  il faut qu'un cote vertical et un cote horizontal soient recouverts par un rectangle du polygone
    \*/
    if ( ( RectangleCourant->Y                        <= PositionY  ) &&
         ( RectangleCourant->Y + RectangleCourant->DY >= PositionDY ) 
       )
    {
      if ( ( ( RectangleCourant->X                        <= PositionDX ) &&
             ( RectangleCourant->X + RectangleCourant->DX >= PositionDX )
           ) ||
           ( ( RectangleCourant->X                        <= PositionX  ) &&
             ( RectangleCourant->X + RectangleCourant->DX >= PositionX  )
           )
         )
      {
        PerimetreYCouvert = DRUC_TRUE;
      }
    }
    /*\
     *  couverture du perimetre horizontal 
     *  haut avec un rectangle en haut
     *  bas avec un rectangle en bas
    \*/
    if ( ( RectangleCourant->X                        <= PositionX  ) &&
         ( RectangleCourant->X + RectangleCourant->DX >= PositionDX )  
       ) 
    {  
      if ( ( ( RectangleCourant->Y                        <= PositionDY ) &&
             ( RectangleCourant->Y + RectangleCourant->DY >= PositionDY ) 
           ) || 
           ( ( RectangleCourant->Y                        <= PositionY  ) &&
             ( RectangleCourant->Y + RectangleCourant->DY >= PositionY  ) 
           )
         ) 
      {  
        PerimetreXCouvert = DRUC_TRUE; 
      } 
    } 
    if ( ( PerimetreXCouvert == DRUC_TRUE  ) &&
         ( PerimetreYCouvert == DRUC_TRUE  ) 
       )
    {
      return ( DRUC_TRUE 
             );
    }
    RectangleCourant = PlusRec->NEXT_EQUI;
    PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  }
  return ( DRUC_FALSE 
         );
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucIsPositionInclusInEqui         |
|                                                             |
\------------------------------------------------------------*/
void DrucIsPositionInclusInEqui ( DrucStructMesure,
                                  DrucInstructionCourante,
                                  IndexMesure,
                                  PositionX,
                                  PositionY,
                                  PositionDX,
                                  PositionDY
                                )
DrucTypeStructMesure      *DrucStructMesure;
DrucTypeStructInstruction *DrucInstructionCourante;
int                        IndexMesure;
long                       PositionX;
long                       PositionY;
long                       PositionDX;
long                       PositionDY;
{
DrucTypeStructPlusRect *PlusRec;
rdsrec_list            *RectangleTete;
rdsrec_list            *RectangleCourant;

/*
printf ("NOTCH %d %d %d %d \n",PositionX,PositionY,PositionDX,PositionDY);
*/
  RectangleTete    = DrucStructMesure->RECTANGLE1;
  PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleTete)+sizeof(rdsrec_list));
  RectangleCourant = PlusRec->NEXT_EQUI;
  PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  while ( RectangleCourant != RectangleTete )
  {
    if ( RectangleCourant == DrucStructMesure->RECTANGLE2 )
    {
      RectangleCourant = PlusRec->NEXT_EQUI;
      PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
      continue;
    }
    if ( ( RectangleCourant->X  <= PositionX  ) &&
         ( RectangleCourant->Y  <= PositionY  ) &&
         ( RectangleCourant->X + RectangleCourant->DX >= PositionDX ) &&
         ( RectangleCourant->Y + RectangleCourant->DY >= PositionDY )
       )
    {
      return;
    }
    RectangleCourant = PlusRec->NEXT_EQUI;
    PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  }
  DrucErreurDeuxCorps ( DrucStructMesure,
                        DrucInstructionCourante,
                        IndexMesure
                      );
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucIsRectanglesContact             |
|                                                             |
\------------------------------------------------------------*/
boolean DrucIsRectanglesContact ( Rectangle1,
                                  Rectangle2
                                )
    rdsrec_list *Rectangle1;
    rdsrec_list *Rectangle2;
{
long PositionX1;
long PositionX2;
long PositionY1;
long PositionY2;
long PositionDX1;
long PositionDX2;
long PositionDY1;
long PositionDY2;

  PositionX1  = Rectangle1->X;
  PositionX2  = Rectangle2->X;
  PositionY1  = Rectangle1->Y;
  PositionY2  = Rectangle2->Y;
  PositionDX1 = PositionX1 + Rectangle1->DX;
  PositionDX2 = PositionX2 + Rectangle2->DX;
  PositionDY1 = PositionY1 + Rectangle1->DY;
  PositionDY2 = PositionY2 + Rectangle2->DY;
  if ( PositionDX1 < PositionX2  ||
       PositionDX2 < PositionX1  ||
       PositionDY1 < PositionY2  ||
       PositionDY2 < PositionY1
     )
  {
    return ( DRUC_FALSE );
  }
  else
  {
    return ( DRUC_TRUE );
  }
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucChercheWindow                  |
|A partir d'un rectangle retourne la liste des fenetres       |
|  qui embrasse ce rectangle                                  |
\------------------------------------------------------------*/
rdsrecwin_list *DrucChercheWindow ( TmpRectangleSource
                                  )
     rdsrec_list               *TmpRectangleSource;
{
static rdsrecwin_list StaticWindowSource2;

  if ( IsRdsOneWindow( TmpRectangleSource ) )
  {
    StaticWindowSource2.WINDOW =  (rdswin_list *)TmpRectangleSource->USER;
    StaticWindowSource2.NEXT   =  (rdsrecwin_list *) NULL;
    return ( & StaticWindowSource2 );
  }
  else
  {
    return (rdsrecwin_list *)TmpRectangleSource->USER;
  }
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucComputeEquiRectangle            |
| cree les equipotentielles du layer courant                  |
\------------------------------------------------------------*/
void DrucComputeEquiRectangle ( DrucRectangle,
                                LayerCourant
                              )
     register  rdsrec_list *DrucRectangle;
     register  int          LayerCourant;
{
register int             TabIndex;
register rdsrec_list    *TmpRectangleCourant;
register rdsrecwin_list *TmpWindowCourante;
register rdswinrec_list *TabRectangleCourant;

  /*\
   *  recuperer toutes les fenetres liees au rectangle
  \*/
  TmpWindowCourante = DrucChercheWindow ( DrucRectangle 
                                        );
  /*\
   * parcours de chaque fenetre
  \*/
  while ( TmpWindowCourante != (rdsrecwin_list *)NULL )
  {
    TabRectangleCourant = ( TmpWindowCourante->WINDOW )->LAYERTAB [ LayerCourant ];
    /*\
     * parcours des tableaux de la fenetre courante
    \*/
    while ( TabRectangleCourant != (rdswinrec_list *)NULL )
    {
      /*\
       * parcourir tous les rectangles tableau courant
      \*/
      TabIndex = 0;
      while ( TabIndex < RWI_MAX_REC
            )
      {
        if ( ( TabRectangleCourant->RECTAB) [ TabIndex ] != 
               ( rdsrec_list *)NULL )
        {
          TmpRectangleCourant = TabRectangleCourant->RECTAB [ TabIndex ];
          if (  ( TmpRectangleCourant != (rdsrec_list*) NULL ) &&
                ( (TmpRectangleCourant->FLAGS & DRUC_FLAG_REC_VISITE ) !=
                  DRUC_FLAG_REC_VISITE )
             )
          {
            /*\
             *  si les rectangles sont en intersection on les chaine par les equi
            \*/
            if ( DrucIsRectanglesContact ( DrucRectangle,
                                           TmpRectangleCourant
                                         )
                 == DRUC_TRUE
               )
            {
              TmpRectangleCourant->FLAGS |= DRUC_FLAG_REC_VISITE;
              MACRO_NEXT_EQUI ( TmpRectangleCourant ) = MACRO_NEXT_EQUI ( DrucRectangle );
              MACRO_NEXT_EQUI ( DrucRectangle )       = TmpRectangleCourant;
            } /* if intersection */
          } /* if TmpRectangle */
        }

        TabIndex ++;
      } /* while les rectangles du tableau courant */
      TabRectangleCourant = TabRectangleCourant->NEXT;
    } /* while les tableaux  */
    TmpWindowCourante = TmpWindowCourante->NEXT;
  } /* while les fenetres */
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucComputeEqui                     |
| cree les equipotentielles du layer courant                  |
\------------------------------------------------------------*/
void DrucComputeEquiLayer ( DrucFigureRds ,
                            LayerCourant
                          )
    rdsfig_list *DrucFigureRds;
    int          LayerCourant;
{
register rdsrec_list    *TmpRectangleCourant;
register rdsrec_list    *TmpRectangleEqui;

  for ( TmpRectangleCourant  = DrucFigureRds->LAYERTAB [ LayerCourant ];
        TmpRectangleCourant != (rdsrec_list *)NULL;
        TmpRectangleCourant  = TmpRectangleCourant->NEXT
      )
  {
    if ( (TmpRectangleCourant->FLAGS & DRUC_FLAG_REC_VISITE ) ==
         DRUC_FLAG_REC_VISITE
       )
    {
      continue;
    }
    TmpRectangleCourant->FLAGS |= DRUC_FLAG_REC_VISITE;
    MACRO_NEXT_EQUI ( TmpRectangleCourant ) = TmpRectangleCourant;
    DrucComputeEquiRectangle ( TmpRectangleCourant,
                               LayerCourant
                             );
    /*\
     *  pour tous les rectangles de l'equi en cours chercher la suite de l'equi
    \*/
    TmpRectangleEqui = MACRO_NEXT_EQUI ( TmpRectangleCourant );
    while ( TmpRectangleEqui != TmpRectangleCourant )
    {
      DrucComputeEquiRectangle ( TmpRectangleEqui,
                                 LayerCourant
                               );
      TmpRectangleEqui = MACRO_NEXT_EQUI ( TmpRectangleEqui );
    } /* while les rectangles de l'equi en cours */
    TmpRectangleEqui = MACRO_NEXT_EQUI ( TmpRectangleCourant );
  } /* for le parcours des rectangles du layer */
  DrucClearFlag ( DrucFigureRds->LAYERTAB [ LayerCourant ],
                  DRUC_FLAG_REC_VISITE
                );
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucComputeEquiFigure              |
|                                                             |
\------------------------------------------------------------*/
void DrucComputeEquiFigure ( DrucFigureRds
                           )
     rdsfig_list *DrucFigureRds;
{
int IndexLayer;
int LayerCourant;

  for ( IndexLayer = 0;
        IndexLayer < RDS_MAX_LAYER ;
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
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucIsRectangleEquipot             |
|                                                             |
\------------------------------------------------------------*/
boolean DrucIsRectangleEquipot ( DrucStructMesure
                               )
     DrucTypeStructMesure  *DrucStructMesure;
{
rdsrec_list            *RectangleTete;
rdsrec_list            *RectangleCourant;
DrucTypeStructPlusRect *PlusRec;

  if ( DrucStructMesure->RECTANGLE1 == DrucStructMesure->RECTANGLE2 )
  {
    return ( DRUC_TRUE );
  }
  RectangleTete    = DrucStructMesure->RECTANGLE1;
  PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleTete)+sizeof(rdsrec_list));
  RectangleCourant = PlusRec->NEXT_EQUI;
  PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  while ( RectangleCourant != RectangleTete )
  {
    if ( RectangleCourant == DrucStructMesure->RECTANGLE2 )
    {
      return ( DRUC_TRUE );
    }
      RectangleCourant = PlusRec->NEXT_EQUI;
      PlusRec          = (DrucTypeStructPlusRect *)((char *)(RectangleCourant)+sizeof(rdsrec_list));
  }
  return ( DRUC_FALSE );
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucIsRectanglesValides            |
| si les deux rectangles appartiennent a des couronnes        |
|    alors tester si ils appartiennent a la meme couronne     |
|      si ils appartiennent a la meme couronne                |
|         alors inutile de verifier la regle courante         |
\------------------------------------------------------------*/
boolean DrucIsRectanglesValides ( DrucStructMesure,
                                  DrucInstructionCourante
                                )
    DrucTypeStructMesure *DrucStructMesure;
    DrucTypeStructInstruction *DrucInstructionCourante;
{
  /*\
   * doit aussi mesurer la ctm en fonction des l'operations courantes
   * contenues dans les tableaux de DrucStructMesure
   * si c'est une relation de superposition
   *    les deux rectangles qui ne se touchent pas ne sont pas valides
   * si c'est la relation distance tenir compte de la ctm max des deux rectangles
   * et tenir compte de l'appartenance des @ rectangles a la meme composition de layers
   * en cas de layer user
  if ( ( (DrucStructMesure->RECTANGLE1)->FLAGS &
         (DrucStructMesure->RECTANGLE2)->FLAGS &
         DRUC_FLAG_INSTANCE_RECTANGLE            ) ==
       DRUC_FLAG_INSTANCE_RECTANGLE
     )
  \*/
  {
/*\
DrucViewRectangle ( &(DrucStructMesure->RECTANGLE1) );
viewrdsrec (DrucStructMesure->RECTANGLE1);
viewrdsrec (DrucStructMesure->RECTANGLE2);
DrucViewRectangle ( &(DrucStructMesure->RECTANGLE2) );
\*/
    if ( ( MACRO_INSTANCE_NUM ( DrucStructMesure->RECTANGLE1 ) != 
           (rdsins_list *)NULL 
         ) &&
         ( ( MACRO_INSTANCE_NUM ( DrucStructMesure->RECTANGLE1 ) ) ==
           ( MACRO_INSTANCE_NUM ( DrucStructMesure->RECTANGLE2 ) )
         )
       )
    {
      return ( DRUC_FALSE );
    }
  }
  DrucStructMesure->BOOLEEN_DISTANCE = !( DrucIsRectanglesContact
                                          (
                                            DrucStructMesure->RECTANGLE1,
                                            DrucStructMesure->RECTANGLE2
                                          )
                                        );
  return ( DRUC_TRUE );
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucSetEquiFlag                     |
| positionne le bit du champ FLAG de tous les rectangles      |
| de l'equi du rectangle                                      |
| le bit a positionner   est indique par le parametre flag    |
\------------------------------------------------------------*/
void DrucSetEquiFlag ( Rectangle,
                       Flag
                     )
     register rdsrec_list               *Rectangle;
     register long                       Flag;
{
register rdsrec_list *TmpEqui  = Rectangle;

  do
  {
    TmpEqui->FLAGS |= Flag;
    TmpEqui         = MACRO_NEXT_EQUI ( TmpEqui );
  }
  while ( Rectangle != TmpEqui );
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucClearEquiFlag                   |
| efface     le bit du champ FLAG de tous les rectangles      |
| de l'equi du rectangle                                      |
| le bit a effacer       est indique par le parametre flag    |
\------------------------------------------------------------*/
void DrucClearEquiFlag ( Rectangle,
                         Flag
                       )
     register rdsrec_list               *Rectangle;
     register long                       Flag;
{
register rdsrec_list *TmpEqui  = Rectangle;

  do
  {
    TmpEqui->FLAGS &= ~( Flag );
    TmpEqui = MACRO_NEXT_EQUI ( TmpEqui );
  }
  while ( Rectangle != TmpEqui );
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucClearFlag                       |
| met a zero le bit du champ FLAG de tous les rectangles      |
| le bit a mettre a zero est indique par le parametre flag    |
\------------------------------------------------------------*/
void DrucClearFlag ( RectangleListe,
                     Flag
                   )
     register rdsrec_list               *RectangleListe;
     register long                       Flag;
{
register rdsrec_list *TmpRectangle;

  for ( TmpRectangle  = RectangleListe;
        TmpRectangle != (rdsrec_list *)NULL;
        TmpRectangle  = TmpRectangle->NEXT
      )
  {
    TmpRectangle->FLAGS &= ~( Flag );
  }
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucAddRecFigure                    |
| insert le rectangle (initialise) dans la liste des          |
| rectangles du layer donne                                   |
\------------------------------------------------------------*/
void DrucAddRecFigure ( RectangleDest ,
                        DrucFigureRds ,
                        LayerDest
                      )
     rdsrec_list *RectangleDest;
     rdsfig_list *DrucFigureRds;
     int          LayerDest;
{
  RectangleDest->NEXT                   = DrucFigureRds->LAYERTAB [ LayerDest ];
  DrucFigureRds->LAYERTAB [ LayerDest ] = RectangleDest;
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucDestroyLayerWindow              |
|                                                             |
\------------------------------------------------------------*/
void DrucDestroyLayerWindow ( DrucFigureRds,
                              DrucRdsWindow,
                              LayerCourant
                            )
     rdsfig_list *DrucFigureRds;
     rdswindow   *DrucRdsWindow;
     int          LayerCourant;
{
rdsrec_list *RectangleCourant;

/*\
DrucViewString ("\nDESTRUCTION DES FENETRES DU LAYER DEST\n"
               );
viewrdsfig     ( DrucFigureRds
               );
\*/

  RectangleCourant = DrucFigureRds->LAYERTAB[ LayerCourant ];
  while ( RectangleCourant != ( rdsrec_list * )NULL )
  {
    delrdsrecwindow ( RectangleCourant,
                      DrucRdsWindow
                    );
    RectangleCourant       = RectangleCourant->NEXT;
  }
/*\
DrucViewString ("\nFIN DESTRUCTION DES FENETRES DU LAYER DEST\n"
               );
\*/
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucBuiltLayerWindow                |
|                                                             |
\------------------------------------------------------------*/
void DrucBuiltLayerWindow ( DrucFigureRds,
                            DrucRdsWindow,
                            LayerCourant
                          )
 
     rdsfig_list *DrucFigureRds;
     rdswindow   *DrucRdsWindow;
     int          LayerCourant;
{
rdsrec_list *RectangleCourant;

/*\
DrucViewString ("\nCONSTRUCTION DES FENETRES DU LAYER DEST\n"
               );
viewrdsfig     ( DrucFigureRds
               );
\*/
  for ( RectangleCourant  = DrucFigureRds->LAYERTAB[ LayerCourant ];
        RectangleCourant != (rdsrec_list *)NULL;
        RectangleCourant  = RectangleCourant->NEXT 
      )
  {
    addrdsrecwindow( RectangleCourant, DrucRdsWindow );

/*\
DrucViewString ("\nCONSTRUCTION EN COURS DES FENETRES DU LAYER DEST\n"
               );
\*/
  }
/*\
DrucViewString ("\nFIN CONSTRUCTION DES FENETRES DU LAYER DEST\n"
               );
\*/
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucVerifEquiLayer                  |
|                                                             |
\------------------------------------------------------------*/
void DrucVerifEquiLayer ( DrucFigureRds,
                          Layer 
                        )
     rdsfig_list *DrucFigureRds;
     int          Layer;
{
rdsrec_list *TmpRectangle;

  DrucViewString ( "\n"
                 );
  DrucViewString ( RDS_LAYER_NAME[Layer] 
                 );
  DrucViewString ( "\n"
                 );

  TmpRectangle = DrucFigureRds->LAYERTAB[Layer];
  while ( TmpRectangle != (rdsrec_list *)NULL
        )
  {
    DrucViewString ("RECTANGLE COURANT PARCOURU PAR NEXT ");
    DrucViewRectangle ( & TmpRectangle
                      );
    if ( ( TmpRectangle->FLAGS & DRUC_FLAG_REC_VISITE ) ==
                                 DRUC_FLAG_REC_VISITE
       )
    {
      TmpRectangle = TmpRectangle->NEXT;
      continue;
    }
    if ( MACRO_NEXT_EQUI ( TmpRectangle ) == (rdsrec_list *)NULL
       )
    {
      DrucViewRectangle ( & TmpRectangle );
      DrucViewString (" et un rectangle qui n'est pas chaine!");
      DrucViewRectangle ( & TmpRectangle );
    }
    else
    {
      DrucViewEqui ( & TmpRectangle
                   );
    }
    TmpRectangle = TmpRectangle->NEXT;
  }
  DrucClearFlag ( DrucFigureRds->LAYERTAB [ Layer ],
                  DRUC_FLAG_REC_VISITE
                );

printf ("\n layer verifie\n");
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucVerifEquiFigure                 |
|                                                             |
\------------------------------------------------------------*/
void DrucVerifEquiFigure ( DrucFigureRds
                         )
     rdsfig_list *DrucFigureRds;
{
int Layer;

  DrucViewString ("Verif de la figure ");
  DrucViewString (DrucFigureRds->NAME);
  for ( Layer  = 0;
        Layer  < RDS_MAX_LAYER;
        Layer ++
      )
  {
  DrucViewString ("Verif des chainages du layer ");
    DrucVerifEquiLayer ( DrucFigureRds,
                         Layer
                       );
  }
}
