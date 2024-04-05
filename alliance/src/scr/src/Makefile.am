## Process this file with automake to produce Makefile.in

AM_YFLAGS        = -d

#CFLAGS          = -g -pg -O2
CFLAGS           = -g -O2
#CFLAGS          = -O2
AM_CFLAGS        = @ALLIANCE_CFLAGS@ \
                   -I$(top_srcdir)/mbk/src \
                   -I$(top_srcdir)/gscr/src

bin_PROGRAMS     = scr

scr_LDADD        = -L$(libdir) @ALLIANCE_LIBS@  \
                   -L$(top_builddir)/mbk/src \
                   -L$(top_builddir)/gscr/src \
                   -lApr -lMpu -lMlu -lMlo -lMph -lMut

scr_SOURCES      = GlobalRouter.c LoadDataBase.c main.c ScrSesame.c ViewDataBase.c \
                   scr_grammar.y scr_lexer.l

BUILD_SOURCES	= scr_grammar.h
nodist_scr_SOURCES = scr_grammar.h
scr_grammar.h: scr_grammar.c
scr_lexer.c: scr_grammar.h
