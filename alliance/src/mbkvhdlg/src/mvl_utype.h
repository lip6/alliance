
/* ###---------------------------------------------------------------### */
/*                                                                       */
/* file         : mvl_utype.h                                             */
/* date         : Aug  8 91                                              */
/* author       : P.BAZARGAN L.A.TABUSSE VUONG H.N.                      */
/*                                                                       */
/* content      : declaration of external functions and global variables */
/*                used by yacc                                           */
/*                                                                       */
/* ###---------------------------------------------------------------### */

struct dct_entry
  {
  struct dct_entry *next;
  struct dct_recrd *data;
  char             *key;
  };

struct dct_recrd
  {
  struct dct_recrd *next;
  char             *key;
  short             fd0_val;
  short             fd1_val;
  short             fd2_val;
  short             fd3_val;
  short             fd4_val;
  short             fd5_val;
  short             fd6_val;
  int               pnt_val;
  };
