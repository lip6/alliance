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
#include <algorithm>
using namespace std;

#include "PSubRow.h"
#include "PBin.h"
#include "PDetPlacement.h"

#include "PDetSubRow.h"

PDetSubRow::~PDetSubRow()
{
    for (PDetInsVector::iterator iit = _detInsVector.begin();
	    iit != _detInsVector.end(); iit++)
	delete *iit;
}

PDetSubRow::PDetSubRow(PSubRow& subrow):
	PContainer(subrow.GetBBox()),
	_orientation(subrow.GetOrientation())
{
    unsigned nbins = 0;
    // on compte le nombre d'instances dans la sousrow
    for (PSubRow::PBins::iterator bit = subrow.GetBins().begin();
	    bit != subrow.GetBins().end();
	    bit++)
    {
	nbins += (*bit)->GetToPlaceInss().size();
    }
    _detInsVector.reserve(nbins);

    // maintenant on cree les instances
    for (PSubRow::PBins::iterator bit = subrow.GetBins().begin();
	    bit != subrow.GetBins().end();
	    bit++)
    {
	for (PBin::PToPlaceInss::iterator iit = (*bit)->GetToPlaceInss().begin();
		iit != (*bit)->GetToPlaceInss().end();
		iit++)
	{
	    _detInsVector.push_back(new PDetToPlaceIns(*iit));
	}
    }
}

void
PDetSubRow::ExpandInstances(const bool eqmargin)
{
    unsigned nins = _detInsVector.size();
    double totalinssize = 0;
    for (PDetInsVector::iterator iit = _detInsVector.begin();
	    iit != _detInsVector.end();
	    iit++)
    {
	totalinssize += (*iit)->GetWidth();
    }
    //xtof: 06 05 2002
    //changing the white space repartition
    //We want the maximum 2-pitch spaces for bodie-tie insertion
    
    double whitespace = GetWidth() - totalinssize;
    double inswhitespace = 0.0;
    double whitespaceremain = 0.0;
    if (eqmargin)
    {
	inswhitespace = (double)(int)(whitespace / nins);
	whitespaceremain = (double)(int)(whitespace
		- nins * inswhitespace + 0.5);
    }
    else
    {
	inswhitespace = (double)(int)(whitespace / (2 * nins));
	inswhitespace *= 2;
	whitespaceremain = (double)(int)(whitespace
		- nins * inswhitespace + 0.5);
    }
    for (PDetInsVector::iterator iit = _detInsVector.begin();
	    iit != _detInsVector.end();
	    iit++)
    {
	(*iit)->SetMarginWidth((*iit)->GetWidth() + inswhitespace);
    }
    PDetInsVector::iterator iit = _detInsVector.begin();
    if (eqmargin)
    {
	while (whitespaceremain-- > 0.0)
	{
	    (*iit++)->AddWhiteSpace();
	}
    }
    else
    {
	while (whitespaceremain >= 2.0)
	{
	    (*iit++)->AddDoubleWhiteSpace();
	    whitespaceremain -= 2.0;
	}
	if (whitespaceremain > 0.0)
	{
	    (*iit)->AddWhiteSpace();
	}
    }
    
    double XPos = GetMinX();
    for (PDetInsVector::iterator iit = _detInsVector.begin();
	    iit != _detInsVector.end();
	    iit++)
    {
	(*iit)->SetLeftCornerX(XPos);
	XPos += (*iit)->GetMarginWidth();
    }
}

class CompareInsPosition
{
    vector<PDetToPlaceIns*>& _insvector;
    public:
    CompareInsPosition(vector<PDetToPlaceIns*>& insvector):
	_insvector(insvector) {}
	
    bool operator()(const PDetToPlaceIns* ins1, const PDetToPlaceIns* ins2) const
	{ return ins1->GetLeftCornerX() < ins2->GetLeftCornerX(); }
};

bool
PDetSubRow::FinalOptimize()
{
    unsigned ninsinproblem = 0;
    unsigned nins = _detInsVector.size();
    if (_detInsVector.size() == 0)
	return false;
    unsigned decal = 2;
    bool optimisation = false;
    
    vector<PDetToPlaceIns*> insvector;
    insvector.reserve(nins);
    for (PDetInsVector::iterator iit = _detInsVector.begin();
	    iit != _detInsVector.end(); iit++)
    {
	insvector.push_back(*iit);
    }
    sort(insvector.begin(), insvector.end(), CompareInsPosition(insvector));
    PDetPlacement::Problem problem;
    problem.reserve(6);
    vector<PDetToPlaceIns*>::iterator ifirst = insvector.begin();
    vector<PDetToPlaceIns*>::iterator ilast = insvector.end();
    while (1)
    {
	while (1)
	{
	    if (ifirst == ilast)
		break;
	    problem.push_back(*ifirst++);
	    ++ninsinproblem;
	    if (ninsinproblem >= 6)
	    {
		ninsinproblem = 0;
		break;
	    }
	}
	PDetPlacement detplace(problem);
	if (detplace.Optimize())
	    optimisation = true;
	
	if (ifirst == ilast)
	    break;

	sort(insvector.begin(), insvector.end(), CompareInsPosition(insvector));
	if ((6 + decal) <= nins)
	{
	    ifirst = insvector.begin() + decal;
	}
	else
	{
	    ifirst = insvector.begin() + decal - 1;
	}
	ilast = insvector.end();
	
	decal += 2;
	problem.clear();
    }
    return optimisation;
}

ofstream& 
PDetSubRow::Plot(ofstream& out) const
{
  out << GetMinX() + 0.2 << " " << GetMinY() + 0.2 << endl
      << GetMinX() + 0.2 << " " << GetMaxY() - 0.2 << endl
      << GetMaxX() - 0.2 << " " << GetMaxY() - 0.2 << endl
      << GetMaxX() - 0.2 << " " << GetMinY() + 0.2 << endl
      << GetMinX() + 0.2 << " " << GetMinY() + 0.2 << endl << endl;

  return out;
}

ostream&
PDetSubRow::Print(ostream& os) const
{
    return os << "PDetSubRow: " << GetMinX() << ',' << GetMinY() << " : " << GetMaxX() << ',' << GetMaxY();
}
