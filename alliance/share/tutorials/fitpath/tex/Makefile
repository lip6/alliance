# Generic Makefile for TeTeX projet
# (C) 1999, Czo
# $Id: Makefile,v 1.1 2000/01/20 15:19:50 czo Exp $


EPSS =  sample_dpt-1.eps \
	sample_dpt-2.eps \
	sample_dpt-3.eps \
	sample_dpt-4.eps \
	sample_dpt-5.eps \
	sample_dpt-6.eps \
	usercol_dpt-2.eps \
	hierarchy_dpt-1.eps

MYFILE=fitpath

%.eps : %.fig
	fig2dev -L ps -c -p dummy $*.fig $*.eps


%.dvi : %.tex 
	latex $*.tex


%.ps : %.dvi 
	dvips -o $*.ps $*.dvi



view : ps
	gv $(MYFILE).ps
        
ps   : $(MYFILE).tex $(EPSS)
	latex $(MYFILE).tex
	dvips $(MYFILE).dvi -o $(MYFILE).ps

distrib : clean ps        
	ps2pdf $(MYFILE).ps
	cp -f $(MYFILE).ps ..
	cp -f $(MYFILE).pdf ..
	$(MAKE) clean

clean :
	rm -f $(MYFILE).ps $(MYFILE).pdf *.log *.lof *.dvi *.aux *.toc *.eps



all: fitpath.ps


