# ###---------------------------------------------------------###
# file		: libraries.mk					#
# description	: Alliance Shared Libraries and Include Files	#
# ###---------------------------------------------------------###

# Last updated on on Friday 06 August 1999 at 13:48 MET DST

# The variables $TOP and $MACHINE are set by
# alc_env.[c]sh script

ALLIANCE_VERSION = '"3.5.003"'
ALLIANCE_BIN     = $(TOP)/bin
ALLIANCE_LIB     = $(TOP)/lib
ALLIANCE_INCLUDE = $(TOP)/include
ALLIANCE_CELLS   = $(TOP)/cells
ALLIANCE_ETC     = $(TOP)/etc

# distrib path
#TARGET_BIN       = $(TOP)/bin
#TARGET_LIB       = $(TOP)/lib
#TARGET_INCLUDE   = $(TOP)/include

# develloppers path
TARGET_BIN       = $(HOME)/labo/$(MACHINE)/bin
TARGET_LIB       = $(HOME)/labo/$(MACHINE)/lib
TARGET_INCLUDE   = $(HOME)/labo/$(MACHINE)/include

# ###---------------------------------------------------------###
#    general purpose utilities					#
# ###---------------------------------------------------------###

MUT_L      =  -lMut325
MUT_LIB    = libMut325.a
MUT_H      =    mut325.h

# ###---------------------------------------------------------###
#    functions related to cone net-list representation :	#
#								#
#      - cns : cone net_list					#
#      - yag : cone extractor form transistor net-list		#
# ###---------------------------------------------------------###

CNS_L      =  -lCns211
CNS_LIB    = libCns211.a
CNS_H      =    cns211.h

YAG_L      =  -lYag300
YAG_LIB    = libYag300.a
YAG_H      =    yag300.h

# ###---------------------------------------------------------###
#    functions related to net-list representation :		#
#								#
#      - rcn : data structures for resistor representation	#
#      - mlo : basic data structures				#
#								#
#      - mal : parser & driver for Al format			#
#      - mcl : parser & driver for COMPASS net-lists		#
#      - mel : parser & driver for EDIF				#
#      - mgl :          driver for VERILOG net-lists		#
#      - mhl :          driver for GHDL net-list		#
#      - msl : parser & driver for SPICE			#
#      - mvl : parser & driver for VHDL net-lists		#
#								#
#      - mlu : user level functions				#
# ###---------------------------------------------------------###


RCN_L      =  -lRcn200
RCN_LIB    = libRcn200.a
RCN_H      =    rcn200.h

MLO_L      =  -lMlo501
MLO_LIB    = libMlo501.a
MLO_H      =    mlo501.h

MAL_L      =  -lMal603
MAL_LIB    = libMal603.a

MCL_L      =  -lMcl413
MCL_LIB    = libMcl413.a

MEL_L      =  -lMel407
MEL_LIB    = libMel407.a

MHL_L      =  -lMhl403
MHL_LIB    = libMhl403.a

MGL_L      =  -lMgl100
MGL_LIB    = libMgl100.a

MSL_L      =  -lMsl700
MSL_LIB    = libMsl700.a
MSL_H      =    msl700.h

MVL_L      =  -lMvl409
MVL_LIB    = libMvl409.a

MLU_L      =  -lMlu501
MLU_LIB    = libMlu501.a
MLU_H      =    mlu501.h

# ###---------------------------------------------------------###
#    functions related to symbolic layout representation :	#
#								#
#      - mph : basic data structures				#
#								#
#      - map : parser & driver for AP format			#
#      - mcp : parser & driver for COMPASS Compose layout	#
#      - mmg : parser & driver for MODGEN			#
#								#
#      - mpu : user level functions				#
# ###---------------------------------------------------------###

MPH_L      =  -lMph409
MPH_LIB    = libMph409.a
MPH_H      =    mph409.h

MAP_L      =  -lMap406
MAP_LIB    = libMap406.a
MCP_L      =  -lMcp407
MCP_LIB    = libMcp407.a
MMG_L      =  -lMmg402
MMG_LIB    = libMmg402.a

MPU_L      =  -lMpu405
MPU_LIB    = libMpu405.a
MPU_H      =    mpu405.h

# ###---------------------------------------------------------###
#    user level functions for GENLIB				#
# ###---------------------------------------------------------###

MGN_L      =  -lMgn328
MGN_LIB    = libMgn328.a
MGN_H      =    mgn328.h

# ###---------------------------------------------------------###
#    functions related to expression representation :		#
#								#
#      - aut : utilites						#
#      - abl : lisp-like trees					#
#      - bdd : binary decision diagram				#
#      - gef : factorized expression representation		# 
# ###---------------------------------------------------------###

AUT_L      =  -lAut103
AUT_LIB    = libAut103.a
AUT_H      =    aut103.h

ABL_L      =  -lAbl103
ABL_LIB    = libAbl103.a
ABL_H      =    abl103.h

BDD_L      =  -lBdd103
BDD_LIB    = libBdd103.a
BDD_H      =    bdd103.h

# attention il s'agit de la lib luc
# ne pas utiliser avec aut abl bdd : utiliser log102...

LOG_L      =  -llog201
LOG_LIB    = liblog201.a
LOG_H      =    log201.h

# ###---------------------------------------------------------###
#    functions related to another behavioural representation :	#
#								#
#      - abe : basic data structures				#
#      - abt : high level functions				#
#      - abv : parser & driver for Data-Flow VHDL and user	#
#              level functions					#
# ###---------------------------------------------------------###

ABE_L      =  -lAbe200
ABE_LIB    = libAbe200.a
ABE_H      =    abe200.h
ABT_L      =  -lAbt200
ABT_LIB    = libAbt200.a
ABT_H      =    abt200.h
ABV_L      =  -lAbv200
ABV_LIB    = libAbv200.a
ABV_H      =    abv200.h

# ###---------------------------------------------------------###
#    functions related to behavioural representation :		#
#								#
#      - beh : basic data structures				#
#      - bhl : high level functions				#
#      - bvl : parser & driver for Data-Flow VHDL and user	#
#              level functions					#
# ###---------------------------------------------------------###

BEH_L      =  -lBeh109
BEH_LIB    = libBeh109.a
BEH_H      =    beh109.h
BHL_L      =  -lBhl109
BHL_LIB    = libBhl109.a
BHL_H      =    bhl109.h
BVL_L      =  -lBvl111
BVL_LIB    = libBvl111.a
BVL_H      =    bvl111.h

# ###---------------------------------------------------------###
#    functions related to finite state machine representation :	#
#								#
#       - fsm : basic data structures				#
#       - fbh : intermediate data structures			#
#								#
#       - fvh : VHDL parser					#
#       - fks : KISS parser					#
# ###---------------------------------------------------------###

FSM_L      =  -lFsm103
FSM_LIB    = libFsm103.a
FSM_H      =    fsm103.h

FVH_L      =  -lFvh103
FVH_LIB    = libFvh103.a
FKS_L      =  -lFks103
FKS_LIB    = libFks103.a

FTL_L      =  -lFtl103
FTL_LIB    = libFtl103.a
FTL_H      =    ftl103.h

# ###---------------------------------------------------------###
#    functions related to simulation patterns representation :	#
#								#
#      - pat : basic data structures				#
#      - phl : high level functions				#
#      - ppt : parser & driver for PAT format and user level	#
#              functions					#
#      - pgn : user level functions for GENPAT			#
# ###---------------------------------------------------------###

PAT_L      =  -lPat106
PAT_LIB    = libPat106.a
PAT_H      =    pat106.h
PHL_L      =  -lPhl106
PHL_LIB    = libPhl106.a
PHL_H      =    phl106.h
PPT_L      =  -lPpt106
PPT_LIB    = libPpt106.a
PPT_H      =    ppt106.h
PGN_L      =  -lPgn102
PGN_LIB    = libPgn102.a
PGN_H      =    Pgn102.h

# ###---------------------------------------------------------###
#    functions related to physical (micron) layout :		#
#								#
#      - rds : basic data structures				#
#      - rut : user level functions				#
#								#
#      - rdf : parser & driver for CIF format			#
#      - rgs : parser & driver for GDSII format			#
#      - rfm : physical layout from symbolic layout		#
#      - rpr : parser for technology file			#
#      - rwi : windowing functions				#
#								#
#      - rtl : user level functions				#
# ###---------------------------------------------------------###

RDS_L      =  -lRds209
RDS_LIB    = libRds209.a
RDS_H      =    rds209.h

RUT_L      =  -lRut208
RUT_LIB    = libRut208.a
RUT_H      =    rut208.h

RCF_L      =  -lRcf111
RCF_LIB    = libRcf111.a
RFM_L      =  -lRfm210
RFM_LIB    = libRfm210.a
RFM_H      =    rfm210.h
RGS_L      =  -lRgs112
RGS_LIB    = libRgs112.a
RPR_L      =  -lRpr212
RPR_LIB    = libRpr212.a
RPR_H      =    rpr212.h
RWI_L      =  -lRwi109
RWI_LIB    = libRwi109.a
RWI_H      =    rwi109.h

RTL_L      =  -lRtl110
RTL_LIB    = libRtl110.a
RTL_H      =    rtl110.h

# ###---------------------------------------------------------###
#    functions related to icon representation :			#
#								#
#      - icn : basic data structures				#
#								#
#      - ica : parser & driver for Alliance icon format		#
#      - icc : parser & driver for Compass  icon format		#
#								#
#      - icu : user level functions				#
# ###---------------------------------------------------------###

ICN_L      =  -lIcn201
ICN_LIB    = libIcn201.a
ICN_H      =    icn201.h

IAC_H      =    iac201.h

ICA_L      =  -lIca201
ICA_LIB    = libIca201.a
ICC_L      =  -lIcc201
ICC_LIB    = libIcc201.a

ICU_L      =  -lIcu201
ICU_LIB    = libIcu201.a
ICU_H      =    icu201.h

# ###---------------------------------------------------------###
#    functions related to portable blocks generators :		#
#								#
#      - gbs : barrel shifter					#
#      - gga : fast adder					#
#      - ggr : rom						#
#      - grf : register file					#
#      - gam : array pipelined multiplier			#
#      - gfp : data path simple operators			#
# ###---------------------------------------------------------###

GBS_L      =  -lGbs201
GBS_LIB    = libGbs201.a
GBS_H      =    gbs201.h

GGA_L      =  -lGga301
GGA_LIB    = libGga301.a
GGA_H      =    gga301.h

GGR_L      =  -lGgr001
GGR_LIB    = libGgr001.a
GGR_H      =    ggr001.h

GRF_L      =  -lGrf604
GRF_LIB    = libGrf604.a
GRF_H      =    grf604.h

GAM_L      =  -lGam000
GAM_LIB    = libGam000.a
GAM_H      =    gam000.h

GFP_L      =  -lGfp114
GFP_LIB    = libGfp114.a
GFP_H      =    gfp114.h

# ###---------------------------------------------------------###
#    place & route functions					#
# ###---------------------------------------------------------###

APR_L      =  -lApr102
APR_LIB    = libApr102.a
APR_H      =    apr102.h

# ###---------------------------------------------------------###
#    design rule checker functions				#
# ###---------------------------------------------------------###

VRD_L      =  -lVrd303
VRD_LIB    = libVrd303.a
VRD_H      =    vrd303.h

# ###---------------------------------------------------------###
#    user level functions for FPGEN				#
# ###---------------------------------------------------------###

FGN_L      =  -lFgn114
FGN_LIB    = libFgn114.a
FGN_H        = Fgn107.h

# ###---------------------------------------------------------###
#    schematic assistant					#
#								#
#      - saB : basic data structures				#
#      - sax : high level functions				#
# ###---------------------------------------------------------###

SAB_L   =  -lSaB402
SAB_LIB = libSaB402.a
SAB_H   =    sab402.h

SAX_L   =  -lSax102
SAX_LIB = libSax102.a
SAX_H   =    sax102.h

# ###---------------------------------------------------------###
#    Timing Analysis						#
#								#
#      - ttv : basic timing data structures			#
#								#
#      - inf : information file parser				#
#      - elp : electrical parameters' file parser		#
#      - tas : static delay analysis functions			#
# ###---------------------------------------------------------###

TTV_L      =  -lTtv110
TTV_LIB    = libTtv110.a
TTV_H      =    ttv110.h

INF_L      =  -lInf112
INF_LIB    = libInf112.a
INF_H      =    inf112.h

ELP_L      =  -lElp105
ELP_LIB    = libElp105.a
ELP_H      =    elp105.h

TAS_L      =  -lTas542
TAS_LIB    = libTas542.a
TAS_H      =    tas542.h

FCL_L      =  -lFcl103
FCL_LIB    = libFcl103.a
FCL_H      =    fcl103.h
