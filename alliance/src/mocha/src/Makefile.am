## Process this file with automake to produce Makefile.in

bin_PROGRAMS = mocha

AM_CFLAGS = @ALLIANCE_CFLAGS@  -Wall

mocha_LDADD = @ALLIANCE_LIBS@ \
          -lFtl -lFks -lFvh -lFsm \
	  -lCtp -lCtl -lVex -lAbt -lAbv -lAbe -lBtr -lBdd -lAbl -lAut -lMut


mocha_SOURCES = \
mocha_check.h  mocha_debug.h  mocha_main.h    mocha_syf.h \
mocha_beh.c    mocha_ctl.c    mocha_fsm.c    mocha_shared.c \
mocha_beh.h    mocha_ctl.h    mocha_fsm.h    mocha_shared.h \
mocha_check.c  mocha_debug.c  mocha_main.c   mocha_syf.c \
mocha_bdd.c mocha_bdd.h
