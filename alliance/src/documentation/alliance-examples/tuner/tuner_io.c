#include <genlib.h>

int main(void)
{
   int i;

   GENLIB_DEF_LOFIG("tuner");
   GENLIB_LOCON("ck", IN, "ck");
   GENLIB_LOCON("d", IN, "d");
   GENLIB_LOCON("o[0:22]", OUT, "o[0:22]");
   GENLIB_LOCON("vdde", IN, "vdde");
   GENLIB_LOCON("vddi", IN, "vddi");
   GENLIB_LOCON("vsse", IN, "vsse");
   GENLIB_LOCON("vssi", IN, "vssi");
   GENLIB_LOINS("tuner_o", "core", "ckc", "di", "oi[0:22]", "vddi", "vssi", 0);
   GENLIB_LOINS("pck_sp", "pck", "ck", "cki", "vdde", "vddi", "vsse", "vssi", 0);
   GENLIB_LOINS("pi_sp",  "pd",  "d", "di", "cki", "vdde", "vddi", "vsse", "vssi", 0);
   /* outputs */
   for (i = 0; i < 23; i++)
      GENLIB_LOINS("po_sp", GENLIB_NAME("po%d", i),
                            GENLIB_ELM("oi", i),
                            GENLIB_ELM("o", i),
                            "cki", "vdde", "vddi", "vsse", "vssi", 0);
   /* Power supplies for the buffers */
   GENLIB_LOINS("pvddeck_sp",  "pvdde",  "ckc", "cki", "vdde", "vddi", "vsse", "vssi", 0);
   GENLIB_LOINS("pvsseck_sp",  "pvsse",  "ckc", "cki", "vdde", "vddi", "vsse", "vssi", 0);
   /* Power supplies for the core */
   GENLIB_LOINS("pvddick_sp",  "pvddi",  "ckc", "cki", "vdde", "vddi", "vsse", "vssi", 0);
   GENLIB_LOINS("pvssick_sp",  "pvssi",  "ckc", "cki", "vdde", "vddi", "vsse", "vssi", 0);

   GENLIB_SAVE_LOFIG();
   exit(0);
}
