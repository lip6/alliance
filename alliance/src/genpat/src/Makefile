#
# This file is part of the Alliance CAD System
# Copyright (C) Laboratoire LIP6 - Département ASIM
# Universite Pierre et Marie Curie
# 
# Home page          : http://www-asim.lip6.fr/alliance/
# E-mail             : mailto:alliance-users@asim.lip6.fr
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

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

LOCAL_TRGBIN   = $(TARGET_BIN)
LOCAL_TRGLIB   = $(TARGET_LIB)
LOCAL_TRGINC   = $(TARGET_INCLUDE)

LOCAL_INC      = $(ALLIANCE_INCLUDE)


LOCAL_CFLAGS = $(CFLAGS) -Wall -I$(LOCAL_INC) -I$(ALLIANCE_INCLUDE) \
                 -DPPT_H='"$(PPT_H)"' \
		 -DPHL_H='"$(PHL_H)"' \
                 -DPAT_H='"$(PAT_H)"' \
                 -DMUT_H='"$(MUT_H)"'

LOCAL_OBJECTS  =  ARRAY.o  DECLAR.o DEF_GEN.o  SAV_GEN.o INIT.o  AFFECT.o   \
                  SAVE.o   CONV.o   libpat_l.o GETCPAT.o LABEL.o SETTUNIT.o


LOCAL_LIBLIST  = -lPgn103 $(PPT_L) $(PAT_L) $(PHL_L) $(MUT_L)



.c.o: 
	$(CC) -c $< $(LOCAL_CFLAGS)

all $(LOCAL_TRGBIN)/genpat : $(LOCAL_TRGLIB)/libPgn103.a
	$(CP) genpat   $(LOCAL_TRGBIN)/genpat
	$(CP) genpat.h $(LOCAL_TRGINC)/genpat.h
	cd tests ; $(MAKE)

$(LOCAL_TRGLIB)/libPgn103.a : $(LOCAL_OBJECTS)
	$(AR) $(ARFLAGS) $(LOCAL_TRGLIB)/libPgn103.a $(LOCAL_OBJECTS)
	$(RANLIB) $(LOCAL_TRGLIB)/libPgn103.a


clean :
	-$(RM) -f $(LOCAL_OBJECTS) 
	-$(RM) -f *.bak .*.bak *~ .*~ core 

distclean realclean : clean
	-$(RM) -f $(LOCAL_TRGBIN)/genpat $(LOCAL_TRGLIB)/libPgn103.a $(LOCAL_TRGINC)/genpat.h


