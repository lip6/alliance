#ifndef __PDETPLACEMENT_H
#define __PDETPLACEMENT_H

#include <vector>
#include <deque>
#include "PDetToPlaceIns.h"
using namespace std;

class PDetPlacement {
  public:
      typedef vector<PDetToPlaceIns*> Problem;
  private:
    unsigned		BBoxFlag;
    // le _costFlag sert lors de la consultation
    // du cout des nets.
    // Chaque net doit etre compte une seule fois
    unsigned		_costFlag;
    unsigned		_saveFlag;

    Problem		_problem;
    
      
    double		LeftEdge;
    double		RightEdge;
    double		OldCost;
    double		Cost;
    double		BestCost;
    deque<unsigned>	Queue; // ordering 
    vector<unsigned>	Stack;
    vector<unsigned>    UVect; // use for loop
    vector<double>	BestSolution;
    int			Idx;

  public:
    PDetPlacement(Problem& problem);
    bool		Optimize();
  private:
    void		AddIns();
    void		RemoveIns();
    double		InitCost(const unsigned BBoxFlag);
    double		CurrentCost();
    void		UnPlaceAll();
    void		PlaceAll();
};

#endif /* __PDETPLACEMENT_H */
