## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d

AM_CFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = ring

ring_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbk/src/libMlu.la \
	  ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkvti/src/libMcp.la \
	  ${top_builddir}/../mbkal/src/libMal.la ${top_builddir}/../mbkap/src/libMap.la \
	  ${top_builddir}/../mbkspice/src/libMsl.la \
	  ${top_builddir}/../mbkedif/src/libMel.la ${top_builddir}/../mbkvrlog/src/libMgl.la \
	  ${top_builddir}/../mbkhilo/src/libMhl.la \
	  ${top_builddir}/../mbkvhdlg/src/libMvg.la \
	  ${top_builddir}/../genlib/src/libMgn.la \
	  ${top_builddir}/../mbkmg/src/libMmg.la \
	  ${top_builddir}/../mbkvhdlg/src/libMvg.la \
	  ${top_builddir}/../mbk/src/libMlo.la \
	  ${top_builddir}/../mbk/src/libMph.la ${top_builddir}/../mbk/src/libMut.la \
	  ${top_builddir}/../mbk/src/libRcn.la 
	   
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
