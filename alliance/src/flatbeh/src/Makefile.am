## Process this file with automake to produce Makefile.in

AM_CFLAGS    = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/abe/src \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/abv/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/mbk/src

bin_PROGRAMS = flatbeh

flatbeh_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/abv/src \
-L$(top_srcdir)/abe/src \
-L$(top_srcdir)/abt/src \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkvti/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkvhdl/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/bdd/src \
-L$(top_srcdir)/abl/src \
-L$(top_srcdir)/aut/src \
          -lAbv -lAbe -lAbt -lMlu -lMcl -lMal -lMsl -lMhl -lMel -lMvl -lMgl -lRcn -lMlo -lBdd -lAbl -lAut -lMut
	   
flatbeh_SOURCES = abstract.c abstract.h utils.c utils.h main.c

