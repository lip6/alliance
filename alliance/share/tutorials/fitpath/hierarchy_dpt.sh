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
# /*  File        :      "hierarchy_dpt.sh"                                 */
# /* ********************************************************************** */
# /*  Data-Path using hierarchical description.                             */
#
#


# Installation Dependent Variables.
#           ALLIANCE_TOP=/labo


# FpGen Macro-Operators Library.
     FPGEN_LIB=./mclib; export FPGEN_LIB


# MBK Enviroment variables settings.
     MBK_IN_PH=ap;      export MBK_IN_PH
     MBK_IN_LO=vst;     export MBK_IN_LO
    MBK_OUT_PH=ap;      export MBK_OUT_PH
    MBK_OUT_LO=vst;     export MBK_OUT_LO
  MBK_WORK_LIB=.;       export MBK_WORK_LIB
  MBK_CATA_LIB=$ALLIANCE_TOP/cells/fitpath/fplib:$ALLIANCE_TOP/cells/rsa:$FPGEN_LIB
                        export MBK_CATA_LIB
MBK_CATAL_NAME=CATAL;   export MBK_CATAL_NAME
       MBK_VDD=vdd;     export MBK_VDD
       MBK_VSS=vss;     export MBK_VSS

#
#
# FpGen : Generation of the Netlist.
# ==================================
#
fpgen -v hierarchy_dpt


#
#
# DPR : Routing the data-path.
# ============================
#
dpr -V -p -o -r hierarchy_dpt hierarchy_dpt


#
#
# DRUC : Symbolic Design RUles Checker.
# =====================================
#
druc sample_dpt


#
#
# LYNX : Gate level netlist extraction.
# =====================================
#
MBK_OUT_LO=al
 MBK_IN_LO=al

lynx -v -f hierarchy_dpt hierarchy_dpt_gates


#
#
# LVX : Netlist comparison.
# =========================
#
lvx vst al hierarchy_dpt hierarchy_dpt_gates -f


#
#
# LYNX : Transistor netlist extraction.
# =====================================
#
lynx -v -t hierarchy_dpt hierarchy_dpt


#
#
# DESB : Behavioral exctraction.
# ==============================
#
desb hierarchy_dpt hierarchy_dpt_desb -v -i


#
#
# PROOF : Formal proof.
# =====================
#
proof -d sample_dpt hierarchy_dpt_desb
