/*
 *  $Id: util_Floorplan.c,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module    :         "./util_Floorplan.c"                    |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "debugoff.h"


/*  ------------------------------------------------------------------
 *  Local constants.
 */

# define        F_NO_OVERLAP        ((long)(1<<0))
# define        F_OVERLAP           ((long)(1<<1))
# define        F_I1_IN_I2          ((long)(1<<2))
# define        F_I2_IN_I1          ((long)(1<<3))
# define        F_SEG_EXCLUS        ((long)(1<<0))
# define        F_SEG_INCLUS        ((long)(1<<1))
# define        F_SEG_HALFOVER      ((long)(1<<2))
# define        F_SEG_OVER          ((long)(1<<3))
# define        F_TERM_XY           ((long)(1<<0))
# define        F_TERM_LAYER        ((long)(1<<1))
# define        F_TERM_SIDE         ((long)(1<<2))
# define        F_TERM_USER         ((long)(1<<3))

# define        F_SIDE_FULL         ((long)(1<<1))

# define        SIDE_NORTH          0
# define        SIDE_SOUTH          1
# define        SIDE_EAST           2
# define        SIDE_WEST           3
# define        SIDE_ALL            4


/*  ------------------------------------------------------------------
 *  Local structures.
 */

  typedef struct ulocon_s {
    long  flags;
  } ulocon_t;

  typedef struct term_s {
             long  flags;
             char *name;
             char  layer;
             long  XY;
    struct term_s *next;
  } term_t;


  typedef struct side_s {
           char    orient;
           char    layer;
           long    flags;
           long    x;
           long    y;
           long    length;
           long    nbTerm;
           long    maxTerm;
    struct term_s *lTerm;
  } side_t;


/*  ------------------------------------------------------------------
 *  Local variables.
 */

  static         char  routingLayers3[256] = "l_alu1.l_alu2.l_alu3.";
  static         char  routingLayers4[256] =
                        "l_alu1.l_alu2.l_alu3.l_alu4.";
  static         char  routingLayers6[256] =
                        "l_alu1.l_alu2.l_alu3.l_alu4.l_alu5.l_alu6.";
  static         long  LV_dupTerm;
  static         long  LV_loTerm;


/*  ------------------------------------------------------------------
 *  Local macros.
 */

# define    SNAPGRID(x)  ((x) - ((x) % MBK_X_GRID))


/*  ----------------------------------------------------------------------
 *  Local functions declarations.
 */

  static struct term_s   *newTerm         __FP((struct term_s *apHead,
                                                         long  aFlags,
                                                         char *aName,
                                                         char  aLayer,
                                                         long  aXY));
  static struct term_s   *freeTerm        __FP((struct term_s *apTerm));
  static struct ulocon_s *getUlocon       __FP((struct locon *apLocon));
  static struct ulocon_s *newUlocon       __FP((struct locon *apLocon));
  static void             freeUlocon      __FP((struct locon *apLocon));
  static void             linkTerm        __FP((struct locon *apLocon,
                                                        long  aFlags));
  static void             unlinkTerm      __FP((struct locon *apLocon));
  static struct side_s   *newSide         __FP((char aOrient,
                                                char aLayer,
                                                long aX,
                                                long aY,
                                                long aLength));
  static void             freeSide        __FP((struct side_s *apSide));
  static struct side_s   *buildSide       __FP((struct phfig *apPhfig,
                                                struct lofig *apLofig,
                                                        char  aOrient,
                                                        char  aLayer));
  static            long  countDupTerms   __FP((struct phfig *apPhfig));
  static            long  addLoconUinfo   __FP((struct lofig *apLofig));
  static            void  freeLoconUinfo  __FP((struct lofig *apLofig));
  static            long  computeArea     __FP((struct lofig *apLofig));
# if 0
  static            long  intervalRel     __FP((long  aI1_min,
                                                long  aI1_max,
                                                long  aI2_min,
                                                long  aI2_max));
  static            long  segInArea       __FP((struct phseg *aPhseg,
                                                        long  aX1,
                                                        long  aY1,
                                                        long  aX2,
                                                        long  aY2));
# endif


/*
 *  /--------------------------------------------------------------------\
 *  |                     Functions Definitions                          |
 *  \--------------------------------------------------------------------/
 */

/*  ----------------------------------------------------------------------
 *  Function  :  "newTerm()".
 */

static struct term_s *newTerm (apHead, aFlags, aName, aLayer, aXY)
  struct term_s *apHead;
           long  aFlags;
           char *aName;
           char  aLayer;
           long  aXY;
{
  struct term_s *pTerm;


  pTerm = (struct term_s*)mbkalloc (sizeof (struct term_s));

  pTerm->flags = aFlags;
  pTerm->name  = aName;
  pTerm->layer = aLayer;
  pTerm->XY    = aXY;
  pTerm->next  = apHead;

  return (pTerm);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "freeTerm()".
 */

static struct term_s *freeTerm (apTerm)
  struct term_s *apTerm;
{
  struct term_s *pNext;


  pNext = apTerm->next;

  mbkfree (apTerm);

  return (pNext);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "getUlocon()".
 */

static struct ulocon_s *getUlocon (apLocon)
  struct locon *apLocon;
{
  struct ptype    *pType;


  if (!(pType = getptype (apLocon->USER, PTYPE_ULOCON))) return (NULL);

  return ((struct ulocon_s*)pType->DATA);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "newUlocon()".
 */

static struct ulocon_s *newUlocon (apLocon)
  struct locon *apLocon;
{
  struct ptype    *pType;
  struct ulocon_s *pUlocon;


  if (!(pUlocon = getUlocon (apLocon))) {
    pUlocon = (struct ulocon_s*)mbkalloc (sizeof (struct ulocon_s));

    pUlocon->flags = 0L;

    apLocon->USER = addptype (apLocon->USER, PTYPE_ULOCON, (void*)pUlocon);

    pType = apLocon->USER;
  }


  return (pUlocon);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "freeUlocon()".
 */

static void  freeUlocon (apLocon)
  struct locon *apLocon;
{
  struct ptype *pType;


  if ((pType = getptype (apLocon->USER, PTYPE_ULOCON))) {
    mbkfree (pType->DATA);

    apLocon->USER = delptype (apLocon->USER, PTYPE_ULOCON);
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "linkTerm()".
 */

static void  linkTerm (apLocon, aFlags)
  struct locon  *apLocon;
          long   aFlags;
{
  struct ulocon_s *pUlocon;


  if (!(pUlocon = getUlocon (apLocon)))
    pUlocon = newUlocon (apLocon);

  pUlocon->flags = aFlags;
}


/*  ----------------------------------------------------------------------
 *  Function  :  "unlinkTerm()".
 */

static void  unlinkTerm (apLocon)
  struct locon *apLocon;
{
  freeUlocon (apLocon);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "newSide()".
 */

static struct side_s *newSide (aOrient, aLayer, aX, aY, aLength)
  char  aOrient;
  char  aLayer;
  long  aX;
  long  aY;
  long  aLength;
{
  struct side_s *pSide;


  pSide = (struct side_s*)mbkalloc (sizeof (struct side_s));

  pSide->nbTerm   = 0;
  pSide->maxTerm  = 0;
  pSide->flags    = 0;
  pSide->lTerm    = NULL;
  pSide->orient   = aOrient;
  pSide->layer    = aLayer;
  pSide->x        = aX;
  pSide->y        = aY;
  pSide->length   = aLength;

  return (pSide);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "freeSide()".
 */

static void  freeSide (apSide)
  struct side_s *apSide;
{
  struct term_s *pTerm;


  for (pTerm = apSide->lTerm; pTerm != NULL; pTerm = freeTerm (pTerm));

  mbkfree (apSide);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "buildSide()".
 */

static struct side_s *buildSide (apPhfig, apLofig, aOrient, aLayer)
  struct phfig *apPhfig;
  struct lofig *apLofig;
          char  aOrient;
          char  aLayer;
{
  struct side_s *pSide;
  struct  phcon *pPhcon;
           long  XSIDE, YSIDE, sideLength;


  switch (aOrient) {
    case WEST:
      XSIDE      = apPhfig->XAB1;
      YSIDE      = apPhfig->YAB1;
      sideLength = apPhfig->YAB2 - apPhfig->YAB1;
      break;
    case EAST:
      XSIDE      = apPhfig->XAB2;
      YSIDE      = apPhfig->YAB1;
      sideLength = apPhfig->YAB2 - apPhfig->YAB1;
      break;
    case NORTH:
      XSIDE      = apPhfig->XAB1;
      YSIDE      = apPhfig->YAB2;
      sideLength = apPhfig->XAB2 - apPhfig->XAB1;
      break;
    default:
    case SOUTH:
      XSIDE      = apPhfig->XAB1;
      YSIDE      = apPhfig->YAB1;
      sideLength = apPhfig->XAB2 - apPhfig->XAB1;
      break;
  }


  pSide = newSide (aOrient, aLayer, XSIDE, YSIDE, sideLength);

  for (pPhcon = apPhfig->PHCON; pPhcon != NULL; pPhcon = pPhcon->NEXT) {
    /* Checking terminal orientation. */
    if (pPhcon->ORIENT == aOrient) {
      switch (aOrient) {
        case EAST:
        case WEST:
          if (XSIDE != pPhcon->XCON) {
            eprinth ("buildSide");
            eprintf ("\n  East/west terminal %s at (%ld,%ld) is not on",
                     pPhcon->NAME,
                     pPhcon->XCON,
                     pPhcon->YCON);
            eprintf ("\n  the abutment-box.\n");
            EXIT (1);
          }

          /* Adding the terminal to the side's list. */
          pSide->nbTerm++;
          pSide->lTerm = newTerm (pSide->lTerm,
                                  F_TERM_XY|F_TERM_LAYER|F_TERM_USER,
                                  pPhcon->NAME,
                                  pPhcon->LAYER,
                                  pPhcon->YCON);
          break;
        case NORTH:
        default:
        case SOUTH:
          if (YSIDE != pPhcon->YCON) {
            eprinth ("buildSide");
            eprintf ("\n  North/south terminal %s at (%ld,%ld) is not on",
                     pPhcon->NAME,
                     pPhcon->XCON,
                     pPhcon->YCON);
            eprintf ("\n  the abutment-box.\n");
            EXIT (1);
          }

          /* Adding the terminal to the side's list. */
          pSide->nbTerm++;
          pSide->lTerm = newTerm (pSide->lTerm,
                                  F_TERM_XY|F_TERM_LAYER|F_TERM_USER,
                                  pPhcon->NAME,
                                  pPhcon->LAYER,
                                  pPhcon->XCON);
          break;
      }

      linkTerm (getlocon (apLofig, pPhcon->NAME),
                F_TERM_XY|F_TERM_LAYER|F_TERM_SIDE);
    }
  }

  return (pSide);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "sortSide()".
 */

static void  sortSide(apSide)
  struct side_s *apSide;
{
  struct term_s *pTerm;
           char *tFilled;
           long  i, spacing, sideOrig;


  tFilled = (char*)mbkalloc (sizeof (char) * apSide->nbTerm);

  switch (apSide->orient) {
    case EAST:
    case WEST:
      sideOrig = apSide->y; break;
    case NORTH:
    case SOUTH:
    default:
      sideOrig = apSide->x; break;
  }

  spacing = apSide->length / apSide->nbTerm;
  for (i = 0; i < apSide->nbTerm; i++) tFilled[i] = '\0';

  for (pTerm = apSide->lTerm; pTerm != NULL; pTerm = pTerm->next) {
    if (pTerm->flags & F_TERM_XY) {
      tFilled[(pTerm->XY - sideOrig) / spacing ] = '\1';
    }
  }

  __DBG( fprintf (stderr, "spacing := %ld\n", spacing); )

  i = 0;
  for (pTerm = apSide->lTerm; pTerm != NULL; pTerm = pTerm->next) {
    __DBG( fprintf (stderr, "i := %ld\n", i); )

    if (!(pTerm->flags & F_TERM_XY)) {
      pTerm->XY    = sideOrig + SNAPGRID((spacing * i) + (spacing >> 1));
      pTerm->layer = apSide->layer;

      __DBG( fprintf (stderr, "pTerm->XY := %ld\n", pTerm->XY); )
    }

    i++;
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "countDupTerms()".
 */

static long  countDupTerms (apPhfig)
  struct phfig *apPhfig;
{
  struct     phcon *pPhcon;
  struct authtable *htCon;
  struct  authelem *pElem;


  htCon = createauthtable (1024);

  /* Find the number of duplicated terminals. */
  for (pPhcon = apPhfig->PHCON; pPhcon != NULL; pPhcon = pPhcon->NEXT) {
    pElem = searchauthelem (htCon, pPhcon->NAME);

    if (!pElem) {
      pElem = addauthelem (htCon, pPhcon->NAME, -1);
    }

    pElem->VALUE++; 
  }


  LV_dupTerm = 0;

  /* Make the sum. */
  for (pPhcon = apPhfig->PHCON; pPhcon != NULL; pPhcon = pPhcon->NEXT) {
    pElem = searchauthelem (htCon, pPhcon->NAME);

    LV_dupTerm += pElem->VALUE; 

    /* Do not count twice duplicated terminals... */
    pElem->VALUE = 0;
  }

  destroyauthtable (htCon);


  return (LV_dupTerm);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "addLoconUinfo()".
 */

static long  addLoconUinfo (apLofig)
  struct lofig *apLofig;
{
  struct     locon *pLocon;


  LV_loTerm = 0;


  for (pLocon = apLofig->LOCON; pLocon != NULL; pLocon = pLocon->NEXT) {
    LV_loTerm++;

    linkTerm (pLocon, 0);
  }

  return (LV_loTerm);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "freeLoconUinfo()".
 */

static void  freeLoconUinfo (apLofig)
  struct lofig *apLofig;
{
  struct     locon *pLocon;


  for (pLocon = apLofig->LOCON; pLocon != NULL; pLocon = pLocon->NEXT) {
    unlinkTerm (pLocon);
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "nextFreeLocon()".
 */

static struct locon *nextFreeLocon (apLocon)
  struct locon *apLocon;
{
  struct ulocon_s *pUlocon;
  struct locon    *pLocon;


  for (pLocon = apLocon; pLocon != NULL; pLocon = pLocon->NEXT) {
    pUlocon = getUlocon (pLocon);

    if (!(pUlocon->flags & F_TERM_SIDE)) return (pLocon);
  }

  return (NULL);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "placeTerms()".
 */

extern void  placeTerms (apLofig, apPhfig)
  struct lofig *apLofig;
  struct phfig *apPhfig;
{
  struct side_s *tSide[4];
  struct term_s *pTerm;
  struct locon  *pLocon;
           long  iSide;
           long  spaceTerm, newSpaceTerm, sideSpaceTerm;
           long  nbPhterm , newNbPhterm;
           long  nbPlacedTerm;
           long  flagLoop;
           long  XCON, YCON;


  addLoconUinfo (apLofig);
  countDupTerms (apPhfig);

  tSide[SIDE_NORTH] = buildSide (apPhfig, apLofig, NORTH, ALU3);
  tSide[SIDE_SOUTH] = buildSide (apPhfig, apLofig, SOUTH, ALU3);
  tSide[SIDE_EAST]  = buildSide (apPhfig, apLofig, EAST , ALU2);
  tSide[SIDE_WEST]  = buildSide (apPhfig, apLofig, WEST , ALU2);


  /* Side allocation for free terminals. */

  nbPhterm     = LV_loTerm + LV_dupTerm;
  spaceTerm    = 0;
  nbPlacedTerm = 0;

  for (iSide = 0; iSide < SIDE_ALL; iSide++) {
    spaceTerm    += tSide[iSide]->length;
    nbPlacedTerm += tSide[iSide]->nbTerm;
  }

  spaceTerm /= (nbPhterm + 1);

  __DBG( fprintf (stderr, "spaceTerm := %ld\n", spaceTerm); )
  __DBG( fprintf (stderr, "nbPhterm  := %ld\n", nbPhterm ); )


  if (nbPlacedTerm < nbPhterm) {
    /* Find sides with free space for terminals. */
    for (flagLoop = TRUE; flagLoop; ) {
      flagLoop = FALSE;

      newSpaceTerm = 0;
      newNbPhterm  = LV_loTerm + LV_dupTerm;

      for (iSide = 0; iSide < SIDE_ALL; iSide++) {
        if (tSide[iSide]->flags & F_SIDE_FULL) {
          newNbPhterm -= tSide[iSide]->nbTerm;
        } else {

          sideSpaceTerm  = tSide[iSide]->length / (tSide[iSide]->nbTerm + 1);
          __DBG( fprintf (stderr, "sideSpaceTerm := %ld\n", sideSpaceTerm); )

          if (sideSpaceTerm <= spaceTerm) {
            __DBG( fprintf (stderr, "side %ld overloaded\n", iSide); )
            /* This side is overloaded : skip it. */
            flagLoop = TRUE;

            tSide[iSide]->flags   |= F_SIDE_FULL;
            tSide[iSide]->maxTerm  = tSide[iSide]->nbTerm;
            newNbPhterm -= tSide[iSide]->nbTerm;
          } else {
            newSpaceTerm  += sideSpaceTerm;
            tSide[iSide]->maxTerm  = tSide[iSide]->length / spaceTerm;
            __DBG( fprintf (stderr,
                            "tSide[%ld]->maxTerm := %ld\n",
                            iSide,
                            tSide[iSide]->maxTerm); )
          }
        }
      }  /* End of 'for(iSide ...)'. */

      spaceTerm = newSpaceTerm / (newNbPhterm + 1);

      __DBG( fprintf (stderr, "spaceTerm := %ld\n", spaceTerm); )
    }  /* End of 'for(flagLoop... )'. */
  }


  /* Dispatch remaining terminals on non-overloaded sides. */
  iSide = 0;

  for (pLocon = nextFreeLocon (apLofig->LOCON);
       pLocon != NULL; pLocon = nextFreeLocon (pLocon->NEXT)) {
    while (tSide[iSide]->nbTerm >= tSide[iSide]->maxTerm) {
      if (iSide >= SIDE_ALL) {
        eprinth ("placeTerms");
        eprintf ("\n  No more sides to place terminals");
        eprintf (" (this is a bug)\n\n");
        EXIT (1);
      }

      iSide++;
    }

    tSide[iSide]->nbTerm++;
    tSide[iSide]->lTerm = newTerm (tSide[iSide]->lTerm,
                                   0,
                                   pLocon->NAME,
                                   0,
                                   0);

    linkTerm (pLocon, F_TERM_SIDE);
  }


  /* Sets the positions on all sides. */
  for (iSide = 0; iSide < SIDE_ALL; iSide++) {
    __DBG (fprintf (stderr, "sortSide(%ld)\n", iSide); )
    sortSide (tSide[iSide]);
  }


  /* Add the physical terminals. */
  for (iSide = 0; iSide < SIDE_ALL; iSide++) {
    for (pTerm = tSide[iSide]->lTerm; pTerm != NULL; pTerm = pTerm->next) {
      switch (tSide[iSide]->orient) {
        case NORTH:
        case SOUTH:
        default:
          XCON = pTerm->XY;
          YCON = tSide[iSide]->y;
          break;
        case EAST:
        case WEST:
          XCON = tSide[iSide]->x;
          YCON = pTerm->XY;
          break;
      }
      
      addphcon (apPhfig,
                tSide[iSide]->orient,
                pTerm->name,
                XCON,
                YCON,
                pTerm->layer,
                MBKSCALE(2));
    }
  }



  /* Free all the sides structures. */
  for (iSide = 0; iSide < SIDE_ALL; iSide++) freeSide (tSide[iSide]);

  freeLoconUinfo (apLofig);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "computeArea()".
 */

static long  computeArea(apLofig)
  struct lofig *apLofig;
{
  struct loins* pLoins;
  struct phfig* pModel;
          long  area;
          long  AB_width, AB_height;


  area = 0L;

  for (pLoins = apLofig->LOINS; pLoins != NULL; pLoins = pLoins->NEXT) {
    pModel = getphfig (pLoins->FIGNAME, 'P');

    AB_width  = pModel->XAB2 - pModel->XAB1;
    AB_height = pModel->YAB2 - pModel->YAB1;


    if (AB_width <= 0) {
      eprinth (NULL);
      eprintf (
        "Negative or null width (%ld) for abutment box of \"%s\".\n",
        AB_width, pModel->NAME);
      EXIT (1);
    }

    if (AB_height <= 0) {
      eprinth (NULL);
      eprintf (
        "Negative or null height (%ld) for abutment box of \"%s\".\n",
        AB_height, pModel->NAME);
      EXIT (1);
    }

    if (AB_width % MBK_X_GRID) {
      eprinth (NULL);
      eprintf (
        "Unpitched width (%ld) for abutment box of \"%s\".\n",
        AB_width, pModel->NAME);
      EXIT (1);
    }

    if (AB_height % MBK_Y_SLICE) {
      eprinth (NULL);
      eprintf (
        "Unpitched height (%ld) for abutment box of \"%s\".\n",
        AB_height, pModel->NAME);
      EXIT (1);
    }


    AB_width  = AB_width  / MBK_X_GRID;
    AB_height = AB_height / MBK_Y_SLICE;

    area += AB_width * AB_height;
  }


  return (area);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "makeFloorplan()".
 */

extern struct phfig *makeFloorplan(apLofig,
                                   aMargin,
                                   aXSize,
                                   aYSize,
                                   aAspectRatio,
                                   aPower,
                                   aFlags)
  struct lofig *apLofig;
          long  aMargin;
          long  aXSize;
          long  aYSize;
        double  aAspectRatio;
          long  aPower;
          long  aFlags;
{
  struct phfig *pPhfig;
          long  specCount, i;
          long  area;
          long  xSize, ySize;
          long  XAB1, YAB1, XAB2, YAB2;
          char  row_name[1024], row_type[1024];
          char *routingLayers;


  specCount  = (aFlags & F_FLOOR_X_SIZE) ? 1 : 0;
  specCount += (aFlags & F_FLOOR_Y_SIZE) ? 1 : 0;
  specCount += (aFlags & F_FLOOR_ASPECT_RATIO) ? 1 : 0;

  if (specCount != 1) {
    errMBK  ("makeFloorplan");
    eprintf ("\n  One, and only one argument among aXSize, aYSize and");
    eprintf (   " aAspectRatio must\n   be non zero.\n");
    EXIT (1);
  }

  pPhfig = addphfig (apLofig->NAME);

  area = computeArea (apLofig);
  area = (long)((1.0 + (double)aMargin / 100.0) * (double)area);

  /* Shut up GCC. */
  xSize = 0;
  ySize = 0;

  if (aFlags & F_FLOOR_X_SIZE) {
    xSize = aXSize;
    ySize =    area / (xSize - 7 * aPower)
           + ((area % (xSize - 7 * aPower)) ? 1 : 0);
  }

  if (aFlags & F_FLOOR_Y_SIZE) {
    ySize = aYSize;
    xSize = (7 * aPower) + (area / aYSize) + ((area / aYSize) ? 1 : 0);
  }

  if (aFlags & F_FLOOR_ASPECT_RATIO) {
    double  x, r, b, xy;

    xy = (double)(Y_SLICE / X_GRID);

    b = ((double)(7 * aPower)) / xy;
    x = (b + sqrt (pow (b, 2.0) + 4 * xy * (double)(area) * aAspectRatio))
      / (2 * xy * aAspectRatio);

    ySize = (long)floor (x * aAspectRatio);

    /* Compute missing area (as ySize is rounded down).
     * The unit the number of horizontal sites.
     */ 
    r = ((double)(area + 7 * aPower * ySize) - pow ((double)ySize, 2.0) * xy)
      /  (double)(ySize);

    xSize = ((long)ceil ((double)ySize * xy + r));
  }


  XAB1 = 0L;
  YAB1 = 0L;
  XAB2 = xSize * MBK_X_GRID;
  YAB2 = ySize * MBK_Y_SLICE;


  /* Build the floorplan. */

  /* The Abutment-box. */
  defab (pPhfig, XAB1, YAB1, XAB2, YAB2);

  /* The row matrix. */
  strcpy (row_type, "core");
  for (i = 0; i < ySize; i++) {
    sprintf (row_name, "row_%ld", i);

    DEF2MBK_row (pPhfig,
                 row_name,
                 row_type,
                 (i % 2) ? DEF_FS : DEF_N,
                 xSize,
                 1L,
                 DEF_X_GRID,
                 DEF_Y_SLICE,
                 XAB1,
                 YAB1 + i * MBK_Y_SLICE);
  }

  /* The tracks matrix. */
  routingLayers = routingLayers6;

  if (aFlags & F_FLOOR_LAYERS_3) {
    routingLayers = routingLayers3;
  }

  if (aFlags & F_FLOOR_LAYERS_4) {
    routingLayers = routingLayers4;
  }

  /* Horizontal tracks. */
  DEF2MBK_track (pPhfig,
                 "Y",
                 MBK2DEF_length (YAB1 + MBK_X_GRID),
                 ySize * (Y_SLICE / X_GRID) - 1,
                 DEF_X_GRID,
                 routingLayers,
                 XAB1,
                 YAB1 + MBK_X_GRID);

  /* Vertical tracks. */
  DEF2MBK_track (pPhfig,
                 "X",
                 MBK2DEF_length (XAB1 + MBK_X_GRID),
                 xSize - 1,
                 DEF_X_GRID,
                 routingLayers,
                 XAB1 + MBK_X_GRID,
                 YAB1);


  return (pPhfig);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "expandFloorplan()".
 */

extern void  expandFloorplan(apPhfig)
  struct phfig *apPhfig;
{
  struct phseg *pPhseg;
  struct phcon *pPhcon;
          long  XAB1, YAB1, XAB2, YAB2;


  XAB1 = apPhfig->XAB1;
  YAB1 = apPhfig->YAB1;
  XAB2 = apPhfig->XAB2;
  YAB2 = apPhfig->YAB2;


  apPhfig->XAB1 -= FLOOR_XY_EXPAND;
  apPhfig->YAB1 -= FLOOR_XY_EXPAND;
  apPhfig->XAB2 += FLOOR_XY_EXPAND;
  apPhfig->YAB2 += FLOOR_XY_EXPAND;


  /* Move terminals to the new AB. */
  for (pPhcon = apPhfig->PHCON; pPhcon != NULL; pPhcon = pPhcon->NEXT) {
    if (pPhcon->XCON == XAB1) pPhcon->XCON = apPhfig->XAB1;
    if (pPhcon->XCON == XAB2) pPhcon->XCON = apPhfig->XAB2;
    if (pPhcon->YCON == YAB1) pPhcon->YCON = apPhfig->YAB1;
    if (pPhcon->YCON == YAB2) pPhcon->YCON = apPhfig->YAB2;
  }


  /* Shrink segments to fit in the new AB. */
  for (pPhseg = apPhfig->PHSEG; pPhseg != NULL; pPhseg = pPhseg->NEXT) {
    if (pPhseg->X1 == XAB1) pPhseg->X1 = apPhfig->XAB1;
    if (pPhseg->X2 == XAB2) pPhseg->X2 = apPhfig->XAB2;
    if (pPhseg->Y1 == YAB1) pPhseg->Y1 = apPhfig->YAB1;
    if (pPhseg->Y2 == YAB2) pPhseg->Y2 = apPhfig->YAB2;
  }


# if 0
  DEF2MBK_blockage (apPhfig,
                    MBK2DEF_length (apPhfig->XAB1),
                    MBK2DEF_length (apPhfig->YAB1),
                    MBK2DEF_length (FLOOR_XY_EXPAND),
                    MBK2DEF_length (FLOOR_XY_EXPAND)
                    );

  DEF2MBK_blockage (apPhfig,
                    MBK2DEF_length (apPhfig->XAB1),
                    MBK2DEF_length (apPhfig->YAB2 - FLOOR_XY_EXPAND),
                    MBK2DEF_length (FLOOR_XY_EXPAND),
                    MBK2DEF_length (FLOOR_XY_EXPAND)
                    );

  DEF2MBK_blockage (apPhfig,
                    MBK2DEF_length (apPhfig->XAB2 - FLOOR_XY_EXPAND),
                    MBK2DEF_length (apPhfig->YAB1),
                    MBK2DEF_length (FLOOR_XY_EXPAND),
                    MBK2DEF_length (FLOOR_XY_EXPAND)
                    );

  DEF2MBK_blockage (apPhfig,
                    MBK2DEF_length (apPhfig->XAB2 - FLOOR_XY_EXPAND),
                    MBK2DEF_length (apPhfig->YAB2 - FLOOR_XY_EXPAND),
                    MBK2DEF_length (FLOOR_XY_EXPAND),
                    MBK2DEF_length (FLOOR_XY_EXPAND)
                    );
# endif
}


# if 0
/*  ----------------------------------------------------------------------
 *  Function  :  "intervalRel()".
 */

static long  intervalRel(aI1_min, aI1_max, aI2_min, aI2_max)
  long  aI1_min, aI1_max, aI2_min, aI2_max;
{
  long  flag;


  if (aI1_min > aI1_max) {
    errMBK  ("intervalRel");
    eprintf (" I1_min(%ld) > I1_max(%ld) !\n", aI1_min, aI1_max);
    EXIT (1);
  }
  if (aI2_min > aI2_max) {
    errMBK  ("intervalRel");
    eprintf (" I2_min(%ld) > I2_max(%ld) !\n", aI2_min, aI2_max);
    EXIT (1);
  }


  flag = 2;

  if ((aI1_min < aI2_min) || (aI1_min > aI2_max)) flag--;
  if ((aI1_max < aI2_min) || (aI1_max > aI2_max)) flag--;
  if ((aI1_min < aI2_min) && (aI1_max > aI2_max)) flag = 3;


  switch (flag) {
    default:
    case 3: flag = F_I2_IN_I1;   break;
    case 2: flag = F_I1_IN_I2;   break;
    case 1: flag = F_OVERLAP;    break;
    case 0: flag = F_NO_OVERLAP; break;
  }

  return (flag);
}


/*  ----------------------------------------------------------------------
 *  Function  :  "segInArea()".
 */

static long  segInArea(aPhseg, aX1, aY1, aX2, aY2)
  struct phseg *aPhseg;
          long  aX1, aY1, aX2, aY2;
{
  long  flagX, flagY;


  if (aX1 > aX2) {
    errMBK  ("segInArea");
    eprintf ("Bad area X1(%ld) > X2(%ld) !\n", aX1, aX2);
    EXIT (1);
  }
  if (aY1 > aY2) {
    errMBK  ("segInArea");
    eprintf ("Bad area Y1(%ld) > Y2(%ld) !\n", aY1, aY2);
    EXIT (1);
  }


  flagX = intervalRel (aPhseg->X1, aPhseg->X2, aX1, aX2);
  flagY = intervalRel (aPhseg->Y1, aPhseg->Y2, aY1, aY2);


  if ((flagX == F_NO_OVERLAP) || (flagY == F_NO_OVERLAP))
    return (F_SEG_EXCLUS);

  if ((flagX == F_I1_IN_I2) && (flagY == F_I1_IN_I2))
    return (F_SEG_INCLUS);

  if ((flagX == F_I2_IN_I1) && (flagY == F_I2_IN_I1))
    return (F_SEG_HALFOVER);

  return (F_SEG_OVER);
}
# endif


/*  ----------------------------------------------------------------------
 *  Function  :  "shrinkFloorplan()".
 */

extern void  shrinkFloorplan(apPhfig)
  struct phfig *apPhfig;
{
  struct phseg *pPhseg;
  struct phcon *pPhcon;
  struct phref *pPhref;
          long  XAB1, YAB1, XAB2, YAB2;


  XAB1 = apPhfig->XAB1;
  YAB1 = apPhfig->YAB1;
  XAB2 = apPhfig->XAB2;
  YAB2 = apPhfig->YAB2;

  /* Shrink the abutment-box. */
  apPhfig->XAB1 += FLOOR_XY_EXPAND;
  apPhfig->YAB1 += FLOOR_XY_EXPAND;
  apPhfig->XAB2 -= FLOOR_XY_EXPAND;
  apPhfig->YAB2 -= FLOOR_XY_EXPAND;


  /* Move terminals to the new AB. */
  for (pPhcon = apPhfig->PHCON; pPhcon != NULL; pPhcon = pPhcon->NEXT) {
    if (pPhcon->XCON == XAB1) pPhcon->XCON = apPhfig->XAB1;
    if (pPhcon->XCON == XAB2) pPhcon->XCON = apPhfig->XAB2;
    if (pPhcon->YCON == YAB1) pPhcon->YCON = apPhfig->YAB1;
    if (pPhcon->YCON == YAB2) pPhcon->YCON = apPhfig->YAB2;
  }


  /* Shrink segments to fit in the new AB. */
  for (pPhseg = apPhfig->PHSEG; pPhseg != NULL; pPhseg = pPhseg->NEXT) {
    if (pPhseg->X1 == XAB1) pPhseg->X1 = apPhfig->XAB1;
    if (pPhseg->X2 == XAB2) pPhseg->X2 = apPhfig->XAB2;
    if (pPhseg->Y1 == YAB1) pPhseg->Y1 = apPhfig->YAB1;
    if (pPhseg->Y2 == YAB2) pPhseg->Y2 = apPhfig->YAB2;
  }


  /* Move references to the new AB. */
  for (pPhref = apPhfig->PHREF; pPhref != NULL; pPhref = pPhref->NEXT) {
    if (pPhref->XREF == XAB1) pPhref->XREF = apPhfig->XAB1;
    if (pPhref->XREF == XAB2) pPhref->XREF = apPhfig->XAB2;
    if (pPhref->YREF == YAB1) pPhref->YREF = apPhfig->YAB1;
    if (pPhref->YREF == YAB2) pPhref->YREF = apPhfig->YAB2;
  }
}


/*  ----------------------------------------------------------------------
 *  Function  :  "protectPowmid()".
 */

extern void  protectPowmid(apPhfig)
  struct phfig *apPhfig;
{
# if 0
  struct phins *pIns;
# endif
  struct phfig *pModel;
          char *powmidName;
          long  width, height, expand;


  expand = FLOOR_XY_EXPAND;


  powmidName = namealloc ("powmid_x0");

  pModel = getphfig ("powmid_x0", 'A');
  width  = pModel->XAB2 - pModel->XAB1;
  height = pModel->YAB2 - pModel->YAB1;

# if 0
  for (pIns = apPhfig->PHINS; pIns != NULL; pIns = pIns->NEXT) {
    if (pIns->FIGNAME == powmidName) {
      if (pIns->YINS == apPhfig->YAB1 + expand) {
        DEF2MBK_blockage (apPhfig,
                          MBK2DEF_length (pIns->XINS),
                          MBK2DEF_length (apPhfig->YAB1),
                          MBK2DEF_length (width),
                          MBK2DEF_length ((MBK_X_GRID << 1) + expand)
                          );
      }

      if (pIns->YINS + height == apPhfig->YAB2 - expand) {
        DEF2MBK_blockage (apPhfig,
                          MBK2DEF_length (pIns->XINS),
                          MBK2DEF_length (apPhfig->YAB2
                                          - (MBK_X_GRID << 1) - expand),
                          MBK2DEF_length (width),
                          MBK2DEF_length ((MBK_X_GRID << 1) + expand)
                          );
      }
    }
  }
# endif
}
