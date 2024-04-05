AM_CFLAGS           = -I$(top_srcdir)/mbk/src
lib_LTLIBRARIES     = libPat.la
include_HEADERS     = pat.h ppt.h phl.h
libPat_la_SOURCES   = pat_addpacom.c pat_addpaevt.c pat_addpagrp.c pat_addpaini.c \
                      pat_addpains.c pat_addpaiol.c pat_addpapat.c pat_addpaseq.c \
                      pat_crtpaiol.c pat_error.c pat_frepacom.c pat_frepaevt.c \
                      pat_frepaini.c pat_frepaiol.c pat_frepapat.c pat_globals.c \
                      pat.h pat_message.c pat_warning.c \
                      pat_decl_l.l pat_decl_y.y pat_desc_y.y \
                      pat_decl.h  pat_defs.h \
                      pat_desc.h  pat_drvpat.c \
                      pat_lodpaseq.c pat_pattostr.c pat_prspat.c pat_savpaseq.c \
                      pat_type.h ppt.h \
                      pat_debug.c pat_debug.h pat_getusage.c phl.h
libPat_la_LDFLAGS   = -version-info @PAT_DLL_VERSION@ -L../../aut/src -L../../mbk/src
libPat_la_LIBADD    = -lAut -lMut

EXTRADIST   = pat_decl_y.c pat_decl_y.h pat_desc_y.h pat_desc_y.c \
              pat_decl_l.c 

CLEANFILES  = pat_decl_y.c pat_decl_y.h pat_desc_y.h pat_desc_y.c \
              pat_decl_l.c 

pat_decl_y.c pat_decl_y.h : $(srcdir)/pat_decl_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/pat_decl_y.y \
	    && $(SED) -e "s/yy/pat_decl_y_/g" -e "s/YY/PAT_DECL_Y_/g" y.tab.c \
	     | $(SED) -e "s/int[        ]*pat_decl_y_char;/extern int pat_decl_y_char;/" \
	     | $(SED) -e "s/int[        ]*pat_decl_y_nerrs;/extern int pat_decl_y_nerrs;/" \
	     | $(SED) -e "s/^PAT_DECL_Y_STYPE[        ]*pat_decl_y_lval;/extern PAT_DECL_Y_STYPE pat_decl_y_lval;/" \
	     > pat_decl_y.c \
	    && $(SED) -e "s/yy/pat_decl_y_/g" -e "s/YY/PAT_DECL_Y_/g" y.tab.h > pat_decl_y.h

pat_decl_l.c : $(srcdir)/pat_decl_l.l pat_decl_y.h
	$(LEX) -t $(srcdir)/pat_decl_l.l | $(SED) -e "s/yy/pat_decl_y_/g" -e "s/YY/PAT_DECL_Y_/g" > pat_decl_l.c

pat_desc_y.c pat_desc_y.h : $(srcdir)/pat_desc_y.y pat_decl_l.c
	$(YACC) -d $(YFLAGS) $(srcdir)/pat_desc_y.y \
	    && $(SED) -e "s/yyparse/pat_desc_y_parse/g" y.tab.c \
	     | $(SED) -e "s/yy/pat_decl_y_/g" -e "s/YY/PAT_DECL_Y_/g" > pat_desc_y.c \
	    && $(SED) -e "s/yyparse/pat_desc_y_parse/g" y.tab.h \
	     | $(SED) -e "s/yy/pat_decl_y_/g" -e "s/YY/PAT_DECL_Y_/g" > pat_desc_y.h

#$(SED) -e "s/yy/pat_desc_y_/g" -e "s/YY/PAT_DESC_Y_/g" > pat_desc_y.c && $(SED) -e "s/yy/pat_desc_y_/g" -e "s/YY/PAT_DESC_Y_/g" y.tab.h > pat_desc_y.h
# $(SED) -e "s/yyin/pat_decl_y_/g" -e "s/yylex/pat_decl_y_lex/g" y.tab.c | 

pat_desc_l.c : $(srcdir)/pat_desc_l.l pat_desc_y.h
	$(LEX) -t $(srcdir)/pat_desc_l.l | $(SED) -e "s/yy/pat_desc_y_/g" -e "s/YY/PAT_DESC_Y_/g" > pat_desc_l.c

