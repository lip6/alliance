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

#define C_NO_ICON        0xFF

#define C_SR          0
#define C_SR_X          8
#define C_SR_Y          2
#define C_SR_DX        (12*6)
#define C_SR_DY        16
#define C_SR_T          12

#define C_RR          1
#define C_RR_X          C_SR_X+C_SR_DX+2
#define C_RR_Y          C_SR_Y
#define C_RR_DX        (12*6)
#define C_RR_DY        16
#define C_RR_T          12

#define C_SD          2
#define C_SD_X          8
#define C_SD_Y          C_SR_Y+C_SR_DY+2
#define C_SD_DX        (13*6)
#define C_SD_DY        16
#define C_SD_T          12

#define C_RD          3
#define C_RD_X          C_SD_X+C_SD_DX+2
#define C_RD_Y          C_SD_Y
#define C_RD_DX        (13*6)
#define C_RD_DY        16
#define C_RD_T          12

#define C_GEOM          4
#define C_GEOM_X        C_VI_X+C_VI_DX+2
#define C_GEOM_Y        C_VI_Y+C_VI_DY+2
#define C_GEOM_DX        (16*6)
#define C_GEOM_DY        16
#define C_GEOM_T        6

#define C_EX          5
#define C_EX_X          C_RD_X+C_RD_DX+2
#define C_EX_Y          C_RD_Y
#define C_EX_DX        (6*6)-2
#define C_EX_DY        16
#define C_EX_T          11

#define C_CA          6
#define C_CA_X          C_RR_X+C_RR_DX+2
#define C_CA_Y          C_RR_Y
#define C_CA_DX        (8*6)-2
#define C_CA_DY        16
#define C_CA_T          5

#define C_VI          7
#define C_VI_X          8
#define C_VI_Y          C_SD_Y+C_SD_DY+7
#define C_VI_DX        (16*6)
#define C_VI_DY        16
#define C_VI_T          6

#define C_PE          8
#define C_PE_X          C_VI_X+C_VI_DX+2
#define C_PE_Y          C_VI_Y
#define C_PE_DX        (16*6)
#define C_PE_DY        16
#define C_PE_T          6

#define C_SC          9
#define C_SC_X          8
#define C_SC_Y          C_VI_Y+C_VI_DY+2
#define C_SC_DX        (16*6)
#define C_SC_DY        16
#define C_SC_T          6

#define C_AV          10
#define C_AV_X          8
#define C_AV_Y          C_SC_Y+C_SC_DY+7
#define C_AV_DX        (16*6)
#define C_AV_DY        16
#define C_AV_T          15

#define C_AI          11
#define C_AI_X          C_AV_X+C_AV_DX+2
#define C_AI_Y          C_AV_Y
#define C_AI_DX        (16*6)
#define C_AI_DY        16
#define C_AI_T          9

#define C_COL1          2
#define C_COL2          106
#define C_Y_Switch      C_AV_Y+C_AV_DY+4
#define C_DY_Switch      17
#define C_DX_Switch      102
#define C_DYIconText      10

#define C_Xon          8
#define C_Xoff          C_COL2
#define C_Xrect        C_COL1
#define C_DYstr        9
#define C_DXrect        (C_COL2+C_DX_Switch-C_COL1)
#define C_DYrect        12

#define C_W_WIDTH        (C_COL1+C_COL2+C_DX_Switch)
#define C_W_HEIGHT      (C_Y_Switch+(C_NB_OBJECT*C_DY_Switch))


#define C_SWITCH_OFF      0
#define C_SWITCH_ON      1
#define C_NO          0
#define C_YES          1
#define C_VALID        'V'
#define C_DELETE        'D'
#define C_UNDO          'u'
#define C_NORMAL        0
#define C_INVERSE        1
#define C_DY_TXT        12

#define C_PATTERN        0
#define C_FILL          1
#define C_OUTLINE        2
