/*
 * $Log: x2vy_main.c,v $
 * Revision 1.2  2009/06/14 13:51:57  ludo
 * - port 64 bits
 *
 * Revision 1.1.1.1  2002/04/26 09:51:07  ludo
 * Mise a plat de mbkvhdlg
 *
 * Revision 1.1.1.1  2002/04/11 08:54:53  fred
 * Importing mbkvhdlg into the new Alliance CVS tree
 *
 * Revision 1.1.1.1  2002/02/28 12:58:31  disydent
 * Creation of Disydent CVS Tree
 *
 * Revision 1.1.1.1  2001/11/19 16:55:27  pwet
 * Changing the CVS tree structure of disydent
 *
 * Revision 1.1.1.1  2001/07/24 13:31:41  pwet
 * cvs tree of part of disydent
 *
 * Revision 1.1.1.1  2001/07/19 14:32:19  pwet
 * New cvs tree
 *
 * Revision 1.5  1999/02/23 15:19:18  pwet
 * add -g option. This option is used to remove generic statements.
 *
 * Revision 1.4  1998/11/12 10:39:25  pwet
 * add makevelofig function.
 *
 * Revision 1.3  1998/01/06 09:52:58  denis
 * invert mode , so now 0 is vst and 1 is vhd
 *
 * Revision 1.2  1997/12/17 16:41:33  denys
 * remove the netlist.vst driven if the VEL_DEBUG flag was set.
 *
 * Revision 1.1  1997/10/16  14:22:05  denys
 * Initial revision
 *
 */

#ident "$Id: x2vy_main.c,v 1.2 2009/06/14 13:51:57 ludo Exp $"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include "gen_generic.h"
#include "vel_velo.h"

#define NOALIM 0x01
#define RENINS 0x02
#define VECCON 0x04
#define VSTDRV 0x08
#define MBKDRV 0x10
#define NOVECT 0x20
#define ADDSIG 0x40
#define NOLOGEN 0x80

static void printusage(void)
{
   fputs("** Usage : x2vy [-nrvc] file [outputfile]\n",               stderr);
   fputs("**         -n : remove power connectors\n",                 stderr);
   fputs("**         -g : remove generic statements\n",               stderr);
   fputs("**         -r : rename instance\n",                         stderr);
   fputs("**         -c : vectorize connectors\n",                    stderr);
   fputs("**         -v : alliance VHDL output (.vst)\n",             stderr);
   fputs("**         -m : use mbkdriver (don't vectorize signals)\n", stderr);
   fputs("**         -u : don't vectorize signals\n",                 stderr);
}

static void removepowers(lofig_list *f)
{
locon_list *c, *cc;
loins_list *i;

   cc=NULL;
   for (c=f->LOCON; c;) {
      if (isvdd(c->NAME) || isvss(c->NAME)) {
         if (cc) {
            cc->NEXT=c->NEXT;
            mbkfree(c);
            c=cc->NEXT;
         } else {
            f->LOCON=c->NEXT;
            mbkfree(c);
            c=f->LOCON;
         }
      } else {
         cc=c;
         c=c->NEXT;
      }
   }
   for (i=f->LOINS; i; i=i->NEXT) {
      cc=NULL;
      for (c=i->LOCON; c;) {
         if (isvdd(c->NAME) || isvss(c->NAME)) {
            if (cc) {
               cc->NEXT=c->NEXT;
               mbkfree(c);
               c=cc->NEXT;
            } else {
               i->LOCON=c->NEXT;
               mbkfree(c);
               c=i->LOCON;
            }
         } else {
            cc=c;
            c=c->NEXT;
         }
      }
   }
}

static void removelogen(lofig_list *f)
{
   loins_list *i;

   if (f->USER)
      delptype(f->USER, LOGEN); /* I know, it's dirty */

   for (i=f->LOINS; i; i=i->NEXT)
      if (i->USER)
         delptype(i->USER, LOGEN); /* still dirty */
      
}

static void renameinstance(lofig_list *f)
{
   loins_list *i;
   char tab[512];

   for (i=f->LOINS;i;i=i->NEXT) {
      strcpy(tab, "v_");
      strcat(tab, i->INSNAME);
      i->INSNAME=namealloc(tab);
   }
}

int main(int argc, char **argv)
{
lofig_list *f;
char *filein=NULL, *fileout=NULL;
int i,j;
int opt=0;

   if (argc<2) {
      printusage();
      exit(1);
   }
   mbkenv();

   for (i=1; i<argc; i++)
      if (argv[i][0]=='-')
         for (j=1; argv[i][j]!='\0'; j++) {
            switch (argv[i][j]) {
               case 'a':
                  opt|=ADDSIG;
                  break;
               case 'n':
                  opt|=NOALIM;
                  break;
               case 'g':
                  opt|=NOLOGEN;
                  break;
               case 'r':
                  opt|=RENINS;
                  break;
               case 'v':
                  opt|=VSTDRV;
                  break;
               case 'c':
                  opt|=VECCON;
                  break;
               case 'm':
                  opt|=MBKDRV;
                  break;
               case 'u':
                  opt|=NOVECT;
                  break;
               default:
                  fprintf(stderr, "** Unknown option -%c\n", argv[i][1]);
                  printusage();
                  exit(2);
            }
         }
      else
         if (!filein)
            filein=argv[i];
         else
            if (!fileout)
               fileout=argv[i];
            else {
               fputs("** Too many filenames\n", stderr);
               printusage();
               exit(4);
            }
               
   if (!filein) {
      fputs("** Filename expected!\n", stderr);
      printusage();
      exit(3);
   }

   f=getlofig(filein,'A');

   guessextdir(f);

   if (opt & NOALIM)
      removepowers(f);

   if (opt & RENINS)
      renameinstance(f);

   if (opt & NOLOGEN)
      removelogen(f);

   #if 0
   if (opt & VECCON) {
      vectlocon(f);
      sortlocon(&f->LOCON);
      f->LOCON=(locon_list *)reverse(f->LOCON);
   }
   #endif

   if (fileout)
      f->NAME=namealloc(fileout);

   if (!(opt & NOVECT))
      vectlosig(&f->LOSIG);

   if (opt & MBKDRV)
      savelofig(f);
   else {
      if (opt&VSTDRV)
         vhdlsavevelofig(f, 0);
      else if (opt&ADDSIG)
         vhdlsavevelofig(f, 1);
      else
         vhdlsavevelofig(f, 2);
   }

#if 0
#ifdef VEL_DEBUG
   f->NAME=namealloc("netlist");
   vhdlsavevelofig(f,1);
#endif
#endif

   exit(0);
}
