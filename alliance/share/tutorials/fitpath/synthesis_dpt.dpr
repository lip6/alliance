#
#
# /*          Alliance CAD System V.R                                       */
# /*             FpGen Package V.RR                                         */
# /*                                                                        */
# /*  Copyright(c) 93-AA, MASI, CAO-VLSI Team                               */
# /*                                                                        */
# /*  Author      :          Jean-Paul CHAPUT                               */
# /*  E-mail      :      cao-vlsi@masi.ibp.fr                               */
# /* ====================================================================== */
# /*  File        :      "synthesis_dpt.dpr"                                */
# /* ********************************************************************** */
# /*  Data-Path sample example.                                             */
#
#
#           Terminal :  Side  :  Slice  :  Track
DP_LOCON   ctrl_zero   NORTH    DEFAULT   DEFAULT
DP_LOCON   ctrl_ncout  NORTH    DEFAULT   DEFAULT
DP_LOCON   ctrl_nover  NORTH    DEFAULT   DEFAULT
#
#
#           Terminal :  Side  :  Slice  :  Track
DP_LOCON  data_a[7:0]   WEST    DEFAULT   DEFAULT
DP_LOCON  data_b[7:0]   WEST    DEFAULT   DEFAULT
DP_LOCON  data_s[7:0]   EAST    DEFAULT   DEFAULT
#
#
#         Model Name : Iterations : Height : CPC
DP_GLUE    adder_us       5000         8      2
#
#
# Number and wide of vertical power refreshment.
DP_POWER  1  50
