lib_LTLIBRARIES       = libRds.la \
                      libRut.la \
                      libRfm.la \
                      libRpr.la \
                      libRwi.la \
                      libRtl.la
AM_CFLAGS           = -DRTL_DEFAULT_TECHNO=\"etc/cmos.rds\" -I$(top_srcdir)/mbk/src

include_HEADERS     = rds.h rfm.h rtl.h rwi.h rpr.h rut.h
libRds_la_SOURCES    = rdsalloc.c rdsfree.c rdsadd.c rdsdel.c rdsview.c \
                      rdserror.c rdsdebug.c \
		      rdsalloc.h rdsfree.h rdsadd.h rdsdel.h rdsview.h \
		      rdserror.h rdsdebug.h
libRut_la_SOURCES    = rutacces.c rutpoly.c rutequi.c rutunify.c ruterror.c \
		      rutacces.h rutpoly.h rutequi.h rutunify.h ruterror.h
libRfm_la_SOURCES    = rfmacces.c rfmview.c rfmerror.c \
		      rfmacces.h rfmview.h rfmerror.h 
libRpr_la_SOURCES    = rprparse.c rprview.c rprerror.c \
		      rprparse.h rprview.h rprerror.h
libRwi_la_SOURCES    = rwialloc.c rwidel.c rwierror.c rwiget.c rwiadd.c \
                      rwibuild.c rwidestroy.c rwifree.c rwiview.c \
		      rwialloc.h rwidel.h rwierror.h rwiget.h rwiadd.h \
		      rwibuild.h rwidestroy.h rwifree.h rwiview.h
libRtl_la_SOURCES    = rtlacces.c rtlenv.c rtlerror.c \
		      rtlacces.h rtlenv.h rtlerror.h
libRds_la_LDFLAGS = -version-info @RDS_DLL_VERSION@
libRut_la_LDFLAGS = -version-info @RUT_DLL_VERSION@
libRfm_la_LDFLAGS = -version-info @RFM_DLL_VERSION@
libRpr_la_LDFLAGS = -version-info @RPR_DLL_VERSION@
libRwi_la_LDFLAGS = -version-info @RWI_DLL_VERSION@
libRtl_la_LDFLAGS = -version-info @RTL_DLL_VERSION@
