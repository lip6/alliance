#include <genlib.h>

main()
{
GENLIB_DEF_LOFIG("data_path");

/* connectors declaration */

GENLIB_LOCON("a[3:0]",IN,"a[3:0]");
GENLIB_LOCON("b[3:0]",IN,"b[3:0]");
GENLIB_LOCON("c[3:0]",IN,"c[3:0]");
GENLIB_LOCON("v",IN,"w");
GENLIB_LOCON("cout",OUT,"ct");
GENLIB_LOCON("s[3:0]",OUT,"s[3:0]");
GENLIB_LOCON("cmd",IN,"cmd");
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");

/* operators creation */

GENLIB_MACRO(DPGEN_NAND2, "model_nand2_4bits", F_PLACE, 4, 1);
GENLIB_MACRO(DPGEN_OR2, "model_or2_4bits", F_PLACE, 4);
GENLIB_MACRO(DPGEN_ADSB2F, "model_add2_4bits", F_PLACE, 4);

/* operators Instanciation */

GENLIB_LOINS("model_nand2_4bits", "model_nand2_4bits",
	"v", "v", "v", "v",
	"a[3:0]",
	"d_aux[3:0]",
	vdd, vss, NULL);

GENLIB_LOINS("model_or2_4bits", "model_or2_4bits",
	"d_aux[3:0]",
	"b[3:0]",
	"e_aux[3:0]",
	vdd, vss, NULL);

GENLIB_LOINS("model_add2_4bits", "model_add2_4bits",
	"cmd",
	"cout",
	"ovr",
	"e_aux[3:0]",
	"c[3:0]",
	"s[3:0]",
	vdd, vss, NULL);

/* Save of figure */

GENLIB_SAVE_LOFIG();
exit(0);
}
																
