
all: EXAMPLE VAR  CATAL02 res.pat


VAR:
	MBK_IN_LO=vst;export MBK_IN_LO ;\
	MBK_CATA_LIB=$ALLIANCE_TOP/cells/sxlib;export MBK_CATA_LIB

CATAL01:
	echo amd2901_ctl C >CATAL
	echo amd2901_dpt C >>CATAL

CATAL02:
	echo amd2901_dpt C >CATAL

EXAMPLE:
	genlib circuit



res.pat: amd2901_chip.vst pattern.pat amd2901_core.vst CATAL
	 asimut -zd amd2901_chip pattern res
	 touch amd2901_chip.vst


amd2901_chip.vst: amd2901_chip.c amd2901_core.vst
		  genlib amd2901_chip

amd2901_core.vst: amd2901_core.c amd2901_ctl.vst amd2901_dpt.vst amd2901_ctl.vbe amd2901_dpt.vbe
		genlib amd2901_core

amd2901_dpt.vst: amd2901_dpt.c
		genlib amd2901_dpt

amd2901_ctl.vst: amd2901_ctl.c
		genlib amd2901_ctl


res2.pat: amd2901_chip.vst pattern.pat amd2901_core.vst CATAL
	asimut amd2901_chip pattern res2
	touch amd2901_chip.vst

clean :
	rm -f Makefile-*                 \
		CATAL \
              circuit.vst \
		data_path.vst \
		amd2901_core.vst          \
              amd2901_chip.vst          \
              amd2901_ctl.vst           \
              amd2901_dpt.vst           \
              model_*                    \
                res.pat  \
                *.frr                      \
              *.log *.rin *.scr *.ap               \
              *.drc                      \
              *.gds                      \
              *.def


