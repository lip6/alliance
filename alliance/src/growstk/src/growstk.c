#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "mph.h"
#include "mpu.h"
#include "mut.h"
#include "rds.h"
#include "rfm.h"
#include "rpr.h"
#include "rwi.h"
#include "rut.h"
#include "rtl.h"

#define NEED_PATCH       0x08000000
#define NEED_PATCH_MASK  0x08000000
#define OBSTACLE_NORTH   0x80000000
#define OBSTACLE_SOUTH   0x40000000
#define OBSTACLE_EAST    0x20000000
#define OBSTACLE_WEST    0x10000000
#define OBSTACLE_MASK    0xF0000000

#define SetRdsNeedPatch(R)  ((R)->FLAGS = ((R)->FLAGS | NEED_PATCH_MASK))
#define RazRdsNeedPatch(R)  ((R)->FLAGS = ((R)->FLAGS & ~NEED_PATCH_MASK))
#define IsRdsNeedPatch(R)   ((R)->FLAGS & NEED_PATCH_MASK)

#define SetRdsObstacle(R,O) ((R)->FLAGS = ((R)->FLAGS | (O)))
#define RazRdsObstacle(R)   ((R)->FLAGS = ((R)->FLAGS & ~OBSTACLE_MASK))
#define IsRdsObstacle(R)    ((R)->FLAGS & OBSTACLE_MASK)

#define rds2mbklayer(L) (((L)==RDS_ALU2) ? ALU2 :\
                         ((L)==RDS_ALU3) ? ALU3 :\
                         ((L)==RDS_ALU4) ? ALU4 :\
                         ((L)==RDS_ALU5) ? ALU5 :\
                         ((L)==RDS_ALU6) ? ALU6 :0)

/* return 1 if r1 and r2 are in touch
 */
int RecInTouch (rdsrec_list * r1, rdsrec_list * r2)
{
   if (  (  ((r2->X >= r1->X) && ((r2->X)          <= (r1->X + r1->DX))) 
         || ((r2->X <= r1->X) && ((r2->X + r2->DX) >= (r1->X)))         
         )
      && (  ((r2->Y >= r1->Y) && ((r2->Y)          <= (r1->Y + r1->DY))) 
         || ((r2->Y <= r1->Y) && ((r2->Y + r2->DY) >= (r1->Y)))         
         ) 
      )
     return 1;
   return 0;
}

long verbose = 1;

void usage (char *av[])
{
  printf ("Usage : %s [-h] [-v level] [-o OutFile] InFile\n\n", av[0]);
  printf ("  version %s\n", VERSION);
  printf ("  -h     this help\n");
  printf ("  -v     verbose mode (level is 1:2:3:4)\n");
  printf ("  -o     define the output file (InFile by default)\n");
  printf ("  InFile define the input file\n");
  printf ("\n uses only symbolic technology file !!!\n");
  printf ("\n");
  exit (1);
}

void getoption (int ac, char *av[], char **OutFile, char **InFile)
{
  char option;
  while ((option = getopt (ac, av, "hv:o:")) != EOF)
  {
    switch (option)
    {
    case 'v':
      verbose = *optarg - '0';
      if ((verbose < 0) || (verbose > 9))
        usage (av);
      break;
    case 'o':
      *OutFile = namealloc (optarg);
      break;
    default:
      usage (av);
    }
  }
  if (optind == ac - 1)
  {
    *InFile = namealloc (av[optind]);
  }
  else
    usage (av);
}

#define  NbLayerList    5
#define  EquivNbLayerList   2
int main (int ac, char *av[])
{
  char           *InFile, *OutFile;
  phfig_list     *PhFig;
  phseg_list     *PhSeg;
  phvia_list     *PhVia;
  rdsfig_list    *RdsFig;
  rdswindow      *Window;
  long           WX, WY, Offset, Pitch, BorderX, BorderY;
  rdswin_list    *Win;
  rdswinrec_list *WinRec;
  rdsrec_list    *NewRec = NULL;
  rdsrec_list    *ScanRec, *WinScanRec;
  rdsrec_list    *RecNorth, *RecSouth, *RecEast, *RecWest;
  int            Layer, WinLayer;
  int            LayerIdx, WinLayerIdx;
  int            MinLayerArea[NbLayerList] = 
                     { 2,  // Alu2
                       2,  // Alu3
                       2,  // Alu4
                       2,  // Alu5
                       2}; // Alu6
  int            LayerList[NbLayerList][EquivNbLayerList] = 
                     {{RDS_LAYER_ALU2, RDS_LAYER_TALU2},
                      {RDS_LAYER_ALU3, RDS_LAYER_TALU3},
                      {RDS_LAYER_ALU4, RDS_LAYER_TALU4},
                      {RDS_LAYER_ALU5, RDS_LAYER_TALU5},
                      {RDS_LAYER_ALU6, RDS_LAYER_TALU6}};
  int            Index;
  ht_t           *dejavu = htinit (63179);
  char           buffer[1000];

  /* read parameters & environment */
  mbkenv ();
  rdsenv ();
  loadrdsparam ();
  if (RDS_LAMBDA/RDS_UNIT != 1) usage (av);

  Pitch = 5 * RDS_LAMBDA;

  getoption (ac, av, &OutFile, &InFile);
  if (verbose != 0)
    alliancebanner ("GrowStk", VERSION, "grows the stacked vias", "2002", ALLIANCE_VERSION);

  /* read files and buids mbk & rds structures */
  PhFig = getphfig (InFile, 'A');
  PhFig -> NAME = namealloc (OutFile);
  for (PhSeg = PhFig->PHSEG; PhSeg; PhSeg = PhSeg->NEXT)
     switch (PhSeg->LAYER)
     { 
     case TALU2 : PhSeg->LAYER = ALU7; break;
     case TALU3 : PhSeg->LAYER = ALU8; break;
     case TALU4 : PhSeg->LAYER = ALU9; break;
     case TALU5 : PhSeg->LAYER = TALU7; break;
     case TALU6 : PhSeg->LAYER = TALU8; break;
     };

  RdsFig = figmbkrds (PhFig, 0, 0);

  RecNorth = allocrdsrec(0); 
  RecSouth = allocrdsrec(0); 
  RecEast = allocrdsrec(0); 
  RecWest = allocrdsrec(0); 
  
  /* build windows */
  Window = buildrdswindow (RdsFig);
  for (LayerIdx = 0; LayerIdx < NbLayerList; LayerIdx++)
  {
    int first = 1;
    Layer = RDS_DYNAMIC_LAYER[LayerList[LayerIdx][0]];
    if (RdsFig->LAYERTAB)
    {
      for (ScanRec=RdsFig->LAYERTAB[Layer]; ScanRec; ScanRec = ScanRec->NEXT)
      {
        if (ScanRec == NULL) break;
        if ((ScanRec->DX > RDS_LAMBDA * MinLayerArea[LayerIdx]) 
        ||  (ScanRec->DY > RDS_LAMBDA * MinLayerArea[LayerIdx]))
           continue;
        
        if (verbose > 3)
        {
          if (first) printf ("LAYER = %s\n", RDS_LAYER_NAME[Layer]);
          printf ("X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f\n", 
                  (float)ScanRec->X/RDS_UNIT, (float)ScanRec->Y/RDS_UNIT, 
                  (float)ScanRec->DX/RDS_UNIT, (float)ScanRec->DY/RDS_UNIT);
          first = 0;
        }  
      
        /* there is a small square of metal */
        /* scan all rectangles of the current window */
        WX = ScanRec->X / Window->SIDE;
        WY = ScanRec->Y / Window->SIDE;
        if (verbose > 2) printf ("  WINDOW (%ld,%ld) ; (X1,Y1,X2,Y2)=(%.2f,%.2f,%.2f,%.2f) \n", 
                                 WX, WY,
                                 (float)(WX*Window->SIDE)/RDS_UNIT, 
                                 (float)(WY*Window->SIDE)/RDS_UNIT,
                                 (float)((WX+1)*Window->SIDE)/RDS_UNIT, 
                                 (float)((WY+1)*Window->SIDE)/RDS_UNIT);
      
        Offset = (WY) * Window->DX + (WX);
        Win = Window->WINTAB + Offset;
        {
          int first = 1;
          for (WinLayerIdx = 0; WinLayerIdx < EquivNbLayerList; WinLayerIdx++)
          {
            WinLayer = RDS_DYNAMIC_LAYER[LayerList[LayerIdx][WinLayerIdx]];
            for (WinRec = Win->LAYERTAB[WinLayer]; WinRec != (rdswinrec_list *) NULL; WinRec = WinRec->NEXT)
            {
              for (Index = 0; Index < RWI_MAX_REC; Index++)
              {
                WinScanRec = WinRec->RECTAB[Index];
                if (WinScanRec == NULL) break;
                if (  (ScanRec->X == WinScanRec->X) 
                   && (ScanRec->Y == WinScanRec->Y) 
                   && (ScanRec->DX == WinScanRec->DX) 
                   && (ScanRec->DY == WinScanRec->DY) 
                   )
                  continue;
                if (RecInTouch (ScanRec, WinScanRec)) 
                {
                  if (verbose > 3) 
                    printf ("  ABORT : X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f is in touch\n", 
                            (float)WinScanRec->X/RDS_UNIT, (float)WinScanRec->Y/RDS_UNIT, 
                            (float)WinScanRec->DX/RDS_UNIT, (float)WinScanRec->DY/RDS_UNIT);
                  goto in_touch;
                }  
                if (verbose > 3)
                {
                  if (first) printf ("  NLAYER = %s\n", RDS_LAYER_NAME[WinLayer]);
                  printf ("  X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f\n", 
                          (float)WinScanRec->X/RDS_UNIT, (float)WinScanRec->Y/RDS_UNIT, 
                          (float)WinScanRec->DX/RDS_UNIT, (float)WinScanRec->DY/RDS_UNIT);
                  first = 0;
                }  
              }
            }  
          }
        } 
        if (verbose > 3) printf ("  need patch\n");
        SetRdsNeedPatch (ScanRec);
      
        RecNorth->X = ScanRec->X; RecNorth->DX = ScanRec->DX; RecNorth->Y = ScanRec->Y; RecNorth->DY = ScanRec->DY + Pitch; 
        RecSouth->X = ScanRec->X; RecSouth->DX = ScanRec->DX; RecSouth->Y = ScanRec->Y - Pitch; RecSouth->DY = ScanRec->DY + Pitch; 
        RecEast->X = ScanRec->X - Pitch; RecEast->DX = ScanRec->DX + Pitch; RecEast->Y = ScanRec->Y; RecEast->DY = ScanRec->DY; 
        RecWest->X = ScanRec->X; RecWest->DX = ScanRec->DX + Pitch; RecWest->Y = ScanRec->Y; RecWest->DY = ScanRec->DY; 
      
        for (BorderX = -1; BorderX <= 1; BorderX++)
        {
          if (BorderX+WX == -1) continue;
          if (BorderX+WX == Window->DX) continue;
          for (BorderY = -1; BorderY <= 1; BorderY++)
          {
            if (BorderY+WY == -1) continue;
            if (BorderY+WY == Window->DY) continue;
            if (verbose > 2) printf ("  NEIGHBOUR WINDOW (%ld,%ld) ; (X1,Y1,X2,Y2)=(%.2f,%.2f,%.2f,%.2f) \n", 
                                 WX+BorderX, WY+BorderY,
                                 (float)((WX+BorderX)*Window->SIDE)/RDS_UNIT, 
                                 (float)((WY+BorderY)*Window->SIDE)/RDS_UNIT,
                                 (float)(((WX+BorderX)+1)*Window->SIDE)/RDS_UNIT, 
                                 (float)(((WY+BorderY)+1)*Window->SIDE)/RDS_UNIT);
      
            Offset = (WY+BorderY) * Window->DX + (WX+BorderX);
            Win = Window->WINTAB + Offset;
                  
            if (Win->LAYERTAB)
            {
              for (WinLayerIdx = 0; WinLayerIdx < EquivNbLayerList; WinLayerIdx++)
              {
                int first = 1;
                WinLayer = RDS_DYNAMIC_LAYER[LayerList[LayerIdx][WinLayerIdx]];
                for (WinRec = Win->LAYERTAB[WinLayer]; WinRec != (rdswinrec_list *) NULL; WinRec = WinRec->NEXT)
                {
                  for (Index = 0; Index < RWI_MAX_REC; Index++)
                  {
                    WinScanRec = WinRec->RECTAB[Index];
                    if (WinScanRec == NULL) break;
                    if (  (ScanRec->X == WinScanRec->X) 
                       && (ScanRec->Y == WinScanRec->Y) 
                       && (ScanRec->DX == WinScanRec->DX) 
                       && (ScanRec->DY == WinScanRec->DY) 
                       )
                      continue; // si c'est soit meme on continue 
                    
                    // seance de ratrapage : on cherche dans les fenetres
                    // voisines, si il n'y aurait pas des rectangles qui
                    // touchent le rectangle en test MAIS QUI N'AURAIT PAS
                    // ETE VU DANS LA FENETRE CENTRALE, EN PRINCIPE
                    // C'EST IMPOSSIBLE MAIS JE L'AI VU
                    if (RecInTouch (ScanRec, WinScanRec)) 
                    {
                      if (verbose > 3) 
                        printf ("  ABORT : X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f is in touch\n", 
                                (float)WinScanRec->X/RDS_UNIT, (float)WinScanRec->Y/RDS_UNIT, 
                                (float)WinScanRec->DX/RDS_UNIT, (float)WinScanRec->DY/RDS_UNIT);
                      RazRdsNeedPatch (ScanRec);
                      RazRdsObstacle (ScanRec);
                      goto in_touch;
                    }  
                    if (RecInTouch (RecNorth, WinScanRec))
                    {
                      SetRdsObstacle(ScanRec,OBSTACLE_NORTH);
                      if (verbose > 3) 
                      {
                        if (first) printf ("  NLAYER = %s\n", RDS_LAYER_NAME[WinLayer]);
                        printf ("  X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f is an obstacle north\n", 
                                (float)WinScanRec->X/RDS_UNIT, (float)WinScanRec->Y/RDS_UNIT, 
                                (float)WinScanRec->DX/RDS_UNIT, (float)WinScanRec->DY/RDS_UNIT);
                        first = 0;
                      }  
                    }
                    if (RecInTouch (RecSouth, WinScanRec))
                    {
                      SetRdsObstacle(ScanRec,OBSTACLE_SOUTH);
                      if (verbose > 3) 
                      {
                        if (first) printf ("  NLAYER = %s\n", RDS_LAYER_NAME[WinLayer]);
                        printf ("  X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f is an obstacle south\n", 
                                (float)WinScanRec->X/RDS_UNIT, (float)WinScanRec->Y/RDS_UNIT, 
                                (float)WinScanRec->DX/RDS_UNIT, (float)WinScanRec->DY/RDS_UNIT);
                        first = 0;
                      }  
                    }
                    if (RecInTouch (RecWest, WinScanRec)) 
                    {
                      SetRdsObstacle(ScanRec,OBSTACLE_WEST);
                      if (verbose > 3) 
                      {
                        if (first) printf ("  NLAYER = %s\n", RDS_LAYER_NAME[WinLayer]);
                        printf ("  X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f is an obstacle west\n", 
                                (float)WinScanRec->X/RDS_UNIT, (float)WinScanRec->Y/RDS_UNIT, 
                                (float)WinScanRec->DX/RDS_UNIT, (float)WinScanRec->DY/RDS_UNIT);
                        first = 0;
                      }  
                    }
                    if (RecInTouch (RecEast, WinScanRec)) 
                    {
                      SetRdsObstacle(ScanRec,OBSTACLE_EAST);
                      if (verbose > 3) 
                      {
                        if (first) printf ("  NLAYER = %s\n", RDS_LAYER_NAME[WinLayer]);
                        printf ("  X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f is an obstacle east\n", 
                                (float)WinScanRec->X/RDS_UNIT, (float)WinScanRec->Y/RDS_UNIT, 
                                (float)WinScanRec->DX/RDS_UNIT, (float)WinScanRec->DY/RDS_UNIT);
                        first = 0;
                      }  
                    }
                  }
                }  
              } 
            }  
          } 
        } 
        in_touch :
      }
    }  
  }
  if (verbose > 1)
    printf ("Segments that need patches :\n");

  for (LayerIdx = 0; LayerIdx < NbLayerList; LayerIdx++)
  {
    int first = 1;
    Layer = RDS_DYNAMIC_LAYER[LayerList[LayerIdx][0]];
    for (ScanRec=RdsFig->LAYERTAB[Layer]; ScanRec; ScanRec = ScanRec->NEXT)
    {
      if (IsRdsNeedPatch(ScanRec))
      {  
        rdsrec_list * rec;

        sprintf (buffer, "%d %d %d %d %d", 
                 ScanRec->X, ScanRec->Y, ScanRec->DX, ScanRec->DY, ScanRec->FLAGS);
        if (!htget (dejavu, buffer))
        {
          htset (dejavu, buffer);
          rec = allocrdsrec(0);
          rec->NEXT = NewRec;
          NewRec = rec;
          NewRec->X = ScanRec->X;
          NewRec->Y = ScanRec->Y;
          NewRec->DX = ScanRec->DX;
          NewRec->DY = ScanRec->DY;
          NewRec->FLAGS = ScanRec->FLAGS;
          NewRec->NAME = ScanRec->NAME;
          NewRec->USER = NULL;
          
          if (verbose > 1)
          {
            if (first) printf ("LAYER = %s\n", RDS_LAYER_NAME[Layer]);
            printf ("  X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f, OBS=%s%s%s%s\n", 
                    (float)ScanRec->X/RDS_UNIT, (float)ScanRec->Y/RDS_UNIT, 
                    (float)ScanRec->DX/RDS_UNIT, (float)ScanRec->DY/RDS_UNIT,
            (IsRdsObstacle(ScanRec) & OBSTACLE_NORTH)?"NORTH ":"",
            (IsRdsObstacle(ScanRec) & OBSTACLE_SOUTH)?"SOUTH ":"",
            (IsRdsObstacle(ScanRec) & OBSTACLE_EAST)?"EAST ":"",
            (IsRdsObstacle(ScanRec) & OBSTACLE_WEST)?"WEST ":""
            );
            first = 0;
          }  
          if ((IsRdsObstacle(ScanRec) & OBSTACLE_NORTH) == 0)
          {  
            NewRec->DY += Pitch; 
          } 
          else  
          if ((IsRdsObstacle(ScanRec) & OBSTACLE_EAST) == 0)
          {  
            NewRec->DX += Pitch;
            NewRec->X -= Pitch;
          } 
          else  
          if ((IsRdsObstacle(ScanRec) & OBSTACLE_SOUTH) == 0)
          {  
            NewRec->DY += Pitch;
            NewRec->Y -= Pitch;
          } 
          else  
          if ((IsRdsObstacle(ScanRec) & OBSTACLE_WEST) == 0)
          {  
            NewRec->DX += Pitch;
          } 
          else
          {
            if (first) printf ("LAYER = %s\n", RDS_LAYER_NAME[Layer]);
            printf ("ERROR  X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f CANNOT BE REPAIRED\n", 
                    (float)ScanRec->X/RDS_UNIT, (float)ScanRec->Y/RDS_UNIT, 
                    (float)ScanRec->DX/RDS_UNIT, (float)ScanRec->DY/RDS_UNIT);
            first = 0;
          }
        }  
        else
        {
          if (verbose > 2)
          {
            printf ("DEJA VU : LAYER = %s ", RDS_LAYER_NAME[Layer]);
            printf ("  X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f, OBS=%s%s%s%s\n", 
                    (float)ScanRec->X/RDS_UNIT, (float)ScanRec->Y/RDS_UNIT, 
                    (float)ScanRec->DX/RDS_UNIT, (float)ScanRec->DY/RDS_UNIT,
            (IsRdsObstacle(ScanRec) & OBSTACLE_NORTH)?"NORTH ":"",
            (IsRdsObstacle(ScanRec) & OBSTACLE_SOUTH)?"SOUTH ":"",
            (IsRdsObstacle(ScanRec) & OBSTACLE_EAST)?"EAST ":"",
            (IsRdsObstacle(ScanRec) & OBSTACLE_WEST)?"WEST ":""
            );
          }  
        }
      }
    }  
  }
  for (ScanRec=NewRec; ScanRec; ScanRec = ScanRec->NEXT)
  {
     char MbkLayer = rds2mbklayer(GetRdsLayer(ScanRec));
     char MbkLayer_bis;
      
     switch (MbkLayer)
     { 
     case ALU2 : MbkLayer_bis = TALU2; break;
     case ALU3 : MbkLayer_bis = TALU3; break;
     case ALU4 : MbkLayer_bis = TALU4; break;
     case ALU5 : MbkLayer_bis = TALU5; break;
     case ALU6 : MbkLayer_bis = TALU6; break;
     };

     if (MbkLayer==0)      
     {
       printf ("Internal error during the layer translation\n");
       printf ("LAYER = %s X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f\n", RDS_LAYER_NAME[GetRdsLayer(ScanRec)],
               (float)ScanRec->X/RDS_UNIT, (float)ScanRec->Y/RDS_UNIT, 
               (float)ScanRec->DX/RDS_UNIT, (float)ScanRec->DY/RDS_UNIT);
       exit (1);
     }
     if (verbose > 1)
     {
       printf ("LAYER = %s X=%.2f, Y=%.2f, DX=%.2f, DY=%.2f\n", RDS_LAYER_NAME[GetRdsLayer(ScanRec)],
               (float)ScanRec->X/RDS_UNIT, (float)ScanRec->Y/RDS_UNIT, 
               (float)ScanRec->DX/RDS_UNIT, (float)ScanRec->DY/RDS_UNIT);
     }  

     if (ScanRec->DX < ScanRec->DY)
     {
       int SHRINK_BOT, SHRINK_TOP;
       if (ScanRec->Y % Pitch == 0)
       {
         SHRINK_BOT = 4;
         SHRINK_TOP = 1;
       }
       else
       {
         SHRINK_BOT = 1;
         SHRINK_TOP = 4;
       }
       addphseg (PhFig, MbkLayer,
                  (SCALE_X * ScanRec->DX) / RDS_UNIT,
                  (SCALE_X * (ScanRec->X + ScanRec->DX / 2)) / RDS_UNIT , 
                  (SCALE_X * (ScanRec->Y + SHRINK_BOT * RDS_LAMBDA)) / RDS_UNIT , 
                  (SCALE_X * (ScanRec->X + ScanRec->DX / 2)) / RDS_UNIT , 
                  (SCALE_X * (ScanRec->Y + ScanRec->DY - SHRINK_TOP * RDS_LAMBDA)) / RDS_UNIT , 
                  ScanRec->NAME);
       addphseg (PhFig, MbkLayer_bis,
                  (SCALE_X * ScanRec->DX) / RDS_UNIT,
                  (SCALE_X * (ScanRec->X + ScanRec->DX / 2)) / RDS_UNIT , 
                  (SCALE_X * (ScanRec->Y + RDS_LAMBDA)) / RDS_UNIT , 
                  (SCALE_X * (ScanRec->X + ScanRec->DX / 2)) / RDS_UNIT , 
                  (SCALE_X * (ScanRec->Y + ScanRec->DY - RDS_LAMBDA)) / RDS_UNIT , 
                  "obs");
     }
     else  
     {  
       int SHRINK_RIG, SHRINK_LEF;
       if (ScanRec->X % Pitch == 0)
       {
         SHRINK_RIG = 4;
         SHRINK_LEF = 1;
       }
       else
       {
         SHRINK_RIG = 1;
         SHRINK_LEF = 4;
       }
       addphseg (PhFig, MbkLayer,
                 (SCALE_X * ScanRec->DY) / RDS_UNIT,
                 (SCALE_X * (ScanRec->X + SHRINK_RIG * RDS_LAMBDA)) / RDS_UNIT , 
                 (SCALE_X * (ScanRec->Y + ScanRec->DY / 2)) / RDS_UNIT , 
                 (SCALE_X * (ScanRec->X + ScanRec->DX - SHRINK_LEF * RDS_LAMBDA)) / RDS_UNIT , 
                 (SCALE_X * (ScanRec->Y + ScanRec->DY / 2)) / RDS_UNIT , 
                 ScanRec->NAME);
       addphseg (PhFig, MbkLayer_bis,
                 (SCALE_X * ScanRec->DY) / RDS_UNIT,
                 (SCALE_X * (ScanRec->X + RDS_LAMBDA)) / RDS_UNIT , 
                 (SCALE_X * (ScanRec->Y + ScanRec->DY / 2)) / RDS_UNIT , 
                 (SCALE_X * (ScanRec->X + ScanRec->DX - RDS_LAMBDA)) / RDS_UNIT , 
                 (SCALE_X * (ScanRec->Y + ScanRec->DY / 2)) / RDS_UNIT , 
                 "obs");
    }
  }
  for (PhSeg = PhFig->PHSEG; PhSeg; PhSeg = PhSeg->NEXT)
     switch (PhSeg->LAYER)
     { 
     case ALU7  : PhSeg->LAYER = TALU2; break;
     case ALU8  : PhSeg->LAYER = TALU3; break;
     case ALU9  : PhSeg->LAYER = TALU4; break;
     case TALU7 : PhSeg->LAYER = TALU5; break;
     case TALU8 : PhSeg->LAYER = TALU6; break;
     };
  savephfig (PhFig);
  return 0;
}
