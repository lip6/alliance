#!/bin/sh


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
# /*  File        :      "synthesis_dpt.sh"                                 */
# /* ********************************************************************** */
# /*  Data-Path using synthetized block example.                            */
#
#


# Installation Dependent Variables.
           ALLIANCE_TOP=/labo

# FpGen Macro-Operators Library.
     FPGEN_LIB=./mclib; export FPGEN_LIB

# MBK Enviroment Variables Settings.
     MBK_IN_PH=cp;      export MBK_IN_PH
     MBK_IN_LO=hns;     export MBK_IN_LO
    MBK_OUT_PH=cp;      export MBK_OUT_PH
    MBK_OUT_LO=hns;     export MBK_OUT_LO
  MBK_WORK_LIB=.;       export MBK_WORK_LIB
  MBK_CATA_LIB=$FPGEN_LIB
  MBK_CATA_LIB=$MBK_CATA_LIB:$ALLIANCE_TOP/cells/fitpath/fplib
  MBK_CATA_LIB=$MBK_CATA_LIB:$ALLIANCE_TOP/cells/fitpath/dplib
                        export MBK_CATA_LIB
MBK_CATAL_NAME=CATAL;   export MBK_CATAL_NAME
       MBK_VDD=vdd;     export MBK_VDD
       MBK_VSS=vss;     export MBK_VSS

# LOGIC Environment Variables.
MBK_TARGET_LIB=$ALLIANCE_TOP/cells/fitpath/dplib
                        export MBK_TARGET_LIB
  MBK_NAME_LOG="";      export MBK_NAME_LOG


#
#
# Logic : Synthesis of the Adder.
# ===============================
# Two steps :
#     1) Optimise data-flow equations.
#     2) Synthesis, using TARGET library.
#
logic -o adder_us     adder_us_opt
logic -s adder_us_opt adder_us


#
#
# FpGen : Generation of the Netlist.
# ==================================
#
fpgen --dev -v synthesis_dpt


#
#
# DPR : Routing the data-path.
# ============================
#
dpr -V -p -r synthesis_dpt synthesis_dpt


#
#
# DRUC : Symbolic Design RUles Checker.
# =====================================
#
druc synthesis_dpt



#
#
# LYNX : Gate level netlist extraction.
# =====================================
#
MBK_OUT_LO=al
 MBK_IN_LO=al

lynx -v -f synthesis_dpt synthesis_dpt_gates


#
#
# LVX : Netlist comparison.
# =========================
#
lvx hns al synthesis_dpt synthesis_dpt_gates -f


#
#
# LYNX : Transistor netlist extraction.
# =====================================
#
lynx -v -t synthesis_dpt synthesis_dpt


#
#
# DESB : Behavioral exctraction.
# ==============================
#
desb synthesis_dpt synthesis_dpt_desb -v -i


#
#
# PROOF : Formal proof.
# =====================
#
proof -d sample_dpt synthesis_dpt_desb
