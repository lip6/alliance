/*
   file     : generic.c
   date     : Mar 7 1997
   version  : v2.00
   author   : D.HOMMAIS
   content  : basic functions for logen structure use
*/
#ident "$Id: gen_generic.c,v 1.1 2002/04/26 09:51:07 ludo Exp $"
#include <stdio.h>
#include <mut.h>
#include "gen_generic.h"

logen_list *addlogen(logen_list *ptgen,char *name)
{
logen_list *p;
   
   if (name) {
      name=namealloc(name);
      for (p=ptgen;p;p=p->NEXT) {
         if (name==p->NAME) {
            fprintf(stderr,"Generic %s already exists\n",name);
            return p;
         }
      }
   }
   p=(logen_list *)mbkalloc(sizeof(logen_list));
   p->NAME=name;
   p->TYPE=0;
   p->TAG=0;
   p->NEXT=ptgen;
   return p;
}

logen_list *getlogen(logen_list *ptgen, char *name)
{
   name=namealloc(name);
   while (ptgen) {
      if (ptgen->NAME==name)
         return ptgen;
      ptgen=ptgen->NEXT;
   }
   return NULL;
}

logen_list *duplogen(logen_list *s, logen_list *d, char *name)
{
   d=addlogen(d,name);
   d->TYPE=s->TYPE;
   d->VALUE=s->VALUE;
   return d;
}

logen_list *dellogen(logen_list *g, char *name)
{
logen_list *s=NULL;
logen_list *p;
   
   for (p=g;p;p=p->NEXT) {
      if (p->NAME==name)
         break;
      s=p;
   }
   if (!p) return g;
   if (!s)
      g=p->NEXT;
   else
      s->NEXT=p->NEXT;
   mbkfree(p);
   return g;
}

logen_list *getprevlogen(logen_list *ptgen, char *name)
{
logen_list *prev=NULL;

   while (ptgen) {
      if (ptgen->NAME==name)
         return prev;
      prev=ptgen;
      ptgen=ptgen->NEXT;
   }
   return NULL;
}

logen_list *logenorder(logen_list *ins, logen_list *mod)
{
logen_list *save, *prev, *ord=NULL;

   while(mod) {
      prev=getprevlogen(ins,mod->NAME);
      save=ord;
      if (prev) {
         ord=prev->NEXT;
         prev->NEXT=ord->NEXT;
         ord->NEXT=save;
      } else {
         ord=ins;
         ins=ins->NEXT;
         ord->NEXT=save;
      }
      mod=mod->NEXT;
   }
   return ord;
}

logen_list *duplogenlist(logen_list *s, logen_list *d, char *name)
{
   logen_list *g;
   
   d=addlogen(d,name);
   d->TYPE=s->TYPE;
   d->VALUE.LIST=NULL;
   if (s->TYPE==GENTYPE_LIST) {
      for (g=s->VALUE.LIST; g; g=g->NEXT)
         d->VALUE.LIST=duplogenlist(g, d->VALUE.LIST, NULL);
      d->VALUE.LIST=(logen_list *)reverse((chain_list *)d->VALUE.LIST);
   } else
      d->VALUE=s->VALUE;

   return d;
}

logen_list *chklogen(logen_list *ins, logen_list *mod, char *insname, char *modname)
{
logen_list *pins, *pmod;

   if (ins) {
      if (ins->NAME) {
         for (pins=ins;pins;pins=pins->NEXT)
            if (pmod=getlogen(mod,pins->NAME))
               pmod->TAG=1;
            else {
               fprintf(stderr,
                   "Instance %s, no generic called %s in model %s, deleted!\n",
                       insname, pins->NAME, modname);
               ins=dellogen(ins, pins->NAME);
            }
         for (pmod=mod;pmod;pmod=pmod->NEXT)
            if (!pmod->TAG)
               ins=duplogenlist(pmod, ins, pmod->NAME);
      } else {
         for (pins=ins,pmod=mod;pins&&pmod;pins=pins->NEXT,pmod=pmod->NEXT)
            pins->NAME=pmod->NAME;
         if (pins) {
            fprintf(stderr,
                    "Too many value in generic map of instance %s, exiting !\n",
                    insname);
            exit(4);
         }
         if (pmod) {
            fprintf(stderr,
                    "Too few value in generic map of instance %s, exiting !\n",
                    insname);
            exit(5);
         }
      }
      for (pmod=mod;pmod;pmod=pmod->NEXT) pmod->TAG=0;
   } else
      for (pmod=mod;pmod;pmod=pmod->NEXT)
         ins=duplogenlist(pmod,ins,pmod->NAME);

   return logenorder(ins,mod);
}
