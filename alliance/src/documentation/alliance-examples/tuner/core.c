#include <stdio.h>
#include <genpat.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

int main(void)
{
int i, j, samples[] = {190, 191, 192, 193, 379, 380, 381, 382};
char s[1024];

   DEF_GENPAT("core");

   /* interface */
   DECLAR("ck", ":1", "B", IN, "", "");
   DECLAR("d", ":1", "B", IN, "", "");
   DECLAR("o", ":1", "B", OUT, "0 to 22", "");
   DECLAR("vdd", ":2", "B", IN, "", "");
   DECLAR("vss", ":2", "B", IN, "", "");
#if 0
   DECLAR("tuner.start", ":2", "B", SIGNAL, "", "");
   DECLAR("tuner.incr_reg", ":2", "X", REGISTER, "14 downto 0", "");
   DECLAR("tuner.test_out", ":2", "B", SIGNAL, "14", "");
   DECLAR("tuner.norm_reg", ":2", "X", REGISTER, "14 downto 0", "");
   DECLAR("tuner.subs_out", ":2", "X", SIGNAL, "8 downto 0", "");
   DECLAR("tuner.deco_reg", ":2", "X", REGISTER, "7 downto 0", "");
#endif

   LABEL("tuner");
   SETTUNIT("ns");
   AFFECT("0", "ck",  "0b0");
   AFFECT("0", "d",   "0b0");
   AFFECT("0", "vdd", "0b1");
   AFFECT("0", "vss", "0b0");

   for (j = 0; j < ARRAY_SIZE(samples); j++) {
   int n = 0;
      AFFECT("+10", "ck", "0b1"); n++;
      AFFECT("+10", "d", "0b1"); 
      AFFECT("+0", "ck", "0b0");
      AFFECT("+10", "ck", "0b1"); n++;
      AFFECT("+10", "d", "0b0"); 
      AFFECT("+0", "ck", "0b0");

      for (i = 0; i < samples[j] - n; i++) {
         AFFECT("+10", "ck", "0b1");
         AFFECT("+10", "ck", "0b0");
      }
   }
   SAV_GENPAT();
}
