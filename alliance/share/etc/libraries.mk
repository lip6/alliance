# ###---------------------------------------------------------###
# file          : libraries.mk                                  #
# description   : Alliance Shared Libraries and Include Files   #
# ###---------------------------------------------------------###

# $Id: libraries.mk,v 1.25 2000/02/11 12:53:38 czo Exp $

# The variables $ALLIANCE_TOP and $ALLIANCE_OS are set by
# alc_env.[c]sh script

# ###---------------------------------------------------------###
#    Common settings                                            #
# ###---------------------------------------------------------###

ALLIANCE_VERSION = '"4.0.7"'
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

YAG_L      =  -lYag300
YAG_LIB    = libYag300.a
YAG_H      =    yag300.h

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

MPH_L      =  -lMph411
MPH_LIB    = libMph411.a
MPH_H      =    mph411.h

MAP_L      =  -lMap407
MAP_LIB    = libMap407.a
MCP_L      =  -lMcp408
MCP_LIB    = libMcp408.a
MMG_L      =  -lMmg402
MMG_LIB    = libMmg402.a

MPU_L      =  -lMpu407
MPU_LIB    = libMpu407.a
MPU_H      =    mpu407.h

# ###---------------------------------------------------------###
#    user level functions for GENLIB                            #
# ###---------------------------------------------------------###

MGN_L      =  -lMgn328
MGN_LIB    = libMgn328.a
MGN_H      =    mgn328.h

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

BDD_L      =  -lBdd104
BDD_LIB    = libBdd104.a
BDD_H      =    bdd104.h

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
#    functions related to simulation scheduler :                #
#                                                               #
#      - sch : simulation scheduler                             #
# ###---------------------------------------------------------###

SCH_L      =  -lSch110
SCH_LIB    = libSch110.a
SCH_H      =    sch110.h


# ###---------------------------------------------------------###
#    functions related to behavioural representation :          #
#                                                               #
#      - beh : basic data structures                            #
#      - cst : chain-like set treatment                         #
#      - bhl : high level functions                             #
#      - bvl : parser & driver for Data-Flow VHDL and user      #
#              level functions                                  #
# ###---------------------------------------------------------###

BEH_L      =  -lBeh110
BEH_LIB    = libBeh110.a
BEH_H      =    beh110.h
CST_L      =  -lCst100
CST_LIB    = libCst100.a
CST_H      =    cst100.h
BHL_L      =  -lBhl110
BHL_LIB    = libBhl110.a
BHL_H      =    bhl110.h
BVL_L      =  -lBvl113
BVL_LIB    = libBvl113.a
BVL_H      =    bvl113.h

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
FKS_L      =  -lFks104
FKS_LIB    = libFks104.a

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

PAT_L      =  -lPat108
PAT_LIB    = libPat108.a
PAT_H      =    pat108.h
PHL_L      =  -lPhl108
PHL_LIB    = libPhl108.a
PHL_H      =    phl108.h
PPT_L      =  -lPpt108
PPT_LIB    = libPpt108.a
PPT_H      =    ppt108.h
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

RDS_L      =  -lRds209
RDS_LIB    = libRds209.a
RDS_H      =    rds209.h

RUT_L      =  -lRut208
RUT_LIB    = libRut208.a
RUT_H      =    rut208.h

RCF_L      =  -lRcf111
RCF_LIB    = libRcf111.a
RFM_L      =  -lRfm211
RFM_LIB    = libRfm211.a
RFM_H      =    rfm211.h
RGS_L      =  -lRgs112
RGS_LIB    = libRgs112.a
RPR_L      =  -lRpr213
RPR_LIB    = libRpr213.a
RPR_H      =    rpr213.h
RWI_L      =  -lRwi109
RWI_LIB    = libRwi109.a
RWI_H      =    rwi109.h

RTL_L      =  -lRtl110
RTL_LIB    = libRtl110.a
RTL_H      =    rtl110.h

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

GFP_L      =  -lGfp115
GFP_LIB    = libGfp115.a
GFP_H      =    gfp115.h

# ###---------------------------------------------------------###
#    place & route functions                                    #
# ###---------------------------------------------------------###

APR_L      =  -lApr103
APR_LIB    = libApr103.a
APR_H      =    apr103.h

# ###---------------------------------------------------------###
#    design rule checker functions                              #
# ###---------------------------------------------------------###

VRD_L      =  -lVrd303
VRD_LIB    = libVrd303.a
VRD_H      =    vrd303.h

# ###---------------------------------------------------------###
#    user level functions for FPGEN				#
# ###---------------------------------------------------------###

FGN_L      =  -lFgn115
FGN_LIB    = libFgn115.a
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
