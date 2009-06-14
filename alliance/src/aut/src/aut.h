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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for AUT         |
|                                                       |
|  Date    :            03.12.96                        |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef AUT_103_H
# define AUT_103_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

# ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__) || defined(__cplusplus)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
# endif

/*------------------------------------------------------\
|                                                       |
|                       Hash Key                        |
|                                                       |
\------------------------------------------------------*/

# define AUT_HASH_KEY_EMPTY      (char *)0
# define AUT_HASH_KEY_DELETED    (char *)1

/*------------------------------------------------------\
|                                                       |
|                      Prime Number                     |
|                                                       |
\------------------------------------------------------*/

# define AUT_MAX_PRIME_NUMBER   58

/*------------------------------------------------------\
|                                                       |
|                        Alloc                          |
|                                                       |
\------------------------------------------------------*/

# define AUT_ALLOC_BLOCK  0
# define AUT_ALLOC_HEAP   1

/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                    Allocation Macros                  |
|                                                       |
\------------------------------------------------------*/

# define autallocblock( S ) ( autalloc( (unsigned int)( S ), AUT_ALLOC_BLOCK ) )
# define autallocheap( S )   ( autalloc( (unsigned int)( S ), AUT_ALLOC_HEAP ) )
# define autfreeblock( P )   ( autfree( (char *)( P ), 0 ) )
# define autfreeheap( P, S ) ( autfree( (char *)( P ), (unsigned int)( S ) ) )

/*------------------------------------------------------\
|                                                       |
|                      Resize Macros                    |
|                                                       |
\------------------------------------------------------*/

# define autresizeblock( P, O, N ) ( autresize( ( P ), ( O ), ( N ) ) )

/*------------------------------------------------------\
|                                                       |
|                    Debug Macros                       |
|                                                       |
\------------------------------------------------------*/

# define autbegin() do {                               \
                                                       \
  if ( AUT_DEBUG_ON ) addautdebug(__LINE__,autbasename(__FILE__,NULL)); } while (0)

# define autend()   do {                               \
                                                       \
  if ( AUT_DEBUG_ON ) delautdebug(); } while (0)

/*------------------------------------------------------\
|                                                       |
|                         Type                          |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                   Aut Debug List                      |
|                                                       |
\------------------------------------------------------*/

  typedef struct autdebug_list
  {
    struct autdebug_list *NEXT;
    int                   LINE;
    char                 *NAME;

  } autdebug_list;

/*------------------------------------------------------\
|                                                       |
|                   Aut Hash Table                      |
|                                                       |
\------------------------------------------------------*/

  typedef struct authelem
  {
    char *KEY;
    long  VALUE;

  } authelem;

  typedef struct authtable
  {
    authelem *TABLE;
    long      TABLE_SIZE;
    long      NUMBER_ELEM;
    long      NUMBER_ADD;
    long      NUMBER_SCAN;
    long      NUMBER_DEL;
    long      NUMBER_STRETCH;
    long    (*FUNC_SIZE)();
    long    (*FUNC_INDEX)();
    long    (*FUNC_KEY)();

  } authtable;

/*------------------------------------------------------\
|                                                       |
|                   Aut Hash Table 2                    |
|                                                       |
\------------------------------------------------------*/

  typedef struct auth2elem
  {
    char *KEY1;
    char *KEY2;
    long  VALUE;

  } auth2elem;

  typedef struct auth2table
  {
    auth2elem *TABLE;
    long       TABLE_SIZE;
    long       NUMBER_ELEM;
    long       NUMBER_ADD;
    long       NUMBER_SCAN;
    long       NUMBER_DEL;
    long       NUMBER_STRETCH;
    long     (*FUNC_SIZE)();
    long     (*FUNC_INDEX)();
    long     (*FUNC_KEY)();

  } auth2table;

/*------------------------------------------------------\
|                                                       |
|                    Aut Graph                          |
|                                                       |
\------------------------------------------------------*/

  typedef struct autnode_list
  {
    struct autnode_list  *NEXT;
    struct autnode_list **PREV;
    char                 *NAME;
    chain_list           *ARC_FROM;
    chain_list           *ARC_TO;
    long                  FLAGS;
    void                 *USER;

  } autnode_list;

  typedef struct autarc_list
  {
    struct autarc_list  *NEXT;
    struct autarc_list **PREV;
    autnode_list        *NODE_FROM;
    autnode_list        *NODE_TO;
    long                 FLAGS;
    void                *USER;

  } autarc_list;

  typedef struct autgraph
  {
    autnode_list  *NODE;
    autarc_list   *ARC;
    authtable     *HASH_NODE;
    auth2table    *HASH_ARC;
    long           NUMBER_NODE;
    long           NUMBER_ARC;
    long           FLAGS;
    void          *USER;

  } autgraph;

/*------------------------------------------------------\
|                                                       |
|                     Global Variables                  |
|                                                       |
\------------------------------------------------------*/

 extern autdebug_list *HEAD_AUTDEBUG;
 extern char           AUT_DEBUG_ON;
 extern long           AUT_HASH_PRIME_NUMBER[ AUT_MAX_PRIME_NUMBER ];
 extern long           AUT_HASH_MAX_SCAN;
 extern long           AUT_HASH_STRETCH_FACTOR;

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

  extern           void  autenv __P(());

/*------------------------------------------------------\
|                                                       |
|                      Exit Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  autexit __P((long Value));

/*------------------------------------------------------\
|                                                       |
|                    Alloc Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern          char * autalloc __P((unsigned int Size, char Heap));
  extern      authelem * allocauthelem __P((unsigned int Number));
  extern     authtable * allocauthtable __P(());
  extern     auth2elem * allocauth2elem __P((unsigned int Number));
  extern    auth2table * allocauth2table __P(());
  extern autdebug_list * allocautdebug __P(());

/*------------------------------------------------------\
|                                                       |
|                    Resize Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern  char * autresize __P((char *Source, unsigned int OldSize, unsigned int NewSize));

/*------------------------------------------------------\
|                                                       |
|                      Free Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern  void  autfree __P((char *Pointer, unsigned int Size));
  extern  void  freeauthelem __P((authelem *Element));
  extern  void  freeauthtable __P((authtable *HashTable));
  extern  void  freeauth2elem __P((auth2elem *Element));
  extern  void  freeauth2table __P((auth2table *HashTable));
  extern  void  freeautdebug __P((autdebug_list *Debug));

/*------------------------------------------------------\
|                                                       |
|                     Debug Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern  void  autdebug __P(());
  extern  void  addautdebug __P((int Line, char *File));
  extern  void  delautdebug __P(());

/*------------------------------------------------------\
|                                                       |
|                   Get Hash Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern  long  getauthsize __P((long Size));
  extern  long  getauthkey __P((authtable *Table, char *Key));
  extern  long  getauthindex __P((authtable *Table, char *Key, long Index));

  extern  long  getauth2size __P((long Size));
  extern  long  getauth2key __P((auth2table *Table, char *Key1, char *Key2));
  extern  long  getauth2index __P((auth2table *Table, char *Key1, char *Key2, long Index));

/*------------------------------------------------------\
|                                                       |
|                   Check Hash Functions                |
|                                                       |
\------------------------------------------------------*/

  extern int  checkauthkey __P((char *Key, int Severity));
  extern int  checkauth2key __P((char *Key1, char *Key2, int Severity));

/*------------------------------------------------------\
|                                                       |
|                  Set Hash Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern void  setauthfunc __P((authtable *HashTable, long (*FuncSize)(), \
                                long (*FuncKey)()   , long (*FuncIndex)()));

  extern void  setauth2func __P((auth2table *HashTable, long (*FuncSize)(), \
                                 long (*FuncKey)(), long (*FuncIndex)()));

/*------------------------------------------------------\
|                                                       |
|                  Hash Table Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern  authtable *createauthtable __P((long Length));
  extern  void       stretchauthtable __P((authtable *HashTable));
  extern  void       destroyauthtable __P((authtable *HashTable));
  extern  void       resetauthtable __P((authtable *HashTable));
  extern  void       viewauthtable __P((authtable *HashTable, void (*FuncView)()));
  extern  void       viewauthelem __P((authelem *Element));

  extern  auth2table *createauth2table __P((long Length));
  extern  void        stretchauth2table __P((auth2table *HashTable));
  extern  void        destroyauth2table __P((auth2table *HashTable));
  extern  void        resetauth2table __P((auth2table *HashTable));
  extern  void        viewauth2table __P((auth2table *HashTable, void (*FuncView)()));
  extern  void        viewauth2elem __P((auth2elem *Element));

/*------------------------------------------------------\
|                                                       |
|                  Hash Element Functions               |
|                                                       |
\------------------------------------------------------*/

  extern authelem *addauthelem __P((authtable *HashTable, char *Key, long Value));
  extern authelem *searchauthelem __P((authtable *HashTable, char *Key));
  extern      int  delauthelem __P((authtable *HashTable, char *Key));

  extern auth2elem *addauth2elem __P((auth2table *HashTable, char *Key1, \
                                       char *Key2, long Value));
  extern auth2elem *searchauth2elem __P((auth2table *HashTable, char *Key1, char *Key2));
  extern       int  delauth2elem __P((auth2table *HashTable, char *Key1, char *Key2));

/*------------------------------------------------------\
|                                                       |
|                  Hash Test Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern  int  testauthtable __P((authtable *HashTable));
  extern  int  testauth2table __P((auth2table *HashTable));

/*------------------------------------------------------\
|                                                       |
|                  Sort Compare Function                |
|                                                       |
\------------------------------------------------------*/

  extern  long  sortautcompare __P((long *ValueArray, long Index1, long Index2));

/*------------------------------------------------------\
|                                                       |
|                    Sort Function                      |
|                                                       |
\------------------------------------------------------*/

  extern  void  sortautarray __P((void *ValueArray, long *IndexArray, \
                                  long ArraySize, long (*FuncCompare)()));

/*------------------------------------------------------\
|                                                       |
|                    File Function                      |
|                                                       |
\------------------------------------------------------*/

  extern char *autbasename __P((char *Name, char *Extension));

/*------------------------------------------------------\
|                                                       |
|                  Name Functions                       |
|                                                       |
\------------------------------------------------------*/

  extern char *autnamealloc __P((char *Name));

/*------------------------------------------------------\
|                                                       |
|                 Graph Functions                       |
|                                                       |
\------------------------------------------------------*/

  extern  autnode_list * searchautgraphnode __P((autgraph *Graph, char *Name));
  extern   autarc_list * searchautgrapharc __P((autgraph *Graph, autnode_list *NodeFrom, \
                                                autnode_list *NodeTo));

  extern  autnode_list * addautgraphnode __P((autgraph *Graph, char *Name));
  extern   autarc_list * addautgrapharc __P((autgraph *Graph, autnode_list *NodeFrom, \
                                             autnode_list *NodeTo));

  extern           void  delautgraphnode __P((autgraph *Graph, autnode_list *Node));
  extern           void  delautgrapharc __P((autgraph *Graph, autarc_list *Arc));

  extern      autgraph * createautgraph __P(());
  extern           void  destroyautgraph __P((autgraph *Graph));

  extern           void  viewautgraph __P((autgraph *Graph, void (*FuncViewNode)(), \
                                           void (*FuncViewArc)()));
  extern      autgraph * loadautgraph __P((char *FileName));
  extern           void  driveautgraph __P((autgraph *Graph, char *FileName));

#ifdef __cplusplus
}
#endif /* __cplusplus */

# endif
