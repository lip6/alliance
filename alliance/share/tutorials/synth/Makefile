############################
# makefile parameters
############################
SHELL =	/bin/sh

circuit =	digicode
CELLS_LIB =	$(ALLIANCE_TOP)/cells/sxlib
AUTO_PATH =	.

SYF_EXE       = $(ALLIANCE_TOP)/bin/syf
# BOP_EXE       = $(ALLIANCE_TOP)/bin/bop
BOP_EXE       = $(HOME)/labo/Solaris/bin/bop

# NEALLIANCE_TOPTIM_EXE  = $(ALLIANCE_TOP)/bin/glop
NEALLIANCE_TOPTIM_EXE  = $(HOME)/labo/Solaris/bin/glop

# SCMAP_EXE     = $(ALLIANCE_TOP)/bin/scmap
SCMAP_EXE     = $(HOME)/labo/Solaris/bin/scmap
SCR_EXE       = $(ALLIANCE_TOP)/bin/scr
LYNX_EXE      = $(ALLIANCE_TOP)/bin/lynx
PROOF_EXE     = $(ALLIANCE_TOP)/bin/proof
YAGLE_EXE     = $(ALLIANCE_TOP)/bin/yagle

SYF =	MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
	$(SYF_EXE)

OPTIM = MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
	$(BOP_EXE) -o

MAPSC = MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
	MBK_TARGET_LIB=$(CELLS_LIB);		export MBK_TARGET_LIB;\
	MBK_IN_LO=vst;				export MBK_IN_LO;\
	MBK_OUT_LO=vst;				export MBK_OUT_LO;\
	$(SCMAP_EXE)

NEALLIANCE_TOPTIM = MBK_CATAL_NAME=CATAL;		export MBK_CATAL_NAME;\
	MBK_CATA_LIB=$(CELLS_LIB);	export MBK_CATA_LIB;\
	MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
	MBK_VDD=vdd;				export MBK_VDD;\
	MBK_VSS=vss;				export MBK_VSS;\
	MBK_IN_LO=vst;				export MBK_IN_LO;\
	MBK_OUT_LO=vst;				export MBK_OUT_LO;\
	$(NEALLIANCE_TOPTIM_EXE) -g

TIMING = MBK_CATAL_NAME=CATAL;			export MBK_CATAL_NAME;\
	MBK_CATA_LIB=$(CELLS_LIB);	export MBK_CATA_LIB;\
	MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
	MBK_VDD=vdd;				export MBK_VDD;\
	MBK_VSS=vss;				export MBK_VSS;\
	MBK_IN_LO=vst;				export MBK_IN_LO;\
	$(NEALLIANCE_TOPTIM_EXE) -t

SCR =	MBK_CATAL_NAME=CATAL;                   export MBK_CATAL_NAME;\
	MBK_CATA_LIB=$(CELLS_LIB);	export MBK_CATA_LIB;\
	MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
	MBK_IN_LO=vst;                         	export MBK_IN_LO;\
	MBK_IN_PH=ap;                         	export MBK_IN_PH;\
	MBK_OUT_PH=ap;                        	export MBK_OUT_PH;\
	$(SCR_EXE) -p -r -i 1000

LYNX =  MBK_CATAL_NAME=CATAL;                   export MBK_CATAL_NAME;\
	MBK_CATA_LIB=$(CELLS_LIB);	export MBK_CATA_LIB;\
        MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
        MBK_IN_PH=ap;	                        export MBK_IN_PH;\
        MBK_OUT_LO=al;    	                export MBK_OUT_LO;\
        $(LYNX_EXE) -f

YAGLE =  MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
	 MBK_CATA_LIB=$(CELLS_LIB);	export MBK_CATA_LIB;\
	 MBK_IN_LO=al;                         	export MBK_IN_LO;\
         $(YAGLE_EXE) -v -i

PROOF =	MBK_WORK_LIB=$(AUTO_PATH);		export MBK_WORK_LIB;\
        $(PROOF_EXE) -d

.SILENT:

all : $(circuit)rg0sct.vbe $(circuit)sg0sct.vbe $(circuit)sg3sct.vbe proof_all results

clean	:
	-@rm -f $(AUTO_PATH)/*.cod
	-@rm -f $(AUTO_PATH)/*.vbe
	-@rm -f $(AUTO_PATH)/*.al
	-@rm -f $(AUTO_PATH)/*.vst
	-@rm -f $(AUTO_PATH)/*.rep
	-@rm -f $(AUTO_PATH)/*.ap
	-@rm -f $(AUTO_PATH)/results
	-@rm -f proof_all

################################################################################
#	State encoding by SYF
################################################################################
$(circuit)s.vbe : $(circuit).fsm
	$(SYF) -E -o  $(circuit) $(circuit)s
$(circuit)r.vbe : $(circuit).fsm
	$(SYF) -E -r $(circuit) $(circuit)r

################################################################################
#	Delay and Area global Optimisation by LOGIC
################################################################################
$(circuit)sg0.vbe : $(circuit)s.vbe area.lax
	$(OPTIM) $(circuit)s $(circuit)sg0 area
$(circuit)rg0.vbe : $(circuit)r.vbe area.lax
	$(OPTIM) $(circuit)r $(circuit)rg0 area
$(circuit)sg3.vbe : $(circuit)s.vbe delay.lax
	$(OPTIM) $(circuit)s $(circuit)sg3 delay

################################################################################
#	Mapping by LOGIC
################################################################################
$(circuit)sg0sc.vst : $(circuit)sg0.vbe
	$(MAPSC) $(circuit)sg0 $(circuit)sg0sc area
$(circuit)rg0sc.vst : $(circuit)rg0.vbe
	$(MAPSC) $(circuit)rg0 $(circuit)rg0sc area
$(circuit)sg3sc.vst : $(circuit)sg3.vbe
	$(MAPSC) $(circuit)sg3 $(circuit)sg3sc delay

################################################################################
#	Delay Optimisation by NEALLIANCE_TOPTIM
################################################################################
$(circuit)sg0scg.vst : $(circuit)sg0sc.vst neto.lax
	$(NEALLIANCE_TOPTIM) $(circuit)sg0sc $(circuit)sg0scg neto
$(circuit)rg0scg.vst : $(circuit)rg0sc.vst neto.lax
	$(NEALLIANCE_TOPTIM) $(circuit)rg0sc $(circuit)rg0scg neto
$(circuit)sg3scg.vst : $(circuit)sg3sc.vst neto.lax
	$(NEALLIANCE_TOPTIM) $(circuit)sg3sc $(circuit)sg3scg neto


################################################################################
#	Placement and Routage by SCR
################################################################################
$(circuit)sg0scg.ap : $(circuit)sg0scg.vst 
	$(SCR) $(circuit)sg0scg
$(circuit)rg0scg.ap : $(circuit)rg0scg.vst 
	$(SCR) $(circuit)rg0scg
$(circuit)sg3scg.ap : $(circuit)sg3scg.vst 
	$(SCR) $(circuit)sg3scg

################################################################################
#	Area and Delay Results
################################################################################
results : $(circuit)sg0scg.ap $(circuit)sg3scg.ap $(circuit)rg0scg.ap
	echo "Simulated annealing encoding and standard cell mapping with area optimisation :" > results
	echo -n "        Area : " >> results
	grep "^A" $(circuit)rg0scg.ap >> results
	$(TIMING) $(circuit)rg0scg toto | grep path | grep -v Min >> results
	echo "Vertical encoding and standard cell mapping with area optimisation :" >>results
	echo -n "        Area : " >> results
	grep "^A" $(circuit)sg0scg.ap >> results
	$(TIMING) $(circuit)sg0scg toto | grep path | grep -v Min >> results
	echo "Vertical encoding and standard cell mapping with delay optimisation :" >>results
	echo -n "        Area : " >> results
	grep "^A" $(circuit)sg3scg.ap >> results
	$(TIMING) $(circuit)sg3scg toto | grep path | grep -v Min >> results
	cat results

################################################################################
#	Netlist Extraction by LYNX
################################################################################
$(circuit)sg0sct.al : $(circuit)sg0scg.ap
	$(LYNX) $(circuit)sg0scg $(circuit)sg0sct
$(circuit)rg0sct.al : $(circuit)rg0scg.ap
	$(LYNX) $(circuit)rg0scg $(circuit)rg0sct
$(circuit)sg3sct.al : $(circuit)sg3scg.ap
	$(LYNX) $(circuit)sg3scg $(circuit)sg3sct

################################################################################
#	Functional Abstraction by YAGLE
################################################################################
$(circuit)sg0sct.vbe : $(circuit)sg0sct.al
	$(YAGLE) $(circuit)sg0sct
	sed -e "s/_dff_s//g" $(circuit)sg0sct.vbe > toto
	mv toto $(circuit)sg0sct.vbe
$(circuit)rg0sct.vbe : $(circuit)rg0sct.al
	$(YAGLE) $(circuit)rg0sct
	sed -e "s/_dff_s//g" $(circuit)rg0sct.vbe > toto
	mv toto $(circuit)rg0sct.vbe
$(circuit)sg3sct.vbe : $(circuit)sg3sct.al
	$(YAGLE) $(circuit)sg3sct
	sed -e "s/_dff_s//g" $(circuit)sg3sct.vbe > toto
	mv toto $(circuit)sg3sct.vbe

################################################################################
#	Formal Proof by PROOF
################################################################################
proof_all :
	$(PROOF) $(circuit)sg0sct $(circuit)s
	$(PROOF) $(circuit)rg0sct $(circuit)r
	$(PROOF) $(circuit)sg3sct $(circuit)s
	touch proof_all
