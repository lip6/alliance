#ifndef T_POS_H
#define T_POS_H

#include <iostream>
using namespace std;

class PPos {

  protected:

    double X;
    double Y;

  public:

    static const PPos Min;
    static const PPos Max;
    
    PPos() : X(0), Y(0) {}
    PPos(double x, double y) : X(x), Y(y) {}
    PPos(const PPos& pos) : X(pos.X), Y(pos.Y) {}

    PPos& operator=(const PPos& pos)
    {
	X = pos.X;
	Y = pos.Y;
	return *this;
    };
    bool operator==(const PPos& pos) const;
    bool operator!=(const PPos& pos) const;
    bool operator<(const PPos& pos) const;
    bool operator<=(const PPos& pos) const { return pos > *this; }
    bool operator>(const PPos& pos) const;
    bool operator>=(const PPos& pos) const { return pos < *this; }
  
    double GetX() const { return X; }
    double GetY() const { return Y; }

    void SetX(const double& x) { X = x; }
    void SetY(const double& y) { Y = y; }
    
    ostream& Print(ostream& os) const;
};

inline ostream& operator<<(ostream& os, const PPos& pos)
{
    return pos.Print(os);
}

inline ostream& operator<<(ostream& os, const PPos* pos)
{
    return pos ? pos->Print(os) : os << "(nil)";
}

#endif /* T_POS_H */
