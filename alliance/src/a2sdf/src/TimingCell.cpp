#include "TimingCell.h"

TimingCell::~TimingCell()
{
    for (IOPaths::iterator it = _ioPaths.begin();
	    it != _ioPaths.end();
	    it++)
	delete it->second;
}

IOPath*
TimingCell::AddIOPath(const char* input, const char* output)
{
    IOPathKey key(input, output);
    IOPath* ioPath = _ioPaths[key];
    if (ioPath)
	return ioPath;
    ioPath = new IOPath(input, output);
    _ioPaths[key] = ioPath;
    return ioPath;
}

ofstream&
TimingCell::Dump(ofstream& out)
{
    if (_ioPaths.empty())
	return out;
    out << "  (CELL" << endl
	<< "    (CELLTYPE \"" << _cellName << "\")" << endl
	<< "    (DELAY" << endl
	<< "      (ABSOLUTE" << endl;
    for (IOPaths::iterator it = _ioPaths.begin();
	    it != _ioPaths.end();
	    it++)
    {
	it->second->Dump(out);
    }
    out << "      )" << endl
	<< "    )" << endl 
	<< "  )" << endl;
    return out;
}
