lib_LTLIBRARIES       = libMcp.la libMcl.la
AM_CFLAGS           = @ALLIANCE_CFLAGS@ -DTECHNO=\"symbolic_cmos\"
libMcp_la_SOURCES    = drive_vti_p.c drive_vti_p.h parse_vti_p.c mcp.h 
libMcl_la_SOURCES    = drive_vti_l.c drive_vti_l.h parse_vti_l.c mcl.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


