#include "PIns.h"
#include "mut.h"
#include "mph.h"
#include "PConstants.h"

PIns::PIns(const loins *instance):
PElem(), _instance(instance)
{
    phfig* phmodel = getphfig(instance->FIGNAME, '0');
    double InsWidth = (double)(phmodel->XAB2 - phmodel->XAB1);
    _width = (double)(int)(InsWidth / PITCH + 0.5);
}

PBBox
PIns::GetBBox() const
{
    return PBBox(PPos(GetPosX() - GetWidth() / 2.0, GetPosY() - GetHeight() / 2.0),
                 PPos(GetPosX() + GetWidth() / 2.0, GetPosY() + GetHeight() / 2.0));
}

ofstream& 
PIns::Plot(ofstream& out) const
{
  out << GetBBox().GetMinX()+0.4 << " " << GetBBox().GetMinY()+0.4 << endl
      << GetBBox().GetMinX()+0.4 << " " << GetBBox().GetMaxY()-0.4 << endl
      << GetBBox().GetMaxX()-0.4 << " " << GetBBox().GetMaxY()-0.4 << endl
      << GetBBox().GetMaxX()-0.4 << " " << GetBBox().GetMinY()+0.4 << endl
      << GetBBox().GetMinX()+0.4 << " " << GetBBox().GetMinY()+0.4 << endl << endl;

  return out;
}
