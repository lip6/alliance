include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk
###############################################################################
SCE		= pcbs.c util.c bstools.c infos.c verif.c traduction.c sig.c  parseur.c emul_yac.c emul_lex.c pga_yac.c pga_lex.c
OBJ		= pcbs.o util.o bstools.o infos.o verif.o traduction.o sig.o  parseur.o emul_yac.o emul_lex.o pga_yac.o pga_lex.o
HEADERS		= pcbs.h util.h  bstools.h infos.h verif.h traduction.h sig.h  parseur.h emul_yac.h pga_yac.h emul31.h type_infos.h global.h
DEBUG		= -g -Wall
CFLAGS          = $(ALC_INC) -D$(ALLIANCE_OS) $(DEBUG)
YACC		= yacc
#PROFILE	= -pg
#PURIFY		= purify -logfile=pure.log
########################PARAMETRES DU PROGRAMME################################
EXE		      = pcbs
VERSION		   = 2.1
PGA_SIZE	      = 17
NBR_EMULBS31	= 4
###############################################################################

#bibliotheque ALLIANCE modifiee
BVL_H       = bvl999.h
BVL_L       = -lBvl999
BVL_INCLUDE = ../bvl
BVL_LIB     = ../bvl

ALC_INC = -DPGA_SIZE=$(PGA_SIZE) \
	  -DNBR_EMUL=$(NBR_EMULBS31) \
	  -DPROGRAM_NAME='"$(EXE)"' \
	  -DPROGRAM_VERSION='"$(VERSION)"' \
     -DALLIANCE_VERSION=$(ALLIANCE_VERSION) \
	  -I$(ALLIANCE_INCLUDE) \
          -DMLU_H='<$(MLU_H)>' \
          -I$(BVL_INCLUDE)     \
	  -DBVL_H='"$(BVL_H)"' \
          -DMLO_H='<$(MLO_H)>' \
          -DBHL_H='<$(BHL_H)>' \
          -DBEH_H='<$(BEH_H)>' \
          -DPPT_H='<$(PPT_H)>' \
          -DPAT_H='<$(PAT_H)>' \
          -DPHL_H='<$(PHL_H)>' \
          -DLOG_H='<$(LOG_H)>' \
          -DMUT_H='<$(MUT_H)>' 

ALC_LIB =  -L$(BVL_LIB) \
           $(BVL_L) \
	   -L$(ALLIANCE_LIB) \
           $(MLO_L) \
           $(BHL_L) \
           $(BEH_L) \
           $(PPT_L) \
           $(PHL_L) \
           $(PAT_L) \
           $(LOG_L) \
           $(MUT_L) 

##+++++++++++++++++++++++++++++++les regles++++++++++++++++++++++++++++++++++##
$(TARGET_BIN)/$(EXE) : $(OBJ)
	@echo -n "Generation de l'executable ($@)...."
	$(PURIFY) $(CC) $(PROFILE) -o $@ $(OBJ) $(ALC_LIB) 
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
	@sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.c  > emul_yac.c
	@sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.h  > emul_yac.h
	@rm y.tab.c y.tab.h
pga_yac.c:  pga.yac
	${YACC} -d  pga.yac
	@sed -e "s/yy/pga_/g" -e "s/YY/PGA_/g" y.tab.c  > pga_yac.c
	@sed -e "s/yy/pga_/g" -e "s/YY/PGA_/g" y.tab.h  > pga_yac.h
	@rm y.tab.c y.tab.h
## sed remplace les noms globaux commencant par yy pour que deux parseurs cohabitent dans le meme programme

emul_lex.c: lexeur.lex
	${LEX} ${LEXFLAGS} lexeur.lex
	@sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" lex.yy.c  > emul_lex.c
pga_lex.c: lexeur.lex
	@sed -e "s/yy/pga_/g" -e "s/YY/PGA_/g" lex.yy.c  > pga_lex.c
	@rm lex.yy.c
##+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++##



clean:
	-/bin/rm -f *.o
	-/bin/rm -f $(TARGET_BIN)/$(EXE)
	-/bin/rm -f *~ core
	-/bin/rm -f pga_yac.c emul_yac.c pga_lex.c emul_lex.c
## le tiret devant la commande signifie que **error 1** est ignore

depend :
	@echo "Computing depends into Makefile.new"
	sed '/^#DO NOT DELETE/q' Makefile > Makefile.new
	$(CC) -M $(SCE) $(ALC_INC) >> Makefile.new
	@echo "Moving Makefile to Makefile.bak"
	rm -f Makefile.bak 
	-mv Makefile Makefile.bak
	@echo "Moving Makefile.new to Makefile"
	-mv Makefile.new Makefile

headers :
	@echo "Generation automatique des Headers"
	for file in $(HEADERS) ; \
	   do sed '/\/\* PUBLIC FONCTIONS \*\//q' \$file:r.h > tmp.h ; \
	   grep EXTERN \$file:r.c | sed -e 's/EXTERN \(.*\)(.*)/extern \1();/' >> tmp.h ; \
	   cat tmp.h \;
	done

###############################################################################
###############################################################################
###############################################################################
#DO NOT DELETE




