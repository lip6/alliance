#include <stdlib.h>
#include MUT_H
#include MLO_H
#include MLU_H

int main (int argc, char **argv) {

lofig_list *ptlofig;
losig_list *ptsignal;

chain_list *ptchain;

int nsignal[10] = {0,0,0,0,0,0,0,0,0,0};
int nb_of_signal = 0;
int nb_of_con;
int nb_of_con_max = 2;
int i;

  if (argc != 2) {
    fprintf(stderr, "usage : stat_netlist file\n");
    exit(1);
  }
  argv++;

  mbkenv();
  ptlofig = getlofig(*argv, 'A');

  lofigchain(ptlofig);
  for (ptsignal = ptlofig->LOSIG; ptsignal; ptsignal = ptsignal->NEXT) {
    if (!naturalstrcmp(getsigname(ptsignal), "vdd") || !naturalstrcmp(getsigname(ptsignal), "vss"))
      continue;
    nb_of_signal++;
    nb_of_con = 0;
    ptchain = (chain_list *)((getptype(ptsignal->USER, (long)LOFIGCHAIN))->DATA);
    while (ptchain) {
      nb_of_con++;
      ptchain = ptchain->NEXT;
    }
    if (nb_of_con > nb_of_con_max)
      nb_of_con_max = nb_of_con;
    if (nb_of_con <= 10)
      nsignal[nb_of_con-2]++;
    else
      nsignal[9]++;
  }

  printf("netlist : %s\n\n", *argv);
  printf("vdd et vss sont ecartés du recensement\n");
  printf("nombre total de signal : %d\n", nb_of_signal);
  for (i = 0; i < 9; i++)
    printf("nombre de %d-signal : %d\n", i+2, nsignal[i]);
  printf("nombre de 11&+-signal : %d\n", nsignal[9]);
  printf("nombre maximum de connecteurs pour un signal : %d\n\n", nb_of_con_max);

  dellofig(*argv);
  exit(0);
}
