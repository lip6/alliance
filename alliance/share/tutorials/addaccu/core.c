/*
   genlib.h is required for all genlib programs.
   it defines the set of functions we will use for schematic capture
*/
#include <genlib.h>

/*
   core.c is to become the `core' executable program. So we must
   define a main procedure main()
*/

main()
{
int i; /* We will build regular structure using a loop,  i is its index */

/*
   When executed,  the addaccu program opens the netlist view 
   "core". Operations performed after opening "accu" are
   taken into account.
*/

   DEF_LOFIG("core");

/*
   The description of the addaccu netlist begins with the
   instanciation of input/output terminals
*/

/*
   We start with the input terminals.
   Note that `a[0:3]' defines a[0],  a[1],  a[2] and a[3] as if three
   calls to `LOCON' had been performed.
   Only signals and connectors can be vectorized.
*/
   LOCON("a[0:3]",   IN,  "a[0:3]");  /* input bus a      */
   LOCON("b[0:3]",   IN,  "b[0:3]");  /* input bus b      */
   LOCON("sel",      IN,  "sel"   );  /* selection switch */
   LOCON("ck",       IN,  "ck"    );  /* clock            */

/*
   Then,  the output terminals.
*/

   LOCON("s[0:3]", INOUT, "s[0:3]"); /* output bus       */

/*
   Then the supplies.
   They are inputs, but we like them better at the end of the
   description.
*/
   LOCON("vdd",      IN,  "vdd"   );  /* power supply     */
   LOCON("vss",      IN,  "vss"   );  /* ground           */

/*
   Now we describe the circuit's schematic in instanciating
   standard cells.

*/

/*
   bit 0
*/
   LOINS("xr2_y", "xr0",
         "mux[0]", "b[0]", "s[0]",
         "vdd", "vss", 0);
 
   LOINS("a2_y", "an0",
         "mux[0]", "b[0]", "carry[0]",
         "vdd", "vss", 0);
 
/*
    bit 1
*/
 
   LOINS("xr2_y", "xr1",
         "mux[1]", "b[1]", "int[1]",
         "vdd", "vss", 0);
 
   LOINS("xr2_y", "xr2",
         "int[1]", "carry[0]", "s[1]",
         "vdd", "vss", 0);
 
   LOINS("a2_y",  "an1",
         "mux[1]", "b[1]", "int[2]",
         "vdd", "vss", 0);
 
   LOINS("a2_y",  "an2",
         "mux[1]", "carry[0]", "int[3]",
         "vdd", "vss", 0);
 
   LOINS("a2_y",  "an3",
         "b[1]", "carry[0]", "int[4]",
         "vdd", "vss", 0);
 
   LOINS("o3_y",  "an4",
         "int[2]", "int[3]", "int[4]", "carry[1]",
         "vdd", "vss", 0);
 
/*
   bit 2
*/
 
   LOINS("xr2_y", "xr3",
         "mux[2]", "b[2]", "int[5]",
         "vdd", "vss", 0);
 
   LOINS("xr2_y", "xr4",
         "int[5]", "carry[1]", "s[2]",
         "vdd", "vss", 0);
 
   LOINS("a2_y",  "an5",
         "mux[2]", "b[2]", "int[6]",
         "vdd", "vss", 0);
 
   LOINS("a2_y",  "an6",
         "mux[2]", "carry[1]", "int[7]",
         "vdd", "vss", 0);
 
   LOINS("a2_y",  "an7",
         "b[2]", "carry[1]", "int[8]",
         "vdd", "vss", 0);
 
   LOINS("o3_y",  "an8",
         "int[6]", "int[7]", "int[8]", "carry[2]",
         "vdd", "vss", 0);
 
/*
   bit 3
*/
   LOINS("xr2_y", "xr5",
         "mux[3]", "b[3]", "int[9]",
         "vdd", "vss", 0);
 
   LOINS("xr2_y", "xr6",
         "int[9]", "carry[2]", "s[3]",
         "vdd", "vss", 0);
 
/*
   mux 2 to 1
*/
   LOINS("n1_y",  "n10", "sel", "nsel", "vdd", "vss", 0);

/*
  The `NAME' function allows to indice a name automatically
*/
   for (i = 0; i < 4; i++) {
     LOINS("mx2_y", NAME("mux%d", i),
            NAME("a[%d]", i), "nsel", NAME("regout[%d]", i),
            "sel", NAME("mux[%d]", i),
            "vdd", "vss", 0);

     LOINS("ms_y",  NAME("l%d", i),
           NAME("s[%d]", i), "ck", NAME("regout[%d]", i),
           "vdd", "vss", 0);
   }

   SAVE_LOFIG();
   exit(0); /* necessary for the proper run of the Makefile */
}
