/*
 * $Log: gen_print.c,v $
 * Revision 1.2  2002/04/26 12:54:30  ludo
 * Hi
 *
 * Revision 1.1.1.1  2002/04/26 09:51:07  ludo
 * Mise a plat de mbkvhdlg
 *
 * Revision 1.1.1.1  2002/04/11 08:54:52  fred
 * Importing mbkvhdlg into the new Alliance CVS tree
 *
 * Revision 1.1.1.1  2002/02/28 12:58:31  disydent
 * Creation of Disydent CVS Tree
 *
 * Revision 1.1.1.1  2001/11/19 16:55:24  pwet
 * Changing the CVS tree structure of disydent
 *
 * Revision 1.1.1.1  2001/07/24 13:31:38  pwet
 * cvs tree of part of disydent
 *
 * Revision 1.1.1.1  2001/07/19 14:32:18  pwet
 * New cvs tree
 *
 * Revision 1.4  1998/06/22 07:57:49  pwet
 * Modify the stdin parser to handle multi dimensional array
 *
 * Revision 1.3  1998/02/17 13:23:31  pwet
 * add multidimensional array feature for generics.
 *
 * Revision 1.2  1998/01/26 12:57:25  pwet
 * add PrintInstances, TestInstances and RessourcesInstances that depend on the
 * netlist.
 *
 * Revision 1.1  1997/09/08 16:59:49  pwet
 * Initial revision
 *
 */

#ident "$Id: gen_print.c,v 1.2 2002/04/26 12:54:30 ludo Exp $"
#include <stdio.h>
#include <string.h>
#include <mut.h>
#include "gen_generic.h"
#define MEALY 0x80

void debuglogenlist(logen_list *g, int level)
{
int i;

   while (g) {
      for (i=0;i<level;i++) putc('\t',stdout);
      switch(g->TYPE) {
         case GENTYPE_EMPTY:
            fprintf(stdout,"   EMPTY\n");
            break;
         case GENTYPE_VAL:
            fprintf(stdout,"   ( val) %ld\n",g->VALUE.VAL);
            break;
         case GENTYPE_TEXT:
            fprintf(stdout,"   (text) %s\n",g->VALUE.TEXT);
            break;
         case GENTYPE_LIST:
            fprintf(stdout,"   (list)\n");
            debuglogenlist(g->VALUE.LIST, level+1);
            break;
         case GENTYPE_ARG:
            fprintf(stdout,"   ( arg) %ld\n",g->VALUE.VAL);
            break;
         case GENTYPE_BIT:
            fprintf(stdout,"   ( bit) %c\n",g->VALUE.CHAR);
            break;
         case GENTYPE_HEX:
            fprintf(stdout,"   ( hex) 0x%lx\n", g->VALUE.VAL);
            break;
         default:
            fprintf(stdout,"Unknown logen type %d\n", g->TYPE);
            break;
      }
      g=g->NEXT;
   }
}

static void printlogen(logen_list *g, int level)
{
int i;

   for (i=0;i<level;i++) putc('\t',stdout);
   switch(g->TYPE) {
      case GENTYPE_EMPTY:
         if (g->NAME)
            fprintf(stdout,"   %s := EMPTY\n",g->NAME);
         else
            fprintf(stdout,"   EMPTY\n");
         break;
      case GENTYPE_VAL:
         if (g->NAME)
            fprintf(stdout,"   %s := ( val) %ld\n", g->NAME, g->VALUE.VAL);
         else
            fprintf(stdout,"   ( val) %ld\n", g->VALUE.VAL);
         break;
      case GENTYPE_TEXT:
         if (g->NAME)
            fprintf(stdout,"   %s := (text) %s\n", g->NAME, g->VALUE.TEXT);
         else
            fprintf(stdout,"   (text) %s\n", g->VALUE.TEXT);
         break;
      case GENTYPE_LIST:
         if (g->NAME)
            fprintf(stdout,"   %s := (list)\n",g->NAME);
         else
            fprintf(stdout,"   (list)\n");
         debuglogenlist(g->VALUE.LIST, level+1);
         break;
      case GENTYPE_ARG:
         if (g->NAME)
            fprintf(stdout,"   %s := ( arg) %ld\n", g->NAME, g->VALUE.VAL);
         else
            fprintf(stdout,"   ( arg) %ld\n", g->VALUE.VAL);
         break;
      case GENTYPE_BIT:
         if (g->NAME)
            fprintf(stdout,"   %s := ( bit) %c\n", g->NAME, g->VALUE.CHAR);
         else
            fprintf(stdout,"   ( bit) %c\n", g->VALUE.CHAR);
         break;
      case GENTYPE_HEX:
         if (g->NAME)
            fprintf(stdout,"   %s := ( hex) 0x%lx\n", g->NAME, g->VALUE.VAL);
         else
            fprintf(stdout,"   ( hex) 0x%lx\n", g->VALUE.VAL);
         break;
      default:
         fprintf(stdout,"Unknown logen type %d\n", g->TYPE);
   }
}

void debuglogen(logen_list *g, int level)
{
   while (g) {
      printlogen(g, level);
      g=g->NEXT;
   }
}


#if 0
static void printlogenvalue(logen_list *g)
{
   switch(g->TYPE) {
      case GENTYPE_EMPTY:
         break;
      case GENTYPE_BIT:
         printf("%c",g->VALUE.CHAR);
         break;
      case GENTYPE_VAL:
      case GENTYPE_ARG:
         printf("%ld",g->VALUE.VAL);
         break;
      case GENTYPE_HEX:
         printf("0x%lx",g->VALUE.VAL);
         break;
      case GENTYPE_TEXT:
         fputs(g->VALUE.TEXT, stdout);
         break;
      case GENTYPE_LIST:
         {
            logen_list *t;
            fputs("( ", stdout);
            for (t=g->VALUE.LIST;t;t=t->NEXT) {
               printlogenvalue(t);
               fputc(' ', stdout);
            }
            fputs(") ", stdout);
         }
         break;
   }
}

static void printlogen(logen_list *g)
{
   while (g) {
      printf("%s %d ", g->NAME, g->TYPE);
      printlogenvalue(g);
      fputc('\n', stdout);
      g=g->NEXT;
   }
}
#endif
