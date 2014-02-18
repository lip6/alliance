/* This file is moreorless a verbatim copy of the swaptype file issued from
   the tool Pwet.
   Author: Frederic Petrot */

#include "gds_swap.h"

/* return 0 for big-endian, 1 for little-endian */
#ifdef __STDC__
int islittle(void)
#else
int islittle()
#endif
{
  int x = 0x00000001;

  return *(char *)&x;
}

/* I assume that I know the sizeof all relevant types...
   Clean implementations would loop on the sizeof. */

#ifdef __STDC__
char swapc(char k)
#else
char swapc(k)
char k;
#endif
{
char t;

   t = k;
   return t;
}

#ifdef __STDC__
short swaps(short s)
#else
short swaps(s)
short s;
#endif
{
short t;

   *((char *)&t + 1) = *(char *)&s;
   *(char *)&t       = *((char *)&s + 1);
   return t;
}

#ifdef __STDC__
int swapi(int i)
#else
int swapi(i)
int i;
#endif
{
int t;

    *((char *)&t + 3) = *(char *)&i;
    *((char *)&t + 2) = *((char *)&i + 1);
    *((char *)&t + 1) = *((char *)&i + 2);
    *(char *)&t       = *((char *)&i + 3);
    return t;
}

#ifdef __STDC__
long swapl(long l)
#else
long swapl(l)
long l;
#endif
{
long t;

    *((char *)&t + 3) = *(char *)&l;
    *((char *)&t + 2) = *((char *)&l + 1);
    *((char *)&t + 1) = *((char *)&l + 2);
    *(char *)&t       = *((char *)&l + 3);
    return t;
}

#ifdef __STDC__
double swapd(double d)
#else
double swapd(d)
double d;
#endif
{
double t = 0;

    *((char *)&t + 7) = *((char *)&d + 0);
    *((char *)&t + 6) = *((char *)&d + 1);
    *((char *)&t + 5) = *((char *)&d + 2);
    *((char *)&t + 4) = *((char *)&d + 3);
    *((char *)&t + 3) = *((char *)&d + 4);
    *((char *)&t + 2) = *((char *)&d + 5);
    *((char *)&t + 1) = *((char *)&d + 6);
    *((char *)&t + 0) = *((char *)&d + 7);
    return t;
}
