all: VAR GEN compil

VAR: 
	@echo "variable"
	MBK_WORK_LIB=.; export MBK_WORK_LIB;\
        MBK_CATA_LIB=.; export MBK_CATA_LIB;\
	VH_BEHSFX=vbe; export VH_BEHSFX;\
	

GEN:
	@echo "genpat"
	genpat amdfindbug

compil	: amd_9.vbe
	for B in *.vbe ; do\
		F=`basename $$B .vbe`;\
		echo $$F;\
		asimut -b -zd  $$F amdfindbug res_$$F;\
	done

clean:
	@echo "erase all .pat generate"
	rm -f *.pat
