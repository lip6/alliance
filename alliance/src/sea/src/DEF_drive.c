/*
 *  $Id: DEF_drive.c,v 1.3 2002/09/14 19:10:40 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./DEF_drive.c"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "debugoff.h"
# include  "util_Defs.h"
# include  "DEF_drive.h"


/*  ------------------------------------------------------------------
 *  Internal types and defines.
 */

#   define        C_DIRECTION_NONE      ((char)0)
#   define        C_DIRECTION_INPUT     ((char)1)
#   define        C_DIRECTION_OUTPUT    ((char)2)
#   define        C_DIRECTION_TRISTATE  ((char)3)
#   define        C_DIRECTION_INOUT     ((char)4)
#   define        C_DIRECTION_FEEDTHRU  ((char)5)

#   define        C_USE_NONE            ((char)0)
#   define        C_USE_SIGNAL          ((char)1)
#   define        C_USE_ANALOG          ((char)2)
#   define        C_USE_POWER           ((char)3)
#   define        C_USE_GROUND          ((char)4)
#   define        C_USE_CLOCK           ((char)5)

#   define        TTERM_SIZE            1024

#   define        PTYPE_PHFIG           1024
#   define        PTYPE_LOFIG           1025

#   define        F_REGULAR_NET         0x00000001
#   define        F_SPECIAL_NET         0x00000002

#   define        F_SEG_WIDTH           0x00000001
#   define        F_SEG_STRIPE          0x00000002


/*  ------------------------------------------------------------------
 *  Internal types.
 */

  typedef struct row_s {
            char *rowName;
            char *rowType;
            long  x;
            long  y;
            long  orient;
            long  doNumber;
            long  byNumber;
            long  stepX;
            long  stepY;
    struct row_s *next;
  } row_t;

  typedef struct track_s {
            char  axisName;
            long  start;
            long  doNumber;
            long  step;
            char *routingLayers;
            long  x;
            long  y;
    struct track_s *next;
  } track_t;

  typedef struct blockage_s {
            long  x;
            long  y;
            long  width;
            long  height;
    struct blockage_s *next;
  } blockage_t;


/*  ------------------------------------------------------------------
 *  Internal variables.
 */

  static              FILE *DEF_FILE;
  static              long  LV_Flags      = 0L;
  static              long  LV_localFlags = 0L;
  static              char *LV_name;
  static struct      lofig *LV_lofig;
  static struct      phfig *LV_phfig;
  static struct      row_s *LV_row;
  static struct    track_s *LV_track;
  static struct blockage_s *LV_blockage;
  static struct  authtable *LV_htTerm;
  static              char  routingLayers3[256] = "L_ALU1 L_ALU2 L_ALU3";
  static              char  routingLayers4[256] =
                             "L_ALU1 L_ALU2 L_ALU3 L_ALU4";
  static              char  routingLayers6[256] =
                             "L_ALU1 L_ALU2 L_ALU3 L_ALU4 L_ALU5 L_ALU6";


/*  ------------------------------------------------------------------
 *  Local functions.
 */

  static   struct phins *getphinslink       __FP((struct loins *apLoins));
  static           void  buildphinslinks    __FP((void));
  static           void  buildtterm         __FP((void));
  static           void  freetterm          __FP((void));
  static           void  freephinslinks     __FP((void));
  static           void  swapOrient         __FP((char *apSameOrient,
                                                  char *apOppositeOrient));
  static           char  normOrient         __FP((char aOrient));
  static           long  orientPhfig        __FP((struct phfig *apPhfig,
                                                          char *apSameOrient,
                                                          char *apOppositeOrient));
  static           char *DIRtoa             __FP((char acDIR));
  static           char *USEtoa             __FP((char acUSE));
  static           char *strLCat            __FP((char *asLine,
                                                  char *asEl,
                                                  long *aplLine,
                                                  long  alMax));
  static           void  fprintDIEAREA      __FP((void));
  static   struct row_s *newrow             __FP((void));
  static           void  freerow            __FP((void));
  static           void  ref2ROW            __FP((void));
  static   struct row_s *getBaseRow         __FP((long  aX,
                                                  long  aY,
                                                  char  aOrient));
  static           void  fprintROWS         __FP((void));
  static struct track_s *newtrack           __FP((void));
  static           void  freetrack          __FP((void));
  static           void  ref2TRACK          __FP((void));
  static           void  fprintTRACKS       __FP((void));
  static struct blockage_s *newblockage     __FP((void));
  static           void  freeblockage       __FP((void));
  static           void  ref2BLOCKAGE       __FP((void));
  static           void  fprintBLOCKAGE     __FP((void));
  static           void  fprintVIAMATRIX    __FP((char aType,
                                                  long aWidth,
                                                  long aHeight));
  static           void  fprintVIAS         __FP((void));
  static           long  checkPhins         __FP((struct phins *apPhins));
  static           void  fprintCOMPONENTS   __FP((void));
  static           void  fprintPIN          __FP((struct locon *apLocon,
                                                  struct phcon *apPhcon,
                                                          char *asState));
  static           void  fprintPINS         __FP((void));
  static           void  fprintWIRESEG      __FP((struct phseg *apPhseg,
                                                          long  aFlags));
  static           void  fprintWIREVIA      __FP((struct phvia *apPhvia,
                                                          long  aFlags));
  static           void  fprintWIRES        __FP((struct losig *apLosig));
  static           void  fprintNET          __FP((struct losig *apLoSig,
                                                          char  aUse));
  static           void  fprintNETS         __FP((void));
  static           void  fprintSPECIALNETS  __FP((void));
  static           void  fprintDEF          __FP((void));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "getphinslink()".
 */

static struct phins *getphinslink(apLoins)
  struct loins *apLoins;
{
  struct ptype *pType;


  if ((pType = getptype (apLoins->USER, PTYPE_PHFIG)) == NULL)
    return (NULL);

  return ((struct phins*)pType->DATA);
}


/*  ------------------------------------------------------------------
 *  Function  :  "buildphinslinks()".
 */

static void  buildphinslinks()
{
  struct loins *pLoins;
  struct phins *pPhins;
          long  flag;


  flag = FALSE;

  /* Check if there is a physical figure. */
  if (!LV_phfig) return;

  for (pLoins = LV_lofig->LOINS; pLoins != NULL; pLoins = pLoins->NEXT) {
    if ((pPhins = findphins (LV_phfig, pLoins->INSNAME))) {
      if (pLoins->FIGNAME != pPhins->FIGNAME) {
        flag = TRUE;

        eprinth (NULL);
        eprintf ("Instances of \"%s\" have NOT the same physical",
                 pLoins->INSNAME);
        eprintf (" and logical model :\n");
        eprintf ("  logical := \"%s\"",    pLoins->FIGNAME);
        eprintf (" physical := \"%s\".\n", pPhins->FIGNAME);

        continue;
      }

      pLoins->USER = addptype (pLoins->USER, PTYPE_PHFIG, (void*)pPhins);
      pPhins->USER = addptype (pPhins->USER, PTYPE_LOFIG, (void*)pLoins);
    }
  }


  /* Check if all physical instances are in the netlist. */


  for (pPhins = LV_phfig->PHINS; pPhins != NULL; pPhins = pPhins->NEXT) {
    if (!getptype (pPhins->USER, PTYPE_LOFIG)) {
      if (!flag) {
        /* Print the head error message. */
        flag = TRUE;

        eprinth (NULL);
        eprintf ("The following physical instances are not in the");
        eprintf (" netlist :\n");
      }

      eprintf ("- \"%s\" (model \"%s\").\n",
               pPhins->INSNAME, pPhins->FIGNAME);
    }
  }


  if (flag) EXIT (1);
}


/*  ------------------------------------------------------------------
 *  Function  :  "freephinslinks()".
 */

static void  freephinslinks()
{
  struct loins *pLoins;
  struct phins *pPhins;


  /* Check if there is a physical figure. */
  if (!LV_phfig) return;

  for (pLoins = LV_lofig->LOINS; pLoins != NULL; pLoins = pLoins->NEXT) {
    if (pLoins->USER)
      pLoins->USER = delptype (pLoins->USER, PTYPE_PHFIG);
  }


  for (pPhins = LV_phfig->PHINS; pPhins != NULL; pPhins = pPhins->NEXT) {
    if (pPhins->USER)
      pPhins->USER = delptype (pPhins->USER, PTYPE_LOFIG);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "buildtterm()".
 */

static void  buildtterm()
{
  struct locon *pLocon;


  LV_htTerm = createauthtable (TTERM_SIZE);

  for (pLocon = LV_lofig->LOCON; pLocon != NULL; pLocon = pLocon->NEXT) {
    addauthelem (LV_htTerm, getsigname (pLocon->SIG), (long)pLocon->NAME);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "freetterm()".
 */

static void  freetterm()
{
  destroyauthtable (LV_htTerm);
}



/*  ------------------------------------------------------------------
 *  Function  :  "swapOrient()".
 */

static void  swapOrient(apSameOrient, apOppositeOrient)
  char *apSameOrient;
  char *apOppositeOrient;
{
  char  tmpOrient;

   tmpOrient        = *apSameOrient;
  *apSameOrient     = *apOppositeOrient;
  *apOppositeOrient =  tmpOrient;
}


/*  ------------------------------------------------------------------
 *  Function  :  "normOrient()".
 */

static char  normOrient(aOrient)
  char  aOrient;
{
  char  normOrient;


  switch (aOrient) {
    default:
    case DEF_N:  normOrient = DEF_N;  break;
    case DEF_FN: normOrient = DEF_N;  break;
    case DEF_FS: normOrient = DEF_FS; break;
    case DEF_S:  normOrient = DEF_FS; break;
    case DEF_W:  normOrient = DEF_W;  break;
    case DEF_FW: normOrient = DEF_W;  break;
    case DEF_E:  normOrient = DEF_FE; break;
    case DEF_FE: normOrient = DEF_FE; break;
  }

  return (normOrient);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getOppositeOrient()".
 */

static char  getOppositeOrient(aOrient)
  char  aOrient;
{
  char  oppositeOrient;


  switch (aOrient) {
    default:
    case DEF_N:  oppositeOrient = DEF_FS; break;
    case DEF_FN: oppositeOrient = DEF_FS; break;
    case DEF_FS: oppositeOrient = DEF_N;  break;
    case DEF_S:  oppositeOrient = DEF_N;  break;
    case DEF_W:  oppositeOrient = DEF_FE; break;
    case DEF_FW: oppositeOrient = DEF_FE; break;
    case DEF_E:  oppositeOrient = DEF_W;  break;
    case DEF_FE: oppositeOrient = DEF_W;  break;
  }

  return (oppositeOrient);
}


/*  ------------------------------------------------------------------
 *  Function  :  "orientPhfig()".
 */

static long  orientPhfig(apPhfig, apSameOrient, apOppositeOrient)
  struct phfig *apPhfig;
          char *apSameOrient;
          char *apOppositeOrient;
{
  struct ePow_s *plPow;
           long  sliceHeight;


  plPow = buildPow (apPhfig, ALU1, C_POWER_HORIZONTAL, "");

  sliceHeight = (apPhfig->YAB2 - apPhfig->YAB1) / MBK_Y_SLICE;

  if (!plPow) {
    if (LV_Flags & F_TRUST_ORIENT) {
      *apSameOrient     = DEF_N;
      *apOppositeOrient = DEF_FS;

      return (FALSE);
    }

    eprinth (NULL);
    eprintf ("\n  Model \"%s\" have no horizontal ALU1 power supplies.",
             apPhfig->NAME);
    eprintf ("\n  Can't guess allowed orientations.\n");
    EXIT (1);
  }


  switch (plPow->Type) {
    default:
    case C_POWER_ISVSS:
      *apSameOrient     = DEF_N;
      *apOppositeOrient = DEF_FS;
      break;
    case C_POWER_ISVDD:
      *apSameOrient     = DEF_FS;
      *apOppositeOrient = DEF_N;
      break;
  }

  if (!(sliceHeight % 2))
    *apOppositeOrient = *apSameOrient;


  freePow (plPow);

  return (TRUE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DIRtoa()".
 */

static char *DIRtoa(acDIR)
  char  acDIR;
{
  switch(acDIR) {
    case C_DIRECTION_NONE:     return((char*)NULL);
    case C_DIRECTION_INPUT:    return("INPUT");
    case C_DIRECTION_OUTPUT:   return("OUTPUT");
    case C_DIRECTION_TRISTATE: return("OUTPUT");
    case C_DIRECTION_INOUT:    return("INOUT");
    case C_DIRECTION_FEEDTHRU: return("FEEDTHRU");
  }
  return((char*)NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "USEtoa()".
 */

static char *USEtoa(acUSE)
  char  acUSE;
{
  switch(acUSE) {
    case C_USE_NONE:   return((char*)NULL);
    case C_USE_SIGNAL: return("signal");
    case C_USE_ANALOG: return("analog");
    case C_USE_POWER:  return("power");
    case C_USE_GROUND: return("ground");
    case C_USE_CLOCK:  return("clock");
  }

  return((char*)NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "strLCat()".
 */

static char *strLCat(asLine, asEl, aplLine, alMax)
  char *asLine, *asEl;
  long *aplLine, alMax;
{
  long  lCopy;


  /* Copy the string `asEl' if there is enougth place. */
  lCopy = m_Min(alMax - *aplLine - 1, strlen(asEl));
  strncpy(asLine + *aplLine, asEl, lCopy);
  *aplLine += lCopy;

  /* Terminate the string by a zero. */
  asLine[(*aplLine)] = (char)0;

  return(asLine);
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintDIEAREA()".
 */

static void  fprintDIEAREA()
{
  if (LV_phfig) {
    fprintf (DEF_FILE,
             "\nDIEAREA ( %ld %ld ) ( %ld %ld ) ;\n",
             MBK2DEF_length (LV_phfig->XAB1),
             MBK2DEF_length (LV_phfig->YAB1),
             MBK2DEF_length (LV_phfig->XAB2),
             MBK2DEF_length (LV_phfig->YAB2));
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "newrow()".
 */

static struct row_s *newrow()
{
  struct row_s *pRow;

  pRow = (row_t*)mbkalloc (sizeof (row_t));
  pRow->next = LV_row;

  return (LV_row = pRow);
}


/*  ------------------------------------------------------------------
 *  Function  :  "freerow()".
 */

static void  freerow()
{
  struct row_s *pRow;

  while (LV_row) {
    pRow = LV_row->next;

    mbkfree (LV_row->rowName);
    mbkfree (LV_row->rowType);
    mbkfree (LV_row);

    LV_row = pRow;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "ref2ROW()".
 */

static void  ref2ROW()
{
  struct phref *pPhref;
  struct row_s *pRow;


  if (LV_phfig) {
    for (pPhref = LV_phfig->PHREF; pPhref != NULL; pPhref = pPhref->NEXT) {
      if (!strncmp (pPhref->NAME, "row_", 4)) {
        pRow = newrow ();

        MBK2DEF_row (pPhref,
                     &pRow->rowName,
                     &pRow->rowType,
                     &pRow->orient,
                     &pRow->doNumber,
                     &pRow->byNumber,
                     &pRow->stepX,
                     &pRow->stepY,
                     &pRow->x,
                     &pRow->y);
      }
    }
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "hasROW()".
 */

extern long  hasROW(apPhfig)
  struct phfig *apPhfig;
{
  struct phref *pPhref;


  for (pPhref = apPhfig->PHREF; pPhref != NULL; pPhref = pPhref->NEXT) {
    if (!strncmp (pPhref->NAME, "row_", 4)) {
      return (TRUE);
    }
  }

  return(FALSE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "buildROWS()".
 */

extern long  buildROWS(apPhfig)
  struct phfig *apPhfig;
{
  struct phins *pIns;
          long  iRow;
          long  rowGridX0, rowGridY0, rowGridIns, rowSizeX, rowSizeY;
          char  sameOrient, oppositeOrient;
          char  rowName[1024];


  if (hasROW(apPhfig)) return (FALSE);


  pIns           = NULL;

  rowGridX0      = apPhfig->XAB1;
  rowGridY0      = apPhfig->YAB1;
      sameOrient = DEF_N;
  oppositeOrient = DEF_FS;

  rowSizeX       = (apPhfig->XAB2 - apPhfig->XAB1) / MBK_X_GRID;
  rowSizeY       = (apPhfig->YAB2 - apPhfig->YAB1) / MBK_Y_SLICE;


  for (pIns = apPhfig->PHINS; pIns != NULL; pIns = pIns->NEXT) {
    /* Skip unpitched instances. */
    if ((pIns->XINS - apPhfig->XAB1) % MBK_X_GRID) continue;
    if ((pIns->YINS - apPhfig->YAB1) % MBK_X_GRID) continue;

    if (!orientPhfig (getphfig (pIns->FIGNAME, 'A'),
                      &sameOrient,
                      &oppositeOrient)) continue;

    mprintf2 ("  o  Taking \"%s\" as reference for the ROW grid.\n",
              pIns->INSNAME);

    rowGridX0  = apPhfig->XAB1;
    rowGridY0  = apPhfig->YAB1
               + (pIns->YINS - apPhfig->YAB1) % MBK_Y_SLICE;
    
    rowSizeX   = (apPhfig->XAB2 - apPhfig->XAB1) / MBK_X_GRID;
    rowSizeY   = (apPhfig->YAB2 - rowGridY0)     / MBK_Y_SLICE;

    rowGridIns = (pIns->YINS - rowGridY0) / MBK_Y_SLICE;

    switch (pIns->TRANSF) {
      default:
      case SYM_X:
      case NOSYM:
        break;
      case SYM_Y:
      case SYMXY:
        swapOrient (&sameOrient, &oppositeOrient);
        break;
      /* We do not allow rotated instances. */
      case ROT_P:
      case ROT_M:
      case SY_RP:
      case SY_RM: continue;
    }

    __DBG (
      fflush (stdout);
      fprintf (stderr, "pIns->YINS     := %ld.\n", pIns->YINS);
      fprintf (stderr, "apPhfig->YAB1  := %ld.\n", apPhfig->YAB1);
      fprintf (stderr, "rowGridY0      := %ld.\n", rowGridY0);
      fprintf (stderr, "rowGridIns     := %ld.\n", rowGridIns);
      fflush (stderr);
    )
    if (rowGridIns % 2)
      swapOrient (&sameOrient, &oppositeOrient);

    break;
  }


  /* Restore the opposite orient, when the cell is 2*N slices tall. */
  oppositeOrient = getOppositeOrient (sameOrient);

  for (iRow = 0; iRow < rowSizeY; iRow++) {
    sprintf (rowName, "ROW_%ld", iRow);

    DEF2MBK_row (apPhfig,
                 rowName,
                 "core",
                 (iRow % 2) ? oppositeOrient : sameOrient,
                 rowSizeX,
                 1,
                 MBK2DEF_length (MBK_X_GRID),
                 MBK2DEF_length (MBK_Y_SLICE),
                 rowGridX0,
                 rowGridY0 + iRow * MBK_Y_SLICE
                 );
  }


  return (TRUE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getBaseRow()".
 */

static struct row_s *getBaseRow(aX, aY, aOrient)
  long  aX, aY;
  char  aOrient;
{
  struct row_s *pRow;
          long  rowMax;
          char  orient;


  for (pRow = LV_row; pRow != NULL; pRow = pRow->next) {
    orient = normOrient (aOrient);

    switch (aOrient) {
      case DEF_N:
      case DEF_FN:
      case DEF_FS:
      case DEF_S:
        if (pRow->y != aY) continue;
        rowMax = DEF2MBK_length (pRow->stepX) * pRow->doNumber;

        if ((aX < pRow->x) || (aX > pRow->x + rowMax)) continue;

        if (pRow->orient == orient) return (pRow);
        break;
      case DEF_W:
      case DEF_E:
      case DEF_FW:
      case DEF_FE:
        if (pRow->x != aX) continue;
        rowMax = DEF2MBK_length (pRow->stepY) * pRow->byNumber;

        if ((aY < pRow->y) || (aY > pRow->y + rowMax)) continue;

        if (pRow->orient == orient)
          return (pRow);
        break;
    }
  }


  return (NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintROWS()".
 */

static void  fprintROWS ()
{
  struct row_s *pRow;


  for (pRow = LV_row; pRow != NULL; pRow = pRow->next) {
    fprintf (DEF_FILE,
             "ROW %s %s %ld %ld %s DO %ld BY %ld STEP %ld %ld ;\n",
             pRow->rowName,
             pRow->rowType,
             pRow->x,
             pRow->y,
             DEF_orient2a(pRow->orient),
             pRow->doNumber,
             pRow->byNumber,
             pRow->stepX,
             pRow->stepY);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "newtrack()".
 */

static struct track_s *newtrack()
{
  struct track_s *pTrack;

  pTrack = (track_t*)mbkalloc (sizeof (track_t));
  pTrack->next = LV_track;

  return (LV_track = pTrack);
}


/*  ------------------------------------------------------------------
 *  Function  :  "freetrack()".
 */

static void  freetrack()
{
  struct track_s *pTrack;

  while (LV_track) {
    pTrack = LV_track->next;

    mbkfree (LV_track->routingLayers);
    mbkfree (LV_track);

    LV_track = pTrack;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "ref2TRACK()".
 */

static void  ref2TRACK()
{
  struct   phref *pPhref;
  struct track_s *pTrack;


  if (LV_phfig) {
    for (pPhref = LV_phfig->PHREF; pPhref != NULL; pPhref = pPhref->NEXT) {
      if (!strncmp (pPhref->NAME, "tracks.", 7)) {
        pTrack = newtrack ();

        MBK2DEF_track (pPhref,
                       &pTrack->axisName,
                       &pTrack->start,
                       &pTrack->doNumber,
                       &pTrack->step,
                       &pTrack->routingLayers,
                       &pTrack->x,
                       &pTrack->y);
      }
    }
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "buildTRACKS()".
 */

extern long  buildTRACKS(apPhfig, aFlags)
  struct phfig *apPhfig;
          long  aFlags;
{
  long  tracksX, tracksY, startX, startY;
  char *routingLayers;


  tracksX = ((apPhfig->XAB2 - apPhfig->XAB1) / MBK_X_GRID) - 1;
  tracksY = ((apPhfig->YAB2 - apPhfig->YAB1) / MBK_X_GRID) - 1;
  startX  =   apPhfig->XAB1 + MBK_X_GRID;
  startY  =   apPhfig->YAB1 + MBK_X_GRID;

  routingLayers = routingLayers6;
  if (aFlags & F_LAYERS_3) {
    routingLayers = routingLayers3;
  }
  if (aFlags & F_LAYERS_4) {
    routingLayers = routingLayers4;
  }


  /* Allocate the horizontal tracks. */
  DEF2MBK_track (apPhfig,
                 "Y",
                 MBK2DEF_length (startY),
                 tracksY,
                 MBK2DEF_length (MBK_X_GRID),
                 routingLayers,
                 apPhfig->XAB1,
                 apPhfig->YAB1 + MBK_X_GRID);


  /* Allocate the vertical tracks. */
  DEF2MBK_track (apPhfig,
                 "X",
                 MBK2DEF_length (startX),
                 tracksX,
                 MBK2DEF_length (MBK_X_GRID),
                 routingLayers,
                 apPhfig->XAB1 + MBK_X_GRID,
                 apPhfig->YAB1);


  return (TRUE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintTRACKS()".
 */

static void  fprintTRACKS ()
{
  struct track_s *pTrack;


  for (pTrack = LV_track; pTrack != NULL; pTrack = pTrack->next) {
    fprintf (DEF_FILE,
             "TRACKS %c %ld DO %ld STEP %ld LAYER %s ;\n",
             pTrack->axisName,
             pTrack->start,
             pTrack->doNumber,
             pTrack->step,
             pTrack->routingLayers);
  }


  fprintf (DEF_FILE, "\n\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "newblockage()".
 */

static struct blockage_s *newblockage()
{
  struct blockage_s *pBlockage;

  pBlockage = (blockage_t*)mbkalloc (sizeof (blockage_t));
  pBlockage->next = LV_blockage;

  return (LV_blockage = pBlockage);
}


/*  ------------------------------------------------------------------
 *  Function  :  "freeblockage()".
 */

static void  freeblockage()
{
  struct blockage_s *pBlockage;

  while (LV_blockage) {
    pBlockage = LV_blockage->next;

    mbkfree (LV_blockage);

    LV_blockage = pBlockage;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "ref2BLOCKAGE()".
 */

static void  ref2BLOCKAGE()
{
  struct   phref *pPhref;
  struct blockage_s *pBlockage;


  if (LV_phfig) {
    for (pPhref = LV_phfig->PHREF; pPhref != NULL; pPhref = pPhref->NEXT) {
      if (!strncmp (pPhref->NAME, "blockage.", 9)) {
        pBlockage = newblockage ();

        MBK2DEF_blockage (pPhref,
                          &pBlockage->x,
                          &pBlockage->y,
                          &pBlockage->width,
                          &pBlockage->height);
      }
    }
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintBLOCKAGE()".
 */

static void  fprintBLOCKAGE()
{
  struct blockage_s *pBlockage;
               long  layer;


  for (pBlockage = LV_blockage;
       pBlockage != NULL; pBlockage = pBlockage->next) {

    if (pBlockage == LV_blockage) {
      fprintf (DEF_FILE,   "    - _BLOCKAGE_RESERVED");
      fprintf (DEF_FILE, "\n      + ROUTED ");
    } else {
      fprintf (DEF_FILE, "\n      NEW ");
    }

    for (layer = 1; layer < 7; layer++) {
      if (layer != 1)
        fprintf (DEF_FILE, "\n      NEW ");

      fprintf (DEF_FILE,
               "L_ALU%ld %ld + SHAPE BLOCKAGEWIRE ( %ld %ld ) ( %ld * )",
               layer,
               pBlockage->height,
               pBlockage->x,
               pBlockage->y + pBlockage->height / 2,
               pBlockage->x + pBlockage->width);
    }
  }

  if (LV_blockage)
    fprintf (DEF_FILE, "\n      + SOURCE DIST + USE GROUND + WEIGHT 0 ;\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintVIAMATRIX()".
 */

static void  fprintVIAMATRIX (aType, aWidth, aHeight)
  char  aType;
  long  aWidth;
  long  aHeight;
{
  long  xCutNB, yCutNB, xCut, yCut;


  xCutNB = (aWidth  / MBKSCALE(5)) / 2;
  yCutNB = (aHeight / MBKSCALE(5)) / 2;


  for (xCut = - xCutNB; xCut <= xCutNB; xCut++) {
    for (yCut = - yCutNB; yCut <= yCutNB; yCut++) {
      fprintf (DEF_FILE, "    + RECT %s ( %ld %ld ) ( %ld %ld )\n",
               DEF_layer2a (aType),
               MBK2DEF_length (xCut - SCALE_X / 2),
               MBK2DEF_length (yCut - SCALE_X / 2),
               MBK2DEF_length (xCut + SCALE_X / 2),
               MBK2DEF_length (yCut + SCALE_X / 2));
    }
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintVIAS()".
 */

static void  fprintVIAS()
{
  struct tLoseg_s *pTLoseg;
  struct eMVIA_s  *pMVIA;
            long   nVIA, flagDefault, width, height;
            char   viaName[1024];


  pTLoseg    = gettloseg (LV_phfig);

  if (pTLoseg) {
    nVIA = 0;

    __DBG(
      fflush (stdout);
      fprintf (stderr, "pTLoseg := 0x%08lx\n", (unsigned long)pTLoseg);
      fprintf (stderr, "tMVIA   := 0x%08lx\n", (unsigned long)pTLoseg->tMVIA);
      fflush (stderr);
    )

    for (pMVIA = pTLoseg->tMVIA; pMVIA != NULL; pMVIA = pMVIA->next)
      { nVIA++; }

    fprintf (DEF_FILE, "VIAS %ld ;\n", nVIA);

    for (pMVIA = pTLoseg->tMVIA; pMVIA != NULL; pMVIA = pMVIA->next) {
      flagDefault =    (pMVIA->width  <= MBKSCALE(2))
                    && (pMVIA->height <= MBKSCALE(2));

      __DBG(
        if (!flagDefault) {
          fprintf (stderr, "macro VIA : %d (%ldx%ld)\n",
                           (int)pMVIA->type, pMVIA->width, pMVIA->height);
        }
      )

      width  = MBKSCALE(2);
      height = MBKSCALE(2);

      fprintf (DEF_FILE, "  - %s", DEF_via2a (pMVIA->type,
                                              pMVIA->width,
                                              pMVIA->height, viaName));

      if (!flagDefault) {
        width  = pMVIA->width;
        height = pMVIA->height;
      }

      fprintf (DEF_FILE, "\n    + RECT %s ( %ld %ld ) ( %ld %ld )\n",
               DEF_layer2a (getBottomVIALayer (pMVIA->type)),
               DEF2MBK_length (- width  / 2),
               DEF2MBK_length (- height / 2),
               DEF2MBK_length (  width  / 2),
               DEF2MBK_length (  height / 2));

      fprintVIAMATRIX (pMVIA->type, width, height);

      fprintf (DEF_FILE, "    + RECT %s ( %ld %ld ) ( %ld %ld ) ;\n",
               DEF_layer2a (getTopVIALayer (pMVIA->type)),
               DEF2MBK_length (- width  / 2),
               DEF2MBK_length (- height / 2),
               DEF2MBK_length (  width  / 2),
               DEF2MBK_length (  height / 2));
    }

    fprintf (DEF_FILE, "END VIAS\n\n");
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintPIN()".
 */

static void  fprintPIN(apLocon, apPhcon, asState)
  struct locon *apLocon;
  struct phcon *apPhcon;
          char *asState;
{
  char  sSig[1024];
  long  conLength;


  MBK2DEF_name(sSig, apLocon->NAME);


  fprintf(DEF_FILE, "    - %s.%ld + NET %s", sSig, apPhcon->INDEX, sSig);

  if (isvdd(apLocon->NAME)) {
    /*
     * if (LV_Flags & F_FIXED_PINS)
     *   fprintf(DEF_FILE, " + SPECIAL");
     */

    fprintf(DEF_FILE, " + DIRECTION INPUT + USE POWER");
  } else if (isvss(apLocon->NAME)) {
    /*
     * if (LV_Flags & F_FIXED_PINS)
     *   fprintf(DEF_FILE, " + SPECIAL");
     */

    fprintf(DEF_FILE, " + DIRECTION INPUT + USE GROUND");
  } else 
    fprintf(DEF_FILE, " + DIRECTION %s",
                       DIRtoa(MBK2DEF_locondir(apLocon)));
  
  if (    (LV_Flags & F_FIXED_PINS)
      &&  (strncmp(apLocon->NAME, "vddv", 4))
      &&  (strncmp(apLocon->NAME, "vssv", 4))
      &&  !isvdd (apLocon->NAME)
      &&  !isvss (apLocon->NAME)) {
    conLength = MBK_X_GRID;
    if (LV_Flags & F_EXPAND_ROUTE) conLength += FLOOR_XY_EXPAND;

    if (strcmp ("UNPLACED", asState)) {
      fprintf(DEF_FILE, "\n        + %s ( %ld %ld ) %s",
                        asState,
                        apPhcon->XCON,
                        apPhcon->YCON,
                        DEF_side2a (apPhcon->ORIENT));

      if (LV_Flags & F_EXPAND_PLACE) conLength += FLOOR_XY_EXPAND;
    } else {
      if (LV_Flags & F_EXPAND_PLACE) conLength += FLOOR_XY_EXPAND / 2;
    }

    fprintf(DEF_FILE,   " + LAYER %s ( %ld 0 ) ( %ld %ld )",
                           DEF_layer2a (apPhcon->LAYER),
                      - MBK2DEF_length (apPhcon->WIDTH / 2),
                        MBK2DEF_length (apPhcon->WIDTH / 2),
                        MBK2DEF_length (conLength));
  }

  fprintf(DEF_FILE, " ;\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintPINS()".
 */

static void  fprintPINS()
{
  struct    locon *pLoCon;
  struct    phcon  Phcon;
  struct eLoseg_s *pLoseg;
             long  loconNB, phconNB;


  loconNB = 0;

  /* First : compute the number of terminals. */
  for(pLoCon = LV_lofig->LOCON;
      pLoCon != (locon_list*)NULL; pLoCon = pLoCon->NEXT) {
    loconNB += 1;
    phconNB  = 0;

    if (LV_phfig) {
      pLoseg = getloseglist (LV_phfig, getsigname (pLoCon->SIG));

      for (; pLoseg != NULL; pLoseg = pLoseg->next) {
        if (pLoseg->type == LOSEG_CON) { phconNB += 1; }
      }

      if (phconNB > 1) loconNB += phconNB - 1;
    }
  }

  fprintf(DEF_FILE, "\n\nPINS %ld ;\n", loconNB);

  for(pLoCon = LV_lofig->LOCON;
      pLoCon != (locon_list*)NULL; pLoCon = pLoCon->NEXT) {

    phconNB = 0;

    if (LV_phfig) {
      pLoseg = getloseglist (LV_phfig, getsigname (pLoCon->SIG));

      for (; pLoseg != NULL; pLoseg = pLoseg->next) {
        __DBG(
          fprintf (stderr, "sig %s pLoseg->type := %ld\n",
                   getsigname (pLoCon->SIG),
                   pLoseg->type);
          fflush (stderr);
        )
        if (pLoseg->type == LOSEG_CON) {
          phconNB += 1;

          fprintPIN (pLoCon, (struct phcon*)(pLoseg->MBKobj), "FIXED");
        }
      }
    }

    if (!phconNB) {
      Phcon.XCON   = 0;
      Phcon.YCON   = 0;
      Phcon.INDEX  = 0;
      Phcon.WIDTH  = MBKSCALE(2);
      Phcon.LAYER  = ALU2;
      Phcon.ORIENT = SOUTH;

      fprintPIN (pLoCon, &Phcon, "UNPLACED");
    }
  }

  fprintf(DEF_FILE, "END PINS\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "checkPhins()".
 */

static long  checkPhins(apPhins)
  struct phins *apPhins;
{
  struct row_s *pRow;
          char  sameOrient, oppositeOrient, orient;


  orientPhfig (getphfig (apPhins->FIGNAME, 'A'),
               &sameOrient,
               &oppositeOrient);

  orient = DEF_N;

  switch (normOrient (MBK2DEF_transf (apPhins->TRANSF))) {
    case DEF_W:
    case DEF_N:  orient = sameOrient;     break;
    case DEF_FE:
    case DEF_FS: orient = oppositeOrient; break;
  }

  pRow = getBaseRow (apPhins->XINS, apPhins->YINS, orient);


  if (!pRow) {
    warnMBK (NULL);
    wprintf ("\n  Instance %s of model %s at (%ld,%ld) is NOT on row grid.\n",
             apPhins->INSNAME,
             apPhins->FIGNAME,
             apPhins->XINS,
             apPhins->YINS);
    wprintf (  "  Will be removed from the placement file.\n");

    return (FALSE);
  }
  
  return (TRUE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintCOMPONENTS()".
 */

static void  fprintCOMPONENTS()
{
  loins_list *pLoins;
  phins_list *pPhins;
        long  loInsNB;


  /* First : compute the number of instances. */
  for(pLoins  = LV_lofig->LOINS, loInsNB = 0L;
      pLoins != NULL; loInsNB++, pLoins = pLoins->NEXT);

  fprintf(DEF_FILE, "\n\nCOMPONENTS %ld ;\n", loInsNB);

  /* Dump the list of instance/model pairs. */
  for(pLoins  = LV_lofig->LOINS;
      pLoins != (loins_list*)NULL; pLoins  = pLoins->NEXT) {
    fprintf(DEF_FILE, "    - %s %s ", pLoins->INSNAME, pLoins->FIGNAME);

    if (!(pPhins = getphinslink(pLoins)) || !checkPhins (pPhins)) {
      fprintf(DEF_FILE, ";\n");
    } else {
      fprintf(DEF_FILE,
              "+ FIXED ( %ld %ld ) %s ;\n",
              MBK2DEF_length(pPhins->XINS),
              MBK2DEF_length(pPhins->YINS),
              DEF_orient2a(MBK2DEF_transf (pPhins->TRANSF)));
    }
  }
  

  fprintf(DEF_FILE, "END COMPONENTS\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintWIRESEG()".
 */

static void  fprintWIRESEG(apPhseg, aFlags)
  struct phseg *apPhseg;
          long  aFlags;
{
  fprintf (DEF_FILE, "%s ", DEF_layer2a (apPhseg->LAYER));

  if (aFlags & F_SEG_WIDTH)
    fprintf (DEF_FILE, "%ld ", MBK2DEF_length (apPhseg->WIDTH));

  if (aFlags & F_SEG_STRIPE)
    fprintf (DEF_FILE, "+ SHAPE STRIPE ");

  fprintf (DEF_FILE, "( %ld %ld ) ( %ld %ld ) ",
           MBK2DEF_length (apPhseg->X1),
           MBK2DEF_length (apPhseg->Y1),
           MBK2DEF_length (apPhseg->X2),
           MBK2DEF_length (apPhseg->Y2));

  __DBG(
    fprintf (stderr, "MBK X1 := %ld, DEF X1 := %ld\n",
                     apPhseg->X1, MBK2DEF_length (apPhseg->X1));
  )
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintWIREVIA()".
 */

static void  fprintWIREVIA(apPhvia, aFlags)
  struct phvia *apPhvia;
          long  aFlags;
{
  char  viaName[1024];


  fprintf (DEF_FILE, "%s ", DEF_layer2a (getBottomVIALayer (apPhvia->TYPE)));

  if (aFlags & F_SEG_WIDTH)
    fprintf (DEF_FILE, "%ld ", MBK2DEF_length (apPhvia->DX));

  fprintf (DEF_FILE, "( %ld %ld ) %s ",
           MBK2DEF_length (apPhvia->XVIA),
           MBK2DEF_length (apPhvia->YVIA),
           DEF_via2a (apPhvia->TYPE, apPhvia->DX, apPhvia->DY, viaName));
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintWIRES()".
 */

static void  fprintWIRES(apLosig)
  struct losig *apLosig;
{
  struct eLoseg_s *pLoseg;
             long  flagFirst, flagSeg;


  flagFirst = TRUE;
  pLoseg    = getloseglist (LV_phfig, getsigname (apLosig));

  flagSeg = 0L;
  if (LV_localFlags & F_SPECIAL_NET) {
    flagSeg = F_SEG_STRIPE | F_SEG_WIDTH;
  }


  if (pLoseg) {
    for (; pLoseg != NULL; pLoseg = pLoseg->next) {
      if (!flagFirst)
        fprintf (DEF_FILE, "\n        NEW ");

      switch (pLoseg->type) {
        case LOSEG_SEG:
          if (flagFirst) fprintf (DEF_FILE, "\n      + ROUTED ");
          fprintWIRESEG ((struct phseg*)pLoseg->MBKobj, flagSeg);
          flagFirst = FALSE;
          break;
        case LOSEG_VIA:
          if (flagFirst) fprintf (DEF_FILE, "\n      + ROUTED ");
          fprintWIREVIA ((struct phvia*)pLoseg->MBKobj, flagSeg);
          flagFirst = FALSE;
          break;
      }

    }
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintNET()".
 */

static void  fprintNET(apLoSig, aUse)
  losig_list *apLoSig;
        char  aUse;
{
  struct    ptype *pType;
  struct    chain *pChain;
  struct    locon *pLoCon;
  struct authelem *pElem;
             char  sLine[80], sItem[1024];
             char *sSig, sSigLEF[1024], sConLEF[1024];
             long  lLine;


  /* Try to guess if this is a VST devectorized internal signal.
   * We first seach a loseg with the signal name, if nothing is
   * found we try with a vectorized name.
   */
  sSig  = getsigname (apLoSig);
  pElem = searchauthelem (LV_htTerm, sSig);

  /* In case of external signal, override with the terminal name. */
  if (pElem != NULL) { sSig = (char*)pElem->VALUE; }

  pType = getptype(apLoSig->USER, (long)LOFIGCHAIN);
  if (pType == (ptype_list*)NULL) {
    wprinth((char*)NULL);
    wprintf(" Signal \"%s\" have no LOFIGCHAIN.\n", sSig);

    pChain = (chain_list*)NULL;
  } else {
    pChain = (chain_list*)(pType->DATA);
  }


  sSig = MBK2DEF_name(sSigLEF, sSig);
/*snprintf(sItem, 79, "    - %s ", sSig);*/
  sprintf(sItem, "    - %s ", sSig);

  sLine[0] = (char)0; lLine = 0L;
  if (strlen (sItem) > 79)
    fprintf(DEF_FILE, "%s\n", sItem);
  else
    strLCat(sLine, sItem, &lLine, 79);



  for(; pChain != (chain_list*)NULL;
        pChain  = pChain->NEXT) {
    pLoCon = (locon_list*)(pChain->DATA);

    if (pLoCon->TYPE == INTERNAL) {
    /*snprintf(sItem, 79, "( %s %s ) ",
     *        ((loins_list*)(pLoCon->ROOT))->INSNAME,
     *        MBK2DEF_name(sConLEF, pLoCon->NAME));
     */
      sprintf(sItem, "( %s %s ) ",
             ((loins_list*)(pLoCon->ROOT))->INSNAME,
             MBK2DEF_name(sConLEF, pLoCon->NAME));

      if (lLine + strlen(sItem) + 4 > 70) {
        fprintf(DEF_FILE, "%s", sLine);
        if (strlen (sItem) > 70) {
          /* sItem too long anyway... flush it immediatly. */
          fprintf(DEF_FILE, " %s\n", sItem);
          sItem[0] = (char)0;
        } else {
          fprintf(DEF_FILE, "\n");
        }
        lLine = 0L;
        strLCat(sLine, "        ", &lLine, 79);
      }
      strLCat(sLine, sItem, &lLine, 79);
    }
  }
  if (lLine > 0) fprintf(DEF_FILE, "%s", sLine);

  if (LV_phfig) fprintWIRES (apLoSig);

  if (aUse != C_USE_NONE)
    fprintf(DEF_FILE, "\n      + USE %s ;\n", USEtoa(aUse));
  else
    fprintf(DEF_FILE, ";\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintNETS()".
 */

static void  fprintNETS()
{
  losig_list *pLoSig;
        long  loSigNB;
        char *sSig;


  LV_localFlags &= ~ F_SPECIAL_NET;
  LV_localFlags |=   F_REGULAR_NET;


  /* First : compute the number of signals. */
  for(pLoSig = LV_lofig->LOSIG, loSigNB = 0L;
      pLoSig != (losig_list*)NULL; pLoSig = pLoSig->NEXT) {
    sSig = getsigname(pLoSig);
    if (isvdd(sSig) || isvss(sSig)) continue;

    loSigNB += 1;
  }

  fprintf(DEF_FILE, "\n\nNETS %ld ;\n", loSigNB);

  /* Dump the list of signal connections. */
  for(pLoSig  = LV_lofig->LOSIG;
      pLoSig != (losig_list*)NULL; pLoSig  = pLoSig->NEXT) {

    sSig = getsigname(pLoSig);
    if (     isvdd(sSig) 
        ||   isvss(sSig)
        || !strcmp(sSig, "cki")) continue;

    fprintNET(pLoSig, C_USE_NONE);
  }

  fprintf(DEF_FILE, "END NETS\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintSPECIALNETS()".
 */

static void  fprintSPECIALNETS()
{
  losig_list *pLoSig;
        long  loSigNB;
        char *sSig, Use;


  LV_localFlags &= ~ F_REGULAR_NET;
  LV_localFlags |=   F_SPECIAL_NET;


  /* First : compute the number of signals. */
  for(pLoSig = LV_lofig->LOSIG, loSigNB = 0L;
      pLoSig != (losig_list*)NULL; pLoSig = pLoSig->NEXT) {
    sSig = getsigname(pLoSig);
    if (    !isvdd(sSig)
        &&  !isvss(sSig)
        &&  strcmp(sSig, "cki")) continue;

    loSigNB += 1;
  }

  fprintf(DEF_FILE, "\n\nSPECIALNETS %ld ;\n",
          loSigNB + ((LV_blockage) ? 1 : 0));

  /* Dump the list of signal connections. */
  for(pLoSig  = LV_lofig->LOSIG;
      pLoSig != (losig_list*)NULL; pLoSig  = pLoSig->NEXT) {

    sSig = getsigname(pLoSig);

    Use = C_USE_NONE;
    if (  isvdd(sSig)       ) Use = C_USE_POWER;
    if (  isvss(sSig)       ) Use = C_USE_GROUND;
    if (!strcmp(sSig, "cki")) Use = C_USE_CLOCK;
    if (Use == C_USE_NONE) continue;

    fprintNET(pLoSig, Use);
  }

  fprintBLOCKAGE ();

  fprintf(DEF_FILE, "END SPECIALNETS\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintDEF()".
 */

static void  fprintDEF()
{
  fprintf (DEF_FILE, "DESIGN %s ;\n\n\n"                   , LV_name);
  fprintf (DEF_FILE, "    TECHNOLOGY %s ;\n\n"             , "cmos_12");
  fprintf (DEF_FILE, "    UNITS DISTANCE MICRONS %ld ;\n", DEF_UNITS_MICRONS);

  if (LV_phfig) {
    fprintDIEAREA ();

    ref2ROW ();
    if (!LV_row) {
      wprinth ("fprintfDEF");
      wprintf ("\n  Generate default rows and tracks. This may lead to");
      wprintf ("\n  incorrect results if the design is expanded.\n");

      buildROWS (LV_phfig);
      buildTRACKS (LV_phfig, LV_Flags);
    } else {
      ref2TRACK ();
    }

    fprintROWS ();
    fprintTRACKS ();

    ref2BLOCKAGE();

    fprintVIAS ();
  }


  fprintCOMPONENTS ();
  fprintPINS ();
  fprintNETS ();

  fprintSPECIALNETS ();

  fprintf(DEF_FILE, "\n\nEND DESIGN\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "defsavelophfig()".
*/

extern void  defsavelophfig(apLofig, apPhfig, aDefName, aFlags)
  struct lofig *apLofig;
  struct phfig *apPhfig;
          char *aDefName;
          long  aFlags;
{
  LV_Flags    = aFlags;
  LV_lofig    = apLofig;
  LV_phfig    = apPhfig;
  LV_name     = aDefName;
  LV_row      = NULL;
  LV_track    = NULL;
  LV_blockage = NULL;

  if (LV_phfig) {
    addfeed (LV_lofig, LV_phfig);
    lofigchain (LV_lofig);
  }

  buildphinslinks ();
  buildtterm ();

  if (!LV_name) LV_name = apLofig->NAME;
  DEF_FILE = mbkfopen (LV_name, "def", "w+");

  fprintDEF ();
  if (LV_phfig) checklosegaccess (LV_phfig);

  fclose (DEF_FILE);
  freerow ();
  freetrack ();
  freeblockage ();

  freetterm ();
  freephinslinks ();

  if (LV_phfig) {
    delfeed (LV_lofig);
    lofigchain (LV_lofig);
  }
}
