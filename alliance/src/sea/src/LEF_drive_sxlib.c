/*
 *  $Id: LEF_drive_sxlib.c,v 1.5 2004/09/27 16:33:43 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :           alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./LEF_drive_sxlib.c"                     |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "LEF_drive.h"
# include  "LEF_drive_sxlib.h"


/*  ------------------------------------------------------------------
 *  Local constants.
 */


# define          XGRID            MBKSCALE(5)
# define          YGRID            MBKSCALE(5)
# define          YSLICE           MBKSCALE(50)
# define            EXT_ALU1       MBKSCALE(0.5)
# define            EXT_ALU2       MBKSCALE(1.0)
# define            EXT_ALU3       MBKSCALE(1.0)
# define            EXT_ALU4       MBKSCALE(1.0)
# define            EXT_ALU5       MBKSCALE(1.0)
# define            EXT_ALU6       MBKSCALE(1.0)
# define          WIDTH_CALUx      MBKSCALE(2)
# define      MIN_WIDTH_ALU1       MBKSCALE(1)
# define      WIDTH_OBS_ALU1       MBKSCALE(2)
# define     HEIGHT_OBS_ALU1       MBKSCALE(2)
# define  LEFTRIGHT_OBS_ALU1       MBKSCALE(2.5)
# define   F_POWER_PIN_LEFT        0x00000001
# define   F_POWER_PIN_RIGHT       0x00000002


/*  ------------------------------------------------------------------
 *  Internal structures definitions.
 */

 typedef struct eGrid_s {
             long  x0;
             long  y0;
             long  x1;
             long  y1;
             long  isPoint;
   struct eGrid_s *Next;
 } eGrid_t;


/*  ------------------------------------------------------------------
 *  Local variables (prefix 'LV_').
 */


  static phfig_list *LV_phfig;       /* Working physical figure.         */
  static lofig_list *LV_lofig;       /* Working logical figure.          */
  static   eMACRO_t *LV_pMACRO;
  static    eGrid_t *LV_tHolesALU1 = (eGrid_t*)NULL;
  static       char *LV_ref_ref;     /* Pointer to the "ref_ref" string. */
  static       char *LV_ref_con;     /* Pointer to the "ref_con" string. */
  static       long  LV_flags;       /* Conversion flags.                */


/*  ------------------------------------------------------------------
 *  Internal functions declarations.
 */

static   long       getLayerExt   __FP((char aLayer));
static  eGrid_t    *getTGrid      __FP((eGrid_t **aptGrid, long aX, long aY));
static  eGrid_t    *addTGrid      __FP((eGrid_t **aptGrid,
                                        long      aX0,
                                        long      aY0,
                                        long      aX1,
                                        long      aY2));
static   void       phseg2TGrid   __FP((eGrid_t    **aptGrid,
                                        phseg_list  *apPhSeg));
static   void       TGrid2PORT    __FP((ePORT_t **aplPORT, eGrid_t *aptGrid));
static   char      *refCon2Name   __FP((char *asRefCon));
static    int       onGrid        __FP((long aX, long aY));
static  locon_list *getLoCon      __FP((char *asName));
static   void       SxLib2SYMMETRY    __FP((void));
static   void       AddEmptyPowerPIN  __FP((char *asName));
static   void       AddPowerPIN   __FP((char *asName,
                                        char  aDir,
                                        char  aLayer,
                                        long  aY,
                                        long  aWidth,
                                        long  aFlags));
static   void       SxLib2PowerPINS __FP((char  aLayer,
                                          char  aDir,
                                          char *asSuffix));
static   void       phref2PINS    __FP((void));
static   void       phseg2PINS    __FP((void));
static   void       SxLib2SignalPINS __FP((void));
static   void       insertPIN     __FP((ePIN_t **appHead,
                                        ePIN_t **appTail,
                                        ePIN_t  *apPIN));
static   void       sortPINS      __FP((void));
static   void       checkPIN      __FP((void));
# if 0
static   void       SxLib2ALU1OBS __FP((void));
# endif
static   void       SxLib2ALUxOBS __FP((char aLayer));
static   void       revPINPORT    __FP((void));
static eMACRO_t    *SxLib2MACRO   __FP((void));


/*
 *  /--------------------------------------------------------------------\
 *  |                     Functions Definitions                          |
 *  \--------------------------------------------------------------------/
 */


/*  ----------------------------------------------------------------------
 *  Function  :  "getLayerExt()".
 */

static long  getLayerExt(aLayer)
  char  aLayer;
{
  switch(aLayer) {
    case ALU1: return(EXT_ALU1);
    case ALU2: return(EXT_ALU2);
    case ALU3: return(EXT_ALU3);
    case ALU4: return(EXT_ALU4);
    case ALU5: return(EXT_ALU5);
    case ALU6: return(EXT_ALU6);
  }

  return(0L);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "getTGrid()".
 */

static eGrid_t *getTGrid(aptGrid, aX, aY)
  eGrid_t  **aptGrid;
   long    aX, aY;
{
  eGrid_t *pGrid;


  for(pGrid = *aptGrid; pGrid != (eGrid_t*)NULL; pGrid = pGrid->Next) {
    if (!pGrid->isPoint) continue;
    if ((pGrid->x0 == aX) && (pGrid->y0 == aY)) return(pGrid);
  }

  return((eGrid_t*)NULL);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "addTGrid()".
 */

static eGrid_t *addTGrid(aptGrid, aX0, aY0, aX1, aY1)
  eGrid_t  **aptGrid;
     long    aX0, aY0, aX1, aY1;
{
  eGrid_t *pGrid = NULL;
  long     isPoint;


  isPoint = (aX0 == aX1) && (aY0 == aY1);

  if ( isPoint ) {
    pGrid = getTGrid(aptGrid, aX0, aY0);
    if (pGrid != (eGrid_t*)NULL) return(pGrid);
  }

  pGrid          = (eGrid_t*)malloc(sizeof(eGrid_t));
  pGrid->x0      = aX0;
  pGrid->y0      = aY0;
  pGrid->x1      = aX1;
  pGrid->y1      = aY1;
  pGrid->isPoint = isPoint;

  pGrid->Next = *aptGrid;
  *aptGrid    = pGrid;

  return(pGrid);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "phseg2TGrid()".
 */

static void  phseg2TGrid(aptGrid, apPhSeg)
  eGrid_t    **aptGrid;
  phseg_list  *apPhSeg;
{
  long  y, yMin, yMax;


  if (apPhSeg->X1 == apPhSeg->X2) {
    if (!(LV_flags & F_ALLOW_OFFGRID)) {
      if ((apPhSeg->X1 - LV_phfig->XAB1) % XGRID) {
        wprinth((char*)NULL);
        wprintf("Segment \"%s\" (%d,%d) (%d,%d) is NOT on grid (skipped).\n",
                apPhSeg->NAME, apPhSeg->X1, apPhSeg->Y1,
                               apPhSeg->X2, apPhSeg->Y2);
        return;
      }
    }

    yMax = apPhSeg->Y2 - (apPhSeg->Y2 % YGRID);
    yMin = apPhSeg->Y1;
    if (apPhSeg->Y1 % YGRID) yMin += YGRID - (apPhSeg->Y1 % YGRID);

    if (LV_flags & F_NO_SPLIT_TERM) {
      addTGrid(aptGrid, apPhSeg->X1, yMin, apPhSeg->X1, yMax);
    } else {
      for(y = yMin; y <= yMax; y += YGRID)
        addTGrid(aptGrid, apPhSeg->X1, y, apPhSeg->X1, y);
    }
  } else {
    if (!(LV_flags & F_ALLOW_OFFGRID)) {
      if ((apPhSeg->Y1 - LV_phfig->YAB1) % YGRID) {
        wprinth((char*)NULL);
        wprintf("Segment \"%s\" (%d,%d) (%d,%d) is NOT on grid (skipped).\n",
                apPhSeg->NAME, apPhSeg->X1, apPhSeg->Y1,
                               apPhSeg->X2, apPhSeg->Y2);
        return;
      }
    }

    yMax = apPhSeg->X2 - (apPhSeg->X2 % XGRID);
    yMin = apPhSeg->X1;
    if (apPhSeg->X1 % XGRID) yMin += XGRID - (apPhSeg->X1 % XGRID);

    if (LV_flags & F_NO_SPLIT_TERM) {
      addTGrid(aptGrid, yMin, apPhSeg->Y1, yMax, apPhSeg->Y1);
    } else {
      for(y = yMin; y <= yMax; y += XGRID) {
        addTGrid(aptGrid, y, apPhSeg->Y1, y, apPhSeg->Y1);
      }
    }
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "TGrid2PORT()".
 */

static void  TGrid2PORT(aplPORT, aptGrid)
   ePORT_t **aplPORT;
   eGrid_t  *aptGrid;
{
  eGrid_t *pGrid;


  for(pGrid = aptGrid; pGrid != NULL; pGrid = pGrid->Next) {
    m_AddPort((*aplPORT), C_PORTITEM_RECT,
      m_AddRect(pGrid->x0 - WIDTH_CALUx / 2,
                pGrid->y0 - WIDTH_CALUx / 2,
                pGrid->x1 + WIDTH_CALUx / 2,
                pGrid->y1 + WIDTH_CALUx / 2));
  } /* End of "pGrid" loop. */
}


/*  ----------------------------------------------------------------------
 *  Function  :  "refCon2Name()".
 */

static char *refCon2Name(asRefCon)
  char  *asRefCon;
{
  char *c, *e, sIOName[1024];


  strcpy(sIOName, asRefCon);

  for(e = (char*)NULL, c = sIOName; *c != (char)0; c++)
    if (*c == '_') e = c;

  if (e != (char*)NULL) *e = (char)0;


  return(namealloc(sIOName));
}


/*  ----------------------------------------------------------------------
 *  Function  :  "onGrid()".
 */

static  int  onGrid(aX, aY)
  long aX, aY;
{
  if ((aX - LV_phfig->XAB1) % XGRID) return(FALSE);
  if ((aY - LV_phfig->YAB1) % YGRID) return(FALSE);

  return(TRUE);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "getLoCon()".
 */

static  locon_list *getLoCon(asName)
  char *asName;
{
  locon_list *pLoCon;


  for(pLoCon  = LV_lofig->LOCON;
      pLoCon != (locon_list*)NULL;
      pLoCon  = pLoCon->NEXT) {
    if (pLoCon->NAME == asName) return(pLoCon);
  }

  /* No matching locon found. */
  return((locon_list *)NULL);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "SxLib2SYMMETRY()".
 */

static  void  SxLib2SYMMETRY()
{
  /* Symmetry generation. */
  if (((LV_phfig->YAB2 - LV_phfig->YAB1) / YSLICE) % 2) {
    LV_pMACRO->SYMMETRY = F_SYMMETRY_XY;
  } else {
    LV_pMACRO->SYMMETRY = F_SYMMETRY_Y;
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "AddEmptyPowerPIN()".
 */

static void  AddEmptyPowerPIN(asName)
  char *asName;
{
  ePIN_t *pPIN;
  char    Use;


  Use = C_USE_NONE;
  if (!strncmp(asName, "vdd", 3)) Use = C_USE_POWER;
  if (!strncmp(asName, "vss", 3)) Use = C_USE_GROUND;


  if ((pPIN = getPIN(LV_pMACRO, asName)) == (ePIN_t*)NULL) {
    m_AddPin(LV_pMACRO->lPIN, asName);
    LV_pMACRO->lPIN->DIRECTION = C_DIRECTION_IN;
    LV_pMACRO->lPIN->USE       = Use;
    LV_pMACRO->lPIN->SHAPE     = C_SHAPE_FEEDTHRU;
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "AddPowerPIN()".
 */

static void  AddPowerPIN(asName, aDir, aLayer, aY, aWidth, aFlags)
  char *asName;
  char  aLayer, aDir;
  long  aY, aWidth, aFlags;
{
  ePATH_t *pPATH;
   ePIN_t *pPIN;
   char    Use, Shape;
   long    XO1, XO2, YO1, YO2;


  pPIN = NULL;  /* Shut up "gcc -w" ... */

  if (aFlags) {
    /* Either left or right terminal is asked or both, we need to create
     * a PIN if it's not already present.
     */
    Use = C_USE_NONE;
    if (!strncmp(asName, "vdd", 3)) Use = C_USE_POWER;
    if (!strncmp(asName, "vss", 3)) Use = C_USE_GROUND;

    Shape = C_SHAPE_FEEDTHRU;
    if (LV_flags & F_POWER_ABUTMENT)
      Shape = C_SHAPE_ABUTMENT;

    if ((pPIN = getPIN(LV_pMACRO, asName)) == (ePIN_t*)NULL) {
      m_AddPin(LV_pMACRO->lPIN, asName);
      LV_pMACRO->lPIN->DIRECTION = C_DIRECTION_IN;
      LV_pMACRO->lPIN->USE       = Use;
      LV_pMACRO->lPIN->SHAPE     = Shape;
      
      pPIN = LV_pMACRO->lPIN;
    }

    m_AddPort(pPIN->lPORT, C_PORTITEM_LAYER, (long)aLayer);
    m_AddPort(pPIN->lPORT, C_PORTITEM_WIDTH, aWidth);
  }

  if (aFlags && (LV_flags & F_POWER_ABUTMENT)) {
    pPATH = (ePATH_t *)NULL;

    switch(aDir) {
      default:
      case C_POWER_HORIZONTAL:
        m_AddPath(pPATH, LV_phfig->XAB1 + aWidth / 2, aY);
        m_AddPath(pPATH, LV_phfig->XAB2 - aWidth / 2, aY);
        break;
      case C_POWER_VERTICAL:
        m_AddPath(pPATH, aY, LV_phfig->YAB1 + aWidth / 2);
        m_AddPath(pPATH, aY, LV_phfig->YAB2 - aWidth / 2);
        break;
    }

    m_AddPort(pPIN->lPORT, C_PORTITEM_PATH, revPATH(pPATH));
  } else {
    switch(aDir) {
      default:
      case C_POWER_HORIZONTAL:
        if (aFlags & F_POWER_PIN_LEFT) {
          pPATH = (ePATH_t *)NULL;

          m_AddPath(pPATH, LV_phfig->XAB1               + aWidth / 2, aY);
          m_AddPath(pPATH, LV_phfig->XAB1 + MBKSCALE(4) + aWidth / 2, aY);
          
          m_AddPort(pPIN->lPORT, C_PORTITEM_PATH, revPATH(pPATH));
        }

        if (aFlags & F_POWER_PIN_RIGHT) {
          pPATH = (ePATH_t *)NULL;

          m_AddPath(pPATH, LV_phfig->XAB2 - MBKSCALE(4) - aWidth / 2, aY);
          m_AddPath(pPATH, LV_phfig->XAB2               - aWidth / 2, aY);

          m_AddPort(pPIN->lPORT, C_PORTITEM_PATH, revPATH(pPATH));
        }

        if (aLayer == ALU1) {
          XO1 = LV_phfig->XAB1 + ((aFlags & F_POWER_PIN_LEFT)
                                  ? MBKSCALE(13)
                                  : MBK_ALU1_SPACING / 2);
          XO2 = LV_phfig->XAB2 - ((aFlags & F_POWER_PIN_RIGHT)
                                  ? MBKSCALE(13) 
                                  : MBK_ALU1_SPACING / 2);
          YO1 = aY - aWidth / 2;
          YO2 = aY + aWidth / 2;

          m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_RECT,
                   m_AddRect(XO1, YO1, XO2, YO2));
        }
        break;
      case C_POWER_VERTICAL:
        pPATH = (ePATH_t *)NULL;

        m_AddPath(pPATH, aY, LV_phfig->YAB1               + aWidth / 2);
        m_AddPath(pPATH, aY, LV_phfig->YAB1 + MBKSCALE(4) + aWidth / 2);

        m_AddPort(pPIN->lPORT, C_PORTITEM_PATH, revPATH(pPATH));
        pPATH = (ePATH_t *)NULL;

        m_AddPath(pPATH, aY, LV_phfig->YAB2 - MBKSCALE(4) - aWidth / 2);
        m_AddPath(pPATH, aY, LV_phfig->YAB2               - aWidth / 2);

        m_AddPort(pPIN->lPORT, C_PORTITEM_PATH, revPATH(pPATH));
        break;
    }
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "SxLib2PowerPINS()".
 */

static void  SxLib2PowerPINS(aLayer, aDir, asSuffix)
  char  aLayer;
  char  aDir;
  char *asSuffix;
{
  ePow_t  *pPow, *plPow;
    char   sTmp[32], *name;
    long   XO1, XO2, YO1, YO2, width_top, width_bottom, flags;


  plPow = buildPow (LV_phfig, aLayer, aDir, asSuffix);


  if (plPow == (ePow_t*)NULL) {
    if (aLayer == ALU1) {
      wprinth((char*)NULL);
      wprints("\n  Can't find any ALU1 power terminals! If this is not a bug");
      wprints(" please consider\n  usage of \"-p\" flag.\n");
    }

    return;
  }


  if (aLayer == ALU1) {
    if (plPow->Type != C_POWER_ISVSS) {
      eprinth (NULL);
      eprintf ("\n  Bottom power supply is not VSS.\n");
      EXIT (1);
    }
  }


  LV_flags |=  F_POWER_ABUTMENT;
  LV_flags &= ~F_POWER_FEEDTHRU;

  for (pPow = plPow; pPow != NULL; pPow = pPow->Next) {
    if (!(pPow->flags & F_POWER_COMPLETE)) {
      LV_flags &= ~F_POWER_ABUTMENT;
      LV_flags |=  F_POWER_FEEDTHRU;
      break;
    }
  }


  if (aLayer == ALU1)
    m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_LAYER, ALU1);


  /* Power pin generation. */
  for (pPow = plPow; pPow != NULL; pPow = pPow->Next) {
    switch (pPow->Type) {
      case C_POWER_ISVDD:
        /*
         * Doesn't exist under SunOS 4.1.4.
         *
         * snprintf(sTmp, 31, "vdd%s", asSuffix);
         */
        sprintf (sTmp, "vdd%s", asSuffix);
        name         = namealloc (sTmp);
        width_bottom = MBK_WIDTH_VDD;
        width_top    = MBK_WIDTH_VSS;

        break;
      default:
      case C_POWER_ISVSS:
        /* snprintf(sTmp, 31, "vss%s", asSuffix);
         */
        sprintf (sTmp, "vss%s", asSuffix);
        name         = namealloc (sTmp);
        width_bottom = MBK_WIDTH_VSS;
        width_top    = MBK_WIDTH_VDD;
        break;
    }

    flags = F_POWER_PIN_LEFT | F_POWER_PIN_RIGHT;

    if (aLayer == ALU1) {
      if (pPow->min != LV_phfig->XAB1) flags &= ~F_POWER_PIN_LEFT;
      if (pPow->max != LV_phfig->XAB2) flags &= ~F_POWER_PIN_RIGHT;
      printf ("max %ld XAB2 %ld\n", pPow->max, LV_phfig->XAB2);
    }
    AddPowerPIN (name, aDir, aLayer, pPow->y, pPow->w, flags);

    /* In case of ALU1 layer add one big obstacle. */
    if ((aLayer == ALU1) && (pPow->Next != NULL)) {
      if (pPow->Next->y - pPow->y >
            MBK_ALU1_SPACING + (width_bottom + width_top) / 2) {
        XO1 = LV_phfig->XAB1 + MBK_ALU1_SPACING / 2;
        XO2 = LV_phfig->XAB2 - MBK_ALU1_SPACING / 2;
        YO1 = pPow->y        + MBK_ALU1_SPACING + width_bottom / 2;
        YO2 = pPow->Next->y  - MBK_ALU1_SPACING - width_top    / 2;

        m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_RECT,
                 m_AddRect(XO1, YO1, XO2, YO2));
      }
    }
  } /* End of "pPow" loop. */


  freePow (plPow);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "phref2PINS()".
 */

static void  phref2PINS()
{
  phref_list *pPhRef;
  locon_list *pLoCon;
      ePIN_t *pPIN;
        char *sIOName;


  for(pPhRef  = LV_phfig->PHREF;
      pPhRef != (phref_list*)NULL;
      pPhRef  = pPhRef->NEXT) {
    if (pPhRef->FIGNAME == LV_ref_con) {

      /* Get the IO name from the "ref_con" name. */
      sIOName = refCon2Name(pPhRef->NAME);

      /* Find the IO in the behavioral view. */
      if ((pLoCon = getLoCon(sIOName)) == (locon_list*)NULL) {
        wprinth((char*)NULL);
        wprintf(
          "\n  Physical terminal \"%s\" doesn't match any ", pPhRef->NAME);
        wprints("logical terminal.\n");

        continue;
      }

      /* Skip connectors not on routing grid. */
      if (!onGrid(pPhRef->XREF, pPhRef->YREF)) {
        wprinth((char*)NULL);
        wprintf("Terminal \"%s\" is NOT on routing grid, skipped.\n",
                pPhRef->NAME);

        continue;
      }

      /* Add to the list of holes in the obstacle grid. */
      addTGrid(&LV_tHolesALU1, pPhRef->XREF, pPhRef->YREF,
                               pPhRef->XREF, pPhRef->YREF);

      if ((pPIN = getPIN(LV_pMACRO, sIOName)) == (ePIN_t*)NULL) {

        /* Create a new pin named from "sIOName". */
        m_AddPin(LV_pMACRO->lPIN, sIOName);
        LV_pMACRO->lPIN->DIRECTION = MBK2DEF_locondir(pLoCon);
	if (isck(sIOName))
	{
	    LV_pMACRO->lPIN->USE = C_USE_GROUND;
	}
        m_AddPort(LV_pMACRO->lPIN->lPORT, C_PORTITEM_LAYER, ALU1);

        pPIN = LV_pMACRO->lPIN;
      }

      m_AddPort(pPIN->lPORT, C_PORTITEM_RECT,
        m_AddRect(pPhRef->XREF - WIDTH_CALUx / 2,
                  pPhRef->YREF - WIDTH_CALUx / 2,
                  pPhRef->XREF + WIDTH_CALUx / 2,
                  pPhRef->YREF + WIDTH_CALUx / 2));
    }
  } /* End of "pPhRef" loop. */
}


/*  ----------------------------------------------------------------------
 *  Function  :  "phseg2PINS()".
 */

static void  phseg2PINS()
{
  phseg_list *pPhSeg;
  locon_list *pLoCon;
      ePIN_t *pPIN;
     eGrid_t *tCALU;


  for(pPhSeg  = LV_phfig->PHSEG;
      pPhSeg != (phseg_list*)NULL;
      pPhSeg  = pPhSeg->NEXT) {
    if (isvdd(pPhSeg->NAME)) continue;
    if (isvss(pPhSeg->NAME)) continue;

    switch(pPhSeg->LAYER) {
      case CALU1:
      case CALU2:
      case CALU3:
      case CALU4:
      case CALU5:
      case CALU6:
        /* Find the IO in the behavioral view. */
        if ((pLoCon = getLoCon(pPhSeg->NAME)) == (locon_list*)NULL) {
          wprinth((char*)NULL);
          wprintf(
            "\n  Physical terminal \"%s\" doesn't match any ", pPhSeg->NAME);
          wprints("logical terminal.\n");

          continue;
        }

        tCALU = (eGrid_t*)NULL;
        phseg2TGrid(&tCALU, pPhSeg);
        
        if ((pPIN = getPIN(LV_pMACRO, pPhSeg->NAME)) == (ePIN_t*)NULL) {
          /* Create a new pin named from "pPhSeg->NAME". */
          m_AddPin(LV_pMACRO->lPIN, pPhSeg->NAME);
          LV_pMACRO->lPIN->DIRECTION = MBK2DEF_locondir(pLoCon);
          if (isck(pPhSeg->NAME))
            LV_pMACRO->lPIN->USE = C_USE_CLOCK;
          
          pPIN = LV_pMACRO->lPIN;
        }

        m_AddPort(pPIN->lPORT, C_PORTITEM_LAYER, (long)pPhSeg->LAYER);
        TGrid2PORT(&(pPIN->lPORT), tCALU);

        break;
    } /* End of "pPhSeg->LAYER" switch. */
  } /* End of "pPhSeg" loop. */
}


/*  ----------------------------------------------------------------------
 *  Function  :  "SxLib2SignalPINS()".
 */

static void  SxLib2SignalPINS()
{
  phref2PINS();
  phseg2PINS();
}


/*  ----------------------------------------------------------------------
 *  Function  :  "insertPIN()".
 */

static void  insertPIN(appHead, appTail, apPIN)
  ePIN_t **appHead, **appTail;
  ePIN_t  *apPIN;
{
  if (*appHead == (ePIN_t*)NULL) {
    *appHead = apPIN;
  } else {
    (*appTail)->Next = apPIN;
  }

   *appTail        = apPIN;
  (*appTail)->Next = (ePIN_t*)NULL;
}


/*  ----------------------------------------------------------------------
 *  Function  :  "sortPINS()".
 */

static void  sortPINS()
{
  ePIN_t *pPIN, *pTmp, **ppHead;
  ePIN_t *pHeadINOUT , *pTailINOUT;
  ePIN_t *pHeadOUTPUT, *pTailOUTPUT;
  ePIN_t *pHeadINPUT , *pTailINPUT;
  ePIN_t *pHeadPOWER , *pTailPOWER;
  ePIN_t *pHeadCLOCK , *pTailCLOCK;


  pHeadINOUT  = pTailINOUT  = (ePIN_t*)NULL;
  pHeadOUTPUT = pTailOUTPUT = (ePIN_t*)NULL;
  pHeadINPUT  = pTailINPUT  = (ePIN_t*)NULL;
  pHeadPOWER  = pTailPOWER  = (ePIN_t*)NULL;
  pHeadCLOCK  = pTailCLOCK  = (ePIN_t*)NULL;

  for(pPIN = LV_pMACRO->lPIN; pPIN != (ePIN_t*)NULL;) {
    pTmp = pPIN;
    pPIN = pPIN->Next;

    switch(pTmp->USE) {
      case C_USE_POWER:
      case C_USE_GROUND:
        insertPIN(&pHeadPOWER, &pTailPOWER, pTmp);
        continue;
      case C_USE_CLOCK:
	insertPIN(&pHeadCLOCK, &pTailCLOCK, pTmp);
	continue;
    } /* End of "USE" switch. */

    switch(pTmp->DIRECTION) {
      case C_DIRECTION_INPUT:
        insertPIN(&pHeadINPUT, &pTailINPUT, pTmp);
        continue;
      case C_DIRECTION_OUTPUT:
      case C_DIRECTION_TRISTATE:
        insertPIN(&pHeadOUTPUT, &pTailOUTPUT, pTmp);
        continue;
      case C_DIRECTION_INOUT:
      case C_DIRECTION_FEEDTHRU:
        insertPIN(&pHeadINOUT, &pTailINOUT, pTmp);
        continue;
    } /* End of "DIRECTION" switch. */

    eprinth((char*)NULL);
    eprintf("\n  Pin \"%s\" have no DIRECTION!?\n", pTmp->pinName);
    EXIT(1);
  } /* End of "pPIN" loop. */

  /* Rebuild the complete list (in reverse order). */
  ppHead = &(LV_pMACRO->lPIN);

  if (pHeadINOUT != (ePIN_t*)NULL) {
    *ppHead = pHeadINOUT;   ppHead = &(pTailINOUT->Next);
  }

  if (pHeadOUTPUT != (ePIN_t*)NULL) {
    *ppHead = pHeadOUTPUT; ppHead = &(pTailOUTPUT->Next);
  }

  if (pHeadINPUT != (ePIN_t*)NULL) {
    *ppHead = pHeadINPUT;  ppHead = &(pTailINPUT->Next);
  }

  if (pHeadPOWER != (ePIN_t*)NULL) {
    *ppHead = pHeadPOWER;  ppHead = &(pTailPOWER->Next);
  }
  
  if (pHeadCLOCK != (ePIN_t*)NULL) {
    *ppHead = pHeadCLOCK;  ppHead = &(pTailCLOCK->Next);
  }

  *ppHead = (ePIN_t*)NULL;
}


/*  ----------------------------------------------------------------------
 *  Function  :  "revPINPORT()".
 */

static void  revPINPORT()
{
  ePIN_t *pPIN;

  for(pPIN = LV_pMACRO->lPIN; pPIN != (ePIN_t*)NULL; pPIN = pPIN->Next) {
    m_RevPort(pPIN->lPORT);
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "checkPIN()".
 */

static void  checkPIN()
{
   ePIN_t    *pPIN;
  ePORT_t    *pPORT;
  locon_list *pLoCon;
   long       fError, fPORT;


  fError = FALSE;
  for(pLoCon  = LV_lofig->LOCON;
      pLoCon != (locon_list*)NULL;
      pLoCon  = pLoCon->NEXT) {
    if (isvdd(pLoCon->NAME)) continue;
    if (isvss(pLoCon->NAME)) continue;

    fPORT = FALSE;

    pPIN = getPIN(LV_pMACRO, pLoCon->NAME);
    if (pPIN == (ePIN_t*)NULL) {
      fError = TRUE;

      eprinth((char*)NULL);
      eprintf("\n  Logical terminal \"%s\" ", pLoCon->NAME);
      eprints("have no physical terminal.\n\n");
    } else {
      for(pPORT = pPIN->lPORT; pPORT != NULL; pPORT = pPORT->Next) {
        switch(pPORT->Type) {
          case C_PORTITEM_RECT:
          case C_PORTITEM_PATH: fPORT = TRUE;
        }
      }
    }

    if (fPORT || fError) continue;

    eprinth((char*)NULL);
    eprintf("\n  Terminal \"%s\" ", pLoCon->NAME);

    if (LV_flags & F_ALLOW_OFFGRID) {
      eprints("have no terminals either on grid or offgrid.");
      eprints("\n  This must be a program bug.\n\n");
    } else
      eprints("have no terminals on grid.\n\n");
    fError = TRUE;
  }

  if (fError) EXIT(1);
}


# if 0
/*  ----------------------------------------------------------------------
 *  Function  :  "SxLib2ALU1OBS()".
 */

static void  SxLib2ALU1OBS()
{
  long  XO1, YO1, XO2, YO2;
  long  iX, iY, iTrack;


  if (LV_phfig->XAB2 - LV_phfig->XAB1 < 2 * MBK_ALU1_SPACING)
    return;

  XO1 = LV_phfig->XAB1 + MBK_ALU1_SPACING;
  XO2 = LV_phfig->XAB2 - MBK_ALU1_SPACING;
  YO1 = LV_phfig->YAB1 + MBK_ALU1_SPACING + MBK_WIDTH_VSS;
  YO2 = LV_phfig->YAB2 - MBK_ALU1_SPACING - MBK_WIDTH_VDD;

  m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_LAYER, ALU1);
  m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_RECT,
           m_AddRect(XO1, YO1, XO2, YO2));


  /* We replace all the "dot" obstacles by one big. */
  for(iY = LV_phfig->YAB1 + (2 * YGRID);
      iY < LV_phfig->YAB2 -      YGRID ; iY += YGRID) {

    /* Do not put obstacles under the power supplies. */
    iTrack = ((iY - LV_phfig->YAB1) % YSLICE) / YGRID;
    if ((iTrack < 2) || (iTrack > 8)) continue;

    for(iX = LV_phfig->XAB1 + XGRID;
        iX < LV_phfig->XAB2; iX += XGRID) {
      /*if (getTGrid(&LV_tHolesALU1, iX, iY) != (eGrid_t*)NULL) continue;*/

      XO1 = iX -  WIDTH_OBS_ALU1 / 2;
      YO1 = iY - HEIGHT_OBS_ALU1 / 2;
      XO2 = iX +  WIDTH_OBS_ALU1 / 2;
      YO2 = iY + HEIGHT_OBS_ALU1 / 2;

      if (iX == LV_phfig->XAB1 + XGRID) XO1 -= LEFTRIGHT_OBS_ALU1;
      if (iX == LV_phfig->XAB2 - XGRID) XO2 += LEFTRIGHT_OBS_ALU1;

      m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_RECT,
        m_AddRect(XO1, YO1, XO2, YO2));
    } /* End of "iX" loop. */
  } /* End of "iY" loop. */
}
# endif


/*  ----------------------------------------------------------------------
 *  Function  :  "SxLib2ALUxOBS()".
 */

static void  SxLib2ALUxOBS(aLayer)
  char  aLayer;
{
  phseg_list *pPhSeg;
        long  fComp, fFirst, X1, Y1, X2, Y2, Ext;


  Ext = getLayerExt(aLayer);

  fFirst = TRUE;


  for(pPhSeg = LV_phfig->PHSEG; pPhSeg != NULL; pPhSeg = pPhSeg->NEXT) {
    fComp = cmpALU(aLayer, pPhSeg->LAYER);

    if (!(fComp & F_EQUAL)) continue;
    if (  fComp & F_CALU  ) continue;

    switch(pPhSeg->TYPE) {
      case LEFT:
      case RIGHT:
      default:
        X1 = pPhSeg->X1 - Ext;
        X2 = pPhSeg->X2 + Ext;
        Y1 = pPhSeg->Y1 - pPhSeg->WIDTH / 2;
        Y2 = pPhSeg->Y2 + pPhSeg->WIDTH / 2;
        break;
      case UP:
      case DOWN:
        X1 = pPhSeg->X1 - pPhSeg->WIDTH / 2;
        X2 = pPhSeg->X2 + pPhSeg->WIDTH / 2;
        Y1 = pPhSeg->Y1 - Ext;
        Y2 = pPhSeg->Y2 + Ext;
        break;
    } /* End of "pPhSeg->TYPE" switch. */

    if (fFirst) {
      fFirst = FALSE;
      m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_LAYER, (long)aLayer);
    }

    m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_RECT, m_AddRect(X1, Y1, X2, Y2));
  } /* End of "pPhSeg" loop. */
}


/*  ----------------------------------------------------------------------
 *  Function  :  "SxLib2MACRO()".
 */

static eMACRO_t *SxLib2MACRO()
{
  /* Shift the cell so that the AB is at (0,0). */
  shiftphfig (LV_phfig, -LV_phfig->XAB1, -LV_phfig->YAB1);

  LV_pMACRO = allocMACRO(LV_phfig->NAME);

  if (   strncmp(LV_phfig->NAME, "tie_",    4)
      && strncmp(LV_phfig->NAME, "rowend_", 4))
    LV_pMACRO->CLASS    = C_CLASS_CORE;
  else
    LV_pMACRO->CLASS    = C_CLASS_FEEDTHRU;

  LV_pMACRO->ORIGIN.x = 0;
  LV_pMACRO->ORIGIN.y = 0;
  /*
   * LV_pMACRO->ORIGIN.x = - MBK2DEF_length(LV_phfig->XAB1);
   * LV_pMACRO->ORIGIN.y = - MBK2DEF_length(LV_phfig->YAB1);
   */
  LV_pMACRO->SIZE.x   =   MBK2DEF_length(LV_phfig->XAB2 - LV_phfig->XAB1);
  LV_pMACRO->SIZE.y   =   MBK2DEF_length(LV_phfig->YAB2 - LV_phfig->YAB1);
  LV_pMACRO->SITE     = "core";

  SxLib2SYMMETRY();

  if (LV_flags & F_NO_POWER_GEOM) {
    long  XO1, XO2, YO1, YO2;

    AddEmptyPowerPIN("vdd");
    AddEmptyPowerPIN("vss");

    m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_LAYER, ALU1);

    XO1 = LV_phfig->XAB1 + MBK_ALU1_SPACING / 2;
    XO2 = LV_phfig->XAB2 - MBK_ALU1_SPACING / 2;
    YO1 = LV_phfig->YAB1 + MBK_ALU1_SPACING / 2;
    YO2 = LV_phfig->YAB2 - MBK_ALU1_SPACING / 2;
    
    m_AddObs(LV_pMACRO->lOBS, C_OBSITEM_RECT,
             m_AddRect(XO1, YO1, XO2, YO2));
  } else {
    SxLib2PowerPINS(ALU1, C_POWER_HORIZONTAL, "");
    SxLib2PowerPINS(ALU3, C_POWER_VERTICAL  , "");
  }
  SxLib2SignalPINS();
  sortPINS();
  revPINPORT();
  checkPIN();

  /* SxLib2ALU1OBS(); */
  SxLib2ALUxOBS(ALU2);
  SxLib2ALUxOBS(ALU3);
  SxLib2ALUxOBS(ALU4);
  SxLib2ALUxOBS(ALU5);
  SxLib2ALUxOBS(ALU6);
  m_RevObs(LV_pMACRO->lOBS);


  return(LV_pMACRO);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "lefsavesxlophig()".
 */

extern void  lefsavesxlophfig(apLofig, apPhfig, aFlags)
  struct lofig *apLofig;
  struct phfig *apPhfig;
          long  aFlags;
{
  FILE   *LEF_FILE;
  char    LEF_name[1024];


  LV_ref_ref = namealloc ("ref_ref");
  LV_ref_con = namealloc ("ref_con");

  LV_lofig = apLofig;
  LV_phfig = apPhfig;
  LV_flags = aFlags;

  (void)SxLib2MACRO ();


  sprintf (LEF_name, "%s.lef", apLofig->NAME);
  LEF_FILE = fopen (LEF_name, "w+");

  fprintMACRO (LEF_FILE, LV_pMACRO);

  fclose (LEF_FILE);
}
