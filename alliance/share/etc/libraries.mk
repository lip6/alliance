# ###---------------------------------------------------------###
# file          : libraries.mk                                  #
# description   : Alliance Shared Libraries and Include Files   #
# ###---------------------------------------------------------###

# $Id: libraries.mk,v 1.31 2000/11/14 14:39:52 czo Exp $

# The variables $ALLIANCE_TOP and $ALLIANCE_OS are set by
# alc_env.[c]sh script

# ###---------------------------------------------------------###
#    Common settings                                            #
# ###---------------------------------------------------------###

ALLIANCE_VERSION = '"4.0.9"'
ALLIANCE_BIN     = $(ALLIANCE_TOP)/bin
ALLIANCE_LIB     = $(ALLIANCE_TOP)/lib
ALLIANCE_INCLUDE = $(ALLIANCE_TOP)/include
ALLIANCE_CELLS   = $(ALLIANCE_TOP)/cells
ALLIANCE_ETC     = $(ALLIANCE_TOP)/etc

# distrib path is now set by make command line
# when building alliance distrib (./build)
#ALLIANCE_INSTALL_DIR     = $(ALLIANCE_TOP)
# develloppers path
ALLIANCE_INSTALL_DIR     = $(HOME)/labo/$(ALLIANCE_OS)

TARGET_BIN       = $(ALLIANCE_INSTALL_DIR)/bin
TARGET_LIB       = $(ALLIANCE_INSTALL_DIR)/lib
TARGET_INCLUDE   = $(ALLIANCE_INSTALL_DIR)/include

# ###---------------------------------------------------------###
#    general purpose utilities                                  #
# ###---------------------------------------------------------###

MUT_L      =  -lMut325
MUT_LIB    = libMut325.a
MUT_H      =    mut325.h

# ###---------------------------------------------------------###
#    functions related to cone net-list representation :        #
#                                                               #
#      - cns : cone net_list                                    #
#      - yag : cone extractor form transistor net-list          #
# ###---------------------------------------------------------###

CNS_L      =  -lCns211
CNS_LIB    = libCns211.a
CNS_H      =    cns211.h

YAG_L      =  -lYag350
YAG_LIB    = libYag350.a
YAG_H      =    yag350.h

# ###---------------------------------------------------------###
#    functions related to net-list representation :             #
#                                                               #
#      - rcn : data structures for resistor representation      #
#      - mlo : basic data structures                            #
#                                                               #
#      - mal : parser & driver for Al format                    #
#      - mcl : parser & driver for COMPASS net-lists            #
#      - mel : parser & driver for EDIF                         #
#      - mgl :          driver for VERILOG net-lists            #
#      - mhl :          driver for GHDL net-list                #
#      - msl : parser & driver for SPICE                        #
#      - mvl : parser & driver for VHDL net-lists               #
#                                                               #
#      - mlu : user level functions                             #
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
#    functions related to symbolic layout representation :      #
#                                                               #
#      - mph : basic data structures                            #
#                                                               #
#      - map : parser & driver for AP format                    #
#      - mcp : parser & driver for COMPASS Compose layout       #
#      - mmg : parser & driver for MODGEN                       #
#                                                               #
#      - mpu : user level functions                             #
# ###---------------------------------------------------------###

MPH_L      =  -lMph412
MPH_LIB    = libMph412.a
MPH_H      =    mph412.h

MAP_L      =  -lMap408
MAP_LIB    = libMap408.a
MCP_L      =  -lMcp409
MCP_LIB    = libMcp409.a
MMG_L      =  -lMmg403
MMG_LIB    = libMmg403.a

MPU_L      =  -lMpu408
MPU_LIB    = libMpu408.a
MPU_H      =    mpu408.h

# ###---------------------------------------------------------###
#    user level functions for GENLIB                            #
# ###---------------------------------------------------------###

MGN_L      =  -lMgn330
MGN_LIB    = libMgn330.a
MGN_H      =    mgn330.h

# ###---------------------------------------------------------###
#    functions related to expression representation :           #
#                                                               #
#      - aut : utilites                                         #
#      - abl : lisp-like trees                                  #
#      - bdd : binary decision diagram                          #
#      - gef : factorized expression representation             # 
# ###---------------------------------------------------------###

AUT_L      =  -lAut103
AUT_LIB    = libAut103.a
AUT_H      =    aut103.h

ABL_L      =  -lAbl103
ABL_LIB    = libAbl103.a
ABL_H      =    abl103.h

BDD_L      =  -lBdd105
BDD_LIB    = libBdd105.a
BDD_H      =    bdd105.h

# attention il s'agit de la lib luc
# ne pas utiliser avec aut abl bdd : utiliser log102...

LOG_L      =  -llog201
LOG_LIB    = liblog201.a
LOG_H      =    log201.h

# ###---------------------------------------------------------###
#    functions related to another behavioural representation :  #
#                                                               #
#      - abe : basic data structures                            #
#      - abt : high level functions                             #
#      - abv : parser & driver for Data-Flow VHDL and user      #
#              level functions                                  #
# ###---------------------------------------------------------###

ABE_L      =  -lAbe201
ABE_LIB    = libAbe201.a
ABE_H      =    abe201.h
ABT_L      =  -lAbt201
ABT_LIB    = libAbt201.a
ABT_H      =    abt201.h
ABV_L      =  -lAbv201
ABV_LIB    = libAbv201.a
ABV_H      =    abv201.h

# ###---------------------------------------------------------###
#    functions related to simulation scheduler :                #
#                                                               #
#      - sch : simulation scheduler                             #
# ###---------------------------------------------------------###

SCH_L      =  -lSch110
SCH_LIB    = libSch110.a
SCH_H      =    sch110.h

# ###---------------------------------------------------------###
#    functions related to schematic :                           #
#                                                               #
#      - scl : Schematic Library                                #
# ###---------------------------------------------------------###

SCL_L      =  -lScl105
SCL_LIB    = libScl105.a
SCL_H      =    scl105.h

# ###---------------------------------------------------------###
#    functions related to behavioural representation :          #
#                                                               #
#      - beh : basic data structures                            #
#      - cst : chain-like set treatment                         #
#      - bhl : high level functions                             #
#      - bvl : parser & driver for Data-Flow VHDL and user      #
#              level functions                                  #
# ###---------------------------------------------------------###

BEH_L      =  -lBeh111
BEH_LIB    = libBeh111.a
BEH_H      =    beh111.h
CST_L      =  -lCst100
CST_LIB    = libCst100.a
CST_H      =    cst100.h
BHL_L      =  -lBhl111
BHL_LIB    = libBhl111.a
BHL_H      =    bhl111.h
BVL_L      =  -lBvl114
BVL_LIB    = libBvl114.a
BVL_H      =    bvl114.h

# ###---------------------------------------------------------###
#    functions related to finite state machine representation : #
#                                                               #
#       - fsm : basic data structures                           #
#       - fbh : intermediate data structures                    #
#                                                               #
#       - fvh : VHDL parser                                     #
#       - fks : KISS parser                                     #
# ###---------------------------------------------------------###

FSM_L      =  -lFsm104
FSM_LIB    = libFsm104.a
FSM_H      =    fsm104.h

FVH_L      =  -lFvh104
FVH_LIB    = libFvh104.a
FVH_H      =    fvh104.h
FKS_L      =  -lFks104
FKS_LIB    = libFks104.a
FKS_H      =    fks104.h

FTL_L      =  -lFtl104
FTL_LIB    = libFtl104.a
FTL_H      =    ftl104.h

# ###---------------------------------------------------------###
#    functions related to simulation patterns representation :  #
#                                                               #
#      - pat : basic data structures                            #
#      - phl : high level functions                             #
#      - ppt : parser & driver for PAT format and user level    #
#              functions                                        #
#      - pgn : user level functions for GENPAT                  #
# ###---------------------------------------------------------###

PAT_L      =  -lPat109
PAT_LIB    = libPat109.a
PAT_H      =    pat109.h
PHL_L      =  -lPhl109
PHL_LIB    = libPhl109.a
PHL_H      =    phl109.h
PPT_L      =  -lPpt109
PPT_LIB    = libPpt109.a
PPT_H      =    ppt109.h
PGN_L      =  -lPgn103
PGN_LIB    = libPgn103.a
PGN_H      =    Pgn102.h

# ###---------------------------------------------------------###
#    functions related to physical (micron) layout :            #
#                                                               #
#      - rds : basic data structures                            #
#      - rut : user level functions                             #
#                                                               #
#      - rdf : parser & driver for CIF format                   #
#      - rgs : parser & driver for GDSII format                 #
#      - rfm : physical layout from symbolic layout             #
#      - rpr : parser for technology file                       #
#      - rwi : windowing functions                              #
#                                                               #
#      - rtl : user level functions                             #
# ###---------------------------------------------------------###

RDS_L      =  -lRds210
RDS_LIB    = libRds210.a
RDS_H      =    rds210.h

RUT_L      =  -lRut209
RUT_LIB    = libRut209.a
RUT_H      =    rut209.h

RCF_L      =  -lRcf112
RCF_LIB    = libRcf112.a
RFM_L      =  -lRfm212
RFM_LIB    = libRfm212.a
RFM_H      =    rfm212.h
RGS_L      =  -lRgs113
RGS_LIB    = libRgs113.a
RPR_L      =  -lRpr214
RPR_LIB    = libRpr214.a
RPR_H      =    rpr214.h
RWI_L      =  -lRwi110
RWI_LIB    = libRwi110.a
RWI_H      =    rwi110.h

RTL_L      =  -lRtl111
RTL_LIB    = libRtl111.a
RTL_H      =    rtl111.h

# ###---------------------------------------------------------###
#    functions related to icon representation :                 #
#                                                               #
#      - icn : basic data structures                            #
#                                                               #
#      - ica : parser & driver for Alliance icon format         #
#      - icc : parser & driver for Compass  icon format         #
#                                                               #
#      - icu : user level functions                             #
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
#    functions related to portable blocks generators :          #
#                                                               #
#      - gbs : barrel shifter                                   #
#      - gga : fast adder                                       #
#      - ggr : rom                                              #
#      - grf : register file                                    #
#      - gam : array pipelined multiplier                       #
#      - gfp : data path simple operators                       #
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

GRF_L      =  -lGrf605
GRF_LIB    = libGrf605.a
GRF_H      =    grf605.h

GAM_L      =  -lGam000
GAM_LIB    = libGam000.a
GAM_H      =    gam000.h

GFP_L      =  -lGfp116
GFP_LIB    = libGfp116.a
GFP_H      =    gfp116.h

# ###---------------------------------------------------------###
#    place & route functions                                    #
# ###---------------------------------------------------------###

APR_L      =  -lApr103
APR_LIB    = libApr103.a
APR_H      =    apr103.h

# ###---------------------------------------------------------###
#    design rule checker functions                              #
# ###---------------------------------------------------------###

VRD_L      =  -lVrd304
VRD_LIB    = libVrd304.a
VRD_H      =    vrd304.h

# ###---------------------------------------------------------###
#    user level functions for FPGEN				#
# ###---------------------------------------------------------###

FGN_L      =  -lFgn116
FGN_LIB    = libFgn116.a
FGN_H        = Fgn115.h


# ###---------------------------------------------------------###
#    Timing Analysis                                            #
#                                                               #
#      - ttv : basic timing data structures                     #
#                                                               #
#      - inf : information file parser                          #
#      - elp : electrical parameters' file parser               #
#      - tas : static delay analysis functions                  #
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

FCL_L      =  -lFcl110
FCL_LIB    = libFcl110.a
FCL_H      =    fcl110.h

# end of libraries.mk
