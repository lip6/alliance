TEX	= synthesis.tex
FIG     = automate.fig ex_digicode.fig clavier.fig synthese.fig T_RC.fig graphe1.fig graphe_solution_digicode.fig hier.fig bloc.fig graphe1.fig datap.fig exemple1.fig exemple2.fig ctl-mrs-1.fig ctl-wen-1.fig dpt-all-1.fig dpt-alu-1.fig ctldecode.fig ctldecodebw.fig dptbanc.fig ctl-alu-1.fig
EPS     = $(FIG:.fig=.eps)

all : synthesis.pdf
	cp synthesis.pdf ../../

synthesis.pdf : synthesis.dvi
	dvipdf $< $@

synthesis.ps : synthesis.dvi
	dvips $< -o $@

synthesis.dvi : synthesis.tex $(EPS)
	latex $<
	latex $<

%.eps : %.fig
	fig2dev -L eps $< > $@

$(EPS) : $(FIG)
$(PDF) : $(EPS)

clean :
	rm -f *~ *.aux *.log *.pdf *.dvi *.ps *.out *.toc *.eps

