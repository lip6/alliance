#TOP    = /users/soft5/newlabo/Solaris
ASIMUT = $(TOP)/bin/asimut
GENLIB = $(TOP)/bin/genlib -v 
SCR    = $(TOP)/bin/scr
RING   = $(TOP)/bin/ring
LYNX   = $(TOP)/bin/lynx
LVX    = $(TOP)/bin/lvx
YAGLE  = $(TOP)/bin/yagle
PROOF  = $(TOP)/bin/proof
DRUC   = $(TOP)/bin/druc
GRAAL  = $(TOP)/bin/graal
S2R    = $(TOP)/bin/s2r

# ### 
# goal of the Makefile: doing the whole chip design and verification
#
# ###

all: specifications.pat addaccu.proof addaccu.cif
	@echo "################################################"
	@echo "##                                            ##" 
	@echo "##                 D O N E                    ##" 
	@echo "##                                            ##" 
	@echo "################################################"
	@echo " " 
	@echo "Please run 'dreal -l addaccu' to see the layout." 
	@echo " " 


# ###---------------------------------------------------------###
# check the correctness of specifications (Data-FLow VHDL)      #
# running the logic simulation                                  #
# ###---------------------------------------------------------###

specifications.pat : addaccu.vbe addaccu.pat
	$(ASIMUT) -b addaccu addaccu specifications

# ###---------------------------------------------------------###
# generate structural description of the circuit's core         #
# core.c is the schematic capture of the core, using            #
# the genlib language                                           #
# the environment variable specify the structural VHDL format   #
# ###---------------------------------------------------------###

core.vst : core.c
	MBK_IN_LO=vst ;\
	MBK_OUT_LO=vst ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib;\
	export MBK_IN_LO MBK_OUT_LO MBK_CATA_LIB ;\
	$(GENLIB) core


# ###---------------------------------------------------------###
# generate structural description of the complete circuit       #
# addaccu.c is the schematic capture of the circuit, using      #
# the genlib language                                           #
# the circuit can be generated only if the core is generated    #
# the environment variable specify the structural VHDL format   #
# ###---------------------------------------------------------###
addaccu.vst : addaccu.c core.vst
	MBK_IN_LO=vst ;\
	MBK_OUT_LO=vst ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_LO MBK_OUT_LO MBK_CATA_LIB ;\
	$(GENLIB) addaccu

# ###---------------------------------------------------------###
# check the correctness of the generated structural             #
# description running the logic simulation                      #
# ###---------------------------------------------------------###

schema.pat : addaccu.vst addaccu.pat
	MBK_IN_LO=vst ;\
	MBK_OUT_LO=vst ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_LO MBK_OUT_LO MBK_CATA_LIB ;\
	$(ASIMUT) addaccu addaccu schema

# ###---------------------------------------------------------###
# place and route the circuit's core                            #
# ###---------------------------------------------------------###

core.ap : core.vst
	MBK_IN_LO=vst ;\
	MBK_IN_PH=ap ;\
	MBK_OUT_PH=ap ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_LO MBK_IN_PH MBK_OUT_PH MBK_CATA_LIB ;\
	$(SCR) -p -r core 

# ###---------------------------------------------------------###
# extract net-list from symbolic layout for the core            #
# ###---------------------------------------------------------###

core.al : core.ap
	MBK_IN_PH=ap ;\
	MBK_OUT_LO=al ;\
	RDS_TECHNO_NAME=$(TOP)/etc/cmos_7.rds ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_PH MBK_OUT_LO RDS_TECHNO_NAME MBK_CATA_LIB ;\
	$(LYNX) -v core core

# ###---------------------------------------------------------###
# compare extracted net-list and structural description         #
# to check that the standard cell router performed correctly    #
# ###---------------------------------------------------------###

core.lvx : core.al core.vst
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_CATA_LIB ;\
	$(LVX) vst al core core -f > core.lvx; \
	cat core.lvx

# ###---------------------------------------------------------###
# place and route the circuit (core and pads)                   #
# this can be done only if the extracted netlist is correct     #
# ###---------------------------------------------------------###

addaccu.ap : core.ap core.lvx addaccu.vst 
	MBK_IN_LO=vst ;\
	MBK_IN_PH=ap ;\
	MBK_OUT_PH=ap ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	MBK_SCALE_X=10; \
	export MBK_SCALE_X MBK_IN_PH MBK_OUT_PH MBK_IN_LO MBK_CATA_LIB ;\
	$(RING) addaccu addaccu

# ###---------------------------------------------------------###
# extract net-list from symbolic layout for the circuit         #
# ###---------------------------------------------------------###

addaccu.al : addaccu.ap
	MBK_IN_PH=ap ;\
	MBK_OUT_LO=al ;\
	RDS_TECHNO_NAME=$(TOP)/etc/cmos_7.rds ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_PH MBK_OUT_LO RDS_TECHNO_NAME MBK_CATA_LIB ;\
	$(LYNX) -v addaccu addaccu

# ###---------------------------------------------------------###
# compare extracted net-list and structural description         #
# to check that the ring router performed correctly             #
# ###---------------------------------------------------------###

addaccu.lvx : addaccu.al addaccu.vst
	$(LVX) vst al addaccu > addaccu.lvx
	cat addaccu.lvx

# ###---------------------------------------------------------###
# simulate the extracted net-list                               #
# ###---------------------------------------------------------###

extracted.pat : addaccu.al
	MBK_IN_LO=al ;\
	MBK_OUT_LO=al ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_LO MBK_OUT_LO MBK_CATA_LIB ;\
	$(ASIMUT) addaccu addaccu extracted

# ###---------------------------------------------------------###
# generate a Data-Flow description from the transistor level    #
# extracted net-list                                            #
# ###---------------------------------------------------------###

addaccue.vbe : addaccu.ap addaccue.inf
	MBK_IN_PH=ap ;\
	MBK_IN_LO=al ;\
	MBK_OUT_LO=al ;\
	RDS_TECHNO_NAME=$(TOP)/etc/cmos_7.rds ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_PH MBK_IN_LO MBK_OUT_LO RDS_TECHNO_NAME MBK_CATA_LIB ;\
	$(LYNX) -v -t addaccu addaccue ;\
	$(YAGLE) addaccue -i -v

# ###---------------------------------------------------------###
# simulate the extracted data_flow description                  #
# ###---------------------------------------------------------###

abstracted.pat : addaccue.vbe
	MBK_IN_LO=vst ;\
	MBK_OUT_LO=vst ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib;\
	export MBK_IN_LO MBK_OUT_LO MBK_CATA_LIB ;\
	$(ASIMUT) -b addaccue addaccu abstracted

# ###---------------------------------------------------------###
# check the correctness of the extracted data_flow              #
# description running the formal proover                        #
# ###----------------------------------------------------------##

addaccu.proof : addaccue.vbe addaccu.vbe
	$(PROOF) -d addaccue addaccu > addaccu.proof; \
	cat addaccu.proof

# ###---------------------------------------------------------###
# check design rules                                            #
# ###---------------------------------------------------------###

addaccu.drc : addaccu.ap
	MBK_IN_PH=ap ;\
	RDS_OUT=cif  ;\
	RDS_TECHNO_NAME=$(TOP)/etc/cmos_7.rds ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_PH RDS_OUT RDS_TECHNO_NAME MBK_CATA_LIB ;\
	$(DRUC) addaccu

# ###---------------------------------------------------------###
# generates CIF description for 1.0 um process                  #
# ###---------------------------------------------------------###

addaccu.cif : addaccu.ap addaccu.drc
	MBK_IN_PH=ap ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	RDS_TECHNO_NAME=$(TOP)/etc/prol10_7.rds ;\
	RDS_OUT=cif  ;\
	RDS_IN=cif ;\
	export MBK_IN_PH RDS_IN RDS_OUT RDS_TECHNO_NAME MBK_CATA_LIB ;\
	$(S2R) -v addaccu

# ###---------------------------------------------------------###
# clean up the directory for tutorial re-run                    #
# ###---------------------------------------------------------###

clean:
	rm -f core addaccu specifications.pat \
	   core.vst schema.pat core.ap core.al core.lvx \
	   addaccu.vst addaccu.ap addaccu.al extracted.pat \
	   addaccue.al addaccue.vbe  addaccue.rep addaccue.dsbr addaccue.fcf \
	   abstracted.pat addaccu.proof \
	   addaccu.drc addaccu*.cif core *~ 

# ###---------------------------------------------------------###
# look at the circuit under graal                               #
# ###---------------------------------------------------------###

graal : addaccu.ap
	MBK_IN_PH=ap ;\
	RDS_TECHNO_NAME=$(TOP)/etc/cmos_7.rds ;\
	MBK_CATA_LIB=$(TOP)/cells/sclib:$(TOP)/cells/padlib ;\
	export MBK_IN_PH RDS_TECHNO_NAME MBK_CATA_LIB ;\
	$(GRAAL) -l addaccu

