#ifndef __PINS_H
#define __PINS_H

#include "PElem.h"
#include "PBBox.h"
extern "C" {
#include "mut.h"
#include "mlo.h"
}

class PIns: public PElem {
    
  protected:  
    const loins*	_instance;
    double		_width;

  public:
    PIns(const loins* ins); 

    const loins*	GetInstance() const			{ return _instance; }
    double		GetWidth() const			{ return _width; }
    
    virtual double	GetHeight() const			{ return 0; }
    virtual ~PIns() {}
    
    PBBox		GetBBox() const;
    virtual ostream&	Print(ostream& os) const = 0;
    
    ofstream&	Plot(ofstream& out) const;
};

#endif /* __PINS_H */
