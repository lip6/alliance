TEX	= simulation.tex
FIG	= addac.fig 
EPS	= $(FIG:.fig=.eps)
PDF	= $(EPS:.eps=.pdf)

		
%.pdf : %.tex
	pdflatex $<

%.dvi : %.tex
	latex $<

%.ps : %.dvi
	dvips -o $@ $<

%.pdf : %.eps
	epstopdf $<

%.eps : %.fig
	fig2dev -L eps $< > $@

all : $(TEX:.tex=.ps) $(TEX:.tex=.pdf)

$(EPS) : $(FIG)
$(PDF) : $(EPS)
$(TEX:.tex=.ps) : $(TEX:.tex=.dvi)
$(TEX:.tex=.dvi) : $(EPS)
$(TEX:.tex=.pdf) : $(PDF)

clean :
	rm -f *~ *.aux *.log *.pdf *.dvi *.ps *.out *.toc *.eps

