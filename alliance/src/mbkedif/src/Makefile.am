CFLAGS = @ALLIANCE_CFLAGS@
lib_LIBRARIES       = libMel.a
libMel_a_SOURCES    = time.c driver.c parser_y.y parser_l.l
CLEANFILES          = parser_y.c parser_y.h parser_l.c

parser_y.c parser_y.h : $(srcdir)/parser_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/parser_y.y && sed -e "s/yy/edif/g" -e "s/YY/EDIF/g" y.tab.c > parser_y.c && sed -e "s/yy/edif/g" -e "s/YY/EDIF/g" y.tab.h > parser_y.h
parser_l.c : $(srcdir)/parser_l.l parser_y.h
	$(LEX) -t $(srcdir)/parser_l.l | sed -e "s/yy/edif/g" -e "s/YY/EDIF/g" > parser_l.c
