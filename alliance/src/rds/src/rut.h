/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rut209.h                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RUT_209_H
# define RUT_209_H

#include <stdint.h>

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
|                           Equi Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define RDS_EQUI_SIZE  ( sizeof( rdsrecequi ) )

/*------------------------------------------------------------\
|                                                             |
|                           Mask for Equi                     |
|                                                             |
\------------------------------------------------------------*/

# define RDS_EQUI_MASK       (long)0x00020000
# define RDS_END_EQUI_MASK   (long)0x00040000

/*------------------------------------------------------------\
|                                                             |
|                          Mask for Unify                     |
|                                                             |
\------------------------------------------------------------*/

# define RDS_DELETED_MASK    (long)0x00080000

/*------------------------------------------------------------\
|                                                             |
|                           Yes or No                         |
|                                                             |
\------------------------------------------------------------*/

# define RDS_YES     1
# define RDS_NO      0

/*------------------------------------------------------------\
|                                                             |
|                             Macros                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Grid Aligned                      |
|                                                             |
\------------------------------------------------------------*/

# define CheckPhysicalGridAligned(c) ((c) % RDS_PHYSICAL_GRID)

/*------------------------------------------------------------\
|                                                             |
|                       Macros for Equi                       |
|                                                             |
\------------------------------------------------------------*/

# define IsRdsEqui( Rectangle )                               \
                                                              \
   ( ( Rectangle )->FLAGS & RDS_EQUI_MASK                     )

# define SetRdsEqui( Rectangle )                              \
                                                              \
   ( ( Rectangle )->FLAGS |= RDS_EQUI_MASK                    )

# define ClearRdsEqui( Rectangle )                            \
                                                              \
   ( ( Rectangle )->FLAGS &= ~RDS_EQUI_MASK                   )

/*------------------------------------------------------------\
|                                                             |
|                    Macros for End Equi                      |
|                                                             |
\------------------------------------------------------------*/

# define IsRdsEndEqui( Rectangle )                            \
                                                              \
   ( ( Rectangle )->FLAGS & RDS_END_EQUI_MASK                 )

# define SetRdsEndEqui( Rectangle )                           \
                                                              \
   ( ( Rectangle )->FLAGS |= RDS_END_EQUI_MASK                )

# define ClearRdsEndEqui( Rectangle )                         \
                                                              \
   ( ( Rectangle )->FLAGS &= ~RDS_END_EQUI_MASK               )

/*------------------------------------------------------------\
|                                                             |
|                    Macros for Delete                        |
|                                                             |
\------------------------------------------------------------*/

# define IsRdsDeleted( Rectangle )                            \
                                                              \
   ( ( Rectangle )->FLAGS & RDS_DELETED_MASK                  )

# define SetRdsDeleted( Rectangle )                           \
                                                              \
   ( ( Rectangle )->FLAGS |= RDS_DELETED_MASK                 )

# define ClearRdsDeleted( Rectangle )                         \
                                                              \
   ( ( Rectangle )->FLAGS &= ~RDS_DELETED_MASK                )

/*------------------------------------------------------------\
|                                                             |
|                          Equi Link                          |
|                                                             |
\------------------------------------------------------------*/

# define RDS_EQUI( R )                                        \
                                                              \
  (((rdsrecequi *)((char *)(R) + sizeof(rdsrec_list)))->EQUI  )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Rds Rectangle Equi                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdsrecequi
  {
    rdsrec_list *EQUI;

  } rdsrecequi;


/*------------------------------------------------------------\
|                                                             |
|                       Coordinates for polygons              |
|                                                             |
\------------------------------------------------------------*/

  typedef struct coord_t
  {
    int32_t X;
    int32_t Y;

  } coord_t;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern char RDS_SYMMETRY_TABLE[ RDS_MAX_TRANSF ][ RDS_MAX_TRANSF ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Search Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern   rdsfig_list * searchrdsfig __P((char *Name));
  extern   rdsins_list * searchrdsins __P((rdsfig_list *Figure, char *Name));

/*------------------------------------------------------------\
|                                                             |
|                         Model Functions                     |
|                                                             |
\------------------------------------------------------------*/

  extern    ptype_list * getrdsmodellist __P((rdsfig_list *Figure));

/*------------------------------------------------------------\
|                                                             |
|                         Apply Functions                     |
|                                                             |
\------------------------------------------------------------*/

  extern           void  applyrdssym __P((long *X, long *Y, long *Dx, long *Dy, long Xins, long Yins, char Sym));
  extern           void  applyrdsmode __P((rdsfig_list *Figure, char Mode));

/*------------------------------------------------------------\
|                                                             |
|                        Flatten Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern           void  flattenrdsfig __P((rdsfig_list *Figure, char *InsName, char Concat));
  extern           void  rflattenrdsfig __P((rdsfig_list *Figure, char Concat, char Catal));

/*------------------------------------------------------------\
|                                                             |
|                          Equi Functions                     |
|                                                             |
\------------------------------------------------------------*/

  extern   rdsrec_list * equirdsfig __P((rdsfig_list *FigureRds, rdswindow *Window));

/*------------------------------------------------------------\
|                                                             |
|                          Unify Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern   rdsrec_list * unifyrdsfig __P((rdsfig_list *FigureRds, rdsrec_list *HeadEqui));

/*------------------------------------------------------------\
|                                                             |
|                          Polygon Functions                  |
|                                                             |
\------------------------------------------------------------*/

  extern           void  diag2rec __P((rdsfig_list *model, char layer, coord_t coord_tab[], unsigned coord_numb));
  extern           void  poly2diag __P((coord_t coord_tab[], unsigned coord_numb, double scale, coord_t **new_coord_tab, unsigned *new_coord_numb));
  extern            int  ispolyrec __P((coord_t coord_tab[], unsigned coord_numb));

# endif
