#include <genlib.h>

main()
{
int i;

   DEF_LOFIG("addaccu");

   LOCON("a[0:3]",  IN,  "a[0:3]"); /* input bus a       */
   LOCON("b[0:3]",  IN,  "b[0:3]"); /* input bus b       */
   LOCON("sel",     IN,  "sel"   ); /* selection switch  */
   LOCON("ck",      IN,  "ck"    ); /* clock             */
   LOCON("vdd",     IN,  "vdd"   ); /* core power supply */
   LOCON("vss",     IN,  "vss"   ); /* core ground       */
   LOCON("vdde",    IN,  "vdde"  ); /* pads power supply */
   LOCON("vsse",    IN,  "vsse"  ); /* pads ground       */
   LOCON("s[0:3]",  OUT, "s[0:3]"); /* output            */

/*
   power supplies:
   pxxxe_p are external power supplies, ie used only by the buffers
       inside the pads.
   pxxxi_p are internal power supplies, for core logic only.
*/

   LOINS ("pvsse_sp", "p15", "cki", "vdde", "vdd", "vsse", "vss", 0);
   LOINS ("pvsse_sp", "p16", "cki", "vdde", "vdd", "vsse", "vss", 0);
   LOINS ("pvddeck_sp", "p17", "clock", "cki", "vdde", "vdd", "vsse", "vss", 0);
   LOINS ("pvssi_sp", "p18", "cki", "vdde", "vdd", "vsse", "vss", 0);
   LOINS ("pvddi_sp", "p19", "cki", "vdde", "vdd", "vsse", "vss", 0);

/*
   core to pads connections are point to point connections in the case
   of this circuit, because no three state ouput pads are used.
   We use a loop to avoid copy/paste kind of things that may lead to
   errors easilly.
*/
   for (i = 0; i < 4; i++)
    LOINS("pi_sp", NAME("p%d", i), 
          NAME("a[%d]", i), NAME("aa[%d]", i), 
          "cki", "vdde", "vdd", "vsse", "vss", 0);

   for (i = 0; i < 4; i++)
    LOINS("pi_sp", NAME("p%d", i + 4), 
          NAME("b[%d]", i), NAME("bb[%d]", i), 
          "cki", "vdde", "vdd", "vsse", "vss", 0);

   for (i = 0; i < 4; i++)
    LOINS("po_sp", NAME("p%d", i + 8), 
          NAME("ss[%d]", i), NAME("s[%d]", i),
          "cki", "vdde", "vdd", "vsse", "vss", 0);


   LOINS("pi_sp", "p12",
         "sel", "selsel",
         "cki", "vdde", "vdd", "vsse", "vss", 0);

   LOINS("pck_sp", "p13",
         "ck",
         "cki", "vdde", "vdd", "vsse", "vss", 0);

   LOINS("core", "core",
         "aa[0:3]", "bb[0:3]", "selsel", "clock", "ss[0:3]",
         "vdd", "vss", 0);

   SAVE_LOFIG();
   exit(0); /* necessary for the proper run of the Makefile */
}
