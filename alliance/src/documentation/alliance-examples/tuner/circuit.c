#include <stdio.h>
#include <genpat.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

int main(void)
{
int i, j, samples[] = {190, 191, 192, 193, 379, 380, 381, 382};
char s[1024];

   DEF_GENPAT("circuit");

   /* interface */
   DECLAR("ck", ":1", "B", IN, "", "");
   DECLAR("d", ":1", "B", IN, "", "");
   DECLAR("o", ":1", "B", OUT, "0 to 22", "");
   DECLAR("vddi", ":2", "B", IN, "", "");
   DECLAR("vssi", ":2", "B", IN, "", "");
   DECLAR("vdde", ":2", "B", IN, "", "");
   DECLAR("vsse", ":2", "B", IN, "", "");

   LABEL("tuner");
   SETTUNIT("ns");
   AFFECT("0", "ck",  "0b0");
   AFFECT("0", "d",   "0b0");
   AFFECT("0", "vddi", "0b1");
   AFFECT("0", "vssi", "0b0");
   AFFECT("0", "vdde", "0b1");
   AFFECT("0", "vsse", "0b0");

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
