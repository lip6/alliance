# /*------------------------------------------------------------\
# |                                                             |
# | Tool   :                      Paper                         |
# |                                                             |
# | File   :                    Makefile                        |
# |                                                             |
# | Author :                 Jacomme Ludovic                    |
# |                         and Fred the king                   |
# |                                                             |
# | Date   :                    01.01.96                        |
# |                                                             |
# \------------------------------------------------------------*/
# /*------------------------------------------------------------\
# |                                                             |
# |                            Variables                        |
# |                                                             |
# \------------------------------------------------------------*/

OPTION = o
FORMAT = fsm
FILE   = digi

FILE_NAME = $(FILE)_$(OPTION)

# /*------------------------------------------------------------\
# |                                                             |
# |                        Power Consumption                    |
# |                                                             |
# \------------------------------------------------------------*/

CLOCK          = clock
FREQUENCY      = 40
CLOCK_WAVE     = "01"
PATTERN_STEP   = 12500ps
PATTERN_PICO   = 25000
SWING          = 3.3
CAPA_OUT       = 100

# CLOCK          = clock
# FREQUENCY      = 20
# CLOCK_WAVE     = "01"
# PATTERN_STEP   = 25ns
# PATTERN_PICO   = 50000
# SWING          = 3.3
# CAPA_OUT       = 100

# /*------------------------------------------------------------\
# |                                                             |
# |                          Alliance Tools                     |
# |                                                             |
# \------------------------------------------------------------*/

SYF      = $(ALLIANCE_TOP)/bin/syf -VC
ASIMUT   = $(ALLIANCE_TOP)/bin/asimut
LVX      = $(ALLIANCE_TOP)/bin/lvx
SCR      = $(ALLIANCE_TOP)/bin/scr
BOOM     = $(ALLIANCE_TOP)/bin/boom
BOOG     = $(ALLIANCE_TOP)/bin/boog
LOON     = $(ALLIANCE_TOP)/bin/loon
LYNX     = $(ALLIANCE_TOP)/bin/lynx
TAS      = $(ALLIANCE_TOP)/bin/tas
YAGLE    = $(ALLIANCE_TOP)/bin/yagle

POCPAG   = $(ALLIANCE_TOP)/bin/pocpag
DTX2DLY  = $(ALLIANCE_TOP)/bin/dtx2dly
SICC     = $(ALLIANCE_TOP)/bin/sicc
EPOC     = $(ALLIANCE_TOP)/bin/epoc

# /*------------------------------------------------------------\
# |                                                             |
# |                          Unix Command                       |
# |                                                             |
# \------------------------------------------------------------*/

RM     = rm
MV     = mv
CP     = cp
CUT    = cut
GREP   = grep
TOUCH  = touch
GZIP   = gzip
TAIL   = tail

# /*------------------------------------------------------------\
# |                                                             |
# |                          Cells libraries                    |
# |                                                             |
# \------------------------------------------------------------*/

CELLS      = $(ALLIANCE_TOP)/cells
CELLS_SCR  = $(ALLIANCE_CELLS)/sxlib

# /*------------------------------------------------------------\
# |                                                             |
# |                             Techno                          |
# |                                                             |
# \------------------------------------------------------------*/

TAS_TECHNO = $(ALLIANCE_TOP)/etc/prol05.elp
RDS_TECHNO = $(ALLIANCE_TOP)/etc/prol05.rds

# /*------------------------------------------------------------\
# |                                                             |
# |                       Environement Variables                |
# |                                                             |
# \------------------------------------------------------------*/

ENV_VST = MBK_IN_LO=vst;              export MBK_IN_LO; \
          MBK_OUT_LO=vst;             export MBK_OUT_LO; \
          MBK_WORK_LIB=.;             export MBK_WORK_LIB; \
          MBK_CATA_LIB=$(CELLS_SCR);  export MBK_CATA_LIB
ENV_AL  = MBK_IN_LO=al;               export MBK_IN_LO; \
          MBK_OUT_LO=al;              export MBK_OUT_LO; \
          MBK_WORK_LIB=.;             export MBK_WORK_LIB; \
          MBK_CATA_LIB=$(CELLS_SCR);  export MBK_CATA_LIB
ENV_AP  = MBK_IN_PH=ap;               export MBK_IN_PH; \
          MBK_OUT_PH=ap;              export MBK_OUT_PH; \
          MBK_WORK_LIB=.;             export MBK_WORK_LIB
ENV_VH  = VH_BEHSFX=vbe;              export VH_BEHSFX; \
          VH_MAXERR="10";             export VH_MAXERR; \
          VH_PATSFX=pat;              export VH_PATSFX
ENV_SL  = MBK_TARGET_LIB=$(CELLS_SCR);export MBK_TARGET_LIB; \
          MBK_VDD=vdd;                export MBK_VDD; \
          MBK_VSS=vss;                export MBK_VSS
ENV_FSM = FSM_IN=$(FORMAT);           export FSM_IN

ENV_RDS = RDS_TECHNO_NAME=$(RDS_TECHNO); export RDS_TECHNO_NAME

# /*------------------------------------------------------------\
# |                                                             |
# |                                 All                         |
# |                                                             |
# \------------------------------------------------------------*/

all : $(FILE_NAME).pow

# /*------------------------------------------------------------\
# |                                                             |
# |                             Synthesis                       |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME).vbe : $(FILE).$(FORMAT)
	$(ENV_FSM); $(SYF) -$(OPTION) $(FILE) $(FILE_NAME)

# /*------------------------------------------------------------\
# |                                                             |
# |                          Optimization                       |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME)_o.vbe : $(FILE_NAME).vbe
	$(ENV_SL); $(BOOM) -V $(FILE_NAME) $(FILE_NAME)_o

# /*------------------------------------------------------------\
# |                                                             |
# |                             Mapping                         |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME).vst : $(FILE_NAME)_o.vbe
	$(ENV_SL); $(ENV_VST); $(BOOG) $(FILE_NAME)_o $(FILE_NAME)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Netoptim                        |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME)_o.vst : $(FILE_NAME).vst
	$(ENV_SL); $(ENV_VST); $(LOON) $(FILE_NAME) $(FILE_NAME)_o 

# /*------------------------------------------------------------\
# |                                                             |
# |                              Route                          |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME).ap : $(FILE_NAME)_o.vst
	$(ENV_VST); $(ENV_AP); $(SCR) -p -r $(FILE_NAME)_o -o $(FILE_NAME)

# /*------------------------------------------------------------\
# |                                                             |
# |                              Extract                        |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME)_t.al : $(FILE_NAME).ap
	$(ENV_AL); $(ENV_AP); $(ENV_RDS); $(LYNX) -v -t -ac $(FILE_NAME) $(FILE_NAME)_t

$(FILE_NAME).al : $(FILE_NAME).ap
	$(ENV_AL); $(ENV_AP); $(ENV_RDS); $(LYNX) -v -ac $(FILE_NAME)

# /*------------------------------------------------------------\
# |                                                             |
# |                              NetCompare                     |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME).lvx : $(FILE_NAME).al $(FILE_NAME).vst
	$(ENV_AL); $(LVX) vst al $(FILE_NAME)_o $(FILE_NAME)
	$(TOUCH) $(FILE_NAME).lvx

# /*------------------------------------------------------------\
# |                                                             |
# |                              Timing                         |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME).dtx : $(FILE_NAME)_t.al $(FILE_NAME).lvx
	ELP_TECHNO_NAME=$(TAS_TECHNO); export ELP_TECHNO_NAME;\
	$(ENV_AL); $(TAS) -t -tec=$(TAS_TECHNO) $(FILE_NAME)_t
	$(MV) $(FILE_NAME)_t.dtx $(FILE_NAME).dtx

$(FILE_NAME)_yagh.vbe $(FILE_NAME)_t_yagh.al : $(FILE_NAME)_t.al
	ELP_TECHNO_NAME=$(TAS_TECHNO); export ELP_TECHNO_NAME;\
	$(ENV_AL); $(YAGLE) -v -nc -h -ffs -t -d -elp $(FILE_NAME)_t $(FILE_NAME)_yagh

# /*------------------------------------------------------------\
# |                                                             |
# |                              Conso                          |
# |                                                             |
# \------------------------------------------------------------*/

$(FILE_NAME).sta : $(FILE_NAME).pat $(FILE_NAME).lvx $(FILE_NAME)_yagh.vbe
	$(ENV_AL); $(ASIMUT) -b -stat $(PATTERN_PICO) \
	           $(FILE_NAME) $(FILE_NAME)_yagh \
	            $(FILE_NAME) $(FILE_NAME)_res

$(FILE_NAME).pat : $(FILE_NAME)_yagh.vbe
	$(POCPAG) -clk ck $(CLOCK_WAVE) -dly $(PATTERN_STEP) -b -nbr 1000 \
	        $(FILE_NAME) default $(FILE_NAME)

$(FILE_NAME).dly : $(FILE_NAME).dtx
	$(DTX2DLY) $(FILE_NAME)

$(FILE_NAME).nrj : $(FILE_NAME).dly $(FILE_NAME)_t_yagh.al
	$(ENV_AL); $(SICC) $(SWING) $(CAPA_OUT) $(FILE_NAME)_t_yagh $(FILE_NAME) 

$(FILE_NAME).pow : $(FILE_NAME).sta $(FILE_NAME).nrj $(FILE_NAME)_yagh.vbe
	$(ENV_AL); $(EPOC) $(FILE_NAME)_yagh $(FILE_NAME) $(FILE_NAME) $(FILE_NAME) $(FREQUENCY)

# /*------------------------------------------------------------\
# |                                                             |
# |                            Clean up                         |
# |                                                             |
# \------------------------------------------------------------*/

clean :
	@$(RM) -f $(FILE_NAME)*.vbe \
	          $(FILE_NAME)*.vst \
	          $(FILE_NAME)*.xsc \
	          $(FILE_NAME)*.ap  \
	          $(FILE_NAME)*.al  \
	          $(FILE_NAME)*.lvx \
	          $(FILE_NAME)*.cns \
	          $(FILE_NAME)*.dtx \
	          $(FILE_NAME)*.ttx \
	          $(FILE_NAME)*.dly \
                  $(FILE_NAME)*.sta \
	          $(FILE_NAME)*.nrj \
                  $(FILE_NAME)*.pat \
                  $(FILE_NAME)*.rep \
                  $(FILE_NAME)*.stb \
                  $(FILE_NAME)*.pow \
		  CATAL
