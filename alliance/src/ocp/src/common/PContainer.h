#ifndef __PContainer_h
#define __PContainer_h

#include <iostream>
#include <fstream>
#include "PBBox.h"
using namespace std;

class PContainer
{

  protected:
    PBBox               _bBox;   // la bbox du container

  public:
    			PContainer():
			    _bBox() {};
                        PContainer(PBBox bbox):
			    _bBox(bbox) {};
			
			virtual ~PContainer() {};

    double		GetMinX() const                 { return _bBox.GetMinX(); }
    double		GetMinY() const                 { return _bBox.GetMinY(); }
    double              GetMaxX() const                 { return _bBox.GetMaxX(); }
    double              GetMaxY() const                 { return _bBox.GetMaxY(); }
    double              GetWidth() const                { return _bBox.GetWidth(); }
    double              GetHeight() const               { return _bBox.GetHeight(); }
    PBBox		GetBBox() const			{ return _bBox; };
    void		MergeBBox(const PBBox bbox)	{_bBox.Merge(bbox); };

    virtual ostream&	Print(ostream& os) const = 0;
};

static inline ostream& operator<<(ostream& os, const PContainer& container) {
    return container.Print(os);
}

static inline ostream& operator<<(ostream& os, const PContainer* container) {
    return container ? container->Print(os) : os << "(nil)";
}
#endif /* __PContainer_h */
