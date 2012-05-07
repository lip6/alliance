
# If ALLIANCE_TOP is *not* set, assume a packaged installation.
 ALLIANCE_TOP       ?= /usr/lib/alliance
 RUN_IN_SOURCE_TREE ?= False

# Standart System binary access paths.
 STANDART_BIN        = /usr/local/bin:/usr/bin:/bin
 STANDART_PATH       = PATH=$(STANDART_BIN); export PATH

ifeq ($(ALLIANCE_TOP),/usr/lib/alliance)
  # Packaged installation.
   ALLIANCE_BIN      = /usr/bin
   SYSCONF_TOP       = /etc/alliance
   CELLS_TOP         = /usr/share/alliance/cells
   ifeq ($(findstring Ubuntu,$(shell uname -v)),Ubuntu)
     ALLIANCE_EXAMPLES = /usr/share/alliance/examples/alliance-examples
   else
     ALLIANCE_EXAMPLES = /usr/share/doc/alliance-doc-5.0/alliance-examples
   endif
else
  # Most likely hand-made installation (from .tar.gz by autotools).
  #ALLIANCE_TOP      = $(HOME)/alliance/Linux.slsoc6x/install
   ALLIANCE_BIN      = $(ALLIANCE_TOP)/bin
   SYSCONF_TOP       = $(ALLIANCE_TOP)/etc
   CELLS_TOP         = $(ALLIANCE_TOP)/cells
   ifeq ($(RUN_IN_SOURCE_TREE),True)
    # Examples in the source tree (conventional location).
     ALLIANCE_EXAMPLES = $(HOME)/alliance/src/documentation/alliance-examples
   else
    # Examples in the installed tree.
     ALLIANCE_EXAMPLES = $(ALLIANCE_TOP)/examples/alliance-examples
   endif
endif

 RDS_TECHNO_SYMB     = $(ALLIANCE_EXAMPLES)/etc/techno-symb.rds
 RDS_TECHNO_REAL     = $(ALLIANCE_EXAMPLES)/etc/techno-035.rds
 SPI_MODEL           = $(SYSCONF_TOP)/spimodel.cfg

