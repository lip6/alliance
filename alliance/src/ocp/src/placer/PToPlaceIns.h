#ifndef __PTOPLACEINS_H
#define __PTOPLACEINS_H

#include "PIns.h"
#include "PBin.h"


class PToPlaceIns : public PIns {
    
  private:  
    PBin*		_bin;
  
  public:
    PToPlaceIns(const loins* ins); 

    void		SetBin(PBin* NewBin)			{ _bin = NewBin; }
    PBin&               GetBin() const				{ return *_bin; }

    PPos		GetPos() const				{ return _bin->GetPos(); }
    double		GetHeight() const			{ return _bin->GetHeight(); }
    bool		GetOrientation() const			{ return _bin->GetOrientation(); }
    
    ostream&	Print(ostream& os) const;
};
#endif /* __PTOPLACEINS_H */
