#include <genlib.h>

main()
{
int i;

   GENLIB_DEF_LOFIG("addaccu");

   GENLIB_LOCON("a[0:3]",  IN,  "a[0:3]"); /* input bus a       */
   GENLIB_LOCON("b[0:3]",  IN,  "b[0:3]"); /* input bus b       */
   GENLIB_LOCON("sel",     IN,  "sel"   ); /* selection switch  */
   GENLIB_LOCON("ck",      IN,  "ck"    ); /* clock             */
   GENLIB_LOCON("vdd",     IN,  "vdd"   ); /* core power supply */
   GENLIB_LOCON("vss",     IN,  "vss"   ); /* core ground       */
   GENLIB_LOCON("vdde",    IN,  "vdde"  ); /* pads power supply */
   GENLIB_LOCON("vsse",    IN,  "vsse"  ); /* pads ground       */
   GENLIB_LOCON("s[0:3]",  OUT, "s[0:3]"); /* output            */

/*
   power supplies:
   pxxxe_p are external power supplies, ie used only by the buffers
       inside the pads.
   pxxxi_p are internal power supplies, for core logic only.
*/

   GENLIB_LOINS ("pvsse_sp", "p15", "cki", "vdde", "vdd", "vsse", "vss", 0);
   GENLIB_LOINS ("pvdde_sp", "p16", "cki", "vdde", "vdd", "vsse", "vss", 0);
   GENLIB_LOINS ("pvddeck_sp", "p17", "clock", "cki", "vdde", "vdd", "vsse", "vss", 0);
   GENLIB_LOINS ("pvssi_sp", "p18", "cki", "vdde", "vdd", "vsse", "vss", 0);
   GENLIB_LOINS ("pvddi_sp", "p19", "cki", "vdde", "vdd", "vsse", "vss", 0);

/*
   core to pads connections are point to point connections in the case
   of this circuit, because no three state ouput pads are used.
   We use a loop to avoid copy/paste kind of things that may lead to
   errors easilly.
*/
   for (i = 0; i < 4; i++)
    GENLIB_LOINS("pi_sp", GENLIB_NAME("p%d", i), 
          GENLIB_NAME("a[%d]", i), GENLIB_NAME("aa[%d]", i), 
          "cki", "vdde", "vdd", "vsse", "vss", 0);

   for (i = 0; i < 4; i++)
    GENLIB_LOINS("pi_sp", GENLIB_NAME("p%d", i + 4), 
          GENLIB_NAME("b[%d]", i), GENLIB_NAME("bb[%d]", i), 
          "cki", "vdde", "vdd", "vsse", "vss", 0);

   for (i = 0; i < 4; i++)
    GENLIB_LOINS("po_sp", GENLIB_NAME("p%d", i + 8), 
          GENLIB_NAME("ss[%d]", i), GENLIB_NAME("s[%d]", i),
          "cki", "vdde", "vdd", "vsse", "vss", 0);


   GENLIB_LOINS("pi_sp", "p12",
         "sel", "selsel",
         "cki", "vdde", "vdd", "vsse", "vss", 0);

   GENLIB_LOINS("pck_sp", "p13",
         "ck",
         "cki", "vdde", "vdd", "vsse", "vss", 0);

   GENLIB_LOINS("core", "core",
         "aa[0:3]", "bb[0:3]", "selsel", "clock", "ss[0:3]",
         "vdd", "vss", 0);

   GENLIB_SAVE_LOFIG();
   exit(0); /* necessary for the proper run of the Makefile */
}
