## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_srcdir)/src/include 

CFLAGS = @CFLAGS@ @ALLIANCE_CFLAGS@

noinst_LIBRARIES = libocrPath.a

libocrPath_a_SOURCES = mbk_tree.c ocrUtil.c \
			ocrWRoutingUtil.c ocrWSegLabeling.c \
			ocrWPlaneLabeling.c \
			ocrWRouting.c ocrToPhFig.c
