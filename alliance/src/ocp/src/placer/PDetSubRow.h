#ifndef __PDETSUBROW_H
#define __PDETSUBROW_H

#include <iostream>
#include <fstream>
#include <vector>
#include "PContainer.h"
#include "PDetToPlaceIns.h"

class PSubRow;

class PDetSubRow : public PContainer {

  public:
   typedef vector<PDetToPlaceIns> PDetInsVector;
  
  private:

    PDetInsVector	_detInsVector;
    bool		_orientation;


  public:
    			PDetSubRow(PSubRow& subrow);
    bool		GetOrientation() const		{ return _orientation; }
    PDetInsVector&	GetInssVector() 		{ return _detInsVector; }
    const PDetInsVector& GetConstInssVector() const		{ return _detInsVector; }
    void		ExpandInstances();
    bool		FinalOptimize();
    ostream&	Print(ostream& os) const;
    ofstream&	Plot(ofstream& out) const;
};
#endif /* __PDETSUBROW_H */
