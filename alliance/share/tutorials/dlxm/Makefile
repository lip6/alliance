############################
#
# makefile parameters
#
############################

SHELL      = /bin/sh
TOUCH      = touch

############################
#
# Executable files ( Alliance Tools )
#
############################

ASM        = $(ALLIANCE_TOP)/bin/dlx_asm_v0_2
ASIMUT     = $(ALLIANCE_TOP)/bin/asimut
SYF        = $(ALLIANCE_TOP)/bin/syf
BOP        = $(ALLIANCE_TOP)/bin/bop
SCMAP      = $(ALLIANCE_TOP)/bin/scmap
PROOF      = $(ALLIANCE_TOP)/bin/proof
# GLOP       = $(ALLIANCE_TOP)/bin/glop
GLOP       = echo
FPGEN      = $(ALLIANCE_TOP)/bin/fpgen
GENLIB     = $(ALLIANCE_TOP)/bin/genlib
SCR_EXE    = $(ALLIANCE_TOP)/bin/scr
LYNX       = $(ALLIANCE_TOP)/bin/lynx
DRUC       = $(ALLIANCE_TOP)/bin/druc
LVX        = $(ALLIANCE_TOP)/bin/lvx
DPR        = $(ALLIANCE_TOP)/bin/dpr
BBR        = $(ALLIANCE_TOP)/bin/bbr
RING_EXE   = $(ALLIANCE_TOP)/bin/ring
S2R        = $(ALLIANCE_TOP)/bin/s2r

XFSM = echo
# XFSM       = $(ALLIANCE_TOP)/bin/xfsm
XPAT = echo
# XPAT       = $(ALLIANCE_TOP)/bin/xpat
GRAAL = echo
# GRAAL      = $(ALLIANCE_TOP)/bin/graal
DREAL = echo
# DREAL      = $(ALLIANCE_TOP)/bin/dreal
GENVIEW = echo
# GENVIEW    = $(ALLIANCE_TOP)/bin/genview

############################
#
# Cells directories
#
#       scr  : Standard cells
#      ring  : Pads cells
#   fitpath  : Datapath cells
#       rsa  : Recurence Solver Adder cells
#       rfg  : Register File Generator cells
#       bsg  : Barrel Shifter Generator cells
#
############################

CELLS      = $(ALLIANCE_TOP)/cells

SCR        = $(CELLS)/sxlib
RING       = $(CELLS)/padlib
FPLIB      = $(CELLS)/fplib
RSA        = $(CELLS)/rsa
RFG        = $(CELLS)/rfg
BSG        = $(CELLS)/bsg

ALL        = $(SCR):$(RING):$(FPLIB):$(RSA):$(RFG):$(BSG)

############################
#
# Environments Variables
#
############################

# Set the logical environment to VST
ENV_VST      = MBK_IN_LO=vst;              export MBK_IN_LO;             \
               MBK_OUT_LO=vst;             export MBK_OUT_LO;            \
               MBK_WORK_LIB=.;             export MBK_WORK_LIB

# Set the logical environment to AL
ENV_AL       = MBK_IN_LO=al;               export MBK_IN_LO;             \
               MBK_OUT_LO=al;              export MBK_OUT_LO;            \
               MBK_WORK_LIB=.;             export MBK_WORK_LIB

# Set the physical environment to AP
ENV_AP       = MBK_IN_PH=ap;               export MBK_IN_PH;             \
               MBK_OUT_PH=ap;              export MBK_OUT_PH;            \
               MBK_WORK_LIB=.;             export MBK_WORK_LIB

# Set the asimut environment
ENV_VH       = VH_BEHSFX=vbe;              export VH_BEHSFX;             \
               VH_MAXERR="10";             export VH_MAXERR;             \
               VH_PATSFX=pat;              export VH_PATSFX

# Set the Synthese Logique environment
ENV_SL       = MBK_TARGET_LIB=$(SCR);       export MBK_TARGET_LIB;       \
               MBK_VDD=vdd;                 export MBK_VDD;              \
               MBK_VSS=vss;                 export MBK_VSS

# Set the Rectangle Data Structure environment
ENV_RDS      = RDS_TECHNO_NAME=$(ALLIANCE_TOP)/etc/prol10_11.rds;                    \
                                           export RDS_TECHNO_NAME;       \
               RDS_IN=cif;                 export RDS_IN;                \
               RDS_OUT=cif;                export RDS_OUT;               \
               MBK_WORK_LIB=.;             export MBK_WORK_LIB

############################
#
# Input of Makefile
#
# When you just call "make", display the availables Makefile entries
#
############################

############################
#
# When you call "make help", display the availables Makefile entries
#
############################

help         :
	@echo " "
	@echo "          Alliance CAD System 3.0,        dlxm_make"
	@echo "          Copyright (c) 94, MASI, CAO-VLSI Team"
	@echo "		 URA CNRS 818 - University Pierre et Marie Curie - "
	@echo "		 Institut Blaise Pascal "
	@echo "          E-mail support:      cao-vlsi@masi.ibp.fr"

	@echo " "
	@echo " How to make all the dlxm with a minimal simulation subset:"
	@echo " "
	@echo "                   all     : Make all the dlxm"
	@echo " "
	@echo " this step can be done in 4 separate steps :"
	@echo " "
	@echo "       1    functional     : Dlxm behavioral model validation"
	@echo "       2    structural     : Logical synthesis and validation"
	@echo "       3      physical     : Layout generation and validation"
	@echo " "
	@echo " this step can be done in 4 separate steps :"
	@echo " "
	@echo "         3.1   ctl_routing : Routing the control"
	@echo "         3.2   dpt_routing : Routing the data-path"
	@echo "         3.3  core_routing : Routing the core"
	@echo "         3.4  dlxm_routing : Routing the dlxm"
	@echo " "
	@echo "       4          real     : real layout file generation"
	@echo " "
	@echo "                 clean     : Clean the directory"
	@echo " "

############################
#
# When you call "make clean", the directory is cleanup
#
############################

clean        : clean_sim      \
               clean_struct   \
               clean_layout   \
               clean_real

clean_sim       :
	rm -f add000_chip_pat
	rm -f add000_blocks_pat
	rm -f add000_gates_pat
	rm -f dlxm_scan_res_pat
	rm -f add000_chip.pat
	rm -f add000_blocks.pat
	rm -f add000_gates.pat
	rm -f dlxm_scan_res.pat
	rm -f romu.vbe
	rm -f roms.vbe

clean_struct     :
	rm -f dlxm_seq.vbe
	rm -f dlxm_seqo.vbe
	rm -f dlxm_seqo_vbe
	rm -f dlxm_seq.cod
	rm -f dlxm_seq.vst
	rm -f dlxm_stso.vbe 
	rm -f dlxm_stso_vbe 
	rm -f dlxm_sts.vst 
	rm -f dlxm_ctl.vst
	rm -f dlxm_dpt.vst
	rm -f ./mclib/*_32x*.vst
	rm -f ./mclib/*_32x*.vbe

clean_layout  :
	rm -f *.ap
	rm -f ./mclib/*.ap
	rm -f ./mclib/CATAL
	rm -f *.al
	rm -f lvx_result_*

clean_real      :
	rm -f dlxm_chip.cif

############################
#
# the option to make all the dlxm
#
############################

all          : functional    \
               structural    \
               physical      \
               real

############################
#
# the option to validate the dlxm functional description
#
############################

functional     : dlxm_chip_sim

############################
#
# the option to validate the dlxm structural description
#
############################

structural   : dlxm_blocks_sim   \
               dlxm_gates_sim    \
               dlxm_scan_sim

############################
#
# the option to validate the dlxm physical description
#
############################

physical     : dlxm_routing

############################
#
# the option to make the ready to foundry dlxm file
#
############################

real         : dlxm_real

##########################################################################
#
#
# Functional part :
#
#   - Simulate with dlxm.vbe =>
#
#      1) Validation of dlxm_chip.vbe
#
#      2) Validation of asm files
#
##########################################################################

dlxm_chip_sim     : add000_chip_pat 

#####################################
#
# Simulation using user rom and supervisor rom
#
#    - Assign the environments variables
#    - Generate romu.vbe
#    - Generate roms.vbe
#    - Simulate
#
#
#####################################

add000_chip_pat    : add000.u add000.s dlxm_chip.vbe
	MBK_CATA_LIB=.;                export MBK_CATA_LIB;                 \
	MBK_CATAL_NAME=CATAL_CPU_CHIP;  export MBK_CATAL_NAME;               \
	$(ENV_VST);                                                          \
	$(ENV_VH);                                                           \
	$(ASM) add000.u romu ;                                          \
	$(ASM) add000.s roms ;                                          \
	$(ASIMUT) -l 1 -p 50 -i 0 -zd -bdd dlxm_cpu dlxm_cpu add000_chip
	touch add000_chip_pat

##########################################################################
#
#
# structural part :
#
#    - Generation of the core structural description
#
#    - Generation of the sequencer behavioral description
#
#    - Simulation of the dlxm with blocks
#
#    - Generation of the corresponding  netlists
#
#    - Simulation of the dlxm with gate leaf cells
#
#    - Simulation of the dlxm with scan-path 
#
##########################################################################

####################################
#
#
####################################


####################################
#
# Creating dlxm_seq.vbe from dlxm_seq.fsm ( with syf )
#
#   - Generation of dlxm_seq.vbe
#
####################################

dlxm_seq.vbe : dlxm_seq.fsm
	$(XFSM) -l dlxm_seq
	MBK_WORK_LIB=.;             export MBK_WORK_LIB ;   \
	$(SYF) -a -E -P dlxm_seq dlxm_seq

####################################
# Simulation of the dlxm described with pads and core in 2 blocks
# data path and control, 
# with control described in 2 blocks : status and sequencer.
# dlxm_sts.vbe dlxm_seq.vbe dlxm_dpt.vbe
####################################

dlxm_blocks_sim      : add000_blocks_pat 			

#####################################
#
#    - Assign the environments variables
#    - Generate romu.vbe
#    - Generate roms.vbe
# Creating dlxm_ctl.vst from dlxm_ctl.vst.h
#    - Simulate
#
#
#####################################

add000_blocks_pat    : add000.u add000.s                    \
                       dlxm_sts.vbe                         \
                       dlxm_seq.vbe                         \
                       dlxm_dpt.vbe                         \
                       dlxm_ctl.vst.h                         \
                       dlxm_core.vst                        \
                       dlxm_chip.vst
	MBK_CATA_LIB=$(RING);                      export MBK_CATA_LIB;  \
	MBK_CATAL_NAME=CATAL_CPU_BLOCKS;             export MBK_CATAL_NAME;  \
	$(ENV_VST);                                                          \
	$(ENV_VH);                                                           \
	$(ASM) add000.u romu ;                                                \
	$(ASM) add000.s roms ;                                                \
	cp dlxm_ctl.vst.h dlxm_ctl.vst;                        \
	chmod 644 dlxm_ctl.vst;					\
	$(ASIMUT) -l 1 -p 50 -i 0 -zd -bdd dlxm_cpu dlxm_cpu add000_blocks
	touch add000_blocks_pat
	$(XPAT) -l add000_blocks.pat

####################################
#
# Creating dlxm_sts.vst from dlxm_sts.vbe ( with logic )
#
#    - Generation of dlxm_stso.vbe
#
#    - Generation of dlxm_sts.vst
#
####################################

dlxm_sts.vst : dlxm_stso_vbe
	@ $(ENV_VST);                                                        \
	$(ENV_SL);                                                           \
	MBK_CATA_LIB=$(SCR);    export MBK_CATA_LIB;                         \
	$(SCMAP) dlxm_stso dlxm_sts

dlxm_stso_vbe : dlxm_sts.vbe
	@ $(ENV_VST);                                                        \
	$(ENV_SL);                                                           \
	MBK_CATA_LIB=$(SCR);    export MBK_CATA_LIB;                         \
	$(BOP) -o dlxm_sts dlxm_stso
	$(PROOF) dlxm_sts dlxm_stso
	$(TOUCH) dlxm_stso_vbe

####################################
#
# Creating dlxm_seq.vst from dlxm_seq.vbe ( with logic )
#
#    - Generation of dlxm_seqo.vbe
#
#    - Generation of dlxm_seq.vst
#
####################################

dlxm_seq.vst : dlxm_seqo_vbe
	$(ENV_VST);                                                        \
	$(ENV_SL);                                                           \
	MBK_CATA_LIB=$(SCR);    export MBK_CATA_LIB;                         \
	$(SCMAP) dlxm_seqo dlxm_seq

dlxm_seqo_vbe : dlxm_seq.vbe
	$(ENV_VST);                                                        \
	$(ENV_SL);                                                           \
	MBK_CATA_LIB=$(SCR);    export MBK_CATA_LIB;                         \
	$(BOP) -o dlxm_seq dlxm_seqo
	$(PROOF) dlxm_seq dlxm_seqo
	$(TOUCH) dlxm_seqo_vbe


#####################################
#
# Restructurate control
#
#   - Call netoptim on control
#
#####################################

dlxm_ctl.vst : dlxm_seq.vst             \
               dlxm_sts.vst             
	$(ENV_VST);                                                    \
	MBK_VDD=vdd;           export MBK_VDD;                           \
	MBK_VSS=vss;           export MBK_VSS;                           \
	MBK_CATA_LIB=$(SCR);   export MBK_CATA_LIB;                      \
	$(GLOP) -g dlxm_ctl dlxm_ctl 

####################################
#
# Creating dlxm_dpt.vst from dlxm_dpt.c ( with fpgen )
#
#    - Using a subdirectory mclib to store the generated operators
#
#    - Generation of dlxm_dpt.vst
#
####################################

dlxm_dpt.vst : dlxm_dpt.c
	@MBK_CATA_LIB=./mclib:$(ALL);       export MBK_CATA_LIB;            \
	$(ENV_VST);                                                          \
	$(ENV_AP);                                                           \
	FPGEN_LIB=./mclib; export FPGEN_LIB;				\
	$(FPGEN) -v dlxm_dpt

####################################
# Simulation of dlxm_sts.vst dlxm_seq.vst dlxm_dpt.vst
####################################

dlxm_gates_sim      : add000_gates_pat 

#####################################
#
# Always the same method :
#
#    - Assign the environments variables
#    - Generate romu.vbe
#    - Generate roms.vbe
#    - Simulate
#
#
#####################################

add000_gates_pat    : add000.u  add000.s                \
                      dlxm_core.vst                     \
                      dlxm_chip.vst                     \
                      dlxm_ctl.vst                      \
                      dlxm_seq.vst                      \
                      dlxm_sts.vst                      \
                      dlxm_dpt.vst
	MBK_CATA_LIB=./mclib:$(ALL);    export MBK_CATA_LIB;     \
	MBK_CATAL_NAME=CATAL_CPU_GATES;    export MBK_CATAL_NAME;   \
	$(ENV_VST);                                                 \
	$(ENV_VH);                                                  \
	$(ASM) add000.u romu ;                                 \
	$(ASM) add000.s roms ;                                 \
	$(ASIMUT) -l 1 -p 50 -i 0 -zd -bdd dlxm_cpu dlxm_cpu add000_gates
	touch add000_gates_pat

##########################################################################
#
# Validation of the scan-path
#
#         Simulate ( ASIMUT )
#
##########################################################################

dlxm_scan_sim : dlxm_scan_res_pat

####################################
#
# Simulate for testing scan-path
#
#    - Call ASIMUT
#
####################################

dlxm_scan_res_pat : dlxm_scan.pat                     \
                    add000.u add000.s                 \
                    dlxm_ctl.vst                          \
                    dlxm_core.vst                          \
                    dlxm_chip.vst                          \
                    dlxm_sts.vst                      \
                    dlxm_seq.vst                      \
                    dlxm_dpt.vst
	MBK_CATA_LIB=./mclib:$(ALL);             export MBK_CATA_LIB;   \
	MBK_CATAL_NAME=CATAL_CPU_GATES;    export MBK_CATAL_NAME;   \
	$(ENV_VST);                                                 \
	$(ENV_VH);                                                  \
	$(ASM) add000.u romu ;                                 \
	$(ASM) add000.s roms ;                                 \
	$(ASIMUT) -l 10 -p 50 -zd -bdd dlxm_cpu dlxm_scan dlxm_scan_res
	touch dlxm_scan_res_pat

##########################################################################
#
# Physical part :
#
#    - Route control
#
#    - Route datapath
#
#    - Route core
#
#    - Route chip
#
##########################################################################

#####################################
#
# How to make control
#
#   - Call the router SCR
#
#   - Extract the netlist from layout ( LYNX )
#
#   - Compare the netlist ( LVX )
#
#####################################

ctl_routing : lvx_result_ctl

#####################################
#
# Routing control
#
#    - Call SCR
#
#####################################

dlxm_ctl.ap : dlxm_ctl.vst dlxm_ctl.scr
	@ MBK_CATA_LIB=$(SCR);   export MBK_CATA_LIB;                        \
	MBK_VDD=vdd;           export MBK_VDD;                               \
	MBK_VSS=vss;           export MBK_VSS;                               \
	$(ENV_VST);                                                          \
	$(ENV_AP);                                                           \
	$(SCR_EXE) -p -r -i 3000 -l 5 -a 5 dlxm_ctl; \
	$(GRAAL) -l dlxm_ctl
	$(GENVIEW) -l dlxm_ctl

#####################################
#
# Extracting control
#
#    - Call LYNX
#
#####################################

dlxm_ctl.al : dlxm_ctl.ap
	@ MBK_CATA_LIB=$(SCR);   export MBK_CATA_LIB;                        \
	$(ENV_AL);                                                           \
	$(ENV_AP);                                                           \
	$(LYNX) -v dlxm_ctl dlxm_ctl

#####################################
#
# Netlist comparaison
#
#    - Call LVX
#
#   - Create a file for makefile dependances
#
#####################################

lvx_result_ctl : dlxm_ctl.al
	@ MBK_CATA_LIB=$(SCR);   export MBK_CATA_LIB;                          \
	$(LVX) vst al dlxm_ctl dlxm_ctl -f
	touch lvx_result_ctl

#####################################
#
# How to make data-path
#
#   - Call DPR
#
#   - Extract the netlist from layout ( LYNX )
#
#   - Compare the netlist ( LVX )
#
#####################################

dpt_routing : lvx_result_dpt

#####################################
#
# Routing data-path
#
#   - Call DPR
#
#####################################

dlxm_dpt.ap : dlxm_dpt.vst dlxm_dpt.dpr
	@ MBK_CATA_LIB=./mclib:$(ALL);     export MBK_CATA_LIB;             \
	$(ENV_VST);                                                          \
	$(ENV_AP);                                                           \
	$(DPR) -o -p -r dlxm_dpt dlxm_dpt

#####################################
#
# Extracting data-path
#
#   - Call LYNX
#
#####################################

dlxm_dpt.al : dlxm_dpt.ap
	@ MBK_CATA_LIB=./mclib:$(ALL);               export MBK_CATA_LIB;    \
	$(ENV_AL);                                                           \
	$(ENV_AP);                                                           \
	$(LYNX) -v dlxm_dpt dlxm_dpt

#####################################
#
# Netlist comparaison
#
#   - Call LVX
#
#   - Create a file for makefile dependances
#
#####################################

lvx_result_dpt : dlxm_dpt.al
	@ MBK_CATA_LIB=./mclib:$(ALL);               export MBK_CATA_LIB;   \
	$(LVX) vst al dlxm_dpt dlxm_dpt 
	touch lvx_result_dpt

#####################################
#
# How to make core
#
#   - Call BBR
#
#   - Extract the netlist from layout ( LYNX )
#
#   - Compare the netlist ( LVX )
#
#####################################

core_routing : dpt_routing ctl_routing lvx_result_core

#####################################
#
# Routing core
#
#   - Create the placement ( GENLIB )
#
#   - Call BBR
#
#####################################

dlxm_core.ap : dlxm_core.c dlxm_dpt.ap dlxm_ctl.ap
	@ MBK_CATA_LIB=./maclib:$(ALL);   export MBK_CATA_LIB;                        \
	$(ENV_VST);                                                          \
	$(ENV_AP);                                                           \
	$(GENLIB) -v dlxm_core;                                              \
	$(BBR) dlxm_core -v -o dlxm_core vdd 12 vss 12; \
	$(GRAAL) -l dlxm_core
	$(GENVIEW) -l dlxm_core

#####################################
#
# Extracting core
#
#   - Call LYNX
#
#####################################

dlxm_core.al : dlxm_core.ap
	@ MBK_CATA_LIB=./mclib:$(ALL);   export MBK_CATA_LIB;                \
	$(ENV_AL);                                                           \
	$(ENV_AP);                                                           \
	$(LYNX) -v dlxm_core dlxm_core

#####################################
#
# Netlist comparaison
#
#   - Call LVX
#
#   - Create a file for makefile dependances
#
#####################################

lvx_result_core : dlxm_core.al
	@ MBK_CATA_LIB=$(ALL);   export MBK_CATA_LIB;                          \
	$(LVX) vst al dlxm_core dlxm_core
	touch lvx_result_core

#####################################
#
# How to make dlxm
#
#   - Call RING
#
#   - Extract the netlist from layout ( LYNX )
#
#   - Compare the netlist ( LVX )
#
#####################################

dlxm_routing : core_routing lvx_result_dlxm

#####################################
#
# Routing dlxm
#
#   - Call RING
#
#####################################

dlxm_chip.ap : dlxm_core.ap dlxm_chip.rin
	@ MBK_CATA_LIB=$(ALL);     export MBK_CATA_LIB;                    \
	$(ENV_VST);                                                          \
	$(ENV_AP);                                                           \
	$(RING_EXE) dlxm_chip dlxm_chip; \
	$(GRAAL) -l dlxm_chip
	$(GENVIEW) -l dlxm_chip

#####################################
#
# Extracting dlxm
#
#   - Call LYNX
#
#####################################

dlxm_chip.al : dlxm_chip.ap
	@ MBK_CATA_LIB=$(ALL);   export MBK_CATA_LIB;                        \
	$(ENV_AL);                                                           \
	$(ENV_AP);                                                           \
	$(LYNX) -v dlxm_chip dlxm_chip

#####################################
#
# Netlist comparaison
#
#   - Call LVX
#
#   - Create a file for makefile dependances
#
#####################################

lvx_result_dlxm : dlxm_chip.al
	@ MBK_CATA_LIB=$(ALL); export MBK_CATA_LIB;                      \
	$(LVX) vst al dlxm_chip dlxm_chip
	touch lvx_result_dlxm

#####################################
#
# How to make real view of dlxm
#
#    - Call S2R : Generate the real view ( cif )
#
#####################################

dlxm_real : physical dlxm_chip.cif

#####################################
#
# Creation of real view of dlxm
#
#   - Call S2R
#
#####################################

dlxm_chip.cif : dlxm_chip.ap
	@ MBK_CATA_LIB=./mclib:$(ALL);        export MBK_CATA_LIB;           \
	$(ENV_RDS);                                                          \
	$(ENV_AP);                                                           \
	$(S2R) dlxm_chip; \
	$(DREAL) -l dlxm_chip

#####################################
#
#       ... The End ...
#
#   - Pirouz . Julien . Mariem . Ludovic . Czo - 
#
#####################################

