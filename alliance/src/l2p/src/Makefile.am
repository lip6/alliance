## Process this file with automake to produce Makefile.in

bin_PROGRAMS = l2p

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

l2p_LDADD = @ALLIANCE_LIBS@ \
 -lMlu -lMpu -lMap -lMmg -lMcp -lMcl -lMel \
 -lMhl -lMvl -lMlo -lMph -lMut -lMal -lRcn \
 -lMsl -lMgl \
 -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds

l2p_SOURCES = \
drive_ps.c  rps_inc.h   tmp_man.c \
dict_color.ps  l2p.c	   tmp_dict.c
