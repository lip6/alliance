/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for BDD         |
|                                                       |
|  Date    :            19.06.2000                      |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef BDD_105_H
# define BDD_105_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                  Hash Oper Constants                  |
|                                                       |
\------------------------------------------------------*/

# define BDD_OPER_RESTRICT    ((long)0x00010000)
# define BDD_OPER_COMPOSE     ((long)0x00020000)
# define BDD_OPER_SIMP_DC_ON  ((long)0x00040000)
# define BDD_OPER_SIMP_DC_OFF ((long)0x00080000)
# define BDD_OPER_INTERSECT   ((long)0x00100000)
# define BDD_OPER_COFACTOR    ((long)0x00200000)
# define BDD_OPER_SUBSTITUTE  ((long)0x00400000)
# define BDD_OPER_EXIST       ((long)0x00800000)
# define BDD_OPER_EXIST_MISS  ((long)0x01000000)
# define BDD_OPER_REDUCE      ((long)0x02000000)
# define BDD_OPER_REL_PROD    ((long)0x04000000)

# define BDD_OPER_MASK        ((long)0x03FF0000)

/*------------------------------------------------------\
|                                                       |
|                       Bdd Hash Node                   |
|                                                       |
\------------------------------------------------------*/

# define BDD_HASH_NODE_EMPTY   ((bddhnode)0)
# define BDD_HASH_NODE_DELETED ((bddhnode)1)

/*------------------------------------------------------\
|                                                       |
|                        Bdd Index                      |
|                                                       |
\------------------------------------------------------*/

# define BDD_INDEX_ZERO   ((bddindex) 0)
# define BDD_INDEX_ONE    ((bddindex) 1)
# define BDD_INDEX_MIN    ((bddindex) 2)

# define BDD_MAX_INDEX    ((bddindex)-1)

/*------------------------------------------------------\
|                                                       |
|                        Bdd Short                      |
|                                                       |
\------------------------------------------------------*/

# define BDD_MAX_SHORT    ((bddshort)-1)

/*------------------------------------------------------\
|                                                       |
|                        Bdd Variable                   |
|                                                       |
\------------------------------------------------------*/

# define BDD_MAX_VAR  ( ((bddvar)-1) - BDD_INDEX_MIN )
# define BDD_MIN_VAR    ((bddvar)-1)

/*------------------------------------------------------\
|                                                       |
|                     Bdd Reference                     |
|                                                       |
\------------------------------------------------------*/

# define BDD_MAX_REF     ((bddref)-1)

/*------------------------------------------------------\
|                                                       |
|                       Bdd Flag                        |
|                                                       |
\------------------------------------------------------*/

# define BDD_FLAG_NUM_MASK      (bddflag)( 0x0001 )
# define BDD_MAX_FLAG           (bddflag)( -1     )

/*------------------------------------------------------\
|                                                       |
|                       Support Type                    |
|                                                       |
\------------------------------------------------------*/

# define BDD_SUPPORT_CHAIN     0
# define BDD_SUPPORT_PTYPE     1

/*------------------------------------------------------\
|                                                       |
|                     Add Input Mode                    |
|                                                       |
\------------------------------------------------------*/

# define BDD_IN_MODE_FIRST      0x00
# define BDD_IN_MODE_LAST       0x01
# define BDD_IN_MODE_BEFORE     0x02
# define BDD_IN_MODE_AFTER      0x03
# define BDD_IN_MODE_IMPOSE     0x04

/*------------------------------------------------------\
|                                                       |
|                     Add Auxiliar Mode                 |
|                                                       |
\------------------------------------------------------*/

# define BDD_AUX_MODE_SINGLE     0x00
# define BDD_AUX_MODE_GLOBAL     0x01

/*------------------------------------------------------\
|                                                       |
|                      User Function Type               |
|                                                       |
\------------------------------------------------------*/

# define BDD_FUNC_RESET_HASH_OPER    0

/*------------------------------------------------------\
|                                                       |
|                    Bdd System Flags                   |
|                                                       |
\------------------------------------------------------*/

# define BDD_SYSTEM_REORDER_ON_MASK          0x0001
# define BDD_SYSTEM_REORDER_MASK             0x0002
# define BDD_SYSTEM_EXPLOSION_ON_MASK        0x0004
# define BDD_SYSTEM_EXPLOSION_MASK           0x0008
# define BDD_SYSTEM_SWAP_VAR_MASK            0x0010
# define BDD_SYSTEM_NO_WARNING_MASK          0x0020

/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/*------------------------------------------------------\
|                                                       |
|                       Bdd System Mask                 |
|                                                       |
\------------------------------------------------------*/

# define SetBddSystemReorderOn( S )   ( (S)->FLAGS |= BDD_SYSTEM_REORDER_ON_MASK )
# define IsBddSystemReorderOn( S )    ( (S)->FLAGS &  BDD_SYSTEM_REORDER_ON_MASK )
# define ClearBddSystemReorderOn( S ) ( (S)->FLAGS &= ~BDD_SYSTEM_REORDER_ON_MASK)

# define SetBddSystemReorder( S )   ( (S)->FLAGS |= BDD_SYSTEM_REORDER_MASK )
# define IsBddSystemReorder( S )    ( (S)->FLAGS &  BDD_SYSTEM_REORDER_MASK )
# define ClearBddSystemReorder( S ) ( (S)->FLAGS &= ~BDD_SYSTEM_REORDER_MASK)

# define SetBddSystemExplosionOn( S )   ( (S)->FLAGS |= BDD_SYSTEM_EXPLOSION_ON_MASK )
# define IsBddSystemExplosionOn( S )    ( (S)->FLAGS &  BDD_SYSTEM_EXPLOSION_ON_MASK )
# define ClearBddSystemExplosionOn( S ) ( (S)->FLAGS &= ~BDD_SYSTEM_EXPLOSION_ON_MASK)

# define SetBddSystemExplosion( S ) ( (S)->FLAGS |= BDD_SYSTEM_EXPLOSION_MASK )
# define IsBddSystemExplosion( S )  ( (S)->FLAGS &  BDD_SYSTEM_EXPLOSION_MASK )
# define ClearBddSystemExplosion( S ) ((S)->FLAGS &= ~BDD_SYSTEM_EXPLOSION_MASK)

# define SetBddSystemSwapVar( S )   ( (S)->FLAGS |= BDD_SYSTEM_SWAP_VAR_MASK )
# define IsBddSystemSwapVar( S )    ( (S)->FLAGS &  BDD_SYSTEM_SWAP_VAR_MASK )
# define ClearBddSystemSwapVar( S ) ( (S)->FLAGS &= ~BDD_SYSTEM_SWAP_VAR_MASK)

# define SetBddSystemNoWarning( S )   ( (S)->FLAGS |= BDD_SYSTEM_NO_WARNING_MASK )
# define IsBddSystemNoWarning( S )    ( (S)->FLAGS &  BDD_SYSTEM_NO_WARNING_MASK )
# define ClearBddSystemNoWarning( S ) ( (S)->FLAGS &= ~BDD_SYSTEM_NO_WARNING_MASK)

/*------------------------------------------------------\
|                                                       |
|                    Bdd Local System                   |
|                                                       |
\------------------------------------------------------*/

# define setbddlocalsystem( S ) \
                                \
  do { if ( (S) ) BddLocalSystem = (S); } while (0)

/*------------------------------------------------------\
|                                                       |
|                    Bdd Local Circuit                  |
|                                                       |
\------------------------------------------------------*/

# define setbddlocalcircuit( C ) \
                                 \
  do { if ( (C) ) { BddLocalCircuit = (C); \
                    BddLocalSystem  = (C)->BDD_SYSTEM; } } while (0)

/*------------------------------------------------------\
|                                                       |
|                      Bdd Support                      |
|                                                       |
\------------------------------------------------------*/

# define getbddnodesupportchain( S, N ) \
                                        \
    ((chain_list *)getbddnodesupport( (S), (N), BDD_SUPPORT_CHAIN ))

# define getbddnodesupportptype( S, N ) \
                                        \
    ((ptype_list *)getbddnodesupport( (S), (N), BDD_SUPPORT_PTYPE ))

/*------------------------------------------------------\
|                                                       |
|                         Type                          |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                 Index Ref Mark and Var                |
|                                                       |
\------------------------------------------------------*/

  typedef unsigned short bddindex;
  typedef unsigned short bddref;
  typedef unsigned char  bddmark;
  typedef unsigned char  bddflag;
  typedef unsigned short bddvar;
  typedef unsigned short bddshort;

/*------------------------------------------------------\
|                                                       |
|                       Bdd Alloc Info                  |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddallocinfo 
  {
    long  CIRCUIT;
    long  SYSTEM;
    long  BLOCK;
    long  NODE_BLOCK;
    long  HASH_NODE_TABLE;
    long  HASH_NODE;
    long  HASH_OPER_TABLE;
    long  HASH_OPER;
    long  VAR_TREE;
    long  VAR_CHILD;
    long  VAR;
    long  VAR_NODE;
    long  INDEX_NODE;
    long  INDEX;
    long  ASSOC;
    long  ASSOC_NODE;
    long  NAME_IN;
    long  INDEX_IN;
    long  HEATH;
    long  USER_FUNC;

  } bddallocinfo;

/*------------------------------------------------------\
|                                                       |
|                       Bdd User Function               |
|                                                       |
\------------------------------------------------------*/

  typedef struct bdduserfunc
  {
    struct bdduserfunc *NEXT;
    void              (*FUNC)();
    void               *DATA;
    long                TYPE;

  } bdduserfunc;

/*------------------------------------------------------\
|                                                       |
|                       Bdd Node                        |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddnode
  {
    struct bddnode *HIGH;
    struct bddnode *LOW;
    bddindex        INDEX;
    bddmark         MARK;
    bddflag         FLAG;
    bddref          REF_EXT;
    bddref          REF_INT;

  } bddnode;

/*------------------------------------------------------\
|                                                       |
|                       Bdd Hash Node                   |
|                                                       |
\------------------------------------------------------*/

  typedef bddnode *bddhnode;

/*------------------------------------------------------\
|                                                       |
|                       Bdd Block                       |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddblock
  {
    struct bddblock *NEXT;
    struct bddnode  *NODE;
    long             NODE_FREE;

  } bddblock;

/*------------------------------------------------------\
|                                                       |
|                    Bdd Hash Node Table                |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddhnodetable
  {
    bddhnode *TABLE;
    long      TABLE_SIZE;
    long      NUMBER_NODE;
    long      NUMBER_ADD;
    long      NUMBER_SCAN;
    long      NUMBER_STRETCH;
    long      NUMBER_RESIZE;
    
  } bddhnodetable;

/*------------------------------------------------------\
|                                                       |
|                       Bdd Hash Oper                   |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddhoper
  {
    bddnode *LEFT;
    bddnode *RIGHT;
    bddnode *FATHER;
    long     OPERATOR;

  } bddhoper;

/*------------------------------------------------------\
|                                                       |
|                    Bdd Hash Oper Table                |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddhopertable
  {
    bddhoper *TABLE;
    long      TABLE_SIZE;
    long      TABLE_FREEZE;
    long      NUMBER_OPER;
    long      NUMBER_ADD;
    long      NUMBER_HIT;
    long      NUMBER_MISS;
    long      NUMBER_RESET;

  } bddhopertable;

/*------------------------------------------------------\
|                                                       |
|                   Bdd Variable Node                   |
|                                                       |
\------------------------------------------------------*/

  typedef bddnode    *bddvarnode;

/*------------------------------------------------------\
|                                                       |
|                    Bdd Index Node                     |
|                                                       |
\------------------------------------------------------*/

  typedef bddhnodetable *bddindexnode;

/*------------------------------------------------------\
|                                                       |
|                     Bdd Assoc Node                    |
|                                                       |
\------------------------------------------------------*/

  typedef bddnode  *bddassocnode;

/*------------------------------------------------------\
|                                                       |
|                       Bdd Assoc                       |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddassoc
  {
    struct bddassoc *NEXT;
    bddassocnode    *ASSOC_NODE;
    bddvar           FIRST;
    bddvar           LAST;
    bddshort         IDENT;
    bddshort         CACHE;

  } bddassoc;

/*------------------------------------------------------\
|                                                       |
|                       Bdd System                      |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddsystem
  {
    bddblock       *BLOCK;
    bddnode        *NODE_FREE;
    long            NUMBER_FREE;

    bddindexnode   *INDEX_NODE;
    bddvarnode     *VAR_NODE;
    bddindex       *VAR_TO_INDEX;
    bddvar         *INDEX_TO_VAR;


    long            NUMBER_NODE;
    long            MAX_NODE;
    long            TOP_NODE;

    long            NUMBER_VAR;
    long            MAX_VAR;

    long            NUMBER_INDEX;
    long            MAX_INDEX;

    bddassoc       *ASSOC;
    unsigned char   ASSOC_RESET;

    bddhopertable  *HASH_OPER;

    bddnode        *ONE;
    bddnode        *ZERO;

    bddmark         MARK;
    long            FLAGS;

    void          (*REORDER_FUNC)();
    long            REORDER_LOW;
    long            REORDER_LIMIT;
    long            REORDER_RATIO;

    bdduserfunc    *USER_FUNC;

    long            VAR_MODEL;
    long            OPER_MODEL;

    void          (*EXPLOSION_FUNC)();
    long            EXPLOSION_LIMIT;

  } bddsystem;

/*------------------------------------------------------\
|                                                       |
|                     Bdd Name Input                    |
|                                                       |
\------------------------------------------------------*/

  typedef char *bddnamein;

/*------------------------------------------------------\
|                                                       |
|                       Bdd Circuit                     |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddcircuit 
  {
    char       *NAME;

    authtable  *HASH_IN;
    authtable  *HASH_OUT;

    bddnamein  *NAME_IN;
    long        NAME_IN_SIZE;

    bddindex   *INDEX_IN;
    long        INDEX_IN_SIZE;

    long        NUMBER_NAME_IN;
    long        NUMBER_NAME_OUT;

    bddsystem  *BDD_SYSTEM;

  } bddcircuit;

/*------------------------------------------------------\
|                                                       |
|                       Bdd Heath                       |
|                                                       |
\------------------------------------------------------*/

  typedef struct bddheath
  {
    struct bddheath *NEXT;
    struct bddnode  *VAR;
    struct bddnode  *MINUS;
    struct bddnode  *PLUS;

  } bddheath;

/*------------------------------------------------------\
|                                                       |
|                     Global Variables                  |
|                                                       |
\------------------------------------------------------*/

  extern long BDD_HNODE_STRETCH_FACTOR;
  extern long BDD_HNODE_RESIZE_FACTOR;
  extern long BDD_HNODE_MIN_FILL_FACTOR;
  extern long BDD_HNODE_MAX_SCAN;

  extern bddsystem    *BddLocalSystem;
  extern bddcircuit   *BddLocalCircuit;
  extern bddallocinfo  BddAllocInfo;

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                       Env Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  bddenv __P(());

/*------------------------------------------------------\
|                                                       |
|                       Alloc Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern    bddcircuit * allocbddcircuit __P(());
  extern     bddsystem * allocbddsystem __P(());

  extern      bddblock * allocbddblock __P(());
  extern       bddnode * allocbddnodeblock __P((long Number));
  extern bddhnodetable * allocbddhnodetable __P(());
  extern      bddhnode * allocbddhnode __P((long Number));
  extern bddhopertable * allocbddhopertable __P(());
  extern      bddhoper * allocbddhoper __P((long Number));

  extern        bddvar * allocbddvar __P((long Number));
  extern    bddvarnode * allocbddvarnode __P((long Number));
  extern  bddindexnode * allocbddindexnode __P((long Number));
  extern      bddindex * allocbddindex __P((long Number));
  extern     bddnamein * allocbddnamein __P((long Number));
  extern      bddindex * allocbddindexin __P((long Number));

  extern      bddassoc * allocbddassoc __P(());
  extern  bddassocnode * allocbddassocnode __P((long Number));

  extern      bddheath * allocbddheath __P(());
  extern   bdduserfunc * allocbdduserfunc __P(());

  extern           void  viewbddallocinfo __P(());

/*------------------------------------------------------\
|                                                       |
|                      Resize Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern        bddvar * resizebddvar __P((bddvar *BddVar, long OldNum, long NewNum));
  extern    bddvarnode * resizebddvarnode __P((bddvarnode *BddVarNode, long OldNum, long NewNum));
  extern  bddindexnode * resizebddindexnode __P((bddindexnode *BddIndexNode, long OldNum, long NewNum));
  extern      bddindex * resizebddindex __P((bddindex *BddIndex, long OldNum, long NewNum));
  extern  bddassocnode * resizebddassocnode __P((bddassocnode *BddAssocNode, long OldNum, long NewNum));
  extern     bddnamein * resizebddnamein __P((bddnamein *NameIn, long OldNum, long NewNum));
  extern      bddindex * resizebddindexin __P((bddindex *IndexIn, long OldNum, long NewNum));

/*------------------------------------------------------\
|                                                       |
|                       Free Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern           void  freebddcircuit __P((bddcircuit *BddCircuit));
  extern           void  freebddsystem __P((bddsystem *BddSystem));
  extern           void  freebddblock __P((bddblock *BddBlock));
  extern           void  freebddnodeblock __P((bddnode *BddNode));
  extern           void  freebddhnodetable __P((bddhnodetable *HashTable));
  extern           void  freebddhnode __P((bddhnode *HashNode));
  extern           void  freebddhopertable __P((bddhopertable *HashTable));
  extern           void  freebddhoper __P((bddhoper *HashOper));
  extern           void  freebddvar __P((bddvar *BddVariable));
  extern           void  freebddvarnode __P((bddvarnode *BddVariableNode));
  extern           void  freebddindex __P((bddindex *BddIndex));
  extern           void  freebddindexnode __P((bddindexnode *BddIndexNode));
  extern           void  freebddnamein __P((bddnamein *NameIn));
  extern           void  freebddindexin __P((bddindex *IndexIn));
  extern           void  freebddassoc __P((bddassoc *BddAssoc));
  extern           void  freebddassocnode __P((bddassocnode *BddAssocNode));
  extern           void  freebddheath __P((bddheath *BddHeath));
  extern           void  freebdduserfunc __P((bdduserfunc *BddUserFunc));

/*------------------------------------------------------\
|                                                       |
|                     Circuit Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern    bddcircuit * createbddcircuit __P((char *Name, long NumberIn, long NumberOut, bddsystem *BddSystem));
  extern           void  resetbddcircuit __P((bddcircuit *BddCircuit));
  extern           void  destroybddcircuit __P((bddcircuit *BddCircuit));
  extern           void  viewbddcircuit __P((bddcircuit *BddCircuit, char ViewName));

  extern       bddnode * searchbddcircuitin __P((bddcircuit *BddCircuit, char *InputName));
  extern       bddnode * addbddcircuitin __P((bddcircuit *BddCircuit, char *InputName, bddindex Index, long Mode));
  extern          char * getbddcircuitinname __P((bddcircuit *BddCircuit, bddindex Index ));
  extern       bddnode * addbddcircuitaux __P((bddcircuit *BddCircuit, char *InputName, bddnode *BddAux, int (*UserFunc)(), long Mode));
  extern       bddnode * renamebddcircuitin __P((bddcircuit *BddCircuit, char *InputName, char *NewName));

  extern       bddnode * searchbddcircuitout __P((bddcircuit *BddCircuit, char *OutputName));
  extern       bddnode * addbddcircuitout __P((bddcircuit *BddCircuit, char *OutputName, bddnode *BddNode));
  extern            int  delbddcircuitout __P((bddcircuit *BddCircuit, char *OutputName));

  extern       bddnode * addbddcircuitabl __P((bddcircuit *BddCircuit, ablexpr *Expr));
  extern       ablexpr * convertbddcircuitabl __P((bddcircuit *BddCircuit, bddnode *BddNode));
  extern       ablexpr * convertbddcircuitsumabl __P((bddcircuit *BddCircuit, bddnode *BddNode));

  extern           long  getbddcircuitnumnode __P(( bddcircuit *BddCircuit ));
  extern           long  getbddcircuitsize __P(( bddcircuit *BddCircuit ));
  extern           void  optimizebddcircuit __P(( bddcircuit *BddCircuit, long (*CostFunc)(), float MaxSizeFactor ));

/*------------------------------------------------------\
|                                                       |
|                      System Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern     bddsystem * createbddsystem __P((long ModelVar, long ModelOper, long MaxVar, long MaxNode));
  extern           void  resetbddsystem __P((bddsystem *BddSystem));
  extern           void  destroybddsystem __P((bddsystem *BddSystem));
  extern           void  viewbddsystem __P((bddsystem *BddSystem, char ViewIndex));
  extern           void  viewbddsysteminfo __P((bddsystem *BddSystem));

/*------------------------------------------------------\
|                                                       |
|                      Block  Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern      bddblock * createbddblock __P((bddsystem *BddSystem));
  extern           void  resetbddblock __P((bddsystem *BddSystem));
  extern           void  destroybddblock __P((bddsystem *BddSystem));
  extern           void  viewbddblock __P((bddsystem *BddSystem, void (*FuncView)()));

  extern      bddblock * addbddblock __P((bddsystem *BddSystem));

/*------------------------------------------------------\
|                                                       |
|                       User Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern   bdduserfunc * addbdduserfunc __P((bddsystem *BddSystem, long Type, void (*Func)(), void *Data));
  extern           void  execbdduserfunc __P((bddsystem *BddSystem, long Type));
  extern            int  delbdduserfunc __P((bddsystem *BddSystem, bdduserfunc *BddUserFunc));
  extern           void  destroybdduserfunc __P((bddsystem *BddSystem));

/*------------------------------------------------------\
|                                                       |
|                    Variable Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern       bddindex  newbddvar __P((bddsystem *BddSystem, bddvar Variable));
  extern       bddnode * addbddvar __P((bddsystem *BddSystem, bddvar Variable));
  extern       bddnode * addbddvarfirst __P((bddsystem *BddSystem));
  extern       bddnode * addbddvarlast __P((bddsystem *BddSystem));
  extern       bddnode * addbddvarbefore __P((bddsystem *BddSystem, bddindex Index));
  extern       bddnode * addbddvarafter __P((bddsystem *BddSystem, bddindex Index));

  extern            int  swapbddvar __P((bddsystem *BddSystem, bddvar Variable));
  extern           void  sweepbddvar __P((bddsystem *BddSystem, bddvar Variable, int SweepRef));

  extern       bddindex  getbddvarindex __P((bddsystem *BddSystem, bddvar Variable));
  extern         bddvar  getbddvarbyindex __P((bddsystem *BddSystem, bddindex Index));
  extern       bddnode * getbddvarnode __P((bddsystem *BddSystem, bddvar Variable));
  extern       bddnode * getbddvarnodebyindex __P((bddsystem *BddSystem, bddindex Index));

/*------------------------------------------------------\
|                                                       |
|                Auxiliary Variable Functions           |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * addbddvarauxsingle __P((bddsystem *BddSystem, bddnode *BddAux));
  extern       bddnode * addbddvarauxglobal __P((bddsystem *BddSystem, bddnode *BddAux, int (*UserFunc)()));
  extern bddnode  *addbddvarauxuser();

/*------------------------------------------------------\
|                                                       |
|                     Node  Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * addbddnode __P((bddsystem *BddSystem, bddindex Index, bddnode *High, bddnode *Low));
  extern           void  delbddnode __P((bddsystem *BddSystem, bddnode *BddNode));
  extern           void  viewbddnode __P((bddsystem *BddSystem, bddnode *BddNode));

  extern    chain_list * addbddnodelist __P((bddsystem *BddSystem, chain_list *HeadList, bddnode *BddNode));
  extern           void  delbddnodelist __P((bddsystem *BddSystem, chain_list *HeadList));

/*------------------------------------------------------\
|                                                       |
|                   Reference Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * incbddrefext __P((bddnode *BddNode));
  extern       bddnode * decbddrefext __P((bddnode *BddNode));
  extern       bddnode * incbddrefint __P((bddnode *BddNode));
  extern       bddnode * decbddrefint __P((bddnode *BddNode));

  extern       bddnode * setbddrefext __P((bddnode *BddNode));
  extern       bddnode * unsetbddrefext __P((bddnode *BddNode));

  extern           void  updatebddref __P((bddnode *BddNode));

  extern           void  clearbddsystemrefext __P((bddsystem *BddSystem));
  extern           void  clearbddsystemrefint __P((bddsystem *BddSystem));
  extern           void  clearbddsystemref __P((bddsystem *BddSystem));

/*------------------------------------------------------\
|                                                       |
|                   Association  Functions              |
|                                                       |
\------------------------------------------------------*/

  extern      bddassoc * addbddassoc __P((bddsystem *BddSystem));
  extern            int  delbddassoc __P((bddsystem *BddSystem, bddassoc *BddAssoc));
  extern           void  viewbddassoc __P((bddsystem *BddSystem, bddassoc *BddAssoc, void (*FuncView)()));

  extern           void  resetbddassoc __P((bddsystem *BddSystem, bddassoc *BddAssoc));
  extern           void  destroybddassoc __P((bddsystem *BddSystem));

  extern      bddassoc * addbddnodeassoc __P((bddsystem *BddSystem, bddassoc *BddAssoc, bddvar Variable, bddnode *BddNode));
  extern            int  delbddnodeassoc __P((bddsystem *BddSystem, bddassoc *BddAssoc, bddvar Variable));

/*------------------------------------------------------\
|                                                       |
|                      Mark Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  newbddsystemmark __P(( bddsystem *BddSystem )); 
  extern           void  setbddnodemark __P(( bddsystem *BddSystem, bddnode *BddNode )); 
  extern            int  testbddnodemark __P(( bddsystem *BddSystem, bddnode *BddNode ));

  extern           long  getbddnodenum __P((bddsystem *BddSystem, bddnode *BddNode));
  extern           long  getbddnodesize __P((bddsystem *BddSystem, bddnode *BddNode));

/*------------------------------------------------------\
|                                                       |
|                       Flag Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern   long  flagbddnode __P((bddsystem *BddSystem, bddnode *BddNode, bddflag Flag));
  extern   long  unflagbddnode __P((bddsystem *BddSystem, bddnode *BddNode, bddflag Flag));

/*------------------------------------------------------\
|                                                       |
|                     Check Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern            int  checkbddindex __P((bddsystem *BddSystem, bddindex Index, int Severity));
  extern            int  checkbddvar __P((bddsystem *BddSystem, bddvar Variable, int Severity));
  extern            int  checkbddoper __P((bddsystem *BddSystem, long Oper, int Severity));
  extern            int  checkbddassoc __P((bddsystem *BddSystem, bddassoc *BddAssoc, int Severity));
  extern            int  checkbddmaxnode __P((bddsystem *BddSystem, int Severity));

/*------------------------------------------------------\
|                                                       |
|                   Get Hash Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern           long  getbddhnodesize __P((long Size));
  extern           long  getbddhnodekey __P((bddhnodetable *Table, bddnode *BddNode));
  extern           long  getbddhnodeindex __P((bddhnodetable *Table, bddnode *BddNode, long Index));

  extern           long  getbddhopersize __P((long Size));
  extern           long  getbddhoperkey __P((bddhopertable *Table, long Oper, bddnode *BddLeft, bddnode *BddRight));

/*------------------------------------------------------\
|                                                       |
|                 Check Hash Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern            int  checkbddhnode __P((bddnode *HashNode, int Severity));

/*------------------------------------------------------\
|                                                       |
|                   Set Hash Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern           void  setbddhnodefunc __P((long (*FuncSize)(), long (*FuncKey)(), long (*FuncIndex)()));
  extern           void  setbddhoperfunc __P((long (*FuncSize)(), long (*FuncKey)()));

/*------------------------------------------------------\
|                                                       |
|                   Hash Table Functions                |
|                                                       |
\------------------------------------------------------*/

  extern bddhnodetable * createbddhnodetable __P((long Length));
  extern           void  stretchbddhnodetable __P((bddhnodetable *HashTable));
  extern           void  destroybddhnodetable __P((bddhnodetable *HashTable));
  extern           void  resetbddhnodetable __P((bddhnodetable *HashTable));
  extern           void  viewbddhnodetable __P((bddhnodetable *HashTable, void (*FuncView)()));

  extern bddhopertable * createbddhopertable __P((long Length));
  extern           void  destroybddhopertable __P((bddhopertable *HashTable));
  extern           void  resetbddhopertable __P((bddhopertable *HashTable));
  extern           void  viewbddhopertable __P((bddhopertable *HashTable, void (*FuncView)()));

/*------------------------------------------------------\
|                                                       |
|                    Hash Node Functions                |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * addbddhnode __P((bddhnodetable *HashTable, bddnode *BddNode));
  extern            int  delbddhnode __P((bddhnodetable *HashTable, bddnode *BddNode));
  extern           void  viewbddhnode __P((bddnode *HashNode));

/*------------------------------------------------------\
|                                                       |
|                    Hash Oper Functions                |
|                                                       |
\------------------------------------------------------*/

  extern      bddhoper * addbddhoper __P((bddhopertable *HashTable, long Oper, bddnode *BddLeft, bddnode *BddRight, bddnode *BddFather));
  extern      bddhoper * searchbddhoper __P((bddhopertable *HashTable, long Oper, bddnode *BddLeft, bddnode *BddRight));
  extern           void  viewbddhoper __P((bddhoper *HashOper));

/*------------------------------------------------------\
|                                                       |
|               View Index and Variable Nodes           |
|                                                       |
\------------------------------------------------------*/

  extern           void  viewbddindexnode __P((bddsystem *BddSystem, void (*FuncView)()));
  extern           void  viewbddvarnode __P((bddsystem *BddSystem, void (*FuncView)()));

/*------------------------------------------------------\
|                                                       |
|                      Apply Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * applybddnode __P((bddsystem *BddSystem, long Oper, bddnode *BddNode1, bddnode *BddNode2));
  extern       bddnode * applybddnodenot __P((bddsystem *BddSystem, bddnode *BddNode));
  extern       bddnode * applybddnodeite __P((bddsystem *BddSystem, bddnode *BddIf, bddnode *BddThen, bddnode *BddElse));
  extern       bddnode * applybddnodeterm __P((bddsystem *BddSystem, long Oper, bddindex Index, bddnode *BddNode));
  extern       bddnode * applybddnodelist __P((bddsystem *BddSystem, long Oper, chain_list *BddList));

/*------------------------------------------------------\
|                                                       |
|                    Support Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern          void * getbddnodesupport __P((bddsystem *BddSystem, bddnode *BddNode, int Mode));
  extern            int  isbddvarinsupport __P((bddsystem *BddSystem, bddnode *BddNode, bddvar Variable));

/*------------------------------------------------------\
|                                                       |
|                    Compose Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * restrictbddnode __P((bddsystem *BddSystem, bddnode *BddNode, bddvar Variable, bddnode *BddSubst));
  extern       bddnode * composebddnode __P((bddsystem *BddSystem, bddnode *BddNode, bddvar Variable, bddnode *BddSubst));

/*------------------------------------------------------\
|                                                       |
|                  Substitute Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * substbddnodeassoc __P((bddsystem *BddSystem, bddnode *BddNode, bddassoc *BddAssoc));

/*------------------------------------------------------\
|                                                       |
|          Existential Quantification Functions         |
|                                                       |
\------------------------------------------------------*/

  extern bddnode *forallbddnodeassoc __P((bddsystem *BddSystem, bddnode *BddNode, bddassoc *BddAssoc));
  extern bddnode *forallbddnodemissassoc __P((bddsystem *BddSystem, bddnode *BddNode, bddassoc *BddAssoc));
  extern bddnode *existbddnodeassoc __P((bddsystem *BddSystem, bddnode *BddNode, bddassoc *BddAssoc));
  extern bddnode *existbddnodemissassoc __P((bddsystem *BddSystem, bddnode *BddNode, bddassoc *BddAssoc));

/*------------------------------------------------------\
|                                                       |
|                  Relational Product Functions         |
|                                                       |
\------------------------------------------------------*/

  extern bddnode *relprodbddnodeassoc __P((bddsystem *BddSystem, bddnode *BddNode1, bddnode *BddNode2, bddassoc *BddAssoc));

/*------------------------------------------------------\
|                                                       |
|               Intersect & Imply Functions             |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * implybddnode __P((bddsystem *BddSystem, bddnode *BddNode1, bddnode *BddNode2));
  extern       bddnode * intersectbddnode __P((bddsystem *BddSystem, bddnode *BddNode1, bddnode *BddNode2));

/*------------------------------------------------------\
|                                                       |
|                 Fraction Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern         double  fractionbddnode __P((bddsystem *BddSystem, bddnode *BddNode));

/*------------------------------------------------------\
|                                                       |
|                 Satisfy Functions                     |
|                                                       |
\------------------------------------------------------*/

  extern bddnode * satisfybddnode __P((bddsystem *BddSystem, bddnode *BddNode));
  extern bddnode * satisfybddnodeassoc __P((bddsystem *BddSystem, bddnode *BddNode, bddassoc *BddAssoc));

/*------------------------------------------------------\
|                                                       |
|                    Cofactor Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern bddnode * cofactorbddnode __P((bddsystem *BddSystem, bddnode *BddNode1, bddnode *BddNode2));

/*------------------------------------------------------\
|                                                       |
|                     Reduce Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern bddnode * reducebddnode __P((bddsystem *BddSystem, bddnode *BddNode1, bddnode *BddNode2));

/*------------------------------------------------------\
|                                                       |
|                      Heath Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern      bddheath * getbddheath __P((bddsystem *BddSystem, bddnode *BddNode));
  extern      bddheath * getbddheathvar __P((bddsystem *BddSystem, bddnode *BddNode, bddvar Variable));
  extern           void  delbddheath __P((bddsystem *BddSystem, bddheath *BddHeath));

/*------------------------------------------------------\
|                                                       |
|                    Simplify Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * simpbddnodedcon __P((bddsystem *BddSystem, bddnode *BddOn, bddnode *BddDc));
  extern       bddnode * simpbddnodedcoff __P((bddsystem *BddSystem, bddnode *BddOff, bddnode *BddDc));

/*------------------------------------------------------\
|                                                       |
|                   Conversion Functions                |
|                                                       |
\------------------------------------------------------*/

  extern    chain_list * convertbddnodeabl __P((bddsystem *BddSystem, char **NameArray, bddindex *IndexArray, bddnode *BddNode));
  extern ablexpr    *convertbddnodemuxabl();
  extern    chain_list * convertbddnodesumabl __P((bddsystem *BddSystem, char **NameArray, bddindex *IndexArray, bddnode *BddNode));
  extern    chain_list * convertbddindexabl __P((bddsystem *BddSystem, char **NameArray, bddindex *IndexArray, bddindex Index));

/*------------------------------------------------------\
|                                                       |
|                Garbage Collect Functions              |
|                                                       |
\------------------------------------------------------*/

  extern           void  garbagebddsystem __P((bddsystem *BddSystem));
  extern           void  sweepbddsystem __P((bddsystem *BddSystem));

/*------------------------------------------------------\
|                                                       |
|                   Reorder Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  reorderbddsystemsimple __P((bddsystem *BddSystem));
  extern           void  reorderbddsystemwindow __P((bddsystem *BddSystem));
  extern           void  reorderbddsystemtop __P((bddsystem *BddSystem));

  extern           void  reorderbddsystemdynamic __P((bddsystem *BddSystem, void (*ReorderFunc)(), long ReorderLow, long ReorderRatio));

/*------------------------------------------------------\
|                                                       |
|                   Explosion Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern  void explosionbddsystemdynamic __P((bddsystem *BddSystem, void (*ExplosionFunc)(), long ExplosionLimit));
  extern  void explosionbddsystemrestart __P((bddsystem *BddSystem));

/*------------------------------------------------------\
|                                                       |
|                    Test Functions                     |
|                                                       |
\------------------------------------------------------*/

  extern            int  testbddcircuit __P((bddcircuit *BddCircuit));

/*------------------------------------------------------\
|                                                       |
|                    Dump Functions                     |
|                                                       |
\------------------------------------------------------*/

  extern    bddcircuit * undumpbddcircuit __P((bddsystem *BddSystem, char *FileName));
  extern           void  dumpbddcircuit __P((bddcircuit *BddCircuit, char *FileName));

/*------------------------------------------------------\
|                                                       |
|                    Name Functions                     |
|                                                       |
\------------------------------------------------------*/

  extern          char * isbddstablename __P((char *StableName));
  extern          char * getbddstablename __P((char *Name));

/*------------------------------------------------------\
|                                                       |
|             Log Compatibility Functions               |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * getlogbddnodeone __P(());
  extern       bddnode * getlogbddnodezero __P(());
  extern       bddnode * getlogbddnodeterm __P((bddindex Index));
  extern       bddnode * getlogbddnodenot __P((bddnode *BddNode));
  extern       bddnode * applylogbddnode __P((long Oper, bddnode *BddNode1, bddnode *BddNode2));
  extern       ablexpr * convertlogbddnode __P((bddcircuit *BddCircuit, bddnode *BddNode));
  extern          short  addlogbddcircuitin __P((bddcircuit *BddCircuit, char *Name));

/*------------------------------------------------------\
|                                                       |
|                Bdd Transfert  Functions               |
|                                                       |
\------------------------------------------------------*/

  extern bddnode * transfertbdd __P(( bddsystem *SystemSrc, bddsystem *SystemDst, bddnode *Node ));


# endif 
