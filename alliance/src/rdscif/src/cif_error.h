#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSCIF                         |
|                                                             |
| File    :                 cif_error.h                       |
|                                                             |
| Authors :       Petrot Frederic & Jacomme Ludovic           |
|                   ---------                                 |
| Date    :    The king ^     04/07/93                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RDSCIF_ERROR
# define RDSCIF_ERROR

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define CIF_ERROR_BAD_TRANS            0
# define CIF_ERROR_CONNECTOR_NAME       1
# define CIF_ERROR_FILE_OPEN            2
# define CIF_ERROR_FILE_CLOSE           3
# define CIF_ERROR_MODEL                4

# define CIF_ERROR_EXPECTED_SEMI        5
# define CIF_ERROR_EXPECTED_SPACE       6
# define CIF_ERROR_EXPECTED_NAME        7
# define CIF_ERROR_EXPECTED_SIGNED      8
# define CIF_ERROR_EXPECTED_UNSIGNED    9
# define CIF_ERROR_EXPECTED_LAYER      10
# define CIF_ERROR_EXPECTED_MANHATTAN  11
# define CIF_ERROR_EXPECTED_EOF        12
# define CIF_ERROR_UNEXPECTED_EOF      13
# define CIF_ERROR_SIGNED_OVERFLOW     14
# define CIF_ERROR_UNSIGNED_OVERFLOW   15
# define CIF_ERROR_LAYER_UNKNOWN       16
# define CIF_ERROR_POLYGON_EMPTY       17
# define CIF_ERROR_POLYGON_OVERFLOW    18
# define CIF_ERROR_POLYGON_NOT_REC     19
# define CIF_ERROR_SYMBOL_STARTED      20
# define CIF_ERROR_SYMBOL_NOT_STARTED  21
# define CIF_ERROR_BAD_DEFINE          22
# define CIF_ERROR_BAD_COMMAND         23
# define CIF_ERROR_BAD_COMMENT         24
# define CIF_ERROR_BAD_CONNECTOR       25
# define CIF_ERROR_BAD_SYMBOL          26
# define CIF_ERROR_SYMBOL_NOT_DEFINE   27
# define CIF_ERROR_BAD_MIRROR          28
# define CIF_ERROR_NO_TOP_MODEL        29

# define CIF_WARNING_PHYSICAL_GRID     0
# define CIF_WARNING_UNCONNECTED_NODE  1
# define CIF_WARNING_ROUND_FLASH       2
# define CIF_WARNING_USER_COMMAND      3

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/
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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern           void  ciferror __P((char Error, char *Text, long Value));
  extern           void  cifwarning __P((char Warning, char *Text, long Value));

# endif
