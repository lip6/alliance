

/*          Alliance CAD System V.R                                         */
/*             FpGen Package V.RR                                           */
/*                                                                          */
/*  Copyright(c) 93-AA, MASI, CAO-VLSI Team                                 */
/*                                                                          */
/*  Author      :          Jean-Paul CHAPUT                                 */
/*  E-mail      :      cao-vlsi@masi.ibp.fr                                 */
/* ======================================================================== */
/*  File        :      "sample_dpt.c"                                       */
/* ************************************************************************ */
/*  Data-Path sample example.                                               */


#include  <genlib.h>
#include  <fpgen.h>


main()
{
  /* Open a new Data-Path figure.
   * ============================
   */
  DP_DEFLOFIG( "place_dpt", 8, LSB_INDEX_ZERO );


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

  /* Zero Detect. */
  DP_NUL( "zero", 8, 0,
          "data_s[7:0]",
          "ctrl_zero",
          EOL );

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
