#ident "$Id: vel_chain.c,v 1.1 2002/04/26 09:51:07 ludo Exp $"
#include <stdio.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include "vel_velo.h"

void debugChain(lofig_list *f) 
{
   ptype_list *p;
   velosig *vs;
   velocon *vc;
   chain_list *ch;

   p=getptype(f->USER, VEL_SIG);
   for (vs=(velosig *)p->DATA; vs; vs=vs->NEXT) {
      printf("+-%s[%ld:%ld]\n", vs->NAME, vs->LEFT, vs->RIGHT);
      if (vs->TYPE=='M')
         printf("    <%s[%ld:%ld]>\n", vs->SUPER->NAME,
                                      vs->SUPER->LEFT, vs->SUPER->RIGHT);
      p=getptype(vs->USER, VEL_CHAIN);
      for (ch=(chain_list *)p->DATA; ch; ch=ch->NEXT) {
         vc=(velocon *)ch->DATA;
         printf("   *-%s[%ld:%ld]\n", vc->NAME, vc->LEFT, vc->RIGHT);
      }
   }
}

void velofigChain(lofig_list *f)
{
velocon *vc;
velosig *vs;
ptype_list *p;
loins_list *i;
chain_list *c;

   p=getptype(f->USER, VEL_SIG);
   if (!p) {
      fprintf(stderr, "No velosig found!");
   }
   for (vs=(velosig *)p->DATA; vs; vs=vs->NEXT) {
      p=getptype(vs->USER, VEL_CHAIN);
      if (p != NULL) {
         freechain((chain_list *)p->DATA);
         p->DATA=NULL;
      } else
         vs->USER=addptype(vs->USER, VEL_CHAIN, NULL);
   }

   p=getptype(f->USER, VEL_CON);
   if (p!=NULL) {
      for (vc=(velocon *)p->DATA; vc; vc=vc->NEXT) {
         for (c=vc->VSIG; c; c=c->NEXT) {
            vs=(velosig *)c->DATA;
            p=getptype(vs->USER, VEL_CHAIN);
            p->DATA=addchain(p->DATA, vc);
            if (vs->TYPE=='M') {
               p=getptype(vs->SUPER->USER, VEL_CHAIN);
               p->DATA=addchain(p->DATA, vc);
            }
         }
      }
   }
   for (i=f->LOINS; i; i=i->NEXT) {
      p=getptype(i->USER, VEL_CON);
      if (p!=NULL) {
         for (vc=(velocon *)p->DATA; vc; vc=vc->NEXT) {
            for (c=vc->VSIG; c; c=c->NEXT) {
               vs=(velosig *)c->DATA;
               p=getptype(vs->USER, VEL_CHAIN);
               p->DATA=addchain(p->DATA, vc);
               if (vs->TYPE=='M') {
                  p=getptype(vs->SUPER->USER, VEL_CHAIN);
                  p->DATA=addchain(p->DATA, vc);
               }
            }
         }
      }
   }

   if (getenv("VEL_DEBUG"))
      debugChain(f);
}

