

/*          Alliance CAD System V.R                                         */
/*             FpGen Package V.RR                                           */
/*                                                                          */
/*  Copyright(c) 93-AA, MASI, CAO-VLSI Team                                 */
/*                                                                          */
/*  Author      :          Jean-Paul CHAPUT                                 */
/*  E-mail      :      cao-vlsi@masi.ibp.fr                                 */
/* ======================================================================== */
/*  File        :      "usercol_dpt.c"                                      */
/* ************************************************************************ */
/*  Example of synthesis inside a Data-Path.                                */


#include  <genlib.h>
#include  <fpgen.h>


static void  mkZeroDetect();


static void  mkZeroDetect()
{
  /* Open the GenLib figure (the user data-path column).
   * ===================================================
   */
  DEF_LOFIG( "nul_us" );


  /* Interface description.
   * ======================
   */
  LOCON(   "i0[7:0]", IN   ,   "i0[7:0]" );
  LOCON( "zero"     ,   OUT, "zero"      );
  LOCON(  "vdd"     , IN   ,  "vdd"      );
  LOCON(  "vss"     , IN   ,  "vss"      );


  /* 8 bits Zero Detect netlist description.
   * =======================================
   */
  LOINS( "no2_dp", "no2_0", "i0[0]", "i0[1]", "z2_0", "vdd", "vss", 0L );
  LOINS( "no2_dp", "no2_2", "i0[2]", "i0[3]", "z2_1", "vdd", "vss", 0L );
  LOINS( "no2_dp", "no2_4", "i0[4]", "i0[5]", "z2_2", "vdd", "vss", 0L );
  LOINS( "no2_dp", "no2_6", "i0[6]", "i0[7]", "z2_3", "vdd", "vss", 0L );

  LOINS( "na2_dp", "na2_1",  "z2_0",  "z2_1", "z4_0", "vdd", "vss", 0L );
  LOINS( "na2_dp", "na2_5",  "z2_2",  "z2_3", "z4_1", "vdd", "vss", 0L );

  LOINS( "no2_dp", "no2_3",  "z4_0",  "z4_1", "zero", "vdd", "vss", 0L );


  /* Terminate the netlist description, and save on disk.
   * ====================================================
   */
  SAVE_LOFIG();
}


main()
{
  /* Generate the Zero Detect Column.
   * ================================
   */
  mkZeroDetect();


  /* Open a new Data-Path figure.
   * ============================
   */
  DP_DEFLOFIG( "usercol_dpt", 8, LSB_INDEX_ZERO );


  /* Interface description.
   * ======================
   */

  /* Control terminals declarations. */
  DP_LOCON( "ctrl_sel"  , IN   , "ctrl_sel"   );
  DP_LOCON( "ctrl_ck"   , IN   , "ctrl_ck"    );
  DP_LOCON( "ctrl_wen"  , IN   , "ctrl_wen"   );
  DP_LOCON( "ctrl_ncout",   OUT, "ctrl_ncout" );
  DP_LOCON( "ctrl_nover",   OUT, "ctrl_nover" );
  DP_LOCON( "ctrl_zero" ,   OUT, "ctrl_zero"  );

  /* Data terminals declarations. */
  DP_LOCON( "data_a[7:0]" , IN   , "data_a[7:0]" );
  DP_LOCON( "data_b[7:0]" , IN   , "data_b[7:0]" );
  DP_LOCON( "data_s[7:0]" , INOUT, "data_s[7:0]" );

  /* Power supplies terminals. */
  DP_LOCON( "vdd", IN   , "vdd" );
  DP_LOCON( "vss", IN   , "vss" );


  /* Data-Path netlist description.
   * ==============================
   */

  DP_TRACE( TRACE_ON );

  /* Multiplexer. */
  DP_MUX2CS( "multiplexer", 8, 0,
             "ctrl_sel",
             "data_b[7:0]",
             "data_q[7:0]",
             "data_m[7:0]",
             EOL );

  /* Fast Adder. */
  DP_ADD2F( "adder",
            "data_a[7:0]",
            "data_m[7:0]",
            "ctrl_ncout",
            "ctrl_nover",
            "data_s[7:0]",
            EOL );

  /* Zero Detect. */
  DP_IMPORT( "nul_us",
             "zero",
             "data_s[7:0]",
             "ctrl_zero",
             EOL );

  /* Register. */
  DP_PDFF( "memory", 8, 0,
           "ctrl_wen",
           "ctrl_ck",
           "data_s[7:0]",
           "data_q[7:0]",
           "data_u[7:0]",  /* This bus is unused. */
           EOL );


  DP_TRACE( TRACE_OFF );

  /* Terminate the netlist description, and save on disk.
   * ====================================================
   */
  DP_SAVLOFIG();


  /* A good C program must always terminate by an "exit(0)". */
  exit(0);
}
