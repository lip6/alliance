/* ###--------------------------------------------------------------------### */
/* #                                                                        # */
/* # file : vel_util.c                                                      # */
/* | date : Jun 03 1997                                                     | */
/* | version : 1.00                                                         | */
/* | author : Denis Hommais                                                 | */
/* |                                                                        | */
/* # contents : netlist vectorization function using VEL structures         # */
/* #                                                                        # */
/* ###--------------------------------------------------------------------### */
/*
 * $Log: vel_util.c,v $
 * Revision 1.1  2002/04/26 09:51:07  ludo
 * Initial revision
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
 * Revision 1.7  1998/11/12 10:39:26  pwet
 * add makevelofig function.
 *
 * Revision 1.6  1998/01/06 09:54:27  denis
 * change strdup to namealloc in mkvhdlname
 *
 * Revision 1.5  1997/12/17 16:41:33  denys
 * add chkdir to check signal direction
 *
 * Revision 1.4  1997/12/10 10:43:05  denys
 * Don't know
 *
 * Revision 1.3  1997/09/24  09:46:48  denys
 * rename the signal when it is connected to a external connector by the
 * name of the connector
 *
 * Revision 1.2  1997/09/23  09:42:46  denys
 * add reverse in mkvector
 * change mkvector to verify the vector
 *
 */
#ident "$Id: vel_util.c,v 1.1 2002/04/26 09:51:07 ludo Exp $"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include "vel_velo.h"

static int signalindex;
static char buf[BUFSIZ];

static void printvelosig(velosig *, char *);

/* ###--------------------------------------------------------------------### */
/* #   Function buildvelo                                                   # */
/* ###--------------------------------------------------------------------### */
void buildvelo(lofig_list *f)
{
losig_list *s;
locon_list *c;
loins_list *i;
velosig    *vs;
velocon    *vc;
ptype_list *ps, *pc;
char *name;
long idx;

   ps=f->USER=addptype(f->USER, VEL_SIG, NULL);
   for (s=f->LOSIG;s;s=s->NEXT) {
      name=getsigname(s);
      idx=vectorindex(name);
      vs=addvelosig(ps, vectorradical(name), idx, idx, s->TYPE);
      vs->NAMECHAIN=s->NAMECHAIN;
      s->USER=addptype(s->USER, VEL_SIG, vs);
   }
   pc=f->USER=addptype(f->USER, VEL_CON, NULL);
   for (c=f->LOCON;c;c=c->NEXT) {
      idx=vectorindex(c->NAME);
      vc=addvelocon(pc, vectorradical(c->NAME), idx, idx);
      vc->ROOT=addchain(NULL,c);
      ps=getptype(c->SIG->USER, VEL_SIG);
      vc->VSIG=addchain(NULL, ps->DATA);
   }
   for (i=f->LOINS;i;i=i->NEXT) {
      pc=i->USER=addptype(i->USER, VEL_CON, NULL);
      for (c=i->LOCON;c;c=c->NEXT) {
         idx=vectorindex(c->NAME);
         vc=addvelocon(pc, vectorradical(c->NAME), idx, idx);
         vc->ROOT=addchain(NULL,c);
         ps=getptype(c->SIG->USER, VEL_SIG);
         vc->VSIG=addchain(NULL, ps->DATA);
      }
   }
}

/* ###--------------------------------------------------------------------### */
/* #   Function vectorize_velosig                                           # */
/* ###--------------------------------------------------------------------### */

void vectorize_velosig(lofig_list *f)
{
ptype_list *p;
velosig *vs, *vv=NULL;
chain_list *ch;

   p=getptype(f->USER, VEL_SIG);
   for (vs=(velosig *)p->DATA;vs;vs=vs->NEXT) {
      if (vv && vv->NAME==vs->NAME && abs(vv->RIGHT-vs->RIGHT)==1 ) {
         vv->RIGHT=vs->RIGHT;
         vv->VSIG=addchain(vv->VSIG, vs);
         vs->TYPE='V';
         vs->SUPER=vv;
      } else {
         if (vv) {
            if (vs->NAME==vv->NAME) {
               /* the vector isn't a valid one */
               for (ch=vv->VSIG; ch; ch=ch->NEXT)
                  ((velosig *)ch->DATA)->TYPE=vv->TYPE;
            } else
               vv->VSIG=(chain_list *)reverse(vv->VSIG);
            vv=NULL;
         }
         if (vs->LEFT!=-1) {
            vv=dupvelosig(p, vs);
            vv->VSIG=addchain(vv->VSIG, vs);
            vs->TYPE='V';
            vs->SUPER=vv;
         }
      }
   }
   if (vv)
      vv->VSIG=(chain_list *)reverse(vv->VSIG);
}

#if 0
void vectorize_velosig(lofig_list *f)
{
ptype_list *p;
velosig *vs, vv, *vm;
chain_list *ch;

   vv.NAME=NULL;
   p=getptype(f->USER, VEL_SIG);
   for (vs=(velosig *)p->DATA;vs;vs=vs->NEXT) {
      if (vv.NAME && vv.NAME==vs->NAME) {
         vv.RIGHT=vs->RIGHT;
         vv.VSIG=addchain(vv.VSIG, vs);
         vs->TYPE='V';
      } else {
         if (vv.NAME) {
            for(ch=getvelosigbyname(p,vv.NAME); ch; ch=ch->NEXT) {
               vm=(velosig *)ch->DATA;
               if ((vm->RIGHT==vv.RIGHT && vm->LEFT == vv.LEFT)
                     || (vm->RIGHT==vv.LEFT && vm->LEFT == vv.RIGHT)) {
                  vm->TYPE=vv.TYPE;
                  vm->VSIG=(chain_list *)reverse(vv.VSIG);
               }
            }
            vv.VSIG=(chain_list *)reverse(vv.VSIG);
            vv.NAME=NULL;
         }
         if (vs->LEFT!=-1) {
            memcpy(&vv,vs,sizeof(velosig));
            vv.VSIG=addchain(vv.VSIG, vs);
            vs->TYPE='V';
         }
      }
   }
   if (vv.NAME)
      vv.VSIG=(chain_list *)reverse(vv.VSIG);
}
#endif

/* ###--------------------------------------------------------------------### */
/* #   Function mkvector                                                    # */
/* ###--------------------------------------------------------------------### */

static chain_list *mkvector(lofig_list *f, chain_list *c)
{
velosig *s, *e;
char *name=NULL;
long left, right;
char type;
chain_list *r=NULL, *sigchain;
ptype_list *ps;

   ps=getptype(f->USER, VEL_SIG);
   while(c) {
      s=(velosig *)c->DATA;
      if (name && s->NAME == name && abs(s->RIGHT-right)==1 ) {
         right=s->RIGHT;
         sigchain=addchain(sigchain, s);
      } else {
         if (name) {
            if (!(e=getvelosig(ps, name, left, right))) {
               e=addvelosig(ps, name, left, right, 'M');
               e->VSIG=reverse(sigchain);
               e->SUPER=getvelosigbyname(ps, name);
            }
            r=addchain(r,e);
         }
         name=s->NAME;
         left=s->LEFT;
         right=s->RIGHT;
         type=s->TYPE;
         sigchain=addchain(NULL, s);
      }
      c=c->NEXT;
   }
   if (!(e=getvelosig(ps, name, left, right))) {
      e=addvelosig(ps, name, left, right, 'M');
      e->VSIG=reverse(sigchain);
      e->SUPER=getvelosigbyname(ps, name);
   }
   r=addchain(r,e);

   return reverse(r);
}

/* ###--------------------------------------------------------------------### */
/* #   Function vectorize_velocon                                           # */
/* ###--------------------------------------------------------------------### */

void vectorize_velocon(lofig_list *f, ptype_list *p)
{
velocon *vc, *vv=NULL;

   for (vc=(velocon *)p->DATA;vc;) {
      if (vv && vv->NAME==vc->NAME) {
         vv->RIGHT=vc->RIGHT;
         vc->VSIG->NEXT=vv->VSIG;
         vv->VSIG=vc->VSIG;
         delvelocon(p, vc);
         vc=vv->NEXT;
      } else {
         if (vv) {
            vv->VSIG=(chain_list *)reverse(vv->VSIG);
            vv->VSIG=mkvector(f, vv->VSIG);
            vv=NULL;
         }
         if (vc->LEFT!=-1) {
            vv=vc;
         }
         vc=vc->NEXT;
      }
   }
   if (vv) {
      vv->VSIG=(chain_list *)reverse(vv->VSIG);
      vv->VSIG=mkvector(f, vv->VSIG);
   }
}

/* ###--------------------------------------------------------------------### */
/* #   Function sigcmp                                                      # */
/* ###--------------------------------------------------------------------### */

static velosig *sigcmp(ptype_list *p, chain_list *c)
{
velosig *s;
chain_list *ms, *mc;

   for(s=(velosig *)p->DATA; s; s=s->NEXT)
      if (s->TYPE=='S') {
         for (ms=s->VSIG, mc=c; ms&&mc; ms=ms->NEXT, mc=mc->NEXT)
            if (ms->DATA != mc->DATA) break;
         if (!ms && !mc)
            return s;
      }
   return NULL;
}

/* ###--------------------------------------------------------------------### */
/* #   Function addsignals                                                  # */
/* ###--------------------------------------------------------------------### */

void addsignals(lofig_list *f)
{
velocon *c;
velosig *s;
loins_list *i;
ptype_list *p, *ps;

   ps=getptype(f->USER, VEL_SIG);
   for (i=f->LOINS; i; i=i->NEXT) {
      p=getptype(i->USER, VEL_CON);
      for (c=(velocon *)p->DATA; c; c=c->NEXT) {
         if (c->VSIG->NEXT!=NULL) {
            if (!(s=sigcmp(ps,c->VSIG))) {
               sprintf(buf, "velo%d%s", signalindex++, c->NAME);
               s=addvelosig(ps, buf, c->LEFT, c->RIGHT, 'S');
               s->VSIG=c->VSIG;
            }
            c->VSIG=addchain(NULL, s);
            c->VSIG=reverse(c->VSIG);
         }
      }
   }

}

/* ###--------------------------------------------------------------------### */
/* #   Function getvelosigbyname                                            # */
/* ###--------------------------------------------------------------------### */

velosig *getvelosigbyname(ptype_list *p, char *name)
{
velosig *vs;

   for (vs=(velosig *)p->DATA; vs; vs=vs->NEXT)
      if (vs->NAME==name && (vs->TYPE=='E' || vs->TYPE=='I' || vs->TYPE=='R'))
         return vs;

   return NULL;
}

/* ###--------------------------------------------------------------------### */
/* #   Function getdir                                                      # */
/* ###--------------------------------------------------------------------### */

static int getdir (velosig *vs)
{
long dir;

   dir=vs->RIGHT-vs->LEFT;

   if (dir!=0)
      if (dir<0)
         return -1;
      else
         return 1;

   return 0;
}

/* ###--------------------------------------------------------------------### */
/* #   Function chkdir                                                      # */
/* ###--------------------------------------------------------------------### */

void chkdir (lofig_list *f)
{
loins_list *ins;
ptype_list *ps;
ptype_list *pc;
velocon *c;
chain_list *ch, **prevch;
velosig *vs, *vr;
int vsdir,vrdir,i;

   ps=getptype(f->USER, VEL_SIG);
   pc=getptype(f->USER, VEL_CON);
   for (c=(velocon *)pc->DATA; c; c=c->NEXT) {
      prevch=&(c->VSIG);
      for (ch=c->VSIG; ch; prevch=&(ch->NEXT), ch=ch->NEXT) {
         vs=(velosig *)ch->DATA;
         if (vs->TYPE=='M' && (vr=getvelosigbyname(ps, vs->NAME))) {
            vsdir=getdir(vs);
            vrdir=getdir(vr);
            if (vsdir && vrdir && vsdir!=vrdir)
               if (vr->LEFT==vs->RIGHT && vs->LEFT==vr->RIGHT) {
                  vs->TYPE=vr->TYPE;
                  vr->TYPE='V';
               }
         }
      }
   }

   for (ins=f->LOINS; ins; ins=ins->NEXT) {
      pc=getptype(ins->USER, VEL_CON);
      for (c=(velocon *)pc->DATA; c; c=c->NEXT) {
         prevch=&(c->VSIG);
         for (ch=c->VSIG; ch; prevch=&(ch->NEXT), ch=ch->NEXT) {
            vs=(velosig *)ch->DATA;
            if (vs->TYPE=='M' && (vr=getvelosigbyname(ps, vs->NAME))) {
               vsdir=getdir(vs);
               vrdir=getdir(vr);
               if (vsdir && vrdir && vsdir!=vrdir)
                  if (vr->TYPE=='I') {
                     if (vr->LEFT==vs->RIGHT && vs->LEFT==vr->RIGHT) {
                        vs->TYPE='R';
                        vs->VSIG=vr->VSIG;
                     } else {
                        addvelosig(ps, vr->NAME, vr->RIGHT, vr->LEFT, 'R');
                     }
                     vr->TYPE='V';
                  } else {
                     printvelosig(vs,">>>");
                     ch->DATA=getvelosig(ps, vs->NAME, vs->RIGHT, vs->RIGHT);
                     printvelosig(ch->DATA,">>>");
                     if (vsdir<0)
                        for (i=vs->RIGHT+1; i<=vs->LEFT; i=i+1)
                           *prevch=addchain(*prevch,
                                            getvelosig(ps, vs->NAME,i,i));
                     else
                        for (i=vs->RIGHT-1; i>=vs->LEFT; i=i-1)
                           *prevch=addchain(*prevch,
                                            getvelosig(ps, vs->NAME,i,i));
                  }
               else
                  if (vsdir && vr->TYPE=='I')
                     vr->TYPE='R';
            }
         }
      }
   }
}

/* ###--------------------------------------------------------------------### */
/* #   Function addoffset                                                   # */
/* ###--------------------------------------------------------------------### */

static char *addoffset(char *offset, char *add)
{
char *no;

   no=(char *)mbkalloc(sizeof(char)*(strlen(offset)+strlen(add)+1));
   strcpy(no,offset);
   strcat(no,add);

   return no;
}

/* ###--------------------------------------------------------------------### */
/* #   Function mkvhdlname                                                  # */
/* ###--------------------------------------------------------------------### */

char *mkvhdlname(char *name)
{
char *n, *c;
int  f=0;


   c=buf;
   if (!isalpha(*name)) {
      sprintf(c,"v_", signalindex++);
      c+=2;
      f=1;
   }

   for (n=name; *n; n++)
      if (!isalnum(*n)) {
         if (*n!='_') {
            if (c!=buf && *(c-1)=='_')
               *c++='x';
            *c++='_';
            f=1;
         } else {
            if (c!=buf && *(c-1)=='_') {
               *c++='x';
               f=1;
            } else
               *c++=*n;
         }
      } else
         *c++=*n;


   if (f) {
      *c='\0';
      while (namefind(buf))
         sprintf(buf, "d%s", buf);
      return namealloc(buf);
   } else {
      return name;
   }
}

/* ###--------------------------------------------------------------------### */
/* #   Function printvelosig                                                # */
/* ###--------------------------------------------------------------------### */

static void printvelosig(velosig *s, char *offset)
{
chain_list *c;
char *no;

   printf("%s +-%s[%ld:%ld] %c\n", offset, s->NAME, s->LEFT, s->RIGHT, s->TYPE);
   /* printf("%s +-%s[%8x][%ld:%ld] %c\n", offset, s->NAME, s, s->LEFT, s->RIGHT, s->TYPE);
   */
   no=addoffset(offset, " |");
   for (c=s->VSIG;c;c=c->NEXT)
      printvelosig(c->DATA, no);
   mbkfree(no);
}

/* ###--------------------------------------------------------------------### */
/* #   Function printvelosiglist                                            # */
/* ###--------------------------------------------------------------------### */

void printvelosiglist(velosig *s)
{
   while (s) {
      printvelosig(s,"         ");
      s=s->NEXT;
   }
}

/* ###--------------------------------------------------------------------### */
/* #   Function printvelocon                                                # */
/* ###--------------------------------------------------------------------### */

static void printvelocon(velocon *s, char *offset)
{
chain_list *c;
char *no;

   printf("%s *-%s[%ld:%ld]\n", offset, s->NAME, s->LEFT, s->RIGHT);
   /* printf("%s *-%s[%8x][%ld:%ld]\n", offset, s->NAME, s, s->LEFT, s->RIGHT);
    */
   no=addoffset(offset, " |");
   for (c=s->VSIG;c;c=c->NEXT)
      printvelosig(c->DATA, no);
   mbkfree(no);
}

/* ###--------------------------------------------------------------------### */
/* #   Function printveloconlist                                            # */
/* ###--------------------------------------------------------------------### */

void printveloconlist(velocon *c)
{
   while (c) {
      printvelocon(c,"         ");
      c=c->NEXT;
   }
}
