/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Scp                           |
|                                                             |
| File    :                 Scp Build                         |
|                                                             |
| Date    :                  04.03.98                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "mut.h"
# include "mlo.h"
# include "aut.h"
# include "scl.h"

# include "scp.h"
# include "scpbuild.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Locals  Functions                    |
|                                                             |
\------------------------------------------------------------*/

 static int SchNetCompare();
 static int CheckNetIntersection __P((chain_list *, schnet_list *));

/*------------------------------------------------------------\
|                                                             |
|                Function  Loc Add Transparence               |
|                                                             |
\------------------------------------------------------------*/

static schbox_list * loc_add_transparence( Figure, Net )

  schfig_list *Figure;
  schnet_list *Net;
{
  schbox_list *Box;
  schcon_list *Con_in;
  schcon_list *Con_out;

  Box = addschbox( Figure, "trans" );
  SetSchBoxTransparence( Box );

  Con_in  = addschboxconin( Figure, Box, "i" );
  Con_out = addschboxconout( Figure, Box, "o" );

  addschnetcon( Net, Con_out );
  addschnetcon( Net, Con_in  );

  return ( Box );
}

/*------------------------------------------------------------\
|                                                             |
|                Function Loc Destroy Transparence            |
|                                                             |
\------------------------------------------------------------*/

static void loc_destroytransparence( Figure, Transparence)

  schfig_list *Figure;
  schbox_list *Transparence;
{
  schnet_list *SchNet;
  schwir_list *SchWire;

  SchNet = Transparence->CON_IN->NET;
  SchWire = addschwir(Figure, SchNet);

  SchWire->X  = Transparence->X + Transparence->CON_IN->X_REL;
  SchWire->Y  = Transparence->Y + Transparence->CON_IN->Y_REL;
  SchWire->DX = Transparence->X + Transparence->CON_OUT->X_REL;
  SchWire->DY = Transparence->Y + Transparence->CON_OUT->Y_REL;
}

/*------------------------------------------------------------\
|                                                             |
|                Function Loc Destroy Cluster Net             |
|                                                             |
\------------------------------------------------------------*/

static void loc_destroyclusternet( Figure, Cluster )

  schfig_list *Figure;
  schbox_list *Cluster;
{
  schnet_list  *SchNetIn;
  schnet_list  *SchNetOut;
  schwir_list  *SchWire;
  schwir_list **PrevSchWire;

  SchNetIn  = Cluster->CON_IN->NET;
  SchNetOut = Cluster->CON_OUT->NET;

  SchWire = addschwir( Figure, SchNetIn );

  SchWire->X  = Cluster->X + Cluster->CON_IN->X_REL;
  SchWire->Y  = Cluster->Y + Cluster->CON_IN->Y_REL;
  SchWire->DX = Cluster->X + Cluster->CON_OUT->X_REL;
  SchWire->DY = Cluster->Y + Cluster->CON_OUT->Y_REL;

  PrevSchWire = &SchNetOut->WIRE;

  for ( SchWire  = SchNetOut->WIRE;
        SchWire != (schwir_list *)0;
        SchWire  = SchWire->NEXT )
  {
    SchWire->NET = SchNetIn;
    PrevSchWire  = &SchWire->NEXT;
  }

  *PrevSchWire    = SchNetIn->WIRE;
  SchNetIn->WIRE  = SchNetOut->WIRE;
  SchNetOut->WIRE = (schwir_list *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                    Function Loc Destroy Cluster             |
|                                                             |
\------------------------------------------------------------*/

static void loc_destroycluster(Figure, Cluster )

  schfig_list *Figure;
  schbox_list *Cluster;
{
  schwir_list    *SchWire1;
  schwir_list    *SchWire2;
  schwir_list    *SchWire3;
  schbox_list    *Box;
  schbox_list    *BoxN;
  schcon_list    *Con;
  schcon_list    *BoxConIn;
  schcon_list    *ConCast;
  schnet_list    *SchNet;
  schcon_list    *ClusterCon;
  ptype_list     *ScanBox;
  ptype_list     *HeadBox;
  chain_list     *ScanCon;
  long            LastDY;
  long            CurrentDX;
  long            Counter;
  int             NumberBoxIn;
  int             Middle;

# ifdef DEBUG
  fprintf( stdout, "loc_destroycluster %s\n", Cluster->NAME );
# endif
/*
** Get the three boxes of the cluster
*/
  HeadBox = (ptype_list  *)Cluster->SOURCE;
  Box     = (schbox_list *)HeadBox->DATA;
  HeadBox = HeadBox->NEXT;
/*
** Compute the position of the output box
*/
  Box->USER = (void *)Cluster->USER;
  Box->X    = Cluster->X + Cluster->DX - Box->DX;
  Box->Y   += Cluster->Y;

  for ( Con  = Box->CON_OUT;
        Con != (schcon_list *)0;
        Con  = Con->NEXT )
  {
    Con->X_REL = Box->DX + SCP_BOX_CON_X;

    ClusterCon = (schcon_list *)Con->USER;

    SchNet = Con->NET;

    for ( ScanCon  = SchNet->CON_NET;
          ScanCon != (chain_list *)0;
          ScanCon  = ScanCon->NEXT )
    {
      ConCast = (schcon_list *) ScanCon->DATA;

      if ( ConCast == ClusterCon )
      {
        ScanCon->DATA = (void *)Con;
      }
    }
  }

  if ( IsSchBoxCluster( Box ) )
  {
    loc_destroycluster( Figure, Box );
  }

  NumberBoxIn = Box->NUMBER_IN;

  Box->CON_IN = (schcon_list *)reverse( (chain_list *)Box->CON_IN );
  BoxConIn    = Box->CON_IN;

  ScanBox   = HeadBox;
  LastDY    = 0;
  Counter   = 1;
  Middle    = 1 + ( NumberBoxIn ) / 2;
  CurrentDX = 0;

  while ( ScanBox != (ptype_list *)0 )
  {
    BoxN = (schbox_list *)ScanBox->DATA;

    if ( Counter != Middle )
    {
      if ( Counter > Middle )
      {
        CurrentDX--;
      }
      else
      {
        CurrentDX++;
      }
    }
    else
    if ( NumberBoxIn & 1 )
    {
      CurrentDX++;
    }

    BoxN->USER = (void *)Cluster->USER;
    BoxN->X    = Cluster->X;
    BoxN->Y    = Cluster->Y + LastDY;
/*
** Compute the position of output connectors of the N input box
*/
    for ( Con  = BoxN->CON_OUT;
          Con != (schcon_list *)0;
          Con  = Con->NEXT )
    {
      Con->X_REL = BoxN->DX + SCP_BOX_CON_X;
    }
/*
** Replace input connectors of the cluster by input connectors of the N box
*/
    for ( Con  = BoxN->CON_IN;
          Con != (schcon_list *)0;
          Con  = Con->NEXT )
    {
      ClusterCon = (schcon_list *)Con->USER;
  
      SchNet = Con->NET;
  
      for ( ScanCon  = SchNet->CON_NET;
            ScanCon != (chain_list *)0;
            ScanCon  = ScanCon->NEXT )
      {
        ConCast = (schcon_list *) ScanCon->DATA;
  
        if ( ConCast == ClusterCon )
        {
          ScanCon->DATA = (void *)Con;
        }
      }
    }
/*
** Compute wire from the output of BoxN to the input of Box
*/
    SchWire1 = addschwir( Figure, BoxN->CON_OUT->NET );

    SchWire1->X  = BoxN->X + BoxN->CON_OUT->X_REL;
    SchWire1->Y  = BoxN->Y + BoxN->CON_OUT->Y_REL;
    SchWire1->DX = Box->X  + BoxConIn->X_REL - (CurrentDX * SCP_CLUSTER_ROUTE_STEP_X);
    SchWire1->DY = SchWire1->Y;

    SchWire2 = addschwir( Figure, BoxN->CON_OUT->NET );

    SchWire2->X  = SchWire1->DX;
    SchWire2->Y  = SchWire1->DY;
    SchWire2->DX = SchWire2->X;
    SchWire2->DY = Box->Y  + BoxConIn->Y_REL;

    SchWire3 = addschwir( Figure, BoxN->CON_OUT->NET );

    SchWire3->X  = SchWire2->DX;
    SchWire3->Y  = SchWire2->DY;
    SchWire3->DX = Box->X  + BoxConIn->X_REL;
    SchWire3->DY = SchWire3->Y;

    if ( IsSchBoxCluster( BoxN ) )
    {
      loc_destroycluster( Figure, BoxN );
    }

    LastDY  += BoxN->DY;
    BoxConIn = BoxConIn->NEXT;

    Counter++;
    ScanBox  = ScanBox->NEXT;
  }

  Box->CON_IN = (schcon_list *)reverse( (chain_list *)Box->CON_IN );
}

/*------------------------------------------------------------\
|                                                             |
|                   Function  Loc Build Cluster               |
|                                                             |
\------------------------------------------------------------*/

static void loc_buildcluster( Figure )

  schfig_list *Figure;
{
  schbox_list  *Box;
  schbox_list  *SavBox;
  schbox_list **PrevBox;
  schcon_list  *SchCon;
  schcon_list  *ConCast;
  schcon_list  *ClusterCon;
  schbox_list  *BoxN;
  schcon_list  *ConN;
  schbox_list  *Cluster;
  schnet_list  *SchNet;
  ptype_list   *HeadList;
  ptype_list   *ScanList;
  chain_list   *ScanChain;
  char          Buffer[ 32 ];
  int           Number;
  int           FoundOne;
  int           Go;

  Number  = 0;
  PrevBox = &Figure->BOX;

  for ( Box  = Figure->BOX;
        Box != (schbox_list *)0;
        Box  = Box->NEXT )
  {
    PrevBox = &Box->NEXT;
  }

  for ( Box  = Figure->BOX;
        Box != (schbox_list *)0;
        Box  = Box->NEXT )
  {
    Go = 1;
/*
** Search a box with one output and two inputs minimum
*/
    if ( ( ! IsSchBoxClustered( Box ) ) &&
         ( Box->NUMBER_IN  >= 1       ) &&
         ( Box->NUMBER_OUT >= 1       ) ) 
    {
      HeadList = (ptype_list *)0;
      FoundOne = 0;

      for ( ConN  = Box->CON_IN;
            ConN != (schcon_list *)0;
            ConN  = ConN->NEXT )
      {
/*
** if the input signal have more than one output or more than one input
** => should create a box net
*/
        if ( ( ConN->NET->NUMBER_OUT != 1 ) ||
             ( ConN->NET->NUMBER_IN  != 1 ) )
        {
          HeadList = addptype( HeadList, SCP_CLUSTER_BOX_NET, (void *)ConN );
        }
        else
        {
/*
** The input signal (with one input and one output) should be connected to a box 
** with only one output
*/
          for ( ScanChain  = ConN->NET->CON_NET;
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT ) 
          {
            ConCast = (schcon_list *)ScanChain->DATA;

            if ( ConCast == ConN ) continue;
/*
** The input of the input signal is a box
*/
            if ( ConCast->ROOT_TYPE == SCH_ROOT_CON_BOX )
            {
              BoxN = (schbox_list *)ConCast->ROOT;
  
              if ( BoxN->NUMBER_OUT != 1 )
              {
/*
** The input of the input signal is a box, and this box has more than one output
** => should create a box net
*/
                HeadList = addptype( HeadList, SCP_CLUSTER_BOX_NET, (void *)ConN );
              }
              else
              {
/*
** The input of the input signal is a box, and this box has only one output, we found it !
** => this box will be part of the cluster 
*/
                HeadList = addptype( HeadList, SCP_CLUSTER_BOX, (void *)BoxN );
                FoundOne = 1;
              }
            }
            else
/*
** The input of the input signal is a primary input
** => should create a box net
*/
            if ( ConCast->ROOT_TYPE == SCH_ROOT_CON_FIG )
            {
              HeadList = addptype( HeadList, SCP_CLUSTER_BOX_NET, (void *)ConN );
            }
            else
            {
              Go = 0; break;
            }
          }
        }

        if ( ! Go ) break;
      }
/*
** Create a cluster with those N boxes !
*/
      if ( ( Go       == 1 ) &&
           ( FoundOne == 1 ) )
      {
        SavBox      = Figure->BOX;
        Figure->BOX = (schbox_list *)0;

        sprintf( Buffer, "cluster_%d", Number++ );

        Cluster     = addschbox( Figure, Buffer );
        Figure->BOX = SavBox;

        *PrevBox = Cluster;
        PrevBox  = &Cluster->NEXT;

        SetSchBoxCluster( Cluster );

        Cluster->SOURCE = (void *)HeadList;

        for ( ScanList  = HeadList;
              ScanList != (ptype_list *)0;
              ScanList  = ScanList->NEXT )
        {
/*
** Must create a cluster box net !
*/
          if ( ScanList->TYPE == SCP_CLUSTER_BOX_NET )
          {
            ConN = (schcon_list *)ScanList->DATA;

            sprintf( Buffer, "clustnet_%d", Number++ );
            BoxN = addschbox( Figure, Buffer );

            SetSchBoxClusterNet( BoxN );

            SchCon  = addschboxconin( Figure , BoxN, ConN->NAME );
            SchCon->NET = ConN->NET;

            for ( ScanChain  = SchCon->NET->CON_NET;
                  ScanChain != (chain_list *)0;
                  ScanChain  = ScanChain->NEXT )
            {
              ConCast = (schcon_list *)ScanChain->DATA;
  
              if ( ConCast == ConN )
              {
                ScanChain->DATA = (void *)SchCon;
              }
            }

            SchCon = addschboxconout( Figure, BoxN, ConN->NAME );
            SchNet = addschnet( Figure );

            addschnetcon( SchNet, SchCon );
            addschnetcon( SchNet, ConN   );

            ScanList->DATA = (void *)BoxN;
          }
          else
          {
            BoxN = (schbox_list *)ScanList->DATA;
          }

          SetSchBoxClustered( BoxN );
/*
** Duplicates all inputs of the N input boxes in the cluster (in the same order)
*/
          BoxN->CON_IN = (schcon_list *)reverse( (chain_list *)BoxN->CON_IN );
  
          for ( SchCon  = BoxN->CON_IN;
                SchCon != (schcon_list *)0;
                SchCon  = SchCon->NEXT )
          {
            ClusterCon = addschboxconin( Figure, Cluster, SchCon->NAME );
            SchNet = SchCon->NET;
  
            for ( ScanChain  = SchNet->CON_NET;
                  ScanChain != (chain_list *)0;
                  ScanChain  = ScanChain->NEXT )
            {
              ConCast = (schcon_list *)ScanChain->DATA;
  
              if ( ConCast == SchCon )
              {
                ScanChain->DATA = (void *)ClusterCon;
              }
            }
  
            ClusterCon->NET = SchNet;
  
            SchCon->USER = (void *)ClusterCon;
          }
  
          BoxN->CON_IN = (schcon_list *)reverse((chain_list *) BoxN->CON_IN );
        }

        SetSchBoxClustered( Box );
        Cluster->SOURCE = addptype( Cluster->SOURCE, SCP_CLUSTER_BOX, (void *)Box );

        Box->CON_OUT = (schcon_list *)reverse((chain_list *)Box->CON_OUT );
/*
** Duplicates all outputs of the output box in the cluster (in the same order)
*/
        for ( SchCon  = Box->CON_OUT;
              SchCon != (schcon_list *)0;
              SchCon  = SchCon->NEXT )
        {
          ClusterCon = addschboxconout( Figure, Cluster, SchCon->NAME );
          SchNet = SchCon->NET;

          for ( ScanChain  = SchNet->CON_NET;
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            ConCast = (schcon_list *)ScanChain->DATA;

            if ( ConCast == SchCon )
            {
              ScanChain->DATA = (void *) ClusterCon;
            }
          }

          ClusterCon->NET = SchNet;
          SchCon->USER = (void *)ClusterCon;
        }

        Box->CON_OUT = (schcon_list *)reverse((chain_list *)Box->CON_OUT );
      }
      else
      {
        freeptype( HeadList );
      }
    }
  }

# ifdef DEBUG
  for ( Box  = Figure->BOX;
        Box != (schbox_list *)0;
        Box  = Box->NEXT )
  {
    if ( IsSchBoxCluster( Box ) )
    {
      fprintf( stdout, "Cluster %s : ", Box->NAME );

      for ( ScanList  = Box->SOURCE;
            ScanList != (ptype_list *)0;
            ScanList  = ScanList->NEXT )
      {
        BoxN = (schbox_list *)ScanList->DATA;
        fprintf( stdout, " %s ", BoxN->NAME );
      }

      fprintf( stdout, "\n" );
    }
  }
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                  Function  Loc Build Place First            |
|                                                             |
\------------------------------------------------------------*/

static scpcol_list *loc_buildplacefirst( Figure )

  schfig_list    *Figure;
{
  scpcol_list    *ColMax;
  scpcel_list    *Cell;
  scpcol_list    *Column;
  schbox_list    *Box;
  schcon_list    *Con;
  long            MaxCumulY;
  long            CumulY;

  MaxCumulY = 0;
  ColMax    = HEAD_SCPCOL;

  for ( Column  = HEAD_SCPCOL;
        Column != (scpcol_list *)0;
        Column  = Column->NEXT ) 
  {
    CumulY = 0;

    for ( Cell  = Column->CELL;
          Cell != (scpcel_list *)0;
          Cell  = Cell->NEXT ) 
    {
      if ( Cell->TYPE == SCP_CEL_BOX ) 
      {
        Box     = (schbox_list *)Cell->ROOT;
        Box->Y  = CumulY;
        CumulY += Box->DY + SCP_BOX_STEP_Y;

        if ( Box->DX > Column->MAX_DX_CELL )
        {
          Column->MAX_DX_CELL = Box->DX;
        }
      }
      else
      if ( Cell->TYPE == SCP_CEL_CON ) 
      {
        Con        = (schcon_list *) Cell->ROOT;
        Con->Y_REL = CumulY;
        CumulY    += SCP_CON_STEP_Y; 

        if ( Column->MAX_DX_CELL < SCP_CELL_MIN_DX )
        {
          Column->MAX_DX_CELL = SCP_CELL_MIN_DX;
        }
      }
    }

    if ( MaxCumulY < CumulY ) 
    {
      ColMax    = Column;
      MaxCumulY = CumulY;
    }
  }

  return ( ColMax );
}


/*------------------------------------------------------------\
|                                                             |
|                Function  Loc Build Place In                 |
|                                                             |
\------------------------------------------------------------*/

static schfig_list *loc_buildplacein( Figure, ColumnFrom, ColumnTo )

  schfig_list    *Figure;
  scpcol_list    *ColumnFrom;
  scpcol_list    *ColumnTo;
{
  scpcel_list    *CellTo;
  schcon_list    *ConTo;
  schbox_list    *BoxTo;

  scpcel_list    *CellFrom;
  schbox_list    *BoxFrom;
  schcon_list    *ConFrom;

  chain_list     *Chain;
  char            Begin = 1;

  long            CumulY;

  for ( CellTo = ColumnTo->LAST_CELL; CellTo != (scpcel_list *)0; CellTo = CellTo->PREV ) 
  {
    if ( CellTo->TYPE == SCP_CEL_BOX )
    {
      BoxTo = (schbox_list *) CellTo->ROOT;

      for (ConTo = BoxTo->CON_IN; ConTo; ConTo = ConTo->NEXT )
      {
        for (Chain = ConTo->NET->CON_NET; Chain; Chain = Chain->NEXT )
        {
          ConFrom = (schcon_list *) Chain->DATA;

          if ( ( IsSchConInternal( ConFrom ) ) && 
               ( IsSchConOut( ConFrom )      ) )
          {
            BoxFrom = (schbox_list *) ConFrom->ROOT;

            if ( (   IsSchBoxTaged( BoxFrom ) ) &&
                 ( ! IsSchBoxPlaced(BoxFrom ) ) )
            {
              CellFrom = (scpcel_list *) BoxFrom->USER;
              if (CellFrom->COL == ColumnFrom)
              {
                if ( Begin )
                {
                  /* juste une idee de grandeur */

                  CumulY = BoxTo->Y + 2 * (BoxFrom->DY) + (BoxTo->DY);
                  Begin  = 0;
                }

                if ( ( BoxTo->Y + ConTo->Y_REL - ConFrom->Y_REL + BoxFrom->DY) <= CumulY )
                {
                  BoxFrom->Y = BoxTo->Y + ConTo->Y_REL - ConFrom->Y_REL;
                }
                else
                {
                  BoxFrom->Y = CumulY - BoxFrom->DY;
                }

                CumulY = BoxFrom->Y - 1;
                SetSchBoxPlaced( BoxFrom );
              }
            }
          }

          if ( (   IsSchConExternal( ConFrom ) ) && 
               (   IsSchConIn( ConFrom )       ) && 
               ( ! IsSchConPlaced( ConFrom )   ) &&
               (   IsSchConTaged( ConFrom )    ) )
          {
            CellFrom = (scpcel_list *) ConFrom->USER;

            if ( CellFrom->COL == ColumnFrom )
            {
              if ( ! IsSchConPlaced( ConFrom ) )
              {
                ConFrom->Y_REL = ( BoxTo->Y + ConTo->Y_REL );
                SetSchConPlaced( ConFrom );
              }
            }
          }
        }
      }
    }
  }

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                Function  Loc Build Place Out                |
|                                                             |
\------------------------------------------------------------*/

static schfig_list *loc_buildplaceout( Figure, ColumnFrom, ColumnTo )

  schfig_list *Figure;
  scpcol_list *ColumnFrom;
  scpcol_list *ColumnTo;
{
  chain_list     *Chain;
  unsigned char   Begin;

  schbox_list    *BoxFrom;
  schcon_list    *ConFrom;
  scpcel_list    *CellFrom;

  schcon_list    *ConTo;
  scpcel_list    *CellTo;
  schbox_list    *BoxTo;

  long            CumulY;

  /* LUDO 29/10/2004 */
  CumulY = 0;
  /* ODUL */

  Begin  = 1;

  for ( CellTo  = ColumnTo->LAST_CELL;
        CellTo != (scpcel_list *)0;
        CellTo  = CellTo->PREV )
  {
    if ( CellTo->TYPE == SCP_CEL_BOX )
    {
      BoxTo = (schbox_list *) CellTo->ROOT;

      for ( ConTo = BoxTo->CON_IN;
            ConTo != (schcon_list *)0;
            ConTo = ConTo->NEXT )
      {
        if ( IsSchBoxPlaced( BoxTo ) ) break;

        for ( Chain  = ConTo->NET->CON_NET;
              Chain != (chain_list *)0;
              Chain  = Chain->NEXT )
        {
          if ( IsSchBoxPlaced( BoxTo ) ) break;

          ConFrom = (schcon_list *) Chain->DATA;

          if ( ConFrom != ConTo )
          {
            if ( ( IsSchConInternal( ConFrom ) ) && 
                 ( IsSchConOut( ConFrom )      ) )
            {
              BoxFrom = (schbox_list *) ConFrom->ROOT;

              if ( IsSchBoxTaged( BoxFrom ) )
              {
                CellFrom = (scpcel_list *) BoxFrom->USER;

                if ( CellFrom->COL == ColumnFrom )
                {
                  if ( Begin )
                  {
                    CumulY = BoxFrom->Y + 2 * (BoxTo->DY + BoxFrom->DY);
                    Begin = 0;
                  }

                  if ( ( BoxFrom->Y + ConFrom->Y_REL - ConTo->Y_REL + BoxTo->DY) <= CumulY )
                  {
                    SetSchBoxPlaced( BoxTo );
                    BoxTo->Y = BoxFrom->Y + ConFrom->Y_REL - ConTo->Y_REL;
                    CumulY = BoxTo->Y - 1;
                  }
                  else
                  {        /* BoxTo->Y =  CumulY-BoxTo->DY; */
                  }
                }
              }
            }
          }
        }
      }

      if ( ! IsSchBoxPlaced( BoxTo ) )
      {
        /* LUDO Bug fix 29/10/2004 */
        if ( Begin )
        {
          CumulY = BoxTo->Y; 
        }
        /* ODUL */
        else
        {
          BoxTo->Y = CumulY - BoxTo->DY;
          SetSchBoxPlaced( BoxTo );
          CumulY = BoxTo->Y - 1;
        }
      }
    }

    if ( CellTo->TYPE == SCP_CEL_CON )
    {
      ConTo = (schcon_list *) CellTo->ROOT;

      for ( Chain  = ConTo->NET->CON_NET; 
            Chain != (chain_list *)0;
            Chain = Chain->NEXT )
      {
        if ( IsSchConPlaced( ConTo ) ) break;

        ConFrom = (schcon_list *)Chain->DATA;

        if ( ConFrom != ConTo )
        {
          if ( ( IsSchConInternal( ConFrom ) ) && 
               ( IsSchConOut( ConFrom      ) ) )
          {
            BoxFrom = (schbox_list *) ConFrom->ROOT;

            if ( IsSchBoxTaged( BoxFrom ) )
            {
              CellFrom = (scpcel_list *) BoxFrom->USER;

              if ( CellFrom->COL == ColumnFrom )
              {
                if ( Begin )
                {
                  CumulY = BoxFrom->Y + 2 * (BoxFrom->DY);
                  Begin = 0;
                }

                if ( ( BoxFrom->Y + ConFrom->Y_REL + 2 ) <= CumulY )
                {
                  SetSchConPlaced(ConTo);
                  ConTo->Y_REL = BoxFrom->Y + ConFrom->Y_REL;
                  CumulY = ConTo->Y_REL - 1;
                }
                else
                {        /* ConTo->Y_REL =  CumulY; */
                }
              }
            }
          }
        }
      }

      if ( Begin && IsSchConPlaced( ConTo ) )
      {
        ConTo->Y_REL = CumulY;
        SetSchConPlaced(ConTo);
        CumulY = ConTo->Y_REL - 1;

        /* LUDO 29/10/2004 */
        Begin = 0;
        /* ODUL */
      }
    }
  }

  return (Figure);
}

/*------------------------------------------------------------\
|                                                             |
|                  Function  Loc Build Place                  |
|                                                             |
\------------------------------------------------------------*/

static schfig_list *loc_buildplace( Figure )

  schfig_list *Figure;
{
  scpcol_list    *Column;
  scpcol_list    *MaxColumn;

  MaxColumn = loc_buildplacefirst( Figure );

  for ( Column = MaxColumn; Column->NEXT; Column = Column->NEXT )
  {
    loc_buildplaceout( Figure, Column, Column->NEXT );
  }

  HEAD_SCPCOL = (scpcol_list *)reverse( (chain_list *)HEAD_SCPCOL );

  for ( Column = MaxColumn; Column->NEXT; Column = Column->NEXT )
  {
    loc_buildplacein(Figure, Column->NEXT, Column);
  }

  HEAD_SCPCOL = (scpcol_list *)reverse((chain_list *)HEAD_SCPCOL );

  return ( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                 Function  Loc Routage Canal                 |
|                                                             |
\------------------------------------------------------------*/

static long loc_routage_canal( Figure, ColumnFrom, ColumnTo, Xdep )

  schfig_list    *Figure;
  scpcol_list    *ColumnFrom;
  scpcol_list    *ColumnTo;
  long            Xdep;
{
  scpcel_list    *ScanCell;
  schbox_list    *SchBox;
  schnet_list    *SchNet;
  schnet_list   **SchNetTable;
  schwir_list    *SchWire;
  schcon_list    *ScanCon;
  schcon_list    *SchCon;
  chain_list     *SchNetList = NULL;
  chain_list     *CurChannelNets = NULL;
  chain_list     *ptchain;
  long            numschnet = 0;
  long            channel;
  long            i = 0;

  Xdep += ColumnFrom->MAX_DX_CELL;

  /* Traverse Canal Input and Output Connectors to */
  /* obtain list of SchNets in Canal and their     */
  /* vertical extremities with direction           */

  /* Channel Input Connectors */

  for ( ScanCell  = ColumnFrom->CELL;
        ScanCell != (scpcel_list *)0;
        ScanCell  = ScanCell->NEXT )
  {
      if ( ScanCell->TYPE == SCP_CEL_BOX )
      {
          SchBox = (schbox_list *) ScanCell->ROOT;
          for (ScanCon = SchBox->CON_OUT; ScanCon; ScanCon = ScanCon->NEXT ) {
              SchNet = ScanCon->NET;
              if (!IsSchNetCanal(SchNet)) {
                  numschnet++;
                  SetSchNetCanal(SchNet);
                  SchNetList = addchain(SchNetList, SchNet);
                  SchNet->YMAX = SchBox->Y + ScanCon->Y_REL;
                  SchNet->YMIN = SchBox->Y + ScanCon->Y_REL;
              }
              else if ((SchBox->Y + ScanCon->Y_REL) > (SchNet->YMAX)) {
                  SchNet->YMAX = SchBox->Y + ScanCon->Y_REL;
              }
              else if ((SchBox->Y + ScanCon->Y_REL) < (SchNet->YMIN)) {
                  SchNet->YMIN = SchBox->Y + ScanCon->Y_REL;
              }
          }
      }
      else if (ScanCell->TYPE == SCP_CEL_CON)
      {
          SchCon = (schcon_list *) ScanCell->ROOT;

          SchNet = SchCon->NET;
          if (!IsSchNetCanal(SchNet)) {
              numschnet++;
              SetSchNetCanal(SchNet);
              SchNetList = addchain(SchNetList, SchNet);
              SchNet->YMAX = SchCon->Y_REL;
              SchNet->YMIN = SchCon->Y_REL;
          }
          if ((SchCon->Y_REL) > (SchNet->YMAX)) {
              SchNet->YMAX = SchCon->Y_REL;
          }
          if ((SchCon->Y_REL) < (SchNet->YMIN)) {
              SchNet->YMIN = SchCon->Y_REL;
          }
      }
    }

    /* Channel Output connectors */

    for (ScanCell = ColumnTo->CELL; ScanCell; ScanCell = ScanCell->NEXT ) {
        if (ScanCell->TYPE == SCP_CEL_BOX) {
            SchBox = (schbox_list *) ScanCell->ROOT;
            for (ScanCon = SchBox->CON_IN; ScanCon; ScanCon = ScanCon->NEXT ) {
                SchNet = ScanCon->NET;
                if (!IsSchNetCanal(SchNet)) {
                    numschnet++;
                    SetSchNetCanal(SchNet);
                    SchNetList = addchain(SchNetList, SchNet);
                    SchNet->YMAX = SchBox->Y + ScanCon->Y_REL;
                    SetSchNetMaxOutput(SchNet);
                    SchNet->YMIN = SchBox->Y + ScanCon->Y_REL;
                    SetSchNetMinOutput(SchNet);
                }
                else if ((SchBox->Y + ScanCon->Y_REL) > (SchNet->YMAX)) {
                    SchNet->YMAX = SchBox->Y + ScanCon->Y_REL;
                    SetSchNetMaxOutput(SchNet);
                }
                else if ((SchBox->Y + ScanCon->Y_REL) < (SchNet->YMIN)) {
                    SchNet->YMIN = SchBox->Y + ScanCon->Y_REL;
                    SetSchNetMinOutput(SchNet);
                }
            }
        }
        else if (ScanCell->TYPE == SCP_CEL_CON) {
            SchCon = (schcon_list *) ScanCell->ROOT;

            SchNet = SchCon->NET;
            if (!IsSchNetCanal(SchNet)) {
                numschnet++;
                SetSchNetCanal(SchNet);
                SchNetList = addchain(SchNetList, SchNet);
                SchNet->YMAX = SchCon->Y_REL;
                SetSchNetMaxOutput(SchNet);
                SchNet->YMIN = SchCon->Y_REL;
                SetSchNetMinOutput(SchNet);
            }
            if ((SchCon->Y_REL) > (SchNet->YMAX)) {
                SchNet->YMAX = SchCon->Y_REL;
                SetSchNetMaxOutput(SchNet);
            }
            if ((SchCon->Y_REL) < (SchNet->YMIN)) {
                SchNet->YMIN = SchCon->Y_REL;
                SetSchNetMinOutput(SchNet);
            }
        }
    }

    /* build table of SchNets and sort */

    SchNetTable = (schnet_list **) autallocblock( (1 + numschnet ) * sizeof(schnet_list *));
    for (ptchain = SchNetList; ptchain; ptchain = ptchain->NEXT ) {
        SchNetTable[i++] = (schnet_list *) ptchain->DATA;
    }
    qsort(SchNetTable, numschnet, sizeof(schnet_list *), SchNetCompare);

    /* Allocate a channel number to each SchNet */

    channel = Xdep + 4;
    for (i = 0; i < numschnet; i++) {
        if ((SchNetTable[i])->YMAX == (SchNetTable[i])->YMIN) {
            (SchNetTable[i])->CANAL = channel;
            continue;
        }
        if (CheckNetIntersection(CurChannelNets, SchNetTable[i])) {
            channel += 2;
            if (CurChannelNets)
                freechain(CurChannelNets);
            CurChannelNets = addchain(NULL, SchNetTable[i]);
        }
        else
            CurChannelNets = addchain(CurChannelNets, SchNetTable[i]);
        (SchNetTable[i])->CANAL = channel;
    }
    mbkfree(SchNetTable);

    /* Draw horizontal wires from input column to vertical wire */

    for (ScanCell = ColumnFrom->CELL; ScanCell; ScanCell = ScanCell->NEXT ) {
        if (ScanCell->TYPE == SCP_CEL_BOX) {
            SchBox = ScanCell->ROOT;

            for (ScanCon = SchBox->CON_OUT; ScanCon; ScanCon = ScanCon->NEXT ) {
                SchNet = ScanCon->NET;
                SchWire = addschwir(Figure, SchNet);
                SchWire->X = SchBox->X + ScanCon->X_REL;
                SchWire->DX = SchNet->CANAL;
                SchWire->Y = SchBox->Y + ScanCon->Y_REL;
                SchWire->DY = SchBox->Y + ScanCon->Y_REL;
            }
        }
        else if (ScanCell->TYPE == SCP_CEL_CON) {
            SchCon = (schcon_list *) ScanCell->ROOT;
            SchNet = SchCon->NET;
            SchWire = addschwir(Figure, SchNet);
            /* ICI LUDO
            SchWire->X = SchCon->X_REL + 2;
            */
            SchWire->X  = SchCon->X_REL;
            SchWire->DX = SchNet->CANAL;
            SchWire->Y  = SchCon->Y_REL;
            SchWire->DY = SchCon->Y_REL;
        }
    }

    /* Draw horizontal wires from vertical wire to output column */

    for (ScanCell = ColumnTo->CELL; ScanCell; ScanCell = ScanCell->NEXT ) {
        if (ScanCell->TYPE == SCP_CEL_BOX) {
            SchBox = ScanCell->ROOT;
            SchBox->X = channel + 6 + (ColumnTo->MAX_DX_CELL - SchBox->DX) / 2;

            for (ScanCon = SchBox->CON_IN; ScanCon; ScanCon = ScanCon->NEXT ) {
                SchNet = ScanCon->NET;
                SchWire = addschwir(Figure, SchNet);
                SchWire->X = SchBox->X + ScanCon->X_REL;
                SchWire->DX = SchNet->CANAL;
                SchWire->Y = SchBox->Y + ScanCon->Y_REL;
                SchWire->DY = SchBox->Y + ScanCon->Y_REL;
            }
        }
        else if (ScanCell->TYPE == SCP_CEL_CON) {
            SchCon = (schcon_list *) ScanCell->ROOT;
            SchCon->X_REL = channel + 6;

            SchNet = SchCon->NET;
            SchWire = addschwir(Figure, SchNet);
            /* ICI LUDO 
            SchWire->X = SchCon->X_REL - 2;
            */
            SchWire->X  = SchCon->X_REL;
            SchWire->DX = SchNet->CANAL;
            SchWire->Y  = SchCon->Y_REL;
            SchWire->DY = SchCon->Y_REL;
        }
    }

    /* Draw vertical wires */

    for (ptchain = SchNetList; ptchain; ptchain = ptchain->NEXT ) {
        SchNet = (schnet_list *) ptchain->DATA;
        ClearSchNetCanal(SchNet);
        ClearSchNetDir(SchNet);
        if (SchNet->YMIN == SchNet->YMAX)
            continue;
        SchWire = addschwir(Figure, SchNet);
        SchWire->X = SchNet->CANAL;
        SchWire->DX = SchNet->CANAL;
        SchWire->Y = SchNet->YMIN;
        SchWire->DY = SchNet->YMAX;
    }
    freechain(SchNetList);

    return (channel + 6);
}

static int 
SchNetCompare(net1, net2)
    schnet_list   **net1;
    schnet_list   **net2;
{
    long            type1, type2;

    type1 = GetSchNetDirType(*net1);
    type2 = GetSchNetDirType(*net2);

    if (type1 > type2)
        return (1);
    if (type1 < type2)
        return (-1);

    switch (type1) {
    case 0:
        return (((*net1)->YMAX - (*net1)->YMIN) - ((*net2)->YMAX - (*net2)->YMIN));
        break;
    case SCH_NET_MINDIR:
        return ((*net1)->YMIN - (*net2)->YMIN);
        break;
    case SCH_NET_MAXDIR:
        return ((*net2)->YMAX - (*net1)->YMAX);
        break;
    case SCH_NET_MAXDIR | SCH_NET_MINDIR:
        return (((*net2)->YMAX - (*net2)->YMIN) - ((*net1)->YMAX - (*net1)->YMIN));
        break;
    }
    return 0;
}

static int 
CheckNetIntersection(NetList, Net)
    chain_list     *NetList;
    schnet_list    *Net;
{
    chain_list     *ptchain;
    schnet_list    *CmpNet;

    for (ptchain = NetList; ptchain; ptchain = ptchain->NEXT ) {
        CmpNet = (schnet_list *) ptchain->DATA;
        if (Net->YMIN > CmpNet->YMAX || Net->YMAX < CmpNet->YMIN)
            continue;
        else
            break;
    }
    if (ptchain != NULL)
        return 1;
    else
        return 0;
}

/*------------------------------------------------------------\
|                                                             |
|                      Function  Loc Route                    |
|                                                             |
\------------------------------------------------------------*/

static schfig_list *loc_route( Figure )

  schfig_list *Figure;
{
  scpcol_list *Column_from;
  scpcol_list *Column_to;
  long         Xdep;
  
  Xdep = 0;

  if ( HEAD_SCPCOL != (scpcol_list *)0 )
  {
    for ( Column_from        = HEAD_SCPCOL;
          Column_from->NEXT != (scpcol_list *)0;
          Column_from        = Column_from->NEXT )
    {
      Column_to = Column_from->NEXT;
      Xdep      = loc_routage_canal( Figure, Column_from, Column_to, Xdep );
    }
  }

  return (Figure);
}

/*------------------------------------------------------------\
|                                                             |
|                   Function  Loc Test Trans                  |
|                                                             |
\------------------------------------------------------------*/

static int loc_test_trans( Net, Column )

  schnet_list *Net;
  scpcol_list *Column;
{
  scpcel_list *ScpCel;
  schcon_list *SchCon;
  schbox_list *SchBox;
  chain_list  *ChainCon;

  for ( ChainCon  = Net->CON_NET;
        ChainCon != (chain_list *)0;
        ChainCon  = ChainCon->NEXT )
  {
    SchCon = (schcon_list *) ChainCon->DATA;

    if ( IsSchConInternal( SchCon ) )
    {
      SchBox = (schbox_list *) SchCon->ROOT;

      if ( IsSchBoxTransparence( SchBox ) )
      {
        ScpCel = (scpcel_list *) SchBox->USER;

        if ( ScpCel->COL == Column ) return( 0 );
      }
    }
  }

  return( 1 );
}


/*------------------------------------------------------------\
|                                                             |
|               Function  Loc Placement Initial               |
|                                                             |
\------------------------------------------------------------*/

static schfig_list *loc_placement_initial( Figure )

  schfig_list *Figure;
{
  int             cpt_transparence;

  int             cpt;
  schcon_list    *Con;
  schcon_list    *Con2;
  chain_list     *Chain;

  schbox_list    *Box;
  schbox_list    *Box2;
  schbox_list    *Box4;
  scpcol_list    *Column2;
  scpcol_list    *Column;
  scpcel_list    *Cell;
  scpcel_list    *Cell2;

  schbox_list    *BoxBox4;
  scpcel_list    *BoxCell2;
  schcon_list    *BoxCon;
  schcon_list    *BoxCon2;
  chain_list     *BoxChain;


  /* connecteurs externes de sortie dans la premiere colonne */

  Column2 = addscpcol();
  Column = addscpcol();

  for ( Con  = Figure->CON_OUT;
        Con != (schcon_list *)0;
        Con  = Con->NEXT )
  {
    addschcontoscpcel( Con, addscpcel( Column2 ) );

    for ( Chain  = Con->NET->CON_NET;
          Chain != (chain_list *)0;
          Chain  = Chain->NEXT )
    {
      if ( loc_test_trans( Con->NET, Column ) )
      {
        Box2 = loc_add_transparence( Figure, Con->NET );
        SetSchBoxTaged(Box2);
        Cell2 = addscpcel(Column);
        addschboxtoscpcel(Box2, Cell2);
      }
    }
  }

  /* LUDO 29/10/2004 */
  cpt_transparence = 0;
  /* ODUL */

  while (cpt_transparence != Column->NUMBER_CELL) {

      Column2 = Column;
      Column = addscpcol();

      cpt = Column2->NUMBER_CELL;
      cpt_transparence = 0;

      for (Cell = Column2->CELL;
           Cell != (scpcel_list *)0;
           Cell = Cell->NEXT ) {
          if (Cell->TYPE == SCP_CEL_BOX) {
              Box = (schbox_list *) Cell->ROOT;
              SetSchBoxReTaged(Box);
          }
      }

      for (Cell = Column2->LAST_CELL;
           cpt != 0;
           Cell = (scpcel_list *) Cell->PREV) {
          cpt--;
          if (Cell->TYPE == SCP_CEL_BOX) {
              Box = (schbox_list *) Cell->ROOT;

              for (Con = Box->CON_IN;
                   Con != (schcon_list *)0;
                   Con = Con->NEXT ) {
                  for (Chain = Con->NET->CON_NET;
                       Chain != (chain_list *)0;
                       Chain = Chain->NEXT ) {
                      Con2 = (schcon_list *) Chain->DATA;

                      if (Con != Con2) {
                          if (IsSchConIn(Con2) && IsSchConInternal(Con2)
                              && !IsSchBoxTaged((schbox_list *) Con2->ROOT)
                              && loc_test_trans(Con->NET, Column)) {
                              Box4 = loc_add_transparence(Figure, Con2->NET);
                              Box4->NAME = namealloc("In_int_not_tag");
                              SetSchBoxTaged(Box4);
                              Cell2 = addscpcel(Column);
                              addschboxtoscpcel(Box4, Cell2);
                              cpt_transparence++;
                          }

                          if (IsSchConOut(Con2) && IsSchConInternal(Con2)) {
                              Box4 = (schbox_list *) Con2->ROOT;
                              if (!IsSchBoxTaged(Box4)) {
                                  SetSchBoxTaged(Box4);
                                  Cell2 = addscpcel(Column);
                                  addschboxtoscpcel(Box4, Cell2);

                                  for (BoxCon = Box4->CON_IN;
                                       BoxCon != (schcon_list *)0;
                                       BoxCon = BoxCon->NEXT ) {
                                      for (BoxChain = BoxCon->NET->CON_NET;
                                           BoxChain != (chain_list *)0;
                                           BoxChain = BoxChain->NEXT ) {
                                          BoxCon2 = (schcon_list *) BoxChain->DATA;

                                          if (BoxCon != BoxCon2) {
                                              if (IsSchConInternal(BoxCon2)
                                                  && IsSchBoxTaged((schbox_list *) BoxCon2->ROOT)
                                                  && IsSchBoxReTaged((schbox_list *) BoxCon2->ROOT)
                                                  && loc_test_trans(BoxCon->NET, Column)) {
                                                  BoxBox4 = loc_add_transparence(Figure, BoxCon2->NET);
                                                  BoxBox4->NAME = namealloc("IN__Int_Tag");
                                                  SetSchBoxTaged(BoxBox4);
                                                  BoxCell2 = addscpcel(Column);
                                                  addschboxtoscpcel(BoxBox4, BoxCell2);
                                                  cpt_transparence++;
                                              }
                                          }
                                      }
                                  }
                                  for (BoxCon = Box4->CON_OUT;
                                       BoxCon != (schcon_list *)0;
                                       BoxCon = BoxCon->NEXT ) {
                                      for (BoxChain = BoxCon->NET->CON_NET;
                                           BoxChain != (chain_list *)0;
                                           BoxChain = BoxChain->NEXT ) {
                                          BoxCon2 = (schcon_list *) BoxChain->DATA;

                                          if (BoxCon != BoxCon2) {
                                              if (IsSchConIn(BoxCon2) && IsSchConInternal(BoxCon2)
                                                  && !IsSchBoxReTaged((schbox_list *) BoxCon2->ROOT)
                                                  && loc_test_trans(BoxCon->NET, Column)) {
                                                  BoxBox4 = loc_add_transparence(Figure, BoxCon2->NET);
                                                  BoxBox4->NAME = namealloc("OUT__In_Int_Tag");
                                                  SetSchBoxTaged(BoxBox4);
                                                  BoxCell2 = addscpcel(Column);
                                                  addschboxtoscpcel(BoxBox4, BoxCell2);
                                                  cpt_transparence++;
                                              }
                                          }
                                      }
                                  }
                              }
                          }

                          /* relier un connecteur d'entree */

                          if (IsSchConIn(Con2) && IsSchConExternal(Con2)
                              && loc_test_trans(Con->NET, Column)) {
                              Box4 = loc_add_transparence(Figure, Con2->NET);
                              SetSchBoxTaged(Box4);
                              Cell2 = addscpcel(Column);
                              addschboxtoscpcel(Box4, Cell2);
                              cpt_transparence++;
                          }
                      }
                  }
              }                /* for */
          }
      }
  }
  Column2 = Column;
  Column = addscpcol();
  cpt = Column2->NUMBER_CELL;

  for (Cell = Column2->LAST_CELL;
       cpt != 0;
       Cell = (scpcel_list *) Cell->PREV) {
      cpt--;
      if (Cell->TYPE == SCP_CEL_BOX) {
          Box = (schbox_list *) Cell->ROOT;
          Con2 = Box->CON_IN;
          for (Chain = Con2->NET->CON_NET;
               Chain != (chain_list *)0;
               Chain = Chain->NEXT ) {
              Con = (schcon_list *) Chain->DATA;
              if (IsSchConExternal(Con) && IsSchConIn(Con) && !IsSchConTaged(Con)) {
                  Cell2 = addscpcel(Column);
                  addschcontoscpcel(Con, Cell2);
                  SetSchConTaged(Con);
              }
          }
      }
  }
  return (Figure);
}

/*------------------------------------------------------------\
|                                                             |
|                    Function  Loc Build Box                  |
|                                                             |
\------------------------------------------------------------*/

static schfig_list *loc_buildbox( Figure )

  schfig_list *Figure;
{
  schbox_list    *Box;
  schbox_list    *Box0;
  schbox_list    *BoxN;
  schcon_list    *Con;
  ptype_list     *ScanBox;
  ptype_list     *HeadList;
  long            Y_cont1;
  long            Y_cont2;
  long            MiddleDY;
  long            MiddleInDY;
  long            Delta;
  int             Number;
  int             NumberIn;
  int             NumberBoxIn0;
  int             NumberOut;
  int             Middle;
  int             MiddleIn;

  for ( Box  = Figure->BOX;
        Box != (schbox_list *)0;
        Box  = Box->NEXT )
  {
/*
** Compute the size of the box on Y
*/
    if ( IsSchBoxTransparence( Box ) )
    {
      Box->DY = SCP_BOX_TRANS_DY;
      Y_cont1 = SCP_BOX_TRANS_DY  / 2;
      Y_cont2 = SCP_BOX_TRANS_DY  / 2;
    }
    else
    {
      NumberIn  = Box->NUMBER_IN;
      NumberOut = Box->NUMBER_OUT;

      if ( NumberIn >= NumberOut )
      {
        Box->DY = ( 2 * SCP_BOX_CON_BASE_Y ) + ( ( NumberIn - 1 ) * SCP_BOX_CON_STEP_Y );
  
        Y_cont1 = Box->DY - SCP_BOX_CON_BASE_Y;
        Y_cont2 = Box->DY - SCP_BOX_CON_BASE_Y - 
                  ( ( NumberIn - NumberOut ) * SCP_BOX_CON_STEP_Y / 2 );
      }
      else
      {
        Box->DY = ( 2 * SCP_BOX_CON_BASE_Y ) + ( ( NumberOut - 1 ) * SCP_BOX_CON_STEP_Y );
  
        Y_cont2 = Box->DY - SCP_BOX_CON_BASE_Y;
        Y_cont1 = Box->DY - SCP_BOX_CON_BASE_Y - 
                  ( ( NumberOut - NumberIn ) * SCP_BOX_CON_STEP_Y / 2 );
      }
    }
/*
** Compute the size of the box on X
*/
    if ( IsSchBoxCluster( Box ) )
    {
/*
** Compute the size of the cluster on X
*/
      Box->DX   = 0;

      HeadList  = (ptype_list  *)Box->SOURCE;
      Box0      = (schbox_list *)HeadList->DATA;
      HeadList  = HeadList->NEXT;

      NumberBoxIn0 = 0;

      for ( ScanBox  = HeadList;
            ScanBox != (ptype_list *)0;
            ScanBox  = ScanBox->NEXT )
      {
        NumberBoxIn0++;

        BoxN = (schbox_list *)ScanBox->DATA;

        if ( Box->DX < BoxN->DX ) Box->DX = BoxN->DX;
      }

      Box->DX += Box0->DX + ( SCP_BOX_CON_X * 2 ) + 
                 ( ( SCP_CLUSTER_ROUTE_STEP_X * NumberBoxIn0 ) / 2 ) + SCP_CLUSTER_ROUTE_STEP_X;
/*
** Compute the position of the cluster's outputs on Y
** => The center of input boxes should be in front of the center of Box0's input connectors
*/
      Number   = 1;
      Middle   = NumberBoxIn0 / 2;
      MiddleDY = 0;
/*
** Compute MiddleDY : the center of input boxes
*/
      for ( ScanBox  = HeadList;
            ScanBox != (ptype_list *)0;
            ScanBox  = ScanBox->NEXT )
      {
        BoxN = (schbox_list *)ScanBox->DATA;
        if ( Number <= Middle ) 
        {
          MiddleDY += BoxN->DY;
        }
        else
        {
          if ( NumberBoxIn0 & 1 ) MiddleDY += BoxN->DY / 2;

          break;
        }

        Number++;
      }
/*
** Compute MiddleInDY : the center of Box0's input connectors
*/
      /* TO BE DONE */
      Number     = 1;
      MiddleIn   = Box0->NUMBER_IN / 2;
      MiddleInDY = SCP_BOX_CON_BASE_Y;

      for ( Con  = Box0->CON_IN;
            Con != (schcon_list *)0;
            Con  = Con->NEXT )
      {
        if ( Number < MiddleIn )
        {
          MiddleInDY += SCP_BOX_CON_STEP_Y;
        }
        else
        {
          if ( ! ( Box0->NUMBER_IN & 0x1 ) )
          {
            MiddleInDY += SCP_BOX_CON_STEP_Y / 2;
          }

          break;
        }

        Number++;
      }

      /*
      MiddleInDY = Box0->DY - MiddleInDY;
      Delta      = MiddleDY - ( Box->DY / 2 );
      Box0->Y    = Delta +    ( Box->DY / 2 ) - MiddleInDY;
      */

      Delta    = MiddleDY - ( Box->DY / 2 );
      Box0->Y  = Delta + ( Box->DY - Box0->DY ) / 2 ;

      /*
      MiddleInDY = Box0->DY - MiddleInDY;
      Delta      = MiddleDY - ( Box->DY / 2 ) + MiddleInDY - ( Box0->DY / 2 );
      Box0->Y    = Delta + ( Box->DY - Box0->DX ) / 2;
      */

      Y_cont2 += Delta;

      while ( IsSchBoxCluster( Box0 ) )
      {
        ScanBox = (ptype_list  *)Box0->SOURCE;
        BoxN    = (schbox_list *)ScanBox->DATA;

        Y_cont2 += BoxN->Y - ( ( Box0->DY - BoxN->DY ) / 2);

        Box0 = BoxN;
      }
    }
    else
    {
      if ( ( IsSchBoxTransparence( Box ) ) ||
           ( IsSchBoxClusterNet( Box )   ) )
      {
        Box->DX = 0;
      }
      else
      {
        if ( Box->DY <= SCP_BOX_MIN_DX )
        {
          Box->DX = SCP_BOX_MIN_DX;
        }
        else
        {
          Box->DX = Box->DY;
        }
      }
    }

    for ( Con  = Box->CON_IN;
          Con != (schcon_list *)0;
          Con  = Con->NEXT )
    {
      Con->X_REL = - SCP_BOX_CON_X;
      Con->Y_REL = Y_cont1;

      Y_cont1 = Y_cont1 - SCP_BOX_CON_STEP_Y;

    }

    for ( Con  = Box->CON_OUT;
          Con != (schcon_list *)0;
          Con  = Con->NEXT )
    {
      Con->X_REL = Box->DX + SCP_BOX_CON_X;
      Con->Y_REL = Y_cont2;

      Y_cont2 = Y_cont2 - SCP_BOX_CON_STEP_Y;
    }
  }

  return ( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                    Function  Loc Compact Wire               |
|                                                             |
\------------------------------------------------------------*/

static void loc_compactwirenet( SchNet )

  schnet_list *SchNet;
{
  schwir_list  *ScanWire;
  schwir_list  *DelWire;
  schwir_list  *NextWire;
  schwir_list **PrevWire;
  long          Xmin;
  long          Xmax;
  long          Ymin;
  long          Ymax;
  short         ScanHor;
  short         NextHor;
  long          ScanX1;
  long          ScanX2;
  long          NextX1;
  long          NextX2;
  long          ScanY1;
  long          ScanY2;
  long          NextY1;
  long          NextY2;
/*
** Delete point wire, and order others
*/
  PrevWire = &SchNet->WIRE;
  ScanWire = SchNet->WIRE;

  while ( ScanWire != (schwir_list *)0 )
  {
    if ( ( ScanWire->X == ScanWire->DX ) &&
         ( ScanWire->Y == ScanWire->DY ) )
    {
      *PrevWire = ScanWire->NEXT;
      DelWire   = ScanWire;
      ScanWire  = ScanWire->NEXT;

      freeschwir( DelWire );
    }
    else
    {
      if ( ScanWire->Y == ScanWire->DY )
      {
        Xmin = ScanWire->X;
        Xmax = ScanWire->DX;

        if ( Xmin > Xmax )
        {
          ScanWire->X  = Xmax;
          ScanWire->DX = Xmin;
        }
      }
      else
      {
        Ymin = ScanWire->Y;
        Ymax = ScanWire->DY;

        if ( Ymin > Ymax )
        {
          ScanWire->Y  = Ymax;
          ScanWire->DY = Ymin;
        }
      }

      PrevWire = &ScanWire->NEXT;
      ScanWire = ScanWire->NEXT;
    }
  }
/*
** Build big wire !
*/
  PrevWire = &SchNet->WIRE;
  ScanWire = SchNet->WIRE;

  while ( ScanWire != (schwir_list *)0 )
  {
    if ( ScanWire->Y == ScanWire->DY ) ScanHor = 1;
    else                               ScanHor = 0;

    for ( NextWire  = ScanWire->NEXT;
          NextWire != (schwir_list *)0;
          NextWire  = NextWire->NEXT )
    {
      if ( NextWire->Y == NextWire->DY ) NextHor = 1;
      else                               NextHor = 0;

      if ( ScanHor == NextHor )
      {
/*
** Horizontal wires
*/
        if ( ScanHor == 1 )
        {
          if ( ScanWire->Y == NextWire->Y )
          {
            ScanX1 = ScanWire->X;
            ScanX2 = ScanWire->DX;
            NextX1 = NextWire->X;
            NextX2 = NextWire->DX;
/*
** Intersection between to horizontal wires
*/
            if ( ( NextX1 <= ScanX2 ) &&
                 ( NextX2 >= ScanX1 ) )
            {
              if ( ScanX1 < NextX1 ) Xmin = ScanX1;
              else                   Xmin = NextX1;

              if ( ScanX2 > NextX2 ) Xmax = ScanX2;
              else                   Xmax = NextX2;

              NextWire->X  = Xmin;
              NextWire->DX = Xmax;

              break;
            }
          }
        }
        else
/*
** Vertical wires
*/
        {
          if ( ScanWire->X == NextWire->X )
          {
            ScanY1 = ScanWire->Y;
            ScanY2 = ScanWire->DY;
            NextY1 = NextWire->Y;
            NextY2 = NextWire->DY;
/*
** Intersection between to vertical wires
*/
            if ( ( NextY1 <= ScanY2 ) &&
                 ( NextY2 >= ScanY1 ) )
            {
              if ( ScanY1 < NextY1 ) Ymin = ScanY1;
              else                   Ymin = NextY1;

              if ( ScanY2 > NextY2 ) Ymax = ScanY2;
              else                   Ymax = NextY2;

              NextWire->Y  = Ymin;
              NextWire->DY = Ymax;

              break;
            }
          }
        }
      }
    }

    if ( NextWire != (schwir_list *)0 )
    {
      *PrevWire = ScanWire->NEXT;
      DelWire   = ScanWire;
      ScanWire  = ScanWire->NEXT;

      freeschwir( DelWire );
    }
    else
    {
      PrevWire = &ScanWire->NEXT;
      ScanWire = ScanWire->NEXT;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Function  Loc Clean All                  |
|                                                             |
\------------------------------------------------------------*/

static void loc_clean_all( Figure )

  schfig_list *Figure;
{
  schbox_list *ScanBox;
  schnet_list *ScanNet;

  for ( ScanBox  = Figure->BOX;
        ScanBox != (schbox_list *)0;
        ScanBox  = ScanBox->NEXT )
  {
    if ( (   IsSchBoxCluster( ScanBox   ) ) &&
         ( ! IsSchBoxClustered( ScanBox ) ) )
    {
      loc_destroycluster( Figure, ScanBox );
    }
  }

  for ( ScanBox  = Figure->BOX;
        ScanBox != (schbox_list *)0;
        ScanBox  = ScanBox->NEXT )
  {
    if ( IsSchBoxTransparence( ScanBox ) )
    {
      loc_destroytransparence( Figure, ScanBox );
    }
  }

  for ( ScanBox  = Figure->BOX;
        ScanBox != (schbox_list *)0;
        ScanBox  = ScanBox->NEXT )
  {
    if ( IsSchBoxClusterNet( ScanBox ) )
    {
      loc_destroyclusternet( Figure, ScanBox );
    }
  }

  for ( ScanNet  = Figure->NET;
        ScanNet != (schnet_list *)0;
        ScanNet  = ScanNet->NEXT )
  {
    loc_compactwirenet( ScanNet );
  }

  delscpcol();
}

/*------------------------------------------------------------\
|                                                             |
|                         Function  Build                     |
|                                                             |
\------------------------------------------------------------*/

void placerouteschfig( SchFigure )

  schfig_list *SchFigure;
{
# ifdef DEBUG
    fprintf(stdout, "--- Detection des clusters ---\n");
    fflush(stdout);
# endif
    loc_buildcluster(SchFigure);
# ifdef DEBUG
    fprintf(stdout, "----- Placement  initial -----\n");
    fflush(stdout);
# endif
    loc_placement_initial(SchFigure);
# ifdef DEBUG
    fprintf(stdout, "--- Fabrication des boites ---\n");
    fflush(stdout);
    fflush(stdout);
# endif
    loc_buildbox(SchFigure);
# ifdef DEBUG
    fprintf(stdout, "---------  Placement ---------\n");
    fflush(stdout);
    fflush(stdout);
# endif
    loc_buildplace(SchFigure);
# ifdef DEBUG
    fprintf(stdout, "----------  Routage ----------\n");
    fflush(stdout);
    fflush(stdout);
# endif
    loc_route(SchFigure);
# ifdef DEBUG
    fprintf(stdout, "---------- Nettoyage  --------\n");
    fflush(stdout);
# endif
    loc_clean_all(SchFigure);
}
