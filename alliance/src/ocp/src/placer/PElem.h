#ifndef __PELEM_H
#define __PELEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include "PPos.h"
using namespace std;

class PNet;

class PElem {
    
  public:
    typedef vector <PNet*> PNets; 
    
  protected:  
    PNets		_nets;
  
  public:
    PElem() : _nets() {} 

    PNets&		GetNets() 				{ return _nets; }
    const PNets&	GetConstNets() const			{ return _nets; }
    
    virtual PPos	GetPos() const = 0;
    virtual ~PElem() {}
    
    double		GetPosX() const				{ return GetPos().GetX(); }
    double		GetPosY() const				{ return GetPos().GetY(); }
    
    virtual ostream&	Print(ostream& os) const = 0;
    void		DescribeOn(ostream& os) const;
};

static inline ostream& operator<<(ostream& os, const PElem& elem) {
    return elem.Print(os);
}

static inline ostream& operator<<(ostream& os, const PElem* elem) {
    return elem ? elem->Print(os) : os << "(nil)";
}

#endif /* __PELEM_H */
