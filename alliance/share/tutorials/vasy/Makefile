# /*------------------------------------------------------------\
# |                                                             |
# | This file is part of the Alliance CAD System Copyright      |
# | (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
# |                                                             |
# | Home page      : http://www-asim.lip6.fr/alliance/          |
# | E-mail support : mailto:alliance-support@asim.lip6.fr       |
# |                                                             |
# | This progam is  free software; you can redistribute it      |
# | and/or modify it under the  terms of the GNU General Public |
# | License as  published by the Free Software Foundation;      |
# | either version 2 of the License, or (at your option) any    |
# | later version.                                              |
# |                                                             |
# | Alliance VLSI  CAD System  is distributed  in the hope that |
# | it  will be useful, but WITHOUT  ANY WARRANTY;              |
# | without even the  implied warranty of MERCHANTABILITY or    |
# | FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
# | Public License for more details.                            |
# |                                                             |
# | You should have received a copy  of the GNU General Public  |
# | License along with the GNU C Library; see the file COPYING. |
# | If not, write to the Free Software Foundation, Inc.,        |
# | 675 Mass Ave, Cambridge, MA 02139, USA.                     |
# |                                                             |
# \------------------------------------------------------------*/

# /*------------------------------------------------------------\
# |                                                             |
# | Tool   :                      VASY                          |
# |                                                             |
# | File   :                    Makefile                        |
# |                                                             |
# | Author :                 Jacomme Ludovic                    |
# |                                                             |
# | Date    :                   30_01_98                        |
# |                                                             |
# \------------------------------------------------------------*/

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

# /*------------------------------------------------------------\
# |                                                             |
# |                              Cells                          |
# |                                                             |
# \------------------------------------------------------------*/
# /*------------------------------------------------------------\
# |                                                             |
# |                             Binary                          |
# |                                                             |
# \------------------------------------------------------------*/

VASY   = $(ALLIANCE_TOP)/bin/vasy
ASIMUT = $(ALLIANCE_TOP)/bin/asimut

# /*------------------------------------------------------------\
# |                                                             |
# |                            Environement                     |
# |                                                             |
# \------------------------------------------------------------*/

ENV_VASY = MBK_WORK_LIB=.; export MBK_WORK_LIB;\
           MBK_CATAL_NAME=NO_CATAL; export MBK_CATAL_NAME

ENV_ASIMUT = MBK_WORK_LIB=.; export MBK_WORK_LIB;\
             MBK_CATAL_NAME=CATAL; export MBK_CATAL_NAME;\
             MBK_IN_LO=vst; export MBK_IN_LO;\
	     MBK_OUT_LO=vst; export MBK_OUT_LO

all : result_1.pat result_2.pat result_3.pat

# /*------------------------------------------------------------\
# |                                                             |
# |                             Asimut                          |
# |                                                             |
# \------------------------------------------------------------*/

result_1.pat : hadamard.vst
	$(ENV_ASIMUT); $(ASIMUT) hadamard hadamard_1 result_1 
result_2.pat : hadamard.vst
	$(ENV_ASIMUT); $(ASIMUT) hadamard hadamard_2 result_2 
result_3.pat : hadamard.vst
	$(ENV_ASIMUT); $(ASIMUT) hadamard hadamard_3 result_3 

# /*------------------------------------------------------------\
# |                                                             |
# |                             Vasy                            |
# |                                                             |
# \------------------------------------------------------------*/

hadamard.vst : hadamard.vhdl
	$(ENV_VASY); $(VASY) -a -I vhdl -H hadamard


# /*------------------------------------------------------------\
# |                                                             |
# |                              Clean                          |
# |                                                             |
# \------------------------------------------------------------*/

realclean : clean

clean     :
	$(RM) -f *.vst *.vbe result_1.pat result_2.pat result_3.pat
	
