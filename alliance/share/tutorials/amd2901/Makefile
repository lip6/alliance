SHELL    = /bin/sh

ASIMUT   = $(ALLIANCE_TOP)/bin/asimut -bdd
YAGLE    = $(ALLIANCE_TOP)/bin/yagle
DRUC     = $(ALLIANCE_TOP)/bin/druc

GRAAL    = $(ALLIANCE_TOP)/bin/graal
DREAL    = $(ALLIANCE_TOP)/bin/dreal
GRAAL    = echo
DREAL    = echo

GENLIB   = $(ALLIANCE_TOP)/bin/genlib
LVX      = $(ALLIANCE_TOP)/bin/lvx
LYNX     = $(ALLIANCE_TOP)/bin/lynx
PROOF    = $(ALLIANCE_TOP)/bin/proof
RING     = $(ALLIANCE_TOP)/bin/ring
S2R      = $(ALLIANCE_TOP)/bin/s2r -v
SCR      = $(ALLIANCE_TOP)/bin/scr
TAS      = $(ALLIANCE_TOP)/bin/tas

#################################################################
#   S2R is the final step of the conception, it generates the   # 
#   real layout description using the desired technology        #
#################################################################

all : amd2901.cif chip.ttv

chip.ttv : chip.al
	MBK_IN_LO=al; \
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib; \
	export MBK_IN_LO MBK_CATA_LIB; \
	$(TAS) chip

amd2901.cif : proof_end
	MBK_IN_PH=ap ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib;\
	RDS_TECHNO_NAME=$(ALLIANCE_TOP)/etc/prol10_7.rds ;\
	RDS_IN=cif ;\
	RDS_OUT=cif ;\
	export MBK_IN_PH MBK_WORK_LIB MBK_CATA_LIB RDS_TECHNO_NAME RDS_IN RDS_OUT;\
	$(S2R) -c chip amd2901;\
	$(DREAL) -l amd2901

#################################################################
# PROOF between amd.vbe chip.vbe				#
#################################################################

proof_end : chip.vbe 
	MBK_WORK_LIB=. ;\
	export MBK_WORK_LIB;\
	$(PROOF) -a -d amd chip ;\
	echo "proof_end done" >> proof_end

#################################################################
# YAGLE on the chip						#
#################################################################

chip.vbe : asimut_end
	MBK_IN_LO=al ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib ;\
	export MBK_IN_LO MBK_WORK_LIB MBK_CATA_LIB;\
	$(YAGLE) chip -i -v

#################################################################
# asimut of the chip						#
#################################################################

asimut_end : lvx_chip
	MBK_IN_LO=al ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib ;\
	export MBK_IN_LO MBK_WORK_LIB MBK_CATA_LIB;\
	VH_PATSFX=pat; export VH_PATSFX;\
	VH_MAXERR=10; export VH_MAXERR;\
	$(ASIMUT) chip pattern res_pattern
	echo " asimut_end done " >> asimut_end

#################################################################
#   LVX of the chip                                             #
#################################################################

lvx_chip : chip.al
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib;\
	export MBK_WORK_LIB MBK_CATA_LIB;\
	$(LVX) vst al chip chip
	echo " lvx_chip done" >> lvx_chip

#################################################################
#   LYNX of the chip                                            #
#################################################################

chip.al : druc_chip
	MBK_OUT_LO=al ;\
	MBK_IN_PH=ap ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib;\
	export MBK_OUT_LO MBK_IN_PH MBK_WORK_LIB MBK_CATA_LIB;\
	$(LYNX) chip 

#################################################################
#   DRUC is a design rules checker, it looks for design         #
#   in the layout description "chip.ap"                         #
#################################################################

druc_chip : chip.ap 
	MBK_IN_PH=ap ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib;\
	RDS_TECHNO_NAME=$(ALLIANCE_TOP)/etc/cmos_7.rds ;\
        RDS_IN=cif; RDS_OUT=cif;\
	export MBK_IN_PH MBK_WORK_LIB MBK_CATA_LIB RDS_TECHNO_NAME RDS_IN RDS_OUT;\
	$(DRUC) chip ;\
	echo "druc_chip done " >> druc_chip


#################################################################
#   RING routes the connections between the pads and the heart. #
#################################################################

chip.ap : lvx_heart chip.rin
	MBK_IN_LO=vst ;\
	MBK_IN_PH=ap ;\
	MBK_OUT_PH=ap ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib;\
	export MBK_IN_LO MBK_IN_PH MBK_OUT_PH MBK_WORK_LIB MBK_CATA_LIB;\
	$(RING) chip chip;\
	$(GRAAL) -l chip

#################################################################
#   LVX of the heart                                            #
#################################################################

lvx_heart : heart.al
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib;\
	export MBK_WORK_LIB MBK_CATA_LIB;\
	$(LVX) vst al heart heart -f
	echo "lvx_heart done " >> lvx_heart

#################################################################
#   LYNX of the heart                                           #
#################################################################

heart.al : druc_heart
	MBK_OUT_LO=al ;\
	MBK_IN_PH=ap ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib ;\
	export MBK_OUT_LO MBK_IN_PH MBK_WORK_LIB MBK_CATA_LIB;\
	$(LYNX) heart 

#################################################################
#   DRUC is a design rules checker, it looks for design         #
#   in the layout description "heart.ap"                        #
#################################################################

druc_heart : heart.ap
	MBK_IN_PH=ap ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib ;\
	RDS_TECHNO_NAME=$(ALLIANCE_TOP)/etc/cmos_7.rds ;\
        RDS_IN=cif; RDS_OUT=cif;\
	export MBK_IN_PH MBK_WORK_LIB MBK_CATA_LIB RDS_TECHNO_NAME RDS_IN RDS_OUT;\
	$(DRUC) heart ;\
	echo "druc_heart done " >> druc_heart

#################################################################
#   SCR is a standard cells router, it makes an automatic       #
#   placement and routing of the "heart.al" and gives the       #
#   corresponding layout description "heart.ap".                #
#################################################################

heart.ap : asimut_vst
	MBK_IN_PH=ap ;\
	MBK_OUT_PH=ap ;\
	MBK_IN_LO=vst ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib;\
        SCR_SCLIB=1 ;\
	export MBK_IN_PH MBK_OUT_PH MBK_IN_LO MBK_WORK_LIB MBK_CATA_LIB SCR_SCLIB ;\
	$(SCR) -p -r heart;\
#	$(GRAAL) -l heart

#################################################################
#   ASIMUT                                                      #
#   This time the "chip.vst" is                                 #
#   simulated by ASIMUT, using the test patternss "pattern.pat" #
#   previously generated in the behaviour step.                 #
#################################################################

asimut_vst : chip.vst
	MBK_IN_LO=vst ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib ;\
	VH_PATSFX=pat; export VH_PATSFX;\
	VH_MAXERR=10; export VH_MAXERR;\
	export MBK_IN_LO MBK_WORK_LIB MBK_CATA_LIB;\
	$(ASIMUT) chip pattern result_str ;\
	echo "asimut_vst done " >> asimut_vst

#################################################################
#   GENLIB is a procedural design language upon C.              #
#   every netlist source files with a ".c" extension are compiled
#   by GENLIB which gives the corresponding gate netlists ended #
#   by ".vst" extension                                         #
#################################################################

chip.vst : asimut_vbe chip.c
	(MBK_IN_LO=vst ;\
	MBK_OUT_LO=vst ;\
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib ;\
	export MBK_IN_LO MBK_OUT_LO MBK_WORK_LIB MBK_CATA_LIB;\
	$(GENLIB) -v chip)

#################################################################
#   ASIMUT                                                      #
#   the  hardware behaviour description "amd.vbe" is            #
#   simulated by ASIMUT, using the test patterns                #
#   previously generated in "pattern.pat".                      #
#################################################################


asimut_vbe : asimut_first
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=.:$(ALLIANCE_TOP)/cells/sclib:$(ALLIANCE_TOP)/cells/padlib;\
	export MBK_WORK_LIB MBK_CATA_LIB;\
	$(ASIMUT) -b amd pattern result_beh;\
	echo "asimut_vbe done " >> asimut_vbe

#################################################################
#  Asimut compilation						#
#################################################################

asimut_first : amd.vbe
	MBK_WORK_LIB=. ;\
	MBK_CATA_LIB=. ; \
	export MBK_WORK_LIB MBK_CATA_LIB;\
	VH_PATSFX=pat; export VH_PATSFX;\
	VH_MAXERR=10; export VH_MAXERR;\
	$(ASIMUT) -b -c amd;\
	echo "asimut_first done " >> asimut_first

clean :
	rm -f *.vst r*.pat *.al *.ap *.grr *.err
	rm -f *.o asimut_* druc_* lvx_* proof_*
	rm -f *.lis *.cif chip.vbe chip.d* *.SHP
	rm -f chip.fcf *.drc *.ttv
	rm -f chip.rep chip.ttx  chip_drc.gds chip_rng.gds heart_drc.gds heart_rng.gds *~
