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

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

#TARGET_LIB      = ../../../archi/$(ALLIANCE_OS)/lib
#TARGET_INCLUDE  = ../../../archi/$(ALLIANCE_OS)/include

LOCAL_INC       = ../include

LOCAL_CFLAGS    = $(CFLAGS) -c -g
LOCAL_CPPFLAGS  = $(CPPFLAGS) -I$(LOCAL_INC) -I$(ALLIANCE_INCLUDE) \
                  -DPAT_H='<pat109.h>' \
                  -DPHL_H='<phl109.h>' \
                  -DMUT_H='<$(MUT_H)>'

LOCAL_OBJECTS   = pat_debug.o pat_getusage.o

.c.o :
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_CPPFLAGS) $<

$(TARGET_LIB)/libPhl109.a : $(LOCAL_OBJECTS)
	@$(CP) $(LOCAL_INC)/phl109.h $(TARGET_INCLUDE)
	$(AR) $(ARFLAGS) $(TARGET_LIB)/libPhl109.a $(LOCAL_OBJECTS)
	-$(RANLIB) $(TARGET_LIB)/libPhl109.a

distrib : $(TARGET_LIB)/libPhl109.a

clean :
	$(RM) -f $(LOCAL_OBJECTS)

realclean : clean
	$(RM) -f $(TARGET_LIB)/libPhl109.a $(TARGET_INCLUDE)/phl109.h
