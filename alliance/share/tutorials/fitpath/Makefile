
#
# %% \textrm{This File Looks Nicer with\textsf{ LGrind} and \LaTeXe.}        %%
# %|                                                                         |%
# %| /---------------------------------------------------------------------\ |%
# %| |                                                                     | |%
# %| |          Alliance CAD System V.R                                    | |%
# %| |     F p G e n   P a c k a g e   V.RR                                | |%
# %| |                                                                     | |%
# %| |  Copyright(c) 95-AA, MASI, CAO-VLSI Team                            | |%
# %| |                                                                     | |%
# %| |  Author    :            Jean-Paul CHAPUT                            | |%
# %| | =================================================================== | |%
# %| |  File      :  "Makefile"                                            | |%
# %| | ******************************************************************* | |%
# %| |  FpGen / DPR examples.                                              | |%
# %| |                                                                     | |%
# %| \---------------------------------------------------------------------/ |%
#

# Installation Dependent Variables.
     TOP_LAB = $(ALLIANCE_TOP)
     TOP_LIB = $(ALLIANCE_TOP)

# Alliance binary access paths.
ALLIANCE_BIN =$(ALLIANCE_TOP)/bin
 FITPATH_BIN =$(ALLIANCE_TOP)/bin
    USER_BIN =$(ALLIANCE_TOP)/bin
     DPR_BIN =$(ALLIANCE_TOP)/bin


## ************************* Various MBK environment *************************


#
#
# Alliance leaf cells libraries paths.
# ====================================
#
  RSA_LIB = $(TOP_LIB)/cells/rsa
    DPLIB = $(TOP_LAB)/cells/dplib
    FPLIB = $(TOP_LAB)/cells/fplib
FPGEN_LIB = ./mclib

 CATA_LIB = $(FPGEN_LIB):$(RSA_LIB):$(FPLIB):$(DPLIB)


#
#
# MBK environment common to all Alliance tools.
# =============================================
#
MBK_STD  = MBK_WORK_LIB=.;           export MBK_WORK_LIB;   \
		   MBK_CATA_LIB=$(CATA_LIB); export MBK_CATA_LIB;   \
		   MBK_CATAL_NAME=CATAL;     export MBK_CATAL_NAME; \
		   MBK_SEPAR=.;              export MBK_SEPAR;      \
		   MBK_VDD=vdd;              export MBK_VDD;        \
		   MBK_VSS=vss;              export MBK_VSS;        \
           MBK_TARGET_LIB=$(DPLIB);  export MBK_TARGET_LIB; \
           MBK_LOG_NAME="";          export MBK_LOG_NAME;   \
           FPGEN_LIB=$(FPGEN_LIB);   export FPGEN_LIB


#
#
# Avalaibles MBK netlist and layout formats.
# ==========================================
#
MBK_FNE  = MBK_OUT_LO=fne; export MBK_OUT_LO; MBK_IN_LO=fne; export MBK_IN_LO
MBK_HNS  = MBK_OUT_LO=hns; export MBK_OUT_LO; MBK_IN_LO=hns; export MBK_IN_LO
MBK_CP   = MBK_OUT_PH=cp;  export MBK_OUT_PH; MBK_IN_PH=cp;  export MBK_IN_PH
MBK_VST  = MBK_OUT_LO=vst; export MBK_OUT_LO; MBK_IN_LO=vst; export MBK_IN_LO
MBK_AL   = MBK_OUT_LO=al;  export MBK_OUT_LO; MBK_IN_LO=al;  export MBK_IN_LO
MBK_AP   = MBK_OUT_PH=ap;  export MBK_OUT_PH; MBK_IN_PH=ap;  export MBK_IN_PH


#
#
# Alliance Tools with the correct MBK environment.
# ================================================
#
     BOP = $(MBK_STD); $(MBK_VST); $(MBK_AP); $(ALLIANCE_BIN)/bop
   SCMAP = $(MBK_STD); $(MBK_VST); $(MBK_AP); $(ALLIANCE_BIN)/scmap
  FPGEN = $(MBK_STD); $(MBK_VST); $(MBK_AP); $(ALLIANCE_BIN)/fpgen
#    DPR = $(MBK_STD); $(MBK_VST); $(MBK_AP); $(ALLIANCE_BIN)/dpr
    DRUC = $(MBK_STD); $(MBK_VST); $(MBK_AP); $(ALLIANCE_BIN)/druc
    LYNX = $(MBK_STD); $(MBK_AL);  $(MBK_AP); $(ALLIANCE_BIN)/lynx
    DESB = $(MBK_STD); $(MBK_AL);             $(ALLIANCE_BIN)/yagle
   PROOF = $(MBK_STD);                        $(ALLIANCE_BIN)/proof
     LVX = $(MBK_STD);                        $(ALLIANCE_BIN)/lvx

#   FPGEN = $(MBK_STD); $(MBK_VST); $(MBK_AP);  $(FITPATH_BIN)/fpgen
#    DPR = $(MBK_STD); $(MBK_VST); $(MBK_AP);  $(FITPATH_BIN)/dpr
     DPR = $(MBK_STD); $(MBK_VST); $(MBK_AP);      $(DPR_BIN)/dpr
#    DPR = $(MBK_STD); $(MBK_VST); $(MBK_AP);     $(LABO_BIN)/dpr
#    LVX = $(MBK_STD);                            $(USER_BIN)/sparc/lvx-2.23
#   DESB = $(MBK_STD); $(MBK_AL);                 $(USER_BIN)/sparc/desb-2.91


## ***************************** Starting Rules ******************************


help:
	@echo 'Makefile:'; \
	 echo '';          \
	 echo '  "$$ make all"       : run all examples.';                \
	 echo '  "$$ make sample"    : run the more sample example.';     \
	 echo '  "$$ make place"     : run optimized placement example.'; \
	 echo '  "$$ make usercol"   : run operator customized example.'; \
	 echo '  "$$ make hierarchy" : run hierarchical design example.'; \
     echo '  "$$ make clean"     : delete all generated files.';      \
	 echo ''


all: sample place usercol hierarchy synthesis

sample:       sample_druc    sample_lvx    sample_proof
place:         place_druc     place_lvx     place_proof
usercol:     usercol_druc   usercol_lvx   usercol_proof
synthesis: synthesis_druc synthesis_lvx synthesis_proof
hierarchy: hierarchy_druc hierarchy_lvx hierarchy_proof


bench: all


## *********************** Rules for sample Data-Path ************************


#
#
# FpGen : Generation of the Netlist.
# ==================================
#
sample_dpt.vst: sample_dpt.c
	$(FPGEN) -v sample_dpt


#
#
# DPR : Routing the data-path.
# ============================
#
sample_dpt.ap: sample_dpt.dpr sample_dpt.vst
	$(DPR) -V -p -r sample_dpt sample_dpt


#
#
# DRUC : Symbolic Design RUles Checker.
# =====================================
#
sample_druc: sample_dpt.ap
	$(DRUC) sample_dpt


#
#
# LYNX : Gate level netlist extraction.
# =====================================
#
sample_dpt_gates.al: sample_dpt.ap
	$(LYNX) -v -f sample_dpt sample_dpt_gates


#
#
# LVX : Netlist comparison.
# =========================
#
sample_lvx: sample_dpt.vst sample_dpt_gates.al
	$(LVX) vst al sample_dpt sample_dpt_gates -f


#
#
# LYNX : Transistor netlist extraction.
# =====================================
#
sample_dpt.al: sample_dpt.ap
	$(LYNX) -v -t sample_dpt sample_dpt


#
#
# DESB : Behavioral exctraction.
# ==============================
#
sample_dpt_desb.vbe: sample_dpt.inf sample_dpt.al
	$(DESB) sample_dpt sample_dpt_desb -v -i


#
#
# PROOF : Formal proof.
# =====================
#
sample_proof: sample_dpt.vbe sample_dpt_desb.vbe
	$(PROOF) -d sample_dpt sample_dpt_desb


## ************** Rules for Data-Path using optimized placement **************


#
#
# FpGen : Generation of the Netlist.
# ==================================
#
place_dpt.vst: place_dpt.c
	$(FPGEN) -v place_dpt


#
#
# DPR : Routing the data-path.
# ============================
#
place_dpt.ap: place_dpt.dpr place_dpt.vst
	$(DPR) -V -o -p -r place_dpt place_dpt


#
#
# DRUC : Symbolic Design RUles Checker.
# =====================================
#
place_druc: place_dpt.ap
	$(DRUC) place_dpt


#
#
# LYNX : Gate level netlist extraction.
# =====================================
#
place_dpt_gates.al: place_dpt.ap
	$(LYNX) -v -f place_dpt place_dpt_gates


#
#
# LVX : Netlist comparison.
# =========================
#
place_lvx: place_dpt.vst place_dpt_gates.al
	$(LVX) vst al place_dpt place_dpt_gates -f


#
#
# LYNX : Transistor netlist extraction.
# =====================================
#
place_dpt.al: place_dpt.ap
	$(LYNX) -v -t place_dpt place_dpt


#
#
# DESB : Behavioral exctraction.
# ==============================
#
place_dpt_desb.vbe: place_dpt.inf place_dpt.al
	$(DESB) place_dpt place_dpt_desb -v -i


#
#
# PROOF : Formal proof.
# =====================
#
place_proof: sample_dpt.vbe place_dpt_desb.vbe
	$(PROOF) -d sample_dpt place_dpt_desb


## *********** Rules for Data-Path using Hierarchical description ************


#
#
# FpGen : Generation of the Netlist.
# ==================================
#
hierarchy_dpt.vst: hierarchy_dpt.c
	$(FPGEN) -v hierarchy_dpt


#
#
# DPR : Routing the data-path.
# ============================
#
hierarchy_dpt.ap: hierarchy_dpt.dpr hierarchy_dpt.vst
	$(DPR) -V -p -r hierarchy_dpt hierarchy_dpt


#
#
# DRUC : Symbolic Design RUles Checker.
# =====================================
#
hierarchy_druc: hierarchy_dpt.ap
	$(DRUC) hierarchy_dpt


#
#
# LYNX : Gate level netlist extraction.
# =====================================
#
hierarchy_dpt_gates.al: hierarchy_dpt.ap
	$(LYNX) -v -f hierarchy_dpt hierarchy_dpt_gates


#
#
# LVX : Netlist comparison.
# =========================
#
hierarchy_lvx: hierarchy_dpt.vst hierarchy_dpt_gates.al
	$(LVX) vst al hierarchy_dpt hierarchy_dpt_gates -f


#
#
# LYNX : Transistor netlist extraction.
# =====================================
#
hierarchy_dpt.al: hierarchy_dpt.ap
	$(LYNX) -v -t hierarchy_dpt hierarchy_dpt


#
#
# DESB : Behavioral exctraction.
# ==============================
#
hierarchy_dpt_desb.vbe: hierarchy_dpt.inf hierarchy_dpt.al
	$(DESB) hierarchy_dpt hierarchy_dpt_desb -v -i


#
#
# PROOF : Formal proof.
# =====================
#
hierarchy_proof: sample_dpt.vbe hierarchy_dpt_desb.vbe
	$(PROOF) -d sample_dpt hierarchy_dpt_desb




## ******************* Rules for Data-Path using synthesis *******************


#
#
# Logic : Synthesis of the Adder.
# ===============================
#
adder_us.vst: adder_us.vbe
	$(BOP)   -o adder_us     adder_us_opt
	$(SCMAP)    adder_us_opt adder_us


#
#
# FpGen : Generation of the Netlist.
# ==================================
#
synthesis_dpt.vst: adder_us.vst synthesis_dpt.c
	$(FPGEN) -v synthesis_dpt


#
#
# DPR : Routing the data-path.
# ============================
#
synthesis_dpt.ap: synthesis_dpt.dpr synthesis_dpt.vst
	$(DPR) -V -p -r synthesis_dpt synthesis_dpt


#
#
# DRUC : Symbolic Design RUles Checker.
# =====================================
#
synthesis_druc: synthesis_dpt.ap
	$(DRUC) synthesis_dpt


#
#
# LYNX : Gate level netlist extraction.
# =====================================
#
synthesis_dpt_gates.al: synthesis_dpt.ap
	$(LYNX) -v -f synthesis_dpt synthesis_dpt_gates


#
#
# LVX : Netlist comparison.
# =========================
#
synthesis_lvx: synthesis_dpt.vst synthesis_dpt_gates.al
	$(LVX) vst al synthesis_dpt synthesis_dpt_gates -f


#
#
# LYNX : Transistor netlist extraction.
# =====================================
#
synthesis_dpt.al: synthesis_dpt.ap
	$(LYNX) -v -t synthesis_dpt synthesis_dpt


#
#
# DESB : Behavioral exctraction.
# ==============================
#
synthesis_dpt_desb.vbe: synthesis_dpt.inf synthesis_dpt.al
	$(DESB) synthesis_dpt synthesis_dpt_desb -v -i


#
#
# PROOF : Formal proof.
# =====================
#
synthesis_proof: sample_dpt.vbe synthesis_dpt_desb.vbe
	$(PROOF) -d sample_dpt synthesis_dpt_desb


## ************** Rules for Data-Path using user defined column **************


#
#
# FpGen : Generation of the Netlist.
# ==================================
#
usercol_dpt.vst: usercol_dpt.c
	$(FPGEN) -v usercol_dpt


#
#
# DPR : Routing the data-path.
# ============================
#
usercol_dpt.ap: usercol_dpt.dpr usercol_dpt.vst
	$(DPR) -V -p -r usercol_dpt usercol_dpt


#
#
# DRUC : Symbolic Design RUles Checker.
# =====================================
#
usercol_druc: usercol_dpt.ap
	$(DRUC) usercol_dpt


#
#
# LYNX : Gate level netlist extraction.
# =====================================
#
usercol_dpt_gates.al: usercol_dpt.ap
	$(LYNX) -v -f usercol_dpt usercol_dpt_gates


#
#
# LVX : Netlist comparison.
# =========================
#
usercol_lvx: usercol_dpt.vst usercol_dpt_gates.al
	$(LVX) vst al usercol_dpt usercol_dpt_gates -f


#
#
# LYNX : Transistor netlist extraction.
# =====================================
#
usercol_dpt.al: usercol_dpt.ap
	$(LYNX) -v -t usercol_dpt usercol_dpt


#
#
# DESB : Behavioral exctraction.
# ==============================
#
usercol_dpt_desb.vbe: usercol_dpt.inf usercol_dpt.al
	$(DESB) usercol_dpt usercol_dpt_desb -v -i


#
#
# PROOF : Formal proof.
# =====================
#
usercol_proof: sample_dpt.vbe usercol_dpt_desb.vbe
	$(PROOF) -d sample_dpt usercol_dpt_desb


## *************************** Miscellaneous Rules ***************************


#
# Removing all generated files.
# =============================
#
clean:
	rm -Rf *_desb.vbe  \
			 *_opt.vbe  \
			  *_bk.vbe  \
			  *_cl.vbe  \
				 *.vst  \
				 *.hns  \
				 *.fne  \
				 *.err  \
				 *.grr  \
				 *.frr  \
				 *.rep  \
				 *.drc  \
				 *.gds  \
				 *.cif  \
				 *.fcf  \
				 *.deb  \
				 *.dsbr \
				 *.al   \
				 *.ap   \
				 *.cp   \
			 CATAL      \
			  core      \
			  Makefile-* \
	$(FPGEN_LIB)/*
