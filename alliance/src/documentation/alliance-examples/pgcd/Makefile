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


all :  pgcd.cif

# /*------------------------------------------------------------\
# |                                                             |
# |                             Vasy                            |
# |                                                             |
# \------------------------------------------------------------*/

pgcd.vst calcul.vbe sequenceur.vbe pgcd_model.vbe : pgcd.vhdl calcul.vhdl \
                sequenceur.vhdl pgcd.vhdl 
	$(ENV_VASY); $(VASY) -a -B -o -p -I vhdl -H pgcd

# /*------------------------------------------------------------\
# |                                                             |
# |                             Asimut                          |
# |                                                             |
# \------------------------------------------------------------*/

res_vasy_1.pat : pgcd.vst calcul.vbe \
                 sequenceur.vbe pgcd_model.vbe
	$(ENV_ASIMUT_VASY); $(ASIMUT) pgcd pgcd res_vasy_1 

res_synth_1.pat : pgcd.vst calcul.vst \
                 sequenceur.vst pgcd_model.vst
	$(ENV_ASIMUT_SYNTH); $(ASIMUT) pgcd pgcd res_synth_1

# /*------------------------------------------------------------\
# |                                                             |
# |                             Boom                            |
# |                                                             |
# \------------------------------------------------------------*/

boom.done : calcul_o.vbe sequenceur_o.vbe \
            pgcd_model_o.vbe
	@$(TOUCH) boom.done

calcul_o.vbe : calcul.vbe calcul.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VPA calcul calcul_o

sequenceur_o.vbe : sequenceur.vbe sequenceur.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP sequenceur sequenceur_o

pgcd_model_o.vbe : pgcd_model.vbe pgcd_model.boom res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP pgcd_model pgcd_model_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Boog                            |
# |                                                             |
# \------------------------------------------------------------*/

boog.done : calcul_o.vst sequenceur_o.vst \
            pgcd_model_o.vst
	@$(TOUCH) boog.done

calcul_o.vst : calcul_o.vbe
	$(ENV_BOOG); $(BOOG) calcul_o

sequenceur_o.vst : sequenceur_o.vbe
	$(ENV_BOOG); $(BOOG) sequenceur_o

pgcd_model_o.vst : pgcd_model_o.vbe
	$(ENV_BOOG); $(BOOG) pgcd_model_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Loon                            |
# |                                                             |
# \------------------------------------------------------------*/

loon.done : calcul.vst sequenceur.vst \
            pgcd_model.vst
	@$(TOUCH) loon.done

calcul.vst : calcul_o.vst
	$(ENV_LOON); $(LOON) calcul_o calcul

sequenceur.vst : sequenceur_o.vst
	$(ENV_LOON); $(LOON) sequenceur_o sequenceur

pgcd_model.vst : pgcd_model_o.vst
	$(ENV_LOON); $(LOON) pgcd_model_o pgcd_model

# /*------------------------------------------------------------\
# |                                                             |
# |                             OCP                             |
# |                                                             |
# \------------------------------------------------------------*/

pgcd_p.ap : res_synth_1.pat
	$(ENV_OCP); $(OCP) -v -ioc pgcd -gnuplot  pgcd pgcd_p

# /*------------------------------------------------------------\
# |                                                             |
# |                             NERO                            |
# |                                                             |
# \------------------------------------------------------------*/

pgcd.ap : pgcd_p.ap pgcd.vst
	$(ENV_NERO); $(NERO) -V -$(METAL_LEVEL) -p pgcd_p pgcd pgcd

# /*------------------------------------------------------------\
# |                                                             |
# |                             Cougar                          |
# |                                                             |
# \------------------------------------------------------------*/

pgcd_e.spi : pgcd.ap
	$(ENV_COUGAR_SPI); $(COUGAR) -v -ac pgcd pgcd_e

pgcd_e.al : pgcd.ap
	$(ENV_COUGAR); $(COUGAR) -v -ac pgcd pgcd_e

pgcd_et.al : pgcd.ap
	$(ENV_COUGAR); $(COUGAR) -v -t -ac pgcd pgcd_et

pgcd_et.spi : pgcd.ap
	$(ENV_COUGAR_SPI); $(COUGAR) -v -ac -t pgcd pgcd_et

pgcd_er.spi : pgcd.cif
	$(ENV_COUGAR); $(COUGAR) -v -r -t pgcd pgcd_er

# /*------------------------------------------------------------\
# |                                                             |
# |                             Lvx                             |
# |                                                             |
# \------------------------------------------------------------*/

lvx.done : pgcd.vst pgcd_e.al  pgcd_e.spi
	$(ENV_LVX); $(LVX) vst al pgcd pgcd_e -f
	$(TOUCH) lvx.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             Druc                            |
# |                                                             |
# \------------------------------------------------------------*/

druc.done : lvx.done pgcd.ap
	$(ENV_DRUC); $(DRUC) pgcd
	$(TOUCH) druc.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             S2R                             |
# |                                                             |
# \------------------------------------------------------------*/

pgcd.cif : druc.done
	$(ENV_S2R); $(S2R) -v -t pgcd

# /*------------------------------------------------------------\
# |                                                             |
# |                             TOOLS                           |
# |                                                             |
# \------------------------------------------------------------*/

graal: pgcd.ap
	$(ENV_S2R); $(GRAAL) -l pgcd

xsch: pgcd.vst
	$(ENV_LOON); $(XSCH) -l pgcd

xscht: pgcd_et.al
	$(ENV_COUGAR); $(XSCH) -l pgcd_et

xpat: res_synth_1.pat
	$(ENV_ASIMUT_SYNTH); $(XPAT) -l res_synth_1

dreal: pgcd.cif
	$(ENV_S2R); $(DREAL) -l pgcd


# /*------------------------------------------------------------\
# |                                                             |
# |                              Clean                          |
# |                                                             |
# \------------------------------------------------------------*/

realclean : clean

clean     :
	$(RM) -f *.vst pgcd_e.spi *.vbe res_*.pat *.boom *.done *.xsc *.gpl \
                 *.ap *.drc *.dat *.gds *.cif *.rep *.stm-data *.stm-plot \
		 *.log *.out *.raw *.al
