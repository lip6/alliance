#
# %% \textrm{This File Looks Nicer with\textsf{ LGrind} and \LaTeXe.}        %%
# %|                                                                         |%
# %| $Id: fpgen.mk,v 1.2 1999/11/05 16:03:19 czo Exp $          |%
# %|                                                                         |%
# %| /---------------------------------------------------------------------\ |%
# %| |                                                                     | |%
# %| |            Alliance CAD System 3.2b                                  | |%
# %| |       F p G e n   P a c k a g e   1.10                              | |%
# %| |                                                                     | |%
# %| |    Copyright(c) 95-97, ASIM, CAO-VLSI Team                          | |%
# %| |                                                                     | |%
# %| |  Author    :                Jean-Paul CHAPUT                        | |%
# %| |  E-mail    :   alliance-support@asim.lip6.fr                        | |%
# %| | =================================================================== | |%
# %| |  File      :   "<>/dev/etc/fpgen.mk"                                | |%
# %| | ******************************************************************* | |%
# %| |  U p d a t e s                                                      | |%
# %| |                                                                     | |%
# %| \---------------------------------------------------------------------/ |%
#

#
# %| /---------------------------------------------------------------------\ |%
# %| |                                                                     | |%
# %| |                   Macros and Alliance libraries                     | |%
# %| |                                                                     | |%
# %| \---------------------------------------------------------------------/ |%
#

include $(TOP)/etc/$(ALLIANCE_OS).mk
include $(TOP)/etc/libraries.mk


#
# %| /---------------------------------------------------------------------\ |%
# %| |                                                                     | |%
# %| |                       MBK Additionnal Macros                        | |%
# %| |                                                                     | |%
# %| \---------------------------------------------------------------------/ |%
#

  LOCAL_ALC_IDIR = $(DEVEL_INCLUDE) -I$(ALLIANCE_INCLUDE)
  LOCAL_ALC_DEFS = -DALLIANCE_VERSION=$(ALLIANCE_VERSION) \
                   -DFGN_H='<$(FGN_H)>' \
                   -DGFP_H='<$(GFP_H)>' \
                   -DGRF_H='<$(GRF_H)>' \
                   -DGBS_H='<$(GBS_H)>' \
                   -DGAM_H='<$(GAM_H)>' \
                   -DRSA_H='<$(GGA_H)>' \
                   -DAPR_H='<$(APR_H)>' \
                   -DMGN_H='<$(MGN_H)>' \
                   -DMLU_H='<$(MLU_H)>' \
                   -DMLO_H='<$(MLO_H)>' \
                   -DMPH_H='<$(MPH_H)>' \
                   -DMPU_H='<$(MPU_H)>' \
                   -DMUT_H='<$(MUT_H)>' \
                   -DMGN_H='<$(MGN_H)>' \
                   -DBEH_H='<$(BEH_H)>' \
                   -DLOG_H='<$(LOG_H)>'
  LOCAL_ALC_INCS = $(LOCAL_ALC_IDIR) $(LOCAL_ALC_DEFS)

  LOCAL_ALC_LDIR = $(DEVEL_LIB) -L$(ALLIANCE_LIB)
  LOCAL_ALC_LFPG = $(FGN_L) \
                   $(GFP_L)
  LOCAL_ALC_LGEN = $(GRF_L) \
                   $(GBS_L) \
                   $(GAM_L) \
                   $(GGA_L)
  LOCAL_ALC_LVBE = $(BVL_L) \
                   $(BEH_L) \
                   $(BHL_L) \
                   $(LOG_L)
  LOCAL_ALC_LAPR = $(APR_L)
  LOCAL_ALC_LICO = $(ICU_L) \
                   $(ICC_L) \
                   $(ICA_L) \
                   $(ICN_L)
  LOCAL_ALC_LMGN = $(MGN_L)
  LOCAL_ALC_LMUT = $(MUT_L)
  LOCAL_ALC_LMLO = $(MLU_L) \
                   $(MLO_L) \
                   $(MAL_L) \
                   $(MCL_L) \
                   $(MGL_L) \
                   $(MEL_L) \
                   $(MHL_L) \
                   $(MSL_L) \
                   $(MVL_L) \
                   $(RCN_L)
  LOCAL_ALC_LMPH = $(MPU_L) \
                   $(MPH_L) \
                   $(MAP_L) \
                   $(MCP_L) \
                   $(MMG_L)
  LOCAL_ALC_LIBS = $(LOCAL_ALC_LDIR) \
                   $(LOCAL_ALC_LFPG) \
                   $(LOCAL_ALC_LGEN) \
                   $(LOCAL_ALC_LVBE) \
                   $(LOCAL_ALC_LAPR) \
                   $(LOCAL_ALC_LICO) \
                   $(LOCAL_ALC_LMGN) \
                   $(LOCAL_ALC_LMLO) \
                   $(LOCAL_ALC_LMPH) \
                   $(LOCAL_ALC_LMUT)

  LOCAL_STD_LIBS = -lm

#
# %| /---------------------------------------------------------------------\ |%
# %| |                                                                     | |%
# %| |                          Internal Macros                            | |%
# %| |                                                                     | |%
# %| \---------------------------------------------------------------------/ |%
#
#  C Flags for normal compilation.
 LOCAL_CFLAGS = $(CFLAGS) -c -g -D$(ALLIANCE_OS)
 LOCAL_OFLAGS = $(OFLAGS) -o
#  C Flags for compilation and profiling.
#LOCAL_CFLAGS = $(CFLAGS) -p -c -g -D$(ALLIANCE_OS)
#LOCAL_OFLAGS = $(OFLAGS) -p -o


#
# %| /---------------------------------------------------------------------\ |%
# %| |                                                                     | |%
# %| |                         Compilation Rules                           | |%
# %| |                                                                     | |%
# %| \---------------------------------------------------------------------/ |%
#

all: $(CSOURCE)

$(CSOURCE): $(CSOURCE).o
	$(CC) $(LOCAL_OFLAGS) $@ $? $(LOCAL_ALC_LIBS) $(LOCAL_STD_LIBS)
	rm -f $?

$(CSOURCE).o: $(CSOURCE).c
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_ALC_INCS) $?


clean:
	rm -f $(CSOURCE) $(CSOURCE).o
