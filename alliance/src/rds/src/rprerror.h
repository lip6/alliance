#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rprerror.h                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RPR_ERROR_H
# define RPR_ERROR_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Rpr Error Constants                   |
|                                                             |
\------------------------------------------------------------*/

# define RPR_DEFINE_MISSING  0 
# define RPR_TABLE_MISSING   1 
# define RPR_LINE_EXPECTED   2
# define RPR_UNKNOWN_DEFINE  3 
# define RPR_NOT_DEFINED     4 
# define RPR_MULTIPLE_GRID   5 
# define RPR_TOO_SMAL        6
# define RPR_MISSING_VALUE   7
# define RPR_MISSING_NAME    8
# define RPR_ILLEGAL_FLOAT   9
# define RPR_UNEXPECTED_LINE 10
# define RPR_UNEXPECTED_EOF  11
# define RPR_TOO_MANY_WORDS  12
# define RPR_MISSING_TABLE   13
# define RPR_OPEN_FILE       14
# define RPR_MISSING_POST    15
# define RPR_NEGATIVE_VALUE  16
# define RPR_SYNTAX_ERROR    17

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define rprerror( E, V, D ) (rpr_error( (E), (V), (D), basename(__FILE__), __LINE__ ))

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern           void  rpr_error __P((char Error, char *Text, long Data, char *File, long Line));

# endif
