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
                      rdserror.c rdsdebug.c \
		      rdsalloc.h rdsfree.h rdsadd.h rdsdel.h rdsview.h \
		      rdserror.h rdsdebug.h
libRut_a_SOURCES    = rutacces.c rutpoly.c rutequi.c rutunify.c ruterror.c \
		      rutacces.h rutpoly.h rutequi.h rutunify.h ruterror.h
libRfm_a_SOURCES    = rfmacces.c rfmview.c rfmerror.c \
		      rfmacces.h rfmview.h rfmerror.h 
libRpr_a_SOURCES    = rprparse.c rprview.c rprerror.c \
		      rprparse.h rprview.h rprerror.h
libRwi_a_SOURCES    = rwialloc.c rwidel.c rwierror.c rwiget.c rwiadd.c \
                      rwibuild.c rwidestroy.c rwifree.c rwiview.c \
		      rwialloc.h rwidel.h rwierror.h rwiget.h rwiadd.h \
		      rwibuild.h rwidestroy.h rwifree.h rwiview.h
libRtl_a_SOURCES    = rtlacces.c rtlenv.c rtlerror.c \
		      rtlacces.h rtlenv.h rtlerror.h

