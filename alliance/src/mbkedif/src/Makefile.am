AM_CFLAGS = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libMel.la
libMel_la_SOURCES    = time.c driver.c parser_y.y parser_l.l
libMel_la_LDFLAGS = -version-info @MEL_DLL_VERSION@
CLEANFILES          = parser_y.c parser_y.h parser_l.c

parser_y.c parser_y.h : $(srcdir)/parser_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/parser_y.y && sed -e "s/yy/edif/g" -e "s/YY/EDIF/g" y.tab.c > parser_y.c && sed -e "s/yy/edif/g" -e "s/YY/EDIF/g" y.tab.h > parser_y.h
parser_l.c : $(srcdir)/parser_l.l parser_y.h
	$(LEX) -t $(srcdir)/parser_l.l | sed -e "s/yy/edif/g" -e "s/YY/EDIF/g" > parser_l.c
