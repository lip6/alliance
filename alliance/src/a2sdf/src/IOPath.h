#ifndef __IO_PATH_H
#define __IO_PATH_H

#include <fstream>
using namespace std;

class TimePath
{
    friend class IOPath;
    private:
	int	_worstValue;
	int	_bestValue;
	TimePath(const int value):
	    _worstValue(value), _bestValue(value)
	    {}
	void	PutValue(const int value)
	{
	    if (value < _worstValue)
		_worstValue = value;
	    else
		if (value > _bestValue)
		    _bestValue = value;
	}
	int	GetWorstValue() const { return _worstValue; }
	int	GetBestValue() const { return _bestValue; }
};

class IOPath
{
    private:
	const char*	_input;
	const char*	_output;
	TimePath*	_risingPath;
	TimePath*	_fallingPath;
    public:
	IOPath(const char* input, const char* output):
	    _input(input), _output(output),
	_risingPath(NULL), _fallingPath(NULL)
	    {}
	~IOPath();
	void		CreateRisingPath(const int value);
	void		CreateFallingPath(const int value); 
	ofstream&	Dump(ofstream& out) const;

};

#endif /* __IO_PATH_H */
