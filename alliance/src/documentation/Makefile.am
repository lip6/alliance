
SUBDIRS = alliance-examples tutorials


docdir = $(prefix)/doc

nobase_doc_DATA = overview/datapath.gif \
                  overview/genview.gif \
                  overview/graal.gif \
                  overview/Makefile \
                  overview/na3dp.ps \
                  overview/na3y.ps \
                  overview/nmx2_dp.ps \
                  overview/nmx2_y.ps \
                  overview/overview.blg \
                  overview/overview.tex \
                  overview/overview.pdf \
                  overview/stacs.gif \
                  overview/thesis.bib \
                  overview/tools.fig \
                  \
                  design-flow/beh-spec-alliance.gif \
                  design-flow/beh-spec-asimut.gif \
                  design-flow/beh-spec-fsm.gif \
                  design-flow/beh-spec-mocha.gif \
                  design-flow/beh-spec-rtl.gif \
                  design-flow/beh-spec-split.gif \
                  design-flow/beh-spec-struct.gif \
                  design-flow/beh-spec-syf.gif \
                  design-flow/beh-spec-vasy.gif \
                  design-flow/dreal.gif \
                  design-flow/flow.html \
                  design-flow/future.html \
                  design-flow/graal.gif \
                  design-flow/index.html \
                  design-flow/intro.html \
                  design-flow/logo.gif \
                  design-flow/mips-small.jpg \
                  design-flow/mips.html \
                  design-flow/mips.jpg \
                  design-flow/place-route-ocpr.gif \
                  design-flow/place-route-ring.gif \
                  design-flow/place-route-s2r.gif \
                  design-flow/place-route-valid.gif \
                  design-flow/rtl-synth-dp.gif \
                  design-flow/rtl-synth-fmi.gif \
                  design-flow/rtl-synth-genlib.gif \
                  design-flow/rtl-synth-logic.gif \
                  design-flow/tools.html \
                  design-flow/xfsm.gif \
                  design-flow/xpat.gif \
                  design-flow/xsch.gif \
                  \
                  alliance-run/Makefile \
                  alliance-run/README \
                  alliance-run/amd2901_chip.c \
                  alliance-run/amd2901_chip.rin \
                  alliance-run/amd2901_core.c \
                  alliance-run/amd2901_core.ioc \
                  alliance-run/amd2901_ctl.lax \
                  alliance-run/amd2901_ctl.vbe \
                  alliance-run/amd2901_dpt.c \
                  alliance-run/bench.zsh \
                  alliance-run/pattern.c


EXTRA_DIST = $(nobase_doc_DATA)

overview/overview.pdf: $(srcdir)/overview/overview.tex
	cd $(srcdir)/overview; make overview.pdf

tutorials/start/start.pdf: $(srcdir)/tutorials/start/start.tex
	cd $(srcdir)/tutorials/start; make start.pdf
