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
| File    :                 cif_parse.h                       |
|                                                             |
| Authors :               Jacomme Ludovic                     |
|                                                             |
| Date    :                   23/08/93                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RDSCIF_PARSE
# define RDSCIF_PARSE

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define CIF_MAX_BUFFER         512
# define CIF_MAX_SYMBOL_HEAD    16
# define CIF_MASK_SYMBOL        (long)(0x0000F)

/*------------------------------------------------------------\
|                                                             |
|                           Commands                          |
|                                                             |
\------------------------------------------------------------*/

# define CIF_COMMAND_NULL         0
# define CIF_COMMAND_POLYGON      1
# define CIF_COMMAND_BOX          2
# define CIF_COMMAND_ROUNDFLASH   3
# define CIF_COMMAND_WIRE         4
# define CIF_COMMAND_LAYER        5
# define CIF_COMMAND_DEFSTART     6
# define CIF_COMMAND_DEFFINISH    7
# define CIF_COMMAND_DEFDELETE    8
# define CIF_COMMAND_CALL         9
# define CIF_COMMAND_COMMENT     10
# define CIF_COMMAND_END         11
# define CIF_COMMAND_EOF         12

/*------------------------------------------------------------\
|                                                             |
|                           Overflow                          |
|                                                             |
\------------------------------------------------------------*/

# define CIF_MAX_UNSIGNED ((unsigned int)((0XFFFFFFFF-9)/10))
# define CIF_MAX_SIGNED   ((int         )((0X7FFFFFFF-9)/10))
# define CIF_MAX_POINT    512

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

#define CIF_TO_RDS_UNIT( X )                                  \
                                                              \
  ( ( ( X )*CifScaleA*RDS_UNIT ) / ( CifScaleB*CIF_UNIT )     )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct symbol_list 
  {
    struct symbol_list *NEXT;
    long                NUMBER;
    rdsfig_list        *MODEL;

  } symbol_list;

  typedef struct node_list 
  {
    struct node_list *NEXT;
    char             *NAME;
    char              LAYER;
    long              X;
    long              Y;

  } node_list;

  typedef struct connector_list
  {
    struct connector_list *NEXT;
    char                  *NAME;
    char                   LAYER;
    long                   X;
    long                   Y;
    long                   WIDTH;

  } connector_list;



# endif
