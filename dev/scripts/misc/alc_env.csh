# -*- Mode: Shell-script -*-
#             ,,,
#            (o o)
####=====oOO--(_)--OOO=========================================####
#
#  Alliance CAD system environnement
#  Usage (in C-shell) : > source alc_env.csh
#  (C) 1997 Czo  -- <sirol@asim.lip6.fr>
#  $Id: alc_env.csh,v 1.1 1999/09/09 14:24:53 czo Exp $
#  $Name:  $


# Which platform for Alliance CAD
switch (`uname`)
	case Linux:
		if ( `uname -r` =~ 1.* ) then
		    setenv MACHINE Linux_aout
		else
		    setenv MACHINE Linux_elf
		endif
	breaksw

	case SunOS:
		if ( `uname -r` =~ 5* ) then
		    setenv MACHINE Solaris
		else
		    setenv MACHINE SunOS
		endif
	breaksw

	default:
		setenv MACHINE Unknown
	breaksw
endsw

# Where the Alliance CAD is installed

setenv TOP /users/soft5/newlabo/$MACHINE

# User def 

setenv MBK_IN_LO         vst
setenv MBK_OUT_LO        vst
setenv MBK_IN_PH         ap
setenv MBK_OUT_PH        ap

setenv MBK_WORK_LIB      .
setenv MBK_CATAL_NAME    CATAL

setenv VH_MAXERR 10
setenv VH_BEHSFX vbe
setenv VH_PATSFX pat

setenv MBK_CATA_LIB      .:$TOP/cells/sclib
setenv MBK_TARGET_LIB    $TOP/cells/sclib
setenv MBK_C4_LIB        ./cellsC4 

setenv XPAT_PARAM_NAME   $TOP/etc/xpat.par
setenv XFSM_PARAM_NAME   $TOP/etc/xfsm.par

setenv DREAL_TECHNO_NAME $TOP/etc/cmos_7.dreal
setenv GRAAL_TECHNO_NAME $TOP/etc/cmos_7.graal

setenv RDS_TECHNO_NAME   $TOP/etc/cmos_7.rds

setenv RDS_IN cif
setenv RDS_OUT cif

setenv ELP_TECHNO_NAME   $TOP/etc/prol10.elp

# Update PATH and MANPATH
if $?PATH then
	setenv PATH $HOME/labo/$MACHINE/bin:$TOP/bin:$PATH
else
	setenv PATH $HOME/labo/$MACHINE/bin:$TOP/bin
endif


if $?MANPATH then
	setenv MANPATH $TOP/man:$MANPATH
else
	setenv MANPATH $TOP/man
endif


# Show results
if ($?prompt) then
	alias xmbk 'eval `\xmbk -c`'
	cat /users/soft5/newlabo/motd
        echo ""
        echo "Alliance settings :"
        echo ""
        echo "    MACHINE = $MACHINE"
        echo "    TOP     = $TOP"
        echo ""
endif


# EOF -- $Source: /dsk/l1/alliance/cvsroot/dev/scripts/misc/alc_env.csh,v $
