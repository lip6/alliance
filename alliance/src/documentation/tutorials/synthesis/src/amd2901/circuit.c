#include <genlib.h>
main()
{
GENLIB_DEF_LOFIG("circuit");

/* connectors Declaration */
GENLIB_LOCON("a",IN,"a1");
GENLIB_LOCON("b",IN,"b1");
GENLIB_LOCON("c",IN,"c1");
GENLIB_LOCON("d",IN,"d1");
GENLIB_LOCON("e",IN,"e1");
GENLIB_LOCON("s",OUT,"s1");
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");

/* Instantiation of the logical doors */
GENLIB_LOINS("na2_x1","nand2","a1","c1","f1","vdd","vss",0);
GENLIB_LOINS("no2_x1","nor2","b1","e1","g1","vdd","vss",0);
GENLIB_LOINS("o2_x2","or2","d1","f1","h1","vdd","vss",0);
GENLIB_LOINS("inv_x1","inv","g1","i1","vdd","vss",0);
GENLIB_LOINS("a2_x2","and2","h1","i1","s1","vdd","vss",0);

/* Save of the figure */
GENLIB_SAVE_LOFIG();
exit(0);
}
														    
