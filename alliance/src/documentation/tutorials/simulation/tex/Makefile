TEX	= simulation.tex
FIG	= addac.fig 
EPS	= $(FIG:.fig=.eps)

all : simulation.pdf
	cp simulation.pdf ../../

		
simulation.dvi : simulation.tex addac.eps
	latex $<

%.eps : %.fig
	fig2dev -L eps $< > $@

simulation.pdf : simulation.dvi
	dvipdf simulation.dvi

simulation.ps : simulation.dvi
	dvips simulation.dvi -o simulation.ps

$(EPS) : $(FIG)
$(TEX:.tex=.ps) : $(TEX:.tex=.dvi)

clean :
	rm -f *~ *.aux *.log *.pdf *.dvi *.ps *.out *.toc *.eps

