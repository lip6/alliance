include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

#TARGET_LIB      = $(HOME)/dev/archi/$(ALLIANCE_OS)/lib
#TARGET_INCLUDE  = $(HOME)/dev/archi/$(ALLIANCE_OS)/lib

LOCAL_INC       = ../include

LOCAL_CFLAGS    = $(CFLAGS) -c -g
LOCAL_CPPFLAGS  = $(CPPFLAGS) -I$(LOCAL_INC) \
                  -I$(ALLIANCE_INCLUDE) \
                  -DMUT_H='<$(MUT_H)>' \
                  -DBEH_H='<$(BEH_H)>' \
                  -DBHL_H='<$(BHL_H)>' \
                  -DBVL_H='<bvl114.h>' \
                  -DLOG_H='<$(LOG_H)>'

LOCAL_YACCFLAGS = $(YACCFLAGS) -d
LOCAL_LEXFLAGS  = $(LEXFLAGS)

LOCAL_ARFLAGS   = $(ARFLAGS)

LOCAL_OBJECTS   = bvl_globals.o bvl_parse.o  bvl_drive.o  \
                  bvl_util.o    bvl_yac.o    bvl_lex.o

.c.o :
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_CPPFLAGS)  $<

$(TARGET_LIB)/libBvl114.a : $(LOCAL_OBJECTS)
	$(CP) $(LOCAL_INC)/bvl114.h $(TARGET_INCLUDE)
	$(AR) $(LOCAL_ARFLAGS) $(TARGET_LIB)/libBvl114.a $(LOCAL_OBJECTS)
	-$(RANLIB) $(TARGET_LIB)/libBvl114.a

bvl_lex.c : bvl_bcomp.lex bvl_yac.c
	$(LEX) $(LOCAL_LEXFLAGS) bvl_bcomp.lex
	$(SED) -e 's/YY/BVL_Y_/g' -e 's/yy/bvl_y_/g' lex.yy.c > bvl_lex.c
	$(RM) lex.yy.c

bvl_yac.c : bvl_bcomp.yac
	$(YACC) $(LOCAL_YACCFLAGS) bvl_bcomp.yac
	$(SED) -e 's/YY/BVL_Y_/g' -e 's/yy/bvl_y_/g' y.tab.c > bvl_yac.c
	$(RM) y.tab.c
	$(SED) -e 's/YY/BVL_Y_/g' -e 's/yy/bvl_y_/g' y.tab.h > bvl_yac.h
	$(RM) y.tab.h

distrib : $(TARGET_LIB)/libBvl114.a

clean :
	$(RM) -f bvl_yac.h bvl_yac.c bvl_lex.c
	$(RM) -f $(LOCAL_OBJECTS)

realclean : clean
	$(RM) -f $(TARGET_LIB)/libBvl114.a $(TARGET_INCLUDE)/bvl114.h
