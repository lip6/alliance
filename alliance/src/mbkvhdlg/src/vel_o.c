/* ###--------------------------------------------------------------------### */
/* #                                                                        # */
/* # file : velo.c                                                          # */
/* | date : Jun 03 1997                                                     | */
/* | version : 1.00                                                         | */
/* | author : Denis Hommais                                                 | */
/* |                                                                        | */
/* # contents : basical functions to used VEL structures                    # */
/* #                                                                        # */
/* ###--------------------------------------------------------------------### */
/*
 * $Log: vel_o.c,v $
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
 * Revision 1.1.1.1  2001/07/19 14:32:19  pwet
 * New cvs tree
 *
 * Revision 1.1  1997/12/10 16:40:22  denys
 * Initial revision
 *
 * Revision 1.2  1997/09/23  09:43:48  denys
 * *** empty log message ***
 *
 * Revision 1.2  1997/09/23  09:43:48  denys
 * *** empty log message ***
 *
 */

#ident "$Id: vel_o.c,v 1.1 2002/04/26 09:51:07 ludo Exp $"


#include <stdio.h>
#include <unistd.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include "vel_velo.h"

/* ###--------------------------------------------------------------------### */
/* #   Function addvelosig                                                  # */
/* ###--------------------------------------------------------------------### */

velosig *addvelosig (ptype_list *tsig, char *name,
                     long left, long right, char type)
{
velosig *s;

   name = namealloc(name);
   s = (velosig *)mbkalloc(sizeof(velosig));
   s->NEXT=(velosig *)tsig->DATA;
   s->NAME=name;
   s->LEFT=left;
   s->RIGHT=right;
   s->VSIG=NULL;
   s->NAMECHAIN=NULL;
   s->USER=NULL;
   s->TYPE=type;
   s->SUPER=NULL;
   tsig->DATA=s;
   return s;
}

/* ###--------------------------------------------------------------------### */
/* #   Function addvelocon                                                  # */
/* ###--------------------------------------------------------------------### */

velocon *addvelocon (ptype_list *tcon, char *name, long left, long right)
{
velocon *c;

   name = namealloc(name);
   c=(velocon *)mbkalloc(sizeof(velocon));
   c->NEXT=(velocon *)tcon->DATA;
   c->NAME=name;
   c->LEFT=left;
   c->RIGHT=right;
   c->VSIG=NULL;
   c->ROOT=NULL;
   c->USER=NULL;
   tcon->DATA=c;
   return c;
}

/* ###--------------------------------------------------------------------### */
/* #   Function dupvelosig                                                  # */
/* ###--------------------------------------------------------------------### */

velosig *dupvelosig(ptype_list *p, velosig *s)
{
velosig *d;

   d=(velosig *)mbkalloc(sizeof(velosig));
   memcpy(d,s,sizeof(velosig));
   d->NEXT=(velosig *)p->DATA;
   p->DATA=d;

   return d;
}

/* ###--------------------------------------------------------------------### */
/* #   Function getvelosig                                                  # */
/* ###--------------------------------------------------------------------### */

velosig *getvelosig(ptype_list *p, char *name, long left, long right)
{
velosig *s;

   for (s=(velosig *)p->DATA; s; s=s->NEXT) {
      if (s->NAME == name && ( s->LEFT == left && s->RIGHT == right))
         return s;
   }
   return NULL;
}

/* ###--------------------------------------------------------------------### */
/* #   Function delvelocon                                                  # */
/* ###--------------------------------------------------------------------### */

int delvelocon(ptype_list *p, velocon *d)
{
   velocon *c, *s;

   for (c=(velocon *)p->DATA; c; c=c->NEXT) {
      if (c==d)
         break;
      s=c;
   }
   if (!c)
      return 0;
   if (c==p->DATA)
      p->DATA=c->NEXT;
   else
      s->NEXT=c->NEXT;
   mbkfree(c);

   return 1;
}

/* ###--------------------------------------------------------------------### */
/* #   Function delvelosig                                                  # */
/* ###--------------------------------------------------------------------### */

int delvelosig(ptype_list *p, velosig *d)
{
   velosig *c, *s;

   for (c=(velosig *)p->DATA; c; c=c->NEXT) {
      if (c==d)
         break;
      s=c;
   }
   if (!c)
      return 0;
   if (c==p->DATA)
      p->DATA=c->NEXT;
   else
      s->NEXT=c->NEXT;
   mbkfree(c);

   return 1;
}
