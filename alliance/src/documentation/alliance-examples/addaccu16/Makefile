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
RDS_TECHNO_SYMB = ../etc/techno-symb.rds
RDS_TECHNO      = ../etc/techno-035.rds
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
           MBK_SPI_MODEL=$(SPI_MODEL); export MBK_SPI_MODEL; \
           MBK_SPI_ONE_NODE_NORC="true"; export MBK_SPI_ONE_NODE_NORC; \
           MBK_SPI_NAMEDNODES="true"; export MBK_SPI_NAMEDNODES; \
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


all :  addaccu.cif

# /*------------------------------------------------------------\
# |                                                             |
# |                             Vasy                            |
# |                                                             |
# \------------------------------------------------------------*/

addaccu.vbe : addaccu.vhdl
	$(ENV_VASY); $(VASY) -a -B -o -p -I vhdl addaccu

# /*------------------------------------------------------------\
# |                                                             |
# |                             Asimut                          |
# |                                                             |
# \------------------------------------------------------------*/

res_vasy_1.pat : addaccu.vbe
	$(ENV_ASIMUT_VASY); $(ASIMUT) -b addaccu addaccu res_vasy_1 

res_synth_1.pat : addaccu.vst 
	$(ENV_ASIMUT_SYNTH); $(ASIMUT) addaccu addaccu res_synth_1

# /*------------------------------------------------------------\
# |                                                             |
# |                             Boom                            |
# |                                                             |
# \------------------------------------------------------------*/

boom.done : addaccu_o.vbe
	@$(TOUCH) boom.done

addaccu_o.vbe : addaccu.vbe addaccu.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP addaccu addaccu_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Boog                            |
# |                                                             |
# \------------------------------------------------------------*/

boog.done : addaccu_o.vst
	@$(TOUCH) boog.done

addaccu_o.vst : addaccu_o.vbe
	$(ENV_BOOG); $(BOOG) addaccu_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Loon                            |
# |                                                             |
# \------------------------------------------------------------*/

loon.done : addaccu.vst
	@$(TOUCH) loon.done

addaccu.vst : addaccu_o.vst
	$(ENV_LOON); $(LOON) addaccu_o addaccu

# /*------------------------------------------------------------\
# |                                                             |
# |                             OCP                             |
# |                                                             |
# \------------------------------------------------------------*/

addaccu_p.ap : res_synth_1.pat
	$(ENV_OCP); $(OCP) -v -gnuplot -ioc addaccu  addaccu addaccu_p

# /*------------------------------------------------------------\
# |                                                             |
# |                             NERO                            |
# |                                                             |
# \------------------------------------------------------------*/

addaccu.ap : addaccu_p.ap addaccu.vst
	$(ENV_NERO); $(NERO) -V -$(METAL_LEVEL) -p addaccu_p addaccu addaccu

# /*------------------------------------------------------------\
# |                                                             |
# |                             Cougar                          |
# |                                                             |
# \------------------------------------------------------------*/

addaccu_e.spi : addaccu.ap
	$(ENV_COUGAR_SPI); $(COUGAR) -v -ac addaccu addaccu_e

addaccu_erc.spi : addaccu.ap
	$(ENV_COUGAR_SPI); $(COUGAR) -v -ar addaccu addaccu_erc

addaccu_erc.al : addaccu.ap
	$(ENV_COUGAR); $(COUGAR) -v -ar addaccu addaccu_erc

addaccu_e.al : addaccu.ap
	$(ENV_COUGAR); $(COUGAR) -v -ac addaccu addaccu_e

addaccu_et.al : addaccu.ap
	$(ENV_COUGAR); $(COUGAR) -v -ac -t addaccu addaccu_et

addaccu_et.spi : addaccu.ap
	$(ENV_COUGAR_SPI); $(COUGAR) -v -ac -t addaccu addaccu_et

addaccu_er.al : addaccu.cif
	$(ENV_COUGAR); $(COUGAR) -v -r -t addaccu addaccu_er

addaccu_real.al : addaccu.ap
	$(ENV_COUGAR); $(ENV_S2R); $(COUGAR) -v -ac addaccu addaccu_real

addaccu_real_t.al : addaccu.ap
	$(ENV_COUGAR); $(ENV_S2R); $(COUGAR) -v -t -ac addaccu addaccu_real_t

# /*------------------------------------------------------------\
# |                                                             |
# |                             Lvx                             |
# |                                                             |
# \------------------------------------------------------------*/

lvx.done : addaccu.vst addaccu_e.al addaccu_e.spi
	$(ENV_LVX); $(LVX) vst al addaccu addaccu_e -f
	$(TOUCH) lvx.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             Druc                            |
# |                                                             |
# \------------------------------------------------------------*/

druc.done : lvx.done addaccu.ap
	$(ENV_DRUC); $(DRUC) addaccu
	$(TOUCH) druc.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             S2R                             |
# |                                                             |
# \------------------------------------------------------------*/

addaccu.cif : druc.done
	$(ENV_S2R); $(S2R) -v addaccu

# /*------------------------------------------------------------\
# |                                                             |
# |                             TOOLS                           |
# |                                                             |
# \------------------------------------------------------------*/

graal :
	$(ENV_S2R); $(GRAAL)

graal_addaccu_p : addaccu_p.ap
	$(ENV_S2R); $(GRAAL) -l addaccu_p

graal_addaccu : addaccu.ap
	$(ENV_S2R); $(GRAAL) -l addaccu

xsch:
	$(ENV_LOON); $(XSCH)

xsch_addaccu_o : addaccu.vst
	$(ENV_LOON); $(XSCH) -l addaccu_o

xsch_addaccu : addaccu.vst
	$(ENV_LOON); $(XSCH) -l addaccu

xsch_addaccu_e: addaccu_e.al
	$(ENV_COUGAR); $(XSCH) -l addaccu_e

xsch_addaccu_et: addaccu_et.al
	$(ENV_COUGAR); $(XSCH) -l addaccu_et

xpat: 
	$(ENV_ASIMUT_SYNTH); $(XPAT)

xpat_synth: res_synth_1.pat
	$(ENV_ASIMUT_SYNTH); $(XPAT) -l res_synth_1

xpat_vasy : res_vasy_1.pat
	$(ENV_ASIMUT_SYNTH); $(XPAT) -l res_vasy_1

dreal: 
	$(ENV_S2R); $(DREAL)

dreal_addaccu : addaccu.cif
	$(ENV_S2R); $(DREAL) -l addaccu


# /*------------------------------------------------------------\
# |                                                             |
# |                              Clean                          |
# |                                                             |
# \------------------------------------------------------------*/

realclean : clean

clean     :
	$(RM) -f *.vst addaccu_e.spi addaccu_et.spi *.vbe res_*.pat *.boom *.done *.xsc *.gpl \
                 *.ap *.drc *.dat *.gds *.cif *.rep \
		 *.log *.out *.raw *.al
