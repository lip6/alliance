/*
 *  $Id: dpgen_Regular.c,v 1.1 2002/04/29 13:40:54 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Module    :         "./dpgen_Regular.c"                       |
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

# define        MAX_TDRIVE         8
# define        ERROR_INDEX     4096

# define        CELLS_INV          4
# define        CELLS_BUFF         3
# define        CELLS_NAND2        2
# define        CELLS_NAND3        2
# define        CELLS_NAND4        2
# define        CELLS_AND2         2
# define        CELLS_AND3         2
# define        CELLS_AND4         2
# define        CELLS_NOR2         2
# define        CELLS_NOR3         2
# define        CELLS_NOR4         2
# define        CELLS_OR2          2
# define        CELLS_OR3          2
# define        CELLS_OR4          2
# define        CELLS_XNOR2        2
# define        CELLS_XOR2         2
# define        CELLS_NMUX2        2
# define        CELLS_MUX2         4
# define        CELLS_NBUSE        2
# define        CELLS_BUSE         4
# define        CELLS_NAND2MASK    3
# define        CELLS_NOR2MASK     3
# define        CELLS_XNOR2MASK    3
# define        CELLS_CONST        2
# define        CELLS_DFF          2
# define        CELLS_DFFT         2
# define        CELLS_SFF          2
# define        CELLS_SFFT         2

# define        SIZE_SNAME      1024




/*  ------------------------------------------------------------------
 *  Local variables  (prefix "LV_").
 */

  static           FILE *LV_FILE;
  static           long  LV_Function;
  static           char *LV_modelName;
  static           long  LV_drive;
  static struct xlong_s *LV_xl;
  static           long  LV_flags;
  static           long  LV_N;
  static           long  LV_ttDrive[6][MAX_TDRIVE] = {
                                  { 1, 0, 0, 0, 0, 0, 0, 0 },
                                  { 1, 2, 4, 8, 0, 0, 0, 0 },
                                  { 2, 4, 8, 0, 0, 0, 0, 0 },
                                  { 1, 4, 0, 0, 0, 0, 0, 0 },
                                  { 2, 4, 0, 0, 0, 0, 0, 0 },
                                  { 4, 8, 0, 0, 0, 0, 0, 0 }
                                };
  static           long *LV_tDrive;
  static           long  LV_macroDrive;
  static           long  LV_iLeafCell;
  static           char *LV_tsLeafCell[] = {
    /* Boolean generators leaf cells. */
     "inv_x1",    "inv_x2",   "inv_x4", "inv_x8",
                  "buf_x2",   "buf_x4", "buf_x8",
     "na2_x1",                "na2_x4",
     "na3_x1",                "na3_x4",
     "na4_x1",                "na4_x4",
                   "a2_x2",    "a2_x4",
                   "a3_x2",    "a3_x4",
                   "a4_x2",    "a4_x4",
     "no2_x1",                "no2_x4",
     "no3_x1",                "no3_x4",
     "no4_x1",                "no4_x4",
                   "o2_x2",    "o2_x4",
                   "o3_x2",    "o3_x4",
                   "o4_x2",    "o4_x4",
    "nxr2_x1",               "nxr2_x4",
     "xr2_x1",                "xr2_x4",
    /* Multiplexer generators leaf cells. */
    "dp_nmux_x1_buf", "dp_nmux_x1",
     "dp_mux_x2_buf",  "dp_mux_x2", "dp_mux_x4_buf", "dp_mux_x4",
    /* Buse generators leaf cells. */
     "dp_nts_x2_buf",  "dp_nts_x2",
      "dp_ts_x4_buf",   "dp_ts_x4",  "dp_ts_x8_buf",  "dp_ts_x8",
    /* Mask generators leaf cells. */
    "inv_x8", "na2_x4",  "inv_x4",
    "buf_x8", "inv_x4",  "no2_x4",
    "buf_x8", "inv_x4", "nxr2_x4",
    /* Constant generators leaf cells. */
    "zero_x0", "one_x0",
    /* Register generators leaf cells. */
         "dp_dff_x4_buf",      "dp_dff_x4",
    "dp_dff_scan_x4_buf", "dp_dff_scan_x4",
         "dp_sff_x4_buf",      "dp_sff_x4",
    "dp_sff_scan_x4_buf", "dp_sff_scan_x4"
  };


/*  ------------------------------------------------------------------
 *  Internal macros declarations.
 */

# define  DPGEN_PORT_BIT(s) \
            SLICE_TO_BIT((s), LV_N, LV_flags & F_MSB_FIRST)

# define  DPGEN_LOCON_BUS(name,dir)  \
            GENLIB_LOCON ( FULL_BUS(name, LV_N, LV_flags & F_MSB_FIRST), dir, \
                           FULL_BUS(name, LV_N, LV_flags & F_MSB_FIRST))

# define  VHDL_BUS_WIDE(bus,msb,lsb) \
            sprintf (bus,                   \
                     "(%ld %s %ld)",        \
                     DPGEN_PORT_BIT (msb),  \
                     (LV_flags & F_MSB_FIRST) ? "to" : "downto" , \
                     DPGEN_PORT_BIT (lsb))


/*  ------------------------------------------------------------------
 *  Internal function declarations.
 */

  static void  set_tDrive       __FP((void));
  static long  set_iLeafCell    __FP((void));
  static void  init_Generator   __FP((long aFunction));
  static long  has_Buffer       __FP((void));
  static void  create_Netlist   __FP((void));
  static void  create_Layout    __FP((void));
  static void  VHDL_entity      __FP((void));
  static void  VHDL_signal      __FP((void));
  static void  VHDL_boolean     __FP((void));
  static void  VHDL_mask        __FP((void));
  static void  VHDL_const       __FP((void));
  static void  VHDL_mux         __FP((void));
  static void  VHDL_buse        __FP((void));
  static void  VHDL_ff          __FP((void));
  static void  VHDL_archi       __FP((void));
  static void  create_VHDL      __FP((void));
  static char *get_model_Cell   __FP((long aiSlice));
  static char *get_model_Buffer __FP((void));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "set_tDrive ()".
 */

static void  set_tDrive ()
{
  switch (LV_Function) {
    case DPGEN_INV:
      LV_tDrive = LV_ttDrive[1]; break;
    case DPGEN_BUFF:
      LV_tDrive = LV_ttDrive[2]; break;
    case DPGEN_NAND2:
    case DPGEN_NAND3:
    case DPGEN_NAND4:
    case DPGEN_NOR2:
    case DPGEN_NOR3:
    case DPGEN_NOR4:
    case DPGEN_XNOR2:
    case DPGEN_XOR2:
      LV_tDrive = LV_ttDrive[3]; break;
    case DPGEN_AND2:
    case DPGEN_AND3:
    case DPGEN_AND4:
    case DPGEN_OR2:
    case DPGEN_OR3:
    case DPGEN_OR4:
    case DPGEN_MUX2:
      LV_tDrive = LV_ttDrive[4]; break;
    case DPGEN_BUSE:
      LV_tDrive = LV_ttDrive[5]; break;
    case DPGEN_NMUX2:
    case DPGEN_NBUSE:
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
    case DPGEN_CONST:
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
    default:
      LV_tDrive = LV_ttDrive[0]; break;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "set_iLeafCell ()".
 */

static long  set_iLeafCell ()
{
  LV_iLeafCell = 0;

  switch (LV_Function) {
    case DPGEN_SFFT:      LV_iLeafCell += CELLS_SFF;
    case DPGEN_SFF:       LV_iLeafCell += CELLS_DFFT;
    case DPGEN_DFFT:      LV_iLeafCell += CELLS_DFF;
    case DPGEN_DFF:       LV_iLeafCell += CELLS_CONST;
    case DPGEN_CONST:     LV_iLeafCell += CELLS_XNOR2MASK;
    case DPGEN_XNOR2MASK: LV_iLeafCell += CELLS_NOR2MASK;
    case DPGEN_NOR2MASK:  LV_iLeafCell += CELLS_NAND2MASK;
    case DPGEN_NAND2MASK: LV_iLeafCell += CELLS_BUSE;
    case DPGEN_BUSE:      LV_iLeafCell += CELLS_NBUSE;
    case DPGEN_NBUSE:     LV_iLeafCell += CELLS_MUX2;
    case DPGEN_MUX2:      LV_iLeafCell += CELLS_NMUX2;
    case DPGEN_NMUX2:     LV_iLeafCell += CELLS_NOR2;
    case DPGEN_XOR2:      LV_iLeafCell += CELLS_XNOR2;
    case DPGEN_XNOR2:     LV_iLeafCell += CELLS_OR4;
    case DPGEN_OR4:       LV_iLeafCell += CELLS_OR3;
    case DPGEN_OR3:       LV_iLeafCell += CELLS_OR2;
    case DPGEN_OR2:       LV_iLeafCell += CELLS_NOR4;
    case DPGEN_NOR4:      LV_iLeafCell += CELLS_NOR3;
    case DPGEN_NOR3:      LV_iLeafCell += CELLS_NOR2;
    case DPGEN_NOR2:      LV_iLeafCell += CELLS_AND4;
    case DPGEN_AND4:      LV_iLeafCell += CELLS_AND3;
    case DPGEN_AND3:      LV_iLeafCell += CELLS_AND2;
    case DPGEN_AND2:      LV_iLeafCell += CELLS_NAND4;
    case DPGEN_NAND4:     LV_iLeafCell += CELLS_NAND3;
    case DPGEN_NAND3:     LV_iLeafCell += CELLS_NAND2;
    case DPGEN_NAND2:     LV_iLeafCell += CELLS_BUFF;
    case DPGEN_BUFF:      LV_iLeafCell += CELLS_INV;
    case DPGEN_INV:
    default:
  }

  return (LV_iLeafCell);
}


/*  ------------------------------------------------------------------
 *  Function  :  "init_Generator ()".
 */

static void  init_Generator (aFunction)
     long  aFunction;
{
  /* Set the function once and for all. */
  LV_Function = aFunction;

  set_tDrive ();
  set_iLeafCell ();
}


/*  ------------------------------------------------------------------
 *  Function  :  "has_Buffer ()".
 */

static long  has_Buffer ()
{
  switch (LV_Function) {
    case DPGEN_INV:
    case DPGEN_BUFF:
    case DPGEN_NAND2:
    case DPGEN_NAND3:
    case DPGEN_NAND4:
    case DPGEN_AND2:
    case DPGEN_AND3:
    case DPGEN_AND4:
    case DPGEN_NOR2:
    case DPGEN_NOR3:
    case DPGEN_NOR4:
    case DPGEN_OR2:
    case DPGEN_OR3:
    case DPGEN_OR4:
    case DPGEN_XNOR2:
    case DPGEN_XOR2:
    case DPGEN_CONST:
      return (FALSE);
    case DPGEN_NMUX2:
    case DPGEN_NBUSE:
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
    case DPGEN_BUSE:
    case DPGEN_MUX2:
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
      return (TRUE);
  }

  return (FALSE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "get_model_Cell ()".
 */

static char *get_model_Cell (aiSlice)
  long  aiSlice;
{
  long  index;


  index = ERROR_INDEX;

  switch (LV_Function) {
    case DPGEN_INV:
    case DPGEN_BUFF:
    case DPGEN_NAND2:
    case DPGEN_NAND3:
    case DPGEN_NAND4:
    case DPGEN_AND2:
    case DPGEN_AND3:
    case DPGEN_AND4:
    case DPGEN_NOR2:
    case DPGEN_NOR3:
    case DPGEN_NOR4:
    case DPGEN_OR2:
    case DPGEN_OR3:
    case DPGEN_OR4:
    case DPGEN_XNOR2:
    case DPGEN_XOR2:
      index = LV_iLeafCell + LV_macroDrive;
      break;
    case DPGEN_NMUX2:
    case DPGEN_NBUSE:
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
      index = LV_iLeafCell + 1;
      break;
    case DPGEN_MUX2:
    case DPGEN_BUSE:
      index = LV_iLeafCell +  (LV_macroDrive << 1) + 1;
      break;
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
      index = LV_iLeafCell + 1 + getxlbit (LV_xl, aiSlice);
      break;
    case DPGEN_CONST:
      index = LV_iLeafCell + getxlbit (LV_xl, aiSlice);
      break;
  }

  if  (index == ERROR_INDEX) return ("bug_leaf");

  __DBG(
    fprintf (stderr,
             "index := %ld, name := %s,",
             index,
             LV_tsLeafCell[ index ]);
    fprintf (stderr,
             " bit := %ld\n",
             SLICE_TO_BIT(aiSlice, LV_N, LV_flags & F_MSB_FIRST));
  fflush (stderr);
  )

  return (LV_tsLeafCell[ index ]);
}


/*  ------------------------------------------------------------------
 *  Function  :  "get_model_Buffer ()".
 */

static char *get_model_Buffer ()
{
  long  index;


  index = ERROR_INDEX;

  switch (LV_Function) {
    case DPGEN_INV:
    case DPGEN_BUFF:
    case DPGEN_NAND2:
    case DPGEN_NAND3:
    case DPGEN_NAND4:
    case DPGEN_AND2:
    case DPGEN_AND3:
    case DPGEN_AND4:
    case DPGEN_NOR2:
    case DPGEN_NOR3:
    case DPGEN_NOR4:
    case DPGEN_OR2:
    case DPGEN_OR3:
    case DPGEN_OR4:
    case DPGEN_XNOR2:
    case DPGEN_XOR2:
    case DPGEN_CONST:
      /* This function shoudn't have been called. */
      break;
    case DPGEN_NMUX2:
    case DPGEN_NBUSE:
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
      index = LV_iLeafCell;
      break;
    case DPGEN_BUSE:
    case DPGEN_MUX2:
      index = LV_iLeafCell +  (LV_macroDrive << 1);
      break;
  }

  if  (index == ERROR_INDEX) return ("bug_buff");

  __DBG(
    fprintf (stderr,
             "index := %ld, name := %s\n",
             index,
             LV_tsLeafCell[ index ]);
    fflush (stderr);
  )

  return (LV_tsLeafCell[ index ]);
}


/*  ------------------------------------------------------------------
 *  Function  :  "create_Netlist ()".
 */

static void  create_Netlist ()
{
  char  sInstance[SIZE_SNAME];
  char *sModel;
  long  iSlice;


  GENLIB_DEF_LOFIG (LV_modelName);

  /* Control terminals. */
  switch (LV_Function) {
    case DPGEN_NMUX2:
    case DPGEN_MUX2:
    case DPGEN_NBUSE:
    case DPGEN_BUSE:
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
      GENLIB_LOCON ("cmd", IN, "cmd");
      break;
    case DPGEN_DFFT:
    case DPGEN_SFFT:
      GENLIB_LOCON ("scan", IN, "scan");
      GENLIB_LOCON ("scin", IN, "scin");
    case DPGEN_DFF:
    case DPGEN_SFF:
      GENLIB_LOCON ("wen", IN, "wen");
      GENLIB_LOCON ( "ck", IN,  "ck");
      break;
  }


  /* Input data buses terminals. */
  switch (LV_Function) {
    case DPGEN_OR4:
    case DPGEN_NOR4:
    case DPGEN_AND4:
    case DPGEN_NAND4:
      DPGEN_LOCON_BUS ("i3", IN);
    case DPGEN_OR3:
    case DPGEN_NOR3:
    case DPGEN_AND3:
    case DPGEN_NAND3:
      DPGEN_LOCON_BUS ("i2", IN);
    case DPGEN_MUX2:
    case DPGEN_NMUX2:
    case DPGEN_XOR2:
    case DPGEN_XNOR2:
    case DPGEN_OR2:
    case DPGEN_NOR2:
    case DPGEN_AND2:
    case DPGEN_NAND2:
      DPGEN_LOCON_BUS ("i1", IN);
    case DPGEN_DFFT:
    case DPGEN_DFF:
    case DPGEN_SFFT:
    case DPGEN_SFF:
    case DPGEN_XNOR2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_NAND2MASK:
    case DPGEN_BUSE:
    case DPGEN_NBUSE:
    case DPGEN_BUFF:
    case DPGEN_INV:
      DPGEN_LOCON_BUS ("i0", IN);
    case DPGEN_CONST:
      break;
  }


  /* Output data buses terminals. */
  switch (LV_Function) {
    case DPGEN_CONST:
    case DPGEN_BUSE:
    case DPGEN_MUX2:
    case DPGEN_DFFT:
    case DPGEN_DFF:
    case DPGEN_SFFT:
    case DPGEN_SFF:
    case DPGEN_XOR2:
    case DPGEN_OR4:
    case DPGEN_OR3:
    case DPGEN_OR2:
    case DPGEN_AND4:
    case DPGEN_AND3:
    case DPGEN_AND2:
    case DPGEN_BUFF:
      DPGEN_LOCON_BUS ("q", OUT);
      break;
    case DPGEN_INV:
    case DPGEN_NAND2:
    case DPGEN_NAND3:
    case DPGEN_NAND4:
    case DPGEN_NOR2:
    case DPGEN_NOR3:
    case DPGEN_NOR4:
    case DPGEN_XNOR2:
    case DPGEN_NMUX2:
    case DPGEN_NBUSE:
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
      DPGEN_LOCON_BUS ("nq", OUT);
      break;
  }


  /* Power terminals. */
  GENLIB_LOCON ("vdd", IN, "vdd");
  GENLIB_LOCON ("vss", IN, "vss");


  /* Loop for all the data slices. */
  for (iSlice = 0; iSlice < LV_N; iSlice++) {

    sModel = get_model_Cell (iSlice);
    sprintf (sInstance, "cell_%ld", iSlice);

    switch (LV_Function) {
      case DPGEN_INV:
        GENLIB_LOINSE (
          sModel,
          sInstance,
          s64printf ("i  => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_BUFF:
        GENLIB_LOINSE (
          sModel,
          sInstance,
          s64printf ( "i => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ( "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_NAND2:
      case DPGEN_NOR2:
      case DPGEN_XNOR2:
        GENLIB_LOINSE (
          sModel,
          sInstance,
          s64printf ("i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i1 => i1[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_AND2:
      case DPGEN_OR2:
      case DPGEN_XOR2:
        GENLIB_LOINSE (
          sModel,
          sInstance,
          s64printf ("i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i1 => i1[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ( "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_NAND3:
      case DPGEN_NOR3:
        GENLIB_LOINSE (
          sModel,
          sInstance,
          s64printf ("i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i1 => i1[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i2 => i2[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_AND3:
      case DPGEN_OR3:
        GENLIB_LOINSE (
          sModel,
          sInstance,
          s64printf ("i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i1 => i1[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i2 => i2[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ( "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_NAND4:
      case DPGEN_NOR4:
        GENLIB_LOINSE (
          sModel,
          sInstance,
          s64printf ("i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i1 => i1[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i2 => i2[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i3 => i3[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_AND4:
      case DPGEN_OR4:
        GENLIB_LOINSE (
          sModel,
          sInstance,
          s64printf ("i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i1 => i1[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i2 => i2[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ("i3 => i3[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf ( "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_NMUX2:
        GENLIB_LOINSE (
          sModel,
          sInstance,
                    "sel0 => sel0",
                    "sel1 => sel1",
          s64printf (  "i1 => i1[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf (  "i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf (  "nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_MUX2:
        GENLIB_LOINSE (
          sModel,
          sInstance,
                    "sel0 => sel0",
                    "sel1 => sel1",
          s64printf (  "i1 => i1[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf (  "i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf (   "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_NBUSE:
        GENLIB_LOINSE (
          sModel,
          sInstance,
                     "enx => enx",
                    "nenx => nenx",
          s64printf (   "i => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf (  "nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_BUSE:
        GENLIB_LOINSE (
          sModel,
          sInstance,
                     "enx => enx",
                    "nenx => nenx",
          s64printf (   "i => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf (   "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_NAND2MASK:
        if (getxlbit (LV_xl, iSlice)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ( "i => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
            s64printf ("nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else {
          GENLIB_LOINSE (
            sModel,
            sInstance,
                       "i1 => ncmd",
            s64printf ("i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
            s64printf ("nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        }
        break;
      case DPGEN_NOR2MASK:
      case DPGEN_XNOR2MASK:
        if (getxlbit (LV_xl, iSlice)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
                       "i1 => bcmd",
            s64printf ("i0 => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
            s64printf ("nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ( "i => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
            s64printf ("nq => nq[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        }
        break;

      case DPGEN_CONST:
        if (getxlbit (LV_xl, iSlice)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ("q => q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ("nq => q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        }
        break;

      case DPGEN_DFF:
      case DPGEN_SFF:
        __DBG(
          fprintf (stderr, "DPGEN_SFF %s %s\n", sModel, sInstance);
        )
        GENLIB_LOINSE (
          sModel,
          sInstance,
                     "ckx => ckx",
                    "nckx => nckx",
                    "wenx => wenx",
                   "nwenx => nwenx",
          s64printf (   "i => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
          s64printf (   "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
          "vdd => vdd",
          "vss => vss", 0L);
        break;

      case DPGEN_DFFT:
      case DPGEN_SFFT:
        if  (iSlice == 0) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
                       "ckx => ckx",
                      "nckx => nckx",
                      "wenx => wenx",
                     "nwenx => nwenx",
                     "scanx => scanx",
                    "nscanx => nscanx",
                      "scin => scoutx",
            s64printf (  "i => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
            s64printf (  "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else {
          GENLIB_LOINSE (
            sModel,
            sInstance,
                        "ckx => ckx",
                       "nckx => nckx",
                       "wenx => wenx",
                      "nwenx => nwenx",
                      "scanx => scanx",
                     "nscanx => nscanx",
            s64printf ("scin =>  q[%ld]", DPGEN_PORT_BIT (iSlice - 1)), 
            s64printf (   "i => i0[%ld]", DPGEN_PORT_BIT (iSlice)),
            s64printf (   "q =>  q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        }
        break;

    }
  } /* End of "iSlice"  (data slices) loop. */


  if (has_Buffer ()) {

    if (iSlice % 2) iSlice++;

    sModel = get_model_Buffer ();
    sprintf (sInstance, "cell_%ld", iSlice);

    switch (LV_Function) {
      case DPGEN_NAND2MASK:
        GENLIB_LOINSE (
          sModel,
          sInstance,
            "i => cmd",
           "nq => ncmd",
          "vdd => vdd",
          "vss => vss", 0L);
        break;
      case DPGEN_NOR2MASK:
      case DPGEN_XNOR2MASK:
        GENLIB_LOINSE (
          sModel,
          sInstance,
            "i => cmd",
            "q => bcmd",
          "vdd => vdd",
          "vss => vss", 0L);
        break;
      case DPGEN_NMUX2:
      case DPGEN_MUX2:
        GENLIB_LOINSE (
          sModel,
          sInstance,
           "sel => cmd",
          "sel0 => sel0",
          "sel1 => sel1",
           "vdd => vdd",
           "vss => vss", 0L);
        break;
      case DPGEN_NBUSE:
      case DPGEN_BUSE:
        GENLIB_LOINSE (
          sModel,
          sInstance,
            "en => cmd",
          "nenx => nenx",
           "enx => enx",
           "vdd => vdd",
           "vss => vss", 0L);
        break;
      case DPGEN_DFF:
      case DPGEN_SFF:
        GENLIB_LOINSE (
          sModel,
          sInstance,
             "ck => ck",
            "wen => wen",
            "ckx => ckx",
           "nckx => nckx",
           "wenx => wenx",
          "nwenx => nwenx",
            "vdd => vdd",
            "vss => vss", 0L);
        break;
      case DPGEN_DFFT:
      case DPGEN_SFFT:
        GENLIB_LOINSE (
          sModel,
          sInstance,
              "ck => ck",
             "wen => wen",
            "scan => scan",
             "ckx => ckx",
            "nckx => nckx",
            "wenx => wenx",
           "nwenx => nwenx",
           "scanx => scanx",
          "nscanx => nscanx",
            "scin => scin",
           "scout => scoutx",
             "vdd => vdd",
             "vss => vss", 0L);
        break;
    }
  }


  GENLIB_SAVE_LOFIG ();
}


/*  ------------------------------------------------------------------
 *  Function  :  "create_Layout ()".
 */

static void  create_Layout ()
{
  char  sInstance[SIZE_SNAME];
  char *sModel;
  long  iSlice;


  GENLIB_DEF_PHFIG (LV_modelName);


  /* Loop for all the data slices. */
  for (iSlice = 0; iSlice < LV_N; iSlice++) {

    sModel = get_model_Cell (iSlice);
    sprintf (sInstance, "cell_%ld", iSlice);

    GENLIB_PLACE (sModel,
                  sInstance,
                  SLICE_TO_SYM (iSlice, LV_N),
                  0,
                  SLICE_TO_Y (iSlice, LV_N));
  } /* End of iSlice loop. */


  /* Add the buffer cell. */
  if (has_Buffer ()) {

    if (iSlice % 2) iSlice++;

    sModel = get_model_Buffer ();
    sprintf (sInstance, "cell_%ld", iSlice);

    GENLIB_PLACE (sModel,
                  sInstance,
                  SLICE_TO_SYM (iSlice, LV_N),
                  0,
                  SLICE_TO_Y (iSlice, LV_N));
  }


  GENLIB_DEF_AB (0, 0, 0, 0);
  GENLIB_SAVE_PHFIG ();
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_entity ()".
 *
 *  Create the ENTITY description of the operator. Essentially the
 *  PORT part.
 */

static void  VHDL_entity ()
{
  char  sBusWide[SIZE_SNAME];


  VHDL_BUS_WIDE (sBusWide, LV_N - 1, 0);


  fprintf (LV_FILE, "\nENTITY %s IS\n  PORT (\n", LV_modelName);

  /* Controls Terminals. */
  switch (LV_Function) {
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
    case DPGEN_BUSE:
    case DPGEN_NBUSE:
    case DPGEN_MUX2:
    case DPGEN_NMUX2:
      fprintf (LV_FILE, "%14s : in    BIT;\n", "cmd");
      break;
    case DPGEN_DFFT:
    case DPGEN_SFFT:
      fprintf (LV_FILE, "%14s : in    BIT;\n", "scan");
      fprintf (LV_FILE, "%14s : in    BIT;\n", "scin");
    case DPGEN_DFF:
    case DPGEN_SFF:
      fprintf (LV_FILE, "%14s : in    BIT;\n", "wen");
      fprintf (LV_FILE, "%14s : in    BIT;\n", "ck");
      break;
  }


  /* Input data buses terminals. */
  switch (LV_Function) {
    case DPGEN_NAND4:
    case DPGEN_AND4:
    case DPGEN_NOR4:
    case DPGEN_OR4:
      fprintf (LV_FILE, "%14s : in    BIT_VECTOR %s;\n", "i3", sBusWide);
    case DPGEN_NAND3:
    case DPGEN_OR3:
    case DPGEN_AND3:
    case DPGEN_NOR3:
      fprintf (LV_FILE, "%14s : in    BIT_VECTOR %s;\n", "i2", sBusWide);
    case DPGEN_NAND2:
    case DPGEN_AND2:
    case DPGEN_NOR2:
    case DPGEN_OR2:
    case DPGEN_XNOR2:
    case DPGEN_XOR2:
    case DPGEN_NMUX2:
    case DPGEN_MUX2:
      fprintf (LV_FILE, "%14s : in    BIT_VECTOR %s;\n", "i1", sBusWide);
    case DPGEN_INV:
    case DPGEN_BUFF:
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
    case DPGEN_NBUSE:
    case DPGEN_BUSE:
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
      fprintf (LV_FILE, "%14s : in    BIT_VECTOR %s;\n", "i0", sBusWide);
      break;
    case DPGEN_CONST:
  }


  /* Output data buses terminals. */
  switch (LV_Function) {
    case DPGEN_NAND4:
    case DPGEN_NOR4:
    case DPGEN_NAND3:
    case DPGEN_NOR3:
    case DPGEN_NAND2:
    case DPGEN_NOR2:
    case DPGEN_XNOR2:
    case DPGEN_NMUX2:
    case DPGEN_INV:
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
      fprintf (LV_FILE, "%14s :   out BIT_VECTOR %s;\n", "nq", sBusWide);
      break;
    case DPGEN_NBUSE:
      fprintf (LV_FILE, "%14s :   out MUX_VECTOR %s BUS;\n", "nq", sBusWide);
      break;
    case DPGEN_AND2:
    case DPGEN_AND3:
    case DPGEN_AND4:
    case DPGEN_OR2:
    case DPGEN_OR3:
    case DPGEN_OR4:
    case DPGEN_XOR2:
    case DPGEN_MUX2:
    case DPGEN_BUFF:
    case DPGEN_CONST:
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
      fprintf (LV_FILE, "%14s :   out BIT_VECTOR %s;\n", "q", sBusWide);
      break;
    case DPGEN_BUSE:
      fprintf (LV_FILE, "%14s :   out MUX_VECTOR %s BUS;\n", "q", sBusWide);
      break;
  }


  /* Power supplies terminals. */
  fprintf (LV_FILE, "%14s : in    BIT;\n", "vdd");
  fprintf (LV_FILE, "%14s : in    BIT\n" , "vss");


  /* End of entity description. */
  fprintf (LV_FILE, "  );\nEND %s;\n\n\n", LV_modelName);
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_signal ()".
 *
 *  Internal VHDL signals.
 */

static void  VHDL_signal ()
{
  char  sBusWide[SIZE_SNAME];


  VHDL_BUS_WIDE (sBusWide, LV_N - 1, 0);

  switch (LV_Function) {
    case DPGEN_MUX2:
    case DPGEN_BUSE:
    case DPGEN_NBUSE:
      break;
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
    case DPGEN_NMUX2:
      fprintf (LV_FILE, "  SIGNAL %20s : BIT_VECTOR %s;\n", "q", sBusWide);
      break;
    case DPGEN_DFFT:
    case DPGEN_SFFT:
      fprintf (LV_FILE, "  SIGNAL %20s : BIT_VECTOR %s;\n",
               "d_test", sBusWide);
    case DPGEN_DFF:
    case DPGEN_SFF:
      fprintf (LV_FILE, "  SIGNAL %20s : BIT_VECTOR %s;\n",
               "d_wen", sBusWide);
      fprintf (LV_FILE, "  SIGNAL %20s : REG_VECTOR %s REGISTER;\n",
               "ff", sBusWide);
      break;
  }

  fprintf (LV_FILE, "\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_boolean ()".
 *
 *  Data-flow equations for booleans operators.
 */

static void  VHDL_boolean ()
{
  switch (LV_Function) {
    case DPGEN_INV:
      fprintf (LV_FILE, "  nq <= not i0;\n\n"); break;
    case DPGEN_BUFF:
      fprintf (LV_FILE, "   q <= i0;\n\n"); break;
    case DPGEN_NAND2:
      fprintf (LV_FILE, "  nq <= not (i0 and i1);\n\n"); break;
    case DPGEN_AND2:
      fprintf (LV_FILE, "   q <= (i0 and i1);\n\n"); break;
    case DPGEN_NOR2:
      fprintf (LV_FILE, "  nq <= not (i0 or i1);\n\n"); break;
    case DPGEN_OR2:
      fprintf (LV_FILE, "   q <= (i0 or i1);\n\n"); break;
    case DPGEN_XNOR2:
      fprintf (LV_FILE, "  nq <= not (i0 xor i1);\n\n"); break;
    case DPGEN_XOR2:
      fprintf (LV_FILE, "   q <= (i0 xor i1);\n\n"); break;
    case DPGEN_NOR3:
      fprintf (LV_FILE, "  nq <= not (i0 or i1 or i2);\n\n"); break;
    case DPGEN_OR3:
      fprintf (LV_FILE, "   q <= (i0 or i1 or i2);\n\n"); break;
    case DPGEN_NAND3:
      fprintf (LV_FILE, "  nq <= not (i0 and i1 and i2);\n\n"); break;
    case DPGEN_AND3:
      fprintf (LV_FILE, "   q <= (i0 and i1 and i2);\n\n"); break;
    case DPGEN_NAND4:
      fprintf (LV_FILE, "  nq <= not (i0 and i1 and i2 and i3);\n\n"); break;
    case DPGEN_AND4:
      fprintf (LV_FILE, "   q <= (i0 and i1 and i2 and i3);\n\n"); break;
    case DPGEN_NOR4:
      fprintf (LV_FILE, "  nq <= not (i0 or i1 or i2 or i3);\n\n"); break;
    case DPGEN_OR4:
      fprintf (LV_FILE, "   q <= (i0 or i1 or i2 or i3);\n\n"); break;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_mask ()".
 *
 *  Data-flow equations for programmable masks operators.
 */

static void  VHDL_mask ()
{
  char sConst[SIZE_SNAME];


  fprintf(LV_FILE, "  WITH  cmd  SELECT\n");
  fprintf(LV_FILE, "    q <=  i0  WHEN \'0\',\n");

  switch (LV_Function) {
    case DPGEN_NAND2MASK:
      fprintf(LV_FILE,
              "          (i0 and %s)  WHEN \'1\';\n",
              xltovhdl (LV_xl, sConst));
      break;
    case DPGEN_NOR2MASK:
      fprintf(LV_FILE,
              "          (i0 or %s) WHEN \'1\';\n",
              xltovhdl (LV_xl, sConst));
      break;
    case DPGEN_XNOR2MASK:
      fprintf(LV_FILE,
              "          (i0 xor %s) WHEN \'1\';\n",
              xltovhdl (LV_xl, sConst));
      break;
  }

  fprintf(LV_FILE, "\n  nq <= not q;\n\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_const ()".
 *
 *  Data-flow equations for constants operators.
 */

static void  VHDL_const ()
{
  char sConst[SIZE_SNAME];


  fprintf(LV_FILE, "  q <= %s;\n\n", xltovhdl (LV_xl, sConst));
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_mux ()".
 *
 *  Data-flow equations for multiplexer operators.
 */

static void  VHDL_mux ()
{
  switch (LV_Function) {
    case DPGEN_NMUX2:
    case DPGEN_MUX2:
      fprintf(LV_FILE,"  WITH  cmd  SELECT\n" );
      fprintf(LV_FILE,"    q <= i0  WHEN  \'0\',\n");
      fprintf(LV_FILE,"         i1  WHEN  \'1\';\n" );
      break;
  }

  switch (LV_Function) {
    case DPGEN_NMUX2:
      fprintf(LV_FILE, "\n  nq <= not q;\n\n");
      break;
    case DPGEN_MUX2:
      fprintf(LV_FILE, "\n\n");
      break;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_buse ()".
 *
 *  Data-flow equations for three-states operators.
 */

static void  VHDL_buse ()
{
  fprintf (LV_FILE, "  label_buse:BLOCK(cmd = '1')\n");
  fprintf (LV_FILE, "  BEGIN\n");

  switch (LV_Function) {
    case DPGEN_NBUSE:
      fprintf (LV_FILE, "    nq <= GUARDED not i0;\n");
      break;
    case DPGEN_BUSE:
      fprintf (LV_FILE, "    q <= GUARDED i0;\n");
      break;
  }

  fprintf (LV_FILE, "  END BLOCK label_buse;\n\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_ff ()".
 *
 *  Data-flow equations for registers operators.
 */

static void  VHDL_ff ()
{
  char  sBusWide[SIZE_SNAME], *dwrite;


  VHDL_BUS_WIDE (sBusWide, LV_N - 2, 0);

  dwrite = "d_wen";

  switch (LV_Function) {
    case DPGEN_DFFT:
    case DPGEN_SFFT:
      fprintf(LV_FILE,"  WITH  scan  SELECT\n" );
      fprintf(LV_FILE,"    d_test <= d_wen  WHEN  \'0\',\n");
      fprintf(LV_FILE,"              q %s & scin  WHEN  \'1\';\n\n",
              sBusWide);

      dwrite = "d_test";
    case DPGEN_DFF:
    case DPGEN_SFF:
      fprintf(LV_FILE,"  WITH  wen  SELECT\n" );
      fprintf(LV_FILE,"    d_wen <=  q  WHEN  \'0\',\n");
      fprintf(LV_FILE,"             i0  WHEN  \'1\';\n\n" );
      break;
  }

  fprintf (LV_FILE, "  label_ff:BLOCK ((ck and not (ck'STABLE)) = \'1\')\n");
  fprintf (LV_FILE, "  BEGIN\n");
  fprintf (LV_FILE, "    ff <= GUARDED %s;\n", dwrite);
  fprintf (LV_FILE, "  END BLOCK label_ff;\n\n");

  fprintf (LV_FILE, "  q <= ff;\n\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "VHDL_archi ()".
 *
 *  Create the ARCHITECTURE description of the operator. This description
 *  use the VHDL data-flow subset.
 */

static void  VHDL_archi ()
{
  fprintf (LV_FILE, "ARCHITECTURE VBE OF %s IS\n\n", LV_modelName);

  VHDL_signal ();

  fprintf (LV_FILE, "BEGIN\n\n");


  switch (LV_Function) {
    case DPGEN_NAND4:
    case DPGEN_AND4:
    case DPGEN_NOR4:
    case DPGEN_OR4:
    case DPGEN_NAND3:
    case DPGEN_OR3:
    case DPGEN_AND3:
    case DPGEN_NOR3:
    case DPGEN_NAND2:
    case DPGEN_AND2:
    case DPGEN_NOR2:
    case DPGEN_OR2:
    case DPGEN_XNOR2:
    case DPGEN_XOR2:
    case DPGEN_INV:
    case DPGEN_BUFF:
      VHDL_boolean ();
      break;
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
      VHDL_mask ();
      break;
    case DPGEN_NMUX2:
    case DPGEN_MUX2:
      VHDL_mux ();
      break;
    case DPGEN_BUSE:
    case DPGEN_NBUSE:
      VHDL_buse ();
      break;
    case DPGEN_CONST:
      VHDL_const ();
      break;
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
      VHDL_ff ();
      break;
  }


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


  /* End of behavioral view. */
  fprintf (LV_FILE, "END VBE;\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "create_VHDL ()".
 */

static void  create_VHDL ()
{
  LV_FILE = mbkfopen (LV_modelName, "vbe", WRITE_TEXT);

  if (!LV_FILE) {
    eprinth ("*** dpgen ***");
    eprintf ("\n  Unable to create file \"%s.vbe\".\n", LV_modelName);
    EXIT (1);
  }


  VHDL_entity ();
  VHDL_archi ();


  fclose (LV_FILE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "dpgen_Regular ()".
 */

extern void  dpgen_Regular (aFunction, aAL)
     long  aFunction;
  va_list  aAL;
{
  long  iDrive;


  /* Parses the mandatory arguments. */
  LV_modelName = va_arg (aAL, char*);
  LV_flags     = va_arg (aAL, long );
  LV_N         = va_arg (aAL, long );

  __DBG (fprintf (stderr, "LV_flags := %ld.\n", LV_flags);)


  /* Initializes for the requested generator. */
  init_Generator (aFunction);


 
  LV_drive      = 1;
  LV_macroDrive = 0;

  /* Parses the optionals arguments. */
  switch (LV_Function) {
    case DPGEN_INV:
    case DPGEN_BUFF:
    case DPGEN_NAND2:
    case DPGEN_NAND3:
    case DPGEN_NAND4:
    case DPGEN_AND2:
    case DPGEN_AND3:
    case DPGEN_AND4:
    case DPGEN_NOR2:
    case DPGEN_NOR3:
    case DPGEN_NOR4:
    case DPGEN_OR2:
    case DPGEN_OR3:
    case DPGEN_OR4:
    case DPGEN_XNOR2:
    case DPGEN_XOR2:
    case DPGEN_MUX2:
    case DPGEN_BUSE:
      LV_drive = va_arg (aAL, long);
      break;
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
    case DPGEN_CONST:
      LV_xl = newxl ();
      atoxl (LV_xl, va_arg (aAL, char*));
      LV_xl->size = LV_N;
      break;
    case DPGEN_NMUX2:
    case DPGEN_NBUSE:
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
  }


  /* Check the drive argument. */
  for (iDrive = 0; iDrive < MAX_TDRIVE; iDrive++) {
    if  (LV_tDrive[iDrive] && (LV_tDrive[iDrive] == LV_drive))
      { LV_macroDrive = iDrive; break; }
  }
  if  (iDrive >= MAX_TDRIVE) {
    eprinth (NULL);
    eprintf ("  GENLIB_MACRO ( %s, ...): Invalid drive value %ld.\n",
             function_To_String (aFunction), LV_drive);
    EXIT (1);
  }


  /* Always create the netlist. */
  create_Netlist ();

  if (LV_flags & F_PLACE) create_Layout ();
  if (LV_flags & F_BEHAV) create_VHDL ();
}
