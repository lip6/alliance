## Process this file with automake to produce Makefile.in

bin_PROGRAMS = scapin

AM_CFLAGS = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/mbk/src \
         -DSCAPIN_DEFAULT_PARAM_NAME=\"etc/sxlib.scapin\"

scapin_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/aut/src \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdlg/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
-lMlu -lMcl -lMel -lMhl -lMvg -lMlo -lMut -lMal -lRcn -lMsl -lMgl -lAut

scapin_SOURCES = \
scan_insert.c scan_insert.h scan_main.c scan_main.h \
scan_param.c scan_param.h scan_path.c scan_path.h
