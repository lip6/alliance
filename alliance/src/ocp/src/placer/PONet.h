#ifndef __PONET_H
#define __PONET_H

#include "PNet.h"
#include "PBBox.h"

class PONet: public PNet {

  private:
    unsigned		_bBoxMark;
    unsigned            _currCostMark;
    unsigned		_saveMark;
    double		Cost[2];
    PBBox		BBox[2];
    unsigned		_curr;

  public:
    
    PONet(const losig* sig);

    double&		CurrentCost() 		{ return Cost[_curr]; }
    double		CurrentCost(const unsigned Flag);
    double&		TempCost() 		{ return Cost[!_curr]; }
    PBBox&		CurrentBBox()		{ return BBox[_curr]; }
    PBBox&		TempBBox()              { return BBox[!_curr]; }

    void		SaveTemp()		{ _curr = !_curr; }
    void		SaveTemp(const unsigned saveflag);

    void                DetPlaceComputeBBox(const unsigned Flag);
    double		DetPlaceUpdateCost();

  private:
    double 		DetPlaceComputeCost(PBBox &Box) const;
};
#endif /* __PONET_H */
