/* ###--------------------------------------------------------------------### */
/* #                                                                        # */
/* # file : velo.h                                                          # */
/* | date : Jun 27 1997                                                     | */
/* | version : 0.00                                                         | */
/* | author : Denis Hommais                                                 | */
/* |                                                                        | */
/* # contents : structure definitions used in VEL library                   # */
/* #                                                                        # */
/* ###--------------------------------------------------------------------### */

#ifndef _VEL_H_
#define _VEL_H_

#define VEL_SIG   296651                      /* ptype identifier for velosig */
#define VEL_CON   296217                      /* ptype identifier for velocon */
#define VEL_CHAIN 296121                      /* ptype identifier for chain   */

typedef struct Velosig {
   struct Velosig *      NEXT;
   char           *      NAME;
   long                  LEFT;
   long                 RIGHT;
   char                  TYPE;
   struct chain   *      VSIG;
   struct Velosig *     SUPER;
   struct chain   * NAMECHAIN;
   struct ptype   *      USER;
} velosig;

typedef struct Velocon {
   struct Velocon *      NEXT;
   char           *      NAME;
   long                  LEFT;
   long                 RIGHT;
   struct chain   *      VSIG;
   struct chain   *      ROOT;
   struct ptype   *      USER;
} velocon;

/* prototype of functions that come from velo.c */
extern velosig *addvelosig (ptype_list *, char *, long, long, char);
extern velocon *addvelocon (ptype_list *, char *, long, long);
extern velosig *dupvelosig (ptype_list *, velosig *);
extern velosig *getvelosig (ptype_list *, char *, long, long);
extern int      delvelocon (ptype_list *, velocon *);

/* prototype of functions that come from vel_util.c */
extern void buildvelo        (lofig_list *);
extern void vectorize_velo   (lofig_list *);
extern char *mkvhdlname      (char *);
extern void printvelosiglist (velosig *);
extern void printveloconlist (velocon *);
extern void makevelofig(lofig_list *);
extern velosig *getvelosigbyname(ptype_list *, char *);
#endif
