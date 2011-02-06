
// This file is part of the Alliance Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// The Alliance Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Alliance Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Alliance Project;  if  not,  write to the  Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 
#include <string.h>
#include "PBin.h"
#include "PPlacement.h"

#include "PDetPlacement.h"

PDetPlacement::PDetPlacement(Problem& problem):
	BBoxFlag(0),
	_costFlag(0),
	_saveFlag(0),
	_problem(problem)
{}

bool
PDetPlacement::Optimize()
{
    bool OptimizationResult = false;
    const int NInss = _problem.size();

    // Save Initial Solution
    BestSolution.reserve(NInss);

    Problem::iterator ifirst = _problem.begin();
    Problem::iterator ilast = _problem.end();
    vector<double>::iterator It = BestSolution.begin();
    while (ifirst != ilast)
	*It++ = (*ifirst++)->GetLeftCornerX();
    
    for(Idx = 0; Idx != NInss; )
	Queue.push_back(Idx++);

    // Init Best Cost
    BBoxFlag++;
    BestCost = InitCost(BBoxFlag);
#ifdef PLACE_DEBUG
    cout<<" Orig Cost: "<< BestCost << endl;
    cout<<" Actual Cost: " << CurrentCost() << endl;
#endif

    // Init Initial Cost
    BBoxFlag++;
    // Init Edges
    LeftEdge = (*_problem.begin())->GetLeftCornerX();
    RightEdge =(*_problem.rbegin())->GetLeftCornerX()
		+ (*_problem.rbegin())->GetMarginWidth();

    UnPlaceAll();
    Cost = InitCost(BBoxFlag);
    

#ifdef PLACE_DEBUG
    cout << " Init Cost: " << Cost << endl;
#endif
    
    UVect.reserve(NInss + 2);
    for (int id = 0; id < NInss + 2; id++)
	UVect.push_back(0);
    UVect[NInss] = NInss;
    UVect[NInss + 1] = NInss + 1;

    Stack.reserve(NInss);

    Idx = NInss - 1;

    unsigned NumAdds = 0;


    while(Idx < NInss)
    {
	AddIns();
	NumAdds++;

        if(UVect[Idx] == 0 || Cost >= BestCost)
      	{

	    UVect[Idx] = 0;	//force a bound

	    if(Cost < BestCost) //got here if:
				// new best complete soln (curWL < best)
				// bounded partial soln (curWL > best)
				//  so there is no need to additionally
				//  check to ensure this is a complete soln
	    {
		OptimizationResult = true;
		BestCost = Cost;
#ifdef PLACE_DEBUG
		cout<<" New Best: "<< Cost <<" found after "<< NumAdds<< endl;
		BBoxFlag++;
		cout << "Cost recalculated " << InitCost(BBoxFlag) << endl;
		cout << "actual Cost" << CurrentCost() << endl;
#endif	

		ifirst = _problem.begin();
		It = BestSolution.begin();
		while (ifirst != ilast)
		{
		    *It++ = (*ifirst++)->GetLeftCornerX();
		}
	    }

            while(UVect[Idx] == 0)
            {
	   	if(Idx < NInss)
		    RemoveIns();
    		UVect[++Idx]--; 
            }
      	}
      	Idx--;
    }
    ifirst = _problem.begin();
    It = BestSolution.begin();
    while (ifirst != ilast)
	(*ifirst++)->SetLeftCornerX(*It++);

    PlaceAll();
    BestSolution.clear();
#ifdef PLACE_DEBUG
    cout<<" Total Add Operations: "<< NumAdds<< endl;
    cout<<" Final solution has cost: "<< BestCost << endl << endl;
#endif
    UVect.clear();
    Queue.clear();
    Stack.clear();
    return OptimizationResult;
}

void
PDetPlacement::AddIns()
{
    BBoxFlag++;
    Stack.push_back(Queue.front());
    Queue.pop_front();
    UVect[Idx] = Idx;

    const unsigned cell = Stack.back();

    if(Idx%2)
    {
	_problem[cell]->SetLeftCornerX(RightEdge - _problem[cell]->GetMarginWidth());
	RightEdge -= _problem[cell]->GetMarginWidth();
    }
    else
    {
	_problem[cell]->SetLeftCornerX(LeftEdge);
	LeftEdge += _problem[cell]->GetMarginWidth();
    }
    _problem[cell]->Place();
    Cost += _problem[cell]->DetPlaceUpdateCost();
}

void
PDetPlacement::RemoveIns()
{
    unsigned cell = Stack.back();
    
    Queue.push_back(cell);
    Stack.pop_back();
    if(Idx%2)
	RightEdge += _problem[cell]->GetMarginWidth();
    else
	LeftEdge -= _problem[cell]->GetMarginWidth();
    _problem[cell]->UnPlace();
    Cost += _problem[cell]->DetPlaceUpdateCost();
}

double
PDetPlacement::InitCost(const unsigned BBoxFlag)
{
    ++_costFlag;
    ++_saveFlag;
    double Cost = 0.0;
    for (Problem::iterator pit = _problem.begin(); pit != _problem.end(); pit++)
	Cost += (*pit)->InitCost(BBoxFlag, _costFlag, _saveFlag);
    return Cost;
}

double
PDetPlacement::CurrentCost()
{
    ++_costFlag;
    double cost = 0.0;
    for (Problem::iterator pit = _problem.begin(); pit != _problem.end(); pit++)
	cost += (*pit)->CurrentCost(_costFlag);
    return cost;
}

void
PDetPlacement::UnPlaceAll()
{
    for (Problem::iterator pit = _problem.begin(); pit != _problem.end(); pit++)
	(*pit)->UnPlace();
}

void
PDetPlacement::PlaceAll()
{
    for (Problem::iterator pit = _problem.begin(); pit != _problem.end(); pit++)
	(*pit)->Place();
}
