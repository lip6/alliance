# Generic Makefile for TeTeX projet
# (C) 1999, Czo
# $Id: Makefile,v 1.3 2000/03/16 09:51:42 czo Exp $

MYFILE=overview

%.eps : %.fig
	fig2dev -L ps -c -p dummy $*.fig $*.eps

%.eps : %.gif
	convert $*.gif $*.eps

view : datapath.eps genview.eps graal.eps stacs.eps tools.eps ps
	gv $(MYFILE).ps
        
ps   : $(MYFILE).tex
	latex $(MYFILE).tex
	dvips $(MYFILE).dvi -o $(MYFILE).ps

distrib : clean ps        
	ps2pdf $(MYFILE).ps
	cp -f $(MYFILE).ps ..
	cp -f $(MYFILE).pdf ..
	$(MAKE) clean

clean :
	rm -f $(MYFILE).ps $(MYFILE).pdf *.log *.dvi *.aux *.bak datapath.eps genview.eps graal.eps stacs.eps tools.eps

