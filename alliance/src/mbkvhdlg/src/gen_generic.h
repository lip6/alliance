#define LOGEN 21

#define GENTYPE_EMPTY 0
#define GENTYPE_BIT   1
#define GENTYPE_VAL   2
#define GENTYPE_ARG   3
#define GENTYPE_TEXT  4
#define GENTYPE_LIST  5
#define GENTYPE_HEX   6
#define GENTYPE_MAX   6

typedef struct logen
{
   struct logen *NEXT;
   char         *NAME;
   char          TYPE;
   union {
      char          CHAR;
      long           VAL;
      char         *TEXT;
      struct logen *LIST;
   }            VALUE;
   char           TAG;
} logen_list;

#ifdef SunOS
#define strtoul (unsigned long)strtol
#endif

extern logen_list *addlogen(logen_list *,char *);
extern logen_list *getlogen(logen_list *,char *);
extern logen_list *duplogen(logen_list *,logen_list *, char *);
extern logen_list *chklogen(logen_list *,logen_list *, char *, char *);
extern logen_list *dellogen(logen_list *, char *name);
extern void        debuglogen(logen_list *g, int level);
