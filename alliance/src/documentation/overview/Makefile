# Generic Makefile for TeTeX projet
# (C) 1999, Czo
# $Id: Makefile,v 1.3 2004/07/15 11:19:39 ludo Exp $

EPS = datapath.eps genview.eps graal.eps stacs.eps tools.eps

%.eps : %.fig
	fig2dev -L ps -c -p dummy $*.fig $*.eps

%.eps : %.gif
	convert $*.gif $*.eps

distrib : overview.pdf
	cp overview.pdf ../

overview.pdf : overview.dvi
	dvipdf overview.dvi

overview.ps : overview.dvi
	dvips overview.dvi -o overview.ps

overview.dvi : overview.tex $(EPS)
	latex overview.tex
	latex overview.tex

clean :
	rm -f overview.ps overview.pdf *.log *.dvi *.aux *.bak datapath.eps genview.eps graal.eps stacs.eps tools.eps

