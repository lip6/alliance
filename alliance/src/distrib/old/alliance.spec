# RPM SPEC file for Alliance
# (C) 2000, Czo [Olivier Sirol]
# $Id: alliance.spec,v 1.1 2002/04/29 14:15:28 czo Exp $
# rpm -ba /users/cao/czo/rpm/SPECS/alliance.spec
# avec $HOME/.rpmmacros :
# %_topdir        /users/cao/czo/rpm

%define  name     alliance
%define  version  MULTIVER
%define  release  MULTIDATE

Summary:        Free VLSI CAD System
Name:           %{name}
Version:        %{version}
Release:        %{release}
Source:         ftp://ftp-asim.lip6.fr/pub/alliance/distribution/
Url:            http://www-asim.lip6.fr/alliance/
Copyright:      LGPL/GPL
Vendor:         LIP6
Packager:       Czo <Olivier.Sirol@lip6.fr>
Group:          X11/Applications
Buildroot:      /tmp/root-%{name}-%{version}

%description

Alliance is a complete set of  free CAD tools and portable libraries for
VLSI design. It includes a  VHDL compiler and simulator, logic synthesis
tools, and automatic  place and route tools. A complete  set of portable
CMOS libraries is  provided, including a RAM generator,  a ROM generator
and a  data-path compiler. Alliance is  the result of a  ten year effort
spent at  ASIM departement  of LIP6  laboratory of  the Pierre  et Marie
Curie  University  (Paris  VI,  France).  Alliance  has  been  used  for
research  projects such  as the  875 000  transistors StaCS  superscalar
microprocessor and 400 000 transistors IEEE Gigabit HSL Router.

%files
/etc/profile.d/alc_env.sh
/etc/profile.d/alc_env.csh
/usr/local/alliance

%install
 rm -rf   $RPM_BUILD_ROOT/*
 mkdir -p $RPM_BUILD_ROOT/etc/profile.d
 mkdir -p $RPM_BUILD_ROOT/usr/local

# ( cd /usr/local ; tar cf - alliance ) | ( cd $RPM_BUILD_ROOT/usr/local ; tar xf -)
 cd $RPM_BUILD_ROOT/usr/local ; tar xzf  /users/largo2/ftp/pub/alliance/unstable/distribution/alliance-%{version}-%{release}-i386-linux-2.4.9-glibc2.tar.gz

 ln -s /usr/local/alliance/share/etc/alc_env.sh  $RPM_BUILD_ROOT/etc/profile.d
 ln -s /usr/local/alliance/share/etc/alc_env.csh $RPM_BUILD_ROOT/etc/profile.d

%clean
 rm -rf $RPM_BUILD_ROOT 

%post 
 ( cd /usr/local/alliance/share/etc ; ./configure )  > /dev/null 2>&1

%changelog
* Tue Jun  5 2001 Czo <Olivier.Sirol@lip6.fr>
- ./configure done on RedHat 7.1.
* Wed Feb  2 2000 Czo <Olivier.Sirol@lip6.fr>
- Packaged from the tgz distrib.

