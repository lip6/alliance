## Process this file with automake to produce Makefile.in

AM_CFLAGS    = @ALLIANCE_CFLAGS@ 

bin_PROGRAMS = flatbeh

flatbeh_LDADD = @ALLIANCE_LIBS@ \
          -lAbv -lAbe -lAbt -lMlu -lMcl -lMal -lMsl -lMhl -lMel -lMvl -lMgl -lRcn -lMlo -lBdd -lAbl -lAut -lMut
	   
flatbeh_SOURCES = abstract.c abstract.h utils.c utils.h main.c

