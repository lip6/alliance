/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
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

/* edit window */

#define E_NO_ICON 0xFF
#define E_DY_TXT 12

#define E_S_BKPT 0
#define E_S_BKPT_X 0
#define E_S_BKPT_Y 2
#define E_S_BKPT_DX 84
#define E_S_BKPT_DY 16
#define E_S_BKPT_T 6

#define E_C_BKPT 1
#define E_C_BKPT_X E_S_BKPT_X
#define E_C_BKPT_Y E_S_BKPT_Y+E_S_BKPT_DY+2
#define E_C_BKPT_DX 93
#define E_C_BKPT_DY 16
#define E_C_BKPT_T 8

#define E_SETTRACE 2
#define E_SETTRACE_X E_C_BKPT_X+E_C_BKPT_DX+2
#define E_SETTRACE_Y E_C_BKPT_Y
#define E_SETTRACE_DX 70
#define E_SETTRACE_DY 16
#define E_SETTRACE_T 8

#define E_UNSTRACE 3
#define E_UNSTRACE_X E_S_BKPT_X+E_S_BKPT_DX+2
#define E_UNSTRACE_Y 2
#define E_UNSTRACE_DX 79
#define E_UNSTRACE_DY 16
#define E_UNSTRACE_T  8

#define E_RUN 4
#define E_RUN_X E_UNSTRACE_X+E_UNSTRACE_DX+8
#define E_RUN_Y 2
#define E_RUN_DX 32
#define E_RUN_DY 16
#define E_RUN_T 7

#define E_STEP 5
#define E_STEP_X E_RUN_X+E_RUN_DX+2
#define E_STEP_Y 2
#define E_STEP_DX 38
#define E_STEP_DY 16
#define E_STEP_T 7

#define E_LOAD 6
#define E_LOAD_X E_COMPILE_X+E_COMPILE_DX+8
#define E_LOAD_Y 2
#define E_LOAD_DX 39
#define E_LOAD_DY 16
#define E_LOAD_T 8

#define E_SAVE 7
#define E_SAVE_X E_LOAD_X
#define E_SAVE_Y E_LOAD_Y+E_LOAD_DY+2
#define E_SAVE_DX 36
#define E_SAVE_DY 16
#define E_SAVE_T 6

#define E_EXIT 8
#define E_EXIT_X E_LOAD_X+E_LOAD_DX+2
#define E_EXIT_Y 2
#define E_EXIT_DX 39
#define E_EXIT_DY 16
#define E_EXIT_T 9

#define E_COMPILE 9
#define E_COMPILE_X E_STEP_X+E_STEP_DX+2
#define E_COMPILE_Y 2
#define E_COMPILE_DX 56
#define E_COMPILE_DY 16
#define E_COMPILE_T 7

#define E_NEXT 10
#define E_NEXT_X E_PARAM_X+E_PARAM_DX+2
#define E_NEXT_Y E_PARAM_Y
#define E_NEXT_DX 36
#define E_NEXT_DY 16
#define E_NEXT_T 6

#define E_PARAM 11
#define E_PARAM_X E_START_X+E_START_DX+2
#define E_PARAM_Y E_START_Y
#define E_PARAM_DX 48
#define E_PARAM_DY 16
#define E_PARAM_T 6

#define E_SHELL 12
#define E_SHELL_X E_SAVE_X+E_SAVE_DX+2
#define E_SHELL_Y E_SAVE_Y
#define E_SHELL_DX 42
#define E_SHELL_DY 16
#define E_SHELL_T 6

#define E_START 13
#define E_START_X E_RUN_X
#define E_START_Y E_RUN_Y+E_RUN_DY+2
#define E_START_DX 42
#define E_START_DY 16
#define E_START_T 6
