## Process this file with automake to produce Makefile.in

bin_PROGRAMS = k2f

CFLAGS = @ALLIANCE_CFLAGS@ -DK2F_VERSION=\"@K2F_VERSION@\"

k2f_LDADD = @ALLIANCE_LIBS@ \
          -lFtl -lFks -lFvh -lFsm \
	  -lBdd -lAbl -lAut -lMut


k2f_SOURCES = \
k2f_error.c   k2f_error.h   k2f_main.c    k2f_main.h
