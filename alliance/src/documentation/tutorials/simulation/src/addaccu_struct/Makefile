ALLIANCE_TOP	= /asim/alliance


all: 	pat_new.pat res_new.pat temp catal catal01 catal02 catal03 valid	


pat_new.pat : pat_new.c
	@echo "***** genpat : stimuli generation ******"
	genpat pat_new

res_new.pat : addaccu.vbe pat_new.pat
	@echo "***** simulation zero delay *****"
	asimut -b -zerodelay addaccu pat_new res_new

temp :
	@echo "***** variable MBK_IN_LO *****"  
	@echo "***** structural validation *****" 
	MBK_IN_LO=vst;export MBK_IN_LO;asimut -c addaccu
	@echo "***** simulation zero delay without result *****" 
	asimut -zerodelay -nores addaccu res_new
	
catal :
	@echo "***** variable MBK_CATA_LIB *****"
	MBK_CATA_LIB=$(ALLIANCE_TOP)/cells/sxlib; export MBK_CATA_LIB
	
catal01 :
	@echo "***** simulation *****" 
	echo accu C >> CATAL01
	echo alu  C >> CATAL01
	echo mux  C >> CATAL01
	mv CATAL01 CATAL
	asimut -c accu
	rm CATAL
	asimut -zerodelay -nores addaccu res_new 	
 
catal02 :
	@echo "***** simulation with accu.vst *****"
	echo alu C >> CATAL02
	echo mux C >> CATAL02
	mv CATAL02 CATAL
	asimut -c alu
	asimut -zerodelay -nores addaccu res_new

catal03 :
	@echo "***** simulation with accu.vst alu.vst *****"
	echo mux C >> CATAL03
	mv CATAL03 CATAL
	asimut -c mux
	rm CATAL
	asimut -zerodelay -nores addaccu res_new

valid :
	@echo "***** test with pattern *****"
	asimut addaccu pat_new result_dly

		
clean :
	@echo "***** clean all .pat result *****"
	rm -f res_new.pat result_dly.pat pat_new.pat
