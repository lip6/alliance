lib_LIBRARIES       = libRds.a \
                      libRut.a \
                      libRfm.a \
                      libRpr.a \
                      libRwi.a \
                      libRtl.a
AM_CFLAGS           = @ALLIANCE_CFLAGS@ \
                      -DRTL_DEFAULT_TECHNO=\"etc/cmos.rds\" 

include_HEADERS     = rds.h rfm.h rtl.h rwi.h rpr.h rut.h
libRds_a_SOURCES    = rdsalloc.c rdsfree.c rdsadd.c rdsdel.c rdsview.c \
                      rdserror.c rdsdebug.c
libRut_a_SOURCES    = rutacces.c rutpoly.c rutequi.c rutunify.c ruterror.c
libRfm_a_SOURCES    = rfmacces.c rfmview.c rfmerror.c
libRpr_a_SOURCES    = rprparse.c rprview.c rprerror.c
libRwi_a_SOURCES    = rwialloc.c rwidel.c rwierror.c rwiget.c rwiadd.c \
                      rwibuild.c rwidestroy.c rwifree.c rwiview.c
libRtl_a_SOURCES    = rtlacces.c rtlenv.c rtlerror.c

