## Process this file with automake to produce Makefile.in

bin_PROGRAMS = loon

AM_CFLAGS =  -I$(top_srcdir)/abe/src \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/abv/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/mbk/src \
@ALLIANCE_CFLAGS@

loon_LDADD = @ALLIANCE_LIBS@ \
-L$(top_builddir)/abe/src \
-L$(top_builddir)/abl/src \
-L$(top_builddir)/abt/src \
-L$(top_builddir)/abv/src \
-L$(top_builddir)/aut/src \
-L$(top_builddir)/bdd/src \
-L$(top_builddir)/mbk/src \
          -lAbv -lAbe -lAbt -lMlu -lMlo -lBdd -lAbl -lAut -lMut
	   
loon_SOURCES = \
lon_lax_param.c           lon_lib_utils.c           lon_optim_capa.h \
lon_lax_param.h           lon_lib_utils.h           lon_optim_stats.c \
lon_lib_format.c          lon_main.c                lon_optim_stats.h \
lon_lib_format.h          lon_normalize_DAG.c       lon_signal_critical.c \
lon_lib_matching.c        lon_normalize_DAG.h       lon_signal_critical.h \
lon_lib_matching.h        lon_normalize_message.c   lon_signal_name.c \
lon_lib_negativ.c         lon_normalize_message.h   lon_signal_name.h \
lon_lib_negativ.h         lon_normalize_power.c     lon_signal_netlist.c \
lon_lib_permute.c         lon_normalize_power.h     lon_signal_netlist.h \
lon_lib_permute.h         lon_normalize_register.c  lon_signal_utils.c \
lon_lib_reader.c          lon_normalize_register.h  lon_signal_utils.h \
lon_lib_reader.h          lon_normalize_simplify.c  lon_xsch_driver.c \
lon_lib_specifications.c  lon_normalize_simplify.h  lon_xsch_driver.h \
lon_lib_specifications.h  lon_optim_capa.c

