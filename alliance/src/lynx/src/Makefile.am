## Process this file with automake to produce Makefile.in

bin_PROGRAMS = lynx flatrds

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

lynx_LDADD = @ALLIANCE_LIBS@ \
 -lMlu -lMpu -lMap -lMmg -lMcp -lMcl -lMel \
 -lMhl -lMvl -lMlo -lMph -lMut -lMal -lRcn \
 -lMsl -lMgl \
 -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds

flatrds_LDADD = @ALLIANCE_LIBS@ \
 -lMlu -lMpu -lMap -lMmg -lMcp -lMcl -lMel \
 -lMhl -lMvl -lMlo -lMph -lMut -lMal -lRcn \
 -lMsl -lMgl \
 -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds

flatrds_SOURCES = flatrds.c

lynx_SOURCES = \
addlynxrcn.c addlynxrcn.h box.c box.h buildrcn.c \
buildrcn.h bulk.c bulk.h chklynxrcn.c chklynxrcn.h \
cutelbow.c cutelbow.h diffusion.c diffusion.h error.c \
error.h extract.c extract.h hole.c \
hole.h inter.c inter.h interval.c interval.h \
lynx.c lynx.h lynxrcn.h mbkrds.c mbkrds.h netlist.c \
netlist.h parse.c parse.h pattern.c pattern.h via.h \
Windows95.c Windows95.h znc.c znc.h
