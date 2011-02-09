AC_DEFUN([ALC_CXX_PROBLEMATIC_OLD_VERSION],
[
AC_REQUIRE([AC_PROG_CXX])
AC_MSG_CHECKING(if this is a problematic old ${CXX} version)
AC_CACHE_VAL(alc_cv_cxx_problematic_version,
[
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_CPP([
#ifdef __GNUC__
#if __GNUC__ < 2
#error ALC works best with GCC version 2.95.2 or higher
#endif /* __GNUC__ < 2 */
#ifdef __GNUC_MINOR__
#if __GNUC__ == 2 && __GNUC_MINOR__ < 95
#error ALC works only with GCC version 2.95.2 or higher
#endif /* __GNUC_MINOR__ < 95.2 */
#endif /* defined(__GNUC_MINOR__) */
#endif /* defined(__GNUC__) */
],
alc_cv_cxx_problematic_version=no, alc_cv_cxx_problematic_version=yes)
AC_LANG_RESTORE
])
AC_MSG_RESULT($alc_cv_cxx_problematic_version)
if test "$alc_cv_cxx_problematic_version" = yes; then
AC_MSG_ERROR(*** This package works only with ${CXX} version 2.95.2 or higher ... please install gcc 2.95.2 or higher or set the environment variable CC and CXX to the good gcc before running the configure script ***)
fi
])dnl
dnl
dnl
