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
# /*  File        :      "sample_dpt.sh"                                    */
# /* ********************************************************************** */
# /*  Data-Path sample example.                                             */
#
#


# Installation Dependent Variables.
           TOP=/labo


# FpGen Macro-Operators Library.
     FPGEN_LIB=./mclib; export FPGEN_LIB


# MBK Enviroment variables settings.
     MBK_IN_PH=ap;      export MBK_IN_PH
     MBK_IN_LO=vst;     export MBK_IN_LO
    MBK_OUT_PH=ap;      export MBK_OUT_PH
    MBK_OUT_LO=vst;     export MBK_OUT_LO
  MBK_WORK_LIB=.;       export MBK_WORK_LIB
  MBK_CATA_LIB=$TOP/cells/fitpath/fplib:$TOP/cells/rsa:$FPGEN_LIB
                        export MBK_CATA_LIB
MBK_CATAL_NAME=MON_CATAL;   export MBK_CATAL_NAME
       MBK_VDD=vdd;     export MBK_VDD
       MBK_VSS=vss;     export MBK_VSS

#
#
# FpGen : Generation of the Netlist.
# ==================================
#
fpgen -v --dev sample_dpt


#
#
# DPR : Routing the data-path.
# ============================
#
dpr -V -p -r sample_dpt sample_dpt


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

lynx -v -f sample_dpt sample_dpt_gates


#
#
# LVX : Netlist comparison.
# =========================
#
lvx vst al sample_dpt sample_dpt_gates -f


#
#
# LYNX : Transistor netlist extraction.
# =====================================
#
lynx -v -t sample_dpt sample_dpt


#
#
# DESB : Behavioral exctraction.
# ==============================
#
desb sample_dpt sample_dpt_desb -v -i


#
#
# PROOF : Formal proof.
# =====================
#
proof -d sample_dpt sample_dpt_desb
