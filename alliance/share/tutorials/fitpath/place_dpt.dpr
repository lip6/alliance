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
# /*  File        :      "place_dpt.dpr"                                    */
# /* ********************************************************************** */
# /*  Data-Path sample example.                                             */
#
#
#           Terminal :  Side  :  Slice  :  Track
DP_LOCON   ctrl_zero   NORTH    DEFAULT   DEFAULT
#
#
#           Terminal :  Side  :  Slice  :  Track
DP_LOCON  data_a[7:0]   WEST    DEFAULT   DEFAULT
DP_LOCON  data_b[7:0]   WEST    DEFAULT   DEFAULT
DP_LOCON  data_s[7:0]   EAST    DEFAULT   DEFAULT
#
#
# Number of vertical power refreshment.
DP_POWER  1  50
