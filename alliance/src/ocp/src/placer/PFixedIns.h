#ifndef __PFIXEDINS_H
#define __PFIXEDINS_H

#include "PIns.h"
#include "PPos.h"
extern "C"{
#include "mut.h"
#include "mph.h"
}

class PFixedIns : public PIns {
    
  private:
    const phins*	_pins;
    PPos		_pos;
    double		_height;
  
  public:
    PFixedIns(const loins* ins, const phins* pins, int dx, int dy);
    ~PFixedIns() {}

    double		GetHeight() const			{ return _height; }
    PPos		GetPos() const				{ return _pos; }
    
    ostream&	Print(ostream& os) const;
};

#endif /* __PFIXEDINS_H */
