/* ###--------------------------------------------------------------------### */
/* #                                                                        # */
/* # file : vel_vect.c                                                      # */
/* | date : Jun 03 1997                                                     | */
/* | version : 1.00                                                         | */
/* | author : Denis Hommais                                                 | */
/* |                                                                        | */
/* # contents : losig vectorization functions                               # */
/* #                                                                        # */
/* ###--------------------------------------------------------------------### */
/*
 * $Log: vel_vect.c,v $
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
 * Revision 1.5  1998/11/12 10:39:27  pwet
 * add makevelofig function.
 *
 * Revision 1.4  1998/01/06 09:54:50  denis
 * rewrite vectlosig function. The new function take care of
 * the good sequenciality of the vector.
 *
 * Revision 1.3  1997/12/17 16:41:33  denys
 * ..
 *
 * Revision 1.2  1997/09/23  09:43:38  denys
 * *** empty log message ***
 *
 * Revision 1.2  1997/09/23  09:43:38  denys
 * *** empty log message ***
 *
 */

#ident "$Id: vel_vect.c,v 1.1 2002/04/26 09:51:07 ludo Exp $"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>

extern void sortlosigv();

static char buffer[BUFSIZ];

#define Signame(sig) ((sig)->NAMECHAIN->DATA)
#define Delvectorname(s) (s)->NAMECHAIN=delchain((s)->NAMECHAIN,(s)->NAMECHAIN)


/* ###--------------------------------------------------------------------### */
/* #   Function addvectorname                                               # */
/* ###--------------------------------------------------------------------### */

static void addvectorname(losig_list *s)
{
char *sn, *en, *name;
   while (s) {
      name=getsigname(s);
      if ((sn=strrchr(name, '_'))) {
         strtol(sn+1, &en, 10);
         if (*en=='\0') {
            strcpy(buffer, name);
            buffer[sn-name]=' ';
            name=namealloc(buffer);
         }
      }
      s->NAMECHAIN=addchain(s->NAMECHAIN, name);
      s=s->NEXT;
   }
}

/* ###--------------------------------------------------------------------### */
/* #   Function delvectnames                                                # */
/* ###--------------------------------------------------------------------### */

static void delvectnames(chain_list *ch)
{
   losig_list *s;

   while (ch) {
      s=(losig_list *)ch->DATA;
      s->NAMECHAIN=delchain(s->NAMECHAIN, s->NAMECHAIN);
      ch=ch->NEXT;
   }
   freechain(ch);
}

/* ###--------------------------------------------------------------------### */
/* #   Function vectorelm                                                   # */
/* ###--------------------------------------------------------------------### */

static char *vectorelm(char *name, long *idx)
{
   char *sn;
   if ((sn=strchr(name, ' '))) {
      *idx=atoi(sn+1);
      strncpy(buffer, name, sn-name);
      buffer[sn-name]='\0';
      return namealloc(buffer);
   }
   *idx=-2;
   return name;
}


/* ###--------------------------------------------------------------------### */
/* #   Function chkvectors                                                  # */
/* ###--------------------------------------------------------------------### */

static void chkvectors(losig_list *s)
{
   chain_list *vchain=NULL;
   char *cur_name=NULL, *new_name;
   long cur_index, new_index;

   while(s) {
      new_name=vectorelm(Signame(s), &new_index);
      if (s->TYPE=='E') {
         if (new_name==cur_name)            /* the vector has the same name */
            delvectnames(vchain);           /* than an external signal      */
         else {
            if (vchain && vchain->NEXT)
               freechain(vchain);
            else
               delvectnames(vchain);
            cur_name=new_name;
         }
         Delvectorname(s);
         vchain=NULL;
      } else
         if (!vchain)
            if (new_name==cur_name)
               Delvectorname(s);
            else {
               cur_name=new_name;
               cur_index=new_index;
               vchain=addchain(NULL, s);
            }
         else {
            if (new_name==cur_name) {
               if (new_index-cur_index!=1) { /* the name are the same but    */
                  delvectnames(vchain);      /* the index aren't consecutive */
                  vchain=NULL;
                  Delvectorname(s);
               } else {                      /* good vector element          */
                  cur_index=new_index;
                  vchain=addchain(vchain, s);
               }
            } else {                         /* next vector name, if the     */
               if (vchain && vchain->NEXT)   /* previous is valid and the    */
                  freechain(vchain);         /* vector have more than one    */
               else                          /* element, a vector is found.  */
                  delvectnames(vchain);
               cur_name=new_name;
               cur_index=new_index;
               vchain=addchain(NULL, s);
            }
         }
      s=s->NEXT;
   }
   if (vchain && vchain->NEXT)
      freechain(vchain);
   else
      delvectnames(vchain);
}

/* ###--------------------------------------------------------------------### */
/* #   Function vectlosig                                                   # */
/* ###--------------------------------------------------------------------### */

void vectlosig(losig_list **s)
{
   addvectorname(*s);
   sortlosigv(s);
   chkvectors(*s);
}


