/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  VMCUNIFY.C                       |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
| evolutions : see at the end of the file                     |
|                                                             |
|                                                             |
|                                                             |
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
#include "vmcunify.h"
 
/*------------------------------------------------------------\
|                                                             |
|                         Fonctions                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrucUnifyDeleteEqui                 |
|                                                             |
\------------------------------------------------------------*/
 
void DrucUnifyDeleteEqui ( RectangleSource
                         )
     rdsrec_list *RectangleSource;
 
{
 
  while ( RectangleSource != ( rdsrec_list *)NULL 
        )
  {
    MACRO_NEXT_EQUI ( RectangleSource ) = ( rdsrec_list *)NULL;
    RectangleSource = RectangleSource->NEXT;
  }
}
 
/*------------------------------------------------------------\
|                                                             |
|                         DrucChaineEquiRectqngle             |
|                                                             |
\------------------------------------------------------------*/

void DrucChaineEquiRectangle ( DrucNouveauRectangle,
                               Rectangle1
                             )
     rdsrec_list *DrucNouveauRectangle;
     rdsrec_list *Rectangle1;

{

  MACRO_NEXT_EQUI (DrucNouveauRectangle ) = MACRO_NEXT_EQUI ( Rectangle1 );
  MACRO_NEXT_EQUI (Rectangle1 )           = DrucNouveauRectangle;
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucUnifyRemoveRectanglesEqui       |
|                                                             |
\------------------------------------------------------------*/

void DrucUnifyRemoveRectanglesEqui ( RectangleSource,
                                     DrucRdsWindow,
                                     Width
                                  )
     rdsrec_list **RectangleSource;
     rdswindow   *DrucRdsWindow;
     long          Width;


{

rdsrec_list *RectanglePrecedant;
rdsrec_list *RectangleSuivant;
rdsrec_list *RectangleOrigine;

  RectangleOrigine  = *RectangleSource;
  /*\
   *  supprimer tous les rectangles de tete de liste qui sont a detruire
  \*/
  while ( ( RectangleOrigine != (rdsrec_list *)NULL
          ) &&
          ( ( ( RectangleOrigine->FLAGS & 
                DRUC_FLAG_REMOVE 
              ) == 
              DRUC_FLAG_REMOVE 
            ) ||
            ( ( RectangleOrigine->DX < Width 
              ) ||
              ( RectangleOrigine->DY < Width 
              )    
            )
          )
        )
  {
    *RectangleSource  = RectangleOrigine->NEXT;

    delrdsrecwindow ( RectangleOrigine,
                      DrucRdsWindow
                    );

    rdsfreeheap ( RectangleOrigine ,
              ( sizeof ( rdsrec_list ) + MACRO_PLUS_TAILLE )
            );

    RectangleOrigine = *RectangleSource;
  }

  RectanglePrecedant = *RectangleSource;
  /*\
   *  puis tous les rectangles apres la tete de liste qui sont a detruire
  \*/

  if ( RectanglePrecedant != (rdsrec_list *)NULL
     )
  {
    do
    {
      RectangleSuivant  = RectanglePrecedant->NEXT;
  
      if ( RectangleSuivant != (rdsrec_list *)NULL 
         )
      {
        if ( ( ( RectangleSuivant->FLAGS & DRUC_FLAG_REMOVE 
               ) == 
               DRUC_FLAG_REMOVE 
             ) ||
             ( ( RectangleSuivant->DX < Width 
               ) ||
               ( RectangleSuivant->DY < Width 
               )    
             )
           )
        {
          RectanglePrecedant->NEXT = RectangleSuivant->NEXT;
  
  
          delrdsrecwindow ( RectangleSuivant,
                            DrucRdsWindow
                          );
          rdsfreeheap ( RectangleSuivant ,
                    sizeof ( rdsrec_list ) + MACRO_PLUS_TAILLE
                  );
  
          RectangleSuivant = RectanglePrecedant->NEXT;
        }
        else
        {
          RectanglePrecedant  = RectangleSuivant;
        }
      }
    }
    while ( RectanglePrecedant->NEXT != (rdsrec_list *)NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucCreateRectangle                 |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *DrucCreateRectangle ( X,
                                   Y,
                                   DX,
                                   DY,
                                   Flag
                                 )
   long         X;
   long         Y;
   long         DX;
   long         DY;
   int          Flag;

{

rdsrec_list * TmpRectangleDest;

  TmpRectangleDest         = allocrdsrec( sizeof ( DrucTypeStructPlusRect ));
  TmpRectangleDest->X      = X ;
  TmpRectangleDest->Y      = Y ;
  TmpRectangleDest->DX     = DX ;
  TmpRectangleDest->DY     = DY ;
  TmpRectangleDest->FLAGS |=Flag;

  return ( TmpRectangleDest
         );
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucUnifyTransfertInfo              |
| recopie les infos a garder du rectangle1 sur le rectangle 2 |
\------------------------------------------------------------*/

void DrucUnifyTransfertInfo ( TmpRectangle1,
                              TmpRectangle2
                            )
     rdsrec_list *TmpRectangle1;
     rdsrec_list *TmpRectangle2;

{

  if ( TmpRectangle2->NAME == (char *)NULL 
     )
  {
    if ( TmpRectangle1->NAME != (char *)NULL 
       )
    {
      TmpRectangle2->NAME = namealloc ( TmpRectangle1->NAME 
                                      );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationOrientation            |
|                                                             |
\------------------------------------------------------------*/

boolean DrucUnifyOrientation (  DrucFigureRds,
                                LayerCourant,
                                Rectangle1,
                                Rectangle2,
                                FlagNextRectangle1,
                                FlagCreateRectangle
                             )
     rdsfig_list *DrucFigureRds;
     int          LayerCourant;
     rdsrec_list *Rectangle1;
     rdsrec_list *Rectangle2;
     boolean     *FlagNextRectangle1;
     boolean     *FlagCreateRectangle;

{

rdsrec_list *DrucNouveauRectangle;
long         XA                     =         Rectangle1->X;
long         YA                     =         Rectangle1->Y;
long         XB                     =         Rectangle2->X;
long         YB                     =         Rectangle2->Y;
long         DXA                    =         Rectangle1->DX;
long         DYA                    =         Rectangle1->DY;
long         DXB                    =         Rectangle2->DX;
long         DYB                    =         Rectangle2->DY;
long         FinXA                  = XA    + DXA;
long         FinXB                  = XB    + DXB;
long         FinYA                  = YA    + DYA;
long         FinYB                  = YB    + DYB;
long         ExtenXAXB              = XA    - XB;
long         ExtenDXAXB             = FinXA - XB;
long         ExtenDXADXB            = FinXA - FinXB;
long         ExtenYAYB              = YA    - YB;
long         ExtenDYAYB             = FinYA - YB;
long         ExtenDYADYB            = FinYA - FinYB;
long         ExtenDXBXA             = FinXB - XA;
long         ExtenDXBDXA            =       - ExtenDXADXB;
long         ExtenDYBYA             = FinYB - YA;
long         ExtenDYBDYA            =       - ExtenDYADYB;
int          TmpCas;
boolean FlagIsEquiModified          = DRUC_FALSE;

  TmpCas = 0;
  if ( XA >= XB )
  {
    TmpCas |= 1;
  }
  if ( FinXA <= FinXB )
  {
    TmpCas |= 2;
  }
  if ( YA >= YB )
  {
    TmpCas |= 4;
  }
  if ( FinYA <= FinYB )
  {
    TmpCas |= 8;
  }
  switch ( TmpCas 
         )
  {
  case 0  :
    { /* A enveloppe B */
      FlagIsEquiModified      = DRUC_TRUE;
      Rectangle2->FLAGS      |= DRUC_FLAG_REMOVE;

      DrucUnifyTransfertInfo       ( Rectangle2,
                                     Rectangle1
                                   );
      break;
    }
  case 1  :
    { /* A marge B */
      FlagIsEquiModified      = DRUC_TRUE;

      if ( XA == XB 
         )
      {
        /* A enveloppe B */
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
        break;
      }
      Rectangle2->DX          = ExtenDXAXB;
      break;
    }
  case 2  :
    { /* A Marge B */
      FlagIsEquiModified      = DRUC_TRUE;

      if ( FinXB == FinXA 
         )
      {
      /* A enveloppe B */
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
        break;
      }
      Rectangle2->X           = XA;
      Rectangle2->DX          = ExtenDXBXA;
      break;
    }
  case 3  :
    { /* A croix B */
      if ( ( XA == XB ) && ( FinXB == FinXA )
         )
      { /* A enveloppe B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
      }
      break;
    }
  case 4  :
    { /* A Marge B */
      FlagIsEquiModified      = DRUC_TRUE;

      if ( YA == YB 
         )
      {
      /* A enveloppe B */
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
        break;
      }
      Rectangle2->DY          = ExtenDYAYB;
      break;
    }
  case 5  :
    { /* A intersection B */
      if ( ( XA == XB ) && ( YA == YB )
         )
      { /* A enveloppe B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
        break;
      }
      if ( XA == XB 
         )
      { /* A marge B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->DY        = ExtenDYAYB;
        break; 
      }
      if ( YA == YB 
         )
      { /* A marge B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->DX        = ExtenDXAXB; 
        break; 
      }
      break;
    }
  case 6  :
    { /* A intersection B */
      if ( ( FinXA == FinXB ) && ( YA == YB )
         )
      { /* A enveloppe B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
        break;
      }
      if ( FinXA == FinXB 
         )
      { /* A marge B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->DY        = ExtenDYAYB;
        break;
      }
      if ( YA == YB 
         )
      { /* A marge B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->X         = XA; 
        Rectangle2->DX        = ExtenDXBXA; 
        break; 
      }
      break;
    }
  case 7  :
    {
      if ( ( XA    == XB    ) &&
           ( FinXA == FinXB )
         )
      {
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
      }
      if (  YA != YB 
         )
      {
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle1->Y         = YB;
        Rectangle1->DY        = ExtenDYAYB;
      }
      break;
    }
  case 8  :
    { /* A marge B */
      FlagIsEquiModified      = DRUC_TRUE;
      if ( FinYA == FinYB 
         )
      {
        /* A enveloppe B */
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
        break;
      }
      Rectangle2->Y          = YA;
      Rectangle2->DY         = ExtenDYBYA;
      break;
    }
  case 9  :
    { /* A intersection B */
      if ( ( XA == XB ) && ( FinYA == FinYB )
         )
      { /* A enveloppe B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;
 
        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
        break;
      }
      if ( XA == XB 
         )
      { /* A marge B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->Y         = YA;
        Rectangle2->DY        = ExtenDYBYA;
        break; 
      }
      if ( FinYA == FinYB 
         )
      { /* A marge B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->DX        = ExtenDXAXB; 
        break; 
      }
      break;
    }
  case 10 :
    { /* A intersection B */
      if ( ( FinXA == FinXB ) && 
           ( FinYA == FinYB )
         )
      { /* A enveloppe B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
        break;
      }
      if ( FinXA == FinXB 
         )
      { /* A marge B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->Y         = YA;
        Rectangle2->DY        = ExtenDYBYA;
        break; 
      }
      if ( FinYA == FinYB 
         )
      { /* A marge B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->X         = XA; 
        Rectangle2->DX        = ExtenDXBXA;
        break; 
      }
      break;
    }
  case 11 :
    {
      if ( ( XA    == XB    ) && 
           ( FinXA == FinXB ) 
         )
      {
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;
 
        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
      }
      if (  FinYA != FinYB 
         )
      {
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle1->DY        = ExtenDYBYA;
      }
      break;
    }
  case 12 :
    { /* A croix B */
      if ( ( YA    == YB    ) && 
           ( FinYB == FinYA )
         )
      { /* A enveloppe B */
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
      }
      break;
    }
  case 13 :
    {
      if ( ( YA    == YB    ) && 
           ( FinYB == FinYA )
         )
      {   
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE;

        DrucUnifyTransfertInfo     ( Rectangle2,
                                     Rectangle1
                                   );
      }    
      if (  XA != XB 
         )
      {    
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle1->X         = XB;
        Rectangle1->DX        = ExtenDXAXB;
      }    
      break;
    }
  case 14 :
    {
      if ( ( YA    == YB    ) && 
           ( FinYB == FinYA )
         )
      {    
        FlagIsEquiModified    = DRUC_TRUE;
        Rectangle2->FLAGS    |= DRUC_FLAG_REMOVE; 

        DrucUnifyTransfertInfo     ( Rectangle2, 
                                     Rectangle1 
                                   ); 
      }     
      if (  FinXA != FinXB 
         )
      {     
        FlagIsEquiModified    = DRUC_TRUE; 
        Rectangle1->DX        = ExtenDXBXA;
      }
      break;
    }
  case 15 :
    { /* A inclus dans B */
     *FlagNextRectangle1      = DRUC_TRUE;
      FlagIsEquiModified      = DRUC_TRUE;
      Rectangle1->FLAGS      |= DRUC_FLAG_REMOVE;

      DrucUnifyTransfertInfo       ( Rectangle1,
                                     Rectangle2
                                   );
      break;
    }
  }/* switch */

  return ( FlagIsEquiModified 
         );
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucUnifyCreate                    |
|                                                             |
\------------------------------------------------------------*/
boolean DrucUnifyCreate (  DrucFigureRds,
                           LayerCourant,
                           Rectangle1,
                           Rectangle2,
                           FlagNextRectangle1,
                           FlagCreateRectangle
                        )
     rdsfig_list *DrucFigureRds;
     int          LayerCourant;
     rdsrec_list *Rectangle1;
     rdsrec_list *Rectangle2;
     boolean     *FlagNextRectangle1;
     boolean     *FlagCreateRectangle;

{

rdsrec_list *DrucNouveauRectangle;
long         XA                  =         Rectangle1->X;
long         YA                  =         Rectangle1->Y;
long         XB                  =         Rectangle2->X;
long         YB                  =         Rectangle2->Y;
long         DXA                 =         Rectangle1->DX;
long         DYA                 =         Rectangle1->DY;
long         DXB                 =         Rectangle2->DX;
long         DYB                 =         Rectangle2->DY;
long         FinXA               = XA    + DXA;
long         FinXB               = XB    + DXB;
long         FinYA               = YA    + DYA;
long         FinYB               = YB    + DYB;
long         ExtenXAXB           = XA    - XB;
long         ExtenDXAXB          = FinXA - XB;
long         ExtenDXADXB         = FinXA - FinXB;
long         ExtenYAYB           = YA    - YB;
long         ExtenDYAYB          = FinYA - YB;
long         ExtenDYADYB         = FinYA - FinYB;
long         ExtenDXBXA          = FinXB - XA;
long         ExtenDXBDXA         =       - ExtenDXADXB;
long         ExtenDYBYA          = FinYB - YA;
long         ExtenDYBDYA         =       - ExtenDYADYB;
int          TmpCas;
boolean      FlagIsEquiModified  = DRUC_FALSE;

  TmpCas = 0;

  if ( XA >= XB 
     )
  {
    TmpCas |= 1;
  }

  if ( FinXA <= FinXB )
  {
    TmpCas |= 2;
  }

  if ( YA >= YB )
  {
    TmpCas |= 4;
  }

  if ( FinYA <= FinYB )
  {
    TmpCas |= 8;
  }

  switch ( TmpCas 
         )
  {
  case 5  :
    { /* A intersection B */
      if ( ( XA > XB ) && 
           ( YA > YB )
         )
      { 
        if ( ( ExtenDXAXB >= 0 ) &&
             ( ExtenDYBYA >= 0 )
           )
        {
          DrucNouveauRectangle = DrucCreateRectangle ( XB,
                                                       YA,
                                                       ExtenDXAXB,
                                                       ExtenDYBYA,
                                                       DRUC_FLAG_INCLUSION_RECTANGLE
                                                     );

          DrucChaineEquiRectangle ( DrucNouveauRectangle,
                                    Rectangle1
                                  );
          SetRdsLayer             ( DrucNouveauRectangle ,
                                    LayerCourant
                                  );
          DrucAddRecFigure        ( DrucNouveauRectangle,
                                    DrucFigureRds ,
                                    LayerCourant
                                  );
        }

        if ( ( ExtenDXBXA >= 0 ) &&
             ( ExtenDYAYB >= 0 )
           )
        { 
          DrucNouveauRectangle = DrucCreateRectangle ( XA,
                                                       YB,
                                                       ExtenDXBXA,
                                                       ExtenDYAYB,
                                                       DRUC_FLAG_INCLUSION_RECTANGLE
                                                     );
          DrucChaineEquiRectangle ( DrucNouveauRectangle,
                                    Rectangle1
                                  );
          SetRdsLayer             ( DrucNouveauRectangle ,
                                    LayerCourant
                                  );
          DrucAddRecFigure        ( DrucNouveauRectangle,
                                    DrucFigureRds ,
                                    LayerCourant
                                  );
        }
      }
      break;
    }
  case 6  :
    { /* A intersection B */
      if ( ( FinXA < FinXB ) && 
           ( YA    > YB    )
         )
      { 
        if ( ( ExtenDXBXA >= 0 ) &&
             ( ExtenDYBYA >= 0 )
           ) 
        {  
          DrucNouveauRectangle = DrucCreateRectangle ( XA,
                                                       YA,
                                                       ExtenDXBXA,
                                                     ExtenDYBYA,
                                                     DRUC_FLAG_INCLUSION_RECTANGLE
                                                     );
          DrucChaineEquiRectangle ( DrucNouveauRectangle,
                                    Rectangle1
                                  );
          SetRdsLayer             ( DrucNouveauRectangle ,
                                    LayerCourant
                                  );
          DrucAddRecFigure        ( DrucNouveauRectangle,
                                    DrucFigureRds ,
                                    LayerCourant
                                  );
        } 
        if ( ( ExtenDXAXB >= 0 ) &&
             ( ExtenDYAYB >= 0 )
           )
        {  
          DrucNouveauRectangle = DrucCreateRectangle ( XB,
                                                       YB,
                                                       ExtenDXAXB,
                                                       ExtenDYAYB,
                                                       DRUC_FLAG_INCLUSION_RECTANGLE
                                                     );
          DrucChaineEquiRectangle ( DrucNouveauRectangle,
                                    Rectangle1
                                  );
          SetRdsLayer             ( DrucNouveauRectangle ,
                                    LayerCourant
                                  );
          DrucAddRecFigure        ( DrucNouveauRectangle,
                                    DrucFigureRds ,
                                    LayerCourant
                                  );
        }
      }
      break;
    }
  case 9  :
    { /* A intersection B */
      if ( ( XA > XB ) && ( FinYA < FinYB )
         )
      { 
        if ( ( ExtenDXAXB >= 0 ) &&
             ( ExtenDYAYB >= 0 )
           ) 
        {  
          DrucNouveauRectangle = DrucCreateRectangle ( XB,
                                                       YB,
                                                       ExtenDXAXB,
                                                       ExtenDYAYB,
                                                       DRUC_FLAG_INCLUSION_RECTANGLE
                                                     );
          DrucChaineEquiRectangle ( DrucNouveauRectangle,
                                    Rectangle1
                                  );
          SetRdsLayer             ( DrucNouveauRectangle ,
                                    LayerCourant
                                  );
          DrucAddRecFigure        ( DrucNouveauRectangle,
                                   DrucFigureRds ,
                                   LayerCourant
                                  );
          } 
          if ( ( ExtenDXBXA >= 0 ) &&
               ( ExtenDYBYA >= 0 )
           )
          {  
          DrucNouveauRectangle = DrucCreateRectangle ( XA,
                                                       YA,
                                                       ExtenDXBXA,
                                                       ExtenDYBYA,
                                                       DRUC_FLAG_INCLUSION_RECTANGLE
                                                     );
          DrucChaineEquiRectangle ( DrucNouveauRectangle,
                                    Rectangle1
                                  );
          SetRdsLayer             ( DrucNouveauRectangle ,
                                    LayerCourant
                                  );
          DrucAddRecFigure        ( DrucNouveauRectangle,
                                    DrucFigureRds ,
                                    LayerCourant
                                  );
        }
      }
      break;
    }
  case 10 :
    { /* A intersection B */
      if ( ( FinXA < FinXB ) && 
           ( FinYA < FinYB )
         )
      { 
        if ( ( ExtenDXBXA >= 0 ) &&
             ( ExtenDYAYB >= 0 )
           ) 
        {
          DrucNouveauRectangle = DrucCreateRectangle ( XA,
                                                       YB,
                                                       ExtenDXBXA,
                                                       ExtenDYAYB,
                                                       DRUC_FLAG_INCLUSION_RECTANGLE
                                                     );
          DrucChaineEquiRectangle ( DrucNouveauRectangle,
                                    Rectangle1
                                  );
          SetRdsLayer             ( DrucNouveauRectangle ,
                                    LayerCourant
                                  );
          DrucAddRecFigure        ( DrucNouveauRectangle,
                                    DrucFigureRds ,
                                    LayerCourant
                                  );
        }
        if ( ( ExtenDXAXB >= 0 ) &&
             ( ExtenDYBYA >= 0 )
           )
        {  
          DrucNouveauRectangle = DrucCreateRectangle ( XB,
                                                       YA,
                                                       ExtenDXAXB,
                                                       ExtenDYBYA,
                                                       DRUC_FLAG_INCLUSION_RECTANGLE
                                                     );
          DrucChaineEquiRectangle ( DrucNouveauRectangle,
                                    Rectangle1
                                  );
          SetRdsLayer             ( DrucNouveauRectangle ,
                                    LayerCourant
                                  );
          DrucAddRecFigure        ( DrucNouveauRectangle,
                                    DrucFigureRds ,
                                    LayerCourant
                                  );
        }
      }
      break;
    }
  }/* switch */

  return ( FlagIsEquiModified 
         );
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucUnifyRectangle                  |
|                                                             |
\------------------------------------------------------------*/

void DrucUnifyRectangle ( DrucFigureRds,
                          LayerCourant,
                          RectangleSource
                        )
    rdsfig_list *DrucFigureRds;
    int          LayerCourant;
    rdsrec_list *RectangleSource;

{

rdsrec_list *TmpRectangle1;
rdsrec_list *TmpRectangle2;
boolean      FlagNextRectangle1;
boolean      FlagIsEquiModified;
boolean      FlagCreateRectangle = DRUC_TRUE;
DrucTypeStructPlusRect *PlusRec1;
DrucTypeStructPlusRect *PlusRec2;

  FlagCreateRectangle = DRUC_FALSE;
  do
  {
    FlagIsEquiModified       = DRUC_FALSE;
    TmpRectangle1            = RectangleSource;
    PlusRec1                 = (DrucTypeStructPlusRect *)
                                  ((char *)(TmpRectangle1)+sizeof(rdsrec_list));

    while ( PlusRec1->NEXT_EQUI != RectangleSource 
          )
    {
      if ( ( TmpRectangle1->FLAGS & DRUC_FLAG_REMOVE ) == DRUC_FLAG_REMOVE 
         )
      {
        TmpRectangle1        = PlusRec1->NEXT_EQUI;
        PlusRec1             = (DrucTypeStructPlusRect *)
                                  ((char *)(TmpRectangle1)+sizeof(rdsrec_list));
        continue;
      }
      TmpRectangle2          = PlusRec1->NEXT_EQUI;
      PlusRec2               = (DrucTypeStructPlusRect *)
                                  ((char *)(TmpRectangle2)+sizeof(rdsrec_list));

      while ( TmpRectangle2 != RectangleSource 
            )
      {
        if ( ( TmpRectangle2->FLAGS & DRUC_FLAG_REMOVE ) == DRUC_FLAG_REMOVE
           )
        {
          TmpRectangle2      = PlusRec2->NEXT_EQUI;
          PlusRec2           = (DrucTypeStructPlusRect *)
                                  ((char *)(TmpRectangle2)+sizeof(rdsrec_list));
          continue;
        }
        FlagNextRectangle1   = DRUC_FALSE;

        if ( (  DrucIsRectanglesContact ( TmpRectangle1,
                                          TmpRectangle2
                                        )
             ) == DRUC_TRUE 
           )
        {
          FlagIsEquiModified  |= DrucUnifyOrientation ( DrucFigureRds,
                                                        LayerCourant,
                                                        TmpRectangle1,
                                                        TmpRectangle2,
                                                       &FlagNextRectangle1,
                                                       &FlagCreateRectangle
                                                      );
        }

        if ( FlagNextRectangle1 == DRUC_TRUE )
        { 
        /*\
         *  pour passer au rectangle1 suivant car le rectangle1 courant 
         *   est a detruire 
        \*/
          FlagNextRectangle1 = DRUC_FALSE;
          break;
        }

        TmpRectangle2 = PlusRec2->NEXT_EQUI;
        PlusRec2      = (DrucTypeStructPlusRect *)
                           ((char *)(TmpRectangle2)+sizeof(rdsrec_list));
      } /* while TmpRectangle2 */

      TmpRectangle1   = PlusRec1->NEXT_EQUI;
      PlusRec1        = (DrucTypeStructPlusRect *)
                           ((char *)(TmpRectangle1)+sizeof(rdsrec_list));
    } /* while TmpRectangle1 */
  }

  while ( FlagIsEquiModified != DRUC_FALSE 
        );
/*\
 * phase de creation des rectangles sur les configurations de croix
\*/
  FlagIsEquiModified       = DRUC_FALSE;
  TmpRectangle1            = RectangleSource;
  PlusRec1                 = (DrucTypeStructPlusRect *)
                                ((char *)(TmpRectangle1)+sizeof(rdsrec_list));

  while ( PlusRec1->NEXT_EQUI != RectangleSource 
        )
  {
    if ( ( TmpRectangle1->FLAGS & DRUC_FLAG_REMOVE ) == DRUC_FLAG_REMOVE
       )  
    {   
      TmpRectangle1        = PlusRec1->NEXT_EQUI;
      PlusRec1             = (DrucTypeStructPlusRect *)
                                ((char *)(TmpRectangle1)+sizeof(rdsrec_list));
      continue;
    }

    TmpRectangle2          = PlusRec1->NEXT_EQUI;
    PlusRec2               = (DrucTypeStructPlusRect *)
                                ((char *)(TmpRectangle2)+sizeof(rdsrec_list));

    while ( TmpRectangle2 != RectangleSource 
          )
    {
      if ( ( TmpRectangle2->FLAGS & DRUC_FLAG_REMOVE ) == DRUC_FLAG_REMOVE
         )  
      {   
        TmpRectangle2      = PlusRec2->NEXT_EQUI;
        PlusRec2           = (DrucTypeStructPlusRect *)
                                ((char *)(TmpRectangle2)+sizeof(rdsrec_list));
        continue;
      }
      FlagNextRectangle1   = DRUC_FALSE;

      if ( (  DrucIsRectanglesContact ( TmpRectangle1,
                                        TmpRectangle2
                                      )
           ) == DRUC_TRUE 
         )
      {
        if ( ( ( TmpRectangle1->FLAGS & 
                 DRUC_FLAG_INCLUSION_RECTANGLE 
               ) !=
                DRUC_FLAG_INCLUSION_RECTANGLE 
             ) &&
             ( ( TmpRectangle2->FLAGS & 
                 DRUC_FLAG_INCLUSION_RECTANGLE 
               ) !=
                DRUC_FLAG_INCLUSION_RECTANGLE 
             )
           )
        {   
          FlagIsEquiModified  |= DrucUnifyCreate ( DrucFigureRds,
                                                   LayerCourant,
                                                   TmpRectangle1,
                                                   TmpRectangle2,
                                                  &FlagNextRectangle1,
                                                  &FlagCreateRectangle
                                                 );
        }
      }
      TmpRectangle2 = PlusRec2->NEXT_EQUI;
      PlusRec2      = (DrucTypeStructPlusRect *)
                          ((char *)(TmpRectangle2)+sizeof(rdsrec_list));
    } /* while TmpRectangle2 */

    TmpRectangle1   = PlusRec1->NEXT_EQUI;
    PlusRec1        = (DrucTypeStructPlusRect *)
                          ((char *)(TmpRectangle1)+sizeof(rdsrec_list));
  } /* while TmpRectangle1 */

/*\
 * dilatation apres creation 
\*/
  FlagCreateRectangle = DRUC_FALSE;

  do
  {
    FlagIsEquiModified       = DRUC_FALSE;
    TmpRectangle1            = RectangleSource;
    PlusRec1                 = (DrucTypeStructPlusRect *)((char *)(TmpRectangle1)+sizeof(rdsrec_list));
    while ( PlusRec1->NEXT_EQUI != RectangleSource 
          )
    {
      if ( ( TmpRectangle1->FLAGS & DRUC_FLAG_REMOVE ) == DRUC_FLAG_REMOVE 
         )
      {
        TmpRectangle1     = PlusRec1->NEXT_EQUI;
        PlusRec1          = (DrucTypeStructPlusRect *)
                                ((char *)(TmpRectangle1)+sizeof(rdsrec_list));
        continue;
      }
      TmpRectangle2       = PlusRec1->NEXT_EQUI;
      PlusRec2            = (DrucTypeStructPlusRect *)
                                ((char *)(TmpRectangle2)+sizeof(rdsrec_list));

      while ( TmpRectangle2 != RectangleSource 
            )
      {
        if ( ( TmpRectangle2->FLAGS & DRUC_FLAG_REMOVE ) == 
             DRUC_FLAG_REMOVE
           )
        {
          TmpRectangle2      = PlusRec2->NEXT_EQUI;
          PlusRec2           = (DrucTypeStructPlusRect *)
                                  ((char *)(TmpRectangle2)+sizeof(rdsrec_list));
          continue;
        }

        FlagNextRectangle1   = DRUC_FALSE;

        if ( (  DrucIsRectanglesContact ( TmpRectangle1,
                                          TmpRectangle2
                                        )
             ) == DRUC_TRUE 
           )
        {
          FlagIsEquiModified  |= DrucUnifyOrientation ( DrucFigureRds,
                                                        LayerCourant,
                                                        TmpRectangle1,
                                                        TmpRectangle2,
                                                       &FlagNextRectangle1,
                                                       &FlagCreateRectangle
                                                      );
        }

        if ( FlagNextRectangle1 == DRUC_TRUE 
           )
        { /* pour passer au rectangle1 suivant car le rectangle1 courant est a detruire */
          FlagNextRectangle1 = DRUC_FALSE;
          break;
        }

        TmpRectangle2 = PlusRec2->NEXT_EQUI;
        PlusRec2      = (DrucTypeStructPlusRect *)((char *)
                           (TmpRectangle2)+sizeof(rdsrec_list));
      } /* while TmpRectangle2 */

      TmpRectangle1   = PlusRec1->NEXT_EQUI;
      PlusRec1        = (DrucTypeStructPlusRect *)
                           ((char *)(TmpRectangle1)+sizeof(rdsrec_list));
    } /* while TmpRectangle1 */
  }

  while ( FlagIsEquiModified != DRUC_FALSE 
        );
  TmpRectangle2  = RectangleSource;

  do
  {
    TmpRectangle2->FLAGS |= DRUC_FLAG_REC_UNIFIE;
    TmpRectangle2         = MACRO_NEXT_EQUI ( TmpRectangle2 );
    PlusRec2              = (DrucTypeStructPlusRect *)((char *)(TmpRectangle2)+sizeof(rdsrec_list));
  }
  while ( TmpRectangle2 != RectangleSource 
        );
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucUnifyLayer                      |
|                                                             |
\------------------------------------------------------------*/

void DrucUnifyLayer ( DrucFigureRds ,
                      DrucRdsWindow,
                      LayerCourant
                    )
    rdsfig_list *DrucFigureRds;
    rdswindow   *DrucRdsWindow;
    int          LayerCourant;

{

rdsrec_list *RectangleSource;

/*\
DrucViewString ("\nUNIFY LAYER\n"
               );
viewrdsfig ( DrucFigureRds
           );
\*/

  for ( RectangleSource  = DrucFigureRds->LAYERTAB [ LayerCourant ];
        RectangleSource != ( rdsrec_list *)NULL;
        RectangleSource  = RectangleSource->NEXT
      )
  {

/*\
DrucViewString ("\nUNIFY layer\n"
               );
viewrdsrec ( RectangleSource
           );
\*/

    if ( ( ( RectangleSource->FLAGS & DRUC_FLAG_REMOVE     ) != DRUC_FLAG_REMOVE     ) &&
         ( ( RectangleSource->FLAGS & DRUC_FLAG_REC_UNIFIE ) != DRUC_FLAG_REC_UNIFIE )
       )
    {
      DrucUnifyRectangle ( DrucFigureRds,
                           LayerCourant,
                           RectangleSource
                         );
    }
  }

  if ( ( DrucFigureRds->LAYERTAB [ LayerCourant ] ) != (rdsrec_list *)NULL 
     )
  {
/*\
DrucViewString ("\nFIN UNIFY layer\n"
               );
viewrdsfig ( DrucFigureRds
           );
\*/
    DrucUnifyRemoveRectanglesEqui ( &(DrucFigureRds->LAYERTAB [ LayerCourant ]),
                                    DrucRdsWindow,
                                    DrucStructRdsLayer [ LayerCourant ].WIDTH 
                                  );
  }

  DrucUnifyDeleteEqui ( DrucFigureRds->LAYERTAB [ LayerCourant ]
                      );

  DrucClearFlag ( DrucFigureRds->LAYERTAB [ LayerCourant ],
                  DRUC_FLAG_REC_UNIFIE
                );
/*\
DrucViewString ("\nFIN UNIFY LAYER\n"
               );
\*/
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucUnify                           |
|                                                             |
\------------------------------------------------------------*/

void DrucUnify ( DrucFigureRds,
                 DrucRdsWindow
               )
    rdsfig_list *DrucFigureRds;
    rdswindow   *DrucRdsWindow;


{

int              IndexLayer;
int              LayerCourant;

  for ( IndexLayer = 0;
        IndexLayer < RDS_MAX_LAYER ;
        IndexLayer ++
      )
  {
    LayerCourant = DrucStructRdsLayer [ IndexLayer ].RDS_INDEX ;
    if ( LayerCourant != DRUC_UNDEFINED_LAYER
       )
    {
      if ( ( ( DrucFigureRds->LAYERTAB ) [ LayerCourant ] ) != ( rdsrec_list * ) NULL
         )
      {
/*\
DrucViewString ( "DrucUnify "
               );
DrucViewNumber ( LayerCourant
               );
DrucViewString ( "\n"
               );
\*/
        DrucUnifyLayer  ( DrucFigureRds,
                          DrucRdsWindow,
                          LayerCourant
                        );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucUnifyFigure                    |
|                                                             |
\------------------------------------------------------------*/

void DrucUnifyFigure ( DrucFigureRds,
                       DrucStructRdsLayer
                     )
     rdsfig_list             *DrucFigureRds;
     DrucTypeStructRdsLayer  *DrucStructRdsLayer;

{

rdswindow               *DrucRdsWindow;
rdsins_list             *InstanceCourante;
int                      IndexLayer;
int                      LayerCourant;

  InstanceCourante        = DrucFigureRds->INSTANCE;
  DrucFigureRds->INSTANCE = (rdsins_list *)NULL;

  DrucRdsWindow           = buildrdswindow ( DrucFigureRds
                                           );

  if ( DrucRdsWindow->MASK != 0 
     )
  {
    DrucComputeEquiFigure ( DrucFigureRds
                          );
/*\
    DrucVerifEquiFigure   ( DrucFigureRds
                          );
\*/
    DrucUnify             ( DrucFigureRds,
                            DrucRdsWindow
                          );
    destroyrdswindow      ( DrucFigureRds,
                            DrucRdsWindow
                          );
  }

  DrucFigureRds->INSTANCE = InstanceCourante;
}

/*------------------------------------------------------------\
|                                                             |
|                    vmcunify Evolutions                      |
|                                                             |
| Patrick 08/03/94:                                           |
| ligne 109 ajouter le test du pointeur null                  |
| ligne 142 ajouter le test du pointeur null                  |
|                                                             |
|                                                             |
\------------------------------------------------------------*/
