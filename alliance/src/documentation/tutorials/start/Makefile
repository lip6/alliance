# start 

%.eps : %.fig
	fig2dev -L ps -c -p dummy $*.fig $*.eps

distrib : start.pdf
	cp start.pdf ..

start.pdf : start.dvi
	dvipdf start.dvi

start.dvi : start.tex
	latex start.tex
	latex start.tex

clean :
	rm -f $(MYFILE).ps $(MYFILE).pdf *.log *.dvi *.aux
