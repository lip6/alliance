#ifndef _RDS_H_
#define _RDS_H_

#include "ht.h"

extern int yydebug;
extern int yyparse(void);
extern FILE *yyin;
extern FILE *yyout;
extern ht_t * dico;
extern int verbose;
extern char * expfname ;

#endif
