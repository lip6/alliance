/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rtl111.h                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RTL_111_H
# define RTL_111_H

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *RDS_IN;
  extern char *RDS_OUT;
  extern char *RDS_TECHNO_NAME;
  extern long  RDS_WINDOW_AREA;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern           void  rdsenv __P(());

  extern   rdsfig_list * getrdsfig __P((char *Name, char Mode, unsigned int Size));
  extern           void  loadrdsfig __P((rdsfig_list *Figure, char *Name, char Mode));
  extern           void  saverdsfig __P((rdsfig_list *Figure));

# endif
