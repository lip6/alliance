## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d

CFLAGS = @CFLAGS@ @ALLIANCE_CFLAGS@

bin_PROGRAMS = ring

ring_LDADD = @ALLIANCE_LIBS@ \
          -lMpu -lMlu \
	  -lMcl -lMcp \
	  -lMal -lMap \
	  -lMsl \
	  -lMel -lMgl \
	  -lMhl \
	  -lMvl \
	  -lMgn \
	  -lMmg \
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

CLEANFILES = ringram.c ringram.h rinscan.c
