## Process this file with automake to produce Makefile.in

bin_PROGRAMS = fmi

CFLAGS = @ALLIANCE_CFLAGS@ -DFMI_VERSION=\"@FMI_VERSION@\"

fmi_LDADD = @ALLIANCE_LIBS@ \
          -lFtl -lFks -lFvh -lFsm \
	  -lBdd -lAbl -lAut -lMut

fmi_SOURCES = \
fmi_bdd.c     fmi_main.c    fmi_optim.c   fmi_parse.c   \
fmi_bdd.h     fmi_main.h    fmi_optim.h   fmi_parse.h
