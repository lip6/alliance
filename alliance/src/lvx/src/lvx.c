/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ident "@(#)Gate Netlist Comparator"

/****************************************************************************/
/*  2.14 : Ressort avec un exit 1 si erreur mbk au chargement */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/*                      ALLIANCE VLSI CAD   LVX                             */
/*                                                                          */
/*    Tool : LVX (Netlist Comparator)                                       */
/*                                                                          */
/*    (c) copyright 1992 Laboratory UPMC/MASI/CAO-VLSI                      */
/*    All rights reserved                                                   */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Author : Francois NACABAL                     October, 1st  1992      */
/*                                                                          */
/****************************************************************************/
/* $Log: lvx.c,v $
 * Revision 1.5  2012/05/14 14:20:21  alliance
 * Updated GNU/FSF address (patch from Thibault North).
 *
 * Revision 1.4  2004/05/22 14:26:08  ludo
 * Now, by default LVX does not check unassigned signals between the two input netlists.
 * (this feature is usefull/mandatory with the new VST driver that adds sometimes unused
 * signals to have consitent VHDL vectors declaration)
 * The command line option '-u' permits to behave like it was before and then check
 * also unassigned signals.
 *
 * Revision 1.3  2002/09/30 16:20:46  czo
 * support/users
 *
 * Revision 1.2  2002/03/22 09:45:47  xtof
 * ALLIANCE_VERSION
 *
 * Revision 1.1.1.1  2002/03/20 17:07:59  xtof
 * importing lvx ....
 *
 * Revision 1.2  2000/10/23 16:33:37  syf
 * Big bug dans LVX (a se demander comment ca a pu marcher un jour ...)
 *
 * Revision 1.1.1.1  1998/10/02 15:25:15  alliance
 * Imported by czo
 *
 * Revision 2.23  1994/01/19  14:57:20  lvx
 * Now the -f option will not print invalid parameter
 *
 * Revision 2.22  1993/10/03  16:11:11  lvx
 * The revision of Alliance is given in the Makefile with ALC define
 *
 * Revision 2.21  1993/09/08  21:15:48  lvx
 * The alliancebanner() is now used.
 *
 * Revision 2.20  1993/09/08  21:04:55  lvx
 * Now to flatten a netlist, you have to specify it using the -f option.
 * The power supply names are defined by MBK_VDD and MBK_VSS.
 * Unconnected signals are checked using their names.
 * Tie cells are defined using the attribute F defined in the CATALOG file.
 *
 * Revision 2.13  1993/01/07  20:44:48  lvx
 * The feed-through cells are detected with the attribute put in the catalog
 * file.
 *
 * Revision 2.12  1992/11/27  18:36:58  lvx
 * Models are put out of memory after loading.
 * Models are put out of memory before orderring.
 *
 * Revision 2.11  1992/10/12  17:20:39  lvx
 * LOCON list has been reversed with reverse()
 * Best way should be modify the orderlocon algorithm
 *
 * Revision 2.10  1992/10/12  16:31:13  lvx
 * The function orderlocon() has been modified to bug fixing
 *
 */

static char rcsid[] = "$Id: lvx.c,v 1.5 2012/05/14 14:20:21 alliance Exp $" ;

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>

#define  TRUE          1
#define  FALSE         0

/* Marks in trees for compare functions */
#define  NET1          1
#define  NET2          2
#define  BOTH          3

/* Error codes */
#define  OK            0
#define  ERR           1 /* irregular netlist or comparison failed */

/*****************************************************************************/
/***********  STRUCTURES DEFINITIONS  ****************************************/
/*****************************************************************************/

/* Binary Tree of terminals (root connectors) */

typedef struct rootlocon_node {
        struct locon          *LOCON;
        unsigned char          MARK;
        struct rootlocon_node *LEFT;
        struct rootlocon_node *RIGHT;
        } rootlocon_tree;


/* Binary Tree of instances */

typedef struct loins_node {
        struct loins      *LOINS;
        unsigned char      MARK;
        struct loins_node *LEFT;
        struct loins_node *RIGHT;
        } loins_tree;


/* Binary Tree of all connectors */

typedef struct locon_node {
        struct locon      *LOCON1;
        struct locon      *LOCON2;
        struct locon_node *LEFT;
        struct locon_node *RIGHT;
        } locon_tree;


/*****************************************************************************/
/*******  GLOBAL VARIABLES  **************************************************/
/*****************************************************************************/

rootlocon_tree *rootlocon_head = NULL; /* Head of the Tree of terminals      */
loins_tree     *loins_head     = NULL; /* Head of the Tree of instances      */
locon_tree     *locon_head     = NULL; /* Head of the Tree of all connectors */

int rootlocon_count = 0;
int     loins_count = 0;
int     locon_count = 0;
int     power_count = 0;
int     error_count = 0;

long int     power_time;
long int rootlocon_time;
long int     loins_time;
long int     locon_time;
long int     total_time;

int flag_chk_unusig = FALSE;
int unusig_flag = FALSE;
int     exitcode = 2;

/*****************************************************************************/
/*******  FUNCTIONS  *********************************************************/
/*****************************************************************************/

/*===========================================================================*/
void  fatalerror (message)
/*===========================================================================*/
char *message;

/* Prints a error message, and EXIT with error code.
*/
{
 (void)fprintf (stderr, "\n!!!!! %s", message);
 (void)fprintf (stderr, " - Stop.\n\n");

 exit (2);
}


/*===========================================================================*/
          rootlocon_tree *createrootlocon (ptlocon, mark)
/*===========================================================================*/
locon_list *ptlocon;
unsigned char mark;
 
/* Allocates a block in memory for a terminal node.
   Never returns NULL, stop if impossible.
*/
{
 rootlocon_tree *ptnode;

 if (ptlocon == (locon_list *)NULL)
    fatalerror ("Fatal error on tree construction. (createrootlocon)");

 if ((ptnode = (rootlocon_tree *)malloc (sizeof (rootlocon_tree))) == NULL)
    fatalerror ("Fatal error on memory allocation. (createrootlocon)");

 ptnode->LOCON = ptlocon;
 ptnode->MARK  = mark;
 ptnode->LEFT  = ptnode->RIGHT = (rootlocon_tree *)NULL;
 
 rootlocon_count ++;
 if ( (error_count == 0) && ((rootlocon_count&3) == 0) )
    printf (".");
 return ( ptnode );
}


/*===========================================================================*/
          loins_tree *createloins (ptloins, mark)
/*===========================================================================*/
loins_list *ptloins;
unsigned char mark;
 
/* Allocates a block in memory for an instance node.
   Never returns NULL, stop if impossible.
*/
{
 loins_tree *ptnode;

 if (ptloins == (loins_list *)NULL)
    fatalerror ("Fatal error on tree construction. (createloins)");

 if ((ptnode = (loins_tree *)malloc (sizeof (loins_tree))) == NULL)
    fatalerror ("Fatal error on memory allocation. (createloins)");

 ptnode->LOINS = ptloins;
 ptnode->MARK  = mark;
 ptnode->LEFT  = ptnode->RIGHT = (loins_tree *)NULL;

 loins_count ++;
 if ( (error_count == 0)  && ((loins_count&63) == 0) )
    printf (".");
 return ( ptnode );
}


/*===========================================================================*/
          locon_tree *createlocon (ptlocon, mark)
/*===========================================================================*/
locon_list *ptlocon;
unsigned char mark;
 
/* Allocates a block in memory for a connector node.
   Never returns NULL, stop if impossible.
*/
{
 locon_tree *ptnode;

 if (ptlocon == (locon_list *)NULL)
    fatalerror ("Fatal error on tree construction. (createlocon)");

 if ((ptnode = (locon_tree *)malloc (sizeof (locon_tree))) == NULL)
    fatalerror ("Fatal error on memory allocation. (createlocon)");

 switch (mark) {
        case NET1 :             
             ptnode->LOCON1 = ptlocon;
             ptnode->LOCON2 = (struct locon *)NULL;
             break;

        case NET2 :
             ptnode->LOCON1 = (struct locon *)NULL;
             ptnode->LOCON2 = ptlocon;
             break;

        default :
             fatalerror ("Fatal error on tree construction. (createlocon)");
        }
 ptnode->LEFT = ptnode->RIGHT = (locon_tree *)NULL;

 locon_count ++;
 if ( (error_count == 0) && ((locon_count&63) == 0))
    printf (".");
 return ( ptnode );
}


/*===========================================================================*/
          int loinscmp (loins1, loins2)
/*===========================================================================*/
loins_list *loins1, *loins2;

/* Compares the two names (insname and figname) of the instances loins1 and
   loins2.
   Returns 0 if equal, 1 if less, and 2 if greater.
   If insnames are equals, but fignames not, this function returns -1. It's an
   error in the netlist(s).
*/
{
 int result;

 if ( (loins1 == (loins_list *)NULL) || (loins2 == (loins_list *)NULL) )
    fatalerror ("Fatal error on instances compare. (loinscmp)");

 if ( (result = strcmp (loins1->INSNAME, loins2->INSNAME)) != 0)
    return ( (result < 0) ? 1 : 2 );

 return ( (loins1->FIGNAME == loins2->FIGNAME) ? 0 : -1 );

}


/*===========================================================================*/
          int loconcmp (locon, ptnode)
/*===========================================================================*/
locon_list *locon;
locon_tree *ptnode;
/*
  Compares the name of the connector pointed by 'locon' with the name of
  the connector pointed by one of LOCON1 and LOCON2 fields in the 'ptnode'.
  Returns 0 if equal, 1 if less, and 2 if greater.
  Returns -1 on error ( pointer to NULL or irregular instances ).
*/
{
 locon_list *ptaux;
 int result1;

 if ( (locon == (locon_list *)NULL) || (ptnode == (locon_tree *)NULL) )
    return ( -1 );

        /* find the first locon pointed by the node */
 if (ptnode->LOCON1 != NULL) ptaux = ptnode->LOCON1;
 else if (ptnode->LOCON2 != NULL) ptaux = ptnode->LOCON2;
      else return ( -1 );

        /* if names different, return less or greater */
 if ( (result1 = strcmp (locon->NAME, ptaux->NAME)) != 0 )
    return ( (result1 < 0) ? 1 : 2 );

        /* same names */
          /* if both terminals, return equal */
 if ( (locon->TYPE == EXTERNAL) && (ptaux->TYPE == EXTERNAL) ) return ( 0 );

         /* if both internal, compare instance names and return that result */
 if ( (locon->TYPE == INTERNAL) && (ptaux->TYPE == INTERNAL) )
    return ( loinscmp (locon->ROOT, ptaux->ROOT) );

         /* if only one is terminal, return it as greater */
 return ( (locon->TYPE == INTERNAL) ? 1 : 2 );
}


/*===========================================================================*/
          int insertrootlocon (locon, mark)
/*===========================================================================*/
locon_list *locon;
unsigned char mark;

/* Inserts the connector pointed by 'locon' with his 'mark' in the Binary
   Tree of root connectors, beginning with the 'rootlocon_head' global pointer.
   Returns ERR if error on insertion, OK else.
*/
{
 rootlocon_tree *prev_node, *ptnode, *ptaux;
 unsigned char prev_mark;
 int compar;

 if (locon == (locon_list *)NULL)
    fatalerror ("Fatal error on tree construction. (insertrootlocon)");

 if (rootlocon_head == NULL) {
    rootlocon_head = createrootlocon (locon, mark);
    return ( OK );
    }

 ptnode = rootlocon_head;
 while ( (ptnode != NULL) && (locon->NAME != ptnode->LOCON->NAME) ) {
       prev_node = ptnode;
       ptnode = (compar = strcmp(locon->NAME, ptnode->LOCON->NAME)) < 0 ?
                ptnode->LEFT :
                ptnode->RIGHT;
       }

 if (ptnode == NULL) {
    ptaux = createrootlocon (locon, mark);
    if (compar < 0) prev_node->LEFT  = ptaux;
               else prev_node->RIGHT = ptaux;
    return ( OK );
    }

 prev_mark = ptnode->MARK;
 if ( (prev_mark == BOTH) || (prev_mark == mark)) {
    /* Error: two connectors with the same name in the same figure */
    error_count ++;
    printf ("\n\nTerminal '%s' repeated in the netlist %i.", locon->NAME, mark);
    return ( ERR );
    }

 if ( (prev_mark + mark) != (NET1 + NET2) )
    fatalerror ("Fatal error on tree construction. (insertrootlocon)");

 ptnode->MARK = BOTH;
 return ( OK );
}


/*===========================================================================*/
          int insertloins (loins, mark)
/*===========================================================================*/
loins_list *loins;
unsigned char mark;

/* Inserts the instance pointed by 'loins' with his 'mark' in the Binary
   Tree of instances, beginning with the 'loins_head' global pointer.
   Returns ERR if error on insertion, OK else.
*/
{
 loins_tree *prev_node, *ptnode, *ptaux;
 unsigned char prev_mark;
 int prev_compar, compar;

 if (loins == (loins_list *)NULL)
    fatalerror ("Fatal error on tree construction. (insertloins)");

 if (loins_head == NULL) {
    loins_head = createloins (loins, mark);
    return ( OK );
    }

 ptnode = loins_head;
 while ( (ptnode != NULL) && ((compar=loinscmp (loins, ptnode->LOINS))>0) ) {
       prev_node = ptnode;
       prev_compar = compar;
       ptnode = (compar == 1) ? ptnode->LEFT : ptnode->RIGHT;
       }

 if (ptnode == NULL) {
    if (prev_compar <= 0)
       fatalerror ("Fatal error on tree construction. (insertloins)");

    ptaux = createloins (loins, mark);
    if (prev_compar == 1) prev_node->LEFT  = ptaux;
                     else prev_node->RIGHT = ptaux;
    return ( OK );
    }

 prev_mark = ptnode->MARK;
 if (compar == -1) { /* two instances with same insname, but fignames differents */
    if (prev_mark == mark) { /* the  two instances are in the same figure */
       error_count ++;
       printf ("\n\nInstance '%s' repeated with different fignames in netlist %i:",
               loins->INSNAME, mark);
       printf ("'%s' and '%s'.", loins->FIGNAME, ptnode->LOINS->FIGNAME);
       }

    else { /* one instance in each netlist */
       error_count ++;
       printf ("\n\nInstance '%s' has different fignames in each netlist :",
               loins->INSNAME);
       if (mark == NET1) printf ("\n'%s' in netlist 1 and '%s' in netlist 2.",
                                 loins->FIGNAME, ptnode->LOINS->FIGNAME);
                    else printf ("\n'%s' in netlist 1 and '%s' in netlist 2.",
                                 ptnode->LOINS->FIGNAME, loins->FIGNAME);
       } /* endelse prev_mark */
    return ( ERR );
    }    /* endif compar      */

 if ( (prev_mark == BOTH) || (prev_mark == mark) ) {
    /* Error : two instances with the same names (insname and figname) in the
       same figure */
    error_count ++;
    printf ("\n\nInstance '%s' repeated in the netlist %i.", loins->INSNAME, mark);
    return ( ERR );
    }

 if ( (prev_mark + mark) != (NET1 + NET2) )
    fatalerror ("Fatal error on tree construction. (insertloins)");

 ptnode->MARK = BOTH;
 return ( OK );
}


/*===========================================================================*/
          int insertlocon (locon, mark)
/*===========================================================================*/
locon_list *locon;
unsigned char mark;

/* Inserts the connector pointed by 'locon' in the Binary Tree of connectors,
   beginning with the 'locon_head' global pointer.
   Returns ERR if error on insertion, OK else.
*/
{
 locon_tree *prev_node = NULL, *ptnode, *ptaux;
 int prev_compar = 0, compar;

 if (locon == (locon_list *)NULL)
    fatalerror ("Fatal error on tree construction. (insertlocon)");

 if (locon_head == NULL) {
    locon_head = createlocon (locon, mark);
    return ( OK );
    }
 
 ptnode = locon_head;
 while ( (ptnode != NULL) && ((compar = loconcmp (locon, ptnode)) > 0) ) {
       prev_node = ptnode;
       prev_compar = compar;
       ptnode = (compar == 1) ? ptnode->LEFT : ptnode->RIGHT;
       }

 if (ptnode == NULL) { /* terminal node in the tree */
    ptaux = createlocon (locon, mark);
    if (prev_compar == 1) prev_node->LEFT  = ptaux;
                     else prev_node->RIGHT = ptaux;
    return ( OK );
    }

 if (compar != 0) fatalerror ("Fatal error on tree construction. (insertlocon)");

 switch (mark) { /* insert new locon in the current node */
        case NET1 :
             if (ptnode->LOCON1 != NULL) {
                /* Error : two connectors with the same name in the same figure */
                error_count ++;
                printf("\n\nConnector '%s' repeated in the netlist 1.",locon->NAME);
                return ( ERR );
                }
             ptnode->LOCON1 = locon;
             return ( OK );

        case NET2 :
             if (ptnode->LOCON2 != NULL) {
                /* Error : two connectors with the same name in the same figure */
                error_count ++;
                printf("\n\nConnector '%s' repeated in the netlist 2.",locon->NAME);
                return ( ERR );
                }
             ptnode->LOCON2 = locon;
             return ( OK );
        }

 fatalerror ("Fatal error on tree construction. (insertlocon)");
 return 0; /* never reached. */
}


/*===========================================================================*/
          int makerootconnectors (lofig, mark)
/*===========================================================================*/
lofig_list *lofig;
unsigned char mark;

/* Constructs (creates or appends) the Binary Tree of root connectors of the
   'lofig' figure, with the 'mark' (NET1 or NET2), beginning with the
   'rootlocon_head' global pointer.
   Returns ERR if one of the terminals is irregular, OK else.
*/
{
 locon_list *ptlocon;
 int result = OK;
 
 if (lofig == (struct lofig *)NULL) {
    error_count ++;
    printf ("\n\nNetlist %i is empty!\n", mark);
    return ( ERR );
    }
     
 for (ptlocon = lofig->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT)
     result |= insertrootlocon (ptlocon, mark);

 return ( result );
}


/*===========================================================================*/
          int makeinstances (lofig, mark)
/*===========================================================================*/
lofig_list *lofig;
unsigned char mark;

/* Constructs (creates or appends) the Binary Tree of the instances in the
   'lofig' figure, with the 'mark' (NET1 or NET2), beginning with the
   'loins_head' global pointer.
   Returns ERR if one of the instances is irregular, OK else.
*/
{
 loins_list *ptloins;
 int result = OK;

 if (lofig == (struct lofig *)NULL) {
    error_count ++;
    printf ("\n\nNetlist %i is empty!\n", mark);
    return ( ERR );
    }
 
 if (lofig->LOINS == NULL) return ( OK );

 for (ptloins = lofig->LOINS; ptloins != NULL; ptloins = ptloins->NEXT)
     result |= insertloins (ptloins, mark);

 return ( result );
}


/*===========================================================================*/
          int makeconnections (lofig, mark)
/*===========================================================================*/
lofig_list *lofig;
unsigned char mark;

/* Constructs (creates or appends) the Binary Tree of all the connectors in the
   'lofig' figure, with the 'mark' (NET1 or NET2), beginning with the
   'locon_head' global pointer.
   Returns ERR if one of the connectors is irregular, OK else.
*/
{
 loins_list *ptloins;
 locon_list *ptlocon;
 int result = OK;

 if (lofig == (struct lofig*)NULL) {
    error_count ++;
    printf ("\n\nNetlist %i is empty!\n", mark);
    return ( ERR );
    }

 /* terminals */
 for (ptlocon = lofig->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT)
     result |= insertlocon (ptlocon, mark);

 /* connectors of instances */
 for (ptloins = lofig->LOINS; ptloins != NULL; ptloins = ptloins->NEXT)
     for (ptlocon = ptloins->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT)
         result |= insertlocon (ptlocon, mark);
     
 return ( result );
} 


/*===========================================================================*/
          int checkrootlocontree (ptnode)
/*===========================================================================*/
rootlocon_tree *ptnode;

/* check recursively if all the marks of the tree pointed by 'ptnode' are BOTH.
   Returns OK if all are BOTH, ERR else.
*/
{
 int checkleft, checkright;

 if (ptnode == NULL) return ( OK );

 switch (ptnode->MARK) {
        case BOTH :
             /* this terminal is present in the two figures, continue */
             checkleft  = checkrootlocontree (ptnode->LEFT);
             checkright = checkrootlocontree (ptnode->RIGHT);
             return ( ((checkleft == OK) && (checkright == OK)) ? OK : ERR );

        case NET1 :
             /* this terminal is not in the figure 2, continue */
             error_count ++;
             printf ("\nTerminal '%s' only in netlist 1", ptnode->LOCON->NAME);
             checkrootlocontree (ptnode->LEFT);
             checkrootlocontree (ptnode->RIGHT);
             return ( ERR );

        case NET2 :
             /* this terminal is not in the figure 1, continue */
             error_count ++;
             printf ("\nTerminal '%s' only in netlist 2", ptnode->LOCON->NAME);
             checkrootlocontree (ptnode->LEFT);
             checkrootlocontree (ptnode->RIGHT);
             return ( ERR );
        }

 fatalerror ("Fatal error on Terminals Tree checking. (checkrootlocontree)");
 return 0; /* never reached. */
}


/*===========================================================================*/
          int checkloinstree (ptnode)
/*===========================================================================*/
loins_tree *ptnode;

/* check recursively if all the marks of the tree pointed by 'ptnode' are BOTH.
   Returns OK if all are BOTH, ERR else.
*/
{
 int checkleft, checkright;

 if (ptnode == NULL) return ( OK );

 switch (ptnode->MARK) {
        case BOTH :
             /* this instance is present in the two figures, continue */
             checkleft  = checkloinstree (ptnode->LEFT);
             checkright = checkloinstree (ptnode->RIGHT);
             return ( ((checkleft == OK) && (checkright == OK)) ? OK : ERR );

        case NET1 :
             /* this instance is not in the figure 2, continue */
             error_count ++;
             printf ("\nInstance '%s' only in netlist 1", ptnode->LOINS->INSNAME);
             checkloinstree (ptnode->LEFT);
             checkloinstree (ptnode->RIGHT);
             return ( ERR );

        case NET2 :
             /* this instance is not in the figure 1, continue */
             error_count ++;
             printf ("\nInstance '%s' only in netlist 2", ptnode->LOINS->INSNAME);
             checkloinstree (ptnode->LEFT);
             checkloinstree (ptnode->RIGHT);
             return ( ERR );
        }

 fatalerror ("Fatal error on Instances Tree checking. (checkloinstree)");
 return 0; /* never reached. */
}


/*===========================================================================*/
void  mylofigchain (lofig)
/*===========================================================================*/
lofig_list *lofig;

/* Same as lofigchain function, but with additionnal error tests about pointers,
   and without transistors list.
*/
{
locon_list *ptcon = NULL;
losig_list *ptsig = NULL;
loins_list *ptins = NULL;
ptype_list *ptype = NULL;

 if (lofig->MODE != 'A')
    fatalerror ("Fatal error on 'mylofigchain': figure is interface only");

/*  cleaning ...    */
 for (ptsig = lofig->LOSIG; ptsig != NULL; ptsig = ptsig->NEXT) {
     ptype = getptype (ptsig->USER, (long)LOFIGCHAIN);
     if (ptype != (ptype_list *)NULL) {
        freechain ((chain_list *)ptype->DATA);
        ptype->DATA = (void *)NULL;
        }
     else ptsig->USER = addptype (ptsig->USER, (long)LOFIGCHAIN, (void *)NULL);
     }

/*  scan connector list  */
 for (ptcon = lofig->LOCON; ptcon != NULL; ptcon = ptcon->NEXT) {
     ptsig = ptcon->SIG;
     if ( (ptype = getptype (ptsig->USER, (long)LOFIGCHAIN)) == NULL ) {
        (void)fprintf (stderr, "\nInvalid signal on terminal %s (%s)",
                       ptcon->NAME, lofig->NAME);
        fatalerror ("Fatal error on 'mylofigchain'");
        }
     ptype->DATA = (void *)addchain ((chain_list *)ptype->DATA, (void*)ptcon);
     }

/*  scan instance  list   */
 for (ptins = lofig->LOINS; ptins != NULL; ptins = ptins->NEXT)    
     for (ptcon = ptins->LOCON; ptcon != NULL; ptcon = ptcon->NEXT) {
         ptsig = ptcon->SIG;
         if ( (ptype = getptype (ptsig->USER, (long)LOFIGCHAIN)) == NULL ) {
            (void)fprintf (stderr, "\nInvalid signal on connector %s of %s (%s)",
                           ptcon->NAME, ptins->INSNAME, lofig->NAME);
            fatalerror ("Fatal error on 'mylofigchain'");
            }
         ptype->DATA = (void *)addchain ((chain_list *)ptype->DATA,(void *)ptcon);
         } /* endfor ptcon */
}


/*===========================================================================*/
          locon_list *findlocon (locon, mark)
/*===========================================================================*/
locon_list *locon;
unsigned char mark;

/* Searchs in the Tree of all connectors the node corresponding to the 'locon',
   and returns the pointer to the locon in the opposite figure. 'mark' is the
   mark of the figure for the given locon.
   Returns NULL if not found.
*/
{
 locon_tree *ptnode;
 int compar;

 ptnode = locon_head;

 while ( (ptnode != NULL) && ((compar = loconcmp (locon, ptnode)) > 0) )
       ptnode = (compar == 1) ? ptnode->LEFT : ptnode->RIGHT;

      /* not found */
 if ( ptnode == NULL ) return ( (locon_list *)NULL );

      /* error on tree scanning */
 if (compar != 0) fatalerror ("Fatal error on tree construction. (findlocon)");

      /* return opposite locon */
 return ( (mark == NET1) ? ptnode->LOCON2 : ptnode->LOCON1 );      
}


/*===========================================================================*/
          char *findrootname (locon)
/*===========================================================================*/
locon_list *locon;

/* Returns a char pointer to the name of the object it belongs to.
   If 'locon' is INTERNAL, returns INSNAME of instance,
   if 'locon' is EXTERNAL, returns NAME of figure,
   else fatal error.
*/
{
 switch (locon->TYPE) {
        case INTERNAL :
             return ( ((loins_list *)(locon->ROOT))->INSNAME );
        case EXTERNAL :
             return ( ((lofig_list *)(locon->ROOT))->NAME );
        }

 fatalerror ("Fatal error on mbk structure: Illegal connector");
 return NULL; /* never reached. */
}


/*===========================================================================*/
          int checkconnections (lofig, mark)
/*===========================================================================*/
lofig_list *lofig;
unsigned char mark;

/*
  Checks that all the signals in the figure 'lofig' have same connectors in the
  opposite figure.
  Two types of error: 
  - the structure is not correct (invalid or inexistent pointers, bad lofigchain),
    Stop on fatal error.
  - the netlists are not identical (different signals in each netlist for one
    connector, inexistent signal or connector), set the 'result' flag to ERR but
    continue to find other errors, or other connectors for the same error.
*/
{
 losig_list *ptlosig;
 long int losig_count = 0;
 int result = OK;

 if (lofig == (struct lofig *)NULL) {
    error_count ++;
    printf ("\n\nNetlist %i is empty!\n", mark);
    return ( ERR );
    }
 for (ptlosig = lofig->LOSIG; ptlosig != NULL; ptlosig = ptlosig->NEXT) {
     ptype_list *ptype;
     chain_list *ptchain;
     locon_list *firstlocon, *ptlocon, *firstopplocon, *ptopplocon;
     losig_list *ptequip;

     if ( (error_count == 0) && ((losig_count&63) == 0) ) {
        printf (".");
        losig_count++;
	}
     ptype = getptype (ptlosig->USER, (long)LOFIGCHAIN);
     if (ptype == NULL) {
        (void)fprintf (stderr,
                       "\n\nInvalid ptype pointer for signal %s in netlist %i",
                       getsigname(ptlosig), mark);
        fatalerror ("Fatal error on 'checkconnections'");
        }
/*
     ptchain = (chain_list *)(ptype->DATA);
     if (ptchain == NULL) {
        (void)fprintf (stderr, "\n\nInvalid signal %s on netlist %i",
                       getsigname(ptlosig), mark);
        fatalerror ("Fatal error on 'checkconnections'");
        }
*/
/*
     ptchain = (chain_list *)(ptype->DATA);
     if (ptchain == NULL) {
        if (strcmp(getsigname(ptlosig),"bulk")!=0) printf ("\nWarning: Signal %s without connector on netlist %i\n",
                       getsigname(ptlosig), mark);
	continue;
        }
*/
     ptchain = (chain_list *)(ptype->DATA);
     if (ptchain == NULL) {
	if ( flag_chk_unusig ) unusig_flag = TRUE; 
	continue;
        }

     firstlocon = (locon_list *)(ptchain->DATA);
        /* find equivalent locon in opposite figure */
     firstopplocon = findlocon ( firstlocon, mark);
     if (firstopplocon == NULL) {
        error_count ++;
        printf ("\n\nConnector %s of '%s' (signal %s) in netlist %i",
                firstlocon->NAME, findrootname (firstlocon),
                getsigname(ptlosig), mark);
        printf (" NOT found in netlist %i", (NET1+NET2-mark));
        result = ERR;
        continue;
        }

        /* read signal of opposite locon */
     ptequip = firstopplocon->SIG;
     if (ptequip == NULL) {
            error_count ++;
            printf ("\nConnector '%s' without signal in netlist %i",
                    firstopplocon->NAME, (NET1+NET2-mark));
            result = ERR;
            continue;
            }

         /* for all other connectors of same signal : find it in opposite */
     for (ptchain = ptchain->NEXT; ptchain != NULL; ptchain = ptchain->NEXT) {
         losig_list *ptsigfound;

         ptlocon = (locon_list *)(ptchain->DATA);
         ptopplocon = findlocon ( ptlocon, mark);
         if (ptopplocon == NULL) {
            error_count ++;
            printf ("\n\nConnector %s of '%s' (signal %s) in netlist %i",
                   ptlocon->NAME, findrootname (ptlocon),
                   getsigname(ptlosig), mark);
            printf (" NOT found in netlist %i", (NET1+NET2-mark));
            result = ERR;
            continue;
            }
         ptsigfound = ptopplocon->SIG;
         if (ptsigfound == NULL) {
            error_count ++;
            printf ("\n\nConnector '%s' without signal in netlist %i",
                    ptopplocon->NAME, mark);
            result = ERR;
            }
         else if (ptopplocon->SIG != ptequip) {
           if (mark == NET1) {
              error_count ++;
              printf ("\n\n%s of '%s' is NOT connected to %s of '%s' in netlist 2",
                      ptopplocon->NAME, findrootname(ptopplocon),
                      firstopplocon->NAME, findrootname(firstopplocon));
              printf ("\nthrough signal %s but to signal %s",
                      getsigname(ptequip), getsigname(ptopplocon->SIG));
              }
           else { /* short circuit */
              error_count ++;
              printf ("\n\n%s of '%s' IS connected to %s of '%s' in netlist 2",
                      ptopplocon->NAME, findrootname(ptopplocon),
                      firstopplocon->NAME, findrootname(firstopplocon));
              printf ("\nthrough signal %s",
                      getsigname(firstlocon->SIG));
              }
           result = ERR;
           } /* endelse ptsigfound */
         }   /* endfor ptchain     */
     }       /* endfor ptlosig     */

 return ( result );
}


/*===========================================================================*/
          int comparerootconnectors (lofig1, lofig2)
/*===========================================================================*/
lofig_list *lofig1, *lofig2;

/* Calls the tree-construction and checking functions.
   Returns OK if terminals are equivalent, ERR else.
*/
{
 int result;

 rootlocon_time = time( (long *)0 );

 if ( makerootconnectors (lofig1, NET1) != OK ) return ( ERR );
 if ( makerootconnectors (lofig2, NET2) != OK ) return ( ERR );
 
 result = checkrootlocontree (rootlocon_head);

 rootlocon_time = time( (long *)0 ) - rootlocon_time;
 return ( result ); 
}


/*===========================================================================*/
          int compareinstances (lofig1, lofig2)
/*===========================================================================*/
lofig_list *lofig1, *lofig2;

/* Calls the tree-construction and checking functions.
   Returns OK if instances are equivalent, ERR else.
*/
{
 int result;

 loins_time = time( (long *)0 );

 if ( makeinstances (lofig1, NET1) != OK ) return ( ERR );
 if ( makeinstances (lofig2, NET2) != OK ) return ( ERR );

 result = checkloinstree (loins_head); 

 loins_time = time( (long *)0 ) - loins_time;
 return ( result );
}


/*===========================================================================*/
          int compareconnections (lofig1, lofig2)
/*===========================================================================*/
lofig_list *lofig1, *lofig2;

/* Constructs the Binary Tree of all connectors in the figure, and check that
   all signals are connected to the same connectors in the two figures.
   Returns OK if equivalent, ERR else.
*/
{
 int result1, result2;

 locon_time = time( (long *)0 );

 result1 = makeconnections (lofig1, NET1);
 result2 = makeconnections (lofig2, NET2);

 if ( (result1 != OK) || (result2 != OK) ) return ( ERR );

 mylofigchain ( lofig1 );
 mylofigchain ( lofig2 );

 result1 = checkconnections (lofig1, NET1);
 result2 = checkconnections (lofig2, NET2);

 locon_time = time( (long *)0 ) - locon_time;
 return ( ((result1 == OK) && (result2 == OK)) ? OK : ERR );
}


/*===========================================================================*/
          int compareunusedsignals (lofig1, lofig2)
/*===========================================================================*/
lofig_list *lofig1, *lofig2;

/*
*/
{
 losig_list *ptlosig;
 int result = OK;

 if (lofig1 == (struct lofig *)NULL) {
    error_count ++;
    printf ("\n\nNetlist 1 is empty!\n");
    return ( ERR );
    }
 if (lofig2 == (struct lofig *)NULL) {
    error_count ++;
    printf ("\n\nNetlist 2 is empty!\n");
    return ( ERR );
    }

 for (ptlosig = lofig1->LOSIG; ptlosig != NULL; ptlosig = ptlosig->NEXT) {
     ptype_list *ptype;
     chain_list *ptchain;
     losig_list *opplosig;

     ptype = getptype (ptlosig->USER, (long)LOFIGCHAIN);
     if (ptype == NULL) {
        (void)fprintf (stderr,
                       "\n\nInvalid ptype pointer for signal %s in netlist 1",
                       getsigname(ptlosig));
        fatalerror ("Fatal error on 'checkunusedsignals'");
        }

     ptchain = (chain_list *)(ptype->DATA);
     if (ptchain == NULL) {
        char *signame;
        int found = FALSE;

        signame = getsigname(ptlosig);

        for (opplosig = lofig2->LOSIG; opplosig != NULL; opplosig = opplosig->NEXT)
            if (getsigname(opplosig) == signame) found = TRUE;
        if (found == FALSE) {
           error_count ++;
           printf ("\n\nNever assigned signal %s in netlist 1",
                   signame);
           printf (" NOT found in netlist 2");
           result = ERR;
           }   
        else
           if (strcmp(signame,"bulk")!=0)
              printf ("\nWarning: Never assigned signal %s in the 2 netlists\n", signame);

        }
     }

 return ( result );
}


/*===========================================================================*/
          int comparelofigs (lofig1, lofig2)
/*===========================================================================*/
lofig_list *lofig1, *lofig2;

/* Compare two logical figures (three phases if necessary).
   Returns OK if the netlists are equivalent, ERR else.
*/
{
 int result1, result2, result3, result4;

 if (lofig1 == (struct lofig *)NULL) {
    error_count ++;
    printf ("\n\nNetlist 1 is empty!\n");
    return ( ERR );
    }
 if (lofig2 == (struct lofig *)NULL) {
    error_count ++;
    printf ("\n\nNetlist 2 is empty!\n");
    return ( ERR );
    }

 printf ("\n\n\n***** Compare Terminals ..........");
 if ((result1 = comparerootconnectors (lofig1, lofig2)) == OK)
    printf ("\n***** O.K.\t(%lu sec)", rootlocon_time);

 printf ("\n\n***** Compare Instances ..........");
 if ((result2 = compareinstances (lofig1, lofig2)) == OK)
    printf ("\n***** O.K.\t(%lu sec)", loins_time);
 
 if ((result1 != OK) || (result2 != OK)) return ( ERR );

 printf ("\n\n***** Compare Connections ........");
 if ((result3 = compareconnections (lofig1, lofig2)) == OK)
    printf ("\n***** O.K.\t(%lu sec)", locon_time);

 if (unusig_flag == TRUE) {
     printf ("\n\n***** Compare Connectionless signals ........");
     if ((result4 = compareunusedsignals (lofig1, lofig2)) == OK)
        printf ("\n***** O.K.");
     }
     else result4 = OK;

 printf ("\n\n===== Terminals .......... %-6d\n", rootlocon_count);
 printf (    "===== Instances .......... %-6d\n", loins_count);
 printf (    "===== Connectors ......... %-6d\n", locon_count);
 if (error_count) printf (  "\n===== Errors ............. %i\n", error_count);

 if ( (result3 == OK) && (result4 == OK)) return ( OK );
                                     else return ( ERR );
 }


/*===========================================================================*/
void          equivterminal (lofig, name1, name2, delete)
/*===========================================================================*/
lofig_list *lofig;
char *name1, *name2;
int delete;

/* Reduces the first (normally single) terminal called 'name1' to 'name2'.
   All the connectors connected to terminal 'name1' are now connected to the
   first 'name2' (could be internal or external, priority to external).
   Stops if impossible, and returns (without error code) if OK.
*/
{
 loins_list *ptloins;
 locon_list *ptlocon, *ptaux;
 locon_list *reflocon = NULL;
 losig_list *oldlosig, *reflosig;
 int found = FALSE;

 if (lofig == NULL) fatalerror ("Invalid figure. (equivterminal)");
 if ( (name1 == NULL) || (name2 == NULL) )
    fatalerror ("Invalid connector name. (equivterminal)");

 name1 = namealloc (name1);
 name2 = namealloc (name2);
/* Search name2 in terminals */
 for (ptlocon = lofig->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT)
     if (ptlocon->NAME == name2) break;

 reflocon = ptlocon;
 if (reflocon == NULL) { /* Search name2 in internal connectors */
    for (ptloins = lofig->LOINS; (ptloins != NULL) && (found != TRUE);
         ptloins = ptloins->NEXT)
        for (ptlocon = ptloins->LOCON; (ptlocon != NULL) && (found != TRUE);
             ptlocon = ptlocon->NEXT)
            if (ptlocon->NAME == name2) {
               found = TRUE;
               reflocon = ptlocon;
               }
    }

 if (reflocon == NULL) {
    (void)fprintf (stderr, "\n\nConnector %s NOT found in %s", name2, lofig->NAME);
    fatalerror ("Fatal error in 'equivterminal'");
    }

 reflosig = reflocon->SIG;
 if (reflosig == NULL) {
    (void)fprintf (stderr, "\n\nInvalid signal for connector %s in %s",
                   reflocon->NAME, lofig->NAME);
    fatalerror ("Fatal error in 'equivterminal'");
    }

/* Search and reduce name1 in terminals */
 for (ptlocon = lofig->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT)
    if (ptlocon->NAME == name1) { /* found name1 */
      oldlosig = ptlocon->SIG;
      if (oldlosig == NULL) {
         (void)fprintf (stderr, "\n\nInvalid signal for connector %s in %s",
                        ptlocon->NAME, lofig->NAME);
         fatalerror ("Fatal error in 'equivterminal'");
         }
           /* branch all the connectors connected to name1 to name2 (ref) */
      for (ptaux = lofig->LOCON; ptaux != NULL; ptaux = ptaux->NEXT)
         if ((ptaux != ptlocon)&&(ptaux->SIG == oldlosig)) ptaux->SIG = reflosig;

      for (ptloins = lofig->LOINS; ptloins != NULL; ptloins = ptloins->NEXT)
        for (ptaux = ptloins->LOCON; ptaux != NULL; ptaux = ptaux->NEXT)
         if ((ptaux != ptlocon)&&(ptaux->SIG == oldlosig)) ptaux->SIG = reflosig;

      if (oldlosig != reflosig)
      if (dellosig (lofig, oldlosig->INDEX) != 1) 
         fatalerror ("Fatal error on signal deleting. (equivterminal)");

      if (delete)
      {
        if (dellocon (lofig, name1) != 1)
           fatalerror ("Fatal error on connector deleting. (equivterminal)");
      }
 
      break;
      }
}


/*===========================================================================*/
void          equivinslocon (lofig, loins, name1, name2)
/*===========================================================================*/
lofig_list *lofig;
loins_list *loins;
char *name1, *name2;

/* Reduces the first connector called 'name1' to 'name2'.
   All the connectors connected to 'name1' are now connected to 'name2', in the
   instance pointed by 'loins'.
   Stops if impossible, and returns (without error code) if OK.
*/
{
 loins_list *ptloins;
 locon_list *ptlocon, *ptaux, *prevlocon;
 locon_list *reflocon = NULL;
 losig_list *oldlosig, *reflosig;

 if (lofig == NULL) fatalerror ("Invalid figure. (equivinslocon)");
 if (loins == NULL) fatalerror ("Invalid instance. (equivinslocon)");
 if ( (name1 == NULL) || (name2 == NULL) )
    fatalerror ("Invalid connector name. (equivinslocon)");

 name1 = namealloc (name1);
 name2 = namealloc (name2);
/* Search name2 */
 for (ptlocon = loins->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT)
     if (ptlocon->NAME == name2) break;

 reflocon = ptlocon;
 if (reflocon == NULL) {
    (void)fprintf (stderr, "\n\nConnector %s NOT found in %s of %s",
                   name2, loins->INSNAME, lofig->NAME);
    fatalerror ("Fatal error in 'equivinslocon'");
    }

 reflosig = reflocon->SIG;
 if (reflosig == NULL) {
    (void)fprintf (stderr, "\n\nInvalid signal for connector %s in %s of %s",
                   reflocon->NAME, loins->INSNAME, lofig->NAME);
    fatalerror ("Fatal error in 'equivinslocon'");
    }

/* Search and reduce name1 */
 prevlocon = NULL;
 for (ptlocon = loins->LOCON; ptlocon != NULL; prevlocon = ptlocon,
      ptlocon = ptlocon->NEXT)
    if (ptlocon->NAME == name1) { /* found name1 */
      oldlosig = ptlocon->SIG;
      if (oldlosig == NULL) {
         (void)fprintf (stderr, "\n\nInvalid signal for connector %s in %s",
                        ptlocon->NAME, loins->INSNAME);
         fatalerror ("Fatal error in 'equivinslocon'");
         }
           /* branch all the connectors connected to name1 to name2 (ref) */
      for (ptaux = lofig->LOCON; ptaux != NULL; ptaux = ptaux->NEXT)
         if ((ptaux != ptlocon)&&(ptaux->SIG == oldlosig)) ptaux->SIG = reflosig;

      for (ptloins = lofig->LOINS; ptloins != NULL; ptloins = ptloins->NEXT)
        for (ptaux = ptloins->LOCON; ptaux != NULL; ptaux = ptaux->NEXT)
         if ((ptaux != ptlocon)&&(ptaux->SIG == oldlosig)) ptaux->SIG = reflosig;

      if (oldlosig != reflosig) if (dellosig (lofig, oldlosig->INDEX) != 1) 
         fatalerror ("Fatal error on signal deleting. (equivinslocon)");

      if (prevlocon == NULL) loins->LOCON = ptlocon->NEXT;
                        else prevlocon->NEXT = ptlocon->NEXT;
       break;
      }
}


/*===========================================================================
          startid ( lofig, name1, name2)

lofig_list *lofig;
char *name1, *name2;
{
 locon_list *ptlocon;

 if ( (name1 == NULL) || (name2 == NULL) )
    fatalerror ("Invalid connector name. (startid)");

 name1 = namealloc (name1);
 name2 = namealloc (name2);
 if (name1 == name2) return;
 for (ptlocon = lofig->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT)
     if (ptlocon->NAME == name2) ptlocon->NAME = name1;
}
===========================================================================*/


/*===========================================================================*/
void  localstartid ( ptlocon, name1, name2)
/*===========================================================================*/
locon_list *ptlocon;
char *name1, *name2;

/* All the connectors in the list beginning with 'ptlocon' named by 'name2'
   are renamed by 'name1'.
*/
{

 if ( (name1 == NULL) || (name2 == NULL) )
    fatalerror ("Invalid connector name. (localstartid)");

 name1 = namealloc (name1);
 name2 = namealloc (name2);
 if (name1 == name2) return;
 for (; ptlocon != NULL; ptlocon = ptlocon->NEXT)
     if (ptlocon->NAME == name2) ptlocon->NAME = name1;
}


/*===========================================================================*/
void   reducepower (lofig1, lofig2)
/*===========================================================================*/
lofig_list *lofig1, *lofig2;

/* Deletes all the connectors (internal or external) beginning with 'vdd' or
   'vss', except one which is renamed by 'vdd' (or 'vss').
   All the connectors in the figure connected to 'vdd*' (or 'vss*') are now
   connected to the last 'vdd' (or 'vss').
   Stops on error, returns without error code if OK.
*/
{
  chain_list *Delete;
  chain_list *ScanChain;
 locon_list *ptlocon;
char *firstvdd;
char *firstvss;
 loins_list *ptloins;

 printf ("\n\n***** Reduce Power ...............");
 power_time = time( (long *)0 );
 power_count = 0;

/* vdd */
 /* instance connectors */
 for (ptloins = lofig1->LOINS; ptloins != NULL; ptloins = ptloins->NEXT) {
   power_count ++;
   if((power_count&63) == 0 )  printf (".");
   firstvdd = NULL; 
   for (ptlocon = ptloins->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT) {
       if (firstvdd == NULL) {
          if (isvdd (ptlocon->NAME)) firstvdd = ptlocon->NAME;
          continue;
          }
       if (isvdd (ptlocon->NAME))
          equivinslocon (lofig1, ptloins, ptlocon->NAME, firstvdd);
       }
   if ( (firstvdd != NULL) && (firstvdd != namealloc (VDD)) )
      localstartid (ptloins->LOCON, VDD, firstvdd);
   }

 /* terminals */
 firstvdd = NULL;
 Delete = NULL;
 for (ptlocon = lofig1->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT) {
     power_count ++;
     if((power_count&63) == 0 )   printf (".");
     if (firstvdd == NULL) {
        if ( isvdd (ptlocon->NAME)) firstvdd = ptlocon->NAME;
        continue;
        }
     if (isvdd (ptlocon->NAME))
       {
        equivterminal (lofig1, ptlocon->NAME, firstvdd, 0);
        Delete = addchain( Delete, ptlocon );
       }
     }

 if ( (firstvdd != NULL) && (firstvdd != namealloc (VDD)) )
    localstartid (lofig1->LOCON, VDD, firstvdd);

 for ( ScanChain = Delete; ScanChain != NULL; ScanChain = ScanChain->NEXT )
 {
   ptlocon = (locon_list *)ScanChain->DATA;
   dellocon( lofig1, ptlocon->NAME );
 }

 freechain( Delete );
 Delete = NULL;

/* vss */
 /* instance connectors */
 for (ptloins = lofig1->LOINS; ptloins != NULL; ptloins = ptloins->NEXT) {
   power_count ++;
   if((power_count&63) == 0 )   printf (".");
   firstvss = NULL; 
   for (ptlocon = ptloins->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT) {
       if (firstvss == NULL) {
          if (isvss (ptlocon->NAME)) firstvss = ptlocon->NAME;
          continue;
          }
       if (isvss (ptlocon->NAME))
          equivinslocon (lofig1, ptloins, ptlocon->NAME, firstvss);
       }
   if ( (firstvss != NULL) && (firstvss!= namealloc (VSS)) )
      localstartid (ptloins->LOCON, VSS, firstvss);
   }

 /* terminals */
 firstvss = NULL;
 for (ptlocon = lofig1->LOCON; ptlocon != NULL; ptlocon = ptlocon->NEXT) {
     power_count ++;
     if((power_count&63) == 0 )   printf (".");
     if (firstvss == NULL) {
        if (isvss (ptlocon->NAME)) firstvss = ptlocon->NAME;
        continue;
        }
     if (isvss (ptlocon->NAME))
     {
        equivterminal (lofig1, ptlocon->NAME, firstvss, 0);
        Delete = addchain( Delete, ptlocon );
     }
     }

 if ( (firstvss != NULL) && (firstvss!= namealloc (VSS)) )
    localstartid (lofig1->LOCON, VSS, firstvss);

 for ( ScanChain = Delete; ScanChain != NULL; ScanChain = ScanChain->NEXT )
 {
   ptlocon = (locon_list *)ScanChain->DATA;
   dellocon( lofig1, ptlocon->NAME );
 }

 freechain( Delete );
 Delete = NULL;
 
 power_time = time( (long *)0 ) - power_time;
 printf ("\n***** O.K. (%lu sec)", power_time);
}


/*===========================================================================*/
          locon_list *orderlocon (list1, list2)
/*===========================================================================*/
locon_list *list1, *list2;

/* Reorders the connector list 'list2' seeking 'list1'.
   Returns the new head of 'list2', or stops if error.
*/
{
 locon_list *head, *headlist2;
 locon_list *reflocon, *ptlocon, *prevlocon;
 static long int count;

 if ( (list1 == NULL) || (list2 == NULL) )
    fatalerror ("Invalid connector lists. (orderlocon)");

   head = NULL ;
   headlist2 = list2 ;
   for( reflocon = list1 ; reflocon != NULL ; reflocon = reflocon->NEXT ) {
      prevlocon = NULL ;
      for( ptlocon = headlist2 ; ptlocon != NULL ; ptlocon = ptlocon->NEXT ) {
         count ++;
         if ((count&63) == 0) printf (".");
         if( ptlocon->NAME == reflocon->NAME ) {
           if( headlist2 == ptlocon )   headlist2 = ptlocon->NEXT ;
             else   prevlocon->NEXT = ptlocon->NEXT ;
           ptlocon->NEXT = head ;
           head = ptlocon ;
           break ;
          }
         prevlocon = ptlocon ;
        }

      if( ptlocon == NULL ) {
        (void)fprintf (stderr, "Connector %s of '%s' NOT found in netlist2",
                       reflocon->NAME, findrootname (reflocon));
        fatalerror ("Fatal error in 'orderlocon'");
        }
     }
 return ((locon_list *)reverse((chain_list *)head));
}


/*===========================================================================*/
void  order (lofig1, lofig2)
/*===========================================================================*/
lofig_list *lofig1, *lofig2;

/* Calls orderlocon() function, for terminals and for instances connectors of
   lofig2 (referring to model figures).
   Stops on fatal error, returns without error code if OK.
*/
{
 loins_list *ptloins;
 long int order_time;

 printf ("***** Orderring ...................");

 order_time = time( (long *)0 );

 if (lofig1 == (struct lofig *)NULL) {
    (void)fprintf (stderr, "\nNetlist 1 is empty!");
    fatalerror ("Fatal error in 'order'");
    }
 if (lofig2 == (struct lofig *)NULL) {
    (void)fprintf (stderr, "\nNetlist 2 is empty!");
    fatalerror ("Fatal error in 'order'");
    }

/* order terminals */
 if ( (lofig2->LOCON = orderlocon (lofig1->LOCON, lofig2->LOCON)) == NULL ) {
    (void)fprintf (stderr, "\nCan not order terminals of '%s'", lofig2->NAME);
    fatalerror ("Fatal error in 'order'");
    }

{ struct chain *mchain;
 for(mchain=lofig2->MODELCHAIN;mchain!=NULL;mchain=mchain->NEXT)
  if(mchain->DATA!=(void *)lofig2->NAME && mchain->DATA!=(void *)lofig1->NAME)
   dellofig((char *)mchain->DATA);
}
{ struct chain *mchain;
 for(mchain=lofig1->MODELCHAIN;mchain!=NULL;mchain=mchain->NEXT)
  if(mchain->DATA!=(void *)lofig2->NAME && mchain->DATA!=(void *)lofig1->NAME)
   dellofig((char *)mchain->DATA);
}

/* order instances connectors */
 for (ptloins = lofig2->LOINS; ptloins != NULL; ptloins = ptloins->NEXT) {
     lofig_list *ptmodel;

     if ( (ptmodel = getlofig (ptloins->FIGNAME, 'P'))== NULL ) {
        (void)fprintf (stderr,
                       "\nModel %s NOT found in netlist 1", ptloins->FIGNAME);
        fatalerror ("Fatal error in 'order'");
        }
     if ((ptloins->LOCON = orderlocon(ptmodel->LOCON,ptloins->LOCON)) == NULL) {
        (void)fprintf (stderr,
                       "\nCan not order connectors of '%s'",ptloins->INSNAME);
        fatalerror ("Fatal error in 'order'");
        }
     }

 order_time = time( (long *)0 ) - order_time;
 printf ("\n***** O.K. (%lu sec)\n\n", order_time);
}


/*===========================================================================*/
          lofig_list *loadfigure (format, figname, flag_flatten)
/*===========================================================================*/
char *format;
char *figname;
int flag_flatten;

/* Load a figure in memory, and calls rflattenlofig().
   Returns a pointer to the figure, or Stops if error.
*/
{
 lofig_list *ptlofig;

 if ( (format == NULL) || (figname == NULL) ) return ( NULL );

 printf ("\n\n***** Loading%s %s (%s)...",
         flag_flatten==TRUE?" and flattening":"",figname, format);
 strcpy ( IN_LO, format);
 ptlofig = getlofig (figname, 'A');

 if (flag_flatten == TRUE)
/* 92/11/27
 * The rflatten keep the models in memory. First lvx loads an .al and then
 * an .alx. The two instance sould not have the same interface, so the
 * models must be destroyed of memory.
 *
 rflattenlofig (ptlofig, 'Y', 'Y');
 */
{
lofig_list *ptfig =ptlofig;
char concat='Y';
char catal='Y';

loins_list *p;
chain_list *c;

        catal = catal == NO ? 0 : 1;
        for (p = ptfig->LOINS; p != NULL;) {
                if (!catal || !incatalog(p->FIGNAME)) {
                        dellofig(p->FIGNAME);
                        flattenlofig(ptfig, p->INSNAME, concat);
                        p = ptfig->LOINS;
                }
                else
                        p = p->NEXT;
        }
        freechain(ptfig->MODELCHAIN);
        ptfig->MODELCHAIN = NULL;
        p = ptfig->LOINS;
        while (p != NULL) {
                c = ptfig->MODELCHAIN;
                while (c != NULL) {
                        if ((char *)c->DATA == p->FIGNAME)
                                break;
                        c = c->NEXT;
                }
                if (c == NULL)
                        ptfig->MODELCHAIN = addchain(ptfig->MODELCHAIN, (void *)p->FIGNAME);
                p = p->NEXT;
        }
}

 return ( ptlofig );
}


/*===========================================================================*/
          void delfeed (lofig)
/*===========================================================================*/
lofig_list *lofig;
{
 loins_list *loins, *temploins;

 for (loins = lofig->LOINS; loins != NULL; loins = temploins) {
     temploins = loins->NEXT;
     if (incatalogfeed(loins->FIGNAME))
        if (delloins (lofig, loins->INSNAME) != 1)
           fatalerror ("Fatal error on instance deleting. (delins)");
     }
}


/*===========================================================================*/
          void delins (lofig, loinsname)
/*===========================================================================*/
lofig_list *lofig;
char *loinsname;

/* Deletes the instance named by 'loinsname' in the figure 'lofig'.
   Returns if OK, stops else.
*/
{
 loins_list *loins, *temploins;
 char       *ptname;

 ptname = namealloc (loinsname);
 for (loins = lofig->LOINS; loins != NULL; loins = temploins) {
     temploins = loins->NEXT;
     if (loins->FIGNAME == ptname)
        if (delloins (lofig, loins->INSNAME) != 1)
           fatalerror ("Fatal error on instance deleting. (delins)");
     }
}


/*===========================================================================*/
void  readparamfile (lofig1, lofig2, filename)
/*===========================================================================*/
lofig_list *lofig1, *lofig2;
char *filename;

/* Reads filename and calls 'equivterminal()'.
*/
{
 FILE *paramfile;
 static char locon1[10];
 static char locon2[10];
 static char str[10];
 int netlist;
 int code = 3;

 if ((paramfile = fopen (filename, "r")) == NULL) {
    (void)fprintf (stderr, "\nCan not open '%s'.\n", filename);
    fatalerror ("Fatal error on reading paramfile");
    }

 for (;;) {
     code = fscanf (paramfile, "equiv %s %s => %s\n", str, locon1, locon2);
     if (code != 3) break;
     sscanf ( str, "%i", &netlist);
     if ( (netlist != NET1) && (netlist != NET2) ) {
        (void)fprintf (stderr, "\nInvalid netlist %s in %s", str, filename);
        fatalerror ("Fatal error on reading paramfile");
        }
     equivterminal ( (netlist == NET1) ? lofig1 : lofig2, locon1, locon2, 1);
     }

 fclose (paramfile);
}


/*===========================================================================*/
void  presentation ()
/*===========================================================================*/
{ char revision[100] ;

 sscanf( rcsid, "$Id: lvx.c,v %s", revision ) ;

 alliancebanner("LVX",revision,"Gate Netlist Comparator","1992",ALLIANCE_VERSION);

}


/*****************************************************************************/
	void lvx_exit()
/*****************************************************************************/
{
	exit (exitcode);
}

/*****************************************************************************/
int  main (argc, argv)
/*****************************************************************************/
int argc;
char **argv;

/* Loads figures, calls comparefigures(), orders if demanded.
*/
{
 static char figname1 [100], figname2 [100];
 lofig_list *lofig1 = NULL, *lofig2 = NULL;
 int firstcount = 4, count = 4, flag_flatten = FALSE, flag_ord  = FALSE, flag_tie = TRUE;
 int result;

 presentation ();

 if (argc < 5) {
    printf ("\nUsage : lvx <format1> <format2> <filename1> <filename2> [-a] [-o] [-f] [-u]\n");
    printf (  "            [-i <paramfile>] [-t <cell_name> <cell_name> ...]\n\n");
    exit (2);
    }
 signal (SIGTERM, lvx_exit);
 mbkenv ();

 while (++firstcount < argc) { char *option;
       option = argv[firstcount];
       if (option[0] == '-') if (option[1] == 'f') flag_flatten = TRUE;
       } 

 if ( (lofig1 = loadfigure (argv[1], argv[3], flag_flatten)) == NULL ) {
    (void)fprintf (stderr, "\n!!!!! Can not load '%s'\n", argv[3]);
    fatalerror ("Fatal error on loading figure");
    }
 sprintf (figname1, "%s_logic", argv[3]);
 lofig1->NAME = namealloc (figname1);

 if ( (lofig2 = loadfigure (argv[2], argv[4], flag_flatten)) == NULL ) {
    (void)fprintf (stderr, "\n!!!!! Can not load '%s'\n", argv[4]);
    fatalerror ("Fatal error on loading figure");
    }
 sprintf (figname2, "%s_extract", argv[4]);
 lofig2->NAME = namealloc (figname2);

 total_time = time( (long *)0 );
 while (++count < argc) { char *option;
       option = argv[count];
       if (option[0] != '-') {
          printf ("\n***** Invalid parameter '%s'\n\n", option); continue;
          }
       if (option[2] != 0) {
          printf ("\n***** Invalid option '%s'\n\n", option); continue;
          }
       switch (option[1]) {
          case 'a' : reducepower(lofig1,lofig2); reducepower(lofig2,lofig1);break;
          case 'f' : break;
          case 'o' : flag_ord = TRUE; break;
          case 'i' : readparamfile (lofig1, lofig2, argv[++count]); break;
          case 't' : flag_tie = FALSE;
                     while ((argv[count+1][0] != '-') && (++count < argc)) {
                           delins (lofig1, argv[count]);
                           delins (lofig2, argv[count]);
                           }
                     break;
          case 'u' : flag_chk_unusig = TRUE;
                     break;
          default  : printf ("\n***** Invalid option '%s'\n\n", option); continue;
          } 
       }

 delfeed(lofig1);delfeed(lofig2);

 result = comparelofigs (lofig1, lofig2);
 total_time = time( (long *)0 ) - total_time;
 if (result != OK) {
    printf ("\n\n***** Netlists are NOT Identical! *****\t(%lu sec)\n\n",
            total_time);
    exit (2);
    }
 printf ("\n\n***** Netlists are Identical. *****\t(%lu sec)\n\n", total_time);

 if (flag_ord) {
    strcpy( IN_LO, argv[1] ) ;
    order (lofig1, lofig2);
    lofig2->NAME = namealloc (argv[4]);
    printf ("***** Saving %s (%s)...\n\n", lofig2->NAME, OUT_LO);
    savelofig (lofig2);
    }
 exit ( 0 );
 return 0;
}
