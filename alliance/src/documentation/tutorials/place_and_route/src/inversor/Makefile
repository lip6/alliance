
all: COUGAR YAGLE PROOF

COUGAR: 
	MBK_IN_PH=ap ;MBK_OUT_LO=al ;cougar -t inv_x1 inv_x1

YAGLE:
	export AVT_LICENSE_SERVER=beny.lip6.fr ;\
        export MBK_IN_LO=al ;\
        export YAGLE_BEH_FORMAT=vbe ;\
	/users/soft5/newlabo/AvtTools/tools/Linux_2.2/bin/yagle inv_x1 inv_x1
	
PROOF:
	proof inv_x1 inversor

clean:
	rm -f *.al inv_x1.vbe *.rep

