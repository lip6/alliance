/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  mbkrds.h                         |
|                                                             |
| Authors :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef LYNX_MBKRDS
# define LYNX_MBKRDS

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            General                          |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_FALSE   0
# define LYNX_TRUE    1

/*------------------------------------------------------------\
|                                                             |
|                          Lynx Mode                          |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_MODE (sizeof( lynxrec ))

/*------------------------------------------------------------\
|                                                             |
|                         Lynx Attributes                     |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_TREAT          (long)0x00020000
# define LYNX_FIRST_LINK     (long)0x00040000

/*------------------------------------------------------------\
|                                                             |
|                          Lynx Orient                        |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_NORTH          (int)0
# define LYNX_SOUTH          (int)1
# define LYNX_EAST           (int)2
# define LYNX_WEST           (int)3
# define LYNX_MAX_ORIENT     (int)4

# define LYNX_NORTH_MASK     (int)1
# define LYNX_SOUTH_MASK     (int)2
# define LYNX_WEST_MASK      (int)4
# define LYNX_EAST_MASK      (int)8

/*------------------------------------------------------------\
|                                                             |
|                       Lynx Cut & Deleted                    |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_CUT            (long)0x00200000
# define LYNX_DELETED        (long)0x00400000

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define IsLynxCut( Rectangle )            \
                                           \
   ( ( Rectangle )->FLAGS & LYNX_CUT       )

# define SetLynxCut( Rectangle )           \
                                           \
   ( ( Rectangle )->FLAGS |= LYNX_CUT      )

# define ClearLynxCut( Rectangle )         \
                                           \
   ( ( Rectangle )->FLAGS &= ~LYNX_CUT     )


# define IsLynxDeleted( Rectangle )        \
                                           \
   ( ( Rectangle )->FLAGS & LYNX_DELETED   )

# define SetLynxDeleted( Rectangle )       \
                                           \
   ( ( Rectangle )->FLAGS |= LYNX_DELETED  )

# define ClearLynxDeleted( Rectangle )     \
                                           \
   ( ( Rectangle )->FLAGS &= ~LYNX_DELETED )


/*------------------------------------------------------------\
|                                                             |
|                            Lynx Mode                        |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_LINK( R )                                      \
                                                             \
    (((lynxrec *)((char *)(R)+sizeof(rdsrec_list)))->LINK    )

# define LYNX_EQUI( R )                                      \
                                                             \
    (((lynxrec *)((char *)(R)+sizeof(rdsrec_list)))->EQUI    )

/*------------------------------------------------------------\
|                                                             |
|                          Mbk Attribute                      |
|                                                             |
\------------------------------------------------------------*/

# define SetLynxAttribute( Rectangle, Attribute )             \
                                                              \
   ( ( Rectangle )->FLAGS |= (((long)Attribute) << 24 )       )

# define GetLynxAttribute( Rectangle )                        \
                                                              \
   ( ( ( ( Rectangle )->FLAGS ) >> 24 ) & 0x1F                )

/*------------------------------------------------------------\
|                                                             |
|                          Mbk Attribute                      |
|                                                             |
\------------------------------------------------------------*/

# define SetLynxOrient( Rectangle, Orient )                   \
                                                              \
   ( ( Rectangle )->FLAGS |= (((long)Orient) << 29 )          )

# define GetLynxOrient( Rectangle )                           \
                                                              \
   ( ( ( ( Rectangle )->FLAGS ) >> 29 ) & 0x7                 )

/*------------------------------------------------------------\
|                                                             |
|                           Lynx Treat                        |
|                                                             |
\------------------------------------------------------------*/

# define IsLynxTreat( Rectangle )           \
                                            \
   ( ( Rectangle )->FLAGS & LYNX_TREAT      )

# define SetLynxTreat( Rectangle )          \
                                            \
   ( ( Rectangle )->FLAGS |= LYNX_TREAT     )

# define ClearLynxTreat( Rectangle )        \
                                            \
   ( ( Rectangle )->FLAGS &= ~LYNX_TREAT    )

/*------------------------------------------------------------\
|                                                             |
|                        Lynx First Link                      |
|                                                             |
\------------------------------------------------------------*/

# define IsLynxFirstLink( Rectangle )            \
                                                 \
   ( ( Rectangle )->FLAGS & LYNX_FIRST_LINK      )

# define SetLynxFirstLink( Rectangle )           \
                                                 \
   ( ( Rectangle )->FLAGS |= LYNX_FIRST_LINK     )

# define ClearLynxFirstLink( Rectangle )         \
                                                 \
   ( ( Rectangle )->FLAGS &= ~LYNX_FIRST_LINK    )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct lynxrec

  {
    rdsrec_list *EQUI;
    rdsrec_list *LINK;

  } lynxrec;

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

  extern rdsfig_list* Lynxfigmbkrds __P((phfig_list* ));
  extern void         Lynxfigrds __P((rdsfig_list*));
  extern void         Lynxfigrdsmbk __P((rdsfig_list*, void*));
  extern void         LynxSaveFigure __P((void*,char));
  extern void         LynxDelFigure __P((void*,char));
  extern void         Lynxrecrdsmbk __P((void*,rdsrec_list*,char));

# endif
