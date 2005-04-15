TEX	= place_and_route.tex
FIG	= buff_x1.fig controleplace.fig dpt-all-1.fig gabarit2_sx.fig gabarit3_sx.fig gabarit_sx.fig hierarchie.fig inv_x1.fig placement.fig preplacement.fig colonnes.fig hier.fig bloc.fig stick.fig 
EPS	= $(FIG:.fig=.eps)

all : place_and_route.pdf
	cp place_and_route.pdf ../../
		
place_and_route.pdf : place_and_route.dvi
	dvipdf place_and_route.dvi

place_and_route.ps : place_and_route.dvi
	dvipds place_and_route.dvi -o place_and_route.ps

place_and_route.dvi : place_and_route.tex $(EPS)
	latex $<
	latex $<

%.eps : %.fig
	/usr/pkg/bin/fig2dev -L eps $< > $@

$(EPS) : $(FIG)
$(PDF) : $(EPS)

clean :
	rm -f *~ *.aux *.log *.pdf *.dvi *.ps *.out *.toc *.eps 

