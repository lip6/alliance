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
