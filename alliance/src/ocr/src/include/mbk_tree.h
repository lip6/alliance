#ifndef _MBK_TREE_H_
#define _MBK_TREE_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:14 $
   $Log: mbk_tree.h,v $
   Revision 1.1  2002/03/15 14:37:14  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:11  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   Revision 1.2  2000/01/25 16:05:07  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */

extern rbtree rbtreeNil;

#define RBTREENULL          &rbtreeNil     /* Nil sentinel */

extern rbtree *getrbtree(rbtree *root, void *key, int (*cmp) (void *, void *));
extern rbtree *getrbtree2(rbtree *root, void *key, rbtree **previous, rbtree **next, int (*cmp) (void *, void *));
extern rbtree *getmostrbtree(rbtree *root);
extern rbtree *getleastrbtree(rbtree *root);
extern rbtree *getpreviousrbtree(rbtree *node);

extern rbtree *getnextrbtree(rbtree *node);
extern rbtree *addrbtree(rbtree *root, void *key, void *data, int (*cmp) (void *, void *));
extern rbtree *addrbtree2(rbtree *root, rbtree *parent, void *key, void *data, int (*cmp) (void *, void *));
extern rbtree *delrbtree(rbtree *root, rbtree *node);
extern void freerbtree(rbtree *root, void (*freekey) (void *), void (*freedata) (void *));

/* Fonctions static internes !
extern void mapmosttoleastrbtree(rbtree *root, void (*fun) (rbtree *));
extern void mapleasttomostrbtree(rbtree *root, void (*fun) (rbtree *));
extern void *mapmosttoleastrbtree2(rbtree *root, void *(*fun) (rbtree *, void *), void *seed);
extern void *mapleasttomostrbtree2(rbtree *root, void *(*fun) (rbtree *, void *), void *seed);
*/
#endif /* _MBK_TREE_H_ */
