AC_DEFUN(AM_ALLIANCE,[
  AC_ARG_WITH(alliance-prefix,
    [  --with-alliance-prefix=PFX    Prefix where alliance is installed (optional)],
    alliance_prefix="$withval", alliance_prefix="")


AC_MSG_CHECKING(for alliance)
if test x$alliance_prefix != x ; then
  ALLIANCE_CFLAGS="-I$alliance_prefix/include"
  ALLIANCE_LIBS="-L$alliance_prefix/lib"
else
  ALLIANCE_CFLAGS="-I${ALLIANCE_TOP}/include"
  ALLIANCE_LIBS="-L${ALLIANCE_TOP}/lib"
fi

ac_save_CFLAGS="$CFLAGS"
ac_save_LIBS="$LIBS"
CFLAGS="$ALLIANCE_CFLAGS $CFLAGS"
LIBS="$ALLIANCE_LIBS -lMut $LIBS"

AC_TRY_RUN([
#include <mut.h>

int main()
{
  chain_list* x = addchain(NULL,NULL);
  return 0;
}
],, no_alliance=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

CFLAGS="$ac_save_CFLAGS"
LIBS="$ac_save_LIBS"

  if test "x$no_alliance" = x ; then
    AC_MSG_RESULT(yes)
    ifelse([$1], , :, [$1])
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR(Please install alliance
    or verify your ALLIANCE_TOP variable
    or use configure with the --with-alliance-prefix option)
   ALLIANCE_CFLAGS=""
   ALLIANCE_LIBS=""
dnl ifelse([$2], , :, [$2])
  fi
  
dnl the following lines should disapear in the future 
dnl end of to delete lines

AC_SUBST(ALLIANCE_CFLAGS)
AC_SUBST(ALLIANCE_LIBS)

INSTALL_DATA='${INSTALL} -m 664'
AC_SUBST(INSTALL_DATA)
INSTALL_PROGRAM='${INSTALL} -m 775'
AC_SUBST(INSTALL_PROGRAM)

AC_DEFINE_UNQUOTED(ALLIANCE_VERSION, "5.0")
])
