
/*
 *  $Id: util_RTU.c,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./util_RTU.c"                            |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "debugoff.h"


# define        LAYERS_NUMBER        6
# define        LAYERS_ALIAS         3
# define        TRACK_PITCH          MBKSCALE(5)
# define        F_PITCH_INF          0x00000001
# define        F_PITCH_SUP          0x00000002
# define        HT_TERM_SIZE         1024


# define        STRING_OBSTACLE      "obs"


/*  ------------------------------------------------------------------
 *  Internal types.
 */

  typedef struct obsTrack_s {
    long  min;
    long  max;
  } obsTrack_t;


/*  ------------------------------------------------------------------
 *  Local variables (prefix 'LV_').
 */

  static struct      phfig *LV_phfig;
  static struct obsTrack_s *LV_ttObs[LAYERS_NUMBER];
  static              char  LV_tLayersNames[LAYERS_NUMBER][LAYERS_ALIAS];
  static              long  LV_hTracksNumber;
  static              long  LV_vTracksNumber;
  static              long  LV_hTracksMax;
  static              long  LV_vTracksMax;
  static         authtable *LV_htTerm;


/*  ------------------------------------------------------------------
 *  Internal functions.
 */

# define  SETMIN(v,m)       ((v) = m_Min((v),(m)))
# define  SETMAX(v,m)       ((v) = m_Max((v),(m)))
# define  LAMBDA2TRACKS(l)  (lambda2pitch((l), F_PITCH_INF))
# define  TRACKS2LAMBDA(t)  ((t) * TRACK_PITCH)
# define  NORMVIASIDE(s)    (((s) > MBK_VIA_SIDE) ? (s) : 0L)

  static void      buildHTTerm   __FP((struct phseg *apSeg));
  static void        delHTTerm   __FP((void));
  static long     lambda2pitch   __FP((long aLength, long aFlags));
  static long     getLayerTable  __FP((char aLayer));
  static void  allocTableLayers  __FP((void));
  static void     allocTableObs  __FP((void));
  static void            addObs  __FP((long  aiTable,
                                       long  aX1,
                                       long  aY1,
                                       long  aX2,
                                       long  aY2));
  static void           seg2obs  __FP((struct phseg *apSeg));
  static void           via2obs  __FP((struct phvia *apVIA));
  static void      instance2obs  __FP((struct phins *pIns));
  extern void     instances2obs  __FP((void));
  static void        wiring2obs  __FP((void));
  static void     obs2layers     __FP((void));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "buildHTTerm()".
 */

static void  buildHTTerm(apSeg)
  struct phseg *apSeg;
{
  struct phseg *pSeg;


  LV_htTerm = createauthtable (HT_TERM_SIZE);

  for (pSeg = apSeg; pSeg != NULL; pSeg = pSeg->NEXT) {
    if (pSeg->NAME == NULL) {
      eprinth ("rtu");
      eprintf ("\n  Segment (%ld,%ld) (%ld,%ld) width %ld layer id %d",
               pSeg->X1,
               pSeg->Y1,
               pSeg->X2,
               pSeg->Y2,
               pSeg->WIDTH,
               (int)pSeg->LAYER);
      eprintf ("\n  doesn't have a name.\n");
      EXIT (1);
    }

    if (!isCALU (pSeg->LAYER))                  continue;
    if (searchauthelem (LV_htTerm, pSeg->NAME)) continue;

    addauthelem (LV_htTerm, pSeg->NAME, 0);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "delHTTerm()".
 */

static void  delHTTerm()
{
  destroyauthtable (LV_htTerm);
}


/*  ------------------------------------------------------------------
 *  Function  :  "lambda2pitch()".
 */

static long  lambda2pitch(aLength, aFlags)
  long  aLength;
  long  aFlags;
{
  long  pitch;


  pitch = aLength / TRACK_PITCH;

  if (aFlags & F_PITCH_SUP)
    pitch += (aLength % TRACK_PITCH) ? 1 : 0;
    
  return (pitch);
}

/*  ------------------------------------------------------------------
 *  Function  :  "getLayerTable()".
 */

static long  getLayerTable(aLayer)
  char  aLayer;
{
  long  iTable, iAlias;


  for (iTable = 1; iTable < LAYERS_NUMBER; iTable++) {
    for (iAlias = 0; iAlias < LAYERS_ALIAS; iAlias++) {
      if (LV_tLayersNames[iTable][iAlias] == aLayer) {
        return (iTable);
      }
    }
  }

  /* No table found : return LAYERS_NUMBER. */
  return (LAYERS_NUMBER);
}


/*  ------------------------------------------------------------------
 *  Function  :  "allocTableLayers()".
 */

static void  allocTableLayers()
{
  /* Base layer. */
  LV_tLayersNames[0][0] = ALU1;
  LV_tLayersNames[1][0] = ALU2;
  LV_tLayersNames[2][0] = ALU3;
  LV_tLayersNames[3][0] = ALU4;
  LV_tLayersNames[4][0] = ALU5;
  LV_tLayersNames[5][0] = ALU6;

  /* Associated obstacles layer. */
  LV_tLayersNames[0][1] = TALU1;
  LV_tLayersNames[1][1] = TALU2;
  LV_tLayersNames[2][1] = TALU3;
  LV_tLayersNames[3][1] = TALU4;
  LV_tLayersNames[4][1] = TALU5;
  LV_tLayersNames[5][1] = TALU6;

  /* Associated terminal layer. */
  LV_tLayersNames[0][2] = CALU1;
  LV_tLayersNames[1][2] = CALU2;
  LV_tLayersNames[2][2] = CALU3;
  LV_tLayersNames[3][2] = CALU4;
  LV_tLayersNames[4][2] = CALU5;
  LV_tLayersNames[5][2] = CALU6;
}


/*  ------------------------------------------------------------------
 *  Function  :  "allocTableObs()".
 */

static void  allocTableObs()
{
  long  iTable, iTrack;
  long  tracksNumber, trackMax;


  LV_hTracksNumber = LAMBDA2TRACKS (LV_phfig->YAB2 - LV_phfig->YAB1) + 1;
  LV_hTracksMax    = LAMBDA2TRACKS (LV_phfig->XAB2 - LV_phfig->XAB1) + 1;

  LV_vTracksNumber = LAMBDA2TRACKS (LV_phfig->XAB2 - LV_phfig->XAB1) + 1;
  LV_vTracksMax    = LAMBDA2TRACKS (LV_phfig->YAB2 - LV_phfig->YAB1) + 1;


  for (iTable = 0; iTable < LAYERS_NUMBER; iTable++) {
    if (!(iTable % 2)) {
      /* i.e. ALU1, ALU3 & ALU5 : vertical tracks. */
      tracksNumber = LV_vTracksNumber;
      trackMax     = LV_vTracksMax;
    } else {
      /* i.e. ALU2, ALU4 & ALU6 : horizontal tracks. */
      tracksNumber = LV_hTracksNumber;
      trackMax     = LV_hTracksMax;
    }


    LV_ttObs[iTable] = (struct obsTrack_s*)mbkalloc (
                         sizeof (struct obsTrack_s) * tracksNumber);

    /* Initialize table boundaries. */
    for (iTrack = 0; iTrack < tracksNumber; iTrack++) {
      LV_ttObs[iTable][iTrack].min = trackMax;
      LV_ttObs[iTable][iTrack].max = 0L;
    }
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "addObs()".
 */

static void  addObs(aiTable, aX1, aY1, aX2, aY2)
  long  aiTable;
  long  aX1, aY1, aX2, aY2;
{
  long  iTrack, ivTrackMin, ivTrackMax;
  long           hTrackMin,  hTrackMax;


  if (!(aiTable % 2)) {
    /* i.e. ALU1, ALU3 & ALU5 : vertical tracks. */
     hTrackMin = lambda2pitch (aY1 - LV_phfig->YAB1, F_PITCH_INF);
     hTrackMax = lambda2pitch (aY2 - LV_phfig->YAB1, F_PITCH_SUP);

    ivTrackMin = lambda2pitch (aX1 - LV_phfig->XAB1, F_PITCH_INF);
    ivTrackMax = lambda2pitch (aX2 - LV_phfig->XAB1, F_PITCH_SUP);
  } else {
    /* i.e. ALU2, ALU4 & ALU6 : horizontal tracks. */
     hTrackMin = lambda2pitch (aX1 - LV_phfig->XAB1, F_PITCH_INF);
     hTrackMax = lambda2pitch (aX2 - LV_phfig->XAB1, F_PITCH_SUP);

    ivTrackMin = lambda2pitch (aY1 - LV_phfig->YAB1, F_PITCH_INF);
    ivTrackMax = lambda2pitch (aY2 - LV_phfig->YAB1, F_PITCH_SUP);
  }


  for (iTrack = ivTrackMin; iTrack <= ivTrackMax; iTrack++) {
    SETMIN (LV_ttObs[aiTable][iTrack].min, hTrackMin);
    SETMAX (LV_ttObs[aiTable][iTrack].max, hTrackMax);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "seg2obs()".
 */

static void  seg2obs(apSeg)
  struct phseg *apSeg;
{
  long  iTable, X1, Y1, X2, Y2, trackWidth;


  /* Special policy for xALU1 layers : segments are all passed "as is"
   * in TALU1.
   */
  switch (apSeg->LAYER) {
    case  ALU1:
    case CALU1:
    case TALU1:
      /* Do nothing.
       *addphseg (LV_phfig,
       *         TALU1,
       *         apSeg->WIDTH,
       *         apSeg->X1,
       *         apSeg->Y1,
       *         apSeg->X2,
       *         apSeg->Y2,
       *         STRING_OBSTACLE
       *         );
       */
      return;
  }


  iTable     = getLayerTable (apSeg->LAYER);


  if (iTable == LAYERS_NUMBER) return;


  trackWidth = getLayerTrackWidth (apSeg->LAYER);

  switch (apSeg->TYPE) {
    case UP:
    case DOWN:
      X1 = apSeg->X1 - (apSeg->WIDTH - trackWidth) / 2;
      X2 = apSeg->X2 + (apSeg->WIDTH - trackWidth) / 2;
      Y1 = apSeg->Y1;
      Y2 = apSeg->Y2;
      break;
    case LEFT:
    case RIGHT:
    default:
      X1 = apSeg->X1;
      X2 = apSeg->X2;
      Y1 = apSeg->Y1 - (apSeg->WIDTH - trackWidth) / 2;
      Y2 = apSeg->Y2 + (apSeg->WIDTH - trackWidth) / 2;
      break;
  }

  addObs (iTable, X1, Y1, X2, Y2);
}


/*  ------------------------------------------------------------------
 *  Function  :  "via2obs()".
 */

static void  via2obs(apVIA)
  struct phvia *apVIA;
{
  long  iTable, X1, Y1, X2, Y2, trackWidth;
  char  layer;
# if 0
  char  topLayer;
# endif


  if (apVIA->NAME == NULL) {
      eprinth ("rtu");
      eprintf ("\n  VIA (%ld,%ld) size (%ld,%ld) type id %d",
               MBKUNSCALE (apVIA->XVIA),
               MBKUNSCALE (apVIA->YVIA),
               MBKUNSCALE (apVIA->DX),
               MBKUNSCALE (apVIA->DY),
               (int)apVIA->TYPE);
      eprintf ("\n  doesn't have a name.\n");
      EXIT (1);
    }


  __DBG( fprintf (stderr, "via2obs(): VIA at %ld %ld.\n",
                          apVIA->XVIA, apVIA->YVIA); )

  /* Check if this VIA belongs to an terminal net. */
  /* if (searchauthelem (LV_htTerm, apVIA->NAME)) return; */


  for (layer = getBottomVIALayer (apVIA->TYPE); TRUE;
       layer = getUpVIALayer     (apVIA->TYPE, layer)) {
    iTable = getLayerTable (layer);

    if ((!isvdd (apVIA->NAME) && !isvss (apVIA->NAME)) || (iTable % 2)) {
      if (iTable != LAYERS_NUMBER) {
        __DBG (fprintf (stderr, "  OBS Layer %d\n", (int)layer); )

          trackWidth = getLayerTrackWidth (layer);

        X1 = apVIA->XVIA - (NORMVIASIDE(apVIA->DX) - trackWidth) / 2;
        X2 = apVIA->XVIA + (NORMVIASIDE(apVIA->DX) - trackWidth) / 2;
        Y1 = apVIA->YVIA - (NORMVIASIDE(apVIA->DY) - trackWidth) / 2;
        Y2 = apVIA->YVIA + (NORMVIASIDE(apVIA->DY) - trackWidth) / 2;

        addObs (iTable, X1, Y1, X2, Y2);
      }
    }

    if (layer == getTopVIALayer (apVIA->TYPE)) break;
  }

# if 0
  fprintf (stderr, "  Intermediate layers :\n");
  layer    = getUpVIALayer  (apVIA->TYPE, getBottomVIALayer (apVIA->TYPE));
  topLayer = getTopVIALayer (apVIA->TYPE);

  while (layer != topLayer) {
    iTable = getLayerTable (layer);

    if (iTable != LAYERS_NUMBER) {
      fprintf (stderr, "  OBS Layer %d\n", (int)layer);

      trackWidth = getLayerTrackWidth (layer);

      X1 = apVIA->XVIA - (NORMVIASIDE(apVIA->DX) - trackWidth) / 2;
      X2 = apVIA->XVIA + (NORMVIASIDE(apVIA->DX) - trackWidth) / 2;
      Y1 = apVIA->YVIA - (NORMVIASIDE(apVIA->DY) - trackWidth) / 2;
      Y2 = apVIA->YVIA + (NORMVIASIDE(apVIA->DY) - trackWidth) / 2;

      addObs (iTable, X1, Y1, X2, Y2);
    }

    layer = getUpVIALayer (apVIA->TYPE, layer);
  }
# endif
}


/*  ------------------------------------------------------------------
 *  Function  :  "instance2obs()".
 */

static void  instance2obs(apIns)
  struct phins *apIns;
{
  struct phfig *pFig;
  struct phseg *pSeg, flatSeg;
# if 0
  struct phvia *pVIA, flatVIA;
          char *viaName = STRING_OBSTACLE;
# endif


  pFig = getphfig (apIns->FIGNAME, 'A');

  for (pSeg = pFig->PHSEG; pSeg != NULL; pSeg = pSeg->NEXT) {
    if (  isvdd (pSeg->NAME ) ||  isvss (pSeg->NAME )) continue;
    if (!isCALU (pSeg->LAYER) && !isobs (pSeg))        continue;
    /*if (isCALU (pSeg->LAYER) || !isobs (pSeg))        continue;*/

    xyflatseg (&flatSeg, pSeg,
               apIns->XINS, apIns->YINS,
                pFig->XAB1,  pFig->YAB1,
                pFig->XAB2,  pFig->YAB2,
               apIns->TRANSF);

    seg2obs (&flatSeg);
  }

# if 0
  for (pVIA = pFig->PHVIA; pVIA != NULL; pVIA = pVIA->NEXT) {
    if (isvdd (pVIA->NAME) || isvss (pVIA->NAME)) continue;

    xyflatvia (&flatVIA, pVIA,
               apIns->XINS, apIns->YINS,
                pFig->XAB1,  pFig->YAB1,
                pFig->XAB2,  pFig->YAB2,
               apIns->TRANSF);

    flatVIA.NAME = (pVIA->NAME != NULL) ? pVIA->NAME : viaName;

    via2obs (&flatVIA);
  }
# endif
}


/*  ------------------------------------------------------------------
 *  Function  :  "wiring2obs()".
 */

static void  wiring2obs()
{
  struct phseg *pSeg;
  struct phvia *pVIA;


  for (pSeg = LV_phfig->PHSEG; pSeg != NULL; pSeg = pSeg->NEXT) {
    /* Terminals (i.e. segments in CALUx layers) are not to be
     * considered as obstacles.
     */
    /*if (!isCALU (pSeg->LAYER)) seg2obs (pSeg);*/
    if (!isvdd (pSeg->NAME) && !isvss (pSeg->NAME)) seg2obs (pSeg);
  }

  for (pVIA = LV_phfig->PHVIA; pVIA != NULL; pVIA = pVIA->NEXT) {
    /* Skip default VIA, as they belongs to regular wiring. */
    if (pVIA->DX || pVIA->DY) via2obs (pVIA);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "instances2obs()".
 */

extern void  instances2obs()
{
  struct phins *pIns;


  for (pIns = LV_phfig->PHINS; pIns != NULL; pIns = pIns->NEXT) {
    instance2obs (pIns);
  }
}



/*  ------------------------------------------------------------------
 *  Function  :  "obs2layers()".
 */

static void  obs2layers()
{
  long  iTable, iTrack, tracksNumber;
  long  X1, Y1, X2, Y2, width;
  char  layer;


  for (iTable = 0; iTable < LAYERS_NUMBER; iTable++) {
    layer = getTALU (LV_tLayersNames[iTable][0]);
    width = getLayerTrackWidth (layer);


    if (!(iTable % 2)) {
      /* i.e. ALU1, ALU3 & ALU5 : vertical tracks. */
      tracksNumber = LV_vTracksNumber;
    } else {
      /* i.e. ALU2, ALU4 & ALU6 : horizontal tracks. */
      tracksNumber = LV_hTracksNumber;
    }


    for (iTrack = 0; iTrack < tracksNumber; iTrack++) {
      /* Skip empty tracks. */
      if (LV_ttObs[iTable][iTrack].min >= LV_ttObs[iTable][iTrack].max)
        continue;

      if (!(iTable % 2)) {
        /* i.e. ALU1, ALU3 & ALU5 : vertical tracks. */
        X1 = X2 = LV_phfig->XAB1 + TRACKS2LAMBDA (iTrack);
        Y1 = LV_phfig->YAB1 + TRACKS2LAMBDA (LV_ttObs[iTable][iTrack].min);
        Y2 = LV_phfig->YAB1 + TRACKS2LAMBDA (LV_ttObs[iTable][iTrack].max);
      } else {
        /* i.e. ALU2, ALU4 & ALU6 : horizontal tracks. */
        Y1 = Y2 = LV_phfig->YAB1 + TRACKS2LAMBDA (iTrack);
        X1 = LV_phfig->XAB1 + TRACKS2LAMBDA (LV_ttObs[iTable][iTrack].min);
        X2 = LV_phfig->XAB1 + TRACKS2LAMBDA (LV_ttObs[iTable][iTrack].max);
      }


      addphseg (LV_phfig, layer, width, X1, Y1, X2, Y2, STRING_OBSTACLE);
    } /* End of "for (iTrack ...)". */
  } /* End of "for (iTable ...)". */
}


/*  ------------------------------------------------------------------
 *  Function  :  "rtu()".
 */

extern void  rtu(aPhfig)
  struct phfig *aPhfig;
{
  LV_phfig = aPhfig;


  allocTableLayers ();
  allocTableObs ();

  buildHTTerm (aPhfig->PHSEG);
  instances2obs ();
  wiring2obs ();
  obs2layers ();
  delHTTerm ();

}
