
/*
 *  $Id: DEF_actions.c,v 1.5 2006/06/28 13:25:06 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./DEF_actions.c"                         |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "debugoff.h"
# include  "util_Defs.h"
# include  "DEF_actions.h"
# include  "DEF_grammar.h"


  /* Flex substitutions. */
# define        yylex              DEF_grammarlex
# define        yyin               DEF_grammarin
# define        yylineno           DEF_grammarlineno

  /* Bison substitutions. */
# define        yyparse            DEF_grammarparse
# define        yydebug            DEF_grammardebug


# define        SIZE_TPT           1024
# define        SIZE_TDATALAYER    16
# define        SIZE_TDATACUT      16
# define        SIZE_TDATAVIA      128
# define        SIZE_TPIN          512
# define        F_DATA_SET         0x00000001
# define        F_PIN_STATE        0x00000001
# define        F_PIN_LAYER        0x00000002
# define        F_PIN_DIRECTION    0x00000004
# define        M_PIN_ISPLACED    (F_PIN_STATE | F_PIN_LAYER)
# define        F_VIA_CUT          0x00000001
# define        F_VIA_PATTERN      0x00000002
# define        F_SEG_INITIAL      0x00000001
# define        F_SEG_SPECIAL      0x00000002
# define        F_SEG_PIN          0x00000004
# define        F_SEG_BLOCKAGE     0x00000008


/*  ------------------------------------------------------------------
 *  Internal types.
 */


  typedef struct dataLayer_s {
    char  layer;  /* MBK layer.                          */
    char  clayer; /* MBK terminal layer.                 */
    char  flags;  /* Tell if this entry is set.          */
    char *name;   /* "nameallocatted" name of the layer. */
    long  width;  /* layer minimal width.                */
  } dataLayer_t;


  typedef struct dataCut_s {
    char  VIA;   /* MBK VIA.                               */
    char  flags; /* Tell if this entry is set    .         */
    char *name;  /* "namellocatted" name of the cut layer. */
  } dataCut_t;


  typedef struct dataVIA_s {
    char  VIA;    /* MBK VIA or TURN_VIA.              */
    char  flags;  /* Tell if this entry is set    .    */
    char *name;   /* "namellocatted" name of the via . */
    long  width;  /* VIA width.                        */
    long  height; /* VIA height.                       */
  } dataVIA_t;


  typedef struct pin_s {
    char *pinName;
    char *netName;
  } pin_t;


/*  ------------------------------------------------------------------
 *  Local variables (prefix 'LV_').
 */

  static                 char *LV_IN_PH   = "def";
  static         struct lofig *LV_pLoFig;
  static         struct phfig *LV_pPhFig;
  static         struct phins *LV_pPhIns;
  static                 char *LV_name;
  static                 char  LV_mode;
  static                 char  LV_defFlags;
  static                 char *LV_blockageName;
  static   struct dataLayer_s  tDataLayer[SIZE_TDATALAYER];
  static   struct   dataCut_s  tDataCut[SIZE_TDATACUT];
  static   struct   dataVIA_s  tDataVIA[SIZE_TDATAVIA];
  static                 long  LV_mVIA    = 0L;
  static                 long  LV_mPin;
  static         struct pin_s *LV_tPin;
  static     struct authtable *LV_htPin;
  static                 char *LV_modelName;
  static                 long  LV_flags;
  static                 long  LV_orient;
  static                 char  LV_layer;
  static                 char  LV_VIA;
  static                 long  LV_width;
  static                 long  LV_height;
  static                 long  LV_lastX;
  static                 long  LV_lastY;
  static            struct pt *LV_ptCurrent;
  static            struct pt *LV_pt1;
  static            struct pt *LV_pt2;
  static                 long  LV_numPins;
  static                 long  LV_numObjects;
  static                 long  LV_numObjectsRead;
  static                 long  LV_direction;
  static                 long  LV_mSig;
  static         struct losig *LV_pLoSig;
  static      struct tLoseg_s *LV_ptLoseg;


/*  ------------------------------------------------------------------
 *  Internal functions.
 */

# define  IS_X_PT(pt)   ((pt)->x != HUGE_VAL)
# define  IS_Y_PT(pt)   ((pt)->y != HUGE_VAL)
# define  IS_XY_PT(pt)  (IS_X_PT(pt) && IS_Y_PT(pt))


  static void        initDataTables  __FP((void));
  static void          setDataLayer  __FP(( int aN,
                                           char *aName,
                                           char  aLayer,
                                           char  aCLayer,
                                           long  aWidth));
  static void            setDataCut  __FP(( int aN,
                                           char *aName,
                                           char  aCut));
  static void            addDataVIA  __FP((char *aName,
                                           char  aCut,
                                           long  width,
                                          long  height));
  static dataVIA_t *DEF2MBK_dataVIA  __FP((char *aName));
  static long           DEF_isLayer  __FP((char *aName));
  static char    DEF2MBK_layer       __FP((char *aName));
  static long    DEF2MBK_layerWidth  __FP((char *aName));
  static long        MBK_layerWidth  __FP((char  aLayer));
  static char    DEF2MBK_dataCut     __FP((char *aName));
  static void            orderCoord  __FP((long *aX1, long *aX2));
  static void         DEF_pinxyflat  __FP((long *apX_flat, long *apY_flat,
                                           long   aX     , long   aY     ,
                                           long   aX_ins , long   aY_ins ,
                                           char   aT));


/*  ------------------------------------------------------------------
 *  Functions from Bison/flex.
 */

  extern FILE *yyin;
  extern long  yylineno;
  extern  int  yydebug;
  extern  int  yyparse __FP((void));



/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "def_design_name()".
 */

extern void	 def_design_name(name)
  char *name;
{
  if (strcmp (name, LV_pPhFig->NAME)) {
    warnMBK ("DEFloadphlofig");
    wprintf ("\n  The design name \"%s\" doesn't match with the", name);
    wprintf (" file name \"%s\" (.%s).\n", LV_pPhFig->NAME, LV_IN_PH);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_tech_name()".
 */

extern void	 def_tech_name(char *name)
{
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_end_design()".
 */

extern void	 def_end_design(void)
{
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_units()".
 */

extern void  def_units(number)
  double  number;
{
  DEF_UNITS_MICRONS = (long)number;
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_die_area()".
 */

extern void	 def_die_area(ab1, ab2)
  struct pt *ab1, *ab2;
{
  if (   !IS_XY_PT (ab1) || !IS_XY_PT (ab2) ) {
    errMBK  ("defloadlophfig");
    eprintf ("Invalid coordinate '*' in DIEAREA at line %ld.\n",
             yylineno);
    EXIT (1);
  }

  if (LV_pPhFig)
    defab(LV_pPhFig, DEF2MBK_length ((long)(ab1->x)),
                     DEF2MBK_length ((long)(ab1->y)),
                     DEF2MBK_length ((long)(ab2->x)),
                     DEF2MBK_length ((long)(ab2->y)));

  def_free_pt (ab1);
  def_free_pt (ab2);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_row_rule()".
 */

extern void	 def_row_rule(row_name, row_type, x, y, orient,
                          do_number, by_number, step_x, step_y)
    char *row_name;
    char *row_type;
  double  x, y, orient, do_number, by_number, step_x, step_y;
{
  if (LV_pPhFig) {
    DEF2MBK_row(LV_pPhFig,
                row_name,
                row_type,
                (long)orient,
                (long)do_number,
                (long)by_number,
                (long)step_x,
                (long)step_y,
                DEF2MBK_length ((long)x),
                DEF2MBK_length ((long)y));
  }


  free (row_name);
  free (row_type);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_track_rule()".
 */

extern void  def_track_rule(axis_name, start, do_number, step, layer_list)
    char *axis_name;
  double  start;
  double  do_number;
  double  step;
    char *layer_list;
{
  long  xREF, yREF;



  if (LV_pPhFig) {
    switch (axis_name[0]) {
      case 'Y':
      case 'y':
        xREF = LV_pPhFig->XAB1;
        yREF = DEF2MBK_length ((long)start);
            break;
      case 'X':
      case 'x':
      default:
        xREF = DEF2MBK_length ((long)start);
        yREF = LV_pPhFig->YAB1;
    }

    DEF2MBK_track(LV_pPhFig,
                  axis_name,
                  (long)start,
                  (long)do_number,
                  (long)step,
                  layer_list,
                  xREF,
                  yREF);
  }


  free (axis_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_track_layers()".
 */

extern char	*def_track_layers(layer_name, layer_list)
  char *layer_name;
  char *layer_list;
{
  char *layerList;


  layerList = s64printf ("%s.%s", layer_name, layer_list);

  free (layer_name);

  return (layerList);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_start_pins()".
 */


extern void	 def_start_pins(number)
  double number;
{
  LV_numObjects     = (long)number;
  LV_numObjectsRead = 0L;
  LV_numPins        = 0L;
  LV_mPin           = LV_numObjects;

  LV_tPin   = (pin_t*)malloc (sizeof (pin_t) * LV_numObjects);

  /* Half empty HT are faster. */
  LV_htPin = createauthtable (LV_numObjects);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_end_pins()".
 */

extern void	 def_end_pins(void)
{
  if (LV_numObjects != LV_numObjectsRead) {
    warnMBK ("DEFloadlophig");
    wprintf ("Invalid pins number : %d, (%d read)\n",
             LV_numObjects, LV_numObjectsRead);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_pin_start()".
 */

extern void	 def_pin_start(char *pin_name, char *net_name)
{
  char *netName;


  netName = DEF2MBK_name (net_name);

  if ((searchauthelem (LV_htPin, netName)) == NULL) {
    addauthelem (LV_htPin, netName, LV_numPins);

    if (LV_numPins >= LV_mPin) {
      LV_mPin += SIZE_TPIN;
      LV_tPin  = (struct pin_s*)realloc (
                   LV_tPin, sizeof (struct pin_s) * LV_mPin);

      if (!LV_tPin) {
        eprinth ("defloadlophfig");
        eprintf ("\n  Not enougth memory to re-allocate \"LV_tPin[]\".\n");
        EXIT (1);
      }
    }

    LV_tPin[LV_numPins].netName = netName;
    LV_tPin[LV_numPins].pinName = DEF2MBK_name (pin_name);

    LV_numPins++;
  }

  LV_flags     = 0L;
  LV_direction = UNKNOWN;

  LV_numObjectsRead++;

  free (pin_name);
  free (net_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_pin_options()".
 */

extern void	 def_pin_options()
{
        char *netName;
        char *pinName;
        char  orientCon;
        long  xCon, yCon, widthCon, lengthCon, fakeCon;
        long  xR1 , yR1 , xR2 , yR2;
        long  xFR1, yFR1, xFR2, yFR2;
  losig_list *pLoSig;
  phseg_list *pPhSeg;
  phcon_list *pPhCon;
    authelem *pElem;


  netName = LV_tPin[LV_numPins - 1].netName;
  pinName = LV_tPin[LV_numPins - 1].pinName;


  /* Process the logical terminal. */

  if (LV_pLoFig) {
    /* The logical terminal/signal pair must be created only once. */
    if (!findlocon (LV_pLoFig, pinName)) {
      pLoSig = addlosig (LV_pLoFig,
                         LV_mSig++,
                         addchain (NULL, (void*)pinName),
                         EXTERNAL);
      addlocon (LV_pLoFig, pinName, pLoSig, LV_direction);
    }
  }


  /* Process the physical terminal, if any. */

  /* Shuts up "gcc -w" ... */
  xR1  = 0;
  yR1  = 0;
  xR2  = 0;
  yR2  = 0;
  xCon = 0;
  yCon = 0;

  if (LV_flags & F_PIN_LAYER) {
    xR1 = DEF2MBK_length ((long)(LV_pt1->x));
    yR1 = DEF2MBK_length ((long)(LV_pt1->y));
    xR2 = DEF2MBK_length ((long)(LV_pt2->x));
    yR2 = DEF2MBK_length ((long)(LV_pt2->y)) - MBKSCALE(1);

    def_free_pt (LV_pt1);
    def_free_pt (LV_pt2);
  }

  if (LV_flags & F_PIN_STATE) {
    xCon = DEF2MBK_length ((long)(LV_ptCurrent->x));
    yCon = DEF2MBK_length ((long)(LV_ptCurrent->y));

    def_free_pt (LV_ptCurrent);
  }


  if ((LV_pPhFig) && ((LV_flags & M_PIN_ISPLACED) == M_PIN_ISPLACED)) {
    pElem = gettlosegitem (LV_ptLoseg, netName); 

    /* Transform (flatten) the PIN segment. */
    DEF_pinxyflat (&xFR1, &yFR1, xR1, yR1, xCon, yCon, LV_orient);
    DEF_pinxyflat (&xFR2, &yFR2, xR2, yR2, xCon, yCon, LV_orient);

    orderCoord (&xFR1, &xFR2);
    orderCoord (&yFR1, &yFR2);


    fakeCon = FALSE;
    pPhSeg  = NULL;   /* Shut up "gcc -w ..." */
    /* if (!(LV_defFlags & F_DEF_NO_IOS)) { */
    /* Add the physical segment. */
    switch (LV_orient) {
      case NOSYM:
      case SYM_X:
      case SYM_Y:
      case SYMXY:
        /* Vertical segment. */
        widthCon  = xFR2 - xFR1;
        lengthCon = yFR2 - yFR1 + MBKSCALE(1);

        if (lengthCon + widthCon < MBK_X_GRID + FLOOR_XY_EXPAND) {
          fakeCon = (LV_defFlags & F_DEF_SHRINK) ? TRUE : FALSE;
          __DBG( fprintf (stderr, "     - Terminal \"%s\" is fake.\n",
                          netName); )
        } else {
          __DBG( fprintf (stderr, "     - Terminal \"%s\" is not fake",
                          netName);
                 fprintf (stderr, " DY := %ld\n", MBKUNSCALE(lengthCon));
          )
          fakeCon = FALSE;
        }

        if (!fakeCon) {
          pPhSeg = addphseg (LV_pPhFig,
                             getCALU (LV_layer),
                             widthCon,
                             xFR1 + widthCon / 2,
                             yFR1,
                             xFR1 + widthCon / 2,
                             yFR2,
                             netName);
        }
        break;
      default:
      case ROT_P:
      case ROT_M:
      case SY_RP:
      case SY_RM:
        /* horizontal segment. */
        widthCon  = yFR2 - yFR1;
        lengthCon = xFR2 - xFR1 + MBKSCALE(1);

        if (lengthCon + widthCon < MBK_X_GRID + FLOOR_XY_EXPAND) {
          fakeCon = (LV_defFlags & F_DEF_SHRINK) ? TRUE : FALSE;
          __DBG( fprintf (stderr, "     - Terminal \"%s\" is fake.\n",
                          netName); )
        } else {
          __DBG( fprintf (stderr, "     - Terminal \"%s\" is not fake",
                          netName);
                 fprintf (stderr, " DX := %ld\n", MBKUNSCALE(lengthCon));
          )
          fakeCon = FALSE;
        }

        if (!fakeCon) {
          pPhSeg = addphseg (LV_pPhFig,
                             getCALU (LV_layer),
                             widthCon,
                             xFR1,
                             yFR1 + widthCon / 2,
                             xFR2,
                             yFR1 + widthCon / 2,
                             netName);
        }
        break;
    } /* End of "switch (LV_orient)". */

    if (!fakeCon || (LV_defFlags & F_DEF_KEEP_IOS)) {
      if (!fakeCon)
        LV_ptLoseg->tLoseg[pElem->VALUE] = addloseg (
          LV_ptLoseg->tLoseg[pElem->VALUE], LOSEG_SEGCON, (void*)pPhSeg);


        /* Add the old-style terminal. */
        switch (LV_orient) {
          
          case NOSYM:              /* DEF :  N (North).         */
          case SYM_X:              /* DEF : FN (Flipped North). */
            orientCon = SOUTH;
                 xCon = xFR1 + widthCon / 2;
                 yCon = yFR1;
            break;

          case SYM_Y:              /* DEF : FS (Flipped South). */
          case SYMXY:              /* DEF :  S (South).         */
            orientCon = NORTH;
                 xCon = xFR1 + widthCon / 2;
                 yCon = yFR2;
            break;

          case ROT_M:              /* DEF :  E (East).         */
          case SY_RM:              /* DEF : FE (Flipped East). */
            orientCon = WEST;
                 xCon = xFR1;
                 yCon = yFR1 + widthCon / 2;
            break;

          default:
          case ROT_P:              /* DEF :  W (West).         */
          case SY_RP:              /* DEF : FW (Flipped West). */
            orientCon = EAST;
                 xCon = xFR2;
                 yCon = yFR1 + widthCon / 2;
            break;
        } /* End of "switch (LV_orient)". */

        pPhCon = addphcon (LV_pPhFig,
                           orientCon,
                           netName,
                           xCon,
                           yCon,
                           LV_layer,
                           widthCon);

        LV_ptLoseg->tLoseg[pElem->VALUE] = addloseg (
          LV_ptLoseg->tLoseg[pElem->VALUE], LOSEG_CON, (void*)pPhCon);
      }
  }
  /* } */
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_pin_option_direction()".
 */

extern void	 def_pin_option_direction(dir)
  double dir;
{
  LV_flags      |= F_PIN_DIRECTION;
  LV_direction   = DEF2MBK_direction (dir);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_pin_option_place()".
 */

extern void	 def_pin_option_place(ptcon, orient)
  struct pt *ptcon;
     double  orient;
{
  LV_flags      |= F_PIN_STATE;
  LV_ptCurrent   = ptcon;
  LV_orient      = DEF2MBK_transf (orient);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_pin_option_layer()".
 */

extern void	 def_pin_option_layer(layer_name, pt1, pt2)
       char *layer_name;
  struct pt *pt1;
  struct pt *pt2;
{
  LV_flags |= F_PIN_LAYER;
  LV_layer  = getALU (DEF2MBK_layer (namealloc (layer_name)));

  LV_pt1 = pt1;
  LV_pt2 = pt2;
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_start_vias()".
 */


extern void	 def_start_vias(number)
  double number;
{
  LV_numObjects     = (long)number;
  LV_numObjectsRead = 0L;
  LV_flags          = 0L;
  /* Reset the VIA table. */
  LV_mVIA           = 0L;
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_end_vias()".
 */

extern void	 def_end_vias(void)
{
  if (LV_numObjects != LV_numObjectsRead) {
    warnMBK ("DEFloadlophig");
    wprintf ("Invalid VIAs number : %d, (%d read)\n",
             LV_numObjects, LV_numObjectsRead);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_via_start()".
 */

extern void	 def_via_start(via_name)
  char *via_name;
{
  LV_modelName = namealloc (via_name);
  LV_numObjectsRead++;

  free (via_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_via_stmt_rect()".
 */

extern void	 def_via_stmt_rect(layer_name, pt1, pt2)
       char *layer_name;
  struct pt *pt1, *pt2;
{
  char *DEF_layer;


  DEF_layer = namealloc (layer_name);

  if (DEF_isLayer (DEF_layer)) {
    LV_layer = DEF2MBK_layer (DEF_layer);

    /* This is one of the layer geometry. */
    LV_width  = DEF2MBK_length ((long)(pt2->x) - (long)(pt1->x));
    LV_height = DEF2MBK_length ((long)(pt2->y) - (long)(pt1->y));

    LV_width  = ABS(LV_width);
    LV_height = ABS(LV_height);

    if (   (LV_height <= MBKSCALE(2))
        && (LV_width  <= MBKSCALE(2))) {
      LV_width  = 0;
      LV_height = 0;
    }
  } else {
    /* This is the cut layer. */
    LV_VIA = DEF2MBK_dataCut (DEF_layer);

    LV_flags |= F_VIA_CUT;
  }
  
  def_free_pt (pt1);
  def_free_pt (pt2);
  free (layer_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_via_stmt_pattern()".
 */

extern void	 def_via_stmt_pattern(pattern_name)
  char *pattern_name;
{
  LV_flags |= F_VIA_PATTERN;

  free (pattern_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_via_end()".
 */

extern void	 def_via_end()
{
  if (!(LV_flags & F_VIA_CUT))
    LV_VIA = getTurnVIA (LV_layer);

  mprintf2 ("     - \"%20s\" id %02d %5ldx%-5ld.\n",
            LV_modelName, LV_VIA, LV_width, LV_height);

  addDataVIA (LV_modelName, LV_VIA, LV_width, LV_height);

  LV_flags = 0;
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_start_comps()".
 */


extern void	 def_start_comps(number)
  double  number;
{
  LV_numObjects     = (long)number;
  LV_numObjectsRead = 0L;
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_end_comps()".
 */

extern void	 def_end_comps(void)
{
  if (LV_numObjects != LV_numObjectsRead) {
    warnMBK ("DEFloadlophig");
    wprintf ("Invalid components number : %d, (%d read)",
             LV_numObjects, LV_numObjectsRead);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_comp_start()".
 */

extern void	 def_comp_start(char *ins_name, char *cell_name)
{
  LV_numObjectsRead++;

  if (LV_pPhFig) {
    /* Coordinates and symetry will be filled by "comp_options()". */
    LV_pPhIns = addphins (LV_pPhFig, cell_name, ins_name, NOSYM, 0, 0);
  }

  if (LV_pLoFig) {
    /* Assignement of signals to terminals will be done in the "net"
     * section.
     */
    addloins_noSig (LV_pLoFig, ins_name, getlofig (cell_name, 'P'));
  }

  free ( ins_name);
  free (cell_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_comp_net_list()".
 */

extern void	 def_comp_net_list(char *name)
{
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_comp_type()".
 */

extern void	 def_comp_type(pt, orient)
  struct pt *pt;
     double  orient;
{
  if (LV_pPhFig) {
    if (!IS_XY_PT (pt)) {
      errMBK  ("defloadlophfig");
      eprintf ("\n  Invalid coordinate '*' for instance placement \"%s\"\n",
               LV_pPhIns->INSNAME);
      eprintf ("  at line %ld.\n", yylineno);
      EXIT (1);
    }

    LV_pPhIns->XINS   = DEF2MBK_length ((long)(pt->x));
    LV_pPhIns->YINS   = DEF2MBK_length ((long)(pt->y));
    LV_pPhIns->TRANSF = DEF2MBK_transf (orient);
  }

  def_free_pt (pt);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_start_nets()".
 */


extern void	 def_start_nets(double number)
{
  LV_numObjects     = (long)number;
  LV_numObjectsRead = 0L;

  mprintf1 ("  o  Reading NETS section (%ld nets)\n", LV_numObjects);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_end_nets()".
 */

extern void	 def_end_nets(void)
{
  if (LV_numObjects != LV_numObjectsRead) {
    warnMBK ("DEFloadlophig");
    wprintf ("Invalid net number : %d, (%d read)",
             LV_numObjects, LV_numObjectsRead);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_net_start()".
 */

extern void	 def_net_start(char *name)
{
  struct authelem *pElem;


  LV_numObjectsRead++;
  LV_modelName = DEF2MBK_name (name);

  mprintf2 ("     - \"%s\"\n", LV_modelName);

  if ((pElem = searchauthelem (LV_htPin, LV_modelName)) != NULL) {
    /* The terminal name no longer override the net name. But we keep
     * the pin hash table because we must know if the net is external
     * or internal (i.e. the layer is ALU or CALU.
     *
     * LV_modelName  = LV_tPin[pElem->VALUE].pinName;
     */
    LV_flags     = F_SEG_PIN;
    if (LV_pLoFig) {
      LV_pLoSig = getlosig (LV_pLoFig, pElem->VALUE);
    }
  } else {
    /* This not is not linked to a terminal. So it has not been created
     * while reading the PIN section. We do it now.
     */
    if (LV_pLoFig) {
      LV_pLoSig = addlosig (LV_pLoFig,
                            LV_mSig++,
                            addchain (NULL, (void*)LV_modelName),
                            INTERNAL);
    }
    LV_flags  = 0L;
  }

  free (name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_path_start()".
 */

extern void def_path_start(layer_name)
  char *layer_name;
{
  char *DEF_layer;


  DEF_layer = namealloc (layer_name);

  LV_layer  = DEF2MBK_layer      (DEF_layer);
  LV_width  = DEF2MBK_layerWidth (DEF_layer);
  LV_flags |= F_SEG_INITIAL;

  free (layer_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_path_item_via()".
 */

extern void def_path_item_via(via_name)
  char *via_name;
{
        char *DEF_viaLayer;
   dataVIA_t *pDataVIA;
    authelem *pElem;
  phvia_list *pPhvia;


  /* This a VIA and maybe a layer change. */
  DEF_viaLayer = namealloc (via_name);


  if (LV_flags & F_SEG_INITIAL) {
    errMBK  ("defloadlophfig");
    eprintf ("Net path cannot begin by a VIA at line %ld", yylineno);
    EXIT (1);
  }


  pDataVIA = DEF2MBK_dataVIA (DEF_viaLayer);
  LV_layer = getAltVIALayer (pDataVIA->VIA, LV_layer);

  if (!(LV_flags & F_SEG_SPECIAL)) {
    LV_width = MBK_layerWidth (LV_layer);
  }


  if (LV_pPhFig && !(LV_flags & F_SEG_BLOCKAGE)) {
    pPhvia = addphvia(LV_pPhFig,
                      pDataVIA->VIA,
                      LV_lastX,
                      LV_lastY,
                      pDataVIA->width,
                      pDataVIA->height,
                      LV_modelName);

    if (!getmvia (LV_ptLoseg->tMVIA, pPhvia)) {
      LV_ptLoseg->tMVIA = addmvia (LV_ptLoseg->tMVIA, pPhvia);
    }

    pElem = gettlosegitem (LV_ptLoseg, LV_modelName); 

    LV_ptLoseg->tLoseg[pElem->VALUE] = addloseg (
        LV_ptLoseg->tLoseg[pElem->VALUE], LOSEG_VIA, (void*)pPhvia);
  }


  free (via_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_path_item_pt()".
 */

extern void def_path_item_pt(pt)
  struct pt *pt;
{
        long  curX, curY;
        char *sigName, powerName[64];
    authelem *pElem;
  phseg_list *pPhSeg;


  if (IS_X_PT(pt)) { curX = DEF2MBK_length ((long)pt->x); }
  else             { curX = LV_lastX; }


  if (IS_Y_PT(pt)) { curY = DEF2MBK_length ((long)pt->y); }
  else             { curY = LV_lastY; }


  if (LV_flags & F_SEG_INITIAL) {
    if (!IS_XY_PT(pt)) {
      errMBK  ("defloadlophfig");
      eprintf ("Invalid first net path point at line %ld", yylineno);
      EXIT (1);
    }
    
    LV_flags &= ~F_SEG_INITIAL;
  } else {
    if (LV_pPhFig) {
      if (   (LV_defFlags & F_DEF_SPLIT_POWER)
          && (isvdd (LV_modelName) || isvss (LV_modelName))) {
        sigName = powerName;
        sprintf (sigName,
                 "%s.%ld",
                 LV_modelName,
                 (curY - LV_pPhFig->XAB1 + MBKSCALE(25)) / MBKSCALE(50));
      } else
        sigName = LV_modelName;

      if (!(LV_flags & F_SEG_BLOCKAGE)) {
        pPhSeg = addphseg (LV_pPhFig,
                           (LV_flags & F_SEG_PIN) ? getCALU (LV_layer)
                                                  : LV_layer,
                           LV_width,
                           LV_lastX,
                           LV_lastY,
                           curX,
                           curY,
                           sigName);

        pElem = gettlosegitem (LV_ptLoseg, sigName); 

        LV_ptLoseg->tLoseg[pElem->VALUE] = addloseg (
            LV_ptLoseg->tLoseg[pElem->VALUE], LOSEG_SEG, (void*)pPhSeg);
      }
    }
  }


  LV_lastX = curX;
  LV_lastY = curY;

  def_free_pt (pt);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_opt_width()".
 */

  extern void	 def_opt_width(number)
  double  number;
{
  LV_width = DEF2MBK_length ((long)number);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_net_conn_start()".
 */

extern void	 def_net_conn_start(char *ins_name, char *pin_name)
{
  if (LV_pLoFig) {
    if (strcmp (ins_name, "PIN")) {
      addlosig_insCon (getloins (LV_pLoFig,
                                 ins_name),
                       DEF2MBK_name (pin_name),
                       LV_pLoSig);

      free (ins_name);
    }
  }

  
  free (pin_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_net_conn_opt_synthesized()".
 */

extern void	 def_net_conn_opt_synthesized()
{
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_start_snets()".
 */


extern void	 def_start_snets(double number)
{
  LV_numObjects     = (long)number;
  LV_numObjectsRead = 0L;

  mprintf1 ("  o  Reading SPECIALNETS section.\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_end_snets()".
 */

extern void	 def_end_snets(void)
{
  if (LV_numObjects != LV_numObjectsRead) {
    warnMBK ("DEFloadlophig");
    wprintf ("Invalid special nets number : %d, (%d read)",
             LV_numObjects, LV_numObjectsRead);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_snet_start()".
 */

extern void	 def_snet_start(char *name)
{
  struct authelem *pElem;


  LV_numObjectsRead++;
  LV_modelName = namealloc (name);
  LV_flags     = F_SEG_SPECIAL;

  if (LV_modelName == LV_blockageName) {
    /* This is a special net named "_BLOCKAGE_RESERVED" used to forbid
     * the corners of the design when placing the IO pins. Must not be
     * driven in the MBK data-base.
     */
    LV_flags |= F_SEG_BLOCKAGE;
  } else {
    if ((pElem = searchauthelem (LV_htPin, LV_modelName)) != NULL) {
      /* The terminal name no longer override the net name. But we keep
       * the pin hash table because we must know if the net is external
       * or internal (i.e. the layer is ALU or CALU.
       *
       * LV_modelName  = LV_tPin[pElem->VALUE].pinName;
       */
      LV_flags     |= F_SEG_PIN;
      if (LV_pLoFig) {
        LV_pLoSig = getlosig (LV_pLoFig, pElem->VALUE);
      }
    } else {
      /* This not is not linked to a terminal. So it has not been created
       * while reading the PIN section. We do it now.
       */
      if (LV_pLoFig) {
        LV_pLoSig = addlosig (LV_pLoFig,
                              LV_mSig++,
                              addchain (NULL,
                                        (void*)DEF2MBK_name(LV_modelName)),
                              INTERNAL);
      }
      LV_flags  = 0L;
    }
  }

  mprintf2 ("     - \"%s\".\n", LV_modelName);


  free (name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_snet_conn_start()".
 */

extern void	 def_snet_conn_start(char *ins_name, char *pin_name)
{
  if (LV_pLoFig) {
    if (strcmp (ins_name, "PIN")) {
      addlosig_insCon (getloins (LV_pLoFig,
                                 ins_name),
                       pin_name,
                       LV_pLoSig);

      free (ins_name);
    }
  }

  
  free (pin_name);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_snet_conn_opt_synthesized()".
 */

extern void	 def_snet_conn_opt_synthesized()
{
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_snet_conn_opt_synthesized()".
 */

extern void  def_parse_error(str)
  char *str;
{
  errMBK  ("DEFloadphlofig");
  eprintf ("%s line %d.\n", str, yylineno);
  EXIT (1);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_alloc_pt()".
 */

extern struct pt *def_alloc_pt(x, y)
  double  x, y;
{
  struct pt *pt;


  pt = (struct pt*)malloc (sizeof (struct pt));

  if (!pt) {
    errMBK  ("DEFloadlophig");
    eprintf ("Not enougth memory in \"def_alloc_pt()\"!\n");
    EXIT (1);
  }

  pt->x = x;
  pt->y = y;

  return (pt);
}


/*  ------------------------------------------------------------------
 *  Function  :  "def_free_pt()".
 */

extern void  def_free_pt(pt)
  struct pt *pt;
{
  free (pt);
}


/*  ------------------------------------------------------------------
 *  Function  :  "initDataTables()".
 */

static void  initDataTables()
{
  int  iData;


  for (iData = 0; iData < SIZE_TDATALAYER; iData++) {
    tDataLayer[iData].flags = 0L;
  }

  for (iData = 0; iData < SIZE_TDATACUT; iData++) {
    tDataCut[iData].flags = 0L;
  }

  for (iData = 0; iData < SIZE_TDATAVIA; iData++) {
    tDataVIA[iData].flags = 0L;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "setDataLayer()".
 */

static void  setDataLayer(aN, aName, aLayer, aCLayer, aWidth)
   int  aN;
  char *aName;
  char  aLayer;
  char  aCLayer;
  long  aWidth;
{
  tDataLayer[aN].name   = namealloc (aName);
  tDataLayer[aN].layer  = aLayer;
  tDataLayer[aN].clayer = aCLayer;
  tDataLayer[aN].width  = aWidth;
  tDataLayer[aN].flags |= F_DATA_SET;
}


/*  ------------------------------------------------------------------
 *  Function  :  "setDataCut()".
 */

static void  setDataCut(aN, aName, aCut)
   int  aN;
  char *aName;
  char  aCut;
{
  tDataCut[aN].name   = namealloc (aName);
  tDataCut[aN].VIA    = aCut;
  tDataCut[aN].flags |= F_DATA_SET;
}


/*  ------------------------------------------------------------------
 *  Function  :  "addDataVIA()".
 */

static void  addDataVIA(aName, aCut, aWidth, aHeight)
  char *aName;
  char  aCut;
  long  aWidth;
  long  aHeight;
{
  if (LV_mVIA > SIZE_TDATAVIA) {
    errMBK  ("DEFloadlophig");
    eprintf ("VIA table overfow (more than %d VIAs).\n", SIZE_TDATAVIA);
    EXIT (1);
  }

  tDataVIA[LV_mVIA].name   = namealloc (aName);
  tDataVIA[LV_mVIA].VIA    = aCut;
  tDataVIA[LV_mVIA].flags |= F_DATA_SET;
  tDataVIA[LV_mVIA].width  = aWidth;
  tDataVIA[LV_mVIA].height = aHeight;

  LV_mVIA++;
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_dataVIA()".
 */

static dataVIA_t *DEF2MBK_dataVIA(aName)
  char *aName;
{
  int  iDV;


  for (iDV = 0; iDV < SIZE_TDATAVIA; iDV++) {
    if (   (tDataVIA[iDV].flags & F_DATA_SET)
        && (tDataVIA[iDV].name == aName)) {
      return (&(tDataVIA[iDV]));
    }
  }

  errMBK  ("DEFloadphlofig");
  eprintf ("\n  Unknown DEF VIA name \"%s\" at line %d.\n",
           aName, yylineno);
  EXIT (1);

  /* Only to shut up GCC... */
  return (NULL);
}
  

/*  ------------------------------------------------------------------
 *  Function  :  "DEF_isLayer()".
 */

static long  DEF_isLayer(aName)
  char *aName;
{
  int  iDL;


  for (iDL = 0; iDL < SIZE_TDATALAYER; iDL++) {
    if (    (tDataLayer[iDL].flags & F_DATA_SET)
        &&  (tDataLayer[iDL].name  == aName)) {
      return (TRUE);
    }
  }

  return (FALSE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_layer()".
 */

static char  DEF2MBK_layer(aName)
  char *aName;
{
  int  iDL;


  for (iDL = 0; iDL < SIZE_TDATALAYER; iDL++) {
    if (   (tDataLayer[iDL].flags & F_DATA_SET)
        && (tDataLayer[iDL].name == aName)) {
      return (tDataLayer[iDL].layer);
    }
  }

  errMBK  ("DEFloadphlofig");
  eprintf ("\n  Unknown DEF layer name \"%s\" at line %d.\n",
           aName, yylineno);
  EXIT (1);

  /* Only to shut up GCC... */
  return ('\0');
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_layerWidth()".
 */

static long  DEF2MBK_layerWidth(aName)
  char *aName;
{
  int  iDL;


  for (iDL = 0; iDL < SIZE_TDATALAYER; iDL++) {
    if (    (tDataLayer[iDL].flags & F_DATA_SET)
        &&  (tDataLayer[iDL].name  == aName)) {
      return (tDataLayer[iDL].width);
    }
  }

  errMBK  ("DEFloadphlofig");
  eprintf ("\n  Unknown DEF layer name \"%s\" at line %d.\n",
           aName, yylineno);
  EXIT (1);

  /* Only to shut up GCC... */
  return (0L);
}


/*  ------------------------------------------------------------------
 *  Function  :  "MBK_layerWidth()".
 */

static long  MBK_layerWidth(aLayer)
  char  aLayer;
{
  int  iDL;


  for (iDL = 0; iDL < SIZE_TDATALAYER; iDL++) {
    if (   (tDataLayer[iDL].flags & F_DATA_SET)
        && (   (tDataLayer[iDL].layer  == aLayer)
            || (tDataLayer[iDL].clayer == aLayer))) {
      return (tDataLayer[iDL].width);
    }
  }

  errMBK  ("DEFloadphlofig");
  eprintf ("\n  Unknown MBK layer id \"%d\" at line %d.\n",
           (int)aLayer, yylineno);
  EXIT (1);

  /* Only to shut up GCC... */
  return (0L);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_dataCut()".
 */

static char  DEF2MBK_dataCut(aName)
  char *aName;
{
  int  iDC;


  for (iDC = 0; iDC < SIZE_TDATACUT; iDC++) {
    if (   (tDataCut[iDC].flags & F_DATA_SET)
        && (tDataCut[iDC].name == aName)) {
      return (tDataCut[iDC].VIA);
    }
  }

  errMBK  ("DEFloadphlofig");
  eprintf ("\n  Unknown DEF cut layer name \"%s\" at line %d.\n",
           aName, yylineno);
  EXIT (1);

  /* Only to shut up GCC... */
  return ('\0');
}


/*  ------------------------------------------------------------------
 *  Function  :  "orderCoord()".
 */

static void  orderCoord (aX1, aX2)
  long *aX1, *aX2;
{
  long  tmp;


  if (*aX1 > *aX2) 
    { tmp = *aX1; *aX1 = *aX2; *aX2 = tmp; }
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF_pinxyflat()".
 */

static void DEF_pinxyflat(apX_flat, apY_flat, aX, aY, aX_ins, aY_ins, aT)
  long *apX_flat, *apY_flat;
  long   aX     ,   aY;
  long   aX_ins ,   aY_ins;
  char   aT;
{
   switch (aT) {
      /* DEF : N (North). */
      case NOSYM :
         *apX_flat = aX_ins + aX;
         *apY_flat = aY_ins + aY;
         break;

      /* DEF : FN (Flipped North). */
      case SYM_X :
         *apX_flat = aX_ins - aX;
         *apY_flat = aY_ins + aY;
         break;

      /* DEF : S (Flipped South). */
      case SYM_Y :
         *apX_flat = aX_ins + aX;
         *apY_flat = aY_ins - aY;
         break;

      /* DEF : S (South). */
      case SYMXY :
         *apX_flat = aX_ins - aX;
         *apY_flat = aY_ins - aY;
         break;

      /* DEF : W (West). */
      case ROT_P :
         *apX_flat = aX_ins - aY;
         *apY_flat = aY_ins + aX;
         break;

      /* DEF : E (East). */
      case ROT_M :
         *apX_flat = aX_ins + aY;
         *apY_flat = aY_ins - aX;
         break;

      /* DEF : FP (Flipped West). */
      case SY_RP :
         *apX_flat = aX_ins + aY;
         *apY_flat = aY_ins + aX;
         break;

      /* DEF : FE (Flipped East). */
      case SY_RM :
         *apX_flat = aX_ins - aY;
         *apY_flat = aY_ins - aX;
      break;
   }
}


/*  ------------------------------------------------------------------
 *  Function  :  "LEF2MBK_setData()".
 */

static void  LEF2MBK_setData()
{
  setDataLayer (0, "ALU1", ALU1, CALU1, MBKSCALE(1));
  setDataLayer (1, "ALU2", ALU2, CALU2, MBKSCALE(2));
  setDataLayer (2, "ALU3", ALU3, CALU3, MBKSCALE(2));
  setDataLayer (3, "ALU4", ALU4, CALU4, MBKSCALE(2));
  setDataLayer (4, "ALU5", ALU5, CALU5, MBKSCALE(2));
  setDataLayer (5, "ALU6", ALU6, CALU6, MBKSCALE(2));
  setDataLayer (6, "ALU7", ALU7, CALU7, MBKSCALE(2));
  setDataLayer (7, "ALU8", ALU8, CALU8, MBKSCALE(2));
  setDataLayer (8, "ALU9", ALU9, CALU9, MBKSCALE(2));

  setDataCut (0, "VIAP", CONT_POLY);
  setDataCut (1, "VIA1", CONT_VIA);
  setDataCut (2, "VIA2", CONT_VIA2);
  setDataCut (3, "VIA3", CONT_VIA3);
  setDataCut (4, "VIA4", CONT_VIA4);
  setDataCut (5, "VIA5", CONT_VIA5);
  setDataCut (6, "VIA6", CONT_VIA6);
  setDataCut (7, "VIA7", CONT_VIA7);
  setDataCut (8, "VIA8", CONT_VIA8);

  addDataVIA ("CONT_VIA" , CONT_VIA , 0, 0);
  addDataVIA ("CONT_VIA2", CONT_VIA2, 0, 0);
  addDataVIA ("CONT_VIA3", CONT_VIA3, 0, 0);
  addDataVIA ("CONT_VIA4", CONT_VIA4, 0, 0);
  addDataVIA ("CONT_VIA5", CONT_VIA5, 0, 0);
}


/*  ------------------------------------------------------------------
 *  Function  :  "defloadlophig()".
 */

extern void  defloadlophfig (apLoFig, apPhFig, aName, aMode, aVL, aFlags)
    lofig_list *apLoFig;
    phfig_list *apPhFig;
          char *aName;
          char  aMode;
          long  aVL;
          long  aFlags;
{
             long  isRouted, iLoseg, type;
             void *MBKobj;
  struct   ePow_s *plPow;
  struct eLoseg_s *pLoseg;


  LV_pPhFig   = apPhFig;
  LV_pLoFig   = apLoFig;
  LV_mode     = aMode;
  LV_name     = namealloc (aName);
  LV_mSig     = 0L;
  LV_defFlags = aFlags;

  LV_blockageName = namealloc ("_BLOCKAGE_RESERVED");

  setVL (aVL);


  initDataTables ();
  LEF2MBK_setData ();


  if (!LV_pLoFig) {
    errMBK  ("defloadlophfig");
    eprintf ("\n  lofig is not allocated");
    eprintf (" while loading \"%s.%s\"\n", aName, LV_IN_PH);
    EXIT (1);
  }

  if (LV_mode != 'A') {
    errMBK  ("defloadlophfig");
    eprintf ("\n  Unsupported load mode '%c'", aMode);
    eprintf (" while loading \"%s.%s\"\n", aName, LV_IN_PH);
    EXIT (1);
  }

  yyin = mbkfopen (LV_name, LV_IN_PH, READ_TEXT);
  if (!yyin) {
    errMBK  ("defloadlophfig");
    eprintf ("\n  Unable to open file \"%s.%s\"\n", aName, LV_IN_PH);
    EXIT (1);
  }


  mprintf2 ("\n");

  LV_ptLoseg = addtloseg (LV_pPhFig);

  yylineno = 1;
  yydebug  = 0;
  yyparse ();

  destroyauthtable (LV_htPin);
  free  (LV_tPin);

  mprintf2 ("\n");


  if (fclose (yyin)) {
    warnMBK ("defloadlophfig");
    wprintf ("\n  Unable to close file \"%s.%s\"\n", aName, LV_IN_PH);
  }


  delfeed    (LV_pLoFig);
  checkLofig (LV_pLoFig);

  if (LV_pPhFig) {
    if (LV_defFlags & F_DEF_NO_IOS) {
      __DBG( fprintf (stderr, "\n  o  Removing fake IO\'s.\n"); )
      for (iLoseg = 0; iLoseg < LV_ptLoseg->sigNB; iLoseg++) {
        isRouted = FALSE;

        for (pLoseg = LV_ptLoseg->tLoseg[iLoseg];
             pLoseg != NULL; pLoseg = pLoseg->next) {
          switch (pLoseg->type) {
            case LOSEG_VIA:
              __DBG(
                fprintf (stderr, "     - \"%s\" is routed.\n",
                         ((struct phvia*)(pLoseg->MBKobj))->NAME);
              )
              isRouted = TRUE;
              break;
            case LOSEG_SEG:
              __DBG(
                fprintf (stderr, "     - \"%s\" is routed.\n",
                         ((struct phseg*)(pLoseg->MBKobj))->NAME);
              )
              isRouted = TRUE;
              break;
            case LOSEG_CON:
            case LOSEG_SEGCON:
              break;
          }

          if (isRouted) break;
        }

        /* If There is only terminal related objects : delete them. */
        if (!isRouted) {
          __DBG( fprintf (stderr, "     - Removing ?\n"); )
          for (pLoseg = LV_ptLoseg->tLoseg[iLoseg]; pLoseg != NULL; ) {
            __DBG( fprintf (stderr, "       - Removing loseg item.\n"); )
            type   = pLoseg->type;
            MBKobj = pLoseg->MBKobj;
            __DBG( fprintf (stderr, "       - MBKobj := %p.\n", MBKobj); )

            pLoseg = delloseg (pLoseg);

            switch (type) {
              case LOSEG_VIA:
                delphvia (LV_pPhFig, (struct phvia*)MBKobj);
                break;
              case LOSEG_CON:
                __DBG(
                  fprintf (stderr, "       - Removing MBK phcon := ");
                  fprintf (stderr, "\"%s\".\n", ((struct phcon*)MBKobj)->NAME);
                )
                
                delphcon (LV_pPhFig, (struct phcon*)MBKobj);
                break;
              case LOSEG_SEG:
              case LOSEG_SEGCON:
                __DBG(
                  fprintf (stderr, "       - Removing MBK phseg := ");
                  fprintf (stderr, "\"%s\".\n", ((struct phseg*)MBKobj)->NAME);
                )
                delphseg (LV_pPhFig, (struct phseg*)MBKobj);
                break;
            }
          }

          LV_ptLoseg->tLoseg[iLoseg] = NULL;
        }
      }
    }

    if (!(LV_defFlags & F_DEF_NO_INTERF)) {
      long  powFlags;


      mprintf1 ("  o  Copying up CALUx terminal segments.\n");
      copyUpCALU (LV_pLoFig, LV_pPhFig, 0);

      mprintf1 ("  o  Adding power supplies terminals.\n");

      
      powFlags = 0L;
      if (LV_defFlags & F_DEF_MERGE_TERM) powFlags = F_POWER_MERGE;

      plPow = buildPow (LV_pPhFig, ALU1, C_POWER_HORIZONTAL, "");
              powToCon (LV_pPhFig, ALU1, C_POWER_HORIZONTAL, plPow, powFlags);

      plPow = buildPow (LV_pPhFig, ALU3, C_POWER_VERTICAL, "");
              powToCon (LV_pPhFig, ALU3, C_POWER_VERTICAL, plPow, powFlags);
    }
  }
}
