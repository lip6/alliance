## Process this file with automake to produce Makefile.in

#CXXFLAGS        = -g -pg -O2 -std=c++11
CXXFLAGS         = -g -O2 -std=gnu++11
#CXXFLAGS        = -O2
AM_CXXFLAGS      = @ALLIANCE_CFLAGS@ \
                   -I$(top_srcdir)/abl/src \
                   -I$(top_srcdir)/aut/src \
                   -I$(top_srcdir)/beh/src \
                   -I$(top_srcdir)/genlib/src \
                   -I$(top_srcdir)/rds/src \
                   -I$(top_srcdir)/mbk/src
AM_CFLAGS        = @ALLIANCE_CFLAGS@ \
                   -I$(top_srcdir)/abl/src \
                   -I$(top_srcdir)/aut/src \
                   -I$(top_srcdir)/beh/src \
                   -I$(top_srcdir)/genlib/src \
                   -I$(top_srcdir)/rds/src \
                   -I$(top_srcdir)/mbk/src 

bin_PROGRAMS     = nero pdv
#noinst_PROGRAMS  = debug

nero_LDADD       = -L$(libdir) @ALLIANCE_LIBS@  \
                   ./libU.a     \
                   -L$(top_builddir)/aut/src/.libs \
                   -L$(top_builddir)/mbk/src/.libs \
                   -L$(top_builddir)/rds/src/.libs \
                   -lMpu -lRds -lMlu -lMlo -lMph -lMut -lRcn -lAut

noinst_LIBRARIES = libU.a


libU_a_SOURCES   = UConst.cpp  \
                   USys.cpp    \
                   UOpts.cpp   \
                   UInter.cpp  \
                   UDefs.h     \
                   MMBK.cpp    \
                   MPower.cpp  \
                   MMBK.h      \
                   MDRGrid.cpp \
                   MNodes.cpp  \
                   MPri.cpp    \
                   MNet.cpp    \
                   MDefs.h     \
                   AAstar.cpp  \
                   ASimple.cpp \
                   ADefs.h     \
                   RBox.cpp    \
                   RMBK.cpp    \
                   RDefs.h

nero_SOURCES     = nero.cpp

pdv_LDADD        = @ALLIANCE_LIBS@  \
                   -L$(top_builddir)/aut/src/.libs \
                   -L$(top_builddir)/mbk/src/.libs \
                   -lMpu -lMlu -lMlo -lMph -lMut -lRcn -lAut

pdv_SOURCES      = pdv.c

#debug_SOURCES    = debug.cpp
