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
# 
# /*------------------------------------------------------------\
# |                                                             |
# | Tool   :                      FSP                           |
# |                                                             |
# | File   :                    Makefile                        |
# |                                                             |
# | Author :                 Jacomme Ludovic                    |
# |                                                             |
# | Date   :                    28.10.94                        |
# |                                                             |
# \------------------------------------------------------------*/

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk
include local.mk

# /*------------------------------------------------------------\
# |                                                             |
# |                            Variables                        |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_CFLAGS = $(CFLAGS) -g -D$(ALLIANCE_OS)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Binary                          |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_FSP_BIN = fsp

# /*------------------------------------------------------------\
# |                                                             |
# |                             Include                         |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_INCLUDE = -I$(ALLIANCE_INCLUDE) -I$(LOCAL_TARGET_INC)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Library                         |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_MBK_LIB = -L$(ALLIANCE_LIB) \
                $(MUT_L)

LOCAL_ABL_LIB = -L$(LOCAL_TARGET_LIB) \
                $(ABL_L)              \
                $(AUT_L)

LOCAL_BDD_LIB = -L$(LOCAL_TARGET_LIB) \
                $(BDD_L)

LOCAL_FSM_LIB = -L$(LOCAL_TARGET_LIB) \
                $(FTL_L) \
                $(FKS_L) \
                $(FVH_L) \
                $(FSM_L) \
                $(FBL_L) \
                $(FVH_L)

LOCAL_LIB = $(LOCAL_FSM_LIB) $(LOCAL_BDD_LIB) $(LOCAL_ABL_LIB) $(LOCAL_MBK_LIB)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Define                          |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_MBK_DEFINE   = -DMUT_H='"$(MUT_H)"'

LOCAL_FSM_DEFINE   = -DFSM_H='"$(FSM_H)"' \
                     -DFTL_H='"$(FTL_H)"' \
                     -DFVH_H='"$(FVH_H)"'

LOCAL_BDD_DEFINE   = -DBDD_H='"$(BDD_H)"'

LOCAL_ABL_DEFINE   = -DAUT_H='"$(AUT_H)"' \
                     -DABL_H='"$(ABL_H)"'

LOCAL_FSP_DEFINE   = -DALLIANCE_VERSION=$(ALLIANCE_VERSION) \
                     -DFSP_VERSION='"1.01"'

LOCAL_DEFINE = $(LOCAL_MBK_DEFINE) $(LOCAL_BDD_DEFINE) \
               $(LOCAL_FSP_DEFINE) $(LOCAL_FSM_DEFINE) \
               $(LOCAL_ABL_DEFINE) 

LOCAL_HEADER = $(LOCAL_INCLUDE) $(LOCAL_DEFINE)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Object                          |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_FSP_OBJ = fsp_debug.o fsp_proof.o fsp_comp.o fsp_main.o

LOCAL_OBJ = $(LOCAL_FSP_OBJ)

.c.o:
	$(CC) -c $(LOCAL_CFLAGS) $(LOCAL_HEADER) $<

# /*------------------------------------------------------------\
# |                                                             |
# |                              Fsp                            |
# |                                                             |
# \------------------------------------------------------------*/

distrib : $(LOCAL_TARGET_BIN)/$(LOCAL_FSP_BIN)

$(LOCAL_TARGET_BIN)/$(LOCAL_FSP_BIN) : $(LOCAL_OBJ)
	$(CC) $(LOCAL_CFLAGS) -o $(LOCAL_TARGET_BIN)/$(LOCAL_FSP_BIN) \
              $(LOCAL_OBJ) $(LOCAL_LIB) -lm

# /*------------------------------------------------------------\
# |                                                             |
# |                              Clean                          |
# |                                                             |
# \------------------------------------------------------------*/

clean :
	$(RM) -f $(LOCAL_OBJ)

realclean :
	$(RM) -f $(LOCAL_OBJ) $(LOCAL_TARGET_BIN)/$(LOCAL_FSP_BIN)
