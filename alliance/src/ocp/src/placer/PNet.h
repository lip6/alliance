#ifndef __PNET_H
#define __PNET_H

#include <iostream>
#include <fstream>
#include <vector>
extern "C" {
#include "mut.h"
#include "mlo.h"
}
using namespace std;

class PElem;

class PNet {

  public:

    typedef vector <PElem*> PElems; 
  
  protected:

    const losig*	_sig;
    PElems		_elems;

  public:
    
    PNet(const losig* sig);

    const losig*	GetSig() const		{ return _sig; }
    PElems&		GetElems()		{ return _elems; }
    const PElems&	GetConstElems() const	{ return _elems; }
    
    ostream&	Print(ostream& os) const;
    void		DescribeOn(ostream& os) const;
    
    ofstream&	Plot(ofstream& out) const;
};

static inline ostream& operator<<(ostream& os, const PNet& net) {
    return net.Print(os);
}

static inline ostream& operator<<(ostream& os, const PNet* net) {
    return net ? net->Print(os) : os << "(nil)";
}

#endif /* __PNET_H */
