
all: COUGAR YAGLE PROOF

COUGAR: 
	MBK_IN_PH=ap ;MBK_OUT_LO=al ;cougar -t buf_x2 buf_x2

YAGLE:
	export AVT_LICENSE_SERVER=beny.lip6.fr ;\
        export MBK_IN_LO=al ;\
        export YAGLE_BEH_FORMAT=vbe ;\
	/users/soft5/newlabo/AvtTools/tools/Linux_2.2/bin/yagle buf_x2 buf_x2
	
PROOF:
	proof buf_x2 buffer

clean:
	rm -f *.al buf_x2.vbe *.rep

