# makefile for elp libraries
# date    : 31/07/95

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

ELP_VERSION              = 1.05
ELP_INSTALL_VERSION      = 105

OBJ          = elperror.o elp.y.o elpy.tab.o elplex.yy.o

ELP_HEADER   = elp$(ELP_INSTALL_VERSION).h
ELP_LIB      = libElp$(ELP_INSTALL_VERSION).a

ELP_H        = elp105.h

LOCAL_INC    = -I$(TARGET_INCLUDE) \
               -I$(ALLIANCE_INCLUDE) 

LOCAL_CFLAGS = $(CFLAGS) -c $(LOCAL_INC) \
               -D$(ALLIANCE_OS) \
               -DMLO_H="<$(MLO_H)>" \
               -DMUT_H="<$(MUT_H)>" \
               -DRCN_H="<$(RCN_H)>" \
               -DTECHNOLOGY='"$(ALLIANCE_ETC)/prol10.elp"' \
               -DELP_H='"$(ELP_HEADER)"' \
               -O3 

LOCAL_YFLAGS = $(YACCFLAGS) -d

$(TARGET_LIB)/$(ELP_LIB) : $(OBJ)
	$(AR) $(ARFLAGS) $(TARGET_LIB)/$(ELP_LIB) $(OBJ)
	$(RANLIB) $(TARGET_LIB)/$(ELP_LIB)
	$(CP) $(ELP_HEADER) $(TARGET_INCLUDE)/

elperror.o                : elperror.c
	$(CC) $(LOCAL_CFLAGS) elperror.c

elp.y.o                   : elp.c
	$(SED) -e 's/yy/elpyy/g' -e 's/YY/elpYY/g' elp.c > elp.y.c
	$(CC) $(LOCAL_CFLAGS) elp.y.c

elpy.tab.o                : elp.yac
	$(YACC) $(LOCAL_YFLAGS) elp.yac
	$(SED) -e 's/yy/elpyy/g' -e 's/YY/elpYY/g' y.tab.c > elpy.tab.c
	$(SED) -e 's/yy/elpyy/g' -e 's/YY/elpYY/g' y.tab.h > elpy.tab.h
	$(CC) $(LOCAL_CFLAGS) elpy.tab.c
	$(RM) elpy.tab.c y.tab.c y.tab.h

elplex.yy.o               : elp.lex
	$(LEX) $(LEXFLAGS) elp.lex
	echo "s/yy/elpyy/g" > tmp
	echo "s/YY/elpYY/g" >> tmp
	echo "s/y\.tab\.h/elpy.tab.h/g" >> tmp
	$(SED) -f tmp lex.yy.c > elplex.yy.c
	$(CC) $(LOCAL_CFLAGS) elplex.yy.c
	$(RM) lex.yy.c elplex.yy.c tmp

realclean : clean
	-$(RM) $(TARGET_LIB)/$(ELP_LIB)
	-$(RM) $(TARGET_INCLUDE)/$(ELP_HEADER)
clean :
	-$(RM) *.o elp.y.c elpy.tab.h
