/*
 *  $Id: dpgen_Nul.c,v 1.3 2009/06/14 13:51:43 ludo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Module    :         "./dpgen_Nul.c"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include   "util_Defs.h"
# include  "dpgen_Defs.h"
# include  "dgn.h"
# include  "debugon.h"


/*  ------------------------------------------------------------------
 *  Local constants.
 */

# define        SIZE_SNAME      1024



/*  ------------------------------------------------------------------
 *  Local variables  (prefix "LV_").
 */

  static           FILE *LV_FILE;
  static           char *LV_modelName;
  static           long  LV_flags;
  static           long  LV_N;
  static           long  LV_prevTreeOut;
  static           long  LV_oneShift;


/*  ------------------------------------------------------------------
 *  Internal macros declarations.
 */


/*  ------------------------------------------------------------------
 *  Internal function declarations.
 */

  static long  create_Netlist_Tree __FP((long  aOffset,
                                         long  amDepth,
                                         long  amDepthNext));
  static void  create_Netlist      __FP((void));
  static long  create_Layout_Tree  __FP((long  aOffset,
                                         long  amDepth,
                                         long  amDepthNext));
  static void  create_Layout       __FP((void));
  static void  create_Layout       __FP((void));
  static void  create_VHDL         __FP((void));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "create_Netlist_Tree ()".
 */

static long  create_Netlist_Tree (aOffset, amDepth, amDepthNext)
  long  aOffset, amDepth, amDepthNext;
{
  long  depth, mk, k, polarity;
  long  cell_index, i0_index, i1_index, i_offset;
  char *sModel;
  char  sInstance[SIZE_SNAME];
  char *sQ, *sI0, *sI1;


  if (amDepthNext > 0) {
    polarity = (amDepthNext % 2);
  } else {
    polarity = (amDepth % 2);
  }


  if (!amDepth) {
    if (!amDepthNext) {
      /* Pathological case : the zero detect is reduced to only
       * one bit! This is only a inversor.
       */
      GENLIB_LOINSE ("inv_x4",
                     s64printf("nul_%ld", aOffset),
                     s64printf("i => i0[%ld]", aOffset),
                     "nq => nul",
                     "vdd => vdd",
                     "vss => vss", NULL);
    } else {
      LV_prevTreeOut = aOffset;

      if ((amDepth % 2) == polarity) {
        /* This branch will skip an even number of layer, we keep
         * the same polarity.
         */
        GENLIB_LOSIGMERGE (s64printf("i0[%ld]", aOffset),
                           s64printf("nul_%ld", aOffset));

      } else {
        /* There is a polarity inversion. */
        GENLIB_LOINSE ("inv_x4",
                       s64printf("nul_%ld", aOffset),
                       s64printf("i  => i0[%ld]", aOffset),
                       s64printf("nq => nul_%ld", aOffset),
                       "vdd => vdd",
                       "vss => vss", NULL);

      }
      LV_oneShift = 1; aOffset++;
    }

    return (++aOffset);
  }


  /* Build the tree. */
  for (depth = 0; depth < amDepth; depth++) {
    mk = iexp2 (amDepth - depth - 1);

    __DBG(
      fprintf (stderr, "depth := %ld\n", depth);
      fprintf (stderr, "mk    := %ld\n", mk);
      fflush  (stderr);
    )

    for (k = 0; k < mk; k++) {
      i_offset = (depth == 0) ? 0 : (iexp2 (depth - 1) - 1);
      cell_index = iexp2 (depth + 1) * k + iexp2 (depth) - 1 + aOffset;
      i1_index  = iexp2 (depth + 1) * k + iexp2 (depth) + i_offset + aOffset;
      i0_index  = iexp2 (depth + 1) * k                 + i_offset + aOffset;

      __DBG(
        fprintf (stderr, "i0 := %ld\n", i0_index);
        fprintf (stderr, "i1 := %ld\n", i1_index);
        fflush  (stderr);
      )

      sModel = (depth % 2) ? "na2_x1" : "no2_x1";
      sprintf (sInstance, "nul_%ld", cell_index);

      sQ  = s64printf("nq => nul_%ld", cell_index);
      sI0 = s64printf("i0 => nul_%ld", i0_index);
      sI1 = s64printf("i1 => nul_%ld", i1_index);
      if (!depth) {
        sI0 = s64printf("i0 => i0[%ld]", i0_index - LV_oneShift);
        sI1 = s64printf("i1 => i0[%ld]", i1_index - LV_oneShift);
      }

      /* Special case of the last layer. */
      if ((mk == 1) && (LV_prevTreeOut < 0)) {
        __DBG( fprintf(stderr, "Last Layer of the first tree.\n"); )
        if (!amDepthNext) {
          /* This is the only tree in the operator. */
          sModel = (depth % 2) ? "a2_x4"    : "no2_x4";
          sQ     = (depth % 2) ? "q => nul" : "nq => nul";

          __DBG( fprintf(stderr, "This is a pure tree.\n"); )
        } else {
          if ((depth % 2) == polarity) {
            /* Only for the first tree : adjust the polarity. */
            sModel = (depth % 2) ? "a2_x2" : "o2_x2";
            sQ     = s64printf("q => nul_%ld", cell_index);
          }
        }
      }

      GENLIB_LOINSE (sModel,
                     sInstance,
                     sI0, sI1, sQ,
                     "vdd => vdd",
                     "vss => vss", NULL);
    }
  }


  /* Check if there is a ripple part. */
  if (LV_prevTreeOut < 0) {
    LV_prevTreeOut = cell_index;

    return (aOffset + iexp2 (amDepth));
  }


  /* Add the ripple part. */

  cell_index = aOffset - 1;

  if (!amDepthNext) {
    sModel = (depth % 2) ? "a2_x4"    : "no2_x4";
    sQ     = (depth % 2) ? "q => nul" : "nq => nul";
  } else {
    sModel = (depth % 2) ? "na2_x1" : "no2_x1";
    sQ     = s64printf("nq => nul_%ld", cell_index);

    if ((depth % 2) == polarity) {
      sModel = (depth % 2) ? "a2_x2" : "o2_x2";
      sQ     = s64printf("q => nul_%ld", cell_index);
    }
  }

  sprintf (sInstance, "nul_%ld", aOffset - 1);

  i1_index = iexp2 (amDepth - 1) - 1 + aOffset;
  i0_index = LV_prevTreeOut;

  GENLIB_LOINSE (sModel,
                 sInstance,
                 s64printf("i0 => nul_%ld", i0_index),
                 s64printf("i1 => nul_%ld", i1_index),
                 sQ,
                 "vdd => vdd",
                 "vss => vss", NULL);

  LV_prevTreeOut = cell_index;


  return (aOffset + iexp2 (amDepth));
}


/*  ------------------------------------------------------------------
 *  Function  :  "create_Netlist ()".
 */

static void  create_Netlist ()
{
  long  mDepth, depth, depthNext, offset;


  mDepth = ilog2 (LV_N);

  LV_prevTreeOut = -1;
  LV_oneShift    =  0;


  GENLIB_DEF_LOFIG (LV_modelName);

  DPGEN_LOCON_BUS ("i0", IN);
  GENLIB_LOCON ("nul",   OUT, "nul");
  GENLIB_LOCON ("vdd", IN   , "vdd");
  GENLIB_LOCON ("vss", IN   , "vss");


  offset     = 0;
  depthNext  = get_depth (LV_N, NULL);

  do {
    depth     = depthNext;
    depthNext = get_depth (LV_N, depth + 1);

    __DBG(
      fprintf (stderr, "prevTreeOut := %ld\n", LV_prevTreeOut);
      fprintf (stderr, "depth       := %ld\n", depth);
      fprintf (stderr, "depthNext   := %ld\n", depthNext);
      fflush  (stderr);
    )
    offset = create_Netlist_Tree (offset, depth, depthNext);
  } while (depthNext);


  GENLIB_SAVE_LOFIG ();
}


/*  ------------------------------------------------------------------
 *  Function  :  "create_Layout_Tree ()".
 */

static long  create_Layout_Tree (aOffset, amDepth, amDepthNext)
  long  aOffset, amDepth, amDepthNext;
{
  long  depth, mk, k, polarity;
  long  cell_index;
  char *sModel;
  char  sInstance[SIZE_SNAME];


  if (amDepthNext > 0) {
    polarity = (amDepthNext % 2);
  } else {
    polarity = (amDepth % 2);
  }


  if (!amDepth) {
    if (!amDepthNext) {
      /* Pathological case : the zero detect is reduced to only
       * one bit! This is only a inversor.
       */
      DPGEN_PLACE ("inv_x4", s64printf("nul_%ld", aOffset), 0, aOffset * 50);
    } else {
      LV_prevTreeOut = aOffset;

      if ((amDepth % 2) != polarity) {
        /* There is a polarity inversion. */
        DPGEN_PLACE ("inv_x4", s64printf("nul_%ld", aOffset), 0, aOffset * 50);
      }
      LV_oneShift = 1; aOffset++;
    }

    return (++aOffset);
  }


  /* Build the tree. */
  for (depth = 0; depth < amDepth; depth++) {
    mk = iexp2 (amDepth - depth - 1);

    for (k = 0; k < mk; k++) {
      cell_index = iexp2 (depth + 1) * k + iexp2 (depth) - 1 + aOffset;

      sModel = (depth % 2) ? "na2_x1" : "no2_x1";
      sprintf (sInstance, "nul_%ld", cell_index);

      /* Special case of the last layer. */
      if ((mk == 1) && (LV_prevTreeOut < 0)) {
        if (!amDepthNext) {
          /* This is the only tree in the operator. */
          sModel = (depth % 2) ? "a2_x4" : "no2_x4";
        } else {
          if ((depth % 2) == polarity) {
            /* Only for the first tree : adjust the polarity. */
            sModel = (depth % 2) ? "a2_x2" : "o2_x2";
          }
        }
      }

      DPGEN_PLACE (sModel, sInstance, 0, cell_index * 50);
    }
  }


  /* Check if there is a ripple part. */
  if (LV_prevTreeOut < 0) {
    LV_prevTreeOut = cell_index;

    return (aOffset + iexp2 (amDepth));
  }


  /* Add the ripple part. */

  cell_index = aOffset - 1;

  if (!amDepthNext) {
    sModel = (depth % 2) ? "a2_x4" : "no2_x4";
  } else {
    sModel = (depth % 2) ? "na2_x1" : "no2_x1";

    if ((depth % 2) == polarity) {
      sModel = (depth % 2) ? "a2_x2" : "o2_x2";
    }
  }

  sprintf (sInstance, "nul_%ld", aOffset - 1);

  DPGEN_PLACE (sModel, sInstance, 0, (aOffset - 1) * 50);

  LV_prevTreeOut = cell_index;


  return (aOffset + iexp2 (amDepth));
}


/*  ------------------------------------------------------------------
 *  Function  :  "create_Layout ()".
 */

static void  create_Layout ()
{
  long  mDepth, depth, depthNext, offset;


  mDepth = ilog2 (LV_N);

  LV_prevTreeOut = -1;
  LV_oneShift    =  0;


  GENLIB_DEF_PHFIG (LV_modelName);


  offset     = 0;
  depthNext  = get_depth (LV_N, NULL);

  do {
    depth     = depthNext;
    depthNext = get_depth (LV_N, depth + 1);

    offset = create_Layout_Tree (offset, depth, depthNext);
  } while (depthNext);


  GENLIB_DEF_AB (0, 0, 0, 0);
  GENLIB_SAVE_PHFIG ();
}


/*  ------------------------------------------------------------------
 *  Function  :  "create_VHDL ()".
 */

static void  create_VHDL ()
{
            char  sConst[SIZE_SNAME];
            char  sBusWide[SIZE_SNAME];
  struct xlong_s *xl_zero;


  LV_FILE = mbkfopen (LV_modelName, "vbe", WRITE_TEXT);

  if (!LV_FILE) {
    eprinth ("*** dpgen ***");
    eprintf ("\n  Unable to create file \"%s.vbe\".\n", LV_modelName);
    EXIT (1);
  }


  VHDL_BUS_WIDE (sBusWide, LV_N - 1, 0);


  /* Entity description. */
  fprintf (LV_FILE, "\nENTITY %s IS\n  PORT (\n", LV_modelName);

  fprintf (LV_FILE, "%14s : in    BIT_VECTOR %s;\n", "i0", sBusWide);
  fprintf (LV_FILE, "%14s :   out BIT;\n", "nul");

  fprintf (LV_FILE, "%14s : in    BIT;\n", "vdd");
  fprintf (LV_FILE, "%14s : in    BIT\n" , "vss");

  /* End of entity description. */
  fprintf (LV_FILE, "  );\nEND %s;\n\n\n", LV_modelName);


  /* Architecture description. */
  fprintf (LV_FILE, "ARCHITECTURE VBE OF %s IS\n\n", LV_modelName);


  fprintf (LV_FILE, "BEGIN\n\n");

  xl_zero = newxl ();
  xl_zero->size = LV_N;

  fprintf (LV_FILE,
           "    nul <= \'1\' WHEN (i0 = %s)\n",
           xltovhdl (xl_zero, sConst));
  fprintf (LV_FILE, "               ELSE \'0\';\n\n\n");
  

  fprintf(LV_FILE, "  ASSERT (vdd = '1')\n");
  fprintf(LV_FILE,
          "    REPORT \"Power supply is missing on vdd of model %s.\"\n",
          LV_modelName);
  fprintf(LV_FILE, "    SEVERITY WARNING;\n\n");


  fprintf(LV_FILE, "  ASSERT (vss = '0')\n");
  fprintf(LV_FILE,
          "    REPORT \"Power supply is missing on vss of model %s.\"\n",
          LV_modelName);
  fprintf(LV_FILE, "    SEVERITY WARNING;\n\n");


  /* End of architecture description. */
  fprintf (LV_FILE, "END VBE;\n");


  fclose (LV_FILE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "dpgen_Nul ()".
 */

extern void  dpgen_Nul (aFunction, aAL)
     long  aFunction;
  va_list  aAL;
{
  /* Parses the mandatory arguments. */
  LV_modelName = va_arg (aAL, char*);
  LV_flags     = va_arg (aAL, long );
  LV_N         = va_arg (aAL, long );

  __DBG (fprintf (stderr, "LV_flags := %ld.\n", LV_flags);)


  /* Always create the netlist. */
  create_Netlist ();

  if (LV_flags & F_PLACE) create_Layout ();
  if (LV_flags & F_BEHAV) create_VHDL ();
}
