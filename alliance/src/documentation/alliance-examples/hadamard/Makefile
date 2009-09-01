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

ALLIANCE_BIN = $(ALLIANCE_TOP)/bin
 
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
METAL_LEVEL     = 6

# /*------------------------------------------------------------\
# |                                                             |
# |                            Environment                     |
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

all :  hadamard_er.al

# /*------------------------------------------------------------\
# |                                                             |
# |                             Vasy                            |
# |                                                             |
# \------------------------------------------------------------*/

hadamard.vst calcul.vbe ram.vbe rom.vbe compteur.vbe sequenceur.vbe hadamard_model.vbe : hadamard.vhdl
	$(ENV_VASY); $(VASY) -a -B -o -p -I vhdl -H hadamard

# /*------------------------------------------------------------\
# |                                                             |
# |                             Asimut                          |
# |                                                             |
# \------------------------------------------------------------*/

res_vasy_1.pat : hadamard.vst calcul.vbe ram.vbe rom.vbe \
                 compteur.vbe sequenceur.vbe hadamard_model.vbe
	$(ENV_ASIMUT_VASY); $(ASIMUT) hadamard hadamard_1 res_vasy_1

res_synth_1.pat : hadamard.vst calcul.vst ram.vst rom.vst \
                 compteur.vst sequenceur.vst hadamard_model.vst
	$(ENV_ASIMUT_SYNTH); $(ASIMUT) -zd hadamard hadamard_1 res_synth_1


# /*------------------------------------------------------------\
# |                                                             |
# |                             Boom                            |
# |                                                             |
# \------------------------------------------------------------*/

boom.done : calcul_o.vbe ram_o.vbe rom_o.vbe compteur_o.vbe \
            sequenceur_o.vbe hadamard_model_o.vbe
	@$(TOUCH) boom.done

calcul_o.vbe : calcul.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP calcul calcul_o

ram_o.vbe : ram.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP ram ram_o

rom_o.vbe : rom.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP rom rom_o

compteur_o.vbe : compteur.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP compteur compteur_o

sequenceur_o.vbe : sequenceur.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP sequenceur sequenceur_o

hadamard_model_o.vbe : hadamard_model.vbe res_vasy_1.pat
	$(ENV_BOOM); $(BOOM) -VP hadamard_model hadamard_model_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Boog                            |
# |                                                             |
# \------------------------------------------------------------*/

boog.done : calcul_o.vst ram_o.vst rom_o.vst compteur_o.vst \
            sequenceur_o.vst hadamard_model_o.vst
	@$(TOUCH) boog.done

calcul_o.vst : calcul_o.vbe
	$(ENV_BOOG); $(BOOG) calcul_o

ram_o.vst : ram_o.vbe
	$(ENV_BOOG); $(BOOG) ram_o

rom_o.vst : rom_o.vbe
	$(ENV_BOOG); $(BOOG) rom_o

compteur_o.vst : compteur_o.vbe
	$(ENV_BOOG); $(BOOG) compteur_o

sequenceur_o.vst : sequenceur_o.vbe
	$(ENV_BOOG); $(BOOG) sequenceur_o

hadamard_model_o.vst : hadamard_model_o.vbe
	$(ENV_BOOG); $(BOOG) hadamard_model_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Loon                            |
# |                                                             |
# \------------------------------------------------------------*/

loon.done : calcul.vst ram.vst rom.vst compteur.vst \
            sequenceur.vst hadamard_model.vst
	@$(TOUCH) loon.done

calcul.vst : calcul_o.vst
	$(ENV_LOON); $(LOON) calcul_o calcul

ram.vst : ram_o.vst
	$(ENV_LOON); $(LOON) ram_o ram

rom.vst : rom_o.vst
	$(ENV_LOON); $(LOON) rom_o rom

compteur.vst : compteur_o.vst
	$(ENV_LOON); $(LOON) compteur_o compteur

sequenceur.vst : sequenceur_o.vst
	$(ENV_LOON); $(LOON) sequenceur_o sequenceur

hadamard_model.vst : hadamard_model_o.vst
	$(ENV_LOON); $(LOON) hadamard_model_o hadamard_model

# /*------------------------------------------------------------\
# |                                                             |
# |                             OCP                             |
# |                                                             |
# \------------------------------------------------------------*/

hadamard_p.ap : res_synth_1.pat
	$(ENV_OCP); $(OCP) -v -ioc hadamard -margin 0.4 -gnuplot  hadamard hadamard_p

# /*------------------------------------------------------------\
# |                                                             |
# |                             NERO                            |
# |                                                             |
# \------------------------------------------------------------*/

hadamard.ap : hadamard_p.ap hadamard.vst
	$(ENV_NERO); $(NERO) -V -$(METAL_LEVEL) -p hadamard_p hadamard hadamard

# /*------------------------------------------------------------\
# |                                                             |
# |                             Cougar                          |
# |                                                             |
# \------------------------------------------------------------*/

hadamard_e.al : hadamard.ap
	$(ENV_COUGAR); $(COUGAR) -v -ac hadamard hadamard_e

hadamard_et.al : hadamard.ap
	$(ENV_COUGAR); $(COUGAR) -v -t -ac hadamard hadamard_et

hadamard_e.spi : hadamard.ap hadamard_e.al
	$(ENV_COUGAR_SPI); $(COUGAR) -v -ac hadamard hadamard_e

hadamard_et.spi : hadamard.ap hadamard_e.al
	$(ENV_COUGAR_SPI); $(COUGAR) -v -t -ac hadamard hadamard_et

hadamard_er.al : hadamard.cif
	$(ENV_COUGAR); $(COUGAR) -v -r -t hadamard hadamard_er

# /*------------------------------------------------------------\
# |                                                             |
# |                             Lvx                             |
# |                                                             |
# \------------------------------------------------------------*/

lvx.done : hadamard.vst hadamard_e.al
	$(ENV_LVX); $(LVX) vst al hadamard hadamard_e -f
	$(TOUCH) lvx.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             Druc                            |
# |                                                             |
# \------------------------------------------------------------*/

druc.done : lvx.done hadamard.ap
	$(ENV_DRUC); $(DRUC) hadamard
	$(TOUCH) druc.done

# /*------------------------------------------------------------\
# |                                                             |
# |                             S2R                             |
# |                                                             |
# \------------------------------------------------------------*/

hadamard.cif : druc.done
	$(ENV_S2R); $(S2R) -v -t hadamard

# /*------------------------------------------------------------\
# |                                                             |
# |                             TOOLS                           |
# |                                                             |
# \------------------------------------------------------------*/

graal: hadamard.ap
	$(ENV_S2R); $(GRAAL) -l hadamard

xsch: hadamard.vst
	$(ENV_LOON); $(XSCH) -l hadamard

xscht: hadamard_et.al
	$(ENV_COUGAR); $(XSCH) -l hadamard_et

xpat: res_synth_1.pat
	$(ENV_ASIMUT_SYNTH); $(XPAT) -l res_synth_1

dreal: hadamard.cif
	$(ENV_S2R); $(DREAL) -l hadamard

# /*------------------------------------------------------------\
# |                                                             |
# |                              Clean                          |
# |                                                             |
# \------------------------------------------------------------*/

realclean : clean

clean     :
	$(RM) -f *.vst *.vbe *.boom *.done *.xsc *.al *.ap *.gpl *.gds \
	      *.log *.drc *.cif *.fin *.dat *.out hadamard_e.spi res_synth_1.pat \
	      res_vasy_1.pat

