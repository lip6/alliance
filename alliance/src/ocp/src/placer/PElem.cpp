#include "PElem.h"
#include "PNet.h"

void
PElem::DescribeOn(ostream& os) const
{
    os << this << endl;
    for (PNets::const_iterator nit = _nets.begin();
	    nit != _nets.end(); nit++)
    {
	os << *nit << endl;
    }
}
