# Generic Makefile for TeTeX projet
# (C) 1999, Czo
# $Id: Makefile,v 1.2 2004/07/09 21:55:20 ludo Exp $

MYFILE=overview

%.eps : %.fig
	fig2dev -L ps -c -p dummy $*.fig $*.eps

%.eps : %.gif
	convert $*.gif $*.eps

all :  datapath.eps genview.eps graal.eps stacs.eps tools.eps ps
	@echo done...

view : all
	gv $(MYFILE).ps
        
ps   : $(MYFILE).tex
	latex $(MYFILE).tex
	dvips $(MYFILE).dvi -o $(MYFILE).ps

distrib : all
	dvipdf $(MYFILE).dvi
	cp -f $(MYFILE).ps ..
	cp -f $(MYFILE).pdf ..
	$(MAKE) clean

clean :
	rm -f $(MYFILE).ps $(MYFILE).pdf *.log *.dvi *.aux *.bak datapath.eps genview.eps graal.eps stacs.eps tools.eps

