#                          Alliance VLSI CAD System
#
#     Product : OVER-THE-CELL ROUTER
#     File    : Makefile
#
#     Author(s)   : Mael NAGAT                         Date : 27/05/1999
#     Modified by :                                    Date : ../../....
#     Modified by :                                    Date : ../../....
#     Modified by :                                    Date : ../../....

# Alliance Shared Libraries and Include Files for Makefiles
include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk

# Sources
OCR_SRC_FILE =

# Include Flags for Alliance Shared Libraries
OCR_ALC_INCLUDE = -I$(ALLIANCE_INCLUDE) \
                  -DMLU_H='<$(MLU_H)>' \
                  -DMPU_H='<$(MPU_H)>' \
                  -DMLO_H='<$(MLO_H)>' \
                  -DMPH_H='<$(MPH_H)>' \
                  -DMUT_H='<$(MUT_H)>'

# Libraries Flags for Alliance Shared Libraries
OCR_ALC_LIB = -L$(ALLIANCE_LIB) \
              $(MLU_L) \
              $(MPU_L) \
              $(MCP_L) \
              $(MAP_L) \
              $(MMG_L) \
              $(MCL_L) \
              $(MGL_L) \
              $(MAL_L) \
              $(MVL_L) \
              $(MEL_L) \
              $(MSL_L) \
              $(MHL_L) \
              $(RCN_L) \
              $(MLO_L) \
              $(MPH_L) \
              $(MUT_L)

# Compilation Flags
OCR_CFLAGS = -Wall -O4 -pg

# Non file targets
.PHONY: clean

TARGET_DIR  = $(HOME)/local/$(ALLIANCE_OS)/bin

# Rules

$(TARGET_DIR)/densite : densite.c
	ctags *.c
	$(CC) $(OCR_CFLAGS) $(OCR_ALC_INCLUDE) $< $(OCR_ALC_LIB) -o $@

clean :
	-$(RM) -f $(TARGET_DIR)/densite *.o
	-$(RM) -f *~ 
