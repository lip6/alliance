dnl AM_PATH_POPT([ACTION-IF-FOUND])
dnl Test for GTK, and define GTK_CFLAGS and GTK_LIBS
AC_DEFUN(AM_PATH_POPT,[
  AC_ARG_WITH(popt-prefix,
    [  --with-popt-prefix=PFX    Prefix where popt is installed (optional)],
    popt_prefix="$withval", popt_prefix="")

  if test x$popt_prefix != x ; then
    POPT_CFLAGS="-I$popt_prefix/include"
    POPT_LIBS="-L$popt_prefix/lib -lpopt"
  else
    POPT_CFLAGS=""
    POPT_LIBS="-lpopt"
  fi
  
  AC_MSG_CHECKING(for popt library)

  ac_save_CFLAGS="$CFLAGS"
  ac_save_LIBS="$LIBS"
  CFLAGS="$CFLAGS $POPT_CFLAGS"
  LIBS="$POPT_LIBS $LIBS"
  
  AC_TRY_RUN([
#include <popt.h>

int main(int argc, const char** argv)
{
  struct poptOption options[] = {
    POPT_AUTOHELP
    { NULL, 0, 0, NULL, 0 }
  };

  poptContext context = poptGetContext("popt-test", argc, argv, options, 0);
  poptGetNextOpt(context);

  return 0;
}
],, no_popt=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
  CFLAGS="$ac_save_CFLAGS"
  LIBS="$ac_save_LIBS"

  if test "x$no_popt" = x ; then
    AC_MSG_RESULT(yes)
    ifelse([$1], , :, [$1])
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([This version of $PACKAGE requires libpopt:
    ftp://ftp.rpm.org/pub/rpm/dist/rpm-4.1.x/])
   POPT_CFLAGS=""
   POPT_LIBS=""
dnl ifelse([$2], , :, [$2])
  fi

  AC_SUBST(POPT_CFLAGS)
  AC_SUBST(POPT_LIBS)
])
