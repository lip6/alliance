#
# This file is part of the Alliance CAD System
# Copyright (C) Laboratoire LIP6 - Département ASIM
# Universite Pierre et Marie Curie
# 
# Home page          : http://www-asim.lip6.fr/alliance/
# E-mail             : mailto:alliance-users@asim.lip6.fr
# 
# This progam is  free software; you can redistribute it  and/or modify it
# under the  terms of the GNU  General Public License as  published by the
# Free Software Foundation;  either version 2 of the License,  or (at your
# option) any later version.
# 
# Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
# useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
# 
# You should have received a copy  of the GNU General Public License along
# with the GNU C Library; see the  file COPYING. If not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

# Makefile for GNU C compiler.
#   Copyright (C) 1987, 1988 Free Software Foundation, Inc.

#This file is part of GNU CC.

#GNU CC is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 1, or (at your option)
#any later version.

#GNU CC is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with GNU CC; see the file COPYING.  If not, write to
#the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

#Notice :
#for genview, this Makefile has been modified in order not to need bison or
#whatever, as the actual state is what genview expects.
#This is a minimized makefile for genview only, is this clear? :-(
#Modified by Fr\'ed\'eric P\'etrot, 11 april 1993

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
CC = gcc
# Variables that exist for you to override.
# See below for how to change them for certain systems.

# Compiler to use for compiling gnulib.
# OLDCC should not be the GNU C compiler.
OLDCC = cc
CFLAGS=-D$(ALLIANCE_OS)

# CFLAGS for use with OLDCC, for compiling gnulib.
# NOTE: -O does not work on some Unix systems!
# On them, you must take it out.
CCLIBFLAGS=-O

# This should be the version of ar to use with output from $(OLDCC).
OLDAR = ar

# CFLAGS for use with OLDCC, for compiling hard-params.
HARD_PARAMS_FLAGS=

# Directory where sources are, from where we are.
srcdir = .
# Directory in which to put the executable for the command `gcc'
libdir = $(prefix)/usr/local/lib

# Additional system libraries to link with.
CLIB=

# Change this to a null string if obstacks are installed in the
# system library.
OBSTACK=obstack.o

# End of variables for you to override.

# If you are making gcc for the first time, and if you are compiling it with
# a non-gcc compiler, and if your system doesn't have a working alloca() in any
# of the standard libraries (as is true for HP/UX or Genix),
# then un-comment the following line when compiling with the system's cc:
# ALLOCA = alloca.o
# But don't do that if compiling using GCC.

# Dependency on obstack, alloca, malloc or whatever library facilities
# are not installed in the system libraries.
LIBDEPS= $(OBSTACK) $(ALLOCA) $(MALLOC)

# How to link with both our special library facilities
# and the system's installed libraries.
LIBS = $(OBSTACK) $(ALLOCA) $(MALLOC) $(CLIB)

# Specify the directories to be searched for header files.
# Both . and srcdir are used, in that order,
# so that tm.h and config.h will be found in the compilation
# subdirectory rather than in the source directory.
INCLUDES = -I. -I$(srcdir) -I$(srcdir)/config
SUBDIR_INCLUDES = -I.. -I../$(srcdir) -I../$(srcdir)/config

# Always use -I$(srcdir)/config when compiling.
.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $<

# Language-specific object files for C.
C_OBJS = c-parse.tab.o c-decl.o c-typeck.o c-convert.o

# Language-specific object files for C++.
# (These are not yet released.)
CPLUS_OBJS = cplus-parse.o cplus-decl.o cplus-typeck.o \
   cplus-cvt.o cplus-search.o cplus-lex.o \
   cplus-class.o cplus-init.o cplus-method.o

# Language-independent object files.
OBJS = toplev.o version.o tree.o print-tree.o stor-layout.o fold-const.o \
 rtl.o rtlanal.o expr.o stmt.o expmed.o explow.o optabs.o varasm.o \
 symout.o dbxout.o sdbout.o emit-rtl.o insn-emit.o \
 integrate.o jump.o cse.o loop.o flow.o stupid.o combine.o \
 regclass.o local-alloc.o global-alloc.o reload.o reload1.o caller-save.o \
 insn-peep.o final.o recog.o insn-recog.o insn-extract.o insn-output.o

# Files to be copied away after each stage in building.
STAGE_GCC=genview_gcc
STAGESTUFF = *.o insn-flags.h insn-config.h insn-codes.h \
 insn-output.c insn-recog.c insn-emit.c insn-extract.c insn-peep.c \
 stamp-flags stamp-config stamp-codes \
 stamp-output stamp-recog stamp-emit stamp-extract stamp-peep \
 genemit genoutput genrecog genextract genflags gencodes genconfig genpeep \
 genview_cc1

# Members of gnulib.
LIBFUNCS = _eprintf _builtin_new _builtin_New _builtin_del _bb \
   _umulsi3 _mulsi3 _udivsi3 _divsi3 _umodsi3 _modsi3 \
   _lshrsi3 _lshlsi3 _ashrsi3 _ashlsi3 \
   _divdf3 _muldf3 _negdf2 _adddf3 _subdf3 _cmpdf2 \
   _fixunsdfsi _fixdfsi _floatsidf _truncdfsf2 _extendsfdf2 \
   _addsf3 _negsf2 _subsf3 _cmpsf2 _mulsf3 _divsf3

# Library members defined in gnulib2.c.
LIB2FUNCS = _adddi3 _subdi3 _muldi3 _divdi3 _moddi3 _udivdi3 _umoddi3 _negdi2 \
    _anddi3 _iordi3 _xordi3 _lshrdi3 _lshldi3 _ashldi3 _ashrdi3 _one_cmpldi2  \
    _bdiv _cmpdi2 _ucmpdi2 _fixunsdfdi _fixdfdi _floatdidf _varargs

# Header files that are made available to programs compiled with gcc.
USER_H = stddef.h assert.h va-i860.h va-mips.h va-pyr.h va-sparc.h \
    va-spur.h limits.h proto.h

# The files that "belong" in CONFIG_H are deliberately omitted
# because having them there would not be useful in actual practice.
# All they would do is cause complete recompilation every time
# one of the machine description files is edited.
# That may or may not be what one wants to do.
# If it is, rm *.o is an easy way to do it.
# CONFIG_H = config.h tm.h
CONFIG_H =
RTL_H = rtl.h rtl.def machmode.def
TREE_H = tree.h real.h tree.def machmode.def
CPLUS_TREE_H = $(TREE_H) cplus-tree.h c-tree.h

# Note that dependencies on obstack.h are not written
# because that file is not part of GCC.
# Dependencies on gvarargs.h are not written
# because all that file does, when not compiling with GCC,
# is include the system varargs.h.

#all: config.status gnulib genview_gcc genview_cc1
all: config.status genview_gcc genview_cc1
# Use this instead of `all' if you need to convert the libraries
# before you can use the compiler.
# Don't forget to do `make gnulib2' before installation.
#all-libconvert: config.status gnulib genview_gcc genview_cc1 float.h
all-libconvert: config.status genview_gcc genview_cc1

#lang-c: config.status gnulib genview_gcc genview_cc1
lang-c: config.status genview_gcc genview_cc1
config.status:
	@echo You must configure genview_gcc.  Look at the INSTALL file for details.
	@false

compilations: ${OBJS}

genview_gcc: gcc.o version.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o gccnew gcc.o version.o $(LIBS)
# Go via `gccnew' to avoid `file busy' if $(CC) is `gcc'.
	mv gccnew$(PROGRAM_SUFFIX) genview_gcc$(PROGRAM_SUFFIX)

genview_cc1: $(C_OBJS) $(OBJS) $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o genview_cc1 $(C_OBJS) $(OBJS) $(LIBS)

#Library of arithmetic subroutines
# Don't compile this with gcc!
# (That would cause most arithmetic functions to call themselves.)
gnulib: gnulib.c $(CONFIG_H) config.status
	-rm -f stamp-gnulib2
	rm -f tmpgnulib gnulib; \
	for name in $(LIBFUNCS); \
	do \
	  echo $${name}; \
	  rm -f $${name}.c; \
	  cp $(srcdir)/gnulib.c $${name}.c; \
	  $(OLDCC) $(CCLIBFLAGS) $(INCLUDES) -c -DL$${name} $${name}.c; \
	  $(OLDAR) qc tmpgnulib $${name}.o; \
	  rm -f $${name}.[co]; \
	done
	-if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ;then $(RANLIB) tmpgnulib; else true; fi
# Actually build it in tmpgnulib above, then rename now,
# so that gnulib itself remains nonexistent if compilation is aborted.
	mv tmpgnulib gnulib
# On HPUX, if you are working with the GNU assembler and linker,
# the previous two command lines must be replaced with the following line.
# No change is needed here if you are using the HPUX assembler and linker.
#	../hp-bin/hpxt tmpgnulib gnulib

gnulib2: stamp-gnulib2;
stamp-gnulib2: gnulib2.c gnulib genview_cc1 genview_gcc cpp $(CONFIG_H)
	for name in $(LIB2FUNCS); \
	do \
	  echo $${name}; \
	  ./gcc -B./ -fstrength-reduce -O $(INCLUDES) $(GNULIB2_CFLAGS) -c -DL$${name} $(srcdir)/gnulib2.c -o $${name}.o; \
	  $(AR) rc gnulib $${name}.o; \
	  rm -f $${name}.o; \
	done
	-if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; then $(RANLIB) gnulib; else true; fi
# On HPUX, if you are working with the GNU assembler and linker,
# the previous line must be commented out.
# No change is needed here if you are using the HPUX assembler and linker.
	touch stamp-gnulib2

float.h:
# Originally, we used `make' rather than $(MAKE), to avoid propagating
# a CC=gcc command option.  However, since hard-params is now made
# with $(OLDCC) explicitly, this is no longer important.
# However, $(MAKE) fails on some systems where it isn't defined.
# `make' has the disadvantage of sometimes running the system's make,
# instead of GNU make.  And the system's make might not support VPATH.
# However, the compilation of hard-params should not need to use VPATH,
# due to the explicit use of `$(srcdir)'.
	make hard-params \
		HARD_PARAMS_FLAGS="$(HARD_PARAMS_FLAGS)" \
		CPPFLAGS="$(CPPFLAGS)" \
		LDFLAGS="$(LDFLAGS)"
	-./hard-params -f > float.h

# Compile hard-params with standard cc.  It avoids some headaches.
hard-params: hard-params.o
	$(OLDCC) $(HARD_PARAMS_FLAGS) $(LDFLAGS) hard-params.o -o $@
hard-params.o: $(srcdir)/hard-params.c
	-cp $(srcdir)/hard-params.c . > /dev/null 2>&1
	$(OLDCC) $(HARD_PARAMS_FLAGS) $(CPPFLAGS) -DNO_SC -c hard-params.c

# C language specific files.

c-parse.tab.o : $(srcdir)/c-parse.tab.c $(CONFIG_H) $(TREE_H) c-parse.h c-tree.h input.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -D$(ALLIANCE_OS) -c $(srcdir)/c-parse.tab.c

c-parse.tab.c : c-parse.y
	bison c-parse.y

c-decl.o : c-decl.c $(CONFIG_H) $(TREE_H) c-tree.h c-parse.h flags.h
c-typeck.o : c-typeck.c $(CONFIG_H) $(TREE_H) c-tree.h flags.h
c-convert.o : c-convert.c $(CONFIG_H) $(TREE_H)

# Language-independent files.

gcc.o: gcc.c $(CONFIG_H) gvarargs.h obstack.h
	$(CC) $(CFLAGS) $(INCLUDES) \
  -DSTANDARD_STARTFILE_PREFIX=\"$(libdir)/\" \
  -DSTANDARD_EXEC_PREFIX=\"$(libdir)/gcc-\" -c \
  `echo $(srcdir)/gcc.c | sed 's,^\./,,'`

version.o: version.c
obstack.o: obstack.c

tree.o : tree.c $(CONFIG_H) $(TREE_H) flags.h
print-tree.o : print-tree.c $(CONFIG_H) $(TREE_H)
stor-layout.o : stor-layout.c $(CONFIG_H) $(TREE_H) $(RTL_H)
fold-const.o : fold-const.c $(CONFIG_H) $(TREE_H)
toplev.o : toplev.c $(CONFIG_H) $(TREE_H) $(RTL_H) flags.h input.h

rtl.o : rtl.c $(CONFIG_H) $(RTL_H)

rtlanal.o : rtlanal.c $(CONFIG_H) $(RTL_H)

varasm.o : varasm.c $(CONFIG_H) $(TREE_H) $(RTL_H) flags.h expr.h \
   insn-codes.h hard-reg-set.h
stmt.o : stmt.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h  \
   insn-flags.h insn-config.h insn-codes.h expr.h regs.h hard-reg-set.h recog.h
expr.o : expr.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h  \
   insn-flags.h insn-codes.h expr.h insn-config.h recog.h typeclass.h
expmed.o : expmed.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h  \
   insn-flags.h insn-codes.h expr.h insn-config.h recog.h
explow.o : explow.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h expr.h insn-codes.h
optabs.o : optabs.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h  \
   insn-flags.h insn-codes.h expr.h insn-config.h recog.h
symout.o : symout.c $(CONFIG_H) $(TREE_H) $(RTL_H) symseg.h gdbfiles.h
dbxout.o : dbxout.c $(CONFIG_H) $(TREE_H) $(RTL_H) flags.h
sdbout.o : sdbout.c $(CONFIG_H) $(TREE_H) $(RTL_H)

emit-rtl.o : emit-rtl.c $(CONFIG_H) $(RTL_H) regs.h insn-config.h real.h

integrate.o : integrate.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h expr.h \
   insn-flags.h insn-codes.h

jump.o : jump.c $(CONFIG_H) $(RTL_H) flags.h regs.h
stupid.o : stupid.c $(CONFIG_H) $(RTL_H) regs.h hard-reg-set.h

cse.o : cse.c $(CONFIG_H) $(RTL_H) regs.h hard-reg-set.h flags.h real.h
loop.o : loop.c $(CONFIG_H) $(RTL_H) insn-config.h insn-codes.h \
   regs.h hard-reg-set.h recog.h flags.h expr.h
flow.o : flow.c $(CONFIG_H) $(RTL_H) basic-block.h regs.h hard-reg-set.h
combine.o : combine.c $(CONFIG_H) $(RTL_H) flags.h  \
   insn-config.h regs.h basic-block.h recog.h
regclass.o : regclass.c $(CONFIG_H) $(RTL_H) hard-reg-set.h flags.h \
   basic-block.h regs.h insn-config.h recog.h 
local-alloc.o : local-alloc.c $(CONFIG_H) $(RTL_H) flags.h basic-block.h regs.h \
   hard-reg-set.h insn-config.h recog.h
global-alloc.o : global-alloc.c $(CONFIG_H) $(RTL_H) flags.h  \
   basic-block.h regs.h hard-reg-set.h insn-config.h

reload.o : reload.c $(CONFIG_H) $(RTL_H) flags.h \
   reload.h recog.h hard-reg-set.h insn-config.h regs.h
reload1.o : reload1.c $(CONFIG_H) $(RTL_H) flags.h  \
   reload.h regs.h hard-reg-set.h insn-config.h basic-block.h recog.h
caller-save.o : caller-save.c $(CONFIG_H) $(RTL_H) flags.h \
   reload.h regs.h hard-reg-set.h insn-config.h basic-block.h recog.h
final.o : final.c $(CONFIG_H) $(RTL_H) flags.h regs.h recog.h conditions.h \
   gdbfiles.h insn-config.h real.h output.h
recog.o : recog.c $(CONFIG_H) $(RTL_H)  \
   regs.h recog.h hard-reg-set.h insn-config.h real.h

# Normally this target is not used; but it is used if you
# define ALLOCA=alloca.o.  In that case, you must get a suitable alloca.c
# from the GNU Emacs distribution.
# Note some machines won't allow $(CC) without -S on this source file.
alloca.o:	alloca.c
	$(CC) $(CFLAGS) -S `echo $(srcdir)/alloca.c | sed 's,^\./,,'`
	as alloca.s -o alloca.o

# Now the source files that are generated from the machine description.

.PRECIOUS: insn-config.h insn-flags.h insn-codes.h \
  insn-emit.c insn-recog.c insn-extract.c insn-output.c insn-peep.c

# The following pair of rules has this effect:
# genconfig is run only if the md has changed since genconfig was last run;
# but the file insn-config.h is touched only when its contents actually change.

# Each of the other insn-* files is handled by a similar pair of rules.

insn-config.h: stamp-config ;
stamp-config : md genconfig $(srcdir)/move-if-change
	./genconfig md > tmp-config.h
	$(srcdir)/move-if-change tmp-config.h insn-config.h
	touch stamp-config

insn-flags.h: stamp-flags ;
stamp-flags : md genflags $(srcdir)/move-if-change
	./genflags md > tmp-flags.h
	$(srcdir)/move-if-change tmp-flags.h insn-flags.h
	touch stamp-flags

insn-codes.h: stamp-codes ;
stamp-codes : md gencodes $(srcdir)/move-if-change
	./gencodes md > tmp-codes.h
	$(srcdir)/move-if-change tmp-codes.h insn-codes.h
	touch stamp-codes

insn-emit.o : insn-emit.c $(CONFIG_H) $(RTL_H) expr.h real.h insn-codes.h \
  insn-config.h insn-flags.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-emit.c

insn-emit.c: stamp-emit ;
stamp-emit : md genemit $(srcdir)/move-if-change
	./genemit md > tmp-emit.c
	$(srcdir)/move-if-change tmp-emit.c insn-emit.c
	touch stamp-emit

insn-recog.o : insn-recog.c $(CONFIG_H) $(RTL_H) insn-config.h real.h recog.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-recog.c

insn-recog.c: stamp-recog ;
stamp-recog : md genrecog $(srcdir)/move-if-change
	./genrecog md > tmp-recog.c
	$(srcdir)/move-if-change tmp-recog.c insn-recog.c
	touch stamp-recog

insn-extract.o : insn-extract.c $(CONFIG_H) $(RTL_H)
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-extract.c

insn-extract.c: stamp-extract ;
stamp-extract : md genextract $(srcdir)/move-if-change
	./genextract md > tmp-extract.c
	$(srcdir)/move-if-change tmp-extract.c insn-extract.c
	touch stamp-extract

insn-peep.o : insn-peep.c $(CONFIG_H) $(RTL_H) regs.h real.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-peep.c

insn-peep.c: stamp-peep ;
stamp-peep : md genpeep $(srcdir)/move-if-change
	./genpeep md > tmp-peep.c
	$(srcdir)/move-if-change tmp-peep.c insn-peep.c
	touch stamp-peep

insn-output.o : insn-output.c $(CONFIG_H) $(RTL_H) regs.h real.h conditions.h \
    hard-reg-set.h insn-config.h insn-flags.h output.h aux-output.c
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-output.c

insn-output.c: stamp-output ;
stamp-output : md genoutput $(srcdir)/move-if-change
	./genoutput md > tmp-output.c
	$(srcdir)/move-if-change tmp-output.c insn-output.c
	touch stamp-output

# Now the programs that generate those files.
# $(CONFIG_H) is omitted from the deps of the gen*.o
# because these programs don't really depend on anything 
# about the target machine.  They do depend on config.h itself,
# since that describes the host machine.

genconfig : genconfig.o rtl.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o genconfig genconfig.o rtl.o $(LIBS)

genconfig.o : genconfig.c $(RTL_H) config.h

genflags : genflags.o rtl.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o genflags genflags.o rtl.o $(LIBS)

genflags.o : genflags.c $(RTL_H) config.h

gencodes : gencodes.o rtl.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o gencodes gencodes.o rtl.o $(LIBS)

gencodes.o : gencodes.c $(RTL_H) config.h

genemit : genemit.o rtl.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o genemit genemit.o rtl.o $(LIBS)

genemit.o : genemit.c $(RTL_H) config.h

genrecog : genrecog.o rtl.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o genrecog genrecog.o rtl.o $(LIBS)

genrecog.o : genrecog.c $(RTL_H) config.h

genextract : genextract.o rtl.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o genextract genextract.o rtl.o $(LIBS)

genextract.o : genextract.c $(RTL_H) config.h

genpeep : genpeep.o rtl.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o genpeep genpeep.o rtl.o $(LIBS)

genpeep.o : genpeep.c $(RTL_H) config.h

genoutput : genoutput.o rtl.o $(LIBDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o genoutput genoutput.o rtl.o $(LIBS)

genoutput.o : genoutput.c $(RTL_H) config.h

# gnulib is not deleted because deleting it would be inconvenient
# for most uses of this target.
realclean clean:
	-rm -f $(STAGESTUFF) $(STAGE_GCC)
# Delete the temp files made in the course of building gnulib.
	-rm -f tmpgnulib
	for name in $(LIBFUNCS); do rm -f $${name}.c; done
	-rm -f stamp-*.[ch] tmp-*
	-rm -f *.s *.s[0-9] *.co *.greg *.lreg *.combine *.flow *.cse *.jump *.rtl *.tree *.loop *.dbr *.jump2
	-rm -f core float.h hard-params
