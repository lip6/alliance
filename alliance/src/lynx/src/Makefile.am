## Process this file with automake to produce Makefile.in

bin_PROGRAMS = cougar flatrds

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src \
-I$(top_srcdir)/rds/src

cougar_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkap/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkmg/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdl/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
-L$(top_srcdir)/rds/src \
-L$(top_srcdir)/rdscif/src \
-L$(top_srcdir)/rdsgds/src \
 -lMlu -lMpu -lMap -lMmg -lMcp -lMcl -lMel \
 -lMhl -lMvl -lMlo -lMph -lMut -lMal -lRcn \
 -lMsl -lMgl \
 -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds

flatrds_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkap/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkmg/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdl/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
-L$(top_srcdir)/rds/src \
-L$(top_srcdir)/rdscif/src \
-L$(top_srcdir)/rdsgds/src \
 -lMlu -lMpu -lMap -lMmg -lMcp -lMcl -lMel \
 -lMhl -lMvl -lMlo -lMph -lMut -lMal -lRcn \
 -lMsl -lMgl \
 -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds

flatrds_SOURCES = flatrds.c

cougar_SOURCES = \
addlynxrcn.c addlynxrcn.h box.c box.h buildrcn.c \
buildrcn.h bulk.c bulk.h chklynxrcn.c chklynxrcn.h \
cutelbow.c cutelbow.h diffusion.c diffusion.h error.c \
error.h extract.c extract.h hole.c \
hole.h inter.c inter.h interval.c interval.h \
lynx.c lynx.h lynxrcn.h mbkrds.c mbkrds.h netlist.c \
netlist.h parse.c parse.h pattern.c pattern.h via.h \
Windows95.c Windows95.h znc.c znc.h
