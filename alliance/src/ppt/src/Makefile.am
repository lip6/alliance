AM_CFLAGS           = @ALLIANCE_CFLAGS@

lib_LIBRARIES       = libPpt.a
include_HEADERS     = ppt.h
libPpt_a_SOURCES    = pat_decl_l.l pat_decl_y.y pat_desc_l.l pat_desc_y.y\
pat_decl.h  pat_defs.h \
pat_desc.h  pat_drvpat.c \
pat_lodpaseq.c pat_pattostr.c pat_prspat.c pat_savpaseq.c \
pat_type.h ppt.h

CLEANFILES  = pat_decl_y.c pat_decl_y.h pat_desc_y.h pat_desc_y.c \
              pat_decl_l.c

pat_decl_y.c pat_decl_y.h : $(srcdir)/pat_decl_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/pat_decl_y.y && sed -e "s/yy/pat_decl_y_/g" -e "s/YY/PAT_DECL_Y_/g" y.tab.c > pat_decl_y.c && sed -e "s/yy/pat_decl_y_/g" -e "s/YY/PAT_DECL_Y_/g" y.tab.h > pat_decl_y.h

pat_decl_l.c : $(srcdir)/pat_decl_l.l pat_decl_y.h
	$(LEX) -t $(srcdir)/pat_decl_l.l | sed -e "s/yy/pat_decl_y_/g" -e "s/YY/PAT_DECL_Y_/g" > pat_decl_l.c

pat_desc_y.c pat_desc_y.h : $(srcdir)/pat_desc_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/pat_desc_y.y && sed -e "s/yy/pat_desc_y_/g" -e "s/YY/PAT_DESC_Y_/g" y.tab.c > pat_desc_y.c && sed -e "s/yy/pat_desc_y_/g" -e "s/YY/PAT_DESC_Y_/g" y.tab.h > pat_desc_y.h

pat_desc_l.c : $(srcdir)/pat_desc_l.l pat_desc_y.h
	$(LEX) -t $(srcdir)/pat_desc_l.l | sed -e "s/yy/pat_desc_y_/g" -e "s/YY/PAT_DESC_Y_/g" > pat_desc_l.c
