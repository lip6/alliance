# Generic Makefile for TeTeX projet
# (C) 1999, Czo
# $Id: Makefile,v 1.2 2004/07/14 22:37:14 ludo Exp $

MYFILE=start

%.eps : %.fig
	fig2dev -L ps -c -p dummy $*.fig $*.eps

distrib : clean ps        
	ps2pdf $(MYFILE).ps
	cp -f $(MYFILE).ps ..
	cp -f $(MYFILE).pdf ..
	$(MAKE) clean


view : ps
	gv $(MYFILE).ps
        
ps   : $(MYFILE).tex
	latex $(MYFILE).tex
	dvips $(MYFILE).dvi -o $(MYFILE).ps

clean :
	rm -f $(MYFILE).ps $(MYFILE).pdf *.log *.dvi *.aux
