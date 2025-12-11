Format:            1.0
Source:            alliance
Binary:            alliance, alliance-doc
Architecture:      any
Version:           5.0
Maintainer:        Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
Homepage:          https://coriolis.lip6.fr/
Standards-Version: 5.0
Build-Depends:     debhelper-compat (= 13),
                   tcsh,
		   build-essential,
		   pkg-config,
                   bison,
                   flex,
                   gawk,
                   libedit-dev,
		   libstdc++6,
		   libboost-all-dev,
		   libbz2-dev,
		   libxml2-dev,
		   libxpm-dev,
		   libmotif-common,
		   libmotif-dev,
		   libxm4,
		   texlive-latex-recommended,
		   texlive-latex-extra,
	           texlive-fonts-recommended,
	           texlive-plain-generic,
	           texlive-pictures,
	           imagemagick,
		   graphviz,
	           fig2dev,
		   xpdf,
                   desktop-file-utils,
Package-List:
 alliance       deb Science/Electronics optional arch=any
 alliance-doc   deb Science/Electronics optional arch=any
DEBTRANSFORM-RELEASE:   1
DEBTRANSFORM-TAR:       alliance-5.0.tar.gz
