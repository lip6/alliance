/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                 Modify.h                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_MODIFY
# define GRAAL_MODIFY

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
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

  extern char GraalModifyAbort;
  extern char GraalModifyLock;
  extern char GraalModifyApply;

/*------------------------------------------------------------\
|                                                             |
|                             Lock                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Segment                           |
|                                                             |
\------------------------------------------------------------*/

  extern char  GraalSegmentMLayer;
  extern long  GraalSegmentMWidth;
  extern char *GraalSegmentMName;

/*------------------------------------------------------------\
|                                                             |
|                              Via                            |
|                                                             |
\------------------------------------------------------------*/

  extern char  GraalViaMType;
  extern char *GraalViaMName;

/*------------------------------------------------------------\
|                                                             |
|                           Transistor                        |
|                                                             |
\------------------------------------------------------------*/

  extern char  GraalTransistorMType;
  extern char *GraalTransistorMName;
  extern long  GraalTransistorMWidth;

/*------------------------------------------------------------\
|                                                             |
|                           Connector                         |
|                                                             |
\------------------------------------------------------------*/

  extern char  GraalConnectorMLayer; 
  extern long  GraalConnectorMWidth; 
  extern char *GraalConnectorMName;  
  extern char  GraalConnectorMOrient;

/*------------------------------------------------------------\
|                                                             |
|                           Reference                         |
|                                                             |
\------------------------------------------------------------*/

  extern char  GraalReferenceMType;  
  extern char *GraalReferenceMName;  

/*------------------------------------------------------------\
|                                                             |
|                           Instance                          |
|                                                             |
\------------------------------------------------------------*/

 extern char  GraalInstanceMSym;
 extern char *GraalInstanceMName;
 extern char *GraalInstanceMModel;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/


# endif
