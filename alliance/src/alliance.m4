AC_DEFUN(AM_ALLIANCE,[
  AC_REQUIRE([AC_PROG_LIBTOOL])
  AC_ARG_WITH(alliance-top,
    [  --with-alliance-top=ALLIANCE_TOP 
                           Prefix where alliance is installed (optional)],
    alliance_top="$withval", alliance_top="")


AC_MSG_CHECKING(for alliance)
if test x$alliance_top != x ; then
  ALLIANCE_CFLAGS="-I$alliance_top/include"
  ALLIANCE_LIBS="-L$alliance_top/lib"
  ALLIANCE_TOP="$alliance_top"
  AC_MSG_RESULT([ALLIANCE_TOP forced to $ALLIANCE_TOP])
else
  ALLIANCE_CFLAGS="-I${ALLIANCE_TOP}/include"
  ALLIANCE_LIBS="-L${ALLIANCE_TOP}/lib"
  AC_MSG_RESULT([using  \$ALLIANCE_TOP ($ALLIANCE_TOP)])
fi

ac_save_CFLAGS="$CFLAGS"
ac_save_LIBS="$LIBS"
CFLAGS="$ALLIANCE_CFLAGS $CFLAGS"
LIBS="$ALLIANCE_LIBS -lMut $LIBS"

dnl AC_TRY_RUN([
dnl #include <mut.h>

dnl int main()
dnl {
dnl  chain_list* x = addchain(NULL,NULL);
dnl  return 0;
dnl }
dnl ],, no_alliance=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

CFLAGS="$ac_save_CFLAGS"
LIBS="$ac_save_LIBS"

dnl  if test "x$no_alliance" = x ; then
dnl    AC_MSG_RESULT(yes)
dnl    ifelse([$1], , :, [$1])
dnl  else
dnl   AC_MSG_RESULT(no)
dnl   AC_MSG_ERROR(Please install alliance
dnl   or verify your ALLIANCE_TOP variable
dnl   or use configure with the --with-alliance-top option)
dnl  ALLIANCE_CFLAGS=""
dnl  ALLIANCE_LIBS=""
dnl ifelse([$2], , :, [$2])
dnl  fi


CFLAGS="-I${ALLIANCE_TOP}/include $CFLAGS"
LDFLAGS="-L${ALLIANCE_TOP}/lib $LDFLAGS"


AC_ARG_ENABLE(devel,
  [  --with-devel             use the user's local Alliance first.],
  [ case $enableval in
      "yes") enable_devel="yes";;
      "no")  enable_devel="no";;
      *)     enable_devel="no";;
    esac
  ],
  [ enable_devel="no" ]
)

if test "x$enable_devel" = "xyes"; then
  echo "alliance: user's local Alliance components will be used."
  CFLAGS="-I\$(includedir) $CFLAGS"
  LDFLAGS="-L\$(libdir) $LDFLAGS"
else
  echo "alliance: only system-wide Alliance will be used."
fi


AC_ARG_ENABLE(alc-shared,
  [  --enable-alc-shared      enable the use of dynamic libraries.],
  [ case $enableval in
      "yes") enable_alc_shared="yes";;
      "no")  enable_alc_shared="no";;
      *)     enable_alc_shared="no";;
    esac
  ],
  [ enable_alc_shared="no" ]
)

if test "x$enable_alc_shared" = "xyes"; then
  echo "alliance: use of shared libraries is enabled."
else
  echo "alliance: use of shared libraries is disabled."
  LDFLAGS="-static $LDFLAGS"
fi


AC_SUBST(ALLIANCE_CFLAGS)
AC_SUBST(ALLIANCE_LIBS)
AC_SUBST(ALLIANCE_TOP)

INSTALL_DATA='${INSTALL} -m 664'
AC_SUBST(INSTALL_DATA)
INSTALL_PROGRAM='${INSTALL} -m 775'
AC_SUBST(INSTALL_PROGRAM)

AC_DEFINE_UNQUOTED(ALLIANCE_VERSION, "5.0")
AC_DEFINE_UNQUOTED(ALLIANCE_TOP, "$ALLIANCE_TOP")

AC_CHECK_LIB(gen, basename)
AC_CHECK_LIB(iberty, basename)

])
