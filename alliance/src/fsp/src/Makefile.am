## Process this file with automake to produce Makefile.in

bin_PROGRAMS = fsp

fsp_LDADD = @LIBS@ \
          -lFtl -lFks -lFvh -lFsm \
	  -lBdd -lAbl -lAut -lMut


fsp_SOURCES = \
fsp_comp.c    fsp_debug.c   fsp_main.c    fsp_proof.c \
fsp_comp.h    fsp_debug.h   fsp_main.h    fsp_proof.h
