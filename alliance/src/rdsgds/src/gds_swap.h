#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
#ifdef __STDC__
int    islittle(void);
char   swapc(char);
short  swaps(short);
int    swapi(int);
long   swapl(long);
double swapd(double);
#else
  extern            int  islittle __P(());
  extern           char  swapc __P(());
  extern          short  swaps __P(());
  extern            int  swapi __P(());
  extern           long  swapl __P(());
  extern         double  swapd __P(());
#endif
