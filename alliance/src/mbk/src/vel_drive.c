/* ###--------------------------------------------------------------------### */
/* #                                                                        # */
/* # file : vel_drive.c                                                     # */
/* | date : Jul  3 1997                                                     | */
/* | version : 1.00                                                         | */
/* | author : Denis Hommais                                                 | */
/* |                                                                        | */
/* # contents : vhdl driver                                                 # */
/* #                                                                        # */
/* ###--------------------------------------------------------------------### */
/*
 * $Log: vel_drive.c,v $
 * Revision 1.9  2002/12/19 12:56:29  fred
 * Cleaning up things when adding signals
 *
 * Revision 1.8  2002/12/17 15:51:20  francois
 * estimate delta for each signal.
 * Not for the first only.
 *
 * Revision 1.7  2002/12/12 21:28:27  xtof
 * What's this MODELNAME doing here ????
 *
 * Revision 1.6  2002/12/12 14:49:49  fred
 * Adding corrections to generate (hopefully) correct VHDL netlists with
 * generics.
 *
 * Revision 1.5  2002/12/10 11:39:05  fred
 * Adding correct generation of incomplete vectors.
 *
 * Revision 1.4  2002/12/06 09:56:05  fred
 * Erasing a forgotten debug dump !
 *
 * Revision 1.3  2002/12/06 08:52:53  fred
 * Adding better support to vectorized VHDL output.
 * The data structure doesn't follow the VHDL conventions, and this makes
 * it hard to produce correct vectorized output from that.
 * There are still some issues, such as bit with different types that are
 * not handled correctly.
 *
 * Revision 1.2  2002/06/12 16:10:05  fred
 * Bug due to the believe that a signal connected to a connector has the
 * name of this connector corrected.
 *
 * Revision 1.1.1.1  2002/04/26 09:51:07  ludo
 * Mise a plat de mbkvhdlg
 *
 * Revision 1.2  2002/04/16 07:24:51  fred
 * Adding modifications to handle limited generics in VHDL files.
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
 * Revision 1.1.1.1  2001/07/24 13:31:42  pwet
 * cvs tree of part of disydent
 *
 * Revision 1.1.1.1  2001/07/19 14:32:20  pwet
 * New cvs tree
 *
 * Revision 1.9  1998/11/12 10:41:26  pwet
 * Add indentation in the driver
 *
 * Revision 1.8  1998/11/12 10:39:26  pwet
 * add makevelofig function.
 *
 * Revision 1.7  1998/01/06 09:53:40  denis
 * merge the two drivers in one function.
 * use an array instead of a case for connector type
 *
 * Revision 1.6  1997/12/17 16:41:33  denys
 * add 'M' type check for partial vector connection
 *
 * Revision 1.5  1997/12/10 10:42:01  denys
 * Change bit_vector to stdlogic_vector
 *
 * Revision 1.4  1997/12/05 10:25:23  denys
 * *** empty log message ***
 *
 * Revision 1.3  1997/09/24  09:46:48  denys
 * rename the signal when it is connected to a external connector by the
 * name of the connector
 *
 * Revision 1.2  1997/09/23  09:42:15  denys
 * change , to & when concatened signals
 *
 */

#ident "$Id: vel_drive.c,v 1.9 2002/12/19 12:56:29 fred Exp $"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include "vel_velo.h"
#include "gen_generic.h"

extern void vhdlsavelofig();
extern int vhdlsavevelofig();
extern void vectorize_velosig();
extern void vectorize_velocon();
extern void chkdir();
extern void addsignals();

/* ###--------------------------------------------------------------------### */
/* #   Static Variables                                                     # */
/* ###--------------------------------------------------------------------### */

static int strlength;
static int first;
static int dim;

/* ###--------------------------------------------------------------------### */
/* #   Function prototype                                                   # */
/* ###--------------------------------------------------------------------### */

void debugvelocon(lofig_list *);
void debugvelosig(lofig_list *);
void debugveloins(lofig_list *);

/* ###--------------------------------------------------------------------### */
/* #   Macro definition                                                     # */
/* ###--------------------------------------------------------------------### */

#define getvectordirection(c) ((c)->LEFT>=(c)->RIGHT)?"downto":"to"

/* ###--------------------------------------------------------------------### */
/* #   Function printconnector                                              # */
/* ###--------------------------------------------------------------------### */

static const char *bittab[2][7]= {
   {
      "      %s : inout   bit%s\n",           /* 0 B */
      "",                                  /* 1 ! */
      "      %s : in      bit%s\n",           /* 2 I */
      "      %s : out     bit%s\n",           /* 3 O */
      "      %s : inout   mux_bit bus%s\n",   /* 4 T */
      "      %s : linkage bit%s\n",           /* 5 X */
      "      %s : out     mux_bit bus%s\n",   /* 6 Z */
   }, {
      "      %s : inout std_logic%s\n",       /* 0 B */
      "",                                  /* 1 ! */
      "      %s : in    std_logic%s\n",       /* 2 I */
      "      %s : out   std_logic%s\n",       /* 3 O */
      "      %s : inout std_logic%s\n",      /* 4 T */
      "      %s : inout std_logic%s\n",      /* 5 X */
      "      %s : out   std_logic%s\n",      /* 6 Z */
   }
};

static const char *vecttab[2][7]= {
   {
      "      %s : inout   bit_vector(%ld %s %ld)%s\n",      /* 0 B */
      "",                                                /* 1 ! */
      "      %s : in      bit_vector(%ld %s %ld)%s\n",      /* 2 I */
      "      %s : out     bit_vector(%ld %s %ld)%s\n",      /* 3 O */
      "      %s : inout   mux_vector(%ld %s %ld) bus%s\n",  /* 4 T */
      "      %s : linkage bit_vector(%ld %s %ld)%s\n",      /* 5 X */
      "      %s : out     mux_vector(%ld %s %ld) bus%s\n",  /* 6 Z */
   }, {
      "      %s : inout std_logic_vector(%ld %s %ld)%s\n",  /* 0 B */
      "",                                                   /* 1 ! */
      "      %s : in    std_logic_vector(%ld %s %ld)%s\n",     /* 2 I */
      "      %s : out   std_logic_vector(%ld %s %ld)%s\n",    /* 3 O */
      "      %s : inout std_logic_vector(%ld %s %ld) %s\n", /* 4 T */
      "      %s : inout std_logic_vector(%ld %s %ld)%s\n",  /* 5 X */
      "      %s : out   std_logic_vector(%ld %s %ld) %s\n",   /* 6 Z */
   }
};

static void printconnector(FILE *file, velocon *c, int mode, int maxlength)
{
   int i, length=strlen(c->NAME);
   char *buf=(char *)mbkalloc((maxlength+1)*sizeof(char));
   if (mode) mode=1;

   strcpy(buf, c->NAME);
   for (i=length; i<maxlength; i++)
      buf[i]=' ';
   buf[maxlength]='\0';

   if (c->LEFT==-1) {
      fprintf(file,
              bittab[mode][(((locon_list *)c->ROOT->DATA)->DIRECTION-'A')/4],
              buf, (c->NEXT)?";":"\n );");
   } else {
      fprintf(file,
              vecttab[mode][(((locon_list *)c->ROOT->DATA)->DIRECTION-'A')/4],
              buf, c->LEFT, getvectordirection(c), c->RIGHT,
              (c->NEXT)?";":"\n );");
   }
   mbkfree(buf);
}

static void drv_genvalue(FILE *ptfile, logen_list *ptgen)
{
   logen_list *l;
   int i;

   switch (ptgen->TYPE) {
      case GENTYPE_BIT:
         fprintf(ptfile, "'%c'", ptgen->VALUE.CHAR);
         break;
      case GENTYPE_HEX:
         fprintf(ptfile, "16#%08lx#", ptgen->VALUE.VAL);
         break;
      case GENTYPE_VAL:
         fprintf(ptfile, "%ld", ptgen->VALUE.VAL);
         break;
      case GENTYPE_ARG:
         fprintf(ptfile, "%ld", ptgen->VALUE.VAL);
         break;
      case GENTYPE_TEXT:
         fputs(ptgen->VALUE.TEXT, ptfile);
         break;
      case GENTYPE_LIST:
         fputc('(', ptfile);
         dim--;
         if (first
             && ptgen->VALUE.LIST && ptgen->VALUE.LIST->TYPE!=GENTYPE_LIST) {
            strlength++;
            first=0;
            dim=1;
         }
         for (l=ptgen->VALUE.LIST; l; l=l->NEXT) {
            drv_genvalue(ptfile, l);
            if (l->NEXT) {
               if (dim==2) {
                  fputs(",\n", ptfile);
                  for (i=0; i<strlength; i++) putc(' ', ptfile);
               } else
                  fputc(',', ptfile);
            }
         }
         fputc(')', ptfile);
         dim++;
         break;
   }
}

static void drv_generic(FILE *ptfile, ptype_list *pttype)
{
struct logen *ptgen;
int maxlength=0, length, j;
char *buf;

   if ((pttype=getptype(pttype,LOGEN))) {
      fprintf (ptfile,"   generic (\n");
      for (ptgen=pttype->DATA;ptgen;ptgen=ptgen->NEXT)
         if (maxlength<strlen(ptgen->NAME))
            maxlength=strlen(ptgen->NAME);

      buf=(char *)mbkalloc((maxlength+1)*sizeof(char));

      for (ptgen=pttype->DATA;ptgen;ptgen=ptgen->NEXT) {
         length=strlen(ptgen->NAME);
         strcpy(buf, ptgen->NAME);
         for (j=length; j<maxlength; j++)
            buf[j]=' ';
         buf[maxlength]='\0';

         strlength+=fprintf (ptfile,"      CONSTANT %s : ",buf);
         switch(ptgen->TYPE) {
            case GENTYPE_BIT:
               strlength+=fputs("bit     := ", ptfile);
               break;
            case GENTYPE_HEX:
               strlength+=fputs("natural := ", ptfile);
               break;
            case GENTYPE_VAL:
               strlength+=fputs("natural := ", ptfile);
               break;
            case GENTYPE_ARG:
               strlength+=fputs("arg     := ", ptfile);
               break;
            case GENTYPE_TEXT:
               strlength+=fputs("string  := ", ptfile);
               break;
            case GENTYPE_LIST:
               strlength+=fputs("list    := ", ptfile);
               break;
         }
         first=1;
         dim=0;
         drv_genvalue(ptfile, ptgen);
         if (ptgen->NEXT) {
            fputs(";\n", ptfile);
            strlength=0;
         }
      }
      fputs("\n   );\n",ptfile);
      strlength=0;
   }
}

/* ###--------------------------------------------------------------------### */
/* #   Function vhdsavevelofig                                              # */
/* ###--------------------------------------------------------------------### */

void vhdsavevelofig (lofig_list *f, int mode)
{
FILE *file;
ptype_list *p;
chain_list *m, *mod=NULL;
loins_list *i;
velocon *c;
velosig *s, *ss;
int maxlength;
char *buf=NULL;
int length, j;

   if (!(file=mbkfopen(f->NAME, mode?"vhd":"vst", "w"))) {
      fprintf(stderr,"Can't open file %s.%s", f->NAME, mode?"vhd":"vst");
      exit(1);
   }

   if (mode&4)
      fputs("library work;\nuse work.all;\n",file);
#if 0
   fputs("library WORK;\nuse WORK.P6_PKG.all;\n",file);
#endif
   if (mode)
      fputs("library ieee;\nuse ieee.std_logic_1164.all;\n",file);

/*
 * Entity Declaration
 */
   fprintf(file, "entity %s is\n", f->NAME);

/*
 * Generic Declaration
 */
   drv_generic(file, f->USER);

/*
 * Port Declaration
 */
   p=getptype(f->USER, VEL_CON);
   if (p && p->DATA) {
      maxlength=0;
      for (c=(velocon *)p->DATA; c; c=c->NEXT)
         if (maxlength<strlen(c->NAME))
            maxlength=strlen(c->NAME);
      fputs("   port (\n", file);
      for (c=(velocon *)p->DATA; c; c=c->NEXT)
         printconnector(file, c, mode, maxlength);
   }
   fprintf(file, "end %s;\n", f->NAME);
   fprintf(file, "\narchitecture structural of %s is\n", f->NAME);

/*
 * Make a list of the used components
 */
   for (i=f->LOINS; i; i=i->NEXT) {
      for (m=mod; m; m=m->NEXT)
          if (((loins_list *)m->DATA)->FIGNAME == i->FIGNAME) break;
      if (!m)
         mod=addchain(mod, i);
   }
/*
 * Write the list as component declaration
 */
   for (m=mod; m; m=m->NEXT) {
      i=(loins_list *)m->DATA;
      fprintf(file, "Component %s\n", i->FIGNAME);
/*
 * Generic Declaration
 */
      drv_generic(file, i->USER);

/*
 * Port Declaration
 */
      p=getptype(i->USER, VEL_CON);
      if (p->DATA) {
         maxlength=0;
         for (c=(velocon *)p->DATA; c; c=c->NEXT)
            if (maxlength<strlen(c->NAME))
               maxlength=strlen(c->NAME);
         fputs("   port (\n", file);
         for (c=(velocon *)p->DATA; c; c=c->NEXT)
            printconnector(file, c, mode, maxlength);
      }
      fputs("end component;\n\n", file);
   }

/*
 * Signal declaration
 */
   {
   char *bittype, *vecttype;
   bittype =(mode)? "std_logic"        : "bit";
   vecttype=(mode)? "std_logic_vector" : "bit_vector";

   p=getptype(f->USER, VEL_SIG);
   maxlength=0;
   if (p) {
      for (s=(velosig *)p->DATA; s; s=s->NEXT)
         if (maxlength<strlen(s->NAME))
            maxlength=strlen(s->NAME);
      for (s=(velosig *)p->DATA; s; s=s->NEXT)
         if (s->TYPE=='I' || s->TYPE=='S' || s->TYPE=='R') {
            int i, length=strlen(s->NAME);
            char *buf=(char *)mbkalloc((maxlength+1)*sizeof(char));
            strcpy(buf, s->NAME);
            for (i=length; i<maxlength; i++)
               buf[i]=' ';
            buf[maxlength]='\0';
            if (s->LEFT==-1)
               fprintf(file, "signal %s : %s;\n", buf, bittype);
            else
               fprintf(file, "signal %s : %s( %ld %s %ld);\n",
                       buf, vecttype,
                       s->LEFT, getvectordirection(s), s->RIGHT);
         }
      }
   }

   fputs("\nbegin\n\n", file);

   /*
    * Signals concatenation
    */
   if (mode&1 && p)
      for (s=(velosig *)p->DATA; s; s=s->NEXT)
         if (s->TYPE=='S') {
            fprintf(file, "   %s <= (", s->NAME);
            for (m=s->VSIG; m; m=m->NEXT) {
               ss=(velosig *)m->DATA;
               fputs(ss->NAME, file);
               if (ss->TYPE=='V' || ss->TYPE=='M') {
                  if (ss->LEFT==ss->RIGHT)
                     fprintf(file, "(%ld)", ss->LEFT);
                  else
                     fprintf(file, "(%ld %s %ld)",
                             ss->LEFT, getvectordirection(ss), ss->RIGHT);
               }
               if (m->NEXT)
                  fputs("& ",file);
            }
            fputs(");\n", file);
         }

   /*
    * Components instantiation
    */
   f->LOINS=(loins_list *)reverse((chain_list *)f->LOINS);
   for (i=f->LOINS; i; i=i->NEXT) {
      fprintf(file, "%s : %s\n", mkvhdlname(i->INSNAME), i->FIGNAME);
/*
 * Generic Map
 */
      {
      logen_list *g;
      ptype_list *p;
      loins_list *ins;

         for (m=mod; m; m=m->NEXT) {
            ins=(loins_list *)m->DATA;
            if (ins->FIGNAME == i->FIGNAME)
               break;
         }

         p=getptype(i->USER,LOGEN);
         if (p)  {
            fputs("   Generic Map (\n",file);
            for (g=p->DATA, maxlength=0; g; g=g->NEXT)
               if (maxlength<strlen(g->NAME))
                  maxlength=strlen(g->NAME);

            buf=(char *)mbkalloc((maxlength+1)*sizeof(char));
            for (g=p->DATA;g;g=g->NEXT) {
               length=strlen(g->NAME);
               strcpy(buf, g->NAME);
               for (j=length; j<maxlength; j++)
                  buf[j]=' ';
               buf[maxlength]='\0';

               strlength+=fprintf(file,"      %s => ",buf);
               first=1;
               dim=0;
               drv_genvalue(file, g);
               if (g->NEXT) {
                  fputs(",\n",file);
                  strlength=0;
               }
            }
            fputs("\n   )\n",file);
            strlength=0;
            mbkfree(buf);
         }
      }

/*
 * Port Map
 */
      p=getptype(i->USER, VEL_CON);
      if (p->DATA) {
         fputs("   port map (\n", file);
         if (!(mode&2)) {
            maxlength=0;
            for (c=(velocon *)p->DATA; c; c=c->NEXT) {
               if (maxlength<strlen(c->NAME))
                  maxlength=strlen(c->NAME);
            }
            buf=(char *)mbkalloc((maxlength+1)*sizeof(char));
         }
         for (c=(velocon *)p->DATA; c; c=c->NEXT) {
            if (mode&2) {
               m=c->VSIG;
               if (m->NEXT) {
                  long b, inc;
                  inc=(c->LEFT>c->RIGHT)? -1 : 1;
                  for (b=c->LEFT; m; m=m->NEXT, b+=inc) {
                     s=(velosig *)m->DATA;
                     if (s->LEFT==s->RIGHT) {
                        fprintf(file, "      %s(%ld) => ", c->NAME, b);
                        fputs(s->NAME, file);
                        if (s->LEFT!=-1)
                           fprintf(file, "(%ld)", s->LEFT);
                     } else {
                        fprintf(file, "      %s(%ld %s %ld) => ", c->NAME,
                              b, getvectordirection(c), b+s->RIGHT-s->LEFT);
                        fputs(s->NAME, file);
                        fprintf(file, "(%ld %s %ld)",
                                s->LEFT, getvectordirection(s), s->RIGHT);
                        b+=s->RIGHT-s->LEFT;
                     }
                     if (m->NEXT)
                        fputs(",\n",file);
                  }
               } else {
                  fprintf(file,"      %s => ", c->NAME);
                  s=(velosig *)m->DATA;
                  fputs(s->NAME, file);
                  if (s->TYPE=='V' || s->TYPE=='M') {
                     if (s->LEFT==s->RIGHT)
                        fprintf(file, "(%ld)", s->LEFT);
                     else
                        fprintf(file, "(%ld %s %ld)",
                                s->LEFT, getvectordirection(s), s->RIGHT);
                  }
               }
            } else {
               length=strlen(c->NAME);
               strcpy(buf, c->NAME);
               for (j=length; j<maxlength; j++)
                  buf[j]=' ';
               buf[maxlength]='\0';

               fprintf(file,"      %s => ", buf);
               for (m=c->VSIG; m; m=m->NEXT) {
                  s=(velosig *)m->DATA;
                  fputs(s->NAME, file);
                  if (s->TYPE=='V' || s->TYPE=='M') {
                     if (s->LEFT==s->RIGHT)
                        fprintf(file, "(%ld)", s->LEFT);
                     else
                        fprintf(file, "(%ld %s %ld)",
                                s->LEFT, getvectordirection(s), s->RIGHT);
                  }
                  if (m->NEXT)
                     fputc('&', file);
               }
            }
            if (c->NEXT)
               fputs(",\n", file);
         }
         fputs("\n   )", file);
         if (!(mode&2))
            mbkfree(buf);
      }
      fputs(";\n\n", file);
   }
   fputs("\nend structural;\n", file);

   if (mode&4) {
      fprintf(file, "configuration cfg_%s of %s is\n", f->NAME, f->NAME);
      fputs("for structural\n", file);
      for (i=f->LOINS; i; i=i->NEXT)
         fprintf(file, "for %s:%s use entity work.%s(%s); end for;\n",
                 i->INSNAME, i->FIGNAME, i->FIGNAME,
                 incatalog(i->FIGNAME)?"behavioural":"structural");
      fputs("end for;\n",file);
      fprintf(file, "end cfg_%s;\n",f->NAME);
   }

   f->LOINS=(loins_list *)reverse((chain_list *)f->LOINS);

   fclose(file);
}

void vhdlsavelofig(lofig_list *f)
{
   vhdlsavevelofig(f, 0);
}

/* ###--------------------------------------------------------------------### */
/* #   Function makevelofig                                                 # */
/* ###--------------------------------------------------------------------### */

void makevelofig(lofig_list *f)
{
ptype_list *p;
loins_list *i;
losig_list *s;
locon_list *c;
char       *cn;
char       *sn, *t=NULL, *r = NULL;
int first = 1, previous=0, delta=0, current=0;
char       Buffer0[1024];
int        sigi = 0;


   /* Ensure that connector and internal signal names are not
    * identical, and correct this prior to build the velofig.
    * This nice check is in O(nc * ns), because I don't feel like
    * building hash tables and all that stuff to speed up things.
    * I may have to if this is really awful */

   for (c = f->LOCON; c; c = c->NEXT) {
      cn = vectorradical(c->NAME);
      for (s = f->LOSIG; s; s = s->NEXT) {
         if (s->TYPE == INTERNAL) {
            t = getsigname(s);
            sn = vectorradical(t);
            if (!strcmp(cn, sn)) {
               char newname[1024];
               sprintf(newname, "%sfr%ded %d", sn, delta, vectorindex(t));
               fprintf(stdout, "External name conflicts with Internal name:"
                               " Changing %s into %s\n", t, newname);
               freechain(s->NAMECHAIN);
               s->NAMECHAIN = addchain(NULL, namealloc(newname));
               if (r && strcmp(r, sn)) {
                  r = sn;
                  delta++;
               }
            }
         }
      }
   }

   /* Computes currently max signal index and chose a correct name for
    * them */
   for (s = f->LOSIG; s; s = s->NEXT) {
      sigi = sigi > s->INDEX ? sigi : s->INDEX;
      if (s->TYPE == INTERNAL) {
         char *name = getsigname(s);
         int  i, t; /* This sucks and I'm not very proud of that, but really,
                    at least it should work */
         for (t = 0, i = 0; name[i] != 0; i++)
            if (name[i] == SEPAR) {
               name[i]='_'; /* Shaaaaaaaame on meeeeeeeeee */
               t = 1;
            }
         if (t) {
            freechain(s->NAMECHAIN);
            s->NAMECHAIN = addchain(NULL, name);
         }
      }
   }

   delta = 0;

   /* Check that internal signal vectors do not have holes, and if they
    * have holes, fill them.
    * TODO: Check also that two signals do not have the same name, as the
    * signal identifier is an index, not a name. */
   /* Code borrowed from checkloconorder, that I wrote so long ago */
   sortlosig(&f->LOSIG);
   s = f->LOSIG;
   while (s) {
      /* Don't have the right to modify the interface */
      if (s->TYPE == EXTERNAL) {
         s = s->NEXT;
         continue;
      }
      sn = getsigname(s);
      if ((r = strchr(sn, ' ')) == NULL) {
avoid_redundancy:
         first = 1;
         s = s->NEXT;
      } else {
         while (s) {
            int i = 1;
            /* redundancy :
               I'ven't found an elegant way to do that, too bad. */
            sn = getsigname(s);
            if (!r)
               if ((r = strchr(sn, ' ')) == NULL)
                  goto avoid_redundancy;
            /* Temporary change :
               alter the string contents just a microsecond. */
            *r = '\0';
            strcpy(Buffer0, sn);
            *r = ' ';
            while (*(r + i) && isdigit((int)*(r + i)))
               i++;
            if (*(r + i)) {
               (void)fflush(stdout);
               (void)fprintf(stderr, "*** mbk error ***\n");
               (void)fprintf(stderr,
                     "the radical %s has a spurious vectorized value %s (%s)\n",
                     Buffer0, r + 1, sn);
               EXIT(1);
            }
            current = atoi(r + 1);
            r = NULL;
            /* first :
               the name is not yet known. so I store it.
               we do that each time a new vector is encountered. */
            if (first) {
its_first:
               t = namealloc(Buffer0);
               delta = first = 0;
               previous = current;
               s = s->NEXT;
            } else { /* let's see if it's a good vhdl legal vector */
               /* its a new vector :
                  in fact this vector follows an other vector! */
               if (t != namealloc(Buffer0))
                  goto its_first;
               
               delta = previous - current;
               if (delta != 1 && delta != -1) {
                  if (delta < 0)
                     for (i = previous + 1; i < current; i++) {
                     char newname[1024];
                        sprintf(newname, "%s %d", t, i);
                        addlosig(f, ++sigi, addchain(NULL, namealloc(newname)),
                                 s->TYPE);
                        fprintf(stdout, "Adding signal '%s'\n", newname);
                     }
                  else
                     for (i = previous - 1; i > current; i--) {
                     char newname[1024];
                        sprintf(newname, "%s %d", t, i);
                        addlosig(f, ++sigi, addchain(NULL, namealloc(newname)),
                                 s->TYPE);
                        fprintf(stdout, "Adding signal '%s'\n", newname);
                     }
               }
               previous = current;
               s = s->NEXT;
            }
         }
      }
   }

   sortlosig(&f->LOSIG);

   buildvelo(f);

 //p=getptype(f->USER, VEL_SIG);
   vectorize_velosig(f);

   p=getptype(f->USER, VEL_CON);
   vectorize_velocon(f, p);

   for (i=f->LOINS; i; i=i->NEXT) {
      p=getptype(i->USER, VEL_CON);
      vectorize_velocon(f, p);
   }

   chkdir(f);

   if (getenv("VEL_DEBUG")) {
      debugvelocon(f);
      debugvelosig(f);
      debugveloins(f);
   }
}

/* ###--------------------------------------------------------------------### */
/* #   Function vhdlsavevelofig                                             # */
/* #   mode = 0 : it drives an alliance vhdl netlist (vst)                  # */
/* #   mode = 1 : it drives a vhdl netlist (vhd)                            # */
/* #   mode = 2 : it drives a vhdl netlist (vhd) adding signals             # */
/* #              to concatenate                                            # */
/* ###--------------------------------------------------------------------### */
static void cleanuplofig(lofig_list *f)
{
locon_list *c;
chain_list *n;

    for (c = f->LOCON; c != NULL; c = c->NEXT) {
        n = addchain(NULL, c->NAME);
        freechain(c->SIG->NAMECHAIN);
        c->SIG->NAMECHAIN = n;
    }
}

int vhdlsavevelofig(lofig_list *f, int mode)
{
ptype_list *p;
loins_list *i;
velocon *c;
velosig *s;
chain_list *ch;
long index, signalSize;

   cleanuplofig(f);
   makevelofig(f);

   p=getptype(f->USER, VEL_CON);
   for (c=(velocon *)p->DATA;c;c=c->NEXT) {
      c->NAME=mkvhdlname(c->NAME);
      if (c->LEFT!=-1) {
         if (c->LEFT>=c->RIGHT) {
            index=c->LEFT;
            for (ch=c->VSIG; ch; ch=ch->NEXT) {
               s=(velosig *)ch->DATA;
               signalSize=s->LEFT-s->RIGHT+1;
               s->NAME=c->NAME;
               s->LEFT=index;
               s->RIGHT=index-signalSize+1;
               s->TYPE='V';
               index-=signalSize;
            }
         } else {
            index=c->LEFT;
            for (ch=c->VSIG; ch; ch=ch->NEXT) {
               s=(velosig *)ch->DATA;
               signalSize=s->RIGHT-s->LEFT+1;
               s->NAME=c->NAME;
               s->LEFT=index;
               s->RIGHT=index+signalSize-1;
               s->TYPE='V';
               index+=signalSize;
            }
         }
      } else
         for (ch=c->VSIG; ch; ch=ch->NEXT) {
            s=(velosig *)ch->DATA;
            s->NAME=c->NAME;
            s->LEFT=-1;
            s->RIGHT=-1;
         }
   }

   for (i=f->LOINS; i; i=i->NEXT) {
      p=getptype(i->USER, VEL_CON);
      for (c=((velocon *)p->DATA);c;c=c->NEXT)
         c->NAME=mkvhdlname(c->NAME);
   }

   p=getptype(f->USER, VEL_SIG);
   for (s=((velosig *)p->DATA);s;s=s->NEXT)
      if (s->TYPE=='I' || s->TYPE=='S')
         s->NAME=mkvhdlname(s->NAME);

   p=getptype(f->USER, VEL_CON);
   for (c=((velocon *)p->DATA);c;c=c->NEXT)
      c->NAME=mkvhdlname(c->NAME);

   for (i=f->LOINS; i; i=i->NEXT) {
      p=getptype(i->USER, VEL_CON);
      for (c=((velocon *)p->DATA);c;c=c->NEXT)
         c->NAME=mkvhdlname(c->NAME);
   }


   if (getenv("VEL_DEBUG")) {
      debugvelocon(f);
      debugvelosig(f);
      debugveloins(f);
   }

   if (mode==1) {
      addsignals(f);
   }
   vhdsavevelofig(f, mode);

   return 0;
}


/* ###--------------------------------------------------------------------### */
/* # Debug function                                                         # */
/* ###--------------------------------------------------------------------### */

void debugvelocon (lofig_list *f)
{
ptype_list *p;

   puts("      External connectors:");
   p=getptype(f->USER, VEL_CON);
   printveloconlist(p->DATA);
   fputc('\n', stdout);
}
void debugvelosig(lofig_list *f)
{
ptype_list *p;

   puts("      Signals:");
   p=getptype(f->USER, VEL_SIG);
   printvelosiglist(p->DATA);
   fputc('\n', stdout);
}
void debugveloins(lofig_list *f)
{
loins_list *i;
ptype_list *p;

   for (i=f->LOINS; i; i=i->NEXT) {
      printf("      Instance %s:%s\n", i->INSNAME, i->FIGNAME);
      p=getptype(i->USER, VEL_CON);
      printveloconlist(p->DATA);
      fputc('\n', stdout);
   }
}
