AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/abl/src
lib_LTLIBRARIES     = libBdd.la
include_HEADERS     = bdd.h
libBdd_la_SOURCES   = bdd.h            bddenv.h         bddimply.h       bddsimpdc.h \
                      bddalloc.c       bdderror.c       bddlog.c         bddsubst.c \
                      bddalloc.h       bdderror.h       bddlog.h         bddsubst.h \
                      bddapply.c       bddexist.c       bddmark.c        bddsupport.c \
                      bddapply.h       bddexist.h       bddmark.h        bddsupport.h \
                      bddassoc.c       bddexplosion.c   bddnode.c        bddsweep.c \
                      bddassoc.h       bddexplosion.h   bddnode.h        bddsweep.h \
                      bddblock.c       bddflag.c        bddoptimize.c    bddsystem.c \
                      bddblock.h       bddflag.h        bddoptimize.h    bddsystem.h \
                      bddcheck.c       bddfraction.c    bddreduce.c      bddtest.c \
                      bddcheck.h       bddfraction.h    bddreduce.h      bddtest.h \
                      bddcircuit.c     bddfree.c        bddref.c         bddtransfert.c \
                      bddcircuit.h     bddfree.h        bddref.h         bddtransfert.h \
                      bddcofactor.c    bddgarbage.c     bddrelprod.c     bdduser.c \
                      bddcofactor.h    bddgarbage.h     bddrelprod.h     bdduser.h \
                      bddcompose.c     bddheath.c       bddreorder.c     bddvar.c \
                      bddcompose.h     bddheath.h       bddreorder.h     bddvar.h \
                      bddconvert.c     bddhnode.c       bddresize.c      bddvaraux.c \
                      bddconvert.h     bddhnode.h       bddresize.h      bddvaraux.h \
                      bdddump.c        bddhoper.c       bddsatisfy.c     \
                      bdddump.h        bddhoper.h       bddsatisfy.h     \
                      bddenv.c         bddimply.c       bddsimpdc.c

libBdd_la_LDFLAGS   = -version-info @BDD_DLL_VERSION@
libBdd_la_LIBADD    = -lAbl -lAut
