# include  <genlib.h>
# include  <dgn.h>

int main()
{
/*Preplacement du coeur*/
GENLIB_DEF_PHFIG("mips_core_place");

GENLIB_PLACE("mips_dpt","dpt",NOSYM,0,0);

/* boite d'aboutement */
GENLIB_DEF_AB(-150,-150,150,2000);

GENLIB_SAVE_PHFIG();

return 0;
}
