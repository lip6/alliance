#ifndef __TIMING_CELL_H
#define __TIMING_CELL_H

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "IOPath.h"
using namespace std;

struct CompIoPaths
{
    bool operator()(const pair<const char*, const char*> p1,
	    const pair<const char*, const char*> p2) const
    {
	return (((int)(p1.first) < (int)(p2.first))
		|| ((int)(p1.second) < (int)(p2.second)));
    }
};

class TimingCell
{
    public:
	typedef pair<const char*, const char*> IOPathKey;
	typedef map<IOPathKey, IOPath*, CompIoPaths> IOPaths;
    private:
	string		_cellName;
	IOPaths		_ioPaths;
    public:
	TimingCell(const char* name):
	    _cellName(name), _ioPaths()
	    {}
	~TimingCell();
	IOPath*		AddIOPath(const char* input, const char* output);
	ofstream& 	Dump(ofstream& out);
};

#endif /* __TIMING_CELL_H */
