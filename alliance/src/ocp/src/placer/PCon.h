#ifndef __PCON_H
#define __PCON_H

#include "PElem.h"
#include "mut.h"
#include "mlo.h"

class PCon: public PElem {
  private:
      const locon*	_con;
      PPos		_pos;
      char		_orient;

  public:
      PCon(const locon* con);
      PCon(const locon* con, PPos pos, char orient);
      PPos		GetPos() const			{ return _pos; }
      const locon*	GetLocon() const		{ return _con; }
      void		SetPos(const PPos pos)		{_pos = pos;}
      void		SetOrient(const char orient)	{_orient = orient;}
      void		Save(struct phfig *physicalfig, const double dx, const double dy) const;

      ostream&	Print(ostream& os) const;
};

#endif /* __PCON_H */
