AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LIBRARIES       = libGcp.a
include_HEADERS     = gcp.h
libGcp_a_SOURCES    = c_ccomp.y \
c_cdecl.c c_cdecl.h c_clex.c c_clex.h c_common.c c_common.h c_convert.c \
c_convert.h c_ctypeck.c c_ctypeck.h c_debug.c c_debug.h c_expand.c c_expand.h \
c_flags.c c_flags.h c_foldconst.c c_foldconst.h c_gperf.h c_parse.c c_parse.h \
c_stmt.c c_stmt.h c_tree.c c_treecompat.h c_tree.h gcp.h

CLEANFILES = c_ccomp.c c_ccomp.h vbl_bcomp_l.c
EXTRA_DIST = c_ccomp.h

c_ccomp.c c_ccomp.h : $(srcdir)/c_ccomp.y
	$(YACC) -d $(YFLAGS) $(srcdir)/c_ccomp.y && sed -e "s/yy/c_y_/g" -e "s/YY/C_Y_/g" y.tab.c > c_ccomp.c && sed -e "s/yy/c_y_/g" -e "s/YY/C_Y_/g" y.tab.h > c_ccomp.h
