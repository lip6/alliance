

     ALLIANCE_TOP = /asim/alliance
      TECHNO_REAL = prol10


#
# /------------------------------------------------------------------\
# |                        Macros definitions                        |
# \------------------------------------------------------------------/
#

# Standart System binary access paths.
 STANDART_BIN  = /usr/local/bin:/labo/gnu/bin:/usr/bin:/bin
 STANDART_PATH = PATH=$(STANDART_BIN); export PATH

# Standart Alliance binary access paths.
 ALLIANCE_BIN  = $(ALLIANCE_TOP)/bin

# FitPath Alliance binary access paths.
    DEVEL_BIN  = $(DEVEL_TOP)/bin


# --------------------------------------------------------------------
# Standarts binaries.

           LS =                  /bin/ls
           CD = PATH=$(STANDART_BIN); cd
           CP = PATH=$(STANDART_BIN); cp
           LN = PATH=$(STANDART_BIN); ln
           MV = PATH=$(STANDART_BIN); mv
           RM = PATH=$(STANDART_BIN); rm
          SED = PATH=$(STANDART_BIN); sed
          AWK = PATH=$(STANDART_BIN); gawk
          CAT = PATH=$(STANDART_BIN); cat
         MAKE = PATH=$(STANDART_BIN); make
       TOUCH = PATH=$(STANDART_BIN); touch
       GREP = PATH=$(STANDART_BIN); grep
         ECHO =                  /bin/echo
#  Alliance paths and formats settings.
 GENERAT_LO   = vst
 EXTRACT_LO   = al
 GENERAT_PH   = ap
 EXTRACT_PH   = ap
 GENERAT_SP   = .
 EXTRACT_SP   = .
    CATA_LIB0 = $(ALLIANCE_TOP)/cells/sxlib
    CATA_LIB1 = $(ALLIANCE_TOP)/cells/dp_sxlib
    CATA_LIB2 = $(ALLIANCE_TOP)/cells/padlib
    CATA_LIB  = .:$(CATA_LIB0):$(CATA_LIB1):$(CATA_LIB2)
   TARGET_LIB = $(MBK_TARGET_LIB)
   RDS_TECHNO = $(ALLIANCE_TOP)/etc/cmos.rds
   GRAAL_TECHNO = $(ALLIANCE_TOP)/etc/cmos.graal


 MBK_GENERAT_ENV = MBK_TARGET_LIB=$(TARGET_LIB);  export MBK_TARGET_LIB;    \
                   MBK_WORK_LIB=.;                export MBK_WORK_LIB;      \
                   MBK_CATA_LIB=$(CATA_LIB);	  export MBK_CATA_LIB;      \
                   MBK_CATAL_NAME=CATAL;          export MBK_CATAL_NAME;    \
                   MBK_OUT_LO=$(GENERAT_LO);      export MBK_OUT_LO;        \
                   MBK_OUT_PH=$(GENERAT_PH);      export MBK_OUT_PH;        \
                   MBK_IN_LO=$(GENERAT_LO);       export MBK_IN_LO;         \
                   MBK_IN_PH=$(GENERAT_PH);       export MBK_IN_PH;         \
                   MBK_SEPAR=$(GENERAT_SP);       export MBK_SEPAR;         \
                   MBK_VDD=vdd;                   export MBK_VDD;           \
                   MBK_VSS=vss;                   export MBK_VSS;	    \
		   RDS_TECHNO_NAME=$(RDS_TECHNO); export RDS_TECHNO_NAME; \
		   GRAAL_TECHNO_NAME=$(GRAAL_TECHNO); export GRAAL_TECHNO_NAME
		   
# MBK extracting environment.
 MBK_EXTRACT_ENV = MBK_TARGET_LIB=$(TARGET_LIB); export MBK_TARGET_LIB; \
                   MBK_WORK_LIB=.;               export MBK_WORK_LIB;   \
                   MBK_CATA_LIB=$(CATA_LIB);     export MBK_CATA_LIB;   \
                   MBK_CATAL_NAME=CATAL;         export MBK_CATAL_NAME; \
                   MBK_OUT_LO=$(EXTRACT_LO);     export MBK_OUT_LO;     \
                   MBK_OUT_PH=$(EXTRACT_PH);     export MBK_OUT_PH;     \
                   MBK_IN_LO=$(EXTRACT_LO);      export MBK_IN_LO;      \
                   MBK_IN_PH=$(EXTRACT_PH);      export MBK_IN_PH;      \
                   MBK_SEPAR=$(EXTRACT_SP);      export MBK_SEPAR;      \
                   MBK_VDD=vdd;                  export MBK_VDD;        \
                   MBK_VSS=vss;                  export MBK_VSS;	\
		   RDS_TECHNO_NAME=$(RDS_TECHNO); export RDS_TECHNO_NAME;\
                   GRAAL_TECHNO_NAME=$(GRAAL_TECHNO); export GRAAL_TECHNO_NAME
		   


# --------------------------------------------------------------------
# Alliance binaries & environment.

     BOOM = $(ALLIANCE_BIN)/boom -V
     BOOG = $(ALLIANCE_BIN)/boog 
     LOON = $(ALLIANCE_BIN)/loon
   ASIMUT1 = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/asimut
   ASIMUT2 = $(MBK_EXTRACT_ENV); $(ALLIANCE_BIN)/asimut
   COUGAR = $(ALLIANCE_BIN)/cougar
     DRUC = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/druc
      LVX = $(MBK_EXTRACT_ENV); $(ALLIANCE_BIN)/lvx
    PROOF = $(MBK_EXTRACT_ENV); $(ALLIANCE_BIN)/proof
   GENLIB = $(MBK_GENERAT_ENV);$(ALLIANCE_BIN)/genlib
   GENPAT = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/genpat
      OCP = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/ocp 
      OCR = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/ocr 
     RING = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/ring
    GRAAL = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/graal
    XSCH  = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/xsch
    XPAT  = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/xpat



#
# /------------------------------------------------------------------\
# |                              Rules                               |
# \------------------------------------------------------------------/
#

all: nb_transistors 

ctl_part: amd2901_ctl.vst

view_ctl_logic: amd2901_ctl.vst
	$(XSCH) -l amd2901_ctl 

dpt_part: amd2901_dpt.ap amd2901_dpt.vst

view_dpt_physic: amd2901_dpt.ap
	$(GRAAL) -l amd2901_dpt

chip_part: amd2901_chip.ap

view_chip_physic: amd2901_chip.ap
	$(GRAAL) -l amd2901_chip

chip_verification: druc_chip lvx_chip test_chip_final.pat

view_chip_simulation: test_chip_final.pat
	$(XPAT) -l test_chip_final

amd2901_core.vst amd2901_core_place.ap: amd2901_core.c amd2901_ctl.vst amd2901_ctl.vbe amd2901_dpt.vst amd2901_dpt.ap 
	$(GENLIB) -v amd2901_core

amd2901_chip.vst: amd2901_core.vst 
	$(GENLIB) -v amd2901_chip

test_chip.pat: amd2901_chip.vst pattern.pat
	$(ASIMUT1) -zd amd2901_chip pattern test_chip

pattern.pat: pattern.c
	$(GENPAT) -v pattern

amd2901_dpt.ap amd2901_dpt.vst: amd2901_dpt.c
	$(GENLIB) -v amd2901_dpt	

amd2901_ctl_boom.vbe : amd2901_ctl.vbe
	$(BOOM) amd2901_ctl amd2901_ctl_boom

amd2901_ctl_boog.vst : amd2901_ctl_boom.vbe amd2901_ctl.lax
	$(BOOG) amd2901_ctl_boom amd2901_ctl_boog amd2901_ctl

amd2901_ctl.vst : amd2901_ctl_boog.vst
	$(LOON) amd2901_ctl_boog amd2901_ctl

amd2901_core_p.ap: amd2901_core.vst amd2901_core_place.ap amd2901_core.ioc
	$(OCP) -v -gnuplot -partial amd2901_core_place -ioc amd2901_core amd2901_core amd2901_core_p
#	$(OCP) -v -gnuplot -partial amd2901_core_place -ring amd2901_core amd2901_core_p

druc_ocp: amd2901_core_p.ap
	$(DRUC) amd2901_core_p
	$(TOUCH) druc_ocp

amd2901_core.ap: druc_ocp
	$(OCR) -P amd2901_core_p -L amd2901_core -O amd2901_core -l 3 -v -i 30

amd2901_core.al: amd2901_core.ap
	$(MBK_EXTRACT_ENV); $(COUGAR) -v -f amd2901_core amd2901_core

lvx_core: amd2901_core.al amd2901_core.vst
	$(LVX) al vst amd2901_core amd2901_core -f
	$(TOUCH) lvx_core

druc_core: amd2901_core.ap
	$(DRUC) amd2901_core
	$(TOUCH) druc_core

amd2901_chip.ap: test_chip.pat amd2901_core.ap amd2901_chip.rin druc_core lvx_core
	$(MBK_GENERAT_ENV); $(RING) amd2901_chip amd2901_chip

amd2901_chip.al: amd2901_chip.ap
	$(MBK_EXTRACT_ENV); $(COUGAR) -v -f amd2901_chip amd2901_chip

lvx_chip: amd2901_chip.al amd2901_chip.vst
	$(LVX) al vst amd2901_chip amd2901_chip -f
	$(TOUCH) lvx_chip

druc_chip: amd2901_chip.ap
	$(DRUC) amd2901_chip
	$(TOUCH) druc_chip

test_chip_final.pat: pattern.pat druc_chip lvx_chip
	$(ASIMUT2) -zd amd2901_chip pattern test_chip_final

nb_transistors: amd2901_chip_tr.al amd2901_core_tr.al 
	@echo "Number of transistors for the core: ";\
	$(GREP) -c "^T" amd2901_core_tr.al
	@echo "Number of transistors for the chip: ";\
	$(GREP) -c "^T" amd2901_chip_tr.al

amd2901_chip_tr.al: test_chip_final.pat
	$(MBK_EXTRACT_ENV); $(COUGAR) -v -t amd2901_chip amd2901_chip_tr

amd2901_core_tr.al: lvx_core druc_core
	$(MBK_EXTRACT_ENV); $(COUGAR) -v -t amd2901_core amd2901_core_tr

graal:
	$(GRAAL) -install
	
clean :
	rm -f Makefile-*                 \
	      amd2901_core.vst 		\
	      amd2901_chip.vst 		\
	      amd2901_ctl.vst	 	\
	      amd2901_dpt.vst	 	\
	      amd2901_ctl_boog.vst \
	      amd2901_ctl_boom.vbe \
	      *.ap \
          	res.pat  \
		*.frr                      \
	      *.log                \
	      *.drc                      \
	      *.gds                      \
	      *.def			\
	      *.gpl		\
	      *.xsc		\
	      *.al	\
	      *.pat	\
	      *~	\
	      *cif \
	      lvx_core druc_core \
	      lvx_chip druc_chip \
	      druc_ocp \
	      alldata* \
	      model_*
