## Process this file with automake to produce Makefile.in

bin_PROGRAMS = exp 

YACC = @YACC@ -d -v --debug
CFLAGS = @CFLAGS@ @ALLIANCE_CFLAGS@
exp_LDADD =

exp_SOURCES = exp.h expy.y expl.l ht.c ht.h main.c
nodist_exp_SOURCES = expy.h expl.h
BUILD_SOURCES = expy.h expl.h
expy.h:	expy.c
expl.c:	expy.h

EXTRA_DIST = expy.h

CLEANFILES = expy.c expl.c y.output expy.h
