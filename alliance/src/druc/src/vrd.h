/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DRUCGRAL.H                       |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef DRUC_104
# define DRUC_104 

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

#define MACRO_PLUS_TAILLE (sizeof( DrucTypeStructPlusRect ))

#define MACRO_REGLE_NUM( Rectangle )                       \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->NOMBRE)
#define MACRO_LAYER_NUM( Rectangle )                       \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->NOMBRE)

#define MACRO_INSTANCE_NUM( Rectangle )                       \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->INSTANCE_NUM)

#define MACRO_NEXT_EQUI( Rectangle )                          \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->NEXT_EQUI)

#define MACRO_NEXT_DEFINE( Rectangle )                       \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->NEXT_DEFINE)

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct struct_plusrect
              {
                int                     NOMBRE;
                rdsins_list            *INSTANCE_NUM;
                rdsrec_list            *NEXT_EQUI;
                struct struct_plusrect *NEXT_DEFINE;
              } DrucTypeStructPlusRect;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern rdsfig_list *drucrdsfig();
  extern char        *drucgeterror();

# endif
