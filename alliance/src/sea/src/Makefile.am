## Process this file with automake to produce Makefile.in

AM_YFLAGS    = -d -v -p DEF_grammar
AM_LFLAGS    = -s -f -8 -pp -PDEF_grammar -olex.yy.c

AM_CFLAGS    = @ALLIANCE_CFLAGS@ \
               -I$(top_srcdir)/aut/src \
               -I$(top_srcdir)/mbk/src

bin_PROGRAMS = a2def def2a sxlib2lef
bin_SCRIPTS  = sea seroute seplace a2lef 

a2def_LDADD  = @ALLIANCE_LIBS@ \
               ./libUtil.a \
               -L$(top_builddir)/aut/src/.libs \
               -L$(top_builddir)/mbk/src/.libs \
               -lMpu -lMlu -lMlo -lMph -lMut -lRcn -lAut

def2a_LDADD  = @ALLIANCE_LIBS@ \
               ./libUtil.a \
               -L$(top_builddir)/aut/src/.libs \
               -L$(top_builddir)/mbk/src/.libs \
               -lMpu -lMlu -lMlo -lMph -lMut -lRcn -lAut

sxlib2lef_LDADD = @ALLIANCE_LIBS@ \
                  ./libUtil.a \
                  -L$(top_builddir)/aut/src/.libs \
                  -L$(top_builddir)/mbk/src/.libs \
                  -lMpu -lMlu -lMlo -lMph -lMut -lRcn -lAut

noinst_LIBRARIES = libUtil.a

libUtil_a_SOURCES = util_Floorplan.c \
                    util_Inter.c \
                    util_LEFDEF.c \
                    util_MBK.c \
                    util_Power.c \
                    util_RTU.c \
                    util_Sys.c \
                    util_Defs.h \
                    GNU_Defs.h

a2def_SOURCES = DEF_drive.c \
                DEF_drive.h \
                MAC_drive.c \
                MAC_drive.h \
                a2DEF.c

def2a_SOURCES = DEF_actions.c \
                DEF_actions.h \
                DEF_grammar.h \
                DEF_grammar_yacc.y \
                DEF_grammar_lex.l \
                DEF2a.c

sxlib2lef_SOURCES = LEF_drive.h \
                    LEF_drive.c \
                    LEF_drive_sxlib.c \
                    LEF_drive_sxlib.h \
                    sxlib2LEF.c

sea: $(srcdir)/sea.sh
	cp $< $@ 
	chmod a+x $@

seplace: $(srcdir)/seplace.sh
	cp $< $@ 
	chmod a+x $@

seroute: $(srcdir)/seroute.sh
	cp $< $@ 
	chmod a+x $@

a2lef: $(srcdir)/a2lef.sh
	cp $< $@ 
	chmod a+x $@

DEF_grammar_lex.c: DEF_grammar_yacc.c

EXTRA_DIST = DEF_grammar_yacc.h debug.c debugon.h debugoff.h \
             sea.sh seplace.sh seroute.sh a2lef.sh
 
CLEANFILES =DEF_grammar_lex.c DEF_grammar_yacc.c \
            y.output DEF_grammar_yacc.h \
            seplace seroute sea a2lef
