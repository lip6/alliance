LIB = grol
OUTIL = grog

vti:
	> vti.log ;\
	date "+%d-%b-%y %l:%M" > vti.atr ;\
	echo $(OUTIL) >> vti.atr ;\
	echo locked >> vti.atr ;\
	echo 2 >> vti.atr ;\
	echo $(LIB) > vti.idx ;\
	cp vti.atr vlsi.atr ;\
	cp vti.idx vlsi.idx ;\
	cp vti.log vlsi.log ;\
	makedb $(LIB)

CATAL:
	> CATAL;\
	for i in `ls *.cp` ; do \
	        B=`basename $$i .cp`;\
	        echo $$B" C" >> CATAL;\
	done
