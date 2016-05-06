%define        prefix    %{_libdir}/%{name}
%define        snapshot  20120515
%define        _default_patch_fuzz  2

Name:          alliance
Version:       5.0
Release:       33.%{snapshot}%{?dist}
Summary:       VLSI EDA System

License:       GPLv2
Group:         Applications/Engineering

Source:        http://www-asim.lip6.fr/pub/alliance/distribution/5.0/%{name}-%{version}-%{snapshot}.tar.gz
URL:           http://www-asim.lip6.fr/recherche/alliance/

# Chitlesh's donated pictures to alliance
# included asfrom snapshot 20090901

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
# RHBZ 442379
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
Summary:      Alliance VLSI CAD System - multilibs
Group:        Applications/Engineering
Requires:     %{name} = %{version}-%{release}
%if ! 0%{?rhel}
Requires:     electronics-menu
%endif


%description libs
Architecture dependent files for the Alliance VLSI CAD System.


%package doc
Summary:      Alliance VLSI CAD System - Documentations
Group:        Applications/Engineering
Requires:     %{name} = %{version}-%{release}
Requires:     gnuplot 
BuildRequires:tetex-latex



%description doc
Documentation and tutorials for the Alliance VLSI CAD System.


%prep
%setup -q
%{__rm} -rf autom4te.cache

cp distrib/alliance.fedora .
sed -i "s|ALLIANCE_TOP|%{prefix}|" distrib/*.desktop

# removed useless copyrighted (by Cadence) lines from the examples
# and even in alliance-run
# https://www-asim.lip6.fr/wws/arc/alliance-users/2007-07/msg00006.html

# ------------------------------------------------------------------------------
# Description : 2008 March : TexLive introduction to Rawhide
sed -i "s|tutorials||" documentation/Makefile.am
sed -i "s|tutorials||" documentation/Makefile.in
sed -i "s|documentation/tutorials/Makefile||" configure*
pushd documentation/tutorials
    # clean unnecessary files
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
    %{__install} -pm 755 ../../distrib/alliance-tutorials-go-all.sh        go-all.sh
    %{__install} -pm 755 ../../distrib/alliance-tutorials-go-all-clean.sh  go-all-clean.sh
    # Fedora Electronic Lab self test for alliance
    #./go-all.sh 2>&1 | tee self-test-tutorials.log
    # clean temporary files
    ./go-all-clean.sh
popd
# ------------------------------------------------------------------------------

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

find documentation/tutorials/ \
    -name *.vbe  -o \
    -name *.pat  -o \
    -name *.vhdl -o \
    -name *.vst  -o \
    -name *.c \
    -exec chmod 0644 {} ';'
    
%build

export ALLIANCE_TOP=%{prefix}

# must run autotools before it does itself and fail
aclocal -I .
libtoolize --force --copy
automake --add-missing --foreign
autoconf

./configure --target=%{_target_platform}   \
            --prefix=%{prefix}             \
            --enable-alc-shared            \
            --disable-static               \
            --includedir=%{prefix}/include \
            --libdir=%{prefix}/lib         \
            --bindir=%{prefix}/bin         \
            --sysconfdir=/etc              \
            --mandir=%{_datadir}/%{name}/man         # RHBZ 252941

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
#%{__mkdir_p} %{buildroot}%{_sysconfdir}/profile.d
#mv %{buildroot}%{prefix}/etc/*.*sh %{buildroot}%{_sysconfdir}/profile.d
pushd %{buildroot}%{_sysconfdir}/profile.d
  chmod 0644 alc_env.*
  sed -i "s|@DATE@|`date`|" alc_env*
  sed -i "s|ALLIANCE_TOP *= *\([^;]*\)|ALLIANCE_TOP=%{prefix}|" alc_env.sh
  sed -i "s|setenv *ALLIANCE_TOP *\([^;]*\)|setenv ALLIANCE_TOP %{prefix}|" alc_env.csh
popd


# documentation
%{__cp} -pr %{buildroot}%{prefix}/doc/ .
%{__cp} -pr %{buildroot}%{prefix}/examples/alliance-examples/ .
%{__cp} -pr %{buildroot}%{prefix}/examples/alliance-run/ .
%{__cp} -p  %{buildroot}%{prefix}/examples/regression.sh .

%{__rm} -rf %{buildroot}%{prefix}/doc/
%{__rm} -rf %{buildroot}%{prefix}/examples/

# Add automated scripts to examples
%{__chmod} ug+x ./alliance-examples/mipsR3000/asm/go-*.sh \
                ./alliance-examples/go-all.sh             \
                ./alliance-examples/go-all-clean.sh       \
                ./regression.sh

pushd alliance-examples/
    # FEL self test for alliance
    #./go-all.sh 2>&1 | tee self-test-examples.log
    # clean temporary files
    ./go-all-clean.sh
popd

find %{buildroot} -name '*.la' -exec rm -f {} ';'


# Adding icons for the menus
%{__mkdir} -p %{buildroot}%{_datadir}/icons/hicolor/48x48/apps/
%{__cp} -p distrib/*.png %{buildroot}%{_datadir}/icons/hicolor/48x48/apps/


# desktop files with enhanced menu from electronics-menu now on Fedora
# thanks Peter Brett
for desktopfile in distrib/*.desktop; do
desktop-file-install --vendor "" \
  --dir %{buildroot}%{_datadir}/applications/ \
  $desktopfile
done


# Architecture independent files
%{__mkdir} -p %{buildroot}%{_sysconfdir}/%{name}
%{__mkdir} -p %{buildroot}%{_bindir}
%{__mv} %{buildroot}%{prefix}/bin/*           %{buildroot}%{_bindir}
%{__mv} %{buildroot}%{prefix}/cells           %{buildroot}%{_datadir}/%{name}/
%{__mv} %{buildroot}%{prefix}%{_sysconfdir}/* %{buildroot}%{_sysconfdir}/%{name}
rmdir   %{buildroot}%{prefix}%{_sysconfdir}
rmdir   %{buildroot}%{prefix}/bin


# protecting hardcoded links
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
%{__rm} -f %{buildroot}%{_sysconfdir}/attila.conf
%{__rm} -f %{buildroot}%{prefix}/bin/attila
%{__rm} -f %{buildroot}%{_datadir}/man/man1/attila*
%{__rm} -f doc/html/alliance/*attila.html
%{__rm} -f doc/pdf/attila.pdf

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
%defattr(-,root,root,-)
%doc CHANGES LICENCE COPYING* FAQ alliance.fedora
%{prefix}/
%{_bindir}/*
%{_datadir}/%{name}/
%{_datadir}/icons/hicolor/48x48/apps/*


%files libs
%defattr(-,root,root,-)
%config(noreplace) %{_sysconfdir}/ld.so.conf.d/*
%config(noreplace) %{_sysconfdir}/%{name}/*
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
%doc alliance-run/
%doc regression.sh



%changelog
* Mon May 14 2012 Thibault North <tnorth [AT] fedoraproject.org> - 5.0-33.20120515
- Updated GNU/GPL address.
- In asimut Makefile.am, put Bvl before Bhl. The symbol dependency problem
  shows only when using static linking.

* Tue Apr 17 2012 Jean-Paul Chaput <Jean-Paul DOT Chaput [AT] lip6 DOT fr> - 5.0-32.20120416
- This snapshot integrates the bug avoidance for druc (uses -O0 and *not* -O2).
- Merge patches from Naohiko Shimizu (<nshimizu [AT] ip-arch DOT jp>.
- Compatibility of ocp & nero with pharosc (variable gabarit).
- Small change of behavior for boog regarding registers, also for pharosc.
- Correct the Alliance examples so they can run once installeds.
- Merge in the scripts written by Chitlest Goorah (no more extra sources).

* Wed Sep 02 2009 Chitlesh Goorah <chitlesh [AT] fedoraproject DOT org> - 5.0-31.20090901snap
- updated to upstream's 20090901 snapshot
- Removed all patches which are accepted by upstream

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
  are in the source tree).
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
