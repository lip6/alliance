## Process this file with automake to produce Makefile.in

bin_PROGRAMS = l2p

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src \
-I$(top_srcdir)/rds/src

l2p_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkap/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkmg/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdlg/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
-L$(top_srcdir)/rds/src \
-L$(top_srcdir)/rdscif/src \
-L$(top_srcdir)/rdsgds/src \
 -lMlu -lMpu -lMap -lMmg -lMcp -lMcl -lMel \
 -lMhl -lMvg -lMlo -lMph -lMut -lMal -lRcn \
 -lMsl -lMgl \
 -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds

l2p_SOURCES = \
drive_ps.c  rps_inc.h   tmp_man.c \
dict_color.ps  l2p.c	   tmp_dict.c
