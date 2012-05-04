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

include ../etc/alliance-env.mk

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

TARGET_LIB      = $(CELLS_TOP)/sxlib
SPI_MODEL       = $(SYSCONF_TOP)/spimodel.cfg
METAL_LEVEL     = 5

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
           RDS_TECHNO_NAME=$(RDS_TECHNO_REAL); export RDS_TECHNO_NAME; \
           RDS_IN=cif; export RDS_IN; \
           RDS_OUT=cif; export RDS_OUT; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

ENV_COUGAR =  MBK_WORK_LIB=.; export MBK_WORK_LIB; \
           MBK_IN_LO=al; export MBK_IN_LO; \
	   MBK_OUT_LO=al; export MBK_OUT_LO; \
           RDS_TECHNO_NAME=$(RDS_TECHNO_REAL); export RDS_TECHNO_NAME; \
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
           RDS_TECHNO_NAME=$(RDS_TECHNO_REAL); export RDS_TECHNO_NAME; \
           RDS_IN=cif; export RDS_IN; \
           RDS_OUT=cif; export RDS_OUT; \
           MBK_IN_PH=ap; export MBK_IN_PH; \
           MBK_OUT_PH=ap; export MBK_OUT_PH; \
           MBK_CATA_LIB=$(TARGET_LIB); export MBK_CATA_LIB; \
           MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME

all :  sqrt32.cif

# /*------------------------------------------------------------\
# |                                                             |
# |                             Vasy                            |
# |                                                             |
# \------------------------------------------------------------*/

sqrt32.vst s0.vbe s0b.vbe s1.vbe s1b.vbe sb.vbe sm.vbe sn.vbe sqrt32_model.vbe : sqrt32.vhdl
	$(ENV_VASY); $(VASY) -a -B -o -p -I vhdl -H sqrt32

# /*------------------------------------------------------------\
# |                                                             |
# |                             Asimut                          |
# |                                                             |
# \------------------------------------------------------------*/

res_vasy_1.pat : sqrt32.vst s0.vbe s0b.vbe s1.vbe s1b.vbe sb.vbe sm.vbe sn.vbe sqrt32_model.vbe
	$(ENV_ASIMUT_VASY); $(ASIMUT) sqrt32 sqrt32 res_vasy_1 

res_synth_1.pat : sqrt32.vst s0.vst s0b.vst s1.vst s1b.vst sb.vst sm.vst sn.vst sqrt32_model.vst 
	$(ENV_ASIMUT_SYNTH); $(ASIMUT) sqrt32 sqrt32 res_synth_1


# /*------------------------------------------------------------\
# |                                                             |
# |                             Boom                            |
# |                                                             |
# \------------------------------------------------------------*/

boom.done : s0_o.vbe s0b_o.vbe s1_o.vbe s1b_o.vbe sb_o.vbe sm_o.vbe sn_o.vbe sqrt32_model_o.vbe
	@$(TOUCH) boom.done

s0_o.vbe : s0.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP s0 s0_o

s0b_o.vbe : s0b.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP s0b s0b_o

s1_o.vbe : s1.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP s1 s1_o

s1b_o.vbe : s1b.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP s1b s1b_o

sb_o.vbe : sb.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP sb sb_o

sm_o.vbe : sm.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP sm sm_o

sn_o.vbe : sn.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP sn sn_o

sqrt32_model_o.vbe : sqrt32_model.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP sqrt32_model sqrt32_model_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Boog                            |
# |                                                             |
# \------------------------------------------------------------*/

boog.done : sqrt32.vst s0_o.vst s0b_o.vst s1_o.vst s1b_o.vst sb_o.vst sm_o.vst sn_o.vst sqrt32_model_o.vst
	@$(TOUCH) boog.done

s0_o.vst : s0_o.vbe res_vasy_1.pat
	$(ENV_BOOG); $(BOOG) s0_o

s0b_o.vst : s0b_o.vbe res_vasy_1.pat
	$(ENV_BOOG); $(BOOG) s0b_o

s1_o.vst : s1_o.vbe res_vasy_1.pat
	$(ENV_BOOG); $(BOOG) s1_o

s1b_o.vst : s1b_o.vbe res_vasy_1.pat
	$(ENV_BOOG); $(BOOG) s1b_o

sb_o.vst : sb_o.vbe res_vasy_1.pat
	$(ENV_BOOG); $(BOOG) sb_o

sm_o.vst : sm_o.vbe res_vasy_1.pat
	$(ENV_BOOG); $(BOOG) sm_o

sn_o.vst : sn_o.vbe res_vasy_1.pat
	$(ENV_BOOG); $(BOOG) sn_o

sqrt32_model_o.vst : sqrt32_model_o.vbe
	$(ENV_BOOG); $(BOOG) sqrt32_model_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Loon                            |
# |                                                             |
# \------------------------------------------------------------*/

loon.done : sqrt32.vst s0.vst s0b.vst s1.vst s1b.vst sb.vst sm.vst sn.vst sqrt32_model.vst
	@$(TOUCH) loon.done

s0.vst : s0_o.vst res_vasy_1.pat
	$(ENV_LOON); $(LOON) s0_o s0

s0b.vst : s0b_o.vst res_vasy_1.pat
	$(ENV_LOON); $(LOON) s0b_o s0b

s1.vst : s1_o.vst res_vasy_1.pat
	$(ENV_LOON); $(LOON) s1_o s1

s1b.vst : s1b_o.vst res_vasy_1.pat
	$(ENV_LOON); $(LOON) s1b_o s1b

sb.vst : sb_o.vst res_vasy_1.pat
	$(ENV_LOON); $(LOON) sb_o sb

sm.vst : sm_o.vst res_vasy_1.pat
	$(ENV_LOON); $(LOON) sm_o sm

sn.vst : sn_o.vst res_vasy_1.pat
	$(ENV_LOON); $(LOON) sn_o sn

sqrt32_model.vst : sqrt32_model_o.vst
	$(ENV_LOON); $(LOON) sqrt32_model_o sqrt32_model

# /*------------------------------------------------------------\
# |                                                             |
# |                             OCP                             |
# |                                                             |
# \------------------------------------------------------------*/

sqrt32_p.ap : res_synth_1.pat
	$(ENV_OCP); $(OCP) -v -ioc sqrt32 -gnuplot  sqrt32 sqrt32_p

# /*------------------------------------------------------------\
# |                                                             |
# |                             NERO                            |
# |                                                             |
# \------------------------------------------------------------*/

sqrt32.ap : sqrt32_p.ap sqrt32.vst
	$(ENV_NERO); $(NERO) -V -$(METAL_LEVEL) -p sqrt32_p sqrt32 sqrt32

# /*------------------------------------------------------------\
# |                                                             |
# |                             Cougar                          |
# |                                                             |
# \------------------------------------------------------------*/

sqrt32_e.al : sqrt32.ap
	$(ENV_COUGAR); $(COUGAR) -v -ac sqrt32 sqrt32_e

sqrt32_et.al : sqrt32.ap
	$(ENV_COUGAR); $(COUGAR) -v -t -ac sqrt32 sqrt32_et

sqrt32_e.spi : sqrt32.ap sqrt32_e.al
	$(ENV_COUGAR_SPI); $(COUGAR) -v -ac sqrt32 sqrt32_e

sqrt32_et.spi : sqrt32.ap sqrt32_e.al
	$(ENV_COUGAR_SPI); $(COUGAR) -v -t -ac sqrt32 sqrt32_et

sqrt32_er.al : sqrt32.cif
	$(ENV_COUGAR); $(COUGAR) -v -r -t sqrt32 sqrt32_er

# /*------------------------------------------------------------\
# |                                                             |
# |                             Lvx                             |
# |                                                             |
# \------------------------------------------------------------*/

lvx.done : sqrt32.vst sqrt32_e.al 
	$(ENV_LVX); $(LVX) vst al sqrt32 sqrt32_e -f
	$(TOUCH) lvx.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             Druc                            |
# |                                                             |
# \------------------------------------------------------------*/

druc.done : lvx.done sqrt32.ap
	$(ENV_DRUC); $(DRUC) sqrt32
	$(TOUCH) druc.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             S2R                             |
# |                                                             |
# \------------------------------------------------------------*/

sqrt32.cif : druc.done
	$(ENV_S2R); $(S2R) -v -t sqrt32

# /*------------------------------------------------------------\
# |                                                             |
# |                             TOOLS                           |
# |                                                             |
# \------------------------------------------------------------*/

graal: sqrt32.ap
	$(ENV_S2R); $(GRAAL) -l sqrt32

xsch: sqrt32.vst
	$(ENV_LOON); $(XSCH) -l sqrt32

xscht: sqrt32_et.al
	$(ENV_COUGAR); $(XSCH) -l sqrt32_et

xpat: res_synth_1.pat
	$(ENV_ASIMUT_SYNTH); $(XPAT) -l res_synth_1

dreal: sqrt32.cif
	$(ENV_S2R); $(DREAL) -l sqrt32

# /*------------------------------------------------------------\
# |                                                             |
# |                              Clean                          |
# |                                                             |
# \------------------------------------------------------------*/

realclean : clean

clean     :
	$(RM) -f *.vst *.vbe *.boom *.done *.xsc *.al *.ap *.gpl *.gds \
	      *.log *.drc *.cif *.fin *.dat *.out sqrt32_e.spi res_synth_1.pat \
	      res_vasy_1.pat
	
