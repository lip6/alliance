dnl
dnl
dnl AC_FIND_MOTIF : find OSF/Motif or LessTif, and provide variables
dnl     to easily use them in a Makefile.
dnl
dnl Adapted from a macro by Andreas Zeller.
dnl
dnl The variables provided are :
dnl     LINK_MOTIF              (e.g. -L/usr/lesstif/lib -lXm)
dnl     INCLUDE_MOTIF           (e.g. -I/usr/lesstif/lib)
dnl     MOTIF_LIBRARIES         (e.g. /usr/lesstif/lib)
dnl     MOTIF_INCLUDES          (e.g. /usr/lesstif/include)
dnl
dnl The LINK_MOTIF and INCLUDE_MOTIF variables should be fit to put on
dnl your application's link line in your Makefile.
dnl
dnl Oleo CVS $Id: motif.m4,v 1.1 2002/03/29 17:41:03 ludo Exp $
dnl
AC_DEFUN(AC_FIND_MOTIF,
[

AC_REQUIRE([AC_PATH_XTRA])

MOTIF_INCLUDES=
MOTIF_LIBRARIES=

dnl AC_ARG_WITH(motif,
dnl [  --without-motif         do not use Motif widgets])
dnl Treat --without-motif like
dnl --without-motif-includes --without-motif-libraries.
dnl if test "$with_motif" = "no"
dnl then
dnl   MOTIF_INCLUDES=none
dnl   MOTIF_LIBRARIES=none
dnl fi

AC_ARG_WITH(motif-includes,
[  --with-motif-includes=DIR    Motif include files are in DIR],
MOTIF_INCLUDES="$withval")

AC_ARG_WITH(motif-libraries,
[  --with-motif-libraries=DIR   Motif libraries are in DIR],
MOTIF_LIBRARIES="$withval")

AC_MSG_CHECKING(for Motif)

#
#
# Search the include files.
#
if test "$MOTIF_INCLUDES" = ""; then
AC_CACHE_VAL(ac_cv_motif_includes,
[
ac_motif_save_LIBS="$LIBS"
ac_motif_save_CFLAGS="$CFLAGS"
ac_motif_save_CPPFLAGS="$CPPFLAGS"
ac_motif_save_LDFLAGS="$LDFLAGS"
#
LIBS="$X_PRE_LIBS -lXm -lXt -lX11 $X_EXTRA_LIBS $LIBS"
CFLAGS="$X_CFLAGS $CFLAGS"
CPPFLAGS="$X_CFLAGS $CPPFLAGS"
LDFLAGS="$X_LIBS $LDFLAGS"
#
ac_cv_motif_includes="none"
AC_TRY_COMPILE([#include <Xm/Xm.h>],[int a;],
[
# Xm/Xm.h is in the standard search path.
ac_cv_motif_includes=
],
[
# Xm/Xm.h is not in the standard search path.
# Locate it and put its directory in `MOTIF_INCLUDES'
#
# /usr/include/Motif* are used on HP-UX (Motif).
# /usr/include/X11* are used on HP-UX (X and Athena).
# /usr/dt is used on Solaris (Motif).
# /usr/openwin is used on Solaris (X and Athena).
# Other directories are just guesses.
for dir in "$x_includes" "${prefix}/include" /usr/include /usr/local/include \
           /usr/include/Motif2.0 /usr/include/Motif1.2 /usr/include/Motif1.1 \
           /usr/include/X11R6 /usr/include/X11R5 /usr/include/X11R4 \
           /usr/dt/include /usr/openwin/include \
           /usr/dt/*/include /opt/*/include /usr/include/Motif* \
           "${prefix}"/*/include /usr/*/include /usr/local/*/include \
           "${prefix}"/include/* /usr/include/* /usr/local/include/*; do
if test -f "$dir/Xm/Xm.h"; then
ac_cv_motif_includes="$dir"
break
fi
done
])
#
LIBS="$ac_motif_save_LIBS"
CFLAGS="$ac_motif_save_CFLAGS"
CPPFLAGS="$ac_motif_save_CPPFLAGS"
LDFLAGS="$ac_motif_save_LDFLAGS"
])
MOTIF_INCLUDES="$ac_cv_motif_includes"
fi
#
#
# Now for the libraries.
#
if test "$MOTIF_LIBRARIES" = ""; then
AC_CACHE_VAL(ac_cv_motif_libraries,
[
ac_motif_save_LIBS="$LIBS"
ac_motif_save_CFLAGS="$CFLAGS"
ac_motif_save_CPPFLAGS="$CPPFLAGS"
ac_motif_save_LDFLAGS="$LDFLAGS"
#
LIBS="$X_PRE_LIBS -lXm -lXt -lX11 $X_EXTRA_LIBS $LIBS"
CFLAGS="$X_CFLAGS $CFLAGS"
CPPFLAGS="$X_CFLAGS $CPPFLAGS"
LDFLAGS="$X_LIBS $LDFLAGS"
#
ac_cv_motif_libraries="none"
AC_TRY_LINK([#include <Xm/Xm.h>],[XtToolkitInitialize();],
[
# libXm.a is in the standard search path.
ac_cv_motif_libraries=
],
[
# libXm.a is not in the standard search path.
# Locate it and put its directory in `MOTIF_LIBRARIES'
#
# /usr/lib/Motif* are used on HP-UX (Motif).
# /usr/lib/X11* are used on HP-UX (X and Athena).
# /usr/dt is used on Solaris (Motif).
# /usr/lesstif is used on Linux (Lesstif).
# /usr/openwin is used on Solaris (X and Athena).
# Other directories are just guesses.
for dir in "$x_libraries" "${prefix}/lib" /usr/lib /usr/local/lib \
           /usr/lib/Motif2.0 /usr/lib/Motif1.2 /usr/lib/Motif1.1 \
           /usr/lib/X11R6 /usr/lib/X11R5 /usr/lib/X11R4 /usr/lib/X11 \
           /usr/dt/lib /usr/openwin/lib \
           /usr/dt/*/lib /opt/*/lib /usr/lib/Motif* \
           /usr/lesstif*/lib /usr/lib/Lesstif* \
           "${prefix}"/*/lib /usr/*/lib /usr/local/*/lib \
           "${prefix}"/lib/* /usr/lib/* /usr/local/lib/*; do
if test -d "$dir" && test "`ls $dir/libXm.* 2> /dev/null`" != ""; then
ac_cv_motif_libraries="$dir"
break
fi
done
])
#
LIBS="$ac_motif_save_LIBS"
CFLAGS="$ac_motif_save_CFLAGS"
CPPFLAGS="$ac_motif_save_CPPFLAGS"
LDFLAGS="$ac_motif_save_LDFLAGS"
])
#
MOTIF_LIBRARIES="$ac_cv_motif_libraries"
fi
#
# Provide an easier way to link
#
if test "$MOTIF_INCLUDES" = "none" -o "$MOTIF_LIBRARIES" = "none"; then
        with_motif="no"
else
        with_motif="yes"
fi

if test "$with_motif" != "no"; then
        if test "$MOTIF_LIBRARIES" = ""; then
                LINK_MOTIF="-lXm"
                MOTIF_LIBS="-lXm"
        else
                LINK_MOTIF="-L$MOTIF_LIBRARIES -lXm"
                MOTIF_LIBS="-L$MOTIF_LIBRARIES -lXm"
        fi
        if test "$MOTIF_INCLUDES" != ""; then
                INCLUDE_MOTIF="-I$MOTIF_INCLUDES"
                MOTIF_CFLAGS="-I$MOTIF_INCLUDES"
        fi
        AC_DEFINE(HAVE_MOTIF)
else
        with_motif="no"
fi
#
AC_SUBST(LINK_MOTIF)
AC_SUBST(INCLUDE_MOTIF)
#
#
#
motif_libraries_result="$MOTIF_LIBRARIES"
motif_includes_result="$MOTIF_INCLUDES"
test "$motif_libraries_result" = "" && motif_libraries_result="in default path"
test "$motif_includes_result" = "" && motif_includes_result="in default path"
test "$motif_libraries_result" = "none" && motif_libraries_result="(none)"
test "$motif_includes_result" = "none" && motif_includes_result="(none)"
AC_MSG_RESULT(
  [libraries $motif_libraries_result, headers $motif_includes_result])
])dnl

