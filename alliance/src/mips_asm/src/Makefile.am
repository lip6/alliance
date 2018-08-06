## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d

#INCLUDES = -I$(srcdir)

AM_CXXFLAGS = @ALLIANCE_CFLAGS@
AM_CFLAGS = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/beh/src \
-I$(top_srcdir)/log/src \
-I$(top_srcdir)/mbk/src

bin_PROGRAMS = mips_asm

mips_asm_LDADD = @ALLIANCE_LIBS@ \
-L$(top_builddir)/abl/src/.libs \
-L$(top_builddir)/aut/src/.libs \
-L$(top_builddir)/beh/src/.libs \
-L$(top_builddir)/log/src/.libs \
-L$(top_builddir)/mbk/src/.libs \
	  -lBeh -lMut -lLog
	   
mips_asm_SOURCES = mips_defs.h mips_y.y mips_l.l mips_globals.c mips_lex.h mips_type.h mips_yacc.h mips_util.c

mips_y.c mips_y.h : $(srcdir)/mips_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/mips_y.y && $(SED) -e "s/yy/mipsyy/g" -e "s/YY/mipsYY/g" y.tab.c > mips_y.c && $(SED) -e "s/yy/mipsyy/g" -e "s/YY/mipsYY/g" y.tab.h > mips_y.h

mips_l.c : $(srcdir)/mips_l.l mips_y.h
	$(LEX) -t $(srcdir)/mips_l.l | $(SED) -e "s/yy/mipsyy/g" -e "s/YY/mipsYY/g" > mips_l.c

CLEANFILES          = y.tab.c y.tab.h mips.c
