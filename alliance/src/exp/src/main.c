#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "exp.h"
#include "ht.h"

void usage (char *av[])
{
  printf ("Usage : %s [-h] [-v level] [-o outfile] [infile]\n\n", av[0]);
  printf ("  -h     this help\n");
  printf ("  -v     verbose mode (level is 1:2:3:4)\n");
  printf ("  -o     define the output file (stdout by default)\n");
  printf ("  infile define the input file (stdin by default)\n");
  printf ("\n");
  exit (1);
}

void getoption (int ac, char *av[], FILE ** expout, FILE ** expin)
{
  char option;
  int verbose;
  *expin = stdin;
  *expout = stdout;
  yydebug = 0;

  dico=htinit(10000);
  htset(dico,"verbose")->VAL.f = -1.0;
  htget(dico,"verbose")->TYPE = HT_INT;
  htget(dico,"verbose")->USED = 1;

  while ((option = getopt (ac, av, "hv:o:")) != EOF)
  {
    switch (option)
    {
    case 'v':
      verbose = *optarg - '0';
      if ((verbose < 0)||(verbose > 9))
        usage (av);
      yydebug = (verbose == 4) ? 1 : 0;
      htset(dico,"verbose")->VAL.f = verbose;
      break;
    case 'o':
      if ((*expout = fopen (optarg, "w")) == NULL)
        usage (av);
      break;
    default:
      usage (av);
    }
  }
  if (optind < ac)
  {
    if ((*expin = fopen (av[optind], "r")) == NULL)
      usage (av);
    expfname = strdup (av[optind++]);
  }
  if (optind < ac)
    usage (av);
}

int main (int ac, char *av[])
{
  getoption (ac, av, &yyout, &yyin);
  if (htget(dico,"verbose")->VAL.f != 0)
  {
    puts(" ");
    puts("                            @@@@@   @@@@  @@@ @@@ @@@");
    puts("                           @     @   @@    @   @@@  @@");
    puts("                          @@     @@   @@  @    @@    @@");
    puts("                          @@@@@@@@@    @@@     @@    @@");
    puts("                          @@           @@@     @@    @@");
    puts("                          @@      @   @  @@    @@    @@");
    puts("                           @@    @@  @    @@   @@@  @@");
    puts("                             @@@@   @@@  @@@@  @@ @@@");
    puts("                                               @@");
    puts("                                              @@@@");
    puts(" ");
    puts("                         multiple expressions calculator");
    puts(" ");
    printf("                  Alliance CAD System %-4s              exp %-3s\n",
                            ALLIANCE_VERSION, VERSION);
    puts("                  Copyright (c) 2002,            ASIM/LIP6/UPMC");
    puts("                  E-mail support: alliance-support@asim.lip6.fr");
    puts(" ");
  }
  yyparse ();
  return 0;
}
