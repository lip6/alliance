

# Standart System binary access paths.
 STANDART_BIN  = /bin:/usr/local/bin:/usr/bin
 STANDART_PATH = PATH=$(STANDART_BIN); export PATH

# Standart Alliance binary access paths.
 ALLIANCE_BIN  = $(ALLIANCE_TOP)/bin
 CELLS_TOP     = $(ALLIANCE_TOP)/cells
 
ALLIANCE_EXAMPLES=/usr/share/doc/alliance-doc-5.0/alliance-examples

# FitPath Alliance binary access paths.
#    DEVEL_BIN  = /users/soft5/newlabo/alliance-4.5.0/archi/Solaris/bin

# --------------------------------------------------------------------
# Standarts binaries.

           LS =                  /bin/ls
           CD = PATH=$(STANDART_BIN); cd
           CP = PATH=$(STANDART_BIN); cp -f
           LN = PATH=$(STANDART_BIN); ln
           MV = PATH=$(STANDART_BIN); mv
           RM = PATH=$(STANDART_BIN); rm -f
          SED = PATH=$(STANDART_BIN); sed
          AWK = PATH=$(STANDART_BIN); gawk
          CAT = PATH=$(STANDART_BIN); cat
         MAKE = PATH=$(STANDART_BIN); make
         ECHO =                  /bin/echo
#  Alliance paths and formats settings.
 GENERAT_LO   = vst
 EXTRACT_LO   = al
 CARAC_LO     = spi
 GENERAT_PH   = ap
 EXTRACT_PH   = ap
 GENERAT_SP   = .
 EXTRACT_SP   = .
    CATA_LIB0 = $(CELLS_TOP)/sxlib
    CATA_LIB1 = $(CELLS_TOP)/dp_sxlib
    CATA_LIB2 = $(CELLS_TOP)/padlib
    CATA_LIB3 = $(CELLS_TOP)/rflib
    CATA_LIB  = .:$(CATA_LIB0):$(CATA_LIB1):$(CATA_LIB2):$(CATA_LIB3)
   TARGET_LIB = $(CELLS_TOP)/sxlib
    FPGEN_LIB = $(CATA_LIB0):$(CATA_LIB1)
  TECHNO_NAME = local-cmos
  CARAC_TECHNO_NAME = local-cmos-035
CARAC_SPI_MODEL = ./model

SPI_MODEL       = $(ALLIANCE_TOP)/etc/spimodel.cfg
RDS_TECHNO      = local-cmos-035.rds

ENV_COUGAR_SPI =  MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=spi; export MBK_IN_LO; \
	   MBK_OUT_LO=spi; export MBK_OUT_LO; \
           MBK_SPI_MODEL=$(SPI_MODEL); export MBK_SPI_MODEL; \
           MBK_SPI_NAMEDNODES="true"; export MBK_SPI_NAMEDNODES; \
           RDS_TECHNO_NAME=$(RDS_TECHNO); export RDS_TECHNO_NAME; \
           RDS_IN=cif; export RDS_IN; \
           RDS_OUT=cif; export RDS_OUT; \
           MBK_CATA_LIB=$(CATA_LIB); export MBK_CATA_LIB; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME


 MBK_GENERAT_ENV = MBK_TARGET_LIB=$(TARGET_LIB);  	export MBK_TARGET_LIB;\
                   MBK_WORK_LIB=.;                	export MBK_WORK_LIB;  \
                   MBK_CATA_LIB=$(CATA_LIB);      	export MBK_CATA_LIB;  \
                   MBK_CATAL_NAME=CATAL;          	export MBK_CATAL_NAME;\
                   MBK_OUT_LO=$(GENERAT_LO);      	export MBK_OUT_LO;    \
                   MBK_OUT_PH=$(GENERAT_PH);      	export MBK_OUT_PH;    \
                   MBK_IN_LO=$(GENERAT_LO);       	export MBK_IN_LO;     \
                   MBK_IN_PH=$(GENERAT_PH);       	export MBK_IN_PH;     \
                   MBK_SEPAR=$(GENERAT_SP);       	export MBK_SEPAR;     \
                   MBK_VDD=vdd;                 	export MBK_VDD;       \
                   MBK_VSS=vss;                   	export MBK_VSS;       \
                   FPGEN_LIB=$(FPGEN_LIB);        	export FPGEN_LIB;     \
		   RDS_TECHNO_NAME=$(TECHNO_NAME).rds;	export RDS_TECHNO_NAME

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
		   RDS_TECHNO_NAME=$(TECHNO_NAME).rds;	export RDS_TECHNO_NAME

# MBK caracterisation.
 MBK_CARAC_ENV =   MBK_TARGET_LIB=$(TARGET_LIB); export MBK_TARGET_LIB; \
                   MBK_WORK_LIB=.;               export MBK_WORK_LIB;   \
                   MBK_CATA_LIB=$(CATA_LIB);     export MBK_CATA_LIB;   \
                   MBK_CATAL_NAME=CATAL;         export MBK_CATAL_NAME; \
                   MBK_OUT_LO=$(CARAC_LO);     export MBK_OUT_LO;     \
		   MBK_SPI_MODEL=$(CARAC_SPI_MODEL); export MBK_SPI_MODEL; \
                   MBK_OUT_PH=$(EXTRACT_PH);     export MBK_OUT_PH;     \
                   MBK_IN_LO=$(EXTRACT_LO);      export MBK_IN_LO;      \
                   MBK_IN_PH=$(EXTRACT_PH);      export MBK_IN_PH;      \
                   MBK_SEPAR=$(EXTRACT_SP);      export MBK_SEPAR;      \
                   MBK_VDD=vdd;                  export MBK_VDD;        \
                   MBK_VSS=vss;                  export MBK_VSS;	\
                   RDS_IN=cif; export RDS_IN; RDS_OUT=cif; export RDS_OUT;\
		   RDS_TECHNO_NAME=$(CARAC_TECHNO_NAME).rds;	export RDS_TECHNO_NAME


# --------------------------------------------------------------------
# Alliance binaries & environment.

    GRAAL = $(MBK_GENERAT_ENV); GRAAL_TECHNO_NAME=$(TECHNO_NAME).graal; export GRAAL_TECHNO_NAME;$(ALLIANCE_BIN)/graal
   ASIMUT = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/asimut -zd -bdd
#   ASIMUT = $(MBK_GENERAT_ENV); /users/enseig/mips4/new_asimut -zd -bdd
  FLATBEH = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/flatbeh
  FLATLO  = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/flatlo
     LYNX = $(MBK_EXTRACT_ENV); $(ALLIANCE_BIN)/cougar -v
    LYNX3 = $(ENV_COUGAR_SPI);   $(ALLIANCE_BIN)/cougar -v
     DRUC = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/druc
      LVX = $(MBK_EXTRACT_ENV); $(ALLIANCE_BIN)/lvx
    PROOF = $(MBK_EXTRACT_ENV); $(ALLIANCE_BIN)/proof
     RING = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/ring
    DPGEN = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/genlib --keep-exec --verbose
    OCP   = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/ocp -v -gnuplot
    OCR   = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/ocr
    NERO  = $(MBK_GENERAT_ENV); $(ALLIANCE_BIN)/nero -V
      S2R = $(MBK_CARAC_ENV);   $(ALLIANCE_BIN)/s2r -v




GENPAT		= $(ALLIANCE_BIN)/genpat
GENLIB		= $(MBK_GENERAT_ENV);$(ALLIANCE_BIN)/genlib
SYF		= $(MBK_GENERAT_ENV);$(ALLIANCE_BIN)/syf -a -CVE
BOOM		= $(MBK_GENERAT_ENV);$(ALLIANCE_BIN)/boom -A -V -l 2 -s
BOOG		= $(MBK_GENERAT_ENV);$(ALLIANCE_BIN)/boog
LOON		= $(MBK_GENERAT_ENV);$(ALLIANCE_BIN)/loon 
SCAPIN		= $(MBK_GENERAT_ENV);$(ALLIANCE_BIN)/scapin -VRB
MIPS_ASM	= $(ALLIANCE_BIN)/mips_asm
# pour un bench (test en cours de creation on pourra l'enlever apres)
BENCH	    = ./bench.sh add000 CONF
# pour tester l'ensemble des programmes (confirmation finale)
ALLBENCH    = ./allbench.sh DEFINITIF

# /*------------------------------------------------------------\
# |                                                             |
# |                          Alliance Tools                     |
# |                                                             |
# \------------------------------------------------------------*/


# /*------------------------------------------------------------\
# |                                                             |
# |                       Environement Variables                |
# |                                                             |
# \------------------------------------------------------------*/
# /*------------------------------------------------------------\
# |                                                             |
# |                          Unix Command                       |
# |                                                             |
# \------------------------------------------------------------*/

# /*------------------------------------------------------------\
# |                                                             |
# |								|
# |                                                             |
# \------------------------------------------------------------*/

mips_chip.cif : mips_chip.ap
	$(S2R) -r mips_chip mips_chip

mips_chip_t.al : mips_chip.ap
	$(LYNX) -t -ac -ar mips_chip mips_chip_t

mips_core_t.al : mips_core.ap
	$(LYNX) -c -t -ac -ar mips_core mips_core_t

mips_chip.ap : mips_core.ap mips_chip.vst mips_chip.rin mips_core.vst mips_core.al
	$(CP) CATAL_VST CATAL
	$(RING) mips_chip mips_chip
       
mips_core.al : mips_core.ap
	$(CP) CATAL_VST CATAL
	$(LYNX) -c -f mips_core mips_core
	$(LVX) vst al mips_core mips_core -f

mips_core.spi : mips_core.ap
	$(LYNX3) -ac -f mips_core mips_core

mips_core_et.spi : mips_core.ap
	$(LYNX3) -ac -t mips_core mips_core_et

# mips_core.ap : mips_core_p.ap mips_ctl.vst
# 	$(CP) CATAL_VST CATAL
# 	$(OCR) -P mips_core_p -L mips_core -O mips_core_p_ocr -v -w 1 -l 5 -i 100
# 	cat mips_core_p_ocr.ap | sed s/core_p_ocr/core/ > mips_core.ap
# 	$(DRUC) mips_core

mips_core.ap : mips_core_p.ap mips_ctl.vst
	$(CP) CATAL_VST CATAL
	$(NERO) -6 -p mips_core_p mips_core mips_core_p_nero
	cat mips_core_p_nero.ap | sed s/core_p_nero/core/ > mips_core.ap
	$(DRUC) mips_core

mips_core_p.ap : mips_core_place.ap mips_ctl.vst mips_core.ioc
	$(CP) CATAL_VST CATAL
	$(OCP) -partial mips_core_place -ioc  mips_core mips_core mips_core_p

mips_core_place.ap : mips_core.c mips_dpt.ap
	$(DPGEN) mips_core

mips_ctl.vst : mips_ctl_loon.vst mips_ctl.path
	$(SCAPIN) -P mips_ctl.scapin mips_ctl_loon mips_ctl mips_ctl

mips_ctl_loon.vst : mips_sts.vst mips_seqo.vst mips_ctl.lax
	$(CP) CATAL_CTL CATAL
	$(FLATLO) -r mips_ctl_nt mips_ctl_flat
	$(LOON) mips_ctl_flat mips_ctl_loon -l mips_ctl 

mips_seqo.vst : mips_seqo_optim.vbe mips_ctl.lax
	$(BOOG) mips_seqo_optim mips_seqo -l mips_ctl

mips_sts.vst : mips_sts_optim.vbe mips_ctl.lax
	$(BOOG) mips_sts_optim mips_sts -l mips_ctl

mips_seqo_optim.vbe : mips_seqo.vbe
	$(BOOM) mips_seqo mips_seqo_optim

mips_sts_optim.vbe : mips_sts.vbe
	$(BOOM) mips_sts mips_sts_optim

mips_bench : mips_cpu.pat mips_cpu.vst
	$(BENCH)
	@echo "<---------- resultat a regarder dans CONF ---------------->"


mips_dpt.ap model_shift.ap : mips_dpt.c
	$(DPGEN) mips_dpt

mips_seqo.vbe : mips_seq.fsm
	$(SYF) 	mips_seq mips_seqo

mips_allbench :
	$(CP) CATAL_VST CATAL 
	$(ALLBENCH)
	@echo "<---------- resultat a regarder dans DEFINITIF ---------------->"

mips_test : mips_ctl.vst mips_scan.pat
	$(ASIMUT) -p 50 mips_cpu mips_scan resultat 2> ajeter

mips_scan.pat : mips_scan.c
	$(GENPAT) mips_scan

graal :
	$(GRAAL)

clean :
	$(RM) mips_dpt.vst;
	$(RM) *.dtx;
	$(RM) *.ttx;
	$(RM) *.drc;
	$(RM) *.rcx;
	$(RM) *.cif;
	$(RM) *.spi;
	$(RM) *.rep;
	$(RM) *.ap;
	$(RM) *.al;
	$(RM) mips_ctl.vst;
	$(RM) mips_ctl_flat.vst;
	$(RM) mips_ctl_loon.vst;
	$(RM) res.pat
	$(RM) resultat.pat
	$(RM) res2
	$(RM) mips_sts.xsc
	$(RM) mips_sts.vst
	$(RM) mips_sts_optim.vbe	
	$(RM) mips_seqo_optim.vbe	
	$(RM) machin		 
	$(RM) bench/*.good 	
	$(RM) CONF 		
	$(RM) CATAL 		
	$(RM) dat* 		
	$(RM) DEFINITIF		
	$(RM) mips_seqo*	
	$(RM) *.xsc	
	$(RM) *.gpl	
	$(RM) *.gds	
	$(RM) model*
	$(RM) alldata.dat
