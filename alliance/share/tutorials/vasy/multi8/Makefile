# /*------------------------------------------------------------\
# |                                                             |
# | This file is part of the Alliance CAD System Copyright      |
# | (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
# |                                                             |
# | Home page      : http://www-asim.lip6.fr/alliance/          |
# | E-mail support : mailto:alliance-support@asim.lip6.fr       |
# |                                                             |
# | This progam is  free software; you can redistribute it      |
# | and/or modify it under the  terms of the GNU General Public |
# | License as  published by the Free Software Foundation;      |
# | either version 2 of the License, or (at your option) any    |
# | later version.                                              |
# |                                                             |
# | Alliance VLSI  CAD System  is distributed  in the hope that |
# | it  will be useful, but WITHOUT  ANY WARRANTY;              |
# | without even the  implied warranty of MERCHANTABILITY or    |
# | FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
# | Public License for more details.                            |
# |                                                             |
# | You should have received a copy  of the GNU General Public  |
# | License along with the GNU C Library; see the file COPYING. |
# | If not, write to the Free Software Foundation, Inc.,        |
# | 675 Mass Ave, Cambridge, MA 02139, USA.                     |
# |                                                             |
# \------------------------------------------------------------*/

# /*------------------------------------------------------------\
# |                                                             |
# | Tool   :                      VASY                          |
# |                                                             |
# | File   :                    Makefile                        |
# |                                                             |
# | Author :                 Jacomme Ludovic                    |
# |                                                             |
# | Date    :                   30_05_2000                      |
# |                                                             |
# \------------------------------------------------------------*/

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

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

TOUCH  = touch
VASY   = $(ALLIANCE_TOP)/bin/vasy
ASIMUT = $(ALLIANCE_TOP)/bin/asimut
BOP    = $(ALLIANCE_TOP)/bin/bop
SCMAP  = $(ALLIANCE_TOP)/bin/scmap
PROOF  = $(ALLIANCE_TOP)/bin/proof
SCR    = $(ALLIANCE_TOP)/bin/scr
LYNX   = $(ALLIANCE_TOP)/bin/lynx
LVX    = $(ALLIANCE_TOP)/bin/lvx
DRUC   = $(ALLIANCE_TOP)/bin/druc

# /*------------------------------------------------------------\
# |                                                             |
# |                            Environement                     |
# |                                                             |
# \------------------------------------------------------------*/

ENV_VASY = MBK_WORK_LIB=.; export MBK_WORK_LIB;\
           MBK_CATAL_NAME=NO_CATAL; export MBK_CATAL_NAME

ENV_ASIMUT = MBK_WORK_LIB=.; export MBK_WORK_LIB;\
             MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME;\
             MBK_IN_LO=vst; export MBK_IN_LO;\
	     MBK_OUT_LO=vst; export MBK_OUT_LO

ENV_SIMU_VBE = MBK_CATA_LIB=$(ALLIANCE_TOP)/cells/sxlib; export MBK_CATA_LIB;\
               MBK_WORK_LIB=.; export MBK_WORK_LIB;\
               MBK_CATAL_NAME=CATAL_VBE; export MBK_CATAL_NAME;\
               MBK_IN_LO=vst; export MBK_IN_LO;\
	       MBK_OUT_LO=vst; export MBK_OUT_LO

ENV_SIMU_VST = MBK_CATA_LIB=$(ALLIANCE_TOP)/cells/sxlib; export MBK_CATA_LIB;\
               MBK_WORK_LIB=.; export MBK_WORK_LIB;\
               MBK_CATAL_NAME=NO_CATAL; export MBK_CATAL_NAME;\
               MBK_IN_LO=vst; export MBK_IN_LO;\
	       MBK_OUT_LO=vst; export MBK_OUT_LO

ENV_SYNTH = MBK_CATA_LIB=$(ALLIANCE_TOP)/cells/sxlib; export MBK_CATA_LIB;\
            MBK_TARGET_LIB=$(ALLIANCE_TOP)/cells/sxlib; export MBK_TARGET_LIB;\
            MBK_WORK_LIB=.; export MBK_WORK_LIB;\
            MBK_CATAL_NAME=CATAL_VBE; export MBK_CATAL_NAME;\
            MBK_IN_LO=vst; export MBK_IN_LO;\
	    MBK_OUT_LO=vst; export MBK_OUT_LO

ENV_ROUTE = MBK_CATA_LIB=$(ALLIANCE_TOP)/cells/sxlib; export MBK_CATA_LIB;\
            MBK_WORK_LIB=.; export MBK_WORK_LIB;\
            MBK_CATAL_NAME=NO_CATAL; export MBK_CATAL_NAME;\
            MBK_IN_PH=ap; export MBK_IN_PH;\
	    MBK_OUT_PH=ap; export MBK_OUT_PH;\
            MBK_IN_LO=vst; export MBK_IN_LO;\
	    MBK_OUT_LO=al; export MBK_OUT_LO

ENV_PHYS = MBK_CATA_LIB=$(ALLIANCE_TOP)/cells/sxlib; export MBK_CATA_LIB;\
           MBK_WORK_LIB=.; export MBK_WORK_LIB;\
           MBK_CATAL_NAME=NO_CATAL; export MBK_CATAL_NAME;\
	   RDS_TECHNO_NAME=$(ALLIANCE_TOP)/etc/cmos_11.rds; \
	   export RDS_TECHNO_NAME;\
	   RDS_IN=cif; export RDS_IN;\
	   RDS_OUT=cif; export RDS_OUT;\
           MBK_IN_PH=ap; export MBK_IN_PH;\
	   MBK_OUT_PH=ap; export MBK_OUT_PH;\
           MBK_IN_LO=al; export MBK_IN_LO;\
	   MBK_OUT_LO=al; export MBK_OUT_LO

all : multi8_drc

# /*------------------------------------------------------------\
# |                                                             |
# |                             Vasy                            |
# |                                                             |
# \------------------------------------------------------------*/

multi8.vst : addaccu.vhdl controller.vhdl multi8.vhdl sra.vhdl srb.vhdl
	$(ENV_VASY); $(VASY) -a -o -I vhdl -HLpV multi8

# /*------------------------------------------------------------\
# |                                                             |
# |                      Behavioral Simulation                  |
# |                                                             |
# \------------------------------------------------------------*/

result_beh_pat : multi8.vst
	$(ENV_ASIMUT); $(ASIMUT) multi8 multi8 result_beh
	$(TOUCH) result_beh_pat

# /*------------------------------------------------------------\
# |                                                             |
# |                         Boolean Optimization                |
# |                                                             |
# \------------------------------------------------------------*/

srao_vbe : result_beh_pat
	$(ENV_SYNTH); $(BOP) -o sra srao
	$(ENV_SYNTH); $(PROOF) sra srao
	$(TOUCH) srao_vbe

srbo_vbe : result_beh_pat
	$(ENV_SYNTH); $(BOP) -o srb srbo
	$(ENV_SYNTH); $(PROOF) srb srbo
	$(TOUCH) srbo_vbe

addaccuo_vbe : result_beh_pat
	$(ENV_SYNTH); $(BOP) -o addaccu addaccuo addaccu
	$(ENV_SYNTH); $(PROOF) addaccu addaccuo
	$(TOUCH) addaccuo_vbe

controllero_vbe : result_beh_pat
	$(ENV_SYNTH); $(BOP) -o controller controllero
	$(ENV_SYNTH); $(PROOF) controller controllero
	$(TOUCH) controllero_vbe

multi8_modelo_vbe : result_beh_pat
	$(ENV_SYNTH); $(BOP) -o multi8_model multi8_modelo
	$(ENV_SYNTH); $(PROOF) multi8_model multi8_modelo
	$(TOUCH) multi8_modelo_vbe

# /*------------------------------------------------------------\
# |                                                             |
# |                           Mapping                           |
# |                                                             |
# \------------------------------------------------------------*/

sra.vst : srao_vbe
	$(ENV_SYNTH); $(SCMAP) srao sra

srb.vst : srbo_vbe
	$(ENV_SYNTH); $(SCMAP) srbo srb

addaccu.vst : addaccuo_vbe
# $(ENV_SYNTH); $(SCMAP) addaccuo addaccu
	echo Bug with SCMAP just make a copy 
	cp addaccu.vst.patched addaccu.vst
	$(TOUCH) addaccu.vst

controller.vst : controllero_vbe
	$(ENV_SYNTH); $(SCMAP) controllero controller

multi8_model.vst : multi8_modelo_vbe
	$(ENV_SYNTH); $(SCMAP) multi8_modelo multi8_model

# /*------------------------------------------------------------\
# |                                                             |
# |                      Structural Simulation                  |
# |                                                             |
# \------------------------------------------------------------*/

result_vst_pat : addaccu.vst controller.vst multi8.vst multi8_model.vst \
                 sra.vst srb.vst
	$(ENV_SIMU_VST); $(ASIMUT) -zd multi8 multi8 result_vst 
	$(TOUCH) result_vst_pat

# /*------------------------------------------------------------\
# |                                                             |
# |                        Place And Route                      |
# |                                                             |
# \------------------------------------------------------------*/

multi8.ap : result_vst_pat
	$(ENV_ROUTE); $(SCR) -p -r -i 1000 multi8 multi8

# /*------------------------------------------------------------\
# |                                                             |
# |                           Extract                           |
# |                                                             |
# \------------------------------------------------------------*/

multi8.al : multi8.ap
	$(ENV_PHYS); $(LYNX) -v multi8 multi8

# /*------------------------------------------------------------\
# |                                                             |
# |                           NetList Comparison                |
# |                                                             |
# \------------------------------------------------------------*/

multi8_lvx : multi8.al
	$(ENV_PHYS); $(LVX) vst al multi8 multi8 -f
	$(TOUCH) multi8_lvx

# /*------------------------------------------------------------\
# |                                                             |
# |                           Design Rules Check                |
# |                                                             |
# \------------------------------------------------------------*/

multi8_drc : multi8_lvx
	$(ENV_PHYS); $(DRUC) multi8
	$(TOUCH) multi8_drc



# /*------------------------------------------------------------\
# |                                                             |
# |                              Clean                          |
# |                                                             |
# \------------------------------------------------------------*/

realclean : clean

clean     :
	$(RM) -f *vst *vbe result_vst.pat  result_beh.pat
	$(RM) -f result_vst_pat  result_beh_pat multi8_drc multi8_lvx
	$(RM) -f *.al *.ap *.lax *.drc *.cif
	
