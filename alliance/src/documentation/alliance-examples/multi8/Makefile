# /*------------------------------------------------------------\
# |                                                             |
# | File   :                    Makefile                        |
# |                                                             |
# | Author :                 Jacomme Ludovic                    |
# |                                                             |
# \------------------------------------------------------------*/
# /*------------------------------------------------------------\
# |                                                             |
# |                              Cells                          |
# |                                                             |
# \------------------------------------------------------------*/
# /*------------------------------------------------------------\
# |                                                             |
# |                             Binary                          |
# |                                                             |
# \------------------------------------------------------------*/

ALLIANCE_BIN=$(ALLIANCE_TOP)/bin
 
ALLIANCE_EXAMPLES=/usr/share/doc/alliance-doc-5.0/alliance-examples

VASY   = $(ALLIANCE_BIN)/vasy
ASIMUT = $(ALLIANCE_BIN)/asimut
BOOM   = $(ALLIANCE_BIN)/boom
BOOG   = $(ALLIANCE_BIN)/boog
LOON   = $(ALLIANCE_BIN)/loon
OCP    = $(ALLIANCE_BIN)/ocp
NERO   = $(ALLIANCE_BIN)/nero
COUGAR = $(ALLIANCE_BIN)/cougar
LVX    = $(ALLIANCE_BIN)/lvx
DRUC   = $(ALLIANCE_BIN)/druc
S2R    = $(ALLIANCE_BIN)/s2r

DREAL  = $(ALLIANCE_BIN)/dreal
GRAAL  = $(ALLIANCE_BIN)/graal
XSCH   = $(ALLIANCE_BIN)/xsch
XPAT   = $(ALLIANCE_BIN)/xpat
XFSM   = $(ALLIANCE_BIN)/xfsm

TOUCH  = touch

TARGET_LIB      = $(ALLIANCE_TOP)/cells/sxlib
RDS_TECHNO_SYMB = $(ALLIANCE_EXAMPLES)/etc/techno-symb.rds
RDS_TECHNO      = $(ALLIANCE_EXAMPLES)/etc/techno-035.rds
SPI_MODEL       = $(ALLIANCE_TOP)/etc/spimodel.cfg
METAL_LEVEL     = 2

# /*------------------------------------------------------------\
# |                                                             |
# |                            Environement                     |
# |                                                             |
# \------------------------------------------------------------*/

ENV_VASY = MBK_WORK_LIB=.; export MBK_WORK_LIB;\
           MBK_CATAL_NAME=NO_CATAL; export MBK_CATAL_NAME

ENV_BOOM = MBK_WORK_LIB=.; export MBK_WORK_LIB;\
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_BOOG = MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=vst; export MBK_IN_LO; \
	   MBK_OUT_LO=vst; export MBK_OUT_LO; \
           MBK_TARGET_LIB=$(TARGET_LIB); export MBK_TARGET_LIB; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_LOON = MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=vst; export MBK_IN_LO; \
	   MBK_OUT_LO=vst; export MBK_OUT_LO; \
           MBK_TARGET_LIB=$(TARGET_LIB); export MBK_TARGET_LIB; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_ASIMUT_VASY = MBK_WORK_LIB=.; export MBK_WORK_LIB;\
             MBK_CATAL_NAME=CATAL_ASIMUT_VASY; export MBK_CATAL_NAME;\
             MBK_IN_LO=vst; export MBK_IN_LO;\
	     MBK_OUT_LO=vst; export MBK_OUT_LO

ENV_ASIMUT_SYNTH = MBK_WORK_LIB=.; export MBK_WORK_LIB;\
             MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME;\
             MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
             MBK_IN_LO=vst; export MBK_IN_LO;\
	     MBK_OUT_LO=vst; export MBK_OUT_LO

ENV_OCP =  MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=vst; export MBK_IN_LO; \
	   MBK_OUT_LO=vst; export MBK_OUT_LO; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_NERO =  MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=vst; export MBK_IN_LO; \
	   MBK_OUT_LO=vst; export MBK_OUT_LO; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_COUGAR_SPI =  MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=spi; export MBK_IN_LO; \
	   MBK_OUT_LO=spi; export MBK_OUT_LO; \
           MBK_SPI_NAMEDNODES="true"; export MBK_SPI_NAMEDNODES; \
           MBK_SPI_MODEL=$(SPI_MODEL); export MBK_SPI_MODEL; \
           RDS_TECHNO_NAME=$(RDS_TECHNO); export RDS_TECHNO_NAME; \
           RDS_IN=cif; export RDS_IN; \
           RDS_OUT=cif; export RDS_OUT; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_COUGAR =  MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=al; export MBK_IN_LO; \
	   MBK_OUT_LO=al; export MBK_OUT_LO; \
           RDS_TECHNO_NAME=$(RDS_TECHNO); export RDS_TECHNO_NAME; \
           RDS_IN=cif; export RDS_IN; \
           RDS_OUT=cif; export RDS_OUT; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_LVX =  MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=vst; export MBK_IN_LO; \
	   MBK_OUT_LO=vst; export MBK_OUT_LO; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_DRUC = MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           RDS_TECHNO_NAME=$(RDS_TECHNO_SYMB); export RDS_TECHNO_NAME; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_S2R  = MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           RDS_TECHNO_NAME=$(RDS_TECHNO); export RDS_TECHNO_NAME; \
           RDS_IN=cif; export RDS_IN; \
           RDS_OUT=cif; export RDS_OUT; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME


all :  multi8.cif

# /*------------------------------------------------------------\
# |                                                             |
# |                             Vasy                            |
# |                                                             |
# \------------------------------------------------------------*/

multi8.vst addaccu.vbe controller.vbe sra.vbe srb.vbe multi8_model.vbe : multi8.vhdl addaccu.vhdl \
                controller.vhdl multi8.vhdl sra.vhdl srb.vhdl
	$(ENV_VASY); $(VASY) -a -B -o -p -I vhdl -H multi8

# /*------------------------------------------------------------\
# |                                                             |
# |                             Asimut                          |
# |                                                             |
# \------------------------------------------------------------*/

res_vasy_1.pat : multi8.vst addaccu.vbe sra.vbe srb.vbe \
                 controller.vbe multi8_model.vbe
	$(ENV_ASIMUT_VASY); $(ASIMUT) multi8 multi8 res_vasy_1 

res_synth_1.pat : multi8.vst addaccu.vst sra.vst srb.vst \
                 controller.vst multi8_model.vst
	$(ENV_ASIMUT_SYNTH); $(ASIMUT) multi8 multi8 res_synth_1

# /*------------------------------------------------------------\
# |                                                             |
# |                             Boom                            |
# |                                                             |
# \------------------------------------------------------------*/

boom.done : addaccu_o.vbe sra_o.vbe srb_o.vbe controller_o.vbe \
            multi8_model_o.vbe
	@$(TOUCH) boom.done

addaccu_o.vbe : addaccu.vbe addaccu.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP addaccu addaccu_o

sra_o.vbe : sra.vbe sra.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP sra sra_o

srb_o.vbe : srb.vbe srb.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP srb srb_o

controller_o.vbe : controller.vbe controller.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP controller controller_o

multi8_model_o.vbe : multi8_model.vbe multi8_model.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP multi8_model multi8_model_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Boog                            |
# |                                                             |
# \------------------------------------------------------------*/

boog.done : addaccu_o.vst sra_o.vst srb_o.vst controller_o.vst \
            multi8_model_o.vst
	@$(TOUCH) boog.done

addaccu_o.vst : addaccu_o.vbe
	$(ENV_BOOG); $(BOOG) addaccu_o

sra_o.vst : sra_o.vbe
	$(ENV_BOOG); $(BOOG) sra_o

srb_o.vst : srb_o.vbe
	$(ENV_BOOG); $(BOOG) srb_o

controller_o.vst : controller_o.vbe
	$(ENV_BOOG); $(BOOG) controller_o

multi8_model_o.vst : multi8_model_o.vbe
	$(ENV_BOOG); $(BOOG) multi8_model_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Loon                            |
# |                                                             |
# \------------------------------------------------------------*/

loon.done : addaccu.vst sra.vst srb.vst controller.vst \
            multi8_model.vst
	@$(TOUCH) loon.done

addaccu.vst : addaccu_o.vst
	$(ENV_LOON); $(LOON) addaccu_o addaccu

sra.vst : sra_o.vst
	$(ENV_LOON); $(LOON) sra_o sra

srb.vst : srb_o.vst
	$(ENV_LOON); $(LOON) srb_o srb

controller.vst : controller_o.vst
	$(ENV_LOON); $(LOON) controller_o controller

multi8_model.vst : multi8_model_o.vst
	$(ENV_LOON); $(LOON) multi8_model_o multi8_model

# /*------------------------------------------------------------\
# |                                                             |
# |                             OCP                             |
# |                                                             |
# \------------------------------------------------------------*/

multi8_p.ap : res_synth_1.pat
	$(ENV_OCP); $(OCP) -v -ioc multi8 -gnuplot  multi8 multi8_p

# /*------------------------------------------------------------\
# |                                                             |
# |                             NERO                            |
# |                                                             |
# \------------------------------------------------------------*/

multi8.ap : multi8_p.ap multi8.vst
	$(ENV_NERO); $(NERO) -V -$(METAL_LEVEL) -p multi8_p multi8 multi8

# /*------------------------------------------------------------\
# |                                                             |
# |                             Cougar                          |
# |                                                             |
# \------------------------------------------------------------*/

multi8_e.spi : multi8.ap
	$(ENV_COUGAR_SPI); $(COUGAR) -v multi8 multi8_e

multi8_e.al : multi8.ap
	$(ENV_COUGAR); $(COUGAR) -v -ac multi8 multi8_e

multi8_et.al : multi8.ap
	$(ENV_COUGAR); $(COUGAR) -v -ac -t multi8 multi8_et

multi8_et.spi : multi8.ap
	$(ENV_COUGAR_SPI); $(COUGAR) -v -ac -t multi8 multi8_et

multi8_er.spi : multi8.cif
	$(ENV_COUGAR); $(COUGAR) -v -r -t multi8 multi8_er

# /*------------------------------------------------------------\
# |                                                             |
# |                             Lvx                             |
# |                                                             |
# \------------------------------------------------------------*/

lvx.done : multi8.vst multi8_e.al  multi8_e.spi
	$(ENV_LVX); $(LVX) vst al multi8 multi8_e -f
	$(TOUCH) lvx.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             Druc                            |
# |                                                             |
# \------------------------------------------------------------*/

druc.done : lvx.done multi8.ap
	$(ENV_DRUC); $(DRUC) multi8
	$(TOUCH) druc.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             S2R                             |
# |                                                             |
# \------------------------------------------------------------*/

multi8.cif : druc.done
	$(ENV_S2R); $(S2R) -v -t multi8

# /*------------------------------------------------------------\
# |                                                             |
# |                             TOOLS                           |
# |                                                             |
# \------------------------------------------------------------*/


graal: multi8.ap
	$(ENV_S2R); $(GRAAL) -l multi8

xsch: multi8.vst
	$(ENV_LOON); $(XSCH) -l multi8

xscht: multi8_et.al
	$(ENV_COUGAR); $(XSCH) -l multi8_et

xpat: res_synth_1.pat
	$(ENV_ASIMUT_SYNTH); $(XPAT) -l res_synth_1

dreal: multi8.cif
	$(ENV_S2R); $(DREAL) -l multi8

# /*------------------------------------------------------------\
# |                                                             |
# |                              Clean                          |
# |                                                             |
# \------------------------------------------------------------*/

realclean : clean

clean     :
	$(RM) -f *.vst multi8_et.spi *.vbe res_*.pat *.boom *.done *.xsc *.gpl \
                 *.ap *.drc *.dat *.gds *.cif *.rep \
		 *.log *.out *.raw *.al
	
