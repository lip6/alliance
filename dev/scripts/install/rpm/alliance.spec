# RPM SPEC file for Alliance
# (C) 2000, Czo [Olivier Sirol]
# $Id: alliance.spec,v 1.1 2000/04/19 12:46:41 czo Exp $

%define  name     alliance
%define  version  4.0.6
%define  release  asim

Summary:        Free VLSI CAD System
Name:           %{name}
Version:        %{version}
Release:        %{release}
Source:         ftp://ftp-asim.lip6.fr/pub/alliance/distribution/
Url:            http://www-asim.lip6.fr/alliance/
Copyright:      GPL
Vendor:         LIP6
Packager:       Czo <Olivier.Sirol@lip6.fr>
Group:          X11/Applications
Buildroot:      /tmp/root-%{name}-%{version}

%description
Alliance is a complete set of  free CAD tools and portable libraries for
VLSI design. It includes a  VHDL compiler and simulator, logic synthesis
tools, and automatic place and  route tools. Advanced verification tools
for functional  abstraction and static  timing analysis are part  of the
system. A complete set of portable CMOS libraries is provided, including
a RAM generator,  a ROM generator and a data-path  compiler. Alliance is
the  result of  a ten  year  effort spent  at ASIM  departement of  LIP6
laboratory of the  Pierre et Marie Curie University  (Paris VI, France).
Alliance  has been  used  for  research projects  such  as  the 875  000
transistors  StaCS superscalar  microprocessor and  400 000  transistors
IEEE Gigabit HSL Router.

%files
/etc/profile.d/alc_env.sh
/etc/profile.d/alc_env.csh
/usr/local/alliance

%install
 rm -rf   $RPM_BUILD_ROOT/*
 mkdir -p $RPM_BUILD_ROOT/etc/profile.d
 mkdir -p $RPM_BUILD_ROOT/usr/local

 ( cd /usr/local ; tar cf - alliance ) | ( cd $RPM_BUILD_ROOT/usr/local ; tar xf -)

 ln -s /usr/local/alliance/share/etc/alc_env.sh  $RPM_BUILD_ROOT/etc/profile.d
 ln -s /usr/local/alliance/share/etc/alc_env.csh $RPM_BUILD_ROOT/etc/profile.d

%clean
 rm -rf $RPM_BUILD_ROOT

%changelog
* Wed Feb  2 2000 Czo <Olivier.Sirol@lip6.fr>
- Packaged from the tgz distrib, ./configure done on RedHat 6.0.
