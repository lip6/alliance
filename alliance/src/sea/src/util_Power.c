/*
 *  $Id: util_Power.c,v 1.2 2002/09/30 16:21:19 czo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./util_Power.c"                          |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "debugoff.h"


/*  ------------------------------------------------------------------
 *  Local variables (prefix 'LV_').
 */

  static long  LV_XAB1;
  static long  LV_YAB1;
  static long  LV_XAB2;
  static long  LV_YAB2;


/*  ----------------------------------------------------------------------
 *  Local functions declarations.
 */

  static  int     onSlice  __FP((long aY));
  static void  setPowerAB  __FP((long XAB1, long YAB1,
                                 long XAB2, long YAB2));


/*
 *  /--------------------------------------------------------------------\
 *  |                     Functions Definitions                          |
 *  \--------------------------------------------------------------------/
 */


/*  ----------------------------------------------------------------------
 *  Function  :  "setPowerAB()".
 */

static void  setPowerAB(XAB1, YAB1, XAB2, YAB2)
  long  XAB1, YAB1, XAB2, YAB2;
{
  LV_XAB1 = XAB1;
  LV_YAB1 = YAB1;
  LV_XAB2 = XAB2;
  LV_YAB2 = YAB2;
}



/*  ----------------------------------------------------------------------
 *  Function  :  "onSlice()".
 */

static  int  onSlice(aY)
  long aY;
{
  if (aY < LV_YAB1) return (FALSE);
  if (aY > LV_YAB2) return (FALSE);

  if (!(((aY - MBK_WIDTH_VDD / 2) - LV_YAB1) % MBK_Y_SLICE)) return (TRUE);
  if (!(((aY + MBK_WIDTH_VDD / 2) - LV_YAB1) % MBK_Y_SLICE)) return (TRUE);

  if (!(((aY - MBK_WIDTH_VSS / 2) - LV_YAB1) % MBK_Y_SLICE)) return (TRUE);
  if (!(((aY + MBK_WIDTH_VSS / 2) - LV_YAB1) % MBK_Y_SLICE)) return (TRUE);

  return (FALSE);
}

/*  ----------------------------------------------------------------------
 *  Function  :  "addPow()".
 */

extern ePow_t *addPow(applPow, aY, aW, aName, aType, aDir)
  ePow_t **applPow;
    long   aY;
    long   aW;
    char  *aName;
    char   aType;
    char   aDir;
{
  ePow_t *pPow, **ppPow;


  pPow        = (ePow_t*)mbkalloc(sizeof(ePow_t));
  pPow->y     = aY;
  pPow->w     = aW;
  pPow->flags = 0;
  pPow->inter = NULL;

  switch(aDir) {
    case C_POWER_VERTICAL:
      pPow->xMin = LV_YAB1;
      pPow->xMax = LV_YAB2;
      pPow->min  = LV_YAB2;
      pPow->max  = LV_YAB1;
      break;
    default:
    case C_POWER_HORIZONTAL:
      pPow->xMin = LV_XAB1;
      pPow->xMax = LV_XAB2;
      pPow->min  = LV_XAB2;
      pPow->max  = LV_XAB1;
      break;
  }

  pPow->Name = namealloc(aName);
  pPow->Type = aType;


  /* Insert in the ordered list. */
  for(ppPow = applPow ; *ppPow != NULL ; ppPow = &((*ppPow)->Next)) {
    if ((*ppPow)->y >= pPow->y) {
      pPow->Next = *ppPow;
      *ppPow = pPow;
      break;
    }
  }


  /* The list is empty or the element is the last one. */
  if (*ppPow == (ePow_t*)NULL) {
    pPow->Next = *ppPow;
    *ppPow = pPow;
  }


  return(pPow);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "freePow()".
 */

extern void  freePow(aplPow)
  struct ePow_s *aplPow;
{
  struct ePow_s *pDel, *pNext;


  for (pDel = aplPow; pDel != NULL; ) {
    pNext = pDel->Next;

    freeinter (pDel->inter);
    mbkfree  (pDel);

    pDel = pNext;
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "getPow()".
 */

extern ePow_t *getPow(aplPow, aY)
  ePow_t *aplPow;
    long  aY;
{
  ePow_t *pPow;


  for( pPow = aplPow ; pPow != NULL ; pPow = pPow->Next )
    if (pPow->y == aY) return( pPow );

  return( NULL );
}


/*  ----------------------------------------------------------------------
 *  Function  :  "mergeXPow()".
 */

extern void  mergeXPow(apPow, axMin, axMax, aW)
  ePow_t *apPow;
    long  axMin, axMax, aW;
{
  if (aW == apPow->w)
    apPow->inter = addinter (apPow->inter, axMin, axMax);

  apPow->min = m_Min(apPow->min, axMin);
  apPow->max = m_Max(apPow->max, axMax);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "buildPow()".
 */

extern ePow_t *buildPow(apPhfig, aLayer, aDir, asSuffix)
  struct phfig *apPhfig;
          char  aLayer;
          char  aDir;
          char *asSuffix;
{
      ePow_t  **pplPow, *plPow, *pPow;
  phseg_list   *pPhseg;
  phcon_list   *pPhcon;
        char    type;
        char    ORIENT1, ORIENT2, *sDir1, *sDir2;
        long    X1, X2, Y1, Y2;


  setPowerAB (apPhfig->XAB1, apPhfig->YAB1,
              apPhfig->XAB2, apPhfig->YAB2);


  switch(aDir) {
    case C_POWER_VERTICAL:
      sDir1   = "vertical";
      sDir2   = "NORTH/SOUTH";
      ORIENT1 = NORTH;
      ORIENT2 = SOUTH;
      break;
    default:
    case C_POWER_HORIZONTAL:
      sDir1   = "horizontal";
      sDir2   = "EAST/WEST";
      ORIENT1 = EAST;
      ORIENT2 = WEST;
      break;
  }


   pplPow = &plPow;
  *pplPow =  NULL;


  /* Find power terminals among CALUx segments (new convention). */
  for(pPhseg = apPhfig->PHSEG ; pPhseg != NULL ; pPhseg = pPhseg->NEXT) {
    type = C_POWER_ISNONE;

    if (isvdd(pPhseg->NAME)) type = C_POWER_ISVDD;
    if (isvss(pPhseg->NAME)) type = C_POWER_ISVSS;

    switch(aDir) {
      case C_POWER_VERTICAL:
        Y1 = pPhseg->X1;
        Y2 = pPhseg->X2;
        X1 = pPhseg->Y1;
        X2 = pPhseg->Y2;
        break;
      default:
      case C_POWER_HORIZONTAL:
        Y1 = pPhseg->Y1;
        Y2 = pPhseg->Y2;
        X1 = pPhseg->X1;
        X2 = pPhseg->X2;
        break;
    }

    if (type != C_POWER_ISNONE) {
      if (cmpALU(aLayer, pPhseg->LAYER) & F_CALU) {
        if (Y1 != Y2) {
          wprinth(NULL);
          wprintf("\n  In figure \"%s\" :", apPhfig->NAME);
          wprintf("\n  CALU1 Power segment \"%s\" at (%d,%d)",
                  pPhseg->NAME,
                  MBKUNSCALE (pPhseg->X1),
                  MBKUNSCALE (pPhseg->Y1));
          wprintf(" is not %s.\n", sDir1);
          continue;
        }

        if (aDir == C_POWER_HORIZONTAL) {
          if (!onSlice(Y1)) {
            wprinth(NULL);
            wprintf("\n  In figure \"%s\" :", apPhfig->NAME);
            wprintf("\n  CALU1 Power segment \"%s\" at (%d,%d)",
                    pPhseg->NAME,
                    MBKUNSCALE (pPhseg->X1),
                    MBKUNSCALE (pPhseg->Y1));
            wprintf(" is not on a slice boundary.");
            wprintf("\n  (valide slices boundaries are");
            wprintf(" ((n * %ld) - %ld) or"  , Y_SLICE, WIDTH_VSS / 2);
            wprintf(" ((n * %ld) + %ld) )\n" , Y_SLICE, WIDTH_VSS / 2);
            continue;
        }
        }

        if ((pPow = getPow(plPow, Y1)) == NULL) {
          pPow = addPow(pplPow       ,
                        Y1           ,
                        pPhseg->WIDTH,
                        pPhseg->NAME ,
                        type         ,
                        aDir         );
        }

        if (pPhseg->NAME != pPow->Name) {
          eprinth(NULL);
          eprintf("\n  In figure \"%s\" :", apPhfig->NAME);
          eprintf("\n  CALU1 Power segment \"%s\" at (%d,%d)\n",
                  pPhseg->NAME,
                  MBKUNSCALE (pPhseg->X1),
                  MBKUNSCALE (pPhseg->Y1));
          eprintf("  conflict with \"%s\" power line at %d.\n",
                  pPow->Name,
                  pPow->y   );
          EXIT(1);
        }

        mergeXPow(pPow, X1, X2, pPhseg->WIDTH);
      }
    }
  } /* End of "pPhseg" loop. */


  /* Find power terminals among terminals (for backward compatibility). */
  for(pPhcon = apPhfig->PHCON ; pPhcon != NULL ; pPhcon = pPhcon->NEXT) {
    type = C_POWER_ISNONE;

    if (isvdd(pPhcon->NAME)) type = C_POWER_ISVDD;
    if (isvss(pPhcon->NAME)) type = C_POWER_ISVSS;

    switch(aDir) {
      case C_POWER_VERTICAL:
        Y1 = pPhcon->XCON;
        X1 = pPhcon->YCON;
        break;
      default:
      case C_POWER_HORIZONTAL:
        Y1 = pPhcon->YCON;
        X1 = pPhcon->XCON;
        break;
    }

    if (type != C_POWER_ISNONE) {
      if (cmpALU(aLayer, pPhcon->LAYER)) {
        if (   (pPhcon->ORIENT != ORIENT1)
            && (pPhcon->ORIENT != ORIENT2)) {
          eprinth(NULL);
          eprintf("\n  In figure \"%s\" :", apPhfig->NAME);
          eprintf("\n  ALU1 Power terminal \"%s\" at (%d,%d)",
                  pPhcon->NAME,
                  MBKUNSCALE (pPhcon->XCON),
                  MBKUNSCALE (pPhcon->YCON));
          eprintf(" is not on %s side.\n", sDir2);
          EXIT(1);
        }

        if (aDir == C_POWER_HORIZONTAL) {
          if (!onSlice(pPhcon->YCON)) {
            eprinth(NULL);
            eprintf("\n  In figure \"%s\" :", apPhfig->NAME);
            eprintf("\n  ALU1 Power terminal \"%s\" at (%d,%d)",
                    pPhcon->NAME,
                    MBKUNSCALE (pPhcon->XCON),
                    MBKUNSCALE (pPhcon->YCON));
            eprintf(" is not on a slice boundary.");
            eprintf("\n  (valide slices boundaries are");
            eprintf(" ((n * %ld) - %ld) or"  , Y_SLICE, WIDTH_VSS / 2);
            eprintf(" ((n * %ld) + %ld) )\n" , Y_SLICE, WIDTH_VSS / 2);
            EXIT(1);
          }
        }

        if ((pPow = getPow(plPow, Y1)) == NULL) {
          pPow = addPow(pplPow       ,
                        Y1           ,
                        pPhcon->WIDTH,
                        pPhcon->NAME ,
                        type         ,
                        aDir         );
        }

        if (pPhcon->NAME != pPow->Name) {
          eprinth(NULL);
          eprintf("\n  In figure \"%s\" :", apPhfig->NAME);
          eprintf("\n  ALU1 Power terminal \"%s\" at (%d,%d)\n",
                  pPhcon->NAME,
                  MBKUNSCALE (pPhcon->XCON),
                  MBKUNSCALE (pPhcon->YCON));
          eprintf("  conflict with \"%s\" power line at %d.\n",
                  pPow->Name,
                  pPow->y   );
          EXIT(1);
        }

        mergeXPow(pPow, X1, X1, pPhcon->WIDTH);
      }
    }
  } /* End of "pPhcon" loop. */


  /* Look if the power lines are complete. */
  for (pPow = plPow; pPow != NULL; pPow = pPow->Next) {
    if (   (pPow->inter->len == 1)
        && (pPow->inter->min == pPow->xMin)
        && (pPow->inter->max == pPow->xMax)) {
      pPow->flags |= F_POWER_COMPLETE;
      __DBG( printf ("powerline at %ld is complete.\n", pPow->y); )
    }
  }


  return (plPow);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "powToCon()".
 */

extern void  powToCon(apPhfig, aLayer, aDir, aplPow, aFlags)
   struct phfig *apPhfig;
           char  aLayer;
           char  aDir;
  struct ePow_s *aplPow;
           long  aFlags;
{
  struct ePow_s *pPow;
           char *conName;
           long  w, y, skipFlag;


  for (pPow = aplPow; pPow != NULL; pPow = pPow->Next) {
    skipFlag = FALSE;

    switch (pPow->Type) {
      default:
      case C_POWER_ISVSS: conName = "vss"; break;
      case C_POWER_ISVDD: conName = "vdd"; break;
    }

    w = pPow->w;
    y = pPow->y;

    if (aFlags & F_POWER_MERGE) {
      if (   (pPow->Next != NULL)
             && (pPow->Next->y == pPow->y + MBKSCALE(6))) {
        skipFlag = TRUE;
        
        w  = MBKSCALE(12);
        y += MBKSCALE(3);
      }
    }

    if (aDir == C_POWER_HORIZONTAL) {
      if (pPow->flags & F_POWER_COMPLETE) {
        addphcon (apPhfig,
                  WEST,
                  conName,
                  pPow->xMin,
                  y,
                  aLayer,
                  w);
      }

      if (pPow->flags & F_POWER_COMPLETE) {
        addphcon (apPhfig,
                  EAST,
                  conName,
                  pPow->xMax,
                  y,
                  aLayer,
                  w);
      }
    } else {
      if (pPow->flags & F_POWER_COMPLETE) {
        addphcon (apPhfig,
                  SOUTH,
                  conName,
                  y,
                  pPow->xMin,
                  aLayer,
                  w);
      }

      if (pPow->flags & F_POWER_COMPLETE) {
        addphcon (apPhfig,
                  NORTH,
                  conName,
                  y,
                  pPow->xMax,
                  aLayer,
                  w);
      }
    }

    if (skipFlag) pPow = pPow->Next;
  }
}
