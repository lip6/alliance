# Generic Makefile for TeTeX projet
# (C) 1999, Czo
# $Id: Makefile,v 1.1 2000/01/20 16:10:03 czo Exp $

MYFILE=amd2901

view : ps
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
	rm -f $(MYFILE).ps $(MYFILE).pdf *.log *.dvi *.aux
