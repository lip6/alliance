# 
# This file is part of the Alliance CAD System
# Copyright (C) Laboratoire LIP6 - Département ASIM
# Universite Pierre et Marie Curie
# 
# Home page          : http://www-asim.lip6.fr/alliance/
# E-mail             : mailto:alliance-users@asim.lip6.fr
# 
# This progam is  free software; you can redistribute it  and/or modify it
# under the  terms of the GNU  General Public License as  published by the
# Free Software Foundation;  either version 2 of the License,  or (at your
# option) any later version.
# 
# Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
# useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
# 
# You should have received a copy  of the GNU General Public License along
# with the GNU C Library; see the  file COPYING. If not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
# 


#               ,,,
#              (o o)
# ####=====oOO--(_)--OOO==========================================####
# ##                                                                ##
# ##   File            :  Makefike for PCBS                         ##
# ##   Author          :  Francois DONNET                           ##
# ##   Date            :  Apr 2001                                  ##
# ##                                                                ##
# ##   E-mail          :  alliance-users@asim.lip6.fr             ##
# ##                                                                ##
# ####============================================================####
#
#
#

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

DEBUG		= -g -Wall #-DNO_HARD
CFLAGS          = $(ALC_INC) -D$(ALLIANCE_OS) $(DEBUG) 
YACC		= yacc
#TARGET_BIN = ..
#PROFILE	= -pg
#PURIFY		= purify -logfile=pure.log

########################PARAMETRES DU PROGRAMME################################
EXE		    = emulbs
VERSION		 = 3.1
NBR_EMULBS31 = 4
###############################################################################

ALC_INC =  \
	  -DNBR_EMULBS=$(NBR_EMULBS31) \
	  -DPROGRAM_NAME='"$(EXE)"' \
	  -DPROGRAM_VERSION='"$(VERSION)"' \
     -DALLIANCE_VERSION=$(ALLIANCE_VERSION) \
	  -I$(ALLIANCE_INCLUDE) \
     -DMLU_H='<$(MLU_H)>' \
     -DMLO_H='<$(MLO_H)>' \
     -DBHL_H='<$(BHL_H)>' \
     -DBEH_H='<$(BEH_H)>' \
     -DPPT_H='<$(PPT_H)>' \
     -DPAT_H='<$(PAT_H)>' \
     -DPHL_H='<$(PHL_H)>' \
     -DLOG_H='<$(LOG_H)>' \
     -DMUT_H='<$(MUT_H)>' 

ALC_LIB = -lm \
	  -L$(ALLIANCE_LIB) \
     $(MLO_L) \
     $(BHL_L) \
     $(BEH_L) \
     $(PPT_L) \
     $(PHL_L) \
     $(PAT_L) \
     $(LOG_L) \
     $(MUT_L) 


###############################################################################
SCE		= emulbs.c util.c bstools.c infos.c verif.c traduction.c emul_yac.c emul_lex.c hardware_io.c
OBJ		= emulbs.o util.o bstools.o infos.o verif.o traduction.o emul_yac.o emul_lex.o hardware_io.o
HEADERS		= global.h util.h  bstools.h infos.h verif.h traduction.h emul_yac.h emul31.h type_infos.h hardware_io.h



##+++++++++++++++++++++++++++++++les regles++++++++++++++++++++++++++++++++++##

$(TARGET_BIN)/$(EXE) : $(OBJ)
	@echo -n "Generation de l'executable ($@)...."
	$(PURIFY) $(CC) $(PROFILE) -o $(TARGET_BIN)/$(EXE) $(OBJ) $(ALC_LIB) 
	@echo "That's it !!"

.c.o : 
	@echo "Compilation de $< (options : '$(DEBUG)')"
	$(CC) -c $(CFLAGS) $<

%.i : %.c 
	@echo "Preprocessing de $<"
	$(CC) -E $(CFLAGS) $< > $@



##+++++++++++++++++++++++++++++++LEX&YACC++++++++++++++++++++++++++++++++++++##
emul_yac.c:  emul.yac
	${YACC} -d  emul.yac
	@${SED} -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.c  > emul_yac.c
	@${SED} -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.h  > emul_yac.h
	@${RM} y.tab.c y.tab.h
## sed remplace les noms globaux commencant par yy pour que deux parseurs cohabitent dans le meme programme

emul_lex.c: emul.lex
	${LEX} ${LEXFLAGS} emul.lex
	@${SED} -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" lex.yy.c  > emul_lex.c
	@${RM} lex.yy.c
##+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++##



clean:
	-$(RM) -f *.o
	-$(RM) -f *~ core
	-$(RM) -f emul_yac.h emul_yac.c emul_lex.c
## le tiret devant la commande signifie que **error 1** est ignore

realclean distclean : clean
	-$(RM) -f $(TARGET_BIN)/$(EXE)


###############################################################################
###############################################################################
###############################################################################
#DO NOT DELETE




