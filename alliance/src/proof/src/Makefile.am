## Process this file with automake to produce Makefile.in

bin_PROGRAMS = proof

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

proof_LDADD = @ALLIANCE_LIBS@ \
          -lBvl -lBhl -lBeh -lLog -lMut

proof_SOURCES = \
proof_compile.c  proof_main.c  proof_util.c \
proof_compile.h proof_util.h
