TEX	= place_and_route.tex
FIG	= buff_x1.fig controleplace.fig dpt-all-1.fig gabarit2_sx.fig gabarit3_sx.fig gabarit_sx.fig hierarchie.fig inv_x1.fig placement.fig preplacement.fig colonnes.fig hier.fig bloc.fig stick.fig 
EPS	= $(FIG:.fig=.eps)
PDF	= $(EPS:.eps=.pdf)

		
%.pdf : %.tex
	pdflatex $<

%.dvi : %.tex
	latex $<
	latex $<

%.ps : %.dvi
	dvips -o $@ $<

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

