/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
#include "PCommon.h"
#include <iostream>
#include <math.h>
#include <string>
#include "PConstants.h"
using namespace std;

bool IsSpecialNet(const losig* const sig)
{
    return ( (isvdd((char*)sig->NAMECHAIN->DATA))
	  || (isvss((char*)sig->NAMECHAIN->DATA)) );
}

const double SquareShape(const double margin,
	const double sumwidth, const double minwidth, int& nrows)
{
    if (!nrows)
	nrows = (int)(sqrt((1.0 + margin) * sumwidth / ROWHEIGHT) + 0.5);
    double RowWidth = (double)((int)(((1.0 + margin) * sumwidth / (double)nrows) + 0.5));
    if (RowWidth < minwidth)
	return minwidth;
    return RowWidth;
}

unsigned NbCons(const losig* const sig)
{
    //on compte le nombre de connecteurs sur un signal
    //lofigchain doit avoir ete execute avant l'appel
    unsigned nbcons = 0;
    for (chain_list* it = (chain_list *)(getptype(sig->USER, (long)LOFIGCHAIN)->DATA);
	    it;
	    it = it->NEXT)
	++nbcons;
    return nbcons;
}

ostream& PrintLocon(ostream& os, const locon* const con)
{
    if (con->TYPE == INTERNAL)
	os << (static_cast<loins*>(con->ROOT))->INSNAME << " : " << con->NAME;
    else
	os << (static_cast<lofig*>(con->ROOT))->NAME << " : " << con->NAME;
    os << endl;
    return os;
}

ostream& PrintLosig(ostream& os, const losig* const sig)
{
    if (!sig->NAMECHAIN)
	return os;
    for (struct chain* ch = sig->NAMECHAIN;
	    ch;
	    ch = ch->NEXT)
    {
	os << (char*)(ch->DATA) << " " ;
    }
    os << endl;
    return os;
}

bool IsTie(const phins* ins)
{
    phfig* fig = getphfig(ins->FIGNAME, 'P');
    string figname(fig->NAME);
    
    if (figname.find("tie_x0") != string::npos)
	return true;
    if (figname.find("rowend_x0") != string::npos)
	return true;
    return false;
}
