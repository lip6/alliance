#include "IOPath.h"

IOPath::~IOPath()
{
    if (_risingPath)
	delete _risingPath;
    if (_fallingPath)
	delete _fallingPath;
}

void
IOPath::CreateRisingPath(const int value)
{
    if (!_risingPath)
	_risingPath = new TimePath(value);
    else
	_risingPath->PutValue(value);
}

void
IOPath::CreateFallingPath(const int value)
{
    if (!_fallingPath)
	_fallingPath = new TimePath(value);
    else
	_fallingPath->PutValue(value);
}

ofstream&
IOPath::Dump(ofstream& out) const
{
    out << "        (IOPATH "
	<< _input
	<< " "
	<< _output
	<< " ";
    if (_risingPath)
    {
	out << "(" << _risingPath->GetWorstValue()
	    << "::"
	    << _risingPath->GetBestValue()
	    << ") ";
    }
    else
	out << "() ";
    if (_fallingPath)
    {
	out << "(" << _fallingPath->GetWorstValue()
	    << "::"
	    << _fallingPath->GetBestValue()
	    << ") ";
    }
    else
	out << "() ";

    out << ")" << endl;
    return out;
}
