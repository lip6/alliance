#include "mut.h"
#include "mph.h"

#include "PConstants.h"

#include "PToPlaceIns.h"

PToPlaceIns::PToPlaceIns(const loins *instance):
		PIns(instance),
		_bin(0)
{}

ostream&
PToPlaceIns::Print(ostream& os) const
{
    return os << "PToPlaceIns: " <<
	_instance->FIGNAME << ':' << _instance->INSNAME << ':' << GetPos();
}
