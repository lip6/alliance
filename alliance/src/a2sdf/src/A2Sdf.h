#ifndef __A2SDF_H
#define __A2SDF_H

#include <list>
#include "TimingCell.h"
using namespace std;

class A2Sdf
{
    public:
	typedef list<TimingCell*> TimingCells;
    private:
	bool		_verbose;
	const char*	_fileName;
	TimingCells	_timingCells;
    public:
	A2Sdf(const char* filename, bool verbose)
	    : _verbose(verbose)
	    , _fileName(filename)
	    , _timingCells()
	    {}
	
	~A2Sdf();
	void	CreateTimingCell(const char* name);
	void	Dump() const;
};

#endif /* __A2SDF_H */
