/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
/* file		: pat_debug.h						*/
/* date		: Mar  9 2000						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: declaration of diefines for pat_debug			*/
/* ###--------------------------------------------------------------### */

struct stack				/* debugger's stack's structure	*/
  {
  short  type;				/* type of the current pointer	*/
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

#define _error_DFN    0x0000ffff

#define TYPE_DFN      0x0000ffff
#define _back_DFN     0
#define _exit_DFN     (_back_DFN     + 1)
#define _jump_DFN     (_exit_DFN     + 1)
#define _save_DFN     (_jump_DFN     + 1)
#define _stop_DFN     (_save_DFN     + 1)
#define _top_DFN      (_stop_DFN     + 1)
#define _up_DFN       (_top_DFN      + 1)
#define _display_DFN  (_up_DFN       + 1)

#define character_DFN (_display_DFN  + 1)
#define short_DFN     (character_DFN + 1)
#define integer_DFN   (short_DFN     + 1)
#define long_DFN      (integer_DFN   + 1)
#define void_DFN      (long_DFN      + 1)
#define string_DFN    (void_DFN      + 1)

#define chain_DFN     (string_DFN    + 1)
#define ptype_DFN     (chain_DFN     + 1)
#define paseq_DFN     (ptype_DFN     + 1)
#define pagrp_DFN     (paseq_DFN     + 1)
#define paiol_DFN     (pagrp_DFN     + 1)
#define papat_DFN     (paiol_DFN     + 1)
#define pacom_DFN     (papat_DFN     + 1)
#define paini_DFN     (pacom_DFN     + 1)
#define paevt_DFN     (paini_DFN     + 1)
#define pains_DFN     (paevt_DFN     + 1)
#define actflag_DFN   (pains_DFN     + 1)
#define blank_DFN     (actflag_DFN   + 1)
#define buffer_DFN    (blank_DFN     + 1)
#define curcom_DFN    (buffer_DFN    + 1)
#define curpat_DFN    (curcom_DFN    + 1)
#define deccom_DFN    (curpat_DFN    + 1)
#define drvseq_DFN    (deccom_DFN    + 1)
#define endflg_DFN    (drvseq_DFN    + 1)
#define errflg_DFN    (endflg_DFN    + 1)
#define filname_DFN   (errflg_DFN    + 1)
#define filpnt_DFN    (filname_DFN   + 1)
#define findex_DFN    (filpnt_DFN    + 1)
#define flag_DFN      (findex_DFN    + 1)
#define format_DFN    (flag_DFN      + 1)
#define index_DFN     (format_DFN    + 1)
#define insname_DFN   (index_DFN     + 1)
#define instance_DFN  (insname_DFN   + 1)
#define iolnbr_DFN    (instance_DFN  + 1)
#define label_DFN     (iolnbr_DFN    + 1)
#define length_DFN    (label_DFN     + 1)
#define line_DFN      (length_DFN    + 1)
#define lineno_DFN    (line_DFN      + 1)
#define mode_DFN      (lineno_DFN    + 1)
#define model_DFN     (mode_DFN      + 1)
#define name_DFN      (model_DFN     + 1)
#define next_DFN      (name_DFN      + 1)
#define nxtpat_DFN    (next_DFN      + 1)
#define oldcom_DFN    (nxtpat_DFN    + 1)
#define oldpat_DFN    (oldcom_DFN    + 1)
#define patnbr_DFN    (oldpat_DFN    + 1)
#define position_DFN  (patnbr_DFN    + 1)
#define savflg_DFN    (position_DFN  + 1)
#define sig_DFN       (savflg_DFN    + 1)
#define simflag_DFN   (sig_DFN       + 1)
#define simval_DFN    (simflag_DFN   + 1)
#define subseq_DFN    (simval_DFN    + 1)
#define text_DFN      (subseq_DFN    + 1)
#define time_DFN      (text_DFN      + 1)
#define usrval_DFN    (time_DFN      + 1)
#define value_DFN     (usrval_DFN    + 1)
#define flags_DFN     (value_DFN     + 1)
#define time_unit_DFN (flags_DFN     + 1)
#define time_step_DFN (time_unit_DFN + 1)

#define MAXCMD_DFN    (time_step_DFN + 1)
