lib_LTLIBRARIES       = libMcp.la libMcl.la
AM_CFLAGS           = -DTECHNO=\"symbolic_cmos\" -I$(top_srcdir)/mbk/src
libMcp_la_SOURCES    = drive_vti_p.c drive_vti_p.h parse_vti_p.c mcp.h 
libMcl_la_SOURCES    = drive_vti_l.c drive_vti_l.h parse_vti_l.c mcl.h
libMcp_la_LDFLAGS = -version-info @MCP_DLL_VERSION@
libMcl_la_LDFLAGS = -version-info @MCL_DLL_VERSION@
