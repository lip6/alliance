#include "PElem.h"

#include "PNet.h"

PNet::PNet(const losig* sig):
	_sig(sig), _elems()
{}

// ======================================================================
// ostream& Print(ostream& os) const;
// ======================================================================
ostream&
PNet::Print(ostream& os) const
{
    if (!_sig->NAMECHAIN)
	return os << "(noname) Index: " << _sig->INDEX;
    
    struct chain* cit = _sig->NAMECHAIN;
    if (!cit->NEXT)
	return os << (char*)cit->DATA;
    
    for (struct chain* cit = _sig->NAMECHAIN; cit; cit = cit->NEXT)
	os << (char*)cit->DATA << " ";
    return os;
}

void
PNet::DescribeOn(ostream& os) const
{
    os << this << endl;
    for (PElems::const_iterator eit = _elems.begin();
	    eit != _elems.end(); eit++)
    {
	os << *eit << endl;
    }
}

ofstream& 
PNet::Plot(ofstream& out) const
{
  int nbElems = 0;
  double baryX = 0;
  double baryY = 0;
  
  for (vector<PElem*>::const_iterator ElemIt = _elems.begin();
      ElemIt != _elems.end(); ElemIt++)
  {
    ++nbElems;
    baryX += (*ElemIt)->GetPosX();
    baryY += (*ElemIt)->GetPosY();
  }
  baryX = baryX / nbElems;
  baryY = baryY / nbElems;

  for (vector<PElem*>::const_iterator ElemIt = _elems.begin();
      ElemIt != _elems.end(); ElemIt++)
  {
    out << baryX << " " << baryY << endl
    << (*ElemIt)->GetPosX() << " " << (*ElemIt)->GetPosY() << endl << endl;
  }  
  
  return out;
}
