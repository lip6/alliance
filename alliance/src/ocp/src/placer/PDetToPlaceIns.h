#ifndef __PDETTOPLACEINS_H
#define __PDETTOPLACEINS_H

#include "PIns.h"

class PToPlaceIns;
class PDetSubRow;

class PDetToPlaceIns : public PIns {
    
  private:  
    PDetSubRow*		_subRow;
    PPos		_pos;
    double		_marginWidth;
    PPos		_leftCorner;
    bool		_placed;
  
  public:
    PDetToPlaceIns(PToPlaceIns* toplaceins);
    ~PDetToPlaceIns() {}

    PPos		GetPos() const				{ return _pos; }
    double		GetHeight() const;
    bool		GetOrientation() const;
    void		SetMarginWidth(const double mwidth)	{ _marginWidth = mwidth; }
    double		GetMarginWidth() const			{ return _marginWidth; }
    void		SetLeftCornerX(const double x);
    double		GetLeftCornerX() const			{ return _leftCorner.GetX(); }
    void		SetSubRow(PDetSubRow* subrow)		{ _subRow = subrow; }
    void		AddWhiteSpace()				{ ++_marginWidth; }
    void		UnPlace()				{ _placed = false; }
    void		Place()					{ _placed = true; }
    bool		IsPlaced() const			{ return _placed; }
    double		InitCost(const unsigned BBoxFlag, const unsigned CostFlag, const unsigned SaveFlag);
    double		DetPlaceUpdateCost();
    double		CurrentCost(const unsigned costflag);
    
    ostream&	Print(ostream& os) const;
    
    void	Save(struct phfig *physicalfig, const double dx, const double dy) const;

};
#endif /* __PDETTOPLACEINS_H */
