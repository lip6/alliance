## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src \
-I$(top_srcdir)/genlib/src

bin_PROGRAMS = ring

ring_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/genlib/src \
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
          -lMpu -lMlu \
	  -lMcl -lMcp \
	  -lMal -lMap \
	  -lMsl \
	  -lMel -lMgl \
	  -lMhl \
	  -lMvg \
	  -lMgn \
	  -lMmg \
	  -lMvg \
	  -lMlo \
	  -lMph -lMut \
	  -lRcn 
	   
ring_SOURCES = bigvia.c bigvia.h \
		compress.c compress.h \
		deport.c deport.h \
		distance.c distance.h \
		ringram.y rinscan.l \
		lireplace.c \
		lireplace.h lirevues.c \
		lirevues.h \
		param.c \
		param.h placement.c \
		placement.h posercircuit.c \
		posercircuit.h ring2.c \
		routage.c routage.h \
		routalim.c routalim.h \
		sesame.c sesame.h \
		stat.c stat.h \
		struct2.c struct.c \
		struct.h \
		barre.c barre.h

EXTRA_DIST = ringram.h

CLEANFILES = ringram.c ringram.h rinscan.c
