#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSGDS                         |
|                                                             |
| File    :                  gds_parse.h                      |
|                                                             |
| Authors :                 Pierre Vittet                     |
|                    Modified by Jacomme Ludovic              |
| Date    :                   04/07/93                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RDSGDS_PARSE
# define RDSGDS_PARSE

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

/* conversion to meters :
   GDS internally defines a unit, let says GDS_UNIT, that is so that
   GDS_UNIT is defined in meters.
   RDS defines its unit so that RDS_UNIT_PARAM is defined in microns.
   So RDS_UNIT_PARAM * MICRONS_IN_METER is given in meters, and :
   Xgds * GDS_UNIT = Xrds / (RDS_UNIT * MICRONS_IN_METER). */

#define MICRONS_IN_METER 1000000

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
  typedef struct node_list
  {
    struct node_list *NEXT;
    char             *NAME;
    char              LAYER;
    long              X;
    long              Y;

  } node_list;

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

# endif
