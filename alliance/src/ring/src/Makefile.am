## Process this file with automake to produce Makefile.in

YACC         = @YACC@ -d

AM_CFLAGS    = @ALLIANCE_CFLAGS@ \
               -I$(top_srcdir)/mbk/src \
               -I$(top_srcdir)/rds/src \
               -I$(top_srcdir)/genlib/src

bin_PROGRAMS = ring

ring_LDADD   = @ALLIANCE_LIBS@ \
               -L$(top_builddir)/genlib/src \
               -L$(top_builddir)/mbk/src \
               -L$(top_builddir)/rds/src \
               -lMgn -lMpu -lMlu -lMlo -lMph -lMut -lRcn -lRds
	   
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

rinscan.c: ringram.c

EXTRA_DIST = ringram.h

CLEANFILES = ringram.c ringram.h rinscan.c
