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
| Tool    :                     SCAN                          |
|                                                             |
| File    :                  scan_param.h                     |
|                                                             |
| Author  :                                                   |
|                                                             |
| Date    :                                                   |
|                                                             |
\------------------------------------------------------------*/

# ifndef SCAN_PARAM_H
# define SCAN_PARAM_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define SCAN_PARAM_NONE         0
# define SCAN_PARAM_MUX          1
# define SCAN_PARAM_REG          2  
# define SCAN_PARAM_REG_MUX      3
# define SCAN_PARAM_BUF          4

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

  typedef struct scanmux_list
  {
    struct scanmux_list *NEXT;
    char                *MODEL;
    char                *SEL;
    char                *INPUT_SEL;
    char                *INPUT_NSEL;
    char                *OUTPUT;
    char                *VDD;
    char                *VSS;

  } scanmux_list;

  typedef struct scanreg_list
  {
    struct scanreg_list *NEXT;
    char                *MODEL;
    char                *CLK;
    char                *INPUT;
    char                *OUTPUT;    
    char                *VDD;
    char                *VSS;
    char                *MUX;
    char                *REG_MUX;
    
  } scanreg_list;

   typedef struct scanregmux_list
   {
   struct scanregmux_list *NEXT;
   char                   *MODEL;
   char                   *CLK;
   char                   *SEL;
   char                   *INPUT_SEL;
   char                   *INPUT_NSEL;
   char                   *OUTPUT;
   char                   *VDD;
   char                   *VSS;
   char                   *MUX;
   char                   *REG;
                        
   } scanregmux_list;                

 typedef struct scanbuf_list
   {
   struct scanbuf_list *NEXT;
   char                *MODEL;
   char                *INPUT;
   char                *OUTPUT;
   char                *VDD;
   char                *VSS;
                        
   } scanbuf_list ;          

  typedef struct scanparam
  {
    scanmux_list      *MUX_LIST;
    scanreg_list      *REG_LIST;
    scanregmux_list   *REG_MUX_LIST;
    scanbuf_list      *BUF_LIST;          
    
  } scanparam;

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
  
  extern scanparam *ScanParseParamFile();

# endif
