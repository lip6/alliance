
all: compil result_vbe.pat result_dly.pat

compil:
	@echo "***** compile *****"
	asimut -b -c addaccu

result_vbe.pat : addaccu.vbe patterns.pat
	@echo "***** test zero delay *****"
	asimut -b addaccu patterns result_vbe

result_dly.pat : addaccu_dly.vbe patterns_dly.pat
	@echo "***** test with delay *****"
	asimut -b addaccu_dly patterns_dly result_dly

clean :
	@echo "***** clean all .pat result *****"
	rm -f result_vbe.pat result_dly.pat
