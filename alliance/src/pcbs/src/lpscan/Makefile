#
# Makefile for the linux kernel.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
# Note 2! The CFLAGS definitions are now in the main makefile...


##                                                                 
##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems 
##  (C) 1995, MASI, CAO-VLSI Team				     
##                                                                 
##  Authors            : Philippe OLEK, Olivier SIROL, Francois Donnet 	     
##                          (Projet Maitrise EEA)                  
##  Initial Revision   : April 1995                                
##  Revision           : June  1995                                
##  Revision           : Febr  2002                                
##                                                                 
##  E-mail        :    cao-vlsi@masi.ibp.fr			     
##                                                                 

# noyau 2.4.9-21
obj-$(CONFIG_LPSCAN) := lpscan.o

# noyau 2.2.10
L_OBJS   := lpscan.o

#target 
L_TARGET := lpscan.a

include $(TOPDIR)/Rules.make
