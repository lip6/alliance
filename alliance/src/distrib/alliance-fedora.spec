%define        prefix    %{_libdir}/%{name}
%define        snapshot  20090901
%define        _default_patch_fuzz  2

Name:          alliance
Version:       5.0
Release:       30.%{snapshot}snap%{?dist}
Summary:       VLSI EDA System

License:       GPLv2
Group:         Applications/Engineering

Source:        http://www-asim.lip6.fr/pub/alliance/distribution/5.0/%{name}-%{version}-%{snapshot}.tar.gz
URL:           http://www-asim.lip6.fr/recherche/alliance/

Patch0:        alliance-env.patch
Patch1:        alliance-run.patch
Patch2:        alliance-perms.patch

# Improving autogeneration of documentation
Patch3:        alliance-tutorials-place_n_route.patch
Patch4:        alliance-tutorials-simulation.patch
Patch5:        alliance-tutorials-start.patch
Patch6:        alliance-tutorials-synthesis.patch
Patch7:        alliance-tutorials-amd2901.patch

# Improving examples
Patch8:        alliance-examples.patch
Patch9:        alliance-examples-adm2901.patch
Patch10:       alliance-examples-mipsR3000.patch

# upstream enhancements
Patch11:       alliance-xgra.patch

# Fixes warning: format '%%d' expects type 'int', but argument N has type 'long int'
Patch12:       alliance-xgra-xgrerror-ld.patch

Source1:       xsch.desktop
Source2:       dreal.desktop
Source3:       xpat.desktop
Source4:       xfsm.desktop
Source5:       xvpn.desktop
Source6:       graal.desktop
Source7:       xgra.desktop

Source8:       alliance.fedora

# Chitlesh's donated pictures to alliance
Source9:       graal.png
Source10:      dreal.png
Source11:      xvpn.png
Source12:      xfsm.png
Source13:      xpat.png
Source14:      xsch.png
Source15:      xgra.png

Source16:      alliance-tutorials-go-all.sh
Source17:      alliance-tutorials-go-all-clean.sh
Source18:      alliance-examples-go-all.sh
Source19:      alliance-examples-go-all-clean.sh

BuildRoot:     %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires: libXt-devel byacc desktop-file-utils bison
BuildRequires: libXp-devel libXpm-devel libstdc++-devel flex m4
BuildRequires: transfig ghostscript

%if 0%{?rhel}
BuildRequires: openmotif-devel
BuildRequires: pkgconfig
%else
BuildRequires: lesstif-devel
%endif

Requires:      xorg-x11-fonts-misc
# 442379
Requires(post): %{name}-libs = %{version}-%{release}

%description
Alliance is a complete set of free CAD tools and portable libraries for
VLSI design. It includes a VHDL compiler and simulator, logic synthesis
tools, and automatic place and route tools.

A complete set of portable CMOS libraries is provided, including a RAM
generator, a ROM generator and a data-path compiler.

Alliance is the result of more than ten years effort spent at ASIM department
of LIP6 laboratory of the Pierre et Marie Curie University (Paris VI, France).

Alliance has been used for research projects such as the 875 000 transistors
StaCS superscalar microprocessor and 400 000 transistors IEEE Gigabit HSL
Router.

You are kindly requested to mention
" Designed with alliance (c) LIP6, Université Pierre et Marie Curie"
so as to spread the word about "alliance CAD system" and its development team.

Alliance provides CAD tools covering most of all the digital design flow:

 * VHDL Compilation and Simulation
 * Model checking and formal proof
 * RTL and Logic synthesis
 * Data-Path compilation
 * Macro-cells generation
 * Place and route
 * Layout edition
 * Netlist extraction and verification
 * Design rules checking

%{name} is listed among Fedora Electronic Lab (FEL) packages.


%package libs
Summary:      Alliance VLSI CAD Sytem - multilibs
Group:        Applications/Engineering
Requires:     %{name} = %{version}-%{release}
Requires:     electronics-menu


%description libs
Architecture dependent files for the Alliance VLSI CAD Sytem.


%package doc
Summary:      Alliance VLSI CAD Sytem - Documentations
Group:        Applications/Engineering
Requires:     %{name} = %{version}-%{release}
Requires:     gnuplot 
BuildRequires:tetex-latex



%description doc
Documentation and tutorials for the Alliance VLSI CAD Sytem.


%prep
%setup -q
%{__rm} -rf autom4te.cache

%{__cp} -p %{SOURCE1} %{SOURCE2} %{SOURCE3} %{SOURCE4} %{SOURCE5} %{SOURCE6} %{SOURCE7} %{SOURCE8} .
sed -i "s|ALLIANCE_TOP|%{prefix}|" *.desktop

# removed useless copyrighted (by Cadence) lines from the examples
# and even in alliance-run
# https://www-asim.lip6.fr/wws/arc/alliance-users/2007-07/msg00006.html

%patch11 -p1 -b .xgra-makefile.am
%patch12 -p0 -b .xgra-long


%patch0 -p0 -b .env
%patch1 -p0 -b .run
%patch2 -p0 -b .perms


# ------------------------------------------------------------------------------
# Description : 2008 March : TexLive introduction to Rawhide
sed -i "s|tutorials||" documentation/Makefile.in
sed -i "s|documentation/tutorials/Makefile||" configure*
%patch3 -p1 -b .doc
%patch4 -p1 -b .doc
%patch5 -p1 -b .doc
%patch6 -p1 -b .doc
%patch7 -p0 -b .doc
pushd documentation/tutorials
    # clean unneccessary files
    %{__rm} Makefile*
    %{__rm} *.pdf
    # build documentation
    for folder in place_and_route/tex start simulation/tex synthesis/tex; do
        pushd $folder
            %{__make}
        popd
        # remove useless directories before %%doc
        %{__rm} -rf $folder
    done
    # Add automated scripts to tutorials
    %{__install} -pm 755 %{SOURCE16} go-all.sh
    %{__install} -pm 755 %{SOURCE17} go-all-clean.sh
    # Fedora Electronic Lab self test for alliance
    #./go-all.sh 2>&1 | tee self-test-tutorials.log
    # clean temporary files
    ./go-all-clean.sh
popd
# ------------------------------------------------------------------------------

%patch8  -p0 -b .examples
%patch9  -p1 -b .examples
%patch10 -p1 -b .examples

# fixing flex and bison update on rawhide
sed -i '30i\#include \"string.h\"' ocp/src/placer/Ocp.cpp ocp/src/placer/PPlacement.h
sed -i '18i\#include \"bvl_bcomp_y.h\"' bvl/src/bvl_bcomp_y.y

# make sure the man pages are UTF-8...
for nonUTF8 in FAQ README LICENCE distrib/doc/alc_origin.1 alcban/man1/alcbanner.1 \
               loon/doc/loon.1 m2e/doc/man1/m2e.1 boog/doc/boog.1 ; do
  %{_bindir}/iconv -f ISO-8859-1 -t utf-8 $nonUTF8 > $nonUTF8.conv
  %{__mv} -f $nonUTF8.conv $nonUTF8
done

pushd documentation/alliance-examples/
# make sure the man pages are UTF-8...
for nonUTF8 in tuner/build_tuner mipsR3000/asm/mips_defs.h tuner/tuner.vbe \
               tuner/README mipsR3000/sce/mips_dpt.c ; do
  %{_bindir}/iconv -f ISO-8859-1 -t utf-8 $nonUTF8 > $nonUTF8.conv
  %{__mv} -f $nonUTF8.conv $nonUTF8
done

#wrong-file-end-of-line-encoding
sed -i 's/\r//' mipsR3000/asm/*
popd

# Fix xgra build 
sed -i "s|AM_CFLAGS =|AM_CFLAGS = -I../../mbk/src -I../../aut/src/|g" xgra/src/Makefile*
sed -i "s|xgra_LDADD =|xgra_LDADD = -L../../mbk/src -L../../aut/src/|g" xgra/src/Makefile*

%build

export ALLIANCE_TOP=%{prefix}

%configure --prefix=%{prefix}             \
           --enable-alc-shared            \
           --disable-static               \
           --includedir=%{prefix}/include \
           --libdir=%{prefix}/lib         \
           --bindir=%{prefix}/bin         \
           --mandir=%{_datadir}/%{name}/man         #252941

# disabling rpath
sed -i 's|^hardcode_libdir_flag_spec="\\${wl}--rpath \\${wl}\\$libdir"|hardcode_libdir_flag_spec=""|g' libtool
sed -i 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' libtool

# clean unused-direct-shlib-dependencies
sed -i -e 's! -shared ! -Wl,--as-needed\0!g' libtool

# Is not parallel-build-safe
%{__make}


%install

%{__rm} -rf %{buildroot}

%{__make} INSTALL="install -p" DESTDIR=%{buildroot} install


# Set execution rights on the alc_env.* batchs and adjust ALLIANCE_TOP.
pushd %{buildroot}%{_sysconfdir}/profile.d
  chmod 0644 alc_env.*
  sed -i "s|@DATE@|`date`|" alc_env*
  sed "s|ALLIANCE_TOP *= *\([^;]*\)|ALLIANCE_TOP=%{prefix}|" alc_env.sh
  sed "s|setenv *ALLIANCE_TOP *\([^;]*\)|setenv ALLIANCE_TOP %{prefix}|" alc_env.csh
popd


# documentation
%{__cp} -pr %{buildroot}%{prefix}/doc/ .
%{__cp} -pr %{buildroot}%{prefix}/examples/alliance-examples/ .

%{__rm} -rf %{buildroot}%{prefix}/doc/
%{__rm} -rf %{buildroot}%{prefix}/examples/

# Add automated scripts to examples
%{__install} -pm 755 %{SOURCE18} alliance-examples/go-all.sh
%{__install} -pm 755 %{SOURCE19} alliance-examples/go-all-clean.sh

pushd alliance-examples/
    # FEL self test for alliance
    #./go-all.sh 2>&1 | tee self-test-examples.log
    # clean temporary files
    ./go-all-clean.sh
popd

find %{buildroot} -name '*.la' -exec rm -f {} ';'


# Adding icons for the menus
%{__mkdir} -p %{buildroot}%{_datadir}/icons/hicolor/48x48/apps/
%{__cp} -p %{SOURCE9} %{SOURCE10} %{SOURCE11} %{SOURCE12} %{SOURCE13} %{SOURCE14} %{SOURCE15} \
    %{buildroot}%{_datadir}/icons/hicolor/48x48/apps/


# desktop files with enhanced menu from electronics-menu now on Fedora
# thanks Peter Brett
for desktopfile in *.desktop; do
desktop-file-install --vendor "" \
  --dir %{buildroot}%{_datadir}/applications/ \
  $desktopfile
done


# Architecture independent files
%{__mv} %{buildroot}%{prefix}/cells %{buildroot}%{_datadir}/%{name}/
%{__mv} %{buildroot}%{prefix}/etc   %{buildroot}%{_datadir}/%{name}/


# protecting hardcoded links
ln -sf ../../..%{_datadir}/%{name}/cells %{buildroot}%{prefix}/cells
ln -sf ../../..%{_datadir}/%{name}/etc   %{buildroot}%{prefix}/etc
ln -sf ../../..%{_datadir}/%{name}/man   %{buildroot}%{prefix}/man


# manpage-not-gzipped
find %{prefix}/man -type f -not -name '*.gz' -print | xargs gzip -9f


%{__mkdir} -p %{buildroot}%{_sysconfdir}/ld.so.conf.d/
cat > %{buildroot}%{_sysconfdir}/ld.so.conf.d/%{name}.conf << EOF
# Alliance VLSI design system
%{prefix}/lib
EOF


# removing tools for compiling and installing Alliance tools
# These are for the packager (i.e me) and not for user
%{__rm} -f %{buildroot}%{_sysconfdir}/%{name}/attila.conf
%{__rm} -f %{buildroot}%{prefix}/etc/attila.conf
%{__rm} -f %{buildroot}%{prefix}/bin/attila
%{__rm} -f %{buildroot}%{_datadir}/man/man1/attila*
%{__rm} -f doc/html/alliance/*attila.html
%{__rm} -f doc/pdf/attila.pdf

# correcting minor documentation details
sed -i "s|/bin/zsh|/bin/sh|" doc/alliance-run/bench.zsh

%{_fixperms} %{buildroot}/*


%post
/sbin/ldconfig
source %{_sysconfdir}/profile.d/alc_env.sh
touch --no-create %{_datadir}/icons/hicolor || :
%{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :


%postun
/sbin/ldconfig
touch --no-create %{_datadir}/icons/hicolor || :
%{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :



%clean
%{__rm} -rf %{buildroot}

#These headers are useful for the _usage_ of the binaries
#without these headers some of the binaries will be broken by default


%files
%doc CHANGES LICENCE COPYING* FAQ alliance.fedora
%defattr(-,root,root,-)
%{prefix}/
%{_datadir}/%{name}/
%{_datadir}/icons/hicolor/48x48/apps/*


%files libs
%defattr(-,root,root,-)
%config(noreplace) %{_sysconfdir}/ld.so.conf.d/*
%{_datadir}/applications/*.desktop
%config(noreplace) %{_sysconfdir}/profile.d/alc_env.*


%files doc
%defattr(-,root,root,-)
%doc doc/html/
%doc doc/design-flow
%doc doc/pdf/*.pdf
%doc doc/overview/*.ps
%doc doc/overview/*.pdf
%doc documentation/tutorials/
#Makefiles are present in alliance-examples/*. It is normal because
# * it gives the VLSI designer a template on how to create his own
#   Makefile for alliance (VLSI designers normally don't know how to do so)
# * it is not part of the build, but part of the working environment of the user
%doc alliance-examples/
%doc doc/alliance-run/



%changelog
* Thu Aug 27 2009 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-30.20090827snap
- updated to upstream's 20090828 snapshot
- merged patches with upstream's snapshot: 64 bits stability patches and upstream enhancements
- fixed EPEL-5 build

* Sat Aug 8 2009 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-29.20070718snap
- improved rawhide build with respect to the broken patches

* Thu Jul 9 2009 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-28.20070718snap
- improved stability on 64 bit architecture
- Patches (14 to 100) added along with new features from upstream

* Sat Jul 4 2009 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-27.20070718snap
- improved autogeneration of documentation and fixed the examples

* Tue Feb 24 2009 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-26.20070718snap
- fixed build due to new releases of flex and bison

* Mon Feb 23 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 5.0-25.20070718snap
 - Rebuilt for https://fedoraproject.org/wiki/Fedora_11_Mass_Rebuild

* Sun Feb 01 2009 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-24.20070718snap
- Improved VHDL generic implementation

* Mon Nov 10 2008 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-23.20070718snap
- Added Requires xorg-x11-fonts-misc to fix launch crash

* Mon Nov 3 2008 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-22.20070718snap
- rebuild for F10

* Mon Sep 15 2008 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-21.20070718snap
- Bugfix : Alliance incorrectly mungs your path and adds the cwd to the path #459336
- Bugfix : Latest alc_env fixes broken system man path #452645

* Mon Aug 04 2008 Aanjhan Ranganathan <aanjhan [AT] tuxmaniac DOT com> - 5.0-20.20070718snap
- Bumped release version to match changelog

* Fri Aug 01 2008 Aanjhan Ranganathan <aanjhan [AT] tuxmaniac DOT com> - 5.0-16.20070718snap
- Rebuild using latest lesstif-devel. For #368441
- Temporarily set fuzz parameter of patch system to be 2

* Fri May 30 2008 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-16.20070718snap
- Bugfix /etc/profile.d/alc_env.csh problem #449062 #448480

* Mon May 26 2008 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-15.20070718snap
- Bugfix: error in postinstall scriptlet: /etc/profile.d/alc_env.sh not found #442379
- Bugfix: /etc/profile.d/alc_env.csh assumes MANPATH is preset #440083

* Tue May 20 2008 Thibault North < tnorth [AT] fedoraproject DOT org> - 5.0-14.20070718snap
- Add to Electronics Menu

* Fri Mar 21 2008 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-13.20070718snap
- Requiring new FEL menu structure
- Fixing previous desktop files
- closing unconfirmed bug #427691
- fixing multilibs issues #340621

* Tue Mar 18 2008 Thibault North < tnorth [AT] fedoraproject DOT org> - 5.0-13.20070718snap
- dropped patch4: alliance-tutorials.patch
- fixed TexLive related issues for documentation
-   commenting previous make in directory tutorials
-   attached tutorials made in F8 Werewolf
- dropped BR :tetex-latex
- added patch4: including more useful includes to C files
- fixed bug #434020 : alliance failed massrebuild attempt for GCC4.3

* Mon Feb 18 2008 Fedora Release Engineering <rel-eng@fedoraproject.org> - 5.0-12.20070718snap
- Autorebuild for GCC 4.3

* Sun Feb 10 2008 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-11.20070718snap
- mass rebuild for gcc43 on rawhide
- ensuring multilibs (#340621)

* Thu Aug 23 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-10.20070718snap
- complying to freedesktop policies - categories

* Sat Aug 18 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-9.20070718snap
- fixing desktop files
- fixing MANPATH and PATH for proper priority

* Thu Aug 16 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-8.20070718snap
- fixing conflicts with QuantLib-doc package (#252941)
- update license macro to comply with new guidelines)

* Sun Aug 12 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-7.20070718snap
- moved Architecture independent files to %%{_datadir}/%%{name}
- Uses verbs in the comments line for desktop files

* Thu Aug 02 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-6.20070718snap
- chose libdir/alliance as prefix
- added new icons to the menus
- removed useless attila
- fixed MANPATH override

* Sat Jul 28 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-5.20070718snap
- fixed unused-direct-shlib-dependency and undefined-non-weak-symbol warnings

* Tue Jul 24 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-4.20070718snap
- removed X-Desktop-File-Install-Version=0.10 from desktop files
- moved the alc_env to /etc/profile.d
- updated to meet reviewer's statements - #248649

* Thu Jul 19 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-3.20070718snap
- New upstream release which includes bug fixes

* Wed Jul 18 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-2.20060509snap
- minor fixes to the tutorials + added transfig ghostscript tetex-latex as BR
- corrected some hardcoded links in the scripts
- removed %%preun and updated alliance.fedora
- spec file updated to satisfy the review

* Tue Jul 17 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-1
- prepared spec file for review and spec clean ups
- removed rm and ln from %%post and %%preun
- removed log.3.gz from mandir since it's a duplicate and conflicts with man-pages

* Sat Jul 14 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-0.7
- since it is not parallel-build-safe, smp mflags are removed : (by wwoods)
- added missing BR : flex : (by wwoods)
- added missing BR : bison: (by rdieter)

* Fri Jul 13 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-0.6
- removing useless copyrighted lines from .ioc files
- added alliance.fedora among the %%doc

* Thu Jul 12 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-0.5
- removing copyrighted materials
- patching the remaining examples so that they will still be valid under another folder

* Wed Jul 04 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-0.4
- removing unwanted debug duplicates

* Wed May 02 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-0.3
- added desktop files

* Wed Feb 14 2007 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-0.2
- fixing documentations

* Wed Dec 13 2006 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-0.1
- Initial package

* Thu Feb 17 2005 Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- Synch with current version: bug & compliance with gcc 3.4.x.

* Fri Jul 16 2004 Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- Added Tutorial sub-package (now managed by autoconf/automake).
- Removed release tag, must be given at compile time using the
  --define command line argument of rpmbuild (see mkdistrib).

* Sat Nov 15 2003 Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- assert is now in assert.h, patch mut.h to include it if
  GCC_VERSION >= 3003 (gcc >= 3.3.x).

* Sat Oct 18 2003 Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- Synched with 2003/10/18 version.
- Missing depcomp : added "--add-missing --copy" to the individual
  packages in autostuff, so the first who needs depcomp will add
  it at top level.

* Sun Oct 13 2002 Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- autoconf m4 macros moved back in the Alliance source tree to avoid
  re-declaration on our development computers (on which the macros
  are in teh source tree).
- Adopt the versioning scheme from czo.
- Try to switch to dynamic libraries.

* Wed Jul 17 2002  Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- Moved autoconf m4 macros to /usr/share/aclocal.
- Synched with the current CVS version of Alliance.

* Fri May 31 2002  Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- GenPat added.
- GenLib docs added.
- seplace/seroute/sea bug fixes.

* Thu May 16 2002  Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- Corrected buggy substitution of ALLIANCE_TOP in alc_env.csh.
- Remove the alc_env.* scripts in "/etc/profile.d" only if this
  is the last package to be removed.

* Mon May  6 2002  Jean-Paul.Chaput <Jean-Paul.Chaput@lip6.fr>
- Initial packaging for release 5.0 (alpha stage).
