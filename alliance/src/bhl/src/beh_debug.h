/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: beh_debug.h						*/
/* date		: Mar  8 2000						*/
/* version	: v111							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: difines and structures for the debug function		*/
/* ###--------------------------------------------------------------### */

struct stack				/* debugger's stack's structure	*/
  {
  short  type;				/* type of the current ponter	*/
  short  mark;				/* stop mark			*/
  void  *data;				/* current pointer		*/
  };

union value
  {
  void *dat;
  long  imd;
  };

#define STKSIZ_DFN    200		/* debugger's stack size	*/

#define KIND_DFN      0xff000000
#define COMMAND_DFN   0x01000000
#define IMMEDIATE_DFN 0x02000000
#define POINTER_DFN   0x03000000
#define ARRAY_DFN     0x04000000
#define ARYOFPNT_DFN  0x05000000

#define FORMAT_DFN    0x00ff0000
#define s_DFN         0x00010000
#define u_DFN         0x00020000
#define d_DFN         0x00030000
#define l_DFN         0x00040000
#define x_DFN         0x00050000
#define c_DFN         0x00060000
#define ABL_DFN       0x00070000
#define BDD_DFN       0x00080000
#define GEX_DFN       0x00090000

#define _error_DFN    0x0000ffff

#define TYPE_DFN      0x0000ffff
#define _back_DFN     0
#define _exit_DFN     _back_DFN     + 1
#define _jump_DFN     _exit_DFN     + 1
#define _save_DFN     _jump_DFN     + 1
#define _stop_DFN     _save_DFN     + 1
#define _top_DFN      _stop_DFN     + 1
#define _up_DFN       _top_DFN      + 1
#define _display_DFN  _up_DFN       + 1

#define character_DFN _display_DFN  + 1
#define short_DFN     character_DFN + 1
#define integer_DFN   short_DFN     + 1
#define long_DFN      integer_DFN   + 1
#define void_DFN      long_DFN      + 1
#define string_DFN    void_DFN      + 1

#define chain_DFN     string_DFN    + 1
#define ptype_DFN     chain_DFN     + 1

#define befig_DFN     ptype_DFN     + 1
#define bereg_DFN     befig_DFN     + 1
#define bemsg_DFN     bereg_DFN     + 1
#define berin_DFN     bemsg_DFN     + 1
#define beout_DFN     berin_DFN     + 1
#define bebus_DFN     beout_DFN     + 1
#define beaux_DFN     bebus_DFN     + 1
#define bedly_DFN     beaux_DFN     + 1
#define bebux_DFN     bedly_DFN     + 1
#define bepor_DFN     bebux_DFN     + 1
#define begen_DFN     bepor_DFN     + 1
#define biabl_DFN     begen_DFN     + 1
#define binode_DFN    biabl_DFN     + 1
#define bequad_DFN    binode_DFN    + 1
#define beder_DFN     bequad_DFN    + 1

#define next_DFN      beder_DFN     + 1
#define name_DFN      next_DFN      + 1
#define user_DFN      name_DFN      + 1
#define errflg_DFN    user_DFN      + 1
#define type_DFN      errflg_DFN    + 1
#define level_DFN     type_DFN      + 1
#define label_DFN     level_DFN     + 1
#define message_DFN   label_DFN     + 1
#define abl_DFN       message_DFN   + 1
#define node_DFN      abl_DFN       + 1
#define direction_DFN node_DFN      + 1
#define cndabl_DFN    direction_DFN + 1
#define valabl_DFN    cndabl_DFN    + 1
#define cndnode_DFN   valabl_DFN    + 1
#define valnode_DFN   cndnode_DFN   + 1
#define out_ref_DFN   valnode_DFN   + 1
#define bus_ref_DFN   out_ref_DFN   + 1
#define aux_ref_DFN   bus_ref_DFN   + 1
#define reg_ref_DFN   aux_ref_DFN   + 1
#define bux_ref_DFN   reg_ref_DFN   + 1
#define msg_ref_DFN   bux_ref_DFN   + 1
#define dly_ref_DFN   msg_ref_DFN   + 1
#define value_DFN     dly_ref_DFN   + 1
#define data_DFN      value_DFN     + 1
#define time_DFN      data_DFN      + 1
#define bdd_DFN       time_DFN      + 1
#define gex_DFN       bdd_DFN       + 1
#define varlist_DFN   gex_DFN       + 1
#define derivate_DFN  varlist_DFN   + 1
#define index_DFN     derivate_DFN  + 1
#define flag_DFN      index_DFN     + 1
#define time_unit_DFN flag_DFN      + 1
#define time_step_DFN time_unit_DFN + 1

#define MAXCMD_DFN    time_step_DFN + 1
