## Process this file with automake to produce Makefile.in

bin_PROGRAMS = b2f

CFLAGS = @ALLIANCE_CFLAGS@ -DB2F_VERSION=\"@B2F_VERSION@\"

b2f_LDADD = @ALLIANCE_LIBS@ \
          -lAbt -lAbv -lAbe -lFtl -lFks -lFvh -lFsm \
	  -lBtr -lBdd -lAbl -lAut -lMut

b2f_SOURCES = \
b2f_beh2fsm.c   b2f_error.c     b2f_main.c \
b2f_beh2fsm.h   b2f_error.h     b2f_main.h
