/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for SCP         |
|                                                       |
|  File    :              scp.h                         |
|                                                       |
|  Date    :            28.04.98                        |
|                                                       |
\------------------------------------------------------*/

# ifndef SCP_H
# define SCP_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/

# define SCP_CEL_BOX        0x0001
# define SCP_CEL_CON        0x0002

/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/

/*------------------------------------------------------\
|                                                       |
|                      Structures                       |
|                                                       |
\------------------------------------------------------*/

/*------------------------------------------------------\
|                                                       |
|                        Colums                         |
|                                                       |
\------------------------------------------------------*/

  typedef struct scpcol_list
  {
    struct scpcol_list  *NEXT;
    struct scpcel_list  *CELL;
    struct scpcel_list  *LAST_CELL;
    long                 NUMBER_CELL;
    long                 MAX_DX_CELL;
  } scpcol_list;

/*------------------------------------------------------\
|                                                       |
|                        Cells                          |
|                                                       |
\------------------------------------------------------*/

  typedef struct scpcel_list
  {
    struct scpcel_list  *NEXT;
    struct scpcel_list  *PREV;
    struct scpcol_list  *COL;
    void                *ROOT;
    long                 TYPE;
  } scpcel_list;

/*------------------------------------------------------\
|                                                       |
|                     Global Variables                  |
|                                                       |
\------------------------------------------------------*/

extern scpcol_list   *HEAD_SCPCOL;

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                    Alloc Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern scpcol_list    *allocscpcol();
  extern scpcel_list    *allocscpcel();

/*------------------------------------------------------\
|                                                       |
|                      Free Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern void  freescpcol();
  extern void  freescpcel();

/*------------------------------------------------------\
|                                                       |
|                      Add Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern scpcel_list    *addscpcel();
  extern scpcol_list    *addscpcol();

  extern scpcel_list    *addschboxtoscpcel();
  extern scpcel_list    *addschcontoscpcel();


/*------------------------------------------------------\
|                                                       |
|                      Del Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern void   delscpcol();

/*------------------------------------------------------\
|                                                       |
|                      Search Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern void   swapscpcel();
  
/*------------------------------------------------------\
|                                                       |
|                     View Functions                    |
|                                                       |
\------------------------------------------------------*/


# endif 
