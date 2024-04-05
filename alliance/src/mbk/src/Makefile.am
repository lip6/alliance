AM_CFLAGS         = -DTECHNO=\"symbolic_cmos\"
YFLAGS            = -d

lib_LTLIBRARIES   = libMut.la libMph.la libMpu.la libRcn.la libMlo.la libMlu.la
include_HEADERS   = mlu.h mlo.h mpu.h mph.h mut.h rcn.h msl.h

libMph_la_SOURCES = mbk_ph.c mbk_ph.h
libMpu_la_SOURCES = mbk_ph_util.c mbk_ph_util.h \
                    alc_driv_p.c alc_pars_p.c map.h \
                    drive_vti_p.c drive_vti_p.h parse_vti_p.c mcp.h \
                    mbk2mg.c mg2mbk_y.y mg2mbk_l.l mmg.h
libMlo_la_SOURCES = mbk_lo.c mbk_lo.h
libMlu_la_SOURCES = mbk_lo_util.c mbk_lo_util.h \
                    alc_driv_l.c alc_pars_l.c alc_pars_l6.c mal.h \
                    drive_vti_l.c  drive_vti_l.h  parse_vti_l.c \
                    spi_drive.c spi_drive.h spi_global.c spi_global.h \
                    spi_hash.c spi_hash.h spi_int.c spi_int.h spi_msg.c \
                    spi_msg.h spi_parse.c spi_parse.h msl.h \
                    time.c driver.c parser_y.y parser_l.l \
                    drive_hilo.c \
                    mvl_drive.c mvl_parse.c mvl_util.c mvl_sspec.c \
                    mvl_scomp_y.y mvl_scomp_l.l \
                    mvl_drive.h mvl_parse.h mvl_scomp_y.h mvl_slex.h \
                    mvl_sspec.h mvl_stdef.h mvl_stype.h mvl_syacc.h \
                    mvl_utdef.h mvl_util.h mvl_utype.h \
                    vel_chain.c  vel_sort.c  vel_vect.c vel_drive.c  vel_o.c vel_util.c  vel_velo.h \
                    gen_generic.c  gen_generic.h  gen_print.c \
                    drive_vlog.c

libMut_la_SOURCES = mbk_sys.c mbk_sys.h mbk_util.c mbk_util.h
libRcn_la_SOURCES = rcn_lo.c  rcn_lo.h  rcn_lo_util.c

libMph_la_LDFLAGS = -version-info @MBK_DLL_VERSION@
libMpu_la_LDFLAGS = -version-info @MBK_DLL_VERSION@
libMlo_la_LDFLAGS = -version-info @MBK_DLL_VERSION@
libMlu_la_LDFLAGS = -version-info @MBK_DLL_VERSION@
libMut_la_LDFLAGS = -version-info @MBK_DLL_VERSION@
libRcn_la_LDFLAGS = -version-info @MBK_DLL_VERSION@

libMph_la_LIBADD  = libMut.la
libMpu_la_LIBADD  = libMut.la libMph.la
libMlo_la_LIBADD  = libMut.la libRcn.la
libMlu_la_LIBADD  = libMut.la libMlo.la
libMut_la_LIBADD  =
libRcn_la_LIBADD  = libMut.la

CLEANFILES        = mg2mbk_y.c mg2mbk_y.h mg2mbk_l.c \
                    parser_y.c parser_y.h parser_l.c \
                    mvl_scomp_y.c mvl_scomp_y.h mvl_scomp_l.c

mg2mbk_y.c mg2mbk_y.h : $(srcdir)/mg2mbk_y.y
	$(YACC) $(YFLAGS) $(srcdir)/mg2mbk_y.y && \
	$(SED) -e "s/yy/mgn/g" -e "s/YY/MGN/g" y.tab.c > mg2mbk_y.c && \
	$(SED) -e "s/yy/mgn/g" -e "s/YY/MGN/g" y.tab.h > mg2mbk_y.h
mg2mbk_l.c : $(srcdir)/mg2mbk_l.l mg2mbk_y.h
	$(LEX) -t $(srcdir)/mg2mbk_l.l | $(SED) -e "s/yy/mgn/g" -e "s/YY/MGN/g" > mg2mbk_l.c

parser_y.c parser_y.h : $(srcdir)/parser_y.y mg2mbk_l.c
	$(YACC) $(YFLAGS) $(srcdir)/parser_y.y && \
	$(SED) -e "s/yy/edif/g" -e "s/YY/EDIF/g" y.tab.c > parser_y.c && \
	$(SED) -e "s/yy/edif/g" -e "s/YY/EDIF/g" y.tab.h > parser_y.h
parser_l.c : $(srcdir)/parser_l.l parser_y.h
	$(LEX) -t $(srcdir)/parser_l.l | $(SED) -e "s/yy/edif/g" -e "s/YY/EDIF/g" > parser_l.c

mvl_scomp_y.c mvl_scomp_y.h : $(srcdir)/mvl_scomp_y.y parser_l.c
	$(YACC) $(YFLAGS) $(srcdir)/mvl_scomp_y.y && \
	$(SED) -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" y.tab.c > mvl_scomp_y.c && \
	$(SED) -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" y.tab.h > mvl_scomp_y.h
mvl_scomp_l.c : $(srcdir)/mvl_scomp_l.l mvl_scomp_y.h
	$(LEX) -t $(srcdir)/mvl_scomp_l.l | $(SED) -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" > mvl_scomp_l.c
