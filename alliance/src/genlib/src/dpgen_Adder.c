/*
 *  $Id: dpgen_Adder.c,v 1.1 2002/04/29 13:40:54 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |          R e c u r r e n c e   S o l v e r                     |
 *  |                                                                |
 *  |  Authors   :                         Alain HOUELLE             |
 *  |                                    Nicolas VAUCHER             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Module    :         "./dpgen_Adder.c"                         |
 *  | ************************************************************** |
 *  |                                                                |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include   "util_Defs.h"
# include  "dpgen_Defs.h"
# include  "dgn.h"


/*  ------------------------------------------------------------------
 *  Local constants.
 */

# define        SIZE_SNAME      1024


/*  ------------------------------------------------------------------
 *  Local variables  (prefix "LV_").
 */

  static           long  LV_function;
  static           char *LV_modelName;
  static           FILE *LV_FILE;
  static           long  LV_adderFlags;
  static           long  LV_flags;
  static           long  LV_N;


/*  ------------------------------------------------------------------
 *  Internal macros declarations.
 */


/*  ------------------------------------------------------------------
 *  Internal function declarations.
 */

  static void  RSA_Layout  __FP((void));
  static void  RSA_netlist __FP((void));
  static void  RSA_Behav   __FP((void));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */


/*  ------------------------------------------------------------------
 *  Function  :  "RSA_Layout ()".
 */

static void  RSA_Layout ()
{
  int  pi, i, n, posy;
  int  twostage = -1;
 

  GENLIB_DEF_PHFIG (LV_modelName);
 

  /* Placement ligne des XOR */
  for (i = 0; i < LV_N; i++) {
    DPGEN_PLACE ("xr2_x1",       GENLIB_NAME ("XORB%d", i), -45, i * 50);
  }


  /* Placement ligne des PI GI */
  for (i = 1; i < LV_N; i++) {
    DPGEN_PLACE ("halfadder_x2", GENLIB_NAME ("PIGI%d", i),   0, i * 50);
  }
 

  /* Placement ligne des PG */
  for (n = 1; n <= ilog2 (LV_N); n += 2) {
    twostage++;

    for(pi = i = LV_N - 1; i > 0 ; i--) {
      if((i >= iexp2 (n) + iexp2 (n - 1)) && (i & (1 << (n - 1)))) {
        DPGEN_PLACE ("a2_x2",
                     GENLIB_NAME ("PG_i1_%d_%d", n, i),
                     80 + twostage * 55, // (n + 1)
                     50 * pi);

        DPGEN_PLACE ("oa22_x2",
                     GENLIB_NAME ("PG_i2_%d_%d", n, i),
                     80 + 25 + twostage * 55, // (n + 1)
                     50 * pi--);
      }

      if((i >= iexp2 (n + 1) + iexp2 (n)) && (i & (1 << (n)))) {
        DPGEN_PLACE ("a2_x2",
                     GENLIB_NAME ("PG_i1_%d_%d", n + 1, i),
                     80 + twostage * 55, // (n + 1)
                     50 * pi);

        DPGEN_PLACE ("oa22_x2",
                     GENLIB_NAME("PG_i2_%d_%d", n + 1, i),
                     80 + 25 + twostage * 55, // (n + 1)
                     50 * pi--);
      }
    }
  }


  posy = n;


  /* LV_N n'est pas une puissance entiere de 2. */
  if (LV_N % (iexp2 (ilog2 (LV_N) - 1))) twostage++;
 

  /* Placement ligne retenue */
  for (n = 2 ; n <= LV_N ; n++) {
    DPGEN_PLACE ("oa22_x2",
                 GENLIB_NAME ("R%d", n),
                 80 + 55 * (twostage),
                 (n - 1) * 50);
  }

 
  /* Placement ligne somme */
  for(n = 1; n < LV_N; n++) {
    DPGEN_PLACE ("xr2_x1" ,
                 GENLIB_NAME ("S%d", n),
                 80 + 30 + 55 * (twostage) ,
                 (n - 1) * 50);
  }

  DPGEN_PLACE ("fulladder_x2", GENLIB_NAME ("S%d", 0), 0, 0 * 50);


  GENLIB_DEF_AB (0, 0, 0, 0);

  GENLIB_SAVE_PHFIG ();
}


/*  ------------------------------------------------------------------
 *  Function  :  "RSA_Netlist ()".
 */

static void RSA_netlist ()
{
  int  i, n, isr, ipg, npg, inpg;


  GENLIB_DEF_LOFIG (LV_modelName);


  GENLIB_LOCON ("add_sub", IN   , "add_sub");
  GENLIB_LOCON ("c31"    ,   OUT, GENLIB_NAME ("R_%d", LV_N    )); /* cout. */
  GENLIB_LOCON ("c30"    ,   OUT, GENLIB_NAME ("R_%d", LV_N - 1)); /* over. */
    
  GENLIB_LOCON (    FULL_BUS ("i0" , LV_N, (LV_flags & F_MSB_FIRST)),
                IN, FULL_BUS ("INA", LV_N, (LV_flags & F_MSB_FIRST)));

  GENLIB_LOCON (    FULL_BUS ("i1" , LV_N, (LV_flags & F_MSB_FIRST)),
                IN, FULL_BUS ("INB", LV_N, (LV_flags & F_MSB_FIRST)));

  GENLIB_LOCON (     FULL_BUS ("q" , LV_N, (LV_flags & F_MSB_FIRST)),
                OUT, FULL_BUS ("S" , LV_N, (LV_flags & F_MSB_FIRST)));

  GENLIB_LOCON ("vdd", IN, "vdd");
  GENLIB_LOCON ("vss", IN, "vss");


  /* Ligne des XOR. */
  for (i = 0; i < LV_N; i++) {
    GENLIB_LOINS ("xr2_x1", GENLIB_NAME ("XORB%d", i),
                  "add_sub",
                  GENLIB_ELM ("INB" , i),
                  GENLIB_ELM ("XORB", i),
                  "VDD", "VSS", 0);
  }


  /* Ligne des PI GI */
  for (i = 1; i < LV_N; i++) {
    GENLIB_LOINS ("halfadder_x2", GENLIB_NAME ("PIGI%d", i),
                  GENLIB_ELM ("INA" , i),
                  GENLIB_ELM ("XORB", i),
                  GENLIB_NAME ("GI_0_%d", i),
                  GENLIB_NAME ("PI_0_%d", i),
                  "VDD", "VSS", 0);
  }
    

  /* Calcul somme et retenue du LSB */
  GENLIB_LOINS ("fulladder_x2", "S0",
                GENLIB_ELM ("INA",0),
                GENLIB_ELM ("INA",0),
                GENLIB_ELM ("INA",0),
                GENLIB_ELM ("INA",0),
                GENLIB_ELM ("XORB",0),
                GENLIB_ELM ("XORB",0),
                GENLIB_ELM ("XORB",0),
                GENLIB_ELM ("XORB",0),
                "add_sub",
                "add_sub",
                "add_sub",
                "R_1",
                GENLIB_ELM ("S", 0),
                "VDD", "VSS", 0);
    

  for (n = 1; n <= ilog2 (LV_N); n++) {
    /* Calcul des retenues (carry) */
    for (isr = 0; isr < iexp2 (n - 1) && iexp2 (n - 1) + isr < LV_N; isr++) {
      GENLIB_LOINS ("oa22_x2",
                    GENLIB_NAME ("R%d"     , iexp2 (n - 1) + isr + 1),
                    GENLIB_NAME ("PI_%d_%d", ilog2 (isr), iexp2 (n - 1 ) + isr),
                    GENLIB_NAME ("R_%d"    , iexp2 (n - 1)),
                    GENLIB_NAME ("GI_%d_%d", ilog2 (isr), iexp2(n - 1) + isr),
                    GENLIB_NAME ("R_%d"    , iexp2 (n - 1) + isr + 1),
                    "VDD", "VSS", 0);
    }

    /* Calcul des PI GI intermediaires (PG) */
    for (ipg = iexp2 (n) + iexp2 (n - 1); ipg < LV_N; ipg += iexp2 (n)) {
      for(npg = 0, inpg = ipg;
          npg < iexp2 (n - 1) && inpg < LV_N; npg++, inpg++) {
        GENLIB_LOINS("a2_x2",
                     GENLIB_NAME ("PG_i1_%d_%d", n, inpg),
                     GENLIB_NAME ("PI_%d_%d"   , n - 1, ipg - 1),
                     GENLIB_NAME ("PI_%d_%d"   , ilog2 (npg), inpg),
                     GENLIB_NAME ("PI_%d_%d"   , n, inpg),
                     "VDD", "VSS", 0);
                        
        GENLIB_LOINS("oa22_x2",
                     GENLIB_NAME ("PG_i2_%d_%d", n, inpg),
                     GENLIB_NAME ("PI_%d_%d"   , ilog2 (npg), inpg),
                     GENLIB_NAME ("GI_%d_%d"   , n - 1, ipg - 1),
                     GENLIB_NAME ("GI_%d_%d"   , ilog2 (npg), inpg),
                     GENLIB_NAME ("GI_%d_%d"   , n, inpg),
                     "VDD", "VSS", 0);
      }
    }
  } 


  /* Calcul des Sommes (sum) */
  for (i = 1; i < LV_N; i++) {
    GENLIB_LOINS("xr2_x1",
                 GENLIB_NAME ("S%d",     i),
                 GENLIB_NAME ("PI_0_%d", i),
                 GENLIB_NAME ("R_%d",    i),
                 GENLIB_ELM  ("S",       i),
                 "VDD","VSS",0);
  }


  GENLIB_SAVE_LOFIG();
}


/*  ------------------------------------------------------------------
 *  Function  :  "RSA_Behav ()".
 */

static void  RSA_Behav ()
{
  char  sBusWide0[SIZE_SNAME];
  char  sBusWide1[SIZE_SNAME];
  char  sBusWide2[SIZE_SNAME];
  long  i;


  LV_FILE = mbkfopen (LV_modelName, "vbe", WRITE_TEXT);

  if (!LV_FILE) {
    eprinth ("*** dpgen ***");
    eprintf ("\n  Unable to create file \"%s.vbe\".\n", LV_modelName);
    EXIT (1);
  }


  VHDL_BUS_WIDE (sBusWide0, LV_N - 1, 0);
  VHDL_BUS_WIDE (sBusWide1, LV_N    , 1);
  VHDL_BUS_WIDE (sBusWide2, LV_N    , 0);


  /* Entity description. */
  fprintf (LV_FILE, "\nENTITY %s IS\n  PORT (\n", LV_modelName);

  fprintf (LV_FILE, "%14s : in    BIT;\n", "add_sub");
  fprintf (LV_FILE, "%14s :   out BIT;\n", "c31");
  fprintf (LV_FILE, "%14s :   out BIT;\n", "c30");
  fprintf (LV_FILE, "%14s : in    BIT_VECTOR %s;\n", "i0", sBusWide0);
  fprintf (LV_FILE, "%14s : in    BIT_VECTOR %s;\n", "i1", sBusWide0);
  fprintf (LV_FILE, "%14s :   out BIT_VECTOR %s;\n",  "q", sBusWide0);
  fprintf (LV_FILE, "%14s : in    BIT;\n", "vdd");
  fprintf (LV_FILE, "%14s : in    BIT\n" , "vss");

  fprintf (LV_FILE, "  );\nEND %s;\n\n\n", LV_modelName);


  /* Architecture description. */
  fprintf (LV_FILE, "ARCHITECTURE VBE OF %s IS\n\n", LV_modelName);

  fprintf (LV_FILE, "  SIGNAL %20s : BIT_VECTOR %s;\n", "carry", sBusWide2);
  fprintf (LV_FILE, "  SIGNAL %20s : BIT_VECTOR %s;\n", "xor_b", sBusWide0);

  fprintf (LV_FILE, "\nBEGIN\n\n");


  for (i = LV_N - 1; i >= 0; i--)
    fprintf (LV_FILE, "  xor_b(%ld) <= i1(%ld) xor add_sub;\n", i, i);
  fprintf (LV_FILE, "\n");

  fprintf (LV_FILE, "  carry(0) <= add_sub;\n\n");


  fprintf (LV_FILE, "  q <=     i0\n");
  fprintf (LV_FILE, "       xor xor_b\n");
  fprintf (LV_FILE, "       xor carry%s;\n\n", sBusWide0);

  fprintf (LV_FILE, "  c31 <=  carry(%ld);\n"  , LV_N);
  fprintf (LV_FILE, "  c30 <=  carry(%ld);\n\n", LV_N - 1);

  fprintf (LV_FILE, "  carry%s <=      (   i0 and xor_b)\n"       , sBusWide1);
  fprintf (LV_FILE, "               or (   i0 and carry%s)\n"     , sBusWide0);
  fprintf (LV_FILE, "               or (xor_b and carry%s);\n\n\n", sBusWide0);


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


  /* End of Architectural description. */
  fprintf (LV_FILE, "END VBE;\n");


  fclose (LV_FILE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "dpgen_Adder ()".
 */

extern void  dpgen_Adder (aFunction, aFlags, aAL)
      long  aFunction;
      long  aFlags;
   va_list  aAL;
{
  LV_function   = aFunction;
  LV_adderFlags = aFlags;
  LV_modelName  = va_arg (aAL, char*);
  LV_flags      = va_arg (aAL, long );
  LV_N          = va_arg (aAL, long );


  RSA_netlist ();
  if (LV_flags & F_PLACE) RSA_Layout ();
  if (LV_flags & F_BEHAV) RSA_Behav  ();
}
