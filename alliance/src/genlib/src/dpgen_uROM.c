/*
 *  $Id: dpgen_uROM.c,v 1.1 2002/04/29 13:40:54 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Module    :         "./dpgen_uROM.c"                          |
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

# define        ERROR_INDEX     4096

# define        CELLS_ROM2         5
# define        CELLS_ROM4        17

# define        SIZE_SNAME      1024



/*  ------------------------------------------------------------------
 *  Local variables  (prefix "LV_").
 */

  static           FILE *LV_FILE;
  static           long  LV_Function;
  static           char *LV_modelName;
  static struct xlong_s *LV_xl0;
  static struct xlong_s *LV_xl1;
  static struct xlong_s *LV_xl2;
  static struct xlong_s *LV_xl3;
  static           long  LV_flags;
  static           long  LV_N;
  static           long  LV_iLeafCell;
  static           char *LV_tsLeafCell[] = {
    /* ROM2 generators leaf cells. */
    "dp_rom2_buf", "zero_x0", "buf_x4", "inv_x4", "one_x0",
    /* ROM4 generators leaf cells. */
    "dp_rom4_buf",
    "zero_x0",           "a2_x4",          "a2_x4", "buf_x4",
      "a2_x4",          "buf_x4", "dp_rom4_xr2_x4",  "o2_x4",
      "a2_x4", "dp_rom4_nxr2_x4",         "buf_x4",  "o2_x4",
     "buf_x4",           "o2_x4",          "o2_x4", "one_x0"
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

  static long  set_iLeafCell    __FP((void));
  static void  init_Generator   __FP((long aFunction));
  static void  create_Netlist   __FP((void));
  static void  create_Layout    __FP((void));
  static void  create_VHDL      __FP((void));
  static long  get_index_Cell   __FP((long aiSlice));
  static char *get_model_Cell   __FP((long aiSlice, long aiCell));
  static char *get_model_Buffer __FP((void));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "set_iLeafCell ()".
 */

static long  set_iLeafCell ()
{
  LV_iLeafCell = 0;

  switch (LV_Function) {
    case DPGEN_ROM4: LV_iLeafCell += CELLS_ROM2;
    case DPGEN_ROM2: 
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

  set_iLeafCell ();
}


/*  ------------------------------------------------------------------
 *  Function  :  "get_index_Cell ()".
 */

static long  get_index_Cell (aiSlice)
  long  aiSlice;
{
  long  index;


  index = ERROR_INDEX;

  switch (LV_Function) {
    case DPGEN_ROM2:
      __DBG(
        fprintf (stderr, "LV_xl0(%ld) := %ld.\n",
                 aiSlice, getxlbit (LV_xl0, aiSlice));
        fprintf (stderr, "LV_xl1(%ld) := %ld.\n",
                 aiSlice, getxlbit (LV_xl1, aiSlice));
        fflush (stderr);
      )
      index = (getxlbit (LV_xl1, aiSlice) << 1)
            +  getxlbit (LV_xl0, aiSlice);
      break;
    case DPGEN_ROM4:
      index = (getxlbit (LV_xl3, aiSlice) << 3)
            + (getxlbit (LV_xl2, aiSlice) << 2)
            + (getxlbit (LV_xl1, aiSlice) << 1)
            +  getxlbit (LV_xl0, aiSlice);
      break;
  }


  return (index);
}


/*  ------------------------------------------------------------------
 *  Function  :  "get_model_Cell ()".
 */

static char *get_model_Cell (aiSlice, aiCell)
  long  aiSlice, aiCell;
{
  long  index;


  index = ERROR_INDEX;

  switch (LV_Function) {
    case DPGEN_ROM4:
    case DPGEN_ROM2:
      index = LV_iLeafCell + 1 + aiCell;
      break;
  }

  if  (index >= ERROR_INDEX) return ("bug_leaf");

  __DBG(
    fprintf (stderr,
             "bit := %2ld, ",
             SLICE_TO_BIT(aiSlice, LV_N, LV_flags & F_MSB_FIRST));
    fprintf (stderr,
             "aiCell := %3ld, name := %s\n",
             aiCell,
             LV_tsLeafCell[ index ]);
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
    case DPGEN_ROM2:
    case DPGEN_ROM4:
      index = LV_iLeafCell;
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
  char *sModel, *sI0, *sI1;
  long  iSlice, iCell;


  GENLIB_DEF_LOFIG (LV_modelName);

  /* Control terminals. */
  switch (LV_Function) {
    case DPGEN_ROM4:
      GENLIB_LOCON ("sel1", IN, "sel1");
    case DPGEN_ROM2:
      GENLIB_LOCON ("sel0", IN, "sel0");
      break;
  }


  /* Output data buses terminals. */
  DPGEN_LOCON_BUS ("q", OUT);


  /* Power terminals. */
  GENLIB_LOCON ("vdd", IN, "vdd");
  GENLIB_LOCON ("vss", IN, "vss");


  /* Loop for all the data slices. */
  for (iSlice = 0; iSlice < LV_N; iSlice++) {
    iCell  = get_index_Cell (iSlice);
    sModel = get_model_Cell (iSlice, iCell);
    sprintf (sInstance, "cell_%ld", iSlice);

    switch (LV_Function) {
      case DPGEN_ROM4:
        sI0 = "i0x";
        sI1 = "i1x";

        /* Select the inputs. */
        switch (iCell) {
          case 0:
          case 15:
            break;
          case 8:
          case 9:
          case 10:
          case 14:
            sI0 = "i0x"; sI1 = "i1x";
            break;
          case 2:
          case 11:
            sI0 = "i0x"; sI1 = "ni1x";
            break;
          case 4:
          case 13:
            sI0 = "ni0x"; sI1 = "i1x";
            break;
          case 1:
          case 5:
          case 6:
          case 7:
            sI0 = "ni0x"; sI1 = "ni1x";
            break;
          case 3:
            sI0 = "ni1x";
            break;
          case 12:
            sI0 = "i1x";
            break;
        }

        /* Select the gate. */
        if (!strncmp (sModel, "zero", 4)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ("nq => q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else if (!strncmp (sModel, "one", 3)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ("q => q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else if (!strncmp (sModel, "buf", 3)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ("i => %s"    , sI0),
            s64printf ("q => q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else if (!strncmp (sModel, "inv", 3)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ( "i => %s"    , sI0),
            s64printf ("nq => q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else if (   !strncmp (sModel, "a2", 2)
                   || !strncmp (sModel, "o2", 2)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
            s64printf ("i0 => %s"    , sI0),
            s64printf ("i1 => %s"    , sI1),
            s64printf ( "q => q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        } else if (   !strncmp (sModel, "dp_rom4_xr2" , 11)
                   || !strncmp (sModel, "dp_rom4_nxr2", 12)) {
          GENLIB_LOINSE (
            sModel,
            sInstance,
             "i0x =>  i0x",
             "i1x =>  i1x",
            "ni0x => ni0x",
            "ni1x => ni1x",
            s64printf ( "q => q[%ld]", DPGEN_PORT_BIT (iSlice)),
            "vdd => vdd",
            "vss => vss", 0L);
        }
        break;

      case DPGEN_ROM2:
        switch (iCell) {
          case 0:
            GENLIB_LOINSE (
              sModel,
              sInstance,
              s64printf ("nq => q[%ld]", DPGEN_PORT_BIT (iSlice)),
              "vdd => vdd",
              "vss => vss", 0L);
            break;
          case 3:
            GENLIB_LOINSE (
              sModel,
              sInstance,
              s64printf ("q => q[%ld]", DPGEN_PORT_BIT (iSlice)),
              "vdd => vdd",
              "vss => vss", 0L);
            break;
          case 1:
            GENLIB_LOINSE (
              sModel,
              sInstance,
              "i => ni0x",
              s64printf ("q => q[%ld]", DPGEN_PORT_BIT (iSlice)),
              "vdd => vdd",
              "vss => vss", 0L);
            break;
          case 2:
            GENLIB_LOINSE (
              sModel,
              sInstance,
              "i => ni0x",
              s64printf ("nq => q[%ld]", DPGEN_PORT_BIT (iSlice)),
              "vdd => vdd",
              "vss => vss", 0L);
            break;
        }
        break;

    }
  } /* End of "iSlice"  (data slices) loop. */


  /* Add the buffer. */
  if (iSlice % 2) iSlice++;

  sModel = get_model_Buffer ();
  sprintf (sInstance, "cell_%ld", iSlice);

  switch (LV_Function) {
    case DPGEN_ROM2:
      GENLIB_LOINSE (
        sModel,
        sInstance,
          "i => sel0",
        "nix => ni0x",
        "vdd => vdd",
        "vss => vss", 0L);
      break;
    case DPGEN_ROM4:
      GENLIB_LOINSE (
        sModel,
        sInstance,
          "i0 => sel0",
          "i1 => sel1",
         "i0x => i0x",
         "i1x => i1x",
        "ni0x => ni0x",
        "ni1x => ni1x",
         "vdd => vdd",
         "vss => vss", 0L);
      break;
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

    sModel = get_model_Cell (iSlice, get_index_Cell (iSlice));
    sprintf (sInstance, "cell_%ld", iSlice);

    GENLIB_PLACE (sModel,
                  sInstance,
                  SLICE_TO_SYM (iSlice, LV_N),
                  0,
                  SLICE_TO_Y (iSlice, LV_N));
  } /* End of iSlice loop. */


  /* Add the buffer cell. */
  if (iSlice % 2) iSlice++;

  sModel = get_model_Buffer ();
  sprintf (sInstance, "cell_%ld", iSlice);

  GENLIB_PLACE (sModel,
                sInstance,
                SLICE_TO_SYM (iSlice, LV_N),
                0,
                SLICE_TO_Y (iSlice, LV_N));

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


  LV_FILE = mbkfopen (LV_modelName, "vbe", WRITE_TEXT);

  if (!LV_FILE) {
    eprinth ("*** dpgen ***");
    eprintf ("\n  Unable to create file \"%s.vbe\".\n", LV_modelName);
    EXIT (1);
  }


  VHDL_BUS_WIDE (sBusWide, LV_N - 1, 0);


  /* Entity description. */
  fprintf (LV_FILE, "\nENTITY %s IS\n  PORT (\n", LV_modelName);

  /* Controls Terminals. */
  switch (LV_Function) {
    case DPGEN_ROM4:
      fprintf (LV_FILE, "%14s : in    BIT;\n", "sel1");
    case DPGEN_ROM2:
      fprintf (LV_FILE, "%14s : in    BIT;\n", "sel0");
      break;
  }

  /* Output data bus. */
  fprintf (LV_FILE, "%14s :   out BIT_VECTOR %s;\n", "q", sBusWide);

  /* Power supplies terminals. */
  fprintf (LV_FILE, "%14s : in    BIT;\n", "vdd");
  fprintf (LV_FILE, "%14s : in    BIT\n" , "vss");

  /* End of entity description. */
  fprintf (LV_FILE, "  );\nEND %s;\n\n\n", LV_modelName);


  /* Architecture description. */
  fprintf (LV_FILE, "ARCHITECTURE VBE OF %s IS\n\n", LV_modelName);


  fprintf (LV_FILE, "BEGIN\n\n");

  switch (LV_Function) {
    case DPGEN_ROM4:
      fprintf (LV_FILE,"  WITH  sel1 & sel0  SELECT\n" );
      fprintf (LV_FILE,
               "    q <= %s WHEN B\"00\",\n"  , xltovhdl (LV_xl0, sConst));
      fprintf (LV_FILE,
               "         %s WHEN B\"01\",\n"  , xltovhdl (LV_xl1, sConst));
      fprintf (LV_FILE,
               "         %s WHEN B\"10\",\n"  , xltovhdl (LV_xl2, sConst));
      fprintf (LV_FILE,
               "         %s WHEN B\"11\";\n\n", xltovhdl (LV_xl3, sConst));
      break;
    case DPGEN_ROM2:
      fprintf (LV_FILE,"  WITH  sel0  SELECT\n" );
      fprintf (LV_FILE,
               "    q <= %s WHEN  \'0\',\n"  , xltovhdl (LV_xl0, sConst));
      fprintf (LV_FILE,
               "         %s WHEN  \'1\';\n\n", xltovhdl (LV_xl1, sConst));
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


  /* End of architecture description. */
  fprintf (LV_FILE, "END VBE;\n");


  fclose (LV_FILE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "dpgen_uROM ()".
 */

extern void  dpgen_uROM (aFunction, aAL)
     long  aFunction;
  va_list  aAL;
{
  /* Parses the mandatory arguments. */
  LV_modelName = va_arg (aAL, char*);
  LV_flags     = va_arg (aAL, long );
  LV_N         = va_arg (aAL, long );

  __DBG (fprintf (stderr, "LV_flags := %ld.\n", LV_flags);)


  /* Initializes for the requested generator. */
  init_Generator (aFunction);

 
  /* Parses the constants. */
  LV_xl0 = newxl ();
  atoxl (LV_xl0, va_arg (aAL, char*));
  LV_xl0->size = LV_N;

  LV_xl1 = newxl ();
  atoxl (LV_xl1, va_arg (aAL, char*));
  LV_xl1->size = LV_N;

  if (LV_Function == DPGEN_ROM4) {
    LV_xl2 = newxl ();
    atoxl (LV_xl2, va_arg (aAL, char*));
    LV_xl2->size = LV_N;

    LV_xl3 = newxl ();
    atoxl (LV_xl3, va_arg (aAL, char*));
    LV_xl3->size = LV_N;
  }


  /* Always create the netlist. */
  create_Netlist ();

  if (LV_flags & F_PLACE) create_Layout ();
  if (LV_flags & F_BEHAV) create_VHDL ();
}
