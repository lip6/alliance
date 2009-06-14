/*
   ### -------------------------------------------------- ### 
   $Author: ludo $
   $Date: 2009/06/14 13:51:59 $
   $Log: mbk_tree.c,v $
   Revision 1.2  2009/06/14 13:51:59  ludo
   - port 64 bits

   Revision 1.1  2002/03/15 14:37:22  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:20  hcl
   First commit in the new tree.

   Revision 1.3  2001/12/20 13:04:03  hcl
   Cosmetique.

   Revision 1.2  2001/12/14 15:58:39  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.1  2001/09/26 14:27:51  hcl
   premier commit....

   Revision 1.2  2000/01/25 16:05:07  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */
#include <stdlib.h>
#include <stdio.h>
#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "mph.h"
#include "mpu.h"
#include "ocr.h"
#include "ocrWRoutingDataBase.h"
#include "ocrWindow.h"
#include "mbk_tree.h"

static char *res_id =
    "$Id: mbk_tree.c,v 1.2 2009/06/14 13:51:59 ludo Exp $";

/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/

rbtree rbtreeNil = { NULL,      /* KEY */
    NULL,                       /* DATA */
    rbtreeblack,                /* COLOUR */
    &rbtreeNil,                 /* PARENT */
    &rbtreeNil,                 /* Left */
    &rbtreeNil
};                              /* Right */

rbtree *HEAD_RBTREE = NULL;     /* red-black tree buffer head */

/*****************************************************************************/
/*                            Static functions                               */
/*****************************************************************************/

// WARNING : no mbk_free !

/** default comparison function **/

static inline int static_defaultcompare(void *first, void *second)
{

    return ((long) first ==
            (long) second) ? 0 : (((long) first > (long) second) ? 1 : -1);
}

/** red-black tree node creation function **/

static inline rbtree *static_createrbtreeelement(void *key,
                                                 void *data,
                                                 rbtreecolour colour,
                                                 rbtree * parent,
                                                 rbtree * left,
                                                 rbtree * right)
{
    rbtree *pt;
    register unsigned long i;

    // allocation blocs par blocs

    if (HEAD_RBTREE == NULL) {
        pt = (rbtree *) mbkalloc(BUFSIZE * sizeof(rbtree));
        HEAD_RBTREE = pt;
        for (i = 1; i < BUFSIZE; i++) {
            pt->PARENT = pt + 1;
            pt++;
        }
        pt->PARENT = NULL;
    }
    // mise à jour du noeud

    pt = HEAD_RBTREE;
    HEAD_RBTREE = HEAD_RBTREE->PARENT;
    pt->KEY = key;
    pt->DATA = data;
    pt->COLOUR = colour;
    pt->PARENT = parent;
    pt->Left = left;
    pt->Right = right;
    return pt;
}

/** red-black tree node destruction function **/

static inline void static_freerbtreeelement(rbtree * node)
{

    // libère le noeud
    // pas de free voir static_createrbtreeelement
    node->PARENT = HEAD_RBTREE;
    HEAD_RBTREE = node;
}

/** red-black tree left rotate function **/

static inline rbtree *static_leftrotaterbtree(rbtree * root, rbtree * node)
{
    rbtree *son = node->Right;

    node->Right = son->Left;
    if (son->Left != RBTREENULL)
        son->Left->PARENT = node;
    son->PARENT = node->PARENT;
    son->Left = node;
    node->PARENT = son;
    if (son->PARENT == RBTREENULL)
        return son;
    else {
        if (son->PARENT->Left == node)
            son->PARENT->Left = son;
        else
            son->PARENT->Right = son;
        return root;
    }
}

/** red-black tree right rotate function **/

static inline rbtree *static_rightrotaterbtree(rbtree * root,
                                               rbtree * node)
{
    rbtree *son = node->Left;

    node->Left = son->Right;
    if (son->Right != RBTREENULL)
        son->Right->PARENT = node;
    son->PARENT = node->PARENT;
    son->Right = node;
    node->PARENT = son;
    if (son->PARENT == RBTREENULL)
        return son;
    else {
        if (son->PARENT->Left == node)
            son->PARENT->Left = son;
        else
            son->PARENT->Right = son;
        return root;
    }
}

/** balance red-black tree after adding node **/

static inline rbtree *static_balanceaddrbtree(rbtree * root, rbtree * node)
{
    rbtree *uncle;

    while ((node != root) && (node->PARENT->COLOUR == rbtreered)) {
        if (node->PARENT == node->PARENT->PARENT->Left) {
            uncle = node->PARENT->PARENT->Right;
            if (uncle->COLOUR == rbtreered) {
                node->PARENT->COLOUR = rbtreeblack;
                uncle->COLOUR = rbtreeblack;
                node->PARENT->PARENT->COLOUR = rbtreered;
                node = node->PARENT->PARENT;
            } else {
                if (node == node->PARENT->Right) {
                    node = node->PARENT;
                    root = static_leftrotaterbtree(root, node);
                }
                node->PARENT->COLOUR = rbtreeblack;
                node->PARENT->PARENT->COLOUR = rbtreered;
                root =
                    static_rightrotaterbtree(root, node->PARENT->PARENT);
            }
        } else {
            uncle = node->PARENT->PARENT->Left;
            if (uncle->COLOUR == rbtreered) {
                node->PARENT->COLOUR = rbtreeblack;
                uncle->COLOUR = rbtreeblack;
                node->PARENT->PARENT->COLOUR = rbtreered;
                node = node->PARENT->PARENT;
            } else {
                if (node == node->PARENT->Left) {
                    node = node->PARENT;
                    root = static_rightrotaterbtree(root, node);
                }
                node->PARENT->COLOUR = rbtreeblack;
                node->PARENT->PARENT->COLOUR = rbtreered;
                root = static_leftrotaterbtree(root, node->PARENT->PARENT);
            }
        }
    }
    root->COLOUR = rbtreeblack;
    return root;
}

/** balance red-black tree after deleting node **/

static inline rbtree *static_balancedelrbtree(rbtree * root, rbtree * node)
{
    rbtree *brother;

    while ((node != root) && (node->COLOUR == rbtreeblack)) {
        if (node == node->PARENT->Left) {
            brother = node->PARENT->Right;
            if (brother->COLOUR == rbtreered) {
                brother->COLOUR = rbtreeblack;
                node->PARENT->COLOUR = rbtreered;
                root = static_leftrotaterbtree(root, node->PARENT);
                brother = node->PARENT->Right;
            }
            if ((brother->Left->COLOUR == rbtreeblack)
                && (brother->Right->COLOUR == rbtreeblack)) {
                brother->COLOUR = rbtreered;
                node = node->PARENT;
            } else {
                if (brother->Right->COLOUR == rbtreeblack) {
                    brother->Left->COLOUR = rbtreeblack;
                    brother->COLOUR = rbtreered;
                    root = static_rightrotaterbtree(root, brother);
                    brother = node->PARENT->Right;
                }
                brother->COLOUR = node->PARENT->COLOUR;
                node->PARENT->COLOUR = rbtreeblack;
                brother->Right->COLOUR = rbtreeblack;
                root = static_leftrotaterbtree(root, node->PARENT);
                node = root;    /* in order to break off looping */
            }
        } else {
            brother = node->PARENT->Left;
            if (brother->COLOUR == rbtreered) {
                brother->COLOUR = rbtreeblack;
                node->PARENT->COLOUR = rbtreered;
                root = static_rightrotaterbtree(root, node->PARENT);
                brother = node->PARENT->Left;
            }
            if ((brother->Left->COLOUR == rbtreeblack)
                && (brother->Right->COLOUR == rbtreeblack)) {
                brother->COLOUR = rbtreered;
                node = node->PARENT;
            } else {
                if (brother->Left->COLOUR == rbtreeblack) {
                    brother->Right->COLOUR = rbtreeblack;
                    brother->COLOUR = rbtreered;
                    root = static_leftrotaterbtree(root, brother);
                    brother = node->PARENT->Left;
                }
                brother->COLOUR = node->PARENT->COLOUR;
                node->PARENT->COLOUR = rbtreeblack;
                brother->Left->COLOUR = rbtreeblack;
                root = static_rightrotaterbtree(root, node->PARENT);
                node = root;    /* in order to break off looping */
            }
        }
    }
    node->COLOUR = rbtreeblack;
    return root;
}

/*****************************************************************************/
/*                            Library functions                              */
/*****************************************************************************/

/** find a red-black tree node **/

rbtree *getrbtree(rbtree * root, void *key, int (*cmp) (void *, void *))
{
    int res;

    if (!cmp)
        cmp = static_defaultcompare;
    if (!root)
        return RBTREENULL;
    while ((root != RBTREENULL) && ((res = cmp(key, root->KEY)) != 0)) {
        if (res < 0)
            root = root->Left;
        else
            root = root->Right;
    }
    return root;
}

/** find a red-black tree node and previous and next nodes **/

rbtree *getrbtree2(rbtree * root, void *key, rbtree ** previous,
                   rbtree ** next, int (*cmp) (void *, void *))
{
    int res;

    *previous = RBTREENULL;
    *next = RBTREENULL;
    if (!cmp)
        cmp = static_defaultcompare;
    if (!root)
        return RBTREENULL;
    while ((root != RBTREENULL) && ((res = cmp(key, root->KEY)) != 0)) {
        if (res < 0) {
            *next = root;
            root = root->Left;
        } else {
            *previous = root;
            root = root->Right;
        }
    }
    if (root != RBTREENULL) {
        if (root->Left != RBTREENULL)
            for (*previous = root->Left; (*previous)->Right != RBTREENULL;
                 *previous = (*previous)->Right);
        if (root->Right != RBTREENULL)
            for (*next = root->Right; (*next)->Left != RBTREENULL;
                 *next = (*next)->Left);
    }
    return root;
}

/** get the most significant red-black tree node **/

rbtree *getmostrbtree(rbtree * root)
{
    rbtree *parent = RBTREENULL;

    if (!root)
        return RBTREENULL;
    while (root != RBTREENULL) {
        parent = root;
        root = root->Left;
    }
    return parent;
}

/** get the least significant red-black tree node **/

rbtree *getleastrbtree(rbtree * root)
{
    rbtree *parent = RBTREENULL;

    if (!root)
        return RBTREENULL;
    while (root != RBTREENULL) {
        parent = root;
        root = root->Right;
    }
    return parent;
}

/** get previous red-black tree node **/

rbtree *getpreviousrbtree(rbtree * node)
{
    rbtree *parent;

    if ((!node) || (node == RBTREENULL))
        return RBTREENULL;
    if (node->Left != RBTREENULL) {
        for (node = node->Left; node->Right != RBTREENULL;
             node = node->Right);
        return node;
    }
    parent = node->PARENT;
    while ((parent != RBTREENULL) && (node == parent->Left)) {
        node = parent;
        parent = node->PARENT;
    }
    return parent;
}

/** get next red-black tree node **/

rbtree *getnextrbtree(rbtree * node)
{
    rbtree *parent;

    if ((!node) || (node == RBTREENULL))
        return RBTREENULL;
    if (node->Right != RBTREENULL) {
        for (node = node->Right; node->Left != RBTREENULL;
             node = node->Left);
        return node;
    }
    parent = node->PARENT;
    while ((parent != RBTREENULL) && (node == parent->Right)) {
        node = parent;
        parent = node->PARENT;
    }
    return parent;
}

/** add a node in a red-black tree **/

rbtree *addrbtree(rbtree * root, void *key, void *data,
                  int (*cmp) (void *, void *))
{
    rbtree *node = (root ? root : RBTREENULL);
    rbtree *parent = RBTREENULL;
    int res = 0;                /* warning: `res' might be used uninitialized in this function */

    if (!cmp)
        cmp = static_defaultcompare;
    while (node != RBTREENULL) {
        parent = node;
        if ((res = cmp(key, node->KEY)) < 0)
            node = node->Left;
        else
            node = node->Right;
    }
    if (parent == RBTREENULL)
        return static_createrbtreeelement(key, data, rbtreeblack,
                                          RBTREENULL, RBTREENULL,
                                          RBTREENULL);
    else {
        node =
            static_createrbtreeelement(key, data, rbtreered, parent,
                                       RBTREENULL, RBTREENULL);
        if (res < 0)
            parent->Left = node;
        else
            parent->Right = node;
        return static_balanceaddrbtree(root, node);
    }
}

/** add a son to a node in a red-black tree **/

rbtree *addrbtree2(rbtree * root, rbtree * parent, void *key, void *data,
                   int (*cmp) (void *, void *))
{
    rbtree *node;

    if (!cmp)
        cmp = static_defaultcompare;
    if ((!root || (root == RBTREENULL)) && parent
        && (parent != RBTREENULL)) {
        fflush(stdout);
        fprintf(stderr, "*** mbk error ***");
        fprintf(stderr,
                "  addrbtree2() impossible : root pointer = NULL/RBTREENULL and parent pointer != NULL/RBTREENULL !\n");
        exit(1);
    }
    if (root && (root != RBTREENULL)
        && (!parent || (parent == RBTREENULL))) {
        fflush(stdout);
        fprintf(stderr, "*** mbk error ***");
        fprintf(stderr,
                "  addrbtree2() impossible : parent pointer = NULL/RBTREENULL and root pointer != NULL/RBTREENULL !\n");
        exit(1);
    }
    if (!parent || (parent == RBTREENULL))
        return static_createrbtreeelement(key, data, rbtreeblack,
                                          RBTREENULL, RBTREENULL,
                                          RBTREENULL);
    else {
        node =
            static_createrbtreeelement(key, data, rbtreered, parent,
                                       RBTREENULL, RBTREENULL);
        if (cmp(key, parent->KEY) < 0) {
            if (parent->Left == RBTREENULL)
                parent->Left = node;
            else {
                fflush(stdout);
                fprintf(stderr, "*** mbk error ***");
                fprintf(stderr,
                        "  addrbtree2() impossible : son pointer field != RBTREENULL !\n");
                exit(1);
            }
        } else {
            if (parent->Right == RBTREENULL)
                parent->Right = node;
            else {
                fflush(stdout);
                fprintf(stderr, "*** mbk error ***");
                fprintf(stderr,
                        "  addrbtree2() impossible : son pointer field != RBTREENULL !\n");
                exit(1);
            }
        }
        return static_balanceaddrbtree(root, node);
    }
}

/** delete a node in a red-black tree **/

rbtree *delrbtree(rbtree * root, rbtree * node)
{
    rbtree *del;
    rbtree *son;

    if (!root || !node) {
        fflush(stdout);
        fprintf(stderr, "*** mbk error ***");
        fprintf(stderr, "  delrbtree() impossible : pointer = NULL !\n");
        exit(1);
    }
    if ((root == RBTREENULL) || (node == RBTREENULL)) {
        //fflush(stdout);
        //fprintf(stderr, "*** mbk error ***");
        //fprintf(stderr,
        //        "  delrbtree() impossible : pointer = RBTREENULL !\n");
/*
    son = NULL;
	 del = son->Left;
*/
        return root;
    }
    if ((node->Left == RBTREENULL) || (node->Right == RBTREENULL))
        del = node;
    else {
        del = getnextrbtree(node);
        node->KEY = del->KEY;
        node->DATA = del->DATA;
    }
    if (del->Left != RBTREENULL)
        son = del->Left;
    else
        son = del->Right;
    son->PARENT = del->PARENT;
    if (del->PARENT == RBTREENULL) {
        static_freerbtreeelement(del);
        son->COLOUR = rbtreeblack;
        return son;
    } else {
        if (del == del->PARENT->Left)
            del->PARENT->Left = son;
        else
            del->PARENT->Right = son;
        if (del->COLOUR == rbtreeblack)
            root = static_balancedelrbtree(root, son);
        static_freerbtreeelement(del);
        return root;
    }
}

/** free a red-black tree **/

void freerbtree(rbtree * root, void (*freekey) (void *),
                void (*freedata) (void *))
{
    rbtree *left;
    rbtree *right;

    if (root && (root != RBTREENULL)) {
        left = root->Left;
        right = root->Right;
        if (freekey)
            freekey(root->KEY);
        if (freedata)
            freedata(root->DATA);
        static_freerbtreeelement(root);
        freerbtree(left, freekey, freedata);    /* TAG : RECURSIVE */
        freerbtree(right, freekey, freedata);   /* TAG : RECURSIVE */
    }
}

/** map a void function on a red-black tree from the most significant node to
    the least significant node **/

void mapmosttoleastrbtree(rbtree * root, void (*fun) (rbtree *))
{

    if (!fun) {
        fflush(stdout);
        fprintf(stderr, "*** mbk error ***");
        fprintf(stderr,
                "  mapmosttoleastrbtree() impossible : function = NULL !\n");
        exit(1);
    }
    if (root && (root != RBTREENULL)) {
        mapmosttoleastrbtree(root->Left, fun);  /* TAG : RECURSIVE */
        fun(root);
        mapmosttoleastrbtree(root->Right, fun); /* TAG : RECURSIVE */
    }
}

/** map a void function on a red-black tree from the least significant node to
    the most significant node **/

void mapleasttomostrbtree(rbtree * root, void (*fun) (rbtree *))
{

    if (!fun) {
        fflush(stdout);
        fprintf(stderr, "*** mbk error ***");
        fprintf(stderr,
                "  mapleasttomostrbtree() impossible : function = NULL !\n");
        exit(1);
    }
    if (root && (root != RBTREENULL)) {
        mapleasttomostrbtree(root->Right, fun); /* TAG : RECURSIVE */
        fun(root);
        mapleasttomostrbtree(root->Left, fun);  /* TAG : RECURSIVE */
    }
}

/** map a function on a red-black tree from the most significant node to the
    least significant node **/

void *mapmosttoleastrbtree2(rbtree * root, void *(*fun) (rbtree *, void *),
                            void *seed)
{


    if (!fun) {
        fflush(stdout);
        fprintf(stderr, "*** mbk error ***");
        fprintf(stderr,
                "  mapmosttoleastrbtree2() impossible : function = NULL !\n");
        exit(1);
    }
    if (root && (root != RBTREENULL)) {
        seed = mapmosttoleastrbtree2(root->Left, fun, seed);    /* TAG : RECURSIVE */
        seed = fun(root, seed);
        seed = mapmosttoleastrbtree2(root->Right, fun, seed);   /* TAG : RECURSIVE */
    }
    return seed;
}

/** map a function on a red-black tree from the least significant node to the
    most significant node **/

void *mapleasttomostrbtree2(rbtree * root, void *(*fun) (rbtree *, void *),
                            void *seed)
{


    if (!fun) {
        fflush(stdout);
        fprintf(stderr, "*** mbk error ***");
        fprintf(stderr,
                "  mapleasttomostrbtree2() impossible : function = NULL !\n");
        exit(1);
    }
    if (root && (root != RBTREENULL)) {
        seed = mapleasttomostrbtree2(root->Right, fun, seed);   /* TAG : RECURSIVE */
        seed = fun(root, seed);
        seed = mapleasttomostrbtree2(root->Left, fun, seed);    /* TAG : RECURSIVE */
    }
    return seed;
}
