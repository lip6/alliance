#
# This file is part of the Alliance CAD System
# Copyright (C) Laboratoire LIP6 - Département ASIM
# Universite Pierre et Marie Curie
# 
# Home page          : http://www-asim.lip6.fr/alliance/
# E-mail support     : mailto:alliance-support@asim.lip6.fr
# 
# This progam is  free software; you can redistribute it  and/or modify it
# under the  terms of the GNU  General Public License as  published by the
# Free Software Foundation;  either version 2 of the License,  or (at your
# option) any later version.
# 
# Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
# useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
# 
# You should have received a copy  of the GNU General Public License along
# with the GNU C Library; see the  file COPYING. If not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

#/***************************************************************************/
#/*                                                                         */
#/*                     Chaine de CAO & VLSI   Alliance                     */
#/*                                                                         */
#/*   Produit :  (R)ectangle (D)ata (S)tructure (2)to (P)ost(S)cript driver */
#/*   Fichier :  Makefile                                                   */
#/*                                                                         */
#/*   (c) copyright 1991 Laboratoire MASI equipe CAO & VLSI                 */
#/*   Tous droits reserves                                                  */
#/*   Support : e-mail cao-vlsi@masi.ibp.fr                                 */
#/*                                                                         */
#/*   Auteur(s) :  Gilles-Eric DESCAMPS                 le : 23/03/1993     */
#/*                                                                         */
#/*   Modifie par :  GED toujours                       le : 05/10/1993     */
#/*   Modifie par :  EL                                 le : 02/03/1995     */
#/*                                                                         */
#/***************************************************************************/

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

RPS_VERSION  = '"1.12"'
RPS_TAIL     = 103

NROFF            = /usr/bin/nroff -man

INC = -DALLIANCE_VERSION=$(ALLIANCE_VERSION) -DRPS_VERSION=$(RPS_VERSION) \
	-DRPS_H='"$(RPS_INC_DEST)/$(RPS_INC)"' -I$(ALLIANCE_INCLUDE) \
	-DRFM_H='<$(RFM_H)>' \
	-DRUT_H='<$(RUT_H)>' \
	-DRDS_H='<$(RDS_H)>' \
	-DRPR_H='<$(RPR_H)>' \
	-DRTL_H='<$(RTL_H)>' \
	-DMPH_H='<$(MPH_H)>' \
	-DRWI_H='<$(RWI_H)>' \
	-DMPU_H='<$(MPU_H)>' \
	-DMUT_H='<$(MUT_H)>'

LIB = -L$(RPS_LIB_DEST) -lRps$(RPS_TAIL) -L$(ALLIANCE_LIB) \
      $(RTL_L) \
      $(RGS_L) \
      $(RCF_L) \
      $(RUT_L) \
      $(RFM_L) \
      $(RPR_L) \
      $(RDS_L) \
      $(MPU_L) \
      $(MCP_L) \
      $(MAP_L) \
      $(MGL_L) \
      $(MPH_L) \
      $(MMG_L) \
      $(MUT_L) \
      -lm

RPS_BIN      = l2p
RPS_LIB      = libRps$(RPS_TAIL).a
RPS_INC      = rps$(RPS_TAIL).h
RPS_BIN_DEST = $(TARGET_BIN)
RPS_LIB_DEST = $(TARGET_LIB)
RPS_INC_DEST = $(TARGET_INCLUDE)

all : $(RPS_INC_DEST)/$(RPS_INC) \
      $(RPS_LIB_DEST)/$(RPS_LIB) \
      $(RPS_BIN_DEST)/$(RPS_BIN)

distrib : all
	-$(RM) $(RPS_BIN_DEST)/INSTALLED
	-$(RM) $(RPS_LIB_DEST)/INSTALLED
	-$(RM) $(RPS_INC_DEST)/INSTALLED

$(RPS_INC_DEST)/$(RPS_INC) : rps_inc.h
	$(CP) rps_inc.h $(RPS_INC_DEST)/$(RPS_INC)

$(RPS_LIB_DEST)/$(RPS_LIB) : drive_ps.o tmp_dict.o
	$(AR) $(ARFLAGS) $(RPS_LIB_DEST)/$(RPS_LIB) drive_ps.o tmp_dict.o
	$(RANLIB) $(RPS_LIB_DEST)/$(RPS_LIB)

$(RPS_BIN_DEST)/$(RPS_BIN) : $(RPS_LIB_DEST)/$(RPS_LIB) \
	$(RPS_INC_DEST)/$(RPS_INC) rds2ps.o tmp_man.o
	$(CC) $(CFLAGS) -o $(RPS_BIN_DEST)/$(RPS_BIN) rds2ps.o tmp_man.o $(LIB)
	$(STRIP) $(RPS_BIN_DEST)/$(RPS_BIN)

drive_ps.o : drive_ps.c $(RPS_INC_DEST)/$(RPS_INC)
	$(CC)  $(CFLAGS) -c $(INC) drive_ps.c

tmp_dict.c : dict_bw.ps dict_color.ps
	$(AWK) 'BEGIN{printf ("extern rps_put();\n"); \
			printf ("void rps_print_dict_bw (pg)\n");\
			printf ("void *pg;\n{\n")}\
		{printf ("\trps_put (pg,\"%s\\n\");\n", $$0)}\
		END{printf("}\n")}' dict_bw.ps > tmp_dict.c
	$(AWK) 'BEGIN{printf ("\nvoid rps_print_dict_color (pg)\n");\
			printf ("void *pg;\n{\n")}\
		{printf ("\trps_put (pg,\"%s\\n\");\n", $$0)}\
		END{printf("}\n")}' dict_color.ps >> tmp_dict.c

tmp_dict.o : tmp_dict.c
	$(CC) $(CFLAGS) -c $(INC) tmp_dict.c

rds2ps.o : rds2ps.c $(RPS_INC_DEST)/$(RPS_INC)
	$(CC) $(CFLAGS) -c $(INC) rds2ps.c

tmp_man.c : $(RPS_BIN).1
	$(NROFF) $(RPS_BIN).1 | \
	$(AWK) 'BEGIN{printf ("void puts ();\n"); \
			printf ("void rps_print_man () {\n")}\
		{printf ("\tputs (\"%s\");\n", $$0)}\
		END{printf("}\n")}' | \
	cat > tmp_man.c

tmp_man.o : tmp_man.c
	$(CC) $(CFLAGS) -c $(INC) tmp_man.c

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

clean :
	$(RM) -f *.o

realclean : clean
	$(RM) -f $(RPS_BIN_DEST)/$(RPS_BIN)
	$(RM) -f $(RPS_LIB_DEST)/$(RPS_LIB)
	$(RM) -f $(RPS_INC_DEST)/$(RPS_INC)

pure : $(RPS_INC_DEST)/$(RPS_INC) \
	$(RPS_LIB_DEST)/$(RPS_LIB) \
	purel2p

purel2p : $(RPS_LIB_DEST)/$(RPS_LIB) \
	$(RPS_INC_DEST)/$(RPS_INC) rds2ps.o tmp_man.o
	purify $(CC) -o purel2p rds2ps.o tmp_man.o $(LIB)

#- - - END OF MAKEFILE - - - - - - - - - - - - - - - - - - - - - - - - - - -
