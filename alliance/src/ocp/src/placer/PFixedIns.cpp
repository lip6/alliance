#include "PFixedIns.h"
#include "PConstants.h"

PFixedIns::PFixedIns(const loins *instance, const phins* pins, int dx, int dy):
		PIns(instance),
		_pins(pins),
		_pos(), _height(0.0)
{
    phfig* phmodel = getphfig(pins->FIGNAME, '0');
    double pinsheight = (double)(phmodel->YAB2 - phmodel->YAB1);
    _height = (double)(int)(pinsheight / PITCH + 0.5);
    double x = (double)((_pins->XINS - dx) / PITCH) + _width / 2.0;
    double y = (double)((_pins->YINS - dy) / PITCH) + _height / 2.0;
    _pos = PPos(x, y);
}

ostream&
PFixedIns::Print(ostream& os) const
{
    return os << "PFixedIns: " <<
	_instance->FIGNAME << ':' << _instance->INSNAME << ':' << GetPos();
}
