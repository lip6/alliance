## Process this file with automake to produce Makefile.in

bin_PROGRAMS = fmi

fmi_LDADD = @LIBS@ \
          -lFtl -lFks -lFvh -lFsm \
	  -lBdd -lAbl -lAut -lMut


fmi_SOURCES = \
fmi_bdd.c     fmi_main.c    fmi_optim.c   fmi_parse.c   fsa_bdd.c \
fmi_bdd.h     fmi_main.h    fmi_optim.h   fmi_parse.h
