AM_CFLAGS           = -DRTL_DEFAULT_TECHNO=\"etc/cmos.rds\" -I$(top_srcdir)/mbk/src

lib_LTLIBRARIES     = libRds.la
include_HEADERS     = rds.h rfm.h rtl.h rwi.h rpr.h rut.h

libRds_la_SOURCES   = rdsalloc.c rdsfree.c rdsadd.c rdsdel.c rdsview.c \
                      rdserror.c rdsdebug.c \
                      rdsalloc.h rdsfree.h rdsadd.h rdsdel.h rdsview.h \
                      rdserror.h rdsdebug.h \
                      rutacces.c rutpoly.c rutequi.c rutunify.c ruterror.c \
                      rutacces.h rutpoly.h rutequi.h rutunify.h ruterror.h \
                      rfmacces.c rfmview.c rfmerror.c \
                      rfmacces.h rfmview.h rfmerror.h \
                      rprparse.c rprview.c rprerror.c \
                      rprparse.h rprview.h rprerror.h \
                      rwialloc.c rwidel.c rwierror.c rwiget.c rwiadd.c \
                      rwibuild.c rwidestroy.c rwifree.c rwiview.c \
                      rwialloc.h rwidel.h rwierror.h rwiget.h rwiadd.h \
                      rwibuild.h rwidestroy.h rwifree.h rwiview.h \
                      rtlacces.c rtlenv.c rtlerror.c \
                      rtlacces.h rtlenv.h rtlerror.h \
                      cif_drive.c cif_error.c cif_parse.c rcf.h cif.h \
                      cif_drive.h cif_error.h cif_parse.h \
                      gds_drive.h gds_parse.c gds_swap.h gds.h \
                      gds_error.c gds_parse.h rgs.h gds_drive.c gds_error.h gds_swap.c
libRds_la_LDFLAGS   = -version-info @RDS_DLL_VERSION@ -L../../mbk/src
libRds_la_LIBADD    = -lMpu -lMut
